#!/usr/bin/env python

import os
import sys
import re
import functools

ASSERT_EQUAL     = 'if( .not. assertEqual({exp}, {act}, {exp_str}, {act_str}, res, {line_no}, "{file_name}") ) return'
ASSERT_NOT_EQUAL = 'if( .not. assertNotEqual({exp}, {act}, {exp_str}, {act_str}, res, {line_no}, "{file_name}") ) return'
ASSERT_CLOSE     = 'if( .not. assertClose({exp}, {act}, {tol}, {exp_str}, {act_str}, res, {line_no}, "{file_name}") ) return'

SUITE_RE = re.compile(r'\bSUITE\b')
TEST_RE  = re.compile(r'\bTEST\b')

class ParseException(Exception):
    pass

#=======================================================================================================================

class Suite(object):
    def __init__(self, name, *args, **kwargs):
        self.name      = strip_comments(name)
        if args:
            self.use_stmts = 'use ' + '\n  use '.join(strip_comments(arg.strip()) for arg in args)
        else:
            self.use_stmts = ''
        self.tests     = []
        self.tests_str = ''
        self.mod_name  = self.name.replace(' ', '')

    def __str__(self):
        return '''
module {self.mod_name}
  {self.use_stmts}
  implicit none

contains

{self.tests_str}

end module {self.mod_name}
'''.format(self=self)

    def set_tests(self, tests):
        self.tests     = tests
        self.tests_str = '\n'.join(str(test) for test in tests)

#=======================================================================================================================

class Test(object):

    COUNT = 0

    def __init__(self, name, body, **kwargs):
        Test.COUNT += 1
        self.test_name = strip_comments(name)
        self.test_body = body
        self.test_no   = Test.COUNT

    def __str__(self):
        return '''
subroutine tunit_test{self.test_no}(info, res) bind(C, name='tunit_test{self.test_no}')
  use tunit
  implicit none
  type(TestInfo) :: info
  type(TestResult) :: res

{self.test_body}

end subroutine tunit_test{self.test_no}
'''.format(self=self)

#=======================================================================================================================

def extract(string, depth=0):

    if not string:
        return (0, '', '')

    idx = 0

    if depth == 0:
        while string[idx].isspace():
            if idx == (len(string) - 1):
                return (0, '', '') # Nothing but whitespace
            idx += 1
        if string[idx] != '(':
            return (0, '', '')
        depth = 1
        idx  += 1

    start = idx

    while idx < len(string):
        if string[idx] == '(': depth += 1
        if string[idx] == ')': depth -= 1
        if depth == 0: break
        idx += 1

    if depth == 0:
        return (depth, string[start:idx], string[idx+1:])
    else:
        return (depth, string[start:idx], string[idx:])

#=======================================================================================================================

def strip_comments(string):
    if (len(string) > 0
            and ((string[0] == '\'' and string[-1] == '\'') or (string[0] == '"' and string[-1] == '"'))):
        return string[1:-1]
    else:
        return string

#=======================================================================================================================

def smart_split(string, char):

    idx    = 0
    depth  = 0
    tokens = []
    in_single_quote = False
    in_double_quote = False

    while idx < len(string):

        if string[idx] == '(': depth += 1
        if string[idx] == ')': depth -= 1

        if string[idx] == '\'':
            if in_single_quote:
                depth -= 1
                in_single_quote = False
            else:
                depth += 1
                in_single_quote = True

        if string[idx] == '"':
            if in_double_quote:
                depth -= 1
                in_double_quote = False
            else:
                depth += 1
                in_double_quote = True

        if string[idx] == ',' and depth == 0:
            tokens.append(string[:idx].strip())
            string = string[idx+1:]
            idx = 0
        else:
            idx += 1
    if len(string):
        tokens.append(string.strip())
    return tuple(tokens)

#=======================================================================================================================

def parse_test(line, lines, line_no, file_name, macros):

    match      = TEST_RE.search(line)
    pre_match  = line[:match.start()]
    post_match = line[match.end():]

    (depth, text, remainder) = extract(post_match)

    start_line_no = line_no

    while depth > 0:
        assert remainder == ''

        if not lines:
            raise ParseException('Found end of file while parsing test')
        line_no, line = lines.pop(0)

        (depth, suite_text, remainder) = extract(line, depth)

        if depth > 0:
            tmp, line_no, depth_delta = parse_macros(suite_text, lines, line_no, file_name, macros)
            text  += tmp
            depth -= depth_delta
        else:
            tmp, line_no, depth_delta = parse_macros(suite_text, None, line_no, file_name, macros) # Not allowed extra lines
            text  += tmp
            depth -= depth_delta

    args = smart_split(text, ',')
    
    line = remainder

    while not line.strip().startswith('('):
        if line.strip():
            raise ParseException('Unexpected text found: ' + line)
        if not lines:
            raise ParseException('Found end of file while parsing test')
        line_no, line = lines.pop(0)

    (depth, text, remainder) = extract(line)
    while depth > 0:
        assert remainder == ''

        if not lines:
            raise ParseException('Found end of file while parsing test (%s:%d)' % (file_name, start_line_no))
        line_no, line = lines.pop(0)

        (depth, test_text, remainder) = extract(line, depth)

        if depth > 0:
            tmp, line_no, depth_delta = parse_macros(test_text, lines, line_no, file_name, macros)
            text  += tmp
            depth -= depth_delta
        else:
            tmp, line_no, depth_delta = parse_macros(test_text, None, line_no, file_name, macros) # Not allowed extra lines
            text  += tmp
            depth -= depth_delta

    test_name = args[0]
    test_body = text

    return Test(test_name, test_body), remainder, line_no

#=======================================================================================================================

def parse_suite(line, lines, line_no, file_name, macros):

    match      = SUITE_RE.search(line)
    pre_match  = line[:match.start()]
    post_match = line[match.end():]

    (depth, text, remainder) = extract(post_match)

    if depth == 0:
        text, line_no, depth_delta = parse_macros(text, None, line_no, file_name, macros) # Not allowed extra lines
        depth += depth_delta
    else:
        while depth > 0:
            assert remainder == ''

            if not lines:
                raise ParseException('Found end of file while parsing suite')
            line_no, line = lines.pop(0)
            (depth, suite_text, remainder) = extract(line, depth)
            if depth > 0:
                tmp, line_no, depth_delta = parse_macros(suite_text, lines, line_no, file_name, macros)
                text  += tmp
                depth -= depth_delta
            else:
                tmp, line_no, depth_delta = parse_macros(suite_text, None, line_no, file_name, macros) # Not allowed extra lines
                text  += tmp
                depth -= depth_delta

    args = smart_split(text, ',')

    suite_name = args[0]

    return Suite(suite_name, *args[1:]), remainder, line_no

#=======================================================================================================================

def parse_macros(line, lines, line_no, file_name, macros):

    found_match = True
    depth_delta = 0

    while found_match:
        found_match = False

        for name, func in macros.iteritems():
            match = name.search(line)
            if match:
                found_match = True
                start_line = line_no
                pre_match  = line[:match.start()]
                post_match = line[match.end():]

                (depth, macro_text, remainder) = extract(post_match)

                while depth > 0:
                    assert remainder == ''

                    if not lines:
                        raise ParseException('Found end of text while parsing macro: ' + text)
                    
                    depth_delta = depth

                    line_no, line = lines.pop(0)
                    (depth, text, remainder) = extract(line, depth)

                    tmp, line_no, delta = parse_macros(text, lines, line_no, file_name, macros)
                    depth_delta += delta
                    macro_text  += tmp

                args = smart_split(macro_text, ',')
                try:
                    line = pre_match + func(*args, line_no=start_line, file_name=file_name) + remainder
                except TypeError:
                    line = pre_match + func(*args) + remainder # Macro doesn't handle keyword arguments

    return line, line_no, depth_delta

#=======================================================================================================================

def _assert_macro(assert_str, exp, act, tol=0., **kwargs):
    exp_str = "'" + exp.replace("'", "''") + "'"
    act_str = "'" + act.replace("'", "''") + "'"
    loc = locals()
    del loc['kwargs']
    kwargs.update(loc)
    return assert_str.format(**kwargs)

#=======================================================================================================================

def parse(file):

    macros = {
        re.compile(r'\bASSERT\b'): functools.partial(_assert_macro, ASSERT_EQUAL),
        re.compile(r'\bASSERT_NOT_EQUAL\b'): functools.partial(_assert_macro, ASSERT_NOT_EQUAL),
        re.compile(r'\bASSERT_CLOSE\b'): functools.partial(_assert_macro, ASSERT_CLOSE),
    }

    lines = [(i + 1, line) for i, line in enumerate(file.readlines())]

    suite = None
    tests = []

    _lines = []
    for line_no, line in lines:
        if line.strip().startswith('#'):
            if line.strip().startswith('# tunit:'):
                custom_func = {'__builtins__': {}}
                exec line.replace('# tunit:', '').strip() in custom_func
                del custom_func['__builtins__']
                for name, func in custom_func.iteritems():
                    if callable(func): macros[re.compile(r'\b' + name + r'\b')] = func
        else:
            _lines.append((line_no, line))

    lines = _lines

    while lines:
        
        line_no, line = lines.pop(0)

        if SUITE_RE.search(line):
            if suite:
                raise ParseException('SUITE defined more than once in file: ' + file_name)
            suite, line, line_no = parse_suite(line, lines, line_no, file.name, macros)

        if TEST_RE.search(line):
            test, line, line_no = parse_test(line, lines, line_no, file.name, macros)
            tests.append(test)

        if line.strip():
            raise ParseException('Unexpected text found: ' + line)

    if not suite:
        raise ParseException('SUITE undefined in file: ' + file.name)
    suite.set_tests(tests)

    return suite

#==============================================================================

def create_runner(fname, suite):
    with open(fname, 'w') as file:
        print >> file, '#include "tunit.h"\n'

        for i, test in enumerate(suite.tests):
            print >> file, 'TUNIT_TEST( %d, "%s" );' % (i+1, test.test_name)

        print >> file, '\nTUNIT_SUITE( "%s" )\n{' % suite.name

        for i in xrange(len(suite.tests)):
            print >> file, '  TUNIT_REGISTER_TEST( %d );' % (i+1)

        print >> file, '}'

#==============================================================================

def main(args):

    if len(args) != 4:
        print 'usage: %s file_name out_file suite_file' % args[0]
        return os.EX_USAGE

    fname  = args[1]
    fout   = args[2]
    fsuite = args[3]

    if not os.path.exists(fname):
        print 'error: file %s does not exist' % fname
        return os.EX_USAGE

    suite = None

    with open(fname) as file:
        try:
            suite = parse(file)
        except ParseException, ex:
            print 'Error parsing file %s -- %s' % (fname, ex)
            raise SystemExit(-1)

    with open(fout, 'w') as file:
        print >> file, suite

    create_runner(fsuite, suite)

#======================================================================================================================

if __name__ == '__main__':
    sys.exit(main(sys.argv))

#!/usr/bin/env python

import os
import sys
import traceback
import string
import re

def rstrip2(text, skip):
  """ this rstrio function is here to support older versions of Python """
  for i in range(len(text)-1, -1, -1):
     if not text[i] in skip:
       return text[:i+1]
  return text


class tppStartParseError(Exception):
  pass

class tppEsacParseError(Exception):
  pass

class tppCaseParseError(Exception):
  pass

class tppEndParseError(Exception):
  pass


def parser():

  case  = 0
  error = 0
  start = 0
  start_pos = 0
  line_no = 0
  writeout = 0
  savestdout = ""

  rawstr = """^\s*!TPP\s*(?P<CMD>\w+)?\s*(?P<COUNTER>.*)$"""
  compile_obj = re.compile(rawstr, re.IGNORECASE)
  digits = re.compile(r'(\d+)')

  a = len(sys.argv)
  if a < 2 or a > 3:
    print "\nUsage:\n\t%s <input file> [output file]\n" % sys.argv[0]
    sys.exit(1)

  out_file = ""
  tpp_file = sys.argv[1]

  if a > 2:
    savestdout = sys.stdout
    out_file = sys.argv[2]
    try:
      fsock = open(out_file, 'w')
    except:
      print >> sys.stderr, "Unable to open %s" % out_file
      sys.exit(-1)
    sys.stdout = fsock

  try:
    src_file = open(tpp_file, 'r')
  except:
    print >> sys.stderr, "Unable to open %s" % tpp_file
    sys.exit(-1)

  try:
    while 1:

      if start > 0 and start_pos == 0:
        start_pos = src_file.tell()
        start_line = line_no

      line = src_file.readline()
      line_no = line_no + 1
      if not line:
        break

      line = rstrip2(line, '\n')
      a = line.split("//")
      tag = a[0]

      match_obj = compile_obj.search(tag)
      if match_obj:

        CMD     = string.lower(match_obj.group('CMD'))
        COUNTER = string.lower(match_obj.group('COUNTER'))

        if CMD == "start":
          if start == 0:
            start = 1
            end = int(COUNTER)
          else:
            raise tppStartParseError

        if CMD == "case":
          if case == 0:
            case = 1
            counter_list = []
            pieces = digits.split(COUNTER)
            for a in pieces:
              try:
                 if int(a):
                   counter_list.append(int(a))
              except:
                 pass

            if start in counter_list:
              writeout = 1
          else:
            raise tppCaseParseError

        if CMD == "esac":
          if case == 1:
            writeout = 0
            case = 0
          else:
            raise tppEsacParseError

        if CMD == "end":
          if  start != 0:
            if (start < end):
              src_file.seek(start_pos, 0)
              line_no = start_line
              start = start + 1
            else:
              start_pos = start = writeout = 0
          else:
            raise tppEndParseError
          
        continue

      else:
        if writeout == 1 or case == 0:
          print "%s" % (line)

  except tppEsacParseError:
    print >> sys.stderr, "<tpp parse error line %s - !TPP ESAC detected without preceeding CASE statement>" % (line_no)
    error = -1

  except tppStartParseError:
    print >> sys.stderr, "<tpp parse error line %s - !TPP START detected before END statement>" % (line_no)
    error = -1

  except tppCaseParseError:
    print >> sys.stderr, "<tpp parse error line %s - !TPP CASE detected without preceeding ESAC statement>" % (line_no)
    error = -1

  except tppEndParseError:
    print >> sys.stderr, "<tpp parse error line %s - !TPP END detected without preceeding START statement>" % (line_no)
    error = -1

  except:
    print >> sys.stderr, "<tpp error : line %s - %s" % ( line_no, line)


  if error < 0:
    if out_file:
      if os.stat(out_file):
        os.unlink(out_file)

  if savestdout:
    sys.stdout = savestdout
    fsock.close()
    
  src_file.close()
  sys.exit(error)

if __name__ == '__main__':
  parser()

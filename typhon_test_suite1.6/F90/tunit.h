
#ifndef TUNIT_H
#define TUNIT_H

#define QUOTE(X) _QUOTE(X)
#define _QUOTE(X) #X
#define MSG_LEN 1024

#if !defined(TUNIT_FORTRAN)

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef TUNIT_MPI
#  include <mpi.h>
#endif

#define TUNIT_SUITE( NAME ) \
    int main(int argc, char ** argv) { return tunit_main(argc, argv, NAME); } \
    void tunit_init(TestFunc_t ** test_funcs, TestInfo_t ** test_info, size_t * num_funcs)

#define TUNIT_TEST( NUM, NAME ) \
    const char * tunit_test_name##NUM = NAME; \
    void tunit_test##NUM(TestInfo_t * info, TestResult_t * result)

#define TUNIT_REGISTER_TEST( NUM ) \
    add_test(tunit_test##NUM, tunit_test_name##NUM, NUM, test_funcs, test_info, num_funcs);

#define TUNIT_EXIT_FUNC( NAME ) void NAME(TestInfo_t * info)

#define TUNIT_ATEXIT( FUNC ) info->exit_func = FUNC;

#define SUITE( NAME ) TUNIT_SUITE( NAME )
#define TEST( NUM, NAME ) TUNIT_TEST( NUM, NAME )
#define REGISTER_TEST( NUM ) TUNIT_REGISTER_TEST( NUM )

#define ASSERT_EQUAL( X, Y ) \
    if ( (X) != (Y) ) { \
        assert_failed(result, "==", QUOTE(X), QUOTE(Y), __FILE__, __LINE__, (long) (Y), sizeof(Y)); \
    }

#define ASSERT_NOT_EQUAL( X, Y ) \
    if ( (X) == (Y) ) { \
        assert_failed(result, "!=", QUOTE(X), QUOTE(Y), __FILE__, __LINE__, (long) (Y), sizeof(Y)); \
    }

#define ASSERT_EQUAL_ARR( X, Y, N ) \
    { \
        size_t n_pos; \
        for (n_pos = 0; n_pos < N; ++n_pos) { \
            if ( (X)[n_pos] != (Y)[n_pos] ) { \
                assert_failed(result, "==", QUOTE(X), QUOTE(Y), __FILE__, __LINE__, (long) (Y)[n_pos], sizeof(*(Y))); \
                break; \
            } \
        } \
    } \

#define ASSERT( X ) \
    if ( ! (X) ) { \
        assert_failed(result, NULL, QUOTE(X), NULL, __FILE__, __LINE__, 0, 0); \
    }

#define ASSERT_FILE_EXISTS( FILE_NAME ) { \
    int iostat = access(FILE_NAME, F_OK); \
    ASSERT_EQUAL(0, iostat); }

#define ASSERT_FILE_NOT_EXISTS( FILE_NAME ) { \
    int iostat = access(FILE_NAME, F_OK); \
    ASSERT_NOT_EQUAL(0, iostat); }

/* ================================================================================================================= */

typedef struct TestInfo TestInfo_t;
typedef struct TestResult TestResult_t;

typedef void (*TestFunc_t)(struct TestInfo *, struct TestResult *);
typedef void (*ExitFunc_t)(struct TestInfo *);

void tunit_init(TestFunc_t ** test_funcs, TestInfo_t ** test_info, size_t * num_funcs);
int tunit_run_test(int argc, char ** argv, int test_num);
int tunit_run_suite(const char * prog, const char * name, int fork);

/* ================================================================================================================= */

struct TestInfo
{
    char name[200];
    int num;
#ifdef TUNIT_MPI
    MPI_Comm comm;
    MPI_Info info;
    MPI_Fint fcomm;
    MPI_Fint finfo;
#else
    size_t comm;
    size_t info;
    int fcomm;
    int finfo;
#endif
    int rank;
    int size;
    ExitFunc_t exit_func;
};

/* ================================================================================================================= */

struct TestResult
{
    char ** messages;
    size_t num_messages;
    int passed;
};

/* ================================================================================================================= */

void
add_test(
        TestFunc_t func,
        const char * name,
        int num,
        TestFunc_t ** test_funcs,
        TestInfo_t ** test_info,
        size_t * num_funcs
        )
{
    *test_funcs = (TestFunc_t *) realloc(*test_funcs, (*num_funcs + 1) * sizeof(TestFunc_t));
    *test_info  = (TestInfo_t *) realloc(*test_info,  (*num_funcs + 1) * sizeof(TestInfo_t));

    memset(&((*test_info)[*num_funcs]), '\0', sizeof(TestInfo_t));
    memcpy((*test_info)[*num_funcs].name, name, 200);
    (*test_info)[*num_funcs].num  = num;

    (*test_funcs)[*num_funcs] = func;

    ++(*num_funcs);
}

/* ================================================================================================================= */

void
print_usage(
        char * prog
        )
{
    printf("usage: %s [test_num]\n", prog);
}

/* ================================================================================================================= */

int parse_int(
        const char * str
        )
{
    int res;
    char * end_ptr;

    errno = 0;
    res = strtol(str, &end_ptr, 10);

    if (!errno && *end_ptr == '\0') {
        return res;
    } else {
        errno = 0;
        printf("Invalid test number: %s\n", str);
        return -1;
    }
}

/* ================================================================================================================= */

int
tunit_main(
        int argc,
        char ** argv,
        const char * name
        )
{
    int opt;
    int fork = 0;

    while ((opt = getopt(argc, argv, "hf")) != -1) {
        switch (opt) {
        case 'h':
            print_usage(argv[0]);
            return EXIT_SUCCESS;
        case 'f':
            fork = 1;
            break;
        default:
            print_usage(argv[0]);
            return EINVAL;
        }
    }

    if (optind >= argc) {
        // No test number given.
        tunit_run_suite(argv[0], name, fork);
    } else {
        int test_num = parse_int(argv[optind]);

        if (test_num != -1) {
            return tunit_run_test(argc, argv, test_num);
        }
    }

    return EXIT_SUCCESS;
}

/* ================================================================================================================= */

int
tunit_run_suite(
        const char * prog,
        const char * name,
        int fork
        )
{
    size_t num_funcs        = 0;
    TestFunc_t * test_funcs = NULL;
    TestInfo_t * test_info  = NULL;

    tunit_init(&test_funcs, &test_info, &num_funcs);

    free(test_funcs);
    free(test_info);

    int run = 0;
    int passed = 0;
    int test_num;
    char test_num_str[100];

    puts("----------------------------------------------------------------------------------------------------");
    printf("Test Suite: %s\n\n", name);

    for (test_num = 0; test_num < num_funcs; ++test_num) {
        memset(test_num_str, '\0', 100);
        sprintf(test_num_str, "%d", test_num);

        if (fork) {
            pid_t pid = vfork();

            if (pid == 0) {
                execl(prog, prog, test_num_str, (char *) NULL);
            } else if (pid != -1) {
                int status;
                waitpid(pid, &status, 0);
                ++run;
                if (WIFEXITED(status)) {
                    passed += (WEXITSTATUS(status) == EXIT_SUCCESS);
                }
            } else {
                printf("Failed to launch test process\n");
                return EXIT_FAILURE;
            }
        } else {
            char ** argv = NULL;
            int argc = 0;
            ++run;
            passed += (tunit_run_test(argc, argv, test_num) == 0);
        }
    }

    printf("\nComplete: %d of %d tests passed\n", passed, run);
    puts("----------------------------------------------------------------------------------------------------");

    return EXIT_SUCCESS;
}

/* ================================================================================================================= */

int
tunit_run_test(
        int argc,
        char ** argv,
        int test_num
        )
{
    int rank;
    int size;
    int retval = EXIT_SUCCESS;

#ifdef TUNIT_MPI
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_rank(MPI_COMM_WORLD, &size);
#else
        rank = 0;
        size = 1;
#endif

    size_t num_funcs        = 0;
    TestFunc_t * test_funcs = NULL;
    TestInfo_t * test_info  = NULL;

    tunit_init(&test_funcs, &test_info, &num_funcs);

    if (test_num > (num_funcs-1)) {
        printf("Invalid test number: %d\n", test_num);
        retval = EINVAL;
    } else {
        TestInfo_t * info = &test_info[test_num];
        TestResult_t result;
        size_t msg_idx;

        memset(&result, '\0', sizeof(TestResult_t));
        result.passed = 1;

#ifdef TUNIT_MPI
        info->comm  = MPI_COMM_WORLD;
        info->info  = MPI_INFO_NULL;
        info->fcomm = MPI_Comm_c2f(MPI_COMM_WORLD);
        info->finfo = MPI_Info_c2f(MPI_INFO_NULL);
#else
        info->comm  = (size_t) 0;
        info->info  = (size_t) 0;
        info->fcomm = (int) 0;
        info->finfo = (int) 0;
#endif        
        info->rank = rank;
        info->size = size;

        test_funcs[test_num](info, &result);
        if (result.passed) {
#ifdef TUNIT_MPI
            printf("<Rank %d> [Test %2d] %- 80s: Passed\n", info->rank, info->num, info->name);
#else
            printf("[Test %2d] %- 80s: Passed\n", info->num, info->name);
#endif
        } else {
#ifdef TUNIT_MPI
            printf("<Rank %d> [Test %2d] %- 80s: Failed\n", info->rank, info->num, info->name);
#else
            printf("[Test %2d] %- 80s: Failed\n", info->num, info->name);
#endif
            for (msg_idx = 0; msg_idx < result.num_messages; ++msg_idx) {
                printf("    %s\n", result.messages[msg_idx]);
            }
            retval = EXIT_FAILURE;
        }

        for (msg_idx = 0; msg_idx < result.num_messages; ++msg_idx) {
            free(result.messages[msg_idx]);
        }
        free(result.messages);

        if (info->exit_func != NULL) {
            info->exit_func(info);
        }
    }

#ifdef TUNIT_MPI
        MPI_Finalize();
#endif

    free(test_funcs);
    free(test_info);

    return retval;
}

/* ================================================================================================================= */

void
append_message(
        char *** messages,
        size_t * num_messages,
        char * message,
        const size_t * message_len
        )
{
    ++(*num_messages);
    *messages = (char **) realloc(*messages, *num_messages * sizeof(char *));
    *messages[*num_messages-1] = (char *) calloc(*message_len + 1, sizeof(char));
    memcpy(*messages[*num_messages-1], message, * message_len);
    messages[*num_messages-1][*message_len] = '\0';
}

void
assert_failed(
        TestResult_t * result,
        const char * type,
        const char * exp,
        const char * var,
        const char * file,
        int line,
        long val,
        size_t val_size
        )
{
    const size_t msg_len = MSG_LEN;

    result->passed = 0;

    char message[msg_len];
    memset(message, '\0', msg_len);

    if (type == NULL) {
        sprintf(message, "Assertion { %s } failed (%s:%d)", exp, file, line); \
    } else {
        if (val_size == sizeof(int)) {
            sprintf(message, "Assertion { %s %s %s } failed for value { %d } (%s:%d)", var, type, exp, (int) val, file, line);
        } else if (val_size == sizeof(long)) {
            sprintf(message, "Assertion { %s %s %s } failed for value { %ld } (%s:%d)", var, type, exp, (long) val, file, line);
        }
    }

    message[msg_len] = '\0';
    append_message(&result->messages, &result->num_messages, message, &msg_len);
}

/* ================================================================================================================= */

#endif // !defined(TUNIT_FORTRAN)

#endif // TUNIT_H

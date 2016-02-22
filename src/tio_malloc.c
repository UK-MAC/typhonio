/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "tio_error_int.h"
#include "tio_malloc_int.h"


#if defined (_TYPHIO_MEMORY_DEBUG) || defined (_TYPHIO_MEMORY_STATS)


/* Wrappers to malloc/realloc and free to optionally track TyphonIO memory usage
   - unashamedly 'borrowed' and hacked from David Tanqueray's (Cray UK) heapmax utility */


#ifdef _TYPHIO_MEMORY_DEBUG

#if defined (__QK_USER__)
#include <catamount/data.h>     /* _my_rank */
#endif


#define SUBNAME_LEN 64

struct heap_data {
  char subname[SUBNAME_LEN];
  int  line;
  void *p;
  size_t size;
};

#else

struct heap_data {
  void   *p;
  size_t size;
};

#endif

static struct heap_data *heap_list = NULL;


/* Heap usage statistics. */

static long memuse    = 0;        /* amount of memory allocated now     */
static long memcount  = 0;        /* number of blocks allocated now     */
static long maxmem    = 0;        /* maximum amount of memory allocated */
static long maxcount  = 0;        /* maximum number of blocks allocated */
static long summem    = 0;        /* sum of  memory allocated           */
static long sumcount  = 0;        /* total number of blocks allocated   */


/* List of allocated blocks used to track sizes. */

static size_t heap_count = 0;     /* number of items on the list        */
static size_t heap_limit = 0;     /* size of the allocated list         */
static size_t heap_holes = 0;     /* number of empty slots in the list  */
static size_t heap_error = 0;     /* if unable to track pointers        */

static const size_t heap_inc = 4096;

#ifdef _TYPHIO_MEMORY_DEBUG
static const int dump = 1;
#else
static const int dump = 0;
#endif

static int disable    = 0;        /* do not track pointers              */
static int add_exit   = 1;        /* flag to register atexit routine    */


static size_t last_requested = 0;


/*
 * Print report when process terminates.
 */
static void exit_stats(void)
{
  char buffer[8192];    /* printing output buffer         */
  char *out = buffer;   /* pointer into the output buffer */

#if defined(__QK_USER__)
  /* Catamount Cray XT3 */
  int rank = _my_rank;

#elif defined(__crayx1)
  /* Cray X1 */
  extern unsigned int _dm_my_pe;
  int rank = _dm_my_pe;

#elif defined(ibm)
  /* IBM AIX */
  char *env_rank = getenv("MP_CHILD");
  int rank;

  if (env_rank != NULL) {
    rank = atoi(env_rank);
  }
  else {
    rank = 0;
  }

#else
  /* MPICH */
  char *env_rank = getenv("MPIRUN_RANK");
  int rank;

  if (env_rank != NULL) {
    rank = atoi(env_rank);
  }
  else {
    rank = 0;
  }
#endif


  if (disable) {
    return;
  }

  if (rank >= 0) {
    out += sprintf(out, "PE %d: ", rank);
  }
  out += sprintf(out, "Heap allocation statistics:\n");

  if (heap_error) {
    out += sprintf(out, "Heap allocation statistics failed.\n");
    out += sprintf(out, "No statistics will be reported.\n");
  }
  else if (0 == sumcount) {
    out += sprintf(out, "No heap allocation occured.\n");
  }
  else {
    double max_mb    = (double) maxmem / 1024 / 1024;
    long   avg_block = (double) summem / sumcount;

    out += sprintf(out, "%12.6f megabytes maximum heap allocation.\n", (float)max_mb);
    out += sprintf(out, "%12lld blocks allocated maximum.\n", (long long int)maxcount);
    out += sprintf(out, "%12lld blocks allocated total.\n",   (long long int)sumcount);
    out += sprintf(out, "%12lld bytes average block size.\n", (long long int)avg_block);
  }

  write(2, buffer, strlen(buffer));   /* unbuffered write to file descriptor 2 = stderr */


  if (dump) {

    size_t i;

    out = buffer;

    if (rank >= 0) {
      out += sprintf(out, "\nPE %d: ", rank);
    }
    out += sprintf(out, "Allocated memory:\n");

    write(2, buffer, strlen(buffer));

    for (i=0; i<heap_count; ++i) {
      if (heap_list[i].p != NULL) {
        out = buffer;

        out += sprintf(out, "[%06d] %12lld bytes allocated - %#012lx ",
                       (int)i, (long long int)heap_list[i].size, (long unsigned int)heap_list[i].p);

#ifdef _TYPHIO_MEMORY_DEBUG
        out += snprintf(out, SUBNAME_LEN, "%s", heap_list[i].subname);
        out += sprintf(out, " (%d)\n", heap_list[i].line);

        /* out += sprintf(out, "%64s (%d)\n", heap_list[i].subname, heap_list[i].line); */

#else
        out += sprintf(out, " \n");
#endif

        write(2, buffer, strlen(buffer));
      }
    }

    if (heap_list != NULL) {
      free (heap_list);
    }

    (void) sprintf(buffer, "   -- Done\n");
    write(2, buffer, strlen(buffer));

  }


  return;
}


/*
 * Add a newly allocated block to the list.
 */
static void list_alloc(const char subname[], const int line, void *p, const size_t size)
{
  size_t insp;


  if (NULL == p)  {
    return;
  }
  if (heap_error) {
    return;
  }
  if (disable) {
    return;
  }

  ++disable;

  if (heap_count < heap_limit) {

    insp = heap_count;
    ++heap_count;

  }
  else if (heap_holes > 0) {

    /* Look for a empty slot in the list. */
    size_t i;

    for (i=0; i<heap_count; i++) {
      if (NULL == heap_list[i].p) {
        insp = i;
        --heap_holes;
        break;
      }
    }

  }
  else {

    /* Expand the list. */
    size_t new_limit = heap_limit + heap_inc;
    size_t i;

    heap_list = (struct heap_data *)realloc(heap_list, (new_limit * sizeof(struct heap_data)));

    if (NULL == heap_list) {
      heap_count = 0;
      heap_limit = 0;
      heap_error = 1;
    }
    else {

      for (i=heap_limit; i<new_limit; ++i) {
#ifdef _TYPHIO_MEMORY_DEBUG
        (void) strcpy(heap_list[i].subname, " ");
        heap_list[i].line = 0;
#endif
        heap_list[i].p    = 0;
        heap_list[i].size = 0;
      }

      heap_limit = new_limit;
      insp = heap_count;
      ++heap_count;
    }
  }


#ifdef _TYPHIO_MEMORY_DEBUG
  (void) strncpy(heap_list[insp].subname, subname, SUBNAME_LEN);
  heap_list[insp].line = line;
#endif
  heap_list[insp].p    = p;
  heap_list[insp].size = size;


  memuse += size;

  if (memuse > maxmem) {
    maxmem = memuse;
  }

  ++memcount;
  if (memcount > maxcount) {
    maxcount = memcount;
  }
  summem += size;
  ++sumcount;


  /* Add the exit check if not already done so.  Allow for the
   * possibility that init() gets called many times. */

  if (add_exit) {
    atexit(exit_stats);
    add_exit = 0;
  }

  --disable;


  return;
}


/*
 * Remove a freed block from the list.
 */
static void list_free(void *p)
{
  size_t i;

  if (NULL == p)  {
    return;
  }
  if (heap_error) {
    return;
  }
  if (disable) {
    return;
  }
  ++disable;

  for (i=0;i < heap_count;i++) {
    if (p == heap_list[i].p) {
      memuse -= heap_list[i].size;
      --memcount;
#ifdef _TYPHIO_MEMORY_DEBUG
      (void) strcpy(heap_list[i].subname, " ");
      heap_list[i].line = 0;
#endif
      heap_list[i].p    = NULL;
      heap_list[i].size = 0;
      ++heap_holes;
      break;
    }
  }
  --disable;

  if (0 == memcount) {
    free (heap_list);
    heap_list  = NULL;
    heap_count = 0;
    heap_limit = 0;
    heap_holes = 0;
  }

  return;
}





/***************************************************************************************************
 *
 * TIOmalloc
 * TIOrealloc
 * TIOfree
 *
 **************************************************************************************************/

void *dTIO_malloc( const char subname[], const int line, const size_t size )
{
  void *p = NULL;    /* Easy addition of NULL to check for failed malloc */

  last_requested = size;

  /* Check that size>0 - TyphonIO should always require this! */
  if (0 == size) {
    (void) iTIO_StoreError( ERR_MEM, TIO_ERR_INT, "TIOmalloc",
                            __FILE__, __LINE__, "Attempting to malloc zero size" );
    return (p);
  }

  p = malloc(size);
  list_alloc(subname, line, p, size);

  return (p);
}


void *dTIO_realloc( const char subname[], const int line, void *p, const size_t size )
{
  /* No check for size==0 as this is valid realloc use */

  last_requested = size;

  list_free(p);
  p = realloc(p, size);
  list_alloc(subname, line, p, size);

  return (p);
}


void dTIO_free( void *p )
{
  list_free(p);
  free (p);

  return;
}

#endif
/* if defined (_TYPHIO_MEMORY_DEBUG) || defined (_TYPHIO_MEMORY_STATS) */




/***************************************************************************************************
 *
 * TIOmalloc
 * TIOrealloc
 * TIOfree
 *
 * Production wrappers for malloc, realloc and free
 * - simply have addedd pre- and post- nullification for error-checking/cleaning-up purposes
 *
 **************************************************************************************************/

void *xTIO_malloc( const size_t size )
{
  void *p = NULL;    /* Easy addition of NULL to check for failed malloc */
  p = malloc(size);

  return (p);
}


void *xTIO_realloc( void *p, const size_t size )
{
  p = realloc(p, size);    /* Don't set to initial NULL - realloc needs original address  */

  return (p);
}


void xTIO_free( void *p )
{
  free (p);

  return;
}


/***************************************************************************************************
 *
 * iTIO_MemStats
 *
 * Simple routine to return a few memory stats
 * Obtains the last memory request to malloc/realloc
 * Also sums the current accounted memory
 *
 **************************************************************************************************/
int iTIO_MemStats(size_t *current, size_t *requested)
{
  size_t total = 0;
  size_t i;

  *current   = 0;
  *requested = 0;

#if defined (_TYPHIO_MEMORY_DEBUG) || defined (_TYPHIO_MEMORY_STATS)

  for (i=0; i<heap_count; ++i) {
    if (heap_list[i].p != NULL) {
      total += heap_list[i].size;
    }
  }

  *current   = total;
  *requested = last_requested;

  return (0);

#endif

  return (-1);
}

/*
 * EOF
 */

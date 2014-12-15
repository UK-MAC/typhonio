/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_MALLOC_INT_H
#define _TIO_MALLOC_INT_H


#if defined (_TYPHIO_MEMORY_DEBUG) || defined (_TYPHIO_MEMORY_STATS)


#define TIOmalloc(s)    dTIO_malloc(subname, __LINE__, (size_t)(s))
#define TIOrealloc(p,s) dTIO_realloc(subname, __LINE__, (p), (size_t)(s))
#define TIOfree(p)      dTIO_free((p))

extern void *dTIO_malloc  ( const char *subname, const int line, const size_t size );
extern void *dTIO_realloc ( const char *subname, const int line, void *p, const size_t size );
extern void dTIO_free     ( void *p );


#else

#define TIOmalloc(s)    xTIO_malloc((size_t)(s))
#define TIOrealloc(p,s) xTIO_realloc((p),(size_t)(s))
#define TIOfree(p)      xTIO_free((p))

#endif


extern void *xTIO_malloc  ( const size_t size );
extern void *xTIO_realloc ( void *p, const size_t size );
extern void xTIO_free     ( void *p );


extern int iTIO_MemStats(size_t *current,  size_t *requested);


#endif

/*
 * EOF
 */

/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_ERROR_INT_H
#define _TIO_ERROR_INT_H

/* -- Need to include tio_global_int.h & tio_error.h to get datatypes */
#include "tio_global_int.h"
#include "tio_error.h"

/*#include <stdarg.h>*/

typedef enum { ERR_USER,
               ERR_INT,
               ERR_MEM,
               ERR_MPI,
               ERR_HDF5 } iErrorClass_t;

/*
 *  Standard internal error codes:
 *  Used to standardise some of the return error values from internal routines for common errors.
 */

typedef enum { ITIO_ERR_NONE     =     0,
               ITIO_ERR_NULL     = -1001,       /* must all be < -1000 */
               ITIO_ERR_FILEID   = -1002,
               ITIO_ERR_OBJECTID = -1003,
               ITIO_ERR_PARENTID = -1004,
               ITIO_ERR_INDEX    = -1005,
               ITIO_ERR_DATATYPE = -1006,
               ITIO_ERR_MESHTYPE = -1007,
               ITIO_ERR_DIMS     = -1008,
               ITIO_ERR_ARG      = -1009,
               ITIO_ERR_CLASS    = -1010,
               ITIO_ERR_CREATE   = -1011,
               ITIO_ERR_OPEN     = -1012,
               ITIO_ERR_CLOSE    = -1013,
               ITIO_ERR_WRITE    = -1014,
               ITIO_ERR_READ     = -1015,
               ITIO_ERR_EXIST    = -1016,
               ITIO_ERR_FILETYPE = -1017,
               ITIO_ERR_MISMATCH = -1020 } internal_error_values;   /* Just sets constants
                                                                       Never gets used as an enum */

/*
 *
 * TIOreturn is production (and debug) - use instead of return() when error stack is to be pushed
 *
 * TIOassert is for debug
 * - works opposite way round to C standard assert in that error is thrown if condition is true
 *
 */


#define TIOreturn( errclass, mesg, rc ) \
           { (void)iTIO_StoreError( errclass, rc, subname, __FILE__, __LINE__, mesg);  \
                                    TIOend(subname,1);                                 \
                                    return (rc);  }

#define cTIOreturn(rc) \
           {                        TIOend(subname,1);                                 \
                                    return (rc);  }

#ifndef NDEBUG
#  define TIOassert( cond, errclass, mesg, rc ) { if (cond) TIOreturn( errclass, mesg, rc); }
#else
#  define TIOassert( cond, errclass, mesg, rc ) { (void)(0); }
#endif





extern int   iTIO_Init( void );

extern int   iTIO_ValidString    ( const char str[],
                                   const int IsName );

extern int   iTIO_ValidDatatype  ( const TIO_Data_t datatype );

extern int   iTIO_ValidDims      ( const TIO_Dims_t ndims, const int tomax );

extern char         *iTIO_PutErr      ( const iErrorClass_t, const char str[] );
extern iErrorClass_t iTIO_GetErrClass ( void );
extern char         *iTIO_GetErrStr   ( void );

extern char *iTIO_GetStringError ( const int err,
                                   const char strname[] );

extern void  iTIO_InitError      ( void );

extern void  iTIO_StoreError     ( const iErrorClass_t errclass,
                                   const int           errcode,
                                   const char          subr[],
                                   const char          file[],
                                   const size_t        line,
                                   const char          desc[] );

extern void  iTIO_KillError      ( void );


extern void  iTIO_Verbose        ( const char subname[],
                                   const int  rank,
                                   const char fmt[],
                                   ... );


#endif

/*
 * EOF
 */

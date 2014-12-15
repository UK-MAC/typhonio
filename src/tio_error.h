/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_ERROR_H
#define _TIO_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif


/* Any changes here get duplicated automatically by the build scripts in F90/tiof_global_f.f90
   Note that the formatting is significant
   Any changes may require extra case handling in tio_error.c
*/

typedef enum {
               TIO_SUCCESS                  =  0,       /* Must be first! */
               TIO_FAIL                     = -1,
               TIO_ERR_INVALID_FILEID       = -2,
               TIO_ERR_INVALID_OBJECTID     = -3,
               TIO_ERR_INVALID_DATATYPE     = -4,
               TIO_ERR_INVALID_DIMS         = -5,
               TIO_ERR_INVALID_ARG          = -6,
               TIO_ERR_INVALID_RANGE        = -7,
               TIO_ERR_INVALID_INDEX        = -8,
               TIO_ERR_INVALID_NAME         = -9,       /* For dodgy object names e.g "atoms/m" */
               TIO_ERR_INVALID_STRING       = -10,      /* For dodgy strings, eg. >TIO_STRLEN   */
               TIO_ERR_INVALID_OPERATION    = -11,
               TIO_ERR_INVALID_CONVERSION   = -12,
               TIO_ERR_INVALID_SECTIONID    = -13,
               TIO_ERR_FILE_CREATE          = -20,
               TIO_ERR_FILE_OPEN            = -21,
               TIO_ERR_FILE_CLOSE           = -22,
               TIO_ERR_FILE_FLUSH           = -23,
               TIO_ERR_FILE_NOT_EXIST       = -24,
               TIO_ERR_FILE_NOT_WRITEABLE   = -30,
               TIO_ERR_FILE_NOT_READABLE    = -31,
               TIO_ERR_FILE_NOT_CLOSEABLE   = -32,
               TIO_ERR_OBJECT_NOT_WRITEABLE = -33,
               TIO_ERR_OBJECT_NOT_READABLE  = -34,
               TIO_ERR_OBJECT_NOT_CLOSEABLE = -35,
               TIO_ERR_OBJECT_CREATE        = -40,
               TIO_ERR_OBJECT_OPEN          = -41,
               TIO_ERR_OBJECT_CLOSE         = -42,
               TIO_ERR_OBJECT_WRITE         = -43,
               TIO_ERR_OBJECT_READ          = -44,
               TIO_ERR_OBJECT_ALREADY_EXIST = -45,
               TIO_ERR_OBJECT_NOT_EXIST     = -46,
               TIO_ERR_VERSION_MISMATCH     = -80,
               TIO_ERR_LIBRARY_MISMATCH     = -81,
               TIO_ERR_UNSUPPORTED_FILETYPE = -82,
               TIO_ERR_F90_API              = -90,
               TIO_ERR_INT                  = -999  } TIO_t;  /* Return codes   */
                                                              /*  0  = success
                                                                 -ve = failure  */

typedef enum { TIO_VERBOSE_NULL             = -1,
               TIO_VERBOSE_OFF              =  0,
               TIO_VERBOSE_ALL              =  1,
               TIO_VERBOSE_MASTER           =  2    } TIO_Verbose_t;


/***************************************************************************************************
 *
 * TIO_Get_Error
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Error
 * </NAME>
 *
 * <PURP>
 * <DATA> Gives a description of a TyphonIO return code
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Error( const TIO_t err,
 * <DATA>                      char        str[STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> err <DATA> TyphonIO error code
 * <DATA> OUT <DATA> str <DATA> String containing error description
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> A brief description of the error signified by <code>err</code> is returned in
 * <DATA> <code>str</code>.
 * <DATA>
 * <DATA> If verbose mode is enabled a stack-trace of any error is also output to
 * <DATA> <code>stderr</code>.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_Error( const TIO_t err,
                            char        str[] );

/***************************************************************************************************
 *
 * TIO_Set_Verbose
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Set_Verbose
 * </NAME>
 *
 * <PURP>
 * <DATA> Sets the verbose mode
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Set_Verbose( const TIO_Verbose_t mode );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> mode <DATA> TyphonIO verbose mode
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Verbose output is not yet fully implemented.
 * <DATA>
 * <DATA> Currently, all verbose mode will do is add stack-trace output to
 * <DATA> <code>TIO_Get_Error()</code>.
 * <DATA> By default, verbose mode is off.
 * <DATA>
 * <DATA> <blockquote>
 * <DATA> <code>TIO_VERBOSE_OFF   </code> - No verbose output<br>
 * <DATA> <code>TIO_VERBOSE_ALL   </code> - Verbose output from all processes<br>
 * <DATA> <code>TIO_VERBOSE_MASTER</code> - Verbose output from only the master process
 * <DATA> </blockquote>
 * <DATA> The master process is the one that gave <code>rank = 0</code> in
 * <DATA> <code>TIO_Create</code> or <code>TIO_Open</code>.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Set_Verbose( const TIO_Verbose_t mode );

/***************************************************************************************************
 *
 * TIO_Get_Verbose
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Verbose
 * </NAME>
 *
 * <PURP>
 * <DATA> Gets the current verbose mode setting
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Verbose( TIO_Verbose_t mode );
 * </CAPI>
 *
 * <PARA>
 * <DATA> OUT <DATA> mode <DATA> TyphonIO verbose mode
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> The value of the current verbose setting is returned.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_Verbose( TIO_Verbose_t *mode );


#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */

/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"
#include "tio_stack_int.h"

#include "typhonio_viz.h"

#include <stdarg.h>

#ifdef _TYPHIO_HDF5
#include "tio_hdf5_int.h"
#endif



#define ENV_ERRORS_FATAL   "TIO_ERRORS_FATAL"
#define ENV_VERBOSE        "TIO_VERBOSE"

#define ELOCLEN 64
#define EMAXLEN 256


struct xError_t {
  iErrorClass_t errclass;
  int           errcode;
  size_t        line;
  char          subr[ELOCLEN];
  char          file[ELOCLEN];
  char          desc[EMAXLEN];
};


static struct iStack_t *error_stack = NULL;

static int   ErrorCount   = 0;
static int   DisableFatal = FALSE;
static int   FatalErrors  = FALSE;

static const char *ErrorClasses[5] = { "ERR_USER", "ERR_INT", "ERR_MEM", "ERR_MPI", "ERR_HDF5" };


static int           HaveInit    = FALSE;
static int           EnvGot      = FALSE;

static int           Verbose     = FALSE;
static TIO_Verbose_t VerboseMode = TIO_VERBOSE_OFF;

static char          ErrorString[EMAXLEN];
static iErrorClass_t ErrorClass;

static int  xTIO_Getenv      ( void );
static void xTIO_DumpErrors  ( void );
static void xTIO_PrintError  ( const size_t        n,
                               const char          *file,
                               const size_t        line,
                               const char          *subr,
                               const iErrorClass_t errclass,
                               const int           errcode,
                               const char          *desc );
static void xTIO_ClearErrors ( void );



/***************************************************************************************************
 *
 * TIO_Get_Error
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Error( const TIO_t err,
               char        str[TIO_STRLEN] )
{
  const char *subname = "TIO_Get_Error()";


  if (str != NULL) {

    /* -- Return a string containing a description corresponding to the given err code */
    switch (err) {

      /*                  0000000001111111111222222222233333333334444444444555555555566666  */
      /*                  1234567890123456789012345678901234567890123456789012345678901234  */

    case TIO_SUCCESS:
      (void) strncpy(str, "No error", TIO_STRLEN);
      break;

    case TIO_FAIL:
      (void) strncpy(str, "Routine failed - misc error: contact code manager", TIO_STRLEN);
      break;


    case TIO_ERR_INVALID_FILEID:
      (void) strncpy(str, "Invalid file ID", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_OBJECTID:
      (void) strncpy(str, "Invalid object ID (may be of wrong object type)", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_DATATYPE:
      (void) strncpy(str, "Invalid datatype", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_DIMS:
      (void) strncpy(str, "Invalid ndims or dims", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_ARG:
      (void) strncpy(str, "Invalid argument given", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_RANGE:
      (void) strncpy(str, "Invalid range given or in file", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_INDEX:
      (void) strncpy(str, "Invalid index value given", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_NAME:
      (void) strncpy(str, "Invalid name given", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_STRING:
      (void) strncpy(str, "Invalid string given", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_OPERATION:
      (void) strncpy(str, "Invalid operation - Cannot perform on this object", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_CONVERSION:
      (void) strncpy(str, "Invalid conversion - Incompatible datatypes", TIO_STRLEN);
      break;

    case TIO_ERR_INVALID_SECTIONID:
      (void) strncpy(str, "Invalid sectionID (myabe wrong mesh / centring )", TIO_STRLEN);
      break;


    case TIO_ERR_FILE_CREATE:
      (void) strncpy(str, "Failed to create file - Check filename & permissions", TIO_STRLEN);
      break;

    case TIO_ERR_FILE_OPEN:
      (void) strncpy(str, "Failed to open file - Check filename & permissions", TIO_STRLEN);
      break;

    case TIO_ERR_FILE_CLOSE:
      (void) strncpy(str, "Failed to close file", TIO_STRLEN);
      break;

    case TIO_ERR_FILE_FLUSH:
      (void) strncpy(str, "Failed to flush file", TIO_STRLEN);
      break;

    case TIO_ERR_FILE_NOT_EXIST:
      (void) strncpy(str, "File does not exist or is not readable", TIO_STRLEN);
      break;

    case TIO_ERR_FILE_NOT_WRITEABLE:
      (void) strncpy(str, "Failed as file was opened read-only", TIO_STRLEN);
      break;

    case TIO_ERR_FILE_NOT_READABLE:
      (void) strncpy(str, "Failed as file hasn't been written & closed", TIO_STRLEN);
      break;

    case TIO_ERR_FILE_NOT_CLOSEABLE:
      (void) strncpy(str, "Failed as file still has open children", TIO_STRLEN);
      break;


    case TIO_ERR_OBJECT_NOT_WRITEABLE:
      (void) strncpy(str, "Failed as object already existed & is not new", TIO_STRLEN);
      break;

    case TIO_ERR_OBJECT_NOT_READABLE:
      (void) strncpy(str, "Failed as object hasn't been written & closed", TIO_STRLEN);
      break;

    case TIO_ERR_OBJECT_NOT_CLOSEABLE:
      (void) strncpy(str, "Failed as object still has open children", TIO_STRLEN);
      break;


    case TIO_ERR_OBJECT_CREATE:
      (void) strncpy(str, "Failed to create object - Check file access mode", TIO_STRLEN);
      break;

    case TIO_ERR_OBJECT_OPEN:
      (void) strncpy(str, "Failed to open object - Check name / idx", TIO_STRLEN);
      break;

    case TIO_ERR_OBJECT_CLOSE:
      (void) strncpy(str, "Failed to close object", TIO_STRLEN);
      break;

    case TIO_ERR_OBJECT_WRITE:
      (void) strncpy(str, "Failed to write object", TIO_STRLEN);
      break;

    case TIO_ERR_OBJECT_READ:
      (void) strncpy(str, "Failed to read object", TIO_STRLEN);
      break;

    case TIO_ERR_OBJECT_ALREADY_EXIST:
      (void) strncpy(str, "Object already exists", TIO_STRLEN);
      break;

    case TIO_ERR_OBJECT_NOT_EXIST:
      (void) strncpy(str, "Object does not exist", TIO_STRLEN);
      break;


    case TIO_ERR_VERSION_MISMATCH:
      (void) strncpy(str, "Can read-only old alpha-version TyphonIO files", TIO_STRLEN);
      break;

    case TIO_ERR_LIBRARY_MISMATCH:
      (void) strncpy(str, "Underlying library mismatch - check library paths", TIO_STRLEN);
      break;

    case TIO_ERR_UNSUPPORTED_FILETYPE:
      (void) strncpy(str, "Underlying filetype is not supported", TIO_STRLEN);
      break;


    case TIO_ERR_F90_API:
      (void) strncpy(str, "Error in F90 call", TIO_STRLEN);
      break;


    case TIO_ERR_INT:
      (void) strncpy(str, "Internal TyphonIO error - contact code manager", TIO_STRLEN);
      break;


    default:
      (void) strncpy(str, "No such error / Undocumented error - contact code manager", TIO_STRLEN);
      return (TIO_ERR_INVALID_ARG);
      break;
    }

  }


  /* -- If in verbose mode, output any TyphonIO and file format error tracebacks (to stderr) */
  if ((err != TIO_SUCCESS) && Verbose) {
    (void)fprintf(stderr, "\n-- TyphonIO error traceback:   ------------------------------- \n\n");
    if (iTIO_IsStackAlloc(error_stack)) {
      (void) xTIO_DumpErrors();
    }
  }


  /* -- Clear TyphonIO and file format error stacks (may have been emptied in printing above) */
  (void) xTIO_ClearErrors();


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Disable_Fatality
 *
 **************************************************************************************************/
TIO_t
TIO_Disable_Fatality( void )
{
  const char *subname = "TIO_Disable_Fatality()";


  FatalErrors  = FALSE;
  DisableFatal = TRUE;

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * iTIO_Init
 *
 **************************************************************************************************/
int
iTIO_Init( void )
{
  const char *subname = "iTIO_Init()";

  int irc;

  if (HaveInit) {
    return (0);
  }

  irc = xTIO_Getenv();
  TIOassert(irc < 0, ERR_INT, "Failed to get TyphonIO environment vars", TIO_ERR_INT);

  HaveInit = TRUE;

  return (0);
}




/***************************************************************************************************
 *
 * iTIO_ValidString
 *
 * iTIO_GetStringError
 *
 **************************************************************************************************/
int
iTIO_ValidString(const char *str, const int IsName)
{
  /* Return values:   0 = string is okay or NULL
   *                 -1 = string too long
   *                 -2 = string starts with "TIO_"
   *                 -3 = string contains a '/'
   */

  size_t ss;


  if (NULL == str) {
    return(0);
  }

  ss = strlen(str);

  if (ss >= TIO_STRLEN) {
    return (-1);
  }

  if (IsName) {
    if (strncmp(str, "TIO_", 4) == 0) {
      return (-2);
    }
    if (strchr(str, (int)'/') != NULL) {
      return (-3);
    }
  }

  return (0);
}


char *
iTIO_GetStringError(const int err, const char *strname)
{
  static char outstr[80];
  char        errstr[30];


  if (0 == err) {
    return (NULL);
  }

  switch (err) {
  case -1:
    (void) strcpy(errstr, "string is too long");
    break;
  case -2:
    (void) strcpy(errstr, "string starts with TIO_");
    break;
  case -3:
    (void) strcpy(errstr, "String contains a '/'");
    break;
  default:
    (void) strcpy(errstr, "other string error");
    /* or no error??? */
    break;
  }

  if (strlen(strname) > 30) {
    (void) snprintf(outstr, sizeof(outstr), "%.30s...: %s", strname, errstr);
  }
  else {
    (void) snprintf(outstr, sizeof(outstr), "%.30s: %s", strname, errstr);
  }


  return (outstr);
}




/***************************************************************************************************
 *
 * iTIO_ValidDatatype
 *
 **************************************************************************************************/
int
iTIO_ValidDatatype( const TIO_Data_t datatype )
{
  if (iTIO_SizeOf(datatype) > 0) {
    return (0);
  }
  else {
    return (-1);
  }
}




/***************************************************************************************************
 *
 * iTIO_ValidDims
 *
 **************************************************************************************************/
int
iTIO_ValidDims( const TIO_Dims_t ndims, const int tomax )
{

  if (tomax) {
    if ((ndims < TIO_0D) || (ndims > TIO_MAXDIMS)) {
      return (-1);
    }
  }
  else {
    if ( (ndims < TIO_0D) || (ndims > TIO_3D) ) {
      return (-1);
    }
  }


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_PutErr
 * iTIO_GetErrClass
 * iTIO_GetErrStr
 *
 **************************************************************************************************/
char *
iTIO_PutErr( const iErrorClass_t err, const char *str )
{
  ErrorClass = err;

  strncpy(ErrorString, str, (size_t)EMAXLEN-1);
  ErrorString[EMAXLEN-1] = '\0';

  return (ErrorString);
}

iErrorClass_t
iTIO_GetErrClass( void )
{
  return (ErrorClass);
}

char *
iTIO_GetErrStr( void )
{
  return (ErrorString);
}




/***************************************************************************************************
 *
 * iTIO_InitError
 *
 **************************************************************************************************/
void
iTIO_InitError( void )
{
  /* Called by all API routines to clear any previous error stack */

  if (ErrorCount != 0) {
    (void) xTIO_ClearErrors();
  }
  ErrorCount = 0;

  return;
}




/***************************************************************************************************
 *
 * iTIO_StoreError
 *
 **************************************************************************************************/
void
iTIO_StoreError( const iErrorClass_t errclass,
                 const int           errcode,
                 const char          subr[],
                 const char          file[],
                 const size_t        line,
                 const char          desc[] )
{
  const char *subname = "iTIO_StoreError()";

  int             irc;
  struct xError_t *errstore = NULL;



  ++ErrorCount;

  if (ERR_MEM == errclass) {
    size_t total, lastreq;

    (void) fprintf(stderr, "\n-- Critical memory error   ----------------------------------- ");
    irc = iTIO_MemStats(&total, &lastreq);
    if (0 == irc) {
      (void) fprintf(stderr, "     Current accounted memory = %12lld bytes\n", (long long int)total);
      (void) fprintf(stderr, "     Last requested memory    = %12lld bytes\n", (long long int)lastreq);
    }
    else {
      (void) fprintf(stderr, "     Current accounted memory = UNAVAILABLE\n");
      (void) fprintf(stderr, "     Last requested memory    = UNAVAILABLE\n");
    }
    FatalErrors = TRUE;
  }

  if (ERR_MPI == errclass) {
    (void) fprintf(stderr, "\n-- Critical MPI error      ----------------------------------- ");
    FatalErrors = TRUE;
  }


  /* -- Allocate a new error store structure */
  if (NULL == error_stack) {

    error_stack = (struct iStack_t *)TIOmalloc(sizeof(struct iStack_t));
    if (NULL == error_stack) {
      (void) iTIO_StoreError(ERR_MEM, ITIO_ERR_NULL, "iTIO_StoreError()", __FILE__, __LINE__,
                             "Failed to allocate error_stack");
    }
    irc = iTIO_StackInit(error_stack);

  }


  errstore = (struct xError_t *)TIOmalloc(sizeof(struct xError_t));
  if (NULL == errstore) {
    (void) iTIO_StoreError(ERR_MEM, ITIO_ERR_NULL, "iTIO_StoreError()", __FILE__, __LINE__,
                           "Failed to allocate errstore");
  }


  /* -- Copy the given args into the errstore */
  errstore->errclass = errclass;
  errstore->errcode  = errcode;
  (void) iTIO_strncpy(errstore->subr, subr, ELOCLEN);
  (void) iTIO_strncpy(errstore->file, file, ELOCLEN);
  (void) iTIO_strncpy(errstore->desc, desc, EMAXLEN);
  errstore->line = line;

  /* -- Push the errstore onto the error stack */
  irc = iTIO_StackPush(error_stack, (void *)errstore);
  if (irc < 0) {
    FatalErrors = TRUE;   /* Won't be able to recover */
    (void) iTIO_StoreError(ERR_INT, irc, "iTIO_StoreError()", __FILE__, __LINE__, "Failed to push stack");
  }


  if (FatalErrors) {
    (void) fprintf(stderr, "\n-- TyphonIO error traceback:   ------------------------------- \n\n");
    (void) xTIO_DumpErrors();
#ifdef _TYPHIO_PARALLEL
    if (ERR_MPI == errclass) {
      irc = MPI_Abort(MPI_COMM_WORLD, errclass);
    }
#endif
    (void) abort();
  }



  return;
}




/***************************************************************************************************
 *
 * iTIO_KillError
 *
 **************************************************************************************************/
void
iTIO_KillError( void )
{
  /* Called by all API routines on exit to do any cleaning up if necessary */
  /* Doesn't need to do anything, but included for possible future use */

  return;
}




/***************************************************************************************************
 *
 * xTIO_Getenv
 *
 **************************************************************************************************/
int
xTIO_Getenv( void )
{
  char *env;


  if (EnvGot) {
    return (0);
  }

  env = getenv( ENV_ERRORS_FATAL );

  FatalErrors = FALSE;

  if (env != NULL) {
    if (atoi(env) > 0) {
      FatalErrors = TRUE;
    }
    else if ( (strcmp(env, "true") == 0) || (strcmp(env, "TRUE") == 0) ) {
      FatalErrors = TRUE;
    }
    else if ( (strcmp(env, "yes") == 0)  || (strcmp(env, "YES") == 0)  ) {
      FatalErrors = TRUE;
    }
    else {
      /* Assume it's a duff value in there */
      ;
    }
  }

  if (DisableFatal) {
    FatalErrors = FALSE;
  }


  env = getenv( ENV_VERBOSE );

  if (env != NULL) {

    int           ienv;
    TIO_Verbose_t tenv;

    ienv = atoi(env);

    if (ienv > 0) {

      tenv = (TIO_Verbose_t)ienv;

      switch (tenv) {
      case TIO_VERBOSE_NULL:
      case TIO_VERBOSE_OFF:
      case TIO_VERBOSE_ALL:
      case TIO_VERBOSE_MASTER:
        VerboseMode = tenv;
        Verbose     = ((TIO_VERBOSE_ALL == tenv) || (TIO_VERBOSE_MASTER == tenv));
        break;
      default:
        VerboseMode = TIO_VERBOSE_NULL;
        Verbose     = FALSE;
        break;
      }

    }
    else {

      if ( (strcmp(env, "true") == 0) || (strcmp(env, "TRUE") == 0) ) {
        VerboseMode = TIO_VERBOSE_ALL;
        Verbose     = TRUE;
      }
      else if ( (strcmp(env, "yes") == 0)  || (strcmp(env, "YES") == 0)  ) {
        VerboseMode = TIO_VERBOSE_ALL;
        Verbose     = TRUE;
      }
      else {
        VerboseMode = TIO_VERBOSE_NULL;
        Verbose     = FALSE;
      }

    }
  }


  EnvGot = TRUE;

  return (0);
}




/***************************************************************************************************
 *
 * xTIO_DumpErrors
 *
 **************************************************************************************************/
static void
xTIO_DumpErrors( void )
{
  size_t i;
  struct xError_t *errstore = NULL;

  int foundHDF5 = FALSE;



  i = 0;

  while ( iTIO_StackPop(error_stack, (void **)&errstore) == 0 ) {

    foundHDF5 = (ERR_HDF5 == errstore->errclass);

    (void) xTIO_PrintError(i, errstore->file, errstore->line, errstore->subr,
                           errstore->errclass, errstore->errcode, errstore->desc);

    TIOfree (errstore);
    errstore = NULL;

    ++i;
  }

#ifdef _TYPHIO_HDF5
  if (foundHDF5) {
    (void) iH5_PrintErrors();
  }
#endif


  return;
}




/***************************************************************************************************
 *
 * xTIO_PrintError
 *
 **************************************************************************************************/
static void
xTIO_PrintError( const size_t        n,
                 const char          *file,
                 const size_t        line,
                 const char          *subr,
                 const iErrorClass_t errclass,
                 const int           errcode,
                 const char          *desc )
{
  (void) fprintf(stderr, "  #%03d %s (%d) in %s:\n", (int)n, file, (int)line, subr);
  (void) fprintf(stderr, "     Error class = %-9s\n", ErrorClasses[errclass]);
  (void) fprintf(stderr, "     Error code  = %d", errcode);

  switch (errcode) {
  case ITIO_ERR_FILEID:
    (void) fprintf(stderr, ": Standard internal error: Invalid fileID");   break;
  case ITIO_ERR_OBJECTID:
    (void) fprintf(stderr, ": Standard internal error: Invalid objectID"); break;
  case ITIO_ERR_PARENTID:
    (void) fprintf(stderr, ": Standard internal error: Invalid parentID"); break;
  case ITIO_ERR_INDEX:
    (void) fprintf(stderr, ": Standard internal error: Invalid index");    break;
  case ITIO_ERR_DATATYPE:
    (void) fprintf(stderr, ": Standard internal error: Invalid datatype"); break;
  case ITIO_ERR_DIMS:
    (void) fprintf(stderr, ": Standard internal error: Invalid dims");     break;
  case ITIO_ERR_ARG:
    (void) fprintf(stderr, ": Standard internal error: Invalid arg");      break;
  case ITIO_ERR_CLASS:
    (void) fprintf(stderr, ": Standard internal error: Invalid class");    break;
  case ITIO_ERR_CREATE:
    (void) fprintf(stderr, ": Standard internal error: Create failed");    break;
  case ITIO_ERR_OPEN:
    (void) fprintf(stderr, ": Standard internal error: Open failed");      break;
  case ITIO_ERR_CLOSE:
    (void) fprintf(stderr, ": Standard internal error: Close failed");     break;
  case ITIO_ERR_MISMATCH:
    (void) fprintf(stderr, ": Standard internal error: Mismatch");         break;
    break;
  default:
    break;
  }

  (void) fprintf(stderr, "\n     %s\n", desc);


  return;
}




/***************************************************************************************************
 *
 * xTIO_ClearErrors
 *
 **************************************************************************************************/
static void
xTIO_ClearErrors( void )
{
  struct xError_t *errstore;

  int foundHDF5 = FALSE;



  while ( iTIO_StackPop(error_stack, (void **)&errstore) == 0 ) {

    foundHDF5 = (errstore->errclass = ERR_HDF5);

    TIOfree (errstore);
    errstore = NULL;
  }

  TIOfree (error_stack);
  error_stack = NULL;


#ifdef _TYPHIO_HDF5
  if (foundHDF5) {
    (void) iH5_ClearErrors();
  }
#endif


  return;
}




/***************************************************************************************************
 *
 * TIO_Set_Verbose
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Verbose( const TIO_Verbose_t mode )
{
  const char *subname = "TIO_Set_Verbose()";

  int irc;


  irc = xTIO_Getenv();
  TIOassert(irc < 0, ERR_INT, "Failed to get TyphonIO environment vars", TIO_ERR_INT);


  if ( (mode < TIO_VERBOSE_OFF) || (mode > TIO_VERBOSE_MASTER) ) {
    TIOreturn(ERR_USER, "Invalid verbose mode", TIO_ERR_INVALID_ARG);
  }
  Verbose     = (mode != TIO_VERBOSE_OFF);
  VerboseMode = mode;


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_Verbose
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Verbose( TIO_Verbose_t *mode )
{
  const char *subname = "TIO_Get_Verbose()";


  int irc;

  irc = xTIO_Getenv();
  TIOassert(irc < 0, ERR_INT, "Failed to get TyphonIO environment vars", TIO_ERR_INT);


  *mode = VerboseMode;


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * iTIO_Verbose
 *
 **************************************************************************************************/
void
iTIO_Verbose( const char subname[],
              const int  rank,
              const char *fmt,
              ... )
{
  va_list arg_ptr;

  static char outbuf[1024];
  char        *buf;
  const int   outlen = 65;
  char        word[65], line[65];
  int         i, w, l;


  if (!Verbose) {
    return;
  }

  va_start(arg_ptr, fmt);

  i = vsnprintf(outbuf, 1024, fmt, arg_ptr);

  if (i > 1023) {
    buf = TIOmalloc((size_t)(i+1));
    vsnprintf(buf, sizeof(buf), fmt, arg_ptr);
  }
  else {
    buf = outbuf;
  }

  va_end(arg_ptr);


  if ((TIO_VERBOSE_MASTER == VerboseMode) && (rank > 0)) {
    return;
  }

  if (rank >= 0) {
    (void) fprintf(stdout, "TIO Verbose: %s - process %d\n", subname, rank);
  }
  else {
    (void) fprintf(stdout, "TIO Verbose: %s\n", subname);
  }

  i = 0;
  w = 0;
  l = 0;

  (void) strcpy(line, "");

  while (TRUE) {

    switch (buf[i]) {

    case '\n':
      word[w] = '\0';

      if ((l+w) > outlen) {
        fprintf(stdout, "             %s\n", line);
        (void) strcpy(line, word);
      }
      else {
        (void) strcat(line, word);
      }
      (void) fprintf(stdout, "             %s\n", line);
      (void) strcpy(line, "");
      l = 0;
      w = 0;
      break;

    case ' ':
      word[w] = '\0';

      if ((l+w) > outlen) {
        (void) fprintf(stdout, "             %s\n", line);
        (void) strcpy(line, word);
        (void) strcat(line, " ");
        l = (w + 1);
      }
      else {
        (void) strcat(line, word);
        (void) strcat(line, " ");
        l += (w + 1);
      }

      w = 0;
      break;

    case '\0':
      word[w] = '\0';

      if ((l+w) > outlen) {
        (void) fprintf(stdout, "             %s\n", line);
        (void) strcpy(line, word);
      }
      else {
        (void) strcat(line, word);
      }
      (void) fprintf(stdout, "             %s\n", line);
      w = -1;
      break;

    default:
      word[w] = buf[i];
      ++w;
      break;
    }

    if (w < 0) {
      break;
    }

    ++i;
  }

  (void) fprintf(stdout, "             ---\n");


  if (buf != outbuf) {
    TIOfree (buf); buf = NULL;
  }

  return;
}



/*
 * EOF
 */

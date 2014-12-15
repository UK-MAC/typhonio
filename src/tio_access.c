/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"
#include "tio_register_int.h"

#include "tio_access.h"
#include "tio_state.h"
#include "tio_mesh.h"
#include "tio_material.h"
#include "tio_quant.h"
#include "tio_vargroup.h"
#include "tio_variable.h"

#include <unistd.h>

#ifdef _TYPHIO_HDF5
#include "tio_hdf5_int.h"
#endif



static TIO_t cTIO_HandleAttr ( const char *subname,
                               struct iFile_t **tfile, const TIO_File_t fileID,
                               const char *attr, const char *name, char *val);

static int xTIO_GetFileInfo  ( const char     subname[],
                               struct iFile_t *tfile,
                               char           filename[],
                               char           codename[],
                               char           version[],
                               char           date[],
                               char           title[] );

static int xTIO_GetFiletype  ( const char filename[], iFiletype_t *filetype );
static int xTIO_SetFiletype  ( struct iFile_t *tfile );
static int xTIO_SetFileFuncs ( struct iFile_t *tfile );

static int xTIO_WriteVersion ( struct iFile_t *tfile, const TIO_File_t fileID );

static int xTIO_ReadVersion  ( const char        filename[],
                               const TIO_File_t  fileID,
                               const iFiletype_t filetype,
                               int *maj, int *min, int *rel );




/***************************************************************************************************
 *
 * xTIO_SetInfo
 *
 **************************************************************************************************/
static MPI_Info xTIO_SetMPIInfo( MPI_Info info )
{
#ifdef _TYPHIO_PARALLEL
#ifdef _TYPHIO_USE_MPIINFO_HINTS

  int mpierr;
  MPI_Info linfo;

  if (info == MPI_INFO_NULL) {

    /* create our own communicator and use */

    MPI_Info_create(&linfo);

    /* Set hints to switch off Data Sieving */
    mpierr = MPI_Info_set( linfo, "romio_ds_write", "disable" );
    mpierr = MPI_Info_set( linfo, "romio_ds_read",  "disable" );

    /* Set hints for collective buffering */
    mpierr = MPI_Info_set( linfo, "romio_cb_write", "enable"  );
    mpierr = MPI_Info_set( linfo, "romio_cb_read",  "enable"  );
    /*
    mpierr = MPI_Info_set( linfo, "collective_buffering", "true" );
    mpierr = MPI_Info_set( linfo, "cb_block_size", "1048576" );
    mpierr = MPI_Info_set( linfo, "cb_buffer_size", "8388608" );
    
    mpierr = MPI_Info_set( linfo, "striping_unit",  "4194304" );
    mpierr = MPI_Info_set( linfo, "striping_size",  "16" );

    mpierr = MPI_Info_set( linfo, "cb_buffer_size", "8388608" );
    mpierr = MPI_Info_set( linfo, "cb_node_size",   "96"      );
    */
  }
  else {

    /* Just duplicate the given one and just use whatever is in it */

    mpierr = MPI_Info_dup( info, &linfo );

    /* In future... could inquire as to what is already set, and if none of the above hints are
       already in there, whack them in.  */
  }

  return (linfo);


#endif
#endif

  return (info);    /* default return used when not in preprocessed code */
}

static int xTIO_FreeMPIInfo( MPI_Info *info )
{
#ifdef _TYPHIO_PARALLEL
#ifdef _TYPHIO_USE_MPIINFO_HINTS

  /* Need to free up the created or duplicated info */
  int mpierr = MPI_Info_free(info);

#endif
#endif

  /* If not preprocessed to use hints, no need to do anything */

  return (0);
}



/***************************************************************************************************
 *
 * TIO_CheckFile
 *
 **************************************************************************************************/
TIO_t
TIO_CheckFile( const char filename[] )
{
  const char *subname = "TIO_CheckFile()";

  iFiletype_t filetype;

  int irc;


  TIObegin(subname,1);

  irc = access(filename, F_OK);
  if (irc < 0) {
    (void) iTIO_Verbose(subname, -1, "file: %s is not accessable (may not exist)", filename);
    TIOreturn (ERR_USER, "Unable to obtain filetype - filetype is unsupported", TIO_ERR_FILE_NOT_EXIST);
  }

  irc = xTIO_GetFiletype(filename, &filetype);
  switch (irc) {
  case 0:
    /* ok */
    break;
  case ITIO_ERR_FILETYPE:
    TIOreturn (ERR_USER, "Unable to obtain filetype - filetype is unsupported", TIO_ERR_UNSUPPORTED_FILETYPE);
    break;
  default:
    (void) iTIO_Verbose(subname, -1, "file: %s is not readable (may not exist)", filename);
    TIOreturn (ERR_USER, "Unable to obtain filetype - file may not exist", TIO_FAIL);
    break;
  }


  irc = xTIO_ReadVersion(filename, TIO_NULL, filetype, NULL, NULL, NULL);
  switch (irc) {
  case 0:
    /* No error */
    break;
  case ITIO_ERR_MISMATCH:
    (void) iTIO_Verbose(subname, -1, "file: %s is not TyphonIO v1.x format", filename);
    TIOreturn (ERR_USER, "File is not a TyphonIO v1.x file", TIO_ERR_VERSION_MISMATCH);
    break;
  default:
    (void) iTIO_Verbose(subname, -1, "file: %s is not TyphonIO v1.x format", filename);
    TIOreturn (ERR_USER, "File is not a TyphonIO file", TIO_FAIL);
    break;
  }

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_CheckObject
 *
 **************************************************************************************************/
TIO_t
TIO_CheckObject( const TIO_File_t   fileID,
                 const TIO_Object_t parentID,
                 const char         name[] )
{
  const char *subname = "TIO_CheckObject()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyCheckObject(subname, fileID, parentID, name);

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Get_LibVersion
 *
 **************************************************************************************************/
TIO_t
TIO_Get_LibVersion( int *maj, int *min, int *rel )
{
  const char *subname = "TIO_Get_LibVersion()";


  TIObegin(subname,1);

  TIOset(maj, TIO_VERSION_MAJ);
  TIOset(min, TIO_VERSION_MIN);
  TIOset(rel, TIO_VERSION_REL);

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_FileVersion
 *
 **************************************************************************************************/
TIO_t
TIO_Get_FileVersion( const char filename[],
                     int        *maj,
                     int        *min,
                     int        *rel )
{
  const char *subname = "TIO_Get_FileVersion()";

  iFiletype_t filetype;

  int irc;


  TIObegin(subname,1);

  irc = xTIO_GetFiletype(filename, &filetype);
  switch (irc) {
  case 0:
    /* ok */
    break;
  case ITIO_ERR_FILETYPE:
    TIOreturn (ERR_USER, "Unable to obtain filetype - filetype is unsupported", TIO_ERR_UNSUPPORTED_FILETYPE);
    break;
  default:
    TIOreturn (ERR_USER, "Unable to obtain filetype - file may not exist", TIO_FAIL);
    break;
  }


  irc = xTIO_ReadVersion(filename, TIO_NULL, filetype, maj, min, rel);
  switch (irc) {
  case 0:
    /* No error */
    break;
  case ITIO_ERR_MISMATCH:
    TIOend(subname,1);
    return (TIO_ERR_VERSION_MISMATCH);
    break;
  default:
    TIOreturn (ERR_USER, "File is not a TyphonIO file", TIO_FAIL);
    break;
  }

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_IsFileOpen
 *
 **************************************************************************************************/
TIO_t
TIO_IsFileOpen( const char filename[] )
{

  const char *subname = "TIO_IsFileOpen()";

  int irc;


  TIObegin(subname,1);

  irc = iTIO_GetFileInstances(TIO_NULL, filename, NULL);

  TIOend(subname,1);

  if (irc > 0) {
    return (TIO_SUCCESS);
  }
  else {
    return (TIO_FAIL);
  }
}




/***************************************************************************************************
 *
 * TIO_IsObjectOpen
 *
 **************************************************************************************************/
TIO_t
TIO_IsObjectOpen(  const TIO_File_t   fileID,
                   const TIO_Object_t parentID,
                   const char         name[]  )
{
  const char *subname = "TIO_IsObjectOpen()";

  TIO_Size_t lnc, ii;
  TIO_Object_t *cID;
  char         *lname;
  int found;
  int irc;


  TIObegin(subname,1);

  irc = iTIO_GetChildren(fileID, parentID, &lnc, NULL);

  found = 0;

  if (lnc > 0) {

    cID = NULL;
    cID = (TIO_Object_t *)TIOmalloc(lnc * sizeof(TIO_Object_t));
    irc = iTIO_GetChildren(fileID, parentID, &lnc, cID);

    for (ii=0; ii<lnc; ++ii) {
      irc = iTIO_HierarchyGetObjectName(fileID, cID[ii], &lname);
      found = (strcmp(lname, name) == 0);
      if (found) break;
    }

    TIOfree(cID); cID = NULL;
  }

  TIOend(subname,1);

  if (found) {
    return (TIO_SUCCESS);
  }
  else {
    return (TIO_FAIL);
  }
}




/***************************************************************************************************
 *
 * TIO_Create
 *
 **************************************************************************************************/
TIO_t
TIO_Create( const char         filename[],
            TIO_File_t         *fileID,
            const TIO_Access_t access,
            const char         codename[],
            const char         version[],
            const char         date[],
            const char         title[],
            const MPI_Comm     comm,
            const MPI_Info     info,
            const int          rank )
{
  const char *subname = "TIO_Create()";

  struct iFile_t *tfile;

  TIO_File_t lfID;
  int        irc;
  TIO_t      trc;


  TIObegin(subname,1);

  *fileID = TIO_NULL;

  /* -- Check inbound strings */
  irc = iTIO_ValidString(codename, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "codename"), TIO_ERR_INVALID_STRING);
  }
  irc = iTIO_ValidString(date, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "date"), TIO_ERR_INVALID_STRING);
  }
  irc = iTIO_ValidString(version, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "version"), TIO_ERR_INVALID_STRING);
  }
  irc = iTIO_ValidString(title, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "title"), TIO_ERR_INVALID_STRING);
  }

  tfile = (struct iFile_t *)TIOmalloc(sizeof(struct iFile_t));
  if (NULL == tfile) {
    TIOreturn(ERR_MEM, "Failed to malloc tfile", TIO_ERR_INT);
  }
  irc = iTIO_RegisterFile(&lfID, tfile);
  TIOassert(irc < 0, ERR_INT, "Failed to register file", TIO_ERR_INT);

  if (iTIO_strcpy(tfile->name, filename) == NULL) {
    (void) iTIO_PutErr(ERR_USER, "Invalid filename");
    trc = TIO_ERR_INVALID_NAME;
    goto cleandie;
  }



  (void) iTIO_strcpy(tfile->codename, codename);
  (void) iTIO_strcpy(tfile->version,  version);
  (void) iTIO_strcpy(tfile->created,  date);
  (void) iTIO_strcpy(tfile->modified, date);
  (void) iTIO_strcpy(tfile->title,    title);

  tfile->vmaj      = TIO_VERSION_MAJ;
  tfile->vmin      = TIO_VERSION_MIN;
  tfile->vrel      = TIO_VERSION_REL;

  tfile->access    = access;
  tfile->writeable = TRUE;

  tfile->comm      = comm;

#ifdef _TYPHIO_PARALLEL
  tfile->info      = xTIO_SetMPIInfo(info);
  tfile->rank      = rank;   /* Assume to be correct - could legally be MPI_PROC_NULL */
#else
  tfile->info      = info;
  tfile->rank      = 0;
#endif

  /* -- Initialize TyphonIO file */

  irc = xTIO_SetFiletype(tfile);
  if (irc < 0) {
    (void) iTIO_PutErr(ERR_USER, "Unable to set filetype");
    trc = TIO_ERR_UNSUPPORTED_FILETYPE;
    goto cleandie;
  }

  irc = xTIO_SetFileFuncs(tfile);
  TIOassert(irc < 0, ERR_INT, "Failed to set file functions", TIO_ERR_INT);

  irc = iTIO_InitHierarchy();
  TIOassert(irc < 0, ERR_INT, "Failed to initialize hierarchy", TIO_ERR_INT);

  irc = tfile->Init();
  if (irc < 0) {
    if (ITIO_ERR_MISMATCH == irc) {
      (void) iTIO_PutErr(ERR_USER, "Underlying library mismatch");
      trc = TIO_ERR_LIBRARY_MISMATCH;
      goto cleandie;
    }
    else {
      TIOassert(TRUE, ERR_INT, "Failed to initialize for file", TIO_ERR_INT);
    }
  }


  trc = cTIO_HierarchyCreateFile( subname, lfID, filename, access,
                                  tfile->comm, tfile->info, tfile->rank, tfile );
  if (trc != TIO_SUCCESS) {
    (void) iTIO_PutErr(ERR_USER, "File creation error");
    /* trc already set from above call */
    goto cleandie;
  }


  /* Write the version info to the file */
  irc = xTIO_WriteVersion(tfile, lfID);
  TIOassert(irc < 0, ERR_INT, "Failed to write version", TIO_ERR_INT);

  /* -- Write the file info attributes */
  irc = 0;
  irc += tfile->WriteAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "codename", TIO_STRING,
                           (void *)tfile->codename );
  irc += tfile->WriteAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "version",  TIO_STRING,
                           (void *)tfile->version  );
  irc += tfile->WriteAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "created",  TIO_STRING,
                           (void *)tfile->created  );
  irc += tfile->WriteAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "modified", TIO_STRING,
                           (void *)tfile->modified );
  irc += tfile->WriteAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "title",    TIO_STRING,
                           (void *)tfile->title    );
  TIOassert(irc != 0, ERR_INT, "Failed to write file attributes", TIO_ERR_INT);


  *fileID = lfID ;

  TIOend(subname,1);

  return (TIO_SUCCESS);


 cleandie:
  irc = iTIO_DeregisterFile(lfID);
  TIOassert(irc < 0, ERR_INT, "Failed to deregister file", TIO_ERR_INT);
  TIOfree (tfile); tfile = NULL;
  TIOreturn(iTIO_GetErrClass(), iTIO_GetErrStr(), trc);
}





/***************************************************************************************************
 *
 * TIO_Open
 *
 **************************************************************************************************/
TIO_t
TIO_Open( const char         filename[],
          TIO_File_t         *fileID,
          const TIO_Access_t access,
          char               codename[],
          char               version[],
          char               date[],
          char               title[],
          const MPI_Comm     comm,
          const MPI_Info     info,
          const int          rank )
{
  const char *subname = "TIO_Open()";

  struct iFile_t *tfile;

  TIO_File_t lfID;
  int        irc, iwrite;
  TIO_t      trc;


  TIObegin(subname,1);

  *fileID = TIO_NULL;


  tfile = (struct iFile_t *)TIOmalloc(sizeof(struct iFile_t));
  if (NULL == tfile) {
    TIOreturn(ERR_MEM, "Failed to malloc tfile", TIO_ERR_INT);
  }

  /* -- Register the file  - this returns TyphonIO file ID*/
  irc = iTIO_RegisterFile(&lfID, tfile);
  TIOassert(irc < 0, ERR_INT, "Failed to register file", TIO_ERR_INT);


  (void) iTIO_strcpy(tfile->name, filename);

#ifndef NDEBUG
  tfile->vmaj      = 0;
  tfile->vmin      = 0;
  tfile->vrel      = 0;
#endif

  tfile->access    = access;
  tfile->writeable = (TIO_ACC_READWRITE == access || TIO_ACC_READWRITESTRICT == access );

  tfile->comm      = comm;

#ifdef _TYPHIO_PARALLEL
  tfile->info      = xTIO_SetMPIInfo(info);
  tfile->rank      = rank;
#else
  tfile->info      = info;
  tfile->rank      = 0;
#endif


  /* -- If writing to the file, check it hasn't already been opened */
  /*    Files can be opened multiple times for read-only */
  irc = iTIO_GetFileInstances(TIO_NULL, filename, &iwrite);
  if ((irc > 1) && iwrite) {        /* > 1 as one instance will be the one registered above */
    (void) iTIO_PutErr(ERR_USER, "File is already open - cannot open again for writing");
    trc = TIO_ERR_FILE_NOT_WRITEABLE;
    goto cleandie;
  }


  /* -- Initialize TyphonIO file */
  irc = xTIO_GetFiletype(filename, &tfile->filetype);
  if (irc < 0) {
    if (ITIO_ERR_FILETYPE == irc) {
      (void) iTIO_PutErr(ERR_USER, "Unable to obtain filetype - filetype unsupported");
      trc = TIO_ERR_UNSUPPORTED_FILETYPE;
    }
    else {
      (void) iTIO_PutErr(ERR_USER, "Unable to obtain filetype - file may not exist");
      trc = TIO_ERR_FILE_OPEN;
    }
    (void) iTIO_Verbose(subname, -1, "file: %s is not readable (may not exist)", filename);
    goto cleandie;
  }


  irc = xTIO_SetFiletype(tfile);
  if (irc < 0) {
    (void) iTIO_PutErr(ERR_USER, "Unable to find file type");
    trc = TIO_ERR_UNSUPPORTED_FILETYPE;
    goto cleandie;
  }

  irc = xTIO_SetFileFuncs(tfile);
  TIOassert(irc < 0, ERR_INT, "Failed to set file functions", TIO_ERR_INT);

  irc = iTIO_InitHierarchy();
  TIOassert(irc < 0, ERR_INT, "Failed to initialize hierarchy", TIO_ERR_INT);


  irc = tfile->Init();
  if (irc < 0) {
    if (ITIO_ERR_MISMATCH == irc) {
      (void) iTIO_PutErr(ERR_USER, "Underlying library mismatch");
      trc = TIO_ERR_LIBRARY_MISMATCH;
      goto cleandie;
    }
    else {
      TIOassert(TRUE, ERR_INT, "Failed to initialize for file", TIO_ERR_INT);
    }
  }


  trc = cTIO_HierarchyOpenFile( subname, lfID, filename, access,
                                tfile->comm, tfile->info, tfile->rank, tfile );
  if (trc != TIO_SUCCESS) {
    (void) iTIO_PutErr(ERR_USER, "File open error");
    /* trc already set from above call - above call checks access value */
    goto cleandie;
  }


  /* -- Check the file is a TyphonIO file */
  irc = xTIO_ReadVersion(NULL, lfID, tfile->filetype, &tfile->vmaj, &tfile->vmin, &tfile->vrel);
  if (irc <0) {

    if (ITIO_ERR_MISMATCH == irc) {
      trc = TIO_ERR_VERSION_MISMATCH;
    }
    else {
      trc = TIO_ERR_FILE_OPEN;
    }

    irc = tfile->CloseFile( lfID );
    irc = tfile->Kill();

    (void) iTIO_Verbose(subname, -1, "file: %s is not TyphonIO v1.x format", filename);

    /* Check that this TyphonIO library can read the file and fail if not  */
    /* ie. check that the version of the file matches library              */

    /* Above read version will fail for v0.1 and v0.2 files */

    (void) iTIO_PutErr(ERR_USER, "File is not TyphonIO v1.x format");
    /* trc set above */
    goto cleandie;
  }


  /* -- Read or write the file info attributes */

  irc = tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "created",  TIO_STRING,
                         (void *)tfile->created);
  TIOassert(irc < 0, ERR_INT, "Failed to read created attribute", TIO_ERR_INT);


  switch (access) {

  case TIO_ACC_READONLY:

    irc = 0;
    irc += tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "codename", TIO_STRING,
                            (void *)tfile->codename);
    irc += tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "version",  TIO_STRING,
                            (void *)tfile->version);
    irc += tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "modified", TIO_STRING,
                            (void *)tfile->modified);
    irc += tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "title",    TIO_STRING,
                            (void *)tfile->title);
    TIOassert(irc != 0, ERR_INT, "Failed to read file attributes", TIO_ERR_INT);

    irc = xTIO_GetFileInfo(subname, tfile, NULL, codename, version, date, title);
    TIOassert(irc < 0, ERR_INT, "Failed to get file info", TIO_ERR_INT);

    break;

  case TIO_ACC_READWRITESTRICT:

    irc = 0;
    irc += tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "codename", TIO_STRING,
                            (void *)tfile->codename);
    irc += tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "version",  TIO_STRING,
                            (void *)tfile->version);
    irc += tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "modified", TIO_STRING,
                            (void *)tfile->modified);
    irc += tfile->ReadAttr( lfID, TIO_NULL, TIO_NULL, CLASS_FILE, "title",    TIO_STRING,
                            (void *)tfile->title);
    TIOassert(irc != 0, ERR_INT, "Failed to read file attributes", TIO_ERR_INT);

    break;

  case TIO_ACC_READWRITE:
    {
      TIO_t trc;

      /* -- Replace file info attributes if given */
      /*    Returns TIO_ERR_INVALID_STRING from cTIO_HandleAttr() if attribute value is invalid */

      trc = cTIO_HandleAttr( subname, &tfile, lfID, codename, "codename", tfile->codename );
      if (trc != TIO_SUCCESS) {
        goto cleandie;
      }
      trc = cTIO_HandleAttr( subname, &tfile, lfID, date,     "modified", tfile->modified );
      if (trc != TIO_SUCCESS) {
        goto cleandie;
      }
      irc = cTIO_HandleAttr( subname, &tfile, lfID, version,  "version",  tfile->version  );
      if (trc != TIO_SUCCESS) {
        goto cleandie;
      }
      irc = cTIO_HandleAttr( subname, &tfile, lfID, title,    "title",    tfile->title    );
      if (trc != TIO_SUCCESS) {
        goto cleandie;
      }
    }

    break;

  default:
    break;
  }


  *fileID = lfID;

  TIOend(subname,1);

  return (TIO_SUCCESS);


 cleandie:
  irc = iTIO_DeregisterFile(lfID);
  TIOassert(irc < 0, ERR_INT, "Failed to deregister file", TIO_ERR_INT);
  TIOfree (tfile); tfile = NULL;
  TIOreturn(iTIO_GetErrClass(), iTIO_GetErrStr(), trc);
}




TIO_t cTIO_HandleAttr( const char *subname,
                       struct iFile_t **tfile, const TIO_File_t fileID,
                       const char *attr, const char *name, char *val)
{
  int strc, irc;
  struct iFile_t *tf = *tfile;


  if (attr != NULL) {
    strc = iTIO_ValidString(attr, FALSE);
    if (strc < 0) {
      (void) iTIO_PutErr(ERR_USER, iTIO_GetStringError(strc, name));
      return (TIO_ERR_INVALID_STRING);
    }

    (void) iTIO_strcpy(val, attr);

    irc = tf->DelAttr( fileID, TIO_NULL, TIO_NULL, CLASS_FILE, name );
    TIOassert(irc < 0, ERR_INT, "Failed to delete pre-existing attribute", TIO_ERR_INT);

    irc = tf->WriteAttr( fileID, TIO_NULL, TIO_NULL, CLASS_FILE, name, TIO_STRING, (void *)val );
    TIOassert(irc < 0, ERR_INT, "Failed to write file attribute", TIO_ERR_INT);
  }
  else {

    irc = tf->ReadAttr( fileID, TIO_NULL, TIO_NULL, CLASS_FILE, name, TIO_STRING, (void *)val);
    TIOassert(irc != 0, ERR_INT, "Failed to read file attribute", TIO_ERR_INT);

  }

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_File_Info
 *
 **************************************************************************************************/
TIO_t
TIO_Get_File_Info( const TIO_File_t fileID,
                   char             filename[],
                   char             codename[],
                   char             version[],
                   char             date[],
                   char             title[] )
{
  const char *subname = "TIO_Get_File_Info()";

  struct iFile_t *tfile;

  int irc;



  TIObegin(subname,1);

  irc = iTIO_GetFile(fileID, &tfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid fileID", TIO_ERR_INVALID_FILEID);
  }
  irc = xTIO_GetFileInfo(subname, tfile, filename, codename, version, date, title);
  TIOassert(irc < 0, ERR_INT, "Failed to get file info", TIO_ERR_INT);

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




static int
xTIO_GetFileInfo( const char     subname[],
                  struct iFile_t *tfile,
                  char           filename[],
                  char           codename[],
                  char           version[],
                  char           date[],
                  char           title[] )
{
  if (filename != NULL) {
    (void) iTIO_strcpy(filename, tfile->name);
  }
  if (codename != NULL) {
    (void) iTIO_strcpy(codename, tfile->codename);
  }
  if (version  != NULL) {
    (void) iTIO_strcpy(version,  tfile->version);
  }
  if (date     != NULL) {
    (void) iTIO_strcpy(date,     tfile->modified);
  }
  if (title    != NULL) {
    (void) iTIO_strcpy(title,    tfile->title);
  }

  return (0);
}




/***************************************************************************************************
 *
 * TIO_Close
 *
 **************************************************************************************************/
TIO_t TIO_Close( const TIO_File_t fileID )
{
  const char *subname = "TIO_Close()";

  struct iFile_t *tfile;

  int   irc;
  TIO_t trc;


  TIObegin(subname,1);


  trc = cTIO_HierarchyPreClose( subname, fileID, NULL, TIO_NULL, CLASS_FILE, &tfile, NULL);

  irc = xTIO_FreeMPIInfo(&tfile->info);

  trc = cTIO_HierarchyCloseFile( subname, fileID);

  irc = iTIO_KillHierarchy();
  TIOassert(irc < 0, ERR_INT, "Failed to kill hierarchy", TIO_ERR_INT);

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Flush
 *
 **************************************************************************************************/
TIO_t
TIO_Flush( const TIO_File_t fileID )
{
  const char *subname = "TIO_Flush()";


  struct iFile_t *tfile;

  TIO_t trc;
  int   irc;


  TIObegin(subname,1);

  irc = iTIO_GetFile(fileID, &tfile);
  if (irc < 0) {
    if (ITIO_ERR_FILEID == irc) {
      TIOreturn(ERR_USER, "Invalid fileID", TIO_ERR_INVALID_FILEID);
    }
    else {
      TIOassert(TRUE, ERR_INT, "Failed to get file", TIO_ERR_INT);
    }
  }


  if (!tfile->writeable) {
    TIOreturn(ERR_USER, "Cannot flush read-only file", TIO_ERR_FILE_NOT_WRITEABLE);
  }

  trc = cTIO_HierarchyFlushFile(subname, fileID);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }


  irc = tfile->FlushFile(fileID);
  TIOassert(irc < 0, ERR_INT, "Failed to flush file", TIO_ERR_FILE_FLUSH);


  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * xTIO_GetFiletype
 *
 **************************************************************************************************/
static int
xTIO_GetFiletype( const char filename[], iFiletype_t *filetype )
{
  const char *subname = "xTIO_GetFiletype()";

  int irc;


  /* Will most likely get more complicated in future!!! */
  /* Will need to probe the file to see if it is HDF5 / pnetcdf? / other? */

#ifdef _TYPHIO_HDF5

  irc = iH5_CheckFile( filename );

#else

  irc = ITIO_ERR_FILETYPE;
  *filetype = FILETYPE_NULL;
  return (irc);

#endif

  if (0 == irc) {
    *filetype = FILETYPE_HDF5;
    irc = 0;
  }
  else {
    *filetype = FILETYPE_NULL;
    irc = -1;
  }


  return (irc);
}




/***************************************************************************************************
 *
 * xTIO_SetFiletype
 *
 **************************************************************************************************/
static int
xTIO_SetFiletype( struct iFile_t *tfile )
{
  const char *subname = "xTIO_SetFiletype()";

  int irc;


  /* Will most likely get more complicated in future!!! */
  /* Will need to probe the file to see if it is HDF5 / pnetcdf? / other? */

#ifdef _TYPHIO_HDF5

  tfile->filetype = FILETYPE_HDF5;
  irc = 0;

#else

  irc = ITIO_ERR_FILETYPE;

#endif


  return (irc);
}




/***************************************************************************************************
 *
 * xTIO_SetFileFuncs
 *
 **************************************************************************************************/
static int
xTIO_SetFileFuncs( struct iFile_t *tfile )
{
  const char *subname = "xTIO_SetFileFuncs()";


  switch (tfile->filetype) {

  case FILETYPE_HDF5:

#ifdef _TYPHIO_HDF5

    tfile->Init                      = iH5_Init;
    tfile->Kill                      = iH5_Kill;

    tfile->InitRef                   = iH5_InitRef;
    tfile->AddRef                    = iH5_AddRef;
    tfile->RemoveRef                 = iH5_RemoveRef;
    tfile->FinalizeRef               = iH5_FinalizeRef;
    tfile->ReadRef                   = iH5_ReadRef;

    tfile->CreateFile                = iH5_CreateFile;
    tfile->OpenFile                  = iH5_OpenFile;
    tfile->CloseFile                 = iH5_CloseFile;
    tfile->FlushFile                 = iH5_FlushFile;

    tfile->CreateObject              = iH5_CreateObject;
    tfile->OpenObject                = iH5_OpenObject;
    tfile->CloseObject               = iH5_CloseObject;

    tfile->CreateDataset             = iH5_CreateDataset;
    tfile->OpenDataset               = iH5_OpenDataset;
    tfile->CloseDataset              = iH5_CloseDataset;
    tfile->WriteDataset              = iH5_WriteDataset;
    tfile->ReadDataset               = iH5_ReadDataset;
    tfile->ReadDatasetSlab           = iH5_ReadDatasetSlab;

    tfile->CreateChunkedDataset      = iH5_CreateChunkedDataset;
    tfile->OpenChunkedDataset        = iH5_OpenChunkedDataset;
    tfile->CloseChunkedDataset       = iH5_CloseChunkedDataset;
    tfile->WriteChunkedDataset       = iH5_WriteChunkedDataset;
    /* tfile->WriteChunkedDatasetAll    = iH5_WriteChunkedDatasetAll; */
    tfile->ReadChunkedDataset        = iH5_ReadChunkedDataset;
    tfile->ReadChunkedDatasetSection = iH5_ReadChunkedDatasetSection;
    tfile->ReadChunkedDatasetAll     = iH5_ReadChunkedDatasetAll;

    tfile->SetSection                = iH5_SetSection;
    tfile->KillSection               = iH5_KillSection;

    tfile->WriteAttr                 = iH5_WriteAttr;
    tfile->ReadAttr                  = iH5_ReadAttr;
    tfile->DelAttr                   = iH5_DelAttr;

    tfile->ConvBuf                   = iH5_ConvBuf;

#else

    TIOreturn(ERR_INT, "HDF5 file format not available", ITIO_ERR_FILETYPE);

#endif

    break;

  default:
    TIOreturn(ERR_INT, "Invalid filetype", ITIO_ERR_ARG);
    break;
  }


  return (0);
}





/***************************************************************************************************
 *
 * xTIO_WriteVersion
 *
 **************************************************************************************************/
static int
xTIO_WriteVersion( struct iFile_t *tfile, const TIO_File_t fileID )
{
  const char *subname = "xTIO_WriteVersion()";

  int irc;


  switch (tfile->filetype) {

  case FILETYPE_HDF5:
#ifdef _TYPHIO_HDF5
    irc = iH5_WriteVersion(fileID, tfile->name, tfile->vmaj, tfile->vmin, tfile->vrel);
    TIOassert(irc != 0, ERR_INT, "Failed to write version", -1);
#else
    TIOreturn(ERR_INT, "HDF5 file format not available", -10);
#endif
    break;

  default:
    TIOreturn(ERR_INT, "Invalid filetype", -20);
    break;

  }


  return (0);
}



/***************************************************************************************************
 *
 * xTIO_ReadVersion
 *
 **************************************************************************************************/
static int
xTIO_ReadVersion( const char        filename[],
                  const TIO_File_t  fileID,
                  const iFiletype_t filetype,
                  int *maj, int *min, int *rel )
{
  const char *subname = "xTIO_ReadVersion()";

  int irc;


  switch (filetype) {

  case FILETYPE_HDF5:
#ifdef _TYPHIO_HDF5
    irc = iH5_ReadVersion(fileID, filename, maj, min, rel);
#else
    TIOreturn(ERR_INT, "HDF5 file format not available", -10);
#endif
    break;

  default:
    TIOreturn(ERR_INT, "Invalid filetype", -20);
    break;

  }


  return (irc);
}



/*
 * EOF
 */

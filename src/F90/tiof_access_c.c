/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tiof_fkit.h"
#include "typhonio.h"


/***************************************************************************************************
 *
 *   tio_get_libversion_c
 *
 **************************************************************************************************/
void
F2C(tio_get_libversion_c,TIO_GET_LIBVERSION_C)
     ( int_f  *maj,
       int_f  *min,
       int_f  *rel,
       TIO_tf *err )
{
  int l_maj, l_min, l_rel;
  
  
  *err = c2f_TIO( TIO_Get_LibVersion( &l_maj, &l_min, &l_rel ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *maj = (int_f)l_maj;
  *min = (int_f)l_min;
  *rel = (int_f)l_rel;
  
  return;
}




/***************************************************************************************************
 *
 *   tio_getfileversion_c
 *
 **************************************************************************************************/
void
F2C(tio_get_fileversion_c,TIO_GET_FILEVERSION_C)
     ( const char  *filename, const int_f *filename_len,
       int_f       *maj,
       int_f       *min,
       int_f       *rel,
       TIO_tf      *err )
{
  char l_filename[TIO_FILENAME_LEN];
  int  l_maj, l_min, l_rel;

  
  
  f2c_fname(filename, filename_len, l_filename);
  
  *err = c2f_TIO( TIO_Get_FileVersion( l_filename, &l_maj, &l_min, &l_rel ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *maj = (int_f)l_maj;
  *min = (int_f)l_min;
  *rel = (int_f)l_rel;
  
  return;
}




/***************************************************************************************************
 *
 *   tio_checkfile_c
 *
 **************************************************************************************************/
void
F2C(tio_checkfile_c,TIO_CHECKFILE_C)
     ( const char  *filename, const int_f *filename_len,
       TIO_tf      *err )
{
  char l_filename[TIO_FILENAME_LEN];
  
  
  f2c_fname(filename, filename_len, l_filename);
  
  *err = c2f_TIO( TIO_CheckFile( l_filename ) );
  
  return;
}




/***************************************************************************************************
 *
 *   tio_checkobject_c
 *
 **************************************************************************************************/
void
F2C(tio_checkobject_c,TIO_CHECKOBJECT_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *parentID,
       const char          *name, const int_f *name_len,
       TIO_tf              *err )
{
  TIO_Object_t l_parentID;
  char         l_name[TIO_STRLEN];
  
  
  if (*parentID != TIO_NULL_F) {
    l_parentID = f2c_object(*parentID);
  }
  else {
    l_parentID = TIO_NULL;
  }
  
  f2c_str(name, name_len, l_name);
  
  *err = c2f_TIO( TIO_CheckObject( f2c_file(*fileID), l_parentID, l_name ) );
  
  return;
}




/***************************************************************************************************
 *
 *   tio_isfileopen_c
 *
 **************************************************************************************************/
void
F2C(tio_isfileopen_c,TIO_ISFILEOPEN_C)
     ( const char  *filename, const int_f *filename_len,
       TIO_tf       *err )
{
  char l_filename[TIO_FILENAME_LEN];
  
  
  
  f2c_fname(filename, filename_len, l_filename);
  
  *err = c2f_TIO( TIO_IsFileOpen( l_filename ) );
  
  return;
}




/***************************************************************************************************
 *
 *   tio_isobjectopen_c
 *
 **************************************************************************************************/
void
F2C(tio_isobjectopen_c,TIO_ISOBJECTOPEN_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *parentID,
       const char          *name, const int_f *name_len,
       TIO_tf              *err )
{
  TIO_Object_t l_parentID;
  char         l_name[TIO_STRLEN];
  
  
  if (*parentID != TIO_NULL_F) {
    l_parentID = f2c_object(*parentID);
  }
  else {
    l_parentID = TIO_NULL;
  }
  
  
  f2c_str(name, name_len, l_name);
  
  *err = c2f_TIO( TIO_IsObjectOpen( f2c_file(*fileID), l_parentID, l_name ) );
  
  return;
}




/***************************************************************************************************
 *
 *   tio_create_c
 *
 **************************************************************************************************/
void
F2C(tio_create_c,TIO_CREATE_C)
     ( const char  *filename, const int_f *filename_len,
       TIO_File_tf *fileID,
       const int_f *access,
       const char  *codename, const int_f *codename_len,
       const char  *version,  const int_f *version_len,
       const char  *date,     const int_f *date_len,
       const char  *title,    const int_f *title_len,
       int_f       *comm,
       int_f       *info,
       int_f       *rank,
       TIO_tf      *err )
{
  TIO_File_t l_fileID;
  
  char l_filename[TIO_FILENAME_LEN];
  char l_codename[TIO_STRLEN];
  char l_version[TIO_STRLEN];
  char l_date[TIO_STRLEN];
  char l_title[TIO_STRLEN];
  
  
  f2c_fname(filename, filename_len, l_filename);
  f2c_str(codename, codename_len, l_codename);
  f2c_str(version,  version_len,  l_version);
  f2c_str(date,     date_len,     l_date);
  f2c_str(title,    title_len,    l_title);
  
  *err = c2f_TIO( TIO_Create( l_filename,
                              &l_fileID,
                              f2c_access(*access),
                              l_codename,
                              l_version,
                              l_date,
                              l_title,
                              f2c_MPIcomm(*comm),
                              f2c_MPIinfo(*info),
                              (int)*rank ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *fileID = c2f_file(l_fileID);
  
  return;
}




/***************************************************************************************************
 *
 *   tio_open_c
 *
 **************************************************************************************************/
void
F2C(tio_open_c,TIO_OPEN_C)
     ( const char  *filename, const int_f *filename_len,
       TIO_File_tf *fileID,
       const int_f *access,
       char        *codename, const int_f *codename_len,
       char        *version,  const int_f *version_len,
       char        *date,     const int_f *date_len,
       char        *title,    const int_f *title_len,
       int_f       *comm,
       int_f       *info,
       int_f       *rank,
       TIO_tf      *err )
{
  TIO_File_t l_fileID;
  
  char l_filename[TIO_FILENAME_LEN];
  char l_codename[TIO_STRLEN];
  char l_version[TIO_STRLEN];
  char l_date[TIO_STRLEN];
  char l_title[TIO_STRLEN];
  
  int_f flen=TIO_STRLEN_F, *flptr=&flen;
  
  
  f2c_fname(filename, filename_len, l_filename);
  
  if (*codename_len > 0) {
    f2c_str(codename, codename_len, l_codename);
    f2c_str(version,  version_len,  l_version);
    f2c_str(date,     date_len,     l_date);
    f2c_str(title,    title_len,    l_title);
  }
  
  *err = c2f_TIO( TIO_Open( l_filename,
                            &l_fileID,
                            f2c_access(*access),
                            l_codename,
                            l_version,
                            l_date,
                            l_title,
                            f2c_MPIcomm(*comm),
                            f2c_MPIinfo(*info),
                            (int)*rank ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  if (*codename_len < 0) {
    c2f_str(l_codename, flptr, codename);
    c2f_str(l_version,  flptr, version);
    c2f_str(l_date,     flptr, date);
    c2f_str(l_title,    flptr, title);
  }
  
  *fileID = c2f_file(l_fileID);
  
  return;
}




/***************************************************************************************************
 *
 *   tio_get_file_info_c
 *
 **************************************************************************************************/
void
F2C(tio_get_file_info_c,TIO_GET_FILE_INFO_C)
     ( const TIO_File_tf *fileID,
       char              *filename, const int_f *filename_len,
       char              *codename, const int_f *codename_len,
       char              *version,  const int_f *version_len,
       char              *date,     const int_f *date_len,
       char              *title,    const int_f *title_len,
       TIO_tf            *err )
{
  char l_filename[TIO_FILENAME_LEN];
  char l_codename[TIO_STRLEN];
  char l_version[TIO_STRLEN];
  char l_date[TIO_STRLEN];
  char l_title[TIO_STRLEN];
  
  
  
  *err = c2f_TIO( TIO_Get_File_Info( f2c_file(*fileID),
                                     l_filename,
                                     l_codename,
                                     l_version,
                                     l_date,
                                     l_title ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  c2f_fname(l_filename, filename_len, filename);
  c2f_str(l_codename, codename_len, codename);
  c2f_str(l_version,  version_len,  version);
  c2f_str(l_date,     date_len,     date);
  c2f_str(l_title,    title_len,    title);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_close_c
 *
 **************************************************************************************************/
void
F2C(tio_close_c,TIO_CLOSE_C)
     ( const TIO_File_tf *fileID,
       TIO_tf            *err )
{
  *err = c2f_TIO( TIO_Close( f2c_file(*fileID) ) );
  
  return;
}




/***************************************************************************************************
 *
 *   tio_flush_c
 *
 **************************************************************************************************/
void
F2C(tio_flush_c,TIO_FLUSH_C)
     ( const TIO_File_tf *fileID,
       TIO_tf            *err )
{
  *err = c2f_TIO( TIO_Flush( f2c_file(*fileID) ) );
  
  return;
}




/*
 * EOF
 */

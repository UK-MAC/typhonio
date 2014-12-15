/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tiof_fkit.h"
#include "typhonio.h"

static void xtioc_copyvarinfo( const char           *name,
                               const TIO_Data_t     datatype,
                               const TIO_Dims_t     ndims,
                               const TIO_Size_t     *dims,
                               const char           *units,
                               char                 *fname,  const int_f *fname_len,
                               int_f                *fdatatype,
                               int_f                *fndims,
                               TIO_Size_tf          *fdims,
                               char                 *funits, const int_f *funits_len );



/***************************************************************************************************
 *
 *   tio_list_variables_c
 *
 **************************************************************************************************/
void
F2C(tio_list_variables_c,TIO_LIST_VARIABLES_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *objectID,
       TIO_Size_tf         *nobj,
       char                *names,
       TIO_tf              *err )
{
  TIO_Object_t l_objectID;
  TIO_Size_t   l_nobj;
  char         *fnptr, ccpy[TIO_STRLEN];
  int          len=TIO_STRLEN, *lptr = &len;
  TIO_Size_t   i;
  
  
  
  if (*objectID != TIO_NULL_F) {
    l_objectID = f2c_object(*objectID);
  }
  else {
    l_objectID = TIO_NULL;
  }
  
  if (*nobj > 0) {
    
    *err = c2f_TIO( TIO_List_Variables( f2c_file(*fileID),
                                        l_objectID,
                                        &l_nobj,
                                        (char(*)[TIO_STRLEN])names ) );
    if (*err != TIO_SUCCESS_F) return;
    
    fnptr = names;
    for (i=0; i<l_nobj; ++i) {
      strcpy(ccpy, fnptr);
      c2f_str(ccpy, lptr, fnptr);
      fnptr += TIO_STRLEN_F;
    }
    
  }
  else {
    *err = c2f_TIO( TIO_List_Variables( f2c_file(*fileID),
                                        l_objectID,
                                        &l_nobj,
                                        NULL ) );
    if (*err != TIO_SUCCESS_F) return;
    
  }
  
  *nobj = c2f_size(l_nobj);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_create_variable_c
 *
 **************************************************************************************************/
void
F2C(tio_create_variable_c,TIO_CREATE_VARIABLE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *objectID,
       const char          *name,
       const int_f         *name_len,
       TIO_Object_tf       *varID,
       const int_f         *datatype,
       const int_f         *ndims,
       const TIO_Size_tf   *dims,
       const char          *units,
       const int_f         *units_len,
       TIO_tf              *err )
{
  TIO_Object_t l_objectID, l_varID;
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Size_t   l_dims[TIO_MAXDIMS];
  
  
  
  if (*objectID != TIO_NULL_F) {
    l_objectID = f2c_object(*objectID);
  }
  else {
    l_objectID = TIO_NULL;
  }
  
  f2c_str(name,  name_len, l_name);
  f2c_dims(dims, ndims, l_dims, 0);
  f2c_str(units, units_len, l_units);
  
  
  *err = c2f_TIO( TIO_Create_Variable( f2c_file(*fileID),
                                       l_objectID,
                                       l_name,
                                       &l_varID,
                                       f2c_datatype(*datatype),
                                       f2c_ndims(*ndims),
                                       l_dims,
                                       l_units ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *varID = c2f_object(l_varID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_open_variable_c
 *
 **************************************************************************************************/
void
F2C(tio_open_variable_c,TIO_OPEN_VARIABLE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *objectID,
       const TIO_Size_tf   *idx,
       char                *name,
       const int_f         *name_len,
       TIO_Object_tf       *varID,
       int_f               *datatype,
       int_f               *ndims,
       TIO_Size_tf         *dims,
       char                *units,
       const int_f         *units_len,
       TIO_tf              *err )
{
  TIO_Object_t l_objectID, l_varID;
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Data_t   l_datatype;
  TIO_Dims_t   l_ndims;
  TIO_Size_t   l_dims[TIO_MAXDIMS];
  
  
  
  if (*objectID != TIO_NULL_F) {
    l_objectID = f2c_object(*objectID);
  }
  else {
    l_objectID = TIO_NULL;
  }
  
  
  if (*idx > 0) {
      *err = c2f_TIO( TIO_Open_Variable_idx( f2c_file(*fileID),
                                             l_objectID,
                                             f2c_idx(*idx),
                                             l_name,
                                             &l_varID,
                                             &l_datatype,
                                             &l_ndims,
                                             l_dims,
                                             l_units ) );
      if (*err != TIO_SUCCESS_F) return;
    
      c2f_str(l_name, name_len, name);
  }
  else {
    f2c_str(name, name_len, l_name);
    
    *err = c2f_TIO( TIO_Open_Variable( f2c_file(*fileID),
                                       l_objectID,
                                       l_name,
                                       &l_varID,
                                       &l_datatype,
                                       &l_ndims,
                                       l_dims,
                                       l_units ) );
    if (*err != TIO_SUCCESS_F) return;
    
  }
  
  (void)xtioc_copyvarinfo( NULL,       l_datatype, l_ndims, l_dims, l_units,
                           NULL, NULL, datatype,   ndims,   dims,   units, units_len );
  
  *varID = c2f_object(l_varID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_variable_info_c
 *
 **************************************************************************************************/
void
F2C(tio_get_variable_info_c,TIO_GET_VARIABLE_INFO_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *varID,
       char                *name,
       const int_f         *name_len,
       int_f               *datatype,
       int_f               *ndims,
       TIO_Size_tf         *dims,
       char                *units,
       const int_f         *units_len,
       TIO_tf              *err )
{
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Data_t   l_datatype;
  TIO_Dims_t   l_ndims;
  TIO_Size_t   l_dims[TIO_MAXDIMS];
  
  
  *err = c2f_TIO( TIO_Get_Variable_Info( f2c_file(*fileID),
                                         f2c_object(*varID),
                                         l_name,
                                         &l_datatype,
                                         &l_ndims,
                                         l_dims,
                                         l_units ) );
  if (*err != TIO_SUCCESS_F) return;
  
  (void)xtioc_copyvarinfo( l_name,         l_datatype, l_ndims, l_dims, l_units,
                           name, name_len, datatype,   ndims,   dims,   units, units_len );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_close_variable_c
 *
 **************************************************************************************************/
void
F2C(tio_close_variable_c,TIO_CLOSE_VARIABLE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *varID,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Close_Variable( f2c_file(*fileID),
                                      f2c_object(*varID) ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_write_variable_c
 *
 **************************************************************************************************/
void
F2C(tio_write_variable_c,TIO_WRITE_VARIABLE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *varID,
       const int_f         *datatype,
       const void          *vdata,
       const int_f         *vdata_len,
       TIO_tf              *err )
{
  TIO_File_t   l_fileID;
  TIO_Object_t l_varID;
  TIO_Data_t   l_datatype;
  TIO_Dims_t   l_ndims;
  char         l_vdata[TIO_STRLEN];
  TIO_t        l_err;
  
  l_fileID   = f2c_file(*fileID);
  l_varID    = f2c_object(*varID);
  l_datatype = f2c_datatype(*datatype);
  
  l_err = TIO_Get_Variable_Info( l_fileID, l_varID, NULL, &l_datatype, &l_ndims, NULL, NULL);
  if (l_err != TIO_SUCCESS) {
    *err = c2f_TIO(l_err);
    return;
  }

  if ((TIO_STRING == l_datatype) && (TIO_0D == l_ndims)) {
    
    f2c_str(vdata,  vdata_len, l_vdata);
    
    *err = c2f_TIO( TIO_Write_Variable( l_fileID,
                                        l_varID,
                                        l_datatype,
                                        l_vdata ) );
  }
  else {
    /* 
     * l_datatype is used for checking for conversion for strings.
     * f2c_datatype(*datatype) is used for other conversions, 
     * i.e real(4) in TIO_Create_Variable while real(8)
     * in TIO_Write_Variable.
     */
    *err = c2f_TIO( TIO_Write_Variable( l_fileID,
                                        l_varID,
                                        f2c_datatype(*datatype),
                                        vdata ) );
  }
  
  return;
}



/***************************************************************************************************
 *
 *   tio_read_variable_c
 *
 **************************************************************************************************/
void
F2C(tio_read_variable_c,TIO_READ_VARIABLE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *varID,
       const int_f         *datatype,
       void                *vdata,
       int_f               *vdata_len,
       TIO_tf              *err )
{
  TIO_File_t   l_fileID;
  TIO_Object_t l_varID;
  TIO_Data_t   l_datatype;
  char         l_vdata[TIO_STRLEN];
  TIO_Dims_t   l_ndims;
  
  
  l_fileID   = f2c_file(*fileID);
  l_varID    = f2c_object(*varID);
  l_datatype = f2c_datatype(*datatype);
  
  
  *err = c2f_TIO( TIO_Get_Variable_Info( l_fileID,
                                         l_varID,
                                         NULL,
                                         NULL,
                                         &l_ndims,
                                         NULL,
                                         NULL ) );
  if (*err != TIO_SUCCESS_F) return;
  
  if ((TIO_STRING == l_datatype) && (TIO_0D == l_ndims)) {
    *err = c2f_TIO( TIO_Read_Variable( l_fileID,
                                       l_varID,
                                       l_datatype,
                                       l_vdata ) );
    if (*err != TIO_SUCCESS_F) return;
    
    c2f_str(l_vdata, vdata_len, vdata);
  }
  else {
    *err = c2f_TIO( TIO_Read_Variable( l_fileID,
                                       l_varID,
                                       l_datatype,
                                       vdata ) );
    
  }
  
  return;
}



/***************************************************************************************************
 *
 *   tio_read_variable_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_variable_section_c,TIO_READ_VARIABLE_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *varID,
       const int_f         *datatype,
       const int_f         *ndims,
       const TIO_Size_tf   *lolimit,
       const TIO_Size_tf   *hilimit,
       void                *vdata,
       int_f               *vdata_len,
       TIO_tf              *err )
{
  TIO_Dims_t lndims, d;
  TIO_Size_t llo[TIO_MAXDIMS], lhi[TIO_MAXDIMS];
  TIO_Data_t l_datatype;
  
  l_datatype = f2c_datatype(*datatype);
  lndims     = f2c_ndims(*ndims);
  
  for (d=TIO_0D; d<lndims; ++d) {
    llo[d] = f2c_idx(lolimit[d]);
    lhi[d] = f2c_idx(hilimit[d]);
  }
  
  *err = c2f_TIO( TIO_Read_Variable_Section( f2c_file(*fileID),
                                             f2c_object(*varID),
                                             l_datatype,
                                             lndims,
                                             llo, lhi,
                                             vdata ) );
  
  
  return;
}



/**************************************************************************************************/
static void
xtioc_copyvarinfo( const char           *name,
                   const TIO_Data_t     datatype,
                   const TIO_Dims_t     ndims,
                   const TIO_Size_t     *dims,
                   const char           *units,
                   char                 *fname,  const int_f *fname_len,
                   int_f                *fdatatype,
                   int_f                *fndims,
                   TIO_Size_tf          *fdims,
                   char                 *funits, const int_f *funits_len )
{
  if (name  != NULL) c2f_str(name, fname_len, fname);
  
  *fdatatype = c2f_datatype(datatype);
  *fndims    = c2f_ndims(ndims);
  
  c2f_dims(dims, ndims, fdims, 0);
  c2f_str(units, funits_len, funits);
  
  return;
}



/*
 * EOF
 */

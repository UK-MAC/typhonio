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
 *   tio_list_vargroups_c
 *
 **************************************************************************************************/
void
F2C(tio_list_vargroups_c,TIO_LIST_VARGROUPS_C)
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
    
    *err = c2f_TIO( TIO_List_Vargroups( f2c_file(*fileID),
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
    *err = c2f_TIO( TIO_List_Vargroups( f2c_file(*fileID),
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
 *   tio_create_vargroup_c
 *
 **************************************************************************************************/
void
F2C(tio_create_vargroup_c,TIO_CREATE_VARGROUP_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *objectID,
       const char          *name, const int_f *name_len,
       TIO_Object_tf       *vgroupID,
       TIO_tf              *err )
{
  TIO_Object_t l_objectID, l_vgroupID;
  char         l_name[TIO_STRLEN];
  
  
  
  if (*objectID != TIO_NULL_F) {
    l_objectID = f2c_object(*objectID);
  }
  else {
    l_objectID = TIO_NULL;
  }
  
  f2c_str(name, name_len, l_name);
  
  *err = c2f_TIO( TIO_Create_Vargroup( f2c_file(*fileID),
                                       l_objectID,
                                       l_name,
                                       &l_vgroupID ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *vgroupID = c2f_object(l_vgroupID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_open_vargroup_c
 *
 **************************************************************************************************/
void
F2C(tio_open_vargroup_c,TIO_OPEN_VARGROUP_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *objectID,
       const TIO_Size_tf   *idx,
       char                *name, const int_f *name_len,
       TIO_Object_tf       *vgroupID,
       TIO_tf              *err )
{
  TIO_Object_t l_objectID, l_vgroupID;
  char         l_name[TIO_STRLEN];  
  
  
  
  if (*objectID != TIO_NULL_F) {
    l_objectID = f2c_object(*objectID);
  }
  else {
    l_objectID = TIO_NULL;
  }
  
  
  if (*idx > 0) {
      *err = c2f_TIO( TIO_Open_Vargroup_idx( f2c_file(*fileID),
                                             l_objectID,
                                             f2c_idx(*idx),
                                             l_name,
                                             &l_vgroupID ) );
      if (*err != TIO_SUCCESS_F) return;

      c2f_str(l_name, name_len, name);
  }
  else {
    f2c_str(name, name_len, l_name);
    
    *err = c2f_TIO( TIO_Open_Vargroup( f2c_file(*fileID),
                                       l_objectID,
                                       l_name,
                                       &l_vgroupID ) );
    
    if (*err != TIO_SUCCESS_F) return;
  }
  
  *vgroupID = c2f_object(l_vgroupID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_vargroup_info_c
 *
 **************************************************************************************************/
void
F2C(tio_get_vargroup_info_c,TIO_GET_VARGROUP_INFO_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *vgroupID,
       char                *name, const int_f *name_len,
       TIO_tf              *err )
{
  char l_name[TIO_STRLEN];
  
  
  *err = c2f_TIO( TIO_Get_Vargroup_Info( f2c_file(*fileID),
                                         f2c_object(*vgroupID),
                                         l_name ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  c2f_str(l_name, name_len, name);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_close_vargroup_c
 *
 **************************************************************************************************/
void
F2C(tio_close_vargroup_c,TIO_CLOSE_VARGROUP_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *vgroupID,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Close_Vargroup( f2c_file(*fileID),
                                      f2c_object(*vgroupID) ) );
  
  return;
}


/*
 * EOF
 */

/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"
#include "tio_register_int.h"

#include "tio_vargroup.h"


static TIO_t cTIO_OpenVgroup( const char         subname[],
                              const int          ByIndex,
                              const TIO_File_t   fileID,
                              const TIO_Object_t objectID,
                              const TIO_Size_t   idx,
                              char               name[],
                              TIO_Object_t       *vgroupID );




/***************************************************************************************************
 *
 * TIO_List_Vargroups
 *
 **************************************************************************************************/
TIO_t
TIO_List_Vargroups( const TIO_File_t   fileID,
                    const TIO_Object_t objectID,
                    TIO_Size_t         *nobj,
                    char               names[][TIO_STRLEN] )
{
  const char *subname = "TIO_List_Vargroups()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyListObjects( subname, fileID, objectID, CLASS_NULL, CLASS_VGROUP,
                                   nobj, names);

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Create_Vargroup
 *
 **************************************************************************************************/
TIO_t
TIO_Create_Vargroup( const TIO_File_t   fileID,
                     const TIO_Object_t objectID,
                     const char         name[],
                     TIO_Object_t       *vgroupID )
{
  const char *subname = "TIO_Create_Vargroup()";

  struct iFile_t   *tfile;
  struct iVgroup_t *tvgroup;

  TIO_Object_t lvgID;
  TIO_t        trc;


  TIObegin(subname,1);

  *vgroupID = TIO_NULL;    /* Ensure NULL value on failure */


  tvgroup = (struct iVgroup_t *)TIOmalloc(sizeof(struct iVgroup_t));
  if (NULL == tvgroup) {
    TIOreturn(ERR_MEM, "Failed to malloc cache", TIO_ERR_INT);
  }

  trc = cTIO_HierarchyCreateObject( subname, fileID, objectID, CLASS_NULL, CLASS_VGROUP, name,
                                    &tfile, &lvgID, tvgroup );
  if (trc != TIO_SUCCESS) {
    TIOfree (tvgroup); tvgroup = NULL;
    cTIOreturn (trc);
  }

  (void) iTIO_strcpy(tvgroup->name, name);


  *vgroupID = lvgID;

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Open_Vargroup
 * TIO_Open_Vargroup_idx
 *
 **************************************************************************************************/
TIO_t
TIO_Open_Vargroup( const TIO_File_t   fileID,
                   const TIO_Object_t objectID,
                   const char         name[],
                   TIO_Object_t       *vgroupID )
{
  return ( cTIO_OpenVgroup("TIO_Open_Vargroup()", FALSE,
                           fileID, objectID, TIO_NULL, (char *)name, vgroupID) );
}


TIO_t
TIO_Open_Vargroup_idx( const TIO_File_t   fileID,
                       const TIO_Object_t objectID,
                       const TIO_Size_t   idx,
                       char               name[],
                       TIO_Object_t       *vgroupID )
{
  return ( cTIO_OpenVgroup("TIO_Open_Vargroup_idx()", TRUE,
                           fileID, objectID, idx, name, vgroupID) );
}


static TIO_t
cTIO_OpenVgroup( const char         subname[],
                 const int          ByIndex,
                 const TIO_File_t   fileID,
                 const TIO_Object_t objectID,
                 const TIO_Size_t   idx,
                 char               name[],
                 TIO_Object_t       *vgroupID )
{
  struct iFile_t   *tfile;
  struct iVgroup_t *tvgroup;

  TIO_Object_t lvgID;
  TIO_t        trc;


  TIObegin(subname,1);

  *vgroupID = TIO_NULL;    /* Ensure NULL value on failure */


  tvgroup = (struct iVgroup_t *)TIOmalloc(sizeof(struct iVgroup_t));
  if (NULL == tvgroup) {
    TIOreturn(ERR_MEM, "Failed to malloc cache", TIO_ERR_INT);
  }

  trc = cTIO_HierarchyOpenObject( subname, fileID, objectID, CLASS_NULL, CLASS_VGROUP,
                                  name, ByIndex, idx,
                                  tvgroup->name, &tfile, &lvgID, (void *)tvgroup );
  if (trc != TIO_SUCCESS) {
    TIOfree (tvgroup); tvgroup = NULL;
    cTIOreturn (trc);
  }

  if (ByIndex && (name != NULL)) {
    (void) iTIO_strcpy(name, tvgroup->name);
  }

  *vgroupID = lvgID;

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_Vargroup_Info
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Vargroup_Info( const TIO_File_t   fileID,
                       const TIO_Object_t vgroupID,
                       char               *name )
{
  const char *subname = "TIO_Get_Vargroup_Info()";

  struct iVgroup_t *tvgroup;

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, vgroupID, CLASS_VGROUP,
                                     NULL, (void **)&tvgroup );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (name != NULL) {
    (void) iTIO_strcpy(name, tvgroup->name);
  }

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Close_Vargroup
 *
 **************************************************************************************************/
TIO_t
TIO_Close_Vargroup( const TIO_File_t   fileID,
                    const TIO_Object_t vgroupID )
{
  const char *subname = "TIO_Close_Vgroup()";

  struct iVgroup_t *tvgroup;

  TIO_Object_t lpID;   /* parentID */
  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyPreClose( subname, fileID, &lpID, vgroupID, CLASS_VGROUP,
                                NULL, (void **)&tvgroup );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  /* -- Free the object cache */
  TIOfree (tvgroup); tvgroup = NULL;

  trc = cTIO_HierarchyCloseObject( subname, fileID, lpID, vgroupID, CLASS_VGROUP );

  TIOend(subname,1);

  return (trc);
}



/*
 * EOF
 */

/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_VARGROUP_H
#define _TIO_VARGROUP_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************************************
 *
 * TIO_List_Vargroups
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_List_Vargroups
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the number of Vargroups within a TyphonIO file or object and their names
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_List_Vargroups( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t objectID,
 * <DATA>                           TIO_Size_t         *nobj,
 * <DATA>                           char               names[][TIO_STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID
 * <DATA> OUT <DATA> nobj     <DATA> Number of Vargroups
 * <DATA> OUT <DATA> names    <DATA> Array of names
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the <code>objectID</code> is given as <code>TIO_NULL</code>, the Vargroups
 * <DATA> in the top file level are listed, otherwise it is the those in the given object.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 * <CDES>
 * <DATA> <code>names</codes> can be <code>NULL</code>
 * </CDES>
 *
 * </ITEM>
 */
extern TIO_t TIO_List_Vargroups( const TIO_File_t   fileID,
                                 const TIO_Object_t objectID,
                                 TIO_Size_t         *nobj,
                                 char               names[][TIO_STRLEN] );


/***************************************************************************************************
 *
 * TIO_Create_Vargroup
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Create_Vargroup
 * </NAME>
 *
 * <PURP>
 * <DATA> Creates a new Vargroup within a TyphonIO file or object
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Create_Vargroup( const TIO_File_t   fileID,
 * <DATA>                            const TIO_Object_t objectID,
 * <DATA>                            const char         name[],
 * <DATA>                            TIO_Object_t       *vgroupID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID
 * <DATA> IN  <DATA> name     <DATA> Name for Vargroup
 * <DATA> OUT <DATA> vgroupID <DATA> ID of new Vargroup
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the <code>objectID</code> is given as <code>TIO_NULL</code>, the Vargroup is
 * <DATA> created in the top file level, otherwise it is created in the specified object.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Create_Vargroup( const TIO_File_t   fileID,
                                  const TIO_Object_t objectID,
                                  const char         name[],
                                  TIO_Object_t       *vgroupID );


/***************************************************************************************************
 *
 * TIO_Open_Vargroup
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Vargroup
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens a Vargroup from within a TyphonIO file or object by name
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Vargroup( const TIO_File_t   fileID,
 * <DATA>                          const TIO_Object_t objectID,
 * <DATA>                          const char         name[],
 * <DATA>                          TIO_Object_t       *vgroupID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID
 * <DATA> IN  <DATA> name     <DATA> Name of Vargroup
 * <DATA> OUT <DATA> vgroupID <DATA> ID of opened Vargroup
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the <code>objectID</code> is given as <code>TIO_NULL</code>, the Vargroup is
 * <DATA> opened from the top file level, otherwise it is opened from the specified object.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Vargroup( const TIO_File_t   fileID,
                                const TIO_Object_t objectID,
                                const char         name[],
                                TIO_Object_t       *vgroupID );


/***************************************************************************************************
 *
 * TIO_Open_Vargroup_idx
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Vargroup_idx
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens a Vargroup from within a TyphonIO file or object by index
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Vargroup_idx( const TIO_File_t   fileID,
 * <DATA>                              const TIO_Object_t objectID,
 * <DATA>                              const TIO_Size_t   idx,
 * <DATA>                              char               name,
 * <DATA>                              TIO_Object_t       *vgroupID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID
 * <DATA> IN  <DATA> idx      <DATA> Index of Vargroup
 * <DATA> OUT <DATA> name     <DATA> Name of Vargroup
 * <DATA> OUT <DATA> vgroupID <DATA> ID of opened Vargroup
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the <code>objectID</code> is given as <code>TIO_NULL</code>, the Vargroup is
 * <DATA> opened from the top file level, otherwise it is opened from the specified object.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Vargroup_idx( const TIO_File_t   fileID,
                                    const TIO_Object_t objectID,
                                    const TIO_Size_t   idx,
                                    char               name[],
                                    TIO_Object_t       *vgroupID );


/***************************************************************************************************
 *
 * TIO_Get_Vargroup_Info
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Vargroup_Info
 * </NAME>
 *
 * <PURP>
 * <DATA> Gets the info of an open Vargroup
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Vargroup_Info( const TIO_File_t   fileID,
 * <DATA>                              const TIO_Object_t vgroupID,
 * <DATA>                              char               name[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> vgroupID <DATA> TyphonIO Vargroup ID
 * <DATA> OUT <DATA> name     <DATA> Name of Vargroup
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Returns the metadata for a Vargroup.  For a Vargroup this is just its name.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_Vargroup_Info( const TIO_File_t   fileID,
                                    const TIO_Object_t vgroupID,
                                    char               name[]    );


/***************************************************************************************************
 *
 * TIO_Close_Vargroup
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Close_Vargroup
 * </NAME>
 *
 * <PURP>
 * <DATA> Closes an open Vargroup
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Close_Vargroup( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t vgroupID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN <DATA> vgroupID <DATA> TyphonIO Vargroup ID
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Closes an open Vargroup.  All child contents of the Vargroup should already be closed.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Close_Vargroup( const TIO_File_t   fileID,
                                 const TIO_Object_t vgroupID  );




#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */

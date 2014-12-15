/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_MATERIAL_H
#define _TIO_MATERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
 *
 * TIO_List_Material
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_List_Material
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the materials object asociated with the mesh
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_List_Material( const TIO_File_t   fileID,
 * <DATA>                          const TIO_Object_t meshID,
 * <DATA>                          char               name[TIO_STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID <DATA> TyphonIO Mesh ID
 * <DATA> OUT <DATA> name   <DATA> The name of the TyphonIO Material
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_List_Material(     const TIO_File_t   fileID,
                                    const TIO_Object_t meshID,
                                    char               name[TIO_STRLEN] );

/***************************************************************************************************
 *
 * TIO_Create_Material
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Create_Material
 * </NAME>
 *
 * <PURP>
 * <DATA> Create a new material
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Create_Material( const TIO_File_t   fileID,
 * <DATA>                            const TIO_Object_t meshID,
 * <DATA>                            const char         name[],
 * <DATA>                            TIO_Object_t       *materialID,
 * <DATA>                            const TIO_Data_t   datatype,
 * <DATA>                            const TIO_Size_t   nmat,
 * <DATA>                            const TIO_Size_t   nghosts,
 * <DATA>                            const TIO_Bool_t   ismixed,
 * <DATA>                            const TIO_Data_t   ncomp_datatype,
 * <DATA>                            const TIO_Data_t   ptr_datatype,
 * <DATA>                            const TIO_Data_t   vf_datatype );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID         <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> name           <DATA> The material name
 * <DATA> OUT <DATA> materialID     <DATA> The material id
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nmat           <DATA> Number of materials
 * <DATA> IN  <DATA> nghosts        <DATA> Number of ghosts
 * <DATA> IN  <DATA> ismixed        <DATA> Whether the material can handle mixed data
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * <CAPI>
 * <DATA> ncomp_datatype and ptr_datatype must be set to TIO_TYPECLASS_SIGNED_INT, vf_datatype must be set to TIO_TYPECLASS_FLOAT.
 * </CAPI>
 *
 * </ITEM>
 */
extern TIO_t TIO_Create_Material(   const TIO_File_t   fileID,
                                    const TIO_Object_t meshID,
                                    const char         name[],
                                    TIO_Object_t       *materialID,
                                    const TIO_Data_t   datatype,
                                    const TIO_Size_t   nmat,
                                    const TIO_Size_t   nghosts,
                                    const TIO_Bool_t   ismixed,
                                    const TIO_Data_t   ncomp_datatype,
                                    const TIO_Data_t   ptr_datatype,
                                    const TIO_Data_t   vf_datatype );

/***************************************************************************************************
 *
 * TIO_Open_Material
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Material
 * </NAME>
 *
 * <PURP>
 * <DATA> Open a mesh material
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Material( const TIO_File_t   fileID,
 * <DATA>                          const TIO_Object_t meshID,
 * <DATA>                          const char         name[],
 * <DATA>                          TIO_Object_t       *materialID,
 * <DATA>                          TIO_Data_t         *datatype,
 * <DATA>                          TIO_Size_t         *nmat,
 * <DATA>                          TIO_Size_t         *nghosts,
 * <DATA>                          TIO_Bool_t         *ismixed,
 * <DATA>                          TIO_Data_t         *ncomp_datatype,
 * <DATA>                          TIO_Data_t         *ptr_datatype,
 * <DATA>                          TIO_Data_t         *vf_datatype );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID         <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> name           <DATA> The material name
 * <DATA> OUT <DATA> materialID     <DATA> The material ID
 * <DATA> OUT <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nmat           <DATA> Number of materials
 * <DATA> OUT <DATA> nghosts        <DATA> Number of ghosts
 * <DATA> OUT <DATA> ismixed        <DATA> Whether the material can handle mixed data
 * <DATA> OUT <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> OUT <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> OUT <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Material(     const TIO_File_t   fileID,
                                    const TIO_Object_t meshID,
                                    const char         name[],
                                    TIO_Object_t       *materialID,
                                    TIO_Data_t         *datatype,
                                    TIO_Size_t         *nmat,
                                    TIO_Size_t         *nghosts,
                                    TIO_Bool_t         *ismixed,
                                    TIO_Data_t         *ncomp_datatype,
                                    TIO_Data_t         *ptr_datatype,
                                    TIO_Data_t         *vf_datatype );

/***************************************************************************************************
 *
 * TIO_Close_Material
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Close_Material
 * </NAME>
 *
 * <PURP>
 * <DATA> Close the mesh material.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Close_Material( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t materialID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID     <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID <DATA> TyphonIO material ID
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Close_Material(    const TIO_File_t   fileID,
                                    const TIO_Object_t materialID );

/***************************************************************************************************
 *
 * TIO_Get_Material_Info
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Material_Info
 * </NAME>
 *
 * <PURP>
 * <DATA> Get material info for an open material.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Material_Info( const TIO_File_t   fileID,
 * <DATA>                              const TIO_Object_t materialID,
 * <DATA>                              char               name[],
 * <DATA>                              TIO_Data_t         *datatype,
 * <DATA>                              TIO_Size_t         *nmat,
 * <DATA>                              TIO_Size_t         *nghosts,
 * <DATA>                              TIO_Bool_t         *ismixed,
 * <DATA>                              TIO_Data_t         *ncomp_datatype,
 * <DATA>                              TIO_Data_t         *ptr_datatype,
 * <DATA>                              TIO_Data_t         *vf_datatype );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyphonIO material ID
 * <DATA> OUT <DATA> name           <DATA> The material name
 * <DATA> OUT <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nmat           <DATA> Number of materials
 * <DATA> OUT <DATA> nghosts        <DATA> Number of ghosts
 * <DATA> OUT <DATA> ismixed        <DATA> Whether the material can handle mixed data
 * <DATA> OUT <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> OUT <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> OUT <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_Material_Info( const TIO_File_t   fileID,
                                    const TIO_Object_t materialID,
                                    char               name[],
                                    TIO_Data_t         *datatype,
                                    TIO_Size_t         *nmat,
                                    TIO_Size_t         *nghosts,
                                    TIO_Bool_t         *ismixed,
                                    TIO_Data_t         *ncomp_datatype,
                                    TIO_Data_t         *ptr_datatype,
                                    TIO_Data_t         *vf_datatype );

/***************************************************************************************************
 *
 * TIO_Set_Material_Range
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Set_Material_Range
 * </NAME>
 *
 * <PURP>
 * <DATA> Sets the numerical and textual labels for the materials.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Set_Material_Range( const TIO_File_t   fileID,
 * <DATA>                               const TIO_Object_t materialID,
 * <DATA>                               const TIO_Data_t   datatype,
 * <DATA>                               const TIO_Size_t   nmat,
 * <DATA>                               const void         *matnums,
 * <DATA>                               char               matnames[][TIO_STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID     <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> datatype   <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nmat       <DATA> Number of materials (must match that used in TIO_Create_Material)
 * <DATA> IN  <DATA> matnums    <DATA> The array of material numerical labels
 * <DATA> IN  <DATA> matnames   <DATA> The array of material names
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Set_Material_Range( const TIO_File_t   fileID,
                                     const TIO_Object_t materialID,
                                     const TIO_Data_t   datatype,
                                     const TIO_Size_t   nmat,
                                     const void         *matnums,
                                     char               matnames[][TIO_STRLEN] );

/***************************************************************************************************
 *
 * TIO_Get_Material_Range
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Material_Range
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the numerical and textual labels for the materials.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Material_Range( const TIO_File_t   fileID,
 * <DATA>                               const TIO_Object_t materialID,
 * <DATA>                               const TIO_Data_t   datatype,
 * <DATA>                               TIO_Size_t         *nmat,
 * <DATA>                               void               *matnums,
 * <DATA>                               char               matnames[][TIO_STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID     <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> datatype   <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nmat       <DATA> Number of materials
 * <DATA> OUT <DATA> matnums    <DATA> The array of material numerical labels
 * <DATA> OUT <DATA> matnames   <DATA> The array of material names
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_Material_Range( const TIO_File_t   fileID,
                                     const TIO_Object_t materialID,
                                     const TIO_Data_t   datatype,
                                     TIO_Size_t         *nmat,
                                     void               *matnums,
                                     char               matnames[][TIO_STRLEN] );


/**************************************
 **************************************
 **
 **   QUAD MATERIALS
 **
 **************************************
 **************************************/

/***************************************************************************************************
 *
 * TIO_Write_QuadMaterial_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_QuadMaterial_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write to file the material data for a structured mesh chunk.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_QuadMaterial_Chunk( const TIO_File_t   fileID,
 * <DATA>                                     const TIO_Object_t materialID,
 * <DATA>                                     const TIO_Size_t   idx,
 * <DATA>                                     const TIO_Xfer_t   xfer,
 * <DATA>                                     const TIO_Data_t   datatype,
 * <DATA>                                     const void         *mat,
 * <DATA>                                     const TIO_Data_t   ncomp_datatype,
 * <DATA>                                     const TIO_Data_t   ptr_datatype,
 * <DATA>                                     const TIO_Data_t   vf_datatype,
 * <DATA>                                     const void         *ncomp,
 * <DATA>                                     const void         *ptr,
 * <DATA>                                     const void         *mixmat,
 * <DATA>                                     const void         *vf );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> idx            <DATA> Chunk index
 * <DATA> IN  <DATA> xfer           <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> IN  <DATA> mat            <DATA> Mesh material data array
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * <DATA> IN  <DATA> ncomp          <DATA> The mixed material num-component array
 * <DATA> IN  <DATA> ptr            <DATA> The mixed material mat-pointer array
 * <DATA> IN  <DATA> mixmat         <DATA> The mixed material mat-number array
 * <DATA> IN  <DATA> vf             <DATA> The mixed material volumn fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * <CAPI>
 * <DATA> ncomp_datatype and ptr_datatype must be set to TIO_TYPECLASS_SIGNED_INT, vf_datatype must be set to TIO_TYPECLASS_FLOAT.
 * </CAPI>
 *
 * </ITEM>
 */
extern TIO_t TIO_Write_QuadMaterial_Chunk(  const TIO_File_t   fileID,
                                            const TIO_Object_t materialID,
                                            const TIO_Size_t   idx,
                                            const TIO_Xfer_t   xfer,
                                            const TIO_Data_t   datatype,
                                            const void         *mat,
                                            const TIO_Data_t   ncomp_datatype,
                                            const TIO_Data_t   ptr_datatype,
                                            const TIO_Data_t   vf_datatype,
                                            const void         *ncomp,
                                            const void         *ptr,
                                            const void         *mixmat,
                                            const void         *vf );

/***************************************************************************************************
 *
 * TIO_Read_QuadMaterial_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadMaterial_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the material data for a structured mesh chunk.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadMaterial_Chunk( const TIO_File_t   fileID,
 * <DATA>                                    const TIO_Object_t materialID,
 * <DATA>                                    const TIO_Size_t   idx,
 * <DATA>                                    const TIO_Xfer_t   xfer,
 * <DATA>                                    const TIO_Data_t   datatype,
 * <DATA>                                    const TIO_Size_t   nghosts,
 * <DATA>                                    TIO_Dims_t         *ndims,
 * <DATA>                                    TIO_Size_t         *ni,
 * <DATA>                                    TIO_Size_t         *nj,
 * <DATA>                                    TIO_Size_t         *nk,
 * <DATA>                                    void               *mat,
 * <DATA>                                    const TIO_Data_t   ncomp_datatype,
 * <DATA>                                    const TIO_Data_t   ptr_datatype,
 * <DATA>                                    const TIO_Data_t   vf_datatype,
 * <DATA>                                    TIO_Size_t         *nmixcell,
 * <DATA>                                    TIO_Size_t         *nmixcomp,
 * <DATA>                                    void               *ncomp,
 * <DATA>                                    void               *ptr,
 * <DATA>                                    void               *mixmat,
 * <DATA>                                    void               *vf );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> idx            <DATA> Chunk index
 * <DATA> IN  <DATA> xfer           <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts        <DATA> Number of ghosts
 * <DATA> OUT <DATA> ndims          <DATA> Number of dimensions
 * <DATA> OUT <DATA> ni             <DATA> Size of the mesh chunk in the 1st dimension
 * <DATA> OUT <DATA> nj             <DATA> Size of the mesh chunk in the 2nd dimension
 * <DATA> OUT <DATA> nk             <DATA> Size of the mesh chunk in the 3rd dimension
 * <DATA> OUT <DATA> mat            <DATA> The material data array
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * <DATA> OUT <DATA> nmixcell       <DATA> Number of mixed cells
 * <DATA> OUT <DATA> nmixcomp       <DATA> Number of mixed components
 * <DATA> OUT <DATA> ncomp          <DATA> The mixed material num-component array
 * <DATA> OUT <DATA> ptr            <DATA> The mixed material mat-pointer array
 * <DATA> OUT <DATA> mixmat         <DATA> The mixed material mat-number array
 * <DATA> OUT <DATA> vf             <DATA> The mixed material volumn fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * <CAPI>
 * <DATA> ncomp_datatype and ptr_datatype must be set to TIO_TYPECLASS_SIGNED_INT, vf_datatype must be set to TIO_TYPECLASS_FLOAT.
 * </CAPI>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_QuadMaterial_Chunk(   const TIO_File_t   fileID,
                                            const TIO_Object_t materialID,
                                            const TIO_Size_t   idx,
                                            const TIO_Xfer_t   xfer,
                                            const TIO_Data_t   datatype,
                                            const TIO_Size_t   nghosts,
                                            TIO_Dims_t         *ndims,
                                            TIO_Size_t         *ni,
                                            TIO_Size_t         *nj,
                                            TIO_Size_t         *nk,
                                            void               *mat,
                                            const TIO_Data_t   ncomp_datatype,
                                            const TIO_Data_t   ptr_datatype,
                                            const TIO_Data_t   vf_datatype,
                                            TIO_Size_t         *nmixcell,
                                            TIO_Size_t         *nmixcomp,
                                            void               *ncomp,
                                            void               *ptr,
                                            void               *mixmat,
                                            void               *vf );

/***************************************************************************************************
 *
 * TIO_Read_QuadMaterial_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadMaterial_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the material data for a structured mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadMaterial_All( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t materialID,
 * <DATA>                                  const TIO_Data_t   datatype,
 * <DATA>                                  TIO_Dims_t         *ndims,
 * <DATA>                                  TIO_Size_t         *ni,
 * <DATA>                                  TIO_Size_t         *nj,
 * <DATA>                                  TIO_Size_t         *nk,
 * <DATA>                                  void               *mat,
 * <DATA>                                  const TIO_Data_t   ncomp_datatype,
 * <DATA>                                  const TIO_Data_t   ptr_datatype,
 * <DATA>                                  const TIO_Data_t   vf_datatype,
 * <DATA>                                  TIO_Size_t         *nmixcell,
 * <DATA>                                  TIO_Size_t         *nmixcomp,
 * <DATA>                                  void               *ncomp,
 * <DATA>                                  void               *ptr,
 * <DATA>                                  void               *mixmat,
 * <DATA>                                  void               *vf );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyphonIO mesh ID
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims          <DATA> Number of dimensions
 * <DATA> OUT <DATA> ni             <DATA> Size of the mesh chunk in the 1st dimension
 * <DATA> OUT <DATA> nj             <DATA> Size of the mesh chunk in the 2nd dimension
 * <DATA> OUT <DATA> nk             <DATA> Size of the mesh chunk in the 3rd dimension
 * <DATA> OUT <DATA> mat            <DATA> The material data array
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * <DATA> OUT <DATA> nmixcell       <DATA> Number of mixed cells
 * <DATA> OUT <DATA> nmixcomp       <DATA> Number of mixed components
 * <DATA> OUT <DATA> ncomp          <DATA> The mixed material num-component array
 * <DATA> OUT <DATA> ptr            <DATA> The mixed material mat-pointer array
 * <DATA> OUT <DATA> mixmat         <DATA> The mixed material mat-number array
 * <DATA> OUT <DATA> vf             <DATA> The mixed material volumn fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * <CAPI>
 * <DATA> ncomp_datatype and ptr_datatype must be set to TIO_TYPECLASS_SIGNED_INT, vf_datatype must be set to TIO_TYPECLASS_FLOAT.
 * </CAPI>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_QuadMaterial_All(     const TIO_File_t   fileID,
                                            const TIO_Object_t materialID,
                                            const TIO_Data_t   datatype,
                                            TIO_Dims_t         *ndims,
                                            TIO_Size_t         *ni,
                                            TIO_Size_t         *nj,
                                            TIO_Size_t         *nk,
                                            void               *mat,
                                            const TIO_Data_t   ncomp_datatype,
                                            const TIO_Data_t   ptr_datatype,
                                            const TIO_Data_t   vf_datatype,
                                            TIO_Size_t         *nmixcell,
                                            TIO_Size_t         *nmixcomp,
                                            void               *ncomp,
                                            void               *ptr,
                                            void               *mixmat,
                                            void               *vf );

/***************************************************************************************************
 *
 * TIO_Read_QuadMaterial_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadMaterial_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the material data for a structured mesh section.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadMaterial_Section( const TIO_File_t   fileID,
 * <DATA>                                      const TIO_Object_t materialID,
 * <DATA>                                      const TIO_Object_t sectionID,
 * <DATA>                                      const TIO_Xfer_t   xfer,
 * <DATA>                                      const TIO_Data_t   datatype,
 * <DATA>                                      TIO_Dims_t         *ndims,
 * <DATA>                                      TIO_Size_t         *ni,
 * <DATA>                                      TIO_Size_t         *nj,
 * <DATA>                                      TIO_Size_t         *nk,
 * <DATA>                                      void               *mat,
 * <DATA>                                      const TIO_Data_t   ncomp_datatype,
 * <DATA>                                      const TIO_Data_t   ptr_datatype,
 * <DATA>                                      const TIO_Data_t   vf_datatype,
 * <DATA>                                      TIO_Size_t         *nmixcell,
 * <DATA>                                      TIO_Size_t         *nmixcomp,
 * <DATA>                                      void               *ncomp,
 * <DATA>                                      void               *ptr,
 * <DATA>                                      void               *mixmat,
 * <DATA>                                      void               *vf );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> sectionID      <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer           <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims          <DATA> Number of dimensions
 * <DATA> OUT <DATA> ni             <DATA> Size of the mesh chunk in the 1st dimension
 * <DATA> OUT <DATA> nj             <DATA> Size of the mesh chunk in the 2nd dimension
 * <DATA> OUT <DATA> nk             <DATA> Size of the mesh chunk in the 3rd dimension
 * <DATA> OUT <DATA> mat            <DATA> The material data array
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * <DATA> OUT <DATA> nmixcell       <DATA> Number of mixed cells
 * <DATA> OUT <DATA> nmixcomp       <DATA> Number of mixed components
 * <DATA> OUT <DATA> ncomp          <DATA> The mixed material num-component array
 * <DATA> OUT <DATA> ptr            <DATA> The mixed material mat-pointer array
 * <DATA> OUT <DATA> mixmat         <DATA> The mixed material mat-number array
 * <DATA> OUT <DATA> vf             <DATA> The mixed material volumn fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_QuadMaterial_Section( const TIO_File_t   fileID,
                                            const TIO_Object_t materialID,
                                            const TIO_Object_t sectionID,
                                            const TIO_Xfer_t   xfer,
                                            const TIO_Data_t   datatype,
                                            TIO_Dims_t         *ndims,
                                            TIO_Size_t         *ni,
                                            TIO_Size_t         *nj,
                                            TIO_Size_t         *nk,
                                            void               *mat,
                                            const TIO_Data_t   ncomp_datatype,
                                            const TIO_Data_t   ptr_datatype,
                                            const TIO_Data_t   vf_datatype,
                                            TIO_Size_t         *nmixcell,
                                            TIO_Size_t         *nmixcomp,
                                            void               *ncomp,
                                            void               *ptr,
                                            void               *mixmat,
                                            void               *vf );


/**************************************
 **************************************
 **
 **   UNSTR MATERIALS
 **
 **************************************
 **************************************/

/***************************************************************************************************
 *
 * TIO_Write_UnstrMaterial_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_UnstrMaterial_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write an unstructured mesh chunk's material data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_UnstrMaterial_Chunk( const TIO_File_t   fileID,
 * <DATA>                                      const TIO_Object_t materialID,
 * <DATA>                                      const TIO_Size_t   idx,
 * <DATA>                                      const TIO_Xfer_t   xfer,
 * <DATA>                                      const TIO_Data_t   datatype,
 * <DATA>                                      const void         *mat,
 * <DATA>                                      const TIO_Data_t   ncomp_datatype,
 * <DATA>                                      const TIO_Data_t   ptr_datatype,
 * <DATA>                                      const TIO_Data_t   vf_datatype,
 * <DATA>                                      const void         *ncomp,
 * <DATA>                                      const void         *ptr,
 * <DATA>                                      const void         *mixmat,
 * <DATA>                                      const void         *vf );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> idx            <DATA> Chunk index
 * <DATA> IN  <DATA> xfer           <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> IN  <DATA> mat            <DATA> Material data array
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * <DATA> IN  <DATA> ncomp          <DATA> The mixed material num-component array
 * <DATA> IN  <DATA> ptr            <DATA> The mixed material mat-pointer array
 * <DATA> IN  <DATA> mixmat         <DATA> The mixed material mat-number array
 * <DATA> IN  <DATA> vf             <DATA> The mixed material volumn fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Write_UnstrMaterial_Chunk(  const TIO_File_t   fileID,
                                             const TIO_Object_t materialID,
                                             const TIO_Size_t   idx,
                                             const TIO_Xfer_t   xfer,
                                             const TIO_Data_t   datatype,
                                             const void         *mat,
                                             const TIO_Data_t   ncomp_datatype,
                                             const TIO_Data_t   ptr_datatype,
                                             const TIO_Data_t   vf_datatype,
                                             const void         *ncomp,
                                             const void         *ptr,
                                             const void         *mixmat,
                                             const void         *vf );

/***************************************************************************************************
 *
 * TIO_Read_UnstrMaterial_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrMaterial_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read an unstructured mesh chunk's material data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrMaterial_Chunk( const TIO_File_t   fileID,
 * <DATA>                                     const TIO_Object_t materialID,
 * <DATA>                                     const TIO_Size_t   idx,
 * <DATA>                                     const TIO_Xfer_t   xfer,
 * <DATA>                                     const TIO_Data_t   datatype,
 * <DATA>                                     const TIO_Size_t   nghosts,
 * <DATA>                                     TIO_Size_t         *ncells,
 * <DATA>                                     void               *mat,
 * <DATA>                                     const TIO_Data_t   ncomp_datatype,
 * <DATA>                                     const TIO_Data_t   ptr_datatype,
 * <DATA>                                     const TIO_Data_t   vf_datatype,
 * <DATA>                                     TIO_Size_t         *nmixcell,
 * <DATA>                                     TIO_Size_t         *nmixcomp,
 * <DATA>                                     void               *ncomp,
 * <DATA>                                     void               *ptr,
 * <DATA>                                     void               *mixmat,
 * <DATA>                                     void               *vf );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyohonIO material ID
 * <DATA> IN  <DATA> idx            <DATA> Chunk index
 * <DATA> IN  <DATA> xfer           <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts        <DATA> Number of ghosts
 * <DATA> OUT <DATA> ncells         <DATA> Number of cells
 * <DATA> OUT <DATA> mat            <DATA> Material data array
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * <DATA> OUT <DATA> nmixcell       <DATA> Number of mixed cells
 * <DATA> OUT <DATA> nmixcomp       <DATA> Number of mixed components
 * <DATA> OUT <DATA> ncomp          <DATA> The mixed material num-component array
 * <DATA> OUT <DATA> ptr            <DATA> The mixed material mat-pointer array
 * <DATA> OUT <DATA> mixmat         <DATA> The mixed material mat-number array
 * <DATA> OUT <DATA> vf             <DATA> The mixed material volumn fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_UnstrMaterial_Chunk(   const TIO_File_t   fileID,
                                             const TIO_Object_t materialID,
                                             const TIO_Size_t   idx,
                                             const TIO_Xfer_t   xfer,
                                             const TIO_Data_t   datatype,
                                             const TIO_Size_t   nghosts,
                                             TIO_Size_t         *ncells,
                                             void               *mat,
                                             const TIO_Data_t   ncomp_datatype,
                                             const TIO_Data_t   ptr_datatype,
                                             const TIO_Data_t   vf_datatype,
                                             TIO_Size_t         *nmixcell,
                                             TIO_Size_t         *nmixcomp,
                                             void               *ncomp,
                                             void               *ptr,
                                             void               *mixmat,
                                             void               *vf );

/***************************************************************************************************
 *
 * TIO_Read_UnstrMaterial_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrMaterial_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read all the material data for an unstructured mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrMaterial_All( const TIO_File_t   fileID,
 * <DATA>                                   const TIO_Object_t materialID,
 * <DATA>                                   const TIO_Data_t   datatype,
 * <DATA>                                   TIO_Size_t         *ncells,
 * <DATA>                                   void               *mat,
 * <DATA>                                   const TIO_Data_t   ncomp_datatype,
 * <DATA>                                   const TIO_Data_t   ptr_datatype,
 * <DATA>                                   const TIO_Data_t   vf_datatype,
 * <DATA>                                   TIO_Size_t         *nmixcell,
 * <DATA>                                   TIO_Size_t         *nmixcomp,
 * <DATA>                                   void               *ncomp,
 * <DATA>                                  void               *ptr,
 * <DATA>                                   void               *mixmat,
 * <DATA>                                   void               *vf );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ncells         <DATA> Number of cells
 * <DATA> OUT <DATA> mat            <DATA> Material data array
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * <DATA> OUT <DATA> nmixcell       <DATA> Number of mixed cells
 * <DATA> OUT <DATA> nmixcomp       <DATA> Number of mixed components
 * <DATA> OUT <DATA> ncomp          <DATA> The mixed material num-component array
 * <DATA> OUT <DATA> ptr            <DATA> The mixed material mat-pointer array
 * <DATA> OUT <DATA> mixmat         <DATA> The mixed material mat-number array
 * <DATA> OUT <DATA> vf             <DATA> The mixed material volumn fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_UnstrMaterial_All(     const TIO_File_t   fileID,
                                             const TIO_Object_t materialID,
                                             const TIO_Data_t   datatype,
                                             TIO_Size_t         *ncells,
                                             void               *mat,
                                             const TIO_Data_t   ncomp_datatype,
                                             const TIO_Data_t   ptr_datatype,
                                             const TIO_Data_t   vf_datatype,
                                             TIO_Size_t         *nmixcell,
                                             TIO_Size_t         *nmixcomp,
                                             void               *ncomp,
                                             void               *ptr,
                                             void               *mixmat,
                                             void               *vf );

/***************************************************************************************************
 *
 * TIO_Read_UnstrMaterial_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrMaterial_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a unstructured mesh section's material data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrMaterial_Section( const TIO_File_t   fileID,
 * <DATA>                                       const TIO_Object_t materialID,
 * <DATA>                                       const TIO_Object_t sectionID,
 * <DATA>                                       const TIO_Xfer_t   xfer,
 * <DATA>                                       const TIO_Data_t   datatype,
 * <DATA>                                       TIO_Size_t         *ncells,
 * <DATA>                                       void               *mat,
 * <DATA>                                       const TIO_Data_t   ncomp_datatype,
 * <DATA>                                       const TIO_Data_t   ptr_datatype,
 * <DATA>                                       const TIO_Data_t   vf_datatype,
 * <DATA>                                       TIO_Size_t         *nmixcell,
 * <DATA>                                       TIO_Size_t         *nmixcomp,
 * <DATA>                                       void               *ncomp,
 * <DATA>                                       void               *ptr,
 * <DATA>                                       void               *mixmat,
 * <DATA>                                       void               *vf );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID     <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> sectionID      <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer           <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype       <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ncells         <DATA> Number of cells
 * <DATA> OUT <DATA> mat            <DATA> Material data array
 * <DATA> IN  <DATA> ncomp_datatype <DATA> Datatype of num-component array
 * <DATA> IN  <DATA> ptr_datatype   <DATA> Datatype of mat-pointer array
 * <DATA> IN  <DATA> vf_datatype    <DATA> Datatype of volume-fraction array
 * <DATA> OUT <DATA> nmixcell       <DATA> Number of mixed cells
 * <DATA> OUT <DATA> nmixcomp       <DATA> Number of mixed components
 * <DATA> OUT <DATA> ncomp          <DATA> The mixed material num-component array
 * <DATA> OUT <DATA> ptr            <DATA> The mixed material mat-pointer array
 * <DATA> OUT <DATA> mixmat         <DATA> The mixed material mat-number array
 * <DATA> OUT <DATA> vf             <DATA> The mixed material volumn fraction array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_UnstrMaterial_Section( const TIO_File_t   fileID,
                                             const TIO_Object_t materialID,
                                             const TIO_Object_t sectionID,
                                             const TIO_Xfer_t   xfer,
                                             const TIO_Data_t   datatype,
                                             TIO_Size_t         *ncells,
                                             void               *mat,
                                             const TIO_Data_t   ncomp_datatype,
                                             const TIO_Data_t   ptr_datatype,
                                             const TIO_Data_t   vf_datatype,
                                             TIO_Size_t         *nmixcell,
                                             TIO_Size_t         *nmixcomp,
                                             void               *ncomp,
                                             void               *ptr,
                                             void               *mixmat,
                                             void               *vf );


/**************************************
 **************************************
 **
 **   POINT MATERIALS
 **
 **************************************
 **************************************/

/***************************************************************************************************
 *
 * TIO_Write_PointMaterial_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_PointMaterial_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write a point mesh chunk's material data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_PointMaterial_Chunk( const TIO_File_t   fileID,
 * <DATA>                                      const TIO_Object_t materialID,
 * <DATA>                                      const TIO_Size_t   idx,
 * <DATA>                                      const TIO_Xfer_t   xfer,
 * <DATA>                                      const TIO_Data_t   datatype,
 * <DATA>                                      const void         *mat );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID     <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> idx        <DATA> Chunk index
 * <DATA> IN  <DATA> xfer       <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype   <DATA> TyphonIO data type
 * <DATA> IN  <DATA> mat        <DATA> Material data array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Write_PointMaterial_Chunk(  const TIO_File_t   fileID,
                                             const TIO_Object_t materialID,
                                             const TIO_Size_t   idx,
                                             const TIO_Xfer_t   xfer,
                                             const TIO_Data_t   datatype,
                                             const void         *mat );

/***************************************************************************************************
 *
 * TIO_Read_PointMaterial_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointMaterial_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a chunks material data for a point mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointMaterial_Chunk( const TIO_File_t   fileID,
 * <DATA>                                     const TIO_Object_t materialID,
 * <DATA>                                     const TIO_Size_t   idx,
 * <DATA>                                     const TIO_Xfer_t   xfer,
 * <DATA>                                     const TIO_Data_t   datatype,
 * <DATA>                                     const TIO_Size_t   nghosts,
 * <DATA>                                     TIO_Size_t         *nnodes,
 * <DATA>                                     void               *mat );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID     <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> idx        <DATA> Chunk index
 * <DATA> IN  <DATA> xfer       <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype   <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts    <DATA> Number of ghosts
 * <DATA> OUT <DATA> nnodes     <DATA> Number of nodes
 * <DATA> OUT <DATA> mat        <DATA> Material data array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_PointMaterial_Chunk(   const TIO_File_t   fileID,
                                             const TIO_Object_t materialID,
                                             const TIO_Size_t   idx,
                                             const TIO_Xfer_t   xfer,
                                             const TIO_Data_t   datatype,
                                             const TIO_Size_t   nghosts,
                                             TIO_Size_t         *nnodes,
                                             void               *mat );

/***************************************************************************************************
 *
 * TIO_Read_PointMaterial_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointMaterial_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read all the material data for a point mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointMaterial_All( const TIO_File_t   fileID,
 * <DATA>                                   const TIO_Object_t materialID,
 * <DATA>                                   const TIO_Data_t   datatype,
 * <DATA>                                   TIO_Size_t         *nnodes,
 * <DATA>                                   void               *mat );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID     <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> datatype   <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nnodes     <DATA> Number of nodes
 * <DATA> OUT <DATA> mat        <DATA> Material data array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_PointMaterial_All(     const TIO_File_t   fileID,
                                             const TIO_Object_t materialID,
                                             const TIO_Data_t   datatype,
                                             TIO_Size_t         *nnodes,
                                             void               *mat );

/***************************************************************************************************
 *
 * TIO_Read_PointMaterial_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointMaterial_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a point mesh material section.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointMaterial_Section( const TIO_File_t   fileID,
 * <DATA>                                       const TIO_Object_t materialID,
 * <DATA>                                       const TIO_Object_t sectionID,
 * <DATA>                                       const TIO_Xfer_t   xfer,
 * <DATA>                                       const TIO_Data_t   datatype,
 * <DATA>                                       TIO_Size_t         *nnodes,
 * <DATA>                                       void               *mat );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID     <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> materialID <DATA> TyphonIO material ID
 * <DATA> IN  <DATA> sectionID  <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer       <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype   <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nnodes     <DATA> Number of nodes
 * <DATA> OUT <DATA> mat        <DATA> Material data array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 *
 * </ITEM>
 *
 **************************************************************************************************/
extern TIO_t TIO_Read_PointMaterial_Section( const TIO_File_t   fileID,
                                             const TIO_Object_t materialID,
                                             const TIO_Object_t sectionID,
                                             const TIO_Xfer_t   xfer,
                                             const TIO_Data_t   datatype,
                                             TIO_Size_t         *nnodes,
                                             void               *mat );


#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */

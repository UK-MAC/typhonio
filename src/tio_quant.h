/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_QUANT_H
#define _TIO_QUANT_H

#ifdef __cplusplus
extern "C" {
#endif



/***************************************************************************************************
 *
 * TIO_List_Quants
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_List_Quants
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the number of Quants within a TyphonIO file or object and their names
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_List_Quants( const TIO_File_t   fileID,
 * <DATA>                        const TIO_Object_t meshID,
 * <DATA>                        TIO_Size_t         *nobj,
 * <DATA>                        char               names[][TIO_STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> OUT <DATA> nobj     <DATA> Number of Quants
 * <DATA> OUT <DATA> names    <DATA> Array of names
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 * <CDES>
 * <DATA> <code>names</codes> can be <code>NULL</code>
 * </CDES>
 *
 * </ITEM>
 */
extern TIO_t TIO_List_Quants( const TIO_File_t   fileID,
                              const TIO_Object_t meshID,
                              TIO_Size_t         *nobj,
                              char               names[][TIO_STRLEN] );


/***************************************************************************************************
 *
 * TIO_Create_Quant
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Create_Quant
 * </NAME>
 *
 * <PURP>
 * <DATA> Creates a new Quant within a Mesh
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Create_Quant( const TIO_File_t   fileID,
 * <DATA>                         const TIO_Object_t meshID,
 * <DATA>                         const char         name[],
 * <DATA>                         TIO_Object_t       *quantID,
 * <DATA>                         const TIO_Data_t   datatype,
 * <DATA>                         const TIO_Centre_t centring,
 * <DATA>                         const TIO_Size_t   nghosts,
 * <DATA>                         const TIO_Bool_t   ismixed,
 * <DATA>                         const char         units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> name     <DATA> Name for Quant
 * <DATA> OUT <DATA> quantID  <DATA> ID of new Quant
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO datatype of Quant
 * <DATA> IN  <DATA> centring <DATA> Data centring of Quant
 * <DATA> IN  <DATA> nghosts  <DATA> Number of ghosts in Quant
 * <DATA> IN  <DATA> ismixed  <DATA> Flag whether Quant is mixed
 * <DATA> IN  <DATA> units    <DATA> String of Quant units
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> <code>ismixed</code> is global
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Create_Quant( const TIO_File_t   fileID,
                               const TIO_Object_t meshID,
                               const char         name[],
                               TIO_Object_t       *quantID,
                               const TIO_Data_t   datatype,
                               const TIO_Centre_t centring,
                               const TIO_Size_t   nghosts,
                               const TIO_Bool_t   ismixed,
                               const char         units[] );


/***************************************************************************************************
 *
 * TIO_Open_Quant
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Quant
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens a Quant from within a Mesh by name
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Quant( const TIO_File_t   fileID,
 * <DATA>                       const TIO_Object_t meshID,
 * <DATA>                       const char         name[],
 * <DATA>                       TIO_Object_t       *quantID,
 * <DATA>                       TIO_Data_t         *datatype,
 * <DATA>                       TIO_Centre_t       *centring,
 * <DATA>                       TIO_Size_t         *nghosts,
 * <DATA>                       TIO_Bool_t         *ismixed,
 * <DATA>                       char               units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> name     <DATA> Name for Quant
 * <DATA> OUT <DATA> quantID  <DATA> ID of new Quant
 * <DATA> OUT <DATA> datatype <DATA> TyphonIO datatype of Quant
 * <DATA> OUT <DATA> centring <DATA> Data centring of Quant
 * <DATA> OUT <DATA> nghosts  <DATA> Number of ghosts in Quant
 * <DATA> OUT <DATA> ismixed  <DATA> Flag whether Quant is mixed
 * <DATA> OUT <DATA> units    <DATA> String of Quant units
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> <code>ismixed</code> is global
 * <DATA>
 * <DATA> Must be called collectively.
 * <DATA>
 * <DATA> Only TIO_CENTRE_NODE can be used for centring with point meshes.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Quant( const TIO_File_t   fileID,
                             const TIO_Object_t meshID,
                             const char         name[],
                             TIO_Object_t       *quantID,
                             TIO_Data_t         *datatype,
                             TIO_Centre_t       *centring,
                             TIO_Size_t         *nghosts,
                             TIO_Bool_t         *ismixed,
                             char               units[] );


/***************************************************************************************************
 *
 * TIO_Open_Quant_idx
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Quant_idx
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens a Quant from within a Mesh by index
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Quant_idx( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t meshID,
 * <DATA>                           const TIO_Size_t   idx,
 * <DATA>                           const char         name[],
 * <DATA>                           TIO_Object_t       *quantID,
 * <DATA>                           TIO_Data_t         *datatype,
 * <DATA>                           TIO_Centre_t       *centring,
 * <DATA>                           TIO_Size_t         *nghosts,
 * <DATA>                           TIO_Bool_t         *ismixed,
 * <DATA>                           char               units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> name     <DATA> Name for Quant
 * <DATA> OUT <DATA> quantID  <DATA> ID of new Quant
 * <DATA> OUT <DATA> datatype <DATA> TyphonIO datatype of Quant
 * <DATA> OUT <DATA> centring <DATA> Data centring of Quant
 * <DATA> OUT <DATA> nghosts  <DATA> Number of ghosts in Quant
 * <DATA> OUT <DATA> ismixed  <DATA> Flag whether Quant is mixed
 * <DATA> OUT <DATA> units    <DATA> String of Quant units
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> <code>ismixed</code> is global
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Quant_idx( const TIO_File_t   fileID,
                                 const TIO_Object_t meshID,
                                 const TIO_Size_t   idx,
                                 char               name[],
                                 TIO_Object_t       *quantID,
                                 TIO_Data_t         *datatype,
                                 TIO_Centre_t       *centring,
                                 TIO_Size_t         *nghosts,
                                 TIO_Bool_t         *ismixed,
                                 char               units[] );


/***************************************************************************************************
 *
 * TIO_Get_Quant_Info
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Quant_Info
 * </NAME>
 *
 * <PURP>
 * <DATA> Gets the info of an open Quant
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Quant_Info( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t quantID,
 * <DATA>                           const char         name[],
 * <DATA>                           TIO_Data_t         *datatype,
 * <DATA>                           TIO_Centre_t       *centring,
 * <DATA>                           TIO_Size_t         *nghosts,
 * <DATA>                           TIO_Bool_t         *ismixed,
 * <DATA>                           char               units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO Quant ID
 * <DATA> IN  <DATA> name     <DATA> Name for Quant
 * <DATA> OUT <DATA> datatype <DATA> TyphonIO datatype of Quant
 * <DATA> OUT <DATA> centring <DATA> Data centring of Quant
 * <DATA> OUT <DATA> nghosts  <DATA> Number of ghosts in Quant
 * <DATA> OUT <DATA> ismixed  <DATA> Flag whether Quant is mixed
 * <DATA> OUT <DATA> units    <DATA> String of Quant units
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> <code>ismixed</code> is global
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_Quant_Info( const TIO_File_t   fileID,
                                 const TIO_Object_t quantID,
                                 char               name[],
                                 TIO_Data_t         *datatype,
                                 TIO_Centre_t       *centring,
                                 TIO_Size_t         *nghosts,
                                 TIO_Bool_t         *ismixed,
                                 char               units[] );

/***************************************************************************************************
 *
 * TIO_Close_Quant
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Close_Quant
 * </NAME>
 *
 * <PURP>
 * <DATA> Closes an open Quant
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Close_Quant( const TIO_File_t   fileID,
 * <DATA>                        const TIO_Object_t quantID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN <DATA> quantID  <DATA> TyphonIO Quant ID
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> <code>ismixed</code> is global
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Close_Quant( const TIO_File_t   fileID,
                              const TIO_Object_t quantID );


/***************************************************************************************************
 *
 * TIO_Set_Quant_Range
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Set_Quant_Range
 * </NAME>
 *
 * <PURP>
 * <DATA> Sets the range to display the given quant between.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Set_Quant_Range( const TIO_File_t   fileID,
 * <DATA>                            const TIO_Object_t quantID,
 * <DATA>                            const TIO_Data_t   datatype,
 * <DATA>                            const void         *qmin,
 * <DATA>                            const void         *qmax );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> qmin     <DATA> The minimum value of the quant to display
 * <DATA> IN  <DATA> qmax     <DATA> The maximum value of the quant to display
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
extern TIO_t TIO_Set_Quant_Range( const TIO_File_t   fileID,
                                  const TIO_Object_t quantID,
                                  const TIO_Data_t   datatype,
                                  const void         *qmin,
                                  const void         *qmax );

/***************************************************************************************************
 *
 * TIO_Get_Quant_Range
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Quant_Range
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the range that the given quant should be displayed between.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Quant_Range( const TIO_File_t   fileID,
 * <DATA>                            const TIO_Object_t quantID,
 * <DATA>                            const TIO_Data_t   datatype,
 * <DATA>                            void               *qmin,
 * <DATA>                            void               *qmax );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> OUT <DATA> qmin     <DATA> The minimum value of the quant to display
 * <DATA> OUT <DATA> qmax     <DATA> The maximum value of the quant to display
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
extern TIO_t TIO_Get_Quant_Range( const TIO_File_t   fileID,
                                  const TIO_Object_t quantID,
                                  const TIO_Data_t   datatype,
                                  void               *qmin,
                                  void               *qmax );



/**************************************
 **************************************
 **
 **   QUAD QUANTS
 **
 **************************************
 **************************************/

/***************************************************************************************************
 *
 * TIO_Write_QuadQuant_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_QuadQuant_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write the quant data for a structured mesh chunk.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_QuadQuant_Chunk( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t quantID,
 * <DATA>                                  const TIO_Size_t   idx,
 * <DATA>                                  const TIO_Xfer_t   xfer,
 * <DATA>                                  const TIO_Data_t   datatype,
 * <DATA>                                  const void         *qdat,
 * <DATA>                                  const void         *qmix );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> qdat     <DATA> The quant data array
 * <DATA> IN  <DATA> qmix     <DATA> The quant mixed data array
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
extern TIO_t TIO_Write_QuadQuant_Chunk(   const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Size_t   idx,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          const void         *qdat,
                                          const void         *qmix );

/***************************************************************************************************
 *
 * TIO_Read_QuadQuant_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadQuant_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the structured mesh chunk's quant data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadQuant_Chunk( const TIO_File_t   fileID,
 * <DATA>                                 const TIO_Object_t quantID,
 * <DATA>                                 const TIO_Size_t   idx,
 * <DATA>                                 const TIO_Xfer_t   xfer,
 * <DATA>                                 const TIO_Data_t   datatype,
 * <DATA>                                 const TIO_Size_t   nghosts,
 * <DATA>                                 TIO_Dims_t         *ndims,
 * <DATA>                                 TIO_Size_t         *ni,
 * <DATA>                                 TIO_Size_t         *nj,
 * <DATA>                                 TIO_Size_t         *nk,
 * <DATA>                                 void               *qdat,
 * <DATA>                                 TIO_Size_t         *nmixcomp,
 * <DATA>                                 void               *qmix );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts  <DATA> Number of ghosts
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimensions
 * <DATA> OUT <DATA> ni       <DATA> Size of chunk in the 1st dimension
 * <DATA> OUT <DATA> nj       <DATA> Size of chunk in the 2nd dimension
 * <DATA> OUT <DATA> nk       <DATA> Size of chunk in the 3rd dimension
 * <DATA> OUT <DATA> qdat     <DATA> The quant data array
 * <DATA> OUT <DATA> nmixcomp <DATA> Number of mixed components
 * <DATA> OUT <DATA> qmix     <DATA> The quant mixed data array
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
extern TIO_t TIO_Read_QuadQuant_Chunk(    const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Size_t   idx,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          const TIO_Size_t   nghosts,
                                          TIO_Dims_t         *ndims,
                                          TIO_Size_t         *ni,
                                          TIO_Size_t         *nj,
                                          TIO_Size_t         *nk,
                                          void               *qdat,
                                          TIO_Size_t         *nmixcomp,
                                          void               *qmix );

/***************************************************************************************************
 *
 * TIO_Read_QuadQuant_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadQuant_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the entire structured mesh's quant data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadQuant_All( const TIO_File_t   fileID,
 * <DATA>                               const TIO_Object_t quantID,
 * <DATA>                               const TIO_Data_t   datatype,
 * <DATA>                               TIO_Dims_t         *ndims,
 * <DATA>                               TIO_Size_t         *ni,
 * <DATA>                               TIO_Size_t         *nj,
 * <DATA>                               TIO_Size_t         *nk,
 * <DATA>                               void               *qdat,
 * <DATA>                               TIO_Size_t         *nmixcomp,
 * <DATA>                               void               *qmix );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimensions
 * <DATA> OUT <DATA> ni       <DATA> Size of mesh in the 1st dimension
 * <DATA> OUT <DATA> nj       <DATA> Size of mesh in the 2nd dimension
 * <DATA> OUT <DATA> nk       <DATA> Size of mesh in the 3rd dimension
 * <DATA> OUT <DATA> qdat     <DATA> The quant data array
 * <DATA> OUT <DATA> nmixcomp <DATA> Number of mixed components
 * <DATA> OUT <DATA> qmix     <DATA> The quant mixed data array
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
extern TIO_t TIO_Read_QuadQuant_All(      const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Data_t   datatype,
                                          TIO_Dims_t         *ndims,
                                          TIO_Size_t         *ni,
                                          TIO_Size_t         *nj,
                                          TIO_Size_t         *nk,
                                          void               *qdat,
                                          TIO_Size_t         *nmixcomp,
                                          void               *qmix );

/***************************************************************************************************
 *
 * TIO_Read_QuadQuant_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadQuant_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the structured mesh section's quant data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadQuant_Section( const TIO_File_t   fileID,
 * <DATA>                                   const TIO_Object_t quantID,
 * <DATA>                                   const TIO_Object_t sectionID,
 * <DATA>                                   const TIO_Xfer_t   xfer,
 * <DATA>                                   const TIO_Data_t   datatype,
 * <DATA>                                   TIO_Dims_t         *ndims,
 * <DATA>                                   TIO_Size_t         *ni,
 * <DATA>                                   TIO_Size_t         *nj,
 * <DATA>                                   TIO_Size_t         *nk,
 * <DATA>                                   void               *qdat,
 * <DATA>                                   TIO_Size_t         *nmixcomp,
 * <DATA>                                   void               *qmix );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID    <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID   <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> sectionID <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer      <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype  <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims     <DATA> Number of dimensions
 * <DATA> OUT <DATA> ni        <DATA> Size of section in the 1st dimension
 * <DATA> OUT <DATA> nj        <DATA> Size of section in the 2nd dimension
 * <DATA> OUT <DATA> nk        <DATA> Size of section in the 3rd dimension
 * <DATA> OUT <DATA> qdat      <DATA> The quant data array
 * <DATA> OUT <DATA> nmixcomp  <DATA> Number of mixed components
 * <DATA> OUT <DATA> qmix      <DATA> The quant mixed data array
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
extern TIO_t TIO_Read_QuadQuant_Section(  const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Object_t sectionID,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          TIO_Dims_t         *ndims,
                                          TIO_Size_t         *ni,
                                          TIO_Size_t         *nj,
                                          TIO_Size_t         *nk,
                                          void               *qdat,
                                          TIO_Size_t         *nmixcomp,
                                          void               *qmix );



/**************************************
 **************************************
 **
 **   UNSTR QUANTS
 **
 **************************************
 **************************************/

/***************************************************************************************************
 *
 * TIO_Write_UnstrQuant_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_UnstrQuant_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write the quant data for an unstructured mesh chunk.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_UnstrQuant_Chunk( const TIO_File_t   fileID,
 * <DATA>                                   const TIO_Object_t quantID,
 * <DATA>                                   const TIO_Size_t   idx,
 * <DATA>                                   const TIO_Xfer_t   xfer,
 * <DATA>                                   const TIO_Data_t   datatype,
 * <DATA>                                   const void         *qdat,
 * <DATA>                                   const void         *qmix );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> qdat     <DATA> The quant data array
 * <DATA> IN  <DATA> qmix     <DATA> The quant mixed data array
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
extern TIO_t TIO_Write_UnstrQuant_Chunk(  const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Size_t   idx,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          const void         *qdat,
                                          const void         *qmix );

/***************************************************************************************************
 *
 * TIO_Read_UnstrQuant_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrQuant_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the unstructured mesh chunk's quant data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrQuant_Chunk( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t quantID,
 * <DATA>                                  const TIO_Size_t   idx,
 * <DATA>                                  const TIO_Xfer_t   xfer,
 * <DATA>                                  const TIO_Data_t   datatype,
 * <DATA>                                  const TIO_Size_t   nghosts,
 * <DATA>                                  TIO_Size_t         *nn,
 * <DATA>                                  void               *qdat,
 * <DATA>                                  TIO_Size_t         *nmixcomp,
 * <DATA>                                  void               *qmix );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts  <DATA> Number of ghosts
 * <DATA> OUT <DATA> nn       <DATA> Number of nodes
 * <DATA> OUT <DATA> qdat     <DATA> The quant data array
 * <DATA> OUT <DATA> nmixcomp <DATA> Number of mixed components
 * <DATA> OUT <DATA> qmix     <DATA> The quant mixed data array
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
extern TIO_t TIO_Read_UnstrQuant_Chunk(   const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Size_t   idx,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          const TIO_Size_t   nghosts,
                                          TIO_Size_t         *nn,       /* cells/nodes wrt centring */
                                          void               *qdat,
                                          TIO_Size_t         *nmixcomp,
                                          void               *qmix );

/***************************************************************************************************
 *
 * TIO_Read_UnstrQuant_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrQuant_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the entire unstructured mesh's quant data
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrQuant_All( const TIO_File_t   fileID,
 * <DATA>                                const TIO_Object_t quantID,
 * <DATA>                                const TIO_Data_t   datatype,
 * <DATA>                                TIO_Size_t         *nn,
 * <DATA>                                void               *qdat,
 * <DATA>                                TIO_Size_t         *nmixcomp,
 * <DATA>                                void               *qmix );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nn       <DATA> Number of nodes
 * <DATA> OUT <DATA> qdat     <DATA> The quant data array
 * <DATA> OUT <DATA> nmixcomp <DATA> Number of mixed components
 * <DATA> OUT <DATA> qmix     <DATA> The quant mixed data array
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
extern TIO_t TIO_Read_UnstrQuant_All(     const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Data_t   datatype,
                                          TIO_Size_t         *nn,       /* cells/nodes wrt centring */
                                          void               *qdat,
                                          TIO_Size_t         *nmixcomp,
                                          void               *qmix );

/***************************************************************************************************
 *
 * TIO_Read_UnstrQuant_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrQuant_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the unstructured mesh section's quant data
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrQuant_Section( const TIO_File_t   fileID,
 * <DATA>                                    const TIO_Object_t quantID,
 * <DATA>                                    const TIO_Object_t sectionID,
 * <DATA>                                    const TIO_Xfer_t   xfer,
 * <DATA>                                    const TIO_Data_t   datatype,
 * <DATA>                                    TIO_Size_t         *nn,
 * <DATA>                                    void               *qdat,
 * <DATA>                                    TIO_Size_t         *nmixcomp,
 * <DATA>                                    void               *qmix );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID    <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID   <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> sectionID <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer      <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype  <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nn        <DATA> Number of nodes
 * <DATA> OUT <DATA> qdat      <DATA> The quant data array
 * <DATA> OUT <DATA> nmixcomp  <DATA> Number of mixed components
 * <DATA> OUT <DATA> qmix      <DATA> The quant mixed data array
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
extern TIO_t TIO_Read_UnstrQuant_Section( const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Object_t sectionID,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          TIO_Size_t         *nn,
                                          void               *qdat,
                                          TIO_Size_t         *nmixcomp,
                                          void               *qmix );



/**************************************
 **************************************
 **
 **   POINT QUANTS
 **
 **************************************
 **************************************/

/***************************************************************************************************
 *
 * TIO_Write_PointQuant_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_PointQuant_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write the quant data for a point mesh chunk.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_PointQuant_Chunk( const TIO_File_t   fileID,
 * <DATA>                                   const TIO_Object_t quantID,
 * <DATA>                                   const TIO_Size_t   idx,
 * <DATA>                                   const TIO_Xfer_t   xfer,
 * <DATA>                                   const TIO_Data_t   datatype,
 * <DATA>                                   const void         *qdat );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> qdat     <DATA> The quant data array
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
extern TIO_t TIO_Write_PointQuant_Chunk(  const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Size_t   idx,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          const void         *qdat );

/***************************************************************************************************
 *
 * TIO_Read_PointQuant_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointQuant_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the point mesh chunk's quant data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointQuant_Chunk( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t quantID,
 * <DATA>                                  const TIO_Size_t   idx,
 * <DATA>                                  const TIO_Xfer_t   xfer,
 * <DATA>                                  const TIO_Data_t   datatype,
 * <DATA>                                  const TIO_Size_t   nghosts,
 * <DATA>                                  TIO_Size_t         *nnodes,
 * <DATA>                                  void               *qdat );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts  <DATA> Number of ghosts
 * <DATA> OUT <DATA> nnodes   <DATA> Number of nodes
 * <DATA> OUT <DATA> qdat     <DATA> The quant data array
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
extern TIO_t TIO_Read_PointQuant_Chunk(   const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Size_t   idx,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          const TIO_Size_t   nghosts,
                                          TIO_Size_t         *nnodes,
                                          void               *qdat );

/***************************************************************************************************
 *
 * TIO_Read_PointQuant_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointQuant_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the entire point mesh's quant data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointQuant_All( const TIO_File_t   fileID,
 * <DATA>                                const TIO_Object_t quantID,
 * <DATA>                                const TIO_Data_t   datatype,
 * <DATA>                                TIO_Size_t         *nnodes,
 * <DATA>                                void               *qdat );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID  <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nnodes   <DATA> Number of nodes
 * <DATA> OUT <DATA> qdat     <DATA> The quant data array
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
extern TIO_t TIO_Read_PointQuant_All(     const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Data_t   datatype,
                                          TIO_Size_t         *nnodes,
                                          void               *qdat );

/***************************************************************************************************
 *
 * TIO_Read_PointQuant_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointQuant_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read the point mesh section's quant data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointQuant_Section( const TIO_File_t   fileID,
 * <DATA>                                    const TIO_Object_t quantID,
 * <DATA>                                    const TIO_Object_t sectionID,
 * <DATA>                                    const TIO_Xfer_t   xfer,
 * <DATA>                                    const TIO_Data_t   datatype,
 * <DATA>                                    TIO_Size_t         *nnodes,
 * <DATA>                                    void               *qdat );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID    <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> quantID   <DATA> TyphonIO quant ID
 * <DATA> IN  <DATA> sectionID <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer      <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype  <DATA> TyphonIO data type
 * <DATA> OUT <DATA> nnodes    <DATA> Number of nodes
 * <DATA> OUT <DATA> qdat      <DATA> The quant data array
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
extern TIO_t TIO_Read_PointQuant_Section( const TIO_File_t   fileID,
                                          const TIO_Object_t quantID,
                                          const TIO_Object_t sectionID,
                                          const TIO_Xfer_t   xfer,
                                          const TIO_Data_t   datatype,
                                          TIO_Size_t         *nnodes,
                                          void               *qdat );




#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */

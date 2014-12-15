/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */
#ifndef _TIO_VIZ_H
#define _TIO_VIZ_H

#ifdef __cplusplus
extern "C" {
#endif


#include <typhonio.h>


#define TIO_GHOSTS_INTERNAL ((TIO_Size_t)(TIO_GHOSTS_ALL  - 1))
  /*
    For structured    - reads 1 ghost regardless of number present in
      mat/quant chunk - gets from other chunks if necessary
      Only reads internal ghosts.  Ghosts lying outside the mesh are ignored.

    For unstructured  - NOT YET IMPLEMENETED

      PLANNED:          read all internal ghosts defined with the mesh,
      reconstructing ghosts from other chunk's data if necessary for mat/quant

      without any ghosts.  Can't reconstruct if no ghosts where in the mesh though.
      Assumes that no ghosts lie outside the mesh for unstructured data, unlike quadmeshes.
  */

#define TIO_GHOSTS_ONLY     ((TIO_Size_t)(TIO_GHOSTS_ALL  - 2))
  /*
    - can only be used in TIO_Read_Shaped_*()
  */


/***************************************************************************************************
 *
 * TIO_Disable_Fatality
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Disable_Fatality
 * </NAME>
 *
 * <PURP>
 * <DATA> Set typhonIO to never abort on error.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Disable_Fatality( void );
 * </CAPI>
 *
 * <PARA>
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> The environmental variable ENV_ERRORS_FATAL is ignored for all code after this function is called.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Disable_Fatality( void );

/***************************************************************************************************
 *
 * TIO_Get_UnstrMesh_nghosts
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_UnstrMesh_nghosts
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the number of ghosts for the unstructured mesh given.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_UnstrMesh_nghosts( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t meshID,
 * <DATA>                                  TIO_Size_t         *nghosts );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID  <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID  <DATA> TyphonIO Mesh ID
 * <DATA> OUT <DATA> nghosts <DATA> Number of ghosts
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_UnstrMesh_nghosts( const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        TIO_Size_t         *nghosts );

/***************************************************************************************************
 *
 * TIO_Get_PointMesh_nghosts
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_PointMesh_nghosts
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the number of ghosts for the point mesh given.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_PointMesh_nghosts( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t meshID,
 * <DATA>                                  TIO_Size_t         *nghosts );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID  <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID  <DATA> TyphonIO Mesh ID
 * <DATA> OUT <DATA> nghosts <DATA> Number of ghosts
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_PointMesh_nghosts( const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        TIO_Size_t         *nghosts );

/***************************************************************************************************
 *
 * TIO_Read_Shaped_Cells
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_Shaped_Cells
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns all the cell data from a unstructured mesh, mat or quant with the specified shape.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_Shaped_Cells( const TIO_File_t   fileID,
 * <DATA>                              const TIO_Object_t objectID,
 * <DATA>                              const TIO_Size_t   idx,
 * <DATA>                              const TIO_Xfer_t   xfer,
 * <DATA>                              const TIO_Shape_t  shape,
 * <DATA>                              const TIO_Data_t   datatype,
 * <DATA>                              const TIO_Size_t   nghosts,
 * <DATA>                              TIO_Size_t         *ncells,
 * <DATA>                              void               *data );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID (mesh, mat or quant)
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> shape    <DATA> Shape of cells to retrieve
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts  <DATA> Number of ghosts
 * <DATA> OUT <DATA> ncells   <DATA> Number of cells found
 * <DATA> OUT <DATA> data     <DATA> Cell data
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * </DESC>
 *
 * <CAPI>
 * <DATA> data can be <code>NULL</code>
 * </CAPI>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_Shaped_Cells(        const TIO_File_t   fileID,
                                           const TIO_Object_t objectID,  /* mesh, mat, quant */
                                           const TIO_Size_t   idx,
                                           const TIO_Xfer_t   xfer,
                                           const TIO_Shape_t  shape,
                                           const TIO_Data_t   datatype,
                                           const TIO_Size_t   nghosts,
                                           TIO_Size_t         *ncells,
                                           void               *data );

/***************************************************************************************************
 *
 * TIO_Read_Shaped_Connectivity
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_Shaped_Connectivity
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns all the connectivity data from a unstructured mesh with the specified shape.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_Shaped_Connectivity( const TIO_File_t   fileID,
 * <DATA>                                     const TIO_Object_t meshID,
 * <DATA>                                     const TIO_Size_t   idx,
 * <DATA>                                     const TIO_Xfer_t   xfer,
 * <DATA>                                     const TIO_Shape_t  shape,
 * <DATA>                                     const TIO_Data_t   datatype,
 * <DATA>                                     const TIO_Size_t   nghosts,
 * <DATA>                                     TIO_Size_t         *nconnectivity,
 * <DATA>                                     void               *connectivity );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID        <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID        <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> idx           <DATA> Chunk index
 * <DATA> IN  <DATA> xfer          <DATA> Data transfer method
 * <DATA> IN  <DATA> shape         <DATA> Shape of cells to retrieve the connectivity for
 * <DATA> IN  <DATA> datatype      <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts       <DATA> Number of ghosts
 * <DATA> OUT <DATA> nconnectivity <DATA> Size of connectivity array for found shapes
 * <DATA> OUT <DATA> connectivity  <DATA> Connectivity array for found shapes
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_Shaped_Connectivity( const TIO_File_t   fileID,
                                           const TIO_Object_t meshID,
                                           const TIO_Size_t   idx,
                                           const TIO_Xfer_t   xfer,
                                           const TIO_Shape_t  shape,
                                           const TIO_Data_t   datatype,
                                           const TIO_Size_t   nghosts,
                                           TIO_Size_t         *nconnectivity,
                                           void               *connectivity );


#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */

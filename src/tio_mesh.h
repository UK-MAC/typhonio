/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_MESH_H
#define _TIO_MESH_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
 *
 * TIO_List_Meshes
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_List_Meshes
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the meshes stored in a given state
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_List_Meshes( const TIO_File_t   fileID,
 * <DATA>                        const TIO_Object_t stateID,
 * <DATA>                        TIO_Size_t         *nobj,
 * <DATA>                        char               names[][TIO_STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID  <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> stateID <DATA> TyphonIO state ID
 * <DATA> OUT <DATA> nobj    <DATA> Number of meshes
 * <DATA> OUT <DATA> names   <DATA> The mesh names
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
 * <DATA>names can be <code>NULL</code>
 * </CAPI>
 *
 * </ITEM>
 */
extern TIO_t TIO_List_Meshes(   const TIO_File_t   fileID,
                                const TIO_Object_t stateID,
                                TIO_Size_t         *nobj,
                                char               names[][TIO_STRLEN] );

/***************************************************************************************************
 *
 * TIO_Create_Mesh
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Create_Mesh
 * </NAME>
 *
 * <PURP>
 * <DATA> Create a mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Create_Mesh( const TIO_File_t   fileID,
 * <DATA>                        const TIO_Object_t stateID,
 * <DATA>                        const char         name[],
 * <DATA>                        TIO_Object_t       *meshID,
 * <DATA>                        const TIO_Mesh_t   meshtype,
 * <DATA>                        const TIO_Coord_t  coordtype,
 * <DATA>                        const TIO_Bool_t   isAMR,
 * <DATA>                        const char         group[],
 * <DATA>                        const TIO_Size_t   order,
 * <DATA>                        const TIO_Data_t   graph_datatype,
 * <DATA>                        const TIO_Data_t   coord_datatype,
 * <DATA>                        const TIO_Dims_t   ndims,
 * <DATA>                        const TIO_Size_t   n1,
 * <DATA>                        const TIO_Size_t   n2,
 * <DATA>                        const TIO_Size_t   n3,
 * <DATA>                        const TIO_Size_t   n4,
 * <DATA>                        const TIO_Size_t   nchunks,
 * <DATA>                        const char         iunits[],
 * <DATA>                        const char         junits[],
 * <DATA>                        const char         kunits[],
 * <DATA>                        const char         ilabel[],
 * <DATA>                        const char         jlabel[],
 * <DATA>                        const char         klabel[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> stateID        <DATA> TyphonIO state ID
 * <DATA> IN  <DATA> name           <DATA> Mesh name
 * <DATA> OUT <DATA> meshID         <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> meshtype       <DATA> TyphonIO mesh type
 * <DATA> IN  <DATA> coordtype      <DATA> Coordinate system to use
 * <DATA> IN  <DATA> isAMR          <DATA> Whether AMR is used with this mesh
 * <DATA> IN  <DATA> group          <DATA> Used for identifying meshes which are related somehow, i.e. AMR levels or Laser Beams
 * <DATA> IN  <DATA> order          <DATA> Used to make meshes unique within a group
 * <DATA> IN  <DATA> graph_datatype <DATA> Datatype of connectivity, node and cell IDs (only used for unstructured meshes)
 * <DATA> IN  <DATA> coord_datatype <DATA> Datatype of the coordinate system
 * <DATA> IN  <DATA> ndims          <DATA> Number of dimension
 * <DATA> IN  <DATA> n1             <DATA> Mesh parameter, see description.
 * <DATA> IN  <DATA> n2             <DATA> Mesh parameter, see description.
 * <DATA> IN  <DATA> n3             <DATA> Mesh parameter, see description.
 * <DATA> IN  <DATA> n4             <DATA> Mesh parameter, see description.
 * <DATA> IN  <DATA> nchunks        <DATA> Number of chunks that will be created in this mesh
 * <DATA> IN  <DATA> iunits         <DATA> Units of the 1st dimension
 * <DATA> IN  <DATA> junits         <DATA> Units of the 2nd dimension
 * <DATA> IN  <DATA> kunits         <DATA> Units of the 3rd dimension
 * <DATA> IN  <DATA> ilabel         <DATA> Label for the 1st dimension
 * <DATA> IN  <DATA> jlabel         <DATA> Label for the 2nd dimension
 * <DATA> IN  <DATA> klabel         <DATA> Label for the 3rd dimension
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Mesh parameters:
 * <DATA>   For colinear meshes:      n1=ni, n2=nj, n3=nk, n4=ignored                         <br>
 * <DATA>   For non-colinear meshes:  n1=ni, n2=nj, n3=nk, n4=nghosts of mesh chunks          <br>
 * <DATA>   For unstructured:         n1=nnodes, n2=ncells, n3=nshapes, n4=nconnectivity      <br>
 * <DATA>   For point meshes:         n1=nnodes, n2,n3,n4=ignored
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Create_Mesh(   const TIO_File_t   fileID,
                                const TIO_Object_t stateID,
                                const char         name[],
                                TIO_Object_t       *meshID,
                                const TIO_Mesh_t   meshtype,
                                const TIO_Coord_t  coordtype,
                                const TIO_Bool_t   isAMR,
                                const char         group[],
                                const TIO_Size_t   order,
                                const TIO_Data_t   graph_datatype,
                                const TIO_Data_t   coord_datatype,
                                const TIO_Dims_t   ndims,
                                const TIO_Size_t   n1,
                                const TIO_Size_t   n2,
                                const TIO_Size_t   n3,
                                const TIO_Size_t   n4,
                                const TIO_Size_t   nchunks,
                                const char         iunits[],
                                const char         junits[],
                                const char         kunits[],
                                const char         ilabel[],
                                const char         jlabel[],
                                const char         klabel[] );

/***************************************************************************************************
 *
 * TIO_Open_Mesh
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Mesh
 * </NAME>
 *
 * <PURP>
 * <DATA> Open a mesh with the given name from the given file and state.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Mesh( const TIO_File_t   fileID,
 * <DATA>                      const TIO_Object_t stateID,
 * <DATA>                      const char         name[],
 * <DATA>                      TIO_Object_t       *meshID,
 * <DATA>                      TIO_Mesh_t         *meshtype,
 * <DATA>                      TIO_Coord_t        *coordtype,
 * <DATA>                      TIO_Bool_t         *isAMR,
 * <DATA>                      char               group[],
 * <DATA>                      TIO_Size_t         *order,
 * <DATA>                      TIO_Data_t         *graph_datatype,
 * <DATA>                      TIO_Data_t         *coord_datatype,
 * <DATA>                      TIO_Dims_t         *ndims,
 * <DATA>                      TIO_Size_t         *n1,
 * <DATA>                      TIO_Size_t         *n2,
 * <DATA>                      TIO_Size_t         *n3,
 * <DATA>                      TIO_Size_t         *n4,
 * <DATA>                      TIO_Size_t         *nchunks,
 * <DATA>                      char               iunits[],
 * <DATA>                      char               junits[],
 * <DATA>                      char               kunits[],
 * <DATA>                      char               ilabel[],
 * <DATA>                      char               jlabel[],
 * <DATA>                      char               klabel[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> stateID        <DATA> TyphonIO state ID
 * <DATA> IN  <DATA> name           <DATA> Mesh name
 * <DATA> OUT <DATA> meshID         <DATA> TyphonIO Mesh ID
 * <DATA> OUT <DATA> meshtype       <DATA> TyphonIO mesh type
 * <DATA> OUT <DATA> coordtype      <DATA> Coordinate system
 * <DATA> OUT <DATA> isAMR          <DATA> Whether AMR is used with this mesh
 * <DATA> OUT <DATA> group          <DATA> Used for identifying meshes which are related somehow, i.e. AMR levels or Laser Beams
 * <DATA> OUT <DATA> order          <DATA> Used to make meshes unique within a group
 * <DATA> OUT <DATA> graph_datatype <DATA> Datatype of connectivity, node and cell IDs (only used for unstructured meshes)
 * <DATA> OUT <DATA> coord_datatype <DATA> Coordinate system datatype
 * <DATA> OUT <DATA> ndims          <DATA> Number of dimension
 * <DATA> OUT <DATA> n1             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n2             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n3             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n4             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> nchunks        <DATA> Number of chunks for the mesh
 * <DATA> OUT <DATA> iunits         <DATA> Units of the 1st dimension
 * <DATA> OUT <DATA> junits         <DATA> Units of the 2nd dimension
 * <DATA> OUT <DATA> kunits         <DATA> Units of the 3rd dimension
 * <DATA> OUT <DATA> ilabel         <DATA> Label for the 1st dimension
 * <DATA> OUT <DATA> jlabel         <DATA> Label for the 2nd dimension
 * <DATA> OUT <DATA> klabel         <DATA> Label for the 3rd dimension
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Mesh parameters:
 * <DATA>   For colinear meshes:      n1=ni, n2=nj, n3=nk, n4=ignored                         <br>
 * <DATA>   For non-colinear meshes:  n1=ni, n2=nj, n3=nk, n4=nghosts of mesh chunks          <br>
 * <DATA>   For unstructured:         n1=nnodes, n2=ncells, n3=nshapes, n4=nconnectivity      <br>
 * <DATA>   For point meshes:         n1=nnodes, n2,n3,n4=ignored
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Mesh(     const TIO_File_t   fileID,
                                const TIO_Object_t stateID,
                                const char         name[],
                                TIO_Object_t       *meshID,
                                TIO_Mesh_t         *meshtype,
                                TIO_Coord_t        *coordtype,
                                TIO_Bool_t         *isAMR,
                                char               group[],
                                TIO_Size_t         *order,
                                TIO_Data_t         *graph_datatype,
                                TIO_Data_t         *coord_datatype,
                                TIO_Dims_t         *ndims,
                                TIO_Size_t         *n1,
                                TIO_Size_t         *n2,
                                TIO_Size_t         *n3,
                                TIO_Size_t         *n4,
                                TIO_Size_t         *nchunks,
                                char               iunits[],
                                char               junits[],
                                char               kunits[],
                                char               ilabel[],
                                char               jlabel[],
                                char               klabel[] );

/***************************************************************************************************
 *
 * TIO_Open_Mesh_idx
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Mesh_idx
 * </NAME>
 *
 * <PURP>
 * <DATA> Open a mesh with the given index from the given file and state.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Mesh_idx( const TIO_File_t   fileID,
 * <DATA>                          const TIO_Object_t stateID,
 * <DATA>                          const TIO_Size_t   idx,
 * <DATA>                          char               name[],
 * <DATA>                          TIO_Object_t       *meshID,
 * <DATA>                          TIO_Mesh_t         *meshtype,
 * <DATA>                          TIO_Coord_t        *coordtype,
 * <DATA>                          TIO_Bool_t         *isAMR,
 * <DATA>                          char               group[],
 * <DATA>                          TIO_Size_t         *order,
 * <DATA>                          TIO_Data_t         *graph_datatype,
 * <DATA>                          TIO_Data_t         *coord_datatype,
 * <DATA>                          TIO_Dims_t         *ndims,
 * <DATA>                          TIO_Size_t         *n1,
 * <DATA>                          TIO_Size_t         *n2,
 * <DATA>                          TIO_Size_t         *n3,
 * <DATA>                          TIO_Size_t         *n4,
 * <DATA>                          TIO_Size_t         *nchunks,
 * <DATA>                          char               iunits[],
 * <DATA>                          char               junits[],
 * <DATA>                          char               kunits[],
 * <DATA>                          char               ilabel[],
 * <DATA>                          char               jlabel[],
 * <DATA>                          char               klabel[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> stateID        <DATA> TyphonIO state ID
 * <DATA> IN  <DATA> idx            <DATA> Index of mesh in state
 * <DATA> OUT <DATA> name           <DATA> Mesh name
 * <DATA> OUT <DATA> meshID         <DATA> TyphonIO Mesh ID
 * <DATA> OUT <DATA> meshtype       <DATA> TyphonIO mesh type
 * <DATA> OUT <DATA> coordtype      <DATA> Coordinate system
 * <DATA> OUT <DATA> isAMR          <DATA> Whether AMR is used with this mesh
 * <DATA> OUT <DATA> group          <DATA> Used for identifying meshes which are related somehow, i.e. AMR levels or Laser Beams
 * <DATA> OUT <DATA> order          <DATA> Used to make meshes unique within a group
 * <DATA> OUT <DATA> graph_datatype <DATA> Datatype of connectivity, node and cell IDs (only used for unstructured meshes)
 * <DATA> OUT <DATA> coord_datatype <DATA> Coordinate system datatype
 * <DATA> OUT <DATA> ndims          <DATA> Number of dimensions
 * <DATA> OUT <DATA> n1             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n2             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n3             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n4             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> nchunks        <DATA> Number of chunks for the mesh
 * <DATA> OUT <DATA> iunits         <DATA> Units of the 1st dimension
 * <DATA> OUT <DATA> junits         <DATA> Units of the 2nd dimension
 * <DATA> OUT <DATA> kunits         <DATA> Units of the 3rd dimension
 * <DATA> OUT <DATA> ilabel         <DATA> Label for the 1st dimension
 * <DATA> OUT <DATA> jlabel         <DATA> Label for the 2nd dimension
 * <DATA> OUT <DATA> klabel         <DATA> Label for the 3rd dimension
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Mesh parameters:
 * <DATA>   For colinear meshes:      n1=ni, n2=nj, n3=nk, n4=ignored                         <br>
 * <DATA>   For non-colinear meshes:  n1=ni, n2=nj, n3=nk, n4=nghosts of mesh chunks          <br>
 * <DATA>   For unstructured:         n1=nnodes, n2=ncells, n3=nshapes, n4=nconnectivity      <br>
 * <DATA>   For point meshes:         n1=nnodes, n2,n3,n4=ignored
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Mesh_idx( const TIO_File_t   fileID,
                                const TIO_Object_t stateID,
                                const TIO_Size_t   idx,
                                char               name[],
                                TIO_Object_t       *meshID,
                                TIO_Mesh_t         *meshtype,
                                TIO_Coord_t        *coordtype,
                                TIO_Bool_t         *isAMR,
                                char               group[],
                                TIO_Size_t         *order,
                                TIO_Data_t         *graph_datatype,
                                TIO_Data_t         *coord_datatype,
                                TIO_Dims_t         *ndims,
                                TIO_Size_t         *n1,
                                TIO_Size_t         *n2,
                                TIO_Size_t         *n3,
                                TIO_Size_t         *n4,
                                TIO_Size_t         *nchunks,
                                char               iunits[],
                                char               junits[],
                                char               kunits[],
                                char               ilabel[],
                                char               jlabel[],
                                char               klabel[] );

/***************************************************************************************************
 *
 * TIO_Close_Mesh
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Close_Mesh
 * </NAME>
 *
 * <PURP>
 * <DATA> Close a mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Close_Mesh( const TIO_File_t   fileID,
 * <DATA>                 const TIO_Object_t meshID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID <DATA> TyphonIO Mesh ID
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
extern TIO_t TIO_Close_Mesh(    const TIO_File_t   fileID,
                                const TIO_Object_t meshID );

/***************************************************************************************************
 *
 * TIO_Get_Mesh_Info
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Mesh_Info
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the mesh info for an already open mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Mesh_Info( const TIO_File_t   fileID,
 * <DATA>                          const TIO_Object_t meshID,
 * <DATA>                          char               name[],
 * <DATA>                          TIO_Mesh_t         *meshtype,
 * <DATA>                          TIO_Coord_t        *coordtype,
 * <DATA>                          TIO_Bool_t         *isAMR,
 * <DATA>                          char               group[],
 * <DATA>                          TIO_Size_t         *order,
 * <DATA>                          TIO_Data_t         *graph_datatype,
 * <DATA>                          TIO_Data_t         *coord_datatype,
 * <DATA>                          TIO_Dims_t         *ndims,
 * <DATA>                          TIO_Size_t         *n1,
 * <DATA>                          TIO_Size_t         *n2,
 * <DATA>                          TIO_Size_t         *n3,
 * <DATA>                          TIO_Size_t         *n4,
 * <DATA>                          TIO_Size_t         *nchunks,
 * <DATA>                          char               iunits[],
 * <DATA>                          char               junits[],
 * <DATA>                          char               kunits[],
 * <DATA>                          char               ilabel[],
 * <DATA>                          char               jlabel[],
 * <DATA>                          char               klabel[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID         <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> stateID        <DATA> TyphonIO mesh ID
 * <DATA> OUT <DATA> name           <DATA> Mesh name
 * <DATA> OUT <DATA> meshtype       <DATA> TyphonIO mesh type
 * <DATA> OUT <DATA> coordtype      <DATA> Coordinate system
 * <DATA> OUT <DATA> isAMR          <DATA> Whether AMR is used with this mesh
 * <DATA> OUT <DATA> group          <DATA> Used for identifying meshes which are related somehow, i.e. AMR levels or Laser Beams
 * <DATA> OUT <DATA> order          <DATA> Used to make meshes unique within a group
 * <DATA> OUT <DATA> graph_datatype <DATA> Datatype of connectivity, node and cell IDs (only used for unstructured meshes)
 * <DATA> OUT <DATA> coord_datatype <DATA> Coordinate system datatype
 * <DATA> OUT <DATA> ndims          <DATA> Number of dimensions
 * <DATA> OUT <DATA> n1             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n2             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n3             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> n4             <DATA> Mesh parameter, see description.
 * <DATA> OUT <DATA> nchunks        <DATA> Number of chunks for the mesh
 * <DATA> OUT <DATA> iunits         <DATA> Units of the 1st dimension
 * <DATA> OUT <DATA> junits         <DATA> Units of the 2nd dimension
 * <DATA> OUT <DATA> kunits         <DATA> Units of the 3rd dimension
 * <DATA> OUT <DATA> ilabel         <DATA> Label for the 1st dimension
 * <DATA> OUT <DATA> jlabel         <DATA> Label for the 2nd dimension
 * <DATA> OUT <DATA> klabel         <DATA> Label for the 3rd dimension
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Mesh parameters:
 * <DATA>   For colinear meshes:      n1=ni, n2=nj, n3=nk, n4=ignored                         <br>
 * <DATA>   For non-colinear meshes:  n1=ni, n2=nj, n3=nk, n4=nghosts of mesh chunks          <br>
 * <DATA>   For unstructured:         n1=nnodes, n2=ncells, n3=nshapes, n4=nconnectivity      <br>
 * <DATA>   For point meshes:         n1=nnodes, n2,n3,n4=ignored
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_Mesh_Info( const TIO_File_t   fileID,
                                const TIO_Object_t meshID,
                                char               name[],
                                TIO_Mesh_t         *meshtype,
                                TIO_Coord_t        *coordtype,
                                TIO_Bool_t         *isAMR,
                                char               group[],
                                TIO_Size_t         *order,
                                TIO_Data_t         *graph_datatype,
                                TIO_Data_t         *coord_datatype,
                                TIO_Dims_t         *ndims,
                                TIO_Size_t         *n1,
                                TIO_Size_t         *n2,
                                TIO_Size_t         *n3,
                                TIO_Size_t         *n4,
                                TIO_Size_t         *nchunks,
                                char               iunits[],
                                char               junits[],
                                char               kunits[],
                                char               ilabel[],
                                char               jlabel[],
                                char               klabel[] );

/***************************************************************************************************
 *
 * TIO_Set_Mesh_Range
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Set_Mesh_Range
 * </NAME>
 *
 * <PURP>
 * <DATA> Sets the dimensional ranges for the given mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Set_Mesh_Range( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t meshID,
 * <DATA>                           const TIO_Data_t   datatype,
 * <DATA>                           const TIO_Dims_t   ndims,
 * <DATA>                           const void         *xl,
 * <DATA>                           const void         *xh,
 * <DATA>                           const void         *yl,
 * <DATA>                           const void         *yh,
 * <DATA>                           const void         *zl,
 * <DATA>                           const void         *zh );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> ndims    <DATA> Number of dimensions
 * <DATA> IN  <DATA> xl       <DATA> The lower bound of the 1st dimension
 * <DATA> IN  <DATA> xh       <DATA> The upper bound of the 1st dimension
 * <DATA> IN  <DATA> yl       <DATA> The lower bound of the 2nd dimension (ignored for ndims < 2)
 * <DATA> IN  <DATA> yh       <DATA> The upper bound of the 2nd dimension (ignored for ndims < 2)
 * <DATA> IN  <DATA> zl       <DATA> The lower bound of the 3rd dimension (ignored for ndims < 3)
 * <DATA> IN  <DATA> zh       <DATA> The upper bound of the 3rd dimension (ignored for ndims < 3)
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
extern TIO_t TIO_Set_Mesh_Range( const TIO_File_t   fileID,
                                 const TIO_Object_t meshID,
                                 const TIO_Data_t   datatype,
                                 const TIO_Dims_t   ndims,
                                 const void         *xl,
                                 const void         *xh,
                                 const void         *yl,
                                 const void         *yh,
                                 const void         *zl,
                                 const void         *zh );

/***************************************************************************************************
 *
 * TIO_Get_Mesh_Range
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Mesh_Range
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the dimensional ranges for the given mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Mesh_Range( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t meshID,
 * <DATA>                           const TIO_Data_t   datatype,
 * <DATA>                           TIO_Dims_t        *ndims,
 * <DATA>                           void               *xl,
 * <DATA>                           void               *xh,
 * <DATA>                           void               *yl,
 * <DATA>                           void               *yh,
 * <DATA>                           void               *zl,
 * <DATA>                           void               *zh );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims    <DATA> The number of dimensions.
 * <DATA> IN  <DATA> xl       <DATA> The lower bound of the 1st dimension
 * <DATA> IN  <DATA> xh       <DATA> The upper bound of the 1st dimension
 * <DATA> IN  <DATA> yl       <DATA> The lower bound of the 2nd dimension (ignored for ndims < 2)
 * <DATA> IN  <DATA> yh       <DATA> The upper bound of the 2nd dimension (ignored for ndims < 2)
 * <DATA> IN  <DATA> zl       <DATA> The lower bound of the 3rd dimension (ignored for ndims < 3)
 * <DATA> IN  <DATA> zh       <DATA> The upper bound of the 3rd dimension (ignored for ndims < 3)
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
extern TIO_t TIO_Get_Mesh_Range( const TIO_File_t   fileID,
                                 const TIO_Object_t meshID,
                                 const TIO_Data_t   datatype,
                                 TIO_Dims_t        *ndims,
                                 void               *xl,
                                 void               *xh,
                                 void               *yl,
                                 void               *yh,
                                 void               *zl,
                                 void               *zh );



/*******  QUAD *******/

/***************************************************************************************************
 *
 * TIO_Write_QuadMesh_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_QuadMesh_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write a chunks worth of a structured mesh to file.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_QuadMesh_Chunk( const TIO_File_t   fileID,
 * <DATA>                                 const TIO_Object_t meshID,
 * <DATA>                                 const TIO_Size_t   idx,
 * <DATA>                                 const TIO_Xfer_t   xfer,
 * <DATA>                                 const TIO_Data_t   datatype,
 * <DATA>                                 const void         *icoords,
 * <DATA>                                 const void         *jcoords,
 * <DATA>                                 const void         *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> icoords  <DATA> the 1st dimension data array
 * <DATA> IN  <DATA> jcoords  <DATA> the 2nd dimension data array
 * <DATA> IN  <DATA> kcoords  <DATA> the 3rd dimension data array
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Must be called collectively if xfer = TIO_XFER_COLLECTIVE, or can be called independentally if xfer = TIO_XFER_INDEPENDENT.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Write_QuadMesh_Chunk(  const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Size_t   idx,
                                        const TIO_Xfer_t   xfer,
                                        const TIO_Data_t   datatype,
                                        const void         *icoords,
                                        const void         *jcoords,
                                        const void         *kcoords );

/***************************************************************************************************
 *
 * TIO_Write_QuadMesh_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_QuadMesh_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Write an entire structured mesh to file.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_QuadMesh_All( const TIO_File_t   fileID,
 * <DATA>                               const TIO_Object_t meshID,
 * <DATA>                               const TIO_Data_t   datatype,
 * <DATA>                               const void         *icoords,
 * <DATA>                               const void         *jcoords,
 * <DATA>                               const void         *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> icoords  <DATA> Mesh data for the 1st dimension
 * <DATA> IN  <DATA> jcoords  <DATA> Mesh data for the 2nd dimension
 * <DATA> IN  <DATA> kcoords  <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Write_QuadMesh_All(    const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Data_t   datatype,
                                        const void         *icoords,
                                        const void         *jcoords,
                                        const void         *kcoords );

/***************************************************************************************************
 *
 * TIO_Read_QuadMesh_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadMesh_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a chunk's structured mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadMesh_Chunk( const TIO_File_t   fileID,
 * <DATA>                                const TIO_Object_t meshID,
 * <DATA>                                const TIO_Size_t   idx,
 * <DATA>                                const TIO_Xfer_t   xfer,
 * <DATA>                                const TIO_Data_t   datatype,
 * <DATA>                                const TIO_Size_t   nghosts,
 * <DATA>                                TIO_Dims_t         *ndims,
 * <DATA>                                TIO_Size_t         *ni,
 * <DATA>                                TIO_Size_t         *nj,
 * <DATA>                                TIO_Size_t         *nk,
 * <DATA>                                void               *icoords,
 * <DATA>                                void               *jcoords,
 * <DATA>                                void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts  <DATA> Number of ghosts
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimensions of chunk
 * <DATA> OUT <DATA> ni       <DATA> Size of the 1st dimension
 * <DATA> OUT <DATA> nj       <DATA> Size of the 2nd dimension
 * <DATA> OUT <DATA> nk       <DATA> Size of the 3rd dimension
 * <DATA> OUT <DATA> icoords  <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords  <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords  <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_QuadMesh_Chunk(   const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Size_t   idx,
                                        const TIO_Xfer_t   xfer,
                                        const TIO_Data_t   datatype,
                                        const TIO_Size_t   nghosts,
                                        TIO_Dims_t         *ndims,
                                        TIO_Size_t         *ni,
                                        TIO_Size_t         *nj,
                                        TIO_Size_t         *nk,
                                        void               *icoords,
                                        void               *jcoords,
                                        void               *kcoords );

/***************************************************************************************************
 *
 * TIO_Read_QuadMesh_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadMesh_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read an entire structured mesh.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadMesh_All( const TIO_File_t   fileID,
 * <DATA>                              const TIO_Object_t meshID,
 * <DATA>                              const TIO_Data_t   datatype,
 * <DATA>                              TIO_Dims_t         *ndims,
 * <DATA>                              TIO_Size_t         *ni,
 * <DATA>                              TIO_Size_t         *nj,
 * <DATA>                              TIO_Size_t         *nk,
 * <DATA>                              void               *icoords,
 * <DATA>                              void               *jcoords,
 * <DATA>                              void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimensions
 * <DATA> OUT <DATA> ni       <DATA> Size of the 1st dimension
 * <DATA> OUT <DATA> nj       <DATA> Size of the 2nd dimension
 * <DATA> OUT <DATA> nk       <DATA> Size of the 3rd dimension
 * <DATA> OUT <DATA> icoords  <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords  <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords  <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_QuadMesh_All(     const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Data_t   datatype,
                                        TIO_Dims_t         *ndims,
                                        TIO_Size_t         *ni,
                                        TIO_Size_t         *nj,
                                        TIO_Size_t         *nk,
                                        void               *icoords,
                                        void               *jcoords,
                                        void               *kcoords );

/***************************************************************************************************
 *
 * TIO_Read_QuadMesh_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_QuadMesh_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a structured mesh section.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_QuadMesh_Section( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t meshID,
 * <DATA>                                  const TIO_Object_t sectionID,
 * <DATA>                                  const TIO_Xfer_t   xfer,
 * <DATA>                                  const TIO_Data_t   datatype,
 * <DATA>                                  TIO_Dims_t         *ndims,
 * <DATA>                                  TIO_Size_t         *ni,
 * <DATA>                                  TIO_Size_t         *nj,
 * <DATA>                                  TIO_Size_t         *nk,
 * <DATA>                                  void               *icoords,
 * <DATA>                                  void               *jcoords,
 * <DATA>                                  void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID    <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID    <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> sectionID <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer      <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype  <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims     <DATA> Number of dimensions
 * <DATA> OUT <DATA> ni        <DATA> Size of the 1st dimension
 * <DATA> OUT <DATA> nj        <DATA> Size of the 2nd dimension
 * <DATA> OUT <DATA> nk        <DATA> Size of the 3rd dimension
 * <DATA> OUT <DATA> icoords   <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords   <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords   <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_QuadMesh_Section( const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Object_t sectionID,
                                        const TIO_Xfer_t   xfer,
                                        const TIO_Data_t   datatype,
                                        TIO_Dims_t         *ndims,
                                        TIO_Size_t         *ni,
                                        TIO_Size_t         *nj,
                                        TIO_Size_t         *nk,
                                        void               *icoords,
                                        void               *jcoords,
                                        void               *kcoords );



/*******  UNSTR *******/

/***************************************************************************************************
 *
 * TIO_Write_UnstrMesh_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_UnstrMesh_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write a chunk's unstructured mesh data to file.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_UnstrMesh_Chunk( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t meshID,
 * <DATA>                                  const TIO_Size_t   idx,
 * <DATA>                                  const TIO_Xfer_t   xfer,
 * <DATA>                                  const TIO_Data_t   graph_datatype,
 * <DATA>                                  const TIO_Data_t   coord_datatype,
 * <DATA>                                  const void         *nodeIDs,
 * <DATA>                                  const void         *cellIDs,
 * <DATA>                                  const TIO_Shape_t  *shapes,
 * <DATA>                                  const void         *ncells_per_shape,
 * <DATA>                                  const void         *connectivity,
 * <DATA>                                  const void         *icoords,
 * <DATA>                                  const void         *jcoords,
 * <DATA>                                  const void         *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID           <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID           <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> idx              <DATA> Chunk index
 * <DATA> IN  <DATA> xfer             <DATA> Data transfer method
 * <DATA> IN  <DATA> graph_datatype   <DATA> Datatype of connectivity, node and cell IDs
 * <DATA> IN  <DATA> coord_datatype   <DATA> The datatype of the coordinate system
 * <DATA> IN  <DATA> nodeIDs          <DATA> The IDs of the nodes to write
 * <DATA> IN  <DATA> cellIDs          <DATA> The IDs of the cells to write
 * <DATA> IN  <DATA> shapes           <DATA> Number of different cell shapes
 * <DATA> IN  <DATA> ncells_per_shape <DATA> Number of cells
 * <DATA> IN  <DATA> connectivity     <DATA> The connectivity array
 * <DATA> IN  <DATA> icoords          <DATA> Mesh data for the 1st dimension
 * <DATA> IN  <DATA> jcoords          <DATA> Mesh data for the 2nd dimension
 * <DATA> IN  <DATA> kcoords          <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Write_UnstrMesh_Chunk( const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Size_t   idx,
                                        const TIO_Xfer_t   xfer,
                                        const TIO_Data_t   graph_datatype,
                                        const TIO_Data_t   coord_datatype,
                                        const void         *nodeIDs,
                                        const void         *cellIDs,
                                        const TIO_Shape_t  *shapes,
                                        const void         *ncells_per_shape,
                                        const void         *connectivity,
                                        const void         *icoords,
                                        const void         *jcoords,
                                        const void         *kcoords );

/***************************************************************************************************
 *
 * TIO_Read_UnstrMesh_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrMesh_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a unstructured mesh chunk's data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrMesh_Chunk( const TIO_File_t   fileID,
 * <DATA>                                 const TIO_Object_t meshID,
 * <DATA>                                 const TIO_Size_t   idx,
 * <DATA>                                 const TIO_Xfer_t   xfer,
 * <DATA>                                 const TIO_Data_t   graph_datatype,
 * <DATA>                                 const TIO_Data_t   coord_datatype,
 * <DATA>                                 const TIO_Size_t   nghosts,
 * <DATA>                                 TIO_Dims_t         *ndims,
 * <DATA>                                 TIO_Size_t         *nnodes,
 * <DATA>                                 TIO_Size_t         *ncells,
 * <DATA>                                 TIO_Size_t         *nshapes,
 * <DATA>                                TIO_Size_t         *nconnectivity,
 * <DATA>                                 void               *nodeIDs,
 * <DATA>                                 void               *cellIDs,
 * <DATA>                                 TIO_Shape_t        *shapes,
 * <DATA>                                 void               *ncells_per_shape,
 * <DATA>                                 void               *connectivity,
 * <DATA>                                 void               *icoords,
 * <DATA>                                 void               *jcoords,
 * <DATA>                                 void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID           <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID           <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> idx              <DATA> Chunk index
 * <DATA> IN  <DATA> xfer             <DATA> Data transfer method
 * <DATA> IN  <DATA> graph_datatype   <DATA> Datatype of connectivity, node and cell IDs
 * <DATA> IN  <DATA> coord_datatype   <DATA> Datatype of the coordinate system
 * <DATA> IN  <DATA> nghosts          <DATA> Number of ghosts
 * <DATA> OUT <DATA> ndims            <DATA> Number of dimensions
 * <DATA> OUT <DATA> nnodes           <DATA> Number of nodes
 * <DATA> OUT <DATA> ncells           <DATA> Number of cells
 * <DATA> OUT <DATA> nshapes          <DATA> Number of different cell shapes
 * <DATA> OUT <DATA> nconnectivity    <DATA> Size of the connectivity array
 * <DATA> OUT <DATA> nodeIDs          <DATA> Node IDs
 * <DATA> OUT <DATA> cellIDs          <DATA> Cell IDs
 * <DATA> OUT <DATA> shapes           <DATA> Cell shapes
 * <DATA> OUT <DATA> ncells_per_shape <DATA> Number of cells
 * <DATA> OUT <DATA> connectivity     <DATA> The connectivity array
 * <DATA> OUT <DATA> icoords          <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords          <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords          <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_UnstrMesh_Chunk(  const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Size_t   idx,
                                        const TIO_Xfer_t   xfer,
                                        const TIO_Data_t   graph_datatype,
                                        const TIO_Data_t   coord_datatype,
                                        const TIO_Size_t   nghosts,
                                        TIO_Dims_t         *ndims,
                                        TIO_Size_t         *nnodes,
                                        TIO_Size_t         *ncells,
                                        TIO_Size_t         *nshapes,
                                        TIO_Size_t         *nconnectivity,
                                        void               *nodeIDs,
                                        void               *cellIDs,
                                        TIO_Shape_t        *shapes,
                                        void               *ncells_per_shape,
                                        void               *connectivity,
                                        void               *icoords,
                                        void               *jcoords,
                                        void               *kcoords );

/***************************************************************************************************
 *
 * TIO_Read_UnstrMesh_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrMesh_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read an entire unstructured mesh's data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrMesh_All( const TIO_File_t   fileID,
 * <DATA>                               const TIO_Object_t meshID,
 * <DATA>                               const TIO_Data_t   graph_datatype,
 * <DATA>                               const TIO_Data_t   coord_datatype,
 * <DATA>                               TIO_Dims_t         *ndims,
 * <DATA>                               TIO_Size_t         *nnodes,
 * <DATA>                               TIO_Size_t         *ncells,
 * <DATA>                               TIO_Size_t         *nshapes,
 * <DATA>                               TIO_Size_t         *nconnectivity,
 * <DATA>                               void               *nodeIDs,
 * <DATA>                               void               *cellIDs,
 * <DATA>                               TIO_Shape_t        *shapes,
 * <DATA>                               void               *ncells_per_shape,
 * <DATA>                               void               *connectivity,
 * <DATA>                               void               *icoords,
 * <DATA>                               void               *jcoords,
 * <DATA>                               void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID           <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID           <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> graph_datatype   <DATA> Datatype of connectivity, node and cell IDs
 * <DATA> IN  <DATA> coord_datatype   <DATA> Datatype of the coordinate system
 * <DATA> OUT <DATA> ndims            <DATA> Number of dimensions
 * <DATA> OUT <DATA> nnodes           <DATA> Number of nodes
 * <DATA> OUT <DATA> ncells           <DATA> Number of cells
 * <DATA> OUT <DATA> nshapes          <DATA> Number of different cell shapes
 * <DATA> OUT <DATA> nconnectivity    <DATA> Size of the connectivity array
 * <DATA> OUT <DATA> nodeIDs          <DATA> Node IDs
 * <DATA> OUT <DATA> cellIDs          <DATA> Cell IDs
 * <DATA> OUT <DATA> shapes           <DATA> Cell shapes
 * <DATA> OUT <DATA> ncells_per_shape <DATA> Number of cells
 * <DATA> OUT <DATA> connectivity     <DATA> The connectivity array
 * <DATA> OUT <DATA> icoords          <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords          <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords          <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_UnstrMesh_All(    const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Data_t   graph_datatype,
                                        const TIO_Data_t   coord_datatype,
                                        TIO_Dims_t         *ndims,
                                        TIO_Size_t         *nnodes,
                                        TIO_Size_t         *ncells,
                                        TIO_Size_t         *nshapes,
                                        TIO_Size_t         *nconnectivity,
                                        void               *nodeIDs,
                                        void               *cellIDs,
                                        TIO_Shape_t        *shapes,
                                        void               *ncells_per_shape,
                                        void               *connectivity,
                                        void               *icoords,
                                        void               *jcoords,
                                        void               *kcoords );


  /*  In future, might have this instead:

extern TIO_t TIO_Read_UnstrMesh_Section( const TIO_File_t   fileID,
                                         const TIO_Object_t meshID,
                                         const TIO_Object_t sectionID,
                                         const TIO_Xfer_t   xfer,
                                         const TIO_Data_t   graph_datatype,
                                         const TIO_Data_t   coord_datatype,
                                         TIO_Dims_t         *ndims,
                                         TIO_Size_t         *nnodes,
                                         TIO_Size_t         *ncells,
                                         TIO_Size_t         *nshapes,
                                         TIO_Size_t         *nconnectivity,
                                         void               *nodeIDs,
                                         void               *cellIDs,
                                         TIO_Shape_t        *shapes,
                                         void               *ncells_per_shape,
                                         void               *connectivity,
                                         void               *icoords,
                                         void               *jcoords,
                                         void               *kcoords );
  */

/***************************************************************************************************
 *
 * TIO_Read_UnstrMesh_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_UnstrMesh_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read an unstructured mesh section's data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_UnstrMesh_Section( const TIO_File_t   fileID,
 * <DATA>                                   const TIO_Object_t meshID,
 * <DATA>                                   const TIO_Object_t sectionID,
 * <DATA>                                   const TIO_Xfer_t   xfer,
 * <DATA>                                   const TIO_Data_t   datatype,
 * <DATA>                                   TIO_Dims_t         *ndims,
 * <DATA>                                   TIO_Size_t         *nnodes,
 * <DATA>                                   void               *icoords,
 * <DATA>                                   void               *jcoords,
 * <DATA>                                   void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID    <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID    <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> sectionID <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer      <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype  <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims     <DATA> Number of dimensions
 * <DATA> OUT <DATA> nnodes    <DATA> Number of nodes
 * <DATA> OUT <DATA> icoords   <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords   <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords   <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_UnstrMesh_Section( const TIO_File_t   fileID,
                                         const TIO_Object_t meshID,
                                         const TIO_Object_t sectionID,
                                         const TIO_Xfer_t   xfer,
                                         const TIO_Data_t   datatype,
                                         TIO_Dims_t         *ndims,
                                         TIO_Size_t         *nnodes,
                                         void               *icoords,
                                         void               *jcoords,
                                         void               *kcoords );


/*******  POINT *******/

/***************************************************************************************************
 *
 * TIO_Write_PointMesh_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_PointMesh_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Write a point mesh chunk's data to file.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_PointMesh_Chunk( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t meshID,
 * <DATA>                                  const TIO_Size_t   idx,
 * <DATA>                                  const TIO_Xfer_t   xfer,
 * <DATA>                                  const TIO_Data_t   datatype,
 * <DATA>                                  const void         *icoords,
 * <DATA>                                  const void         *jcoords,
 * <DATA>                                  const void         *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> icoords  <DATA> Mesh data for the 1st dimension
 * <DATA> IN  <DATA> jcoords  <DATA> Mesh data for the 2nd dimension
 * <DATA> IN  <DATA> kcoords  <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Write_PointMesh_Chunk( const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Size_t   idx,
                                        const TIO_Xfer_t   xfer,
                                        const TIO_Data_t   datatype,
                                        const void         *icoords,
                                        const void         *jcoords,
                                        const void         *kcoords );

/***************************************************************************************************
 *
 * TIO_Read_PointMesh_Chunk
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointMesh_Chunk
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a point mesh chunk's data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointMesh_Chunk( const TIO_File_t   fileID,
 * <DATA>                                 const TIO_Object_t meshID,
 * <DATA>                                 const TIO_Size_t   idx,
 * <DATA>                                 const TIO_Xfer_t   xfer,
 * <DATA>                                 const TIO_Data_t   datatype,
 * <DATA>                                 const TIO_Size_t   nghosts,
 * <DATA>                                 TIO_Dims_t         *ndims,
 * <DATA>                                 TIO_Size_t         *nnodes,
 * <DATA>                                 void               *icoords,
 * <DATA>                                 void               *jcoords,
 * <DATA>                                 void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> idx      <DATA> Chunk index
 * <DATA> IN  <DATA> xfer     <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> IN  <DATA> nghosts  <DATA> Number of ghosts
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimensions
 * <DATA> OUT <DATA> nnodes   <DATA> Number of nodes
 * <DATA> OUT <DATA> icoords  <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords  <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords  <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_PointMesh_Chunk(  const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Size_t   idx,
                                        const TIO_Xfer_t   xfer,
                                        const TIO_Data_t   datatype,
                                        const TIO_Size_t   nghosts,
                                        TIO_Dims_t         *ndims,
                                        TIO_Size_t         *nnodes,
                                        void               *icoords,
                                        void               *jcoords,
                                        void               *kcoords );

/***************************************************************************************************
 *
 * TIO_Read_PointMesh_All
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointMesh_All
 * </NAME>
 *
 * <PURP>
 * <DATA> Read an entire point mesh's data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointMesh_All( const TIO_File_t   fileID,
 * <DATA>                               const TIO_Object_t meshID,
 * <DATA>                               const TIO_Data_t   datatype,
 * <DATA>                               TIO_Dims_t         *ndims,
 * <DATA>                               TIO_Size_t         *nnodes,
 * <DATA>                               void               *icoords,
 * <DATA>                               void               *jcoords,
 * <DATA>                               void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID   <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimensions
 * <DATA> OUT <DATA> nnodes   <DATA> Number of nodes
 * <DATA> OUT <DATA> icoords  <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords  <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords  <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_PointMesh_All(    const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Data_t   datatype,
                                        TIO_Dims_t         *ndims,
                                        TIO_Size_t         *nnodes,
                                        void               *icoords,
                                        void               *jcoords,
                                        void               *kcoords );

/***************************************************************************************************
 *
 * TIO_Read_PointMesh_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_PointMesh_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a point mesh section's data.
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_PointMesh_Section( const TIO_File_t   fileID,
 * <DATA>                                   const TIO_Object_t meshID,
 * <DATA>                                   const TIO_Object_t sectionID,
 * <DATA>                                   const TIO_Xfer_t   xfer,
 * <DATA>                                   const TIO_Data_t   datatype,
 * <DATA>                                   TIO_Dims_t         *ndims,
 * <DATA>                                   TIO_Size_t         *nnodes,
 * <DATA>                                   void               *icoords,
 * <DATA>                                   void               *jcoords,
 * <DATA>                                   void               *kcoords );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID    <DATA> TyphonIO file ID
 * <DATA> IN  <DATA> meshID    <DATA> TyphonIO Mesh ID
 * <DATA> IN  <DATA> sectionID <DATA> TyphonIO section ID
 * <DATA> IN  <DATA> xfer      <DATA> Data transfer method
 * <DATA> IN  <DATA> datatype  <DATA> TyphonIO data type
 * <DATA> OUT <DATA> ndims     <DATA> Number of dimensions
 * <DATA> OUT <DATA> nnodes    <DATA> Number of nodes
 * <DATA> OUT <DATA> icoords   <DATA> Mesh data for the 1st dimension
 * <DATA> OUT <DATA> jcoords   <DATA> Mesh data for the 2nd dimension
 * <DATA> OUT <DATA> kcoords   <DATA> Mesh data for the 3rd dimension
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
extern TIO_t TIO_Read_PointMesh_Section( const TIO_File_t   fileID,
                                         const TIO_Object_t meshID,
                                         const TIO_Object_t sectionID,
                                         const TIO_Xfer_t   xfer,
                                         const TIO_Data_t   datatype,
                                         TIO_Dims_t         *ndims,
                                         TIO_Size_t         *nnodes,
                                         void               *icoords,
                                         void               *jcoords,
                                         void               *kcoords );

#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */

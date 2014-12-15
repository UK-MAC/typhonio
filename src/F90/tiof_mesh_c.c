/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tiof_fkit.h"
#include "typhonio.h"



static void xtioc_copymeshinfo( const char           *name,
                                const TIO_Mesh_t     meshtype,
                                const TIO_Coord_t    coordtype,
                                const TIO_Bool_t     isAMR,
                                const char           *group,
                                const TIO_Size_t     order,
                                const TIO_Data_t     graph_datatype,
                                const TIO_Data_t     coord_datatype,
                                const TIO_Dims_t     ndims,
                                const TIO_Size_t     n1,
                                const TIO_Size_t     n2,
                                const TIO_Size_t     n3,
                                const TIO_Size_t     n4,
                                const TIO_Size_t     nchunks,
                                const char           *iunits,
                                const char           *junits,
                                const char           *kunits,
                                const char           *ilabel,
                                const char           *jlabel,
                                const char           *klabel,
                                char                 *fname,   const int_f *fname_len,
                                int_f                *fmeshtype,
                                int_f                *fcoordtype,
                                int_f                *fisAMR,
                                char                 *fgroup,  const int_f *fgroup_len,
                                TIO_Size_tf          *forder,
                                int_f                *fgraph_datatype,
                                int_f                *fcoord_datatype,
                                int_f                *fndims,
                                TIO_Size_tf          *fn1,
                                TIO_Size_tf          *fn2,
                                TIO_Size_tf          *fn3,
                                TIO_Size_tf          *fn4,
                                TIO_Size_tf          *fnchunks,
                                char                 *fiunits, const int_f *fiunits_len,
                                char                 *fjunits, const int_f *fjunits_len,
                                char                 *fkunits, const int_f *fkunits_len,
                                char                 *filabel, const int_f *filabel_len,
                                char                 *fjlabel, const int_f *fjlabel_len,
                                char                 *fklabel, const int_f *fklabel_len );



/***************************************************************************************************
 *
 *   tio_list_meshes_c
 *
 **************************************************************************************************/
void
F2C(tio_list_meshes_c,TIO_LIST_MESHES_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *stateID,
       TIO_Size_tf         *nobj,
       char                *names,
       TIO_tf              *err )
{
  TIO_Size_t l_nobj;
  char       *fnptr, ccpy[TIO_STRLEN];
  int        len=TIO_STRLEN, *lptr = &len;
  TIO_Size_t i;
  
  
  
  if (*nobj > 0) {
    
    *err = c2f_TIO( TIO_List_Meshes( f2c_file(*fileID),
                                     f2c_object(*stateID),
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
    *err = c2f_TIO( TIO_List_Meshes( f2c_file(*fileID),
                                     f2c_object(*stateID),
                                     &l_nobj,
                                     NULL ) );

    if (*err != TIO_SUCCESS_F) return;
  }
  
  *nobj = c2f_size(l_nobj);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_create_mesh_c
 *
 **************************************************************************************************/
void
F2C(tio_create_mesh_c,TIO_CREATE_MESH_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *stateID,
       const char          *name,    const int_f *name_len,
       TIO_Object_tf       *meshID,
       const int_f         *meshtype,
       const int_f         *coordtype,
       const int_f         *isAMR,
       const char          *group,   const int_f *group_len,
       const TIO_Size_tf   *order,
       const int_f         *graph_datatype,
       const int_f         *coord_datatype,
       const int_f         *ndims,
       const TIO_Size_tf   *n1,
       const TIO_Size_tf   *n2,
       const TIO_Size_tf   *n3,
       const TIO_Size_tf   *n4,
       const TIO_Size_tf   *nchunks,
       const char          *iunits,  const int_f *iunits_len,
       const char          *junits,  const int_f *junits_len,
       const char          *kunits,  const int_f *kunits_len,
       const char          *ilabel,  const int_f *ilabel_len,
       const char          *jlabel,  const int_f *jlabel_len,
       const char          *klabel,  const int_f *klabel_len,
       TIO_tf              *err )
{
  char l_name[TIO_STRLEN], l_group[TIO_STRLEN];
  char l_iu[TIO_STRLEN], l_ju[TIO_STRLEN], l_ku[TIO_STRLEN];
  char l_il[TIO_STRLEN], l_jl[TIO_STRLEN], l_kl[TIO_STRLEN];
  TIO_Object_t l_meshID;
  TIO_Bool_t l_isAMR;
  
  
  
  f2c_str(name,   name_len,   l_name);
  f2c_str(group,  group_len,  l_group);
  
  f2c_str(iunits, iunits_len, l_iu);
  f2c_str(junits, junits_len, l_ju);
  f2c_str(kunits, kunits_len, l_ku);
  
  f2c_str(ilabel, ilabel_len, l_il);
  f2c_str(jlabel, jlabel_len, l_jl);
  f2c_str(klabel, klabel_len, l_kl);
  
  if (*isAMR > 0)
    l_isAMR = TIO_TRUE;
  else
    l_isAMR = TIO_FALSE;
  
  
  *err = c2f_TIO( TIO_Create_Mesh( f2c_file(*fileID),
                                   f2c_object(*stateID),
                                   l_name,
                                   &l_meshID,
                                   f2c_meshtype(*meshtype),
                                   f2c_coordtype(*coordtype),
                                   l_isAMR,
                                   l_group,
                                   f2c_size(*order),
                                   f2c_datatype(*graph_datatype),
                                   f2c_datatype(*coord_datatype),
                                   f2c_ndims(*ndims),
                                   f2c_size(*n1),
                                   f2c_size(*n2),
                                   f2c_size(*n3),
                                   f2c_size(*n4),
                                   f2c_size(*nchunks),
                                   l_iu, l_ju, l_ku,
                                   l_il, l_jl, l_kl ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *meshID = c2f_object(l_meshID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_open_mesh_c
 *
 **************************************************************************************************/
void
F2C(tio_open_mesh_c,TIO_OPEN_MESH_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *stateID,
       const TIO_Size_tf   *idx,
       char                *name,    const int_f *name_len,
       TIO_Object_tf       *meshID,
       int_f               *meshtype,
       int_f               *coordtype,
       int_f               *isAMR,
       char                *group,   const int_f *group_len,
       TIO_Size_tf         *order,
       int_f               *graph_datatype,
       int_f               *coord_datatype,
       int_f               *ndims,
       TIO_Size_tf         *n1,
       TIO_Size_tf         *n2,
       TIO_Size_tf         *n3,
       TIO_Size_tf         *n4,
       TIO_Size_tf         *nchunks,
       char                *iunits,  const int_f *iunits_len,
       char                *junits,  const int_f *junits_len,
       char                *kunits,  const int_f *kunits_len,
       char                *ilabel,  const int_f *ilabel_len,
       char                *jlabel,  const int_f *jlabel_len,
       char                *klabel,  const int_f *klabel_len,
       TIO_tf              *err )
{
  char           l_name[TIO_STRLEN];
  TIO_Object_t   l_meshID;
  TIO_Mesh_t     l_meshtype;
  TIO_Coord_t    l_coordtype;
  TIO_Bool_t     l_isAMR;
  char           l_group[TIO_STRLEN];
  TIO_Size_t     l_order;
  TIO_Data_t     l_cdtype, l_gdtype;
  TIO_Dims_t     l_ndims;
  TIO_Size_t     l_n1, l_n2, l_n3, l_n4, l_nchunks;
  char           l_iu[TIO_STRLEN], l_ju[TIO_STRLEN], l_ku[TIO_STRLEN];
  char           l_il[TIO_STRLEN], l_jl[TIO_STRLEN], l_kl[TIO_STRLEN];
  
  
  
  if (*idx > 0) {
    *err = c2f_TIO( TIO_Open_Mesh_idx( f2c_file(*fileID),
                                       f2c_object(*stateID),
                                       f2c_idx(*idx),
                                       l_name,
                                       &l_meshID,
                                       &l_meshtype,
                                       &l_coordtype,
                                       &l_isAMR,
                                       l_group,
                                       &l_order,
                                       &l_cdtype,
                                       &l_gdtype,
                                       &l_ndims,
                                       &l_n1,
                                       &l_n2,
                                       &l_n3,
                                       &l_n4,
                                       &l_nchunks,
                                       l_iu, l_ju, l_ku,
                                       l_il, l_jl, l_kl ) );
    
    if (*err != TIO_SUCCESS_F) return;
    
    c2f_str(l_name, name_len, name);
  }
  else {
    f2c_str(name, name_len, l_name);  
    
    *err = c2f_TIO( TIO_Open_Mesh( f2c_file(*fileID),
                                   f2c_object(*stateID),
                                   l_name,
                                   &l_meshID,
                                   &l_meshtype,
                                   &l_coordtype,
                                   &l_isAMR,
                                   l_group,
                                   &l_order,
                                   &l_cdtype,
                                   &l_gdtype,
                                   &l_ndims,
                                   &l_n1,
                                   &l_n2,
                                   &l_n3,
                                   &l_n4,
                                   &l_nchunks,
                                   l_iu, l_ju, l_ku,
                                   l_il, l_jl, l_kl ) );
    
    if (*err != TIO_SUCCESS_F) return;
  }
  
  (void)xtioc_copymeshinfo( NULL, l_meshtype, l_coordtype, l_isAMR, l_group, l_order,
                            l_cdtype, l_gdtype, l_ndims, l_n1, l_n2, l_n3, l_n4, l_nchunks,
                            l_iu, l_ju, l_ku, l_il, l_jl, l_kl,
                            NULL, NULL, meshtype, coordtype, isAMR, group, group_len, order,
                            graph_datatype, coord_datatype, ndims, n1, n2, n3, n4, nchunks,
                            iunits, iunits_len, junits, junits_len, kunits, kunits_len,
                            ilabel, ilabel_len, jlabel, jlabel_len, klabel, klabel_len);
  
  *meshID = c2f_object(l_meshID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_close_mesh_c
 *
 **************************************************************************************************/
void
F2C(tio_close_mesh_c,TIO_CLOSE_MESH_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Close_Mesh( f2c_file(*fileID),
                                  f2c_object(*meshID) ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_mesh_info_c
 *
 **************************************************************************************************/
void
F2C(tio_get_mesh_info_c,TIO_GET_MESH_INFO_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       char                *name,    const int_f *name_len,
       int_f               *meshtype,
       int_f               *coordtype,
       int_f               *isAMR,
       char                *group,   const int_f *group_len,
       TIO_Size_tf         *order,
       int_f               *graph_datatype,
       int_f               *coord_datatype,
       int_f               *ndims,
       TIO_Size_tf         *n1,
       TIO_Size_tf         *n2,
       TIO_Size_tf         *n3,
       TIO_Size_tf         *n4,
       TIO_Size_tf         *nchunks,
       char                *iunits,  const int_f *iunits_len,
       char                *junits,  const int_f *junits_len,
       char                *kunits,  const int_f *kunits_len,
       char                *ilabel,  const int_f *ilabel_len,
       char                *jlabel,  const int_f *jlabel_len,
       char                *klabel,  const int_f *klabel_len,
       TIO_tf              *err )
{
  char           l_name[TIO_STRLEN];
  TIO_Mesh_t     l_meshtype;
  TIO_Coord_t    l_coordtype;
  TIO_Bool_t     l_isAMR;
  char           l_group[TIO_STRLEN];
  TIO_Size_t     l_order;
  TIO_Data_t     l_cdtype, l_gdtype;
  TIO_Dims_t     l_ndims;
  TIO_Size_t     l_n1, l_n2, l_n3, l_n4, l_nchunks;
  char           l_iu[TIO_STRLEN], l_ju[TIO_STRLEN], l_ku[TIO_STRLEN];
  char           l_il[TIO_STRLEN], l_jl[TIO_STRLEN], l_kl[TIO_STRLEN];
  
  
  *err = c2f_TIO( TIO_Get_Mesh_Info(  f2c_file(*fileID),
                                      f2c_object(*meshID),
                                      l_name,
                                      &l_meshtype,
                                      &l_coordtype,
                                      &l_isAMR,
                                      l_group,
                                      &l_order,
                                      &l_cdtype,
                                      &l_gdtype,
                                      &l_ndims,
                                      &l_n1,
                                      &l_n2,
                                      &l_n3,
                                      &l_n4,
                                      &l_nchunks,
                                      l_iu, l_ju, l_ku,
                                      l_il, l_jl, l_kl ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  (void)xtioc_copymeshinfo( l_name, l_meshtype, l_coordtype, l_isAMR, l_group, l_order,
                            l_cdtype, l_gdtype, l_ndims, l_n1, l_n2, l_n3, l_n4, l_nchunks,
                            l_iu, l_ju, l_ku, l_il, l_jl, l_kl,
                            name, name_len, meshtype, coordtype, isAMR, group, group_len, order,
                            graph_datatype, coord_datatype, ndims, n1, n2, n3, n4, nchunks,
                            iunits, iunits_len, junits, junits_len, kunits, kunits_len,
                            ilabel, ilabel_len, jlabel, jlabel_len, klabel, klabel_len);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_set_mesh_range_c
 *
 **************************************************************************************************/
void
F2C(tio_set_mesh_range_c,TIO_SET_MESH_RANGE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const int_f         *datatype,
       const int_f         *ndims,
       const void          *il,
       const void          *ih,
       const void          *jl,
       const void          *jh,
       const void          *kl,
       const void          *kh,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Set_Mesh_Range( f2c_file(*fileID),
                                      f2c_object(*meshID),
                                      f2c_datatype(*datatype),
                                      f2c_ndims(*ndims),
                                      il, ih, jl, jh, kl, kh ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_mesh_range_c
 *
 **************************************************************************************************/
void
F2C(tio_get_mesh_range_c,TIO_GET_MESH_RANGE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const int_f         *datatype,
       int_f               *ndims,
       void                *il,
       void                *ih,
       void                *jl,
       void                *jh,
       void                *kl,
       void                *kh,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  
  *err = c2f_TIO( TIO_Get_Mesh_Range( f2c_file(*fileID),
                                      f2c_object(*meshID),
                                      f2c_datatype(*datatype),
                                      &l_ndims,
                                      il, ih, jl, jh, kl, kh ) );
  
  *ndims = c2f_ndims(l_ndims);
  
  return;
}



/************************************
 ************************************
 **                                **
 **                                **
 **           QUADMESHES           **
 **                                **
 **                                **
 ************************************
 ************************************/


/***************************************************************************************************
 *
 *   tio_write_quadmesh_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_quadmesh_chunk_c,TIO_WRITE_QUADMESH_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const void          *icoords,
       const void          *jcoords,
       const void          *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  const void *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Write_QuadMesh_Chunk( f2c_file(*fileID),
                                            f2c_object(*meshID),
                                            f2c_idx(*idx),
                                            f2c_xfer(*xfer),
                                            f2c_datatype(*datatype),
                                            iptr, jptr, kptr) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_write_quadmesh_all_c
 *
 **************************************************************************************************/
void
F2C(tio_write_quadmesh_all_c,TIO_WRITE_QUADMESH_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const int_f         *datatype,
       const void          *icoords,
       const void          *jcoords,
       const void          *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  const void *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Write_QuadMesh_All( f2c_file(*fileID),
                                          f2c_object(*meshID),
                                          f2c_datatype(*datatype),
                                          iptr, jptr, kptr) );
  
  return;
}

  
/***************************************************************************************************
 *
 *   tio_read_quadmesh_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadmesh_chunk_c,TIO_READ_QUADMESH_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const TIO_Size_tf   *nghosts,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_QuadMesh_Chunk( f2c_file(*fileID),
                                           f2c_object(*meshID),
                                           f2c_idx(*idx),
                                           f2c_xfer(*xfer),
                                           f2c_datatype(*datatype),
                                           f2c_ghosts(*nghosts),
                                           &l_ndims,
                                           &l_ni,  &l_nj,  &l_nk,
                                           iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims = c2f_ndims(l_ndims);
  *ni    = c2f_size(l_ni);
  *nj    = c2f_size(l_nj);
  *nk    = c2f_size(l_nk);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_quadmesh_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadmesh_section_c,TIO_READ_QUADMESH_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_QuadMesh_Section( f2c_file(*fileID),
                                             f2c_object(*meshID),
                                             f2c_object(*sectionID),
                                             f2c_xfer(*xfer),
                                             f2c_datatype(*datatype),
                                             &l_ndims, &l_ni, &l_nj, &l_nk,
                                             iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims = c2f_ndims(l_ndims);
  *ni    = c2f_size(l_ni);
  *nj    = c2f_size(l_nj);
  *nk    = c2f_size(l_nk);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_quadmesh_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadmesh_all_c,TIO_READ_QUADMESH_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_QuadMesh_All( f2c_file(*fileID),
                                         f2c_object(*meshID),
                                         f2c_datatype(*datatype),
                                         &l_ndims, &l_ni, &l_nj, &l_nk,
                                         iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims = c2f_ndims(l_ndims);
  *ni    = c2f_size(l_ni);
  *nj    = c2f_size(l_nj);
  *nk    = c2f_size(l_nk);
  
  return;
}




/************************************
 ************************************
 **                                **
 **                                **
 **          UNSTRMESHES           **
 **                                **
 **                                **
 ************************************
 ************************************/


/***************************************************************************************************
 *
 *   tio_write_unstrmesh_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_unstrmesh_chunk_c,TIO_WRITE_UNSTRMESH_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *graph_datatype,
       const int_f         *coord_datatype,
       const void          *nodeIDs,
       const void          *cellIDs,
       const TIO_Shape_tf  *shapes,
       const void          *ncells_per_shape,
       const void          *connectivity,
       const void          *icoords,
       const void          *jcoords,
       const void          *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  const void *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Write_UnstrMesh_Chunk( f2c_file(*fileID),
                                             f2c_object(*meshID),
                                             f2c_idx(*idx),
                                             f2c_xfer(*xfer),
                                             f2c_datatype(*graph_datatype),
                                             f2c_datatype(*coord_datatype),
                                             nodeIDs, cellIDs, shapes,
                                             ncells_per_shape, connectivity,
                                             iptr, jptr, kptr) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_unstrmesh_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrmesh_chunk_c,TIO_READ_UNSTRMESH_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *graph_datatype,
       const int_f         *coord_datatype,
       const TIO_Size_tf   *nghosts,
       int_f               *ndims,
       TIO_Size_tf         *nnodes,
       TIO_Size_tf         *ncells,
       TIO_Size_tf         *nshapes,
       TIO_Size_tf         *nconnectivity,
       void                *nodeIDs,
       void                *cellIDs,
       TIO_Shape_tf        *shapes,
       void                *ncells_per_shape,
       void                *connectivity,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_nn, l_nc, l_ns, l_nv;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_UnstrMesh_Chunk( f2c_file(*fileID),
                                            f2c_object(*meshID),
                                            f2c_idx(*idx),
                                            f2c_xfer(*xfer),
                                            f2c_datatype(*graph_datatype),
                                            f2c_datatype(*coord_datatype),
                                            f2c_ghosts(*nghosts),
                                            &l_ndims,
                                            &l_nn,  &l_nc,  &l_ns,  &l_nv,
                                            nodeIDs, cellIDs,
                                            shapes, ncells_per_shape,
                                            connectivity,
                                            iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims         = c2f_ndims(l_ndims);
  *nnodes        = c2f_size(l_nn);
  *ncells        = c2f_size(l_nc);
  *nshapes       = c2f_size(l_ns);
  *nconnectivity = c2f_size(l_nv);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_unstrmesh_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrmesh_all_c,TIO_READ_UNSTRMESH_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const int_f         *graph_datatype,
       const int_f         *coord_datatype,
       int_f               *ndims,
       TIO_Size_tf         *nnodes,
       TIO_Size_tf         *ncells,
       TIO_Size_tf         *nshapes,
       TIO_Size_tf         *nconnectivity,
       void                *nodeIDs,
       void                *cellIDs,
       TIO_Shape_tf        *shapes,
       void                *ncells_per_shape,
       void                *connectivity,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_nn, l_nc, l_ns, l_nv;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_UnstrMesh_All( f2c_file(*fileID),
                                          f2c_object(*meshID),
                                          f2c_datatype(*graph_datatype),
                                          f2c_datatype(*coord_datatype),
                                          &l_ndims, &l_nn, &l_nc, &l_ns, &l_nv,
                                          nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,
                                          iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims         = c2f_ndims(l_ndims);
  *nnodes        = c2f_size(l_nn);
  *ncells        = c2f_size(l_nc);
  *nshapes       = c2f_size(l_ns);
  *nconnectivity = c2f_size(l_nv);
  
  return;
}




/***************************************************************************************************
 *
 *   tio_read_unstrmesh_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrmesh_section_c,TIO_READ_UNSTRMESH_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *nnodes,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_nn;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_UnstrMesh_Section( f2c_file(*fileID),
                                              f2c_object(*meshID),
                                              f2c_object(*sectionID),
                                              f2c_xfer(*xfer),
                                              f2c_datatype(*datatype),
                                              &l_ndims, &l_nn,
                                              iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims         = c2f_ndims(l_ndims);
  *nnodes        = c2f_size(l_nn);
  
  return;
}




/************************************
 ************************************
 **                                **
 **                                **
 **          POINTMESHES           **
 **                                **
 **                                **
 ************************************
 ************************************/


/***************************************************************************************************
 *
 *   tio_write_pointmesh_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_pointmesh_chunk_c,TIO_WRITE_POINTMESH_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const void          *icoords,
       const void          *jcoords,
       const void          *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  const void *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Write_PointMesh_Chunk( f2c_file(*fileID),
                                             f2c_object(*meshID),
                                             f2c_idx(*idx),
                                             f2c_xfer(*xfer),
                                             f2c_datatype(*datatype),
                                             iptr, jptr, kptr) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointmesh_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointmesh_chunk_c,TIO_READ_POINTMESH_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const TIO_Size_tf   *nghosts,
       int_f               *ndims,
       TIO_Size_tf         *nnodes,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_nn;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_PointMesh_Chunk( f2c_file(*fileID),
                                            f2c_object(*meshID),
                                            f2c_idx(*idx),
                                            f2c_xfer(*xfer),
                                            f2c_datatype(*datatype),
                                            f2c_ghosts(*nghosts),
                                            &l_ndims,
                                            &l_nn,
                                            iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims  = c2f_ndims(l_ndims);
  *nnodes = c2f_size(l_nn);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointmesh_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointmesh_all_c,TIO_READ_POINTMESH_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *nnodes,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_nn;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_PointMesh_All( f2c_file(*fileID),
                                          f2c_object(*meshID),
                                          f2c_datatype(*datatype),
                                          &l_ndims,
                                          &l_nn,
                                          iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims  = c2f_ndims(l_ndims);
  *nnodes = c2f_size(l_nn);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointmesh_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointmesh_section_c,TIO_READ_POINTMESH_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *nnodes,
       void                *icoords,
       void                *jcoords,
       void                *kcoords,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_nn;
  void       *iptr, *jptr, *kptr;
  
  
  setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords);
  
  *err = c2f_TIO( TIO_Read_PointMesh_Section( f2c_file(*fileID),
                                              f2c_object(*meshID),
                                              f2c_object(*sectionID),
                                              f2c_xfer(*xfer),
                                              f2c_datatype(*datatype),
                                              &l_ndims, &l_nn,
                                              iptr, jptr, kptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims         = c2f_ndims(l_ndims);
  *nnodes        = c2f_size(l_nn);
  
  return;
}




/**************************************************************************************************/
/**************************************************************************************************/
static void
xtioc_copymeshinfo( const char           *name,
                    const TIO_Mesh_t     meshtype,
                    const TIO_Coord_t    coordtype,
                    const TIO_Bool_t     isAMR,
                    const char           *group,
                    const TIO_Size_t     order,
                    const TIO_Data_t     graph_datatype,
                    const TIO_Data_t     coord_datatype,
                    const TIO_Dims_t     ndims,
                    const TIO_Size_t     n1,
                    const TIO_Size_t     n2,
                    const TIO_Size_t     n3,
                    const TIO_Size_t     n4,
                    const TIO_Size_t     nchunks,
                    const char           *iunits,
                    const char           *junits,
                    const char           *kunits,
                    const char           *ilabel,
                    const char           *jlabel,
                    const char           *klabel,
                    char                 *fname,   const int_f *fname_len,
                    int_f                *fmeshtype,
                    int_f                *fcoordtype,
                    int_f                *fisAMR,
                    char                 *fgroup,  const int_f *fgroup_len,
                    TIO_Size_tf          *forder,
                    int_f                *fgraph_datatype,
                    int_f                *fcoord_datatype,
                    int_f                *fndims,
                    TIO_Size_tf          *fn1,
                    TIO_Size_tf          *fn2,
                    TIO_Size_tf          *fn3,
                    TIO_Size_tf          *fn4,
                    TIO_Size_tf          *fnchunks,
                    char                 *fiunits, const int_f *fiunits_len,
                    char                 *fjunits, const int_f *fjunits_len,
                    char                 *fkunits, const int_f *fkunits_len,
                    char                 *filabel, const int_f *filabel_len,
                    char                 *fjlabel, const int_f *fjlabel_len,
                    char                 *fklabel, const int_f *fklabel_len )
{
  if (name != NULL) c2f_str(name, fname_len, fname);
  
  *fmeshtype  = c2f_meshtype(meshtype);
  *fcoordtype = c2f_coordtype(coordtype);
  
  if (isAMR == TIO_TRUE)
    *fisAMR = 1;
  else
    *fisAMR = 0;
  
  c2f_str(group, fgroup_len, fgroup);
  
  *forder          = c2f_size(order);
  *fgraph_datatype = c2f_datatype(graph_datatype);
  *fcoord_datatype = c2f_datatype(coord_datatype);
  *fndims          = c2f_ndims(ndims);
  
  *fn1      = c2f_size(n1);
  *fn2      = c2f_size(n2);
  *fn3      = c2f_size(n3);
  *fn4      = c2f_size(n4);
  *fnchunks = c2f_size(nchunks);
  
  c2f_str(iunits, fiunits_len, fiunits);
  c2f_str(junits, fjunits_len, fjunits);
  c2f_str(kunits, fkunits_len, fkunits);
  c2f_str(ilabel, filabel_len, filabel);
  c2f_str(jlabel, fjlabel_len, fjlabel);
  c2f_str(klabel, fklabel_len, fklabel);
  
  return;
}



/*
 * EOF
 */

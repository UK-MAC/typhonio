/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tiof_fkit.h"
#include "typhonio.h"



static void xtioc_copyquadinfo(  const TIO_Dims_t ndims,
                                 const TIO_Size_t *ni,
                                 const TIO_Size_t *nj,
                                 const TIO_Size_t *nk,
                                 const TIO_Size_t il,
                                 const TIO_Size_t ih,
                                 const TIO_Size_t jl,
                                 const TIO_Size_t jh,
                                 const TIO_Size_t kl,
                                 const TIO_Size_t kh,
                                 const TIO_Size_t nmixcell,
                                 const TIO_Size_t nmixcomp,
                                 int_f            *fndims,
                                 TIO_Size_tf      *fni,
                                 TIO_Size_tf      *fnj,
                                 TIO_Size_tf      *fnk,
                                 TIO_Size_tf      *fil,
                                 TIO_Size_tf      *fih,
                                 TIO_Size_tf      *fjl,
                                 TIO_Size_tf      *fjh,
                                 TIO_Size_tf      *fkl,
                                 TIO_Size_tf      *fkh,
                                 TIO_Size_tf      *fnmixcell,
                                 TIO_Size_tf      *fnmixcomp );

static void xtioc_copyunstrinfo( const TIO_Dims_t ndims,
                                 const TIO_Size_t nnodes,
                                 const TIO_Size_t ncells,
                                 const TIO_Size_t nshapes,
                                 const TIO_Size_t nconnectivity,
                                 const TIO_Size_t nghost_nodes,
                                 const TIO_Size_t nghost_cells,
                                 const TIO_Size_t nghost_shapes,
                                 const TIO_Size_t nghost_connectivity,
                                 const TIO_Size_t nmixcell,
                                 const TIO_Size_t nmixcomp,
                                 int_f            *fndims,
                                 TIO_Size_tf      *fnnodes,
                                 TIO_Size_tf      *fncells,
                                 TIO_Size_tf      *fnshapes,
                                 TIO_Size_tf      *fnconnectivity,
                                 TIO_Size_tf      *fnghost_nodes,
                                 TIO_Size_tf      *fnghost_cells,
                                 TIO_Size_tf      *fnghost_shapes,
                                 TIO_Size_tf      *fnghost_connectivity,
                                 TIO_Size_tf      *fnmixcell,
                                 TIO_Size_tf      *fnmixcomp );

static void xtioc_copypointinfo( const TIO_Dims_t ndims,
                                 const TIO_Size_t *nnodes,
                                 const TIO_Size_t nl,
                                 const TIO_Size_t nh,
                                 const TIO_Size_t nghost_nodes,
                                 int_f            *fndims,
                                 TIO_Size_tf      *fnnodes,
                                 TIO_Size_tf      *fnl,
                                 TIO_Size_tf      *fnh,
                                 TIO_Size_tf      *fnghost_nodes );



/***************************************************************************************************
 *
 *   tio_get_nchunks_c
 *
 **************************************************************************************************/
void
F2C(tio_get_nchunks_c,TIO_GET_NCHUNKS_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *objectID,
       TIO_Size_tf         *nchunks,
       TIO_tf              *err )
{
  TIO_Size_t l_nchunks;


  *err = c2f_TIO( TIO_Get_nchunks( f2c_file(*fileID),
                                   f2c_object(*objectID),
                                   &l_nchunks ) );
  
  *nchunks = c2f_size(l_nchunks);

  return;
}



/***************************************************************************************************
 *
 *   tio_set_quad_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_set_quad_chunk_c,TIO_SET_QUAD_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *ndims,
       const TIO_Size_tf   *il,
       const TIO_Size_tf   *ih,
       const TIO_Size_tf   *jl,
       const TIO_Size_tf   *jh,
       const TIO_Size_tf   *kl,
       const TIO_Size_tf   *kh,
       const TIO_Size_tf   *nmixcell,
       const TIO_Size_tf   *nmixcomp,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Set_Quad_Chunk( f2c_file(*fileID),
                                      f2c_object(*meshID),
                                      f2c_idx(*idx),
                                      f2c_ndims(*ndims),
                                      f2c_idx(*il),
                                      f2c_idx(*ih),
                                      f2c_idx(*jl),
                                      f2c_idx(*jh),
                                      f2c_idx(*kl),
                                      f2c_idx(*kh),
                                      f2c_size(*nmixcell),
                                      f2c_size(*nmixcomp) ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_quad_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_get_quad_chunk_c,TIO_GET_QUAD_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       int_f               *ndims,
       TIO_Size_tf         *il,
       TIO_Size_tf         *ih,
       TIO_Size_tf         *jl,
       TIO_Size_tf         *jh,
       TIO_Size_tf         *kl,
       TIO_Size_tf         *kh,
       TIO_Size_tf         *nmixcell,
       TIO_Size_tf         *nmixcomp,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_il, l_ih, l_jl, l_jh, l_kl, l_kh, l_nmx, l_nmc;


  *err = c2f_TIO( TIO_Get_Quad_Chunk( f2c_file(*fileID),
                                      f2c_object(*meshID),
                                      f2c_idx(*idx),
                                      &l_ndims,
                                      &l_il, &l_ih,
                                      &l_jl, &l_jh,
                                      &l_kl, &l_kh,
                                      &l_nmx, &l_nmc ) );
  
  if (*err != TIO_SUCCESS_F) return;

  xtioc_copyquadinfo(l_ndims, NULL, NULL, NULL, l_il, l_ih, l_jl, l_jh, l_kl, l_kh, l_nmx, l_nmc,
                     ndims, NULL, NULL, NULL, il, ih, jl, jh, kl, kh, nmixcell, nmixcomp );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_set_quad_section_c
 *
 **************************************************************************************************/
void
F2C(tio_set_quad_section_c,TIO_SET_QUAD_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       TIO_Object_tf       *sectionID,
       const int_f         *ndims,
       const TIO_Size_tf   *il,
       const TIO_Size_tf   *ih,
       const TIO_Size_tf   *jl,
       const TIO_Size_tf   *jh,
       const TIO_Size_tf   *kl,
       const TIO_Size_tf   *kh,
       TIO_tf              *err )
{
  TIO_Object_t l_sectionID;

  *err = c2f_TIO( TIO_Set_Quad_Section( f2c_file(*fileID),
                                        f2c_object(*meshID),
                                        &l_sectionID,
                                        f2c_ndims(*ndims),
                                        f2c_idx(*il),
                                        f2c_idx(*ih),
                                        f2c_idx(*jl),
                                        f2c_idx(*jh),
                                        f2c_idx(*kl),
                                        f2c_idx(*kh) ) );
  if (*err != TIO_SUCCESS_F) return;

  *sectionID = c2f_object(l_sectionID);

  return;
}



/***************************************************************************************************
 *
 *   tio_set_unstr_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_set_unstr_chunk_c,TIO_SET_UNSTR_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *ndims,
       const TIO_Size_tf   *nnodes,
       const TIO_Size_tf   *ncells,
       const TIO_Size_tf   *nshapes,
       const TIO_Size_tf   *nconnectivity,
       const TIO_Size_tf   *nghost_nodes,
       const TIO_Size_tf   *nghost_cells,
       const TIO_Size_tf   *nghost_shapes,
       const TIO_Size_tf   *nghost_connectivity,
       const TIO_Size_tf   *nmixcell,
       const TIO_Size_tf   *nmixcomp,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Set_Unstr_Chunk( f2c_file(*fileID),
                                       f2c_object(*meshID),
                                       f2c_idx(*idx),
                                       f2c_ndims(*ndims),
                                       f2c_size(*nnodes),
                                       f2c_size(*ncells),
                                       f2c_size(*nshapes),
                                       f2c_size(*nconnectivity),
                                       f2c_size(*nghost_nodes),
                                       f2c_size(*nghost_cells),
                                       f2c_size(*nghost_shapes),
                                       f2c_size(*nghost_connectivity),
                                       f2c_size(*nmixcell),
                                       f2c_size(*nmixcomp) ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_unstr_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_get_unstr_chunk_c,TIO_GET_UNSTR_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       int_f               *ndims,
       TIO_Size_tf         *nnodes,
       TIO_Size_tf         *ncells,
       TIO_Size_tf         *nshapes,
       TIO_Size_tf         *nconnectivity,
       TIO_Size_tf         *nghost_nodes,
       TIO_Size_tf         *nghost_cells,
       TIO_Size_tf         *nghost_shapes,
       TIO_Size_tf         *nghost_connectivity,
       TIO_Size_tf         *nmixcell,
       TIO_Size_tf         *nmixcomp,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_nn, l_nc, l_ns, l_nv, l_ngn, l_ngc, l_ngs, l_ngv, l_nmx, l_nmc;


  *err = c2f_TIO( TIO_Get_Unstr_Chunk( f2c_file(*fileID),
                                       f2c_object(*meshID),
                                       f2c_idx(*idx),
                                       &l_ndims,
                                       &l_nn,  &l_nc,  &l_ns,  &l_nv,
                                       &l_ngn, &l_ngc, &l_ngs, &l_ngv,
                                       &l_nmx, &l_nmc ) );
  
  if (*err != TIO_SUCCESS_F) return;

  xtioc_copyunstrinfo(l_ndims, l_nn, l_nc, l_ns, l_nv, l_ngn, l_ngc, l_ngs, l_ngv, l_nmx, l_nmc,
                      ndims, nnodes, ncells, nshapes, nconnectivity,
                      nghost_nodes, nghost_cells, nghost_shapes, nghost_connectivity,
                      nmixcell, nmixcomp);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_set_unstr_section_c
 *
 **************************************************************************************************/
void
F2C(tio_set_unstr_section_c,TIO_SET_UNSTR_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       TIO_Object_tf       *sectionID,
       const int_f         *centring,
       const TIO_Size_tf   *nIDs,
       const TIO_Size_tf   *IDs,
       TIO_tf              *err )
{
  TIO_Object_t l_sectionID;
  TIO_Size_t   *l_IDs;
  TIO_Size_t   l_nIDs;

  TIO_Size_t  *cptr;
  const TIO_Size_tf *fptr;
  TIO_Size_t  ii;


  l_nIDs = f2c_size(*nIDs);

  l_IDs = NULL;
  l_IDs = (TIO_Size_t *)malloc(sizeof(TIO_Size_t) * (size_t)l_nIDs);
  if (l_IDs == NULL) {
    *err = c2f_TIO(TIO_ERR_F90_API);
    return;
  }

  cptr = l_IDs;
  fptr = IDs;

  for (ii=0; ii<l_nIDs; ++ii) {
    *cptr = f2c_size(*fptr);      /* Stored as 1-origin IDs, not arbitrary index */
    ++cptr;
    ++fptr;
  }

  *err = c2f_TIO( TIO_Set_Unstr_Section( f2c_file(*fileID),
                                         f2c_object(*meshID),
                                         &l_sectionID,
                                         f2c_centre(*centring),
                                         l_nIDs,
                                         l_IDs ) );
  free (l_IDs);

  if (*err != TIO_SUCCESS_F) return;


  *sectionID = c2f_object(l_sectionID);

  return;
}



/***************************************************************************************************
 *
 *   tio_set_point_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_set_point_chunk_c,TIO_SET_POINT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       const int_f         *ndims,
       const TIO_Size_tf   *nl,
       const TIO_Size_tf   *nh,
       const TIO_Size_tf   *nghost_nodes,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Set_Point_Chunk( f2c_file(*fileID),
                                       f2c_object(*meshID),
                                       f2c_idx(*idx),
                                       f2c_ndims(*ndims),
                                       f2c_idx(*nl),
                                       f2c_idx(*nh),
                                       f2c_size(*nghost_nodes) ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_point_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_get_point_chunk_c,TIO_GET_POINT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       int_f               *ndims,
       TIO_Size_tf         *nl,
       TIO_Size_tf         *nh,
       TIO_Size_tf         *nghost_nodes,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_nl, l_nh, l_ngn;


  *err = c2f_TIO( TIO_Get_Point_Chunk( f2c_file(*fileID),
                                       f2c_object(*meshID),
                                       f2c_idx(*idx),
                                       &l_ndims,
                                       &l_nl,  &l_nh,
                                       &l_ngn ) );
  
  if (*err != TIO_SUCCESS_F) return;

  xtioc_copypointinfo(l_ndims, NULL, l_nl, l_nh, l_ngn,
                      ndims,   NULL, nl,   nh,   nghost_nodes);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_set_point_section_c
 *
 **************************************************************************************************/
void
F2C(tio_set_point_section_c,TIO_SET_POINT_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       TIO_Object_tf       *sectionID,
       const TIO_Size_tf   *nIDs,
       const TIO_Size_tf   *IDs,
       TIO_tf              *err )
{
  TIO_Object_t l_sectionID;
  TIO_Size_t   *l_IDs;
  TIO_Size_t   l_nIDs;

  TIO_Size_t  *cptr;
  const TIO_Size_tf *fptr;
  TIO_Size_t  ii;


  l_nIDs = f2c_size(*nIDs);

  l_IDs = NULL;
  l_IDs = (TIO_Size_t *)malloc(sizeof(TIO_Size_t) * (size_t)l_nIDs);
  if (l_IDs == NULL) {
    *err = c2f_TIO(TIO_ERR_F90_API);
    return;
  }

  cptr = l_IDs;
  fptr = IDs;

  for (ii=0; ii<l_nIDs; ++ii) {
    *cptr = f2c_idx(*fptr);      /* Assume user gives 1-origin arbitrary indices */
    ++cptr;
    ++fptr;
  }

  *err = c2f_TIO( TIO_Set_Point_Section( f2c_file(*fileID),
                                         f2c_object(*meshID),
                                         &l_sectionID,
                                         l_nIDs,
                                         l_IDs ) );
  free (l_IDs);

  if (*err != TIO_SUCCESS_F) return;


  *sectionID = c2f_object(l_sectionID);

  return;
}



/**************************************************************************************************/

static void
xtioc_copyquadinfo(  const TIO_Dims_t ndims,
                     const TIO_Size_t *ni,
                     const TIO_Size_t *nj,
                     const TIO_Size_t *nk,
                     const TIO_Size_t il,
                     const TIO_Size_t ih,
                     const TIO_Size_t jl,
                     const TIO_Size_t jh,
                     const TIO_Size_t kl,
                     const TIO_Size_t kh,
                     const TIO_Size_t nmixcell,
                     const TIO_Size_t nmixcomp,
                     int_f            *fndims,
                     TIO_Size_tf      *fni,
                     TIO_Size_tf      *fnj,
                     TIO_Size_tf      *fnk,
                     TIO_Size_tf      *fil,
                     TIO_Size_tf      *fih,
                     TIO_Size_tf      *fjl,
                     TIO_Size_tf      *fjh,
                     TIO_Size_tf      *fkl,
                     TIO_Size_tf      *fkh,
                     TIO_Size_tf      *fnmixcell,
                     TIO_Size_tf      *fnmixcomp )
{
  *fndims = c2f_ndims(ndims);

  if (ni != NULL) *fni = c2f_size(*ni);
  if (nj != NULL) *fnj = c2f_size(*nj);
  if (nk != NULL) *fnk = c2f_size(*nk);

  *fil = c2f_idx(il);
  *fih = c2f_idx(ih);
  *fjl = c2f_idx(jl);
  *fjh = c2f_idx(jh);
  *fkl = c2f_idx(kl);
  *fkh = c2f_idx(kh);

  *fnmixcell = c2f_size(nmixcell);
  *fnmixcomp = c2f_size(nmixcomp);

  return;
}


static void
xtioc_copyunstrinfo( const TIO_Dims_t ndims,
                     const TIO_Size_t nnodes,
                     const TIO_Size_t ncells,
                     const TIO_Size_t nshapes,
                     const TIO_Size_t nconnectivity,
                     const TIO_Size_t nghost_nodes,
                     const TIO_Size_t nghost_cells,
                     const TIO_Size_t nghost_shapes,
                     const TIO_Size_t nghost_connectivity,
                     const TIO_Size_t nmixcell,
                     const TIO_Size_t nmixcomp,
                     int_f            *fndims,
                     TIO_Size_tf      *fnnodes,
                     TIO_Size_tf      *fncells,
                     TIO_Size_tf      *fnshapes,
                     TIO_Size_tf      *fnconnectivity,
                     TIO_Size_tf      *fnghost_nodes,
                     TIO_Size_tf      *fnghost_cells,
                     TIO_Size_tf      *fnghost_shapes,
                     TIO_Size_tf      *fnghost_connectivity,
                     TIO_Size_tf      *fnmixcell,
                     TIO_Size_tf      *fnmixcomp )
{
  *fndims = c2f_ndims(ndims);

  *fnnodes        = c2f_size(nnodes);
  *fncells        = c2f_size(ncells);
  *fnshapes       = c2f_size(nshapes);
  *fnconnectivity = c2f_size(nconnectivity);

  *fnghost_nodes        = c2f_size(nghost_nodes);
  *fnghost_cells        = c2f_size(nghost_cells);
  *fnghost_shapes       = c2f_size(nghost_shapes);
  *fnghost_connectivity = c2f_size(nghost_connectivity);

  *fnmixcell = c2f_size(nmixcell);
  *fnmixcomp = c2f_size(nmixcomp);

  return;
}


static void
xtioc_copypointinfo( const TIO_Dims_t ndims,
                     const TIO_Size_t *nnodes,
                     const TIO_Size_t nl,
                     const TIO_Size_t nh,
                     const TIO_Size_t nghost_nodes,
                     int_f            *fndims,
                     TIO_Size_tf      *fnnodes,
                     TIO_Size_tf      *fnl,
                     TIO_Size_tf      *fnh,
                     TIO_Size_tf      *fnghost_nodes )
{
  *fndims = c2f_ndims(ndims);

  if (nnodes != NULL) *fnnodes = c2f_size(*nnodes);

  *fnl = c2f_idx(nl);
  *fnh = c2f_idx(nh);

  *fnghost_nodes = c2f_size(nghost_nodes);

  return;
}


/*
 * EOF
 */


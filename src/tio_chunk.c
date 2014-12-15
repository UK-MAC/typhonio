/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"
#include "tio_register_int.h"

#include "tio_mesh_int.h"

#include "tio_chunk.h"


static TIO_t cTIO_SetChunk(   const char          *subname,
                              const TIO_File_t    fileID,
                              const TIO_Object_t  meshID,
                              const TIO_Mesh_t    meshtype,
                              const TIO_Size_t    idx,
                              const TIO_Dims_t    ndims,
                              const union iInfo_t *info );


static TIO_t cTIO_GetChunk(   const char          *subname,
                              const TIO_File_t    fileID,
                              const TIO_Object_t  meshID,
                              const TIO_Mesh_t    meshtype,
                              const TIO_Size_t    idx,
                              TIO_Dims_t          *ndims,
                              TIO_Size_t          *nghosts,
                              int                 *ismixed,
                              union iInfo_t       *info,
                              TIO_Size_t          *nchunks,
                              const int           nchunks_only );


TIO_t cTIO_SetSection(        const char             *subname,
                              const TIO_File_t       fileID,
                              const TIO_Object_t     meshID,
                              TIO_Object_t           *sectionID,
                              const TIO_Mesh_t       meshtype,
                              const TIO_Dims_t       ndims,
                              const union iSection_t *section );



/***************************************************************************************************
 *
 * TIO_Get_nchunks
 *
 **************************************************************************************************/
TIO_t
TIO_Get_nchunks( const TIO_File_t   fileID,
                 const TIO_Object_t objectID,
                 TIO_Size_t         *nchunks )
{
  const char *subname = "TIO_Get_nchunks()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_GetChunk( subname, fileID, objectID, TIO_MESH_NULL, TIO_NULL,
                       NULL, NULL, NULL, NULL, nchunks, TRUE );

  TIOend(subname,1);

  return (trc);
}




/*********************************
 * ############################# *
 * #                           # *
 * #          Quad             # *
 * #                           # *
 * ############################# *
 *********************************/


/***************************************************************************************************
 *
 * TIO_Set_Quad_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Quad_Chunk( const TIO_File_t   fileID,
                    const TIO_Object_t meshID,
                    const TIO_Size_t   idx,
                    const TIO_Dims_t   ndims,
                    const TIO_Size_t   il,
                    const TIO_Size_t   ih,
                    const TIO_Size_t   jl,
                    const TIO_Size_t   jh,
                    const TIO_Size_t   kl,
                    const TIO_Size_t   kh,
                    const TIO_Size_t   nmixcell,
                    const TIO_Size_t   nmixcomp )
{
  const char *subname = "TIO_Set_Quad_Chunk()";

  union iInfo_t info;

  TIO_t trc;


  TIObegin(subname,1);

  info.quad.ni = info.quad.nj = info.quad.nk = 0;
  info.quad.il = info.quad.jl = info.quad.kl = 0;
  info.quad.ih = info.quad.jh = info.quad.kh = 0;

  switch (ndims) {
  case TIO_3D:
    info.quad.kl = kl;
    info.quad.kh = kh;
    info.quad.nk = kh - kl + 1;
    /* Drop through */
  case TIO_2D:
    info.quad.jl = jl;
    info.quad.jh = jh;
    info.quad.nj = jh - jl + 1;
    /* Drop through */
  case TIO_1D:
    info.quad.il = il;
    info.quad.ih = ih;
    info.quad.ni = ih - il + 1;
    break;
  default:
    TIOreturn(ERR_USER, "Invalid ndims - Must use TIO_1D, TIO_2D or TIO_3D", TIO_ERR_INVALID_ARG);
    break;
  }

  info.quad.nmixcell = nmixcell;
  info.quad.nmixcomp = nmixcomp;

#ifndef NDEBUG
  info.quad.nghosts  = 888888;
#endif


  /* Passing MESH_QUAD_COLINEAR to signify both colinear and non-colinear meshes
     - ok to do this as info is same and check will still be good later */

  trc = cTIO_SetChunk( subname, fileID, meshID, TIO_MESH_QUAD_COLINEAR, idx, ndims, &info);

  TIOend(subname,1);

  return (trc);

}




/***************************************************************************************************
 *
 * TIO_Get_Quad_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Quad_Chunk( const TIO_File_t     fileID,
                    const TIO_Object_t   objectID,
                    const TIO_Size_t     idx,
                    TIO_Dims_t           *ndims,
                    TIO_Size_t           *il,
                    TIO_Size_t           *ih,
                    TIO_Size_t           *jl,
                    TIO_Size_t           *jh,
                    TIO_Size_t           *kl,
                    TIO_Size_t           *kh,
                    TIO_Size_t           *nmixcell,
                    TIO_Size_t           *nmixcomp )
{
  const char *subname="TIO_Get_Quad_Chunk()";

  union iInfo_t info;
  int           ismixed;

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_GetChunk( subname, fileID, objectID, TIO_MESH_QUAD_COLINEAR, idx,
                       ndims, NULL, &ismixed, &info, NULL, FALSE );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  TIOset( il, info.quad.il );
  TIOset( ih, info.quad.ih );
  TIOset( jl, info.quad.jl );
  TIOset( jh, info.quad.jh );
  TIOset( kl, info.quad.kl );
  TIOset( kh, info.quad.kh );

  if (ismixed) {
    TIOset( nmixcell, info.quad.nmixcell );
    TIOset( nmixcomp, info.quad.nmixcomp );
  }
  else {
    TIOset( nmixcell, 0 );
    TIOset( nmixcomp, 0 );
  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Set_Quad_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Quad_Section( const TIO_File_t   fileID,
                      const TIO_Object_t meshID,
                      TIO_Object_t       *sectionID,
                      const TIO_Dims_t   ndims,
                      const TIO_Size_t   il,
                      const TIO_Size_t   ih,
                      const TIO_Size_t   jl,
                      const TIO_Size_t   jh,
                      const TIO_Size_t   kl,
                      const TIO_Size_t   kh )
{
  const char *subname = "TIO_Set_Quad_Section()";

  union iSection_t section;

  TIO_t trc;


  TIObegin(subname,1);

  section.quad.il = il;
  section.quad.ih = ih;
  section.quad.jl = jl;
  section.quad.jh = jh;
  section.quad.kl = kl;
  section.quad.kh = kh;

  trc = cTIO_SetSection(subname, fileID, meshID, sectionID, TIO_MESH_QUAD_COLINEAR,
                        ndims, &section);
  if (trc != TIO_SUCCESS) {
    return(trc);
  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/*********************************
 * ############################# *
 * #                           # *
 * #          Unstr            # *
 * #                           # *
 * ############################# *
 *********************************/


/***************************************************************************************************
 *
 * TIO_Set_Unstr_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Unstr_Chunk( const TIO_File_t   fileID,
                     const TIO_Object_t meshID,
                     const TIO_Size_t   idx,
                     const TIO_Dims_t   ndims,
                     const TIO_Size_t   nnodes,
                     const TIO_Size_t   ncells,
                     const TIO_Size_t   nshapes,
                     const TIO_Size_t   nconnectivity,
                     const TIO_Size_t   nghost_nodes,
                     const TIO_Size_t   nghost_cells,
                     const TIO_Size_t   nghost_shapes,
                     const TIO_Size_t   nghost_connectivity,
                     const TIO_Size_t   nmixcell,
                     const TIO_Size_t   nmixcomp )
{
  const char *subname="TIO_Set_Unstr_Chunk()";

  union iInfo_t info;

  TIO_t trc;


  TIObegin(subname,1);

  info.unstr.nnodes              = nnodes;
  info.unstr.ncells              = ncells;
  info.unstr.nshapes             = nshapes;
  info.unstr.nconnectivity       = nconnectivity;
  info.unstr.nghost_nodes        = nghost_nodes;
  info.unstr.nghost_cells        = nghost_cells;
  info.unstr.nghost_shapes       = nghost_shapes;
  info.unstr.nghost_connectivity = nghost_connectivity;
  info.unstr.nmixcell            = nmixcell;
  info.unstr.nmixcomp            = nmixcomp;

  trc = cTIO_SetChunk( subname, fileID, meshID, TIO_MESH_UNSTRUCT, idx, ndims, &info);

  TIOend(subname,1);


  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Get_Unstr_Chunk
 *
 **************************************************************************************************/
TIO_t TIO_Get_Unstr_Chunk(  const TIO_File_t   fileID,
                            const TIO_Object_t objectID,
                            const TIO_Size_t   idx,
                            TIO_Dims_t         *ndims,
                            TIO_Size_t         *nnodes,
                            TIO_Size_t         *ncells,
                            TIO_Size_t         *nshapes,
                            TIO_Size_t         *nconnectivity,
                            TIO_Size_t         *nghost_nodes,
                            TIO_Size_t         *nghost_cells,
                            TIO_Size_t         *nghost_shapes,
                            TIO_Size_t         *nghost_connectivity,
                            TIO_Size_t         *nmixcell,
                            TIO_Size_t         *nmixcomp )

{
  const char *subname="TIO_Get_Unstr_Chunk()";

  union iInfo_t info;
  TIO_Size_t    ng;
  int           ismixed;

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_GetChunk( subname, fileID, objectID, TIO_MESH_UNSTRUCT, idx,
                       ndims, &ng, &ismixed, &info, NULL, FALSE );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (TIO_GHOSTS_NONE == ng) {
    info.unstr.nghost_nodes        = 0;
    info.unstr.nghost_cells        = 0;
    info.unstr.nghost_shapes       = 0;
    info.unstr.nghost_connectivity = 0;
  }

  TIOset( nnodes,              info.unstr.nnodes              );
  TIOset( ncells,              info.unstr.ncells              );
  TIOset( nshapes,             info.unstr.nshapes             );
  TIOset( nconnectivity,       info.unstr.nconnectivity       );
  TIOset( nghost_nodes,        info.unstr.nghost_nodes        );
  TIOset( nghost_cells,        info.unstr.nghost_cells        );
  TIOset( nghost_shapes,       info.unstr.nghost_shapes       );
  TIOset( nghost_connectivity, info.unstr.nghost_connectivity );

  if (ismixed) {
    TIOset( nmixcell, info.unstr.nmixcell );
    TIOset( nmixcomp, info.unstr.nmixcomp );
  }
  else {
    TIOset( nmixcell, 0 );
    TIOset( nmixcomp, 0 );
  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Set_Unstr_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Unstr_Section( const TIO_File_t   fileID,
                       const TIO_Object_t meshID,
                       TIO_Object_t       *sectionID,
                       const TIO_Centre_t centring,
                       const TIO_Size_t   nIDs,
                       const TIO_Size_t   IDs[] )
{
  const char *subname="TIO_Set_Unstr_Section()";

  union iSection_t section;

  TIO_t trc;


  TIObegin(subname,1);

  if ((centring != TIO_CENTRE_NODE) && (centring != TIO_CENTRE_CELL)) {
    TIOreturn(ERR_USER, "Invalid centring - must be nodal or cell", TIO_ERR_INVALID_ARG);
  }

  section.unstr.nIDs     = nIDs;
  section.unstr.IDs      = (TIO_Size_t *)IDs;
  section.unstr.centring = centring;

  trc = cTIO_SetSection( subname, fileID, meshID, sectionID, TIO_MESH_UNSTRUCT,
                         TIO_DIMS_NULL, &section);
  if (trc != TIO_SUCCESS) {
    return(trc);
  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/*********************************
 * ############################# *
 * #                           # *
 * #          Point            # *
 * #                           # *
 * ############################# *
 *********************************/


/***************************************************************************************************
 *
 * TIO_Set_Point_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Point_Chunk( const TIO_File_t   fileID,
                     const TIO_Object_t meshID,
                     const TIO_Size_t   idx,
                     const TIO_Dims_t   ndims,
                     const TIO_Size_t   nl,
                     const TIO_Size_t   nh,
                     const TIO_Size_t   nghosts )
{
  const char *subname="TIO_Set_Point_Chunk()";

  union iInfo_t info;

  TIO_t trc;


  TIObegin(subname,1);

  info.point.nnodes  = nh - nl + 1;
  info.point.nl      = nl;
  info.point.nh      = nh;
  info.point.nghosts = nghosts;

  trc = cTIO_SetChunk(subname, fileID, meshID, TIO_MESH_POINT, idx, ndims, &info);

  TIOend(subname,1);

  return (trc);
}


/***************************************************************************************************
 *
 * TIO_Get_Point_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Point_Chunk( const TIO_File_t   fileID,
                     const TIO_Object_t objectID,
                     const TIO_Size_t   idx,
                     TIO_Dims_t         *ndims,
                     TIO_Size_t         *nl,
                     TIO_Size_t         *nh,
                     TIO_Size_t         *nghosts )
{
  const char *subname="TIO_Get_Point_Chunk()";

  union iInfo_t info;
  TIO_Size_t    ng;

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_GetChunk( subname, fileID, objectID, TIO_MESH_POINT, idx, ndims,
                       &ng, NULL, &info,  NULL, FALSE );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (TIO_GHOSTS_NONE == ng) {
    info.point.nghosts = 0;
  }
  TIOset( nl,      info.point.nl      );
  TIOset( nh,      info.point.nh      );
  TIOset( nghosts, info.point.nghosts );

  TIOend(subname,1);


  return (TIO_SUCCESS);
}



/***************************************************************************************************
 *
 * TIO_Set_Point_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Point_Section( const TIO_File_t   fileID,
                       const TIO_Object_t meshID,
                       TIO_Object_t       *sectionID,
                       const TIO_Size_t   nIDs,
                       const TIO_Size_t   IDs[] )
{
  const char *subname="TIO_Set_Point_Section()";

  union iSection_t section;

  TIO_t trc;


  TIObegin(subname,1);

  section.point.nIDs = nIDs;
  section.point.IDs  = (TIO_Size_t *)IDs;

  trc = cTIO_SetSection( subname, fileID, meshID, sectionID, TIO_MESH_POINT,
                         TIO_DIMS_NULL, &section);
  if (trc != TIO_SUCCESS) {
    return(trc);
  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}





/***************************************************************************************************
 *
 * cTIO_SetChunk
 *
 **************************************************************************************************/
static TIO_t
cTIO_SetChunk( const char          *subname,
               const TIO_File_t    fileID,
               const TIO_Object_t  meshID,
               const TIO_Mesh_t    meshtype,
               const TIO_Size_t    idx,
               const TIO_Dims_t    ndims,
               const union iInfo_t *info )
{
  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;    /* parent ID */
  int   irc;
  TIO_t trc;


  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (tmesh->status != STATUS_NEW) {
    TIOreturn(ERR_USER, "Mesh chunks already defined", TIO_ERR_INVALID_OPERATION  );
  }
  if (0 == tmesh->nchunks) {
    TIOreturn(ERR_USER, "Mesh was not created with chunks", TIO_ERR_INVALID_OPERATION  );
  }
  if (tmesh->meshtype != meshtype) {
    if ((TIO_MESH_QUAD_COLINEAR == meshtype) && (TIO_MESH_QUAD_NONCOLINEAR == tmesh->meshtype)) {
      ; /* ok */
    }
    else {
      TIOreturn(ERR_USER, "Invalid meshID - chunk must be of meshtype", TIO_ERR_INVALID_OPERATION);
    }
  }

  if (ndims != tmesh->ndims) {
    TIOreturn(ERR_USER, "Invalid ndims - does not match parent mesh ndims", TIO_ERR_INVALID_ARG);
  }

  if (idx >= tmesh->nchunks) {
    TIOreturn(ERR_USER, "Chunk idx is larger than nchunks in mesh", TIO_ERR_INVALID_INDEX);
  }

  if (tmesh->chunkset[idx]) {
    TIOreturn(ERR_USER, "Chunk with idx has already been set", TIO_ERR_INVALID_ARG);
  }

  tmesh->chunkinfo[idx] = *info;
  tmesh->chunkset[idx]  = TRUE;


  /* Increment counter of global mix cells and components over all chunks
     - this duplicates mixed ghosts, but this doesn't matter */

  switch (meshtype) {
  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:
    tmesh->info.quad.nmixcell += info->quad.nmixcell;
    tmesh->info.quad.nmixcomp += info->quad.nmixcomp;
    break;
  case TIO_MESH_UNSTRUCT:
    tmesh->info.unstr.nmixcell += info->unstr.nmixcell;
    tmesh->info.unstr.nmixcomp += info->unstr.nmixcomp;
    break;
  case TIO_MESH_POINT:
    break;
  default:
    TIOreturn(ERR_INT, "Invalid meshtype", TIO_ERR_INT);
    break;
  }


  --tmesh->nchunks_unset;


  if (0 == tmesh->nchunks_unset) {

    irc = iTIO_CreateMeshDatasets(fileID, lpID, meshID, tfile, tmesh);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to create mesh datasets - check sizes", TIO_ERR_OBJECT_CREATE);
    }
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_GetChunk
 *
 **************************************************************************************************/
static TIO_t
cTIO_GetChunk( const char         *subname,
               const TIO_File_t   fileID,
               const TIO_Object_t objectID,
               const TIO_Mesh_t   meshtype,       /* unused */
               const TIO_Size_t   idx,
               TIO_Dims_t         *ndims,
               TIO_Size_t         *nghosts,
               int                *ismixed,
               union iInfo_t      *info,
               TIO_Size_t         *nchunks,
               const int          nchunks_only )
{
  TIO_Size_t    lnchunks;
  union iInfo_t *chunkinfo;

  iClass_t     class;

  TIO_Dims_t   lndims;
  TIO_Size_t   lng;
  int          lmix;

  TIO_t        trc;


  trc = cTIO_HierarchyGetChunkedObjectInfo( subname, fileID, objectID,
                                            &class, &lndims, &lng, &lmix, &lnchunks, &chunkinfo);
  if (trc != TIO_SUCCESS) {
    cTIOreturn(trc);
  }

  if (CLASS_MESH == class) {
    lng  = TIO_GHOSTS_NULL;
    lmix = TIO_TRUE;
  }


  if (nchunks_only) {
    TIOset( nchunks, lnchunks );
  }
  else{

    if (idx >= lnchunks) {
      TIOreturn(ERR_USER, "Invalid idx - chunk does not exist", TIO_ERR_INVALID_INDEX);
    }
    TIOset( ndims,   lndims         );
    TIOset( nghosts, lng            );
    TIOset( ismixed, lmix           );
    TIOset( info,    chunkinfo[idx] );
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 **************************************************************************************************/
TIO_t
cTIO_SetSection( const char             *subname,
                 const TIO_File_t       fileID,
                 const TIO_Object_t     meshID,
                 TIO_Object_t           *sectionID,
                 const TIO_Mesh_t       meshtype,
                 const TIO_Dims_t       ndims,
                 const union iSection_t *section )
{
  struct iFile_t    *tfile;
  struct iMesh_t    *tmesh;
  union  iSection_t *tsection;

  TIO_Object_t lsectID;
  size_t       nsect;
  TIO_Centre_t centring;

  int   irc;
  TIO_t trc;

  *sectionID = TIO_NULL;   /* Ensure NULL value on failure */

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  switch (meshtype) {
  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    if (ndims != tmesh->ndims) {
      TIOreturn(ERR_USER, "ndims does not match that of mesh", TIO_ERR_INVALID_DIMS);
    }

    switch (ndims) {
    case TIO_3D:
      irc = ((section->quad.kh > tmesh->info.quad.nk) || (section->quad.kl > section->quad.kh));
      if (irc) {
        break;
      }
      /* drop through */
    case TIO_2D:
      irc = ((section->quad.jh > tmesh->info.quad.nj) || (section->quad.jl > section->quad.jh));
      if (irc) {
        break;
      }
      /* drop through */
    case TIO_1D:
      irc = ((section->quad.ih > tmesh->info.quad.ni) || (section->quad.il > section->quad.ih));
      if (irc) {
        break;
      }
      break;

    default:
      TIOassert(TRUE, ERR_INT, "Invalid ndims", TIO_ERR_INT);
      break;
    }
    if (irc) {
      TIOreturn(ERR_USER, "Invalid range specified for section", TIO_ERR_INVALID_RANGE);
    }
    centring = TIO_CENTRE_NULL;   /* Doesn't need anything specific here */
    break;

  case TIO_MESH_UNSTRUCT:
    centring = section->unstr.centring;
    break;

  case TIO_MESH_POINT:
    centring = TIO_CENTRE_NODE;
    break;

  default:
    TIOreturn(ERR_INT, "Invalid meshtype", TIO_ERR_INT);
    break;
  }


  tsection = (union iSection_t *)TIOmalloc(sizeof(union iSection_t));
  if (NULL == tsection) {
    TIOreturn(ERR_MEM, "Failed to malloc tsection", TIO_ERR_INT);
  }
  irc = iTIO_RegisterObject(fileID, meshID, &lsectID, CLASS_SECTION, (void *)tsection);
  TIOassert(irc < 0, ERR_INT, "Failed to register object", TIO_ERR_INT);


  if (NULL == tmesh->sectionCache) {

    const size_t Nalloc = 5;

    tmesh->sectionCache  = TIOmalloc(sizeof(struct iSectionCache_t));
    if (NULL == tmesh->sectionCache) {
      TIOreturn(ERR_MEM, "Failed to malloc tmesh->sectionCache", TIO_ERR_INT);
    }
    tmesh->sectionCache->sectionID = TIOmalloc(sizeof(struct iSectionCache_t) * Nalloc);
    if (NULL == tmesh->sectionCache->sectionID) {
      TIOreturn(ERR_MEM, "Failed to malloc tmesh->sectionCache->sectionID", TIO_ERR_INT);
    }
    tmesh->sectionCache->nlist  = 0;
    tmesh->sectionCache->nalloc = Nalloc;

  }
  else if (tmesh->sectionCache->nlist ==  tmesh->sectionCache->nalloc) {

    size_t Ndelta = 5;

    Ndelta += tmesh->sectionCache->nalloc;

    tmesh->sectionCache->sectionID = TIOrealloc(tmesh->sectionCache->sectionID,
                                                (sizeof(struct iSectionCache_t) * Ndelta));
    if (NULL == tmesh->sectionCache->sectionID) {
      TIOreturn(ERR_MEM, "Failed to realloc tmesh->sectionCache->sectionID", TIO_ERR_INT);
    }
    tmesh->sectionCache->nalloc = Ndelta;

  }
  else {
    /* all ok */
    ;
  }

  nsect = tmesh->sectionCache->nlist;
  ++tmesh->sectionCache->nlist;
  tmesh->sectionCache->sectionID[nsect] = lsectID;


  switch (meshtype) {
  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:
    (void) memcpy(tsection, section, sizeof(union iSection_t));
    break;

  case TIO_MESH_UNSTRUCT:
    tsection->unstr.nIDs = section->unstr.nIDs;
    tsection->unstr.IDs  = (TIO_Size_t *)TIOmalloc(sizeof(TIO_Size_t) * section->unstr.nIDs);
    if (NULL == tsection->unstr.IDs) {
      TIOreturn(ERR_MEM, "Failed to malloc tsection->unstr.IDs", TIO_ERR_INT);
    }
    (void) memcpy(tsection->unstr.IDs, section->unstr.IDs, (size_t)section->unstr.nIDs * sizeof(TIO_Size_t));

    tsection->unstr.centring = centring;

    break;

  case TIO_MESH_POINT:
    tsection->point.nIDs = section->point.nIDs;
    tsection->point.IDs  = (TIO_Size_t *)TIOmalloc(sizeof(TIO_Size_t) * section->point.nIDs);
    if (NULL == tsection->point.IDs) {
      TIOreturn(ERR_MEM, "Failed to malloc tsection->point.IDs", TIO_ERR_INT);
    }
    (void) memcpy(tsection->point.IDs, section->point.IDs, (size_t)section->point.nIDs * sizeof(TIO_Size_t));
    break;

  default:
    break;
  }


  tfile->SetSection(fileID, meshID, lsectID, meshtype, ndims, tsection,
                    iTIO_Centring2Datapos(centring, meshtype,FALSE) );

  *sectionID = lsectID;


  return (TIO_SUCCESS);
}



/*
 * EOF
 */

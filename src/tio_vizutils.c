/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"
#include "tio_register_int.h"

#include "tio_chunk.h"

#include "tio_mesh_int.h"
#include "tio_mesh.h"
#include "typhonio_viz.h"



static size_t xTIO_ShapeSize(const TIO_Shape_t shape);


/*
 * Could've written these routines embedded more in HDF5/whatever,
 * but instead have left them higher-level, so are more maintainable
 * and will use the more basic lower-level routines.
 */


/***************************************************************************************************
 *
 * TIO_Read_Shaped_Connectivity
 *
 **************************************************************************************************/
TIO_t
TIO_Read_Shaped_Connectivity( const TIO_File_t   fileID,
                              const TIO_Object_t meshID,
                              const TIO_Size_t   idx,
                              const TIO_Xfer_t   xfer,
                              const TIO_Shape_t  shape,
                              const TIO_Data_t   datatype,
                              const TIO_Size_t   nghosts,
                              TIO_Size_t         *nconnectivity,
                              void               *connectivity )
{
  const char *subname = "TIO_Read_Shaped_Connectivity()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Size_t   nshapes, ishape, soffset, nallshapes;
  TIO_Shape_t  *shapes,  *sptr;
  TIO_Size_t   *ncps,    *nptr, nc;
  TIO_Size_t   voffset;

  size_t       dtsize, shsize, cpsize;
  char         *vdata, *vptr, *dptr;    /* assuming that char = 1 byte */

  int   irc;
  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (tmesh->meshtype != TIO_MESH_UNSTRUCT) {
    TIOreturn(ERR_USER, "Mesh is not unstruct type - cannot read shaped connectivity",
              TIO_ERR_INVALID_OPERATION);
  }
  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }

  shsize = xTIO_ShapeSize(shape);
  if (0 == shsize) {
    TIOreturn(ERR_USER, "Invalid shape", TIO_ERR_INVALID_ARG);
  }
  dtsize = iTIO_SizeOf(datatype);
  if (0 == dtsize) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }


  if (TIO_GHOSTS_NONE == nghosts) {
    soffset = 0;
    voffset = 0;
    nshapes = tmesh->chunkinfo[idx].unstr.nshapes;
  }
  else if (TIO_GHOSTS_ALL == nghosts) {
    soffset = 0;
    voffset = 0;
    nshapes = tmesh->chunkinfo[idx].unstr.nshapes + tmesh->chunkinfo[idx].unstr.nghost_shapes;
  }
  else if (TIO_GHOSTS_ONLY == nghosts) {
    soffset = tmesh->chunkinfo[idx].unstr.nshapes;
    voffset = tmesh->chunkinfo[idx].unstr.nconnectivity;
    nshapes = tmesh->chunkinfo[idx].unstr.nghost_shapes;
  }
  else {
    TIOreturn(ERR_USER, "Invalid nghosts - use TIO_GHOSTS_[NONE|ONLY|ALL]", TIO_ERR_INVALID_ARG);
  }


  /* Read shape info for whole chunk including all ghosts */

  nallshapes = tmesh->chunkinfo[idx].unstr.nshapes + tmesh->chunkinfo[idx].unstr.nghost_shapes;

  shapes  = TIOmalloc(nallshapes * sizeof(TIO_Shape_t));
  ncps    = TIOmalloc(nallshapes * sizeof(TIO_Size_t));

  irc = tfile->ReadChunkedDataset( fileID, lpID, meshID,
                                   CLASS_MESH, UNSTR_SHAPES,
                                   xfer, TIO_ENUM_T, TIO_MESH_UNSTRUCT,
                                   tmesh->ndims, TIO_GHOSTS_ALL,
                                   tmesh->nchunks, tmesh->chunkinfo, 
                                   DATAPOS_SHAPE, idx, NULL, shapes );

  irc = tfile->ReadChunkedDataset( fileID, lpID, meshID,
                                   CLASS_MESH, UNSTR_NCPS,
                                   xfer, TIO_SIZE_T, TIO_MESH_UNSTRUCT,
                                   tmesh->ndims, TIO_GHOSTS_ALL,
                                   tmesh->nchunks, tmesh->chunkinfo, 
                                   DATAPOS_SHAPE, idx, NULL, ncps );


  /* Now ignore data with regard to the specified nghosts - use nshapes not nallshapes */


  /* Count how many cells of the specified shape there is */
  nc   = 0;
  sptr = shapes + soffset;
  nptr = ncps   + soffset;
  for (ishape=0; ishape<nshapes; ++ishape) {
    if (*sptr == shape) {
      nc += *nptr;
    }
    ++sptr;
    ++nptr;
  }


  nc = nc * shsize;

  TIOset(nconnectivity, nc);


  if (connectivity != NULL) {

    /* Read all the chunk data including all ghosts ready for 'shape extraction' */

    nc      = tmesh->chunkinfo[idx].unstr.nconnectivity
            + tmesh->chunkinfo[idx].unstr.nghost_connectivity;
    vdata   = TIOmalloc(nc * dtsize);

    /* Have to read using objghosts so that mat/quant with no ghosts in file is ok */
    irc = tfile->ReadChunkedDataset( fileID, lpID, meshID, CLASS_MESH, UNSTR_CONNECT,
                                     xfer, datatype, TIO_MESH_UNSTRUCT,
                                     tmesh->ndims, TIO_GHOSTS_ALL,
                                     tmesh->nchunks, tmesh->chunkinfo, 
                                     DATAPOS_CONNECT, idx, NULL, vdata );

    vptr = vdata + (voffset * dtsize);
    dptr = (char *)connectivity;


    sptr = shapes + soffset;
    nptr = ncps   + soffset;
    for (ishape=0; ishape<nshapes; ++ishape) {
      cpsize = *nptr * dtsize * xTIO_ShapeSize(*sptr);
      if (*sptr == shape) {
        memcpy(dptr, vptr, cpsize);
        dptr += cpsize;
      }
      vptr += cpsize;

      ++sptr;
      ++nptr;
    }

    TIOfree (vdata); vdata = NULL;
  }

  TIOfree (shapes); shapes = NULL;
  TIOfree (ncps);   ncps   = NULL;

  TIOend(subname,1);


  return (TIO_SUCCESS);
}



/***************************************************************************************************
 *
 * TIO_Read_Shaped_Cells
 *
 **************************************************************************************************/
TIO_t
TIO_Read_Shaped_Cells( const TIO_File_t   fileID,
                       const TIO_Object_t objectID,
                       const TIO_Size_t   idx,
                       const TIO_Xfer_t   xfer,
                       const TIO_Shape_t  shape,
                       const TIO_Data_t   datatype,
                       const TIO_Size_t   nghosts,
                       TIO_Size_t         *ncells,
                       void               *data )
{
  const char *subname = "TIO_Read_Shaped_Cells()";

  struct iFile_t     *tfile;
  struct iMesh_t     *tmesh;
  void               *tobject;

  TIO_Object_t parentID, mpID, meshID;
  iClass_t     class;
  int          ndset;

  TIO_Size_t  nshapes, ishape, soffset, nallshapes;
  TIO_Shape_t *shapes,  *sptr;
  TIO_Size_t  *ncps,    *nptr, nc;
  TIO_Size_t  coffset;

  TIO_Size_t  meshghosts, objghosts;
  size_t      dtsize, cpsize;
  char        *cdata, *cptr, *dptr;    /* assuming that char = 1 byte */

  TIO_t trc;
  int  irc;



  TIObegin(subname,1);

  irc = iTIO_GetFile(fileID, &tfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid fileID", TIO_ERR_INVALID_FILEID);
  }
  irc = iTIO_GetClassObject(fileID, objectID, &class, (void **)&tobject);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid objectID", TIO_ERR_INVALID_OBJECTID);
  }

  if (class != CLASS_MESH) {
    irc = iTIO_GetObject(fileID, &meshID, objectID, class, NULL);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to get parent", TIO_ERR_INT);
    }
  }
  else {
    meshID = objectID;
  }


  irc = iTIO_GetObject(fileID, &mpID, meshID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid objectID - is not direct child object of mesh",
              TIO_ERR_INVALID_OBJECTID);
  }

  switch (class) {

  case CLASS_MESH:

    /* Just reading global cellID data */
    parentID  = mpID;
    ndset     = UNSTR_CELLIDS;
    objghosts = TIO_GHOSTS_ALL;
    break;

  case CLASS_MATERIAL:
    {
      struct iMaterial_t *tmat = tobject;

      /* Just the pure material data */
      parentID  = meshID;
      ndset     = MAT_MATERIAL;
      objghosts = tmat->nghosts;
    }
    break;

  case CLASS_QUANT:
    {
      struct iQuant_t *tquant = tobject;

      /* Just the pure quant data */
      parentID  = meshID;
      ndset     = 0;
      objghosts = tquant->nghosts;

      if (tquant->centring != TIO_CENTRE_CELL) {
        TIOreturn(ERR_USER, "Quant must be cell-centred", TIO_ERR_INVALID_OPERATION);
      }
    }
    break;

  default:
    TIOreturn(ERR_USER, "Invalid objectID - must be mesh, material or quant object",
              TIO_ERR_INVALID_OBJECTID);
    break;
  }



  if (tmesh->meshtype != TIO_MESH_UNSTRUCT) {
    TIOreturn(ERR_USER, "Material is not unstruct type - cannot read shapes material",
              TIO_ERR_INVALID_OPERATION);
  }
  if (xTIO_ShapeSize(shape) == 0) {
    TIOreturn(ERR_USER, "Invalid shape", TIO_ERR_INVALID_ARG);
  }
  dtsize = iTIO_SizeOf(datatype);
  if (0 == dtsize) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  trc = cTIO_GetMeshGhosts(subname, fileID, meshID, TIO_MESH_UNSTRUCT, &meshghosts);

  if (TIO_GHOSTS_NONE == nghosts) {
    soffset = 0;
    coffset = 0;
    nshapes = tmesh->chunkinfo[idx].unstr.nshapes;
  }
  else if (TIO_GHOSTS_ALL == nghosts) {
    soffset = 0;
    coffset = 0;
    nshapes = tmesh->chunkinfo[idx].unstr.nshapes + tmesh->chunkinfo[idx].unstr.nghost_shapes;

    if ((TIO_GHOSTS_ALL == meshghosts) && (TIO_GHOSTS_NONE == objghosts)) {
      TIOreturn(ERR_USER, "Invalid nghosts - cannot read more ghosts than stored",
                TIO_ERR_INVALID_ARG);
    }
  }
  else if (TIO_GHOSTS_ONLY == nghosts) {
    soffset = tmesh->chunkinfo[idx].unstr.nshapes;
    coffset = tmesh->chunkinfo[idx].unstr.ncells;
    nshapes = tmesh->chunkinfo[idx].unstr.nghost_shapes;

    if ((TIO_GHOSTS_ALL == meshghosts) && (TIO_GHOSTS_NONE == objghosts)) {
      TIOreturn(ERR_USER, "Invalid nghosts - cannot read more ghosts than stored",
                TIO_ERR_INVALID_ARG);
    }
  }
  else {
    TIOreturn(ERR_USER, "Invalid nghosts - use TIO_GHOSTS_[ALL|NONE|ONLY]", TIO_ERR_INVALID_ARG);
  }


  /* Read shape info for whole chunk including all ghosts */

  nallshapes = tmesh->chunkinfo[idx].unstr.nshapes + tmesh->chunkinfo[idx].unstr.nghost_shapes;

  shapes  = TIOmalloc(nallshapes * sizeof(TIO_Shape_t));
  ncps    = TIOmalloc(nallshapes * sizeof(TIO_Size_t));

  irc = tfile->ReadChunkedDataset( fileID, mpID, meshID,
                                   CLASS_MESH, UNSTR_SHAPES,
                                   xfer, TIO_ENUM_T, TIO_MESH_UNSTRUCT,
                                   tmesh->ndims, TIO_GHOSTS_ALL,
                                   tmesh->nchunks, tmesh->chunkinfo, 
                                   DATAPOS_SHAPE, idx, NULL, shapes );

  irc = tfile->ReadChunkedDataset( fileID, mpID, meshID,
                                   CLASS_MESH, UNSTR_NCPS,
                                   xfer, TIO_SIZE_T, TIO_MESH_UNSTRUCT,
                                   tmesh->ndims, TIO_GHOSTS_ALL,
                                   tmesh->nchunks, tmesh->chunkinfo, 
                                   DATAPOS_SHAPE, idx, NULL, ncps );


  /* Now ignore data with regard to the specified nghosts - use nshapes not nallshapes */


  /* Count how many cells of the specified shape there is */
  nc   = 0;
  sptr = shapes + soffset;
  nptr = ncps   + soffset;
  for (ishape=0; ishape<nshapes; ++ishape) {
    if (*sptr == shape) {
      nc += *nptr;
    }
    ++sptr;
    ++nptr;
  }

  TIOset(ncells, nc);


  if (data != NULL) {

    /* Read all the chunk data including all ghosts ready for 'shape extraction' */

    nc      = tmesh->chunkinfo[idx].unstr.ncells + tmesh->chunkinfo[idx].unstr.nghost_cells;
    cdata   = TIOmalloc(nc * dtsize);

    /* Have to read using objghosts so that mat/quant with no ghosts in file is ok */
    irc = tfile->ReadChunkedDataset( fileID, parentID, objectID, class, ndset,
                                     xfer, datatype, TIO_MESH_UNSTRUCT,
                                     tmesh->ndims, objghosts,
                                     tmesh->nchunks, tmesh->chunkinfo, 
                                     DATAPOS_CELL, idx, NULL, cdata );

    cptr = cdata + (coffset * dtsize);
    dptr = (char *)data;

    sptr = shapes + soffset;
    nptr = ncps   + soffset;
    for (ishape=0; ishape<nshapes; ++ishape) {
      cpsize = *nptr * dtsize;
      if (*sptr == shape) {
        memcpy(dptr, cptr, cpsize);
        dptr += cpsize;
      }
      cptr += cpsize;

      ++sptr;
      ++nptr;
    }

    TIOfree (cdata); cdata = NULL;
  }

  TIOfree (shapes); shapes = NULL;
  TIOfree (ncps);   ncps   = NULL;


  TIOend(subname,1);


  return (TIO_SUCCESS);
}



static size_t
xTIO_ShapeSize(const TIO_Shape_t shape)
{
  switch (shape) {
  case TIO_SHAPE_POINT1:
    return (1);
    break;
  case TIO_SHAPE_BAR2:
    return (2);
    break;
  case TIO_SHAPE_TRI3:
    return (3);
    break;
  case TIO_SHAPE_QUAD4:
    return (4);
    break;
  case TIO_SHAPE_TET4:
    return (4);
    break;
  case TIO_SHAPE_WEDGE6:
    return (6);
    break;
  case TIO_SHAPE_HEX8:
    return (8);
    break;
  case TIO_SHAPE_PYR5:
    return (5);
    break;
  default:
    return (-1);
    break;
  }


  return (0);
}

/*
 * EOF
 */

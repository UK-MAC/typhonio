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
#include "tio_mesh.h"
#include "typhonio_viz.h"



static TIO_t cTIO_OpenMesh     ( const char         subname[],
                                 const int          ByIndex,
                                 const TIO_File_t   fileID,
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

static int xTIO_GetMeshInfo    ( const char         subname[],
                                 const int          getname,
                                 struct iMesh_t     *tmesh,
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


static int xTIO_OpenMeshDatasets(   const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t meshID,
                                    struct iFile_t     *tfile,
                                    struct iMesh_t     *tmesh );

static int xTIO_CloseMeshDatasets(  const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t meshID,
                                    struct iFile_t     *tfile,
                                    struct iMesh_t     *tmesh );


static int xTIO_InitChunkInfo(      const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t meshID,
                                    struct iFile_t     *tfile,
                                    struct iMesh_t     *tmesh,
                                    TIO_Size_t         *nchunks,
                                    const int          reading );

static int xTIO_FinalizeChunkInfo(  const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t meshID,
                                    struct iFile_t     *tfile,
                                    struct iMesh_t     *tmesh );

static void * xTIO_SetCoords( const TIO_Dims_t dim,
                              void *ic,
                              void *jc,
                              void *kc)
{
  int idim = (int)dim;

  if (0 == idim) {
    return (ic);
  }
  else if (1 == idim) {
    return(jc);
  }
  else {
    return(kc);
  }

  return (NULL);
}



static int xTIO_ValidMeshType  ( const TIO_Mesh_t   meshtype  );
static int xTIO_ValidCoordType ( const TIO_Coord_t  coordtype );

#define HaveCoords(i,j,k) (((i)!=NULL) || ((j)!=NULL) || ((k)!=NULL))



static const char *xMeshCoordName[3] = { "TIO_MESH_ICOORD", "TIO_MESH_JCOORD", "TIO_MESH_KCOORD" };
static const char *xMeshNodeIDsName       = "TIO_MESH_NODEIDS";
static const char *xMeshCellIDsName       = "TIO_MESH_CELLIDS";
static const char *xMeshShapesName        = "TIO_MESH_SHAPES";
static const char *xMeshCellsPerShapeName = "TIO_MESH_CELLS_PER_SHAPE";
static const char *xMeshConnectivityName  = "TIO_MESH_CONNECTIVITY";
static const char *xMeshRangeName         = "TIO_MESH_RANGE";
static const char *xMeshChunkInfoName     = "TIO_MESH_CHUNKINFO";



/***************************************************************************************************
 *
 * TIO_List_Meshes
 *
 **************************************************************************************************/
TIO_t
TIO_List_Meshes( const TIO_File_t   fileID,
                 const TIO_Object_t stateID,
                 TIO_Size_t         *nobj,
                 char               names[][TIO_STRLEN] )
{
  const char *subname = "TIO_List_Meshes()";

  TIO_t trc;

  TIObegin(subname,1);

  trc = cTIO_HierarchyListObjects( subname, fileID, stateID, CLASS_STATE, CLASS_MESH,
                                   nobj, names );

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Create_Mesh
 *
 **************************************************************************************************/
TIO_t
TIO_Create_Mesh( const TIO_File_t   fileID,
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
                 const char         klabel[] )
{
  const char *subname = "TIO_Create_Mesh()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lmsID;
  TIO_Size_t   lncks;

  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  *meshID = TIO_NULL;    /* Ensure NULL value on failure */


  /* -- Check object-specific inbound args are valid */

  if (xTIO_ValidMeshType(meshtype) < 0) {
    TIOreturn(ERR_USER, "Invalid arg (meshtype)", TIO_ERR_INVALID_ARG);
  }
  if ((TIO_COORD_CYLINDRICAL_AXISYMMETRIC == coordtype) && (ndims != TIO_2D)) {
    TIOreturn(ERR_USER, "Cannot use axisymmetric coords with 1D/3D mesh", TIO_ERR_INVALID_ARG);
  }
  if (xTIO_ValidCoordType(coordtype) < 0) {
    TIOreturn(ERR_USER, "Invalid arg (coordtype)", TIO_ERR_INVALID_ARG);
  }
  irc = iTIO_ValidString(group, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "group"),  TIO_ERR_INVALID_STRING);
  }
  trc = cTIO_CheckDatatype(subname, "graph_datatype", (meshtype != TIO_MESH_UNSTRUCT),
                           graph_datatype, TIO_TYPECLASS_SIGNED_INT);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }
  if (0 == iTIO_SizeOf(coord_datatype)) {
    TIOreturn(ERR_USER, "Invalid coord_datatype", TIO_ERR_INVALID_DATATYPE);
  }
  if (iTIO_ValidDims(ndims, FALSE) < 0) {
    TIOreturn(ERR_USER, "Invalid arg (ndims)", TIO_ERR_INVALID_DIMS);
  }
  if (nchunks < 1) {
    TIOreturn(ERR_USER, "Invalid nchunks - must be > 0", TIO_ERR_INVALID_ARG);
  }

  irc = iTIO_ValidString(iunits, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "iunits"), TIO_ERR_INVALID_STRING);
  }
  irc = iTIO_ValidString(junits, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "junits"), TIO_ERR_INVALID_STRING);
  }
  irc = iTIO_ValidString(kunits, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "kunits"), TIO_ERR_INVALID_STRING);
  }

  irc = iTIO_ValidString(ilabel, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "ilabel"), TIO_ERR_INVALID_STRING);
  }
  irc = iTIO_ValidString(jlabel, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "jlabel"), TIO_ERR_INVALID_STRING);
  }
  irc = iTIO_ValidString(klabel, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "klabel"), TIO_ERR_INVALID_STRING);
  }

  tmesh = (struct iMesh_t *)TIOmalloc(sizeof(struct iMesh_t));
  if (NULL == tmesh) {
    TIOreturn(ERR_MEM, "Failed to malloc tmesh", TIO_ERR_INT);
  }

  trc = cTIO_HierarchyCreateObject( subname, fileID, stateID, CLASS_STATE, CLASS_MESH, name,
                                    &tfile, &lmsID, tmesh );
  if (trc != TIO_SUCCESS) {
    TIOfree (tmesh); tmesh = NULL;
    cTIOreturn (trc);
  }


  (void) iTIO_strcpy(tmesh->name, name);
  tmesh->status         = STATUS_NEW;

  tmesh->meshtype       = meshtype;
  tmesh->coordtype      = coordtype;
  tmesh->isAMR          = (isAMR != TIO_FALSE);    /* a tad convoluted but stricter */
  (void) iTIO_strcpy(tmesh->group, group);
  tmesh->order          = order;
  tmesh->matghosts      = TIO_GHOSTS_NULL;
  tmesh->graph_datatype = graph_datatype;
  tmesh->coord_datatype = coord_datatype;

  (void) iTIO_strcpy(tmesh->iunits, iunits);
  (void) iTIO_strcpy(tmesh->junits, junits);
  (void) iTIO_strcpy(tmesh->kunits, kunits);
  (void) iTIO_strcpy(tmesh->ilabel, ilabel);
  (void) iTIO_strcpy(tmesh->jlabel, jlabel);
  (void) iTIO_strcpy(tmesh->klabel, klabel);

  tmesh->ndims     = ndims;

  tmesh->todo         = NULL;      /* use NULLness to later detect whether these */
  tmesh->sectionCache = NULL;      /* components are in use or not               */

  tmesh->rangeset  = FALSE;
  tmesh->range     = TIOmalloc(iTIO_SizeOf(coord_datatype) * (size_t)(ndims * 2));
  if (NULL == tmesh->range) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc tmesh->range");
    trc = TIO_ERR_INT;
    goto cleandie;
  }


  /* -- Write object attributes */

  irc = 0;

  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "meshtype",       TIO_ENUM_T,
                           (void *)&tmesh->meshtype);
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "coordtype",      TIO_ENUM_T,
                           (void *)&tmesh->coordtype);

  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "amrflag",        TIO_INT,
                           (void *)&tmesh->isAMR);
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "group",          TIO_STRING,
                           (void *)tmesh->group);
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "order",          TIO_SIZE_T,
                           (void *)&tmesh->order);

  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "graph_datatype", TIO_ENUM_T,
                           (void *)&tmesh->graph_datatype);
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "coord_datatype", TIO_ENUM_T,
                           (void *)&tmesh->coord_datatype);
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "ndims",          TIO_ENUM_T,
                           (void *)&tmesh->ndims);

  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "iunits",         TIO_STRING,
                           (void *)tmesh->iunits );
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "junits",         TIO_STRING,
                           (void *)tmesh->junits );
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "kunits",         TIO_STRING,
                           (void *)tmesh->kunits );
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "ilabel",         TIO_STRING,
                           (void *)tmesh->ilabel );
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "jlabel",         TIO_STRING,
                           (void *)tmesh->jlabel );
  irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "klabel",         TIO_STRING,
                           (void *)tmesh->klabel );

  TIOassert(irc != 0, ERR_INT, "Failed to write attributes", TIO_ERR_INT);


  lncks = nchunks;

  irc = xTIO_InitChunkInfo(fileID, stateID, lmsID, tfile, tmesh, &lncks, FALSE);
  TIOassert(irc < 0, ERR_INT, "Failed to init chunk info", TIO_ERR_INT);


  /* -- Mesh datasets also created on final chunk set
        - full chunk sizes are only then known (unstructured, num ghosts on chunk) */

  irc = 0;

  switch (tmesh->meshtype) {

  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    tmesh->info.quad.nj = 0;
    tmesh->info.quad.nk = 0;
    switch (tmesh->ndims) {
    case TIO_3D:
      tmesh->info.quad.nk = n3;
    case TIO_2D:
      tmesh->info.quad.nj = n2;
    case TIO_1D:
      tmesh->info.quad.ni = n1;
      break;
    default:
      TIOreturn(ERR_INT, "Invalid ndims", TIO_ERR_INVALID_ARG);
      break;
    }

    if (TIO_MESH_QUAD_NONCOLINEAR == tmesh->meshtype) {
      tmesh->info.quad.nghosts = n4;
    }
    else {
      tmesh->info.quad.nghosts = 0;
    }

    irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "ni", TIO_SIZE_T,
                             (void *)&tmesh->info.quad.ni );
    irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "nj", TIO_SIZE_T,
                             (void *)&tmesh->info.quad.nj );
    irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "nk", TIO_SIZE_T,
                             (void *)&tmesh->info.quad.nk );

    irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "nghosts", TIO_SIZE_T,
                             (void *)&tmesh->info.quad.nghosts );

    tmesh->info.quad.nmixcell = 0;
    tmesh->info.quad.nmixcomp = 0;

    break;

  case TIO_MESH_UNSTRUCT:

    if (n1 < 1) {
      TIOreturn(ERR_USER, "Invalid value for n1 (=nnodes)", TIO_ERR_INVALID_ARG);
    }
    if (n3 < 1) {
      TIOreturn(ERR_USER, "Invalid value for n3 (=nshapes)", TIO_ERR_INVALID_ARG);
    }

    /* ncells and nconnectivity could be zero for point elements */

    tmesh->info.unstr.nnodes        = n1;
    tmesh->info.unstr.ncells        = n2;
    tmesh->info.unstr.nshapes       = n3;
    tmesh->info.unstr.nconnectivity = n4;

    irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "nnodes", TIO_SIZE_T,
                             (void *)&tmesh->info.unstr.nnodes );
    irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "ncells", TIO_SIZE_T,
                             (void *)&tmesh->info.unstr.ncells );
    irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "nshapes", TIO_SIZE_T,
                             (void *)&tmesh->info.unstr.nshapes );
    irc += tfile->WriteAttr( fileID, stateID, lmsID, CLASS_MESH, "nconnectivity", TIO_SIZE_T,
                             (void *)&tmesh->info.unstr.nconnectivity );

    tmesh->info.unstr.nmixcell = 0;
    tmesh->info.unstr.nmixcomp = 0;

    break;

  case TIO_MESH_POINT:

    tmesh->info.point.nnodes = n1;

    irc += tfile->WriteAttr( fileID, TIO_NULL, lmsID, CLASS_MESH, "nnodes", TIO_SIZE_T,
                             (void *)&tmesh->info.point.nnodes );

    break;

  default:
    TIOreturn(ERR_USER, "Invalid meshtype", TIO_ERR_INVALID_ARG);
    break;

  }

  TIOassert(irc != 0, ERR_INT, "Failed to write attributes", TIO_ERR_INT);


  *meshID = lmsID;

  TIOend(subname,1);

  return (TIO_SUCCESS);


 cleandie:
  trc = cTIO_HierarchyKillObject(subname, fileID, stateID, lmsID, CLASS_MESH);
  TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to kill object", TIO_ERR_INT);

  if (tmesh->range != NULL) {
    TIOfree (tmesh->range); tmesh->range = NULL;
  }
  TIOfree (tmesh); tmesh = NULL;
  TIOreturn(iTIO_GetErrClass(), iTIO_GetErrStr(), trc);
}




/***************************************************************************************************
 *
 * TIO_Open_Mesh
 * TIO_Open_Mesh_idx
 *
 **************************************************************************************************/
TIO_t
TIO_Open_Mesh( const TIO_File_t fileID,
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
               char               klabel[] )
{
  return ( cTIO_OpenMesh("TIO_Open_Mesh()", FALSE,
                         fileID, stateID, TIO_NULL, (char *)name, meshID,
                         meshtype, coordtype,
                         isAMR, group, order,
                         graph_datatype, coord_datatype,
                         ndims, n1, n2, n3, n4, nchunks,
                         iunits, junits, kunits,
                         ilabel, jlabel, klabel ) );
}


TIO_t
TIO_Open_Mesh_idx( const TIO_File_t   fileID,
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
                   char               klabel[] )
{
  return ( cTIO_OpenMesh("TIO_Open_Mesh_idx()", TRUE,
                         fileID, stateID, idx, name, meshID,
                         meshtype, coordtype,
                         isAMR, group, order,
                         graph_datatype, coord_datatype,
                         ndims, n1, n2, n3, n4, nchunks,
                         iunits, junits, kunits,
                         ilabel, jlabel, klabel ) );
}


static TIO_t
cTIO_OpenMesh( const char         subname[],
               const int          ByIndex,
               const TIO_File_t   fileID,
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
               char               klabel[] )
{
  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lmsID;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  *meshID = TIO_NULL;    /* Ensure NULL value on failure */


  tmesh = (struct iMesh_t *)TIOmalloc(sizeof(struct iMesh_t));
  if (NULL == tmesh) {
    TIOreturn(ERR_MEM, "Failed to malloc tmesh", TIO_ERR_INT);
  }

  trc = cTIO_HierarchyOpenObject( subname, fileID, stateID, CLASS_STATE, CLASS_MESH,
                                  name, ByIndex, idx,
                                  tmesh->name, &tfile, &lmsID, (void *)tmesh );
  if (trc != TIO_SUCCESS) {
    TIOfree (tmesh); tmesh = NULL;
    cTIOreturn (trc);
  }


  /* -- Read object attributes */
  irc = 0;

  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "meshtype",       TIO_ENUM_T,
                           (void *)&tmesh->meshtype );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "coordtype",      TIO_ENUM_T,
                           (void *)&tmesh->coordtype );

  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "amrflag",        TIO_INT,
                           (void *)&tmesh->isAMR );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "group",          TIO_STRING,
                           (void *)tmesh->group );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "order",          TIO_SIZE_T,
                           (void *)&tmesh->order );

  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "graph_datatype", TIO_ENUM_T,
                           (void *)&tmesh->graph_datatype );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "coord_datatype", TIO_ENUM_T,
                           (void *)&tmesh->coord_datatype );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "ndims",          TIO_ENUM_T,
                           (void *)&tmesh->ndims );

  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "iunits",         TIO_STRING,
                           (void *)tmesh->iunits );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "junits",         TIO_STRING,
                           (void *)tmesh->junits );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "kunits",         TIO_STRING,
                           (void *)tmesh->kunits );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "ilabel",         TIO_STRING,
                           (void *)tmesh->ilabel );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "jlabel",         TIO_STRING,
                           (void *)tmesh->jlabel );
  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "klabel",         TIO_STRING,
                           (void *)tmesh->klabel );

  irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "matghosts",      TIO_SIZE_T,
                           (void *)&tmesh->matghosts );

  TIOassert(irc != 0, ERR_INT, "Failed to read attributes", TIO_ERR_INT);


  irc = 0;

  switch (tmesh->meshtype) {

  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "ni", TIO_SIZE_T,
                            (void *)&tmesh->info.quad.ni );
    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nj", TIO_SIZE_T,
                            (void *)&tmesh->info.quad.nj );
    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nk", TIO_SIZE_T,
                            (void *)&tmesh->info.quad.nk );

    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nghosts", TIO_SIZE_T,
                            (void *)&tmesh->info.quad.nghosts );

    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nmixcell", TIO_SIZE_T,
                            (void *)&tmesh->info.quad.nmixcell );
    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nmixcomp", TIO_SIZE_T,
                            (void *)&tmesh->info.quad.nmixcomp );
    break;

  case TIO_MESH_UNSTRUCT:

    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nnodes", TIO_SIZE_T,
                            (void *)&tmesh->info.unstr.nnodes );
    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "ncells", TIO_SIZE_T,
                            (void *)&tmesh->info.unstr.ncells );
    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nshapes", TIO_SIZE_T,
                            (void *)&tmesh->info.unstr.nshapes );
    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nconnectivity", TIO_SIZE_T,
                            (void *)&tmesh->info.unstr.nconnectivity );

    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nmixcell", TIO_SIZE_T,
                            (void *)&tmesh->info.unstr.nmixcell );
    irc += tfile->ReadAttr( fileID, stateID, lmsID, CLASS_MESH, "nmixcomp", TIO_SIZE_T,
                            (void *)&tmesh->info.unstr.nmixcomp );
    break;

  case TIO_MESH_POINT:

    irc += tfile->ReadAttr( fileID, TIO_NULL, lmsID, CLASS_MESH, "nnodes", TIO_SIZE_T,
                            (void *)&tmesh->info.point.nnodes );

    break;

  default:
    TIOreturn(ERR_USER, "Invalid meshtype", TIO_ERR_INVALID_ARG);
    break;

  }

  TIOassert(irc != 0, ERR_INT, "Failed to read meshsize attributes", TIO_ERR_INT);


  tmesh->sectionCache = NULL;      /* use NULLness to later detect whether in use or not */


  /* Read mesh range dataset */

  tmesh->range = TIOmalloc(iTIO_SizeOf(tmesh->coord_datatype) * (size_t)(tmesh->ndims * 2));
  if (NULL == tmesh->range) {
    TIOreturn(ERR_MEM, "Failed to malloc tmesh->range", TIO_ERR_INT);
  }

  irc = tfile->OpenDataset( fileID, stateID, lmsID, CLASS_MATERIAL, xMeshRangeName, 0,
                            NULL, NULL, NULL );
  switch (irc) {

  case ITIO_ERR_OPEN:

    /* Range dataset does not exist in the file - was never set */
    tmesh->rangeset = FALSE;

    break;

  default:

    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to open range dataset", TIO_ERR_INT);
    }

    /* All ok if got here - read range dataset */
    irc = tfile->ReadDataset(fileID, stateID, lmsID, CLASS_MESH,
                             0, TIO_XFER_COLLECTIVE, tmesh->coord_datatype, tmesh->range);
    TIOassert(irc < 0, ERR_INT, "Failed to read range dataset", TIO_ERR_INT);

    irc = tfile->CloseDataset(fileID, stateID, lmsID, CLASS_MESH, 0);
    TIOassert(irc < 0, ERR_INT, "Failed to close range dataset", TIO_ERR_INT);

    tmesh->rangeset = TRUE;

    break;
  }


  tmesh->status = STATUS_OLD;      /* Disallows writing of mesh datasets */


  irc = xTIO_InitChunkInfo( fileID, stateID, lmsID, tfile, tmesh, &tmesh->nchunks, TRUE );
  TIOassert(irc < 0, ERR_INT, "Failed to init chunk info", TIO_ERR_INT);


  irc = xTIO_OpenMeshDatasets( fileID, stateID, lmsID, tfile, tmesh );
  TIOassert(irc < 0, ERR_INT, "Failed to open mesh datasets", TIO_ERR_INT);


  irc = xTIO_GetMeshInfo( subname, ByIndex, tmesh, name, meshtype, coordtype, isAMR, group, order,
                          graph_datatype, coord_datatype, ndims, n1, n2, n3, n4, nchunks,
                          iunits, junits, kunits, ilabel, jlabel, klabel );

  TIOassert(irc < 0, ERR_INT, "Failed to get mesh info", TIO_ERR_INT);

  *meshID = lmsID;

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_Mesh_Info
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Mesh_Info( const TIO_File_t   fileID,
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
                   char               klabel[] )
{
  const char *subname = "TIO_Get_Mesh_Info()";

  struct iMesh_t *tmesh;

  int   irc;
  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, meshID, CLASS_MESH,
                                     NULL, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = xTIO_GetMeshInfo( subname, TRUE, tmesh, name, meshtype, coordtype, isAMR, group, order,
                          graph_datatype, coord_datatype, ndims, n1, n2, n3, n4, nchunks,
                          iunits, junits, kunits, ilabel, jlabel, klabel );
  TIOassert(irc < 0, ERR_INT, "Failed to get mesh info", TIO_ERR_INT);


  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Close_Mesh
 *
 **************************************************************************************************/
TIO_t
TIO_Close_Mesh( const TIO_File_t   fileID,
                const TIO_Object_t meshID )
{
  const char *subname = "TIO_Close_Mesh()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;         /* parentID   */
  TIO_Size_t   nmx, nmc;     /* mix counts */
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyPreClose( subname, fileID, &lpID, meshID, CLASS_MESH,
                                &tfile, (void **)&tmesh );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (tmesh->status != STATUS_OLD) {

    /* tmesh->status will == NEW, CREATED, WRITTEN or MODIFIED */

    /* Write material ghosts attribute - this is needed for when opening the mesh to determine
       if a material exists and how many ghosts it has without the need for expensively opening the
       actual material */

    irc = tfile->WriteAttr( fileID, lpID, meshID, CLASS_MESH, "matghosts", TIO_SIZE_T,
                            (void *)&tmesh->matghosts);
    TIOassert(irc != 0, ERR_INT, "Failed to write matghosts attribute", TIO_ERR_INT);


    switch (tmesh->meshtype) {
    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR:
      nmx = tmesh->info.quad.nmixcell;
      nmc = tmesh->info.quad.nmixcomp;
      break;
    case TIO_MESH_UNSTRUCT:
      nmx = tmesh->info.unstr.nmixcell;
      nmc = tmesh->info.unstr.nmixcomp;
      break;
    default:
      nmx = nmc = 0;
      break;
    }

    /* Write total numbers of mixcells and mixcomps over all chunks (includes ghost duplicates) */

    irc = 0;

    irc += tfile->WriteAttr( fileID, lpID, meshID, CLASS_MESH, "nmixcell", TIO_SIZE_T,
                             (void *)&nmx);

    irc += tfile->WriteAttr( fileID, lpID, meshID, CLASS_MESH, "nmixcomp", TIO_SIZE_T,
                             (void *)&nmc );

    TIOassert(irc < 0, ERR_INT, "Failed to write material mix attributes", TIO_ERR_INT);


    /* There is a chance that material / quants were created in the mesh and all chunks weren't set,
       in which case the todo list will still be allocated.  Check if so and free. */

    if (tmesh->todo != NULL) {
      TIOfree (tmesh->todo->objectID);  tmesh->todo->objectID = NULL;
      TIOfree (tmesh->todo->class);     tmesh->todo->class    = NULL;
      TIOfree (tmesh->todo);            tmesh->todo           = NULL;
    }

  }


  /* Close mesh datasets */
  /* Possible that none were written in newly created mesh (you never know!),
     so no need to close if so */
  if (tmesh->status != STATUS_NEW) {
    irc = xTIO_CloseMeshDatasets(fileID, lpID, meshID, tfile, tmesh);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to close mesh datasets", TIO_ERR_OBJECT_CLOSE);
    }
  }


  /* Write the mesh range if it was set - is safe to re-use ndsets */

  if ((tmesh->status != STATUS_OLD) && tmesh->rangeset) {

    TIO_Size_t dsize[1];

    dsize[0] = (TIO_Size_t)(tmesh->ndims * 2);

    irc = tfile->CreateDataset(fileID, lpID, meshID, CLASS_MESH,
                               xMeshRangeName, 0, TIO_1D, dsize, tmesh->coord_datatype);

    if (0 == tfile->rank) {
      irc = tfile->WriteDataset(fileID, lpID, meshID, CLASS_MESH,
                                0, TIO_XFER_INDEPENDENT, tmesh->coord_datatype, tmesh->range);
    }

    irc = tfile->CloseDataset(fileID, lpID, meshID, CLASS_MESH, 0);

  }

  TIOfree (tmesh->range); tmesh->range = NULL;



  /* Free up any section in the mesh */

  if (tmesh->sectionCache != NULL) {

    TIO_Object_t     sectID;
    size_t           i;
    union iSection_t *tsection;

    for (i=0; i<tmesh->sectionCache->nlist; ++i) {

      sectID = tmesh->sectionCache->sectionID[i];

      irc = iTIO_GetObject(fileID, NULL, sectID, CLASS_SECTION, (void **)&tsection);
      TIOassert(irc < 0, ERR_INT, "Failed to get section object", TIO_ERR_INT);

      irc = tfile->KillSection(fileID, meshID, sectID);
      TIOassert(irc < 0, ERR_INT, "Failed to kill section", TIO_ERR_INT);

      irc = iTIO_DeregisterObject(fileID, sectID);
      TIOassert(irc < 0, ERR_INT, "Failed to deregister section object", TIO_ERR_INT);

      switch (tmesh->meshtype) {
      case TIO_MESH_UNSTRUCT:
        TIOfree(tsection->unstr.IDs); tsection->unstr.IDs = NULL;
        break;
      case TIO_MESH_POINT:
        TIOfree(tsection->point.IDs); tsection->point.IDs = NULL;
        break;
      default:
        break;
      }

      TIOfree(tsection); tsection = NULL;
    }

    TIOfree (tmesh->sectionCache->sectionID); tmesh->sectionCache->sectionID = NULL;
    TIOfree (tmesh->sectionCache);            tmesh->sectionCache            = NULL;
  }


  /* Write chunk info to a dataset and close up */
  irc = xTIO_FinalizeChunkInfo(fileID, lpID, meshID, tfile, tmesh);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to finalize chunk info", TIO_ERR_INT);
  }

  /* -- Free the object cache */
  TIOfree (tmesh); tmesh = NULL;

  trc = cTIO_HierarchyCloseObject( subname, fileID, lpID, meshID, CLASS_MESH );

  TIOend(subname,1);


  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Set_Mesh_Range
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Mesh_Range( const TIO_File_t   fileID,
                    const TIO_Object_t meshID,
                    const TIO_Data_t   datatype,
                    const TIO_Dims_t   ndims,
                    const void         *xl,
                    const void         *xh,
                    const void         *yl,
                    const void         *yh,
                    const void         *zl,
                    const void         *zh )
{
  const char *subname = "TIO_Set_Mesh_Range()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  size_t dsi, dsid, dso, ndx2;
  char   *lbuf;

  int   irc;
  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  /* -- Check args */
  dsi = iTIO_SizeOf(datatype);
  if (0 == dsi) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }
  if (ndims != tmesh->ndims) {
    TIOreturn(ERR_USER, "Invalid argument - ndims does not match that of mesh", TIO_ERR_INVALID_ARG);
  }
  if (tmesh->rangeset) {
    TIOreturn(ERR_USER, "Mesh range has already been set", TIO_ERR_INVALID_OPERATION);
  }

  /* Next bit depends on sizeof(char) == 1  - should always be the case, but best to check! */
  TIOassert((sizeof(char) != 1), ERR_INT, "FATAL ERROR: sizeof(char) != 1", TIO_ERR_INT);

  ndx2 = (size_t)(ndims * 2);
  dsid = dsi * ndx2;

  lbuf = (char *)TIOmalloc(dsid);
  if (NULL == lbuf) {
    TIOreturn(ERR_MEM, "Failed to malloc lbuf", TIO_ERR_INT);
  }

  dso = 0;
  switch (ndims) {
  case TIO_3D:
    if (NULL == xl) TIOreturn(ERR_USER, "il must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == xh) TIOreturn(ERR_USER, "ih must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == yl) TIOreturn(ERR_USER, "jl must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == yh) TIOreturn(ERR_USER, "jh must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == zl) TIOreturn(ERR_USER, "kl must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == zh) TIOreturn(ERR_USER, "kh must be specified in range", TIO_ERR_INVALID_ARG);
    (void) memcpy(&lbuf[dso], xl, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], xh, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], yl, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], yh, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], zl, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], zh, dsi);
    break;
  case TIO_2D:
    if (NULL == xl) TIOreturn(ERR_USER, "il must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == xh) TIOreturn(ERR_USER, "ih must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == yl) TIOreturn(ERR_USER, "jl must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == yh) TIOreturn(ERR_USER, "jh must be specified in range", TIO_ERR_INVALID_ARG);
    (void) memcpy(&lbuf[dso], xl, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], xh, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], yl, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], yh, dsi);
    break;
  case TIO_1D:
    if (NULL == xl) TIOreturn(ERR_USER, "il must be specified in range", TIO_ERR_INVALID_ARG);
    if (NULL == xh) TIOreturn(ERR_USER, "ih must be specified in range", TIO_ERR_INVALID_ARG);
    (void) memcpy(&lbuf[dso], xl, dsi); dso += dsi;
    (void) memcpy(&lbuf[dso], xh, dsi);
    break;
  default:
    break;
  }


  /* Now have supplied range in local continuous buffer - now copy it to mesh cache */

  if (datatype != tmesh->coord_datatype) {

    size_t dsf, dsfd;
    void   *cbuf;

    dsf  = iTIO_SizeOf(tmesh->coord_datatype);
    dsfd = dsf * ndx2;
    cbuf = TIOmalloc( TIOmax(dsid, dsfd));

    (void) memcpy(cbuf, lbuf, dsid);

    irc = tfile->ConvBuf(datatype, tmesh->coord_datatype, (TIO_Size_t)ndx2, cbuf);
    if (irc < 0) {
      TIOreturn(ERR_USER, "Unable to convert between given datatype and that of material",
                TIO_ERR_INVALID_CONVERSION);
    }
    (void) memcpy(tmesh->range, cbuf, dsfd);

    TIOfree (cbuf); cbuf = NULL;

  }
  else {
    (void) memcpy(tmesh->range, lbuf, dsid);
  }

  tmesh->rangeset = TRUE;

  TIOfree(lbuf); lbuf = NULL;


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_Mesh_Range
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Mesh_Range( const TIO_File_t   fileID,
                    const TIO_Object_t meshID,
                    const TIO_Data_t   datatype,
                    TIO_Dims_t        *ndims,
                    void               *xl,
                    void               *xh,
                    void               *yl,
                    void               *yh,
                    void               *zl,
                    void               *zh )
{
  const char *subname = "TIO_Get_Mesh_Range()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  size_t          dsi, dsl, dsh, dss;
  int             dim;
  TIO_TypeClass_t typeclass;
  TIO_Data_t      ldatatype;

  int   ilbuf[6] = {999,666,999,666,999,666};
  float flbuf[6] = {999.0,666.0,999.0,666.0,999.0,666.0};

  void   *lbuf;
  char   *cbuf;

  int    irc;
  TIO_t  trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  dsi = iTIO_SizeOf(datatype);
  if (0 == dsi) {
    TIOreturn(ERR_USER, "Invalid argument (datatype)", TIO_ERR_INVALID_DATATYPE);
  }

  if (!tmesh->rangeset) {

    /* use dummy range values */

    trc = TIO_Get_TypeClass(datatype, &typeclass);
    switch (typeclass) {
    case TIO_TYPECLASS_SIGNED_INT:
    case TIO_TYPECLASS_UNSIGNED_INT:
      ldatatype = TIO_INT;
      lbuf      = ilbuf;
      break;
    case TIO_TYPECLASS_FLOAT:
      lbuf      = flbuf;
      ldatatype = TIO_FLOAT;
      break;
    default:
      break;
    }

  }
  else {

    /* Use buffer in mesh object cache */

    lbuf      = tmesh->range;
    ldatatype = tmesh->coord_datatype;

  }


  /* convert datatype if necessary */

  if (datatype != ldatatype) {

    size_t ndx2 = tmesh->ndims * 2;

    dsl  = TIO_SizeOf(ldatatype);
    dss  = TIOmax(dsi, dsl);
    cbuf = NULL;
    cbuf = TIOmalloc(ndx2 * dss);
    TIOassert(NULL == cbuf, ERR_MEM, "Failed to malloc cbuf", TIO_ERR_INT);

    (void) memcpy(cbuf, lbuf, ndx2 * dsl);
    irc = tfile->ConvBuf(ldatatype, datatype, (TIO_Size_t)ndx2, cbuf);
    if (irc < 0) {
      TIOfree (cbuf); cbuf = NULL;
      TIOreturn(ERR_USER, "Unable to convert between given datatype and mesh range",
                TIO_ERR_INVALID_CONVERSION);
    }

  }
  else {
    cbuf = lbuf;
  }

  dss = dsi * 2;

  dsl = 0;
  dsh = dsi;

  switch (tmesh->ndims) {
  case TIO_3D:
    if (xl != NULL) (void) memcpy(xl, &cbuf[dsl], dsi); dsl += dss;
    if (xh != NULL) (void) memcpy(xh, &cbuf[dsh], dsi); dsh += dss;
    if (yl != NULL) (void) memcpy(yl, &cbuf[dsl], dsi); dsl += dss;
    if (yh != NULL) (void) memcpy(yh, &cbuf[dsh], dsi); dsh += dss;
    if (zl != NULL) (void) memcpy(zl, &cbuf[dsl], dsi);
    if (zh != NULL) (void) memcpy(zh, &cbuf[dsh], dsi);
    break;
  case TIO_2D:
    if (xl != NULL) (void) memcpy(xl, &cbuf[dsl], dsi); dsl += dss;
    if (xh != NULL) (void) memcpy(xh, &cbuf[dsh], dsi); dsh += dss;
    if (yl != NULL) (void) memcpy(yl, &cbuf[dsl], dsi);
    if (yh != NULL) (void) memcpy(yh, &cbuf[dsh], dsi);
    if (zl != NULL) (void) memset(zl, 0, dsi);
    if (zh != NULL) (void) memset(zh, 0, dsi);
    break;
  case TIO_1D:
    if (xl != NULL) (void) memcpy(xl, &cbuf[dsl], dsi);
    if (xh != NULL) (void) memcpy(xh, &cbuf[dsh], dsi);
    if (yl != NULL) (void) memset(yl, 0, dsi);
    if (yh != NULL) (void) memset(yh, 0, dsi);
    if (zl != NULL) (void) memset(zl, 0, dsi);
    if (zh != NULL) (void) memset(zh, 0, dsi);
    break;
  default:
    break;
  }


  /* Check that range found is valid or not */


  if (tmesh->rangeset) {

    dsl = 0;
    dsh = dsi;

    for (dim=0; dim<tmesh->ndims; ++dim) {

      trc = cTIO_CheckRange(subname, tfile, datatype, &cbuf[dsl], &cbuf[dsh]);

      if (TIO_ERR_INVALID_RANGE == trc) {
        break;
      }
      TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to check range", TIO_ERR_INT);

      dsl += dss;
      dsh += dss;
    }

  }
  else {

    /* Range wasn't set, so it's the dummy invalid one we've made up */

    trc = TIO_ERR_INVALID_RANGE;

  }

  if (cbuf != lbuf) {
    TIOfree (cbuf); cbuf = NULL;
  }

  TIOset(ndims, tmesh->ndims);

  TIOend(subname,1);

  return (trc);
}




/**************************************
 **************************************
 **
 **   QUAD MESHES
 **
 **************************************
 **************************************/


/***************************************************************************************************
 *
 * TIO_Write_QuadMesh_Chunk
 *
 * - can only write noncolinear quadmesh as separate chunks
 *   colinear quadmesh is written using TIO_Write_QuadMesh_All
 *
 **************************************************************************************************/
TIO_t
TIO_Write_QuadMesh_Chunk( const TIO_File_t   fileID,
                          const TIO_Object_t meshID,
                          const TIO_Size_t   idx,
                          const TIO_Xfer_t   xfer,
                          const TIO_Data_t   datatype,
                          const void         *icoords,
                          const void         *jcoords,
                          const void         *kcoords )
{
  const char *subname = "TIO_Write_QuadMesh_Chunk()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (tmesh->meshtype != TIO_MESH_QUAD_NONCOLINEAR) {
    TIOreturn(ERR_USER, "Mesh is not noncolinear quadmesh", TIO_ERR_INVALID_OPERATION);
  }
  if (tmesh->nchunks_unset > 0) {
    TIOreturn(ERR_USER, "Not all mesh chunks have been set", TIO_ERR_OBJECT_WRITE);
  }
  if ((tmesh->status != STATUS_WRITTEN) && (tmesh->status != STATUS_CREATED)) {
    TIOreturn(ERR_USER, "Cannot write to pre-existing mesh", TIO_ERR_OBJECT_NOT_WRITEABLE);
  }
  if (iTIO_SizeOf(datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    const void *coordptr = xTIO_SetCoords(dim, (void *)icoords, (void *)jcoords, (void *)kcoords);

    irc = tfile->WriteChunkedDataset( fileID, lpID, meshID,
                                      CLASS_MESH, (int)dim,
                                      xfer, datatype, tmesh->meshtype,
                                      tmesh->ndims, tmesh->info.quad.nghosts,
                                      tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                      DATAPOS_NODE, idx, coordptr );
    if (0 == irc) {
      tmesh->status = STATUS_WRITTEN;
    }
    else if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write mesh datasets", TIO_ERR_OBJECT_WRITE);
    }
    else {
      /* nothing to worry about */
    }
  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Write_QuadMesh_All
 *
 **************************************************************************************************/
TIO_t
TIO_Write_QuadMesh_All( const TIO_File_t   fileID,
                        const TIO_Object_t meshID,
                        const TIO_Data_t   datatype,
                        const void         *icoords,
                        const void         *jcoords,
                        const void         *kcoords )
{
  const char *subname = "TIO_Write_QuadMesh_All()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (tmesh->meshtype != TIO_MESH_QUAD_COLINEAR) {
    TIOreturn(ERR_USER, "Mesh is not colinear quadmesh", TIO_ERR_INVALID_OPERATION);
  }
  if (tmesh->nchunks_unset > 0) {
    TIOreturn(ERR_USER, "Not all mesh chunks have been set", TIO_ERR_OBJECT_WRITE);
  }
  if ((tmesh->status != STATUS_WRITTEN) && (tmesh->status != STATUS_CREATED)) {
    TIOreturn(ERR_USER, "Cannot write to pre-existing mesh", TIO_ERR_OBJECT_NOT_WRITEABLE);
  }
  if (iTIO_SizeOf(datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    const void *coordptr = xTIO_SetCoords(dim, (void *)icoords, (void *)jcoords, (void *)kcoords);

    irc = tfile->WriteChunkedDataset( fileID, lpID, meshID,
                                      CLASS_MESH, (int)dim,
                                      TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                      tmesh->ndims, 0, tmesh->nchunks, TIO_TRUE, &tmesh->info,
                                      DATAPOS_NODE, tmesh->nchunks, coordptr );
    if (0 == irc) {
      tmesh->status = STATUS_WRITTEN;
    }
    else if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write mesh dataset (coord)", TIO_ERR_OBJECT_WRITE);
    }
    else {
      /* nothing to worry about */
    }
  }


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_QuadMesh_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadMesh_Chunk( const TIO_File_t   fileID,
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
                         void               *kcoords )
{
  const char *subname = "TIO_Read_QuadMesh_Chunk()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Size_t   lnghosts;
  TIO_Size_t   nn[3], *nnptr[3];
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  lnghosts = nghosts;
  nn[0]    = 0;
  nn[1]    = 0;
  nn[2]    = 0;


  switch (tmesh->meshtype) {

  case TIO_MESH_QUAD_COLINEAR:

    if (TIO_GHOSTS_ALL == lnghosts) {
      TIOreturn(ERR_USER, "TIO_GHOSTS_ALL not valid for colinear quadmesh", TIO_ERR_INVALID_ARG);
    }

    nnptr[0] = &nn[0];
    nnptr[1] = &nn[1];
    nnptr[2] = &nn[2];

    break;

  case TIO_MESH_QUAD_NONCOLINEAR:

    if (TIO_GHOSTS_INTERNAL == lnghosts) {
      /* ok */
      ;
    }
    else if (TIO_GHOSTS_ALL == lnghosts) {
      lnghosts = tmesh->info.quad.nghosts;
    }
    else if (lnghosts > tmesh->info.quad.nghosts) {
      TIOreturn(ERR_USER, "Cannot read more ghosts than stored", TIO_ERR_INVALID_ARG);
    }
    else {
      /* ok */
      ;
    }

    nnptr[0] = &nn[0];
    nnptr[1] = NULL;
    nnptr[2] = NULL;

    break;

  default:
    TIOreturn(ERR_USER, "Mesh is not quadmesh type", TIO_ERR_INVALID_OPERATION);
    break;
  }

  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if ((iTIO_SizeOf(datatype) == 0) && HaveCoords(icoords, jcoords, kcoords)) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDataset( fileID, lpID, meshID,
                                     CLASS_MESH, (int)dim,
                                     xfer, datatype, tmesh->meshtype,
                                     tmesh->ndims, lnghosts, tmesh->nchunks, tmesh->chunkinfo, 
                                     DATAPOS_NODE, idx, nnptr[dim], coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset (coord)", TIO_ERR_OBJECT_READ);
    }
  }


  TIOset(ndims, tmesh->ndims);
  TIOset(ni,    nn[0]);
  TIOset(nj,    nn[1]);
  TIOset(nk,    nn[2]);


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_QuadMesh_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadMesh_All( const TIO_File_t   fileID,
                       const TIO_Object_t meshID,
                       const TIO_Data_t   datatype,
                       TIO_Dims_t         *ndims,
                       TIO_Size_t         *ni,
                       TIO_Size_t         *nj,
                       TIO_Size_t         *nk,
                       void               *icoords,
                       void               *jcoords,
                       void               *kcoords )
{
  const char *subname = "TIO_Read_QuadMesh_All()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (   (tmesh->meshtype != TIO_MESH_QUAD_COLINEAR)
      && (tmesh->meshtype != TIO_MESH_QUAD_NONCOLINEAR)) {
    TIOreturn(ERR_USER, "Mesh is not colinear or noncolinear quadmesh", TIO_ERR_INVALID_OPERATION);
  }
  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if ((iTIO_SizeOf(datatype) == 0) && HaveCoords(icoords, jcoords, kcoords)) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, meshID,
                                        CLASS_MESH, (int)dim,
                                        TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                        tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info, 
                                        DATAPOS_NODE, NULL, coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset (coord)", TIO_ERR_OBJECT_READ);
    }
  }


  TIOset(ndims, tmesh->ndims);
  TIOset(ni,    tmesh->info.quad.ni);
  TIOset(nj,    tmesh->info.quad.nj);
  TIOset(nk,    tmesh->info.quad.nk);


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_QuadMesh_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadMesh_Section( const TIO_File_t   fileID,
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
                           void               *kcoords )
{
  const char *subname = "TIO_Read_QuadMesh_Section()";


  struct iFile_t    *tfile;
  struct iMesh_t    *tmesh;
  union  iSection_t *tsection;

  TIO_Object_t lpID;
  TIO_Size_t   nn[3], *nnptr[3];
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = iTIO_GetObject(fileID, NULL, sectionID, CLASS_SECTION, (void **)&tsection);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid sectionID", TIO_ERR_INVALID_SECTIONID);
  }
  if (!iTIO_CheckSection(tmesh, sectionID)) {
    TIOreturn(ERR_USER, "Invalid sectionID - not defined for this mesh", TIO_ERR_INVALID_SECTIONID);
  }

  nn[0] = 0;
  nn[1] = 0;
  nn[2] = 0;


  switch (tmesh->meshtype) {
  case TIO_MESH_QUAD_COLINEAR:
    nnptr[0] = &nn[0];
    nnptr[1] = &nn[1];
    nnptr[2] = &nn[2];
    break;
  case TIO_MESH_QUAD_NONCOLINEAR:
    nnptr[0] = &nn[0];
    nnptr[1] = NULL;
    nnptr[2] = NULL;
    break;
  default:
    TIOreturn(ERR_USER, "Mesh is not quadmesh type", TIO_ERR_INVALID_OPERATION);
    break;
  }


  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if ((iTIO_SizeOf(datatype) == 0) && HaveCoords(icoords,jcoords,kcoords)) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDatasetSection( fileID, lpID, meshID, sectionID,
                                            CLASS_MESH, (int)dim,
                                            TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                            tmesh->ndims,
                                            tmesh->nchunks, tmesh->chunkinfo, tsection,
                                            DATAPOS_NODE, nnptr[dim], coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset section (coord)", TIO_ERR_OBJECT_READ);
    }
  }

  TIOset(ndims, tmesh->ndims);
  TIOset(ni, nn[0]);
  TIOset(nj, nn[1]);
  TIOset(nk, nn[2]);


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_UnstrMesh_nghosts
 * TIO_Get_PointMesh_nghosts
 *
 **************************************************************************************************/
TIO_t
TIO_Get_UnstrMesh_nghosts( const TIO_File_t   fileID,
                           const TIO_Object_t meshID,
                           TIO_Size_t         *nghosts )
{
  const char *subname = "TIO_Get_UnstrMesh_nghosts()";

  return (cTIO_GetMeshGhosts(subname, fileID, meshID, TIO_MESH_UNSTRUCT, nghosts));
}


TIO_t
TIO_Get_PointMesh_nghosts( const TIO_File_t   fileID,
                           const TIO_Object_t meshID,
                           TIO_Size_t         *nghosts )
{
  const char *subname = "TIO_Get_PointMesh_nghosts()";

  return (cTIO_GetMeshGhosts(subname, fileID, meshID, TIO_MESH_POINT, nghosts));
}


TIO_t
cTIO_GetMeshGhosts( const char         subname[],
                    const TIO_File_t   fileID,
                    const TIO_Object_t meshID,
                    const TIO_Mesh_t   meshtype,
                    TIO_Size_t         *nghosts )
{
  struct iMesh_t *tmesh;

  TIO_Size_t idx;
  int        hasghosts;
  TIO_t      trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, meshID, CLASS_MESH,
                                     NULL, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  hasghosts = FALSE;

  switch (meshtype) {

  case TIO_MESH_UNSTRUCT:
    for (idx=0; idx < tmesh->nchunks; ++idx) {
      hasghosts = (tmesh->chunkinfo[idx].unstr.nghost_nodes > 0);
      if (hasghosts) {
        break;
      }
    }
    break;

  case TIO_MESH_POINT:
    for (idx=0; idx < tmesh->nchunks; ++idx) {
      hasghosts = (tmesh->chunkinfo[idx].point.nghosts > 0);
      if (hasghosts) {
        break;
      }
    }
    break;

  default:
    break;
  }

  if (hasghosts) {
    TIOset(nghosts, TIO_GHOSTS_ALL);
  }
  else {
    TIOset(nghosts, TIO_GHOSTS_NONE);
  }


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/**************************************
 **************************************
 **
 **   UNSTR MESHES
 **
 **************************************
 **************************************/


/***************************************************************************************************
 *
 * TIO_Write_UnstrMesh_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Write_UnstrMesh_Chunk( const TIO_File_t   fileID,
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
                           const void         *kcoords )
{
  const char *subname = "TIO_Write_UnstrMesh_Chunk()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  iUnstr_t     unstr;
  TIO_Data_t   dtype;
  iDataPos_t   datapos;
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (tmesh->meshtype != TIO_MESH_UNSTRUCT) {
    TIOreturn(ERR_USER, "Mesh is not unstruct type - cannot write unstr chunk",
              TIO_ERR_INVALID_OPERATION);
  }

  if (tmesh->nchunks_unset > 0) {
    TIOreturn(ERR_USER, "Not all mesh chunks have been set", TIO_ERR_OBJECT_WRITE);
  }
  if ((tmesh->status != STATUS_CREATED) && (tmesh->status != STATUS_WRITTEN)) {
    TIOreturn(ERR_USER, "Cannot write to pre-existing mesh", TIO_ERR_OBJECT_NOT_WRITEABLE);
  }
  if (iTIO_SizeOf(graph_datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid graph_datatype", TIO_ERR_INVALID_DATATYPE);
  }
  if (iTIO_SizeOf(coord_datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid coord_datatype", TIO_ERR_INVALID_DATATYPE);
  }


  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    const void *coordptr = xTIO_SetCoords(dim, (void *)icoords, (void *)jcoords, (void *)kcoords);

    irc = tfile->WriteChunkedDataset( fileID, lpID, meshID,
                                      CLASS_MESH, (int)dim,
                                      xfer, coord_datatype, TIO_MESH_UNSTRUCT,
                                      tmesh->ndims, TIO_GHOSTS_ALL,
                                      tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                      DATAPOS_NODE, idx, coordptr );
    if (0 == irc) {
      tmesh->status = STATUS_WRITTEN;
    }
    else if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write mesh dataset (coord)", TIO_ERR_OBJECT_WRITE);
    }
    else {
      /* nothing to worry about */
      ;
    }

  }

  for (unstr=UNSTR_NODEIDS; unstr<=UNSTR_CONNECT; ++unstr) {

    const void *meshptr;

    switch (unstr) {
    case UNSTR_NODEIDS:
      meshptr = nodeIDs;
      dtype   = graph_datatype;
      datapos = DATAPOS_NODE;
      break;
    case UNSTR_CELLIDS:
      meshptr = cellIDs;
      dtype   = graph_datatype;
      datapos = DATAPOS_CELL;
      break;
    case UNSTR_SHAPES:
      meshptr = shapes;
      dtype   = TIO_ENUM_T;
      datapos = DATAPOS_SHAPE;
      break;
    case UNSTR_NCPS:
      meshptr = ncells_per_shape;
      dtype   = graph_datatype;
      datapos = DATAPOS_SHAPE;
      break;
    case UNSTR_CONNECT:
      meshptr = connectivity;
      dtype   = graph_datatype;
      datapos = DATAPOS_CONNECT;
      break;
    default:
      break;
    }

    if (NULL == meshptr) {
      continue;
    }

    irc = tfile->WriteChunkedDataset( fileID, lpID, meshID,
                                      CLASS_MESH, (int)unstr,
                                      xfer, dtype, TIO_MESH_UNSTRUCT,
                                      tmesh->ndims, TIO_GHOSTS_ALL,
                                      tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                      datapos, idx, meshptr );
    if (0 == irc) {
      tmesh->status = STATUS_WRITTEN;
    }
    else if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write mesh dataset (graph)", TIO_ERR_OBJECT_WRITE);
    }
    else {
      /* nothing to worry about */
    }

  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_UnstrMesh_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_UnstrMesh_Chunk( const TIO_File_t   fileID,
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
                          void               *kcoords )
{
  const char *subname = "TIO_Read_UnstrMesh_Chunk()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Size_t   *nn;
  TIO_Dims_t   dim;
  iUnstr_t     unstr;
  TIO_Data_t   dtype;
  iDataPos_t   datapos;
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (tmesh->meshtype != TIO_MESH_UNSTRUCT) {
    TIOreturn(ERR_USER, "Mesh is not unstruct type - cannot read unstr chunk",
              TIO_ERR_INVALID_OPERATION);
  }
  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if (    (nodeIDs != NULL) || (cellIDs != NULL)
       || (ncells_per_shape != NULL) || (connectivity != NULL)) {
    if (iTIO_SizeOf(graph_datatype) == 0)
      TIOreturn(ERR_USER, "Invalid graph_datatype", TIO_ERR_INVALID_DATATYPE);
  }

  if ((icoords != NULL) || (jcoords != NULL) || (kcoords != NULL)) {
    if (iTIO_SizeOf(coord_datatype) == 0) {
      TIOreturn(ERR_USER, "Invalid coord_datatype", TIO_ERR_INVALID_DATATYPE);
    }
  }

  if (    (TIO_GHOSTS_ALL      == nghosts)
       || (TIO_GHOSTS_NONE     == nghosts)
       || (TIO_GHOSTS_INTERNAL == nghosts ) )
    /* ok */
    ;
  else {
    TIOreturn(ERR_USER, "Invalid nghosts - must use TIO_GHOSTS_ALL or TIO_GHOSTS_NONE",
              TIO_ERR_INVALID_ARG);
  }

  TIOset(ndims, tmesh->ndims);


  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDataset( fileID, lpID, meshID,
                                     CLASS_MESH, (int)dim,
                                     xfer, coord_datatype, TIO_MESH_UNSTRUCT,
                                     tmesh->ndims, nghosts,
                                     tmesh->nchunks, tmesh->chunkinfo, 
                                     DATAPOS_NODE, idx, NULL, coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset (coord)", TIO_ERR_OBJECT_READ);
    }
  }


  for (unstr=UNSTR_NODEIDS; unstr<=UNSTR_CONNECT; ++unstr) {

    void *meshptr;

    switch (unstr) {
    case UNSTR_NODEIDS:
      meshptr = nodeIDs;
      nn      = nnodes;
      dtype   = graph_datatype;
      datapos = DATAPOS_NODE;
      break;
    case UNSTR_CELLIDS:
      meshptr = cellIDs;
      nn      = ncells;
      dtype   = graph_datatype;
      datapos = DATAPOS_CELL;
      break;
    case UNSTR_SHAPES:
      meshptr = shapes;
      nn      = nshapes;
      dtype   = TIO_ENUM_T;
      datapos = DATAPOS_SHAPE;
      break;
    case UNSTR_NCPS:
      meshptr = ncells_per_shape;
      nn      = NULL;
      dtype   = graph_datatype;
      datapos = DATAPOS_SHAPE;
      break;
    case UNSTR_CONNECT:
      meshptr = connectivity;
      nn      = nconnectivity;
      dtype   = graph_datatype;
      datapos = DATAPOS_CONNECT;
      break;
    default:
      break;
    }

    irc = tfile->ReadChunkedDataset( fileID, lpID, meshID,
                                     CLASS_MESH, (int)unstr,
                                     xfer, dtype, TIO_MESH_UNSTRUCT,
                                     tmesh->ndims, nghosts,
                                     tmesh->nchunks, tmesh->chunkinfo, 
                                     datapos, idx, nn, meshptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset (graph)", TIO_ERR_OBJECT_READ);
    }
  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_UnstrMesh_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_UnstrMesh_All( const TIO_File_t   fileID,
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
                        void               *kcoords )
{
  const char *subname = "TIO_Read_UnstrMesh_All()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  iUnstr_t     unstr;
  TIO_Data_t   dtype;
  iDataPos_t   datapos;
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (tmesh->meshtype != TIO_MESH_UNSTRUCT) {
    TIOreturn(ERR_USER, "Mesh is not unstruct type", TIO_ERR_INVALID_OPERATION);
  }
  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if (iTIO_SizeOf(graph_datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid graph_datatype", TIO_ERR_INVALID_DATATYPE);
  }
  if (iTIO_SizeOf(coord_datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid coord_datatype", TIO_ERR_INVALID_DATATYPE);
  }

  for (unstr=UNSTR_NODEIDS; unstr<=UNSTR_CONNECT; ++unstr) {

    void *meshptr;

    switch (unstr) {
    case UNSTR_NODEIDS:
      meshptr = nodeIDs;
      dtype   = graph_datatype;
      datapos = DATAPOS_NODE;
      break;
    case UNSTR_CELLIDS:
      meshptr = cellIDs;
      dtype   = graph_datatype;
      datapos = DATAPOS_CELL;
      break;
    case UNSTR_SHAPES:
      meshptr = shapes;
      dtype   = TIO_ENUM_T;
      datapos = DATAPOS_SHAPE;
      break;
    case UNSTR_NCPS:
      meshptr = ncells_per_shape;
      dtype   = graph_datatype;
      datapos = DATAPOS_SHAPE;
      break;
    case UNSTR_CONNECT:
      meshptr = connectivity;
      dtype   = graph_datatype;
      datapos = DATAPOS_CONNECT;
      break;
    }

    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, meshID,
                                        CLASS_MESH, (int)unstr,
                                        TIO_XFER_INDEPENDENT, dtype, TIO_MESH_UNSTRUCT,
                                        tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info, 
                                        datapos, NULL, meshptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset (graph)", TIO_ERR_OBJECT_READ);
    }
  }


  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, meshID,
                                        CLASS_MESH, (int)dim,
                                        TIO_XFER_INDEPENDENT, coord_datatype, TIO_MESH_UNSTRUCT,
                                        tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info, 
                                        DATAPOS_NODE, NULL, coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset (coord)", TIO_ERR_OBJECT_READ);
    }
  }


  TIOset(ndims,         tmesh->ndims);
  TIOset(nnodes,        tmesh->info.unstr.nnodes);
  TIOset(ncells,        tmesh->info.unstr.ncells);
  TIOset(nshapes,       tmesh->info.unstr.nshapes);
  TIOset(nconnectivity, tmesh->info.unstr.nconnectivity);

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_UnstrMesh_Section
 *
 **************************************************************************************************/
/*
TIO_t
TIO_Read_UnstrMesh_Section( const TIO_File_t   fileID,
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
                            void               *kcoords )
*/


TIO_t
TIO_Read_UnstrMesh_Section( const TIO_File_t   fileID,
                            const TIO_Object_t meshID,
                            const TIO_Object_t sectionID,
                            const TIO_Xfer_t   xfer,
                            const TIO_Data_t   datatype,
                            TIO_Dims_t         *ndims,
                            TIO_Size_t         *nnodes,
                            void               *icoords,
                            void               *jcoords,
                            void               *kcoords )
{
  const char *subname = "TIO_Read_UnstrMesh_Section()";

  struct iFile_t    *tfile;
  struct iMesh_t    *tmesh;
  union  iSection_t *tsection;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = iTIO_GetObject(fileID, NULL, sectionID, CLASS_SECTION, (void **)&tsection);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid sectionID", TIO_ERR_INVALID_SECTIONID);
  }

  if (!iTIO_CheckSection(tmesh, sectionID)) {
    TIOreturn(ERR_USER, "Invalid sectionID - not defined for this mesh", TIO_ERR_INVALID_SECTIONID);
  }
  if (tmesh->meshtype != TIO_MESH_UNSTRUCT) {
    TIOreturn(ERR_USER, "Mesh is not unstruct type - cannot read unstr section",
              TIO_ERR_INVALID_OPERATION);
  }
  if (tsection->unstr.centring != TIO_CENTRE_NODE) {
    TIOreturn(ERR_USER, "Section centring must be TIO_CENTRE_NODE", TIO_ERR_INVALID_OPERATION);
  }
  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if ((iTIO_SizeOf(datatype) == 0) && HaveCoords(icoords,jcoords,kcoords)) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDatasetSection( fileID, lpID, meshID, sectionID,
                                            CLASS_MESH, (int)dim,
                                            TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                            tmesh->ndims,
                                            tmesh->nchunks, tmesh->chunkinfo, tsection,
                                            DATAPOS_NODE, NULL, coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset section (coord)", TIO_ERR_OBJECT_READ);
    }
  }

  TIOset(ndims, tmesh->ndims);
  TIOset(nnodes, tsection->unstr.nIDs);


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/**************************************
 **************************************
 **
 **   POINT MESHES
 **
 **************************************
 **************************************/


/***************************************************************************************************
 *
 * TIO_Write_PointMesh_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Write_PointMesh_Chunk( const TIO_File_t   fileID,
                           const TIO_Object_t meshID,
                           const TIO_Size_t   idx,
                           const TIO_Xfer_t   xfer,
                           const TIO_Data_t   datatype,
                           const void         *icoords,
                           const void         *jcoords,
                           const void         *kcoords )
{
  const char *subname = "TIO_Write_PointMesh_Chunk()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (iTIO_SizeOf(datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }
  if (tmesh->nchunks_unset > 0) {
    TIOreturn(ERR_USER, "Not all mesh chunks have been set", TIO_ERR_OBJECT_WRITE);
  }
  if ((tmesh->status != STATUS_CREATED) && (tmesh->status != STATUS_WRITTEN)) {
    TIOreturn(ERR_USER, "Cannot write to pre-existing mesh", TIO_ERR_OBJECT_NOT_WRITEABLE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    const void *coordptr = xTIO_SetCoords(dim, (void *)icoords, (void *)jcoords, (void *)kcoords);

    irc = tfile->WriteChunkedDataset( fileID, lpID, meshID,
                                      CLASS_MESH, (int)dim,
                                      xfer, datatype, TIO_MESH_POINT,
                                      tmesh->ndims, TIO_GHOSTS_ALL,
                                      tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo,
                                      DATAPOS_NODE, idx, coordptr );
    if (0 == irc) {
      tmesh->status = STATUS_WRITTEN;
    }
    else if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write mesh dataset (coords)", TIO_ERR_OBJECT_WRITE);
    }
    else {
      /* nothing to worry about */
    }

  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}


/***************************************************************************************************
 *
 * TIO_Read_PointMesh_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointMesh_Chunk( const TIO_File_t   fileID,
                          const TIO_Object_t meshID,
                          const TIO_Size_t   idx,
                          const TIO_Xfer_t   xfer,
                          const TIO_Data_t   datatype,
                          const TIO_Size_t   nghosts,
                          TIO_Dims_t         *ndims,
                          TIO_Size_t         *nnodes,
                          void               *icoords,
                          void               *jcoords,
                          void               *kcoords )
{
  const char *subname = "TIO_Read_PointMesh_Chunk()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (iTIO_SizeOf(datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if (    (TIO_GHOSTS_ALL  == nghosts)
       || (TIO_GHOSTS_NONE == nghosts) ) {
    /* ok */
    ;
  }
  else {
    TIOreturn(ERR_USER, "Invalid nghosts - must use TIO_GHOSTS_ALL or TIO_GHOSTS_NONE",
              TIO_ERR_INVALID_ARG);
  }

  TIOset(ndims, tmesh->ndims);

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDataset( fileID, lpID, meshID,
                                     CLASS_MESH, (int)dim,
                                     xfer, datatype, TIO_MESH_POINT,
                                     tmesh->ndims, nghosts,
                                     tmesh->nchunks, tmesh->chunkinfo, 
                                     DATAPOS_NODE, idx, nnodes, coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset (coord)", TIO_ERR_OBJECT_READ);
    }
  }


  TIOend(subname,1);


  return (TIO_SUCCESS);
}


/***************************************************************************************************
 *
 * TIO_Read_PointMesh_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointMesh_All( const TIO_File_t   fileID,
                        const TIO_Object_t meshID,
                        const TIO_Data_t   datatype,
                        TIO_Dims_t         *ndims,
                        TIO_Size_t         *nnodes,
                        void               *icoords,
                        void               *jcoords,
                        void               *kcoords )
{
  const char *subname = "TIO_Read_PointMesh_All()";

  struct iFile_t *tfile;
  struct iMesh_t *tmesh;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (iTIO_SizeOf(datatype) == 0) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, meshID,
                                        CLASS_MESH, (int)dim,
                                        TIO_XFER_INDEPENDENT, datatype, TIO_MESH_POINT,
                                        tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info, 
                                        DATAPOS_NODE, NULL, coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset (coord)", TIO_ERR_OBJECT_READ);
    }
  }


  TIOset(ndims,  tmesh->ndims);
  TIOset(nnodes, tmesh->info.point.nnodes);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_PointMesh_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointMesh_Section(  const TIO_File_t   fileID,
                             const TIO_Object_t meshID,
                             const TIO_Object_t sectionID,
                             const TIO_Xfer_t   xfer,
                             const TIO_Data_t   datatype,
                             TIO_Dims_t         *ndims,
                             TIO_Size_t         *nnodes,
                             void               *icoords,
                             void               *jcoords,
                             void               *kcoords )
{
  const char *subname = "TIO_Read_PointMesh_Section()";

  struct iFile_t    *tfile;
  struct iMesh_t    *tmesh;
  union  iSection_t *tsection;

  TIO_Object_t lpID;
  TIO_Dims_t   dim;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, meshID, CLASS_MESH,
                                     &tfile, (void **)&tmesh);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = iTIO_GetObject(fileID, NULL, sectionID, CLASS_SECTION, (void **)&tsection);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid sectionID", TIO_ERR_INVALID_SECTIONID);
  }

  if (!iTIO_CheckSection(tmesh, sectionID)) {
    TIOreturn(ERR_USER, "Invalid sectionID - not defined for this mesh", TIO_ERR_INVALID_SECTIONID);
  }

  if (tmesh->meshtype != TIO_MESH_POINT) {
    TIOreturn(ERR_USER, "Mesh is not point type - cannot read point section",
              TIO_ERR_INVALID_OPERATION);
  }

  if (tmesh->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written mesh", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if ((iTIO_SizeOf(datatype) == 0) && HaveCoords(icoords,jcoords,kcoords)) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {

    void *coordptr = xTIO_SetCoords(dim, icoords, jcoords, kcoords);

    irc = tfile->ReadChunkedDatasetSection( fileID, lpID, meshID, sectionID,
                                            CLASS_MESH, (int)dim,
                                            TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                            tmesh->ndims,
                                            tmesh->nchunks, tmesh->chunkinfo, tsection,
                                            DATAPOS_NODE, NULL, coordptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read mesh dataset section (coord)", TIO_ERR_OBJECT_READ);
    }
  }

  TIOset(ndims, tmesh->ndims);
  TIOset(nnodes, tsection->point.nIDs);


  TIOend(subname,1);


  return (TIO_SUCCESS);
}









/***************************************************************************************************
 *
 * iTIO_CreateMeshDatasets
 *
 **************************************************************************************************/
int
iTIO_CreateMeshDatasets( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const TIO_Object_t meshID,
                         struct iFile_t     *tfile,
                         struct iMesh_t     *tmesh )
{
  const char *subname = "iTIO_CreateMeshDatasets()";

  TIO_Dims_t dim;
  size_t nl, ii;
  int irc;



  TIOassert((tmesh->nchunks_unset > 0), ERR_USER, "Not all mesh chunks have been set", -1);


  switch (tmesh->meshtype) {

  case TIO_MESH_QUAD_COLINEAR:

    /* Need 3 datasets :
            0  - icoords
            1  - jcoords
            2  - kcoords

       Note - chunkinfo isn't passed into create call below, whole mesh info is
              passed for colinear.
    */

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                         xMeshCoordName[dim], (int)dim, tmesh->coord_datatype,
                                         tmesh->meshtype, tmesh->ndims, 0,
                                         tmesh->nchunks, tmesh->chunkinfo,
                                         &tmesh->info, DATAPOS_NODE );
      if (irc <0) {
        TIOreturn(ERR_INT, "Failed to create coord dataset", ITIO_ERR_CREATE);
      }
    }

    break;

  case TIO_MESH_QUAD_NONCOLINEAR:

    /* Need 3 datasets :
            0  - icoords
            1  - jcoords
            2  - kcoords
    */

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                         xMeshCoordName[dim], (int)dim, tmesh->coord_datatype,
                                         tmesh->meshtype, tmesh->ndims, tmesh->info.quad.nghosts,
                                         tmesh->nchunks, tmesh->chunkinfo,
                                         &tmesh->info, DATAPOS_NODE );
      if (irc < 0) {
        TIOreturn(ERR_INT, "Failed to create coord dataset", ITIO_ERR_CREATE);
      }
    }

    break;


  case TIO_MESH_UNSTRUCT:

    /* Calculate size of required datasets.
       Need 8 datasets:
            0  - to hold icoords co-ordinate values for all non-ghost and ghost nodes on all chunks
            1  - jcoords
            2  - kcoords
            3  - to hold all non-ghost and ghost nodeIDs
            4  - cellIDs
            5  - to hold non-ghost and ghost cell shapes on all chunks
            6  - to hold non-ghost and ghost number of cells per shape on all chunks
            7  - connectivity  */

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                         xMeshCoordName[dim], (int)dim, tmesh->coord_datatype,
                                         tmesh->meshtype, tmesh->ndims,
                                         TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                         NULL, DATAPOS_NODE );
      if (irc < 0) {
        TIOreturn(ERR_INT, "Failed to create coord dataset", ITIO_ERR_CREATE);
      }
    }

    irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                       xMeshNodeIDsName, UNSTR_NODEIDS,
                                       tmesh->graph_datatype, tmesh->meshtype, tmesh->ndims,
                                       TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                       NULL, DATAPOS_NODE );
    TIOassert(irc < 0, ERR_INT, "Failed to create nodeIDs dataset", TIO_ERR_INT);


    irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                       xMeshCellIDsName, UNSTR_CELLIDS,
                                       tmesh->graph_datatype, tmesh->meshtype, tmesh->ndims,
                                       TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                       NULL, DATAPOS_CELL );
    TIOassert(irc < 0, ERR_INT, "Failed to create cellIDs dataset", TIO_ERR_INT);


    irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                       xMeshShapesName, UNSTR_SHAPES,
                                       TIO_ENUM_T, tmesh->meshtype, tmesh->ndims,
                                       TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                       NULL, DATAPOS_SHAPE );
    TIOassert(irc < 0, ERR_INT, "Failed to create shapes dataset", TIO_ERR_INT);


    irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                       xMeshCellsPerShapeName, UNSTR_NCPS,
                                       tmesh->graph_datatype, tmesh->meshtype, tmesh->ndims,
                                       TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                       NULL, DATAPOS_SHAPE );
    TIOassert(irc < 0, ERR_INT, "Failed to create shapes dataset", TIO_ERR_INT);


    irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                       xMeshConnectivityName, UNSTR_CONNECT,
                                       tmesh->graph_datatype, tmesh->meshtype, tmesh->ndims,
                                       TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                       NULL, DATAPOS_CONNECT );
    TIOassert(irc < 0, ERR_INT, "Failed to create shape dataset", TIO_ERR_INT);

    break;


  case TIO_MESH_POINT:

    /* Need 3 datasets :
            0  - icoords
            1  - jcoords
            2  - kcoords
    */

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->CreateChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                         xMeshCoordName[dim], (int)dim, tmesh->coord_datatype,
                                         tmesh->meshtype, tmesh->ndims,
                                         TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                         NULL, DATAPOS_NODE );
      if (irc < 0) {
        TIOreturn(ERR_INT, "Failed to create coord dataset", ITIO_ERR_CREATE);
      }
    }

    break;

  default:
    break;
  }


  /* Increase the status flag to signify that datasets have been created and are ready to
     be written to */

  tmesh->status = STATUS_CREATED;


  /* -- Now create datasets for any cached materials and quants */

  if (NULL == tmesh->todo) {
    return (0);
  }


  /* -- If mesh child objects (material & quants) created before all chunks set,
        these now need datasets creating for them */

  nl = tmesh->todo->nlist;
  for (ii=0; ii<nl; ++ii) {

    TIO_Object_t objID;
    iClass_t     objclass;

    objID    = tmesh->todo->objectID[ii];
    objclass = tmesh->todo->class[ii];

    irc = iTIO_HierarchyCreateObjectDatasets( fileID, meshID, objID, objclass, tfile, tmesh );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to create object datasets", TIO_ERR_INT);
    }
  }

  TIOfree (tmesh->todo->objectID);  tmesh->todo->objectID = NULL;
  TIOfree (tmesh->todo->class);     tmesh->todo->class    = NULL;
  TIOfree (tmesh->todo);            tmesh->todo           = NULL;  /* Nullify so that it isn't
                                                                      freed later in mesh close */

  return (0);
}



/***************************************************************************************************
 *
 * iTIO_AddMeshObject
 *
 **************************************************************************************************/
int
iTIO_AddMeshObject( const TIO_File_t   fileID,
                    const TIO_Object_t meshID,
                    struct iFile_t     *tfile,
                    struct iMesh_t     *tmesh,
                    const TIO_Object_t objectID,
                    const iClass_t     class )
{
  const char *subname = "iTIO_AddMeshObject()";

  const size_t Nalloc = 50;
  const size_t Ndelta = 20;

  size_t i;


  if (NULL == tmesh->todo) {

    tmesh->todo = (struct iToDo_t *)TIOmalloc(sizeof(struct iToDo_t));
    if (NULL == tmesh->todo) {
      TIOreturn(ERR_MEM, "Failed to malloc tmesh->todo", TIO_ERR_INT);
    }
    tmesh->todo->objectID = (TIO_Object_t *)TIOmalloc(sizeof(TIO_Object_t) * Nalloc);
    if (NULL == tmesh->todo->objectID) {
      TIOreturn(ERR_MEM, "Failed to malloc tmesh->todo->objectID", TIO_ERR_INT);
    }
    tmesh->todo->class = (iClass_t *)TIOmalloc(sizeof(iClass_t) * Nalloc);
    if (NULL == tmesh->todo->class) {
      TIOreturn(ERR_MEM, "Failed to malloc tmesh->todo->class", TIO_ERR_INT);
    }
    tmesh->todo->nlist  = 0;
    tmesh->todo->nalloc = Nalloc;
  }


  i = tmesh->todo->nlist;

  if ((i+1) > tmesh->todo->nalloc) {

    tmesh->todo->nalloc += Ndelta;

    tmesh->todo->objectID = TIOrealloc(tmesh->todo->objectID,  tmesh->todo->nalloc);
    if (NULL == tmesh->todo->objectID)
      TIOreturn(ERR_MEM, "Failed to realloc tmesh->todo->objectID", TIO_ERR_INT);

    tmesh->todo->class = TIOrealloc(tmesh->todo->class,  tmesh->todo->nalloc);
    if (NULL == tmesh->todo->class)
      TIOreturn(ERR_MEM, "Failed to realloc tmesh->todo->class", TIO_ERR_INT);
  }

  tmesh->todo->objectID[i] = objectID;
  tmesh->todo->class[i]    = class;

  ++tmesh->todo->nlist;


  return (0);
}





/***************************************************************************************************
 *
 * xTIO_GetMeshInfo
 *
 **************************************************************************************************/
static int
xTIO_GetMeshInfo( const char     subname[],
                  const int      getname,
                  struct iMesh_t *tmesh,
                  char           name[],
                  TIO_Mesh_t     *meshtype,
                  TIO_Coord_t    *coordtype,
                  TIO_Bool_t     *isAMR,
                  char           group[],
                  TIO_Size_t     *order,
                  TIO_Data_t     *graph_datatype,
                  TIO_Data_t     *coord_datatype,
                  TIO_Dims_t     *ndims,
                  TIO_Size_t     *n1,
                  TIO_Size_t     *n2,
                  TIO_Size_t     *n3,
                  TIO_Size_t     *n4,
                  TIO_Size_t     *nchunks,
                  char           iunits[],
                  char           junits[],
                  char           kunits[],
                  char           ilabel[],
                  char           jlabel[],
                  char           klabel[] )
{
  if (getname && (name != NULL)) {
    (void) iTIO_strcpy(name, tmesh->name);
  }

  TIOset(meshtype,  tmesh->meshtype);
  TIOset(coordtype, tmesh->coordtype);

  if (isAMR != NULL) {
    if (tmesh->isAMR) {
      *isAMR = TIO_TRUE;
    }
    else {
      *isAMR = TIO_FALSE;
    }
  }
  if (group != NULL) {
    (void) iTIO_strcpy(group, tmesh->group);
  }

  TIOset(order,          tmesh->order);
  TIOset(graph_datatype, tmesh->graph_datatype);
  TIOset(coord_datatype, tmesh->coord_datatype);
  TIOset(ndims,          tmesh->ndims);

  switch (tmesh->meshtype) {
  case TIO_MESH_QUAD_COLINEAR:
    TIOset(n1, tmesh->info.quad.ni);
    TIOset(n2, tmesh->info.quad.nj);
    TIOset(n3, tmesh->info.quad.nk);
    TIOset(n4, TIO_NULL);
    break;
  case TIO_MESH_QUAD_NONCOLINEAR:
    TIOset(n1, tmesh->info.quad.ni);
    TIOset(n2, tmesh->info.quad.nj);
    TIOset(n3, tmesh->info.quad.nk);
    TIOset(n4, tmesh->info.quad.nghosts);
    break;
  case TIO_MESH_UNSTRUCT:
    TIOset(n1, tmesh->info.unstr.nnodes);
    TIOset(n2, tmesh->info.unstr.ncells);
    TIOset(n3, tmesh->info.unstr.nshapes);
    TIOset(n4, tmesh->info.unstr.nconnectivity);
    break;
  case TIO_MESH_POINT:
    TIOset(n1, tmesh->info.point.nnodes);
    TIOset(n2, TIO_NULL);
    TIOset(n3, TIO_NULL);
    TIOset(n4, TIO_NULL);
    break;
  default:
    TIOreturn(ERR_INT, "Invalid meshtype", -1);
    break;
  }

  TIOset(nchunks, tmesh->nchunks);

  if (iunits  != NULL) (void) iTIO_strcpy(iunits, tmesh->iunits);
  if (junits  != NULL) (void) iTIO_strcpy(junits, tmesh->junits);
  if (kunits  != NULL) (void) iTIO_strcpy(kunits, tmesh->kunits);
  if (ilabel  != NULL) (void) iTIO_strcpy(ilabel, tmesh->ilabel);
  if (jlabel  != NULL) (void) iTIO_strcpy(jlabel, tmesh->jlabel);
  if (klabel  != NULL) (void) iTIO_strcpy(klabel, tmesh->klabel);


  return (0);
}




/***************************************************************************************************
 *
 * xTIO_OpenMeshDatasets
 *
 **************************************************************************************************/
static int
xTIO_OpenMeshDatasets( const TIO_File_t   fileID,
                       const TIO_Object_t parentID,
                       const TIO_Object_t meshID,
                       struct iFile_t     *tfile,
                       struct iMesh_t     *tmesh )
{
  const char *subname = "xTIO_OpenMeshDatasets()";

  TIO_Dims_t dim;

  int irc;


  switch (tmesh->meshtype) {

  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    /* 3 datasets :
            0  - icoords
            1  - jcoords
            2  - kcoords
    */

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->OpenChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                       xMeshCoordName[dim], (int)dim,
                                       tmesh->coord_datatype, tmesh->meshtype, tmesh->ndims,
                                       tmesh->info.quad.nghosts, tmesh->nchunks, tmesh->chunkinfo,
                                       &tmesh->info, DATAPOS_NODE );
      TIOassert(irc < 0, ERR_INT, "Failed to create coord dataset", TIO_ERR_INT);
    }

    break;


  case TIO_MESH_UNSTRUCT:

    /* 7 datasets:
            0  - to hold icoords co-ordinate values for all non-ghost and ghost nodes on all chunks
            1  - jcoords
            2  - kcoords
            3  - to hold all non-ghost and ghost nodeIDs
            4  - cellIDs
            5  - connectivity
            6  - to hold non-ghost and ghost cell shapes on all chunks  */

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->OpenChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                       xMeshCoordName[dim], (int)dim,
                                       tmesh->coord_datatype, tmesh->meshtype, tmesh->ndims,
                                       TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                       NULL, DATAPOS_NODE );
      TIOassert(irc < 0, ERR_INT, "Failed to open coord dataset", TIO_ERR_INT);
    }

    irc = tfile->OpenChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                     xMeshNodeIDsName, UNSTR_NODEIDS,
                                     tmesh->graph_datatype, tmesh->meshtype, tmesh->ndims,
                                     TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                     NULL, DATAPOS_NODE );
    TIOassert(irc < 0, ERR_INT, "Failed to open nodeIDs dataset", TIO_ERR_INT);


    irc = tfile->OpenChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                     xMeshCellIDsName, UNSTR_CELLIDS,
                                     tmesh->graph_datatype, tmesh->meshtype, tmesh->ndims,
                                     TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                     NULL, DATAPOS_CELL );
    TIOassert(irc < 0, ERR_INT, "Failed to open cellIDs dataset", TIO_ERR_INT);


    irc = tfile->OpenChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                     xMeshShapesName, UNSTR_SHAPES,
                                     TIO_ENUM_T, tmesh->meshtype, tmesh->ndims,
                                     TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                     NULL, DATAPOS_SHAPE );
    TIOassert(irc < 0, ERR_INT, "Failed to open shapes dataset", TIO_ERR_INT);

    irc = tfile->OpenChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                     xMeshCellsPerShapeName, UNSTR_NCPS,
                                     tmesh->graph_datatype, tmesh->meshtype, tmesh->ndims,
                                     TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                     NULL, DATAPOS_SHAPE );
    TIOassert(irc < 0, ERR_INT, "Failed to open shapes dataset", TIO_ERR_INT);


    irc = tfile->OpenChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                     xMeshConnectivityName, UNSTR_CONNECT,
                                     tmesh->graph_datatype, tmesh->meshtype, tmesh->ndims,
                                     TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                     NULL, DATAPOS_CONNECT );
    TIOassert(irc < 0, ERR_INT, "Failed to open shape dataset", TIO_ERR_INT);

    break;


  case TIO_MESH_POINT:

    /* 3 datasets :
            0  - icoords
            1  - jcoords
            2  - kcoords
    */

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->OpenChunkedDataset( fileID, parentID, meshID, CLASS_MESH,
                                       xMeshCoordName[dim], (int)dim,
                                       tmesh->coord_datatype, tmesh->meshtype, tmesh->ndims,
                                       TIO_GHOSTS_ALL, tmesh->nchunks, tmesh->chunkinfo,
                                       NULL, DATAPOS_NODE );
      TIOassert(irc < 0, ERR_INT, "Failed to open coord dataset", TIO_ERR_INT);
    }

    break;

  default:
    break;
  }


  return (0);
}





/***************************************************************************************************
 *
 * xTIO_CloseMeshDatasets
 *
 **************************************************************************************************/
static int
xTIO_CloseMeshDatasets( const TIO_File_t   fileID,
                        const TIO_Object_t parentID,
                        const TIO_Object_t meshID,
                        struct iFile_t     *tfile,
                        struct iMesh_t     *tmesh )
{
  const char *subname = "xTIO_CloseMeshDatasets()";

  TIO_Dims_t dim;
  int irc;


  switch (tmesh->meshtype) {

  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->CloseChunkedDataset( fileID, parentID, meshID, CLASS_MESH, (int)dim );
      TIOassert(irc < 0, ERR_INT, "Failed to close coord dataset", TIO_ERR_INT);
    }

    break;


  case TIO_MESH_UNSTRUCT:


    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->CloseChunkedDataset( fileID, parentID, meshID, CLASS_MESH, (int)dim );
      TIOassert(irc < 0, ERR_INT, "Failed to close coord dataset", TIO_ERR_INT);
    }

    irc = tfile->CloseChunkedDataset( fileID, parentID, meshID, CLASS_MESH, UNSTR_NODEIDS );
    TIOassert(irc < 0, ERR_INT, "Failed to close nodeIDs dataset", TIO_ERR_INT);

    irc = tfile->CloseChunkedDataset( fileID, parentID, meshID, CLASS_MESH, UNSTR_CELLIDS );
    TIOassert(irc < 0, ERR_INT, "Failed to close cellIDs dataset", TIO_ERR_INT);

    irc = tfile->CloseChunkedDataset( fileID, parentID, meshID, CLASS_MESH, UNSTR_SHAPES );
    TIOassert(irc < 0, ERR_INT, "Failed to close shape dataset", TIO_ERR_INT);

    irc = tfile->CloseChunkedDataset( fileID, parentID, meshID, CLASS_MESH, UNSTR_NCPS );
    TIOassert(irc < 0, ERR_INT, "Failed to close ncells per shape dataset", TIO_ERR_INT);

    irc = tfile->CloseChunkedDataset( fileID, parentID, meshID, CLASS_MESH, UNSTR_CONNECT );
    TIOassert(irc < 0, ERR_INT, "Failed to close connectivity dataset", TIO_ERR_INT);

    break;


  case TIO_MESH_POINT:

    for (dim=TIO_0D; dim<tmesh->ndims; ++dim) {
      irc = tfile->CloseChunkedDataset( fileID, parentID, meshID, CLASS_MESH, (int)dim );
      TIOassert(irc < 0, ERR_INT, "Failed to close coord dataset", TIO_ERR_INT);
    }

    break;


  default:
    break;
  }


  return (0);
}





/***************************************************************************************************
 *
 * xTIO_InitChunkInfo
 *
 * Must be called before creating/reading mesh datasets
 *
 **************************************************************************************************/
static int
xTIO_InitChunkInfo( const TIO_File_t   fileID,
                    const TIO_Object_t parentID,
                    const TIO_Object_t meshID,
                    struct iFile_t     *tfile,
                    struct iMesh_t     *tmesh,
                    TIO_Size_t         *nchunks,
                    const int          reading )
{
  const char *subname = "xTIO_InitChunkInfo()";

  TIO_Size_t lnck;
  TIO_Size_t ii;
  const union iInfo_t tmpinfo= {0};
  int irc;


  if (reading) {

    TIO_Size_t csize[1];

    /* use 0 for chunkinfo dataset ID within mesh object
       - will be closed by actual mesh gets read */

    irc = tfile->OpenDataset(fileID, parentID, meshID, CLASS_MESH,
                             xMeshChunkInfoName, 0, NULL, csize, NULL);

    lnck = csize[0] / INFONSIZE;

    *nchunks = lnck;

  }
  else {
    lnck = *nchunks;
  }


  tmesh->nchunks = lnck;

  tmesh->chunkinfo = (union iInfo_t *)TIOmalloc(lnck * sizeof(union iInfo_t));
  if (NULL == tmesh->chunkinfo) {
    TIOreturn(ERR_MEM, "Failed to malloc tmesh->chunkinfo", TIO_ERR_INT);
  }


  if (reading) {

    irc = tfile->ReadDataset(fileID, parentID, meshID, CLASS_MESH,
                             0, TIO_XFER_COLLECTIVE, TIO_SIZE_T, tmesh->chunkinfo);

    irc = tfile->CloseDataset(fileID, parentID, meshID, CLASS_MESH, 0);

    tmesh->nchunks_unset = 0;
    tmesh->chunkset      = NULL;

  }
  else {

    tmesh->nchunks_unset = lnck;

    tmesh->chunkset = (int *)TIOmalloc(lnck * sizeof(int));
    if (NULL == tmesh->chunkset) {
      TIOreturn(ERR_MEM, "Failed to malloc tmesh->chunkset", TIO_ERR_INT);
    }

    for (ii=0; ii<lnck; ++ii) {
      tmesh->chunkset[ii] = FALSE;
      tmesh->chunkinfo[ii] = tmpinfo;
#ifndef NDEBUG
      switch (tmesh->meshtype) {
      case TIO_MESH_QUAD_COLINEAR:
      case TIO_MESH_QUAD_NONCOLINEAR:
        tmesh->chunkinfo[ii].quad.ni       = 0;
        tmesh->chunkinfo[ii].quad.nj       = 0;
        tmesh->chunkinfo[ii].quad.nk       = 0;
        tmesh->chunkinfo[ii].quad.il       = 0;
        tmesh->chunkinfo[ii].quad.ih       = 0;
        tmesh->chunkinfo[ii].quad.jl       = 0;
        tmesh->chunkinfo[ii].quad.jh       = 0;
        tmesh->chunkinfo[ii].quad.kl       = 0;
        tmesh->chunkinfo[ii].quad.kh       = 0;
        tmesh->chunkinfo[ii].quad.nghosts  = 0;
        tmesh->chunkinfo[ii].quad.nmixcell = 0;
        tmesh->chunkinfo[ii].quad.nmixcomp = 0;
        break;
      case TIO_MESH_UNSTRUCT:
        tmesh->chunkinfo[ii].unstr.nnodes              = 0;
        tmesh->chunkinfo[ii].unstr.ncells              = 0;
        tmesh->chunkinfo[ii].unstr.nshapes             = 0;
        tmesh->chunkinfo[ii].unstr.nconnectivity       = 0;
        tmesh->chunkinfo[ii].unstr.nghost_nodes        = 0;
        tmesh->chunkinfo[ii].unstr.nghost_cells        = 0;
        tmesh->chunkinfo[ii].unstr.nghost_shapes       = 0;
        tmesh->chunkinfo[ii].unstr.nghost_connectivity = 0;
        tmesh->chunkinfo[ii].unstr.nmixcell            = 0;
        tmesh->chunkinfo[ii].unstr.nmixcomp            = 0;
        break;
      case TIO_MESH_POINT:
        tmesh->chunkinfo[ii].point.nnodes   = 0;
        tmesh->chunkinfo[ii].point.nl       = 0;
        tmesh->chunkinfo[ii].point.nh       = 0;
        tmesh->chunkinfo[ii].point.nghosts  = 0;
        break;
      default:
        break;
      }
#endif
    }


  }

  tmesh->todo = NULL;   /* Uses nullness to detect whether anything is later needed */


  return (0);
}


/***************************************************************************************************
 *
 * xTIO_FinalizeChunkInfo
 *
 * Must be called after closing mesh datasets
 *
 **************************************************************************************************/
static int
xTIO_FinalizeChunkInfo( const TIO_File_t   fileID,
                        const TIO_Object_t parentID,
                        const TIO_Object_t meshID,
                        struct iFile_t     *tfile,
                        struct iMesh_t     *tmesh )
{
  const char *subname="xTIO_FinalizeChunkInfo()";

  TIO_Size_t csize[1];

  int irc;


  /* Create and write chunk table if it doesn't already exist  */

  if (tmesh->status == STATUS_OLD) {
    /* chunkset array not allocated when mesh is old */
    TIOfree (tmesh->chunkinfo); tmesh->chunkinfo = NULL;
    return (0);
  }

  csize[0] = INFONSIZE * tmesh->nchunks;


  /* use 0 for dataset ID within mesh object
     - all other mesh datasets will have been closed by now */

  irc = tfile->CreateDataset(fileID, parentID, meshID, CLASS_MESH,
                             xMeshChunkInfoName, 0, TIO_1D, csize, TIO_SIZE_T);

  if (0 == tfile->rank) {
    irc = tfile->WriteDataset(fileID, parentID, meshID, CLASS_MESH,
                              0, TIO_XFER_INDEPENDENT, TIO_SIZE_T, tmesh->chunkinfo);
  }

  tfile->CloseDataset(fileID, parentID, meshID, CLASS_MESH, 0);


  TIOfree (tmesh->chunkset);   tmesh->chunkset  = NULL;
  TIOfree (tmesh->chunkinfo);  tmesh->chunkinfo = NULL;

  return (0);
}




/***************************************************************************************************
 *
 * xTIO_ValidMeshType
 * xTIO_ValidCoordType
 *
 **************************************************************************************************/
static int
xTIO_ValidMeshType( const TIO_Mesh_t meshtype )
{
  if ( (meshtype < TIO_MESH_MIN) || (meshtype > TIO_MESH_MAX) ) {
    return (-1);
  }

  return (0);
}


static int
xTIO_ValidCoordType( const TIO_Coord_t coordtype )
{
  if ( (coordtype < TIO_COORD_MIN) || (coordtype > TIO_COORD_MAX) )
    return (-1);

  return (0);
}



/*
 * EOF
 */

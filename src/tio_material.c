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

#include "tio_material.h"


static int xTIO_GetMaterialInfo(       const char         *subname,
                                       const int          getname,
                                       struct iMaterial_t *tmat,
                                       char               name[],
                                       TIO_Data_t         *datatype,
                                       TIO_Size_t         *nmat,
                                       TIO_Size_t         *nghosts,
                                       TIO_Bool_t         *ismixed,
                                       TIO_Data_t         *ncomp_datatype,
                                       TIO_Data_t         *ptr_datatype,
                                       TIO_Data_t         *vf_datatype );

static TIO_t cTIO_WriteMaterialChunk(  const char         subname[],
                                       const TIO_Mesh_t   subtype,
                                       const TIO_File_t   fileID,
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

static TIO_t cTIO_ReadMaterialChunk(   const char         subname[],
                                       const TIO_Mesh_t   subtype,
                                       const TIO_File_t   fileID,
                                       const TIO_Object_t materialID,
                                       const TIO_Size_t   idx,
                                       const TIO_Xfer_t   xfer,
                                       const TIO_Data_t   datatype,
                                       const TIO_Size_t   nghosts,
                                       TIO_Dims_t         *ndims,
                                       TIO_Size_t         *nn,
                                       void               *mat,
                                       const TIO_Data_t   ncomp_datatype,
                                       const TIO_Data_t   ptr_datatype,
                                       const TIO_Data_t   vf_datatype,
                                       TIO_Size_t         *nmixcell,
                                       TIO_Size_t         *nmixcomp,
                                       void               *ncomp,
                                       void               *ptr,
                                       void               *mixmat,
                                       void               *vf  );

static TIO_t cTIO_ReadMaterialAll(     const char         subname[],
                                       const TIO_Mesh_t   subtype,
                                       const TIO_File_t   fileID,
                                       const TIO_Object_t materialID,
                                       const TIO_Data_t   datatype,
                                       TIO_Dims_t         *ndims,
                                       TIO_Size_t         *nn,
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

static TIO_t cTIO_ReadMaterialSection( const char             subname[],
                                       const TIO_Mesh_t       subtype,
                                       const TIO_File_t       fileID,
                                       const TIO_Object_t     materialID,
                                       const TIO_Object_t     sectionID,
                                       const TIO_Xfer_t       xfer,
                                       const TIO_Data_t       datatype,
                                       TIO_Dims_t             *ndims,
                                       TIO_Size_t             *nn,
                                       void                   *mat,
                                       const TIO_Data_t       ncomp_datatype,
                                       const TIO_Data_t       ptr_datatype,
                                       const TIO_Data_t       vf_datatype,
                                       TIO_Size_t             *nmixcell,
                                       TIO_Size_t             *nmixcomp,
                                       void                   *ncomp,
                                       void                   *ptr,
                                       void                   *mixmat,
                                       void                   *vf );


static const char *xMaterialNumsName    = "TIO_MATERIAL_NUMS";
static const char *xMaterialNamesName   = "TIO_MATERIAL_NAMES";

static const char *xMaterialDataName    = "TIO_DATA";
static const char *xMaterialNcompName   = "TIO_NCOMP";
static const char *xMaterialPtrName     = "TIO_PTR";
static const char *xMaterialVolfracName = "TIO_VOLFRAC";



/***************************************************************************************************
 *
 * TIO_List_Material
 *
 **************************************************************************************************/
TIO_t
TIO_List_Material( const TIO_File_t   fileID,
                   const TIO_Object_t meshID,
                   char               name[TIO_STRLEN] )
{
  const char *subname = "TIO_List_Material()";

  TIO_Size_t n;
  char       lname[1][TIO_STRLEN];
  TIO_t      trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyListObjects( subname, fileID, meshID, CLASS_MESH, CLASS_MATERIAL,
                                   &n, lname );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }


  /* -- n should be positive if a material exists */
  if (n > 0) {
    if (name != NULL) {
      (void)strcpy(name, lname[0]);
    }
    trc = TIO_SUCCESS;
  }
  else {
    trc = TIO_FAIL;
  }

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Create_Material
 *
 **************************************************************************************************/
TIO_t
TIO_Create_Material( const TIO_File_t   fileID,
                     const TIO_Object_t meshID,
                     const char         name[],
                     TIO_Object_t       *materialID,
                     const TIO_Data_t   datatype,
                     const TIO_Size_t   nmat,
                     const TIO_Size_t   nghosts,
                     const TIO_Bool_t   ismixed,
                     const TIO_Data_t   ncomp_datatype,
                     const TIO_Data_t   ptr_datatype,
                     const TIO_Data_t   vf_datatype )
{
  const char *subname = "TIO_Create_Material()";

  struct iFile_t     *tfile;
  struct iMesh_t     *tmesh;
  struct iMaterial_t *tmat;

  TIO_Object_t    lmatID;
  TIO_Size_t      lnghosts;

  int             irc;
  TIO_t           trc;



  TIObegin(subname,1);

  *materialID = TIO_NULL;    /* Ensure NULL value on failure */


  /* -- Check in the parent mesh to check it doesn't already contain a material */

  irc = iTIO_GetObject(fileID, NULL, meshID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid meshID", TIO_ERR_INVALID_OBJECTID);
  }
  if (tmesh->matghosts != TIO_GHOSTS_NULL) {
    TIOreturn(ERR_USER, "Cannot only have one material per mesh", TIO_ERR_INVALID_OPERATION);
  }
  else {
    if (STATUS_OLD == tmesh->status) {
      tmesh->status = STATUS_MODIFIED;    /*  Ensures that the  matghosts attribute gets  */
    }                                     /*  written to the old, existing mesh           */
  }


  /* -- Check object-specific inbound args are valid */

  trc = cTIO_CheckDatatype(subname, "datatype", FALSE, datatype, TIO_TYPECLASS_SIGNED_INT);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  /* Check nghosts & mixed */

  trc = cTIO_CheckGhosts( subname, fileID, meshID, tmesh, nghosts, TIO_GHOSTS_NULL, &lnghosts, FALSE );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (TIO_TRUE == ismixed) {

    if (TIO_MESH_POINT == tmesh->meshtype) {
      TIOreturn(ERR_USER, "ismixed must be false for material in point mesh",
                TIO_ERR_INVALID_OPERATION);
    }

    trc = cTIO_CheckDatatype(subname, "ncomp_datatype", FALSE, ncomp_datatype,
                             TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "ptr_datatype", FALSE, ptr_datatype,
                             TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "vf_datatype", FALSE, vf_datatype,
                             TIO_TYPECLASS_FLOAT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
  }


  tmat = (struct iMaterial_t *)TIOmalloc(sizeof(struct iMaterial_t));
  if (NULL == tmat) {
    TIOreturn(ERR_MEM, "Failed to malloc tmat", TIO_ERR_INT);
  }

  tmat->matnames = NULL;   /* Explicitly nullify so can check in cleandie */
  tmat->matnums  = NULL;


  trc = cTIO_HierarchyCreateObject( subname, fileID, meshID, CLASS_MESH, CLASS_MATERIAL, name,
                                    &tfile, &lmatID, tmat );
  if (trc != TIO_SUCCESS) {
    TIOfree (tmat); tmat = NULL;
    cTIOreturn (trc);
  }


  (void) iTIO_strcpy(tmat->name, name);
  tmat->status         = STATUS_NEW;                    /* Allows writing of mat datasets */
  tmat->rangeset       = FALSE;
  tmat->namesset       = FALSE;
  tmat->datatype       = datatype;
  tmat->nmat           = nmat;
  tmat->nghosts        = lnghosts;
  tmat->ismixed        = (ismixed != TIO_FALSE);
  tmat->ncomp_datatype = ncomp_datatype;
  tmat->ptr_datatype   = ptr_datatype;
  tmat->vf_datatype    = vf_datatype;


  /* -- Malloc the stores for material numbers and names */

  tmat->matnames = TIOmalloc(nmat * sizeof(char) * TIO_STRLEN);
  if (NULL == tmat->matnames) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc mat matnames");
    trc = TIO_ERR_INT;
    goto cleandie;
  }

  tmat->matnums = TIOmalloc(nmat * iTIO_SizeOf(datatype));
  if (NULL == tmat->matnums) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc mat matnums");
    trc = TIO_ERR_INT;
    goto cleandie;
  }

  /* -- Create the material datasets, using the parent mesh chunks, if all chunks are set,
        otherwise this will be deferred until all are set*/

  if (0 == tmesh->nchunks_unset) {

    irc = iTIO_CreateMaterialDatasets(fileID, meshID, lmatID, tfile, tmesh, tmat);
    if (irc < 0) {
      (void) iTIO_PutErr(ERR_INT, "Failed to create material datasets");
      trc = TIO_ERR_OBJECT_CREATE;
      goto cleandie;
    }

  }
  else {

    irc = iTIO_AddMeshObject(fileID, meshID, tfile, tmesh, lmatID, CLASS_MATERIAL);
    TIOassert(irc < 0, ERR_INT, "Failed to add mesh object", TIO_ERR_INT);

  }


  /* -- Write object attributes */

  irc = 0;

  irc += tfile->WriteAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "datatype",       TIO_ENUM_T,
                           (void *)&tmat->datatype);

  irc += tfile->WriteAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "ncomp_datatype", TIO_ENUM_T,
                           (void *)&tmat->ncomp_datatype);

  irc += tfile->WriteAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "ptr_datatype",   TIO_ENUM_T,
                           (void *)&tmat->ptr_datatype);

  irc += tfile->WriteAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "vf_datatype",    TIO_ENUM_T,
                           (void *)&tmat->vf_datatype);

  irc += tfile->WriteAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "nmat",           TIO_SIZE_T,
                           (void *)&tmat->nmat);

  irc += tfile->WriteAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "nghosts",        TIO_SIZE_T,
                           (void *)&tmat->nghosts);

  irc += tfile->WriteAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "mixedflag",      TIO_INT,
                           (void *)&tmat->ismixed);

  TIOassert(irc != 0, ERR_INT, "Failed to write attributes", TIO_ERR_INT);


  tmesh->matghosts = lnghosts;


  *materialID = lmatID;

  TIOend(subname,1);

  return (TIO_SUCCESS);


 cleandie:
  trc = cTIO_HierarchyKillObject(subname, fileID, meshID, lmatID, CLASS_MATERIAL);
  TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to kill object", TIO_ERR_INT);

  if (tmat->matnames != NULL) {
    TIOfree (tmat->matnames); tmat->matnames = NULL;
  }
  if (tmat->matnums  != NULL) {
    TIOfree (tmat->matnums);  tmat->matnums  = NULL;
  }
  TIOfree (tmat); tmat = NULL;
  TIOreturn(iTIO_GetErrClass(), iTIO_GetErrStr(), trc);
}




/***************************************************************************************************
 *
 * TIO_Open_Material
 *
 **************************************************************************************************/
TIO_t
TIO_Open_Material( const TIO_File_t   fileID,
                   const TIO_Object_t meshID,
                   const char         name[],
                   TIO_Object_t       *materialID,
                   TIO_Data_t         *datatype,
                   TIO_Size_t         *nmat,
                   TIO_Size_t         *nghosts,
                   TIO_Bool_t         *ismixed,
                   TIO_Data_t         *ncomp_datatype,
                   TIO_Data_t         *ptr_datatype,
                   TIO_Data_t         *vf_datatype )
{
  const char *subname = "TIO_Open_Material()";

  struct iFile_t     *tfile;
  struct iMesh_t     *tmesh;
  struct iMaterial_t *tmat;

  TIO_Object_t lmatID;
  iDataPos_t   datapos;
  int          irc;
  TIO_t        trc;

  TIObegin(subname,1);

  *materialID = TIO_NULL;    /* Ensure NULL value on failure */


  irc = iTIO_GetObject(fileID, NULL, meshID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid meshID", TIO_ERR_INVALID_OBJECTID);
  }

  tmat = (struct iMaterial_t *)TIOmalloc(sizeof(struct iMaterial_t));
  if (NULL == tmat) {
    TIOreturn(ERR_MEM, "Failed to malloc tmat", TIO_ERR_INT);
  }

  tmat->matnames = NULL;   /* Explicitly nullify so can check in cleandie */
  tmat->matnums  = NULL;


  trc = cTIO_HierarchyOpenObject( subname, fileID, meshID, CLASS_MESH, CLASS_MATERIAL,
                                  name, FALSE, TIO_NULL,
                                  tmat->name, &tfile, &lmatID, (void *)tmat );
  if (trc != TIO_SUCCESS) {
    TIOfree (tmat); tmat = NULL;
    cTIOreturn (trc);
  }


  (void) iTIO_strcpy(tmat->name, name);


  /* -- Read object attributes */
  irc = 0;

  irc += tfile->ReadAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "datatype",       TIO_ENUM_T,
                          (void *)&tmat->datatype);

  irc += tfile->ReadAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "ncomp_datatype", TIO_ENUM_T,
                          (void *)&tmat->ncomp_datatype);

  irc += tfile->ReadAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "ptr_datatype",   TIO_ENUM_T,
                          (void *)&tmat->ptr_datatype);

  irc += tfile->ReadAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "vf_datatype",    TIO_ENUM_T,
                          (void *)&tmat->vf_datatype);

  irc += tfile->ReadAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "nmat",           TIO_SIZE_T,
                          (void *)&tmat->nmat);

  irc += tfile->ReadAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "nghosts",        TIO_SIZE_T,
                          (void *)&tmat->nghosts);

  irc += tfile->ReadAttr( fileID, meshID, lmatID, CLASS_MATERIAL, "mixedflag",      TIO_INT,
                          (void *)&tmat->ismixed);

  TIOassert(irc != 0, ERR_INT, "Failed to read attributes", TIO_ERR_INT);


  /* -- open, read & close the material range datasets */

  tmat->matnames = TIOmalloc(tmat->nmat * sizeof(char) * TIO_STRLEN);
  if (NULL == tmat->matnames) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc mat matnames");
    trc = TIO_ERR_INT;
    goto cleandie;
  }
  tmat->matnums  = TIOmalloc(tmat->nmat * iTIO_SizeOf(tmat->datatype));
  if (NULL == tmat->matnums) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc mat matnums");
    trc = TIO_ERR_INT;
    goto cleandie;
  }

  irc = tfile->OpenDataset(fileID, meshID, lmatID, CLASS_MATERIAL,
                           xMaterialNumsName,  0, NULL, NULL, NULL);
  TIOassert(irc < 0, ERR_INT, "Failed to open matnums dataset", TIO_ERR_INT);

  irc = tfile->OpenDataset(fileID, meshID, lmatID, CLASS_MATERIAL,
                           xMaterialNamesName, 1, NULL, NULL, NULL);
  TIOassert(irc < 0, ERR_INT, "Failed to open matnames dataset", TIO_ERR_INT);

  irc = tfile->ReadDataset(fileID, meshID, lmatID, CLASS_MATERIAL,
                           0, TIO_XFER_COLLECTIVE, tmat->datatype, tmat->matnums);
  TIOassert(irc < 0, ERR_INT, "Failed to read matnums dataset", TIO_ERR_INT);

  irc = tfile->ReadDataset(fileID, meshID, lmatID, CLASS_MATERIAL,
                           1, TIO_XFER_COLLECTIVE, TIO_STRING, tmat->matnames);
  TIOassert(irc < 0, ERR_INT, "Failed to read matnames dataset", TIO_ERR_INT);

  irc = tfile->CloseDataset(fileID, meshID, lmatID, CLASS_MATERIAL, 0);
  TIOassert(irc < 0, ERR_INT, "Failed to close matnums dataset", TIO_ERR_INT);

  irc = tfile->CloseDataset(fileID, meshID, lmatID, CLASS_MATERIAL, 1);
  TIOassert(irc < 0, ERR_INT, "Failed to close matnames dataset", TIO_ERR_INT);


  tmat->status   = STATUS_OLD;
  tmat->rangeset = TRUE;
  tmat->namesset = TRUE;


  /* Open material datasets here */

  if (tmat->ismixed) {

    irc = tfile->OpenChunkedDataset( fileID, meshID, lmatID, CLASS_MATERIAL,
                                     xMaterialDataName, MAT_MATERIAL,
                                     tmat->datatype, tmesh->meshtype, tmesh->ndims,
                                     tmat->nghosts, tmesh->nchunks, tmesh->chunkinfo,
                                     &tmesh->info, DATAPOS_MIXED );
                                                        /* = PURE + appended MIX */
    if (irc < 0) {
      (void) iTIO_PutErr(ERR_INT, "Failed to open material dataset");
      trc = TIO_ERR_OBJECT_OPEN;
      goto cleandie;
    }

    irc = tfile->OpenChunkedDataset( fileID, meshID, lmatID, CLASS_MATERIAL,
                                     xMaterialNcompName, MAT_NCOMP,
                                     tmat->ncomp_datatype, tmesh->meshtype, tmesh->ndims,
                                     tmat->nghosts, tmesh->nchunks, tmesh->chunkinfo,
                                     &tmesh->info, DATAPOS_MIXCELL );
    if (irc < 0) {
      (void) iTIO_PutErr(ERR_INT, "Failed to open ncomp dataset");
      trc = TIO_ERR_OBJECT_OPEN;
      goto cleandie;
    }

    irc = tfile->OpenChunkedDataset( fileID, meshID, lmatID, CLASS_MATERIAL,
                                     xMaterialPtrName, MAT_PTR,
                                     tmat->ptr_datatype, tmesh->meshtype, tmesh->ndims,
                                     tmat->nghosts, tmesh->nchunks, tmesh->chunkinfo,
                                     &tmesh->info, DATAPOS_MIXCELL );
    if (irc < 0) {
      (void) iTIO_PutErr(ERR_INT, "Failed to open ptr dataset");
      trc = TIO_ERR_OBJECT_OPEN;
      goto cleandie;
    }

    irc = tfile->OpenChunkedDataset( fileID, meshID, lmatID, CLASS_MATERIAL,
                                     xMaterialVolfracName, MAT_VOLFRAC,
                                     tmat->vf_datatype, tmesh->meshtype, tmesh->ndims,
                                     tmat->nghosts, tmesh->nchunks, tmesh->chunkinfo,
                                     &tmesh->info, DATAPOS_MIXCOMP );
    if (irc < 0) {
      (void) iTIO_PutErr(ERR_INT, "Failed to open volfrac dataset");
      trc = TIO_ERR_OBJECT_OPEN;
      goto cleandie;
    }

  }
  else {

    switch (tmesh->meshtype) {
    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR:
    case TIO_MESH_UNSTRUCT:
      datapos = DATAPOS_CELL;
      break;
    case TIO_MESH_POINT:
      datapos = DATAPOS_NODE;
      break;
    default:
      TIOassert(TRUE, ERR_INT, "Invalid value for tmesh->meshtype", TIO_ERR_INT);
      break;
    }


    irc = tfile->OpenChunkedDataset( fileID, meshID, lmatID, CLASS_MATERIAL,
                                     xMaterialDataName, MAT_MATERIAL,
                                     tmat->datatype, tmesh->meshtype, tmesh->ndims,
                                     tmat->nghosts, tmesh->nchunks, tmesh->chunkinfo,
                                     &tmesh->info, datapos );
    if (irc < 0) {
      (void) iTIO_PutErr(ERR_INT, "Failed to open material dataset");
      trc = TIO_ERR_OBJECT_OPEN;
      goto cleandie;
    }
  }


  irc = xTIO_GetMaterialInfo(subname, TRUE, tmat, NULL, datatype, nmat, nghosts,
                             ismixed, ncomp_datatype, ptr_datatype, vf_datatype);
  TIOassert(irc < 0, ERR_INT, "Failed to get material info", TIO_ERR_INT);


  *materialID = lmatID;

  TIOend(subname,1);

  return (TIO_SUCCESS);


 cleandie:
  trc = cTIO_HierarchyKillObject(subname, fileID, meshID, lmatID, CLASS_MATERIAL);
  TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to kill object", TIO_ERR_INT);

  if (tmat->matnames != NULL) {
    TIOfree (tmat->matnames); tmat->matnames = NULL;
  }
  if (tmat->matnums  != NULL) {
    TIOfree (tmat->matnums);  tmat->matnums  = NULL;
  }
  TIOfree (tmat); tmat = NULL;
  TIOreturn( iTIO_GetErrClass(), iTIO_GetErrStr(), trc );
}




/***************************************************************************************************
 *
 * TIO_Get_Material_Info
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Material_Info( const TIO_File_t   fileID,
                       const TIO_Object_t materialID,
                       char               name[],
                       TIO_Data_t         *datatype,
                       TIO_Size_t         *nmat,
                       TIO_Size_t         *nghosts,
                       TIO_Bool_t         *ismixed,
                       TIO_Data_t         *ncomp_datatype,
                       TIO_Data_t         *ptr_datatype,
                       TIO_Data_t         *vf_datatype )
{
  const char *subname = "TIO_Get_Material_Info()";

  struct iMaterial_t *tmat;

  int   irc;
  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, materialID, CLASS_MATERIAL,
                                     NULL, (void **)&tmat);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }
  irc = xTIO_GetMaterialInfo(subname, TRUE, tmat, name, datatype, nmat, nghosts, ismixed,
                             ncomp_datatype, ptr_datatype, vf_datatype);
  TIOassert(irc < 0, ERR_INT, "Failed to get material info", TIO_ERR_INT);

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Close_Material
 *
 **************************************************************************************************/
TIO_t
TIO_Close_Material( const TIO_File_t   fileID,
                    const TIO_Object_t materialID )
{
  const char *subname = "TIO_Close_Material()";     /* Function name for debugging and error */

  struct iFile_t     *tfile;
  struct iMaterial_t *tmat;

  TIO_Object_t lpID;     /* parentID */
  TIO_Size_t   dsize[1];
  int          irc;
  TIO_t        trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyPreClose( subname, fileID, &lpID, materialID, CLASS_MATERIAL,
                                &tfile, (void **)&tmat );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }
  irc = iTIO_GetFile(fileID, &tfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid fileID", TIO_ERR_INVALID_FILEID);
  }

  /* Close material datasets */

  /* Possible that none were written (you never know!) so no need to close if so */


  if (tmat->status != STATUS_NEW) {

    irc = tfile->CloseChunkedDataset(fileID, lpID, materialID, CLASS_MATERIAL, MAT_MATERIAL);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to close material dataset", TIO_ERR_OBJECT_CLOSE);
    }
    if (tmat->ismixed) {

      irc = tfile->CloseChunkedDataset(fileID, lpID, materialID, CLASS_MATERIAL, MAT_NCOMP);
      if (irc < 0) {
        TIOreturn(ERR_INT, "Failed to close material ncomp dataset", TIO_ERR_OBJECT_CLOSE);
      }
      irc = tfile->CloseChunkedDataset(fileID, lpID, materialID, CLASS_MATERIAL, MAT_PTR);
      if (irc < 0) {
        TIOreturn(ERR_INT, "Failed to close material ptr dataset", TIO_ERR_OBJECT_CLOSE);
      }
      irc = tfile->CloseChunkedDataset(fileID, lpID, materialID, CLASS_MATERIAL, MAT_VOLFRAC);
      if (irc < 0) {
        TIOreturn(ERR_INT, "Failed to close material volfrac dataset", TIO_ERR_OBJECT_CLOSE);
      }
    }

  }



  if (tmat->status != STATUS_OLD) {

    if (!tmat->rangeset) {

      /* If no range set, use default range of 1,2,...,nmat */

      int *matnums;
      TIO_Size_t ii;

      matnums = (int *)TIOmalloc(sizeof(int) * tmat->nmat);
      if (NULL == matnums) {
        TIOreturn(ERR_MEM, "Failed to malloc matnums", TIO_ERR_INT);
      }
      for (ii=0; ii<tmat->nmat; ++ii) {
        matnums[ii] = ii+1;
      }

      trc = TIO_Set_Material_Range(fileID, materialID, TIO_INT, tmat->nmat, matnums, NULL);
      TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to set material range", TIO_ERR_INT);

      TIOfree (matnums); matnums = NULL;
    }

    if (!tmat->namesset) {

      char (*matnames)[TIO_STRLEN];
      TIO_Size_t ii;

      matnames = TIOmalloc(sizeof(char) * TIO_STRLEN * tmat->nmat);
      if (NULL == matnames) {
        TIOreturn(ERR_MEM, "Failed to malloc matnames", TIO_ERR_INT);
      }
      for (ii=0; ii<tmat->nmat; ++ii) {
        snprintf(matnames[ii], sizeof(matnames[ii]), "material%ld", (long int)(ii+1));
      }
      trc = TIO_Set_Material_Range(fileID, materialID, TIO_DATATYPE_NULL,
                                   tmat->nmat, NULL, matnames);
      TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to set material range", TIO_ERR_INT);

      TIOfree (matnames); matnames = NULL;
    }


    /* Write material matnames and matnums datasets - is safe to re-use ndsets */

    dsize[0] = tmat->nmat;

    irc = tfile->CreateDataset(fileID, lpID, materialID, CLASS_MATERIAL,
                               xMaterialNumsName,  0, TIO_1D, dsize, tmat->datatype);
    TIOassert(irc < 0, ERR_INT, "Failed to create material range dataset - nums", TIO_ERR_INT);

    irc = tfile->CreateDataset(fileID, lpID, materialID, CLASS_MATERIAL,
                               xMaterialNamesName, 1, TIO_1D, dsize, TIO_STRING);
    TIOassert(irc < 0, ERR_INT, "Failed to create material range dataset - names", TIO_ERR_INT);

    if (0 == tfile->rank) {
      irc = tfile->WriteDataset(fileID, lpID, materialID, CLASS_MATERIAL,
                                0, TIO_XFER_INDEPENDENT, tmat->datatype, tmat->matnums);
      TIOassert(irc < 0, ERR_INT, "Failed to write material range dataset - nums", TIO_ERR_INT);

      irc = tfile->WriteDataset(fileID, lpID, materialID, CLASS_MATERIAL,
                                1, TIO_XFER_INDEPENDENT, TIO_STRING, tmat->matnames);
      TIOassert(irc < 0, ERR_INT, "Failed to write material range dataset - names", TIO_ERR_INT);
    }

    irc = tfile->CloseDataset(fileID, lpID, materialID, CLASS_MATERIAL, 0);
    TIOassert(irc < 0, ERR_INT, "Failed to close material range dataset - nums", TIO_ERR_INT);

    irc = tfile->CloseDataset(fileID, lpID, materialID, CLASS_MATERIAL, 1);
    TIOassert(irc < 0, ERR_INT, "Failed to close material range dataset - names", TIO_ERR_INT);

  }

  TIOfree (tmat->matnames);  tmat->matnames = NULL;
  TIOfree (tmat->matnums);   tmat->matnums  = NULL;
  TIOfree (tmat);            tmat           = NULL;

  trc = cTIO_HierarchyCloseObject( subname, fileID, lpID, materialID, CLASS_MATERIAL );

  TIOend(subname,1);


  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Set_Material_Range
 *
 * - can be called any time between create_material and close_material
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Material_Range( const TIO_File_t   fileID,
                        const TIO_Object_t materialID,
                        const TIO_Data_t   datatype,
                        const TIO_Size_t   nmat,
                        const void         *matnums,
                        char               matnames[][TIO_STRLEN] )
{
  const char *subname = "TIO_Set_Material_Range()";

  struct iFile_t     *tfile;
  struct iMaterial_t *tmat;

  size_t dsi;

  int    irc;
  TIO_t  trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, materialID, CLASS_MATERIAL,
                                     &tfile, (void **)&tmat);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  /* -- Check args */

  if (nmat != tmat->nmat) {
    TIOreturn(ERR_USER, "Invalid argument (nmat) - does not match that given at material creation",
              TIO_ERR_INVALID_ARG);
  }


  /* Copy material nums into cache - tmat->matnums already allocated in create routine */
  if (matnums != NULL) {

    if (tmat->rangeset) {
      TIOreturn(ERR_USER, "Material nums has already been set", TIO_ERR_INVALID_OPERATION);
    }

    dsi = iTIO_SizeOf(datatype);
    if (0 == dsi) {
      TIOreturn(ERR_USER, "Invalid argument (datatype)", TIO_ERR_INVALID_DATATYPE);
    }

    if (datatype != tmat->datatype) {

      size_t dsf;
      void   *buf;

      dsf = iTIO_SizeOf(tmat->datatype);

      buf = TIOmalloc( TIOmax(dsi, dsf) * nmat );

      (void) memcpy(buf, matnums, (nmat * dsi));

      irc = tfile->ConvBuf(datatype, tmat->datatype, nmat, buf);
      if (irc < 0) {
        TIOfree (buf); buf = NULL;
        TIOreturn(ERR_USER, "Unable to convert between given datatype and that of material",
                  TIO_ERR_INVALID_CONVERSION);
      }
      (void) memcpy(tmat->matnums, buf, (nmat * dsf));

      TIOfree (buf); buf = NULL;

    }
    else {
      (void) memcpy(tmat->matnums, matnums, (nmat * dsi));
    }
    tmat->rangeset = TRUE;
  }


  /* Copy material names into cache - tmat->matnames already allocated in create routine */
  if (matnames != NULL) {

    if (tmat->namesset) {
      TIOreturn(ERR_USER, "Material names has already been set", TIO_ERR_INVALID_OPERATION);
    }
    (void) memcpy(tmat->matnames, matnames, (nmat * TIO_STRLEN));

    tmat->namesset = TRUE;
  }

  /* Postpone writing until collective material close call */


  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_Material_Range
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Material_Range( const TIO_File_t   fileID,
                        const TIO_Object_t materialID,
                        const TIO_Data_t   datatype,
                        TIO_Size_t         *nmat,
                        void               *matnums,
                        char               matnames[][TIO_STRLEN] )
{
  const char *subname = "TIO_Get_Material_Range()";

  struct iFile_t     *tfile;
  struct iMaterial_t *tmat;

  size_t dsi;

  int    irc;
  TIO_t  trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, materialID, CLASS_MATERIAL,
                                     &tfile, (void **)&tmat);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (nmat != NULL) {
    *nmat = tmat->nmat;
  }

  /* Copy material nums from cache */
  if (matnums != NULL) {

    dsi = iTIO_SizeOf(datatype);
    if (0 == dsi) {
      TIOreturn(ERR_USER, "Invalid argument (datatype)", TIO_ERR_INVALID_DATATYPE);
    }

    if (datatype != tmat->datatype) {

      size_t dsf;
      void   *buf;

      dsf = iTIO_SizeOf(tmat->datatype);

      buf = TIOmalloc( TIOmax(dsi, dsf) * tmat->nmat );

      (void) memcpy(buf, tmat->matnums, (tmat->nmat * dsf));

      irc = tfile->ConvBuf(tmat->datatype, datatype, tmat->nmat, buf);
      if (irc < 0) {
        TIOfree (buf); buf = NULL;
        TIOreturn(ERR_USER, "Unable to convert between given datatype and that of material",
                  TIO_ERR_INVALID_CONVERSION);
      }
      (void) memcpy(matnums, buf, (tmat->nmat * dsi));

      TIOfree (buf); buf = NULL;

    }
    else {
      (void) memcpy(matnums, tmat->matnums, (tmat->nmat * dsi));
    }
  }


  /* Copy material names into cache */
  if (matnames != NULL) {
    (void) memcpy(matnames, tmat->matnames, (tmat->nmat * TIO_STRLEN));
  }


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




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
 **************************************************************************************************/
TIO_t
TIO_Write_QuadMaterial_Chunk( const TIO_File_t   fileID,
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
                              const void         *vf )
{
  const char *subname = "TIO_Write_QuadMaterial_Chunk()";

  TIO_t trc;

  TIObegin(subname,1);

  trc = cTIO_WriteMaterialChunk( subname, TIO_MESH_QUAD_NONCOLINEAR,
                                 fileID, materialID, idx, xfer, datatype, mat,
                                 ncomp_datatype, ptr_datatype, vf_datatype,
                                 ncomp, ptr, mixmat, vf);
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_QuadMaterial_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadMaterial_Chunk( const TIO_File_t   fileID,
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
                             void               *vf )
{
  const char *subname = "TIO_Read_QuadMaterial_Chunk()";

  TIO_Size_t nn[3];
  TIO_t      trc;


  TIObegin(subname,1);

  nn[0] = 0;
  nn[1] = 0;
  nn[2] = 0;

  trc = cTIO_ReadMaterialChunk( subname, TIO_MESH_QUAD_NONCOLINEAR,
                                fileID, materialID, idx, xfer, datatype, nghosts,
                                ndims, nn, mat,
                                ncomp_datatype, ptr_datatype, vf_datatype,
                                nmixcell, nmixcomp,
                                ncomp, ptr, mixmat, vf );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  TIOset(ni, nn[0]);
  TIOset(nj, nn[1]);
  TIOset(nk, nn[2]);

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_QuadMaterial_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadMaterial_All( const TIO_File_t   fileID,
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
                           void               *vf )
{
  const char *subname = "TIO_Read_QuadMaterial_All()";

  TIO_Size_t nn[3];
  TIO_t      trc;


  TIObegin(subname,1);

  nn[0] = 0;
  nn[1] = 0;
  nn[2] = 0;

  trc = cTIO_ReadMaterialAll( subname, TIO_MESH_QUAD_NONCOLINEAR,
                              fileID, materialID, datatype,
                              ndims, nn, mat,
                              ncomp_datatype, ptr_datatype, vf_datatype,
                              nmixcell, nmixcomp,
                              ncomp, ptr, mixmat, vf);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  TIOset(ni, nn[0]);
  TIOset(nj, nn[1]);
  TIOset(nk, nn[2]);

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_QuadMaterial_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadMaterial_Section( const TIO_File_t   fileID,
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
                               void               *vf )
{
  const char *subname = "TIO_Read_QuadMaterial_Section()";

  TIO_Size_t nn[3];
  TIO_t      trc;


  TIObegin(subname,1);

  nn[0] = 0;
  nn[1] = 0;
  nn[2] = 0;

  trc = cTIO_ReadMaterialSection( subname, TIO_MESH_QUAD_NONCOLINEAR,
                                  fileID, materialID, sectionID, xfer, datatype,
                                  ndims, nn, mat,
                                  ncomp_datatype, ptr_datatype, vf_datatype,
                                  nmixcell, nmixcomp,
                                  ncomp, ptr, mixmat, vf );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  TIOset(ni, nn[0]);
  TIOset(nj, nn[1]);
  TIOset(nk, nn[2]);

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




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
 **************************************************************************************************/
TIO_t
TIO_Write_UnstrMaterial_Chunk( const TIO_File_t   fileID,
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
                               const void         *vf )
{
  const char *subname = "TIO_Write_UnstrMaterial_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_WriteMaterialChunk( subname, TIO_MESH_UNSTRUCT,
                                 fileID, materialID, idx, xfer, datatype, mat,
                                 ncomp_datatype, ptr_datatype, vf_datatype,
                                 ncomp, ptr, mixmat, vf );
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_UnstrMaterial_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_UnstrMaterial_Chunk( const TIO_File_t   fileID,
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
                              void               *vf  )
{
  const char *subname = "TIO_Read_UnstrMaterial_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadMaterialChunk( subname, TIO_MESH_UNSTRUCT,
                                fileID, materialID, idx, xfer, datatype, nghosts,
                                NULL, ncells, mat,
                                ncomp_datatype, ptr_datatype, vf_datatype,
                                nmixcell, nmixcomp,
                                ncomp, ptr, mixmat, vf );
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_UnstrMaterial_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_UnstrMaterial_All( const TIO_File_t   fileID,
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
                            void               *vf )
{
  const char *subname = "TIO_Read_UnstrMaterial_All()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadMaterialAll( subname, TIO_MESH_UNSTRUCT,
                              fileID, materialID, datatype,
                              NULL, ncells, mat,
                              ncomp_datatype, ptr_datatype, vf_datatype,
                              nmixcell, nmixcomp,
                              ncomp, ptr, mixmat, vf );
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_UnstrMaterial_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_UnstrMaterial_Section( const TIO_File_t   fileID,
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
                                void               *vf )
{
  const char *subname = "TIO_Read_UnstrMaterial_Section()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadMaterialSection( subname, TIO_MESH_UNSTRUCT,
                                  fileID, materialID, sectionID, xfer, datatype,
                                  NULL, ncells, mat,
                                  ncomp_datatype, ptr_datatype, vf_datatype,
                                  nmixcell, nmixcomp,
                                  ncomp, ptr, mixmat, vf );
  TIOend(subname,1);

  return (trc);
}




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
 **************************************************************************************************/
TIO_t
TIO_Write_PointMaterial_Chunk( const TIO_File_t   fileID,
                               const TIO_Object_t materialID,
                               const TIO_Size_t   idx,
                               const TIO_Xfer_t   xfer,
                               const TIO_Data_t   datatype,
                               const void         *mat )
{
  const char *subname = "TIO_Write_PointMaterial_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_WriteMaterialChunk( subname, TIO_MESH_POINT,
                                 fileID, materialID, idx, xfer, datatype, mat,
                                 TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, TIO_DATATYPE_NULL,
                                 NULL, NULL, NULL, NULL );
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_PointMaterial_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointMaterial_Chunk( const TIO_File_t   fileID,
                              const TIO_Object_t materialID,
                              const TIO_Size_t   idx,
                              const TIO_Xfer_t   xfer,
                              const TIO_Data_t   datatype,
                              const TIO_Size_t   nghosts,
                              TIO_Size_t         *nnodes,
                              void               *mat )
{
  const char *subname = "TIO_Read_PointMaterial_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadMaterialChunk( subname, TIO_MESH_POINT,
                                fileID, materialID, idx, xfer, datatype, nghosts,
                                NULL, nnodes, mat,
                                TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, TIO_DATATYPE_NULL,
                                NULL, NULL,
                                NULL, NULL, NULL, NULL );
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_PointMaterial_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointMaterial_All( const TIO_File_t   fileID,
                            const TIO_Object_t materialID,
                            const TIO_Data_t   datatype,
                            TIO_Size_t         *nnodes,
                            void               *mat )
{
  const char *subname = "TIO_Read_PointMaterial_All()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadMaterialAll( subname, TIO_MESH_POINT,
                              fileID, materialID, datatype,
                              NULL, nnodes, mat,
                              TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, TIO_DATATYPE_NULL,
                              NULL, NULL,
                              NULL, NULL, NULL, NULL );
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_PointMaterial_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointMaterial_Section( const TIO_File_t   fileID,
                                const TIO_Object_t materialID,
                                const TIO_Object_t sectionID,
                                const TIO_Xfer_t   xfer,
                                const TIO_Data_t   datatype,
                                TIO_Size_t         *nnodes,
                                void               *mat )
{
  const char *subname = "TIO_Read_PointMaterial_Section()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadMaterialSection( subname, TIO_MESH_POINT,
                                  fileID, materialID, sectionID, xfer, datatype,
                                  NULL, nnodes, mat,
                                  TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, TIO_DATATYPE_NULL,
                                  NULL, NULL, NULL, NULL, NULL, NULL );
  TIOend(subname,1);

  return (trc);
}




/**************************************
 **************************************
 **
 **   ALL MATERIALS
 **
 **************************************
 **************************************/


/***************************************************************************************************
 *
 * cTIO_WriteMaterialChunk
 *
 **************************************************************************************************/
static TIO_t
cTIO_WriteMaterialChunk( const char         subname[],
                         const TIO_Mesh_t   subtype,
                         const TIO_File_t   fileID,
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
                         const void         *vf )
{
  struct iFile_t     *tfile;
  struct iMesh_t     *tmesh;
  struct iMaterial_t *tmat;

  TIO_Object_t lpID;     /* parent ID */
  iDataPos_t   datapos;

  int   irc;
  TIO_t trc;



  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, materialID, CLASS_MATERIAL,
                                     &tfile, (void **)&tmat);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = iTIO_GetObject(fileID, NULL, lpID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get parent mesh", TIO_ERR_INT);
  }

  if (tmesh->meshtype != subtype) {
    if ((TIO_MESH_QUAD_NONCOLINEAR == subtype) && (TIO_MESH_QUAD_COLINEAR == tmesh->meshtype)) {
      ; /* ok */
    }
    else {
      TIOreturn(ERR_USER, "Material is not of correct mesh type - cannot write chunk",
                TIO_ERR_INVALID_OPERATION);
    }
  }


  if (tmesh->nchunks_unset > 0) {
    TIOreturn(ERR_USER, "Not all parent mesh chunks have been set", TIO_ERR_OBJECT_WRITE);
  }
  if ((tmat->status != STATUS_CREATED) && (tmat->status != STATUS_WRITTEN)) {
    TIOreturn(ERR_USER, "Cannot write to pre-existing material", TIO_ERR_OBJECT_NOT_WRITEABLE);
  }

  trc = cTIO_CheckDatatype(subname, "datatype", FALSE, datatype, TIO_TYPECLASS_SIGNED_INT);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if ((NULL == mat) && (TIO_XFER_COLLECTIVE_RR != xfer)) {
    TIOreturn(ERR_USER, "Material must have pure material data specified", TIO_ERR_INVALID_ARG);
  }

  if (TIO_MESH_POINT == subtype) {
    datapos = DATAPOS_NODE;
  }
  else {
    datapos = DATAPOS_CELL;
  }

  irc = tfile->WriteChunkedDataset( fileID, lpID, materialID,
                                    CLASS_MATERIAL, MAT_MATERIAL,
                                    xfer, datatype, tmesh->meshtype, tmesh->ndims,
                                    tmat->nghosts, tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                    datapos, idx, mat );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to write material chunk data", TIO_ERR_INT);
  }

  tmat->status = STATUS_WRITTEN;


  if (tmat->ismixed) {

    trc = cTIO_CheckDatatype(subname, "ncomp_datatype", (NULL == ncomp),
                             ncomp_datatype, TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "ptr_datatype",   (NULL == ptr),
                             ptr_datatype, TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "vf_datatype",    (NULL == vf),
                             vf_datatype, TIO_TYPECLASS_FLOAT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }

    irc = tfile->WriteChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_MATERIAL,
                                      xfer, datatype, tmesh->meshtype, tmesh->ndims,
                                      tmat->nghosts, tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo,
                                      DATAPOS_MIXED, idx, mixmat );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write material chunk mixmat data", TIO_ERR_INT);
    }
    irc = tfile->WriteChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_NCOMP,
                                      xfer, ncomp_datatype, tmesh->meshtype, tmesh->ndims,
                                      tmat->nghosts, tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                      DATAPOS_MIXCELL, idx, ncomp );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write material ncomp chunk data", TIO_ERR_INT);
    }
    irc = tfile->WriteChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_PTR,
                                      xfer, ptr_datatype, tmesh->meshtype, tmesh->ndims,
                                      tmat->nghosts, tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                      DATAPOS_MIXCELL, idx, ptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write material ptr data", TIO_ERR_INT);
    }
    irc = tfile->WriteChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_VOLFRAC,
                                      xfer, vf_datatype, tmesh->meshtype, tmesh->ndims,
                                      tmat->nghosts, tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                      DATAPOS_MIXCOMP, idx, vf );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write material volfrac chunk data", TIO_ERR_INT);
    }
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_ReadMaterialChunk
 *
 **************************************************************************************************/
static TIO_t
cTIO_ReadMaterialChunk( const char         subname[],
                        const TIO_Mesh_t   subtype,
                        const TIO_File_t   fileID,
                        const TIO_Object_t materialID,
                        const TIO_Size_t   idx,
                        const TIO_Xfer_t   xfer,
                        const TIO_Data_t   datatype,
                        const TIO_Size_t   nghosts,
                        TIO_Dims_t         *ndims,
                        TIO_Size_t         *nn,
                        void               *mat,
                        const TIO_Data_t   ncomp_datatype,
                        const TIO_Data_t   ptr_datatype,
                        const TIO_Data_t   vf_datatype,
                        TIO_Size_t         *nmixcell,
                        TIO_Size_t         *nmixcomp,
                        void               *ncomp,
                        void               *ptr,
                        void               *mixmat,
                        void               *vf  )
{
  struct iFile_t     *tfile;
  struct iMesh_t     *tmesh;
  struct iMaterial_t *tmat;

  TIO_Object_t lpID;     /* parent ID */
  TIO_Size_t   lnghosts;
  iDataPos_t   datapos;

  int   irc;
  TIO_t trc;



  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, materialID, CLASS_MATERIAL,
                                     &tfile, (void **)&tmat);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }
  irc = iTIO_GetObject(fileID, NULL, lpID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get parent mesh", TIO_ERR_INT);
  }

  if (tmesh->meshtype != subtype) {
    if ((TIO_MESH_QUAD_NONCOLINEAR == subtype) && (TIO_MESH_QUAD_COLINEAR == tmesh->meshtype)) {
      ; /* ok */
    }
    else {
      TIOreturn(ERR_USER, "Material is not of correct mesh type - cannot read chunk",
                TIO_ERR_INVALID_OPERATION);
    }
  }

  if (tmat->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written material", TIO_ERR_OBJECT_NOT_READABLE);
  }

  trc = cTIO_CheckDatatype(subname, "datatype", ((NULL == mat) && (NULL == mixmat)),
                           datatype, TIO_TYPECLASS_SIGNED_INT);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  trc = cTIO_CheckGhosts(subname, fileID, lpID, tmesh, nghosts, tmat->nghosts, &lnghosts, TRUE);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  switch (subtype) {
  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:
    TIOset(ndims, tmesh->ndims);
    datapos = DATAPOS_CELL;
    break;
  case TIO_MESH_UNSTRUCT:
    /* ndims is NULL for unstruct mesh */
    datapos  = DATAPOS_CELL;
    break;
  case TIO_MESH_POINT:
    /* ndims is NULL for point mesh */
    datapos  = DATAPOS_NODE;
    break;
  default:
    TIOreturn(ERR_INT, "Invalid subtype", TIO_ERR_INT);
    break;
  }


  irc = tfile->ReadChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_MATERIAL,
                                   xfer, datatype, subtype, tmesh->ndims, lnghosts,
                                   tmesh->nchunks, tmesh->chunkinfo, 
                                   datapos, idx, nn, mat );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to read material chunk data", TIO_ERR_INT);
  }

  if (tmat->ismixed) {

    trc = cTIO_CheckDatatype(subname, "ncomp_datatype", (NULL == ncomp),
                             ncomp_datatype, TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "ptr_datatype",   (NULL == ptr),
                             ptr_datatype, TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "vf_datatype",    (NULL == vf),
                             vf_datatype, TIO_TYPECLASS_FLOAT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }

    irc = tfile->ReadChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_MATERIAL,
                                     xfer, datatype, subtype, tmesh->ndims, lnghosts,
                                     tmesh->nchunks, tmesh->chunkinfo,
                                     DATAPOS_MIXED, idx, nmixcomp, mixmat );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material chunk mixmat data", TIO_ERR_INT);
    }
    irc = tfile->ReadChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_NCOMP,
                                     xfer, ncomp_datatype, subtype, tmesh->ndims, lnghosts,
                                     tmesh->nchunks, tmesh->chunkinfo,
                                     DATAPOS_MIXCELL, idx, nmixcell, ncomp );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material chunk ncomp data", TIO_ERR_INT);
    }
    irc = tfile->ReadChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_PTR,
                                     xfer, ptr_datatype, subtype, tmesh->ndims, lnghosts,
                                     tmesh->nchunks, tmesh->chunkinfo,
                                     DATAPOS_MIXCELL, idx, NULL, ptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material chunk ptr data", TIO_ERR_INT);
    }
    irc = tfile->ReadChunkedDataset( fileID, lpID, materialID, CLASS_MATERIAL, MAT_VOLFRAC,
                                     xfer, vf_datatype, subtype, tmesh->ndims, lnghosts,
                                     tmesh->nchunks, tmesh->chunkinfo, 
                                     DATAPOS_MIXCOMP, idx, NULL, vf );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material chunk volfrac data", TIO_ERR_INT);
    }
  }
  else {
    if (nmixcell != NULL) {
      *nmixcell = 0;
    }
    if (nmixcomp != NULL) {
      *nmixcomp = 0;
    }
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_ReadMaterialAll
 *
 **************************************************************************************************/
static TIO_t
cTIO_ReadMaterialAll( const char         subname[],
                      const TIO_Mesh_t   subtype,
                      const TIO_File_t   fileID,
                      const TIO_Object_t materialID,
                      const TIO_Data_t   datatype,
                      TIO_Dims_t         *ndims,
                      TIO_Size_t         *nn,
                      void               *mat,
                      const TIO_Data_t   ncomp_datatype,
                      const TIO_Data_t   ptr_datatype,
                      const TIO_Data_t   vf_datatype,
                      TIO_Size_t         *nmixcell,
                      TIO_Size_t         *nmixcomp,
                      void               *ncomp,
                      void               *ptr,
                      void               *mixmat,
                      void               *vf )
{
  struct iFile_t     *tfile;
  struct iMesh_t     *tmesh;
  struct iMaterial_t *tmat;

  TIO_Object_t lpID;     /* parent ID */
  iDataPos_t   datapos;

  int   irc;
  TIO_t trc;



  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, materialID, CLASS_MATERIAL,
                                     &tfile, (void **)&tmat);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = iTIO_GetObject(fileID, NULL, lpID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get parent mesh", TIO_ERR_INT);
  }

  if (tmesh->meshtype != subtype) {
    if ((TIO_MESH_QUAD_NONCOLINEAR == subtype) && (TIO_MESH_QUAD_COLINEAR == tmesh->meshtype)) {
      ; /* ok */
    }
    else {
      TIOreturn(ERR_USER, "Material is not of correct mesh type - cannot read all",
                TIO_ERR_INVALID_OPERATION);
    }
  }

  if (tmat->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written material", TIO_ERR_OBJECT_NOT_READABLE);
  }

  trc = cTIO_CheckDatatype(subname, "datatype", ((NULL == mat) && (NULL == mixmat)),
                           datatype, TIO_TYPECLASS_SIGNED_INT);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }


  switch (subtype) {
  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:
    TIOset(ndims, tmesh->ndims);
    datapos = DATAPOS_CELL;
    break;
  case TIO_MESH_UNSTRUCT:
    datapos = DATAPOS_CELL;
    break;
  case TIO_MESH_POINT:
    datapos = DATAPOS_NODE;
    break;
  default:
    TIOreturn(ERR_INT, "Invalid subtype", TIO_ERR_INT);
  }


  irc = tfile->ReadChunkedDatasetAll( fileID, lpID, materialID, CLASS_MATERIAL, MAT_MATERIAL,
                                      TIO_XFER_INDEPENDENT, datatype, subtype, tmesh->ndims,
                                      tmesh->nchunks, tmesh->chunkinfo, &tmesh->info, 
                                      datapos, nn, mat );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to read material all data", TIO_ERR_OBJECT_READ);
  }

  if (tmat->ismixed) {

    trc = cTIO_CheckDatatype(subname, "ncomp_datatype", (NULL == ncomp),
                             ncomp_datatype, TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "ptr_datatype",   (NULL == ptr),
                             ptr_datatype, TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "vf_datatype",    (NULL == vf),
                             vf_datatype, TIO_TYPECLASS_FLOAT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }

    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, materialID, CLASS_MATERIAL, MAT_MATERIAL,
                                        TIO_XFER_INDEPENDENT, datatype, subtype, tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info,
                                        DATAPOS_MIXED, nmixcomp, mixmat );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material all mixmat data", TIO_ERR_OBJECT_READ);
    }
    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, materialID, CLASS_MATERIAL, MAT_NCOMP,
                                        TIO_XFER_INDEPENDENT, ncomp_datatype, subtype, tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info,
                                        DATAPOS_MIXCELL, nmixcell, ncomp );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material all ncomp data", TIO_ERR_OBJECT_READ);
    }
    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, materialID, CLASS_MATERIAL, MAT_PTR,
                                        TIO_XFER_INDEPENDENT, ptr_datatype, subtype, tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info,
                                        DATAPOS_MIXCELL, NULL, ptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material all ptr data", TIO_ERR_OBJECT_READ);
    }
    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, materialID, CLASS_MATERIAL, MAT_VOLFRAC,
                                        TIO_XFER_INDEPENDENT, vf_datatype, subtype, tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info,
                                        DATAPOS_MIXCOMP, NULL, vf );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material all volfrac data", TIO_ERR_OBJECT_READ);
    }
  }
  else {
    if (nmixcell != NULL) {
      *nmixcell = 0;
    }
    if (nmixcomp != NULL) {
      *nmixcomp = 0;
    }
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_ReadMaterialSection
 *
 **************************************************************************************************/
static TIO_t
cTIO_ReadMaterialSection( const char             subname[],
                          const TIO_Mesh_t       subtype,
                          const TIO_File_t       fileID,
                          const TIO_Object_t     materialID,
                          const TIO_Object_t     sectionID,
                          const TIO_Xfer_t       xfer,
                          const TIO_Data_t       datatype,
                          TIO_Dims_t             *ndims,
                          TIO_Size_t             *nn,
                          void                   *mat,
                          const TIO_Data_t       ncomp_datatype,
                          const TIO_Data_t       ptr_datatype,
                          const TIO_Data_t       vf_datatype,
                          TIO_Size_t             *nmixcell,
                          TIO_Size_t             *nmixcomp,
                          void                   *ncomp,
                          void                   *ptr,
                          void                   *mixmat,
                          void                   *vf )
{
  struct iFile_t     *tfile;
  struct iMesh_t     *tmesh;
  union  iSection_t  *tsection;
  struct iMaterial_t *tmat;

  TIO_Object_t lpID;     /* parent ID */
  iDataPos_t datapos;

  int   irc;
  TIO_t trc;



  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, materialID, CLASS_MATERIAL,
                                     &tfile, (void **)&tmat);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }
  irc = iTIO_GetObject(fileID, NULL, lpID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get parent mesh", TIO_ERR_INT);
  }
  irc = iTIO_GetObject(fileID, NULL, sectionID, CLASS_SECTION, (void **)&tsection);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid sectionID", TIO_ERR_INVALID_SECTIONID);
  }
  if (!iTIO_CheckSection(tmesh, sectionID)) {
    TIOreturn(ERR_USER, "Invalid sectionID - not defined in parent mesh",
              TIO_ERR_INVALID_SECTIONID);
  }

  if (tmesh->meshtype != subtype) {
    if ((TIO_MESH_QUAD_NONCOLINEAR == subtype) && (TIO_MESH_QUAD_COLINEAR == tmesh->meshtype)) {
      ; /* ok */
    }
    else {
      TIOreturn(ERR_USER, "Material is not of correct mesh type - cannot read section",
                TIO_ERR_INVALID_OPERATION);
    }
  }


  if ((TIO_MESH_UNSTRUCT == tmesh->meshtype) && (tsection->unstr.centring != TIO_CENTRE_CELL)) {
      TIOreturn(ERR_USER, "Section centring must be TIO_CENTRE_CELL", TIO_ERR_INVALID_OPERATION);
  }

  if (tmat->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written material", TIO_ERR_OBJECT_NOT_READABLE);
  }
  trc = cTIO_CheckDatatype(subname, "datatype", ((NULL == mat) && (NULL == mixmat)),
                           datatype, TIO_TYPECLASS_SIGNED_INT);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  switch (subtype) {
  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:
    TIOset(ndims, tmesh->ndims);
    datapos = DATAPOS_CELL;
    break;
  case TIO_MESH_UNSTRUCT:
    datapos = DATAPOS_CELL;
    break;
  case TIO_MESH_POINT:
    datapos = DATAPOS_NODE;
    break;
  default:
    TIOreturn(ERR_INT, "Invalid subtype", TIO_ERR_INT);
  }


  irc = tfile->ReadChunkedDatasetSection( fileID, lpID, materialID, sectionID,
                                          CLASS_MATERIAL, MAT_MATERIAL,
                                          xfer, datatype, tmesh->meshtype,
                                          tmesh->ndims,
                                          tmesh->nchunks, tmesh->chunkinfo, tsection,
                                          datapos, nn, mat );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to read material dataset section (mat)", TIO_ERR_OBJECT_READ);
  }

  if (tmat->ismixed) {

    trc = cTIO_CheckDatatype(subname, "ncomp_datatype", (NULL == ncomp),
                             ncomp_datatype, TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "ptr_datatype",   (NULL == ptr),
                             ptr_datatype, TIO_TYPECLASS_SIGNED_INT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }
    trc = cTIO_CheckDatatype(subname, "vf_datatype",    (NULL == vf),
                             vf_datatype, TIO_TYPECLASS_FLOAT);
    if (trc != TIO_SUCCESS) {
      cTIOreturn (trc);
    }

    irc = tfile->ReadChunkedDatasetSection( fileID, lpID, materialID, sectionID,
                                            CLASS_MATERIAL, MAT_MATERIAL,
                                            xfer, datatype, tmesh->meshtype,
                                            tmesh->ndims,
                                            tmesh->nchunks, tmesh->chunkinfo, tsection,
                                            DATAPOS_MIXED, nmixcomp, mixmat );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material section mixmat data", TIO_ERR_INT);
    }
    irc = tfile->ReadChunkedDatasetSection( fileID, lpID, materialID, sectionID,
                                            CLASS_MATERIAL, MAT_NCOMP,
                                            xfer, ncomp_datatype, tmesh->meshtype,
                                            tmesh->ndims,
                                            tmesh->nchunks, tmesh->chunkinfo, tsection,
                                            DATAPOS_MIXCELL, nmixcell, ncomp );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material section mixmat data", TIO_ERR_INT);
    }
    irc = tfile->ReadChunkedDatasetSection( fileID, lpID, materialID, sectionID,
                                            CLASS_MATERIAL, MAT_PTR,
                                            xfer, ptr_datatype, tmesh->meshtype,
                                            tmesh->ndims,
                                            tmesh->nchunks, tmesh->chunkinfo, tsection,
                                            DATAPOS_MIXCELL, NULL, ptr );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material section mixmat data", TIO_ERR_INT);
    }
    irc = tfile->ReadChunkedDatasetSection( fileID, lpID, materialID, sectionID,
                                            CLASS_MATERIAL, MAT_VOLFRAC,
                                            xfer, vf_datatype, tmesh->meshtype,
                                            tmesh->ndims,
                                            tmesh->nchunks, tmesh->chunkinfo, tsection,
                                            DATAPOS_MIXCOMP, NULL, vf );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read material section mixmat data", TIO_ERR_INT);
    }
  }
  else {
    if (nmixcell != NULL) {
      *nmixcell = 0;
    }
    if (nmixcomp != NULL) {
      *nmixcomp = 0;
    }
  }


  TIOset(ndims, tmesh->ndims);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * iTIO_CreateMaterialDatasets
 *
 **************************************************************************************************/
int
iTIO_CreateMaterialDatasets( const TIO_File_t   fileID,
                             const TIO_Object_t meshID,
                             const TIO_Object_t matID,
                             struct iFile_t     *tfile,
                             struct iMesh_t     *tmesh,
                             struct iMaterial_t *tmat )
{
  const char *subname = "iTIO_CreateMaterialDatasets()";

  iDataPos_t datapos;

  int irc;


  if (tmat->ismixed) {

    irc = tfile->CreateChunkedDataset( fileID, meshID, matID, CLASS_MATERIAL,
                                       xMaterialDataName, MAT_MATERIAL, tmat->datatype,
                                       tmesh->meshtype, tmesh->ndims, tmat->nghosts,
                                       tmesh->nchunks, tmesh->chunkinfo,
                                       &tmesh->info, DATAPOS_MIXED );
                                                        /* MIXCOMP = PURE + appended MIX */
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to create material dataset", TIO_ERR_OBJECT_CREATE);
    }
    irc = tfile->CreateChunkedDataset( fileID, meshID, matID, CLASS_MATERIAL,
                                       xMaterialNcompName, MAT_NCOMP, tmat->ncomp_datatype,
                                       tmesh->meshtype, tmesh->ndims, tmat->nghosts,
                                       tmesh->nchunks, tmesh->chunkinfo,
                                       &tmesh->info, DATAPOS_MIXCELL );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to create ncomp dataset", TIO_ERR_OBJECT_CREATE);
    }
    irc = tfile->CreateChunkedDataset( fileID, meshID, matID, CLASS_MATERIAL,
                                       xMaterialPtrName, MAT_PTR, tmat->ptr_datatype,
                                       tmesh->meshtype, tmesh->ndims, tmat->nghosts,
                                       tmesh->nchunks, tmesh->chunkinfo,
                                       &tmesh->info, DATAPOS_MIXCELL );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to create ptr dataset", TIO_ERR_OBJECT_CREATE);
    }
    irc = tfile->CreateChunkedDataset( fileID, meshID, matID, CLASS_MATERIAL,
                                       xMaterialVolfracName, MAT_VOLFRAC, tmat->vf_datatype,
                                       tmesh->meshtype, tmesh->ndims, tmat->nghosts,
                                       tmesh->nchunks, tmesh->chunkinfo,
                                       &tmesh->info, DATAPOS_MIXCOMP );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to create volfrac dataset", TIO_ERR_OBJECT_CREATE);
    }
  }
  else {

    switch (tmesh->meshtype) {
    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR:
    case TIO_MESH_UNSTRUCT:
      datapos = DATAPOS_CELL;
      break;
    case TIO_MESH_POINT:
      datapos = DATAPOS_NODE;
      break;
    default:
      TIOreturn(ERR_INT, "Invalid value for tmesh->meshtype", -1);
      break;
    }

    irc = tfile->CreateChunkedDataset( fileID, meshID, matID, CLASS_MATERIAL,
                                       xMaterialDataName, MAT_MATERIAL, tmat->datatype,
                                       tmesh->meshtype, tmesh->ndims, tmat->nghosts,
                                       tmesh->nchunks, tmesh->chunkinfo,
                                       &tmesh->info, datapos );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to create material dataset", TIO_ERR_OBJECT_CREATE);
    }
  }


  tmat->status = STATUS_CREATED;


  return (0);
}




/***************************************************************************************************
 *
 * xTIO_GetMaterialInfo
 *
 **************************************************************************************************/
static int
xTIO_GetMaterialInfo( const char         *subname,
                      const int          getname,
                      struct iMaterial_t *tmat,
                      char               name[],
                      TIO_Data_t         *datatype,
                      TIO_Size_t         *nmat,
                      TIO_Size_t         *nghosts,
                      TIO_Bool_t         *ismixed,
                      TIO_Data_t         *ncomp_datatype,
                      TIO_Data_t         *ptr_datatype,
                      TIO_Data_t         *vf_datatype )
{
  if (getname && (name != NULL)) {
    (void) iTIO_strcpy(name, tmat->name);
  }

  TIOset(datatype, tmat->datatype);
  TIOset(nmat,     tmat->nmat);
  TIOset(nghosts,  tmat->nghosts);

  if (ismixed != NULL) {
    if (tmat->ismixed) {
      *ismixed = TIO_TRUE;
    }
    else {
      *ismixed = TIO_FALSE;
    }
  }

  if (tmat->ismixed) {
    TIOset(ncomp_datatype, tmat->ncomp_datatype);
    TIOset(ptr_datatype,   tmat->ptr_datatype);
    TIOset(vf_datatype,    tmat->vf_datatype);
  }
  else {
    TIOset(ncomp_datatype, TIO_DATATYPE_NULL);
    TIOset(ptr_datatype,   TIO_DATATYPE_NULL);
    TIOset(vf_datatype,    TIO_DATATYPE_NULL);
  }


  return (0);
}



/*
 * EOF
 */

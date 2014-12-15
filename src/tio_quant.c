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

#include "tio_quant.h"


static TIO_t cTIO_OpenQuant(        const char         *subname,
                                    const int          ByIndex,
                                    const TIO_File_t   fileID,
                                    const TIO_Object_t meshID,
                                    const TIO_Size_t   idx,
                                    char               name[],
                                    TIO_Object_t       *quantID,
                                    TIO_Data_t         *datatype,
                                    TIO_Centre_t       *centring,
                                    TIO_Size_t         *nghosts,
                                    TIO_Bool_t         *ismixed,
                                    char               units[] );

static TIO_t cTIO_WriteQuantChunk(  const char         subname[],
                                    const TIO_Mesh_t   subtype,
                                    const TIO_File_t   fileID,
                                    const TIO_Object_t quantID,
                                    const TIO_Size_t   idx,
                                    const TIO_Xfer_t   xfer,
                                    const TIO_Data_t   datatype,
                                    const void         *qdat,
                                    const void         *qmix );

static TIO_t cTIO_ReadQuantChunk(   const char         subname[],
                                    const TIO_Mesh_t   subtype,
                                    const TIO_File_t   fileID,
                                    const TIO_Object_t quantID,
                                    const TIO_Size_t   idx,
                                    const TIO_Xfer_t   xfer,
                                    const TIO_Data_t   datatype,
                                    const TIO_Size_t   nghosts,
                                    TIO_Dims_t         *ndims,
                                    TIO_Size_t         *nn,
                                    void               *qdat,
                                    TIO_Size_t         *nmixcomp,
                                    void               *qmix );

static TIO_t cTIO_ReadQuantAll(     const char         subname[],
                                    const TIO_Mesh_t   subtype,
                                    const TIO_File_t   fileID,
                                    const TIO_Object_t quantID,
                                    const TIO_Data_t   datatype,
                                    TIO_Dims_t         *ndims,
                                    TIO_Size_t         *nn,
                                    void               *qdat,
                                    TIO_Size_t         *nmixcomp,
                                    void               *qmix );

static TIO_t cTIO_ReadQuantSection( const char         subname[],
                                    const TIO_Mesh_t   subtype,
                                    const TIO_File_t   fileID,
                                    const TIO_Object_t quantID,
                                    const TIO_Object_t sectionID,
                                    const TIO_Xfer_t   xfer,
                                    const TIO_Data_t   datatype,
                                    TIO_Dims_t         *ndims,
                                    TIO_Size_t         *nn,
                                    void               *qdat,
                                    TIO_Size_t         *nmixcomp,
                                    void               *qmix );

static int xTIO_GetQuantInfo(       const char         *subname,
                                    const int          getname,
                                    struct iQuant_t    *tquant,
                                    char               name[],
                                    TIO_Data_t         *datatype,
                                    TIO_Centre_t       *centring,
                                    TIO_Size_t         *nghosts,
                                    TIO_Bool_t         *ismixed,
                                    char               units[] );


static const char *xQuantDataName = "TIO_DATA";



/***************************************************************************************************
 *
 * TIO_List_Quants
 *
 **************************************************************************************************/
TIO_t
TIO_List_Quants( const TIO_File_t   fileID,
                 const TIO_Object_t meshID,
                 TIO_Size_t         *nobj,
                 char               names[][TIO_STRLEN] )
{
  const char *subname = "TIO_List_Quants()";

  TIO_t trc;

  TIObegin(subname,1);

  trc = cTIO_HierarchyListObjects( subname, fileID, meshID, CLASS_MESH, CLASS_QUANT,
                                   nobj, names );
  TIOend(subname,1);

  return (trc);
}



/***************************************************************************************************
 *
 * TIO_Create_Quant
 *
 **************************************************************************************************/
TIO_t
TIO_Create_Quant( const TIO_File_t   fileID,
                  const TIO_Object_t meshID,
                  const char         name[],
                  TIO_Object_t       *quantID,
                  const TIO_Data_t   datatype,
                  const TIO_Centre_t centring,
                  const TIO_Size_t   nghosts,
                  const TIO_Bool_t   ismixed,
                  const char         units[] )
{
  const char *subname = "TIO_Create_Quant()";

  struct iFile_t  *tfile;
  struct iMesh_t  *tmesh;
  struct iQuant_t *tquant;

  TIO_Object_t    lqID;
  TIO_TypeClass_t typeclass;
  size_t          ds;
  TIO_Size_t      lnghosts;

  int             irc;
  TIO_t           trc;



  TIObegin(subname,1);

  *quantID = TIO_NULL;    /* Ensure NULL value on failure */


  irc = iTIO_GetObject(fileID, NULL, meshID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid meshID", TIO_ERR_INVALID_OBJECTID);
  }

  /* -- Check object-specific inbound args are valid */

  ds = iTIO_SizeOf(datatype);
  if (0 == ds) {
    TIOreturn(ERR_USER, "Invalid arg (datatype)", TIO_ERR_INVALID_DATATYPE);
  }

  trc = TIO_Get_TypeClass(datatype, &typeclass);
  switch (typeclass) {
  case TIO_TYPECLASS_SIGNED_INT:
  case TIO_TYPECLASS_UNSIGNED_INT:
  case TIO_TYPECLASS_FLOAT:
    break;
  default:
    TIOreturn(ERR_USER, "Invalid arg (datatype) - quants must be of int or float typeclass",
              TIO_ERR_INVALID_DATATYPE);
    break;
  }


  if (iTIO_Centring2Datapos(centring, tmesh->meshtype, (TIO_TRUE == ismixed)) == DATAPOS_NULL) {
    TIOreturn(ERR_USER, "Invalid arg (centring)", TIO_ERR_INVALID_ARG);
  }

  /* -- Check there is a mixed material with appropriate if quant is also mixed */

  if (TIO_TRUE == ismixed) {
    if (TIO_GHOSTS_NULL == tmesh->matghosts) {
      TIOreturn(ERR_USER, "Must have a mixed material pre-defined to create mixed quant",
                TIO_ERR_INVALID_OPERATION);
    }
    if (nghosts > tmesh->matghosts) {
      TIOreturn(ERR_USER, "Quant nghosts must be <= nghosts of material if mixed",
                TIO_ERR_INVALID_OPERATION);
    }
  }


  trc = cTIO_CheckGhosts( subname, fileID, meshID, tmesh,
                          nghosts, TIO_GHOSTS_NULL, &lnghosts, FALSE );
  if (TIO_SUCCESS != trc) {
    cTIOreturn (trc);
  }

  irc = iTIO_ValidString(units, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "units"), TIO_ERR_INVALID_STRING);
  }

  tquant = (struct iQuant_t *)TIOmalloc(sizeof(struct iQuant_t));
  if (NULL == tquant) {
    TIOreturn(ERR_MEM, "Failed to malloc tquant", TIO_ERR_INT);
  }

  tquant->qmin = NULL;   /* Explicitly nullify so can check in cleandie */
  tquant->qmax = NULL;


  trc = cTIO_HierarchyCreateObject( subname, fileID, meshID, CLASS_MESH, CLASS_QUANT, name,
                                    &tfile, &lqID, tquant );
  if (TIO_SUCCESS != trc) {
    TIOfree (tquant); tquant = NULL;
    cTIOreturn (trc);
  }


  (void) iTIO_strcpy(tquant->name, name);
  tquant->status   = STATUS_NEW;                    /* Allows writing of quant datasets */
  tquant->rangeset = FALSE;
  tquant->datatype = datatype;
  tquant->centring = centring;
  tquant->nghosts  = lnghosts;
  tquant->ismixed  = (ismixed != TIO_FALSE);

  (void) iTIO_strcpy(tquant->units, units);

  /* -- Malloc the range attributes and fill with null values */
  tquant->qmin = (void *)TIOmalloc(ds);
  if (NULL == tquant->qmin) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc quant qmin");
    trc = TIO_ERR_INT;
    goto cleandie;
  }
  tquant->qmax = (void *)TIOmalloc(ds);
  if (NULL == tquant->qmax) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc quant qmax");
    trc = TIO_ERR_INT;
    goto cleandie;
  }


  /* -- Create the quantity dataset using the parent mesh chunks if all have been set
        If chunks aren't set yet, put in the cache of datasets to be created on finalize */

  if (0 == tmesh->nchunks_unset) {
    irc = iTIO_CreateQuantDatasets(fileID, meshID, lqID, tfile, tmesh, tquant);
    if (irc < 0) {
      (void) iTIO_PutErr(ERR_INT, "Failed to create quantity dataset");
      trc = TIO_ERR_OBJECT_CREATE;
      goto cleandie;
    }
  }
  else {
    irc = iTIO_AddMeshObject(fileID, meshID, tfile, tmesh, lqID, CLASS_QUANT);
    TIOassert(irc < 0, ERR_INT, "Failed to add mesh object", TIO_ERR_INT);
  }


  /* -- Write object attributes */

  irc = 0;

  irc += tfile->WriteAttr( fileID, meshID, lqID, CLASS_QUANT, "datatype",  TIO_ENUM_T,
                           (void *)&tquant->datatype);

  irc += tfile->WriteAttr( fileID, meshID, lqID, CLASS_QUANT, "centring",  TIO_ENUM_T,
                           (void *)&tquant->centring);

  irc += tfile->WriteAttr( fileID, meshID, lqID, CLASS_QUANT, "nghosts",   TIO_SIZE_T,
                           (void *)&tquant->nghosts);

  irc += tfile->WriteAttr( fileID, meshID, lqID, CLASS_QUANT, "mixedflag", TIO_INT,
                           (void *)&tquant->ismixed);

  irc += tfile->WriteAttr( fileID, meshID, lqID, CLASS_QUANT, "units",     TIO_STRING,
                           (void *)tquant->units );

  TIOassert(irc != 0, ERR_INT, "Failed to write attributes", TIO_ERR_INT);


  *quantID = lqID;

  TIOend(subname,1);


  return (TIO_SUCCESS);


 cleandie:
  trc = cTIO_HierarchyKillObject(subname, fileID, meshID, lqID, CLASS_QUANT);
  TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to kill object", TIO_ERR_INT);

  if (tquant->qmin != NULL) {
    TIOfree (tquant->qmin); tquant->qmin = NULL;
  }
  if (tquant->qmax != NULL) {
    TIOfree (tquant->qmax); tquant->qmax = NULL;
  }
  TIOfree (tquant); tquant = NULL;
  TIOreturn(iTIO_GetErrClass(), iTIO_GetErrStr(), trc);
}




/***************************************************************************************************
 *
 * TIO_Open_Quant
 *
 **************************************************************************************************/
TIO_t
TIO_Open_Quant( const TIO_File_t   fileID,
                const TIO_Object_t meshID,
                const char         name[],
                TIO_Object_t       *quantID,
                TIO_Data_t         *datatype,
                TIO_Centre_t       *centring,
                TIO_Size_t         *nghosts,
                TIO_Bool_t         *ismixed,
                char               units[] )
{
  return ( cTIO_OpenQuant( "TIO_Open_Quant()",
                           FALSE,
                           fileID,
                           meshID,
                           TIO_NULL,
                           (char *)name,
                           quantID,
                           datatype,
                           centring,
                           nghosts,
                           ismixed,
                           units ) );
}




/***************************************************************************************************
 *
 * TIO_Open_Quant_idx
 *
 **************************************************************************************************/
TIO_t
TIO_Open_Quant_idx( const TIO_File_t   fileID,
                    const TIO_Object_t meshID,
                    const TIO_Size_t   idx,
                    char               name[],
                    TIO_Object_t       *quantID,
                    TIO_Data_t         *datatype,
                    TIO_Centre_t       *centring,
                    TIO_Size_t         *nghosts,
                    TIO_Bool_t         *ismixed,
                    char               units[] )
{
  return ( cTIO_OpenQuant( "TIO_Open_Quant_idx()",
                           TRUE,
                           fileID,
                           meshID,
                           idx,
                           name,
                           quantID,
                           datatype,
                           centring,
                           nghosts,
                           ismixed,
                           units ) );
}



static TIO_t
cTIO_OpenQuant( const char         *subname,
                const int          ByIndex,
                const TIO_File_t   fileID,
                const TIO_Object_t meshID,
                const TIO_Size_t   idx,
                char               name[],
                TIO_Object_t       *quantID,
                TIO_Data_t         *datatype,
                TIO_Centre_t       *centring,
                TIO_Size_t         *nghosts,
                TIO_Bool_t         *ismixed,
                char               units[] )
{
  struct iFile_t  *tfile;
  struct iMesh_t  *tmesh;
  struct iQuant_t *tquant;

  size_t       ds;
  iDataPos_t   datapos;

  TIO_Object_t lqID;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  *quantID = TIO_NULL;    /* Ensure NULL value on failure */

  irc = iTIO_GetObject(fileID, NULL, meshID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid meshID", TIO_ERR_INVALID_OBJECTID);
  }

  tquant = (struct iQuant_t *)TIOmalloc(sizeof(struct iQuant_t));
  if (NULL == tquant) {
    TIOreturn(ERR_MEM, "Failed to malloc tquant", TIO_ERR_INT);
  }

  tquant->qmin = NULL;   /* Explicitly nullify so can check in cleandie */
  tquant->qmax = NULL;


  trc = cTIO_HierarchyOpenObject( subname, fileID, meshID, CLASS_MESH, CLASS_QUANT,
                                  name, ByIndex, idx,
                                  tquant->name, &tfile, &lqID, (void *)tquant );
  if (TIO_SUCCESS != trc) {
    TIOfree (tquant); tquant = NULL;
    cTIOreturn (trc);
  }


  /* -- Read object attributes */
  irc = 0;

  irc += tfile->ReadAttr( fileID, meshID, lqID, CLASS_QUANT, "datatype",  TIO_ENUM_T,
                          (void *)&tquant->datatype);

  irc += tfile->ReadAttr( fileID, meshID, lqID, CLASS_QUANT, "centring",  TIO_ENUM_T,
                          (void *)&tquant->centring);

  irc += tfile->ReadAttr( fileID, meshID, lqID, CLASS_QUANT, "nghosts",   TIO_SIZE_T,
                          (void *)&tquant->nghosts);

  irc += tfile->ReadAttr( fileID, meshID, lqID, CLASS_QUANT, "mixedflag", TIO_INT,
                          (void *)&tquant->ismixed);

  irc += tfile->ReadAttr( fileID, meshID, lqID, CLASS_QUANT, "units",     TIO_STRING,
                          (void *)tquant->units );

  TIOassert(irc != 0, ERR_INT, "Failed to read attributes", TIO_ERR_INT);


  /* -- Read the range attributes using the stored datatype */

  ds = iTIO_SizeOf(tquant->datatype);
  if (0 == ds) {
    (void) iTIO_PutErr(ERR_INT, "Invalid datatype in file");
    trc = TIO_ERR_INT;
    goto cleandie;
  }

  tquant->qmin = (void *)TIOmalloc(ds);
  if (NULL == tquant->qmin) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc quant qmin");
    trc = TIO_ERR_INT;
    goto cleandie;
  }
  tquant->qmax = (void *)TIOmalloc(ds);
  if (NULL == tquant->qmax) {
    (void) iTIO_PutErr(ERR_MEM, "Failed to malloc quant qmax");
    trc = TIO_ERR_INT;
    goto cleandie;
  }

  irc += tfile->ReadAttr( fileID, meshID, lqID, CLASS_QUANT, "qmin", tquant->datatype,
                          (void *)tquant->qmin );

  irc += tfile->ReadAttr( fileID, meshID, lqID, CLASS_QUANT, "qmax", tquant->datatype,
                          (void *)tquant->qmax );

  TIOassert(irc != 0, ERR_INT, "Failed to read quant range attributes", TIO_ERR_INT);


  tquant->status   = STATUS_OLD;      /* Disallows writing of mesh datasets */
  tquant->rangeset = TRUE;


  /* Open quant datasets here */

  datapos = iTIO_Centring2Datapos(tquant->centring, tmesh->meshtype, tquant->ismixed);

  irc = tfile->OpenChunkedDataset( fileID, meshID, lqID, CLASS_QUANT,
                                   xQuantDataName, 0,
                                   tquant->datatype, tmesh->meshtype,
                                   tmesh->ndims, tquant->nghosts,
                                   tmesh->nchunks, tmesh->chunkinfo,
                                   &tmesh->info, datapos );
  if (irc < 0) {
    (void) iTIO_PutErr(ERR_INT, "Failed to quant material dataset");
    trc = TIO_ERR_OBJECT_OPEN;
    goto cleandie;
  }

  irc = xTIO_GetQuantInfo( subname, ByIndex, tquant, name, datatype, centring, nghosts,
                           ismixed, units );
  TIOassert(irc < 0, ERR_INT, "Failed to get quant info", TIO_ERR_INT);


  *quantID = lqID;

  TIOend(subname,1);

  return (TIO_SUCCESS);


 cleandie:
  trc = cTIO_HierarchyKillObject(subname, fileID, meshID, lqID, CLASS_QUANT);
  TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to kill object", TIO_ERR_INT);

  if (tquant->qmin != NULL) {
    TIOfree (tquant->qmin); tquant->qmin = NULL;
  }
  if (tquant->qmax != NULL) {
    TIOfree (tquant->qmax); tquant->qmax = NULL;
  }
  TIOfree (tquant); tquant = NULL;
  TIOreturn(iTIO_GetErrClass(), iTIO_GetErrStr(), trc);
}




/***************************************************************************************************
 *
 * TIO_Get_Quant_Info
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Quant_Info( const TIO_File_t   fileID,
                    const TIO_Object_t quantID,
                    char               name[],
                    TIO_Data_t         *datatype,
                    TIO_Centre_t       *centring,
                    TIO_Size_t         *nghosts,
                    TIO_Bool_t         *ismixed,
                    char               units[] )
{
  const char *subname = "TIO_Get_Quant_Info()";

  struct iQuant_t *tquant;

  int   irc;
  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, quantID, CLASS_QUANT,
                                     NULL, (void **)&tquant);
  if (TIO_SUCCESS != trc) {
    cTIOreturn (trc);
  }

  irc = xTIO_GetQuantInfo( subname, TRUE, tquant, name, datatype, centring, nghosts,
                           ismixed, units );
  TIOassert(irc < 0, ERR_INT, "Failed to get quant info", TIO_ERR_INT);


  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Close_Quant
 *
 **************************************************************************************************/
TIO_t
TIO_Close_Quant( const TIO_File_t   fileID,
                 const TIO_Object_t quantID )
{
  const char *subname = "TIO_Close_Quant()";

  struct iFile_t  *tfile;
  struct iQuant_t *tquant;

  TIO_Object_t    lpID;         /* parentID   */
  TIO_TypeClass_t typeclass;
  int             irc;
  TIO_t           trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyPreClose( subname, fileID, &lpID, quantID, CLASS_QUANT,
                                &tfile, (void **)&tquant );
  if (TIO_SUCCESS != trc) {
    cTIOreturn (trc);
  }

  /* Close quant dataset */

  /* Possible that none were written (you never know!) so no need to close if so */


  if (tquant->status != STATUS_NEW) {
    irc = tfile->CloseChunkedDataset(fileID, lpID, quantID, CLASS_QUANT, 0);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to close quant dataset", TIO_ERR_OBJECT_CLOSE);
    }
  }


  if (tquant->status != STATUS_OLD) {

    if (!tquant->rangeset) {

      /* -- Set default null range values for quantity
            Conversion will get done to turn values into quant's datatype
            Doesn't really matter what the values are provided that qmin > qmax
            - this flags to viz/whatever apps that they need to work them out themselves */

      trc = TIO_Get_TypeClass(tquant->datatype, &typeclass);
      switch (typeclass) {
      case TIO_TYPECLASS_SIGNED_INT:
      case TIO_TYPECLASS_UNSIGNED_INT:
        {
          int iqmin=999, iqmax=666;

          trc = TIO_Set_Quant_Range(fileID, quantID, TIO_INT, &iqmin, &iqmax);
          TIOassert(TIO_SUCCESS != trc, ERR_INT, "Failed to set quant range", TIO_ERR_INT);
        }
        break;
      case TIO_TYPECLASS_FLOAT:
        {
          float fqmin=999.0, fqmax=666.0;

          trc = TIO_Set_Quant_Range(fileID, quantID, TIO_FLOAT, &fqmin, &fqmax);
          TIOassert(TIO_SUCCESS != trc, ERR_INT, "Failed to set quant range", TIO_ERR_INT);
        }
        break;

      default:
        break;
      }

    }


    /* -- Write range attributes  - these will above default null values if not explicit set */

    irc = 0;

    irc += tfile->WriteAttr( fileID, lpID, quantID, CLASS_QUANT, "qmin",
                             tquant->datatype, (void *)tquant->qmin);

    irc += tfile->WriteAttr( fileID, lpID, quantID, CLASS_QUANT, "qmax",
                             tquant->datatype, (void *)tquant->qmax );

    TIOassert(irc < 0, ERR_INT, "Failed to write quant range attributes", TIO_ERR_INT);
  }

  TIOfree (tquant->qmin); tquant->qmin = NULL;
  TIOfree (tquant->qmax); tquant->qmax = NULL;
  TIOfree (tquant);       tquant       = NULL;

  trc = cTIO_HierarchyCloseObject( subname, fileID, lpID, quantID, CLASS_QUANT );

  TIOend(subname,1);


  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Set_Quant_Range
 *
 * - can be called any time between create_quant and close_quant
 *
 **************************************************************************************************/
TIO_t
TIO_Set_Quant_Range( const TIO_File_t   fileID,
                     const TIO_Object_t quantID,
                     const TIO_Data_t   datatype,
                     const void         *qmin,
                     const void         *qmax )
{
  const char *subname = "TIO_Set_Quant_Range()";

  struct iFile_t  *tfile;
  struct iQuant_t *tquant;

  size_t dsi;

  int    irc;
  TIO_t  trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, quantID, CLASS_QUANT,
                                     &tfile, (void **)&tquant);
  if (TIO_SUCCESS != trc) {
    cTIOreturn (trc);
  }

  dsi = iTIO_SizeOf(datatype);
  if (0 == dsi) {
    TIOreturn(ERR_USER, "Invalid argument (datatype)", TIO_ERR_INVALID_DATATYPE);
  }
  if (tquant->rangeset) {
    TIOreturn(ERR_USER, "Quant range has already been set", TIO_ERR_INVALID_OPERATION);
  }

  if (datatype != tquant->datatype) {

    size_t dsf;
    void   *buf;

    dsf = iTIO_SizeOf(tquant->datatype);

    buf = TIOmalloc(TIOmax(dsi, dsf));

    if (qmin != NULL) {
      (void) memcpy(buf, qmin, dsi);
      irc = tfile->ConvBuf(datatype, tquant->datatype, 1, buf);
      if (irc < 0) {
        TIOfree (buf); buf = NULL;
        TIOreturn(ERR_USER, "Unable to convert between given datatype and that of quant",
                  TIO_ERR_INVALID_CONVERSION);
      }
      (void) memcpy(tquant->qmin, buf, dsf);
    }

    if (qmax != NULL) {
      (void) memcpy(buf, qmax, dsi);
      irc = tfile->ConvBuf(datatype, tquant->datatype, 1, buf);
      if (irc < 0) {
        TIOfree (buf); buf = NULL;
        TIOreturn(ERR_USER, "Unable to convert between given datatype and that of quant",
                  TIO_ERR_INVALID_CONVERSION);
      }
      (void) memcpy(tquant->qmax, buf, dsf);
    }

    TIOfree (buf); buf = NULL;

  }
  else {

    if (qmin != NULL) {
      (void) memcpy(tquant->qmin, qmin, dsi);
    }
    if (qmax != NULL) {
      (void) memcpy(tquant->qmax, qmax, dsi);
    }

  }

  /* Postpone writing until collective quantity close call */

  tquant->rangeset = TRUE;


  TIOend(subname,1);

  return (TIO_SUCCESS);

}




/***************************************************************************************************
 *
 * TIO_Get_Quant_Range
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Quant_Range( const TIO_File_t   fileID,
                     const TIO_Object_t quantID,
                     const TIO_Data_t   datatype,
                     void               *qmin,
                     void               *qmax )
{
  const char *subname = "TIO_Get_Quant_Range()";

  struct iFile_t  *tfile;
  struct iQuant_t *tquant;

  void   *minbuf, *maxbuf;

  size_t dsi, dsf, dss;

  int   irc;
  TIO_t trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, quantID, CLASS_QUANT,
                                     &tfile, (void **)&tquant);
  if (TIO_SUCCESS != trc) {
    cTIOreturn (trc);
  }

  dsi = iTIO_SizeOf(datatype);
  if (0 == dsi) {
    if ((TIO_DATATYPE_NULL == datatype) && (NULL == qmin) && (NULL == qmax)) {
      /* ok */
    }
    else {
      TIOreturn(ERR_USER, "Invalid argument (datatype)", TIO_ERR_INVALID_DATATYPE);
    }
  }

  dsf = iTIO_SizeOf(tquant->datatype);


  dss = TIOmax(dsi, dsf);

  minbuf = TIOmalloc(dss);
  maxbuf = TIOmalloc(dss);

  (void) memcpy(minbuf, tquant->qmin, dsf);
  (void) memcpy(maxbuf, tquant->qmax, dsf);


  if (datatype != tquant->datatype) {

    void *buf;

    buf = TIOmalloc(dss);

    if (qmin != NULL) {
      (void) memcpy(buf, minbuf, dsf);
      irc = tfile->ConvBuf(tquant->datatype, datatype, 1, buf);
      if (irc < 0) {
        TIOfree (minbuf); minbuf = NULL;
        TIOfree (maxbuf); maxbuf = NULL;
        TIOfree (buf);    buf    = NULL;
        TIOreturn(ERR_USER, "Unable to convert between given datatype and that of quant",
                  TIO_ERR_INVALID_CONVERSION);
      }
      (void) memcpy(qmin, buf, dsi);
    }

    if (qmax != NULL) {
      (void) memcpy(buf, maxbuf, dsf);
      irc = tfile->ConvBuf(tquant->datatype, datatype, 1, buf);
      if (irc < 0) {
        TIOfree (minbuf); minbuf = NULL;
        TIOfree (maxbuf); maxbuf = NULL;
        TIOfree (buf);    buf    = NULL;
        TIOreturn(ERR_USER, "Unable to convert between given datatype and that of quant",
                  TIO_ERR_INVALID_CONVERSION);
      }
      (void) memcpy(qmax, buf, dsi);
    }

    TIOfree (buf); buf = NULL;

  }
  else {

    if (qmin != NULL) {
      (void) memcpy(qmin, tquant->qmin, dsi);
    }
    if (qmax != NULL) {
      (void) memcpy(qmax, tquant->qmax, dsi);
    }

  }


  /* -- Check if range is valid */

  trc = cTIO_CheckRange(subname, tfile, tquant->datatype, minbuf, maxbuf);


  TIOfree (minbuf); minbuf = NULL;
  TIOfree (maxbuf); maxbuf = NULL;


  TIOend(subname,1);

  return (trc);
}




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
 **************************************************************************************************/
TIO_t
TIO_Write_QuadQuant_Chunk( const TIO_File_t   fileID,
                           const TIO_Object_t quantID,
                           const TIO_Size_t   idx,
                           const TIO_Xfer_t   xfer,
                           const TIO_Data_t   datatype,
                           const void         *qdat,
                           const void         *qmix )
{
  const char *subname = "TIO_Write_QuadQuant_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_WriteQuantChunk( subname, TIO_MESH_QUAD_NONCOLINEAR,   /* noncolinear for all quad */
                              fileID, quantID, idx, xfer, datatype, qdat, qmix );

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_QuadQuant_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadQuant_Chunk( const TIO_File_t   fileID,
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
                          void               *qmix )
{
  const char *subname = "TIO_Read_QuadQuant_Chunk()";

  TIO_Size_t nn[3];
  TIO_t      trc;


  TIObegin(subname,1);

  nn[0] = 0;
  nn[1] = 0;
  nn[2] = 0;


  trc = cTIO_ReadQuantChunk( subname, TIO_MESH_QUAD_NONCOLINEAR,   /* noncolinear used for all */
                             fileID, quantID, idx, xfer, datatype, nghosts,
                             ndims, nn, qdat, nmixcomp, qmix );
  if (TIO_SUCCESS != trc) {
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
 * TIO_Read_QuadQuant_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadQuant_All( const TIO_File_t   fileID,
                        const TIO_Object_t quantID,
                        const TIO_Data_t   datatype,
                        TIO_Dims_t         *ndims,
                        TIO_Size_t         *ni,
                        TIO_Size_t         *nj,
                        TIO_Size_t         *nk,
                        void               *qdat,
                        TIO_Size_t         *nmixcomp,
                        void               *qmix )
{
  const char *subname = "TIO_Read_QuadQuant_All()";

  TIO_Size_t nn[3];
  TIO_t      trc;


  TIObegin(subname,1);

  nn[0] = 0;
  nn[1] = 0;
  nn[2] = 0;

  trc = cTIO_ReadQuantAll( subname, TIO_MESH_QUAD_NONCOLINEAR,   /* colinear used for all quad */
                           fileID, quantID, datatype, ndims, nn, qdat, nmixcomp, qmix );
  if (TIO_SUCCESS != trc) {
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
 * TIO_Read_QuadQuant_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Read_QuadQuant_Section( const TIO_File_t   fileID,
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
                            void               *qmix )
{
  const char *subname = "TIO_Read_QuadQuant_Section()";

  TIO_Size_t nn[3];
  TIO_t      trc;


  TIObegin(subname,1);

  nn[0] = 0;
  nn[1] = 0;
  nn[2] = 0;

  trc = cTIO_ReadQuantSection( subname, TIO_MESH_QUAD_NONCOLINEAR,
                               fileID, quantID, sectionID,
                               xfer, datatype, ndims, nn, qdat, nmixcomp, qmix );
  if (TIO_SUCCESS != trc) {
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
 **   UNSTR QUANTS
 **
 **************************************
 **************************************/


/***************************************************************************************************
 *
 * TIO_Write_UnstQuant_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Write_UnstrQuant_Chunk( const TIO_File_t   fileID,
                            const TIO_Object_t quantID,
                            const TIO_Size_t   idx,
                            const TIO_Xfer_t   xfer,
                            const TIO_Data_t   datatype,
                            const void         *qdat,
                            const void         *qmix )
{
  const char *subname = "TIO_Write_UnstrQuant_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_WriteQuantChunk( subname, TIO_MESH_UNSTRUCT,
                              fileID, quantID, idx, xfer, datatype, qdat, qmix );

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_UnstrQuant_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_UnstrQuant_Chunk( const TIO_File_t   fileID,
                           const TIO_Object_t quantID,
                           const TIO_Size_t   idx,
                           const TIO_Xfer_t   xfer,
                           const TIO_Data_t   datatype,
                           const TIO_Size_t   nghosts,
                           TIO_Size_t         *nn,       /* cells/nodes wrt centring */
                           void               *qdat,
                           TIO_Size_t         *nmixcomp,
                           void               *qmix )
{
  const char *subname = "TIO_Read_UnstrQuant_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadQuantChunk( subname, TIO_MESH_UNSTRUCT,
                             fileID, quantID, idx, xfer, datatype, nghosts,
                             NULL, nn, qdat, nmixcomp, qmix );
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_UnstrQuant_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_UnstrQuant_All( const TIO_File_t   fileID,
                         const TIO_Object_t quantID,
                         const TIO_Data_t   datatype,
                         TIO_Size_t         *nn,       /* cells/nodes wrt centring */
                         void               *qdat,
                         TIO_Size_t         *nmixcomp,
                         void               *qmix )
{
  const char *subname = "TIO_Read_UnstrQuant_All()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadQuantAll( subname, TIO_MESH_UNSTRUCT,
                           fileID, quantID, datatype, NULL, nn, qdat, nmixcomp, qmix );
  TIOend(subname,1);

  return (trc);
}



/***************************************************************************************************
 *
 * TIO_Read_UnstrQuant_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Read_UnstrQuant_Section( const TIO_File_t   fileID,
                             const TIO_Object_t quantID,
                             const TIO_Object_t sectionID,
                             const TIO_Xfer_t   xfer,
                             const TIO_Data_t   datatype,
                             TIO_Size_t         *nn,
                             void               *qdat,
                             TIO_Size_t         *nmixcomp,
                             void               *qmix )
{
  const char *subname = "TIO_Read_UnstrQuant_Section()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadQuantSection( subname, TIO_MESH_UNSTRUCT,
                               fileID, quantID, sectionID,
                               xfer, datatype, NULL, nn, qdat, nmixcomp, qmix );
  TIOend(subname,1);

  return (trc);
}




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
 **************************************************************************************************/
TIO_t
TIO_Write_PointQuant_Chunk( const TIO_File_t   fileID,
                            const TIO_Object_t quantID,
                            const TIO_Size_t   idx,
                            const TIO_Xfer_t   xfer,
                            const TIO_Data_t   datatype,
                            const void         *qdat )
{
  const char *subname = "TIO_Write_PointQuant_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_WriteQuantChunk( subname, TIO_MESH_POINT,
                              fileID, quantID, idx, xfer, datatype, qdat, NULL );

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_PointQuant_Chunk
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointQuant_Chunk( const TIO_File_t   fileID,
                           const TIO_Object_t quantID,
                           const TIO_Size_t   idx,
                           const TIO_Xfer_t   xfer,
                           const TIO_Data_t   datatype,
                           const TIO_Size_t   nghosts,
                           TIO_Size_t         *nnodes,
                           void               *qdat )
{
  const char *subname = "TIO_Read_PointQuant_Chunk()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadQuantChunk( subname, TIO_MESH_POINT,
                             fileID, quantID, idx, xfer, datatype, nghosts,
                             NULL, nnodes, qdat, NULL, NULL );
  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_PointQuant_All
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointQuant_All( const TIO_File_t   fileID,
                         const TIO_Object_t quantID,
                         const TIO_Data_t   datatype,
                         TIO_Size_t         *nnodes,
                         void               *qdat )
{
  const char *subname = "TIO_Read_PointQuant_All()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadQuantAll( subname, TIO_MESH_POINT,
                           fileID, quantID, datatype, NULL, nnodes, qdat, NULL, NULL );

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Read_PointQuant_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Read_PointQuant_Section( const TIO_File_t   fileID,
                             const TIO_Object_t quantID,
                             const TIO_Object_t sectionID,
                             const TIO_Xfer_t   xfer,
                             const TIO_Data_t   datatype,
                             TIO_Size_t         *nnodes,
                             void               *qdat )
{
  const char *subname = "TIO_Read_PointrQuant_Section()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_ReadQuantSection( subname, TIO_MESH_POINT,
                               fileID, quantID, sectionID,
                               xfer, datatype, NULL, nnodes, qdat, NULL, NULL );
  TIOend(subname,1);

  return (trc);
}




/**************************************
 **************************************
 **
 **   ALL QUANTS
 **
 **************************************
 **************************************/




/***************************************************************************************************
 *
 * cTIO_WriteQuantChunk
 *
 **************************************************************************************************/
static TIO_t
cTIO_WriteQuantChunk( const char         subname[],
                      const TIO_Mesh_t   subtype,
                      const TIO_File_t   fileID,
                      const TIO_Object_t quantID,
                      const TIO_Size_t   idx,
                      const TIO_Xfer_t   xfer,
                      const TIO_Data_t   datatype,
                      const void         *qdat,
                      const void         *qmix )
{
  struct iFile_t  *tfile;
  struct iMesh_t  *tmesh;
  struct iQuant_t *tquant;

  TIO_Object_t lpID;     /* parent ID */
  iDataPos_t   datapos;

  int   irc;
  TIO_t trc;



  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, quantID, CLASS_QUANT,
                                     &tfile, (void **)&tquant);
  if (TIO_SUCCESS != trc) {
    return (trc);
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
      TIOreturn(ERR_USER, "Quant is not of correct mesh type - cannot write chunk",
                TIO_ERR_INVALID_OPERATION);
    }
  }

  if (tmesh->nchunks_unset > 0) {
    TIOreturn(ERR_USER, "Not all parent mesh chunks have been set", TIO_ERR_OBJECT_WRITE);
  }
  if ((tquant->status != STATUS_CREATED) && (tquant->status != STATUS_WRITTEN)) {
    TIOreturn(ERR_USER, "Cannot write to pre-existing quant", TIO_ERR_OBJECT_NOT_WRITEABLE);
  }
  if (0 == iTIO_SizeOf(datatype)) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  datapos = iTIO_Centring2Datapos(tquant->centring, tmesh->meshtype, FALSE);

  irc = tfile->WriteChunkedDataset( fileID, lpID, quantID,
                                    CLASS_QUANT, 0,
                                    xfer, datatype, tmesh->meshtype,
                                    tmesh->ndims, tquant->nghosts,
                                    tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                    datapos, idx, qdat );

  if (0 == irc) {
    tquant->status = STATUS_WRITTEN;
  }
  else if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to write quant chunk data", TIO_ERR_INT);
  }
  else {
    /* all ok */
    ;
  }

  if (tquant->ismixed) {

    datapos = DATAPOS_MIXED;

    irc = tfile->WriteChunkedDataset( fileID, lpID, quantID,
                                      CLASS_QUANT, 0,
                                      xfer, datatype, tmesh->meshtype,
                                      tmesh->ndims, tquant->nghosts,
                                      tmesh->nchunks, TIO_FALSE, tmesh->chunkinfo, 
                                      datapos, idx, qmix );
    if (0 == irc) {
      tquant->status = STATUS_WRITTEN;
    }
    else if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to write quant chunk mix data", TIO_ERR_INT);
    }
    else {
      /* all ok */
      ;
    }
  }


  return (TIO_SUCCESS);
}



/***************************************************************************************************
 *
 * cTIO_ReadQuantChunk
 *
 **************************************************************************************************/
static TIO_t
cTIO_ReadQuantChunk( const char         subname[],
                     const TIO_Mesh_t   subtype,
                     const TIO_File_t   fileID,
                     const TIO_Object_t quantID,
                     const TIO_Size_t   idx,
                     const TIO_Xfer_t   xfer,
                     const TIO_Data_t   datatype,
                     const TIO_Size_t   nghosts,
                     TIO_Dims_t         *ndims,
                     TIO_Size_t         *nn,
                     void               *qdat,
                     TIO_Size_t         *nmixcomp,
                     void               *qmix )
{
  struct iFile_t  *tfile;
  struct iMesh_t  *tmesh;
  struct iQuant_t *tquant;

  TIO_Object_t lpID;     /* parent ID */
  TIO_Size_t   lnghosts;
  iDataPos_t   datapos;

  int        irc;
  TIO_t trc;



  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, quantID, CLASS_QUANT,
                                     &tfile, (void **)&tquant);
  if (TIO_SUCCESS != trc) {
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
      TIOreturn(ERR_USER, "Quant is not of correct mesh type - cannot read chunk",
                TIO_ERR_INVALID_OPERATION);
    }
  }

  if (tquant->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written quant", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if (0 == iTIO_SizeOf(datatype)) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  /* check nghosts and set up anything else specific to subtype */

  trc = cTIO_CheckGhosts(subname, fileID, lpID, tmesh, nghosts, tquant->nghosts, &lnghosts, TRUE);
  if (TIO_SUCCESS != trc) {
    cTIOreturn (trc);
  }

  datapos = iTIO_Centring2Datapos(tquant->centring, tmesh->meshtype, FALSE);


  irc = tfile->ReadChunkedDataset( fileID, lpID, quantID, CLASS_QUANT, 0,
                                   xfer, datatype, tmesh->meshtype,
                                   tmesh->ndims, lnghosts,
                                   tmesh->nchunks, tmesh->chunkinfo, 
                                   datapos, idx, nn, qdat );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to read quant chunk data", TIO_ERR_INT);
  }

  if (tquant->ismixed) {

    datapos = DATAPOS_MIXED;

    irc = tfile->ReadChunkedDataset( fileID, lpID, quantID, CLASS_QUANT, 0,
                                     xfer, datatype, tmesh->meshtype,
                                     tmesh->ndims, lnghosts,
                                     tmesh->nchunks, tmesh->chunkinfo, 
                                     datapos, idx, nmixcomp, qmix );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read quant chunk mix data", TIO_ERR_INT);
    }
  }
  else {
    TIOset(nmixcomp, 0);
  }


  TIOset(ndims, tmesh->ndims);


  return (TIO_SUCCESS);
}





/***************************************************************************************************
 *
 * cTIO_ReadQuantAll
 *
 **************************************************************************************************/
static TIO_t
cTIO_ReadQuantAll( const char         subname[],
                   const TIO_Mesh_t   subtype,
                   const TIO_File_t   fileID,
                   const TIO_Object_t quantID,
                   const TIO_Data_t   datatype,
                   TIO_Dims_t         *ndims,
                   TIO_Size_t         *nn,
                   void               *qdat,
                   TIO_Size_t         *nmixcomp,
                   void               *qmix )
{
  struct iFile_t  *tfile;
  struct iMesh_t  *tmesh;
  struct iQuant_t *tquant;

  TIO_Object_t lpID;         /* parentID   */
  iDataPos_t   datapos;

  int   irc;
  TIO_t trc;



  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, quantID, CLASS_QUANT,
                                     &tfile, (void **)&tquant);
  if (TIO_SUCCESS != trc) {
    return (trc);
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
      TIOreturn(ERR_USER, "Quant is not of correct mesh type - cannot read chunk",
                TIO_ERR_INVALID_OPERATION);
    }
  }

  if (tquant->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written quant", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if (0 == iTIO_SizeOf(datatype)) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }

  datapos = iTIO_Centring2Datapos(tquant->centring, tmesh->meshtype, FALSE);

  irc = tfile->ReadChunkedDatasetAll( fileID, lpID, quantID, CLASS_QUANT, 0,
                                      TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                      tmesh->ndims,
                                      tmesh->nchunks, tmesh->chunkinfo, &tmesh->info,
                                       datapos, nn, qdat );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to read quant all data", TIO_ERR_OBJECT_READ);
  }

  if (tquant->ismixed) {

    datapos = DATAPOS_MIXED;

    irc = tfile->ReadChunkedDatasetAll( fileID, lpID, quantID, CLASS_QUANT, 0,
                                        TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                        tmesh->ndims,
                                        tmesh->nchunks, tmesh->chunkinfo, &tmesh->info,
                                        datapos, nmixcomp, qmix );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read quant all mix data", TIO_ERR_OBJECT_READ);
    }
  }
  else {
    TIOset(nmixcomp, 0);
  }

  TIOset(ndims, tmesh->ndims);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_ReadQuantSection
 *
 **************************************************************************************************/
static TIO_t
cTIO_ReadQuantSection( const char         subname[],
                       const TIO_Mesh_t   subtype,
                       const TIO_File_t   fileID,
                       const TIO_Object_t quantID,
                       const TIO_Object_t sectionID,
                       const TIO_Xfer_t   xfer,
                       const TIO_Data_t   datatype,
                       TIO_Dims_t         *ndims,
                       TIO_Size_t         *nn,
                       void               *qdat,
                       TIO_Size_t         *nmixcomp,
                       void               *qmix )
{
  struct iFile_t    *tfile;
  struct iMesh_t    *tmesh;
  struct iQuant_t   *tquant;
  union  iSection_t *tsection;

  TIO_Object_t lpID;         /* parentID   */
  iDataPos_t   datapos;

  int   irc;
  TIO_t trc;


  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, quantID, CLASS_QUANT,
                                     &tfile, (void **)&tquant);
  if (TIO_SUCCESS != trc) {
    return (trc);
  }
  irc = iTIO_GetObject(fileID, NULL, sectionID, CLASS_SECTION, (void **)&tsection);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid sectionID", TIO_ERR_INVALID_SECTIONID);
  }
  irc = iTIO_GetObject(fileID, NULL, lpID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get parent mesh", TIO_ERR_INT);
  }

  if (!iTIO_CheckSection(tmesh, sectionID)) {
    TIOreturn(ERR_USER, "Invalid sectionID - not defined parent mesh", TIO_ERR_INVALID_SECTIONID);
  }

  if (tmesh->meshtype != subtype) {
    if ((TIO_MESH_QUAD_NONCOLINEAR == subtype) && (TIO_MESH_QUAD_COLINEAR == tmesh->meshtype)) {
      ; /* ok */
    }
    else {
      TIOreturn(ERR_USER, "Quant is not of correct mesh type - cannot write chunk",
                TIO_ERR_INVALID_OPERATION);
    }
  }

  if (TIO_MESH_UNSTRUCT == tmesh->meshtype) {
    if (tquant->centring != tsection->unstr.centring) {
      TIOreturn(ERR_USER, "Quant centring does not match section centring",
                TIO_ERR_INVALID_OPERATION);
    }

  }

  if (tquant->status < STATUS_OLD) {
    TIOreturn(ERR_USER, "Cannot read non-written quant", TIO_ERR_OBJECT_NOT_READABLE);
  }
  if (0 == iTIO_SizeOf(datatype)) {
    if ((qdat != NULL) || (qmix != NULL)) {
      TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
    }
  }

  datapos = iTIO_Centring2Datapos(tquant->centring, tmesh->meshtype, FALSE);


  irc = tfile->ReadChunkedDatasetSection( fileID, lpID, quantID, sectionID, CLASS_QUANT, 0,
                                          TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                          tmesh->ndims, tmesh->nchunks, tmesh->chunkinfo, tsection,
                                          datapos, nn, qdat );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to read quant section data", TIO_ERR_OBJECT_READ);
  }

  if (tquant->ismixed) {
    irc = tfile->ReadChunkedDatasetSection( fileID, lpID, quantID, sectionID, CLASS_QUANT, 0,
                                            TIO_XFER_INDEPENDENT, datatype, tmesh->meshtype,
                                            tmesh->ndims, tmesh->nchunks, tmesh->chunkinfo, tsection,
                                            DATAPOS_MIXED, nmixcomp, qmix );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read quant section mix data", TIO_ERR_OBJECT_READ);
    }
  }
  else {
    TIOset(nmixcomp, 0);
  }


  TIOset(ndims, tmesh->ndims);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * iTIO_CreateQuantDatasets
 *
 **************************************************************************************************/
int
iTIO_CreateQuantDatasets( const TIO_File_t   fileID,
                          const TIO_Object_t meshID,
                          const TIO_Object_t quantID,
                          struct iFile_t     *tfile,
                          struct iMesh_t     *tmesh,
                          struct iQuant_t    *tquant )
{
  const char *subname = "iTIO_CreateQuantDatasets()";

  int        irc;
  iDataPos_t datapos;


  datapos = iTIO_Centring2Datapos(tquant->centring, tmesh->meshtype, tquant->ismixed);

  irc = tfile->CreateChunkedDataset( fileID, meshID, quantID, CLASS_QUANT,
                                     xQuantDataName, 0, tquant->datatype,
                                     tmesh->meshtype, tmesh->ndims, tquant->nghosts,
                                     tmesh->nchunks, tmesh->chunkinfo,
                                     &tmesh->info, datapos );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to create quant dataset", TIO_ERR_OBJECT_CREATE);
  }

  tquant->status = STATUS_CREATED;


  return (0);
}



/***************************************************************************************************
 *
 * xTIO_GetQuantInfo
 *
 **************************************************************************************************/
static int
xTIO_GetQuantInfo( const char      *subname,
                   const int       getname,
                   struct iQuant_t *tquant,
                   char            name[],
                   TIO_Data_t      *datatype,
                   TIO_Centre_t    *centring,
                   TIO_Size_t      *nghosts,
                   TIO_Bool_t      *ismixed,
                   char            units[] )
{
  TIO_Bool_t lm;

  if (getname && (name != NULL)) {
    iTIO_strcpy(name, tquant->name);
  }

  TIOset(datatype, tquant->datatype);
  TIOset(centring, tquant->centring);
  TIOset(nghosts,  tquant->nghosts);

  if (tquant->ismixed) {
    lm = TIO_TRUE;
  }
  else {
    lm = TIO_FALSE;
  }

  TIOset(ismixed, lm);

  if (units != NULL) {
    iTIO_strcpy(units, tquant->units);
  }


  return (0);
}




/*
 * EOF
 */

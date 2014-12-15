/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"
#include "tio_register_int.h"

#include "tio_variable.h"


static TIO_t cTIO_OpenVar( const char         subname[],
                           const int          ByIndex,
                           const TIO_File_t   fileID,
                           const TIO_Object_t objectID,
                           const TIO_Size_t   idx,
                           char               name[],
                           TIO_Object_t       *varID,
                           TIO_Data_t         *datatype,
                           TIO_Dims_t         *ndims,
                           TIO_Size_t         dims[],
                           char               *units );

static int xTIO_GetVariableInfo( const char         subname[],
                                 const int          getname,
                                 struct iVariable_t *tvar,
                                 char               name[],
                                 TIO_Data_t         *datatype,
                                 TIO_Dims_t         *ndims,
                                 TIO_Size_t         dims[],
                                 char               units[] );




/***************************************************************************************************
 *
 * TIO_List_Variables
 *
 **************************************************************************************************/
TIO_t
TIO_List_Variables( const TIO_File_t   fileID,
                    const TIO_Object_t objectID,
                    TIO_Size_t         *nobj,
                    char               names[][TIO_STRLEN] )
{
  const char *subname = "TIO_List_Variables()";

  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyListObjects( subname, fileID, objectID, CLASS_NULL, CLASS_VAR,
                                   nobj, names);

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Create_Variable
 *
 **************************************************************************************************/
TIO_t
TIO_Create_Variable( const TIO_File_t   fileID,
                     const TIO_Object_t objectID,
                     const char         name[],
                     TIO_Object_t       *varID,
                     const TIO_Data_t   datatype,
                     const TIO_Dims_t   ndims,
                     const TIO_Size_t   dims[],
                     const char         units[] )
{
  const char *subname = "TIO_Create_Variable()";

  struct iFile_t     *tfile;
  struct iVariable_t *tvar;

  TIO_Object_t lvarID;
  TIO_Dims_t   di;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  *varID = TIO_NULL;    /* Ensure NULL value on failure */


  /* -- Check object-specific inbound args are valid */

  irc = iTIO_ValidDatatype(datatype);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
  }
  irc = iTIO_ValidDims(ndims, TRUE);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid ndims", TIO_ERR_INVALID_DIMS);
  }
  irc = iTIO_ValidString(units, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "units"), TIO_ERR_INVALID_STRING);
  }

  tvar = (struct iVariable_t *)TIOmalloc(sizeof(struct iVariable_t));
  if (NULL == tvar) {
    TIOreturn(ERR_MEM, "Failed to malloc cache", TIO_ERR_INT);
  }

  trc = cTIO_HierarchyCreateObject( subname, fileID, objectID, CLASS_NULL, CLASS_VAR, name,
                                    &tfile, &lvarID, tvar );
  if (trc != TIO_SUCCESS) {
    TIOfree (tvar); tvar = NULL;
    cTIOreturn (trc);
  }

  tvar->datatype = datatype;
  tvar->ndims    = ndims;
  for (di=TIO_0D; di<ndims; ++di) {
    tvar->dims[di] = dims[di];
  }
  (void) iTIO_strcpy(tvar->name, name);
  (void) iTIO_strcpy(tvar->units, units);


  /* -- Create dataset for the variable */
  irc = tfile->CreateDataset( fileID, objectID, lvarID, CLASS_VAR, name, 0, ndims, dims, datatype );
  if (irc < 0) {
    (void) iTIO_PutErr(ERR_INT, "Failed to create object dataset");
    trc = TIO_ERR_OBJECT_CREATE;
    goto cleandie;
  }


  /* -- Write the object attributes */
  irc = tfile->WriteAttr( fileID, objectID, lvarID, CLASS_VAR, "units", TIO_STRING,
                          (void *)&tvar->units );
  TIOassert(irc != 0, ERR_INT, "Failed to write attributes", TIO_ERR_INT);


  *varID = lvarID;

  TIOend(subname,1);

  return (TIO_SUCCESS);


 cleandie:
  trc = cTIO_HierarchyKillObject(subname, fileID, objectID, lvarID, CLASS_VAR);
  TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to kill object", TIO_ERR_INT);

  TIOfree (tvar); tvar = NULL;
  TIOreturn(iTIO_GetErrClass(), iTIO_GetErrStr(), trc);
}




/***************************************************************************************************
 *
 * TIO_Open_Variable
 * TIO_Open_Variable_idx
 *
 **************************************************************************************************/
TIO_t
TIO_Open_Variable( const TIO_File_t   fileID,
                   const TIO_Object_t objectID,
                   const char         name[],
                   TIO_Object_t       *varID,
                   TIO_Data_t         *datatype,
                   TIO_Dims_t         *ndims,
                   TIO_Size_t         dims[],
                   char               units[] )
{
  return ( cTIO_OpenVar("TIO_Open_Variable()", FALSE,
                        fileID, objectID, TIO_NULL, (char *)name,
                        varID, datatype, ndims, dims, units) );
}


TIO_t
TIO_Open_Variable_idx( const TIO_File_t   fileID,
                       const TIO_Object_t objectID,
                       const TIO_Size_t   idx,
                       char               name[],
                       TIO_Object_t       *varID,
                       TIO_Data_t         *datatype,
                       TIO_Dims_t         *ndims,
                       TIO_Size_t         dims[],
                       char               units[] )
{
  return ( cTIO_OpenVar("TIO_Open_Variable_idx()", TRUE,
                        fileID, objectID, idx, name,
                        varID, datatype, ndims, dims, units) );
}


static TIO_t
cTIO_OpenVar( const char         subname[],
              const int          ByIndex,
              const TIO_File_t   fileID,
              const TIO_Object_t objectID,
              const TIO_Size_t   idx,
              char               name[],
              TIO_Object_t       *varID,
              TIO_Data_t         *datatype,
              TIO_Dims_t         *ndims,
              TIO_Size_t         dims[],
              char               *units )
{
  struct iFile_t     *tfile;
  struct iVariable_t *tvar;

  TIO_Object_t lvarID;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  *varID = TIO_NULL;    /* Ensure NULL value on failure */


  tvar = (struct iVariable_t *)TIOmalloc(sizeof(struct iVariable_t));
  if (NULL == tvar) {
    TIOreturn(ERR_MEM, "Failed to malloc cache", TIO_ERR_INT);
  }

  trc = cTIO_HierarchyOpenObject( subname, fileID, objectID, CLASS_NULL, CLASS_VAR,
                                  name, ByIndex, idx,
                                  tvar->name, &tfile, &lvarID, (void *)tvar );
  if (trc != TIO_SUCCESS) {
    TIOfree (tvar); tvar = NULL;
    cTIOreturn (trc);
  }


  /* -- Open the dataset */
  irc = tfile->OpenDataset( fileID, objectID, lvarID, CLASS_VAR, tvar->name, 0,
                            &tvar->ndims, tvar->dims, &tvar->datatype );
  if (irc < 0) {
    (void) iTIO_PutErr(ERR_INT, "Failed to open object dataset");
    trc = TIO_ERR_OBJECT_OPEN;
    goto cleandie;
  }


  /* -- Read object attributes */
  irc = tfile->ReadAttr( fileID, objectID, lvarID, CLASS_VAR, "units", TIO_STRING,
                         (void *)tvar->units);
  TIOassert(irc != 0, ERR_INT, "Failed to read attributes", TIO_ERR_INT);


  irc = xTIO_GetVariableInfo(subname, ByIndex, tvar, name, datatype, ndims, dims, units);

  TIOassert(irc < 0, ERR_INT, "Failed to get variable info", TIO_ERR_INT);

  *varID = lvarID;

  TIOend(subname,1);

  return (TIO_SUCCESS);


 cleandie:
  trc = cTIO_HierarchyKillObject(subname, fileID, objectID, lvarID, CLASS_VAR);
  TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to kill object", TIO_ERR_INT);

  TIOfree (tvar); tvar = NULL;
  TIOreturn(iTIO_GetErrClass(), iTIO_GetErrStr(), trc);
}




/***************************************************************************************************
 *
 * TIO_Get_Variable_Info
 *
 **************************************************************************************************/
TIO_t
TIO_Get_Variable_Info( const TIO_File_t   fileID,
                       const TIO_Object_t varID,
                       char               name[],
                       TIO_Data_t         *datatype,
                       TIO_Dims_t         *ndims,
                       TIO_Size_t         dims[],
                       char               units[] )
{
  const char *subname = "TIO_Get_Variable_Info()";

  struct iVariable_t *tvar;

  int irc;
  TIO_t trc;

  TIObegin(subname,1);


  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, varID, CLASS_VAR, NULL, (void **)&tvar);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = xTIO_GetVariableInfo(subname, TRUE, tvar, name, datatype, ndims, dims, units);
  TIOassert(irc < 0, ERR_INT, "Failed to get variable info", TIO_ERR_INT);


  TIOend(subname,1);

  return (TIO_SUCCESS);
}



static int
xTIO_GetVariableInfo( const char         subname[],
                      const int          getname,
                      struct iVariable_t *tvar,
                      char               name[],
                      TIO_Data_t         *datatype,
                      TIO_Dims_t         *ndims,
                      TIO_Size_t         dims[],
                      char               units[] )
{
  int ii;


  if (getname && (name != NULL)) {
    (void) iTIO_strcpy(name, tvar->name);
  }

  TIOset(datatype, tvar->datatype);
  TIOset(ndims,    tvar->ndims);

  if (dims  != NULL) {
    for (ii=0; ii<tvar->ndims; ++ii) {
      dims[ii] = tvar->dims[ii];
    }
  }
  if (units != NULL) {
    (void) iTIO_strcpy(units, tvar->units);
  }

  return (0);
}




/***************************************************************************************************
 *
 * TIO_Close_Variable
 *
 **************************************************************************************************/
TIO_t
TIO_Close_Variable( const TIO_File_t   fileID,
                    const TIO_Object_t varID )
{
  const char *subname = "TIO_Close_Variable()";

  struct iFile_t     *tfile;
  struct iVariable_t *tvar;

  TIO_Object_t lpID;   /* parentID */
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyPreClose( subname, fileID, &lpID, varID, CLASS_VAR, &tfile, (void **)&tvar );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  /* -- Close the variable dataset */
  irc = tfile->CloseDataset( fileID, lpID, varID, CLASS_VAR, 0 );
  if (irc < 0) {
    TIOfree (tvar); tvar = NULL;
    TIOreturn(ERR_INT, "Failed to close object dataset", TIO_ERR_OBJECT_CLOSE);
  }

  /* -- Free the object cache */
  TIOfree (tvar); tvar = NULL;

  trc = cTIO_HierarchyCloseObject( subname, fileID, lpID, varID, CLASS_VAR );

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Write_Variable
 *
 **************************************************************************************************/
TIO_t
TIO_Write_Variable( const TIO_File_t   fileID,
                    const TIO_Object_t varID,
                    const TIO_Data_t   datatype,
                    const void         *vdata )
{
  const char *subname = "TIO_Write_Variable()";

  struct iFile_t     *tfile;
  struct iVariable_t *tvar;

  TIO_Object_t lpID;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, varID, CLASS_VAR,
                                     &tfile, (void **)&tvar);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  if (!tfile->writeable) {
     TIOreturn(ERR_USER, "Cannot write to pre-existing object", TIO_ERR_OBJECT_NOT_WRITEABLE);
  }

  irc = tfile->WriteDataset( fileID, lpID, varID, CLASS_VAR, 0,
                             TIO_XFER_INDEPENDENT, datatype, vdata );
  switch (irc) {
  case 0:
    /* ok */
    break;
  case ITIO_ERR_DATATYPE:
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
    break;
  default:
    TIOreturn(ERR_INT, "Failed to write variable", TIO_ERR_OBJECT_WRITE);
    break;
  }


  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_Variable
 *
 **************************************************************************************************/
TIO_t
TIO_Read_Variable( const TIO_File_t   fileID,
                   const TIO_Object_t varID,
                   const TIO_Data_t   datatype,
                   void               *vdata )
{
  const char *subname = "TIO_Read_Variable()";

  struct iFile_t     *tfile;
  struct iVariable_t *tvar;

  TIO_Object_t lpID;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, varID, CLASS_VAR,
                                     &tfile, (void **)&tvar);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = tfile->ReadDataset( fileID, lpID, varID, CLASS_VAR, 0,
                            TIO_XFER_INDEPENDENT, datatype, vdata );
  switch (irc) {
  case 0:
    /* ok */
    break;
  case ITIO_ERR_DATATYPE:
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
    break;
  default:
    TIOreturn(ERR_INT, "Failed to read variable", TIO_ERR_OBJECT_READ);
    break;
  }

  TIOend(subname,1);


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Read_Variable_Section
 *
 **************************************************************************************************/
TIO_t
TIO_Read_Variable_Section( const TIO_File_t   fileID,
                           const TIO_Object_t varID,
                           const TIO_Data_t   datatype,
                           const TIO_Dims_t   ndims,
                           const TIO_Size_t   lolimit[],
                           const TIO_Size_t   hilimit[],
                           void               *vdata )
{
  const char *subname = "TIO_Read_Variable_Section()";

  struct iFile_t     *tfile;
  struct iVariable_t *tvar;

  TIO_Size_t   slabinfo[TIO_MAXDIMS][2];
  TIO_Dims_t   dim;

  TIO_Object_t lpID;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, &lpID, varID, CLASS_VAR,
                                     &tfile, (void **)&tvar);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  /* -- Check the arguments */

  if (ndims != tvar->ndims) {
    TIOreturn(ERR_USER, "Invalid ndims - does not match that of variable", TIO_ERR_INVALID_ARG);
  }
  if (NULL == lolimit) {
    TIOreturn(ERR_USER, "lolimit must be given", TIO_ERR_INVALID_ARG);
  }
  if (NULL == hilimit) {
    TIOreturn(ERR_USER, "hilimit must be given", TIO_ERR_INVALID_ARG);
  }

  for (dim=TIO_0D; dim<ndims; ++dim) {

    if (lolimit[dim] > tvar->dims[dim]) {
      TIOreturn(ERR_USER, "Invalid range - lolimit > upper limit of variable",
                TIO_ERR_INVALID_RANGE);
    }
    if (hilimit[dim] > tvar->dims[dim]) {
      TIOreturn(ERR_USER, "Invalid range - lolimit > upper limit of variable",
                TIO_ERR_INVALID_RANGE);
    }
    if (lolimit[dim] > hilimit[dim]) {
      TIOreturn(ERR_USER, "Invalid range - lolimit > hilimit", TIO_ERR_INVALID_RANGE);
    }
    slabinfo[dim][0] = lolimit[dim];
    slabinfo[dim][1] = hilimit[dim];
  }

  irc = tfile->ReadDatasetSlab( fileID, lpID, varID, CLASS_VAR, 0,
                                TIO_XFER_INDEPENDENT, datatype,
                                ndims, (const TIO_Size_t(*)[2])slabinfo, vdata );
  switch (irc) {

  case 0:
    /* ok */
    break;

  case ITIO_ERR_DATATYPE:
    TIOreturn(ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
    break;

  case ITIO_ERR_DIMS:
    TIOreturn(ERR_USER, "Invalid range - section is bigger than the variable", TIO_ERR_INVALID_RANGE);
    break;

  case ITIO_ERR_READ:
    TIOreturn(ERR_INT, "Failed to read variable", TIO_ERR_OBJECT_READ);
    break;

  default:
    TIOreturn(ERR_INT, "Failed to read variable", TIO_ERR_OBJECT_READ);
  }


  TIOend(subname,1);


  return (TIO_SUCCESS);
}



/*
 * EOF
 */

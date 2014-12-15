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

#include "typhonio.h"     /* For public API */

/*
 * Define internal hierarchy globally
 * - this makes it very easy to add further child objects to parents
 *
 * Full definition and values are filled in tio_global.c
 *
 */


#define TIO_NUM_OBJECTS  7
#define TIO_MAX_CHILDREN 4


static const iClass_t xHierarchy[TIO_NUM_OBJECTS][TIO_MAX_CHILDREN] = {
  /* file       */  { CLASS_VAR,  CLASS_VGROUP, CLASS_STATE, CLASS_NULL },
  /* state      */  { CLASS_VAR,  CLASS_VGROUP, CLASS_MESH,  CLASS_NULL },
  /* mesh       */  { CLASS_VAR,  CLASS_VGROUP, CLASS_MATERIAL, CLASS_QUANT },
  /* material   */  { CLASS_VAR,  CLASS_VGROUP, CLASS_NULL,  CLASS_NULL },
  /* quant      */  { CLASS_VAR,  CLASS_VGROUP, CLASS_NULL,  CLASS_NULL },
  /* vgroup     */  { CLASS_VAR,  CLASS_VGROUP, CLASS_NULL,  CLASS_NULL },
  /* var        */  { CLASS_NULL, CLASS_NULL,   CLASS_NULL,  CLASS_NULL } };

static const int xNChildren[TIO_NUM_OBJECTS] = {
  /* file       */   3,
  /* state      */   3,
  /* mesh       */   4,
  /* material   */   2,
  /* quant      */   2,
  /* vgroup     */   2,
  /* var        */   0  };

static const char *xNames[TIO_NUM_OBJECTS] = {
  /* file       */                                    "FILE",
  /* state      */                                        "STATE",
  /* mesh       */                                  /*  |  */ "MESH",
  /* material   */                                  /*  |   |  */ "MATERIAL",
  /* quant      */                                  /*  |   |   |  */ "QUANT",
  /* vgroup     */                                  /*  |   |   |   |  */ "VGROUP",
  /* var        */                                  /*  |   |   |   |   |  */ "VARIABLE"
};
static const TIO_Size_t xSize    [TIO_NUM_OBJECTS] = {  0,  50, 20, 1,  50,  5,  10 };
static const TIO_Size_t xDelta   [TIO_NUM_OBJECTS] = {  0,  20, 10, 1,  20,  5,  50 };
static const int        xPriority[TIO_NUM_OBJECTS] = {  -1, 2,  2,  3,  2,   1,  0  };
#define Class2Hierarchy(class) ((int)class + 1)


static int InitCount = 0;




/**************************************************************************************************/


static int
xGetNChildren( const iClass_t class )
{
  return ( xNChildren[Class2Hierarchy(class)] );
}


static iClass_t
xGetHierarchy( const iClass_t class,
               const int      count )
{
  return ( xHierarchy[Class2Hierarchy(class)][count] );
}


static int
xMyChild( const iClass_t parent,
          const iClass_t child )
{
  int ii, irc;

  irc = FALSE;

  for (ii=0; ii<xGetNChildren(parent); ++ii) {
    if ( xGetHierarchy(parent,ii) == child) {
      irc = TRUE;
      break;
    }
  }

  return (irc);
}


/**************************************************************************************************/

static int
xGetParentClass( const char         *subname,
                 const TIO_File_t   fileID,
                 const TIO_Object_t parentID,
                 const iClass_t     parentclass,
                 iClass_t           *class )
{
  iClass_t lpclass;
  int      irc;

  *class = CLASS_NULL;

  if (parentID == TIO_NULL) {           /* Parent is the file */
    lpclass = CLASS_FILE;
  }
  else {
    if (parentclass == CLASS_NULL) {    /* unknown parent class
                                           (eg. anything that can contain var / vgroup ) */
      irc = iTIO_GetClassObject(fileID, parentID, &lpclass, NULL);
      if (irc < 0) {
        TIOreturn(ERR_USER, "Invalid object ID - no object found", irc);
      }
    }
    else {
      irc = iTIO_GetObject(fileID, NULL, parentID, parentclass, NULL);
      if (irc < 0) {
        TIOreturn(ERR_USER, "Invalid object ID - no object found", irc);
      }
      lpclass = parentclass;
    }
  }

  *class = lpclass;

  return (0);
}




/***************************************************************************************************
 *
 * iTIO_InitHierarchy
 * iTIO_KillHierarchy
 *
 **************************************************************************************************/
int iTIO_InitHierarchy( void )
{
  if (InitCount == 0) {
    /* May need to do something a bit more clever in future */
  }

  ++ InitCount;

  return (0);
}

int iTIO_KillHierarchy( void )
{
  --InitCount;
  if (InitCount > 0) {
    return (0);
  }

  /* else continue to clean up anything that may be needed in future */

  return (0);
}




/***************************************************************************************************
 *
 * iTIO_HierarchyGetNumChildren
 *
 **************************************************************************************************/
int iTIO_HierarchyGetNumChildren( const iClass_t class, int *nchildren )
{
  *nchildren = xGetNChildren(class);

  return (0);
}




/***************************************************************************************************
 *
 * iTIO_HierarchyGetName
 *
 **************************************************************************************************/
int iTIO_HierarchyGetName( const iClass_t class, char **name )
{
  *name = (char *)xNames[Class2Hierarchy(class)];

  return (0);
}




/***************************************************************************************************
 *
 * iTIO_HierarchyGetSize
 * iTIO_HierarchyGetDelta
 * iTIO_HierarchyGetPriority
 *
 **************************************************************************************************/
int iTIO_HierarchyGetSize( const iClass_t class, TIO_Size_t *size )
{
  *size = xSize[Class2Hierarchy(class)];

  return (0);
}

int iTIO_HierarchyGetDelta( const iClass_t class, TIO_Size_t *delta )
{
  *delta = xDelta[Class2Hierarchy(class)];

  return (0);
}

int iTIO_HierarchyGetPriority( const iClass_t class, int *priority )
{
  *priority = xPriority[Class2Hierarchy(class)];

  /* Priority simply states the order of objects within a parent */

  return (0);
}



/***************************************************************************************************
 *
 * iTIO_HierarchyGetObjectName
 *
 **************************************************************************************************/
int iTIO_HierarchyGetObjectName( const TIO_File_t fileID,
                                 const TIO_Object_t objectID,
                                 char **name )
{
  iClass_t lclass;

  void     *tobject;
  int      irc;

  /* returns a pointer to name of given object */

  irc = iTIO_GetClassObject( fileID, objectID, &lclass, &tobject);


  switch (lclass) {
  case CLASS_VAR:
    {
      struct iVariable_t *tobj = (struct iVariable_t *)tobject;
      *name = tobj->name;
    }
    break;
  case CLASS_VGROUP:
    {
      struct iVgroup_t *tobj = (struct iVgroup_t *)tobject;
      *name = tobj->name;
    }
    break;
  case CLASS_QUANT:
    {
      struct iQuant_t *tobj = (struct iQuant_t *)tobject;
      *name = tobj->name;
    }
    break;
  case CLASS_MATERIAL:
    {
      struct iMaterial_t *tobj = (struct iMaterial_t *)tobject;
      *name = tobj->name;
    }
    break;
  case CLASS_MESH:
    {
      struct iMesh_t *tobj = (struct iMesh_t *)tobject;
      *name = tobj->name;
    }
    break;
  case CLASS_STATE:
    {
      struct iState_t *tobj = (struct iState_t *)tobject;
      *name = tobj->name;
    }
    break;
  default:
    irc = TIO_FAIL;
    break;
  }

  return (0);
}



/***************************************************************************************************
 *
 * cTIO_HierarchyCheckObject
 *
 **************************************************************************************************/

/*
 *  To make this a bit more efficient, I have grabbed the references directly from HDF5 routines,
 *  rather than reading and making a copy.
 *
 *  In future version:  Pull out reference caches from HDF5 code area and bring into register (?).
 *                      HDF5 stuff should ONLY do file ops.
 */

#include "tio_xhdf5_int.h"

static int xCheckIndexInRef( const char         *subname,
                             const TIO_File_t   fileID,
                             const TIO_Object_t parentID,
                             const iClass_t     parentclass,
                             const iClass_t     objectclass,
                             const TIO_Size_t   idx )
{
  int irc;

  struct xH5ref_t *ref;

  irc = xH5_GetRef(fileID, parentID, NULL, objectclass, &ref);
  TIOassert(irc < 0, ERR_INT, "Failed to get reference struct", -1);

  if (idx > ref->n) {
    return (1);
  }
  else {
    return (0);
  }
}

static int xCheckObjectInRef( const char         *subname,
                              const TIO_File_t   fileID,
                              const TIO_Object_t parentID,
                              const iClass_t     parentclass,
                              const iClass_t     objectclass,
                              const char         *name )
{
  int irc;
  int ii;
  int notfound = 1;

  struct xH5ref_t *ref;

  irc = xH5_GetRef(fileID, parentID, NULL, objectclass, &ref);
  TIOassert(irc < 0, ERR_INT, "Failed to get reference struct", -1);

  for (ii=0; ii<ref->n; ++ii) {
    if (strcmp(ref->buf[ii], name) == 0) {
      notfound = 0;
      break;
    }
  }

  return (notfound);   /* strange to return 0 if found, but means 0=success, !0=fail|error */
}


TIO_t
cTIO_HierarchyCheckObject( const char         *subname,
                           const TIO_File_t   fileID,
                           const TIO_Object_t parentID,
                           const char         *name )
{
  iClass_t lpclass;

  int nref;
  int iref;
  int found;
  int irc;


  irc = xGetParentClass( subname, fileID, parentID, CLASS_NULL, &lpclass );
  switch (irc) {
  case 0:
    break; /* no error */
  case ITIO_ERR_OBJECTID:
    TIOreturn(ERR_USER, "Invalid parent ID", TIO_ERR_INVALID_OBJECTID);
    break;
  default:
    TIOreturn(ERR_INT, "Internal error", TIO_ERR_INT);
    break;
  }

  nref  = xGetNChildren(lpclass);
  found = 0;

  for (iref=0; iref<nref; ++iref) {

    irc =  xCheckObjectInRef(subname, fileID, parentID, lpclass, xGetHierarchy(lpclass,iref), name);
    switch (irc) {
    case 0:
      found = 1;
      break;
    case 1:
      break;
    default:
      TIOreturn(ERR_INT, "Failed to interrogate parent reference - internal error", TIO_ERR_INT);
      break;
    }
  }

  if (found) {
    return(TIO_SUCCESS);
  }
  else {
    TIOreturn(ERR_USER, "Object does not exist in the parent", TIO_ERR_OBJECT_NOT_EXIST);
  }
}



/***************************************************************************************************
 *
 * cTIO_HierarchyGetObjectInfo
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyGetObjectInfo( const char         *subname,
                             const TIO_File_t   fileID,
                             TIO_Object_t       *parentID,
                             const TIO_Object_t objectID,
                             const iClass_t     class,
                             struct iFile_t     **tfile,
                             void               **objinfo )
{
  int irc;

  irc = iTIO_GetFile(fileID, tfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid file ID", TIO_ERR_INVALID_FILEID);
  }

  irc = iTIO_GetObject(fileID, parentID, objectID, class, objinfo);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid object ID", TIO_ERR_INVALID_OBJECTID);
  }

  return (TIO_SUCCESS);
}



/***************************************************************************************************
 *
 * cTIO_HierarchyGetChunkedObjectInfo
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyGetChunkedObjectInfo( const char         *subname,
                                    const TIO_File_t   fileID,
                                    const TIO_Object_t objectID,
                                    iClass_t           *class,
                                    TIO_Dims_t         *ndims,
                                    TIO_Size_t         *nghosts,
                                    int                *ismixed,
                                    TIO_Size_t         *nchunks,
                                    union iInfo_t      **chunkinfo)
{
  struct iMesh_t     *tmesh;

  void               *tobject;

  iClass_t lclass;

  TIO_Object_t meshID;     /* mesh ID */

  int irc;


  irc = iTIO_GetFile(fileID, NULL);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid fileID", TIO_ERR_INVALID_FILEID);
  }

  /* -- Get info for object (class unknown to start with) and it's parent ID */

  irc = iTIO_GetClassObject(fileID, objectID, &lclass, (void **)&tobject);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid objectID", TIO_ERR_INVALID_OBJECTID);
  }


  if (lclass != CLASS_MESH) {

    irc = iTIO_GetObject(fileID, &meshID, objectID, lclass, NULL);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to get parent", TIO_ERR_INT);
    }
  }
  else {
    meshID = objectID;
  }


  irc = iTIO_GetObject(fileID, NULL, meshID, CLASS_MESH, (void **)&tmesh);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid objectID - is not direct child object of mesh",
              TIO_ERR_INVALID_OBJECTID);
  }

  switch (lclass) {

  case CLASS_MESH:

    TIOset( nghosts, TIO_GHOSTS_ALL );
    TIOset( ismixed, TIO_FALSE );

    break;

  case CLASS_MATERIAL:
    {
      struct iMaterial_t *tmat = tobject;

      TIOset( nghosts, tmat->nghosts);
      TIOset( ismixed, tmat->ismixed);
    }
    break;

  case CLASS_QUANT:
    {
      struct iQuant_t *tquant = tobject;

      TIOset( nghosts, tquant->nghosts );
      TIOset( ismixed, tquant->ismixed );
    }
    break;


  default:
    TIOreturn(ERR_USER, "Invalid objectID - object cannot contain chunks",
              TIO_ERR_INVALID_OBJECTID);
    break;
  }

  TIOset( class,      lclass );
  TIOset( ndims,      tmesh->ndims );
  TIOset( nchunks,    tmesh->nchunks );
  /*TIOset( *chunkinfo, tmesh->chunkinfo );*/

  if (chunkinfo != NULL ) {
    *chunkinfo = tmesh->chunkinfo;
  }



  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_HierarchyListObjects
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyListObjects( const char         *subname,
                           const TIO_File_t   fileID,
                           const TIO_Object_t parentID,
                           const iClass_t     parentclass,
                           const iClass_t     objectclass,
                           TIO_Size_t         *nobj,
                           char               names[][TIO_STRLEN] )
{
  struct iFile_t *lfile;
  int            irc;
  iClass_t       lpclass;
  TIO_Size_t     lnobj;


  irc = iTIO_GetFile(fileID, &lfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid file ID", TIO_ERR_INVALID_FILEID);
  }

  irc = xGetParentClass( subname, fileID, parentID, parentclass, &lpclass );
  switch (irc) {
  case 0:
    break; /* no error */
  case ITIO_ERR_OBJECTID:
    TIOreturn(ERR_USER, "Invalid object ID", TIO_ERR_INVALID_OBJECTID);
    break;
  default:
    TIOreturn(ERR_INT, "Internal error", TIO_ERR_INT);
    break;
  }

  irc = xMyChild(lpclass, objectclass);
  if (!irc) {
    TIOreturn(ERR_USER, "Invalid parent/child combination", TIO_ERR_INVALID_OPERATION);
  }

  irc = lfile->ReadRef(fileID, parentID, objectclass, &lnobj, names);
  TIOassert(irc < 0, ERR_INT, "Failed to read reference", TIO_ERR_INT);

  TIOset(nobj, lnobj);


  return (TIO_SUCCESS);

  /* ****
     For future TyphonIO v2.x
     -- nobj should be positive if 1 or more objects of the class exist

     if (lnobj > 0) {
       return (TIO_SUCCESS);
     }
     else {
       return (TIO_ERR_OBJECT_NOT_EXIST);
     }
  **** */
}



/***************************************************************************************************
 *
 * cTIO_HierarchyPreClose
 *
 **************************************************************************************************/

static TIO_Object_t    store_objectID = TIO_NULL;
static struct iFile_t *store_lfile    = NULL;

TIO_t
cTIO_HierarchyPreClose( const char         *subname,
                        const TIO_File_t   fileID,
                        TIO_Object_t       *parentID,
                        const TIO_Object_t objectID,
                        const iClass_t     class,
                        struct iFile_t     **tfile,
                        void               **objinfo )
{
  struct iFile_t *lfile;

  int irc;


  irc = iTIO_GetFile(fileID, &lfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid file ID", TIO_ERR_INVALID_FILEID);
  }

  if (tfile != NULL) {
    *tfile = lfile;
  }

  if (class != CLASS_FILE) {
    irc = iTIO_GetObject(fileID, parentID, objectID, class, objinfo);
    if (irc < 0) {
      TIOreturn(ERR_USER, "Invalid object ID", TIO_ERR_INVALID_OBJECTID);
    }
  }

  store_lfile    = lfile;
  store_objectID = objectID;

  return (TIO_SUCCESS);
}



/***************************************************************************************************
 *
 * cTIO_HierarchyCreateFile
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyCreateFile( const char         *subname,
                          const TIO_File_t   fileID,
                          const char         *filename,
                          const TIO_Access_t access,
                          const MPI_Comm     comm,
                          const MPI_Info     info,
                          const int          rank,
                          struct iFile_t     *tfile )
{
  int iref;
  int irc;


  /* -- Create the file */

  irc = tfile->CreateFile( fileID, filename, access, comm, info, rank );

  switch (irc) {
  case 0:
    /* success */
    break;

  case ITIO_ERR_ARG:
    TIOreturn(ERR_USER, "Invalid access value - use TIO_ACC_CREATE or TIO_ACC_REPLACE",
              TIO_ERR_INVALID_ARG);
    break;

  case ITIO_ERR_CREATE:
    TIOreturn(ERR_INT,
              "Check file doesn't exist, filename and/or permissions",
              TIO_ERR_FILE_CREATE);
    break;

  default:
    TIOassert(irc < 0, ERR_INT, "Failed to initialize file", TIO_ERR_INT);
    break;
  }


  /* -- Create the empty reference objects for children */
  for (iref=0; iref<xGetNChildren(CLASS_FILE); ++iref) {
    irc = tfile->InitRef( fileID, TIO_NULL, xGetHierarchy(CLASS_FILE,iref), FALSE );
    TIOassert(irc < 0, ERR_INT, "Failed to create reference", TIO_ERR_INT);
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_HierarchyOpenFile
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyOpenFile( const char         *subname,
                        const TIO_File_t   fileID,
                        const char         *filename,
                        const TIO_Access_t access,
                        const MPI_Comm     comm,
                        const MPI_Info     info,
                        const int          rank,
                        struct iFile_t     *tfile )
{
  int irc;
  int iref;


  /* -- Open the file */

  irc = tfile->OpenFile( fileID, filename, access, comm, info, rank );

  switch (irc) {

  case 0:
    /* success */
    break;

  case ITIO_ERR_ARG:
    TIOreturn(ERR_USER, "Invalid access value - use TIO_ACC_READWRITE or TIO_ACC_READONLY",
              TIO_ERR_INVALID_ARG);
    break;

  case ITIO_ERR_OPEN:
    TIOreturn(ERR_INT,   "File open error.  Check file exists, filename and/or permissions",
              TIO_ERR_FILE_OPEN);
      break;

  default:
    TIOassert(irc < 0, ERR_INT, "Failed to initialize file", TIO_ERR_INT);
  }


  /* -- Create the empty reference objects for children */
  for (iref=0; iref<xGetNChildren(CLASS_FILE); ++iref) {
    irc = tfile->InitRef( fileID, TIO_NULL, xGetHierarchy(CLASS_FILE,iref), TRUE);
    TIOassert(irc < 0, ERR_INT, "Failed to create reference", TIO_ERR_INT);
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_HierarchyCloseFile
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyCloseFile( const char       *subname,
                         const TIO_File_t fileID)
{
  struct iFile_t *lfile;
  iClass_t class;

  int iref;
  int irc;
  TIO_t trc;


  irc = iTIO_GetFile(fileID, &lfile);
  if (irc < 0) {
    if (irc == ITIO_ERR_FILEID) {
      TIOreturn(ERR_USER, "Invalid fileID", TIO_ERR_INVALID_FILEID);
    }
    else {
      TIOassert(TRUE, ERR_INT, "Failed to get file", TIO_ERR_INT);
    }
  }

  class = CLASS_MAX;

  while (class > CLASS_FILE) {

    /* Looping backwards through classes ensures child objects are closed before parents */
    TIO_Object_t objID;

    irc = iTIO_GetAnyObject(fileID, class, &objID);
    if (irc > 0) {

      /* TO-DO:  Print out a warning here to stderr */

      switch (class) {
      case CLASS_VAR:
        trc = TIO_Close_Variable(fileID, objID);
        break;
      case CLASS_VGROUP:
        trc = TIO_Close_Vargroup(fileID, objID);
        break;
      case CLASS_QUANT:
        trc = TIO_Close_Quant(fileID, objID);
        break;
      case CLASS_MATERIAL:
        trc = TIO_Close_Material(fileID, objID);
        break;
      case CLASS_MESH:
        trc = TIO_Close_Mesh(fileID, objID);
        break;
      case CLASS_STATE:
        trc = TIO_Close_State(fileID, objID);
        break;
      default:
        trc = TIO_FAIL;
        break;
      }

      if (trc != TIO_SUCCESS) {
        TIOreturn(ERR_INT, "Failed to close up a dangling object", TIO_ERR_INT);
      }
    }
    else {
      TIOassert(irc < 0, ERR_INT, "Failed whilst trying to get any object", TIO_ERR_INT);

      --class;  /* Nothing needs closing of this class */
    }

  }


  for (iref=0; iref<xGetNChildren(class); ++iref) {
    irc = lfile->FinalizeRef( fileID, TIO_NULL,  xGetHierarchy(class, iref) );
    TIOassert(irc < 0, ERR_INT, "Failed to close reference", TIO_ERR_INT);
  }


  irc = lfile->CloseFile( fileID );
  if (irc < 0) {
    TIOfree (lfile);  lfile = NULL;
    TIOreturn(ERR_INT, "Failed to close file", TIO_ERR_FILE_CLOSE);
  }

  irc = lfile->Kill();
  TIOassert(irc < 0, ERR_INT, "Failed to kill", TIO_ERR_INT);

  irc = iTIO_DeregisterFile(fileID);
  TIOassert(irc < 0, ERR_INT, "Failed to deregister file", TIO_ERR_INT);

  TIOfree (lfile); lfile = NULL;

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_HierarchyFlushFile
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyFlushFile( const char       *subname,
                         const TIO_File_t fileID)
{
  struct iFile_t *lfile;

  TIO_Object_t objID;
  iClass_t     class;
  int          iref;
  int          irc;                          /* Return code from internal routines */



  /* Flush all internal TyphonIO references, variables cache and objects */
  /* Flush references by finalizing and re-opening */
  /* Variable caches are finalized automatically when finalizing the variable reference */


  irc = iTIO_GetFile(fileID, &lfile);
  if (irc < 0) {
    if (irc == ITIO_ERR_FILEID) {
      TIOreturn(ERR_USER, "Invalid fileID", TIO_ERR_INVALID_FILEID);
    }
    else {
      TIOassert(TRUE, ERR_INT, "Failed to get file", TIO_ERR_INT);
    }
  }



  class = CLASS_MAX;

  while (class > CLASS_FILE) {

    irc = iTIO_GetAnyObject(fileID, class, &objID);
    if (irc > 0) {

      for (iref=0; iref<xGetNChildren(class); ++iref) {
        irc = lfile->FinalizeRef( fileID, objID, xGetHierarchy(class,iref) );
        TIOassert(irc < 0, ERR_INT, "Failed to finalize reference", TIO_ERR_INT);

        irc = lfile->InitRef( fileID, objID, xGetHierarchy(class,iref), TRUE );
        TIOassert(irc < 0, ERR_INT, "Failed to init reference", TIO_ERR_INT);
      }

    }
    else {
      TIOassert(irc < 0, ERR_INT, "Failed whilst trying to get any object", TIO_ERR_INT);
    }


    --class;  /* Nothing needs closing of this class */

  }


  /* Base file references */

  for (iref=0; iref<xGetNChildren(class); ++iref) {
    irc = lfile->FinalizeRef( fileID, TIO_NULL, xGetHierarchy(CLASS_FILE,iref) );
    TIOassert(irc < 0, ERR_INT, "Failed to finalize reference", TIO_ERR_INT);

    irc = lfile->InitRef( fileID, TIO_NULL, xGetHierarchy(CLASS_FILE,iref), TRUE );
    TIOassert(irc < 0, ERR_INT, "Failed to init reference", TIO_ERR_INT);
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_HierarchyCreateObject
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyCreateObject( const char         *subname,
                            const TIO_File_t   fileID,
                            const TIO_Object_t parentID,
                            const iClass_t     parentclass,
                            const iClass_t     objectclass,
                            const char         *name,
                            struct iFile_t     **tfile,
                            TIO_Object_t       *objectID,
                            void               *objinfo )
{
  struct iFile_t *lfile;

  TIO_Object_t   lobjID;
  iClass_t       lpclass;
  int            iref;
  int            irc;


  *tfile    = NULL;
  *objectID = TIO_NULL;

  irc = iTIO_GetFile(fileID, &lfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid file ID", TIO_ERR_INVALID_FILEID);
  }
  if (!lfile->writeable) {
    TIOreturn(ERR_USER, "Cannot create object in read-only file", TIO_ERR_FILE_NOT_WRITEABLE);
  }

  /* -- Check parent/child combination is ok */

  irc = xGetParentClass( subname, fileID, parentID, parentclass, &lpclass );
  switch (irc) {
  case 0:
    break; /* no error */
  case ITIO_ERR_OBJECTID:
    TIOreturn(ERR_USER, "Invalid object ID", TIO_ERR_INVALID_OBJECTID);
    break;
  default:
    TIOreturn(ERR_INT, "Internal error", TIO_ERR_INT);
    break;
  }

  irc = xMyChild(lpclass, objectclass);
  if (!irc) {
    TIOreturn(ERR_USER, "Invalid parent/child combination", TIO_ERR_INVALID_OPERATION);
  }

  /* -- Check name string is valid */
  irc = iTIO_ValidString(name, TRUE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "name"), TIO_ERR_INVALID_NAME);
  }

  /* -- Register the object - this returns TyphonIO object ID */
  irc = iTIO_RegisterObject(fileID, parentID, &lobjID, objectclass, objinfo);
  TIOassert(irc < 0, ERR_INT, "Failed to register object", TIO_ERR_INT);


  /* -- Create the new object */
  irc = lfile->CreateObject( fileID, parentID, lobjID, objectclass, name );
  switch (irc) {
  case 0:
    /* No error */
    break;

  default:
    {
      int irc2;

      irc2 = iTIO_DeregisterObject(fileID, lobjID);
      TIOassert(irc2 < 0, ERR_INT, "Failed to deregister object", TIO_ERR_INT);

      switch (irc) {
      case ITIO_ERR_FILEID:
        TIOreturn(ERR_INT, "Failed to create object - invalid file ID", TIO_ERR_INVALID_FILEID);
        break;
      case ITIO_ERR_PARENTID:
        TIOreturn(ERR_INT, "Failed to create object - invalid parent ID", TIO_ERR_INVALID_OBJECTID);
        break;
      case ITIO_ERR_CREATE:
        TIOreturn(ERR_INT, "Failed to create object - may already exist", TIO_ERR_OBJECT_CREATE);
        break;

      default:
        TIOreturn(ERR_INT, "Failed to create object - internal error", TIO_ERR_INT);
        break;
      }
    }
    break;
  }

  /* -- Create the empty reference objects for children */
  for (iref=0; iref<xGetNChildren(objectclass); ++iref) {
    irc = lfile->InitRef( fileID, lobjID, xGetHierarchy(objectclass,iref), FALSE );
    TIOassert(irc < 0, ERR_INT, "Failed to create reference", TIO_ERR_INT);
  }

  /* -- Update the references in the parent object */
  irc = lfile->AddRef( fileID, parentID, lobjID, objectclass, name);
  TIOassert(irc < 0, ERR_INT, "Failed to update reference", TIO_ERR_INT);

  *tfile    = lfile;
  *objectID = lobjID;

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_HierarchyOpenObject
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyOpenObject( const char         *subname,
                          const TIO_File_t   fileID,
                          const TIO_Object_t parentID,
                          const iClass_t     parentclass,
                          const iClass_t     objectclass,
                          const char         *name,
                          const int          byindex,
                          const TIO_Size_t   idx,
                          char               *outname,
                          struct iFile_t     **tfile,
                          TIO_Object_t       *objectID,
                          void               *objinfo )
{
  struct iFile_t *lfile;

  TIO_Object_t   lobjID;
  iClass_t       lpclass;
  int            iref;
  int            irc;

  char lname[TIO_STRLEN];


  *tfile    = NULL;
  *objectID = TIO_NULL;

  irc = iTIO_GetFile(fileID, &lfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid file ID", TIO_ERR_INVALID_FILEID);
  }

  /* -- Check parent/child combination is ok */
  irc = xGetParentClass( subname, fileID, parentID, parentclass, &lpclass );
  switch (irc) {
  case 0:
    break; /* no error */
  case ITIO_ERR_OBJECTID:
    TIOreturn(ERR_USER, "Invalid object ID", TIO_ERR_INVALID_OBJECTID);
    break;
  default:
    TIOreturn(ERR_INT, "Internal error", TIO_ERR_INT);
    break;
  }
  irc = xMyChild(lpclass, objectclass);
  if (!irc) {
    TIOreturn(ERR_USER, "Invalid parent/child combination", TIO_ERR_INVALID_OPERATION);
  }

  if (!byindex) {

    /* -- Check string for name is valid */
    irc = iTIO_ValidString(name, TRUE);
    if (irc < 0) {
      TIOreturn(ERR_USER, iTIO_GetStringError(irc, "name"), TIO_ERR_INVALID_NAME);
    }
    (void) iTIO_strcpy(lname, name);

    /* -- Check object being opened exists in the parent's references */
    irc = xCheckObjectInRef(subname, fileID, parentID, lpclass, objectclass, name);
    switch (irc) {
    case 0:
      /* No error */
      break;
    case 1:
      TIOreturn(ERR_USER, "Object does not exist in the parent", TIO_ERR_OBJECT_NOT_EXIST);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to interrogate parent reference - internal error", TIO_ERR_INT);
      break;
    }
  }
  else {

    /* -- Check index value is valid with respect to the parent's reference */
    irc = xCheckIndexInRef(subname, fileID, parentID, lpclass, objectclass, idx);
    switch (irc) {
    case 0:
      /* No error */
      break;
    case 1:
      TIOreturn(ERR_USER, "Index too greater than number of objects in the parent",
                TIO_ERR_INVALID_INDEX);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to interrogate parent reference - internal error", TIO_ERR_INT);
      break;
    }

  }


  /* -- Register the object - this returns TyphonIO object ID */
  irc = iTIO_RegisterObject(fileID, parentID, &lobjID, objectclass, objinfo);
  TIOassert(irc < 0, ERR_INT, "Failed to register object", TIO_ERR_INT);


  /* -- Open the object
        If this fails, then object may not exist in the file, so exit cleanly and free up stuff */
  irc = lfile->OpenObject( fileID, parentID, lobjID, objectclass, byindex, lname, idx );
  switch (irc) {
  case 0:
    /* No error */
    break;

  default:
    {
      int irc2;

      irc2 = iTIO_DeregisterObject(fileID, lobjID);
      TIOassert(irc2 < 0, ERR_INT, "Failed to deregister object", TIO_ERR_INT);

      switch (irc) {
      case ITIO_ERR_FILEID:
        TIOreturn(ERR_USER, "Failed to open object - invalid file ID",    TIO_ERR_INVALID_FILEID);
        break;
      case ITIO_ERR_PARENTID:
        TIOreturn(ERR_USER, "Failed to open object - invalid parent ID",  TIO_ERR_INVALID_OBJECTID);
        break;
      case ITIO_ERR_OPEN:
        TIOreturn(ERR_INT,  "Failed to open object - may not exist",      TIO_ERR_OBJECT_OPEN);
        break;
      case ITIO_ERR_INDEX:
        TIOreturn(ERR_USER, "Failed to open object - invalid idx value ", TIO_ERR_INVALID_INDEX);
        break;

      default:
        TIOreturn(ERR_INT,  "Failed to open object - internal error",     TIO_ERR_INT);
        break;
      }
    }
    break;
  }


  /* -- Open the reference objects for children */
  for (iref=0; iref<xGetNChildren(objectclass); ++iref) {
    irc = lfile->InitRef( fileID, lobjID, xGetHierarchy(objectclass, iref), TRUE );
    TIOassert(irc < 0, ERR_INT, "Failed to open reference", TIO_ERR_INT);
  }


  (void) iTIO_strcpy(outname, lname);
  *tfile    = lfile;
  *objectID = lobjID;

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_HierarchyCloseObject
 *
 **************************************************************************************************/
TIO_t
cTIO_HierarchyCloseObject( const char        *subname,
                           const TIO_File_t   fileID,
                           const TIO_Object_t parentID,
                           const TIO_Object_t objectID,
                           const iClass_t     class )
{
  struct iFile_t *lfile;

  int iref;
  int irc;


  if (objectID != store_objectID) {
    TIOreturn (ERR_INT, "objectID does not match that of pre-close", TIO_ERR_INT);
  }

  lfile          = store_lfile;
  store_lfile    = NULL;
  store_objectID = TIO_NULL;


  for (iref=0; iref<xGetNChildren(class); ++iref) {
    irc = lfile->FinalizeRef( fileID, objectID,  xGetHierarchy(class, iref) );
    TIOassert(irc < 0, ERR_INT, "Failed to close reference", TIO_ERR_INT);
  }


  irc = lfile->CloseObject( fileID, parentID, objectID, class );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to close object", TIO_ERR_OBJECT_CLOSE);
  }

  irc = iTIO_DeregisterObject(fileID, objectID);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to deregister object", TIO_ERR_INT);
  }

  return (TIO_SUCCESS);
}


/***************************************************************************************************
 *
 * cTIO_HierarchyKillObject
 *
 **************************************************************************************************/

TIO_t
cTIO_HierarchyKillObject( const char         *subname,
                          const TIO_File_t   fileID,
                          TIO_Object_t       parentID,
                          const TIO_Object_t objectID,
                          const iClass_t     class )
{
  /* Cleans up an object if something fails after creation/opening */

  struct iFile_t *lfile;

  int irc;

  irc = iTIO_GetFile(fileID, &lfile);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid file ID", TIO_ERR_INVALID_FILEID);
  }

  /* No real need to call this, but things have gone wrong anyway, so might as well, for sanity */
  irc = iTIO_GetObject(fileID, NULL, objectID, class, NULL);
  if (irc < 0) {
    TIOreturn(ERR_USER, "Invalid object ID", TIO_ERR_INVALID_OBJECTID);
  }

  irc = lfile->RemoveRef(fileID, parentID, objectID, class, NULL);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to remove object ref from parent", TIO_ERR_INT);
  }

  irc = lfile->CloseObject( fileID, parentID, objectID, class );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to close object", TIO_ERR_INT);
  }

  irc = iTIO_DeregisterObject(fileID, objectID);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to deregister object", TIO_ERR_INT);
  }

  return (TIO_SUCCESS);
}



/***************************************************************************************************
 *
 * iTIO_HierarchyCreateObjectDatasets
 *
 **************************************************************************************************/

int
iTIO_HierarchyCreateObjectDatasets( const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t objectID,
                                    const iClass_t     class,
                                    struct iFile_t     *tfile,
                                    void               *parentinfo )
{
  const char *subname = "iTIO_HierarchyCreateObjectDatasets()";

  void *objptr;
  int irc;

  irc = iTIO_GetObject(fileID, NULL, objectID, class, &objptr);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to find object", TIO_ERR_INT);
  }

  switch (class) {
  case CLASS_MATERIAL:
    irc = iTIO_CreateMaterialDatasets(fileID, parentID, objectID, tfile, parentinfo, objptr);
    break;
  case CLASS_QUANT:
    irc = iTIO_CreateQuantDatasets(fileID, parentID, objectID, tfile, parentinfo, objptr);
    break;
  default:
    break;
  }

  return (irc);
}


/*
 * EOF
 */

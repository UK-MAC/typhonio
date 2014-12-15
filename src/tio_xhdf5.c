/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"
#include "tio_register_int.h"

#include "tio_xhdf5_int.h"

#include "tio_chunk_int.h"
#include "typhonio_viz.h"

#ifdef _TYPHIO_HDF5

#include <hdf5.h>



/*
 *  Routines:
 */

static int xH5_SetVartype( const TIO_Dims_t   ndims,
                           const TIO_Data_t   datatype );

static int xH5_GetVartype( const int          vartype,
                           TIO_Dims_t         *ndims,
                           TIO_Data_t         *datatype );

static int xH5_ConvertVar( const TIO_Data_t   src_dtype,
                           const TIO_Data_t   dst_dtype,
                           const void         **src,
                           void               **dst );

static int xH5_CalcChunkedSpace( struct xH5object_t  *hobject,
                                 const iClass_t      class,
                                 const TIO_Mesh_t    meshtype,
                                 const iDataPos_t    datapos,
                                 const int           ndset,
                                 const TIO_Dims_t    ndims,
                                 const TIO_Size_t    nghosts,
                                 const TIO_Size_t    nchunks,
                                 const union iInfo_t *chunkinfo,
                                 const TIO_Size_t    idx,
                                 TIO_Size_t          *space_size,
                                 hid_t               *memsp_id,
                                 hid_t               *filsp_id );



static int xH5_GetOffset( const TIO_Mesh_t    meshtype,
                          const iDataPos_t    datapos,
                          const TIO_Dims_t    ndims,
                          const TIO_Size_t    nghosts,
                          const TIO_Size_t    nchunks,
                          const union iInfo_t *chunkinfo,
                          const TIO_Size_t    idx,
                          hsize_t             *offset );

static int xH5_CalcOffsetSpace(  const TIO_Mesh_t    meshtype,
                                 const iDataPos_t    datapos,
                                 const TIO_Dims_t    ndims,
                                 const TIO_Size_t    nghosts,
                                 const TIO_Size_t    fnghosts,
                                 const TIO_Size_t    nchunks,
                                 const union iInfo_t *chunkinfo,
                                 const TIO_Size_t    idx,
                                 const hid_t         lfsp_id,
                                 const hsize_t       sdims[1]   );

/*
 *  Vars and constants;
 */

static hid_t DatatypeLookup[(TIO_STRING + 1)];


/* Ref names and sizes match enum for class */
static char    xRefNames[CLASS_MAX+1][TIO_STRLEN];
static hsize_t xRefSizes[CLASS_MAX+1];
static hsize_t xRefDelta[CLASS_MAX+1];
static int     xRefClass[CLASS_MAX+1];

static const char *xVarInfoUnits   = "TIO_VARIABLE_INFO_UNITS";
static const char *xVarInfoTypes   = "TIO_VARIABLE_INFO_TYPES";
static const char *xVarInfoIndex   = "TIO_VARIABLE_INFO_INDEX";

static const char *xVarCacheNames[NUM_DATATYPES] = { "TIO_VARIABLE_CACHE_0D_SHORT",
                                                     "TIO_VARIABLE_CACHE_0D_USHORT",
                                                     "TIO_VARIABLE_CACHE_0D_INT",
                                                     "TIO_VARIABLE_CACHE_0D_UINT",
                                                     "TIO_VARIABLE_CACHE_0D_LONG",
                                                     "TIO_VARIABLE_CACHE_0D_ULONG",
                                                     "TIO_VARIABLE_CACHE_0D_LLONG",
                                                     "TIO_VARIABLE_CACHE_0D_ULLONG",
                                                     "TIO_VARIABLE_CACHE_0D_FLOAT",
                                                     "TIO_VARIABLE_CACHE_0D_DOUBLE",
                                                     "TIO_VARIABLE_CACHE_0D_LDOUBLE",
                                                     "TIO_VARIABLE_CACHE_0D_LOGICAL",
                                                     "TIO_VARIABLE_CACHE_0D_CHAR",
                                                     "TIO_VARIABLE_CACHE_0D_UCHAR" };



/***************************************************************************************************
 *
 * xH5_SetRefNames
 *
 **************************************************************************************************/
int
xH5_SetRefNames( void )
{
  const char *subname = "xH5_SetRefNames()";

  char     *name;
  iClass_t class;
  int      irc;

  TIO_Size_t refsize, refdelta;
  int        refpriority;


  for (class=CLASS_STATE; class<=CLASS_MAX; ++class) {

    irc = iTIO_HierarchyGetName(class, &name);

    snprintf(&xRefNames[class][0], sizeof(xRefNames[class]), "TIO_%s_REF", name);

    irc = iTIO_HierarchyGetSize(class, &refsize);
    irc = iTIO_HierarchyGetDelta(class, &refdelta);
    irc = iTIO_HierarchyGetPriority(class, &refpriority);

    xRefSizes[class] = (hsize_t)refsize;
    xRefDelta[class] = (hsize_t)refdelta;
    xRefClass[class] = (int)refpriority;

  }

  return (0);
}



/***************************************************************************************************
 *
 * xH5_GetRefName
 * xH5_GetRefSize
 * xH5_GetRefDelta
 *
 **************************************************************************************************/
char *
xH5_GetRefName( const iClass_t class )
{
  const char *subname = "xH5_GetRefName()";

  return (xRefNames[class]);
}

hsize_t
xH5_GetRefSize( const iClass_t class )
{
  const char *subname = "xH5_GetRefSize()";

  return (xRefSizes[class]);
}

hsize_t
xH5_GetRefDelta( const iClass_t class )
{
  const char *subname = "xH5_GetRefDelta()";

  return (xRefDelta[class]);
}



/***************************************************************************************************
 *
 * xH5_SetTypes
 *
 **************************************************************************************************/
int
xH5_SetTypes( void )
{
  herr_t hrc;


  DatatypeLookup[TIO_SHORT]   = H5T_NATIVE_SHORT;
  DatatypeLookup[TIO_USHORT]  = H5T_NATIVE_USHORT;
  DatatypeLookup[TIO_INT]     = H5T_NATIVE_INT;
  DatatypeLookup[TIO_UINT]    = H5T_NATIVE_UINT;
  DatatypeLookup[TIO_LONG]    = H5T_NATIVE_LONG;
  DatatypeLookup[TIO_ULONG]   = H5T_NATIVE_ULONG;
  DatatypeLookup[TIO_LLONG]   = H5T_NATIVE_LLONG;
  DatatypeLookup[TIO_ULLONG]  = H5T_NATIVE_ULLONG;
  DatatypeLookup[TIO_FLOAT]   = H5T_NATIVE_FLOAT;
  DatatypeLookup[TIO_DOUBLE]  = H5T_NATIVE_DOUBLE;
  DatatypeLookup[TIO_LDOUBLE] = H5T_NATIVE_LDOUBLE;

  DatatypeLookup[TIO_LOGICAL] = H5T_NATIVE_UINT;

  DatatypeLookup[TIO_CHAR]    = H5T_NATIVE_SCHAR;
  DatatypeLookup[TIO_UCHAR]   = H5T_NATIVE_UCHAR;

  /* These need to be set to whatever the equivalent of whatever TIO_*_t is typedef-ed to: */
  DatatypeLookup[TIO_ENUM_T]  = H5T_NATIVE_INT;
  DatatypeLookup[TIO_SIZE_T]  = H5T_NATIVE_ULLONG;
  DatatypeLookup[TIO_STEP_T]  = H5T_NATIVE_ULLONG;
  DatatypeLookup[TIO_TIME_T]  = H5T_NATIVE_DOUBLE;


  DatatypeLookup[TIO_STRING]  = H5Tcopy(H5T_C_S1);
  /* Don't use H5Tcopy(H5T_NATIVE_CHAR) as H5T_NATIVE_CHAR is signed/unsigned int! */

  /* Default string size of TIO_STRLEN char */
  hrc = H5Tset_size(DatatypeLookup[TIO_STRING], TIO_STRLEN);

  return (0);
}



/***************************************************************************************************
 *
 * xH5_FreeTypes
 *
 **************************************************************************************************/
int
xH5_FreeTypes( void )
{
  herr_t hrc;


  /* Free copied string type */
  hrc = H5Tclose(DatatypeLookup[TIO_STRING]);

  return (0);
}




/***************************************************************************************************
 *
 * xH5_GetH5type
 *
 **************************************************************************************************/
hid_t
xH5_GetH5type( const TIO_Data_t datatype )
{
  if ( (datatype < TIO_SHORT) || (datatype > TIO_STRING) ) {
    return ((hid_t)-1);
  }
  else {
    return (DatatypeLookup[(int)datatype]);
  }
}




/***************************************************************************************************
 *
 * xH5_SetXfer
 * xH5_GetXfer
 * xH5_FreeXfer
 *
 **************************************************************************************************/
int
xH5_SetXfer( struct xH5file_t *hfile )
{
  const char *subname = "xH5_SetXfer()";

  herr_t hrc;                        /* HDF5 return code */


  hfile->colxfer_plist = H5Pcreate(H5P_DATASET_XFER);
  TIOassert(hfile->colxfer_plist < 0, ERR_HDF5, "Failed to create colxfer_plist", -1);
/*
  hrc = H5Pset_buffer( hfile->colxfer_plist, 64 * 1024 *1024, NULL, NULL);
*/
  hfile->indxfer_plist = H5Pcreate(H5P_DATASET_XFER);
  TIOassert(hfile->indxfer_plist < 0, ERR_HDF5, "Failed to create indxfer_plist", -2);
/*
  hrc = H5Pset_buffer( hfile->indxfer_plist, 64 * 1024 *1024, NULL, NULL);
*/
#ifdef _TYPHIO_PARALLEL
  {
    /* -- Parallel IO being used */

    hrc = H5Pset_dxpl_mpio(hfile->colxfer_plist, H5FD_MPIO_COLLECTIVE);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set collective parallel transfer plist", -10);

    hrc = H5Pset_dxpl_mpio(hfile->indxfer_plist, H5FD_MPIO_INDEPENDENT);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set independent parallel transfer plist", -11);
  }
#endif

  return (0);
}


hid_t
xH5_GetXfer( const struct xH5file_t *hfile,
             const TIO_Xfer_t       xfer )
{
#ifdef _TYPHIO_ALLINDEPENDENT

  return (hfile->indxfer_plist);   /* collective doesn't work on Willow & Blackthorn ? */

#else

  switch (xfer) {
  case TIO_XFER_NULL:
  case TIO_XFER_INDEPENDENT:
    return (hfile->indxfer_plist);
    break;
  case TIO_XFER_COLLECTIVE:
    return (hfile->colxfer_plist);
    break;
  default:
    break;
  }

#endif

  return ((hid_t)-1);
}


int
xH5_FreeXfer( struct xH5file_t *hfile )
{
  const char *subname = "xH5_FreeXfer()";

  herr_t hrc;

  /* -- Close property lists for the file */
  hrc = H5Pclose(hfile->colxfer_plist);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close collective parallel transfer plist", -1);

  hrc = H5Pclose(hfile->indxfer_plist);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close independent parallel transfer plist", -2);

  return (0);
}


/***************************************************************************************************
 *
 * xH5_SetVartype
 * xH5_GetVartype
 *
 * - encode/decode dimension and datatype information into a single integer
 *
 **************************************************************************************************/
static int
xH5_SetVartype( const TIO_Dims_t ndims,
                const TIO_Data_t datatype )
{
  return((ndims * 1000) + datatype);
}

static int
xH5_GetVartype( const int  vartype,
                TIO_Dims_t *ndims,
                TIO_Data_t *datatype )
{
  TIOset (ndims,    (TIO_Dims_t)(vartype / 1000));
  TIOset (datatype, (TIO_Data_t)(vartype % 1000));

  return (0);
}




/***************************************************************************************************
 *
 * xH5_GetRef
 *
 **************************************************************************************************/
int
xH5_GetRef( const TIO_File_t     fileID,
            const TIO_Object_t   objectID,
            hid_t                *obj_id,
            const iClass_t       refclass,
            struct xH5ref_t      **ref )
{
  const char *subname = "xH5_GetRef()";

  struct xH5file_t   *hfile;
  struct xH5object_t *hobject;

  int irc;



  /* -- Get ref for the input file/object */
  if (objectID > 0) {

    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    if (irc < 0) {
      /* Possible invalid object ID */
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);
    }
    TIOset(obj_id, hobject->group_id);
    TIOset(ref,    &hobject->ref[xRefClass[refclass]]);

  }
  else {

    irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
    if (irc < 0) {
      /* Possible invalid file ID */
      TIOreturn(ERR_INT, "Failed to get file ffinfo", ITIO_ERR_FILEID);
    }
    TIOset(obj_id, hfile->root_id);
    TIOset(ref,    &hfile->ref[xRefClass[refclass]]);

  }


  return (0);
}




/***************************************************************************************************
 *
 * xH5_GetVcache
 *
 **************************************************************************************************/
int
xH5_GetVcache( const TIO_File_t     fileID,
               const TIO_Object_t   objectID,
               struct xH5varcache_t **varcache )
{
  const char *subname = "xH5_GetVcache()";

  struct xH5file_t   *hfile;
  struct xH5object_t *hobject;

  int irc;



  /* -- Get ref for the input file/object */
  if (objectID > 0) {

    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    if (irc < 0) {
      /* Possible invalid object ID */
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);
    }
    *varcache = hobject->varcache;

  }
  else {

    irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
    if (irc < 0) {
      /* Possible invalid file ID */
      TIOreturn(ERR_INT, "Failed to get file ffinfo", ITIO_ERR_FILEID);
    }
    *varcache = hfile->varcache;

  }


  return (0);
}




/***************************************************************************************************
 *
 * xH5_InitVarCache
 *
 **************************************************************************************************/
int
xH5_InitVarCache( const TIO_File_t     fileID,
                  const TIO_Object_t   objectID,
                  const hsize_t        nvar,
                  const hsize_t        varrefsize,
                  const int            reading,
                  const hid_t          obj_id )
{
  const char *subname = "xH5_InitVarCache()";

  struct xH5file_t     *hfile;
  struct xH5object_t   *hobject;
  struct xH5varcache_t *vcache;
  size_t               i;

  int irc;


  /*
    This routine only gets called as-and-when variable cache is needed
    Before this the varcache would not have been malloc-ed
  */


  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  if (irc < 0) {
    /* Possible invalid file ID */
    TIOreturn(ERR_INT, "Failed to get file ffinfo", ITIO_ERR_FILEID);
  }

  if (objectID > 0) {

    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    if (irc < 0) {
      /* Possible invalid object ID */
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);
    }

    hobject->varcache = (struct xH5varcache_t *)TIOmalloc(sizeof(struct xH5varcache_t));

    vcache = hobject->varcache;
  }
  else {

    hfile->varcache = (struct xH5varcache_t *)TIOmalloc(sizeof(struct xH5varcache_t));

    vcache = hfile->varcache;
  }

  if (NULL == vcache)
    TIOreturn(ERR_MEM, "Failed to malloc varcache", -1);


#ifndef NDEBUG
  if (objectID > 0) {
    (void) strcpy(vcache->name, hobject->name);
  }
  else {
    (void) strcpy(vcache->name, hfile->name);
  }
  (void) strcat(vcache->name, " : varcache");

  vcache->vtypes_id = 888888;
  vcache->vindex_id = 888888;
  vcache->units     = NULL;
  vcache->types     = NULL;
  vcache->index     = NULL;
#endif

  vcache->vunits_id = 0;              /* Used as a flag for whether datasets are open */

  for (i=0; i<NUM_DATATYPES; ++i) {
    vcache->ncache[i]      = 0;
    vcache->memspace_id[i] = 0;
    vcache->filspace_id[i] = 0;       /* Used to hold size of cache required globally */
    vcache->cachesize[i]   = 0;
    vcache->vcache_id[i]   = 0;       /* >0 used to flag dataset exists && open    */
    vcache->cache[i]       = NULL;    /* != NULL used to flag that cache is in use */
  }


  vcache->units = TIOmalloc(varrefsize * sizeof(char [TIO_STRLEN]));
  if (NULL == vcache->units)
    TIOreturn(ERR_MEM, "Failed to malloc vcache->units", -3);

  vcache->types = TIOmalloc(varrefsize * sizeof(int));
  if (NULL == vcache->types)
    TIOreturn(ERR_MEM, "Failed to malloc vcache->types", -4);

  vcache->index = TIOmalloc(varrefsize * sizeof(TIO_Size_t));
  if (NULL == vcache->index)
    TIOreturn(ERR_MEM, "Failed to malloc vcache->index", -5);


  if (reading && (nvar > 0)) {

    /* Don't want to read from a pre-existing file/object with no variables as units, etc, datasets
       won't exist */

    /* Read variable info collectively (all processes need a filled variable info tables) */

    herr_t hrc;
    hid_t  xfer_plist;


    xfer_plist = xH5_GetXfer(hfile, TIO_XFER_COLLECTIVE);

    /* Units */
    vcache->vunits_id = H5Dopen(obj_id, xVarInfoUnits, H5P_DEFAULT);

    TIOassert(vcache->vunits_id < 0, ERR_HDF5, "Failed to open variables units dataset", -11);

    hrc = H5Dread(vcache->vunits_id, xH5_GetH5type(TIO_STRING), H5S_ALL, H5S_ALL,
                  xfer_plist, (void *)vcache->units);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to read variable units dataset", -12);


    /* Types */
    vcache->vtypes_id = H5Dopen(obj_id, xVarInfoTypes, H5P_DEFAULT);

    TIOassert(vcache->vtypes_id < 0, ERR_HDF5, "Failed to open variables types dataset", -13);

    hrc = H5Dread(vcache->vtypes_id, xH5_GetH5type(TIO_INT), H5S_ALL, H5S_ALL,
                  xfer_plist, (void *)vcache->types);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to read variable types dataset", -14);


    /* Index */
    vcache->vindex_id = H5Dopen(obj_id, xVarInfoIndex, H5P_DEFAULT);

    TIOassert(vcache->vindex_id < 0, ERR_HDF5, "Failed to open variables index dataset", -15);

    hrc = H5Dread(vcache->vindex_id, xH5_GetH5type(TIO_SIZE_T), H5S_ALL, H5S_ALL,
                  xfer_plist, (void *)vcache->index);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to read variable types dataset", -16);


    /* Open and read and fill the cache datasets */
    /* If we're at this point in the code it's because variables are being appended, in which case
       we need the data in the caches in order to append; or because a variable is being opened, in
       which case it is highly likely that variables are going to be read. */


    for (i=0; i<nvar; ++i) {

      int        vartype;
      TIO_Dims_t ndims;
      TIO_Data_t dt;
      hsize_t    spdims[1];

      hid_t xfer_plist;

      xfer_plist = xH5_GetXfer(hfile, TIO_XFER_COLLECTIVE);

      vartype = vcache->types[i];

      irc = xH5_GetVartype(vartype, &ndims, &dt);

      if ((ndims > TIO_0D) || (TIO_STRING == dt)) continue;
      if (vcache->vcache_id[dt] > 0)              continue;

      vcache->vcache_id[dt] = H5Dopen(obj_id, xVarCacheNames[dt], H5P_DEFAULT);

      TIOassert(vcache->vcache_id[dt] < 0, ERR_HDF5, "Failed to open variable cache dataset", -11);

      vcache->memspace_id[dt] = H5Dget_space(vcache->vcache_id[dt]);
      TIOassert(vcache->memspace_id[dt] < 0, ERR_HDF5, "Failed to get existing dataspace", -12);

      irc = H5Sget_simple_extent_dims(vcache->memspace_id[dt], spdims, NULL);
      TIOassert(irc < 0, ERR_HDF5, "Failed to get dataspace extent", -14);


      vcache->ncache[dt]    = (TIO_Size_t)spdims[0];
      vcache->cachesize[dt] = (TIO_Size_t)spdims[0];

      vcache->cache[dt] = TIOmalloc(spdims[0] * iTIO_SizeOf(dt));
      if (NULL == vcache->cache[dt])
        TIOreturn(ERR_MEM, "Failed to malloc vcache->cache[datatype]", -15);

      hrc = H5Dread(vcache->vcache_id[dt], xH5_GetH5type(dt), H5S_ALL, H5S_ALL,
                    xfer_plist, (void *)vcache->cache[dt]);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to read variable units dataset", -16);

      /* Leave dataset open in case of appending / modification */

      /* Set dataspaces now to contain no selection.  Any appended or modified variables will
         get set in the selection.  This automagically handles everything! */

      hrc = H5Sselect_none(vcache->memspace_id[dt]);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select none of dataspace", -17);

    }

  }


  return (0);
}




/***************************************************************************************************
 *
 * xH5_FinalizeVarCache
 *
 **************************************************************************************************/
int
xH5_FinalizeVarCache( const TIO_File_t     fileID,
                      const TIO_Object_t   objectID,
                      const hsize_t        nvar,
                      const hsize_t        varrefsize,
                      const int            mod,
                      const hid_t          obj_id )

{
  const char *subname = "xH5_FinalizeVarCache()";

  struct xH5file_t     *hfile;
  struct xH5object_t   *hobject;
  struct xH5varcache_t **varcache, *vcache;

  int        idt;
  TIO_Data_t dt;

  herr_t hrc;
  int    irc;



  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get file ffinfo", ITIO_ERR_FILEID);
  }

  if (objectID > 0) {
    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);
    }
    varcache = &hobject->varcache;
  }
  else {
    varcache = &hfile->varcache;
  }

  vcache = *varcache;


  if (NULL == vcache) {   /*  If vcache is NULL, no variables exist at all for the object  */
    return (0);           /*  and no caches or datasets will exist and be open             */
  }


  /* Write the contents of the variable caches */


  if ((nvar > 0) && mod) {

    hid_t    memsp_id;                             /* HDF5 ID for memory dataspace */
    hid_t    cplist_id;                            /* HDF5 ID for dataset creation property list */
    hid_t    xfer_plist;
    hsize_t  dims[1];                              /* Initial dimension extent of dataset */

    const hsize_t chunkdims[1] = { 1 };               /* Size of dataset chunks (1 = 1 reference) */

    hssize_t n;


    /* If we get this far it is because there is an allocated variable reference */

    if (vcache->vunits_id > 0) {

      /* -- Extend info datasets */

      /* Datasets already exist and have been opened, so extend if necessary */

      dims[0] = nvar;

      hrc = H5Dextend(vcache->vunits_id, dims);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to extend varinfounits dataset", -11);

      hrc = H5Dextend(vcache->vtypes_id, dims);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to extend varinfotypes dataset", -12);

      hrc = H5Dextend(vcache->vindex_id, dims);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to extend varinfoindex dataset", -13);

    }
    else {

      /* -- New info datasets */

      /* Create unlimited, extendable, chunked datasets
         Also need to have dataset allocation time set to H5D_ALLOC_TIME_EARLY */

      dims[0] = nvar;

      memsp_id = H5Screate_simple(1, dims, UNLIMDIMS);
      TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create dataspace", -21);

      cplist_id = H5Pcreate(H5P_DATASET_CREATE);
      TIOassert(cplist_id < 0, ERR_HDF5, "Failed to create property list", -22);

      hrc =  H5Pset_layout(cplist_id, H5D_CHUNKED);
      hrc += H5Pset_chunk(cplist_id, 1, chunkdims);
      hrc += H5Pset_alloc_time(cplist_id, H5D_ALLOC_TIME_EARLY);
      hrc += H5Pset_fill_time(cplist_id, H5D_FILL_TIME_NEVER);
      TIOassert(hrc != 0, ERR_HDF5, "Failed to set dataset properties", -23);

      vcache->vunits_id = H5Dcreate(obj_id, xVarInfoUnits, xH5_GetH5type(TIO_STRING),
                                    memsp_id, H5P_DEFAULT, cplist_id, H5P_DEFAULT);

      TIOassert(vcache->vunits_id < 0, ERR_HDF5, "Failed to create varinfounits dataset", -25);

      vcache->vtypes_id = H5Dcreate(obj_id, xVarInfoTypes, xH5_GetH5type(TIO_INT),
                                     memsp_id, H5P_DEFAULT, cplist_id, H5P_DEFAULT);

      TIOassert(vcache->vtypes_id < 0, ERR_HDF5, "Failed to create varinfotypes dataset", -26);

      vcache->vindex_id = H5Dcreate(obj_id, xVarInfoIndex, xH5_GetH5type(TIO_SIZE_T),
                                    memsp_id, H5P_DEFAULT, cplist_id, H5P_DEFAULT);

      TIOassert(vcache->vindex_id < 0, ERR_HDF5, "Failed to create varinfoindex dataset", -27);

      /* -- Close dataspace */
      hrc = H5Sclose(memsp_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataspace", -28);

    }


    /* Now at this point the variable info datasets will exist and will need writing... */


    if (0 == hfile->rank) {

      xfer_plist = xH5_GetXfer(hfile, TIO_XFER_INDEPENDENT);

      hrc = H5Dwrite(vcache->vunits_id, xH5_GetH5type(TIO_STRING), H5S_ALL, H5S_ALL,
                     xfer_plist, (const void*)vcache->units);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to write varinfounits dataset", -31);

      hrc = H5Dwrite(vcache->vtypes_id, xH5_GetH5type(TIO_INT), H5S_ALL, H5S_ALL,
                     xfer_plist, (const void*)vcache->types);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to write varinfotypes dataset", -32);

      hrc = H5Dwrite(vcache->vindex_id, xH5_GetH5type(TIO_SIZE_T), H5S_ALL, H5S_ALL,
                     xfer_plist, (const void*)vcache->index);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to write varinfoindex dataset", -33);

    }


    /* -- And now update/write the caches... */

    for (idt=0; idt<NUM_DATATYPES; ++idt) {

      dt = (TIO_Data_t)idt;

      if (0 == vcache->ncache[idt]     ) continue;
      if (0 == vcache->filspace_id[idt]) continue;   /* This cache[dt] hasn't been modified */


      irc = H5Sget_simple_extent_dims(vcache->filspace_id[idt], dims, NULL);
      TIOassert(irc < 0, ERR_HDF5, "Failed to get dataspace extent", -50);

      if (vcache->vcache_id[idt] > 0) {

        /* -- Cache dataset already exists - extend it */

        hrc = H5Dextend(vcache->vcache_id[idt], dims);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to extend cache dataset", -51);

      }
      else {

        /* -- Create cache dataset */

        cplist_id = H5Pcreate(H5P_DATASET_CREATE);
        TIOassert(cplist_id < 0, ERR_HDF5, "Failed to create property list", -55);

        hrc =  H5Pset_layout(cplist_id, H5D_CHUNKED);
        hrc += H5Pset_chunk(cplist_id, 1, chunkdims);
        hrc += H5Pset_alloc_time(cplist_id, H5D_ALLOC_TIME_EARLY);
        hrc += H5Pset_fill_time(cplist_id, H5D_FILL_TIME_NEVER);
        TIOassert(hrc != 0, ERR_HDF5, "Failed to set dataset properties", -56);

        vcache->vcache_id[idt] = H5Dcreate(obj_id, xVarCacheNames[idt], xH5_GetH5type(dt),
                                  vcache->filspace_id[idt], H5P_DEFAULT, cplist_id, H5P_DEFAULT);

        TIOassert(vcache->vcache_id[idt] < 0, ERR_HDF5, "Failed to create vcache dataset", -57);
      }


      /* -- Now write */

      /* Could do this collectively and assume that processes with no contribution to go to disk
         don't do anything.  However, some MPI-IO implementations are known to struggle with this,
         so we'll rely on independent writing instead.  */

      n = H5Sget_select_npoints(vcache->filspace_id[idt]);
      TIOassert(n < 0, ERR_HDF5, "Failed to get filspace npoints", -60);

      if (n > 0) {

        xfer_plist = xH5_GetXfer(hfile, TIO_XFER_INDEPENDENT);

        hrc = H5Dwrite(vcache->vcache_id[idt], xH5_GetH5type(dt),
                       vcache->memspace_id[idt], vcache->filspace_id[idt],
                       xfer_plist, (const void*)vcache->cache[idt]);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to write vcache dataset", -61);

      }

    }  /* for (all datatypes) */


  }   /* Finished writing variable caches */



  /* Can now close the units, types and index datasets  */

  if (vcache->vunits_id > 0) {

    hrc = H5Dclose(vcache->vunits_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close vunits dataset", -40);

    hrc = H5Dclose(vcache->vtypes_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close vtypes dataset", -41);

    hrc = H5Dclose(vcache->vindex_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close vindex dataset", -42);

  }


  for (idt=0; idt<NUM_DATATYPES; ++idt) {

    if (0 == vcache->vcache_id[idt]) continue;

    /* Is possible that this process did not write any variable data */

    if (vcache->memspace_id[idt] > 0) {
      hrc = H5Sclose(vcache->memspace_id[idt]);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close varcache memspace", -70);
    }

    if (vcache->filspace_id[idt] > 0) {
      hrc = H5Sclose(vcache->filspace_id[idt]);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close varcache filspace", -71);
    }

    hrc = H5Dclose(vcache->vcache_id[idt]);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close varcache dataset", -72);


    /* Dataspaces and dataset may exist on process, but this process may not have written any
       data. therefore cache wont have been malloced */

    if (vcache->cache[idt] != NULL) {
      TIOfree (vcache->cache[idt]); vcache->cache[idt] = NULL;
    }

  }

  TIOfree (vcache->units); vcache->units = NULL;
  TIOfree (vcache->types); vcache->types = NULL;
  TIOfree (vcache->index); vcache->index = NULL;

  TIOfree (*varcache); *varcache = NULL;

  return (0);
}





/***************************************************************************************************
 *
 * xH5_CreateVarDataset
 *
 **************************************************************************************************/
int
xH5_CreateVarDataset( const TIO_File_t   fileID,
                      const TIO_Object_t parentID,
                      const TIO_Object_t varID,
                      const char         name[],
                      const TIO_Dims_t   ndims,
                      const TIO_Size_t   *dims,
                      const TIO_Data_t   datatype )
{
  const char *subname = "xH5_CreateVarDataset()";

  struct xH5varcache_t *vcache;
  struct xH5object_t   *vobject;

  hsize_t idx;

  herr_t  hrc;
  int     irc;



  irc = xH5_GetVcache(fileID, parentID, &vcache);
  if (irc < 0)
    TIOreturn(ERR_INT, "Failed to get file/parent varcache", irc);  /* irc = Invalid fileID/parentID */

  irc = iTIO_GetFFinfo( 0, varID, (void **)&vobject );
  if (irc < 0)
    TIOreturn(ERR_INT, "Failed to get variable ffinfo", ITIO_ERR_OBJECTID);  /* Invalid varID */


  idx = vobject->varrefindex;

  TIOassert((NULL == vcache), ERR_INT, "varcache not allocated", -3);

  vcache->types[idx] = xH5_SetVartype(ndims, datatype);


  if ((ndims > TIO_0D) || (TIO_STRING == datatype)) {

    /* -- Data is >0D, so has it's own dataset, ie. not cached */

    hid_t   dtype_id, space_id, create_plist;
    hsize_t dimsf[TIO_MAXDIMS];
    int     lndims, i, j;


    vobject->vardataindex = (hssize_t)-1;   /* Flags that data isn't in the cache */
    vcache->index[idx]    = (TIO_Size_t)0;


    dtype_id = xH5_GetH5type(datatype);
    TIOassert (dtype_id < 0, ERR_INT, "Invalid datatype", ITIO_ERR_DATATYPE);


    /* -- Create the simple scalar space required for the variable */
    lndims = (int)ndims;
    for (i=0, j=(lndims-1); i<TIO_MAXDIMS; ++i, --j) {
      if (i < lndims) {
        dimsf[i] = dims[j];
      }
      else {
        dimsf[i] = 0;
      }
    }

    /*????  (void) iTIO_ReshapeDims(ndims, dimsf); ???? */

    space_id = H5Screate_simple(lndims, dimsf, NULL);
    TIOassert(space_id < 0, ERR_HDF5, "Failed to create variable space", -20);


    /* -- Construct the variable creation property list */
    create_plist = H5Pcreate(H5P_DATASET_CREATE);
    TIOassert(create_plist < 0, ERR_HDF5, "Failed to create creation plist", -21);

    /* Default value:  create_plist = H5P_DEFAULT, set to allocate dataset in file early */

    hrc =  H5Pset_alloc_time(create_plist, H5D_ALLOC_TIME_EARLY);
    hrc += H5Pset_fill_time(create_plist, H5D_FILL_TIME_NEVER);
    TIOassert(hrc != 0, ERR_HDF5, "Failed to set dataset properties", -22);

    /* -- Create the variable dataset (data) and close the creation property list */
    vobject->dset_id[0] = H5Dcreate(vobject->group_id, name, dtype_id, 
                                    space_id, H5P_DEFAULT, create_plist, H5P_DEFAULT);

    if (vobject->dset_id[0] < 0)
      TIOreturn(ERR_HDF5, "Failed to create variable", ITIO_ERR_CREATE);

    hrc = H5Pclose(create_plist);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close create plist", -24);


    /* -- Close the dataspace */
    hrc = H5Sclose(space_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close space", -25);

  }
  else {

    /* -- Data is truly 0D so caching */

    hsize_t hdims[1];


    ++vcache->ncache[datatype];

    hdims[0] = (hsize_t)vcache->ncache[datatype];

    if (vcache->filspace_id[datatype] > 0) {

      /* Extend already existing file dataspace to new required size */

      hrc = H5Sset_extent_simple(vcache->filspace_id[datatype], 1, hdims, UNLIMDIMS);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to set dataspace extent", -10);

    }
    else {

      /* Create or open dataspace */

      if (vcache->vcache_id[datatype] > 0) {
        vcache->filspace_id[datatype] = H5Dget_space(vcache->vcache_id[datatype]);
        TIOassert(vcache->filspace_id[datatype] < 0, ERR_HDF5, "Failed to get dataspace", -20);
      }
      else {
        vcache->filspace_id[datatype] = H5Screate_simple(1, hdims, UNLIMDIMS);
        TIOassert(vcache->filspace_id[datatype] < 0, ERR_HDF5, "Failed to create dataspace", -21);
      }

      hrc = H5Sselect_none(vcache->filspace_id[datatype]);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select none on dataspace", -22);

    }

    vobject->vardataindex = (hssize_t)0;  /* Flags that data is to go into cache */
                                          /* - proper index value later */
    vcache->index[idx]    = vcache->ncache[datatype];

  }


  return (0);
}





/***************************************************************************************************
 *
 * xH5_OpenVarDataset
 *
 **************************************************************************************************/
int
xH5_OpenVarDataset( const TIO_File_t   fileID,
                    const TIO_Object_t parentID,
                    const TIO_Object_t varID,
                    const char         name[],
                    TIO_Dims_t         *ndims,
                    TIO_Size_t         *dims,
                    TIO_Data_t         *datatype )
{
  const char *subname = "xH5_OpenVarDataset()";

  struct xH5varcache_t *vcache;
  struct xH5object_t   *vobject;

  int        vtype;

  TIO_Data_t ldatatype;
  TIO_Dims_t lndims;

  int    irc;
  herr_t hrc;



  irc = xH5_GetVcache(fileID, parentID, &vcache);
  if (irc < 0)
    TIOreturn(ERR_INT, "Failed to get file/parent varcache", irc);  /* Invalid fileID/parentID */

  irc = iTIO_GetFFinfo( 0, varID, (void **)&vobject );
  if (irc < 0)
    TIOreturn(ERR_INT, "Failed to get variable ffinfo", ITIO_ERR_OBJECTID);  /* Invalid varID) */


  TIOassert((NULL == vcache), ERR_INT, "varcache not allocated", -3);


  /* GET THE VTYPE HERE FROM CACHEINFO - THIS GIVES DIMENSION AND DATATYPE IT WAS STORED AS */

  vtype = vcache->types[vobject->varrefindex];

  irc = xH5_GetVartype(vtype, &lndims, &ldatatype);

  TIOset (datatype, ldatatype);
  TIOset (ndims,   lndims);


  if ((lndims > TIO_0D) || (TIO_STRING == ldatatype)) {

    /* Variable is > 0D */

    vobject->vardataindex = (hssize_t)-1;   /* Flags that data isn't in the cache */

    vobject->dset_id[0] = H5Dopen(vobject->group_id, name, H5P_DEFAULT);

    if (vobject->dset_id[0] < 0)
      TIOreturn(ERR_HDF5, "Failed to open variable dataset", ITIO_ERR_OPEN);

    if (dims != NULL) {

      hid_t   space_id;
      hsize_t sdims[TIO_MAXDIMS];
      int     i, j;


      space_id = H5Dget_space(vobject->dset_id[0]);

      irc = H5Sget_simple_extent_dims(space_id, sdims, NULL);

      if (dims != NULL) {
        for (i=0, j=(lndims-1); i<lndims; ++i, --j) {
          dims[j] = (TIO_Size_t)sdims[i];
        }
      }

      hrc = H5Sclose(space_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataspace", -4);

    }

  }
  else {

    /* Comes from the cache */

    vobject->vardataindex = vcache->index[vobject->varrefindex];

  }


  return (0);
}





/***************************************************************************************************
 *
 * xH5_CloseVarDataset
 *
 **************************************************************************************************/
int
xH5_CloseVarDataset( const TIO_File_t   fileID,
                     const TIO_Object_t parentID,
                     const TIO_Object_t varID )
{
  const char *subname = "xH5_CloseVarDataset()";

  struct xH5object_t *vobject;

  int irc;


#ifndef NDEBUG
  /* Don't actually need anything from parent */
  irc = xH5_GetRef(fileID, parentID, NULL, CLASS_NULL, NULL);
  if (irc < 0)
    TIOreturn(ERR_INT, "Failed to get file/parent ref", irc);  /* Invalid fileID/parentID */
#endif


  irc = iTIO_GetFFinfo( 0, varID, (void **)&vobject );
  if (irc < 0)
    TIOreturn(ERR_INT, "Failed to get variable ffinfo", ITIO_ERR_OBJECTID);  /* Invalid varID */


  if (vobject->vardataindex < 0) {

    /* Variable is >0D, ie. has a dataset */

    herr_t  hrc;

    /* -- Close the variable dataset */
    hrc = H5Dclose(vobject->dset_id[0]);
    if (hrc < 0)
      TIOreturn(ERR_HDF5, "Failed to close variable dataset", ITIO_ERR_CLOSE);

  }

  else {

    /* Variable is 0D, ie. is in var cache */
    /* No need to do anything as caches are closed and written on parent close */

  }


  return (0);
}




/***************************************************************************************************
 *
 * xH5_WriteVarDataset
 *
 **************************************************************************************************/
int
xH5_WriteVarDataset( const TIO_File_t   fileID,
                     const TIO_Object_t parentID,
                     const TIO_Object_t varID,
                     const TIO_Data_t   datatype,
                     const void         *data )
{
  const char *subname = "xH5_WriteVarDataset()";

  struct xH5file_t     *hfile;
  struct xH5object_t   *vobject;

  hssize_t datidx;
  herr_t   hrc;
  int      irc;



  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Invalid fileID", ITIO_ERR_FILEID);

  irc = iTIO_GetFFinfo( 0, varID, (void **)&vobject );
  if (irc < 0)
    TIOreturn(ERR_INT, "Failed to get variable ffinfo", ITIO_ERR_OBJECTID);  /* Invalid varID */


  datidx = vobject->vardataindex;


  if (datidx < 0) {

    /* Variable is >0D, ie. has a proper dataset */

    hid_t   dtype_id;
    herr_t  hrc;

    hid_t   xfer_plist;

    xfer_plist = xH5_GetXfer(hfile, TIO_XFER_INDEPENDENT);

    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Invalid datatype", ITIO_ERR_DATATYPE);;

    /* Will convert on-the-fly if datatype differs to that at creation */

    hrc = H5Dwrite(vobject->dset_id[0], dtype_id, H5S_ALL, H5S_ALL, xfer_plist, data);
    if (hrc < 0)
      TIOreturn(ERR_HDF5, "Failed to write variable dataset", -12);

  }
  else {

    /* datidx >= 0 */
    /* Variable is 0D, ie. is in var cache */


    struct xH5ref_t      *pref;
    struct xH5varcache_t *vcache;

    hsize_t    refidx;
    TIO_Data_t vdtype;
    hsize_t    dims[1];
    hsize_t    coord[1][1];

    void *tempbuf;
    char *ptr;


    irc = xH5_GetRef(fileID, parentID, NULL, CLASS_VAR, &pref);
    irc = xH5_GetVcache(fileID, parentID, &vcache);

    if (irc < 0)
      TIOreturn(ERR_INT, "Failed to get parent ref", ITIO_ERR_PARENTID);  /* Invalid parentID */

    refidx = vobject->varrefindex;

    ++pref->mod;   /* Flags that caches have to be finalized */

    irc = xH5_GetVartype(vcache->types[refidx], NULL, &vdtype);

    /*
     */

    if (0 == vcache->memspace_id[vdtype]) {    /* First item to go into cache[datatype] */

      vcache->cachesize[vdtype] = xRefSizes[CLASS_VAR];

      vcache->cache[vdtype] = TIOmalloc(vcache->cachesize[vdtype] * iTIO_SizeOf(vdtype));
      if (NULL == vcache->cache[vdtype]) {
        TIOreturn(ERR_MEM, "Memory allocation error for vcache->cache[vdtype]", -11);
      }

      /* Create memory dataspace */
      dims[0] = 1;

      vcache->memspace_id[vdtype] = H5Screate_simple(1, dims, UNLIMDIMS);

      vobject->vardataindex = (hssize_t)dims[0];   /* next entry in datatype cache */

    }
    else {    /* Not the first item to go into cache[datatype] */

      /* Extend memory dataspace if necessary
         - Data could be a new addition to the end of the cache or overwriting data already in it */

      if (datidx != 0) {

        /* Data already in cache => overwriting */

        dims[0] = datidx;

        --datidx;  /* 1-origin to 0-origin */

        /* Get file dataspace from existing dataset if first overwrite (no need to extend) */
        if (0 == vcache->filspace_id[vdtype]) {
          vcache->filspace_id[vdtype] = H5Dget_space(vcache->vcache_id[vdtype]);
          TIOassert(vcache->filspace_id[vdtype] < 0, ERR_HDF5, "Failed to get dataspace", -12);
        }

      }
      else {

        /* datidx == 0 */

        /* Data not already in cache => appending */

        irc = H5Sget_simple_extent_dims(vcache->memspace_id[vdtype], dims, NULL);
        TIOassert(irc < 0, ERR_HDF5, "Failed to get dataspace extent", -15);

        datidx = dims[0];

        ++dims[0];

        hrc = H5Sset_extent_simple(vcache->memspace_id[vdtype], 1, dims, UNLIMDIMS);
        TIOassert(irc < 0, ERR_HDF5, "Failed to set dataspace extent", -16);

        vobject->vardataindex = (hssize_t)dims[0];   /* next entry in datatype cache */


        /* Expand cache if necessary */
        if (dims[0] > vcache->cachesize[vdtype]) {

          vcache->cachesize[vdtype] += xRefDelta[CLASS_VAR];

          vcache->cache[vdtype] = TIOrealloc(vcache->cache[vdtype],
                                             (vcache->cachesize[vdtype] * iTIO_SizeOf(vdtype)));
          if (NULL == vcache->cache[vdtype]) {
            TIOreturn(ERR_MEM, "Memory reallocation error for vcache->cache[vdtype]", -17);
          }
        }

      }

    }


    /* Set memory and file dataspaces to select this cache data element for later writing to file */

    coord[0][0] = datidx;

#if (  ( (H5_VERS_MAJOR == 1) && (H5_VERS_MINOR == 6) && (H5_VERS_RELEASE <= 3) ) \
    && ( defined(H5_USE_16_API) ) )
    hrc = H5Sselect_elements(vcache->memspace_id[vdtype], H5S_SELECT_APPEND, 1,
                             (const hsize_t **)coord);
#else
    hrc = H5Sselect_elements(vcache->memspace_id[vdtype], H5S_SELECT_APPEND, 1,
                             (const hsize_t *)coord);
#endif
    TIOassert(hrc < 0, ERR_HDF5, "Failed to append to memory dataspace", -20);

    coord[0][0] = (hssize_t)vcache->index[refidx] - 1;

#if (  ( (H5_VERS_MAJOR == 1) && (H5_VERS_MINOR == 6) && (H5_VERS_RELEASE <= 3) ) \
    &&  defined(H5_USE_16_API) )
    hrc = H5Sselect_elements(vcache->filspace_id[vdtype], H5S_SELECT_APPEND, 1,
                             (const hsize_t **)coord);
#else
    hrc = H5Sselect_elements(vcache->filspace_id[vdtype], H5S_SELECT_APPEND, 1,
                             (const hsize_t *)coord);
#endif
    TIOassert(hrc < 0, ERR_HDF5, "Failed to append to file dataspace", -21);



    /* Need to convert if datatype doesn't match that at creation
       -- do this into temporary buffer as sizeof(vdtype) could be > sizeof(datatype) */

    ptr  = (char *)vcache->cache[vdtype];
    ptr += (iTIO_SizeOf(vdtype) * datidx);

    irc = xH5_ConvertVar(datatype, vdtype, &data, &tempbuf);
    switch (irc) {
    case 0:
      (void) memcpy(ptr, tempbuf, iTIO_SizeOf(vdtype));
      TIOfree (tempbuf); tempbuf = NULL;
      break;
    case 1:
      (void) memcpy(ptr, data, iTIO_SizeOf(vdtype));
      break;
    case ITIO_ERR_DATATYPE:
      TIOreturn(ERR_USER, "Invalid datatype", ITIO_ERR_DATATYPE);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to perform data conversion", -22);
      break;
    }

  }


  return (0);
}




/***************************************************************************************************
 *
 * xH5_ReadVarDataset
 *
 **************************************************************************************************/
int
xH5_ReadVarDataset( const TIO_File_t   fileID,
                    const TIO_Object_t parentID,
                    const TIO_Object_t varID,
                    const TIO_Data_t   datatype,
                    void               *data )
{
  const char *subname = "xH5_ReadVarDataset()";

  struct xH5file_t   *hfile;
  struct xH5object_t *vobject;

  hssize_t datidx;
  int      irc;



  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Invalid fileID", ITIO_ERR_FILEID);

  irc = iTIO_GetFFinfo( 0, varID, (void **)&vobject );
  if (irc < 0)
    TIOreturn(ERR_INT, "Failed to get variable ffinfo", ITIO_ERR_OBJECTID);  /* Invalid varID */


  datidx = vobject->vardataindex;


  if (datidx < 0) {

    /* Variable is >0D, ie. has a proper dataset */

    hid_t   dtype_id;
    herr_t  hrc;

    hid_t   xfer_plist;

    xfer_plist = xH5_GetXfer(hfile, TIO_XFER_INDEPENDENT);

    dtype_id = xH5_GetH5type(datatype);
    if (dtype_id < 0)
      TIOreturn(ERR_USER, "Invalid datatype", ITIO_ERR_DATATYPE);

    /* Will convert on-the-fly if reading datatype differs to that in file */

    hrc = H5Dread(vobject->dset_id[0], dtype_id, H5S_ALL, H5S_ALL, xfer_plist, data);
    if (hrc < 0)
      TIOreturn(ERR_HDF5, "Failed to read variable dataset", -12);

  }
  else {

    struct xH5varcache_t *vcache;

    hsize_t    refidx;
    TIO_Data_t vdtype;
    void       *tempbuf;
    char       *ptr;


    irc = xH5_GetVcache(fileID, parentID, &vcache);
    if (irc < 0)
      TIOreturn(ERR_INT, "Failed to get parent varcache", ITIO_ERR_PARENTID);

    refidx = vobject->varrefindex;
    --datidx;  /* 1-origin to 0-origin */

    irc = xH5_GetVartype(vcache->types[refidx], NULL, &vdtype);   /* vdtype = datatype in file */


    ptr  = (char *)vcache->cache[vdtype];
    ptr += (iTIO_SizeOf(datatype) * datidx);

    irc = xH5_ConvertVar(vdtype, datatype, (const void **)&ptr, &tempbuf);
    switch (irc) {
    case 0:
      (void) memcpy(data, tempbuf, iTIO_SizeOf(vdtype));
      TIOfree (tempbuf); tempbuf = NULL;
      break;
    case 1:
      (void) memcpy(data, ptr, iTIO_SizeOf(vdtype));
      break;
    case ITIO_ERR_DATATYPE:
      TIOreturn(ERR_USER, "Invalid datatype", ITIO_ERR_DATATYPE);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to perform data conversion", -22);
      break;
    }

  }


  return (0);
}




/***************************************************************************************************
 *
 * xH5_ReadQuadAll
 *
 **************************************************************************************************/
int
xH5_ReadQuadAll( struct xH5object_t  *hobject,
                 struct xH5object_t  *hmesh,
                 const iClass_t      class,
                 const TIO_Mesh_t    meshtype,
                 const iDataPos_t    datapos,
                 const TIO_Size_t    nchunks,
                 const union iInfo_t *chunkinfo,
                 const union iInfo_t *allinfo,
                 const int           ndset,
                 const TIO_Dims_t    ndims,
                 const hid_t         dset_id,
                 const hid_t         dtype_id,
                 const hid_t         xfer_plist,
                 TIO_Size_t          *datasize,
                 void                *data )
{
  const char *subname = "xH5_ReadQuadAll()";

  const union iInfo_t *ckinfo;


  hsize_t offset[3], moffset[3];
  hsize_t nblock[3];
  hsize_t bksize[3], mbksize[3];
  hsize_t stride[3];
  hid_t   filsp_id, memsp_id;

  int     ndsp;
  int     dni, dnj, dnk;
  hsize_t mixcellcount=0, mixcompcount=0;

  TIO_Size_t ichunk;

  herr_t hrc;
  int    irc;


  /* data can be NULL */


  /* Handle special case of colinear quadmesh */

  if ((CLASS_MESH == class) && (TIO_MESH_QUAD_COLINEAR == meshtype)) {

    if (data != NULL) {
      hrc = H5Dread(dset_id, dtype_id, H5S_ALL, H5S_ALL, xfer_plist, data);
      if (hrc < 0)
        TIOreturn(ERR_HDF5, "Failed to read quadmesh dataset", -100);
    }

    if (datasize != NULL) {
      switch (ndset) {
      case 0:
        *datasize = allinfo->quad.ni;
        break;
      case 1:
        *datasize = allinfo->quad.nj;
        break;
      case 2:
        *datasize = allinfo->quad.nk;
        break;
      }
    }

    return (0);
  }


  /* All other objects will be stored as whole mesh-based */

  /* Can map chunks stored as 1D in file straight to 1/2/3-D in memory */
  /* This makes selecting memory hyperslabs a fair bit easier!         */


  /* Could just read this as a section... */


  irc = iTIO_DataposAdjust(datapos, &dni, &dnj, &dnk);

  if (data != NULL) {

    /* data needs to be read */


    switch (ndims) {
    case TIO_3D:
      stride[0] = (hsize_t)allinfo->quad.nk - (hsize_t)dnk;
      stride[1] = (hsize_t)allinfo->quad.nj - (hsize_t)dnj;
      stride[2] = (hsize_t)allinfo->quad.ni - (hsize_t)dni;
      break;
    case TIO_2D:
      stride[0] = (hsize_t)allinfo->quad.nj - (hsize_t)dnj;
      stride[1] = (hsize_t)allinfo->quad.ni - (hsize_t)dni;
      break;
    case TIO_1D:
      stride[0] = (hsize_t)allinfo->quad.ni - (hsize_t)dni;
      break;
    default:
      break;
    }


    switch (datapos) {
    case DATAPOS_MIXED:
    case DATAPOS_MIXCOMP:
      ndsp = 1;
      nblock[0]= (hsize_t)allinfo->quad.nmixcomp;
      break;
    case DATAPOS_MIXCELL:
      ndsp = 1;
      nblock[0]= (hsize_t)allinfo->quad.nmixcell;
      break;

    default:
      /* all other normal mesh-based datapos */
      ndsp      = (int)ndims;
      nblock[0] = stride[0];
      nblock[1] = stride[1];
      nblock[2] = stride[2];
      break;
    }


    memsp_id = H5Screate_simple(ndsp, nblock, nblock);
    TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -2);


    /* Could be clever about this and select hyperslabs within file and memory to cover all that is
       necessary and do one single read, however, much simpler to do it per chunk so removing ghosts
       and things is easier - get to (re)use other routines */

    nblock[0] = 1;
    nblock[1] = 1;
    nblock[2] = 1;

    for (ichunk=0; ichunk<nchunks; ++ichunk) {

      ckinfo = &chunkinfo[ichunk];

      /* Get chunk filespace (without ghosts) */
      irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, TIO_MESH_QUAD_NONCOLINEAR, datapos,
                                ndset, ndims, 0, nchunks, chunkinfo,  FALSE, ichunk,
                                &mbksize[0], &filsp_id);
      TIOassert(irc < 0, ERR_INT, "Failed to get file dataspace for chunk", -3);

      /* keep track of memory hyperslab in this routine - keep offset & strides */

      /* select this chunks position in memory space */

      switch (ndims) {
      case TIO_3D:
        offset[0] = (hsize_t)ckinfo->quad.kl;
        offset[1] = (hsize_t)ckinfo->quad.jl;
        offset[2] = (hsize_t)ckinfo->quad.il;
        bksize[0] = (hsize_t)ckinfo->quad.nk - (hsize_t)dnk;
        bksize[1] = (hsize_t)ckinfo->quad.nj - (hsize_t)dnj;
        bksize[2] = (hsize_t)ckinfo->quad.ni - (hsize_t)dni;
        break;
      case TIO_2D:
        offset[0] = (hsize_t)ckinfo->quad.jl;
        offset[1] = (hsize_t)ckinfo->quad.il;
        bksize[0] = (hsize_t)ckinfo->quad.nj - (hsize_t)dnj;
        bksize[1] = (hsize_t)ckinfo->quad.ni - (hsize_t)dni;
        break;
      case TIO_1D:
        offset[0] = (hsize_t)ckinfo->quad.il;
        bksize[0] = (hsize_t)ckinfo->quad.ni - (hsize_t)dni;
        break;
      default:
        break;
      }

      switch (datapos) {
      case DATAPOS_MIXED:
      case DATAPOS_MIXCOMP:
        moffset[0] = mixcompcount;
        /* mbksize[0] unchanged */
        break;
      case DATAPOS_MIXCELL:
	    moffset[0] = mixcellcount;
		break;

      default:
        /* all other normal mesh-based datapos */
        moffset[0] = offset[0];
        moffset[1] = offset[1];
        moffset[2] = offset[2];
        mbksize[0] = bksize[0];
        mbksize[1] = bksize[1];
        mbksize[2] = bksize[2];
        break;
      }

      hrc = H5Sselect_hyperslab(memsp_id, H5S_SELECT_SET, moffset, NULL, nblock, mbksize );
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select memory hyperslab", -5);

      hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
      if (hrc < 0)
        TIOreturn(ERR_HDF5, "Failed to read chunk hyperslab", -6);


      /* Juggle any mixed material data */

      if ((CLASS_MATERIAL == class) && (ckinfo->quad.nmixcell > 0)) {
        if ( (MAT_MATERIAL == ndset) && (DATAPOS_CELL == datapos) ) {   /* mat, not mixmat */
          irc = xH5_ConvertMaterial( TRUE, dtype_id, mixcellcount, ndims, offset,
                                     mbksize, stride, data );
        }
        else if (MAT_PTR == ndset) {   /* ptr */
          irc = xH5_ConvertMaterial( FALSE, dtype_id, mixcompcount, TIO_1D, &mixcellcount,
                                     mbksize, stride, data );
        }
        else {
          /* not a material dataset that needs converting */
          ;
        }
        TIOassert(irc < 0, ERR_INT, "Failed in material conversion", -500);

        /* Don't need to do anything to ncomp, mixmat and vf arrays */
      }

      mixcellcount += (hsize_t)ckinfo->quad.nmixcell;
      mixcompcount += (hsize_t)ckinfo->quad.nmixcomp;


    }

    hrc = H5Sclose(memsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -7);

  }


  if (datasize != NULL) {

    switch (datapos) {

    case DATAPOS_MIXED:
    case DATAPOS_MIXCOMP:
      *datasize = (hsize_t)allinfo->quad.nmixcomp;
      break;

    case DATAPOS_MIXCELL:
      *datasize = (hsize_t)allinfo->quad.nmixcell;
      break;


    default:
      switch (ndims) {
      case TIO_3D:
        datasize[2] = allinfo->quad.nk - (hsize_t)dnk;
      case TIO_2D:
        datasize[1] = allinfo->quad.nj - (hsize_t)dnj;
      default:
        break;
      }
      datasize[0]   = allinfo->quad.ni - (hsize_t)dni;

      break;
    }

  }

  return (0);
}




/***************************************************************************************************
 *
 * xH5_ReadUnstrAll
 *
 **************************************************************************************************/
int
xH5_ReadUnstrAll(  struct xH5object_t  *hobject,
                   struct xH5object_t  *hmesh,
                   const iClass_t      class,
                   const iDataPos_t    datapos,
                   const TIO_Size_t    nchunks,
                   const union iInfo_t *chunkinfo,
                   const union iInfo_t *allinfo,
                   const int           ndset,
                   const hid_t         dset_id,
                   const hid_t         dtype_id,
                   const hid_t         xfer_plist,
                   TIO_Size_t          *datasize,
                   void                *data )
{
  const char *subname = "iH5_ReadUnstrAll()";

  hsize_t *gnodeIDs;

  hsize_t offset[1], memoffset[1];
  hsize_t nblock[1];
  hsize_t bksize[1];
  hid_t   filsp_id, memsp_id;
  hid_t   nodal_dset_id;
  hsize_t mixcellcount=0, mixcompcount=0;
  TIO_Size_t ichunk, i;

  herr_t  hrc;
  int     irc;


  switch (datapos) {

  case DATAPOS_CELL:
  case DATAPOS_SHAPE:
  case DATAPOS_MIXED:
  case DATAPOS_MIXCELL:
  case DATAPOS_MIXCOMP:

    /* Append individual chunks contiguously */

    switch (datapos) {
    case DATAPOS_CELL:
      bksize[0] = (hsize_t)allinfo->unstr.ncells;
      break;
    case DATAPOS_SHAPE:
      bksize[0] = (hsize_t)allinfo->unstr.nshapes;
      break;
    case DATAPOS_MIXED:
    case DATAPOS_MIXCOMP:
      bksize[0] = (hsize_t)allinfo->unstr.nmixcomp;
      break;
    case DATAPOS_MIXCELL:
      bksize[0] = (hsize_t)allinfo->unstr.nmixcell;
      break;
    default:
      break;
    }

    if (NULL == data) {
      TIOset(datasize, bksize[0]);
      return (0);
    }


    memoffset[0] = 0;

    memsp_id = H5Screate_simple(1, bksize, bksize);
    TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -1);

    for (ichunk=0; ichunk<nchunks; ++ichunk) {

      /* Get space of chunk (without ghosts) within the file */
      irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, TIO_MESH_UNSTRUCT,
                                datapos, ndset, TIO_1D, TIO_GHOSTS_NONE,
                                nchunks, chunkinfo,  FALSE,
                                ichunk, &bksize[0], &filsp_id);
      TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -2);

      /* Now set next dataspace bit in memory to read into */
      nblock[0] = 1;
      hrc = H5Sselect_hyperslab(memsp_id, H5S_SELECT_SET, memoffset, NULL, nblock, bksize);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select file hyperslab", -5);

      hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
      if (hrc < 0)
        TIOreturn(ERR_HDF5, "Failed to read unstruct dataset", -7);

      hrc = H5Sclose(filsp_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -8);


      /* Juggle any mixed material data */

      if ((CLASS_MATERIAL == class) && (chunkinfo[ichunk].unstr.nmixcell > 0)) {
        if ( (MAT_MATERIAL == ndset) && (DATAPOS_CELL == datapos) ) {   /* mat, not mixmat */
          irc = xH5_ConvertMaterial( TRUE, dtype_id, mixcellcount, TIO_1D, memoffset,
                                     bksize, bksize, data );    /* 2nd bksize is dummy */
        }
        else if (MAT_PTR == ndset) {   /* ptr */
          irc = xH5_ConvertMaterial( FALSE, dtype_id, mixcompcount, TIO_1D, &mixcellcount,
                                     bksize, bksize, data );    /* 2nd bksize is dummy */
        }
        else {
          /* not a material dataset that needs converting */
          ;
        }
        TIOassert(irc < 0, ERR_INT, "Failed in material conversion", -500);

        /* Don't need to do anything to ncomp, mixmat and vf arrays */

        mixcellcount += (hsize_t)chunkinfo[ichunk].unstr.nmixcell;
        mixcompcount += (hsize_t)chunkinfo[ichunk].unstr.nmixcomp;
      }


      memoffset[0] += bksize[0];

    }

    hrc = H5Sclose(memsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -9);

    break;


  case DATAPOS_NODE:

    /* Reorder nodal data into ascending global node ID */

    bksize[0] = (hsize_t)allinfo->unstr.nnodes;

    if (NULL == data) {
      TIOset(datasize, bksize[0]);
      return (0);
    }


    nodal_dset_id = hmesh->dset_id[UNSTR_NODEIDS];

    memsp_id = H5Screate_simple(1, bksize, bksize);
    TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -10);

    gnodeIDs = NULL;

    for (ichunk=0; ichunk<nchunks; ++ichunk) {

      /* Read local copy of global node IDs per chunk (just the non-ghosts) */
      /* We do this per-chunk to use as little memory as possible as full number of nodes
         could be huge - although it is a bit wasteful reading this every time... ho hum */

      irc = xH5_GetChunkedSpace( hobject, SPACE_FILE, class, TIO_MESH_UNSTRUCT, DATAPOS_NODE,
                                 UNSTR_NODEIDS, TIO_1D, TIO_GHOSTS_NONE,
                                 nchunks, chunkinfo,  FALSE, ichunk,
                                 &bksize[0], &filsp_id );
      TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -11);


      gnodeIDs = (hsize_t *)TIOrealloc(gnodeIDs, bksize[0] * sizeof(hsize_t));
      if (NULL == gnodeIDs) {
        printf("ndset=%d\nichunk=%d\nbksize[0]=%d\n", (int)ndset, (int)ichunk, (int)bksize[0]);
        TIOreturn(ERR_MEM, "Failed to allocate gnodeIDs", -14);
      }

      /* Select the front of the full memory space temporarily for reading local copy of node IDs */
      offset[0] = 0;
      nblock[0] = 1;
      hrc = H5Sselect_hyperslab(memsp_id, H5S_SELECT_SET, offset, NULL, nblock, bksize);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select memory hyperslab", -16);

      hrc = H5Dread(nodal_dset_id, H5T_NATIVE_HSIZE, memsp_id, filsp_id, xfer_plist, gnodeIDs);
      if (hrc < 0)
        TIOreturn(ERR_HDF5, "Failed to read nodeIDs dataset", -17);



      /* Now have global node IDs for this chunk */
      /* Use these node IDs as index for picking out nodal data - this automagically re-orders
         into global nodeID order and resolves duplicate nodes (those shared between chunks) */

      for (i=0; i<bksize[0]; ++i) {
        gnodeIDs[i] = gnodeIDs[i] + offset[0] - 1;
      }

      /* Re-use the memory dataspace back for the bits of the full one */

#if (  ( (H5_VERS_MAJOR == 1) && (H5_VERS_MINOR == 6) && (H5_VERS_RELEASE <= 3) ) \
    &&  defined(H5_USE_16_API) )
      hrc = H5Sselect_elements(memsp_id, H5S_SELECT_SET, (size_t)bksize[0],
                               (const hsize_t **)gnodeIDs);
#else
      hrc = H5Sselect_elements(memsp_id, H5S_SELECT_SET, (size_t)bksize[0],
                               (const hsize_t *)gnodeIDs);
#endif

      TIOassert(hrc < 0, ERR_HDF5, "Failed to select elements", -19);

      hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
      if (hrc < 0)
        TIOreturn(ERR_HDF5, "Failed to read nodal-based dataset", -20);


      hrc = H5Sclose(filsp_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -18);
    }

    hrc = H5Sclose(memsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -21);

    TIOfree (gnodeIDs); gnodeIDs = NULL;

    memoffset[0] = (hsize_t)allinfo->unstr.nnodes;

    break;


  case DATAPOS_CONNECT:

    /* Juggle connectivity around wrt global node IDs */

    bksize[0] = (hsize_t)allinfo->unstr.nconnectivity;   /* Will be >= unstr.nnodes */

    if (NULL == data) {
      TIOset(datasize, bksize[0]);
      return (0);
    }


    nodal_dset_id = hmesh->dset_id[UNSTR_NODEIDS];

    memoffset[0] = 0;

    memsp_id = H5Screate_simple(1, bksize, bksize);
    TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -24);

    gnodeIDs = NULL;

    for (ichunk=0; ichunk<nchunks; ++ichunk) {

      /* Read global node IDs for chunk */

      irc = xH5_GetChunkedSpace( hobject, SPACE_FILE, class, TIO_MESH_UNSTRUCT, DATAPOS_NODE,
                                 UNSTR_NODEIDS, TIO_1D, TIO_GHOSTS_NONE,
                                 nchunks, chunkinfo,  FALSE, ichunk,
                                 &bksize[0], &filsp_id);
      TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -26);


      gnodeIDs = (hsize_t *)TIOrealloc(gnodeIDs, bksize[0] * sizeof(hsize_t));
      if (NULL == gnodeIDs)
        TIOreturn(ERR_MEM, "Failed to allocate gnodeIDs", -27);

      /* Select the front of the full memory space temporarily for reading local copy of node IDs */
      offset[0] = 0;
      nblock[0] = 1;

      hrc = H5Sselect_hyperslab(memsp_id, H5S_SELECT_SET, offset, NULL, nblock, bksize);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select memory hyperslab", -28);

      hrc = H5Dread(nodal_dset_id, H5T_NATIVE_HSIZE, memsp_id, filsp_id, xfer_plist, gnodeIDs);
      if (hrc < 0)
        TIOreturn(ERR_HDF5, "Failed to read nodeIDs dataset", -29);

      hrc = H5Sclose(filsp_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -30);


      /* Now read connectivity-centred data for the chunk */

      irc = xH5_GetChunkedSpace( hobject, SPACE_FILE, class, TIO_MESH_UNSTRUCT, DATAPOS_CONNECT,
                                 UNSTR_CONNECT, TIO_1D, TIO_GHOSTS_NONE,
                                 nchunks, chunkinfo,  FALSE, ichunk,
                                 &bksize[0], &filsp_id);
      TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -31);


      hrc = H5Sselect_hyperslab(memsp_id, H5S_SELECT_SET, memoffset, NULL, nblock, bksize);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select file hyperslab", -32);

      hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
      if (hrc < 0)
        TIOreturn(ERR_HDF5, "Failed to read connectivity-based dataset", -33);


      hrc = H5Sclose(filsp_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -34);


      /* If the data is the actually mesh connectivity (could be a connectivity based quant) then
         use global node IDs to fiddle connectivity it */

      if (CLASS_MESH == class) {

        switch (H5Tget_size(dtype_id)) {
        case 2:
          {
            int16_t *i2ptr, i2nid;

            i2ptr = (int16_t *)data + memoffset[0];
            for (i=0; i<bksize[0]; ++i) {
              i2nid  = (*i2ptr) - 1;
              i2nid  = (int16_t)*(gnodeIDs + i2nid);
              *i2ptr = i2nid;
              ++i2ptr;
            }
          }
          break;

        case 4:
          {
            int32_t *i4ptr, i4nid;

            i4ptr = (int32_t *)data + memoffset[0];
            for (i=0; i<bksize[0]; ++i) {
              i4nid  = (*i4ptr) - 1;
              i4nid  = (int32_t)*(gnodeIDs + i4nid);
              *i4ptr = i4nid;
              ++i4ptr;
            }
          }
          break;

        case 8:
          {
            int64_t *i8ptr, i8nid;

            i8ptr = (int64_t *)data + memoffset[0];
            for (i=0; i<bksize[0]; ++i) {
              i8nid  = (*i8ptr) - 1;
              i8nid  = (int64_t)*(gnodeIDs + i8nid);
              *i8ptr = i8nid;
              ++i8ptr;
            }
          }
          break;

        default:
          TIOreturn(ERR_INT, "Unsupported/failed bit-size for connectivity datatype value", -35);
          break;
        }
      }

      memoffset[0] += bksize[0];

    }


    hrc = H5Sclose(memsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -36);

    TIOfree (gnodeIDs); gnodeIDs = NULL;

    break;


  default:
    break;
  }


  TIOset (datasize, (TIO_Size_t)memoffset[0]);

  return (0);
}



/***************************************************************************************************
 *
 * xH5_ReadPointAll
 *
 **************************************************************************************************/
int
xH5_ReadPointAll( struct xH5object_t  *hobject,
                  struct xH5object_t  *hmesh,
                  const iClass_t      class,
                  const iDataPos_t    datapos,
                  const TIO_Size_t    nchunks,
                  const union iInfo_t *chunkinfo,
                  const union iInfo_t *allinfo,
                  const int           ndset,
                  const hid_t         dset_id,
                  const hid_t         dtype_id,
                  const hid_t         xfer_plist,
                  TIO_Size_t          *datasize,
                  void                *data )
{
  const char *subname = "xH5_ReadPointAll()";

  hsize_t memoffset[1];
  hsize_t nblock[1];
  hsize_t bksize[1];
  hsize_t memsize[1];
  hid_t   filsp_id, memsp_id;

  TIO_Size_t ichunk;

  herr_t  hrc;
  int     irc;


  TIOassert((datapos != DATAPOS_NODE), ERR_INT, "datapos must be nodal for point meshes", -1);

  memsize[0] = (hsize_t)allinfo->point.nnodes;

  if (data != NULL) {

    /* data needs to be read */

    nblock[0] = 1;

    memsp_id = H5Screate_simple(1, memsize, memsize);
    TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -2);

    for (ichunk=0; ichunk<nchunks; ++ichunk) {

      memoffset[0] = chunkinfo[ichunk].point.nl;

      irc = xH5_GetChunkedSpace( hobject, SPACE_FILE, class, TIO_MESH_POINT, DATAPOS_NODE,
                                 ndset, TIO_1D, TIO_GHOSTS_NONE,
                                 nchunks, chunkinfo,  FALSE, ichunk,
                                 &bksize[0], &filsp_id );
      TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -3);

      hrc = H5Sselect_hyperslab(memsp_id, H5S_SELECT_SET, memoffset, NULL, nblock, bksize);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select file hyperslab", -4);

      hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
      if (hrc < 0)
        TIOreturn(ERR_HDF5, "Failed to read dataset", -5);

      hrc = H5Sclose(filsp_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -6);

      memoffset[0] += bksize[0];
    }

    hrc = H5Sclose(memsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -7);
  }


  TIOset (datasize, (TIO_Size_t)memsize[0]);


  return (0);
}





/**************************************************************************************************/

int
xH5_ReadQuadMeshSection( const int              ndset,
                         const hid_t            dset_id,
                         const hid_t            dtype_id,
                         const hid_t            xfer_plist,
                         const int              nodata,
                         const int              collective,
                         const union iSection_t *sectioninfo,
                         TIO_Size_t             *datasize,
                         void                   *data )
{
  const char *subname = "xH5_ReadQuadMeshSection()";

  int     spdims = 1;
  hsize_t offset[3];
  hsize_t nblock[3] = {1,1,1};
  hsize_t bksize[3];

  hid_t memsp_id, filsp_id;

  herr_t  hrc;


  /* Handle colinear quadmesh case */

  switch (ndset) {
  case 0:
    offset[0] = (hsize_t)(sectioninfo->quad.il);
    bksize[0] = (hsize_t)(sectioninfo->quad.ih - sectioninfo->quad.il + 1);
    break;
  case 1:
    offset[0] = (hsize_t)(sectioninfo->quad.jl);
    bksize[0] = (hsize_t)(sectioninfo->quad.jh - sectioninfo->quad.jl + 1);
    break;
  case 2:
    offset[0] = (hsize_t)(sectioninfo->quad.kl);
    bksize[0] = (hsize_t)(sectioninfo->quad.kh - sectioninfo->quad.kl + 1);
    break;
  }

  if (collective) {

    if (nodata) {

      /* set up an empty space to do no read */
      memsp_id = xH5_H5Screate_simple_none();
      TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create empty dataspace", -1);

      filsp_id = xH5_H5Screate_simple_none();
      TIOassert(filsp_id < 0, ERR_HDF5, "Failed to create empty dataspace", -2);
    }
    else {
      /* Create memory and file dataspaces */
      memsp_id = H5Screate_simple(spdims, bksize, NULL);
      TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -3);

      filsp_id = H5Dget_space(dset_id);
      TIOassert(filsp_id < 0, ERR_HDF5, "Failed to create file dataspace", -4);

      /* Select hyperslab of section within file */
      hrc = H5Sselect_hyperslab(filsp_id, H5S_SELECT_SET, offset, NULL, nblock, bksize);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to set hyperslab", -5);
    }

    hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
    if (hrc < 0) {
      TIOreturn(ERR_HDF5, "Failed to read quad section", -6);
    }

    hrc = H5Sclose(memsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -7);

    hrc = H5Sclose(filsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -8);
  }
  else if (!nodata) {     /* and also not collective! */

    /* Create memory and file dataspaces */
    memsp_id = H5Screate_simple(spdims, bksize, NULL);
    TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -21);

    filsp_id = H5Dget_space(dset_id);
    TIOassert(filsp_id < 0, ERR_HDF5, "Failed to create file dataspace", -22);

    /* Select hyperslab of section within file */
    hrc = H5Sselect_hyperslab(filsp_id, H5S_SELECT_SET, offset, NULL, nblock, bksize);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set hyperslab", -23);

    /* Read the data */
    hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
    if (hrc < 0) {
      TIOreturn(ERR_HDF5, "Failed to read quad section", -24);
    }

    /* Close dataspaces */
    hrc = H5Sclose(memsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -25);

    hrc = H5Sclose(filsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -26);
  }

  TIOset(datasize, (TIO_Size_t)(bksize[0]));

  return (0);
}




/**************************************************************************************************/
int
xH5_GetSection( struct xH5file_t          *hfile,
                struct xH5object_t        *hobject,
                struct xH5object_t        *hmesh,
                struct xH5section_t       *hsection,
                const TIO_Mesh_t          meshtype,
                const TIO_Dims_t          ndims,
                const TIO_Size_t          nchunks,
                const union iInfo_t       *chunkinfo,
                const union iSection_t    *sectioninfo,
                const iDataPos_t          datapos,
                struct xH5sectionlookup_t **hsectlookup,
                hsize_t                   *datasize )
{
  const char *subname = "xH5_GetSection()";

  const TIO_Size_t delta = 2;

  TIO_Size_t i;
  int irc;


  switch (datapos) {
  case DATAPOS_MIXED:
  case DATAPOS_MIXCOMP:
    irc = xH5_GetSection( hfile, hobject, hmesh, hsection,
                          meshtype, ndims, nchunks, chunkinfo, sectioninfo, 
			  DATAPOS_CELL, hsectlookup, datasize );
    datasize[0] = hsection->nmixcomp;
    return (0);
    break;
  case DATAPOS_MIXCELL:
    irc = xH5_GetSection( hfile, hobject, hmesh, hsection,
                          meshtype, ndims, nchunks, chunkinfo, sectioninfo, 
                          DATAPOS_CELL, hsectlookup, datasize );
    datasize[0] = hsection->nmixcell;
    return (0);
    break;

  default:
    break;
  }

  irc = FALSE;

  for (i=0; i<hsection->nlist; ++i) {

    irc = (hsection->lookup[i].datapos == datapos);

    if (irc) break;

  }



  if (irc){

    *hsectlookup = &hsection->lookup[i];

    datasize[0] = hsection->lookup[i].datasize[0];
    datasize[1] = hsection->lookup[i].datasize[1];
    datasize[2] = hsection->lookup[i].datasize[2];

    return (0);

  }
  else {

    /* Calculate and store for later */

    hsize_t    dsize[3] = { 0, 0, 0};
    TIO_Size_t ii, ichunk;

    int    irc;
    herr_t hrc;

    struct xH5sectionlookup_t *llookup;


    i = hsection->nlist;

    if (i+1 > hsection->nalloc) {

      TIO_Size_t n;

      n = hsection->nalloc + delta;

      hsection->lookup = TIOrealloc(hsection->lookup, sizeof(struct xH5sectionlookup_t) * n);
      if (NULL == hsection->lookup)
        TIOreturn(ERR_MEM, "Failed to realloc hsection->lookup", -1);

#ifndef NDEBUG
      for (ii=hsection->nalloc; ii<n; ++ii) {
        hsection->lookup[ii].datapos = DATAPOS_NULL;
      }
#endif

      hsection->nalloc = n;
    }

    ++hsection->nlist;

    llookup = &hsection->lookup[i];

    llookup->datapos = datapos;


    if (NULL == hsection->chunklog) {

      hsection->chunklog = (TIO_Size_t *)TIOmalloc((size_t)nchunks * sizeof(TIO_Size_t));
      if (NULL == hsection->chunklog)
        TIOreturn(ERR_MEM, "Failed to malloc hsection->chunklog", -101);

      for (ichunk=0; ichunk<nchunks; ++ichunk) {
        hsection->chunklog[ichunk] = 0;
      }

      hsection->nmixcell = 0;
      hsection->nmixcomp = 0;



    }


    switch (meshtype) {

    case TIO_MESH_QUAD_COLINEAR:     /************************  QUAD      ************************/
    case TIO_MESH_QUAD_NONCOLINEAR:
      {
        TIO_Size_t cil, cih, cjl, cjh, ckl, ckh;  /* Piece within whole chunk that contributes */
        TIO_Size_t sil, sih, sjl, sjh, skl, skh;  /* Piece of section within memory            */
        int        dni, dnj, dnk;                 /* Centring adjusters                        */
        TIO_Size_t ick;

        const union iInfo_t *ckinfo;



        irc = iTIO_DataposAdjust(datapos, &dni, &dnj, &dnk);

        switch (ndims) {
        case TIO_3D:
          dsize[2] = sectioninfo->quad.kh - sectioninfo->quad.kl + 1 - dnk;
        case TIO_2D:
          dsize[1] = sectioninfo->quad.jh - sectioninfo->quad.jl + 1 - dnj;
        case TIO_1D:
          dsize[0] = sectioninfo->quad.ih - sectioninfo->quad.il + 1 - dni;
        default:
          break;
        }

        llookup->secthyper = NULL;
        llookup->secthyper = (TIO_Size_t *)TIOmalloc((size_t)nchunks * 6 * sizeof(TIO_Size_t));
        if (NULL == llookup->secthyper)
          TIOreturn(ERR_MEM, "Failed to malloc hsection->secthyper", -102);

        llookup->sectchunk = NULL;
        llookup->sectchunk = (TIO_Size_t *)TIOmalloc((size_t)nchunks * 6 * sizeof(TIO_Size_t));
        if (NULL == llookup->sectchunk)
          TIOreturn(ERR_MEM, "Failed to malloc hsection->sectchunk", -103);


        for (ichunk=0, ick=0; ichunk<nchunks; ++ichunk, ick+=6) {

          ckinfo = &chunkinfo[ichunk];

          irc = TRUE;

          switch (ndims) {
          case TIO_3D:
            skl = (hsize_t)TIOmax(ckinfo->quad.kl, sectioninfo->quad.kl);
            skh = (hsize_t)TIOmin(ckinfo->quad.kh, sectioninfo->quad.kh);
            irc &= (skh >= skl);
            /* drop through */
          case TIO_2D:
            sjl = (hsize_t)TIOmax(ckinfo->quad.jl, sectioninfo->quad.jl);
            sjh = (hsize_t)TIOmin(ckinfo->quad.jh, sectioninfo->quad.jh);
            irc &= (sjh >= sjl);
            /* drop through */
          case TIO_1D:
            sil = (hsize_t)TIOmax(ckinfo->quad.il, sectioninfo->quad.il);
            sih = (hsize_t)TIOmin(ckinfo->quad.ih, sectioninfo->quad.ih);
            irc &= (sih >= sil);
          default:
            break;
          }

          if (!irc) continue;   /* Chunk does not contribute to section */

          hsection->chunklog[ichunk] = 1;

          if (1 == hsection->nlist) {
            hsection->nmixcell += ckinfo->quad.nmixcell;
            hsection->nmixcomp += ckinfo->quad.nmixcomp;

          }

          switch (ndims) {
          case TIO_3D:
            ckl  = skl - ckinfo->quad.kl;
            ckh  = skh - ckinfo->quad.kl - dnk;
            skl -= sectioninfo->quad.kl;
            skh -= sectioninfo->quad.kl + dnk;  /* + due to associativity of - */
            /* drop through */
          case TIO_2D:
            cjl  = sjl - ckinfo->quad.jl;
            cjh  = sjh - ckinfo->quad.jl - dnj;
            sjl -= sectioninfo->quad.jl;
            sjh -= sectioninfo->quad.jl + dnj;
            /* drop through */
          case TIO_1D:
            cil  = sil - ckinfo->quad.il;
            cih  = sih - ckinfo->quad.il - dni;
            sil -= sectioninfo->quad.il;
            sih -= sectioninfo->quad.il + dni;
          default:
            break;
          }

          llookup->secthyper[ick + 0] = sil;
          llookup->secthyper[ick + 1] = sih;
          llookup->secthyper[ick + 2] = sjl;
          llookup->secthyper[ick + 3] = sjh;
          llookup->secthyper[ick + 4] = skl;
          llookup->secthyper[ick + 5] = skh;

          llookup->sectchunk[ick + 0] = cil;
          llookup->sectchunk[ick + 1] = cih;
          llookup->sectchunk[ick + 2] = cjl;
          llookup->sectchunk[ick + 3] = cjh;
          llookup->sectchunk[ick + 4] = ckl;
          llookup->sectchunk[ick + 5] = ckh;
        }

      }
      break;

    case TIO_MESH_UNSTRUCT:          /************************  UNSTRUCT  ************************/
      {
        int        idx_ndset;
        iDataPos_t idx_datapos;
        hid_t      idx_filsp_id, idx_memsp_id, idx_dset_id, idx_dtype_id;
        hid_t      xfer_plist;
        hsize_t    cksize[1];
        TIO_Size_t *lckIDs;
        int        *log;
        TIO_Size_t ic, is, cki, ssi, lpi, ckc;

        switch (datapos) {
        case DATAPOS_CELL:
          idx_ndset   = UNSTR_CELLIDS;
          idx_datapos = DATAPOS_CELL;
          break;
        case DATAPOS_NODE:
          idx_ndset   = UNSTR_NODEIDS;
          idx_datapos = DATAPOS_NODE;
          break;
        default:
          TIOreturn(ERR_INT, "Must be cell- or nodal- centred section", -100);
        }


        llookup->secthyper = NULL;
        llookup->secthyper =
          (TIO_Size_t *)TIOmalloc((size_t)sectioninfo->unstr.nIDs * sizeof(TIO_Size_t));
        if (NULL == llookup->secthyper)
          TIOreturn(ERR_MEM, "Failed to malloc hsection->secthyper", -102);

        llookup->sectchunk = NULL;
        llookup->sectchunk =
          (TIO_Size_t *)TIOmalloc((size_t)sectioninfo->unstr.nIDs * sizeof(TIO_Size_t));
        if (NULL == llookup->sectchunk)
          TIOreturn(ERR_MEM, "Failed to malloc hsection->sectchunk", -103);

        idx_dset_id  = hmesh->dset_id[idx_ndset];
        idx_dtype_id = xH5_GetH5type(TIO_SIZE_T);

        log = NULL;
        log = (int *)TIOmalloc((size_t)sectioninfo->unstr.nIDs * sizeof(int));
        if (NULL == log)
          TIOreturn(ERR_MEM, "Failed to malloc log[]", -103);

        for (is=0; is<sectioninfo->unstr.nIDs; ++is) {
          log[is] = FALSE;
        }


        lckIDs = NULL;
        lpi    = 0;

        for (ichunk=0; ichunk<nchunks; ++ichunk) {

          xfer_plist = xH5_GetXfer(hfile, TIO_XFER_INDEPENDENT);

          /* Read chunk ID data without ghosts - not needed in section selection */

          /* Get space for chunk within dataset in file */

          irc = xH5_GetChunkedSpace( hmesh, SPACE_FILE, CLASS_MESH, TIO_MESH_UNSTRUCT, idx_datapos,
                                     idx_ndset, TIO_1D, TIO_GHOSTS_NONE,
                                     nchunks, chunkinfo,  FALSE,
                                     ichunk, cksize, &idx_filsp_id);
          TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -105);

          idx_memsp_id = H5Screate_simple(1, cksize, cksize);
          TIOassert(idx_memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -106);

          lckIDs = (TIO_Size_t *)TIOrealloc(lckIDs, (size_t)cksize[0] * sizeof(TIO_Size_t));
          if (NULL == lckIDs)
            TIOreturn(ERR_MEM, "Failed to allocate lckIDs", -107);

          hrc = H5Dread(idx_dset_id, idx_dtype_id, idx_memsp_id, idx_filsp_id, xfer_plist, lckIDs);
          TIOassert(hrc < 0, ERR_HDF5, "Failed to read local chunk IDs", -108);

          hrc = H5Sclose(idx_memsp_id);
          hrc = H5Sclose(idx_filsp_id);


          ckc = 0;

          for (is = 0; is<sectioninfo->unstr.nIDs; ++is) {
            ssi = sectioninfo->unstr.IDs[is];

            if (log[is]) continue;   /* Have already found this element in the section */

            for (ic=0; ic<cksize[0]; ++ic) {
              cki = lckIDs[ic];

              if (cki == ssi) {

                llookup->secthyper[lpi] = is;
                llookup->sectchunk[lpi] = ic;
                ++lpi;

                log[is] = TRUE;
                ++ckc;
                break;  /* No point looking through rest of chunk */
              }
            }
          }

          hsection->chunklog[ichunk] = ckc;
          if (ckc > 0) {
            hsection->nmixcell += chunkinfo[ichunk].unstr.nmixcell;
            hsection->nmixcomp += chunkinfo[ichunk].unstr.nmixcomp;

          }
        }


        /* --- POSSIBLE_ERROR_CHECK ---
           irc = FALSE;
           for (is = 0; is<sectioninfo->unstr.nIDs; ++is) {
             if (irc = !log[is]) break;
           }
           if (irc) {
             TIOfree(lckIDs); lckIDs = NULL;
             TIOfree(log);    log    = NULL;
             TIOreturn(ERR_USER, "Invalid IDs given in section", -108);
           }
        */

        dsize[0] = (hsize_t)sectioninfo->unstr.nIDs;

        TIOfree(lckIDs); lckIDs = NULL;
        TIOfree(log);    log    = NULL;
      }
      break;


    case TIO_MESH_POINT:          /************************  POINT  ************************/
      {
        TIO_Size_t ic, is, ssi, lpi, ckc;

        llookup->secthyper = NULL;
        llookup->secthyper =
          (TIO_Size_t *)TIOmalloc((size_t)sectioninfo->point.nIDs * sizeof(TIO_Size_t));
        if (NULL == llookup->secthyper)
          TIOreturn(ERR_MEM, "Failed to malloc hsection->secthyper", -102);

        llookup->sectchunk = NULL;
        llookup->sectchunk =
          (TIO_Size_t *)TIOmalloc((size_t)sectioninfo->point.nIDs * sizeof(TIO_Size_t));
        if (NULL == llookup->sectchunk)
          TIOreturn(ERR_MEM, "Failed to malloc hsection->sectchunk", -103);


        lpi    = 0;
        for (ichunk=0; ichunk<nchunks; ++ichunk) {

          ckc = 0;
          for (is=0; is<sectioninfo->point.nIDs; ++is) {

            ssi = sectioninfo->point.IDs[is];

            if ((ssi >= chunkinfo[ichunk].point.nl) && (ssi <= chunkinfo[ichunk].point.nh)) {

              ic = ssi - chunkinfo[ichunk].point.nl;

              llookup->secthyper[lpi] = is;
              llookup->sectchunk[lpi] = ic;
              ++lpi;
              ++ckc;
            }
          }

          hsection->chunklog[ichunk] = ckc;
        }

        dsize[0] = (hsize_t)sectioninfo->point.nIDs;
      }
      break;

    default:
      break;
    }

    llookup->datasize[0] = dsize[0];
    llookup->datasize[1] = dsize[1];
    llookup->datasize[2] = dsize[2];

    *hsectlookup = llookup;


    datasize[0] = dsize[0];
    datasize[1] = dsize[1];
    datasize[2] = dsize[2];
  }


  return (0);
}




/***************************************************************************************************
 *
 * xH5_ConvertMaterial
 *
 **************************************************************************************************/
int
xH5_ConvertMaterial( const int        ismat,
                     const hid_t      dtype_id,
                     const hsize_t    mixcount,    /* = mixcell for mat, mixcomp for ptr */
                     const TIO_Dims_t ndims,
                     const hsize_t    *offset,
                     const hsize_t    *msize,
                     const hsize_t    *mstride,
                     void             *matptr )
{
  const char *subname = "xH5_ConvertMaterial()";

  size_t  dsize;
  int16_t *i2ptr;
  int32_t *i4ptr;
  int64_t *i8ptr;

  hsize_t  ni, nj, nk, sj, sk, i, j, k, noff;


  switch (ndims) {
  case TIO_3D:
    nk   = msize[0];
    nj   = msize[1];
    ni   = msize[2];
    sk   = (mstride[1] - nj) * mstride[2];
    sj   = mstride[2] - ni;
    noff = (offset[0] * mstride[1] * mstride[2]) + (offset[1] * mstride[2]) + offset[2];
    break;
  case TIO_2D:
    nk   = 1;
    nj   = msize[0];
    ni   = msize[1];
    sk   = 0;
    sj   = mstride[1] - ni;
    noff = (offset[0] * mstride[1]) + offset[1];
    break;
  case TIO_1D:
    nk   = 1;
    nj   = 1;
    ni   = msize[0];
    sk   = 0;
    sj   = 0;
    noff = offset[0];
    break;
  default:
    break;
  }


  dsize = H5Tget_size(dtype_id);

  if (ismat) {

    /* material data array */

    switch (dsize) {
    case 2:
      i2ptr = (int16_t *)matptr + noff;
      for (k=0; k<nk; ++k) {
        for (j=0; j<nj; ++j) {
          for (i=0; i<ni; ++i) {
            if (*i2ptr < 0) *i2ptr = -( TIOabs(*i2ptr) + (int16_t)mixcount);
            ++i2ptr;
          }
          i2ptr += sj;
        }
        i2ptr += sk;
      }
      break;
    case 4:
      i4ptr = (int32_t *)matptr + noff;
      for (k=0; k<nk; ++k) {
        for (j=0; j<nj; ++j) {
          for (i=0; i<ni; ++i) {
            if (*i4ptr < 0) *i4ptr = -( TIOabs(*i4ptr) + (int32_t)mixcount);
            ++i4ptr;
          }
          i4ptr += sj;
        }
        i4ptr += sk;
      }
      break;
    case 8:
      i8ptr = (int64_t *)matptr + noff;
      for (k=0; k<nk; ++k) {
        for (j=0; j<nj; ++j) {
          for (i=0; i<ni; ++i) {
            if (*i8ptr < 0) *i8ptr = -( TIOabs(*i8ptr) + (int64_t)mixcount);
            ++i8ptr;
          }
          i8ptr += sj;
        }
        i8ptr += sk;
      }
      break;
    default:
      TIOreturn(ERR_INT, "Unsupported/failed bit-size for material datatype value", -1);
      break;
    }

  }
  else {

    /* material ptr array */

    switch (H5Tget_size(dtype_id)) {
    case 2:
      i2ptr = (int16_t *)matptr + noff;
      for (i=0; i<ni; ++i) {
        *i2ptr += (int16_t)mixcount;
        ++i2ptr;
      }
      break;
    case 4:
      i4ptr = (int32_t *)matptr + noff;
      for (i=0; i<ni; ++i) {
        *i4ptr += (int32_t)mixcount;
        ++i4ptr;
      }
      break;
    case 8:
      i8ptr = (int64_t *)matptr + noff;
      for (i=0; i<ni; ++i) {
        *i8ptr += (int64_t)mixcount;
        ++i8ptr;
      }
      break;
    default:
      TIOreturn(ERR_INT, "Unsupported/failed bit-size for material datatype value", -2);
      break;
    }

  }

  return (0);
}




/***************************************************************************************************
 *
 * xH5_ConvertVar
 *
 **************************************************************************************************/
static int
xH5_ConvertVar( const TIO_Data_t src_dtype, const TIO_Data_t dst_dtype,
                const void **src, void **dst )
{
  const char *subname = "xH5_ConvertVar()";

  /* Converts a single 0D variable from one datatype to another */

  /* Returns :   0  if conversion performed
                 1  if no conversion performed
               -ve  if error during conversion
  */

  int rc;


  if (src_dtype != dst_dtype) {

    size_t ts, src_size, dst_size;
    hid_t  src_dtype_id, dst_dtype_id;
    herr_t hrc;


    src_size = iTIO_SizeOf(src_dtype);
    dst_size = iTIO_SizeOf(dst_dtype);

    src_dtype_id = xH5_GetH5type(src_dtype);
    if (src_dtype_id < 0)
      TIOreturn(ERR_INT, "Invalid source datatype for conversion", ITIO_ERR_DATATYPE);

    dst_dtype_id = xH5_GetH5type(dst_dtype);
    if (dst_dtype_id < 0)
      TIOreturn(ERR_INT, "Invalid destination datatype for conversion", ITIO_ERR_DATATYPE);

    ts   = TIOmax(src_size, dst_size);
    *dst = TIOmalloc(ts);

    (void) memcpy(*dst, *src, src_size);

    hrc = H5Tconvert(src_dtype_id, dst_dtype_id, 1, *dst, NULL, H5P_DEFAULT);
    if (hrc < 0)
      TIOreturn (ERR_HDF5, "Failed to convert between datatypes", -1);

    rc = 0;

  }
  else {

    rc = 1;

  }

  return (rc);
}




/***************************************************************************************************
 *
 * xH5_SieveQuadData
 *
 **************************************************************************************************/
int
xH5_SieveQuadData( const iClass_t                  class,
                   const iDataPos_t                datapos,
                   const TIO_Size_t                nchunks,
                   const union iInfo_t             *chunkinfo,
                   const TIO_Size_t                ichunk,
                   const union iSection_t          *sectioninfo,
                   const TIO_Dims_t                ndims,
                   const TIO_Size_t                nghosts,
                   const struct xH5sectionlookup_t *sectlookup,
                   const size_t                    dtsize,
                   void                            *fulldata,
                   void                            *data,
                   TIO_Size_t                      *mixcellcount )
{
  const char *subname = "xH5_SieveQuadData()";

  TIO_Size_t soffset, sjstride, skstride;
  TIO_Size_t cioffset, cjoffset, ckoffset, cjstride, ckstride;


  TIO_Size_t cil, cih, cjl, cjh, ckl, ckh;
  TIO_Size_t sil, sih, sjl, sjh, skl, skh;

  TIO_Size_t ii, jj, kk, cni, cnj, cnk, ng2, lmc, ick;
  int dni, dnj, dnk;

  int irc;

  ick = ichunk * 6;

  cil = sectlookup->sectchunk[ick + 0];
  cih = sectlookup->sectchunk[ick + 1];
  cjl = sectlookup->sectchunk[ick + 2];
  cjh = sectlookup->sectchunk[ick + 3];
  ckl = sectlookup->sectchunk[ick + 4];
  ckh = sectlookup->sectchunk[ick + 5];
  sil = sectlookup->secthyper[ick + 0];
  sih = sectlookup->secthyper[ick + 1];
  sjl = sectlookup->secthyper[ick + 2];
  sjh = sectlookup->secthyper[ick + 3];
  skl = sectlookup->secthyper[ick + 4];
  skh = sectlookup->secthyper[ick + 5];

  irc = iTIO_DataposAdjust(datapos, &dni, &dnj, &dnk);

  ng2 = nghosts * 2;
  lmc = *mixcellcount;

  switch (ndims) {

  case TIO_3D:
    cni      = cih - cil + 1;
    cnj      = cjh - cjl + 1;
    cnk      = ckh - ckl + 1;
    jj       = chunkinfo[ichunk].quad.nj + ng2 - (TIO_Size_t)dnj;
    cjstride = chunkinfo[ichunk].quad.ni + ng2 - (TIO_Size_t)dni;
    ckstride = jj * cjstride;
    cioffset = cil + nghosts;
    cjoffset = cjstride * (cjl + nghosts);
    ckoffset = ckstride * (ckl + nghosts);
    sjstride = sectioninfo->quad.ih - sectioninfo->quad.il + 1 - (TIO_Size_t)dni;
    skstride = sectioninfo->quad.jh - sectioninfo->quad.jl + 1 - (TIO_Size_t)dnj;
    skstride = sjstride * skstride;
    break;

  case TIO_2D:
    cni      = cih - cil + 1;
    cnj      = cjh - cjl + 1;
    cnk      = 1;
    cjstride = chunkinfo[ichunk].quad.ni + ng2 - (TIO_Size_t)dni;
    ckstride = 0;
    cioffset = cil + nghosts;
    cjoffset = cjstride * (cjl + nghosts);
    ckoffset = 0;
    sjstride = sectioninfo->quad.ih - sectioninfo->quad.il + 1 - (TIO_Size_t)dni;
    skstride = 0;
    break;

  case TIO_1D:
    cni      = cih - cil + 1;
    cnj      = 1;
    cnk      = 1;
    cjstride = 0;
    ckstride = 0;
    cioffset = cil + nghosts;
    cjoffset = 0;
    ckoffset = 0;
    sjstride = 0;
    skstride = 0;
    break;

  default:
    break;
  }

  soffset  = (skstride * skl) + (sjstride * sjl) + sil;
  kk = ckoffset + cjoffset + cioffset;



  if (CLASS_MATERIAL == class) {

    switch (dtsize) {

    case 2:
      {
        int16_t *atptr, *adptr;
        int16_t *jtptr, *jdptr, *idptr;

        atptr = (int16_t *)fulldata + kk;
        adptr = (int16_t *)data + soffset;

        for (kk=0; kk<cnk; ++kk) {

          jtptr = atptr;
          jdptr = adptr;

          for (jj=0; jj<cnj; ++jj) {

            (void) memcpy(jdptr, jtptr, cni * dtsize);
            idptr = jdptr;

            for (ii=0; ii<cni; ++ii) {
              if (*idptr < 0) {
                *idptr = -( TIOabs(*idptr) + (int16_t)lmc );
              }
              ++idptr;
            }
            jtptr += cjstride;
            jdptr += sjstride;
          }
          atptr += ckstride;
          adptr += skstride;
        }

      }
      break;

    case 4:
      {
        int32_t *atptr, *adptr;
        int32_t *jtptr, *jdptr, *idptr;

        atptr = (int32_t *)fulldata + kk;
        adptr = (int32_t *)data + soffset;

        for (kk=0; kk<cnk; ++kk) {

          jtptr = atptr;
          jdptr = adptr;

          for (jj=0; jj<cnj; ++jj) {

            (void) memcpy(jdptr, jtptr, cni * dtsize);
            idptr = jdptr;

            for (ii=0; ii<cni; ++ii) {
              if (*idptr < 0) {
                *idptr = -( TIOabs(*idptr) + (int32_t)lmc );
              }
              ++idptr;
            }
            jtptr += cjstride;
            jdptr += sjstride;
          }
          atptr += ckstride;
          adptr += skstride;
        }
      }
      break;

    case 8:
      {
        int64_t *atptr, *adptr;
        int64_t *jtptr, *jdptr, *idptr;

        atptr = (int64_t *)fulldata + kk;
        adptr = (int64_t *)data + soffset;

        for (kk=0; kk<cnk; ++kk) {

          jtptr = atptr;
          jdptr = adptr;

          for (jj=0; jj<cnj; ++jj) {

            (void) memcpy(jdptr, jtptr, cni * dtsize);
            idptr = jdptr;

            for (ii=0; ii<cni; ++ii) {
              if (*idptr < 0) {
                *idptr = -( TIOabs(*idptr) + (int64_t)lmc );
              }
              ++idptr;
            }
            jtptr += cjstride;
            jdptr += sjstride;
          }
          atptr += ckstride;
          adptr += skstride;
        }
      }
      break;

    default:
      TIOreturn(ERR_INT, "Unsupported/failed bit-size for material datatype value", -1);
      break;
    }

    *mixcellcount += chunkinfo[ichunk].quad.nmixcell;

  }

  else {  /* class != CLASS_MATERIAL */

    switch (dtsize) {

    case 2:
      {
        int16_t *atptr, *adptr;
        int16_t *jtptr, *jdptr;

        atptr = (int16_t *)fulldata + kk;
        adptr = (int16_t *)data + soffset;

        for (kk=0; kk<cnk; ++kk) {

          jtptr = atptr;
          jdptr = adptr;

          for (jj=0; jj<cnj; ++jj) {

            (void) memcpy(jdptr, jtptr, cni * dtsize);

            jtptr += cjstride;
            jdptr += sjstride;
          }
          atptr += ckstride;
          adptr += skstride;
        }

      }
      break;

    case 4:
      {
        int32_t *atptr, *adptr;
        int32_t *jtptr, *jdptr;

        atptr = (int32_t *)fulldata + kk;
        adptr = (int32_t *)data + soffset;

        for (kk=0; kk<cnk; ++kk) {

          jtptr = atptr;
          jdptr = adptr;

          for (jj=0; jj<cnj; ++jj) {

            (void) memcpy(jdptr, jtptr, cni * dtsize);

            jtptr += cjstride;
            jdptr += sjstride;
          }
          atptr += ckstride;
          adptr += skstride;
        }

      }
      break;

    case 8:
      {
        int64_t *atptr, *adptr;
        int64_t *jtptr, *jdptr;

        atptr = (int64_t *)fulldata + kk;
        adptr = (int64_t *)data + soffset;

        for (kk=0; kk<cnk; ++kk) {

          jtptr = atptr;
          jdptr = adptr;

          for (jj=0; jj<cnj; ++jj) {

            (void) memcpy(jdptr, jtptr, cni * dtsize);

            jtptr += cjstride;
            jdptr += sjstride;
          }
          atptr += ckstride;
          adptr += skstride;
        }

      }
      break;

    default:
      TIOreturn(ERR_INT, "Unsupported/failed bit-size for material datatype value", -30);
      break;
    }


  }


  return (0);
}




/***************************************************************************************************
 *
 * xH5_SieveNonQuadData
 *
 **************************************************************************************************/
int
xH5_SieveNonQuadData(  const iClass_t                  class,
                       const TIO_Mesh_t                meshtype,
                       const union iInfo_t             *chunkinfo,
                       const TIO_Size_t                ichunk,
                       const TIO_Size_t                nchunklog,    /* no. elements for chunk */
                       TIO_Size_t                      *chunkcount,
                       const struct xH5sectionlookup_t *sectlookup,
                       const size_t                    dtsize,
                       void                            *fulldata,
                       void                            *data,
                       TIO_Size_t                      *mixcellcount )
{
  const char *subname = "xH5_SieveNonQuadData()";

  int16_t *i2ptr, *ti2ptr;
  int32_t *i4ptr, *ti4ptr;
  int64_t *i8ptr, *ti8ptr;

  TIO_Size_t lmc, lcc, ki;
  TIO_Size_t cidx;
  TIO_Size_t sidx;

  lcc = *chunkcount;


  if ((TIO_MESH_UNSTRUCT == meshtype) && (CLASS_MATERIAL == class)) {

    lmc = *mixcellcount;

    for (ki=0; ki<nchunklog; ++ki) {

      cidx = sectlookup->sectchunk[lcc];
      sidx = sectlookup->secthyper[lcc];

      switch (dtsize) {
      case 2:
        i2ptr  = (int16_t *)data + sidx;
        ti2ptr = (int16_t *)fulldata + cidx;
        (void) memcpy(i2ptr, ti2ptr, dtsize);
        if (*i2ptr < 0) {
        *i2ptr = -( TIOabs(*i2ptr) + (int16_t)lmc );
        }
        break;
      case 4:
        i4ptr  = (int32_t *)data + sidx;
        ti4ptr = (int32_t *)fulldata + cidx;
        (void) memcpy(i4ptr, ti4ptr, dtsize);
        if (*i4ptr < 0) {
          *i4ptr = -( TIOabs(*i4ptr) + (int32_t)lmc );
        }
        break;
      case 8:
        i8ptr  = (int64_t *)data + sidx;
        ti8ptr = (int64_t *)fulldata + cidx;
        (void) memcpy(i8ptr, ti8ptr, dtsize);
        if (*i8ptr < 0) {
          *i8ptr = -( TIOabs(*i8ptr) + (int64_t)lmc );
        }
        break;
      default:
        TIOreturn(ERR_INT, "Unsupported/failed bit-size for material datatype value", -30);
        break;
      }

      ++lcc;
    }

    *mixcellcount += chunkinfo[ichunk].unstr.nmixcell;
  }

  else {

    for (ki=0; ki<nchunklog; ++ki) {

      cidx = sectlookup->sectchunk[lcc];
      sidx = sectlookup->secthyper[lcc];

      switch (dtsize) {
      case 2:
        i2ptr  = (int16_t *)data + sidx;
        ti2ptr = (int16_t *)fulldata + cidx;
        (void) memcpy(i2ptr, ti2ptr, dtsize);
        break;
      case 4:
        i4ptr  = (int32_t *)data + sidx;
        ti4ptr = (int32_t *)fulldata + cidx;
        (void) memcpy(i4ptr, ti4ptr, dtsize);
        break;
      case 8:
        i8ptr  = (int64_t *)data + sidx;
        ti8ptr = (int64_t *)fulldata + cidx;
        (void) memcpy(i8ptr, ti8ptr, dtsize);
        break;
      default:
        TIOreturn(ERR_INT, "Unsupported/failed bit-size for material datatype value", -30);
        break;
      }
      ++lcc;
    }

  }

  *chunkcount = lcc;

  return (0);
}




/***************************************************************************************************
 *
 * xH5_GetChunkedSpace
 *
 ***************************************************************************************************
 *
 * Calculates the desired space
 *
 * Returns > 0 if space is zero-sized (with space_id -ve)
 *
 **************************************************************************************************/
int
xH5_GetChunkedSpace( struct xH5object_t    *hobject,
                     const xSpacetype_t    spacetype,
                     const iClass_t        class,
                     const TIO_Mesh_t      meshtype,
                     const iDataPos_t      datapos,
                     const int             ndset,
                     const TIO_Dims_t      ndims,
                     const TIO_Size_t      nghosts,
                     const TIO_Size_t      nchunks,
                     const union iInfo_t   *chunkinfo,
                     const int             nodata,   /* enables zero-sized dataspace if data null */
                     const TIO_Size_t      idx,
                     TIO_Size_t            *space_size,
                     hid_t                 *space_id )
{
  const char *subname = "xH5_GetChunkedSpace()";

  TIO_Size_t lsp_size;
  hid_t      lsp_id;

  hsize_t    sdims[1];

  hsize_t nl, nh, ml;
  hsize_t bksize[1];
  hsize_t offset[1];
  hsize_t nblock[1];
  herr_t  hrc;

  TIO_Size_t ng, ng2;

  int fullspace, iquadmesh, imixed;
  int irc, zerosized;


  /* still return an empty dataspace and do read for independent instead of returning */

  /* if idx == nchunks return space for whole dataset for creation purposes */


  fullspace = (idx == nchunks);
  iquadmesh = ((TIO_MESH_QUAD_COLINEAR == meshtype) && (CLASS_MESH == class));


  /* Handle special case of CLASS_MESH and COLINEAR mesh here */

  if (iquadmesh) {

    ng2 = nghosts * 2;

    switch (ndset) {
    case 0:
      sdims[0] = (hsize_t)hobject->ni;
      break;
    case 1:
      sdims[0] = (hsize_t)hobject->nj;
      break;
    case 2:
      sdims[0] = (hsize_t)hobject->nk;
      break;
    }

    if (!fullspace) {

      switch (ndset) {
      case 0:
        nl       = TIObottom( nghosts, chunkinfo[idx].quad.il, 0                );
        nh       = TIOtop(    nghosts, chunkinfo[idx].quad.ih, (hobject->ni - 1));
        lsp_size = chunkinfo[idx].quad.ni + ng2;
        ml       = chunkinfo[idx].quad.il;
        break;
      case 1:
        nl       = TIObottom( nghosts, chunkinfo[idx].quad.jl, 0                );
        nh       = TIOtop(    nghosts, chunkinfo[idx].quad.jh, (hobject->nj - 1));
        lsp_size = chunkinfo[idx].quad.nj + ng2;
        ml       = chunkinfo[idx].quad.jl;
        break;
      case 2:
        nl       = TIObottom( nghosts, chunkinfo[idx].quad.kl, 0                );
        nh       = TIOtop(    nghosts, chunkinfo[idx].quad.kh, (hobject->nk - 1));
        lsp_size = chunkinfo[idx].quad.nk + ng2;
        ml       = chunkinfo[idx].quad.kl;
        break;
      }

      switch (spacetype) {
      case SPACE_FILE:
        offset[0] = nl;
        break;
      case SPACE_MEMORY:
        sdims[0] += ng2;

        if (nghosts > ml) {
          offset[0] = nghosts;
        }
        else {
          offset[0] = 0;
        }
        break;
      }

      bksize[0] = nh - nl + 1;
      nblock[0] = 1;

    }
    else {

      lsp_size  = (TIO_Size_t)sdims[0];

    }

    zerosized = (0 == lsp_size);

    TIOset(space_size, lsp_size);

  }
  else {

    /* Carry on for all other meshes/objects */

    hid_t lfsp_id, lmsp_id;

    /* For mixed data, always return full amount stored in file, regardless of nghosts */

    switch (datapos) {
    case DATAPOS_MIXED:
    case DATAPOS_MIXCELL:
    case DATAPOS_MIXCOMP:

      ng     = hobject->nghosts;
      imixed = TRUE;
      break;
    default:
      ng     = nghosts;
      imixed = FALSE;
      break;
    }






    /* Space needs to be calculated  */

    irc = xH5_CalcChunkedSpace(hobject, class, meshtype, datapos, ndset, ndims,
                               ng, nchunks, chunkinfo,  idx,
                               &lsp_size, &lmsp_id, &lfsp_id);
    zerosized = (irc > 0);
    TIOassert(irc < 0, ERR_INT, "Failed to calc space", -1);

    /* lsp_id could still be -ve here if it is of zero size */

    switch (spacetype) {
    case SPACE_FILE:
      lsp_id = lfsp_id;
      break;
    case SPACE_MEMORY:
      lsp_id = lmsp_id;
      break;
    }


    if (space_size != NULL) {

      /* If non-mixed quad object, fill size as an array per dimension, otherwise it's a scalar */

      if ( !imixed && (   (TIO_MESH_QUAD_COLINEAR == meshtype)
                       || (TIO_MESH_QUAD_NONCOLINEAR == meshtype) ) ) {

        int dni, dnj, dnk;

        ng2 = nghosts * 2;

        irc = iTIO_DataposAdjust(datapos, &dni, &dnj, &dnk);

        switch (ndims) {
        case TIO_3D:
          space_size[2] = chunkinfo[idx].quad.nk + ng2 - (TIO_Size_t)dnk;
        case TIO_2D:
          space_size[1] = chunkinfo[idx].quad.nj + ng2 - (TIO_Size_t)dnj;
        case TIO_1D:
          space_size[0] = chunkinfo[idx].quad.ni + ng2 - (TIO_Size_t)dni;
        default:
          break;
        }

      }
      else {

        *space_size = lsp_size;

      }
    }

  }


  if (space_id != NULL) {

    if (zerosized || nodata) {

      /* Construct a dummy zero-sized dataspace */

      lsp_id = xH5_H5Screate_simple_none();
      TIOassert(lsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -4);

      *space_id = lsp_id;

    }
    else if (iquadmesh) {

      /* Create a simple dataspace for colinear quadmesh chunk */

      lsp_id = H5Screate_simple(1, sdims, NULL);
      TIOassert(lsp_id < 0, ERR_HDF5, "Failed to create dataspace", -6);

      if (!fullspace) {
        hrc = H5Sselect_hyperslab(lsp_id, H5S_SELECT_SET, offset, NULL, nblock, bksize);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to select zero-sized memory hyperslab", -7);
      }

      *space_id = lsp_id;

    }
    else {

      /* Create a copy of the stored dataspace */

      *space_id = H5Scopy(lsp_id);
      TIOassert(*space_id < 0, ERR_HDF5, "Failed to copy lsp_id to space_id", -7);

    }
  }


  if (zerosized) {
    return (1);
  }
  else {
    return (0);
  }
}





static int
xH5_CalcChunkedSpace( struct xH5object_t  *hobject,
                      const iClass_t      class,
                      const TIO_Mesh_t    meshtype,
                      const iDataPos_t    datapos,
                      const int           ndset,
                      const TIO_Dims_t    ndims,
                      const TIO_Size_t    nghosts,
                      const TIO_Size_t    nchunks,
                      const union iInfo_t *chunkinfo,
                      const TIO_Size_t    idx,
                      TIO_Size_t          *space_size,
                      hid_t               *memsp_id,
                      hid_t               *filsp_id )
{
  const char *subname = "xH5_CalcChunkedSpace()";

  TIO_Size_t fnghosts;
  hsize_t    sdims[1], soffset[1], nblock[1], stride[1];
  hid_t      lfsp_id, lmsp_id;

  herr_t  hrc;
  int     irc;



  /* Get dataspace count */

  irc = xH5_GetCount( meshtype, datapos, ndims, nghosts, nchunks, chunkinfo,  idx, &sdims[0] );
  TIOassert(irc < 0, ERR_INT, "Failed to get count", -1);

  /* All chunked datasets, whether Mesh, Material or Quant are 1D and the
     size of chunked dataset has been found above wrt class, meshtype & data position */

  /* Possible that the size of the dataset is zero (eg - a material created as mixed, but just
     happens to be all pure, say, at t=0
     If this is the case then return accordingly */


  if (0 == sdims[0]) {

    /* Dataspace has zero size - this can happen, say, when a material is defined as mixed, but
       there is no mixed at a particular time. */

    TIOset(space_size, 0);
    TIOset(filsp_id,  -1);
    TIOset(memsp_id,  -1);

    return (1);
  }


  if (idx == nchunks) {   /* This is for the fullspace - ie. the full on-disk dataset */

    lfsp_id = H5Screate_simple(1, sdims, NULL);
    TIOassert(lfsp_id < 0, ERR_HDF5, "Failed to create dataspace", -2);

    lmsp_id = -1;

  }
  else {                  /* This is for the individual chunk within the datset */

    hid_t fullspace_id;


    /* Get the fullspace and select the chunk within it */

    fnghosts = hobject->nghosts;

    irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims, fnghosts,
                              nchunks, chunkinfo, FALSE, nchunks, NULL, &fullspace_id);
    TIOassert(irc < 0, ERR_INT, "Failed to get chunked fullspace_id", -3);

    lfsp_id = H5Scopy(fullspace_id);
    TIOassert(lfsp_id < 0, ERR_HDF5, "Failed to create dataspace", -4);

    hrc = H5Sclose(fullspace_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close fullspace_id", -5);


    irc = xH5_CalcOffsetSpace( meshtype, datapos, ndims, nghosts, fnghosts,
                              nchunks, chunkinfo, 
			      idx, lfsp_id, sdims);

    /* Create a memory dataspace for the chunk, as continuous in memory */

    lmsp_id = H5Screate_simple(1, sdims, NULL);
    TIOassert(lmsp_id < 0, ERR_HDF5, "Failed to create dataspace", -6);

  }

  *space_size = (TIO_Size_t)sdims[0];
  *filsp_id   = lfsp_id;
  *memsp_id   = lmsp_id;


  return (0);
}




static int
xH5_CalcOffsetSpace(  const TIO_Mesh_t    meshtype,
                      const iDataPos_t    datapos,
                      const TIO_Dims_t    ndims,
                      const TIO_Size_t    nghosts,
                      const TIO_Size_t    fnghosts,
                      const TIO_Size_t    nchunks,
                      const union iInfo_t *chunkinfo,
                      const TIO_Size_t    idx,
                      const hid_t         lfsp_id,
                      const hsize_t       sdims[1]   )
{
  const char *subname = "xH5_CalcOffsetSpace()";

  hsize_t    soffset[1], nblock[1], stride[1];

  herr_t  hrc;
  int     irc;


    /* Get dataspace offset - must use nghosts of the hobject, ie. those in the file */

    /* nchunks for idx arg of this routine makes it return the full size required */

    switch (meshtype) {

    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR:
      irc = xH5_GetOffset( meshtype, datapos, ndims, fnghosts,
                           nchunks, chunkinfo, idx, &soffset[0] );
      TIOassert(irc < 0, ERR_INT, "Failed to get quad offset", -10);

      /* We've got where it starts within the file           */
      /* Now fiddle with striding and stuff to handle ghosts */

      if (nghosts != fnghosts) {

        const union iInfo_t *ckinfo;

        TIO_Size_t dng, fng2, ng2;
        hsize_t    ni;
        hsize_t    nfi, nfj, nfij;  /* Size of dimensions in file   */
        hsize_t    ngi, ngj, ngk;   /* Size of dimensions to select */
        int        dni, dnj, dnk;   /* Centring adjusters           */
        hsize_t    foffset[1], block[1];
        hsize_t    k;


        TIOassert((DATAPOS_MIXED   == datapos), ERR_INT, "Can't calc space for mixed", -100);
        TIOassert((DATAPOS_MIXCELL == datapos), ERR_INT, "Can't calc space for mixed", -101);
        TIOassert((DATAPOS_MIXCOMP == datapos), ERR_INT, "Can't calc space for mixed", -102);


        ckinfo = &chunkinfo[idx];
        dng    = fnghosts - nghosts;
        fng2   = fnghosts * 2;
        ng2    = nghosts  * 2;

        irc = iTIO_DataposAdjust(datapos, &dni, &dnj, &dnk);

        switch (ndims) {
        case TIO_3D:
          nfi   = (hsize_t)(ckinfo->quad.ni + fng2) - (hsize_t)dni;
          nfj   = (hsize_t)(ckinfo->quad.nj + fng2) - (hsize_t)dnj;
          ngj   = (hsize_t)(ckinfo->quad.nj + ng2)  - (hsize_t)dnj;
          ngk   = (hsize_t)(ckinfo->quad.nk + ng2)  - (hsize_t)dnk;
          break;
        case TIO_2D:
          nfi   = (hsize_t)(ckinfo->quad.ni + fng2) - (hsize_t)dni;
          nfj   = (hsize_t)1;
          ngj   = (hsize_t)(ckinfo->quad.nj + ng2)  - (hsize_t)dnj;
          ngk   = (hsize_t)1;
          break;
        case TIO_1D:
          nfi   = (hsize_t)1;
          nfj   = (hsize_t)1;
          ngj   = (hsize_t)1;
          ngk   = (hsize_t)1;
          break;
        default:
          break;
        }

        ni         = (hsize_t)ckinfo->quad.ni - (hsize_t)dni;
        ngi        = ni + (hsize_t)(ng2);
        nfij       = nfi * nfj;
        foffset[0] = 0;

        switch (ndims) {
        case TIO_3D:
          foffset[0] += (dng * nfij);
          /* Drop through */
        case TIO_2D:
          foffset[0] += (dng * nfi);
          /* Drop through */
        case TIO_1D:
          foffset[0] += dng;
        default:
          break;
        }


        foffset[0] += soffset[0];
        stride[0]   = ni + (hsize_t)fng2;
        nblock[0]   = ngj;
        block[0]    = ngi;

        hrc = H5Sselect_none(lfsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to select none", -11);

        /* repeatedly select 1D/2D hyperslabs to fill dimension space */
        for (k=0; k<ngk; ++k) {

          /* This select a whole i-j plane per k */

          hrc = H5Sselect_hyperslab(lfsp_id, H5S_SELECT_OR, foffset, stride, nblock, block);
          TIOassert(hrc < 0, ERR_HDF5, "Failed to select memory hyperslab", -12);

          foffset[0] += nfij;
        }

        /* sdims[0] = block[0] * nblock[0] * ngk;  == value already obtained from xH5_GetCount */

      }
      else {

        nblock[0] = 1;

        hrc = H5Sselect_hyperslab(lfsp_id, H5S_SELECT_SET, soffset, NULL, nblock, sdims);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to select memory hyperslab", -15);
      }

      break;


    case TIO_MESH_UNSTRUCT:
    case TIO_MESH_POINT:
      irc = xH5_GetOffset( meshtype, datapos, TIO_1D, fnghosts,
                           nchunks, chunkinfo, idx, &soffset[0] );
      TIOassert(irc < 0, ERR_INT, "Failed to get unstr/point offset", -20);

      nblock[0] = 1;

      hrc = H5Sselect_hyperslab(lfsp_id, H5S_SELECT_SET, soffset, NULL, nblock, sdims);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to select memory hyperslab", -21);

      break;

    default:
      TIOreturn(ERR_INT, "Invalid meshtype", ITIO_ERR_MESHTYPE);
      break;
    }

  return (0);
}











int
xH5_GetCount( const TIO_Mesh_t    meshtype,
              const iDataPos_t    datapos,
              const TIO_Dims_t    ndims,
              const TIO_Size_t    nghosts,
              const TIO_Size_t    nchunks,
              const union iInfo_t *chunkinfo,
              const TIO_Size_t    chunkidx,
              hsize_t             *count )
{
  const char *subname="xH5_GetCount()";

  const union iInfo_t *chunk;

  int dni, dnj, dnk, ngx2;
  int irc;


#ifndef NDEBUG
  if (NULL == count)
    TIOreturn(ERR_INT, "count must != NULL", -1);
#endif


  /* Want Count for full space if chunkidx == nchunks */

  if (chunkidx == nchunks) {

    TIO_Size_t idx;
    hsize_t    ccount, lcount;

    ccount = 0;
    for (idx=0; idx<nchunks; ++idx) {
      irc = xH5_GetCount(meshtype, datapos, ndims, nghosts, nchunks, chunkinfo, idx, &lcount);
      ccount += lcount;
    }

    /* Handle case of mixed being appended to cell-centred */
    if (DATAPOS_MIXED == datapos) {
      irc = xH5_GetCount(meshtype, DATAPOS_CELL, ndims, nghosts, nchunks, chunkinfo, idx, &lcount);
      ccount += lcount;
    }

    *count = ccount;

    return (0);
  }


  /* Counts for chunks */

  chunk = &chunkinfo[chunkidx];

  switch (meshtype) {


  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    switch (datapos) {

    case DATAPOS_CELL:
    case DATAPOS_NODE:
    case DATAPOS_EDGE_I:
    case DATAPOS_EDGE_J:
    case DATAPOS_EDGE_K:
    case DATAPOS_FACE_I:
    case DATAPOS_FACE_J:
    case DATAPOS_FACE_K:
      ngx2 = nghosts * 2;
      irc = iTIO_DataposAdjust(datapos, &dni, &dnj, &dnk);
      *count = (hsize_t)chunk->quad.ni + (hsize_t)(ngx2 - dni);

      switch (ndims) {
      case TIO_3D:
        *count *= (hsize_t)chunk->quad.nk + (hsize_t)(ngx2 - dnk);
      case TIO_2D:
        *count *= (hsize_t)chunk->quad.nj + (hsize_t)(ngx2 - dnj);
      default:
        break;
      }
      break;

    case DATAPOS_MIXED:
    case DATAPOS_MIXCOMP:
      *count = (hsize_t)chunk->quad.nmixcomp;
      break;

    case DATAPOS_MIXCELL:
      *count = (hsize_t)chunk->quad.nmixcell;
      break;

    default:
      break;
    }

    break;


  case TIO_MESH_UNSTRUCT:

    switch (datapos) {

    case DATAPOS_CELL:
      if (TIO_GHOSTS_ALL == nghosts) {
        *count = chunk->unstr.ncells + chunk->unstr.nghost_cells;
      }
      else {
        *count = (hsize_t)chunk->unstr.ncells;
      }
      break;

    case DATAPOS_NODE:
      if (TIO_GHOSTS_ALL == nghosts) {
        *count = (hsize_t)(chunk->unstr.nnodes + chunk->unstr.nghost_nodes);
      }
      else {
        *count = (hsize_t)chunk->unstr.nnodes;
      }
      break;

    case DATAPOS_SHAPE:
      if (TIO_GHOSTS_ALL == nghosts) {
        *count = (hsize_t)(chunk->unstr.nshapes + chunk->unstr.nghost_shapes);
      }
      else {
        *count = (hsize_t)chunk->unstr.nshapes;
      }
      break;

    case DATAPOS_CONNECT:
      if (TIO_GHOSTS_ALL == nghosts) {
        *count = (hsize_t)(chunk->unstr.nconnectivity + chunk->unstr.nghost_connectivity);
      }
      else {
        *count = (hsize_t)chunk->unstr.nconnectivity;
      }
      break;

    case DATAPOS_MIXED:
    case DATAPOS_MIXCOMP:
      *count = (hsize_t)chunk->unstr.nmixcomp;
      break;

    case DATAPOS_MIXCELL:
      *count = (hsize_t)chunk->unstr.nmixcell;
      break;

    default:
      break;
    }

    break;


  case TIO_MESH_POINT:

    if (TIO_GHOSTS_ALL == nghosts) {
      *count = (hsize_t)(chunk->point.nnodes + chunk->point.nghosts);   /* Only can ever be nodal */
    }
    else {
      *count = (hsize_t)chunk->point.nnodes;
    }
    break;

  default:
    TIOreturn(ERR_INT, "Invalid meshtype", ITIO_ERR_MESHTYPE);
    break;
  }

  return (0);
}




static int
xH5_GetOffset( const TIO_Mesh_t    meshtype,
               const iDataPos_t    datapos,
               const TIO_Dims_t    ndims,
               const TIO_Size_t    nghosts,
               const TIO_Size_t    nchunks,
               const union iInfo_t *chunkinfo,
               const TIO_Size_t    idx,
               hsize_t             *offset )
{
  const char *subname = "xH5_GetOffset()";

  hsize_t    noff, cksize;
  TIO_Size_t i;

  int irc;


  switch (meshtype) {

  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    if (idx == nchunks) {    /* full space case */
      *offset = 0;
      return (0);
    }


    noff = 0;

    switch (datapos) {

    case DATAPOS_MIXED:
      irc = xH5_GetCount(meshtype, DATAPOS_CELL, ndims, nghosts,
                         nchunks, chunkinfo, nchunks, &noff);
      for (i=0; i<idx; ++i) {
        noff += chunkinfo[i].quad.nmixcomp;
      }
      break;

    case DATAPOS_MIXCELL:
      for (i=0; i<idx; ++i) {
        noff += chunkinfo[i].quad.nmixcell;
      }
      break;

    case DATAPOS_MIXCOMP:
      for (i=0; i<idx; ++i) {
        noff += chunkinfo[i].quad.nmixcomp;
      }
      break;

    default:

      /* All normal mesh-based objects */

      for (i=0; i<idx; ++i) {
        irc = xH5_GetCount(meshtype, datapos, ndims, nghosts, nchunks, chunkinfo,
                i, &cksize);
        noff += cksize;
      }

      break;

    }
    break;

  case TIO_MESH_UNSTRUCT:

    if (idx == nchunks) {    /* full space case */
      *offset = 0;
      return (0);
    }


    noff = 0;

    switch (datapos) {

    case DATAPOS_CELL:
      if (TIO_GHOSTS_ALL == nghosts) {
        for (i=0; i<idx; ++i) {
          noff += chunkinfo[i].unstr.ncells + chunkinfo[i].unstr.nghost_cells;
        }
      }
      else {
        for (i=0; i<idx; ++i) {
          noff += chunkinfo[i].unstr.ncells;
        }
      }
      break;

    case DATAPOS_NODE:
      if (TIO_GHOSTS_ALL == nghosts) {
        for (i=0; i<idx; ++i) {
          noff += chunkinfo[i].unstr.nnodes + chunkinfo[i].unstr.nghost_nodes;
        }
      }
      else {
        for (i=0; i<idx; ++i) {
          noff += chunkinfo[i].unstr.nnodes;
        }
      }
      break;

    case DATAPOS_SHAPE:
      if (TIO_GHOSTS_ALL == nghosts) {
        for (i=0; i<idx; ++i) {
          noff += chunkinfo[i].unstr.nshapes + chunkinfo[i].unstr.nghost_shapes;
        }
      }
      else {
        for (i=0; i<idx; ++i) {
          noff += chunkinfo[i].unstr.nshapes;
        }
      }
      break;

    case DATAPOS_CONNECT:
      if (TIO_GHOSTS_ALL == nghosts) {
        for (i=0; i<idx; ++i) {
          noff += chunkinfo[i].unstr.nconnectivity + chunkinfo[i].unstr.nghost_connectivity;
        }
      }
      else {
        for (i=0; i<idx; ++i) {
          noff += chunkinfo[i].unstr.nconnectivity;
        }
      }
      break;

    case DATAPOS_MIXED:
      irc = xH5_GetCount(TIO_MESH_UNSTRUCT, DATAPOS_CELL, TIO_1D, nghosts,
                         nchunks, chunkinfo,  nchunks, &noff);
      for (i=0; i<idx; ++i) {
        noff += chunkinfo[i].unstr.nmixcomp;
      }
      break;

    case DATAPOS_MIXCELL:
      for (i=0; i<idx; ++i) {
        noff += chunkinfo[i].unstr.nmixcell;
      }
      break;

    case DATAPOS_MIXCOMP:
      for (i=0; i<idx; ++i) {
        noff += chunkinfo[i].unstr.nmixcomp;
      }
      break;


    default:
      TIOassert(TRUE, ERR_INT, "Invalid datapos", -1);
      break;
    }

    break;

  case TIO_MESH_POINT:

    TIOassert(datapos != DATAPOS_NODE, ERR_INT, "Invalid datapos", -2);

    noff = 0;

    if (TIO_GHOSTS_ALL == nghosts) {
      for (i=0; i<idx; ++i) {
        noff += chunkinfo[i].point.nnodes + chunkinfo[i].point.nghosts;
      }
    }
    else {
      for (i=0; i<idx; ++i) {
        noff += chunkinfo[i].point.nnodes;
      }
    }

    break;


  default:
    break;
  }

  *offset = noff;

  return (0);
}




/***************************************************************************************************
 *
 * xH5ObjectExists()
 *
 **************************************************************************************************/
int
xH5_ObjectExists( const hid_t group_id, const char name[] )
{
  herr_t  hrc;
  hbool_t follow_link = 1;

/*#ifdef _H5_USE_16_API*/
  hrc = H5Gget_objinfo(group_id, name, follow_link, NULL);
/*
#else
  H5L_info_t lnk;
  hrc = H5Lget_info(group_id, name, &lnk, H5P_DEFAULT);
#endif
*/
  return (hrc >= 0);
}




/***************************************************************************************************
 *
 * xH5_H5Screate_simple_none()
 *
 **************************************************************************************************/
hid_t
xH5_H5Screate_simple_none( void )
{
  const char *subname = "xH5_H5Screate_simple_none()";

  const hsize_t offset[1] = {0};
  const hsize_t nblock[1] = {1};
  const hsize_t b1size[1] = {1};
  const hsize_t b0size[1] = {0};

  hid_t  sp_id;
  herr_t hrc;

  sp_id = H5Screate_simple(1, b1size, b1size);
  TIOassert(sp_id < 0, ERR_HDF5, "Failed to create dataspace", -1);

  hrc = H5Sselect_hyperslab(sp_id, H5S_SELECT_SET, offset, NULL, nblock, b0size);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to select zero-sized hyperslab", -2);

  return (sp_id);
}




/* ----------------------

     Debugging routines

   ---------------------- */

#ifndef NDEBUG

/***************************************************************************************************
 *
 * dNullFile()
 *
 **************************************************************************************************/
void
dNullFile( struct xH5file_t   *hfile,
           const char         name[] )
{
  (void) strcpy(hfile->name, name);

  hfile->file_id       = 888888;
  hfile->root_id       = 888888;
  hfile->indxfer_plist = 888888;
  hfile->colxfer_plist = 888888;

  hfile->rank          = -1;

  return;
}



/***************************************************************************************************
 *
 * dNullObject()
 *
 **************************************************************************************************/
void
dNullObject( struct xH5object_t *hobject )
{
  hobject->nghosts      = 888888;
  hobject->group_id     = 888888;

  {
    int i;
    for (i=0; i<NUM_DATASETS; ++i) {
      hobject->dset_id[i]  = 888888;
    }
  }

  hobject->varrefindex  =  666;
  hobject->vardataindex = -666;

  hobject->ref          = NULL;
  hobject->varcache     = NULL;

  return;
}



/***************************************************************************************************
 *
 * dNullRef()
 *
 **************************************************************************************************/
void
dNullRef( struct xH5file_t   *hfile,
          struct xH5object_t *hobject,
          const iClass_t     class,
          const char         name[] )
{
  int nrefs;
  int i;
  int irc;


  struct xH5ref_t *ref;

  if (hfile != NULL) {
    ref = hfile->ref;
  }
  else {
    ref = hobject->ref;
  }

  irc = iTIO_HierarchyGetNumChildren(class, &nrefs);


  for (i=0; i<nrefs; ++i) {
    ref[i].n    = 888888;
    ref[i].size = 888888;
    ref[i].id   = 999;
    ref[i].mod  = 666;
    ref[i].buf  = NULL;
  }


  if (nrefs < 1) return;

  (void) strcpy(ref[0].name, name);
  (void) strcat(ref[0].name, " : ");
  (void) strcat(ref[0].name, xRefNames[CLASS_VAR]);

  (void) strcpy(ref[1].name, name);
  (void) strcat(ref[1].name, " : ");
  (void) strcat(ref[1].name, xRefNames[CLASS_VGROUP]);

  if (nrefs < 3) return;

  (void) strcpy(ref[2].name, name);
  (void) strcat(ref[2].name, " : ");

  switch (class) {

  case CLASS_FILE:
    (void) strcat(ref[2].name, xRefNames[CLASS_STATE]);
    break;

  case CLASS_STATE:
    (void) strcat(ref[2].name, xRefNames[CLASS_MESH]);
    break;

  case CLASS_MESH:
    (void) strcat(ref[2].name, xRefNames[CLASS_QUANT]);

    (void) strcpy(ref[3].name, name);
    (void) strcat(ref[3].name, " : ");
    (void) strcat(ref[3].name, xRefNames[CLASS_MATERIAL]);


    break;

  default:
    break;
  }

  return;
}



/***************************************************************************************************
 *
 * dPrintSpace()
 *
 * Debugging routine to print out properties of an HDF5 dataspace to stderr
 *
 **************************************************************************************************/
void
dPrintSpace( const hid_t space_id )
{
  const char *subname = "dPrintSpace()";

  int          sprank, b, i, h;
  hsize_t      *dims;
  H5S_sel_type sel;
  herr_t       hrc;


  sprank  = H5Sget_simple_extent_ndims(space_id);
  if (sprank < 0) {
    fprintf(stderr, "   Dataspace ID invalid\n");
    return;
  }

  dims    = TIOmalloc(sizeof(hsize_t) * sprank);

  sprank  = H5Sget_simple_extent_dims(space_id, dims, NULL);

  fprintf(stderr, "space id=%d  rank=%d dims=", (int)space_id, sprank);

  for (i=0; i<sprank; i++) {
    fprintf(stderr, "%d ", (int)dims[i]);
  }
  fprintf(stderr, "\n");


  sel = H5Sget_select_type(space_id);

  switch (sel) {

  case H5S_SEL_NONE:
    fprintf(stderr, "   - Nothing selected\n");
    break;

  case H5S_SEL_POINTS:
    fprintf(stderr, "   - Points selected\n");
    {
      hssize_t npoints;
      hsize_t  *points;


      npoints = H5Sget_select_npoints(space_id);
      npoints = H5Sget_select_elem_npoints(space_id);

      points = (hsize_t *)TIOmalloc(sizeof(hsize_t) * npoints);

      hrc    = H5Sget_select_elem_pointlist(space_id, 0, npoints, points );

      fprintf(stderr, "        npoints = %d\n", (int)npoints);
      fprintf(stderr, "        points  = ");
      for (i=0; i<npoints; ++i) {
        fprintf(stderr,"%d ", (int)points[i]);
      }
      fprintf(stderr, "\n");

      TIOfree (points); points = NULL;

    }
    break;

  case H5S_SEL_HYPERSLABS:
    fprintf(stderr, "   - Hyperslab or compound hyperslab selected\n");
    {
      hssize_t nblocks;
      hsize_t  *blockbuf;

      nblocks  = H5Sget_select_hyper_nblocks(space_id);

      blockbuf = TIOmalloc(sizeof(hsize_t) * sprank * 2);

      fprintf(stderr, "        nblocks = %d\n", (int)nblocks);
      fprintf(stderr, "        blocks  = \n");

      for (b=0; b<nblocks; ++b) {

        hrc = H5Sget_select_hyper_blocklist(space_id, b, 1, blockbuf);

        h = sprank;

        fprintf(stderr, "                  ");
        for (i=0, h=sprank; i<sprank; ++i, ++h) {
          fprintf(stderr,"[ %d : %d ] ", (int)blockbuf[i], (int)blockbuf[h]);
        }
        fprintf(stderr, "\n");
      }

      TIOfree (blockbuf); blockbuf = NULL;

    }
    break;

  case H5S_SEL_ALL:
    fprintf(stderr, "   - Entire dataset selected\n");
    break;

  default:
    break;
  }


  if (H5Sselect_valid(space_id)) {
    fprintf(stderr, "   - Dataspace is VALID\n");
  }
  else {
    fprintf(stderr, "   - Dataspace is INVALID\n");
  }

  TIOfree (dims); dims = NULL;


  return;
}

#endif



#if ( !((H5_VERS_MAJOR == 1) && (H5_VERS_MINOR >= 8)) )


/*
 *   Routine to allow HDF5 to convert integer to floating-point.
 *   HDF5 v 1.6.x cannot do this natively.
 *   Should be able to get rid of this when HDF5 v1.8 comes along.
 */
herr_t xH5_Conv_i2f( hid_t       src_id,
                     hid_t       dst_id,
                     H5T_cdata_t *cdata,
                     size_t      nelmts,
                     size_t      buf_stride,       /* ignorable */
                     size_t      bkg_stride,       /* ignorable */
                     void        *buf,
                     void        *bkg,             /* ignorable */
                     hid_t       dset_xfer_plist   /* ignorable */
                     )
{
  const char *subname = "xH5_Conv_i2f()";

  unsigned char *isrc_buf = (unsigned char *)buf;
  unsigned char *fdst_buf = isrc_buf;

  long double temp;

  typedef struct {
    int     direction;
    int     issigned;
    size_t  isrc_size, fdst_size;
    ssize_t isrc_incr, fdst_incr;
  } priv_t;

  priv_t *priv = NULL;

  size_t  ii;


  switch (cdata->command) {

  case H5T_CONV_INIT:
    /*
     * We are being queried to see if we handle this conversion.
     * We can handle conversion from any integer to any float.
     */

    if ( (H5Tget_class(src_id) != H5T_INTEGER) ||
         (H5Tget_class(dst_id) != H5T_FLOAT) ) {
      return (-1);
    }

    /*
     * Initialize private data.
     * If the destination size is larger than the source size,
     * then we must process the elements from right to left.
     * We assume the buffer is big enough.
     */

    cdata->need_bkg = H5T_BKG_NO;

    cdata->priv = priv = (priv_t *)TIOmalloc(sizeof(priv_t));

    if (priv->fdst_size > priv->isrc_size) {
      priv->direction = -1;
    }
    else {
      priv->direction = 1;
    }

    priv->issigned  = (H5Tget_sign(src_id) != H5T_SGN_NONE);
    priv->isrc_size = H5Tget_size(src_id);
    priv->fdst_size = H5Tget_size(dst_id);
    priv->isrc_incr = (ssize_t)priv->isrc_size * priv->direction;
    priv->fdst_incr = (ssize_t)priv->fdst_size * priv->direction;

    break;


  case H5T_CONV_FREE:
    /*
     * Free private data.
     */
    TIOfree (cdata->priv); cdata->priv = NULL;

    break;


  case H5T_CONV_CONV:
    /*
     * Convert each element, watch out for overlap src_buf
     * with dst_buf on the left-most element of the buffer.
     */

    priv = (priv_t *)(cdata->priv);

    if (priv->direction < 0) {   /* r to l  - advance to end of buffer */
      isrc_buf += (nelmts - 1) * priv->isrc_size;
      fdst_buf += (nelmts - 1) * priv->fdst_size;
    }


    for (ii=0; ii<nelmts; ii++) {

      switch (priv->isrc_size) {

      case 1:
        if (priv->issigned) {
          int8_t *ptr = (int8_t *)isrc_buf;
          temp = (long double)*ptr;
        }
        else {
          uint8_t *ptr = (uint8_t *)isrc_buf;
          temp = (long double)*ptr;
        }
        break;

      case 2:
        if (priv->issigned) {
          int16_t *ptr = (int16_t *)isrc_buf;
          temp = (long double)*ptr;
        }
        else {
          uint16_t *ptr = (uint16_t *)isrc_buf;
          temp = (long double)*ptr;
        }
        break;

      case 4:
        if (priv->issigned) {
          int32_t *ptr = (int32_t *)isrc_buf;
          temp = (long double)*ptr;
        }
        else {
          uint32_t *ptr = (uint32_t *)isrc_buf;
          temp = (long double)*ptr;
        }
        break;

      case 8:
        if (priv->issigned) {
          int64_t *ptr = (int64_t *)isrc_buf;
          temp = (long double)*ptr;
        }
        else {
          uint64_t *ptr = (uint64_t *)isrc_buf;
          temp = (long double)*ptr;
        }
        break;

      }


      switch (priv->fdst_size) {

      case 4:
        {
          float *ptr = (float *)fdst_buf;
          *ptr = (float)temp;
        }
        break;

      case 8:
        {
          double *ptr = (double *)fdst_buf;
          *ptr = (double)temp;
        }
        break;

      case 16:
        {
          long double *ptr = (long double *)fdst_buf;
          *ptr = (long double)temp;
        }
        break;

      }

      isrc_buf += priv->isrc_incr;
      fdst_buf += priv->fdst_incr;

    }

    break;

  default:
    /*
     * Unknown command.
     */
    return (-1);
  }


  return (0);
}

#endif

#endif

/* ifdef _TYPHIO_HDF5 */


/*
 * EOF
 */

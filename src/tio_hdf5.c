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
#include "tio_hdf5_int.h"

#include "tio_chunk_int.h"
#include "typhonio_viz.h"

#ifdef _TYPHIO_HDF5

#include <hdf5.h>

/*
 *  Vars and constants;
 */

static int InitCount = 0;

static const char *xVersionMajName = "TIO_version_major";
static const char *xVersionMinName = "TIO_version_minor";
static const char *xVersionRelName = "TIO_version_release";



/***************************************************************************************************
 *
 * iH5_CheckFile
 *
 **************************************************************************************************/
int
iH5_CheckFile( const char name[] )
{
  htri_t isHDF5;
  herr_t hrc;
  /* -- Tell HDF5 not to call cleanup at exit
     -- necessary to stop this for dynamically linked applications (eg. IDL) */
  hrc = H5dont_atexit();

  /* -- Tell HDF5 not to print default errors */
  hrc = H5Eset_auto(H5E_DEFAULT, NULL, NULL);


  /* -- Check if the file is an HDF5 one */
  isHDF5 = H5Fis_hdf5(name);

  hrc = H5Eclear(H5E_DEFAULT);

  if (isHDF5 > 0) {
    return (0);
  }
  else {
    return (-1);
  }
}




/***************************************************************************************************
 *
 * iH5_WriteVersion
 *
 **************************************************************************************************/
int
iH5_WriteVersion( const TIO_File_t fileID, const char name[],
                  const int maj, const int min, const int rel )
{
  const char *subname = "iH5_WriteVersion()";

  int irc;


  /* Will assume that file is open and writeable and that HDF5 has been initialized */

  irc =  iH5_WriteAttr(fileID, TIO_NULL, TIO_NULL, CLASS_FILE, xVersionMajName, TIO_INT,
                       (void *)&maj);
  irc += iH5_WriteAttr(fileID, TIO_NULL, TIO_NULL, CLASS_FILE, xVersionMinName, TIO_INT,
                       (void *)&min);
  irc += iH5_WriteAttr(fileID, TIO_NULL, TIO_NULL, CLASS_FILE, xVersionRelName, TIO_INT,
                       (void *)&rel);

  TIOassert(irc != 0, ERR_INT, "Failed to write version attributes", -1);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_ReadVersion
 *
 **************************************************************************************************/
int
iH5_ReadVersion( const TIO_File_t fileID, const char name[],
                 int *maj, int *min, int *rel )
{
  const char *subname = "iH5_ReadVersion()";

  hid_t  file_id, root_id;
  hid_t  dtype_id;
  hid_t  attr_id;
  herr_t hrc;

  const char *info_string = "TyphonIO_FileInfo";
  const char *vsn_string = "TyphonIO_nversion";

  int lmaj, lmin, lrel;
  int irc;



  if (fileID > 0) {

    /* If fileID > 0 reading from open file */
    irc = xH5_GetRef(fileID, TIO_NULL, &root_id, CLASS_NULL, NULL);
    switch (irc) {
    case ITIO_ERR_NONE:
      /* No error */
      break;
    case ITIO_ERR_FILEID:
      TIOreturn(ERR_INT, "Failed to get file ref - invalid fileID", ITIO_ERR_FILEID);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to get file ref", -1);
      break;
    }

    dtype_id = xH5_GetH5type(TIO_INT);

    /* At this point, the open file may not have the version attributes in the root if it is not
       a TyphonIO v1.x file */

    attr_id = H5Aopen_name(root_id, xVersionMajName);
    if (attr_id < 0) {

      /* Failed to open major version attribute
         - could be because the file isn't a TyphonIO v1.x file
         - but could also be it's a TyphonIO v0.x file instead
         - check for this */

      hid_t infogroup_id;

      infogroup_id = H5Gopen(root_id, info_string, H5P_DEFAULT);

      if (infogroup_id < 0) {
        /* Not a version 0.x file -and can't be 1.x as attr open above would've succeeded
           - return cleanly without error stack, but -ve to catch in caller */
        return (-100);
      }


      /* Must be a v0.1 or v0.2 file to get this far though... */

      lmaj = 0;
      lmin = 0;

      attr_id = H5Aopen_name(infogroup_id, vsn_string);
      if (attr_id < 0) {
        /* v0.1 */
        lrel = 1;
      }
      else {
        /* v0.2 */
        hrc  = H5Aclose(attr_id);
        lrel = 2;
      }

      hrc = H5Gclose(infogroup_id);

      hrc = H5Eclear(H5E_DEFAULT);

      TIOset(maj, lmaj);
      TIOset(min, lmin);
      TIOset(rel, lrel);


      /* So have established it's a v0.x file - return with appropriate error code. */
      return (ITIO_ERR_MISMATCH);
    }


    /* Ok - so if this far it's a v1.x file.  Now read version number attributes */


    hrc = H5Aread(attr_id, dtype_id, &lmaj);
    if (hrc < 0) {
      TIOreturn(ERR_HDF5, "Failed to read attribute", -2);
    }

    hrc = H5Aclose(attr_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close attribute", -3);


    attr_id = H5Aopen_name(root_id, xVersionMinName);
    if (attr_id < 0) {
      return (-101);
    }

    hrc = H5Aread(attr_id, dtype_id, &lmin);
    if (hrc < 0) {
      TIOreturn(ERR_HDF5, "Failed to read attribute", -4);
    }

    hrc = H5Aclose(attr_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close attribute", -5);


    attr_id = H5Aopen_name(root_id, xVersionRelName);
    if (attr_id < 0) {
      return (-102);
    }

    hrc = H5Aread(attr_id, dtype_id, &lmaj);
    if (hrc < 0) {
      TIOreturn(ERR_HDF5, "Failed to read attribute", -6);
    }

    hrc = H5Aclose(attr_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close attribute", -7);


  }
  else {

    /* If fileID < 1 assume file not open and HDF5 not initialized */
    /* Can assume file is an HDF5 file though */
    /* This routine is used as a test for whether a file is TyphonIO or not by checking
       that the version attributes exist.  So return accordingly.  */

    irc = iH5_Init();
    TIOassert(irc != 0, ERR_INT, "Failed to initialize HDF5", -8);

    file_id  = H5Fopen(name, H5F_ACC_RDONLY, H5P_DEFAULT);

    root_id  = H5Gopen(file_id, "/", H5P_DEFAULT);

    dtype_id = H5T_NATIVE_INT;



    attr_id = H5Aopen_name(root_id, xVersionMajName);
    if (attr_id < 0) {

      /* Failed to open major version attribute
         - could be because the file isn't a TyphonIO v1.x file
         - but could also be it's a TyphonIO v0.x file instead
         - check for this */

      hid_t infogroup_id;

      infogroup_id = H5Gopen(root_id, info_string, H5P_DEFAULT);

      if (infogroup_id < 0) {
        /* Not a version 0.x file -and can't be 1.x as attr open above would've succeeded
           - return cleanly without error stack, but -ve to catch in caller */
        return (-200);
      }


      /* Must be a v0.1 or v0.2 file to get this far though... */

      lmaj = 0;
      lmin = 0;

      attr_id = H5Aopen_name(infogroup_id, vsn_string);
      if (attr_id < 0) {
        /* v0.1 */
        lrel = 1;
      }
      else {
        /* v0.2 */
        hrc  = H5Aclose(attr_id);
        lrel = 2;
      }

      hrc = H5Gclose(infogroup_id);
      hrc = H5Gclose(root_id);
      hrc = H5Fclose(file_id);

      hrc = H5Eclear(H5E_DEFAULT);

      TIOset(maj, lmaj);
      TIOset(min, lmin);
      TIOset(rel, lrel);


      /* So have established it's a v0.x file - return with appropriate error code. */
      return (ITIO_ERR_MISMATCH);
    }


    /* Ok - so if we've got this far it's a v1.x file.  Now read version number attributes */

    /* Major */

    hrc = H5Aread(attr_id, dtype_id, &lmaj);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to read attribute", -10);

    hrc = H5Aclose(attr_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close attribute", -11);


    /* Minor */

    attr_id = H5Aopen_name(root_id, xVersionMinName);
    if (attr_id < 0) {
      TIOreturn(ERR_INT, "Failed to open version attribute", -20);
    }

    hrc = H5Aread(attr_id, dtype_id, &lmin);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to read attribute", -21);

    hrc = H5Aclose(attr_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close attribute", -22);


    /* Release */

    attr_id = H5Aopen_name(root_id, xVersionRelName);
    if (attr_id < 0) {
      TIOreturn(ERR_INT, "Failed to open version attribute", -30);
    }

    hrc = H5Aread(attr_id, dtype_id, &lrel);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to read attribute", -31);

    hrc = H5Aclose(attr_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close attribute", -32);


    hrc = H5Gclose(root_id);
    hrc = H5Fclose(file_id);

    irc = iH5_Kill();
    TIOassert(irc != 0, ERR_INT, "Failed to kill HDF5", -9);

  }


  TIOset(maj, lmaj);
  TIOset(min, lmin);
  TIOset(rel, lrel);

  return (0);
}



/***************************************************************************************************
 *
 * iH5_Init
 *
 **************************************************************************************************/
int iH5_Init( void )
{
  const char *subname = "iH5_Init()";

  int    irc;
  herr_t hrc;


  if (0 == InitCount) {

    hrc = H5open();   /* Initializes HDF5 if first time or closed previously */

    /* -- Stop HDF5 from calling cleanup at exit */
    /*    This is so a dynamically-linked HDF5 library doesn't cause problems - see HDF5 docs */
    /*       -- necessary to stop this for dynamically linking applications (eg. IDL) */

    hrc = H5dont_atexit();

    /* No need to check hrc - if H5dont_atexit is called more than once, the routine performs
       no action but returns a -1. */


    /* -- Turn off default printing of HDF5 errors */
    /*    Throughout all of TyphonIO, the HDF5 return codes are checked and if any errors are
          found, they are output from the TyphonIO error routines.
          The HDF5 errors are written to an internal stack, and this stack is emptied and printed
          by the H5Eprint() routine. */

    hrc = H5Eset_auto(H5E_DEFAULT, NULL, NULL);

    TIOassert(hrc < 0, ERR_HDF5, "HDF5 failed to overide error handler", -1);


    /* -- If the versions don't match, this routine will abort the application */
    hrc = H5check_version(H5_VERS_MAJOR, H5_VERS_MINOR, H5_VERS_RELEASE);
    if (hrc < 0) {
      TIOreturn(ERR_HDF5, "Failed on check of HDF5 version", ITIO_ERR_MISMATCH);
    }

    /* -- Initialize TyphonIO reference dataset names and HDF5 types */

    irc = xH5_SetRefNames();
    TIOassert(irc != 0, ERR_INT, "Failed on datatypes setup", -2);

    irc = xH5_SetTypes();
    TIOassert(irc != 0, ERR_INT, "Failed on datatypes setup", -3);


#if (( !((H5_VERS_MAJOR == 1) && (H5_VERS_MINOR >= 8)) ) && defined(H5_USE_16_API) )


    /* -- Register integer to floating-point conversion routine */
    /*    Should be able to remove this when HDF5 v1.8 comes along */

    hrc = H5Tregister( H5T_PERS_SOFT, "TIO_i2f", H5T_NATIVE_INT, H5T_NATIVE_FLOAT, &xH5_Conv_i2f );
    TIOassert(hrc < 0, ERR_HDF5, "Failed to register int->float conversion routine", -4);

#endif

  }

  ++InitCount;

  return (0);
}




/***************************************************************************************************
 *
 * iH5_Kill
 *
 **************************************************************************************************/
int
iH5_Kill( void )
{
  const char *subname = "iH5_Kill()";

  int    irc;
  herr_t hrc;


  --InitCount;

  if (InitCount > 0) {
    return (0);
  }

  /* -- Free up the type equivalences set up on initialisation */
  irc = xH5_FreeTypes();
  TIOassert(irc < 0, ERR_INT, "Failed to free types", -1);

  /* --  Garbage collect HDF5 to clean up memory */
  hrc = H5garbage_collect();
  TIOassert(hrc < 0, ERR_HDF5, "HDF5 failed to garbage collect", -2);


#ifndef _TYPHIO_NOH5CLOSE

  hrc = H5close();  /* -- This frees all resources (memory) used by HDF5 */

  /* Do not call the above if used in DLL applications - HDF5 will be closed whilst other DLLs
     may still be using it! */
#endif


  return (0);
}




/***************************************************************************************************
 *
 * iH5_PrintErrors
 *
 **************************************************************************************************/
int
iH5_PrintErrors( void )
{
  const char *subname = "iH5_PrintErrors()";

  herr_t hrc;

  (void)fprintf(stderr, "\n\n-- HDF5 Error traceback:   ----------------------------------- \n\n");

  hrc = H5Eprint(H5E_DEFAULT, stderr);

  if (hrc < 0) {
    TIOreturn(ERR_HDF5, "Failed on H5Eprint()", -1);
  }

  return (0);
}




/***************************************************************************************************
 *
 * iH5_ClearErrors
 *
 **************************************************************************************************/
int
iH5_ClearErrors( void )
{
  const char *subname = "iH5_ClearErrors()";

  herr_t hrc;

  hrc = H5Eclear(H5E_DEFAULT);

  if (hrc < 0) {
    TIOreturn(ERR_HDF5, "Failed on H5Eclear()", -1);
  }

  return (0);
}




/***************************************************************************************************
 *
 * iH5_InitRef
 *
 **************************************************************************************************/
int
iH5_InitRef( const TIO_File_t   fileID,
             const TIO_Object_t parentID,
             const iClass_t     class,
             const int          reading )
{
  const char *subname = "iH5_InitRef()";

  struct xH5ref_t *ref;

  hid_t      obj_id;
  hid_t      ref_id;
  hsize_t    nref;
  hsize_t    refsize;
  int        irc;

  /* -- Initialize HDF5 references simply by allocating references tables and setting counts
        If references already exist (ie. the file is being read) then read sizes and counts
        from file and read (collectively) to fill reference table.  Any new references will get
        appended to these.
        If not reading, use default sizes. */

  irc = xH5_GetRef(fileID, parentID, &obj_id, class, &ref);
  TIOassert(irc < 0, ERR_INT, "Failed to get reference struct", -1);


  ref->mod = 0;   /* mod is a count of how many times the reference gets modified */

  if (reading) {

    hsize_t spdims[1];
    hid_t   spc_id;
    herr_t  hrc;

    ref_id = H5Dopen(obj_id, xH5_GetRefName(class), H5P_DEFAULT);

    /* Reference may not have been written if none of the objects exist in the parent */

    if (ref_id < 0) {

      nref    = 0;
      refsize = xH5_GetRefSize(class);
      ref_id  = 0;

    }
    else {

      spc_id = H5Dget_space(ref_id);
      TIOassert(spc_id < 0, ERR_HDF5, "Failed to get existing dataspace", -50);

      irc = H5Sget_simple_extent_dims(spc_id, spdims, NULL);
      TIOassert(irc < 0, ERR_HDF5, "Failed to get dataspace extent", -51);

      hrc = H5Sclose(spc_id);
      TIOassert(hrc != 0, ERR_HDF5, "Failed to close dataspace", -52);

      nref      = spdims[0];
      refsize   = TIOmax(spdims[0], xH5_GetRefSize(class));
      /* ref_id = as set above */
    }

  }
  else {

    nref      = 0;
    refsize   = xH5_GetRefSize(class);
    ref_id    = 0;

  }


  ref->buf = TIOmalloc(refsize * sizeof(char [TIO_STRLEN]));
  if (NULL == ref->buf) {
    TIOreturn(ERR_MEM, "Failed to malloc ref->buf", -4);
  }

  ref->n    = nref;
  ref->size = refsize;
  ref->id   = ref_id;


  if (reading && (nref > 0)) {

    /* Read references collectively (all processes need a filled reference table) */

    struct xH5file_t *hfile;
    hid_t  xfer_plist;
    herr_t hrc;

    irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
    TIOassert(irc < 0, ERR_USER, "Invalid fileID", -51);

    xfer_plist = xH5_GetXfer(hfile, TIO_XFER_COLLECTIVE);

    hrc = H5Dread(ref_id, xH5_GetH5type(TIO_STRING), H5S_ALL, H5S_ALL, xfer_plist, ref->buf);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to read reference dataset", -54);
  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_AddRef
 *
 **************************************************************************************************/
int
iH5_AddRef( const TIO_File_t   fileID,
            const TIO_Object_t parentID,
            const TIO_Object_t objectID,
            const iClass_t     class,
            const char         name[]   )
{
  const char *subname = "iH5_AddRef()";

  struct xH5ref_t      *ref;
  struct xH5object_t   *vobject;
  struct xH5varcache_t *vcache;

  hsize_t    ii;
  int        irc;



  irc = xH5_GetRef(fileID, parentID, NULL, class, &ref);
  TIOassert(irc < 0, ERR_INT, "Failed to get reference struct", -1);

  ++ref->mod;

  if (CLASS_VAR == class) {

    /* Get variable reference in parent */
    irc = xH5_GetVcache(fileID, parentID, &vcache);
    TIOassert(irc < 0, ERR_INT, "Failed to get reference struct", -2);


    irc = iTIO_GetFFinfo( 0, objectID, (void **)&vobject );
    TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);  /* Invalid objectID */

    /* -- Expand variable cache if necessary */
    /*    This is so it is of the correct size for putting caches of units, etc., later */

    if ((ref->n + 1) > ref->size) {

      ii = ref->size + xH5_GetRefDelta(CLASS_VAR);

      vcache->units = TIOrealloc(vcache->units, (ii * sizeof(char [TIO_STRLEN])));
      if (NULL == vcache->units) {
        TIOreturn(ERR_MEM, "Failed to realloc ref->units", -3);
      }
      vcache->types = TIOrealloc(vcache->types, (ii * sizeof(int)));
      if (NULL == vcache->types) {
        TIOreturn(ERR_MEM, "Failed to realloc ref->types", -4);
      }
      vcache->index = TIOrealloc(vcache->index, (ii * sizeof(TIO_Size_t)));
      if (NULL == vcache->index) {
        TIOreturn(ERR_MEM, "Failed to realloc ref->index", -5);
      }
    }
  }


  ii = ref->n;
  ++ref->n;


  /* -- Expand reference table if necessary */

  if (ref->n > ref->size) {

    ref->size += xH5_GetRefDelta(class);

    ref->buf = TIOrealloc(ref->buf, (ref->size * sizeof(char [TIO_STRLEN])));
    if (NULL == ref->buf) {
      TIOreturn(ERR_MEM, "Failed to realloc ref->buf", -8);
    }
  }


  /* -- Store reference to object in next position in table */
  (void) strcpy(ref->buf[ii], name);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_RemoveRef
 *
 **************************************************************************************************/
int
iH5_RemoveRef( const TIO_File_t   fileID,
               const TIO_Object_t parentID,
               const TIO_Object_t objectID,
               const iClass_t     class,
               const char         name[] )
{
  const char *subname = "iH5_RemoveRef()";

  struct xH5ref_t      *ref;
  int        irc;

  /* Removes the latest addition to the parent object of the given class */

  irc = xH5_GetRef(fileID, parentID, NULL, class, &ref);
  TIOassert(irc < 0, ERR_INT, "Failed to get reference struct", -1);

  --ref->mod;
  --ref->n;

#ifndef NDEBUG
  /* check given name matches reference being removed */
  /*
    CALLING ROUTINES NOT ACTUALLY PASSING NAME IN YET!
  if (strcmp(name, ref->buf[ref->n] != 0)) {
    TIOassert(irc < 0, ERR_INT, "name of reference being removed does not match", -2);
  }
*/
#endif

  /* No real need to blow away the contents of the ref buf */
  /* -- Store reference to object in next position in table */
  /* (void) strcpy(ref->buf[ref->n], name); */



  return (0);
}




/***************************************************************************************************
 *
 * iH5_FinalizeRef
 *
 **************************************************************************************************/
int
iH5_FinalizeRef( const TIO_File_t   fileID,
                 const TIO_Object_t parentID,
                 const iClass_t     class    )
{
  const char *subname = "iH5_FinalizeRef()";

  struct xH5file_t *hfile;
  struct xH5ref_t  *ref;

  hid_t   obj_id;
  hid_t   memsp_id;                                /* HDF5 ID for memory dataspace */
  hid_t   cplist_id;                               /* HDF5 ID for dataset creation property list */
  hid_t   xfer_plist;
  hsize_t dims[1];                                 /* Initial dimension extent of dataset */
  const hsize_t chunkdims[1] = {1};                /* Size of dataset chunks (1 = 1 reference) */

  herr_t  hrc;
  int    irc;



  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Invalid fileID", -1);

  irc = xH5_GetRef(fileID, parentID, &obj_id, class, &ref);
  TIOassert(irc < 0, ERR_INT, "Failed to get reference struct", -2);


  if (CLASS_VAR == class) {
    /* ref will be variable reference */
    irc = xH5_FinalizeVarCache(fileID, parentID, ref->n, ref->size, ref->mod, obj_id);
    TIOassert(irc < 0, ERR_INT, "Failed to finalize variable references", -3);
  }


  if (ref->id > 0) {

    /* -- Reference dataset exists already */

    if (ref->mod) {

      xfer_plist = xH5_GetXfer(hfile, TIO_XFER_INDEPENDENT);

      /* -- Appending only if it has been modified (can assume ref->n > 0) */

      /* -- Set the increased size of the reference dataset and extend it */

      dims[0] = (hsize_t)ref->n;
      hrc     = H5Dextend(ref->id, dims);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to extend dataset", -10);


      /* -- Could select a hyperslab of the new references at the end of the table and just
            write those into the newly extended space at the end of dataset.
            But instead, just write the whole lot - ie. the old ones simply get re-written
            with the same old data.  I figure a single write to the whole dataset (which ain't
            going to be very big anyway) should be more efficient than pratting about with
            hyperslabs.  */


      /* -- Write the references to the dataset */
      if (0 == hfile->rank) {
        hrc = H5Dwrite(ref->id, xH5_GetH5type(TIO_STRING), H5S_ALL, H5S_ALL,
                       xfer_plist, (const void*)ref->buf);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to write reference dataset", -11);
      }

    }

    /* -- Close reference dataset */
    hrc = H5Dclose(ref->id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataset", -12);

  }


  else {

    /* -- New reference dataset may be required if objects exist in the parent */

    if (ref->n > 0) {

      /* -- Create a new unlimited 1D dataspace to hold references */

      dims[0]  = ref->n;

      memsp_id = H5Screate_simple(1, dims, UNLIMDIMS);
      TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create dataspace", -21);

      /* -- Create an extendable, HDF5 chunked dataset - have to change dataset creation properties
       *
       *    Have to have dataset allocation time set to H5D_ALLOC_TIME_EARLY in both serial and
       *    parallel libraries, so that files with serially-created datasets (which by default would
       *    have H5D_ALLOC_TIME_INCR) can be post-processed in parallel
       */

      cplist_id = H5Pcreate(H5P_DATASET_CREATE);
      TIOassert(cplist_id < 0, ERR_HDF5, "Failed to create property list", -22);

      hrc =  H5Pset_layout(cplist_id, H5D_CHUNKED);
      hrc += H5Pset_chunk(cplist_id, 1, chunkdims);
      hrc += H5Pset_alloc_time(cplist_id, H5D_ALLOC_TIME_EARLY);
      hrc += H5Pset_fill_time(cplist_id, H5D_FILL_TIME_NEVER);
      TIOassert(hrc != 0, ERR_HDF5, "Failed to set dataset properties", -23);

      ref->id = H5Dcreate(obj_id, xH5_GetRefName(class), xH5_GetH5type(TIO_STRING),
                          memsp_id, H5P_DEFAULT, cplist_id, H5P_DEFAULT);

      TIOassert(ref->id < 0, ERR_HDF5, "Failed to create reference dataset", -24);

      hrc = H5Pclose(cplist_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataset property list", -25);

      if (0 == hfile->rank) {

        xfer_plist = xH5_GetXfer(hfile, TIO_XFER_INDEPENDENT);

        hrc = H5Dwrite(ref->id, xH5_GetH5type(TIO_STRING), H5S_ALL, H5S_ALL,
                       xfer_plist, (const void*)ref->buf);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to write reference dataset", -26);
      }

      /* -- Close dataspace */
      hrc = H5Sclose(memsp_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataspace", -27);

      /* -- Close reference dataset */
      hrc = H5Dclose(ref->id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataset", -28);
    }

    /* else - no need to create and write any reference dataset
              used to write a zero-length dataset, but no real need to do this
              also makes it easier to expand number of model features allowed in parent
              objects if the number of reference datasets isn't fixed and they are assumed
              to exist */
  }



  TIOfree (ref->buf); ref->buf = NULL;
#ifndef NDEBUG
  ref->n    = 888888;
  ref->size = 888888;
  ref->id   = 888888;
#endif


  return (0);
}




/***************************************************************************************************
 *
 * iH5_ReadRef
 *
 **************************************************************************************************/
int
iH5_ReadRef( const TIO_File_t   fileID,
             const TIO_Object_t parentID,
             const iClass_t     class,
             TIO_Size_t         *nobj,
             char               names[][TIO_STRLEN] )
{
  const char *subname = "iH5_ReadRef()";

  struct xH5ref_t *ref;
  hsize_t         i;

  int irc;



  irc = xH5_GetRef(fileID, parentID, NULL, class, &ref);
  TIOassert(irc < 0, ERR_INT, "Failed to get reference struct", ITIO_ERR_PARENTID);

  /* check invalid parentID, etc... */


  TIOset(nobj, ref->n);

  if ((names != NULL) && (ref->n > 0)) {
    for (i=0; i<ref->n; ++i) {
      (void) iTIO_strcpy(names[i], ref->buf[i]);
    }
  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_CreateFile
 *
 **************************************************************************************************/
int
iH5_CreateFile( const TIO_File_t   fileID,
                const char         name[],
                const TIO_Access_t access,
                const MPI_Comm     comm,
                const MPI_Info     info,
                const int          rank )
{
  const char *subname = "iH5_CreateFile()";

  struct xH5file_t *hfile;

  hid_t access_plist;                      /* HDF5 ID for file access property list */
  hid_t create_plist;                      /* HDF5 ID for file/dataset creation property list */

  unsigned int accessH5;                   /* HDF5 file access flag */
  int          nchildren;                  /* Number of child objects */
  int          irc;                        /* Return code from internal routines */
  herr_t       hrc;                        /* HDF5 return code */



  hfile = (struct xH5file_t *)TIOmalloc(sizeof(struct xH5file_t));
  if (NULL == hfile) {
    TIOreturn(ERR_MEM, "Failed to malloc hfile", -1);
  }

  hfile->varcache = NULL;

  /* File needs references for all child objects */
  irc = iTIO_HierarchyGetNumChildren(CLASS_FILE, &nchildren);
  hfile->ref = TIOmalloc(sizeof(struct xH5ref_t) * nchildren);
  if (NULL == hfile->ref) {
    TIOreturn(ERR_MEM, "Failed to allocate file reference table", ERR_INT);
  }

#ifndef NDEBUG
  (void) dNullFile(hfile, name);
  (void) dNullRef(hfile, NULL, CLASS_FILE, name);
#endif


  access_plist = H5Pcreate(H5P_FILE_ACCESS);
  TIOassert(access_plist < 0, ERR_HDF5, "Failed to create access_plist", -2);

  /* -- Created transfer property lists above will have default value:  H5P_DEFAULT */

  /* -- Set any required parallel IO drivers on the data transfer property lists */

#ifdef _TYPHIO_PARALLEL
  {
    int mpierr;

    hsize_t threshold = (512*1024);   /* = 256 kilobytes  - only align objects bigger than this  */
    hsize_t alignment = (1024*1024);  /* = 1 megabyte     - alignment -> good for lustre!        */
    hsize_t metasize  = (1024*1024);  /* = 1~4 megabyte    - metadata block size ??  template??*/
    hsize_t smallsize = (4*1024*1024);  /* = 1~16 megabyte   - small data block size ??          */


    /* -- Parallel IO being used */
    /*    Duplicate MPI comm for sanity */

    mpierr = MPI_Comm_dup(comm, &hfile->comm);
    TIOassert(mpierr != MPI_SUCCESS, ERR_MPI, "Failed to duplicate MPI comm", -20);

    mpierr = MPI_Barrier(hfile->comm);
    TIOassert(mpierr != MPI_SUCCESS, ERR_MPI, "Failed on MPI barrier", -21);

    hfile->rank = rank;

    hrc = H5Pset_fapl_mpio(access_plist, hfile->comm, info);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set MPI-IO collective file access plist", -22);
/*
    hrc = H5Pset_fapl_mpiposix(access_plist, hfile->comm, 0);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set MPI-IO collective file access plist", -22);

    hrc = H5Pset_sieve_buf_size(access_plist, alignment);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set alignment access plist", -23);

    hrc = H5Pset_alignment(access_plist, threshold, alignment);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set alignment access plist", -23);

    hrc = H5Pset_small_data_block_size( access_plist, smallsize);
 
    hrc = H5Pset_meta_block_size(access_plist, metasize);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set alignment access plist", -23);
*/
  }
#else

  hfile->comm = comm;
  hfile->rank = 0;

#endif


  /* -- Set the HDF5 access property according to the TyphonIO one */
  switch (access) {
  case TIO_ACC_CREATE:
    accessH5 = H5F_ACC_EXCL;
    break;
  case TIO_ACC_REPLACE:
    accessH5 = H5F_ACC_TRUNC;
    break;
  default:
    TIOfree (hfile); hfile = NULL;
    TIOreturn(ERR_USER, "Invalid file access value", ITIO_ERR_ARG);
    break;
  }


  /* -- Create a file creation property list */
  create_plist = H5Pcreate(H5P_FILE_CREATE);
  TIOassert(create_plist < 0, ERR_HDF5, "Failed to create create_plist", -5);

  /* -- Set object offsets and lengths - these are defined in tio_hdf5_int.h */
  hrc = H5Pset_sizes(create_plist, TIO_H5_OBJECT_OFFSET, TIO_H5_OBJECT_LENGTH);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to set file offsets/lengths", -6);

  hrc = H5Pset_libver_bounds(access_plist, H5F_LIBVER_LATEST, H5F_LIBVER_LATEST);
  TIOassert(access_plist < 0, ERR_HDF5, "Failed to set access_plist version bound", -2);

  /* -- Create the file */
  hfile->file_id = H5Fcreate(name, accessH5, create_plist, access_plist);

  /* -- Close file creation and access property lists */
  hrc =  H5Pclose(create_plist);
  hrc += H5Pclose(access_plist);
  TIOassert(hrc != 0, ERR_HDF5, "Failed to close create/access plist", -7);

  if (hfile->file_id <  0) {

#ifdef _TYPHIO_PARALLEL

    int mpierr;

    mpierr = MPI_Comm_free(&hfile->comm);
    TIOassert(mpierr != MPI_SUCCESS, ERR_MPI, "Failed to free comm", -30);

#endif

    TIOfree (hfile); hfile = NULL;
    TIOreturn(ERR_HDF5, "HDF5 file creation error", ITIO_ERR_CREATE);
  }

  /* -- Set up data transfer protocols (collective & independent) */
  irc = xH5_SetXfer(hfile);
  TIOassert(irc < 0, ERR_INT, "Failed to set transfer protocols", -8);


  /* -- Open the root group within the new file */

  hfile->root_id = H5Gopen(hfile->file_id, "/", H5P_DEFAULT);

  TIOassert(hfile->root_id < 0, ERR_HDF5, "Failed to open file root group", -9);


  /* -- Register the HDF5-specific info */
  irc = iTIO_RegisterFFinfo(fileID, TIO_NULL, (void *)hfile);
  TIOassert(irc < 0, ERR_INT, "Failed to register ffinfo", -10);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_OpenFile
 *
 **************************************************************************************************/
int
iH5_OpenFile( const TIO_File_t   fileID,
              const char         name[],
              const TIO_Access_t access,
              const MPI_Comm     comm,
              const MPI_Info     info,
              const int          rank  )
{
  const char *subname = "iH5_OpenFile()";

  struct xH5file_t *hfile;

  hid_t access_plist;                      /* HDF5 ID for file access property list */

  unsigned int accessH5;                   /* HDF5 file access flag */
  int          nchildren;                  /* Number of child objects */
  int          irc;                        /* Return code from internal routines */
  herr_t       hrc;                        /* HDF5 return code */



  hfile = (struct xH5file_t *)TIOmalloc(sizeof(struct xH5file_t));
  if (NULL == hfile) {
    TIOreturn(ERR_MEM, "Failed to malloc hfile", -1);
  }

  hfile->ref      = NULL;
  hfile->varcache = NULL;

  /* File needs references for all child objects */
  irc = iTIO_HierarchyGetNumChildren(CLASS_FILE, &nchildren);
  hfile->ref = TIOmalloc(sizeof(struct xH5ref_t) * nchildren);
  if (NULL == hfile->ref) {
     TIOreturn(ERR_MEM, "Failed to allocate file reference table", ERR_INT);
  }

#ifndef NDEBUG
  (void) dNullFile(hfile, name);
  (void) dNullRef(hfile, NULL, CLASS_FILE, name);
#endif


  access_plist = H5Pcreate(H5P_FILE_ACCESS);
  TIOassert(access_plist < 0, ERR_HDF5, "Failed to create access_plist", -2);

  /* -- Created transfer property lists above will have default value:  H5P_DEFAULT */

  /* -- Set any required parallel IO drivers on the data transfer property lists */


#ifdef _TYPHIO_PARALLEL
  {
    int mpierr;

    /* -- Parallel IO being used */
    /*    Duplicate MPI comm for sanity */

    mpierr = MPI_Comm_dup(comm, &hfile->comm);
    TIOassert(mpierr != MPI_SUCCESS, ERR_MPI, "Failed to duplicate MPI comm", -20);

    hfile->rank = rank;

    mpierr = MPI_Barrier(hfile->comm);
    TIOassert(mpierr != MPI_SUCCESS, ERR_MPI, "Failed on MPI barrier", -21);

    hrc = H5Pset_fapl_mpio(access_plist, hfile->comm, info);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set MPI-IO collective file access plist", -22);
/*
    hrc = H5Pset_fapl_mpiposix(access_plist, hfile->comm, 0);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set MPI-IO collective file access plist", -22);
*/
  }
#else

  hfile->comm = comm;
  hfile->rank = 0;

#endif


  /* -- Set the HDF5 access property according to the TyphonIO one */
  switch (access) {
  case TIO_ACC_READWRITE:
  case TIO_ACC_READWRITESTRICT:
    accessH5 = H5F_ACC_RDWR;
    break;
  case TIO_ACC_READONLY:
    accessH5 = H5F_ACC_RDONLY;
    break;
  default:
    TIOfree (hfile); hfile = NULL;
    TIOreturn(ERR_USER, "Invalid file access value", ITIO_ERR_ARG);
    break;
  }


  /* -- Open the file - don't need any file open property list, just access, unlike create */
  hfile->file_id = H5Fopen(name, accessH5, access_plist);

  /* disable metadata cache evictions */
#ifdef _DISABLE_METADATA
  H5AC_cache_config_t mdc_config;

  mdc_config.version = H5AC__CURR_CACHE_CONFIG_VERSION;

  H5Pget_mdc_config(hfile->file_id, &mdc_config);
  mdc_config.evictions_enabled =FALSE;
  mdc_config.incr_mode = H5C_incr__off;
  mdc_config.decr_mode = H5C_decr__off;

  H5Pset_mdc_config(hfile->file_id, &mdc_config);
#endif

  /* -- Close file access property list */
  hrc = H5Pclose(access_plist);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close access_plist", -3);


  if (hfile->file_id < 0) {

#ifdef _TYPHIO_PARALLEL

    int mpierr;

    mpierr = MPI_Comm_free(&hfile->comm);
    TIOassert(mpierr != MPI_SUCCESS, ERR_MPI, "Failed to free comm", -23);

#endif

    TIOfree (hfile); hfile = NULL;
    TIOreturn(ERR_HDF5, "File open error.  Check file exists and/or permissions", ITIO_ERR_OPEN);
  }


  /* -- Set up data transfer protocols (collective & independent) */
  irc = xH5_SetXfer(hfile);
  TIOassert(irc < 0, ERR_INT, "Failed to set transfer protocols", -8);


  /* -- Open the root group within the new file */

  hfile->root_id = H5Gopen(hfile->file_id, "/", H5P_DEFAULT);


  TIOassert(hfile->root_id < 0, ERR_HDF5, "Failed to open file root group", -6);


  /* -- Register the HDF5-specific info */
  irc = iTIO_RegisterFFinfo(fileID, TIO_NULL, (void *)hfile);
  TIOassert(irc < 0, ERR_INT, "Failed to register ffinfo", -7);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_CloseFile
 *
 **************************************************************************************************/
int iH5_CloseFile( const TIO_File_t fileID )
{
  const char *subname = "iH5_CloseFile()";

  struct xH5file_t *hfile;

  hid_t  *objlist;                         /* List of open HDF5 object IDs in the file */
  hid_t  obj_id;                           /* A specific HDF5 object ID */
  int    objcount;                         /* Count of open objects */
  int    i;                                /* Loop variable */

  int    irc;                              /* Return code from internal routines */
  herr_t hrc;                              /* HDF5 return code */


  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Invalid fileID", ITIO_ERR_FILEID);

  /* -- Close property lists for the file */
  irc = xH5_FreeXfer(hfile);
  TIOassert(irc < 0, ERR_INT, "Failed to close data transfer property list for file", -2);


  /* -- Close the root group in the file */
  hrc = H5Gclose(hfile->root_id);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close root group", -3);


  /* -- Close any open objects in the file - if TyphonIO used properly there shouldn't be any   */
  /*    Could later improve this to actually close objects as TyphonIO objects rather than HDF5 */

  /*    It is possible to have the same file open a multiple of times as read-only, so need to
        check before clearing up.  */

  i = iTIO_GetFileInstances(fileID, NULL, NULL);

  if (1 == i) {

#ifndef NDEBUG

    /* Debug - look for unexpected open objects and report */

    objcount = H5Fget_obj_count(hfile->file_id, H5F_OBJ_ALL); /* should == 1 for file object only */
    if (objcount > 1) {

      H5I_type_t objtype;
      char objname[256];

      objlist  = (hid_t *)TIOmalloc(sizeof(hid_t) * objcount);

      irc = H5Fget_obj_ids(hfile->file_id, H5F_OBJ_ALL, objcount, objlist);

      for (i=0; i<objcount; ++i) {
        obj_id = objlist[i];

        objtype = H5Iget_type(obj_id);
        (void) H5Iget_name(obj_id, objname, 256);

        fprintf(stderr, "-- Object still open:  %s\n", objname);

      }

      TIOfree (objlist); objlist = NULL;

    }

#else

    /* Production - close up all open objects */

    objcount = H5Fget_obj_count(hfile->file_id, H5F_OBJ_DATASET);
    if (objcount > 0) {
      objlist  = (hid_t *)TIOmalloc(sizeof(hid_t) * (size_t)objcount);
      irc = H5Fget_obj_ids(hfile->file_id, H5F_OBJ_DATASET, objcount, objlist);
      for (i=0; i<objcount; ++i) {
        obj_id = objlist[i];
        (void) H5Dclose(obj_id);
      }
      TIOfree (objlist); objlist = NULL;
    }

    objcount = H5Fget_obj_count(hfile->file_id, H5F_OBJ_GROUP);
    if (objcount > 0) {
      objlist  = (hid_t *)TIOmalloc(sizeof(hid_t) * (size_t)objcount);
      irc = H5Fget_obj_ids(hfile->file_id, H5F_OBJ_GROUP, objcount, objlist);
      for (i=0; i<objcount; ++i) {
        obj_id = objlist[i];
        (void) H5Gclose(obj_id);
      }
      TIOfree (objlist); objlist = NULL;
    }

#endif

  }
  else {
    TIOassert(i < 0, ERR_INT, "Get file instances failed", -4);
  }


  hrc = H5Fflush(hfile->file_id, H5F_SCOPE_GLOBAL);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to flush file", -5);


  /* -- Close the file */
  hrc = H5Fclose(hfile->file_id);
  if (hrc < 0) {
    TIOreturn(ERR_HDF5, "Failed to close file", ITIO_ERR_CLOSE);
  }

  /* -- Deregister the HDF5-specific info */
  irc = iTIO_DeregisterFFinfo(fileID, TIO_NULL);
  TIOassert(irc < 0, ERR_INT, "Failed to deregister ffinfo", -6);


#ifdef _TYPHIO_PARALLEL
  {
    int mpierr;

    mpierr = MPI_Comm_free(&hfile->comm);
    TIOassert(mpierr != MPI_SUCCESS, ERR_MPI, "Failed to free comm", -10);
  }
#endif


  TIOfree (hfile->ref); hfile->ref = NULL;
  TIOfree (hfile);      hfile      = NULL;


  return (0);
}




/***************************************************************************************************
 *
 * iH5_FlushFile
 *
 **************************************************************************************************/
int iH5_FlushFile( const TIO_File_t fileID )
{
  const char *subname = "iH5_FlushFile()";

  struct xH5file_t *hfile;

  int    irc;                              /* Return code from internal routines */
  herr_t hrc;                              /* HDF5 return code */



  /* Flush all references, variables cache and objects in the file - this ain't pretty */
  /* Flush references by finalizing and re-opening */
  /* Variable caches are finalized automatically when finalizing the variable reference */

  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Invalid fileID", ITIO_ERR_FILEID);


  /* Finally flush wrt HDF5 to make sure everything is written to disk */

  hrc = H5Fflush(hfile->file_id, H5F_SCOPE_GLOBAL);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to flush file", -3);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_CreateObject
 *
 **************************************************************************************************/
int
iH5_CreateObject( const TIO_File_t   fileID,
                  const TIO_Object_t parentID,
                  const TIO_Object_t objectID,
                  const iClass_t     class,
                  const char         name[] )
{
  const char *subname = "iH5_CreateObject()";

  struct xH5object_t   *hobject;
  struct xH5ref_t      *ref;
  struct xH5varcache_t *varcache;

  size_t nrefs;
  hid_t  obj_id;

  int nchildren;
  int irc;
  herr_t hrc;    


  /* -- Get hid_t of parent location - can be either an object or the file root */
  irc = xH5_GetRef(fileID, parentID, &obj_id, class, &ref);
  switch (irc) {
  case ITIO_ERR_NONE:
    /* No error */
    break;
  case ITIO_ERR_FILEID:
    TIOreturn(ERR_INT, "Failed to get object ref - invalid fileID", ITIO_ERR_FILEID);
    break;
  case ITIO_ERR_OBJECTID:
    TIOreturn(ERR_INT, "Failed to get object ref - invalid parentID", ITIO_ERR_PARENTID);
    break;
  case ITIO_ERR_CLASS:
    TIOreturn(ERR_INT, "Failed to get object ref - invalid  class", ITIO_ERR_CLASS);
    break;
  default:
    TIOreturn(ERR_INT, "Failed to get object ref", -1);
    break;
  }


  hobject = (struct xH5object_t *)TIOmalloc(sizeof(struct xH5object_t));
  if (NULL == hobject) {
    TIOreturn(ERR_MEM, "Failed to malloc hobject", -2);
  }

#ifndef NDEBUG
  (void) dNullObject(hobject);
  (void) strcpy(hobject->name, name);
  hobject->class  = class;
#endif


  /* Number of references for possible child objects */
  irc   = iTIO_HierarchyGetNumChildren(class, &nchildren);
  nrefs = (size_t)nchildren;


  if (CLASS_VAR == class) {

    irc = xH5_GetVcache(fileID, parentID, &varcache);

    /* -- Variables exist in the parent rather than being a separate HDF5 object (ie. in group
          of it's own).
          So, need to now initialize the variable cache in the parent.  These are only
          initialized when the first variable is created as not all objects will contain variables
          and malloc-ing caches when unnecessary will add pointless overhead.
          Variable object also doesn't need any references (they can't have child objects)  */

    /*    May also need to read the varcache to get units as this variable being created may be
          being appended.  If don't read the units, units cache will only contain appended values
          and not the originals.
          ref[CLASS_VAR]->id > 0 tells us that we should read */

    if (NULL == varcache) {
      irc = xH5_InitVarCache(fileID, parentID, ref->n, ref->size, (ref->id > 0), obj_id);
      TIOassert(irc < 0, ERR_INT, "Failed to initialize variable reference", -10);

      irc = xH5_GetVcache(fileID, parentID, &varcache);
    }


    /*    As variables aren't contained in an separate HDF5 group like other objects (well, apart
          from >0D variables), need to check the variable names don't conflict with other objects
          in the parent.  So check against all existing variables and other existing objects */

    /* Check no object in the parent already with this name */
    if (xH5_ObjectExists(obj_id, name)) {
      TIOfree (hobject); hobject = NULL;
      TIOreturn(ERR_USER, "Object already exists in parent with given name", ITIO_ERR_CREATE);
    }

    /* Check no variable in the parent already with this name */
    {
      hsize_t i;

      for (i=0; i<ref->n; ++i) {
        if (0 == strcmp(ref->buf[i], name)) {
          TIOfree (hobject); hobject = NULL;
          TIOreturn(ERR_USER, "Variable already exists in parent with given name", ITIO_ERR_CREATE);
        }
		/* in the unlikely case of no variable name - length  */
        if (0 == strlen(name)) {
          TIOfree (hobject); hobject = NULL;
          TIOreturn(ERR_USER, "Variable name not given", ITIO_ERR_CREATE);
        };
      }
    }

    /* -- Variables exist in the parent object group, so use group_id for this
          Also set index to the current variable count - this is the variable's position
          in the reference tables.  */

    hobject->group_id    = obj_id;
    hobject->varrefindex = ref->n;    /* Yet to be ++ when reference for new variable added */

  }
  else {    /*  (class != CLASS_VAR)  */

    /* nrefs will be > 0 */

    hobject->varcache = NULL;

    hobject->ref = (struct xH5ref_t *)TIOmalloc(sizeof(struct xH5ref_t) * nrefs);
    if (NULL == hobject->ref) {
      TIOfree (hobject); hobject = NULL;
      TIOreturn(ERR_MEM, "Failed to malloc hobject->ref", -20);
    }

#ifndef NDEBUG
    (void) dNullRef(NULL, hobject, class, name);
#endif

    /* -- Create group */

    hid_t  gcpl_id;
    gcpl_id = H5Pcreate(H5P_GROUP_CREATE);
    TIOassert(gcpl_id < 0, ERR_HDF5, "Failed to create gcpl_id", -3); 
   
    hrc = H5Pset_link_creation_order(gcpl_id, H5P_CRT_ORDER_TRACKED|H5P_CRT_ORDER_INDEXED);
    hrc += H5Pset_attr_creation_order(gcpl_id, H5P_CRT_ORDER_TRACKED|H5P_CRT_ORDER_INDEXED);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to set group creation order", -4);

    hobject->group_id = H5Gcreate2(obj_id, name, H5P_DEFAULT, gcpl_id, H5P_DEFAULT);
 
    hrc = H5Pclose(gcpl_id);
    TIOassert(hrc < 0, ERR_INT, "Failed to close", -5);

    if (hobject->group_id < 0) {
      TIOfree (hobject->ref); hobject->ref = NULL;
      TIOfree (hobject);      hobject      = NULL;
      TIOreturn(ERR_HDF5, "Failed to create group", ITIO_ERR_CREATE);
    }

  }  /* if (class == CLASS_VAR)  */


  hobject->chunksectionID = NULL;

  /* -- Register the HDF5-specific info */
  irc = iTIO_RegisterFFinfo(TIO_NULL, objectID, (void *)hobject);
  TIOassert(irc < 0, ERR_INT, "Failed to register ffinfo", -6);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_OpenObject
 *
 **************************************************************************************************/
int
iH5_OpenObject( const TIO_File_t   fileID,
                const TIO_Object_t parentID,
                const TIO_Object_t objectID,
                const iClass_t     class,
                const int          byindex,
                char               name[],
                const TIO_Size_t   idx )
{
  const char *subname = "iH5_OpenObject()";

  struct xH5object_t   *hobject;
  struct xH5ref_t      *ref;
  struct xH5varcache_t *varcache;

  int     nchildren;
  hsize_t nrefs;
  hid_t   obj_id;

  int     irc;



  /* -- Get hid_t of parent location - can be either an object or the file root */
  irc = xH5_GetRef(fileID, parentID, &obj_id, class, &ref);
  switch (irc) {
  case ITIO_ERR_NONE:
    /* No error */
    break;
  case ITIO_ERR_FILEID:
    TIOreturn(ERR_INT, "Failed to get object ref - invalid fileID", ITIO_ERR_FILEID);
    break;
  case ITIO_ERR_OBJECTID:
    TIOreturn(ERR_INT, "Failed to get object ref - invalid parent ID", ITIO_ERR_PARENTID);
    break;
  case ITIO_ERR_CLASS:
    TIOreturn(ERR_INT, "Failed to get object ref - invalid class", ITIO_ERR_CLASS);
    break;
  default:
    TIOreturn(ERR_INT, "Failed to get object ref", -1);
    break;
  }


  hobject = (struct xH5object_t *)TIOmalloc(sizeof(struct xH5object_t));
  if (NULL == hobject) {
    TIOreturn(ERR_MEM, "Failed to malloc hobject", -2);
  }

#ifndef NDEBUG
  (void) dNullObject(hobject);
  hobject->class  = class;
#endif


  /* Number of references for possible child objects */
  irc   = iTIO_HierarchyGetNumChildren(class, &nchildren);
  nrefs = (hsize_t)nchildren;


  if (byindex) {

    /* -- Opening by idx */

    /* -- Simply find name at entry idx in the reference table.
          Then use this to open the object.  */

    if ((hsize_t)(idx + 1) > ref->n) {
      TIOfree (hobject); hobject = NULL;
      TIOreturn(ERR_INT, "idx > refsize", ITIO_ERR_INDEX);
    }

    /* -- Get the name of the object from i-th entry in the reference table */
    (void) strcpy(name, ref->buf[idx]);

  }


  /* -- Now open the object by name */

  if (CLASS_VAR == class) {

    irc = xH5_GetVcache(fileID, parentID, &varcache);

    if (NULL == varcache) {
      irc = xH5_InitVarCache(fileID, parentID, ref->n, ref->size, TRUE, obj_id);
      TIOassert(irc < 0, ERR_INT, "Failed to initialize variable reference", -10);

      irc = xH5_GetVcache(fileID, parentID, &varcache);
    }

    hobject->group_id = obj_id;

    if (byindex) {
      hobject->varrefindex = (hsize_t)idx;
    }
    else {

      hsize_t i;
      int     found;

      found = FALSE;

      for (i=0; i<ref->n; ++i) {
        if (strcmp(name, ref->buf[i]) == 0) {
          found = TRUE;
          break;
        }
      }

      if (!found) {
        TIOfree (hobject); hobject = NULL;
        TIOreturn(ERR_USER, "Variable with name does not exist", ITIO_ERR_OPEN);
      }

      hobject->varrefindex = (hsize_t)i;
    }

  }
  else {    /*  (class != CLASS_VAR)  */

    hobject->ref      = NULL;
    hobject->varcache = NULL;

    hobject->ref = (struct xH5ref_t *)TIOmalloc(sizeof(struct xH5ref_t) * nrefs);
    if (NULL == hobject->ref) {
      TIOfree (hobject); hobject = NULL;
      TIOreturn(ERR_MEM, "Failed to malloc hobject", -20);
    }

#ifndef NDEBUG
    (void) dNullRef(NULL, hobject, class, name);
#endif

    /* -- Open group */

    hobject->group_id = H5Gopen2(obj_id, name, H5P_DEFAULT);

    if (hobject->group_id < 0) {
      TIOfree (hobject->ref);  hobject->ref = NULL;
      TIOfree (hobject);       hobject      = NULL;
      TIOreturn(ERR_HDF5, "Failed to open object group", ITIO_ERR_OPEN);
    }

  }  /* if (class == CLASS_VAR) */


#ifndef NDEBUG
  (void) strcpy(hobject->name, name);  /* Didn't have name earlier if opened by index */
#endif


  hobject->chunksectionID = NULL;


  /* -- Register the HDF5-specific info */
  irc = iTIO_RegisterFFinfo(TIO_NULL, objectID, (void *)hobject);
  TIOassert(irc < 0, ERR_INT, "Failed to register ffinfo", -10);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_CloseObject
 *
 **************************************************************************************************/
int
iH5_CloseObject( const TIO_File_t   fileID,
                 const TIO_Object_t parentID,
                 const TIO_Object_t objectID,
                 const iClass_t     class )
{
  const char *subname = "iH5_CloseObject()";

  struct xH5object_t *hobject;

  int    irc;
  herr_t hrc;


  /* -- Get ffinfo for actual object group is being closed */
  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);  /* Invalid objectID */
  }
  irc = iTIO_DeregisterFFinfo( TIO_NULL, objectID );
  TIOassert(irc < 0, ERR_INT, "Failed to deregister object ffinfo", -2);


  if (class != CLASS_VAR) {

    /* -- Close group */
    hrc = H5Gclose(hobject->group_id);
    if (hrc < 0) {
      TIOreturn(ERR_HDF5, "Failed to close group", ITIO_ERR_CLOSE);
    }
    TIOfree (hobject->ref); hobject->ref = NULL;

  }      /* (class != CLASS_VAR) */

  if (hobject->chunksectionID != NULL) {
    TIOfree(hobject->chunksectionID); hobject->chunksectionID = NULL;
  }

  TIOfree (hobject); hobject = NULL;


  return (0);
}




/***************************************************************************************************
 *
 * iH5_CreateDataset
 *
 * Return values:
 *   0          Successful
 *   !=0        !Successful
 *
 **************************************************************************************************/
int
iH5_CreateDataset( const TIO_File_t   fileID,
                   const TIO_Object_t parentID,
                   const TIO_Object_t objectID,
                   const iClass_t     class,
                   const char         name[],
                   const int          ndset,
                   const TIO_Dims_t   ndims,
                   const TIO_Size_t   *dims,
                   const TIO_Data_t   datatype )
{
  const char *subname = "iH5_CreateDataset()";

  int irc;



#ifndef NDEBUG
  {
    struct xH5file_t *hfile;

    irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
    TIOassert(irc < 0, ERR_USER, "Invalid fileID", ITIO_ERR_FILEID);
  }

  if (iTIO_ValidDims(ndims, TRUE) < 0) {
    TIOreturn(ERR_INT, "Invalid ndims", ITIO_ERR_DIMS);
  }
  if (iTIO_SizeOf(datatype) == 0) {
    TIOreturn(ERR_INT, "Invalid datatype", ITIO_ERR_DATATYPE);
  }
#endif


  if (CLASS_VAR == class) {

    irc = xH5_CreateVarDataset(fileID, parentID, objectID, name, ndims, dims, datatype);
    TIOassert(irc < 0, ERR_INT, "Failed to create variable dataset", -1);

  }
  else {

    /* Handles mesh chunkinfo, and material nums and names - all 1D only */

    struct xH5object_t *hobject;

    hsize_t hdims[1];
    hid_t   space_id, cplist_id, dtype_id;

    herr_t  hrc;


    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);
    }

    hdims[0] = (hsize_t)dims[0];

    space_id = H5Screate_simple(1, hdims, NULL);
    TIOassert(space_id < 0, ERR_HDF5, "Failed to create dataspace", -11);

    cplist_id = H5Pcreate(H5P_DATASET_CREATE);
    TIOassert(cplist_id < 0, ERR_HDF5, "Failed to create property list", -12);

    hrc =  H5Pset_alloc_time(cplist_id, H5D_ALLOC_TIME_EARLY);
    hrc += H5Pset_fill_time(cplist_id, H5D_FILL_TIME_NEVER);
    TIOassert(hrc != 0, ERR_HDF5, "Failed to set dataset properties", -13);

    dtype_id = xH5_GetH5type(datatype);

    hobject->dset_id[ndset] = H5Dcreate2(hobject->group_id, name,
                                        dtype_id, space_id, H5P_DEFAULT, cplist_id, H5P_DEFAULT);


    if (hobject->dset_id[ndset] < 0) {
      TIOreturn(ERR_HDF5, "Failed to create dataset", ITIO_ERR_CREATE);
    }

    hrc = H5Pclose(cplist_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataset property list", -15);

    hrc = H5Sclose(space_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataspace", -16);
  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_OpenDataset
 *
 **************************************************************************************************/
int
iH5_OpenDataset( const TIO_File_t   fileID,
                 const TIO_Object_t parentID,
                 const TIO_Object_t objectID,
                 const iClass_t     class,
                 const char         name[],
                 const int          ndset,
                 TIO_Dims_t         *ndims,
                 TIO_Size_t         *dims,
                 TIO_Data_t         *datatype )
{
  const char *subname = "iH5_OpenDataset()";

  int irc;


  if (CLASS_VAR == class) {

    irc = xH5_OpenVarDataset(fileID, parentID, objectID, name, ndims, dims, datatype);
    TIOassert(irc < 0, ERR_INT, "Failed to open variable dataset", -1);

  }
  else {

    /* Handles mesh range and chunkinfo, and material nums and names - all 1D only */

    struct xH5object_t *hobject;

    hsize_t hdims[1];
    hid_t   space_id;

    herr_t  hrc;


    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID); /* Invalid objectID */
    }

    /* Datasets are sometimes opened speculatively - eg. mesh range datasets
       so don't issue a hard TIOreturn afterwards in-case of fatal errors enabled */

    hobject->dset_id[ndset] = H5Dopen(hobject->group_id, name, H5P_DEFAULT);

    if (hobject->dset_id[ndset] < 0) {
      return(ITIO_ERR_OPEN);
    }

    space_id = H5Dget_space(hobject->dset_id[ndset]);
    TIOassert(space_id < 0, ERR_HDF5, "Failed to get dataspace", -2);

    irc = H5Sget_simple_extent_dims(space_id, hdims, NULL);
    TIOassert(irc < 0, ERR_HDF5, "Failed to get dataspace extent", -3);

    hrc = H5Sclose(space_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataspace", -4);


    TIOset (ndims, TIO_1D);

    if (dims != NULL) {
      dims[0] = (TIO_Size_t)hdims[0];
    }

    /* simply returning datatype as null value, rather than working it out.
       The calling routine will already have found out what datatype the object is from the info
       attributes */

    TIOset (datatype, TIO_DATATYPE_NULL);
    /* = xH5_GetTIOType(dtype_id = H5Dget_type(hobject->dset_id[ndset]) ); */

  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_CloseDataset
 *
 **************************************************************************************************/
int
iH5_CloseDataset( const TIO_File_t   fileID,
                  const TIO_Object_t parentID,
                  const TIO_Object_t objectID,
                  const iClass_t     class,
                  const int          ndset )
{
  const char *subname = "iH5_CloseDataset()";

  struct xH5object_t *hobject;
  herr_t hrc;
  int    irc;



  if (CLASS_VAR == class) {

    irc = xH5_CloseVarDataset(fileID, parentID, objectID);
    TIOassert(irc < 0, ERR_INT, "Failed to close variable dataset", -1);

  }
  else {   /* (class != CLASS_VAR) */

    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);  /* Invalid objectID */
    }

    hrc = H5Dclose(hobject->dset_id[ndset]);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataset", ITIO_ERR_CLOSE);

  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_WriteDataset
 *
 **************************************************************************************************/
int
iH5_WriteDataset( const TIO_File_t   fileID,
                  const TIO_Object_t parentID,
                  const TIO_Object_t objectID,
                  const iClass_t     class,
                  const int          ndset,
                  const TIO_Xfer_t   xfer,
                  const TIO_Data_t   datatype,
                  const void         *data    )
{
  const char *subname = "iH5_WriteDataset()";

  int irc;


  /* convert on the fly if needed */

  if (CLASS_VAR == class) {

    irc = xH5_WriteVarDataset(fileID, parentID, objectID, datatype, data);
    TIOassert(irc < 0, ERR_INT, "Failed to read var dataset", TIO_ERR_INT);

  }
  else {    /* (class != CLASS_VAR) */

    struct xH5file_t   *hfile;
    struct xH5object_t *hobject;

    hid_t  xfer_plist, dtype_id;
    herr_t hrc;


    irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
    TIOassert(irc < 0, ERR_USER, "Failed to get file ffinfo", ITIO_ERR_FILEID);

    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);

    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Failed to get dtype_id", ITIO_ERR_DATATYPE);

    xfer_plist = xH5_GetXfer(hfile, xfer);
    TIOassert(xfer_plist < 0, ERR_INT, "Failed to get xfer_plist", ITIO_ERR_ARG);

    hrc = H5Dwrite(hobject->dset_id[ndset], dtype_id, H5S_ALL, H5S_ALL, xfer_plist, data);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to write dataset", -1);

  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_ReadDataset
 *
 **************************************************************************************************/
int
iH5_ReadDataset( const TIO_File_t   fileID,
                 const TIO_Object_t parentID,
                 const TIO_Object_t objectID,
                 const iClass_t     class,
                 const int          ndset,
                 const TIO_Xfer_t   xfer,
                 const TIO_Data_t   datatype,
                 void               *data    )
{
  const char *subname = "iH5_ReadDataset()";

  int irc;


  if (CLASS_VAR == class) {

    irc = xH5_ReadVarDataset(fileID, parentID, objectID, datatype, data);
    if (irc < 0) {
      return(irc);
    }
  }
  else {   /* (class != CLASS_VAR) */

    struct xH5file_t   *hfile;
    struct xH5object_t *hobject;

    hid_t  xfer_plist, dtype_id;
    herr_t hrc;


    irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
    TIOassert(irc < 0, ERR_USER, "Failed to get file ffinfo", ITIO_ERR_FILEID);

    irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
    TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);

    xfer_plist = xH5_GetXfer(hfile, xfer);
    TIOassert(xfer_plist < 0, ERR_INT, "Failed to get xfer_plist", ITIO_ERR_ARG);

    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Failed to get dtype_id", ITIO_ERR_DATATYPE);

    hrc = H5Dread(hobject->dset_id[ndset], dtype_id, H5S_ALL, H5S_ALL, xfer_plist, data);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to read dataset", -1);
  }

  return (0);
}



/***************************************************************************************************
 *
 * iH5_ReadDatasetSlab
 *
 **************************************************************************************************/
int
iH5_ReadDatasetSlab ( const TIO_File_t   fileID,
                      const TIO_Object_t parentID,
                      const TIO_Object_t objectID,
                      const iClass_t     class,
                      const int          ndset,
                      const TIO_Xfer_t   xfer,
                      const TIO_Data_t   datatype,
                      const TIO_Dims_t   ndims,
                      const TIO_Size_t   slabinfo[TIO_MAXDIMS][2],
                      void               *data    )
{
  const char *subname = "iH5_ReadDatasetSlab()";

  struct xH5file_t   *hfile;
  struct xH5object_t *hobject;

  hid_t   dset_id, xfer_plist, filsp_id, memsp_id;
  hsize_t offset[TIO_MAXDIMS];
  hsize_t nblock[TIO_MAXDIMS];
  hsize_t bksize[TIO_MAXDIMS];

  int    dim, rdim;

  int    irc;
  herr_t hrc;



  if (class != CLASS_VAR) {
    TIOreturn (ERR_INT, "Can only read section of variable object", ITIO_ERR_CLASS);
  }


  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Failed to get file ffinfo", ITIO_ERR_FILEID);

  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
  TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);


  if (hobject->vardataindex > 0) {
    /* Variable is 0D */
    TIOreturn (ERR_INT, "Variable is scalar - can't read section", ITIO_ERR_READ);
  }

  xfer_plist = xH5_GetXfer(hfile, xfer);
  TIOassert(xfer_plist < 0, ERR_INT, "Failed to get xfer_plist", ITIO_ERR_ARG);

  dset_id  = hobject->dset_id[ndset];
  filsp_id = H5Dget_space(dset_id);
  TIOassert(filsp_id < 0, ERR_HDF5, "Failed to get file dataspace", -2);

  /*
    dim = H5Sget_simple_extent_dims(filsp_id, nblock, NULL);
    TIOassert (dim < 0, ERR_HDF5, "Failed to get dataspace extents", -3);
  */

  for (dim=0, rdim=(ndims-1); dim<ndims; ++dim, --rdim) {

    bksize[rdim] = (hsize_t)(slabinfo[dim][1] - slabinfo[dim][0] + 1);

    /*
    if (bksize[rdim] > nblock[rdim]) {

      hrc = H5Sclose(filsp_id);
      TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -10);

      TIOreturn(ERR_USER, "Range supplied is > dataset", iTIO_ERR_DIMS)
    }
    */

    nblock[rdim] = (hsize_t)1;
    offset[rdim] = (hsize_t)slabinfo[dim][0];
  }

  memsp_id = H5Screate_simple(ndims, bksize, bksize);
  TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create memory dataspace", -4);

  hrc = H5Sselect_hyperslab(filsp_id, H5S_SELECT_SET, offset, NULL, nblock, bksize);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to set hyperslab", -5);

  hrc = H5Dread(dset_id, xH5_GetH5type(datatype), memsp_id, filsp_id, xfer_plist, data);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to read dataset", -6);


  /* -- Close dataspaces*/

  hrc = H5Sclose(memsp_id);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -7);

  hrc = H5Sclose(filsp_id);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -8);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_CreateChunkedDataset
 *
 * Return values:
 *   0          Successful
 *   !=0        !Successful
 *
 **************************************************************************************************/
int
iH5_CreateChunkedDataset( const TIO_File_t      fileID,
                          const TIO_Object_t    parentID,
                          const TIO_Object_t    objectID,
                          const iClass_t        class,
                          const char            name[],
                          const int             ndset,
                          const TIO_Data_t      datatype,
                          const TIO_Mesh_t      meshtype,
                          const TIO_Dims_t      ndims,
                          const TIO_Size_t      nghosts,
                          const TIO_Size_t      nchunks,
                          const union iInfo_t   *chunkinfo,
                          const union iInfo_t   *allinfo,
                          const iDataPos_t      datapos )
{
  const char *subname = "iH5_CreateChunkedDataset()";

  struct xH5object_t *hobject;

  hid_t   space_id, cplist_id, dtype_id;
  herr_t  hrc;
  int     irc;
  hsize_t chunkdims[1] = {1};  
  int     itype;
  size_t  isize;
  size_t  maxchunksize = 4294967296;  /* 4GB chunk limit for hdf5 */

  isize = iTIO_SizeOf(datatype);
#ifndef NDEBUG
  if (0 == isize) {
    TIOreturn(ERR_INT, "Invalid datatype", ITIO_ERR_DATATYPE);
  }
#endif


  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);
  }

  if (0 == ndset) {
    switch (meshtype) {
    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR:
      switch (ndims) {
      case TIO_3D:
        hobject->nk = allinfo->quad.nk;
      case TIO_2D:
        hobject->nj = allinfo->quad.nj;
      case TIO_1D:
        hobject->ni = allinfo->quad.ni;
      default:
        break;
      }
      /* drop through */

    case TIO_MESH_UNSTRUCT:
    case TIO_MESH_POINT:
      hobject->nghosts = nghosts;
      break;

    default:
      TIOreturn(ERR_INT, "Invalid meshtype", ITIO_ERR_MESHTYPE);
      break;
    }
  }

  irc = xH5_GetChunkedSpace( hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims, nghosts,
                             nchunks, chunkinfo,  FALSE, FALSE, nchunks, NULL, &space_id );
  TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -1);

  /* Possible that the size of the dataset is zero (eg - a material created as mixed, but just
     happens to be all pure, say, at t=0
     If this is the case then set the dset_id to 0 as a flag that no dataset exists*/

  if (irc > 0) {
    hobject->dset_id[ndset] = 0;
    return (0);
  }
  TIOassert(irc < 0, ERR_INT, "Failed to get space", -2);


  cplist_id = H5Pcreate(H5P_DATASET_CREATE);
  TIOassert(cplist_id < 0, ERR_HDF5, "Failed to create property list", -3);

#ifdef _ENABLE_HDF5_CHUNK

  hrc  = H5Pset_layout(cplist_id, H5D_CHUNKED);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to set dataset layout property", -4);

  irc = H5Sget_simple_extent_dims(space_id, NULL, chunkdims);
  TIOassert(irc < 0, ERR_HDF5, "Failed to get dataspace extent", -3);

  isize = iTIO_SizeOf(datatype);
  if (chunkdims[0]*isize > maxchunksize){
     chunkdims[0] = chunkdims[0]/nchunks;
  }
  hrc  = H5Pset_chunk(cplist_id, 1, chunkdims);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to set dataset chunk property", -5);

#endif

  hrc =  H5Pset_alloc_time(cplist_id, H5D_ALLOC_TIME_EARLY);
  hrc += H5Pset_fill_time(cplist_id, H5D_FILL_TIME_NEVER);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to set dataset creation properties", -6);

  dtype_id = xH5_GetH5type(datatype);

  hobject->dset_id[ndset] = H5Dcreate(hobject->group_id, name, dtype_id, 
		   space_id, H5P_DEFAULT, cplist_id, H5P_DEFAULT);

  if (hobject->dset_id[ndset] < 0) {
    TIOreturn(ERR_HDF5, "Failed to create dataset", ITIO_ERR_CREATE);
  }

  hrc = H5Pclose(cplist_id);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataset property list", -7);

  hrc = H5Sclose(space_id);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataspace", -8);


  return (0);
}




/***************************************************************************************************
 *
 * iH5_OpenChunkedDataset
 *
 **************************************************************************************************/
int
iH5_OpenChunkedDataset( const TIO_File_t      fileID,
                        const TIO_Object_t    parentID,
                        const TIO_Object_t    objectID,
                        const iClass_t        class,
                        const char            name[],
                        const int             ndset,
                        const TIO_Data_t      datatype,
                        const TIO_Mesh_t      meshtype,
                        const TIO_Dims_t      ndims,
                        const TIO_Size_t      nghosts,
                        const TIO_Size_t      nchunks,
                        const union iInfo_t   *chunkinfo,
                        const union iInfo_t   *allinfo,
                        iDataPos_t            datapos )
{
  const char *subname = "iH5_OpenChunkedDataset()";

  struct xH5object_t *hobject;

  int     irc;


  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);
  }

  if (0 == ndset) {
    switch (meshtype) {
    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR:
      switch (ndims) {
      case TIO_3D:
        hobject->nk = allinfo->quad.nk;
      case TIO_2D:
        hobject->nj = allinfo->quad.nj;
      case TIO_1D:
        hobject->ni = allinfo->quad.ni;
      default:
        break;
      }
      /* drop through */

    case TIO_MESH_UNSTRUCT:
    case TIO_MESH_POINT:
      hobject->nghosts = nghosts;
      break;

    default:
      TIOreturn(ERR_INT, "Invalid meshtype", ITIO_ERR_MESHTYPE);
      break;
    }
  }


  /* idx == nchunks -> Calculates full space of dataset in the file for later use */
  irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims, nghosts,
                            nchunks, chunkinfo,  FALSE, FALSE, nchunks, NULL, NULL);
  TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -1);

  /* No dataset was written if it would've had zero size */
  if (irc > 0) {
    hobject->dset_id[ndset] = 0;
    return (0);
  }
  TIOassert(irc < 0, ERR_INT, "Failed to get space", -2);

  hobject->dset_id[ndset] = H5Dopen(hobject->group_id, name, H5P_DEFAULT);

  if (hobject->dset_id[ndset] < 0) {
    TIOreturn(ERR_HDF5, "Failed to open dataset", ITIO_ERR_OPEN);
  }

  return (0);
}




/***************************************************************************************************
 *
 * iH5_CloseChunkedDataset
 *
 **************************************************************************************************/
int
iH5_CloseChunkedDataset( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const TIO_Object_t objectID,
                         const iClass_t     class,
                         const int          ndset )
{
  const char *subname = "iH5_CloseChunkedDataset()";

  struct xH5file_t   *hfile;
  struct xH5object_t *hobject;

  int    irc;
  herr_t hrc;

  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Failed to get file ffinfo", ITIO_ERR_FILEID);

  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
  TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);

  if (hobject->dset_id[ndset] > 0) {
    hrc = H5Dclose(hobject->dset_id[ndset]);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close dataset", -1);
  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_WriteChunkedDataset
 *
 **************************************************************************************************/
int
iH5_WriteChunkedDataset( const TIO_File_t      fileID,
                         const TIO_Object_t    parentID,
                         const TIO_Object_t    objectID,
                         const iClass_t        class,
                         const int             ndset,
                         const TIO_Xfer_t      xfer,
                         const TIO_Data_t      datatype,
                         const TIO_Mesh_t      meshtype,
                         const TIO_Dims_t      ndims,
                         const TIO_Size_t      nghosts,
                         const TIO_Size_t      nchunks,
                         const TIO_Bool_t      writeall,
                         const union iInfo_t   *chunkinfo,
                         const iDataPos_t      datapos,
                         const TIO_Size_t      chunkidx,
                         const void            *data )
{
  const char *subname = "iH5_WriteChunkedDataset()";

  struct xH5file_t   *hfile;
  struct xH5object_t *hobject;

  hid_t   dset_id, filsp_id, memsp_id, dtype_id, xfer_plist;
  herr_t  hrc;

  int irc, lrc;
  int nodata;

  const void *dataptr;


  if ((chunkidx >= nchunks) && (  TIO_FALSE == writeall ))  {
    TIOreturn(ERR_USER, "Invalid idx value - greater than number in mesh", ITIO_ERR_INDEX);
  }

  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Failed to get file ffinfo", ITIO_ERR_FILEID);

  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
  TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);


  dset_id = hobject->dset_id[ndset];
  if (0 == dset_id) {
    return (0);   /* globally a zero-sized dataset -> nothing to write at all */
  }

  if (NULL == data) {
    nodata  = TRUE;
    dataptr = (const void *)&irc;  /* dummy address - HDF5 can't handle NULL for data arg */
    lrc     = 1;                   /* Return >0 - so caller can flag if nothing actually written */

    dtype_id = H5T_NATIVE_INT;      /* dummy datatype - HDF5 can't handle a NULL one */
  }
  else {
    nodata  = FALSE;
    dataptr = data;
    lrc     = 0;

    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Failed to get dtype_id", ITIO_ERR_DATATYPE);
  }



#ifdef _TYPHIO_PARALLEL

  if (TIO_XFER_COLLECTIVE_RR == xfer ) {

    const int masterp = 0;
    int p;
    int mpisize;
    int mpierr;

    MPI_Status status;

    TIO_Size_t pchunkidx;
    hsize_t    pcount;

    int pnodata;
    int psize;
    const void *mdptr;
    void *temp  = NULL;

    TIO_Size_t rrchunk[2];


    xfer_plist = xH5_GetXfer(hfile, TIO_XFER_INDEPENDENT);

    mpierr = MPI_Comm_size(hfile->comm, &mpisize);

    if (masterp == hfile->rank) {

      /* -- Master writer */

      /* Write incoming data in turn from each process */
      for (p=0; p<mpisize; ++p) {

        if (p == masterp) {

          /* Write data from self */

          pchunkidx = chunkidx;
          pnodata   = nodata;
          mdptr     = dataptr;
          temp      = NULL;

        }
        else {

          /* Recv chunkidx */
          mpierr = MPI_Recv(rrchunk, (sizeof(TIO_Size_t) * 2), MPI_BYTE, p, 111, hfile->comm, &status);

          pchunkidx = rrchunk[0];
          pnodata   = (rrchunk[1] == 1);

          /* Get size of the incoming chunk and allocate temp buffer for it */
          irc = xH5_GetCount( meshtype, datapos, ndims, nghosts, nchunks, chunkinfo, 
                              pchunkidx, &pcount );

          if (!pnodata && (pcount > 0)  ) {
            psize = (int)pcount * (int)iTIO_SizeOf(datatype);
            temp = (void *)TIOmalloc(psize);
            mdptr = (const void *)temp;

            /* Recv chunk data */
            mpierr = MPI_Recv(temp, psize, MPI_BYTE, p, 222, hfile->comm, &status);
          }
        }

        /* Get space for chunk within dataset in file */
        irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims, nghosts,
                                  nchunks, chunkinfo,  FALSE, pnodata, pchunkidx, NULL, &filsp_id);
        TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -1);

        /* Get space for this chunk within memory */
        irc = xH5_GetChunkedSpace(hobject, SPACE_MEMORY, class, meshtype, datapos, ndset, ndims, nghosts,
                                  nchunks, chunkinfo,  FALSE, pnodata, pchunkidx, NULL, &memsp_id);
        TIOassert(irc < 0, ERR_INT, "Failed to get chunked memory dataspace", -2);

        if (pnodata) {
          mdptr = (const void *)&irc;
        }

        hrc = H5Dwrite(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, mdptr);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to write dataset", -ITIO_ERR_WRITE);

        if (NULL != temp) {
          TIOfree(temp);
          temp = NULL;
        }

      }

    }
    else {

      /* -- Slave sender */

      rrchunk[0] = chunkidx;
      if (nodata)
        rrchunk[1] = 1;
      else
        rrchunk[1] = 0;

      /* Send chunk idx - block on sends to ensure master is ready to write */
      mpierr = MPI_Ssend((void *)rrchunk, (sizeof(TIO_Size_t) * 2), MPI_BYTE, masterp, 111, hfile->comm);

      irc = xH5_GetCount( meshtype, datapos, ndims, nghosts, nchunks, chunkinfo, 
                          chunkidx, &pcount );

      if (!nodata && (pcount > 0)) {

        psize = (int)pcount * (int)iTIO_SizeOf(datatype);

        /* Send data */
        mpierr = MPI_Ssend((void *)dataptr, psize, MPI_BYTE, masterp, 222, hfile->comm);
      }
    }


  }
  else

#endif

  {
    /* Normal collective or independent I/O */

    xfer_plist = xH5_GetXfer(hfile, xfer);
    TIOassert(xfer_plist < 0, ERR_INT, "Failed to get xfer_plist", ITIO_ERR_ARG);

    /* Get space for chunk within dataset in file */
    irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims, nghosts,
                              nchunks, chunkinfo, FALSE, nodata, chunkidx, NULL, &filsp_id);
    TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -1);

    /* Get space for this chunk within memory */
    irc = xH5_GetChunkedSpace(hobject, SPACE_MEMORY, class, meshtype, datapos, ndset, ndims, nghosts,
                              nchunks, chunkinfo, FALSE, nodata, chunkidx, NULL, &memsp_id);
    TIOassert(irc < 0, ERR_INT, "Failed to get chunked memory dataspace", -2);


    hrc = H5Dwrite(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, dataptr);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to write dataset", -ITIO_ERR_WRITE);

    hrc = H5Sclose(memsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -3);

    hrc = H5Sclose(filsp_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -4);
  }

  return (lrc);
}




/***************************************************************************************************
 *
 * iH5_ReadChunkedDataset
 *
 * data arg can be NULL
 *
 **************************************************************************************************/
int
iH5_ReadChunkedDataset( const TIO_File_t      fileID,
                        const TIO_Object_t    parentID,
                        const TIO_Object_t    objectID,
                        const iClass_t        class,
                        const int             ndset,
                        const TIO_Xfer_t      xfer,
                        const TIO_Data_t      datatype,
                        const TIO_Mesh_t      meshtype,
                        const TIO_Dims_t      ndims,
                        const TIO_Size_t      nghosts,
                        const TIO_Size_t      nchunks,
                        const union iInfo_t   *chunkinfo,
                        const iDataPos_t      datapos,
                        const TIO_Size_t      chunkidx,
                        TIO_Size_t            *datasize,
                        void                  *data )
{
  const char *subname = "iH5_ReadChunkedDataset()";

  struct xH5file_t   *hfile;
  struct xH5object_t *hobject;

  hid_t   dset_id, filsp_id, memsp_id, dtype_id, xfer_plist;
  herr_t  hrc;
  herr_t  trc;
 
  int irc;
  int nodata;

  void *dataptr;


  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Failed to get file ffinfo", ITIO_ERR_FILEID);

  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
  TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);


  if (TIO_GHOSTS_INTERNAL == nghosts) {

    /*
       Ultimately want to have this so it'll read the ghosts from the chunk, if available, and
       reconstruct from other chunks only if necessary, ie. only if the object has no ghosts

       If chunks are available, should calculate the dataspace and store it like for any other
       chunk reading case, so it can be re-used for other objects

       Could also reconstruct like a section and store the dataspace for the section, like for a
       chunk, rather than using the ReadChunkedDatasetSection routine
    */


    /* For now, just to make things easier, read the internal ghosts thing by contructing a
       section */


    struct xH5object_t *hmesh;
    TIO_Size_t il, ih, jl, jh, kl, kh, ii;
    TIO_Object_t lmeshID, lsectID;
    union iSection_t section;

    TIO_t trc;


    switch (class) {
    case CLASS_MESH:
      hmesh   = hobject;
      lmeshID = objectID;
      break;
    case CLASS_QUANT:
    case CLASS_MATERIAL:
      irc = iTIO_GetFFinfo( 0, parentID, (void **)&hmesh);
      if (irc < 0) {
        TIOreturn(ERR_INT, "Failed to get mesh ffinfo", ITIO_ERR_PARENTID);
      }
      lmeshID = parentID;
      break;
    default:
      TIOreturn(ERR_INT, "Invalid class", ITIO_ERR_CLASS);
      break;
    }


    if (NULL == hmesh->chunksectionID) {
      hmesh->chunksectionID = TIOmalloc(sizeof(TIO_Object_t) * nchunks);
      for (ii=0; ii<nchunks; ++ii) {
        hmesh->chunksectionID[ii] = TIO_NULL;
      }
    }


    switch (meshtype) {

    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR:

      il = chunkinfo[chunkidx].quad.il;
      ih = chunkinfo[chunkidx].quad.ih;
      jl = chunkinfo[chunkidx].quad.jl;
      jh = chunkinfo[chunkidx].quad.jh;
      kl = chunkinfo[chunkidx].quad.kl;
      kh = chunkinfo[chunkidx].quad.kh;

      if (il > 0) --il;
      if (jl > 0) --jl;
      if (kl > 0) --kl;
      if (ih < (hobject->ni - 1)) ++ih;
      if (jh < (hobject->nj - 1)) ++jh;
      if (kh < (hobject->nk - 1)) ++kh;

      irc = iTIO_SetSection(meshtype, ndims, il, ih, jl, jh, kl, kh, TIO_NULL, NULL, &section);
      TIOassert(irc < 0, ERR_INT, "Failed to set section for internal ghosts", -1);

      if (TIO_NULL == hmesh->chunksectionID[chunkidx]) {
        trc = cTIO_SetSection(subname, fileID, lmeshID, &lsectID, meshtype, ndims, &section);
        hmesh->chunksectionID[chunkidx] = lsectID;
      }
      else {
        lsectID = hmesh->chunksectionID[chunkidx];
      }

      irc = iH5_ReadChunkedDatasetSection( fileID, parentID, objectID, lsectID,
                                           class, ndset, xfer, datatype,
                                           meshtype, ndims, nchunks, chunkinfo, &section, 
                                           datapos, datasize, data );
      TIOassert(irc < 0, ERR_INT, "Failed to read dataset section for internal ghosts", -2);

      break;

    case TIO_MESH_UNSTRUCT:
      /* Need to construct ghost node IDs for unstruct and point */
      return (-666);

    case TIO_MESH_POINT:
      TIOreturn(ERR_USER, "Cannot read internal ghosts for point mesh", ITIO_ERR_MESHTYPE);
      break;

    default:
      TIOreturn(ERR_INT, "Invalid meshtype", ITIO_ERR_MESHTYPE);
    }

    return (0);
  }

  /* else if not internal ghosts ... */


  dset_id = hobject->dset_id[ndset];
  if (0 == dset_id) {
      /* zero-sized dataset - globally nothing to read */
    TIOset(datasize, 0);
    return (0);
  }

  xfer_plist = xH5_GetXfer(hfile, xfer);
  TIOassert(xfer_plist < 0, ERR_INT, "Failed to get xfer_plist", ITIO_ERR_ARG);


  nodata = (NULL == data);

  if (nodata) {
    dataptr  = (void *)&irc;         /* dummy address - HDF5 1.6.x can't handle NULL for data arg */
    dtype_id = H5Dget_type(dset_id); /* dummy datatype - HDF5 can't handle a NULL one and using
                                        the one in the dataset ensures no conversion problems     */
  }
  else {
    dataptr  = data;
    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Failed to get dtype_id", ITIO_ERR_DATATYPE);
  }


  /* Get space for chunk within dataset in file */
  irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims, nghosts,
                            nchunks, chunkinfo,  TRUE, nodata, chunkidx, NULL, &filsp_id);
  TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -10);

  /* Get space for this chunk in memory */
  irc = xH5_GetChunkedSpace(hobject, SPACE_MEMORY, class, meshtype, datapos, ndset, ndims, nghosts,
                            nchunks, chunkinfo,  TRUE,  nodata, chunkidx, datasize, &memsp_id);
  TIOassert(irc < 0, ERR_INT, "Failed to get chunked memory dataspace", -11);


  /* dataspaces will have been set to perform zero-length read if no data */
  trc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, dataptr);
  if (nodata) {
    hrc = H5Tclose(dtype_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close dummy dataspace", -14);
  }
  TIOassert(trc < 0, ERR_HDF5, "Failed to read dataset", -ITIO_ERR_READ);

  hrc = H5Sclose(memsp_id);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -12);

  hrc = H5Sclose(filsp_id);
  TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -13);


  return (0);
}



/***************************************************************************************************
 *
 * iH5_ReadChunkedDatasetSection
 *
 * data arg can be NULL
 *
 **************************************************************************************************/
int
iH5_ReadChunkedDatasetSection( const TIO_File_t       fileID,
                               const TIO_Object_t     parentID,
                               const TIO_Object_t     objectID,
                               const TIO_Object_t     sectionID,
                               const iClass_t         class,
                               const int              ndset,
                               const TIO_Xfer_t       xfer,
                               const TIO_Data_t       datatype,
                               const TIO_Mesh_t       meshtype,
                               const TIO_Dims_t       ndims,
                               const TIO_Size_t       nchunks,
                               const union iInfo_t    *chunkinfo,
                               const union iSection_t *sectioninfo,
                               const iDataPos_t       datapos,
                               TIO_Size_t             *datasize,
                               void                   *data )
{
  const char *subname = "iH5_ReadChunkedDatasetSection()";

  struct xH5file_t          *hfile;
  struct xH5object_t        *hobject;
  struct xH5object_t        *hmesh;
  struct xH5section_t       *hsection;
  struct xH5sectionlookup_t *hsectlookup;

  void *tempdata;

  hid_t dset_id, dtype_id, xfer_plist;
  hid_t memsp_id, filsp_id;

  TIO_Size_t mixcompcount, mixcellcount, *mcptr;


  TIO_Size_t ichunk, gchunk, chunkcount, gchunkcount;
  TIO_Size_t fnghosts;
  TIO_Dims_t di, dndims;
  size_t  dtsize;
  hsize_t memsize[3];

  int nodata, collective, allnodata;
  int irc;

  herr_t hrc;


  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Failed to get file ffinfo", ITIO_ERR_FILEID);

  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
  TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);

  switch (class) {
  case CLASS_MESH:
    hmesh = hobject;
    break;
  case CLASS_QUANT:
  case CLASS_MATERIAL:
    irc = iTIO_GetFFinfo( 0, parentID, (void **)&hmesh);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to get mesh ffinfo", ITIO_ERR_PARENTID);
    }
    break;
  default:
    TIOreturn(ERR_INT, "Invalid class", ITIO_ERR_CLASS);
    break;
  }

  irc = iTIO_GetFFinfo( 0, sectionID, (void **)&hsection );
  TIOassert(irc < 0, ERR_INT, "Failed to get section ffinfo", ITIO_ERR_OBJECTID);


  dset_id = hobject->dset_id[ndset];
  if (0 == dset_id) {
    /* zero-sized dataset - globally nothing to read */
    TIOset(datasize, 0);
    return (0);
  }


  collective = (TIO_XFER_COLLECTIVE == xfer);
  xfer_plist = xH5_GetXfer(hfile, xfer);
  TIOassert(xfer_plist < 0, ERR_INT, "Failed to get xfer_plist", ITIO_ERR_ARG);

  nodata = (NULL == data);

  if (collective) {
#ifdef _TYPHIO_PARALLEL
    irc = MPI_Allreduce(&nodata, &allnodata, 1, MPI_INT, MPI_LAND, hfile->comm);

#else
    allnodata = nodata;
#endif
  }
  else {
    allnodata = nodata;
  }

  if (!nodata) {
    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Failed to get H5 datatype", ITIO_ERR_DATATYPE);

    dtsize = H5Tget_size(dtype_id);
  }
  else {
    dtype_id = xH5_GetH5type(TIO_INT);   /* Never gets used in anger - just need a valid value. */
    dtsize   = 0;
  }


  /* Handle special case of colinear quadmesh section */

  if ((CLASS_MESH == class) && (TIO_MESH_QUAD_COLINEAR == meshtype)) {
    return ( xH5_ReadQuadMeshSection( ndset, dset_id, dtype_id, xfer_plist,
                                      nodata, collective, sectioninfo, datasize, data ) );
  }


  /* Otherwise, for all non-colinear quad, unstr and point mesh/mat/quant... */
  /* ... work out which chunks contibute and then read the section data */


  irc = xH5_GetSection( hfile, hobject, hmesh, hsection, meshtype, ndims,
                        nchunks, chunkinfo, sectioninfo, datapos, &hsectlookup, memsize );
  TIOassert(irc < 0, ERR_INT, "Failed to get section", -10);


  if (collective) {
    chunkcount = 0;
    for (ichunk=0; ichunk<nchunks; ++ichunk) {
      if (0 == hsection->chunklog[ichunk]) {    /* Chunk is not part of section */
        continue;
      }
      ++chunkcount;
    }
#ifdef _TYPHIO_PARALLEL
    {
      long long int lcc, gcc;

      lcc = (long long int)chunkcount;

      irc = MPI_Allreduce(&lcc, &gcc, 1, MPI_LONG_LONG, MPI_MAX, hfile->comm);

      gchunkcount = (TIO_Size_t)gcc;
    }
#else
    gchunkcount = chunkcount;
#endif
  }



  /* Handle mixed data - same for quad and unstruct (irrelevant for point) */
  mixcompcount = 0;
  mixcellcount = 0;
  mcptr        = NULL;

  switch (datapos) {
  case DATAPOS_MIXED:

  case DATAPOS_MIXCOMP:
    mcptr = &mixcompcount;
    /* drop through */

  case DATAPOS_MIXCELL:

    TIOset(datasize, (TIO_Size_t)memsize[0]);

    if (NULL == mcptr) {
      mcptr = &mixcellcount;
    }


    if (collective) {

      hsize_t cksize[1], offset[1], dsize[1], nblock[1] = {1};

      if (allnodata) {
        return (0);
      }

      ichunk = 0;

      for (gchunk=0; gchunk<gchunkcount; ++gchunk) {

        int havechunk = 0;

        /* find next contributing chunk for section on this process */
        for (ichunk=ichunk; ichunk<nchunks; ++ichunk) {
          if (0 != hsection->chunklog[ichunk]) {
            havechunk = 1;
            break;
          }
        }

        if (havechunk) {

          irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims,
                                    TIO_GHOSTS_NONE, nchunks, chunkinfo, 
                                    TRUE, nodata, ichunk, cksize, &filsp_id);

          /* routine above will return empty filsp_id if nodata */

          if (irc > 0) {
            /* chunk overlaps section, but it has no mix, so no contribution after all */
            /* filsp_id will return from above call as an empty space */
            memsp_id  = xH5_H5Screate_simple_none();
            TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create empty dataspace", -50);
          }
          else {
            /* create and select mix memory dataspace from the chunk */

            offset[0] = (hsize_t)*mcptr;

            dsize[0]  = offset[0] + cksize[0];

            memsp_id = H5Screate_simple(1, dsize, dsize);
            TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create dataspace", -51);

            hrc = H5Sselect_hyperslab(memsp_id, H5S_SELECT_SET, offset, NULL, nblock, cksize);
            TIOassert(hrc < 0, ERR_HDF5, "Failed to set mix hyperslab", -52);
          }
        }
        else {   /* no chunk = no contribution */
          filsp_id  = xH5_H5Screate_simple_none();
          TIOassert(filsp_id < 0, ERR_HDF5, "Failed to create empty dataspace", -53);
          memsp_id  = xH5_H5Screate_simple_none();
          TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create empty dataspace", -54);
        }

        /* collective read */
        hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to read dataset", -55);

        hrc = H5Sclose(filsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -56);

        hrc = H5Sclose(memsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -57);


        if ((CLASS_MATERIAL == class) && (MAT_PTR == ndset)) {   /* ptr */
          irc = xH5_ConvertMaterial( FALSE, dtype_id, mixcompcount, TIO_1D, &mixcellcount,
                                     cksize, cksize, data );    /* 2nd cksize is dummy */
          TIOassert(irc < 0, ERR_INT, "Failed to convert material", -58);
        }

          if (TIO_MESH_UNSTRUCT == meshtype) {
            mixcellcount += chunkinfo[ichunk].unstr.nmixcell;
            mixcompcount += chunkinfo[ichunk].unstr.nmixcomp;
          }
          else {
            mixcellcount += chunkinfo[ichunk].quad.nmixcell;
           mixcompcount += chunkinfo[ichunk].quad.nmixcomp;
          }


      }
    }

    else {    /* not collective */

      if (nodata) {
        return (0);
      }

      for (ichunk=0; ichunk<nchunks; ++ichunk) {

        hsize_t cksize[1], offset[1], dsize[1], nblock[1] = {1};

        if (0 == hsection->chunklog[ichunk]) {
          continue; /* This chunk is not part of section */
        }

        irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims,
                                  TIO_GHOSTS_NONE, nchunks, chunkinfo, 
                                  TRUE, nodata, ichunk, cksize, &filsp_id);
        TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -11);

        if (irc > 0) continue; /* Chunk is part of section, but has no mix to contribute */

        offset[0] = (hsize_t)*mcptr;


        dsize[0] = offset[0] + cksize[0];

        /* create and select mix memory dataspace */

        memsp_id = H5Screate_simple(1, dsize, dsize);
        TIOassert(memsp_id < 0, ERR_HDF5, "Failed to create dataspace", -12);

        hrc = H5Sselect_hyperslab(memsp_id, H5S_SELECT_SET, offset, NULL, nblock, cksize);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to set mix hyperslab", -13);

        hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, data);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to read mix dataset", -14);

        hrc = H5Sclose(filsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -16);
        hrc = H5Sclose(memsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -17);

        if ((CLASS_MATERIAL == class) && (MAT_PTR == ndset)) {   /* ptr */
          irc = xH5_ConvertMaterial( FALSE, dtype_id, mixcompcount, TIO_1D, &mixcellcount,
                                     cksize, cksize, data );    /* 2nd cksize is dummy */
          TIOassert(irc < 0, ERR_INT, "Failed to convert material", -18);
        }

          if (TIO_MESH_UNSTRUCT == meshtype) {
            mixcellcount += chunkinfo[ichunk].unstr.nmixcell;
            mixcompcount += chunkinfo[ichunk].unstr.nmixcomp;
          }
          else {
            mixcellcount += chunkinfo[ichunk].quad.nmixcell;
            mixcompcount += chunkinfo[ichunk].quad.nmixcomp;
          }


      }
    }

    return (0);

  default:
    break;
  }


  /* For remaining section stuff, which is pure !!!!OR MESH!!!! data, we read the whole chunks and then
     manually sieve out what we don't want.
     Could use HDF5 to do this, but it was found to perform badly! */


  switch (meshtype) {
  case TIO_MESH_QUAD_NONCOLINEAR:
  case TIO_MESH_QUAD_COLINEAR:
    fnghosts = hobject->nghosts;
    dndims = ndims;
    break;
  case TIO_MESH_UNSTRUCT:
  case TIO_MESH_POINT:
    fnghosts = TIO_GHOSTS_NONE;
    dndims = TIO_1D;
    break;
  }

  if (datasize != NULL) {
    for (di=TIO_0D; di<dndims; ++di) {
      datasize[di] = (TIO_Size_t)memsize[di];
    }
  }
  if (allnodata) {
    return (0);
  }


  chunkcount = 0;
  tempdata = NULL;

  {
    TIO_Size_t ms;


    if (collective) {

      ichunk = 0;
      for (gchunk=0; gchunk<gchunkcount; ++gchunk) {

        int havechunk = 0;

        /* find next contributing chunk for section on this process */
        for (ichunk=ichunk; ichunk<nchunks; ++ichunk) {
          if (0 != hsection->chunklog[ichunk]) {
            havechunk = 1;
            break;
          }
        }

        if (havechunk) {

          /* Get space for chunk within dataset in file  - will be empty if nodata */
          irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims,
                                    fnghosts, nchunks, chunkinfo, TRUE,  nodata, ichunk,
                                    NULL, &filsp_id);
          TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -20);

          /* Get space for this chunk in memory   - will be empty if nodata */
          irc = xH5_GetChunkedSpace(hobject, SPACE_MEMORY, class, meshtype, datapos, ndset, ndims,
                                    fnghosts, nchunks, chunkinfo, TRUE, nodata, ichunk,
                                    memsize, &memsp_id);
          TIOassert(irc < 0, ERR_INT, "Failed to get chunked memory dataspace", -21);

          ms = 1;
          for (di=TIO_0D; di<dndims; ++di) {
            ms *= memsize[di];   /* Map n-D array to 1D */
          }

          if (!nodata) {
            tempdata = TIOrealloc(tempdata, ms * dtsize);
            if (NULL == tempdata) {
              TIOreturn(ERR_MEM, "Failed to realloc tempdata", -23);
            }
          }
        }
        else {
          /* no chunk contribution */
          filsp_id = xH5_H5Screate_simple_none();
          memsp_id = xH5_H5Screate_simple_none();
        }

        hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, tempdata);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to read dataset", -ITIO_ERR_READ);

        hrc = H5Sclose(memsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -23);

        hrc = H5Sclose(filsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -24);

        /* traverse the tempdata and sieve out the bits we want for the section using lookup */
        if (!nodata) {

          switch (meshtype) {
          case TIO_MESH_QUAD_NONCOLINEAR:
          case TIO_MESH_QUAD_COLINEAR:

            irc = xH5_SieveQuadData( class, datapos, nchunks, chunkinfo, ichunk, sectioninfo,
                                     ndims, fnghosts, hsectlookup, dtsize,
                                     tempdata, data, &mixcellcount );
            TIOassert(irc < 0, ERR_INT, "Failed to get sieve dataset", -25);

            break;

          case TIO_MESH_UNSTRUCT:
          case TIO_MESH_POINT:

            irc = xH5_SieveNonQuadData( class, meshtype, chunkinfo, ichunk,
                                        hsection->chunklog[ichunk], &chunkcount, hsectlookup, dtsize,
                                        tempdata, data, &mixcellcount );
            TIOassert(irc < 0, ERR_INT, "Failed to get sieve dataset", -26);


            break;

          }
        }
      }
    }

    else if (!nodata) {        /* not collective and has data */

      for (ichunk=0; ichunk<nchunks; ++ichunk) {

        if (0 == hsection->chunklog[ichunk]) continue;

        /* Get space for chunk within dataset in file */
        irc = xH5_GetChunkedSpace(hobject, SPACE_FILE, class, meshtype, datapos, ndset, ndims,
                                  fnghosts, nchunks, chunkinfo, TRUE, nodata, ichunk,
                                  NULL, &filsp_id);
        TIOassert(irc < 0, ERR_INT, "Failed to get chunked file dataspace", -30);

        /* Get space for this chunk in memory */
        irc = xH5_GetChunkedSpace(hobject, SPACE_MEMORY, class, meshtype, datapos, ndset, ndims,
                                  fnghosts, nchunks, chunkinfo, TRUE, nodata, ichunk,
                                  memsize, &memsp_id);
        TIOassert(irc < 0, ERR_INT, "Failed to get chunked memory dataspace", -31);

        ms = 1;
        for (di=TIO_0D; di<dndims; ++di) {
          ms *= memsize[di];   /* Map n-D array to 1D */
        }

        tempdata = TIOrealloc(tempdata, ms * dtsize);
        if (NULL == tempdata) {
          TIOreturn(ERR_MEM, "Failed to realloc tempdata", -32);
        }

        hrc = H5Dread(dset_id, dtype_id, memsp_id, filsp_id, xfer_plist, tempdata);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to read dataset", -ITIO_ERR_READ);

        hrc = H5Sclose(memsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to close memory dataspace", -33);

        hrc = H5Sclose(filsp_id);
        TIOassert(hrc < 0, ERR_HDF5, "Failed to close file dataspace", -34);

        switch (meshtype) {
        case TIO_MESH_QUAD_NONCOLINEAR:
        case TIO_MESH_QUAD_COLINEAR:

          irc = xH5_SieveQuadData( class, datapos, nchunks, chunkinfo, ichunk, sectioninfo,
                                   ndims, hobject->nghosts, hsectlookup, dtsize,
                                   tempdata, data, &mixcellcount );
          TIOassert(irc < 0, ERR_INT, "Failed to get sieve dataset", -35);

          break;

        case TIO_MESH_UNSTRUCT:
        case TIO_MESH_POINT:

          irc = xH5_SieveNonQuadData( class, meshtype, chunkinfo, ichunk,
                                      hsection->chunklog[ichunk], &chunkcount, hsectlookup, dtsize,
                                      tempdata, data, &mixcellcount );
          TIOassert(irc < 0, ERR_INT, "Failed to get sieve dataset", -36);


          break;
        }
      }
    }
  }

  if (tempdata != NULL) {
    TIOfree(tempdata);
    tempdata = NULL;
  }

  return (0);
}












/***************************************************************************************************
 *
 * iH5_ReadChunkedDatasetAll
 *
 * data arg can be NULL
 * restricted so can only use TIO_XFER_INDEPENDENT - would be a nightmare otherwise!
 *
 **************************************************************************************************/
int
iH5_ReadChunkedDatasetAll( const TIO_File_t    fileID,
                           const TIO_Object_t  parentID,
                           const TIO_Object_t  objectID,
                           const iClass_t      class,
                           const int           ndset,
                           const TIO_Xfer_t    xfer,
                           const TIO_Data_t    datatype,
                           const TIO_Mesh_t    meshtype,
                           const TIO_Dims_t    ndims,
                           const TIO_Size_t    nchunks,
                           const union iInfo_t *chunkinfo,
                           const union iInfo_t *allinfo,
                           const iDataPos_t    datapos,
                           TIO_Size_t          *datasize,
                           void                *data )
{
  const char *subname = "iH5_ReadChunkedDatasetAll()";

  struct xH5file_t   *hfile;
  struct xH5object_t *hobject;
  struct xH5object_t *hmesh;

  hid_t   xfer_plist;
  hid_t   dset_id, dtype_id;

  int     irc;



  irc = iTIO_GetFFinfo(fileID, TIO_NULL, (void **)&hfile);
  TIOassert(irc < 0, ERR_USER, "Failed to get file ffinfo", ITIO_ERR_FILEID);

  irc = iTIO_GetFFinfo( 0, objectID, (void **)&hobject );
  TIOassert(irc < 0, ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);


  switch (class) {
  case CLASS_MESH:
    hmesh = hobject;
    break;

  case CLASS_QUANT:
  case CLASS_MATERIAL:
    irc = iTIO_GetFFinfo( 0, parentID, (void **)&hmesh);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to get mesh ffinfo", ITIO_ERR_PARENTID);
    }
    break;
  default:
    TIOreturn(ERR_INT, "Invalid class", ITIO_ERR_CLASS);
    break;
  }


  if (xfer == TIO_XFER_COLLECTIVE) {
    TIOreturn(ERR_INT, "Cannot use TIO_XFER_COLLECTIVE with ReadChunkedDatasetAll", -1);
  }
  xfer_plist = xH5_GetXfer(hfile, xfer);
  if (xfer_plist  < 0) {
    TIOreturn(ERR_USER, "Invalid xfer value", ITIO_ERR_ARG);
  }


  /* If got this far then need to actually read the data */

  dset_id = hobject->dset_id[ndset];

  if (0 == dset_id) {
    /* zero-sized dataset - globally nothing to read */
    TIOset(datasize, 0);
    return (0);
  }

  if (NULL == data) {
    ;
  }
  else {
    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Failed to get dtype_id", ITIO_ERR_DATATYPE);
  }

  switch (meshtype) {

  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    irc = xH5_ReadQuadAll( hobject, hmesh, class, meshtype, datapos, nchunks, chunkinfo, allinfo,
                           ndset, ndims, dset_id, dtype_id, xfer_plist, datasize, data );

    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read all quad data", -101);
    }
    break;

  case TIO_MESH_UNSTRUCT:

    irc = xH5_ReadUnstrAll( hobject, hmesh, class, datapos, nchunks, chunkinfo, allinfo,
                            ndset, dset_id, dtype_id, xfer_plist, datasize, data );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read all unstr data", -102);
    }
    break;

  case TIO_MESH_POINT:

    irc = xH5_ReadPointAll( hobject, hmesh, class, datapos, nchunks, chunkinfo, allinfo,
                            ndset, dset_id, dtype_id, xfer_plist, datasize, data );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to read all point data", -103);
    }
    break;

  default:
    TIOreturn(ERR_INT, "Invalid meshtype", ITIO_ERR_MESHTYPE);
    break;
  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_SetSection  /  iH5_KillSection
 *
 ***************************************************************************************************
 *
 *
 * Return values:
 *   0          Successfully set section
 *   <0         Failed somewhere
 *
 * Independent
 *
 **************************************************************************************************/
int
iH5_SetSection ( const TIO_File_t       fileID,
                 const TIO_Object_t     meshID,
                 const TIO_Object_t     sectionID,
                 const TIO_Mesh_t       meshtype,
                 const TIO_Dims_t       ndims,
                 const union iSection_t *section,
                 const iDataPos_t       datapos )
{
  const char *subname = "iH5_SetSection()";

  struct xH5section_t *hsection;

  const TIO_Size_t defsize = 2;
  TIO_Size_t ii;
  int irc;



  hsection = NULL;
  hsection = (struct xH5section_t *)TIOmalloc(sizeof(struct xH5section_t));
  if (NULL == hsection) {
    TIOreturn(ERR_MEM, "Failed to malloc hsection", -1);
  }

  hsection->lookup = NULL;
  hsection->lookup = TIOmalloc(sizeof(struct xH5sectionlookup_t) * defsize);
  if (NULL == hsection->lookup) {
    TIOreturn(ERR_MEM, "Failed to realloc ref->units", -2);
  }

#ifndef NDEBUG
  for (ii=0; ii<defsize; ++ii) {
    hsection->lookup[ii].datapos = DATAPOS_NULL;
  }
#endif

  hsection->nalloc = defsize;
  hsection->nlist  = 0;

  hsection->chunklog = NULL;

  irc = iTIO_RegisterFFinfo(TIO_NULL, sectionID, (void *)hsection);
  TIOassert(irc < 0, ERR_INT, "Failed to register ffinfo", -3);


  return (0);
}

int
iH5_KillSection ( const TIO_File_t   fileID,
                  const TIO_Object_t meshID,
                  const TIO_Object_t sectionID )
{
  const char *subname = "iH5_KillSection()";

  struct xH5section_t       *hsection;
  struct xH5sectionlookup_t *llookup;

  TIO_Size_t isect;

  int    irc;


  irc = iTIO_GetFFinfo(0, sectionID, (void **)&hsection);
  TIOassert(irc < 0, ERR_USER, "Invalid sectionID", ITIO_ERR_OBJECTID);


  /* Free the lookups and chunklog */

  for (isect=0; isect<hsection->nlist; ++isect) {
    llookup = &hsection->lookup[isect];

    TIOfree(llookup->secthyper); llookup->secthyper = NULL;
    TIOfree(llookup->sectchunk); llookup->sectchunk = NULL;
  }

  TIOfree(hsection->lookup); hsection->lookup = NULL;
  hsection->nalloc = 0;
  hsection->nlist  = 0;

  if (hsection->chunklog != NULL) {
    TIOfree(hsection->chunklog); hsection->chunklog = NULL;
  }

  /* FREE LOOKUPS */

  TIOfree(hsection); hsection = NULL;


  return (0);
}




/***************************************************************************************************
 *
 * iH5_WriteAttr
 *
 ***************************************************************************************************
 *
 *
 * Return values:
 *   0          Successfully wrote HDF5 attribute
 *   <0         Failed somewhere
 *
 * Collective
 *
 **************************************************************************************************/
int
iH5_WriteAttr( const TIO_File_t   fileID,
               const TIO_Object_t parentID,
               const TIO_Object_t objectID,
               const iClass_t     class,
               const char         name[],
               const TIO_Data_t   datatype,
               const void         *data )
{
  const char *subname = "iH5_WriteAttr()";

  int irc;



  if (CLASS_VAR == class) {

    struct xH5varcache_t *vcache;
    struct xH5object_t   *vobject;

    /* Only attribute allowed on variables is "units" string */

    /* Variable units live in reference of parent too */

    irc = xH5_GetVcache(fileID, parentID, &vcache);
    switch (irc) {
    case ITIO_ERR_NONE:
      /* No error */
      break;
    case ITIO_ERR_FILEID:
      TIOreturn(ERR_INT, "Failed to get object varcache - invalid fileID", ITIO_ERR_FILEID);
      break;
    case ITIO_ERR_OBJECTID:
      TIOreturn(ERR_INT, "Failed to get object varcache - invalid parentID", ITIO_ERR_PARENTID);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to get object varcache", -1);
      break;
    }


    TIOassert( (strcmp(name, "units") != 0),
               ERR_INT, "Cannot write attributes other than units to variable object", -10);

    irc = iTIO_GetFFinfo( 0, objectID, (void **)&vobject );
    if (irc < 0)
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);

    (void) iTIO_strcpy(vcache->units[vobject->varrefindex], (char *)data);

  }
  else {      /* (Class != CLASS_VAR) */

    hid_t  obj_id;                                    /* HDF5 ID of file / object              */
    hid_t  dtype_id;                                  /* HDF5 ID of attribute HDF5 datatype    */
    hid_t  space_id;                                  /* HDF5 ID of filespace for attribute    */
    hid_t  attr_id;                                   /* HDF5 ID of the attribute              */
    hid_t  create_plist;                              /* HDF5 attribute creation property list */
    herr_t hrc;                                       /* HDF5 return code                      */


    irc = xH5_GetRef(fileID, objectID, &obj_id, CLASS_NULL, NULL);
    switch (irc) {
    case ITIO_ERR_NONE:
      /* No error */
      break;
    case ITIO_ERR_FILEID:
      TIOreturn(ERR_INT, "Failed to get object ref - invalid fileID", ITIO_ERR_FILEID);
      break;
    case ITIO_ERR_OBJECTID:
      TIOreturn(ERR_INT, "Failed to get object ref - invalid objectID", ITIO_ERR_OBJECTID);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to get object ref", -1);
      break;
    }


    /* -- Get HDF5 datatype corresponding to TyphonIO one given */
    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Invalid datatype", ITIO_ERR_DATATYPE);


    create_plist = H5P_DEFAULT;   /* Currently unused for attribute create, but may be in future */

    /* -- Create the simple scalar space required for the attribute */
    space_id = H5Screate(H5S_SCALAR);
    TIOassert(space_id < 0, ERR_HDF5, "Failed to create attribute space", -2);

    /* -- Create the attribute */

    attr_id  = H5Acreate(obj_id, name, dtype_id, space_id, create_plist, H5P_DEFAULT);

    TIOassert(attr_id < 0, ERR_HDF5, "Failed to create attribute", -3);

    /* -- Write the actual data into file
          PARALLEL - HDF5 dictates that the Master process (MPI rank = 0) writes the data
          All processes need to make this call to 'know' about the attribute data though */
    hrc = H5Awrite(attr_id, dtype_id, data);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to write to attribute", -4);


    /* -- Close the space and attribute */
    hrc =  H5Sclose(space_id);
    hrc += H5Aclose(attr_id);
    TIOassert(hrc != 0, ERR_HDF5, "Failed to close attribute and/or space", -5);

  }

  return (0);
}




/***************************************************************************************************
 *
 * iH5_ReadAttr
 *
 ***************************************************************************************************
 *
 *
 * Return values:
 *   0          Successfully read HDF5 attribute
 *   <0         Failed somewhere
 *
 **************************************************************************************************/
int
iH5_ReadAttr( const TIO_File_t   fileID,
              const TIO_Object_t parentID,
              const TIO_Object_t objectID,
              const iClass_t     class,
              const char         name[],
              const TIO_Data_t   datatype,
              void               *data )
{
  const char *subname = "iH5_ReadAttr()";

  int irc;



  if (CLASS_VAR == class) {

    struct xH5varcache_t *vcache;
    struct xH5object_t   *vobject;

    /* Only attribute allowed on variables is "units" string */

    /* Variable units live in reference of parent too */

    irc = xH5_GetVcache(fileID, parentID, &vcache);
    switch (irc) {
    case ITIO_ERR_NONE:
      /* No error */
      break;
    case ITIO_ERR_FILEID:
      TIOreturn(ERR_INT, "Failed to get object varcache - invalid fileID", ITIO_ERR_FILEID);
      break;
    case ITIO_ERR_OBJECTID:
      TIOreturn(ERR_INT, "Failed to get object varcache - invalid parentID", ITIO_ERR_PARENTID);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to get object varcache", -1);
      break;
    }


    TIOassert( (strcmp(name, "units") != 0),
               ERR_INT, "Cannot read attributes other than units from variable object", -10);

    irc = iTIO_GetFFinfo( 0, objectID, (void **)&vobject );
    if (irc < 0)
      TIOreturn(ERR_INT, "Failed to get object ffinfo", ITIO_ERR_OBJECTID);  /* Invalid objectID */

    (void) iTIO_strcpy((char *)data, vcache->units[vobject->varrefindex]);

  }
  else {

    hid_t  obj_id;                                    /* HDF5 ID of file / object           */
    hid_t  dtype_id;                                  /* HDF5 ID of attribute HDF5 datatype */
    hid_t  attr_id;                                   /* HDF5 ID of the attribute           */

    herr_t hrc;                                       /* HDF5 return code                   */


    irc = xH5_GetRef(fileID, objectID, &obj_id, CLASS_NULL, NULL);
    switch (irc) {
    case ITIO_ERR_NONE:
      /* No error */
      break;
    case ITIO_ERR_FILEID:
      TIOreturn(ERR_INT, "Failed to get object ref - invalid fileID", ITIO_ERR_FILEID);
      break;
    case ITIO_ERR_OBJECTID:
      TIOreturn(ERR_INT, "Failed to get object ref - invalid objectID", ITIO_ERR_OBJECTID);
      break;
    default:
      TIOreturn(ERR_INT, "Failed to get object ref", -1);
      break;
    }


    /* -- Get HDF5 datatype corresponding to TyphonIO one given */
    dtype_id = xH5_GetH5type(datatype);
    TIOassert(dtype_id < 0, ERR_INT, "Invalid datatype", -2);

    /* -- Open the attribute by name */
    attr_id = H5Aopen_name(obj_id, name);
    if (attr_id < 0)
      TIOreturn(ERR_HDF5, "Failed to open attribute", -3);

    /* -- Read the attribute data */
    hrc = H5Aread(attr_id, dtype_id, data);
    if (hrc < 0)
      TIOreturn(ERR_HDF5, "Failed to read attribute", -4);

    /* -- Close the attribute */
    hrc = H5Aclose(attr_id);
    TIOassert(hrc < 0, ERR_HDF5, "Failed to close attribute", -5);

  }


  return (0);
}




/***************************************************************************************************
 *
 * iH5_DelAttr
 *
 ***************************************************************************************************
 *
 *
 * Return values:
 *   0          Successfully initialized HDF5 and associated gubbins
 *   <0         Failed somewhere
 *
 **************************************************************************************************/
int
iH5_DelAttr( const TIO_File_t   fileID,
             const TIO_Object_t parentID,
             const TIO_Object_t objectID,
             const iClass_t     class,
             const char         name[] )
{
  const char *subname = "iH5_DelAttr()";

  hid_t  obj_id;

  int    irc;
  herr_t hrc;



  TIOassert((CLASS_VAR == class), ERR_INT, "Cannot delete attributes from variable", -1);

  irc = xH5_GetRef(fileID, objectID, &obj_id, CLASS_NULL, NULL);
  switch (irc) {
  case ITIO_ERR_NONE:
    /* No error */
    break;
  case ITIO_ERR_FILEID:
    TIOreturn(ERR_INT, "Failed to get object ref - invalid fileID", ITIO_ERR_FILEID);
    break;
  case ITIO_ERR_OBJECTID:
    TIOreturn(ERR_INT, "Failed to get object ref - invalid objectID", ITIO_ERR_OBJECTID);
    break;
  default:
    TIOreturn(ERR_INT, "Failed to get object ref", -2);
    break;
  }


  /* -- Open the attribute by name */
  hrc = H5Adelete(obj_id, name);
  if (hrc < 0)
    TIOreturn(ERR_HDF5, "Failed to delete attribute", -3);


  return (0);
}



/***************************************************************************************************
 *
 * iH5_ConvBuf
 *
 **************************************************************************************************/
int
iH5_ConvBuf( const TIO_Data_t idatatype,
             const TIO_Data_t odatatype,
             const TIO_Size_t nbuf,
             void             *buf )
{
  const char *subname = "iH5_ConvBuf()";

  /* Converts data from idatatype to odatatype in-buffer */

  void   *tempbuf = NULL;
  size_t isize, osize, bsize, tsize;
  hid_t  idtype_id, odtype_id;
  herr_t hrc;


  if (idatatype == odatatype)
    return (0);

  idtype_id = xH5_GetH5type(idatatype);
  odtype_id = xH5_GetH5type(odatatype);

  if (H5Tequal(idtype_id, odtype_id) > 0)
    return (0);

  isize    = iTIO_SizeOf(idatatype);
  TIOassert((0 == isize), ERR_INT, "Invalid idatatype", TIO_ERR_INT);

  osize    = iTIO_SizeOf(odatatype);
  TIOassert((0 == osize), ERR_INT, "Invalid odatatype", TIO_ERR_INT);


  bsize    = isize * nbuf;
  tsize    = TIOmax(isize, osize) * nbuf;
  tempbuf  = TIOmalloc(tsize);

  (void) memcpy(tempbuf, buf, bsize);


  hrc = H5Tconvert(idtype_id, odtype_id, (hsize_t)nbuf, tempbuf, NULL, H5P_DEFAULT);
  if (hrc < 0)
    TIOreturn (ERR_HDF5, "Failed to convert between datatypes", -1);


  bsize = osize * nbuf;

  (void) memcpy(buf, tempbuf, bsize);

  TIOfree (tempbuf); tempbuf = NULL;


  return (0);
}


#endif
/* ifdef _TYPHIO_HDF5 */


/*
 * EOF
 */

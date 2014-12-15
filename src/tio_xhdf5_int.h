/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_XHDF5_INT_H
#define _TIO_XHDF5_INT_H

#include "tio_global_int.h"

#ifdef _TYPHIO_HDF5

#ifdef _H5_USE_16_API
#define H5_USE_16_API
#endif


#include <hdf5.h>



/*
 *  Types:
 */


#define NUM_DATATYPES (14)
/*             = No. possible external datatypes for scalar variables & mat/quant
               := TIO_SHORT to TIO_UCHAR */

#define NUM_DATASETS  (8)
/*             = max no. HDF5 datasets in an object    */



typedef enum { SPACE_FILE   = 101,
               SPACE_MEMORY = 102 } xSpacetype_t;


static const hsize_t UNLIMDIMS[1] = { H5S_UNLIMITED };  /* Maximum dimension extent of dataset */



struct xH5ref_t
{
#ifndef NDEBUG
  char  name[TIO_FILENAME_LEN+20];
#endif

  hsize_t n;
  hsize_t size;
  hid_t   id;
  int     mod;
  char    (*buf)[TIO_STRLEN];
};


struct xH5varcache_t
{
#ifndef NDEBUG
  char  name[TIO_FILENAME_LEN+20];        /* Name of parent file/object for debugging */
#endif

  hid_t      vunits_id;                   /* HDF5 ID of units dataset                 */
  hid_t      vtypes_id;                   /* HDF5 ID of variable types dataset        */
  hid_t      vindex_id;                   /* HDF5 ID of variable index dataset        */

  char       (*units)[TIO_STRLEN];        /* Buffer for all variables' units          */
  int        *types;                      /* Buffer for all variables' types          */
  TIO_Size_t *index;                      /* Buffer for all variables' index
                                             This is where the variable data is within
                                             the cache.  If == 0 then variable is 0D  */

  TIO_Size_t ncache[NUM_DATATYPES];       /* No. vars of each datatype cached globally */
  TIO_Size_t cachesize[NUM_DATATYPES];    /* Size of local cache[i]                    */

  hid_t      vcache_id[NUM_DATATYPES];    /* */
  hid_t      memspace_id[NUM_DATATYPES];  /* */
  hid_t      filspace_id[NUM_DATATYPES];  /* */

  void       *cache[NUM_DATATYPES];       /* Local variable cache per datatype        */
};


struct xH5file_t
{
#ifndef NDEBUG
  char  name[TIO_FILENAME_LEN];
#endif

  hid_t file_id;
  hid_t root_id;
  hid_t indxfer_plist;
  hid_t colxfer_plist;

  MPI_Comm comm;     /* Duplicate of MPI communicator */
  int   rank;        /* Copy of rank in iFile_t for efficiency */

  struct xH5ref_t      *ref;
  struct xH5varcache_t *varcache;
};


struct xH5object_t
{
#ifndef NDEBUG
  char       name[TIO_STRLEN];
  iClass_t   class;
#endif

  TIO_Size_t nghosts;       /* Number of ghosts for the object as stored in the file              */
  TIO_Size_t ni, nj, nk;    /* Maxmimum size if object is quad wrt nodal mesh                     */

  hid_t      group_id;      /* Group container for object - all datasets of object within this    */
  hid_t      dset_id[NUM_DATASETS];         /* Object dataset IDs                                 */

  hsize_t    varrefindex;   /* If variable object, position of variable in reference tables       */
  hssize_t   vardataindex;  /* If variable object, position of any variable data in a cache       */
                            /* - Scalar variables are cached and written as a table
                                 in which case this gives the position of the var data within
                                 the cache, but is 1-index, not 0, so will need to -1 to get
                                 actual lookup entry.
                                 0 value indicates data is still to be written and assigned a
                                 proper index value.
                                 Non-scalar variables (1D or more) should have this set to -1     */

  TIO_Object_t *chunksectionID; /* Per chunk - sectionID for chunk + internal ghosts    */

  struct xH5ref_t      *ref;
  struct xH5varcache_t *varcache;
};


struct xH5sectionlookup_t
{
  iDataPos_t datapos;
  hsize_t    datasize[3];
  TIO_Size_t *secthyper;       /*  = where the data is in the section              */
  TIO_Size_t *sectchunk;       /*  = where the data is in the chunks               */
};                             /*     - both above are IDs for u&p, il:ih,jl:jh,kl:kh for q  */


struct xH5section_t
{
  TIO_Size_t nalloc;
  TIO_Size_t nlist;
  hsize_t    nmixcell;
  hsize_t    nmixcomp;
  struct xH5sectionlookup_t *lookup;
  TIO_Size_t *chunklog;
};



/*
 *  Routines:
 */

extern int     xH5_SetRefNames( void );
extern char *  xH5_GetRefName(  const iClass_t class );
extern hsize_t xH5_GetRefSize(  const iClass_t class );
extern hsize_t xH5_GetRefDelta( const iClass_t class );


extern int   xH5_SetTypes  ( void );
extern int   xH5_FreeTypes ( void );

extern int   xH5_SetXfer   ( struct xH5file_t *hfile );
extern hid_t xH5_GetXfer   ( const struct xH5file_t *hfile,
                             const TIO_Xfer_t       xfer );
extern int   xH5_FreeXfer  ( struct xH5file_t *hfile );

extern hid_t xH5_GetH5type ( const TIO_Data_t   datatype );

extern int   xH5_GetRef(   const TIO_File_t     fileID,
                           const TIO_Object_t   objectID,
                           hid_t                *obj_id,
                           const iClass_t       refclass,
                           struct xH5ref_t      **ref );

extern int   xH5_GetVcache( const TIO_File_t     fileID,
                            const TIO_Object_t   objectID,
                            struct xH5varcache_t **varcache );



extern int   xH5_InitVarCache     ( const TIO_File_t     fileID,
                                    const TIO_Object_t   objectID,
                                    const hsize_t        nvar,
                                    const hsize_t        varrefsize,
                                    const int            reading,
                                    const hid_t          obj_id );

extern int   xH5_FinalizeVarCache ( const TIO_File_t     fileID,
                                    const TIO_Object_t   objectID,
                                    const hsize_t        nvar,
                                    const hsize_t        varrefsize,
                                    const int            mod,
                                    const hid_t          obj_id );


extern int   xH5_CreateVarDataset ( const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t varID,
                                    const char         name[],
                                    const TIO_Dims_t   ndims,
                                    const TIO_Size_t   *dims,
                                    const TIO_Data_t   datatype );

extern int   xH5_OpenVarDataset   ( const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t varID,
                                    const char         name[],
                                    TIO_Dims_t         *ndims,
                                    TIO_Size_t         *dims,
                                    TIO_Data_t         *datatype );

extern int   xH5_CloseVarDataset  ( const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t varID );


extern int   xH5_WriteVarDataset  ( const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t varID,
                                    const TIO_Data_t   datatype,
                                    const void         *data );

extern int   xH5_ReadVarDataset   ( const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t varID,
                                    const TIO_Data_t   datatype,
                                    void               *data );

extern int xH5_ReadQuadAll(  struct xH5object_t  *hobject,
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
                             void                *data );

extern int xH5_ReadUnstrAll( struct xH5object_t  *hobject,
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
                             void                *data );

extern int xH5_ReadPointAll( struct xH5object_t  *hobject,
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
                             void                *data );

extern int xH5_ReadQuadMeshSection( const int              ndset,
                                    const hid_t            dset_id,
                                    const hid_t            dtype_id,
                                    const hid_t            xfer_plist,
                                    const int              nodata,
                                    const int              collective,
                                    const union iSection_t *sectioninfo,
                                    TIO_Size_t             *datasize,
                                    void                   *data );

extern int xH5_ConvertMaterial(  const int          ismat,
                                 const hid_t        dtype_id,
                                 const hsize_t      mixcount,
                                 const TIO_Dims_t   ndims,
                                 const hsize_t      *offset,
                                 const hsize_t      *msize,
                                 const hsize_t      *mstride,
                                 void               *matptr );

extern int xH5_SieveQuadData(    const iClass_t      class,
                                 const iDataPos_t    datapos,
                                 const TIO_Size_t    nchunks,
                                 const union iInfo_t *chunkinfo,
                                 const TIO_Size_t    ichunk,
                                 const union iSection_t *sectioninfo,
                                 const TIO_Dims_t    ndims,
                                 const TIO_Size_t    nghosts,
                                 const struct xH5sectionlookup_t *sectlookup,
                                 const size_t        dtsize,
                                 void                *fulldata,
                                 void                *data,
                                 TIO_Size_t          *mixcellcount );

extern int xH5_SieveNonQuadData( const iClass_t      class,
                                 const TIO_Mesh_t    meshtype,
                                 const union iInfo_t *chunkinfo,
                                 const TIO_Size_t    ichunk,
                                 const TIO_Size_t                nchunklog,
                                 TIO_Size_t                      *chunkcount,
                                 const struct xH5sectionlookup_t *sectlookup,
                                 const size_t        dtsize,
                                 void                *fulldata,
                                 void                *data,
                                 TIO_Size_t          *mixcellcount );

extern int xH5_GetChunkedSpace( struct xH5object_t    *hobject,
                                const xSpacetype_t    spacetype,
                                const iClass_t        class,
                                const TIO_Mesh_t      meshtype,
                                const iDataPos_t      datapos,
                                const int             ndset,
                                const TIO_Dims_t      ndims,
                                const TIO_Size_t      nghosts,
                                const TIO_Size_t      nchunks,
                                const union iInfo_t   *chunkinfo,
                                const int             nodata,     /* enables zero-sized dataspace if data null */
                                const TIO_Size_t      idx,
                                TIO_Size_t            *space_size,
                                hid_t                 *space_id );

extern int xH5_GetCount(  const TIO_Mesh_t    meshtype,
                          const iDataPos_t    datapos,
                          const TIO_Dims_t    ndims,
                          const TIO_Size_t    nghosts,
                          const TIO_Size_t    nchunks,
                          const union iInfo_t *chunkinfo,
                          const TIO_Size_t    chunkidx,
                          hsize_t             *count );

extern int xH5_GetSection( struct xH5file_t          *hfile,
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
                           hsize_t                   *datasize );

extern int xH5_ObjectExists( const hid_t group_id, const char name[] );

extern hid_t xH5_H5Screate_simple_none( void );



#ifndef NDEBUG

/* Debugging routines */

extern void dNullFile   ( struct xH5file_t   *hfile,
                          const char         name[]   );

extern void dNullObject ( struct xH5object_t *hobject );

extern void dNullRef    ( struct xH5file_t   *hfile,
                          struct xH5object_t *hobject,
                          const iClass_t     class,
                          const char         name[]   );

extern void dPrintSpace ( const hid_t space_id );

#endif




#if ( !((H5_VERS_MAJOR == 1) && (H5_VERS_MINOR >= 8)) )

/* Need conversion routine for int to float - not required if HDF5 >= v1.8.x */

extern herr_t xH5_Conv_i2f ( hid_t       src_id,
                      hid_t       dst_id,
                      H5T_cdata_t *cdata,
                      size_t      nelmts,
                      size_t      buf_stride,
                      size_t      bkg_stride,
                      void        *buf,
                      void        *bkg,
                      hid_t       dset_xfer_plist );

#endif
/* end if HDF5 >= v1.8.x */



#endif
/* end ifdef _TYPHIO_XHDF5 */

#endif

/*
 * EOF
 */

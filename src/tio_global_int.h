/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_GLOBAL_INT_H
#define _TIO_GLOBAL_INT_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <sys/param.h>
#endif


/* -- Need tio_error.h and tio_global.h included to get enum and TIO_t definitions */
#include "tio_error.h"
#include "tio_global.h"


/* -- True and False integer representation for return codes, etc: */

#ifdef FALSE
#undef FALSE
#endif
#define FALSE (1 == 0)

#ifdef TRUE
#undef TRUE
#endif
#define TRUE  (1 == 1)


#ifdef TIOmin
#undef TIOmin
#endif
#define TIOmin(x,y) ((x) < (y) ? (x) : (y))

#ifdef TIOmax
#undef TIOmax
#endif
#define TIOmax(x,y) ((x) > (y) ? (x) : (y))

#ifdef TIOabs
#undef TIOabs
#endif
#define TIOabs(x) (((x) < 0) ? -(x) : (x))


#ifdef TIObottom
#undef TIObottom
#endif
#define TIObottom(n,x,l) ((n)>(x)) ? (l) : ((x)-(n))

#ifdef TIOtop
#undef TIOtop
#endif
#define TIOtop(n,x,h)    (((n)+(x))>(h)) ? (h) :((n)+(x))



/*
  Begin and end brackets used in top API routines.
  Used to (re)initialize error stack for each routine call.
  Could also contain timing/tracing routines in future.
 */

#define TIObegin(subr, tag) { iTIO_Init(); iTIO_InitError(); }
#define TIOend(subr, tag)   { iTIO_KillError(); }


/*
  Macro to set a pointer to a value if non-null.
*/
#define TIOset(ptr, val) { if (ptr != NULL) *ptr = val; }

typedef enum { CLASS_NULL       = -11,
               CLASS_ANY        = -10,
               CLASS_SECTION    = -2,
               CLASS_FILE       = -1,
               CLASS_STATE      =  0,   /* This value to CLASS_MAX used as array indices */
               CLASS_MESH       =  1,   /* in names for stored reference tables          */
               CLASS_MATERIAL   =  2,
               CLASS_QUANT      =  3,
               CLASS_VGROUP     =  4,
               CLASS_VAR        =  5,
               CLASS_MAX        =  5  } iClass_t;

typedef enum { DATAPOS_NULL     = -1,
               DATAPOS_CELL     =  0,   /* These values up to...                         */
               DATAPOS_EDGE_I   =  1,   /* ...                                           */
               DATAPOS_EDGE_J   =  2,   /* ...                                           */
               DATAPOS_EDGE_K   =  3,   /* ...                                           */
               DATAPOS_FACE_I   =  4,   /* ...                                           */
               DATAPOS_FACE_J   =  5,   /* ...                                           */
               DATAPOS_FACE_K   =  6,   /* ...                                           */
               DATAPOS_NODE     =  7,   /* here must match equivalent centring values    */
               DATAPOS_SHAPE    =  8,
               DATAPOS_CONNECT  =  9,
               DATAPOS_MIXED    = 10,   /* = cell centred + appended mixed data          */
               DATAPOS_MIXCELL  = 11,   /* = just mixed cell data                        */
               DATAPOS_MIXCOMP  = 12    /* = just mixed component data (only ever in vf) */
	       } iDataPos_t;

typedef enum { UNSTR_NODEIDS    =  3,   /* Used when registering unstructured datasets   */
               UNSTR_CELLIDS    =  4,
               UNSTR_SHAPES     =  5,
               UNSTR_NCPS       =  6,
               UNSTR_CONNECT    =  7  } iUnstr_t;

typedef enum { MAT_MATERIAL     =  0,   /* Used when registering unstructured datasets   */
               MAT_NCOMP        =  1,
               MAT_PTR          =  2,
               MAT_VOLFRAC      =  3  } iMatdata_t;


typedef enum { STATUS_NULL      = -1,
               STATUS_NEW       =  0,   /* Object is new, datasets yet to be created             */
               STATUS_CREATED   =  1,   /* Object is created, so are datasets, but not written   */
               STATUS_WRITTEN   =  2,   /* New object now written to - can be further written to */
               STATUS_OLD       =  3,   /* Object pre-exisiting and opened                       */
               STATUS_MODIFIED  =  4    /* Object pre-existed and has been modified              */
                                     } iStatus_t;

typedef enum { FILETYPE_NULL    = -1,
               FILETYPE_HDF5    =  0  } iFiletype_t;



/**************************************************************************************************/



union iInfo_t {

  /* Important:  Only TIO_Size_t to be used within this type
                 This allows arrays of the whole union to be written as a contiguously using
                 the same datatype */

  struct {
    TIO_Size_t  ni, nj, nk;
    TIO_Size_t  il, ih, jl, jh, kl, kh;      /* nodal wrt mesh */
    TIO_Size_t  nghosts;               /* not used for chunks, but is for whole quad object */
    TIO_Size_t  nmixcell, nmixcomp;
  } quad;

  struct {
    TIO_Size_t  nnodes,       ncells,       nshapes,       nconnectivity;
    TIO_Size_t  nghost_nodes, nghost_cells, nghost_shapes, nghost_connectivity;
    TIO_Size_t  nmixcell, nmixcomp;
  } unstr;

  struct {
    TIO_Size_t  nnodes;
    TIO_Size_t  nl, nh;
    TIO_Size_t  nghosts;
  } point;

};



#define INFONSIZE 12
#define EXTSIZE 4
/*                    = effective number of TIO_Size_t's in iInfo_t
                      ( = (sizeof(struct iInfo_t) / sizeof(TIO_Size_t)).
                      ( try to keep this even if possible - may help performance a bit )
                       - if this number ever changes, will need to put logic into read
                         of chunk info
*/


union iSection_t {

  struct {
    TIO_Size_t il, ih, jl, jh, kl, kh;      /* nodal wrt mesh        */
    /* TIO_Size_t nmixcell, nmixcomp; */
  } quad;

  struct {
    TIO_Size_t   nIDs;
    TIO_Size_t   *IDs;
    /* TIO_Size_t   nmixcell, nmixcomp; */
    TIO_Centre_t centring;
  } unstr;

  struct {
    TIO_Size_t nIDs;
    TIO_Size_t *IDs;
  } point;

};


struct iToDo_t {
  size_t       nlist, nalloc;
  TIO_Object_t *objectID;
  iClass_t     *class;
};

struct iSectionCache_t {
  size_t       nlist, nalloc;
  TIO_Object_t *sectionID;
};



/**************************************************************************************************/
struct iFile_t {
  char         name[TIO_FILENAME_LEN];

  /* ??? char realpath[PATH_MAX]  ???  - use for seeing if files are open / multiple open..??? */

  char         codename[TIO_STRLEN];
  char         version[TIO_STRLEN];
  char         created[TIO_STRLEN];
  char         modified[TIO_STRLEN];
  char         title[TIO_STRLEN];

  int          vmaj, vmin, vrel;

  TIO_Access_t access;
  int          writeable;

  MPI_Comm     comm;
  MPI_Info     info;
  int          rank;


  iFiletype_t  filetype;

  int (* Init)         ( void );
  int (* Kill)         ( void );

  int (* InitRef)      ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const iClass_t     class,
                         const int          append    );

  int (* AddRef)       ( const TIO_File_t   fileID,        /*  fileID                       */
                         const TIO_Object_t parentID,      /*  Object containing reference  */
                         const TIO_Object_t objectID,      /*  Object to reference to       */
                         const iClass_t     class,         /*  Class of reference object    */
                         const char         name[]    );   /*  Name of object to reference  */

  int (* RemoveRef)    ( const TIO_File_t   fileID,        /*  fileID                       */
                         const TIO_Object_t parentID,      /*  Object containing reference  */
                         const TIO_Object_t objectID,      /*  Object to reference to       */
                         const iClass_t     class,         /*  Class of reference object    */
                         const char         name[]    );   /*  Name of object to reference  */

  int (* FinalizeRef)  ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const iClass_t     class     );

  int (* ReadRef)      ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const iClass_t     class,
                         TIO_Size_t         *nobj,
                         char               names[][TIO_STRLEN] );


  int (* CreateFile)   ( const TIO_File_t   fileID,
                         const char         name[],
                         const TIO_Access_t access,
                         const MPI_Comm     comm,
                         const MPI_Info     info,
                         const int          rank      );

  int (* OpenFile)     ( const TIO_File_t   fileID,
                         const char         name[],
                         const TIO_Access_t access,
                         const MPI_Comm     comm,
                         const MPI_Info     info,
                         const int          rank      );

  int (* CloseFile)    ( const TIO_File_t   fileID    );

  int (* FlushFile)    ( const TIO_File_t   fileID    );


  int (* CreateObject) ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const TIO_Object_t objectID,
                         const iClass_t     class,
                         const char         name[]    );

  int (* OpenObject)   ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const TIO_Object_t objectID,
                         const iClass_t     class,
                         const int          byindex,
                         char               name[],
                         const TIO_Size_t   idx       );

  int (* CloseObject)  ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const TIO_Object_t objectID,
                         const iClass_t     class     );


  int (* CreateDataset)   ( const TIO_File_t   fileID,
                            const TIO_Object_t parentID,
                            const TIO_Object_t objectID,
                            const iClass_t     class,
                            const char         name[],
                            const int          ndset,
                            const TIO_Dims_t   ndims,
                            const TIO_Size_t   *dims,
                            const TIO_Data_t   datatype  );

  int (* OpenDataset)     ( const TIO_File_t   fileID,
                            const TIO_Object_t parentID,
                            const TIO_Object_t objectID,
                            const iClass_t     class,
                            const char         name[],
                            const int          ndset,
                            TIO_Dims_t         *ndims,
                            TIO_Size_t         *dims,
                            TIO_Data_t         *datatype );

  int (* CloseDataset)    ( const TIO_File_t   fileID,
                            const TIO_Object_t parentID,
                            const TIO_Object_t objectID,
                            const iClass_t     class,
                            const int          ndset     );

  int (* WriteDataset)    ( const TIO_File_t   fileID,
                            const TIO_Object_t parentID,
                            const TIO_Object_t objectID,
                            const iClass_t     class,
                            const int          ndset,
                            const TIO_Xfer_t   xfer,
                            const TIO_Data_t   datatype,
                            const void         *data     );

  int (* ReadDataset)     ( const TIO_File_t   fileID,
                            const TIO_Object_t parentID,
                            const TIO_Object_t objectID,
                            const iClass_t     class,
                            const int          ndset,
                            const TIO_Xfer_t   xfer,
                            const TIO_Data_t   datatype,
                            void               *data );

  int (* ReadDatasetSlab) ( const TIO_File_t   fileID,
                            const TIO_Object_t parentID,
                            const TIO_Object_t objectID,
                            const iClass_t     class,
                            const int          ndset,
                            const TIO_Xfer_t   xfer,
                            const TIO_Data_t   datatype,
                            const TIO_Dims_t   ndims,
                            const TIO_Size_t   slabinfo[TIO_MAXDIMS][2],
                            void               *data );


  int (* CreateChunkedDataset)   ( const TIO_File_t    fileID,
                                   const TIO_Object_t  parentID,
                                   const TIO_Object_t  objectID,
                                   const iClass_t      class,
                                   const char          name[],
                                   const int           ndset,
                                   const TIO_Data_t    datatype,
                                   const TIO_Mesh_t    meshtype,
                                   const TIO_Dims_t    ndims,
                                   const TIO_Size_t    nghosts,
                                   const TIO_Size_t    nchunks,
                                   const union iInfo_t *chunkinfo,
                                   const union iInfo_t *allinfo,
                                   const iDataPos_t    datapos );

  int (* OpenChunkedDataset)     ( const TIO_File_t    fileID,
                                   const TIO_Object_t  parentID,
                                   const TIO_Object_t  objectID,
                                   const iClass_t      class,
                                   const char          name[],
                                   const int           ndset,
                                   const TIO_Data_t    datatype,
                                   const TIO_Mesh_t    meshtype,
                                   const TIO_Dims_t    ndims,
                                   const TIO_Size_t    nghosts,
                                   const TIO_Size_t    nchunks,
                                   const union iInfo_t *chunkinfo,
                                   const union iInfo_t *allinfo,
                                   const iDataPos_t    datapos );

  int (* CloseChunkedDataset)    ( const TIO_File_t    fileID,
                                   const TIO_Object_t  parentID,
                                   const TIO_Object_t  objectID,
                                   const iClass_t      class,
                                   const int           ndset    );

  int (* WriteChunkedDataset)    ( const TIO_File_t    fileID,
                                   const TIO_Object_t  parentID,
                                   const TIO_Object_t  objectID,
                                   const iClass_t      class,
                                   const int           ndset,
                                   const TIO_Xfer_t    xfer,
                                   const TIO_Data_t    datatype,
                                   const TIO_Mesh_t    meshtype,
                                   const TIO_Dims_t    ndims,
                                   const TIO_Size_t    nghosts,
                                   const TIO_Size_t    nchunks,
                                   const TIO_Bool_t    writeall,
                                   const union iInfo_t *chunkinfo,
                                   const iDataPos_t    datapos,
                                   const TIO_Size_t    chunkidx,
                                   const void          *data    );

  /*
 int (* WriteChunkedDatasetAll)  ( const TIO_File_t   fileID,
                                   const TIO_Object_t parentID,
                                   const TIO_Object_t objectID,
                                   const iClass_t     class,
                                   const int          ndset,
                                   const TIO_Xfer_t   xfer,
                                   const TIO_Data_t   datatype,
                                   const TIO_Mesh_t   meshtype,
                                   const TIO_Dims_t   ndims,
                                   const TIO_Size_t   nchunks,
                                   const union iInfo_t *chunkinfo,
                                   const union iInfo_t *allinfo,
                                   const iDataPos_t   datapos,
                                   const void         *data );
  */

  int (* ReadChunkedDataset)     ( const TIO_File_t    fileID,
                                   const TIO_Object_t  parentID,
                                   const TIO_Object_t  objectID,
                                   const iClass_t      class,
                                   const int           ndset,
                                   const TIO_Xfer_t    xfer,
                                   const TIO_Data_t    datatype,
                                   const TIO_Mesh_t    meshtype,
                                   const TIO_Dims_t    ndims,
                                   const TIO_Size_t    nghosts,
                                   const TIO_Size_t    nchunks,
                                   const union iInfo_t *chunkinfo,
                                   const iDataPos_t    datapos,
                                   const TIO_Size_t    chunkidx,
                                   TIO_Size_t          *datasize,
                                   void                *data );

  int (* ReadChunkedDatasetSection) ( const TIO_File_t       fileID,
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
                                      void                   *data );

  int (* ReadChunkedDatasetAll)  ( const TIO_File_t    fileID,
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
                                   void                *data );

  int (* SetSection)  ( const TIO_File_t       fileID,
                        const TIO_Object_t     meshID,
                        const TIO_Object_t     sectionID,
                        const TIO_Mesh_t       meshtype,
                        const TIO_Dims_t       ndims,
                        const union iSection_t *section,
                        const iDataPos_t       datapos );

  int (* KillSection) ( const TIO_File_t       fileID,
                        const TIO_Object_t     meshID,
                        const TIO_Object_t     sectionID );


  int (* WriteAttr)    ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const TIO_Object_t objectID,
                         const iClass_t     class,
                         const char         name[],
                         const TIO_Data_t   datatype,
                         const void         *data );

  int (* ReadAttr)     ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const TIO_Object_t objectID,
                         const iClass_t     class,
                         const char         name[],
                         const TIO_Data_t   datatype,
                         void               *data );

  int (* DelAttr)      ( const TIO_File_t   fileID,
                         const TIO_Object_t parentID,
                         const TIO_Object_t objectID,
                         const iClass_t     class,
                         const char         name[] );

  int (* ConvBuf)      ( const TIO_Data_t   idatatype,
                         const TIO_Data_t   odatatype,
                         const TIO_Size_t   nbuf,
                         void               *buf );

};



/**************************************************************************************************/

struct iState_t {
  char       name[TIO_STRLEN];

  TIO_Step_t step;
  TIO_Time_t time;
  char       units[TIO_STRLEN];
};



/**************************************************************************************************/

struct iVgroup_t {
  char         name[TIO_STRLEN];
};



/**************************************************************************************************/

struct iVariable_t {
  char         name[TIO_STRLEN];

  TIO_Dims_t   ndims;
  TIO_Size_t   dims[TIO_MAXDIMS];
  TIO_Data_t   datatype;
  char         units[TIO_STRLEN];
};



/**************************************************************************************************/

struct iMesh_t {
  char           name[TIO_STRLEN];
  iStatus_t      status;
  int            rangeset;

  TIO_Mesh_t     meshtype;
  TIO_Coord_t    coordtype;
  int            isAMR;
  char           group[TIO_STRLEN];
  TIO_Size_t     order;
  TIO_Size_t     matghosts;   /* = -1 if no material defined, otherwise num mat ghosts */

  TIO_Data_t     graph_datatype, coord_datatype;
  char           iunits[TIO_STRLEN], junits[TIO_STRLEN], kunits[TIO_STRLEN];
  char           ilabel[TIO_STRLEN], jlabel[TIO_STRLEN], klabel[TIO_STRLEN];

  union iInfo_t          info;            /* Totals / maximums                                    */
  TIO_Dims_t             ndims;
  TIO_Size_t             nchunks;         /* Total num chunks in mesh                             */
  TIO_Size_t             nchunks_unset;   /* Number of chunks currently undefined                 */
  union iInfo_t          *chunkinfo;      /* Per chunk                                            */
  int                    *chunkset;       /* Set flag per chunk                                   */
  TIO_Object_t           *chunksectionID; /* Per chunk - sectionID for chunk + internal ghosts    */

  struct iToDo_t         *todo;           /* Caches mat/quants to create if chunks not done first */
  struct iSectionCache_t *sectionCache;   /* Sections in use                                      */
  void                   *range;          /* Range of the mesh = bounding box [il,ih,jl,jh,kl,kh] */
};



/**************************************************************************************************/
struct iMaterial_t {
  char         name[TIO_STRLEN];
  iStatus_t    status;
  int          rangeset;
  int          namesset;

  TIO_Data_t   datatype, ncomp_datatype, ptr_datatype, vf_datatype;
  TIO_Size_t   nmat;
  TIO_Size_t   nghosts;
  int          ismixed;

  char         (*matnames)[TIO_STRLEN];
  void         *matnums;
};



/**************************************************************************************************/
struct iQuant_t {
  char         name[TIO_STRLEN];
  iStatus_t    status;
  int          rangeset;

  TIO_Data_t   datatype;
  TIO_Centre_t centring;
  TIO_Size_t   nghosts;
  int          ismixed;

  char         units[TIO_STRLEN];
  void         *qmin, *qmax;
};



/**************************************************************************************************/
struct iArrayQuant_t {
  char         name[TIO_STRLEN];
  iStatus_t    status;
  int          rangeset;

  TIO_Data_t   datatype;
  TIO_Centre_t centring;
  TIO_Size_t   nghosts;

  TIO_Dims_t   ndims1;
  TIO_Dims_t   ndims2;
  TIO_Size_t   dims1[TIO_MAXDIMS];
  TIO_Size_t   *dims2;

  int          *dims2set;
  TIO_Size_t   ndims2_unset;

  char         units[TIO_STRLEN];
  void         *aqmin, *aqmax;
};



/**************************************************************************************************/
struct iIDlist_t {                      /* Used for surface and nodelist objects */
  char         name[TIO_STRLEN];
  TIO_Data_t   datatype;
  TIO_Size_t   nIDs;
};


/**************************************************************************************************/

/*
  Include malloc and hierarchy header files to that .c files need only include this header
  Need to put them here so that some of the TyphonIO types defined above can be used within them.
*/

#include "tio_malloc_int.h"
#include "tio_hierarchy_int.h"

/**************************************************************************************************/



extern char *iTIO_strcpy  ( char *s1, const char *s2 );
extern char *iTIO_strncpy ( char *s1, const char *s2, size_t n );

extern size_t iTIO_SizeOf( const TIO_Data_t datatype );


extern TIO_t  cTIO_CheckDatatype( const char            subname[],
                                  const char            argname[],
                                  const int             ignore,
                                  const TIO_Data_t      datatype,
                                  const TIO_TypeClass_t typeclass );

extern TIO_t cTIO_CheckGhosts(    const char         subname[],
                                  const TIO_File_t   fileID,
                                  const TIO_Object_t meshID,
                                  struct iMesh_t     *tmesh,
                                  const TIO_Size_t   nghosts,
                                  const TIO_Size_t   objnghosts,
                                  TIO_Size_t         *outnghosts,
                                  const int          reading );


extern iDataPos_t iTIO_Centring2Datapos ( const TIO_Centre_t centring,
                                          const TIO_Mesh_t   meshtype,
                                          const int          ismixed );

extern int iTIO_DataposAdjust( const iDataPos_t datapos, int *dni, int *dnj, int *dnk );

extern int iTIO_SetSection( const TIO_Mesh_t meshtype,
                            const TIO_Dims_t ndims,
                            const TIO_Size_t il,
                            const TIO_Size_t ih,
                            const TIO_Size_t jl,
                            const TIO_Size_t jh,
                            const TIO_Size_t kl,
                            const TIO_Size_t kh,
                            const TIO_Size_t nn,
                            const TIO_Size_t *IDs,
                            union iSection_t *section );

extern int iTIO_CheckSection( const struct iMesh_t *tmesh,
                              const TIO_Object_t sectionID );

extern TIO_t cTIO_CheckRange( const char           subname[],
                              const struct iFile_t *tfile,
                              const TIO_Data_t     datatype,
                              const void           *minval,
                              const void           *maxval );


#endif

/*
 * EOF
 */

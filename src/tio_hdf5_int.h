/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_HDF5_INT_H
#define _TIO_HDF5_INT_H


#include "tio_global_int.h"


#define TIO_H5_OBJECT_OFFSET ((size_t)8)
#define TIO_H5_OBJECT_LENGTH ((size_t)8)


/* -- Check file is HDF5 */
extern int iH5_CheckFile( const char name[] );


extern int iH5_WriteVersion( const TIO_File_t fileID, const char name[],
                             const int maj, const int min, const int rel );

extern int iH5_ReadVersion( const TIO_File_t fileID, const char name[],
                            int *maj, int *min, int *rel );


/* -- Start up and kill HDF5 */

extern int iH5_Init( void );
extern int iH5_Kill( void );


/* -- Error printing / clearing */

extern int iH5_PrintErrors( void );
extern int iH5_ClearErrors( void );


/* -- References */

extern int iH5_InitRef       ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const iClass_t     class,
                               const int          reading  );

extern int iH5_AddRef        ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const char         name[]   );

extern int iH5_RemoveRef     ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const char         name[] );

extern int iH5_FinalizeRef   ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const iClass_t     class    );

extern int iH5_ReadRef       ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const iClass_t     class,
                               TIO_Size_t         *nobj,
                               char               names[][TIO_STRLEN] );



/* -- File operations */

extern int iH5_CreateFile    ( const TIO_File_t   fileID,
                               const char         name[],
                               const TIO_Access_t access,
                               const MPI_Comm     comm,
                               const MPI_Info     info,
                               const int          rank   );

extern int iH5_OpenFile      ( const TIO_File_t   fileID,
                               const char         name[],
                               const TIO_Access_t access,
                               const MPI_Comm     comm,
                               const MPI_Info     info,
                               const int          rank     );

extern int iH5_CloseFile     ( const TIO_File_t   fileID   );

extern int iH5_FlushFile     ( const TIO_File_t   fileID   );



extern int iH5_CreateObject  ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const char         name[]   );

extern int iH5_OpenObject    ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const int          byindex,
                               char               name[],
                               const TIO_Size_t   idx      );

extern int iH5_CloseObject   ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class    );

extern int iH5_CreateDataset ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const char         name[],
                               const int          ndset,
                               const TIO_Dims_t   ndims,
                               const TIO_Size_t   *dims,
                               const TIO_Data_t   datatype );


extern int iH5_OpenDataset   ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const char         name[],
                               const int          ndset,
                               TIO_Dims_t         *ndims,
                               TIO_Size_t         *dims,
                               TIO_Data_t         *datatype );

extern int iH5_CloseDataset  ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const int          ndset     );

extern int iH5_WriteDataset  ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const int          ndset,
                               const TIO_Xfer_t   xfer,
                               const TIO_Data_t   datatype,
                               const void         *data );

extern int iH5_ReadDataset   ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const int          ndset,
                               const TIO_Xfer_t   xfer,
                               const TIO_Data_t   datatype,
                               void               *data );

extern int iH5_ReadDatasetSlab ( const TIO_File_t   fileID,
                                 const TIO_Object_t parentID,
                                 const TIO_Object_t objectID,
                                 const iClass_t     class,
                                 const int          ndset,
                                 const TIO_Xfer_t   xfer,
                                 const TIO_Data_t   datatype,
                                 const TIO_Dims_t   ndims,
                                 const TIO_Size_t   slabinfo[TIO_MAXDIMS][2],
                                 void               *data );


extern int iH5_CreateChunkedDataset ( const TIO_File_t      fileID,
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
                                      const iDataPos_t      datapos );

extern int iH5_OpenChunkedDataset   ( const TIO_File_t      fileID,
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
                                      const iDataPos_t      datapos );

extern int iH5_CloseChunkedDataset  ( const TIO_File_t      fileID,
                                      const TIO_Object_t    parentID,
                                      const TIO_Object_t    objectID,
                                      const iClass_t        class,
                                      const int             ndset );

extern int iH5_WriteChunkedDataset  ( const TIO_File_t      fileID,
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
                                      const void            *data    );

extern int iH5_ReadChunkedDataset   ( const TIO_File_t      fileID,
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
                                      void                  *data );

extern int iH5_ReadChunkedDatasetSection ( const TIO_File_t       fileID,
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

extern int iH5_ReadChunkedDatasetAll     ( const TIO_File_t    fileID,
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

extern int iH5_SetSection  ( const TIO_File_t       fileID,
                             const TIO_Object_t     meshID,
                             const TIO_Object_t     sectionID,
                             const TIO_Mesh_t       meshtype,
                             const TIO_Dims_t       ndims,
                             const union iSection_t *section,
                             const iDataPos_t       datapos );

extern int iH5_KillSection ( const TIO_File_t       fileID,
                             const TIO_Object_t     meshID,
                             const TIO_Object_t     sectionID );


extern int iH5_WriteAttr     ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const char         name[],
                               const TIO_Data_t   datatype,
                               const void         *data    );

extern int iH5_ReadAttr      ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const char         name[],
                               const TIO_Data_t   datatype,
                               void               *data    );

extern int iH5_DelAttr       ( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const TIO_Object_t objectID,
                               const iClass_t     class,
                               const char         name[]   );

extern int iH5_ConvBuf       ( const TIO_Data_t   idatatype,
                               const TIO_Data_t   odatatype,
                               const TIO_Size_t   nbuf,
                               void               *buf );

#endif

/*
 * EOF
 */

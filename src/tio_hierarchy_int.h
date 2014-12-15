/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_HIERARCHY_INT_H
#define _TIO_HIERARCHY_INT_H


#include "tio_global_int.h"

extern int iTIO_InitHierarchy( void );
extern int iTIO_KillHierarchy( void );

extern int iTIO_HierarchyGetNumChildren ( const iClass_t class, int *nchildren    );
extern int iTIO_HierarchyGetName        ( const iClass_t class, char **name       );
extern int iTIO_HierarchyGetSize        ( const iClass_t class, TIO_Size_t *size  );
extern int iTIO_HierarchyGetDelta       ( const iClass_t class, TIO_Size_t *delta );
extern int iTIO_HierarchyGetPriority    ( const iClass_t class, int *priority     );

extern int iTIO_HierarchyGetObjectName  ( const TIO_File_t   fileID,
                                          const TIO_Object_t objectID,
                                          char               **name );

extern TIO_t cTIO_HierarchyCheckObject(   const char         *subname,
                                          const TIO_File_t   fileID,
                                          const TIO_Object_t parentID,
                                          const char         *name );

extern TIO_t cTIO_HierarchyGetObjectInfo( const char         subname[],
                                          const TIO_File_t   fileID,
                                          TIO_Object_t       *parentID,
                                          const TIO_Object_t objectID,
                                          const iClass_t     class,
                                          struct iFile_t     **tfile,
                                          void               **objinfo );

extern TIO_t cTIO_HierarchyGetChunkedObjectInfo( const char         *subname,
                                                 const TIO_File_t   fileID,
                                                 const TIO_Object_t objectID,
                                                 iClass_t           *class,
                                                 TIO_Dims_t         *ndims,
                                                 TIO_Size_t         *nghosts,
                                                 int                *ismixed,
                                                 TIO_Size_t         *nchunks,
                                                 union iInfo_t      **chunkinfo);

extern TIO_t cTIO_HierarchyListObjects(   const char         subname[],
                                          const TIO_File_t   fileID,
                                          const TIO_Object_t parentID,
                                          const iClass_t     parentclass,
                                          const iClass_t     objectclass,
                                          TIO_Size_t         *nobj,
                                          char               names[][TIO_STRLEN] );


extern TIO_t cTIO_HierarchyPreClose(   const char         *subname,
                                       const TIO_File_t   fileID,
                                       TIO_Object_t       *parentID,
                                       const TIO_Object_t objectID,
                                       const iClass_t     class,
                                       struct iFile_t     **tfile,
                                       void               **objinfo );

extern TIO_t cTIO_HierarchyCreateFile( const char         subname[],
                                       const TIO_File_t   fileID,
                                       const char         *filename,
                                       const TIO_Access_t access,
                                       const MPI_Comm     comm,
                                       const MPI_Info     info,
                                       const int          rank,
                                       struct iFile_t     *tfile );

extern TIO_t cTIO_HierarchyOpenFile(   const char         subname[],
                                       const TIO_File_t   fileID,
                                       const char         *filename,
                                       const TIO_Access_t access,
                                       const MPI_Comm     comm,
                                       const MPI_Info     info,
                                       const int          rank,
                                       struct iFile_t     *tfile );

extern TIO_t cTIO_HierarchyCloseFile(  const char         subname[],
                                       const TIO_File_t   fileID );

extern TIO_t cTIO_HierarchyFlushFile(  const char         subname[],
                                       const TIO_File_t   fileID );



extern TIO_t cTIO_HierarchyCreateObject(   const char         subname[],
                                           const TIO_File_t   fileID,
                                           const TIO_Object_t parentID,
                                           const iClass_t     parentclass,
                                           const iClass_t     objectclass,
                                           const char         name[],
                                           struct iFile_t     **tfile,
                                           TIO_Object_t       *objectID,
                                           void               *objinfo );

extern TIO_t cTIO_HierarchyOpenObject(     const char         subname[],
                                           const TIO_File_t   fileID,
                                           const TIO_Object_t parentID,
                                           const iClass_t     parentclass,
                                           const iClass_t     objectclass,
                                           const char         name[],
                                           const int          byindex,
                                           const TIO_Size_t   idx,
                                           char               outname[],
                                           struct iFile_t     **tfile,
                                           TIO_Object_t       *objectID,
                                           void               *objinfo );

extern TIO_t cTIO_HierarchyCloseObject(    const char         subname[],
                                           const TIO_File_t   fileID,
                                           const TIO_Object_t parentID,
                                           const TIO_Object_t objectID,
                                           const iClass_t     class );

extern TIO_t cTIO_HierarchyKillObject(     const char         subname[],
                                           const TIO_File_t   fileID,
                                           const TIO_Object_t parentID,
                                           const TIO_Object_t objectID,
                                           const iClass_t     class );

extern int iTIO_HierarchyCreateObjectDatasets( const TIO_File_t   fileID,
                                               const TIO_Object_t parentID,
                                               const TIO_Object_t objectID,
                                               const iClass_t     class,
                                               struct iFile_t     *tfile,
                                               void               *parentinfo );
#endif

/*
 * EOF
 */

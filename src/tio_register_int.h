/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_REGISTER_INT_H
#define _TIO_REGISTER_INT_H

#include "tio_global_int.h"


extern int iTIO_RegisterFile     ( TIO_File_t      *fileID, struct iFile_t  *tfile );

extern int iTIO_GetFile          ( const TIO_File_t fileID, struct iFile_t **tfile );
extern int iTIO_DeregisterFile   ( const TIO_File_t fileID );

extern int iTIO_GetFileInstances ( const TIO_File_t fileID,
                                   const char       filename[],
                                   int              *writeable );



extern int iTIO_RegisterObject   ( const TIO_File_t   fileID,
                                   const TIO_Object_t parentID,
                                   TIO_Object_t       *objectID,
                                   const iClass_t     class,
                                   void               *tobject  );

extern int iTIO_GetObject        ( const TIO_File_t   fileID,
                                   TIO_Object_t       *parentID,
                                   const TIO_Object_t objectID,
                                   const iClass_t     class,
                                   void               **tobject );

extern int iTIO_GetClassObject   ( const TIO_File_t   fileID,
                                   const TIO_Object_t objectID,
                                   iClass_t           *class,
                                   void               **tobject );

extern int iTIO_GetAnyObject     ( const TIO_File_t   fileID,
                                   const iClass_t     class,
                                   TIO_Object_t       *objectID );

extern int iTIO_GetChildren(       const TIO_File_t   fileID,
                                   const TIO_Object_t parentID,
                                   TIO_Size_t         *nchildren,
                                   TIO_Object_t       *childrenID );

extern int iTIO_DeregisterObject ( const TIO_File_t   fileID,
                                   const TIO_Object_t objectID );




extern int iTIO_RegisterFFinfo   ( const TIO_File_t   fileID,
                                   const TIO_Object_t objectID,
                                   void               *ffinfo );

extern int iTIO_GetFFinfo        ( const TIO_File_t   fileID,
                                   const TIO_Object_t objectID,
                                   void               **ffinfo );

extern int iTIO_DeregisterFFinfo ( const TIO_File_t   fileID,
                                   const TIO_Object_t objectID );


#endif

/*
 * EOF
 */

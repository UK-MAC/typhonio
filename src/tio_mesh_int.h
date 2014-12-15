/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_MESH_INT_H
#define _TIO_MESH_INT_H


#include "tio_global_int.h"


extern TIO_t cTIO_GetMeshGhosts( const char         subname[],
                                 const TIO_File_t   fileID,
                                 const TIO_Object_t meshID,
                                 const TIO_Mesh_t   meshtype,
                                 TIO_Size_t         *nghosts );

extern int iTIO_CreateMeshDatasets( const TIO_File_t   fileID,
                                    const TIO_Object_t parentID,
                                    const TIO_Object_t meshID,
                                    struct iFile_t     *tfile,
                                    struct iMesh_t     *tmesh );


extern int iTIO_AddMeshObject( const TIO_File_t   fileID,
                               const TIO_Object_t meshID,
                               struct iFile_t     *tfile,
                               struct iMesh_t     *tmesh,
                               const TIO_Object_t objectID,
                               const iClass_t     class );


/* Defined in tio_material.c, but included here for ease: */

extern int iTIO_CreateMaterialDatasets( const TIO_File_t   fileID,
                                        const TIO_Object_t meshID,
                                        const TIO_Object_t matID,
                                        struct iFile_t     *tfile,
                                        struct iMesh_t     *tmesh,
                                        struct iMaterial_t *tmat );


/* Defined in tio_quant.c, but included here for ease: */

extern int iTIO_CreateQuantDatasets( const TIO_File_t   fileID,
                                     const TIO_Object_t meshID,
                                     const TIO_Object_t quantID,
                                     struct iFile_t     *tfile,
                                     struct iMesh_t     *tmesh,
                                     struct iQuant_t    *tquant );




#endif

/*
 * EOF
 */

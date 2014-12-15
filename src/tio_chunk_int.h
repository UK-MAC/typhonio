/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_CHUNK_INT_H
#define _TIO_CHUNK_INT_H

#include "tio_global_int.h"

extern TIO_t cTIO_SetSection( const char             subname[],
                              const TIO_File_t       fileID,
                              const TIO_Object_t     meshID,
                              TIO_Object_t           *sectionID,
                              const TIO_Mesh_t       meshtype,
                              const TIO_Dims_t       ndims,
                              const union iSection_t *section );

#endif

/*
 * EOF
 */

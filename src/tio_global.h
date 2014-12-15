/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */
#ifndef _TIO_GLOBAL_H
#define _TIO_GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif


#include <limits.h>
#include <stdlib.h>



#define TIO_VERSION_INFO "TyphonIO 1.6.0"
#define TIO_VERSION_MAJ  1
#define TIO_VERSION_MIN  6
#define TIO_VERSION_REL  0


#ifdef _TYPHIO_PARALLEL

#ifndef MPI_VERSION
#include <mpi.h>
#endif

#else

#ifndef MPI_VERSION
typedef int MPI_Comm;
typedef int MPI_Info;
#define MPI_COMM_NULL (MPI_Comm)(-1)
#define MPI_INFO_NULL (MPI_Info)(-1)
#define MPI_PROC_NULL (-1)
#endif

#endif


#define TIO_NULL          (-1)
#define TIO_FILENAME_LEN  (PATH_MAX)    /* UNIX maximum (from limits.h)           */
#define TIO_STRLEN        (64)          /* Nothing too big                        */
#define TIO_MAXDIMS       (7)           /* Max no. dimensions for variables, etc. */


#ifndef ULLONG_MAX
#   define ULLONG_MAX   ((TIO_Size_t)((long long)(-1)))
#endif


#define TIO_GHOSTS_NULL ((TIO_Size_t)(ULLONG_MAX))
#define TIO_GHOSTS_NONE 0
#define TIO_GHOSTS_ALL  ((TIO_Size_t)(TIO_GHOSTS_NULL - 1))


#define TIO_ARRAYQ_DIM1 (ULLONG_MAX - 1)
#define TIO_ARRAYQ_DIM2 (ULLONG_MAX - 2)
#define TIO_ARRAYQ_DIM3 (ULLONG_MAX - 3)
#define TIO_ARRAYQ_CELL (ULLONG_MAX - 4)
#define TIO_ARRAYQ_NODE (ULLONG_MAX - 5)



typedef int                    TIO_File_t;       /* File handles          */
typedef int                    TIO_Object_t;     /* Objects within files  */
typedef unsigned long long int TIO_Size_t;       /* General sizes         */
typedef unsigned long long int TIO_Step_t;       /* For step numbers      */
typedef double                 TIO_Time_t;       /* For times             */


typedef enum { TIO_FALSE                          =  0,
               TIO_TRUE                           =  1  } TIO_Bool_t;   /* For boolean args   */


typedef enum { TIO_ACC_NULL                       = -1,
               TIO_ACC_CREATE                     =  0,
               TIO_ACC_REPLACE                    =  1,
               TIO_ACC_READWRITE                  =  2,
               TIO_ACC_READONLY                   =  3,
               TIO_ACC_READWRITESTRICT            =  4,
               TIO_ACC_MIN                        =  0,
               TIO_ACC_MAX                        =  4  } TIO_Access_t;


typedef enum { TIO_DATATYPE_NULL                  = -1,
               TIO_SHORT                          =  0,                /* TIO_SHORT MUST = 0      */
               TIO_USHORT                         =  1,
               TIO_INT                            =  2,
               TIO_UINT                           =  3,
               TIO_LONG                           =  4,
               TIO_ULONG                          =  5,
               TIO_LLONG                          =  6,
               TIO_ULLONG                         =  7,
               TIO_FLOAT                          =  8,
               TIO_DOUBLE                         =  9,
               TIO_LDOUBLE                        = 10,
               TIO_LOGICAL                        = 11,
               TIO_CHAR                           = 12,
               TIO_UCHAR                          = 13,
               TIO_ENUM_T                         = 14,                /* Used internally         */
               TIO_SIZE_T                         = 15,                /* Used internally         */
               TIO_STEP_T                         = 16,                /* Used internally         */
               TIO_TIME_T                         = 17,                /* Used internally         */
               TIO_STRING                         = 18,
               TIO_DATATYPE_MIN                   = 0,
               TIO_DATATYPE_MAX                   = 18  } TIO_Data_t;  /* TIO_STRING MUST be last */

typedef enum { TIO_TYPECLASS_NULL                 = -1,
               TIO_TYPECLASS_SIGNED_INT           =  0,
               TIO_TYPECLASS_UNSIGNED_INT         =  1,
               TIO_TYPECLASS_FLOAT                =  2,
               TIO_TYPECLASS_STRING               =  3,
               TIO_TYPECLASS_MIN                  =  0,
               TIO_TYPECLASS_MAX                  =  3  } TIO_TypeClass_t;


typedef enum { TIO_DIMS_NULL                      = -1,
               TIO_0D                             =  0,
               TIO_1D                             =  1,
               TIO_2D                             =  2,
               TIO_3D                             =  3,
               TIO_4D                             =  4,
               TIO_5D                             =  5,
               TIO_6D                             =  6,
               TIO_7D                             =  7  } TIO_Dims_t;


typedef enum { TIO_MESH_NULL                      = -1,
               TIO_MESH_STATIC                    =  0,   /* Not yet supported */
               TIO_MESH_QUAD_COLINEAR             =  1,
               TIO_MESH_QUAD_NONCOLINEAR          =  2,
               TIO_MESH_QUAD                      =  1,   /* = colinear */
               TIO_MESH_UNSTRUCT                  =  3,
               TIO_MESH_POINT                     =  4,
               TIO_MESH_MIN                       =  1,   /* change to 0 when static supported */
               TIO_MESH_MAX                       =  4  } TIO_Mesh_t;

typedef enum { TIO_COORD_NULL                     = -1,
               TIO_COORD_CARTESIAN                =  0,
               TIO_COORD_SPHERICAL                =  1,
               TIO_COORD_CYLINDRICAL              =  2,
               TIO_COORD_CYLINDRICAL_AXISYMMETRIC =  3,
               TIO_COORD_MIN                      =  0,
               TIO_COORD_MAX                      =  3  } TIO_Coord_t;

               /*
                *  Spherical and cylindrical co-ordinates angles are in degrees
                *  Spherical   = (r, theta, thi)
                *  Cylindrical = (r, thi, z)
                *  thi is angle in x-y plane in both cases
                *
                *  For 2D meshes, cylindrical axisymmetric can be used which is (z, r)
                */

typedef enum { TIO_SHAPE_NULL                     = -1,
               TIO_SHAPE_POINT1                   = -10,
               TIO_SHAPE_BAR2                     = -11,
               TIO_SHAPE_TRI3                     = -12,
               TIO_SHAPE_QUAD4                    = -13,
               TIO_SHAPE_TET4                     = -14,
               TIO_SHAPE_WEDGE6                   = -15,
               TIO_SHAPE_HEX8                     = -16,
               TIO_SHAPE_PYR5                     = -17,
               TIO_SHAPE_MIN                      = -10,
               TIO_SHAPE_MAX                      = -17  } TIO_Shape_t;

               /*
                *  Predefined shapes all have -ve values
                *  Supplying a positive value, n, for a shape signifies a polygon with n nodes
                */


typedef enum { TIO_CENTRE_NULL                    = -1,
               TIO_CENTRE_CELL                    =  0,
               TIO_CENTRE_EDGE_I                  =  1,
               TIO_CENTRE_EDGE_J                  =  2,
               TIO_CENTRE_EDGE_K                  =  3,
               TIO_CENTRE_FACE_I                  =  4,
               TIO_CENTRE_FACE_J                  =  5,
               TIO_CENTRE_FACE_K                  =  6,
               TIO_CENTRE_NODE                    =  7,
               TIO_CENTRE_MIN                     =  0,
               TIO_CENTRE_MAX                     =  8  } TIO_Centre_t;


typedef enum { TIO_XFER_NULL                      = -1,
               TIO_XFER_INDEPENDENT               =  0,
               TIO_XFER_COLLECTIVE                =  1,
               TIO_XFER_COLLECTIVE_RR             =  2,  /* not yet supported */
               TIO_XFER_MIN                       =  0,
               TIO_XFER_MAX                       =  2  } TIO_Xfer_t;



/***************************************************************************************************
 *
 * TIO_Get_TypeClass
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_TypeClass
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the TyphonIO TypeClass of a TyphonIO datatype
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_TypeClass( const TIO_Data_t datatype,
 * <DATA>                          TIO_TypeClass_t  *typeclass );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> datatype  <DATA> TyphonIO datatype
 * <DATA> OUT <DATA> typeclass <DATA> TyphonIO typeclass
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> The typeclass of the datatype is simply it's class of fundamental datatype,
 * <DATA> ie. integer, float, or string.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 * <CDES>
 * <DATA> char datatypes are of integer TypeClass.
 * </CDES>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_TypeClass( const TIO_Data_t datatype, TIO_TypeClass_t *typeclass);


/***************************************************************************************************
 *
 * TIO_SizeOf
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_SizeOf
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the size of TyphonIO datatype
 * </PURP>
 *
 * <CAPI>
 * <DATA> int TIO_SizeOf( const TIO_Data_t datatype );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> datatype  <DATA> TyphonIO datatype
 * </PARA>
 *
 * <RETN>
 * <DATA> </a> Size of <code>datatype</code> in bytes
 * </RETN>
 *
 * <DESC>
 * <DATA> Returns the size in bytes of the fundamental datatypes within TyphonIO.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern int TIO_SizeOf( const TIO_Data_t datatype);



#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */

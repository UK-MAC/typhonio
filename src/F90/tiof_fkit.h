/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TYPHIO_FKIT_H
#define _TYPHIO_FKIT_H

#include <stdlib.h>

#include "typhonio.h"
#include <f2c.h>


#define TIO_STRLEN_F 64
#define TIO_NULL_F ((int_f)-1)


enum datatype_ft  { TIO_DATATYPE_NULL_F          = 1000,
                    TIO_REAL4_F                  = 1001,
                    TIO_REAL8_F                  = 1002,
                    TIO_INTEGER4_F               = 1003,
                    TIO_INTEGER8_F               = 1004,
                    TIO_LOGICAL_F                = 1005,
                    TIO_CHAR_F                   = 1006,
                    TIO_STRING_F                 = 1007,
                    TIO_SIZE_F                   = 1008   };

enum typeclass_ft { TIO_TYPECLASS_NULL_F         = 2000,
                    TIO_TYPECLASS_REAL_F         = 2001,
                    TIO_TYPECLASS_INTEGER_F      = 2002,
                    TIO_TYPECLASS_STRING_F       = 2003   };

enum nghosts_tf   { TIO_GHOSTS_NULL_F            = -1,
                    TIO_GHOSTS_NONE_F            = 0,
                    TIO_GHOSTS_ALL_F             = -2,
                    TIO_GHOSTS_INTERNAL_F        = -3,
                    TIO_GHOSTS_ONLY_F            = -4     };

enum access_ft    { TIO_ACC_NULL_F               = 90010,
                    TIO_ACC_CREATE_F             = 90011,
                    TIO_ACC_REPLACE_F            = 90012,
                    TIO_ACC_READWRITE_F          = 90013,
                    TIO_ACC_READONLY_F           = 90014,
                    TIO_ACC_READWRITESTRICT_F    = 90015  };

enum meshtype_ft  { TIO_MESH_NULL_F              = 90020,
                    TIO_MESH_QUAD_F              = 90021,
                    TIO_MESH_QUAD_COLINEAR_F     = 90021,
                    TIO_MESH_QUAD_NONCOLINEAR_F  = 90022,
                    TIO_MESH_UNSTRUCT_F          = 90023,
                    TIO_MESH_POINT_F             = 90024  };

enum coordtype_ft { TIO_COORD_NULL_F             = 90030,
                    TIO_COORD_CARTESIAN_F        = 90031,
                    TIO_COORD_SPHERICAL_F        = 90032,
                    TIO_COORD_CYLINDRICAL_F      = 90033,
                    TIO_COORD_CYL_AXISYMMETRIC_F = 90034  };

enum centring_tf  { TIO_CENTRE_NULL_F            = 90040,
                    TIO_CENTRE_CELL_F            = 90041,
                    TIO_CENTRE_EDGE_I_F          = 90042,
                    TIO_CENTRE_EDGE_J_F          = 90043,
                    TIO_CENTRE_EDGE_K_F          = 90044,
                    TIO_CENTRE_FACE_I_F          = 90045,
                    TIO_CENTRE_FACE_J_F          = 90046,
                    TIO_CENTRE_FACE_K_F          = 90047,
                    TIO_CENTRE_NODE_F            = 90048  };
  
enum arrayq_tf    { TIO_ARRAYQ_DIM1_F            = -101,
                    TIO_ARRAYQ_DIM2_F            = -102,
                    TIO_ARRAYQ_DIM3_F            = -103,
                    TIO_ARRAYQ_CELL_F            = -104,
                    TIO_ARRAYQ_NODE_F            = -105   };

enum xfer_tf      { TIO_XFER_NULL_F              = 90050,
                    TIO_XFER_INDEPENDENT_F       = 90051,
                    TIO_XFER_COLLECTIVE_F        = 90052  };

enum verbose_ft   { TIO_VERBOSE_NULL_F           = 90060,
                    TIO_VERBOSE_OFF_F            = 90061,
                    TIO_VERBOSE_ALL_F            = 90062,
                    TIO_VERBOSE_MASTER_F         = 90063  };


/* Listclass values - only used in wrappers */

enum listclass_ft { LISTCLASS_VGROUP             = 1,
                    LISTCLASS_VARIABLE           = 2,
                    LISTCLASS_STATE              = 3,
                    LISTCLASS_MESH               = 4,
                    LISTCLASS_MAT                = 5,
                    LISTCLASS_QUANT              = 6};


/* C type equivalents to Fortran integers and reals used */

typedef int    int_f;               /*  integer - for all enums  */
typedef int    TIO_tf;              /*  integer(kind=TIO_ERRK)   */
typedef int    TIO_File_tf;         /*  integer(kind=TIO_FILEK)  */
typedef int    TIO_Object_tf;       /*  integer(kind=TIO_OBJK)   */
typedef int    TIO_Size_tf;         /*  integer(kind=TIO_SIZEK)  */
typedef int    TIO_Step_tf;         /*  integer(kind=TIO_STEPK)  */
typedef double TIO_Time_tf;         /*  integer(kind=TIO_TIMEK)  */

typedef TIO_Shape_t TIO_Shape_tf;   /*  F90 integer datatype is specified to match TIO_Shape_t */



/* Direct integer conversion for TIO_t - error code values match between APIs */

#define c2f_TIO(x) ((TIO_tf)(x))
#define f2c_TIO(x) ((TIO_t)(x))

#define TIO_SUCCESS_F TIO_SUCCESS
#define TIO_FAIL_F    TIO_FAIL


#define c2f_file(x) ((TIO_File_tf)(x))
#define f2c_file(x) ((TIO_File_t)(x))

#define c2f_object(x) ((TIO_Object_tf)(x))
#define f2c_object(x) ((TIO_Object_t)(x))

#define c2f_idx(x) ((TIO_Size_tf)(x + 1))
#define f2c_idx(x) ((TIO_Size_t)(x - 1))

#define c2f_size(x) ((TIO_Size_tf)(x))
#define f2c_size(x) ((TIO_Size_t)(x))

#define c2f_step(x) ((TIO_Step_tf)(x))
#define f2c_step(x) ((TIO_Step_t)(x))

#define c2f_time(x) ((TIO_Time_tf)(x))
#define f2c_time(x) ((TIO_Time_t)(x))

#define c2f_ndims(x) ((int_f)(x))
#define f2c_ndims(x) ((TIO_Dims_t)(x))

#define c2f_str(c, l, f) C2F_strcpy(f, (int)*l, c)
#define c2f_fname(c, l, f) C2F_strcpy(f, (int)*l, c)

#undef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define f2c_str(f, l, c) {(void)memcpy((void *)c, (const void *)f, (size_t)*l);c[MIN(*l,TIO_STRLEN_F-1)]='\0';}
#define f2c_fname(f, l, c) {(void)memcpy((void *)c, (const void *)f, (size_t)*l);c[MIN(*l,TIO_FILENAME_LEN-1)]='\0';}



extern MPI_Comm       f2c_MPIcomm( const int_f fcomm);
extern MPI_Info       f2c_MPIinfo( const int_f finfo);

extern int_f          c2f_datatype(const TIO_Data_t cdatatype);
extern TIO_Data_t     f2c_datatype(const int_f      fdatatype);

extern int_f          c2f_typeclass(const TIO_TypeClass_t ctypeclass);

extern void           f2c_dims(const TIO_Size_tf fdims[], const int_f *ndims,
                               TIO_Size_t  cdims[], const int arrayq);
extern void           c2f_dims(const TIO_Size_t  cdims[], const TIO_Dims_t ndims,
                               TIO_Size_tf fdims[], const int arayq);

extern TIO_Size_tf    c2f_ghosts(const TIO_Size_t  cnghosts);
extern TIO_Size_t     f2c_ghosts(const TIO_Size_tf fnghosts);


extern TIO_Access_t   f2c_access(const int_f facc);

extern int_f          c2f_meshtype(const TIO_Mesh_t cmeshtype);
extern TIO_Mesh_t     f2c_meshtype(const int_f      fmeshtype);

extern int_f          c2f_coordtype(const TIO_Coord_t ccoordtype);
extern TIO_Coord_t    f2c_coordtype(const int_f       fcoordtype);

extern int_f          c2f_centre(const TIO_Centre_t ccentring);
extern TIO_Centre_t   f2c_centre(const int_f        fcentring);

extern TIO_Xfer_t     f2c_xfer(const int_f fxfer);

extern int_f          c2f_verbosemode(const TIO_Verbose_t cvmode);
extern TIO_Verbose_t  f2c_verbosemode(const int_f         fvmode);



#define setijk(narg, iptr, jptr, kptr, icoords, jcoords, kcoords) \
          { switch (*narg) {              \
            case 3:                       \
              iptr = icoords;             \
              jptr = jcoords;             \
              kptr = kcoords;             \
              break;                      \
            case 2:                       \
              iptr = icoords;             \
              jptr = jcoords;             \
              kptr = NULL;                \
              break;                      \
            case 1:                       \
              iptr = icoords;             \
              jptr = NULL;                \
              kptr = NULL;                \
              break;                      \
            default:                      \
              iptr = jptr = kptr = NULL;  \
              break;                      \
            }                             \
          }

#define setpm(narg, pptr, mptr, pdat, mdat)   \
          { switch (*narg) {                  \
            case 0:                           \
              /* No pure/mix data */          \
              pptr = mptr = NULL;             \
              break;                          \
            case 1:                           \
              /* Just pure data */            \
              pptr = pdat;                    \
              if (mptr != pptr) mptr = NULL;  \
              break;                          \
            case 2:                           \
              /* Pure and mix data */         \
              pptr = pdat;                    \
              mptr = mdat;                    \
              break;                          \
            default:                          \
              break;                          \
            }                                 \
          }

#define setmat(narg, matptr, ncompptr, ptrptr, mixmatptr, vfptr, mat, ncomp, ptr, mixmat, vf) \
          { matptr    = NULL;                            \
            mixmatptr = NULL;                            \
            ncompptr  = NULL;                            \
            ptrptr    = NULL;                            \
            vfptr     = NULL;                            \
            switch (*narg) {                             \
            case 0:                                      \
              /* No pure/mix data */                     \
              break;                                     \
            case 1:                                      \
              /* Just pure data */                       \
              matptr = mat;                              \
              if (mixmatptr != matptr) mixmatptr = NULL; \
              break;                                     \
            case 2:                                      \
              /* All pure and mix data */                \
              matptr    = mat;                           \
              mixmatptr = mixmat;                        \
              ncompptr  = ncomp;                         \
              ptrptr    = ptr;                           \
              vfptr     = vf;                            \
              break;                                     \
            case 1025:                                   \
              /* Just mix ncomp data */                  \
              ncompptr  = ncomp;                         \
              break;                                     \
            case 1026:                                   \
              /* No mix ptr data */                      \
              ptrptr    = ptr;                           \
              break;                                     \
            case 1027:                                   \
              /* No mix mixmat data */                   \
              mixmatptr = mixmat;                        \
              break;                                     \
            case 1028:                                   \
              /* No mix vf data */                       \
              vfptr     = vf;                            \
              break;                                     \
            }                                            \
          }

#define setext(narg, extptr, nscompptr, ptrptr, nsptr, mixextptr, ncmpsptr, ncptr, ext, nscomp, ptr, ns, mixext, ncmps, nc) \
          { extptr    = NULL;                            \
            nscompptr = NULL;                            \
            ptrptr    = NULL;                            \
            nsptr     = NULL;                            \
            mixextptr = NULL;                            \
            ncmpsptr  = NULL;                            \
            ncptr     = NULL;                            \
            switch (*narg) {                             \
            case 0:                                      \
              /* No pure/mix data */                     \
              break;                                     \
            case 1:                                      \
              /* Just pure data */                       \
              extptr = ext;                              \
              if (mixextptr != extptr) mixextptr = NULL; \
              break;                                     \
            case 2:                                      \
              /* All pure and mix data */                \
              extptr    = ext;                           \
              nscompptr = nscomp;                        \
              ptrptr    = ptr;                           \
              nsptr     = ns;                            \
              mixextptr = mixext;                        \
              ncmpsptr  = ncmps;                         \
              ncptr     = nc;                            \
              break;                                     \
            default:                                     \
              break;                                     \
            }                                            \
          }

#endif

/*
 * EOF
 */

/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include <ctype.h>
#include <string.h>

#include "tiof_fkit.h"



/***********************************************************************************************
 *
 *   MPI types
 *
 ***********************************************************************************************/
MPI_Comm
f2c_MPIcomm( const int_f fcomm)
{
#ifdef _TYPHIO_PARALLEL
  return ( MPI_Comm_f2c((MPI_Fint)fcomm) );
#else
  return ( (MPI_Comm)TIO_NULL_F ) ;
#endif
}

MPI_Info
f2c_MPIinfo( const int_f finfo)
{
#ifdef _TYPHIO_PARALLEL
  return ( MPI_Info_f2c((MPI_Fint)finfo) );
#else
  return ( (MPI_Info)TIO_NULL_F );
#endif
}



/***********************************************************************************************
 *
 *   Datatype type
 *
 ***********************************************************************************************/
TIO_Data_t
f2c_datatype(const int_f fdatatype)
{
  switch (fdatatype) {

  case TIO_REAL4_F:
    return(TIO_FLOAT);
    break;

  case TIO_REAL8_F:
    return(TIO_DOUBLE);
    break;

  case TIO_INTEGER4_F:
    return(TIO_INT);
    break;

  case TIO_INTEGER8_F:
    return(TIO_LLONG);
    break;

  case TIO_LOGICAL_F:
    return(TIO_LOGICAL);
    break;

  case TIO_CHAR_F:
    return(TIO_CHAR);
    break;

  case TIO_STRING_F:
    return(TIO_STRING);
    break;

  case TIO_SIZE_F:
    return(TIO_SIZE_T);
    break;
    
  default:
    return(TIO_DATATYPE_NULL);
    break;
  }
}

int_f
c2f_datatype(const TIO_Data_t cdatatype)
{
  switch (cdatatype) {

  case TIO_FLOAT:
    return(TIO_REAL4_F);
    break;

  case TIO_DOUBLE:
    return(TIO_REAL8_F);
    break;

  case TIO_INT:
  case TIO_UINT:
    return(TIO_INTEGER4_F);
    break;

  case TIO_LLONG:
    return(TIO_INTEGER8_F);
    break;

  case TIO_LOGICAL:
    return(TIO_LOGICAL_F);
    break;

  case TIO_CHAR:
    return(TIO_CHAR_F);
    break;

  case TIO_STRING:
    return(TIO_STRING_F);
    break;

  case TIO_SIZE_T:
    return(TIO_SIZE_F);
    break;
    
  default:
    return(TIO_DATATYPE_NULL_F);
    break;
  }
}


/***********************************************************************************************
 *
 *   TypeClass type
 *
 ***********************************************************************************************/
int_f
c2f_typeclass(const TIO_TypeClass_t ctypeclass)
{
  switch (ctypeclass) {
    
  case TIO_TYPECLASS_SIGNED_INT:
  case TIO_TYPECLASS_UNSIGNED_INT:
    return(TIO_TYPECLASS_INTEGER_F);
    break;
    
  case TIO_TYPECLASS_FLOAT:
    return(TIO_TYPECLASS_REAL_F);
    break;
    
  case TIO_TYPECLASS_STRING:
    return(TIO_TYPECLASS_STRING_F);
    break;

  default:
    return(TIO_TYPECLASS_NULL_F);
    break;
  } 
}


/***********************************************************************************************
 *
 *   Dimensions
 *
 ***********************************************************************************************/
void
f2c_dims(const TIO_Size_tf fdims[], const int_f *ndims, TIO_Size_t cdims[], const int arrayq)
{
  TIO_Dims_t di, lndims;
  
  
  lndims = f2c_ndims(*ndims);
  
  if (lndims < 0) {
    return;
  }
  
  
  if (arrayq) {
    
    int_f      fd;
    TIO_Size_t cd;
    
    
    for (di=TIO_0D; di<lndims; ++di) {
      
      fd = fdims[di];
      
      switch (fd) {
      case TIO_ARRAYQ_DIM1_F:
        cd = TIO_ARRAYQ_DIM1;
        break;
      case TIO_ARRAYQ_DIM2_F:
        cd = TIO_ARRAYQ_DIM2;
        break;
      case TIO_ARRAYQ_DIM3_F:
        cd = TIO_ARRAYQ_DIM3;
        break;
      case TIO_ARRAYQ_CELL_F:
        cd = TIO_ARRAYQ_CELL;
        break;
      case TIO_ARRAYQ_NODE_F:
        cd = TIO_ARRAYQ_NODE;
        break;
      default:
        cd = (TIO_Size_t)fd;
        break;
      }
      
      cdims[di] = cd;
    }
    
  }
  else {
    
    for (di=TIO_0D; di<lndims; ++di) {
      cdims[di] = (TIO_Size_t)fdims[di];
    }
  }
  
  return;
}

void
c2f_dims(const TIO_Size_t cdims[], const TIO_Dims_t ndims, TIO_Size_tf fdims[], const int arrayq)
{
  TIO_Dims_t di;
  
  if (arrayq) {
    
    TIO_Size_t cd;
    int_f      fd;
    
    
    for (di=TIO_0D; di<ndims; ++di) {
      
      cd = cdims[di];
      
      switch (cd) {
      case TIO_ARRAYQ_DIM1:
        fd = TIO_ARRAYQ_DIM1_F;
        break;
      case TIO_ARRAYQ_DIM2:
        fd = TIO_ARRAYQ_DIM2_F;
        break;
      case TIO_ARRAYQ_DIM3:
        fd = TIO_ARRAYQ_DIM3_F;
        break;
      case TIO_ARRAYQ_CELL:
        fd = TIO_ARRAYQ_CELL_F;
        break;
      case TIO_ARRAYQ_NODE:
        fd = TIO_ARRAYQ_NODE_F;
        break;
      default:
        fd = (TIO_Size_tf)cd;
        break;
      }
      
      fdims[di] = fd;
    }    
    
  }
  else {
    
    for (di=TIO_0D; di<ndims; ++di) {
      fdims[di] = (TIO_Size_tf)cdims[di];
    }
  }
  
  return;
}


/***********************************************************************************************
 *
 *   Ghosts
 *
 ***********************************************************************************************/
TIO_Size_t
f2c_ghosts(const TIO_Size_tf fnghosts)
{
  switch (fnghosts) {
    
  case TIO_GHOSTS_NULL_F:
    return (TIO_GHOSTS_NULL);
    break;
    
  case TIO_GHOSTS_NONE_F:
    return (TIO_GHOSTS_NONE);
    break;
    
  case TIO_GHOSTS_ALL_F:
    return (TIO_GHOSTS_ALL);
    break;
    
    /*    
  case TIO_GHOSTS_INTERNAL_F:
    return (TIO_GHOSTS_INTERNAL);
    break;
    
  case TIO_GHOSTS_ONLY_F:
    return (TIO_GHOSTS_ONLY);
    break;
    */
    
  default:
    break;
  }
  
  return (f2c_size(fnghosts));    /* default if not a special value */
}

TIO_Size_tf
c2f_ghosts(const TIO_Size_t cnghosts)
{
  if (TIO_GHOSTS_NULL == cnghosts) return (TIO_GHOSTS_NULL_F);
  if (TIO_GHOSTS_NONE == cnghosts) return (TIO_GHOSTS_NONE_F);
  if (TIO_GHOSTS_ALL  == cnghosts) return (TIO_GHOSTS_ALL_F);

  /*
    if (TIO_GHOSTS_INTERNAL == cnghosts) return (TIO_GHOSTS_INTERNAL_F);
    if (TIO_GHOSTS_ONLY     == cnghosts) return (TIO_GHOSTS_ONLY_F);
  */

  return (c2f_size(cnghosts));
}


/***********************************************************************************************
 *
 *   Access mode type
 *
 ***********************************************************************************************/
TIO_Access_t
f2c_access(const int_f facc)
{
  switch (facc) {
  case TIO_ACC_CREATE_F:
    return (TIO_ACC_CREATE);
    break;
  case TIO_ACC_REPLACE_F:
    return (TIO_ACC_REPLACE);
    break;
  case TIO_ACC_READWRITE_F:
    return (TIO_ACC_READWRITE);
    break;
  case TIO_ACC_READONLY_F:
    return (TIO_ACC_READONLY);
    break;
  case TIO_ACC_READWRITESTRICT_F:
    return (TIO_ACC_READWRITESTRICT);
    break;
  default:
    return (TIO_ACC_NULL);
    break;
  }
}


/***********************************************************************************************
 *
 *   Mesh type
 *
 ***********************************************************************************************/
int_f
c2f_meshtype(const TIO_Mesh_t cmeshtype)
{
  switch (cmeshtype) {
    
  case TIO_MESH_QUAD_COLINEAR:
    return (TIO_MESH_QUAD_COLINEAR_F);
    break;
    
  case TIO_MESH_QUAD_NONCOLINEAR:
    return (TIO_MESH_QUAD_NONCOLINEAR_F);
    break;

  case TIO_MESH_UNSTRUCT:
    return (TIO_MESH_UNSTRUCT_F);
    break;

  case TIO_MESH_POINT:
    return (TIO_MESH_POINT_F);
    break;

  default:
    return (TIO_MESH_NULL_F);
    break;
  }
}

TIO_Mesh_t
f2c_meshtype(const int_f fmeshtype)
{
  switch (fmeshtype) {
    
  case TIO_MESH_QUAD_COLINEAR_F:
    return (TIO_MESH_QUAD_COLINEAR);
    break;
    
  case TIO_MESH_QUAD_NONCOLINEAR_F:
    return (TIO_MESH_QUAD_NONCOLINEAR);
    break;

  case TIO_MESH_UNSTRUCT_F:
    return (TIO_MESH_UNSTRUCT);
    break;
    
  case TIO_MESH_POINT_F:
    return (TIO_MESH_POINT);
    break;
    
  default:
    return (TIO_MESH_NULL);
    break;
  }
}


/***********************************************************************************************
 *
 *   Coord type
 *
 ***********************************************************************************************/
int_f
c2f_coordtype(const TIO_Coord_t ccoordtype)
{
  switch (ccoordtype) {

  case TIO_COORD_CARTESIAN:
    return (TIO_COORD_CARTESIAN_F);
    break;

  case TIO_COORD_SPHERICAL:
    return (TIO_COORD_SPHERICAL_F);
    break;

  case TIO_COORD_CYLINDRICAL:
    return (TIO_COORD_CYLINDRICAL_F);
    break;

  case TIO_COORD_CYLINDRICAL_AXISYMMETRIC:
    return (TIO_COORD_CYL_AXISYMMETRIC_F);
    break;
    
  default:
    return (TIO_COORD_NULL_F);
    break;
  }
}

TIO_Coord_t
f2c_coordtype(const int_f fcoordtype)
{
  switch (fcoordtype) {

  case TIO_COORD_CARTESIAN_F:
    return (TIO_COORD_CARTESIAN);
    break;

  case TIO_COORD_SPHERICAL_F:
    return (TIO_COORD_SPHERICAL);
    break;

  case TIO_COORD_CYLINDRICAL_F:
    return (TIO_COORD_CYLINDRICAL);
    break;

  case TIO_COORD_CYL_AXISYMMETRIC_F:
    return (TIO_COORD_CYLINDRICAL_AXISYMMETRIC);
    break;
    
  default:
    return (TIO_COORD_NULL);
    break;
  }
}


/***********************************************************************************************
 *
 *   Centre type
 *
 ***********************************************************************************************/
int_f
c2f_centre(const TIO_Centre_t ccentre)
{
  switch (ccentre) {

  case TIO_CENTRE_CELL:
    return (TIO_CENTRE_CELL_F);
    break;
    
  case TIO_CENTRE_EDGE_I:
    return (TIO_CENTRE_EDGE_I_F);
    break;
    
  case TIO_CENTRE_EDGE_J:
    return (TIO_CENTRE_EDGE_J_F);
    break;
    
  case TIO_CENTRE_EDGE_K:
    return (TIO_CENTRE_EDGE_K_F);
    break;
    
  case TIO_CENTRE_FACE_I:
    return (TIO_CENTRE_FACE_I_F);
    break;
    
  case TIO_CENTRE_FACE_J:
    return (TIO_CENTRE_FACE_J_F);
    break;
    
  case TIO_CENTRE_FACE_K:
    return (TIO_CENTRE_FACE_K_F);
    break;
    
  case TIO_CENTRE_NODE:
    return (TIO_CENTRE_NODE_F);
    break;
  
  default:
    return (TIO_CENTRE_NULL_F);
    break;
  }
}

TIO_Centre_t
f2c_centre(const int_f fcentre)
{
  switch (fcentre) {

  case TIO_CENTRE_CELL_F:
    return (TIO_CENTRE_CELL);
    break;
    
  case TIO_CENTRE_EDGE_I_F:
    return (TIO_CENTRE_EDGE_I);
    break;
    
  case TIO_CENTRE_EDGE_J_F:
    return (TIO_CENTRE_EDGE_J);
    break;
    
  case TIO_CENTRE_EDGE_K_F:
    return (TIO_CENTRE_EDGE_K);
    break;
    
  case TIO_CENTRE_FACE_I_F:
    return (TIO_CENTRE_FACE_I);
    break;
    
  case TIO_CENTRE_FACE_J_F:
    return (TIO_CENTRE_FACE_J);
    break;
    
  case TIO_CENTRE_FACE_K_F:
    return (TIO_CENTRE_FACE_K);
    break;
    
  case TIO_CENTRE_NODE_F:
    return (TIO_CENTRE_NODE);
    break;
    
  default:
    return (TIO_CENTRE_NULL);
    break;
  }
}


/***********************************************************************************************
 *
 *   Transfer mode type
 *
 ***********************************************************************************************/
TIO_Xfer_t
f2c_xfer(const int_f fxfer)
{
  switch (fxfer) {
    
  case TIO_XFER_INDEPENDENT_F:
    return (TIO_XFER_INDEPENDENT);
    break;
    
  case TIO_XFER_COLLECTIVE_F:
    return (TIO_XFER_COLLECTIVE);
    break;
    
  default:
    return (TIO_XFER_NULL);
    break;
  }
}


/***********************************************************************************************
 *
 *   Verbose mode type
 *
 ***********************************************************************************************/
TIO_Verbose_t
f2c_verbosemode(const int_f fvmode)
{
  switch (fvmode) {
    
  case TIO_VERBOSE_OFF_F:
    return (TIO_VERBOSE_OFF);
    break;
    
  case TIO_VERBOSE_ALL_F:
    return (TIO_VERBOSE_ALL);
    break;
    
  case TIO_VERBOSE_MASTER_F:
    return (TIO_VERBOSE_MASTER);
    break;
    
  default:
    return (TIO_VERBOSE_NULL);
    break;
  }
}

int_f
c2f_verbosemode(const TIO_Verbose_t cvmode)
{
  switch (cvmode) {
    
  case TIO_VERBOSE_OFF:
    return (TIO_VERBOSE_OFF_F);
    break;
    
  case TIO_VERBOSE_ALL:
    return (TIO_VERBOSE_ALL_F);
    break;
    
  case TIO_VERBOSE_MASTER:
    return (TIO_VERBOSE_MASTER_F);
    break;
    
  default:
    return (TIO_VERBOSE_NULL_F);
    break;
  }
}





/***************************************************************************************************
 *
 *   listnames_c
 *
 **************************************************************************************************/
void
F2C(listnames_c,LISTNAMES_C)
  ( const int_f         *class,
    const TIO_File_tf   *fileID,
    const TIO_Object_tf *objectID,
    TIO_Size_tf         *nobj,
    char                *names,
    TIO_tf              *err )
{
  TIO_Size_t l_nobj, i;
  char       *fnptr, ccpy[TIO_STRLEN];
  int        len=TIO_STRLEN, *lptr = &len;
  
  
  TIO_t (* listfunc) ( const TIO_File_t   fileID,
                       const TIO_Object_t parentID,
                       TIO_Size_t         *nobj,
                       char               names[][TIO_STRLEN] );
  
  
  switch (*class) {
    

  default:
    *err = c2f_TIO(TIO_ERR_F90_API);
    return;
  }
  
  
  if (*nobj > 0) {
    
    *err = c2f_TIO( listfunc( f2c_file(*fileID),
                              f2c_object(*objectID),
                              &l_nobj,
                              (char(*)[TIO_STRLEN])names ) );
    if (*err != TIO_SUCCESS_F) {
      return;
    }
    
    fnptr = names;
    for (i=0; i<l_nobj; ++i) {
      strcpy(ccpy, fnptr);
      c2f_str(ccpy, lptr, fnptr);
      fnptr += TIO_STRLEN_F;
    }
    
  }
  else {
    *err = c2f_TIO( listfunc( f2c_file(*fileID),
                              f2c_object(*objectID),
                              &l_nobj,
                              NULL ) );
    if (*err != TIO_SUCCESS_F) {
      return;
    }
  }
  
  *nobj = c2f_size(l_nobj);
  
  return;
}


/*
 * EOF
 */

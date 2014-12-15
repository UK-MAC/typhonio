/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"

#include "tio_mesh_int.h"

#include "typhonio_viz.h"


static const char *TIO_Version = TIO_VERSION_INFO;   /* Version info string that can be
                                                        found with UNIX strings command */



/***************************************************************************************************
 *
 * TIO_Get_TypeClass
 *
 **************************************************************************************************/
TIO_t
TIO_Get_TypeClass( const TIO_Data_t datatype, TIO_TypeClass_t *typeclass)
{
  const char *subname = "TIO_Get_TypeClass()";      /* Function name for debugging and error */


  TIObegin(subname,1);

  switch (datatype) {
  case TIO_DATATYPE_NULL:
    *typeclass = TIO_TYPECLASS_NULL;
    break;

  case TIO_CHAR:
  case TIO_SHORT:
  case TIO_INT:
  case TIO_LONG:
  case TIO_LLONG:
  case TIO_ENUM_T:
    *typeclass = TIO_TYPECLASS_SIGNED_INT;
    break;

  case TIO_UCHAR:
  case TIO_USHORT:
  case TIO_UINT:
  case TIO_ULONG:
  case TIO_ULLONG:
  case TIO_LOGICAL:
  case TIO_STEP_T:
  case TIO_SIZE_T:
    *typeclass = TIO_TYPECLASS_UNSIGNED_INT;
    break;

  case TIO_FLOAT:
  case TIO_DOUBLE:
  case TIO_LDOUBLE:
  case TIO_TIME_T:
    *typeclass = TIO_TYPECLASS_FLOAT;
    break;

  case TIO_STRING:
    *typeclass = TIO_TYPECLASS_STRING;
    break;

  default:
    *typeclass = TIO_TYPECLASS_NULL;
    TIOreturn (ERR_USER, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);
    break;

  }


  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_SizeOf
 *
 **************************************************************************************************/
int
TIO_SizeOf( const TIO_Data_t datatype)
{
  const char *subname = "TIO_SizeOf()";      /* Function name for debugging and error */

  size_t ds;

  int irc;


  TIObegin(subname,1);

  ds = iTIO_SizeOf(datatype);

  if (ds > 0) {
    irc = (int)ds;
  }
  else {
    irc = -1;
  }

  TIOend(subname,1);

  return (irc);
}




/***************************************************************************************************
 *
 * iTIO_strcpy
 * iTIO_strncpy
 *
 **************************************************************************************************/
char *
iTIO_strcpy(char *s1, const char *s2)
{
  if (NULL == s2) {
    (void)strcpy(s1, "\0");
    return (NULL);
  }
  else {
    return (strcpy(s1, s2));
  }
}


char *
iTIO_strncpy(char *s1, const char *s2, size_t n)
{
  if (NULL == s2) {
    (void)strcpy(s1, "\0");
    return (NULL);
  }
  else {
    s1[n-1] = '\0';
    return (strncpy(s1, s2, (n-1)));
  }
}




/***************************************************************************************************
 *
 * iTIO_SizeOf
 *
 **************************************************************************************************/
size_t
iTIO_SizeOf( const TIO_Data_t datatype)
{
  const char *subname = "iTIO_SizeOf()";      /* Function name for debugging and error */


  switch (datatype) {

  case TIO_CHAR:
    return (sizeof(char));
    break;
  case TIO_SHORT:
    return (sizeof(short int));
    break;
  case TIO_INT:
    return (sizeof(int));
    break;
  case TIO_LONG:
    return (sizeof(long int));
    break;
  case TIO_LLONG:
    return (sizeof(long long int));
    break;

  case TIO_LOGICAL:
    return (sizeof(unsigned int));
    break;

  case TIO_UCHAR:
    return (sizeof(unsigned char));
    break;
  case TIO_USHORT:
    return (sizeof(unsigned short));
    break;
  case TIO_UINT:
    return (sizeof(unsigned int));
    break;
  case TIO_ULONG:
    return (sizeof(unsigned long));
    break;
  case TIO_ULLONG:
    return (sizeof(unsigned long long));
    break;

  case TIO_FLOAT:
    return (sizeof(float));
    break;
  case TIO_DOUBLE:
    return (sizeof(double));
    break;
  case TIO_LDOUBLE:
    return (sizeof(long double));
    break;

  case TIO_STRING:
    return (sizeof(char) * TIO_STRLEN);
    break;

  case TIO_ENUM_T:
    return (sizeof(TIO_Data_t));  /* any old typedef enum */
    break;
  case TIO_SIZE_T:
    return (sizeof(TIO_Size_t));
    break;
  case TIO_STEP_T:
    return (sizeof(TIO_Step_t));
    break;
  case TIO_TIME_T:
    return (sizeof(TIO_Time_t));
    break;

  default:
    return (0);   /*  Return 0 if datatype not found
                      Can't return -ve as return type is size_t  */
    break;
  }
}




/***************************************************************************************************
 *
 * cTIO_CheckDatatype
 *
 **************************************************************************************************/
TIO_t
cTIO_CheckDatatype( const char            *subname,
                    const char            *argname,
                    const int             ignore,
                    const TIO_Data_t      datatype,
                    const TIO_TypeClass_t typeclass )
{
  TIO_t           trc;
  TIO_TypeClass_t ltclass;
  char            errstr[128];



  if (ignore) {
    return (TIO_SUCCESS);
  }


  if (iTIO_SizeOf(datatype) == 0) {
    snprintf(errstr, sizeof(errstr), "Invalid arg (%s)", argname);
    TIOreturn(ERR_USER, errstr, TIO_ERR_INVALID_DATATYPE);
  }

  /* 
   * Note: CheckDatatype used mainly for materials/extended, treat all unsigned int 
   * as signed. 
   */
  trc = TIO_Get_TypeClass(datatype, &ltclass);
  if (TIO_TYPECLASS_UNSIGNED_INT == ltclass) {
    ltclass = TIO_TYPECLASS_SIGNED_INT;
  }

  if (ltclass != typeclass) {
    snprintf(errstr, sizeof(errstr), "Invalid arg (%s) - of wrong typeclass", argname);
    TIOreturn(ERR_USER, errstr, TIO_ERR_INVALID_DATATYPE);
  }


  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * cTIO_CheckGhosts
 *
 **************************************************************************************************/
TIO_t
cTIO_CheckGhosts( const char         *subname,
                  const TIO_File_t   fileID,
                  const TIO_Object_t meshID,
                  struct iMesh_t     *tmesh,
                  const TIO_Size_t   nghosts,
                  const TIO_Size_t   objnghosts,
                  TIO_Size_t         *outnghosts,
                  const int          reading )
{
  const TIO_t err = TIO_ERR_INVALID_ARG;

  TIO_Size_t  meshghosts;
  TIO_t       trc;

  TIO_Mesh_t  meshtype = tmesh->meshtype;



  *outnghosts = nghosts;   /* Default nghosts assignment - can be changed in logic below */

  if (reading) {

    switch (meshtype) {

    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR:
      if (TIO_GHOSTS_INTERNAL == nghosts) {
        /* ok */
      }
      else if (TIO_GHOSTS_ALL == nghosts) {
        *outnghosts = objnghosts;
      }
      else if (nghosts > objnghosts) {
        TIOreturn(ERR_USER, "Invalid nghosts - cannot read more ghosts than stored", err);
      }
      else {
        /* ok */
        ;
      }
      break;

    case TIO_MESH_UNSTRUCT:

      if (TIO_GHOSTS_ALL == nghosts) {
        trc = cTIO_GetMeshGhosts(subname, fileID, meshID, meshtype, &meshghosts);

        if ((TIO_GHOSTS_ALL == meshghosts) && (TIO_GHOSTS_NONE == objnghosts)) {
          TIOreturn(ERR_USER, "Invalid nghosts - cannot read more ghosts than stored", err);
        }
      }
      else if ((TIO_GHOSTS_NONE == nghosts) || (TIO_GHOSTS_INTERNAL == nghosts)) {
        /* ok */
        ;
      }
      else {
        TIOreturn(ERR_USER, "Invalid nghosts - use TIO_GHOSTS_ALL or TIO_GHOSTS_NONE", err);
      }
      break;

    case TIO_MESH_POINT:

      if (TIO_GHOSTS_ALL == nghosts) {

        trc = cTIO_GetMeshGhosts(subname, fileID, meshID, TIO_MESH_POINT, &meshghosts);

        if ((TIO_GHOSTS_ALL == meshghosts) && (TIO_GHOSTS_NONE  == objnghosts)) {
          TIOreturn(ERR_USER, "Invalid nghosts - cannot read more ghosts than stored", err);
        }
      }
      else if (nghosts != TIO_GHOSTS_NONE) {
        TIOreturn(ERR_USER, "Invalid nghosts - must use TIO_GHOSTS_ALL or TIO_GHOSTS_NONE", err);
      }
      break;

    default:
      TIOreturn(ERR_INT, "Invalid meshtype encountered", TIO_ERR_INT);
      break;
    }

  }
  else {   /* (!reading)  ie. writing */

    switch (meshtype) {

    case TIO_MESH_QUAD_COLINEAR:
      if ( (TIO_GHOSTS_ALL == nghosts) || (TIO_GHOSTS_INTERNAL == nghosts) ) {
        TIOreturn(ERR_USER, "Invalid value given for nghosts", err);
      }
      break;

    case TIO_MESH_QUAD_NONCOLINEAR:
      if (TIO_GHOSTS_ALL == nghosts) {
        *outnghosts = tmesh->info.quad.nghosts;
      }
      else if (TIO_GHOSTS_INTERNAL == nghosts) {
        TIOreturn(ERR_USER, "Invalid value given for nghosts", err);
      }
      if (*outnghosts > tmesh->info.quad.nghosts) {
        TIOreturn(ERR_USER, "nghosts must not be greater than that of parent mesh", err);
      }
      break;

    case TIO_MESH_UNSTRUCT:
    case TIO_MESH_POINT:
      if ((nghosts != TIO_GHOSTS_NONE) && (nghosts != TIO_GHOSTS_ALL)) {
        TIOreturn(ERR_USER, "nghosts must be TIO_GHOSTS_NONE or TIO_GHOSTS_ALL", err);
      }
      break;

    default:
      TIOreturn(ERR_INT, "Invalid meshtype encountered", TIO_ERR_INT);
      break;
    }

  }

  return (TIO_SUCCESS);
}



/***************************************************************************************************
 **************************************************************************************************/
iDataPos_t
iTIO_Centring2Datapos( const TIO_Centre_t centring,
                       const TIO_Mesh_t   meshtype,
                       const int          ismixed )
{
  const char *subname = "iTIO_Centring2Datapos()";


  switch (meshtype) {

  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:
    /* All centrings allowed */
    break;

  case TIO_MESH_UNSTRUCT:
    /* Only nodal and cell centring allowed */
    if ((centring != TIO_CENTRE_NODE) && (centring != TIO_CENTRE_CELL)) {
      return (DATAPOS_NULL);
    }
    break;

  case TIO_MESH_POINT:
    /* Only nodal centring allowed */
    if (centring != TIO_CENTRE_NODE) {
      return (DATAPOS_NULL);
    }
    break;

  default:
    TIOassert(TRUE, ERR_INT, "Invalid meshtype", DATAPOS_NULL);
    break;
  }

  if (ismixed) {
    if (TIO_CENTRE_CELL == centring) {
      return (DATAPOS_MIXED);
    }
    else {
      return (DATAPOS_NULL);
    }
  }

  return ((iDataPos_t)centring);
}



/***************************************************************************************************
 **************************************************************************************************/
int
iTIO_DataposAdjust( const iDataPos_t datapos, int *dni, int *dnj, int *dnk )
{
  /* Returns the dni, dnj, dnk required to convert from node-centred to other centring, using:
   *
   *   ni = Nni - dni
   *   nj = Nnj - dnj
   *   nk = Nnk - dnk
   *
   *   ni,  nj,  nk  = count for centring
   *   Nni, Nnj, Nnk = nodal count
   */


  switch (datapos) {
  case DATAPOS_CELL:
  case DATAPOS_MIXED:
    *dni = 1;
    *dnj = 1;
    *dnk = 1;
    break;
  case DATAPOS_NODE:
    *dni = 0;
    *dnj = 0;
    *dnk = 0;
    break;
  case DATAPOS_EDGE_I:
    *dni = 1;
    *dnj = 0;
    *dnk = 0;
    break;
  case DATAPOS_EDGE_J:
    *dni = 0;
    *dnj = 1;
    *dnk = 0;
    break;
  case DATAPOS_EDGE_K:
    *dni = 0;
    *dnj = 0;
    *dnk = 1;
    break;
  case DATAPOS_FACE_I:
    *dni = 0;
    *dnj = 1;
    *dnk = 1;
    break;
  case DATAPOS_FACE_J:
    *dni = 1;
    *dnj = 0;
    *dnk = 1;
    break;
  case DATAPOS_FACE_K:
    *dni = 1;
    *dnj = 1;
    *dnk = 0;
    break;
  default:
    return (-1);
    break;
  }

  return (0);
}



/***************************************************************************************************
 **************************************************************************************************/
int
iTIO_SetSection( const TIO_Mesh_t meshtype,
                 const TIO_Dims_t ndims,
                 const TIO_Size_t il,
                 const TIO_Size_t ih,
                 const TIO_Size_t jl,
                 const TIO_Size_t jh,
                 const TIO_Size_t kl,
                 const TIO_Size_t kh,
                 const TIO_Size_t nn,
                 const TIO_Size_t *IDs,
                 union iSection_t *section )
{
  const char *subname = "iTIO_SetSection()";


  switch (meshtype) {

  case TIO_MESH_QUAD_COLINEAR:
  case TIO_MESH_QUAD_NONCOLINEAR:

    /* section->quad.il = 0;
       section->quad.ih = 0;    - can assume >= 1D */

    section->quad.jl = 0;
    section->quad.jh = 0;
    section->quad.kl = 0;
    section->quad.kh = 0;

    switch (ndims) {
    case TIO_3D:
      if (kl > kh) {
        TIOreturn(ERR_USER, "kl:kh range is invalid", ITIO_ERR_ARG);
      }
      section->quad.kl = kl;
      section->quad.kh = kh;
      /* drop through */

    case TIO_2D:
      if (jl > jh) {
        TIOreturn(ERR_USER, "jl:jh range is invalid", ITIO_ERR_ARG);
      }
      section->quad.jl = jl;
      section->quad.jh = jh;
      /* drop through */

    case TIO_1D:
      if (il > ih) {
        TIOreturn(ERR_USER, "il:ih range is invalid", ITIO_ERR_ARG);
      }
      section->quad.il = il;
      section->quad.ih = ih;

      break;

    default:
      TIOreturn(ERR_INT, "Invalid ndims", -1);
      break;
    }

    break;

  case TIO_MESH_UNSTRUCT:

    /* Do some check that nn <= tmesh->nnodes/ncells
       and IDs[i] <> min/max node/cell ID */

    section->unstr.nIDs = nn;
    section->unstr.IDs  = (TIO_Size_t *)IDs;

    break;

  case TIO_MESH_POINT:

    /* Do some check that nn <= tmesh->nnodes
       and IDs[i] <> min/max node ID */

    section->point.nIDs = nn;
    section->point.IDs  = (TIO_Size_t *)IDs;

    break;

  default:
    TIOreturn(ERR_INT, "Invalid meshtype", ITIO_ERR_MESHTYPE);
    break;
  }


  return (0);
}



/***************************************************************************************************
 *
 * Checks whether a given section is defined for the given mesh
 *
 **************************************************************************************************/
int iTIO_CheckSection( const struct iMesh_t *tmesh,
                       const TIO_Object_t sectionID )
{
  TIO_Size_t i;
  int        found;

  if (NULL == tmesh->sectionCache) {
    return (FALSE);
  }

  for (i=0; i<tmesh->sectionCache->nlist; ++i) {
    found = (tmesh->sectionCache->sectionID[i] == sectionID);
    if (found) {
      break;
    }
  }

  return (found);
}


/***************************************************************************************************
 **************************************************************************************************/
TIO_t
cTIO_CheckRange( const char           *subname,
                 const struct iFile_t *tfile,
                 const TIO_Data_t     datatype,
                 const void           *minval,
                 const void           *maxval )
{
  TIO_TypeClass_t typeclass;
  size_t          dsi;

  int   irc;
  TIO_t trc;


  /*    Do this by converting the value given into the biggest datatype of the same class and
        checking that qmax > qmin */


  dsi = iTIO_SizeOf(datatype);
  TIOassert((0 == dsi), ERR_INT, "Invalid datatype", TIO_ERR_INVALID_DATATYPE);

  trc = TIO_Get_TypeClass(datatype, &typeclass);
  TIOassert(trc != TIO_SUCCESS, ERR_INT, "Failed to get typeclass of quant datatype", TIO_ERR_INT);

  trc = TIO_SUCCESS;

  switch (typeclass) {
  case TIO_TYPECLASS_SIGNED_INT:
    {
      signed long long int sllmin, sllmax;

      (void) memcpy(&sllmin, minval, dsi);
      (void) memcpy(&sllmax, maxval, dsi);

      irc = tfile->ConvBuf(datatype, TIO_LLONG, 1, &sllmin);
      irc = tfile->ConvBuf(datatype, TIO_LLONG, 1, &sllmax);

      if (sllmin > sllmax) {
        trc = TIO_ERR_INVALID_RANGE;
      }
    }
    break;

  case TIO_TYPECLASS_UNSIGNED_INT:
    {
      unsigned long long int ullmin, ullmax;

      (void) memcpy(&ullmin, minval, dsi);
      (void) memcpy(&ullmax, maxval, dsi);

      irc = tfile->ConvBuf(datatype, TIO_ULLONG, 1, &ullmin);
      irc = tfile->ConvBuf(datatype, TIO_ULLONG, 1, &ullmax);

      if (ullmin > ullmax) {
        trc = TIO_ERR_INVALID_RANGE;
      }
    }
    break;

  case TIO_TYPECLASS_FLOAT:
    {
      long double ldmin, ldmax;

      (void) memcpy(&ldmin, minval, dsi);
      (void) memcpy(&ldmax, maxval, dsi);

      irc = tfile->ConvBuf(datatype, TIO_LDOUBLE, 1, &ldmin);
      irc = tfile->ConvBuf(datatype, TIO_LDOUBLE, 1, &ldmax);

      if (ldmin > ldmax) {
        trc = TIO_ERR_INVALID_RANGE;
      }
    }
    break;

  default:
    TIOreturn(ERR_INT, "Invalid typeclass", TIO_ERR_INT);
    break;
  }


  return (trc);
}



/*
 * EOF
 */

/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tiof_fkit.h"
#include "typhonio.h"


/***************************************************************************************************
 *
 *   tio_sizeof_c
 *
 **************************************************************************************************/
void
F2C(tio_sizeof_c,TIO_SIZEOF_C)
     ( const int_f  *datatype,
       int_f        *val )
{
  *val = (int_f) TIO_SizeOf( f2c_datatype(*datatype) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_typeclass_c
 *
 **************************************************************************************************/
void
F2C(tio_get_typeclass_c,TIO_GET_TYPECLASS_C)
     ( const int_f *datatype,
       int_f       *typeclass,
       TIO_tf      *err )
{
  TIO_TypeClass_t ltclass;
  
  *err = c2f_TIO( TIO_Get_TypeClass( f2c_datatype(*datatype),
                                     &ltclass ) );
  
  if (*err != TIO_SUCCESS_F) return;
    
  *typeclass = c2f_typeclass(ltclass);
  
  return;
}



/*
 * EOF
 */

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
 *   tio_get_error_c
 *
 **************************************************************************************************/
void
F2C(tio_get_error_c,TIO_GET_ERROR_C)
     ( const TIO_tf *errcode,
       char         *str,
       const int_f  *str_len,
       TIO_tf       *err )
{
  TIO_t l_errcode;
  char  l_str[TIO_STRLEN];
  
  
  l_errcode = (TIO_t)*errcode;
  
    
  *err = c2f_TIO( TIO_Get_Error(f2c_TIO(*errcode), l_str) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  c2f_str(l_str, str_len, str);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_set_verbose_c
 *
 **************************************************************************************************/
void
F2C(tio_set_verbose_c,TIO_SET_VERBOSE_C)
     ( const int_f *mode,
       TIO_tf      *err )
{
  *err = c2f_TIO( TIO_Set_Verbose( f2c_verbosemode(*mode) ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_verbose_c
 *
 **************************************************************************************************/
void
F2C(tio_get_verbose_c,TIO_GET_VERBOSE_C)
     ( int_f  *mode,
       TIO_tf *err )
{
  TIO_Verbose_t lmode;

  *err = c2f_TIO( TIO_Get_Verbose(&lmode) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *mode = c2f_verbosemode(lmode);
  
  return;
}



/*
 * EOF
 */

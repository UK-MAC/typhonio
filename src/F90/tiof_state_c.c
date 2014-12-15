/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tiof_fkit.h"
#include "typhonio.h"



static void xtioc_copystateinfo( const char       *name,
                                 const TIO_Step_t step,
                                 const TIO_Time_t time,
                                 const char       *units,
                                 char             *fname,  const int_f *fname_len,
                                 TIO_Step_tf      *fstep,
                                 TIO_Time_tf      *ftime,
                                 char             *funits, const int_f *funits_len );

     

/***************************************************************************************************
 *
 *   tio_list_states_c
 *
 **************************************************************************************************/
void
F2C(tio_list_states_c,TIO_LIST_STATES_C)
     ( const TIO_File_tf *fileID,
       TIO_Size_tf       *nobj,
       char              *names,
       TIO_tf            *err )
{
  TIO_Size_t l_nobj;
  char       *fnptr, ccpy[TIO_STRLEN];
  int        len=TIO_STRLEN, *lptr = &len;
  TIO_Size_t i;
  
  
  
  if (*nobj > 0) {
    
    *err = c2f_TIO( TIO_List_States( f2c_file(*fileID),
                                     &l_nobj,
                                     (char(*)[TIO_STRLEN])names ) );

    if (*err != TIO_SUCCESS_F) return;
    
    fnptr = names;
    for (i=0; i<l_nobj; ++i) {
      strcpy(ccpy, fnptr);
      c2f_str(ccpy, lptr, fnptr);
      fnptr += TIO_STRLEN_F;
    }
    
  }
  else {
    *err = c2f_TIO( TIO_List_States( f2c_file(*fileID),
                                     &l_nobj,
                                     NULL ) );
    
    if (*err != TIO_SUCCESS_F) return;
  }
  
  *nobj = c2f_size(l_nobj);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_create_state_c
 *
 **************************************************************************************************/
void
F2C(tio_create_state_c,TIO_CREATE_STATE_C)
     ( const TIO_File_tf *fileID,
       const char        *name,    const int_f *name_len,
       TIO_Object_tf     *stateID,
       const TIO_Step_tf *step,
       const TIO_Time_tf *time,
       const char        *units,   const int_f *units_len,
       TIO_tf            *err )
{
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Object_t l_stateID;
  
  
  
  f2c_str(name,  name_len,  l_name);
  f2c_str(units, units_len, l_units);
  
  *err = c2f_TIO( TIO_Create_State( f2c_file(*fileID),
                                    l_name,
                                    &l_stateID,
                                    f2c_step(*step),
                                    f2c_time(*time),
                                    l_units ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *stateID = c2f_object(l_stateID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_open_state_c
 *
 **************************************************************************************************/
void
F2C(tio_open_state_c,TIO_OPEN_STATE_C)
     ( const TIO_File_tf *fileID,
       const TIO_Size_tf *idx,
       char              *name,    const int_f *name_len,
       TIO_Object_tf     *stateID,
       TIO_Step_tf       *step,
       TIO_Time_tf       *time,
       char              *units,   const int_f *units_len,
       TIO_tf            *err )
{
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Object_t l_stateID;
  TIO_Step_t   l_step;
  TIO_Time_t   l_time;
  
  
  
  if (*idx > 0) {
    *err = c2f_TIO( TIO_Open_State_idx( f2c_file(*fileID),
                                        f2c_idx(*idx),
                                        l_name,
                                        &l_stateID,
                                        &l_step,
                                        &l_time,
                                        l_units ) );
    
    if (*err != TIO_SUCCESS_F) return;
    
    c2f_str(l_name, name_len, name);
  }
  else {
    f2c_str(name, name_len, l_name);
    
    *err = c2f_TIO( TIO_Open_State( f2c_file(*fileID),
                                    l_name,
                                    &l_stateID,
                                    &l_step,
                                    &l_time,
                                    l_units ) );
    
    if (*err != TIO_SUCCESS_F) return;
  }
  
  (void)xtioc_copystateinfo(NULL,       l_step, l_time, l_units,
                            NULL, NULL, step,   time,   units, units_len );
  
  *stateID = c2f_object(l_stateID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_state_info_c
 *
 **************************************************************************************************/
void
F2C(tio_get_state_info_c,TIO_GET_STATE_INFO_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *stateID,
       char                *name,  const int_f *name_len,
       TIO_Step_tf         *step,
       TIO_Time_tf         *time,
       char                *units, const int_f *units_len,
       TIO_tf              *err )
{
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Step_t   l_step;
  TIO_Time_t   l_time;
  
  
  *err = c2f_TIO( TIO_Get_State_Info( f2c_file(*fileID),
                                      f2c_object(*stateID),
                                      l_name,
                                      &l_step,
                                      &l_time,
                                      l_units ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  (void)xtioc_copystateinfo(l_name,         l_step, l_time, l_units,
                            name, name_len, step,   time,   units, units_len );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_close_state_c
 *
 **************************************************************************************************/
void
F2C(tio_close_state_c,TIO_CLOSE_STATE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *stateID,
       TIO_tf *err )
{
  *err = c2f_TIO( TIO_Close_State( f2c_file(*fileID),
                                   f2c_object(*stateID) ) );
  
  return;
}



/**************************************************************************************************/
static void
xtioc_copystateinfo( const char       *name,
                     const TIO_Step_t step,
                     const TIO_Time_t time,
                     const char       *units,
                     char             *fname,  const int_f *fname_len,
                     TIO_Step_tf      *fstep,
                     TIO_Time_tf      *ftime,
                     char             *funits, const int_f *funits_len )
{
  if (name != NULL) c2f_str(name, fname_len, fname);
  *fstep = c2f_step(step);
  *ftime = c2f_time(time);
  c2f_str(units, funits_len, funits);
  
  return;
}


/*
 * EOF
 */

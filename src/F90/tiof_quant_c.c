/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tiof_fkit.h"
#include "typhonio.h"



static void xtioc_copyquantinfo( const char         *name,
                                 const TIO_Data_t   datatype,
                                 const TIO_Centre_t centring,
                                 const TIO_Size_t   nghosts,
                                 const TIO_Bool_t   ismixed,
                                 const char         *units,
                                 char               *fname,  const int_f *fname_len,
                                 int_f              *fdatatype,
                                 int_f              *fcentring,
                                 TIO_Size_tf        *fnghosts,
                                 int_f              *fismixed,
                                 char               *funits, const int_f *funits_len );

     

/***************************************************************************************************
 *
 *   tio_list_quants_c
 *
 **************************************************************************************************/
void
F2C(tio_list_quants_c,TIO_LIST_QUANTS_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       TIO_Size_tf         *nobj,
       char                *names,
       TIO_tf              *err )
{
  TIO_Size_t l_nobj, i;
  char       *fnptr, ccpy[TIO_STRLEN];
  int        len=TIO_STRLEN, *lptr = &len;
  
  
  if (*nobj > 0) {
    
    *err = c2f_TIO( TIO_List_Quants( f2c_file(*fileID),
                                     f2c_object(*meshID),
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
    *err = c2f_TIO( TIO_List_Quants( f2c_file(*fileID),
                                     f2c_object(*meshID),
                                     &l_nobj,
                                     NULL ) );
    
    if (*err != TIO_SUCCESS_F) return;
  }
  
  *nobj = c2f_size(l_nobj);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_create_quant_c
 *
 **************************************************************************************************/
void
F2C(tio_create_quant_c,TIO_CREATE_QUANT_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const char          *name,
       const int_f         *name_len,
       TIO_Object_tf       *quantID,
       const int_f         *datatype,
       const int_f         *centring,
       const TIO_Size_tf   *nghosts,
       const int_f         *ismixed,
       const char          *units,
       const int_f         *units_len,
       TIO_tf              *err )
{
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Object_t l_quantID;
  TIO_Bool_t   l_ismixed;
  
  
  f2c_str(name, name_len, l_name);
  f2c_str(units, units_len, l_units);
  
  if (*ismixed > 0)
    l_ismixed = TIO_TRUE;
  else
    l_ismixed = TIO_FALSE;
  
  
  *err = c2f_TIO( TIO_Create_Quant( f2c_file(*fileID),
                                    f2c_object(*meshID),
                                    l_name,
                                    &l_quantID,
                                    f2c_datatype(*datatype),
                                    f2c_centre(*centring),
                                    f2c_ghosts(*nghosts),
                                    l_ismixed,
                                    l_units ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *quantID = c2f_object(l_quantID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_open_quant_c
 *
 **************************************************************************************************/
void
F2C(tio_open_quant_c,TIO_OPEN_QUANT_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const TIO_Size_tf   *idx,
       char                *name,
       const int_f         *name_len,
       TIO_Object_tf       *quantID,
       int_f               *datatype,
       int_f               *centring,
       TIO_Size_tf         *nghosts,
       int_f               *ismixed,
       char                *units,
       const int_f         *units_len,
       TIO_tf              *err )
{
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Object_t l_quantID;
  TIO_Data_t   l_datatype;
  TIO_Centre_t l_centring;
  TIO_Size_t   l_nghosts;
  TIO_Bool_t   l_ismixed;
  
  if (*idx > 0) {
    *err = c2f_TIO( TIO_Open_Quant_idx( f2c_file(*fileID),
                                        f2c_object(*meshID),
                                        f2c_idx(*idx),
                                        l_name,
                                        &l_quantID,
                                        &l_datatype,
                                        &l_centring,
                                        &l_nghosts,
                                        &l_ismixed,
                                        l_units ) );
    
    if (*err != TIO_SUCCESS_F) return;
    
    c2f_str(l_name, name_len, name);
  }
  else {
    f2c_str(name, name_len, l_name);
    
    *err = c2f_TIO( TIO_Open_Quant( f2c_file(*fileID),
                                    f2c_object(*meshID),
                                    l_name,
                                    &l_quantID,
                                    &l_datatype,
                                    &l_centring,
                                    &l_nghosts,
                                    &l_ismixed,
                                    l_units ) );
    
    if (*err != TIO_SUCCESS_F) return;
  }
  
  (void)xtioc_copyquantinfo( NULL,       l_datatype, l_centring, l_nghosts, l_ismixed, l_units,
                             NULL, NULL, datatype, centring, nghosts, ismixed, units, units_len );
  
  *quantID = c2f_object(l_quantID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_quant_info_c
 *
 **************************************************************************************************/
void
F2C(tio_get_quant_info_c,TIO_GET_QUANT_INFO_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       char                *name,
       const int_f         *name_len,
       int_f               *datatype,
       int_f               *centring,
       TIO_Size_tf         *nghosts,
       int_f               *ismixed,
       char                *units,
       const int_f         *units_len,
       TIO_tf              *err )
{
  char         l_name[TIO_STRLEN], l_units[TIO_STRLEN];
  TIO_Data_t   l_datatype;
  TIO_Centre_t l_centring;
  TIO_Size_t   l_nghosts;
  TIO_Bool_t   l_ismixed;  
  
  
  *err = c2f_TIO( TIO_Get_Quant_Info( f2c_file(*fileID),
                                      f2c_object(*quantID),
                                      l_name,
                                      &l_datatype,
                                      &l_centring,
                                      &l_nghosts,
                                      &l_ismixed,
                                      l_units ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  (void)xtioc_copyquantinfo(l_name,       l_datatype, l_centring, l_nghosts, l_ismixed, l_units,
                            name, name_len, datatype, centring, nghosts, ismixed, units, units_len);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_set_quant_range_c
 *
 **************************************************************************************************/
void
F2C(tio_set_quant_range_c,TIO_SET_QUANT_RANGE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const int_f         *datatype,
       const void          *qmin,
       const void          *qmax,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Set_Quant_Range( f2c_file(*fileID),
                                       f2c_object(*quantID),
                                       f2c_datatype(*datatype),
                                       qmin, qmax ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_quant_range_c
 *
 **************************************************************************************************/
void
F2C(tio_get_quant_range_c,TIO_GET_QUANT_RANGE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const int_f         *datatype,
       void                *qmin,
       void                *qmax,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Get_Quant_Range( f2c_file(*fileID),
                                       f2c_object(*quantID),
                                       f2c_datatype(*datatype),
                                       qmin, qmax ) );
  
  return;
}



/***************************************************************************************************
 *
 *   tio_close_quant_c
 *
 **************************************************************************************************/
void
F2C(tio_close_quant_c,TIO_CLOSE_QUANT_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       TIO_tf *err )
{
  *err = c2f_TIO( TIO_Close_Quant( f2c_file(*fileID),
                                   f2c_object(*quantID) ) );
  
  return;
}




/************************************
 ************************************
 **                                **
 **                                **
 **            QUAD                **
 **                                **
 **                                **
 ************************************
 ************************************/


/***************************************************************************************************
 *
 *   tio_write_quadquant_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_quadquant_chunk_c,TIO_WRITE_QUADQUANT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const void          *qdat,
       const void          *qmix,
       const int_f         *narg,
       TIO_tf              *err )
{
  const void *datptr, *mixptr;
  
  setpm(narg, datptr, mixptr, qdat, qmix);
  
  *err = c2f_TIO( TIO_Write_QuadQuant_Chunk( f2c_file(*fileID),
                                             f2c_object(*quantID),
                                             f2c_idx(*idx),
                                             f2c_xfer(*xfer),
                                             f2c_datatype(*datatype),
                                             datptr,
                                             mixptr ) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_quadquant_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadquant_chunk_c,TIO_READ_QUADQUANT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const TIO_Size_tf   *nghosts,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *qdat,
       TIO_Size_tf         *nmixcomp,
       void                *qmix,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk, l_nmc;
  void *datptr, *mixptr;
  
  
  setpm(narg, datptr, mixptr, qdat, qmix);
  
  *err = c2f_TIO( TIO_Read_QuadQuant_Chunk( f2c_file(*fileID),
                                                f2c_object(*quantID),
                                                f2c_idx(*idx),
                                                f2c_xfer(*xfer),
                                                f2c_datatype(*datatype),
                                                f2c_ghosts(*nghosts),
                                                &l_ndims,
                                                &l_ni,
                                                &l_nj,
                                                &l_nk,
                                                datptr,
                                                &l_nmc,
                                                mixptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims    = c2f_size(l_ndims);
  *ni       = c2f_size(l_ni);
  *nj       = c2f_size(l_nj);
  *nk       = c2f_size(l_nk);
  *nmixcomp = c2f_size(l_nmc);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_quadquant_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadquant_section_c,TIO_READ_QUADQUANT_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *qdat,
       TIO_Size_tf         *nmixcomp,
       void                *qmix,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk, l_nmc;
  void *datptr, *mixptr;
    
  setpm(narg, datptr, mixptr, qdat, qmix);
  
  *err = c2f_TIO( TIO_Read_QuadQuant_Section( f2c_file(*fileID),
                                              f2c_object(*quantID),
                                              f2c_object(*sectionID),
                                              f2c_xfer(*xfer),
                                              f2c_datatype(*datatype),
                                              &l_ndims, &l_ni, &l_nj, &l_nk,
                                              datptr,
                                              &l_nmc,
                                              mixptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims    = c2f_ndims(l_ndims);
  *ni       = c2f_size(l_ni);
  *nj       = c2f_size(l_nj);
  *nk       = c2f_size(l_nk);
  *nmixcomp = c2f_size(l_nmc);

  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_quadquant_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadquant_all_c,TIO_READ_QUADQUANT_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *qdat,
       TIO_Size_tf         *nmixcomp,
       void                *qmix,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk, l_nmc;
  void *datptr, *mixptr;
  
  
  setpm(narg, datptr, mixptr, qdat, qmix);
  
  *err = c2f_TIO( TIO_Read_QuadQuant_All( f2c_file(*fileID),
                                          f2c_object(*quantID),
                                          f2c_datatype(*datatype),
                                          &l_ndims,
                                          &l_ni,
                                          &l_nj,
                                          &l_nk,
                                          datptr,
                                          &l_nmc,
                                          mixptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims    = c2f_size(l_ndims);
  *ni       = c2f_size(l_ni);
  *nj       = c2f_size(l_nj);
  *nk       = c2f_size(l_nk);
  *nmixcomp = c2f_size(l_nmc);
  
  return;
}





/************************************
 ************************************
 **                                **
 **                                **
 **            UNSTR               **
 **                                **
 **                                **
 ************************************
 ************************************/


/***************************************************************************************************
 *
 *   tio_write_unstrquant_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_unstrquant_chunk_c,TIO_WRITE_UNSTRQUANT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const void          *qdat,
       const void          *qmix,
       const int_f         *narg,
       TIO_tf              *err )
{
  const void *datptr, *mixptr;
  
  setpm(narg, datptr, mixptr, qdat, qmix);
  
  *err = c2f_TIO( TIO_Write_UnstrQuant_Chunk( f2c_file(*fileID),
                                              f2c_object(*quantID),
                                              f2c_idx(*idx),
                                              f2c_xfer(*xfer),
                                              f2c_datatype(*datatype),
                                              datptr,
                                              mixptr ) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_unstrquant_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrquant_chunk_c,TIO_READ_UNSTRQUANT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const TIO_Size_tf   *nghosts,
       TIO_Size_tf         *nn,
       void                *qdat,
       TIO_Size_tf         *nmixcomp,
       void                *qmix,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn, l_nmc;
  void *datptr, *mixptr;
  
  
  setpm(narg, datptr, mixptr, qdat, qmix);
  
  *err = c2f_TIO( TIO_Read_UnstrQuant_Chunk( f2c_file(*fileID),
                                             f2c_object(*quantID),
                                             f2c_idx(*idx),
                                             f2c_xfer(*xfer),
                                             f2c_datatype(*datatype),
                                             f2c_ghosts(*nghosts),
                                             &l_nn,
                                             datptr,
                                             &l_nmc,
                                             mixptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nn       = c2f_size(l_nn);
  *nmixcomp = c2f_size(l_nmc);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_unstrquant_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrquant_all_c,TIO_READ_UNSTRQUANT_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const int_f         *datatype,
       TIO_Size_tf         *nn,
       void                *qdat,
       TIO_Size_tf         *nmixcomp,
       void                *qmix,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn, l_nmc;
  void *datptr, *mixptr;
  
  
  setpm(narg, datptr, mixptr, qdat, qmix);
  
  *err = c2f_TIO( TIO_Read_UnstrQuant_All( f2c_file(*fileID),
                                           f2c_object(*quantID),
                                           f2c_datatype(*datatype),
                                           &l_nn,
                                           datptr,
                                           &l_nmc,
                                           mixptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nn       = c2f_size(l_nn);
  *nmixcomp = c2f_size(l_nmc);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_unstrquant_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrquant_section_c,TIO_READ_UNSTRQUANT_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       TIO_Size_tf         *nn,
       void                *qdat,
       TIO_Size_tf         *nmixcomp,
       void                *qmix,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn, l_nmc;
  void *datptr, *mixptr;
    
  setpm(narg, datptr, mixptr, qdat, qmix);
  
  *err = c2f_TIO( TIO_Read_UnstrQuant_Section( f2c_file(*fileID),
                                               f2c_object(*quantID),
                                               f2c_object(*sectionID),
                                               f2c_xfer(*xfer),
                                               f2c_datatype(*datatype),
                                               &l_nn,
                                               datptr,
                                               &l_nmc,
                                               mixptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nn       = c2f_size(l_nn);
  *nmixcomp = c2f_size(l_nmc);
  
  
  return;
}




/************************************
 ************************************
 **                                **
 **                                **
 **            POINT               **
 **                                **
 **                                **
 ************************************
 ************************************/


/***************************************************************************************************
 *
 *   tio_write_pointquant_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_pointquant_chunk_c,TIO_WRITE_POINTQUANT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const void          *qdat,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Write_PointQuant_Chunk( f2c_file(*fileID),
                                              f2c_object(*quantID),
                                              f2c_idx(*idx),
                                              f2c_xfer(*xfer),
                                              f2c_datatype(*datatype),
                                              qdat ) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointquant_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointquant_chunk_c,TIO_READ_POINTQUANT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const TIO_Size_tf   *nghosts,
       TIO_Size_tf         *nnodes,
       void                *qdat,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn;
  void       *qptr;
  
  
  setpm(narg, qptr, qptr, qdat, qdat);
  
  *err = c2f_TIO( TIO_Read_PointQuant_Chunk( f2c_file(*fileID),
                                             f2c_object(*quantID),
                                             f2c_idx(*idx),
                                             f2c_xfer(*xfer),
                                             f2c_datatype(*datatype),
                                             f2c_ghosts(*nghosts),
                                             &l_nn,
                                             qptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nnodes = c2f_size(l_nn);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointquant_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointquant_all_c,TIO_READ_POINTQUANT_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const int_f         *datatype,
       TIO_Size_tf         *nnodes,
       void                *qdat,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn;
  void       *qptr;
  
  
  setpm(narg, qptr, qptr, qdat, qdat);
  
  *err = c2f_TIO( TIO_Read_PointQuant_All( f2c_file(*fileID),
                                           f2c_object(*quantID),
                                           f2c_datatype(*datatype),
                                           &l_nn,
                                           qptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nnodes = c2f_size(l_nn);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointquant_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointquant_section_c,TIO_READ_POINTQUANT_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *quantID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       TIO_Size_tf         *nnodes,
       void                *qdat,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn;
  void       *qptr;
  
  setpm(narg, qptr, qptr, qdat, qdat);
  
  *err = c2f_TIO( TIO_Read_PointQuant_Section( f2c_file(*fileID),
                                               f2c_object(*quantID),
                                               f2c_object(*sectionID),
                                               f2c_xfer(*xfer),
                                               f2c_datatype(*datatype),
                                               &l_nn,
                                               qptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nnodes = c2f_size(l_nn);
  
  
  return;
}




/**************************************************************************************************/
static void
xtioc_copyquantinfo( const char         *name,
                     const TIO_Data_t   datatype,
                     const TIO_Centre_t centring,
                     const TIO_Size_t   nghosts,
                     const TIO_Bool_t   ismixed,
                     const char         *units,
                     char               *fname,  const int_f *fname_len,
                     int_f              *fdatatype,
                     int_f              *fcentring,
                     TIO_Size_tf        *fnghosts,
                     int_f              *fismixed,
                     char               *funits, const int_f *funits_len )
{
  if (name != NULL) c2f_str(name, fname_len, fname);
  
  *fdatatype = c2f_datatype(datatype);
  *fcentring = c2f_centre(centring);
  *fnghosts  = c2f_ghosts(nghosts);
  
  if (ismixed == TIO_TRUE)
    *fismixed = 1;
  else
    *fismixed = 0;
  
  c2f_str(units, funits_len, funits);
  
  return;
}



/*
 * EOF
 */

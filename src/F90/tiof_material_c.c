/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tiof_fkit.h"
#include "typhonio.h"



static void xtioc_copymatinfo( const char       *name,
                               const TIO_Data_t datatype,
                               const TIO_Size_t nmat,
                               const TIO_Size_t nghosts,
                               const TIO_Bool_t ismixed,
                               const TIO_Data_t ncomp_datatype,
                               const TIO_Data_t ptr_datatype,
                               const TIO_Data_t vf_datatype,
                               char             *fname, const int_f *fname_len,
                               int_f            *fdatatype,
                               TIO_Size_tf      *fnmat,
                               TIO_Size_tf      *fnghosts,
                               int_f            *fismixed,
                               int_f            *fncomp_datatype,
                               int_f            *fptr_datatype,
                               int_f            *fvf_datatype );
  

/***************************************************************************************************
 *
 *   tio_list_mat_c
 *
 **************************************************************************************************/
void
F2C(tio_list_mat_c,TIO_LIST_MAT_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       char                *name,
       TIO_tf              *err )
{
  char       l_name[TIO_STRLEN];
  int        len=TIO_STRLEN, *lptr = &len;
  
  
  *err = c2f_TIO( TIO_List_Material( f2c_file(*fileID),
                                     f2c_object(*meshID),
                                     l_name ) );

  if (*err != TIO_SUCCESS_F) return;
  
  c2f_str(l_name, lptr, name);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_create_mat_c
 *
 **************************************************************************************************/
void
F2C(tio_create_mat_c,TIO_CREATE_MAT_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *meshID,
       const char          *name, const int_f *name_len,
       TIO_Object_tf       *materialID,
       const int_f         *datatype,
       const TIO_Size_tf   *nmat,
       const TIO_Size_tf   *nghosts,
       const int_f         *ismixed,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       TIO_tf              *err )
{
  char         l_name[TIO_STRLEN];
  TIO_Object_t l_materialID;
  TIO_Bool_t   l_ismixed;
  
  
  f2c_str(name, name_len, l_name);
  
  if (*ismixed > 0)
    l_ismixed = TIO_TRUE;
  else
    l_ismixed = TIO_FALSE;
  
  
  *err = c2f_TIO( TIO_Create_Material( f2c_file(*fileID),
                                       f2c_object(*meshID),
                                       l_name,
                                       &l_materialID,
                                       f2c_datatype(*datatype),
                                       f2c_size(*nmat),
                                       f2c_ghosts(*nghosts),
                                       l_ismixed,
                                       f2c_datatype(*ncomp_datatype),
                                       f2c_datatype(*ptr_datatype),
                                       f2c_datatype(*vf_datatype) ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *materialID = c2f_object(l_materialID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_open_mat_c
 *
 **************************************************************************************************/
void
F2C(tio_open_mat_c,TIO_OPEN_MAT_C)
     ( const TIO_File_tf  *fileID,
       const TIO_Object_t *meshID,
       char               *name, const int_f *name_len,
       TIO_Object_tf      *materialID,
       int_f              *datatype,
       TIO_Size_tf        *nmat,
       TIO_Size_tf        *nghosts,
       int_f              *ismixed,
       int_f              *ncomp_datatype,
       int_f              *ptr_datatype,
       int_f              *vf_datatype,
       TIO_tf             *err )
{
  char         l_name[TIO_STRLEN];
  TIO_Object_t l_materialID;
  TIO_Size_t   l_nmat, l_nghosts;
  TIO_Bool_t   l_ismixed;
  TIO_Data_t   l_md, l_nd, l_pd, l_vd;
  
  
  f2c_str(name, name_len, l_name);
  
  *err = c2f_TIO( TIO_Open_Material( f2c_file(*fileID),
                                     f2c_object(*meshID),
                                     l_name,
                                     &l_materialID,
                                     &l_md,
                                     &l_nmat,
                                     &l_nghosts,
                                     &l_ismixed,
                                     &l_nd, &l_pd, &l_vd ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  (void)xtioc_copymatinfo(NULL,       l_md,     l_nmat, l_nghosts, l_ismixed, l_nd, l_pd, l_vd,
                               NULL, NULL, datatype, nmat,   nghosts,   ismixed,
                               ncomp_datatype, ptr_datatype, vf_datatype);
  
  *materialID = c2f_object(l_materialID);
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_mat_info_c
 *
 **************************************************************************************************/
void
F2C(tio_get_mat_info_c,TIO_GET_MAT_INFO_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       char                *name, const int_f *name_len,
       int_f               *datatype,
       TIO_Size_tf         *nmat,
       TIO_Size_tf         *nghosts,
       int_f               *ismixed,
       int_f               *ncomp_datatype,
       int_f               *ptr_datatype,
       int_f               *vf_datatype,
       TIO_tf              *err )
{
  char         l_name[TIO_STRLEN];
  TIO_Size_t   l_nmat, l_nghosts;
  TIO_Bool_t   l_ismixed;
  TIO_Data_t   l_md, l_nd, l_pd, l_vd;
  
  
  *err = c2f_TIO( TIO_Get_Material_Info( f2c_file(*fileID),
                                         f2c_object(*materialID),
                                         l_name,
                                         &l_md,
                                         &l_nmat,
                                         &l_nghosts,
                                         &l_ismixed,
                                         &l_nd, &l_pd, &l_vd ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  (void)xtioc_copymatinfo(l_name,         l_md,     l_nmat, l_nghosts, l_ismixed,
                               l_nd,           l_pd,         l_vd,
                               name, name_len, datatype, nmat,   nghosts,   ismixed,
                               ncomp_datatype, ptr_datatype, vf_datatype);
  return;
}



/***************************************************************************************************
 *
 *   tio_set_mat_range_c
 *
 **************************************************************************************************/
void
F2C(tio_set_mat_range_c,TIO_SET_MAT_RANGE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const int_f         *datatype,
       const TIO_Size_tf   *nmat,
       const void          *matnums,
       char                *matnames, const int_f *matnames_len,
       TIO_tf              *err )
{
  TIO_Size_t l_nmat, i;
  char       *fnptr;
  
  
  if (*nmat > 0) {
    l_nmat = f2c_size(*nmat);
    
    fnptr = matnames;
    for (i=0; i<l_nmat; ++i) {
      f2c_str(fnptr, &matnames_len[i], fnptr);
      fnptr += TIO_STRLEN_F;
    }
    
    *err = c2f_TIO( TIO_Set_Material_Range( f2c_file(*fileID),
                                            f2c_object(*materialID),
                                            f2c_datatype(*datatype),
                                            l_nmat,
                                            matnums,
                                            (char(*)[TIO_STRLEN])matnames ) );

  }
  else {
    l_nmat = f2c_size(-(*nmat));
    
    *err = c2f_TIO( TIO_Set_Material_Range( f2c_file(*fileID),
                                            f2c_object(*materialID),
                                            f2c_datatype(*datatype),
                                            l_nmat,
                                            matnums,
                                            NULL ) );
  }
  
  
  return;
}



/***************************************************************************************************
 *
 *   tio_get_mat_range_c
 *
 **************************************************************************************************/
void
F2C(tio_get_mat_range_c,TIO_GET_MAT_RANGE_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const int_f         *datatype,
       TIO_Size_tf         *nmat,
       void                *matnums,
       char                *matnames,
       TIO_tf              *err )
{
  TIO_Size_t l_nmat, i;
  char       *fnptr, ccpy[TIO_STRLEN];
  int        len=TIO_STRLEN, *lptr = &len;
  
  
  if (*nmat > 0) {
    
    *err = c2f_TIO( TIO_Get_Material_Range( f2c_file(*fileID),
                                            f2c_object(*materialID),
                                            f2c_datatype(*datatype),
                                            &l_nmat,
                                            NULL,
                                            (char(*)[TIO_STRLEN])matnames ) );
    if (*err != TIO_SUCCESS_F) return;
    
    fnptr = matnames;
    for (i=0; i<l_nmat; ++i) {
      strcpy(ccpy, fnptr);
      c2f_str(ccpy, lptr, fnptr);
      fnptr += TIO_STRLEN_F;
    }
    
  }
  else {
    
    *err = c2f_TIO( TIO_Get_Material_Range( f2c_file(*fileID),
                                            f2c_object(*materialID),
                                            f2c_datatype(*datatype),
                                            &l_nmat,
                                            matnums,
                                            NULL ) );
    
    *nmat = c2f_size(l_nmat);
  }
  
  
  return;
}



/***************************************************************************************************
 *
 *   tio_close_mat_c
 *
 **************************************************************************************************/
void
F2C(tio_close_mat_c,TIO_CLOSE_MAT_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       TIO_tf *err )
{
  *err = c2f_TIO( TIO_Close_Material( f2c_file(*fileID),
                                      f2c_object(*materialID) ) );
  
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
 *   tio_write_quadmat_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_quadmat_chunk_c,TIO_WRITE_QUADMAT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const void          *mat,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       const void          *ncomp,
       const void          *ptr,
       const void          *mixmat,
       const void          *vf,
       const int_f         *narg,
       TIO_tf              *err )
{
  const void *matptr, *ncompptr, *ptrptr, *mixmatptr, *vfptr;
  
  setpm(narg, matptr, mixmatptr, mat, mixmat);
  
  if (*narg > 1) {
    ncompptr = ncomp;
    ptrptr   = ptr;
    vfptr    = vf;
  }
  else {
    ncompptr = ptrptr = vfptr = NULL;
  }
  
  *err = c2f_TIO( TIO_Write_QuadMaterial_Chunk( f2c_file(*fileID),
                                                 f2c_object(*materialID),
                                                 f2c_idx(*idx),
                                                 f2c_xfer(*xfer),
                                                 f2c_datatype(*datatype),
                                                 matptr,
                                                 f2c_datatype(*ncomp_datatype),
                                                 f2c_datatype(*ptr_datatype),
                                                 f2c_datatype(*vf_datatype),
                                                 ncompptr,
                                                 ptrptr,
                                                 mixmatptr,
                                                 vfptr ) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_quadmat_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadmat_chunk_c,TIO_READ_QUADMAT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const TIO_Size_tf   *nghosts,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *mat,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       TIO_Size_tf         *nmixcell,
       TIO_Size_tf         *nmixcomp,
       void                *ncomp,
       void                *ptr,
       void                *mixmat,
       void                *vf,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk, l_nmx, l_nmc;
  void *matptr, *ncompptr, *ptrptr, *mixmatptr, *vfptr;
  
  
  setmat(narg, matptr, ncompptr, ptrptr, mixmatptr, vfptr, mat, ncomp, ptr, mixmat, vf);
  
  *err = c2f_TIO( TIO_Read_QuadMaterial_Chunk( f2c_file(*fileID),
                                               f2c_object(*materialID),
                                               f2c_idx(*idx),
                                               f2c_xfer(*xfer),
                                               f2c_datatype(*datatype),
                                               f2c_ghosts(*nghosts),
                                               &l_ndims,
                                               &l_ni,
                                               &l_nj,
                                               &l_nk,
                                               matptr,
                                               f2c_datatype(*ncomp_datatype),
                                               f2c_datatype(*ptr_datatype),
                                               f2c_datatype(*vf_datatype),
                                               &l_nmx,
                                               &l_nmc,
                                               ncompptr,
                                               ptrptr,
                                               mixmatptr,
                                               vfptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims    = c2f_size(l_ndims);
  *ni       = c2f_size(l_ni);
  *nj       = c2f_size(l_nj);
  *nk       = c2f_size(l_nk);
  *nmixcell = c2f_size(l_nmx);
  *nmixcomp = c2f_size(l_nmc);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_quadmat_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadmat_section_c,TIO_READ_QUADMAT_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *mat,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       TIO_Size_tf         *nmixcell,
       TIO_Size_tf         *nmixcomp,
       void                *ncomp,
       void                *ptr,
       void                *mixmat,
       void                *vf,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk, l_nmx, l_nmc;
  void *matptr, *ncompptr, *ptrptr, *mixmatptr, *vfptr;
  
  
  setmat(narg, matptr, ncompptr, ptrptr, mixmatptr, vfptr, mat, ncomp, ptr, mixmat, vf);
  
  *err = c2f_TIO( TIO_Read_QuadMaterial_Section( f2c_file(*fileID),
                                                 f2c_object(*materialID),
                                                 f2c_object(*sectionID),
                                                 f2c_xfer(*xfer),
                                                 f2c_datatype(*datatype),
                                                 &l_ndims, &l_ni, &l_nj, &l_nk,
                                                 matptr,
                                                 f2c_datatype(*ncomp_datatype),
                                                 f2c_datatype(*ptr_datatype),
                                                 f2c_datatype(*vf_datatype),
                                                 &l_nmx,
                                                 &l_nmc,
                                                 ncompptr,
                                                 ptrptr,
                                                 mixmatptr,
                                                 vfptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims    = c2f_ndims(l_ndims);
  *ni       = c2f_size(l_ni);
  *nj       = c2f_size(l_nj);
  *nk       = c2f_size(l_nk);
  *nmixcell = c2f_size(l_nmx);
  *nmixcomp = c2f_size(l_nmc);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_quadmat_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_quadmat_all_c,TIO_READ_QUADMAT_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const int_f         *datatype,
       int_f               *ndims,
       TIO_Size_tf         *ni,
       TIO_Size_tf         *nj,
       TIO_Size_tf         *nk,
       void                *mat,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       TIO_Size_tf         *nmixcell,
       TIO_Size_tf         *nmixcomp,
       void                *ncomp,
       void                *ptr,
       void                *mixmat,
       void                *vf,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Dims_t l_ndims;
  TIO_Size_t l_ni, l_nj, l_nk, l_nmx, l_nmc;
  void *matptr, *ncompptr, *ptrptr, *mixmatptr, *vfptr;
  
  
  setmat(narg, matptr, ncompptr, ptrptr, mixmatptr, vfptr, mat, ncomp, ptr, mixmat, vf);
  
  *err = c2f_TIO( TIO_Read_QuadMaterial_All( f2c_file(*fileID),
                                             f2c_object(*materialID),
                                             f2c_datatype(*datatype),
                                             &l_ndims,
                                             &l_ni,
                                             &l_nj,
                                             &l_nk,
                                             matptr,
                                             f2c_datatype(*ncomp_datatype),
                                             f2c_datatype(*ptr_datatype),
                                             f2c_datatype(*vf_datatype),
                                             &l_nmx,
                                             &l_nmc,
                                             ncompptr,
                                             ptrptr,
                                             mixmatptr,
                                             vfptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ndims    = c2f_size(l_ndims);
  *ni       = c2f_size(l_ni);
  *nj       = c2f_size(l_nj);
  *nk       = c2f_size(l_nk);
  *nmixcell = c2f_size(l_nmx);
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
 *   tio_write_unstrmat_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_unstrmat_chunk_c,TIO_WRITE_UNSTRMAT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const void          *mat,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       const void          *ncomp,
       const void          *ptr,
       const void          *mixmat,
       const void          *vf,
       const int_f         *narg,
       TIO_tf              *err )
{
  const void *matptr, *ncompptr, *ptrptr, *mixmatptr, *vfptr;
  
  setpm(narg, matptr, mixmatptr, mat, mixmat);
  
  if (*narg > 1) {
    ncompptr = ncomp;
    ptrptr   = ptr;
    vfptr    = vf;
  }
  else {
    ncompptr = ptrptr = vfptr = NULL;
  }
  
  *err = c2f_TIO( TIO_Write_UnstrMaterial_Chunk( f2c_file(*fileID),
                                                 f2c_object(*materialID),
                                                 f2c_idx(*idx),
                                                 f2c_xfer(*xfer),
                                                 f2c_datatype(*datatype),
                                                 matptr,
                                                 f2c_datatype(*ncomp_datatype),
                                                 f2c_datatype(*ptr_datatype),
                                                 f2c_datatype(*vf_datatype),
                                                 ncompptr,
                                                 ptrptr,
                                                 mixmatptr,
                                                 vfptr ) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_unstrmat_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrmat_chunk_c,TIO_READ_UNSTRMAT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const TIO_Size_tf   *nghosts,
       TIO_Size_tf         *ncells,
       void                *mat,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       TIO_Size_tf         *nmixcell,
       TIO_Size_tf         *nmixcomp,
       void                *ncomp,
       void                *ptr,
       void                *mixmat,
       void                *vf,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nc, l_nmx, l_nmc;
  void *matptr, *ncompptr, *ptrptr, *mixmatptr, *vfptr;
  
  
  setpm(narg, matptr, mixmatptr, mat, mixmat);
  
  if (*narg > 1) {
    ncompptr = ncomp;
    ptrptr   = ptr;
    vfptr    = vf;
  }
  else {
    ncompptr = ptrptr = vfptr = NULL;
  }
  
  *err = c2f_TIO( TIO_Read_UnstrMaterial_Chunk( f2c_file(*fileID),
                                                f2c_object(*materialID),
                                                f2c_idx(*idx),
                                                f2c_xfer(*xfer),
                                                f2c_datatype(*datatype),
                                                f2c_ghosts(*nghosts),
                                                &l_nc,
                                                matptr,
                                                f2c_datatype(*ncomp_datatype),
                                                f2c_datatype(*ptr_datatype),
                                                f2c_datatype(*vf_datatype),
                                                &l_nmx,
                                                &l_nmc,
                                                ncompptr,
                                                ptrptr,
                                                mixmatptr,
                                                vfptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ncells   = c2f_size(l_nc);
  *nmixcell = c2f_size(l_nmx);
  *nmixcomp = c2f_size(l_nmc);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_unstrmat_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrmat_all_c,TIO_READ_UNSTRMAT_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const int_f         *datatype,
       TIO_Size_tf         *ncells,
       void                *mat,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       TIO_Size_tf         *nmixcell,
       TIO_Size_tf         *nmixcomp,
       void                *ncomp,
       void                *ptr,
       void                *mixmat,
       void                *vf,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nc, l_nmx, l_nmc;
  void *matptr, *ncompptr, *ptrptr, *mixmatptr, *vfptr;
  
  
  setpm(narg, matptr, mixmatptr, mat, mixmat);
  
  if (*narg > 1) {
    ncompptr = ncomp;
    ptrptr   = ptr;
    vfptr    = vf;
  }
  else {
    ncompptr = ptrptr = vfptr = NULL;
  }
  
  *err = c2f_TIO( TIO_Read_UnstrMaterial_All( f2c_file(*fileID),
                                              f2c_object(*materialID),
                                              f2c_datatype(*datatype),
                                              &l_nc,
                                              matptr,
                                              f2c_datatype(*ncomp_datatype),
                                              f2c_datatype(*ptr_datatype),
                                              f2c_datatype(*vf_datatype),
                                              &l_nmx,
                                              &l_nmc,
                                              ncompptr,
                                              ptrptr,
                                              mixmatptr,
                                              vfptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *ncells   = c2f_size(l_nc);
  *nmixcell = c2f_size(l_nmx);
  *nmixcomp = c2f_size(l_nmc);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_unstrmat_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_unstrmat_section_c,TIO_READ_UNSTRMAT_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       TIO_Size_tf         *nn,
       void                *mat,
       const int_f         *ncomp_datatype,
       const int_f         *ptr_datatype,
       const int_f         *vf_datatype,
       TIO_Size_tf         *nmixcell,
       TIO_Size_tf         *nmixcomp,
       void                *ncomp,
       void                *ptr,
       void                *mixmat,
       void                *vf,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn, l_nmx, l_nmc;
  void *matptr, *ncompptr, *ptrptr, *mixmatptr, *vfptr;
  
  
  setpm(narg, matptr, mixmatptr, mat, mixmat);
  
  if (*narg > 1) {
    ncompptr = ncomp;
    ptrptr   = ptr;
    vfptr    = vf;
  }
  else {
    ncompptr = ptrptr = vfptr = NULL;
  }
  
  *err = c2f_TIO( TIO_Read_UnstrMaterial_Section( f2c_file(*fileID),
                                                  f2c_object(*materialID),
                                                  f2c_object(*sectionID),
                                                  f2c_xfer(*xfer),
                                                  f2c_datatype(*datatype),
                                                  &l_nn,
                                                  matptr,
                                                  f2c_datatype(*ncomp_datatype),
                                                  f2c_datatype(*ptr_datatype),
                                                  f2c_datatype(*vf_datatype),
                                                  &l_nmx,
                                                  &l_nmc,
                                                  ncompptr,
                                                  ptrptr,
                                                  mixmatptr,
                                                  vfptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nn       = c2f_size(l_nn);
  *nmixcell = c2f_size(l_nmx);
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
 *   tio_write_pointmat_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_write_pointmat_chunk_c,TIO_WRITE_POINTMAT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const void          *mat,
       TIO_tf              *err )
{
  *err = c2f_TIO( TIO_Write_PointMaterial_Chunk( f2c_file(*fileID),
                                                 f2c_object(*materialID),
                                                 f2c_idx(*idx),
                                                 f2c_xfer(*xfer),
                                                 f2c_datatype(*datatype),
                                                 mat ) );
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointmat_chunk_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointmat_chunk_c,TIO_READ_POINTMAT_CHUNK_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Size_tf   *idx,
       const int_f         *xfer,
       const int_f         *datatype,
       const TIO_Size_tf   *nghosts,
       TIO_Size_tf         *nnodes,
       void                *mat,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn;
  void *matptr;
  
  
  setpm(narg, matptr, matptr, mat, mat);
  
  *err = c2f_TIO( TIO_Read_PointMaterial_Chunk( f2c_file(*fileID),
                                                f2c_object(*materialID),
                                                f2c_idx(*idx),
                                                f2c_xfer(*xfer),
                                                f2c_datatype(*datatype),
                                                f2c_ghosts(*nghosts),
                                                &l_nn,
                                                matptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nnodes   = c2f_size(l_nn);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointmat_all_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointmat_all_c,TIO_READ_POINTMAT_ALL_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const int_f         *datatype,
       TIO_Size_tf         *nnodes,
       void                *mat,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn;
  void *matptr;
  
  
  setpm(narg, matptr, matptr, mat, mat);
  
  *err = c2f_TIO( TIO_Read_PointMaterial_All( f2c_file(*fileID),
                                              f2c_object(*materialID),
                                              f2c_datatype(*datatype),
                                              &l_nn,
                                              matptr ) );
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nnodes   = c2f_size(l_nn);
  
  return;
}


/***************************************************************************************************
 *
 *   tio_read_pointmat_section_c
 *
 **************************************************************************************************/
void
F2C(tio_read_pointmat_section_c,TIO_READ_POINTMAT_SECTION_C)
     ( const TIO_File_tf   *fileID,
       const TIO_Object_tf *materialID,
       const TIO_Object_tf *sectionID,
       const int_f         *xfer,
       const int_f         *datatype,
       TIO_Size_tf         *nnodes,
       void                *mat,
       const int_f         *narg,
       TIO_tf              *err )
{
  TIO_Size_t l_nn;
  void *matptr;
  
  
  setpm(narg, matptr, matptr, mat, mat);
  
  *err = c2f_TIO( TIO_Read_PointMaterial_Section( f2c_file(*fileID),
                                                  f2c_object(*materialID),
                                                  f2c_object(*sectionID),
                                                  f2c_xfer(*xfer),
                                                  f2c_datatype(*datatype),
                                                  &l_nn,
                                                  matptr ) );  
  
  if (*err != TIO_SUCCESS_F) return;
  
  *nnodes = c2f_size(l_nn);
  
  return;
}




/**************************************************************************************************/
static void
xtioc_copymatinfo( const char       *name,
                   const TIO_Data_t datatype,
                   const TIO_Size_t nmat,
                   const TIO_Size_t nghosts,
                   const TIO_Bool_t ismixed,
                   const TIO_Data_t ncomp_datatype,
                   const TIO_Data_t ptr_datatype,
                   const TIO_Data_t vf_datatype,
                   char             *fname,  const int_f *fname_len,
                   int_f            *fdatatype,
                   TIO_Size_tf      *fnmat,
                   TIO_Size_tf      *fnghosts,
                   int_f            *fismixed,
                   int_f            *fncomp_datatype,
                   int_f            *fptr_datatype,
                   int_f            *fvf_datatype )
{
  if (name != NULL) c2f_str(name, fname_len, fname);
  
  *fdatatype       = c2f_datatype(datatype);
  *fnmat           = c2f_size(nmat);
  *fnghosts        = c2f_ghosts(nghosts);
  
  if (ismixed == TIO_TRUE)
    *fismixed = 1;
  else
    *fismixed = 0;
  
  *fncomp_datatype = c2f_datatype(ncomp_datatype);
  *fptr_datatype   = c2f_datatype(ptr_datatype);
  *fvf_datatype    = c2f_datatype(vf_datatype);
  
  return;
}



/*
 * EOF
 */

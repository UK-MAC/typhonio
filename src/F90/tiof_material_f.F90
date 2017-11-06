!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!
!-*-Mode: F90;-*-  emacs mode specifier


module TIO_Material_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  use TIO_Utils_mod
  
  implicit none
  
  
  private
  public :: TIO_List_Mat_f
  public :: TIO_Create_Mat_f, TIO_Open_Mat_f
  public :: TIO_Get_Mat_Info_f
  public :: TIO_Set_Mat_Range_f, TIO_Get_Mat_Range_f
  public :: TIO_Close_Mat_f
  
  public :: TIO_Write_QuadMat_Chunk_f, TIO_Read_QuadMat_Chunk_f
  public :: TIO_Read_QuadMat_All_f, TIO_Read_QuadMat_Section_f
  
  public :: TIO_Write_UnstrMat_Chunk_f, TIO_Read_UnstrMat_Chunk_f
  public :: TIO_Read_UnstrMat_All_f, TIO_Read_UnstrMat_Section_f
  
  public :: TIO_Write_PointMat_Chunk_f, TIO_Read_PointMat_Chunk_f
  public :: TIO_Read_PointMat_f
!!$, TIO_Read_PointMat_Section_f
  
  
  interface TIO_Set_Mat_Range_f
    module procedure SetRange_i4, SetRange_i8
  end interface
  
  interface TIO_Get_Mat_Range_f
    module procedure GetRange_i4, GetRange_i8
  end interface
  
  
  interface TIO_Write_QuadMat_Chunk_f
    module procedure WriteQuadMatChunk_1D_i4
    module procedure WriteQuadMatChunk_1D_i8
    module procedure WriteQuadMatChunk_1D_i4i4i4r4
    module procedure WriteQuadMatChunk_1D_i4i4i4r8
    module procedure WriteQuadMatChunk_1D_i8i8i8r4
    module procedure WriteQuadMatChunk_1D_i8i8i8r8
    module procedure WriteQuadMatChunk_2D_i4
    module procedure WriteQuadMatChunk_2D_i8
    module procedure WriteQuadMatChunk_2D_i4i4i4r4
    module procedure WriteQuadMatChunk_2D_i4i4i4r8
    module procedure WriteQuadMatChunk_2D_i8i8i8r4
    module procedure WriteQuadMatChunk_2D_i8i8i8r8
    module procedure WriteQuadMatChunk_3D_i4
    module procedure WriteQuadMatChunk_3D_i8
    module procedure WriteQuadMatChunk_3D_i4i4i4r4
    module procedure WriteQuadMatChunk_3D_i4i4i4r8
    module procedure WriteQuadMatChunk_3D_i8i8i8r4
    module procedure WriteQuadMatChunk_3D_i8i8i8r8
    module procedure WriteQuadMatChunk_null
  end interface
  
  interface TIO_Read_QuadMat_Chunk_f
    module procedure ReadQuadMatChunk_meta
    module procedure ReadQuadMatChunk_1D_i4
    module procedure ReadQuadMatChunk_1D_i8
    module procedure ReadQuadMatChunk_1D_i4i4i4r4
    module procedure ReadQuadMatChunk_1D_i4i4i4r8
    module procedure ReadQuadMatChunk_1D_i8i8i8r4
    module procedure ReadQuadMatChunk_1D_i8i8i8r8
    module procedure ReadQuadMatChunk_2D_i4
    module procedure ReadQuadMatChunk_2D_i8
    module procedure ReadQuadMatChunk_2D_i4i4i4r4
    module procedure ReadQuadMatChunk_2D_i4i4i4r8
    module procedure ReadQuadMatChunk_2D_i8i8i8r4
    module procedure ReadQuadMatChunk_2D_i8i8i8r8
    module procedure ReadQuadMatChunk_3D_i4
    module procedure ReadQuadMatChunk_3D_i8
    module procedure ReadQuadMatChunk_3D_i4i4i4r4
    module procedure ReadQuadMatChunk_3D_i4i4i4r8
    module procedure ReadQuadMatChunk_3D_i8i8i8r4
    module procedure ReadQuadMatChunk_3D_i8i8i8r8
  end interface
  
  interface TIO_Read_QuadMat_All_f
    module procedure ReadQuadMatAll_meta        ! no pure or mix material data, just size metadata
    module procedure ReadQuadMatAll_1D_i4
    module procedure ReadQuadMatAll_1D_i8
    module procedure ReadQuadMatAll_1D_i4i4i4r4
    module procedure ReadQuadMatAll_1D_i4i4i4r8
    module procedure ReadQuadMatAll_1D_i8i8i8r4
    module procedure ReadQuadMatAll_1D_i8i8i8r8
    module procedure ReadQuadMatAll_2D_i4
    module procedure ReadQuadMatAll_2D_i8
    module procedure ReadQuadMatAll_2D_i4i4i4r4
    module procedure ReadQuadMatAll_2D_i4i4i4r8
    module procedure ReadQuadMatAll_2D_i8i8i8r4
    module procedure ReadQuadMatAll_2D_i8i8i8r8
    module procedure ReadQuadMatAll_3D_i4
    module procedure ReadQuadMatAll_3D_i8
    module procedure ReadQuadMatAll_3D_i4i4i4r4
    module procedure ReadQuadMatAll_3D_i4i4i4r8
    module procedure ReadQuadMatAll_3D_i8i8i8r4
    module procedure ReadQuadMatAll_3D_i8i8i8r8
    module procedure ReadQuadMatAll_ncomp
  end interface
  
  interface TIO_Read_QuadMat_Section_f
    module procedure ReadQuadMatSection_meta
    module procedure ReadQuadMatSection_1D_i4
    module procedure ReadQuadMatSection_1D_i8
    module procedure ReadQuadMatSection_1D_i4i4i4r4
    module procedure ReadQuadMatSection_1D_i4i4i4r8
    module procedure ReadQuadMatSection_1D_i8i8i8r4
    module procedure ReadQuadMatSection_1D_i8i8i8r8
    module procedure ReadQuadMatSection_2D_i4
    module procedure ReadQuadMatSection_2D_i8
    module procedure ReadQuadMatSection_2D_i4i4i4r4
    module procedure ReadQuadMatSection_2D_i4i4i4r8
    module procedure ReadQuadMatSection_2D_i8i8i8r4
    module procedure ReadQuadMatSection_2D_i8i8i8r8
    module procedure ReadQuadMatSection_3D_i4
    module procedure ReadQuadMatSection_3D_i8
    module procedure ReadQuadMatSection_3D_i4i4i4r4
    module procedure ReadQuadMatSection_3D_i4i4i4r8
    module procedure ReadQuadMatSection_3D_i8i8i8r4
    module procedure ReadQuadMatSection_3D_i8i8i8r8
  end interface
  
  
  interface TIO_Write_UnstrMat_Chunk_f
    module procedure WriteUnstrMatChunk_i4
    module procedure WriteUnstrMatChunk_i8
    module procedure WriteUnstrMatChunk_i4_i4_i4_r4
    module procedure WriteUnstrMatChunk_i4_i4_i4_r8
    module procedure WriteUnstrMatChunk_i8_i8_i8_r4
    module procedure WriteUnstrMatChunk_i8_i8_i8_r8
  end interface
  
  interface TIO_Read_UnstrMat_Chunk_f
    module procedure ReadUnstrMatChunk_meta
    module procedure ReadUnstrMatChunk_i4
    module procedure ReadUnstrMatChunk_i8
    module procedure ReadUnstrMatChunk_i4_i4_i4_r4
    module procedure ReadUnstrMatChunk_i4_i4_i4_r8
    module procedure ReadUnstrMatChunk_i8_i8_i8_r4
    module procedure ReadUnstrMatChunk_i8_i8_i8_r8
  end interface
  
  interface TIO_Read_UnstrMat_All_f
    module procedure ReadUnstrMatAll_meta        ! no pure or mix material data, just size metadata
    module procedure ReadUnstrMatAll_i4
    module procedure ReadUnstrMatAll_i8
    module procedure ReadUnstrMatAll_i4_i4_i4_r4
    module procedure ReadUnstrMatAll_i4_i4_i4_r8
    module procedure ReadUnstrMatAll_i8_i8_i8_r4
    module procedure ReadUnstrMatAll_i8_i8_i8_r8
  end interface
  
  interface TIO_Read_UnstrMat_Section_f
    module procedure ReadUnstrMatSection_meta
    module procedure ReadUnstrMatSection_i4
    module procedure ReadUnstrMatSection_i8
    module procedure ReadUnstrMatSection_i4_i4_i4_r4
    module procedure ReadUnstrMatSection_i4_i4_i4_r8
    module procedure ReadUnstrMatSection_i8_i8_i8_r4
    module procedure ReadUnstrMatSection_i8_i8_i8_r8
  end interface
  
  
  interface TIO_Write_PointMat_Chunk_f
    module procedure WritePointMatChunk_i4
    module procedure WritePointMatChunk_i8
  end interface
  
  interface TIO_Read_PointMat_Chunk_f
    module procedure ReadPointMatChunk_meta
    module procedure ReadPointMatChunk_i4
    module procedure ReadPointMatChunk_i8
  end interface
  
  interface TIO_Read_PointMat_f
    module procedure ReadPointMatAll_meta
    module procedure ReadPointMatAll_i4
    module procedure ReadPointMatAll_i8
  end interface
  
  interface TIO_Read_PointMat_Section_f
    module procedure ReadPointMatSection_meta
    module procedure ReadPointMatSection_i4
    module procedure ReadPointMatSection_i8
  end interface
  
  
contains
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_List_Mat_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_List_Material
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_List_Mat_f( fileID, meshID, name )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: meshID
  ! <DATA>   character(len=*),        intent(out) :: name
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_List_Mat_f( fileID, meshID, name )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    character(len=*),        intent(out) :: name
    
    character(len=TIO_STRLEN_F) :: lname
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_list_mat_c(fileID, meshID, lname, err)
    TIO_List_Mat_f = err
    if (TIO_List_Mat_f /= TIO_SUCCESS_F) return
    
    TIO_List_Mat_f = setstr("TIO_List_Mat_f", "name", lname, name)
    
  end function TIO_List_Mat_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Create_Mat_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Create_Material
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Create_Mat_f( fileID, meshID, name, materialID,
  ! <DATA>                                                   datatype, nmat, nghosts, ismixed,
  ! <DATA>                                                   ncomp_datatype, ptr_datatype, vf_datatype )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)           :: meshID
  ! <DATA>   character(len=*),        intent(in)           :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out)          :: materialID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)           :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)           :: nmat, nghosts
  ! <DATA>   logical(kind=TIO_LPK),   intent(in)           :: ismixed
  ! <DATA>   integer(kind=TIO_IPK),   intent(in), optional :: ncomp_datatype, ptr_datatype, vf_datatype
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Create_Mat_f( fileID, meshID, name, materialID,         &
       &                                            datatype, nmat, nghosts, ismixed,         &
       &                                            ncomp_datatype, ptr_datatype, vf_datatype )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)           :: fileID
    integer(kind=TIO_OBJK),  intent(in)           :: meshID
    character(len=*),        intent(in)           :: name
    integer(kind=TIO_OBJK),  intent(out)          :: materialID
    integer(kind=TIO_IPK),   intent(in)           :: datatype
    integer(kind=TIO_SIZEK), intent(in)           :: nmat, nghosts
    logical(kind=TIO_LPK),   intent(in)           :: ismixed
    integer(kind=TIO_IPK),   intent(in), optional :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in), optional :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in), optional :: vf_datatype
    
    integer :: name_len
    integer :: lismixed, lnd, lpd, lvd
    integer(kind=TIO_ERRK) :: err
    
    
    TIO_Create_Mat_f = setlen("TIO_Create_Mat_f", "name", name, name_len)
    if (TIO_Create_Mat_f /= TIO_SUCCESS_F) return
    
    if (ismixed) then
      lismixed = 1
    else
      lismixed = 0
    end if
    
    if (present(ncomp_datatype)) then
      lnd = ncomp_datatype
    else
      lnd = TIO_DATATYPE_NULL_F
    end if
    if (present(ptr_datatype)) then
      lpd = ptr_datatype
    else
      lpd = TIO_DATATYPE_NULL_F
    end if
    if (present(vf_datatype)) then
      lvd = vf_datatype
    else
      lvd = TIO_DATATYPE_NULL_F
    end if
    
    call tio_create_mat_c( fileID, meshID, name, name_len, materialID,   &
         &                 datatype, nmat, nghosts, lismixed,            &
         &                 lnd, lpd, lvd, err )
    
    
    TIO_Create_Mat_f = err
    
  end function TIO_Create_Mat_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Mat_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Material
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Mat_f( fileID, meshID, name, materialID,
  ! <DATA>                                                 datatype, nmat, nghosts, ismixed,
  ! <DATA>                                                 ncomp_datatype, ptr_datatype, vf_datatype )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),  intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),   intent(in)            :: meshID
  ! <DATA>   character(len=*),         intent(in)            :: name
  ! <DATA>   integer(kind=TIO_OBJK),   intent(out)           :: materialID
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),  intent(out), optional :: nmat, nghosts
  ! <DATA>   logical(kind=TIO_LPK),    intent(out), optional :: ismixed
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: ncomp_datatype
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: ptr_datatype
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: vf_datatype
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Mat_f( fileID, meshID, name, materialID,         &
       &                                          datatype, nmat, nghosts, ismixed,         &
       &                                          ncomp_datatype, ptr_datatype, vf_datatype )
    
    implicit none
    
    integer(kind=TIO_FILEK),  intent(in)            :: fileID
    integer(kind=TIO_OBJK),   intent(in)            :: meshID
    character(len=*),         intent(in)            :: name
    integer(kind=TIO_OBJK),   intent(out)           :: materialID
    integer(kind=TIO_IPK),    intent(out), optional :: datatype
    integer(kind=TIO_SIZEK),  intent(out), optional :: nmat, nghosts
    logical(kind=TIO_LPK),    intent(out), optional :: ismixed
    integer(kind=TIO_IPK),    intent(out), optional :: ncomp_datatype
    integer(kind=TIO_IPK),    intent(out), optional :: ptr_datatype
    integer(kind=TIO_IPK),    intent(out), optional :: vf_datatype
    
    
    integer                 :: name_len
    integer                 :: lmd, lnd, lpd, lvd, lismixed
    integer(kind=TIO_SIZEK) :: lnmat, lnghosts
    integer(kind=TIO_ERRK)  :: err
    
    
    TIO_Open_Mat_f = setlen("TIO_Open_Mat_f", "name", name, name_len)
    if (TIO_Open_Mat_f /= TIO_SUCCESS_F) return
    
    call tio_open_mat_c( fileID, meshID, name, name_len, materialID,  &
         &               lmd, lnmat, lnghosts, lismixed, lnd, lpd, lvd, err )
    
    TIO_Open_Mat_f = err
    if (TIO_Open_Mat_f /= TIO_SUCCESS_F) return
    
    if (present(datatype))       datatype       = lmd
    if (present(nmat))           nmat           = lnmat
    if (present(nghosts))        nghosts        = lnghosts
    if (present(ismixed))        ismixed        = (lismixed > 0)
    if (present(ncomp_datatype)) ncomp_datatype = lnd
    if (present(ptr_datatype))   ptr_datatype   = lpd
    if (present(vf_datatype))    vf_datatype    = lvd
    
  end function TIO_Open_Mat_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Mat_Info_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Material_Info
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Mat_Info_f( fileID, materialID, name,
  ! <DATA>                                                     datatype, nmat, nghosts, ismixed,
  ! <DATA>                                                     ncomp_datatype, ptr_datatype, vf_datatype )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: materialID
  ! <DATA>   character(len=*),        intent(out), optional :: name
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmat, nghosts
  ! <DATA>   logical(kind=TIO_LPK),   intent(out), optional :: ismixed
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: ncomp_datatype
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: ptr_datatype
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: vf_datatype
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer function TIO_Get_Mat_Info_f( fileID, materialID, name,                 &
       &                               datatype, nmat, nghosts, ismixed,         &
       &                               ncomp_datatype, ptr_datatype, vf_datatype )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: materialID
    character(len=*),        intent(out), optional :: name
    integer(kind=TIO_IPK),   intent(out), optional :: datatype
    integer(kind=TIO_SIZEK), intent(out), optional :: nmat, nghosts
    logical(kind=TIO_LPK),   intent(out), optional :: ismixed
    integer(kind=TIO_IPK),   intent(out), optional :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(out), optional :: ptr_datatype
    integer(kind=TIO_IPK),   intent(out), optional :: vf_datatype
    
    character(len=TIO_STRLEN_F) :: lname=""
    integer                     :: name_len
    integer                     :: lmd, lnd, lpd, lvd, lismixed
    integer(kind=TIO_SIZEK)     :: lnmat, lnghosts
    integer(kind=TIO_ERRK)      :: err
    
    call tio_get_mat_info_c( fileID, materialID, lname, len(lname),             &
         &                   lmd, lnmat, lnghosts, lismixed, lnd, lpd, lvd, err )
    
    TIO_Get_Mat_Info_f = err
    if (TIO_Get_Mat_Info_f /= TIO_SUCCESS_F) return
    
    TIO_Get_Mat_Info_f = err
    if (TIO_Get_Mat_Info_f /= TIO_SUCCESS_F) return
    
    if (present(name)) then
      TIO_Get_Mat_Info_f = setstr("TIO_Get_Mat_Info_f", "name", lname, name)
      if (TIO_Get_Mat_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(datatype))       datatype       = lmd
    if (present(nmat))           nmat           = lnmat
    if (present(nghosts))        nghosts        = lnghosts
    if (present(ismixed))        ismixed        = (lismixed > 0)
    if (present(ncomp_datatype)) ncomp_datatype = lnd
    if (present(ptr_datatype))   ptr_datatype   = lpd
    if (present(vf_datatype))    vf_datatype    = lvd
    
  end function TIO_Get_Mat_Info_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Mat_Range_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Material_Range
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Mat_Range_f( fileID, materialID, datatype,
  ! <DATA>                                                      nmat, matnums, matnames )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)           :: materialID
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)           :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)           :: nmat
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(in)           :: matnums
  ! <DATA>   character(len=*), dimension(:), intent(in), optional :: matnames
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function SetRange_i4(fileID, materialID, datatype, nmat, matnums, matnames)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    integer(kind=TIO_SIZEK), intent(in) :: nmat
    
    integer(kind=4), dimension(1:), intent(in) :: matnums
    
    character(len=*), dimension(1:), intent(in), optional :: matnames
    
    
    character(len=TIO_STRLEN_F), dimension(nmat) :: lnames
    integer,                     dimension(nmat) :: names_len
    
    integer(kind=TIO_SIZEK) :: i
    integer(kind=TIO_ERRK)  :: err
    
    
    if (present(matnames)) then
      
      do i = 1, nmat
        names_len(i) = len_trim(matnames(i))
        if (names_len(i) > TIO_STRLEN_F) then
          call seterror("TIO_Set_Mat_Range_f", "matnames(:) string lengths must be <= TIO_STRLEN_F")
          SetRange_i4 = TIO_ERR_F90_API_F
          return
        end if
        lnames(i) = trim(matnames(i))
      end do
      
      i = nmat
      
    else
      
      i = -nmat
      
    end if
    
    call tio_set_mat_range_c( fileID, materialID, datatype, i, matnums, lnames, names_len, err)
    
    SetRange_i4 = err
  end function SetRange_i4
  integer(kind=TIO_ERRK) function SetRange_i8(fileID, materialID, datatype, nmat, matnums, matnames)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    integer(kind=TIO_SIZEK), intent(in) :: nmat
    
    integer(kind=8), dimension(1:), intent(in) :: matnums
    
    character(len=*), dimension(1:), intent(in), optional :: matnames
    
    
    character(len=TIO_STRLEN_F), dimension(nmat) :: lnames
    integer,                     dimension(nmat) :: names_len
    
    integer(kind=TIO_SIZEK) :: i
    integer(kind=TIO_ERRK)  :: err
    
    
    if (present(matnames)) then
      
      do i = 1, nmat
        names_len(i) = len_trim(matnames(i))
        if (names_len(i) > TIO_STRLEN_F) then
          call seterror("TIO_Set_Mat_Range_f", "matnames(:) string lengths must be <= TIO_STRLEN_F")
          SetRange_i8 = TIO_ERR_F90_API_F
          return
        end if
        lnames(i) = trim(matnames(i))
      end do
      
      i = nmat
      
    else
      
      i = -nmat
      
    end if
    
    call tio_set_mat_range_c( fileID, materialID, datatype, i, matnums, lnames, names_len, err)
    
    SetRange_i8 = err
  end function SetRange_i8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Mat_Range_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Material_Range
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Mat_Range_f( fileID, materialID, datatype,
  ! <DATA>                                                      nmat, matnums, matnames )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: nmat
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out)           :: matnums
  ! <DATA>   character(len=*), dimension(:), intent(out), optional :: matnames
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function GetRange_i4(fileID, materialID, datatype, nmat, matnums, matnames)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmat
    
    integer(kind=4), dimension(1:), intent(out) :: matnums
    
    character(len=*), dimension(1:), intent(out), optional :: matnames
    
    
    character :: cdummy
    
    integer(kind=TIO_SIZEK) :: i
    integer(kind=TIO_ERRK)  :: err
    
    
    i = -1
    call tio_get_mat_range_c( fileID, materialID, datatype, i, matnums, cdummy, err)
    GetRange_i4 = err
    if (GetRange_i4 /= TIO_SUCCESS_F) return
    
    nmat = i
    
    
    if (present(matnames)) then
      GetRange_i4 = listnames("TIO_Get_Mat_Range", LISTCLASS_MAT, fileID, materialID, i, matnames)
    end if
    
    
    GetRange_i4 = err
  end function GetRange_i4
  integer(kind=TIO_ERRK) function GetRange_i8(fileID, materialID, datatype, nmat, matnums, matnames)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmat
    
    integer(kind=8), dimension(1:), intent(out) :: matnums
    
    character(len=*), dimension(1:), intent(out), optional :: matnames
    
    
    character :: cdummy
    
    integer(kind=TIO_SIZEK) :: i
    integer(kind=TIO_ERRK)  :: err
    
    
    i = -1
    call tio_get_mat_range_c( fileID, materialID, datatype, i, matnums, cdummy, err)
    GetRange_i8 = err
    if (GetRange_i8 /= TIO_SUCCESS_F) return
    
    nmat = i
    
    
    if (present(matnames)) then
      GetRange_i8 = listnames("TIO_Get_Mat_Range", LISTCLASS_MAT, fileID, materialID, i, matnames)
    end if
    
    
    GetRange_i8 = err
  end function GetRange_i8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Close_Mat_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Close_Material
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Close_Mat_f(fileID, materialID)
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: materialID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Close_Mat_f(fileID, materialID)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_close_mat_c(fileID, materialID, err)
    
    TIO_Close_Mat_f = err
    
  end function TIO_Close_Mat_f
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **           QUADMATS             ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_QuadMat_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_QuadMaterial_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_QuadMat_Chunk_f( fileID, materialID, idx, xfer,
  ! <DATA>                                                datatype, mat,
  ! <DATA>                                                ncomp_datatype, ptr_datatype, vf_datatype,
  ! <DATA>                                                ncomp, ptr, mixmat, vf  )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)           :: materialID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)           :: idx
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)           :: xfer, datatype
  ! <DATA>   [INTEGER CHOICE], [DIMENSION],  intent(in)           :: mat
  ! <DATA>   integer(kind=TIO_IPK),          intent(in), optional :: ncomp_datatype, ptr_datatype, vf_datatype
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(in), optional :: ncomp, ptr, mixmat
  ! <DATA>   [REAL CHOICE],    dimension(:), intent(in), optional :: vf
  ! </FAPI>
  ! <FDES>
  ! <DATA> [INTEGER CHOICE] must be the same for mat and mixmat, but others can differ.
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function WriteQuadMatChunk_1D_i4( fileID, materialID, idx, xfer,   &
       &                                                   datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:),     intent(in) :: mat




    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 1
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat,  &
         &                          ddum, ddum, ddum, idum, idum, idum, idum, n, err )


    
    
    WriteQuadMatChunk_1D_i4 = err
  end function WriteQuadMatChunk_1D_i4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_1D_i8( fileID, materialID, idx, xfer,   &
       &                                                   datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:),     intent(in) :: mat




    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 1
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat,  &
         &                          ddum, ddum, ddum, idum, idum, idum, idum, n, err )


    
    
    WriteQuadMatChunk_1D_i8 = err
  end function WriteQuadMatChunk_1D_i8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_1D_i4i4i4r4( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:),     intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_1D_i4i4i4r4 = err
  end function WriteQuadMatChunk_1D_i4i4i4r4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_1D_i4i4i4r8( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:),     intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_1D_i4i4i4r8 = err
  end function WriteQuadMatChunk_1D_i4i4i4r8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_1D_i8i8i8r4( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:),     intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=8), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_1D_i8i8i8r4 = err
  end function WriteQuadMatChunk_1D_i8i8i8r4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_1D_i8i8i8r8( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:),     intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=8), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_1D_i8i8i8r8 = err
  end function WriteQuadMatChunk_1D_i8i8i8r8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_2D_i4( fileID, materialID, idx, xfer,   &
       &                                                   datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:,:),   intent(in) :: mat




    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 1
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat,  &
         &                          ddum, ddum, ddum, idum, idum, idum, idum, n, err )


    
    
    WriteQuadMatChunk_2D_i4 = err
  end function WriteQuadMatChunk_2D_i4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_2D_i8( fileID, materialID, idx, xfer,   &
       &                                                   datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:,:),   intent(in) :: mat




    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 1
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat,  &
         &                          ddum, ddum, ddum, idum, idum, idum, idum, n, err )


    
    
    WriteQuadMatChunk_2D_i8 = err
  end function WriteQuadMatChunk_2D_i8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_2D_i4i4i4r4( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:,:),   intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_2D_i4i4i4r4 = err
  end function WriteQuadMatChunk_2D_i4i4i4r4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_2D_i4i4i4r8( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:,:),   intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_2D_i4i4i4r8 = err
  end function WriteQuadMatChunk_2D_i4i4i4r8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_2D_i8i8i8r4( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:,:),   intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=8), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_2D_i8i8i8r4 = err
  end function WriteQuadMatChunk_2D_i8i8i8r4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_2D_i8i8i8r8( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:,:),   intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=8), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_2D_i8i8i8r8 = err
  end function WriteQuadMatChunk_2D_i8i8i8r8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_3D_i4( fileID, materialID, idx, xfer,   &
       &                                                   datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:,:,:), intent(in) :: mat




    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 1
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat,  &
         &                          ddum, ddum, ddum, idum, idum, idum, idum, n, err )


    
    
    WriteQuadMatChunk_3D_i4 = err
  end function WriteQuadMatChunk_3D_i4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_3D_i8( fileID, materialID, idx, xfer,   &
       &                                                   datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:,:,:), intent(in) :: mat




    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 1
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat,  &
         &                          ddum, ddum, ddum, idum, idum, idum, idum, n, err )


    
    
    WriteQuadMatChunk_3D_i8 = err
  end function WriteQuadMatChunk_3D_i8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_3D_i4i4i4r4( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:,:,:), intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_3D_i4i4i4r4 = err
  end function WriteQuadMatChunk_3D_i4i4i4r4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_3D_i4i4i4r8( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=4), dimension(:,:,:), intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_3D_i4i4i4r8 = err
  end function WriteQuadMatChunk_3D_i4i4i4r8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_3D_i8i8i8r4( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:,:,:), intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=8), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_3D_i8i8i8r4 = err
  end function WriteQuadMatChunk_3D_i8i8i8r4
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_3D_i8i8i8r8( fileID, materialID, idx, xfer,   &
       &        datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    
    integer(kind=8), dimension(:,:,:), intent(in) :: mat




    integer, intent(in) :: ncomp_datatype
    integer, intent(in) :: ptr_datatype
    integer, intent(in) :: vf_datatype
    
    integer(kind=8), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                          ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteQuadMatChunk_3D_i8i8i8r8 = err
  end function WriteQuadMatChunk_3D_i8i8i8r8
  integer(kind=TIO_ERRK) function WriteQuadMatChunk_null( fileID, materialID, idx, xfer,   &
       &                                                  datatype )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: materialID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer, datatype
    
    




    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 0
    call tio_write_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype, idum,  &
         &                          ddum, ddum, ddum, idum, idum, idum, idum, n, err )
    
    
    WriteQuadMatChunk_null = err
  end function WriteQuadMatChunk_null
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadMat_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadMaterial_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadMat_Chunk_f( fileID, materialID, idx, xfer,
  ! <DATA>                             datatype, nghosts, ndims, ni, nj, nk, mat,
  ! <DATA>                             ncomp_datatype, ptr_datatype, vf_datatype,
  ! <DATA>                             nmixcell, nmixcomp,
  ! <DATA>                             ncomp, ptr, mixmat, vf  )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: xfer, datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)            :: nghosts
  ! <DATA>   integer(kind=TIO_IPK),          intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: ni, nj, nk
  ! <DATA>   [INTEGER CHOICE], [DIMENSION],  intent(out), optional :: mat
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ncomp_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ptr_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: vf_datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nmixcell, nmixcomp
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: ncomp, ptr
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mixmat
  ! <DATA>   [REAL CHOICE],    dimension(:), intent(out), optional :: vf
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadQuadMatChunk_meta( fileID, materialID, idx, xfer,          &
       &                                                 datatype, nghosts, ndims, ni, nj, nk,   &
       &                                                 nmixcell, nmixcomp ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk


    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp










    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    


    n = 0
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,  &
         &                         nghosts, ndims, ni, nj, nk, idum,         &
         &                         ddum, ddum, ddum, lnmx, lnmc,             &
         &                         idum, idum, idum, idum, n, err )
    
    if (present(nmixcell)) nmixcell = lnmx
    if (present(nmixcomp)) nmixcomp = lnmc
    
    
    
    
    fresult = err
    
  end function ReadQuadMatChunk_meta
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_1D_i4(       fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:),     intent(out) :: mat








    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,  &
         &                         nghosts, ndims, ni, nj, nk, mat,          &
         &                         ddum, ddum, ddum,  lnmx, lnmc,            &
         &                         idum, idum, idum, idum, n, err )
    
    
    
    fresult = err
    
  end function ReadQuadMatChunk_1D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_1D_i8(       fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:),     intent(out) :: mat








    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,  &
         &                         nghosts, ndims, ni, nj, nk, mat,          &
         &                         ddum, ddum, ddum,  lnmx, lnmc,            &
         &                         idum, idum, idum, idum, n, err )
    
    
    
    fresult = err
    
  end function ReadQuadMatChunk_1D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_1D_i4i4i4r4( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_1D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_1D_i4i4i4r8( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_1D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_1D_i8i8i8r4( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_1D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_1D_i8i8i8r8( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_1D_i8i8i8r8
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_2D_i4(       fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:,:),   intent(out) :: mat








    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,  &
         &                         nghosts, ndims, ni, nj, nk, mat,          &
         &                         ddum, ddum, ddum,  lnmx, lnmc,            &
         &                         idum, idum, idum, idum, n, err )
    
    
    
    fresult = err
    
  end function ReadQuadMatChunk_2D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_2D_i8(       fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:,:),   intent(out) :: mat








    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,  &
         &                         nghosts, ndims, ni, nj, nk, mat,          &
         &                         ddum, ddum, ddum,  lnmx, lnmc,            &
         &                         idum, idum, idum, idum, n, err )
    
    
    
    fresult = err
    
  end function ReadQuadMatChunk_2D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_2D_i4i4i4r4( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_2D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_2D_i4i4i4r8( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_2D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_2D_i8i8i8r4( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_2D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_2D_i8i8i8r8( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_2D_i8i8i8r8
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_3D_i4(       fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:,:,:), intent(out) :: mat








    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,  &
         &                         nghosts, ndims, ni, nj, nk, mat,          &
         &                         ddum, ddum, ddum,  lnmx, lnmc,            &
         &                         idum, idum, idum, idum, n, err )
    
    
    
    fresult = err
    
  end function ReadQuadMatChunk_3D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_3D_i8(       fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:,:,:), intent(out) :: mat








    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,  &
         &                         nghosts, ndims, ni, nj, nk, mat,          &
         &                         ddum, ddum, ddum,  lnmx, lnmc,            &
         &                         idum, idum, idum, idum, n, err )
    
    
    
    fresult = err
    
  end function ReadQuadMatChunk_3D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_3D_i4i4i4r4( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_3D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_3D_i4i4i4r8( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=4), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_3D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_3D_i8i8i8r4( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_3D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatChunk_3D_i8i8i8r8( fileID, materialID, idx, xfer,   &
       &        datatype, nghosts, ndims, ni, nj, nk, mat,                         &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fresult)
       
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk




    integer(kind=8), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf


    


    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_chunk_c( fileID, materialID, idx, xfer, datatype,    &
         &                         nghosts, ndims, ni, nj, nk, mat,            &
         &                         ncomp_datatype, ptr_datatype, vf_datatype,  &
         &                         nmixcell, nmixcomp,                         &
         &                         ncomp, ptr, mixmat, vf, n, err )
    
    
    fresult = err
    
  end function ReadQuadMatChunk_3D_i8i8i8r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadMat_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadMaterial_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadMat_All_f( fileID, materialID, datatype,
  ! <DATA>                             ndims, ni, nj, nk, mat,
  ! <DATA>                             ncomp_datatype, ptr_datatype, vf_datatype,
  ! <DATA>                             nmixcell, nmixcomp,
  ! <DATA>                             ncomp, ptr, mixmat, vf )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: ni, nj, nk
  ! <DATA>   [INTEGER CHOICE], [DIMENSION],  intent(out), optional :: mat
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ncomp_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ptr_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: vf_datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nmixcell, nmixcomp
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: ncomp, ptr
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mixmat
  ! <DATA>   [REAL CHOICE],    dimension(:), intent(out), optional :: vf
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadQuadMatAll_meta( fileID, materialID, datatype,           &
       &                                               ndims, ni, nj, nk, nmixcell, nmixcomp)  &
       &                                               result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp






    
    






    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    


    n = 0
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, idum,       &
         &                       ddum, ddum, ddum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    if (present(nmixcell)) nmixcell = lnmx
    if (present(nmixcomp)) nmixcomp = lnmc
    
    
    
    


    
    
    fres = err
    
  end function ReadQuadMatAll_meta
  integer(kind=TIO_ERRK) function ReadQuadMatAll_1D_i4(       fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:),     intent(out) :: mat




    
    






    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,         &
         &                       ddum, ddum, ddum,  lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    
    


    
    
    fres = err
    
  end function ReadQuadMatAll_1D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_1D_i8(       fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:),     intent(out) :: mat




    
    






    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,         &
         &                       ddum, ddum, ddum,  lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    
    


    
    
    fres = err
    
  end function ReadQuadMatAll_1D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatAll_1D_i4i4i4r4( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_1D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_1D_i4i4i4r8( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_1D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatAll_1D_i8i8i8r4( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_1D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_1D_i8i8i8r8( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_1D_i8i8i8r8
  integer(kind=TIO_ERRK) function ReadQuadMatAll_2D_i4(       fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:,:),   intent(out) :: mat




    
    






    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,         &
         &                       ddum, ddum, ddum,  lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    
    


    
    
    fres = err
    
  end function ReadQuadMatAll_2D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_2D_i8(       fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:,:),   intent(out) :: mat




    
    






    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,         &
         &                       ddum, ddum, ddum,  lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    
    


    
    
    fres = err
    
  end function ReadQuadMatAll_2D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatAll_2D_i4i4i4r4( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_2D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_2D_i4i4i4r8( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_2D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatAll_2D_i8i8i8r4( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_2D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_2D_i8i8i8r8( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_2D_i8i8i8r8
  integer(kind=TIO_ERRK) function ReadQuadMatAll_3D_i4(       fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:,:,:), intent(out) :: mat




    
    






    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,         &
         &                       ddum, ddum, ddum,  lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    
    


    
    
    fres = err
    
  end function ReadQuadMatAll_3D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_3D_i8(       fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:,:,:), intent(out) :: mat




    
    






    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,         &
         &                       ddum, ddum, ddum,  lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    
    


    
    
    fres = err
    
  end function ReadQuadMatAll_3D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatAll_3D_i4i4i4r4( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_3D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_3D_i4i4i4r8( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=4), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_3D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatAll_3D_i8i8i8r4( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_3D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatAll_3D_i8i8i8r8( fileID, materialID, datatype,    &
       &        ndims, ni, nj, nk, mat,                                            &
       &        ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,     &
       &        ncomp, ptr, mixmat, vf  ) result (fres)


    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    


    integer(kind=8), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    






    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    
    
    
    n = 2
    call tio_read_quadmat_all_c( fileID, materialID, datatype, ndims, ni, nj, nk, mat,           &
         &                       ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,  &
         &                       ncomp, ptr, mixmat, vf, n, err )
    


    
    
    fres = err
    
  end function ReadQuadMatAll_3D_i8i8i8r8


    






    
    
       
  integer(kind=TIO_ERRK) function ReadQuadMatAll_ncomp( fileID, materialID,  &
    &               ncomp_datatype, nmixcell, nmixcomp, ncomp  ) result (fres)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    integer(kind=4), dimension(:), intent(out) :: ncomp
    
    integer(kind=TIO_IPK)   :: lndims
    integer(kind=TIO_SIZEK) :: lni, lnj, lnk






    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                 :: n
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    


    
    n = 1025
    call tio_read_quadmat_all_c( fileID, materialID, ddum, lndims, lni, lnj, lnk, idum,          &
         &                       ncomp_datatype, ddum, ddum, nmixcell, nmixcomp,  &
         &                       ncomp, idum, idum, idum, n, err )
    
    
    
    fres = err
    
  end function ReadQuadMatAll_ncomp
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadMat_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadMaterial_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadMat_Section_f( fileID, materialID, sectionID,
  ! <DATA>                             xfer, datatype, ndims, ni, nj, nk, mat,
  ! <DATA>                             ncomp_datatype, ptr_datatype, vf_datatype,
  ! <DATA>                             nmixcell, nmixcomp, ncomp, ptr, mixmat, vf )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID, sectionID
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: xfer, datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: ni, nj, nk
  ! <DATA>   [INTEGER CHOICE], [DIMENSION],  intent(out), optional :: mat
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ncomp_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ptr_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: vf_datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nmixcell, nmixcomp
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: ncomp, ptr
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mixmat
  ! <DATA>   [REAL CHOICE],    dimension(:), intent(out), optional :: vf
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadQuadMatSection_meta( fileID, materialID, sectionID, xfer, datatype,  &
       &                                                   ndims, ni, nj, nk, nmixcell, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp






    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    n = 0
    
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           idum, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    if (present(nmixcell)) nmixcell = lnmx
    if (present(nmixcomp)) nmixcomp = lnmc
    
    
    


    
    ReadQuadMatSection_meta = err
  end function ReadQuadMatSection_meta
  integer(kind=TIO_ERRK) function ReadQuadMatSection_1D_i4( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:),     intent(out) :: mat




    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    n = 1
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,    &
         &                           mat, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    


    
    ReadQuadMatSection_1D_i4 = err
  end function ReadQuadMatSection_1D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_1D_i8( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:),     intent(out) :: mat




    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    n = 1
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,    &
         &                           mat, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    


    
    ReadQuadMatSection_1D_i8 = err
  end function ReadQuadMatSection_1D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatSection_1D_i4i4i4r4( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_1D_i4i4i4r4 = err
  end function ReadQuadMatSection_1D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_1D_i4i4i4r8( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_1D_i4i4i4r8 = err
  end function ReadQuadMatSection_1D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatSection_1D_i8i8i8r4( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_1D_i8i8i8r4 = err
  end function ReadQuadMatSection_1D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_1D_i8i8i8r8( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:),     intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_1D_i8i8i8r8 = err
  end function ReadQuadMatSection_1D_i8i8i8r8
  integer(kind=TIO_ERRK) function ReadQuadMatSection_2D_i4( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:,:),   intent(out) :: mat




    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    n = 1
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,    &
         &                           mat, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    


    
    ReadQuadMatSection_2D_i4 = err
  end function ReadQuadMatSection_2D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_2D_i8( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:,:),   intent(out) :: mat




    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    n = 1
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,    &
         &                           mat, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    


    
    ReadQuadMatSection_2D_i8 = err
  end function ReadQuadMatSection_2D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatSection_2D_i4i4i4r4( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_2D_i4i4i4r4 = err
  end function ReadQuadMatSection_2D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_2D_i4i4i4r8( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_2D_i4i4i4r8 = err
  end function ReadQuadMatSection_2D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatSection_2D_i8i8i8r4( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_2D_i8i8i8r4 = err
  end function ReadQuadMatSection_2D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_2D_i8i8i8r8( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:,:),   intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_2D_i8i8i8r8 = err
  end function ReadQuadMatSection_2D_i8i8i8r8
  integer(kind=TIO_ERRK) function ReadQuadMatSection_3D_i4( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:,:,:), intent(out) :: mat




    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    n = 1
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,    &
         &                           mat, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    


    
    ReadQuadMatSection_3D_i4 = err
  end function ReadQuadMatSection_3D_i4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_3D_i8( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:,:,:), intent(out) :: mat




    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    n = 1
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,    &
         &                           mat, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    


    
    ReadQuadMatSection_3D_i8 = err
  end function ReadQuadMatSection_3D_i8
  integer(kind=TIO_ERRK) function ReadQuadMatSection_3D_i4i4i4r4( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_3D_i4i4i4r4 = err
  end function ReadQuadMatSection_3D_i4i4i4r4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_3D_i4i4i4r8( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=4), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_3D_i4i4i4r8 = err
  end function ReadQuadMatSection_3D_i4i4i4r8
  integer(kind=TIO_ERRK) function ReadQuadMatSection_3D_i8i8i8r4( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_3D_i8i8i8r4 = err
  end function ReadQuadMatSection_3D_i8i8i8r4
  integer(kind=TIO_ERRK) function ReadQuadMatSection_3D_i8i8i8r8( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ndims, ni, nj, nk, mat,                     &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    integer(kind=8), dimension(:,:,:), intent(out) :: mat




    integer(kind=TIO_IPK),   intent(in)  :: ncomp_datatype
    integer(kind=TIO_IPK),   intent(in)  :: ptr_datatype
    integer(kind=TIO_IPK),   intent(in)  :: vf_datatype
    integer(kind=TIO_SIZEK), intent(out) :: nmixcell, nmixcomp
    
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    
    
    
    
    n = 2
    call tio_read_quadmat_section_c( fileID, materialID, sectionID, xfer, datatype, ndims, ni, nj, nk,     &
         &                           mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                           ncomp, ptr, mixmat, vf, n, err )
    


    
    ReadQuadMatSection_3D_i8i8i8r8 = err
  end function ReadQuadMatSection_3D_i8i8i8r8
  
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **          UNSTRMATS             ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_UnstrMat_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_UnstrMaterial_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_UnstrMat_Chunk_f( fileID, materialID, idx, xfer,
  ! <DATA>                             datatype, mat,
  ! <DATA>                             ncomp_datatype, ptr_datatype, vf_datatype,
  ! <DATA>                             ncomp, ptr, mixmat, vf )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)           :: materialID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)           :: idx
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)           :: xfer, datatype
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(in)           :: mat
  ! <DATA>   integer(kind=TIO_IPK),          intent(in), optional :: ncomp_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in), optional :: ptr_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in), optional :: vf_datatype
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(in), optional :: ncomp, ptr, mixmat
  ! <DATA>   [REAL CHOICE],    dimension(:), intent(in), optional :: vf
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function WriteUnstrMatChunk_i4( fileID, materialID, idx, xfer, datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK),       intent(in) :: fileID
    integer(kind=TIO_OBJK),        intent(in) :: materialID
    integer(kind=TIO_SIZEK),       intent(in) :: idx
    integer(kind=TIO_IPK),         intent(in) :: xfer, datatype
    
    integer(kind=4), dimension(:), intent(in) :: mat
    
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 1
    call tio_write_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat,  &
         &                           ddum, ddum, ddum, idum, idum, idum, idum, n, err )


    
    
    WriteUnstrMatChunk_i4 = err
  end function WriteUnstrMatChunk_i4
  integer(kind=TIO_ERRK) function WriteUnstrMatChunk_i8( fileID, materialID, idx, xfer, datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK),       intent(in) :: fileID
    integer(kind=TIO_OBJK),        intent(in) :: materialID
    integer(kind=TIO_SIZEK),       intent(in) :: idx
    integer(kind=TIO_IPK),         intent(in) :: xfer, datatype
    
    integer(kind=8), dimension(:), intent(in) :: mat
    
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    integer :: ddum = TIO_DATATYPE_NULL_F, idum
    
    n = 1
    call tio_write_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat,  &
         &                           ddum, ddum, ddum, idum, idum, idum, idum, n, err )


    
    
    WriteUnstrMatChunk_i8 = err
  end function WriteUnstrMatChunk_i8
  integer(kind=TIO_ERRK) function WriteUnstrMatChunk_i4_i4_i4_r4( fileID, materialID,  &
       &    idx, xfer, datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf )
    
    implicit none
    
    integer(kind=TIO_FILEK),       intent(in) :: fileID
    integer(kind=TIO_OBJK),        intent(in) :: materialID
    integer(kind=TIO_SIZEK),       intent(in) :: idx
    integer(kind=TIO_IPK),         intent(in) :: xfer, datatype
    
    integer(kind=4), dimension(:), intent(in) :: mat
    
    integer(kind=TIO_IPK),         intent(in) :: ncomp_datatype
    integer(kind=TIO_IPK),         intent(in) :: ptr_datatype
    integer(kind=TIO_IPK),         intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                           ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                           ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteUnstrMatChunk_i4_i4_i4_r4 = err
  end function WriteUnstrMatChunk_i4_i4_i4_r4
  integer(kind=TIO_ERRK) function WriteUnstrMatChunk_i4_i4_i4_r8( fileID, materialID,  &
       &    idx, xfer, datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf )
    
    implicit none
    
    integer(kind=TIO_FILEK),       intent(in) :: fileID
    integer(kind=TIO_OBJK),        intent(in) :: materialID
    integer(kind=TIO_SIZEK),       intent(in) :: idx
    integer(kind=TIO_IPK),         intent(in) :: xfer, datatype
    
    integer(kind=4), dimension(:), intent(in) :: mat
    
    integer(kind=TIO_IPK),         intent(in) :: ncomp_datatype
    integer(kind=TIO_IPK),         intent(in) :: ptr_datatype
    integer(kind=TIO_IPK),         intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                           ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                           ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteUnstrMatChunk_i4_i4_i4_r8 = err
  end function WriteUnstrMatChunk_i4_i4_i4_r8
  integer(kind=TIO_ERRK) function WriteUnstrMatChunk_i8_i8_i8_r4( fileID, materialID,  &
       &    idx, xfer, datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf )
    
    implicit none
    
    integer(kind=TIO_FILEK),       intent(in) :: fileID
    integer(kind=TIO_OBJK),        intent(in) :: materialID
    integer(kind=TIO_SIZEK),       intent(in) :: idx
    integer(kind=TIO_IPK),         intent(in) :: xfer, datatype
    
    integer(kind=8), dimension(:), intent(in) :: mat
    
    integer(kind=TIO_IPK),         intent(in) :: ncomp_datatype
    integer(kind=TIO_IPK),         intent(in) :: ptr_datatype
    integer(kind=TIO_IPK),         intent(in) :: vf_datatype
    
    integer(kind=4), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                           ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                           ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteUnstrMatChunk_i8_i8_i8_r4 = err
  end function WriteUnstrMatChunk_i8_i8_i8_r4
  integer(kind=TIO_ERRK) function WriteUnstrMatChunk_i8_i8_i8_r8( fileID, materialID,  &
       &    idx, xfer, datatype, mat, ncomp_datatype, ptr_datatype, vf_datatype, ncomp, ptr, mixmat, vf )
    
    implicit none
    
    integer(kind=TIO_FILEK),       intent(in) :: fileID
    integer(kind=TIO_OBJK),        intent(in) :: materialID
    integer(kind=TIO_SIZEK),       intent(in) :: idx
    integer(kind=TIO_IPK),         intent(in) :: xfer, datatype
    
    integer(kind=8), dimension(:), intent(in) :: mat
    
    integer(kind=TIO_IPK),         intent(in) :: ncomp_datatype
    integer(kind=TIO_IPK),         intent(in) :: ptr_datatype
    integer(kind=TIO_IPK),         intent(in) :: vf_datatype
    
    integer(kind=8), dimension(:), intent(in) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(in) :: vf
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    n = 2
    call tio_write_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, &
         &                           ncomp_datatype, ptr_datatype, vf_datatype,    &
         &                           ncomp, ptr, mixmat, vf, n, err )
    
    
    
    WriteUnstrMatChunk_i8_i8_i8_r8 = err
  end function WriteUnstrMatChunk_i8_i8_i8_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrMat_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrMaterial_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrMat_Chunk_f( fileID, materialID, idx, xfer,
  ! <DATA>                             datatype, nghosts, ncells, mat,
  ! <DATA>                             ncomp_datatype, ptr_datatype, vf_datatype,
  ! <DATA>                             nmixcell, nmixcomp, ncomp, ptr, mixmat, vf )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: xfer, datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)            :: nghosts
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: ncells
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out)           :: mat
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ncomp_datatype, ptr_datatype, vf_datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nmixcell, nmixcomp
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: ncomp, ptr
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mixmat
  ! <DATA>   [REAL CHOICE],    dimension(:), intent(out), optional :: vf
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function ReadUnstrMatChunk_meta( fileID, materialID, idx, xfer, datatype, &
       &                                                  nghosts, ncells, nmixcell, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp
    
    


    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    


    n = 0
    call tio_read_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,            &
         &                          ncells, idum,                                                &
         &                          ddum, ddum, ddum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    if (present(nmixcell)) nmixcell = lnmx
    if (present(nmixcomp)) nmixcomp = lnmc
    








    
    ReadUnstrMatChunk_meta = err
  end function ReadUnstrMatChunk_meta
  integer(kind=TIO_ERRK) function ReadUnstrMatChunk_i4(   fileID, materialID, idx, xfer, datatype, &
       &                                                  nghosts, ncells, mat                )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    integer(kind=4), dimension(:), intent(out) :: mat
    


    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    






    n = 1
    call tio_read_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,            &
         &                          ncells, mat,                                                 &
         &                          ddum, ddum, ddum, lnmx, lnmc, idum, idum, idum, idum, n, err )






    
    ReadUnstrMatChunk_i4 = err
  end function ReadUnstrMatChunk_i4
  integer(kind=TIO_ERRK) function ReadUnstrMatChunk_i8(   fileID, materialID, idx, xfer, datatype, &
       &                                                  nghosts, ncells, mat                )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    integer(kind=8), dimension(:), intent(out) :: mat
    


    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    






    n = 1
    call tio_read_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,            &
         &                          ncells, mat,                                                 &
         &                          ddum, ddum, ddum, lnmx, lnmc, idum, idum, idum, idum, n, err )






    
    ReadUnstrMatChunk_i8 = err
  end function ReadUnstrMatChunk_i8
  integer(kind=TIO_ERRK) function ReadUnstrMatChunk_i4_i4_i4_r4( fileID, materialID, idx, xfer,   &
       &                                            datatype, nghosts, ncells, mat,             &
       &                                            ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                            nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: mat
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    








    n = 2
    call tio_read_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,    &
         &                          ncells, mat,                                         &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,           &
         &                          nmixcell, nmixcomp, ncomp, ptr, mixmat, vf, n, err   )




    
    ReadUnstrMatChunk_i4_i4_i4_r4 = err
  end function ReadUnstrMatChunk_i4_i4_i4_r4
  integer(kind=TIO_ERRK) function ReadUnstrMatChunk_i4_i4_i4_r8( fileID, materialID, idx, xfer,   &
       &                                            datatype, nghosts, ncells, mat,             &
       &                                            ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                            nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: mat
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    








    n = 2
    call tio_read_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,    &
         &                          ncells, mat,                                         &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,           &
         &                          nmixcell, nmixcomp, ncomp, ptr, mixmat, vf, n, err   )




    
    ReadUnstrMatChunk_i4_i4_i4_r8 = err
  end function ReadUnstrMatChunk_i4_i4_i4_r8
  integer(kind=TIO_ERRK) function ReadUnstrMatChunk_i8_i8_i8_r4( fileID, materialID, idx, xfer,   &
       &                                            datatype, nghosts, ncells, mat,             &
       &                                            ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                            nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: mat
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    








    n = 2
    call tio_read_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,    &
         &                          ncells, mat,                                         &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,           &
         &                          nmixcell, nmixcomp, ncomp, ptr, mixmat, vf, n, err   )




    
    ReadUnstrMatChunk_i8_i8_i8_r4 = err
  end function ReadUnstrMatChunk_i8_i8_i8_r4
  integer(kind=TIO_ERRK) function ReadUnstrMatChunk_i8_i8_i8_r8( fileID, materialID, idx, xfer,   &
       &                                            datatype, nghosts, ncells, mat,             &
       &                                            ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                            nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: mat
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    








    n = 2
    call tio_read_unstrmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,    &
         &                          ncells, mat,                                         &
         &                          ncomp_datatype, ptr_datatype, vf_datatype,           &
         &                          nmixcell, nmixcomp, ncomp, ptr, mixmat, vf, n, err   )




    
    ReadUnstrMatChunk_i8_i8_i8_r8 = err
  end function ReadUnstrMatChunk_i8_i8_i8_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrMat_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrMaterial_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrMat_All_f( fileID, materialID, datatype,
  ! <DATA>                             ncells, mat,
  ! <DATA>                             ncomp_datatype, ptr_datatype, vf_datatype,
  ! <DATA>                             nmixcell, nmixcomp, ncomp, ptr, mixmat, vf )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: ncells
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out)           :: mat
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ncomp_datatype, ptr_datatype, vf_datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nmixcell, nmixcomp
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: ncomp, ptr
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mixmat
  ! <DATA>   [REAL CHOICE],    dimension(:), intent(out), optional :: vf
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadUnstrMatAll_meta( fileID, materialID, datatype,  &
       &                                                ncells, nmixcell, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp
    
    


    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    


    n = 0
    call tio_read_unstrmat_all_c( fileID, materialID, datatype, ncells, idum,                 &
         &                        ddum, ddum, ddum, lnmx, lnmc, idum, idum, idum, idum, n, err)
    
    if (present(nmixcell)) nmixcell = lnmx
    if (present(nmixcomp)) nmixcomp = lnmc
    








    
    ReadUnstrMatAll_meta = err
  end function ReadUnstrMatAll_meta
  integer(kind=TIO_ERRK) function ReadUnstrMatAll_i4( fileID, materialID, datatype, ncells, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    integer(kind=4), dimension(:), intent(out) :: mat
    


    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    






    n = 1
    call tio_read_unstrmat_all_c( fileID, materialID, datatype, ncells, mat,                  &
         &                        ddum, ddum, ddum, lnmx, lnmc, idum, idum, idum, idum, n, err)






    
    ReadUnstrMatAll_i4 = err
  end function ReadUnstrMatAll_i4
  integer(kind=TIO_ERRK) function ReadUnstrMatAll_i8( fileID, materialID, datatype, ncells, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    integer(kind=8), dimension(:), intent(out) :: mat
    


    
    integer                 :: ddum = TIO_DATATYPE_NULL_F, idum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    






    n = 1
    call tio_read_unstrmat_all_c( fileID, materialID, datatype, ncells, mat,                  &
         &                        ddum, ddum, ddum, lnmx, lnmc, idum, idum, idum, idum, n, err)






    
    ReadUnstrMatAll_i8 = err
  end function ReadUnstrMatAll_i8
  integer(kind=TIO_ERRK) function ReadUnstrMatAll_i4_i4_i4_r4( fileID, materialID, datatype,   &
       &                                           ncells, mat,                                &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: mat
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    








    n = 2
    call tio_read_unstrmat_all_c( fileID, materialID, datatype, ncells, mat,        &
         &                        ncomp_datatype, ptr_datatype, vf_datatype,        &
         &                        nmixcell, nmixcomp, ncomp, ptr, mixmat, vf, n, err)




    
    ReadUnstrMatAll_i4_i4_i4_r4 = err
  end function ReadUnstrMatAll_i4_i4_i4_r4
  integer(kind=TIO_ERRK) function ReadUnstrMatAll_i4_i4_i4_r8( fileID, materialID, datatype,   &
       &                                           ncells, mat,                                &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: mat
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    








    n = 2
    call tio_read_unstrmat_all_c( fileID, materialID, datatype, ncells, mat,        &
         &                        ncomp_datatype, ptr_datatype, vf_datatype,        &
         &                        nmixcell, nmixcomp, ncomp, ptr, mixmat, vf, n, err)




    
    ReadUnstrMatAll_i4_i4_i4_r8 = err
  end function ReadUnstrMatAll_i4_i4_i4_r8
  integer(kind=TIO_ERRK) function ReadUnstrMatAll_i8_i8_i8_r4( fileID, materialID, datatype,   &
       &                                           ncells, mat,                                &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: mat
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    








    n = 2
    call tio_read_unstrmat_all_c( fileID, materialID, datatype, ncells, mat,        &
         &                        ncomp_datatype, ptr_datatype, vf_datatype,        &
         &                        nmixcell, nmixcomp, ncomp, ptr, mixmat, vf, n, err)




    
    ReadUnstrMatAll_i8_i8_i8_r4 = err
  end function ReadUnstrMatAll_i8_i8_i8_r4
  integer(kind=TIO_ERRK) function ReadUnstrMatAll_i8_i8_i8_r8( fileID, materialID, datatype,   &
       &                                           ncells, mat,                                &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: mat
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    
    
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    








    n = 2
    call tio_read_unstrmat_all_c( fileID, materialID, datatype, ncells, mat,        &
         &                        ncomp_datatype, ptr_datatype, vf_datatype,        &
         &                        nmixcell, nmixcomp, ncomp, ptr, mixmat, vf, n, err)




    
    ReadUnstrMatAll_i8_i8_i8_r8 = err
  end function ReadUnstrMatAll_i8_i8_i8_r8
  
  


  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrMat_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrMaterial_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrMat_Section_f( fileID, materialID, sectionID,
  ! <DATA>                             xfer, datatype, ncells, mat,
  ! <DATA>                             ncomp_datatype, ptr_datatype, vf_datatype,
  ! <DATA>                             nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: sectionID
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: ncells
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mat
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ncomp_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: ptr_datatype
  ! <DATA>   integer(kind=TIO_IPK),          intent(in),  optional :: vf_datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nmixcell, nmixcomp
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: ncomp, ptr, mixmat
  ! <DATA>   [REAL CHOICE],    dimension(:), intent(out), optional :: vf
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadUnstrMatSection_meta( fileID, materialID, sectionID, xfer, datatype,  &
       &                                                    ncells, nmixcell, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp
    
    


    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    


    
    n = 0
    
    call tio_read_unstrmat_section_c( fileID, materialID, sectionID, xfer, datatype, ncells,     &
         &                            idum, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    if (present(nmixcell)) nmixcell = lnmx
    if (present(nmixcomp)) nmixcomp = lnmc


    
    
    
    
    ReadUnstrMatSection_meta = err
  end function ReadUnstrMatSection_meta
  integer(kind=TIO_ERRK) function ReadUnstrMatSection_i4( fileID, materialID, sectionID, xfer, datatype, ncells, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    integer(kind=4), dimension(:), intent(out) :: mat
    


    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    


    
    
    n = 1
    call tio_read_unstrmat_section_c( fileID, materialID, sectionID, xfer, datatype, ncells,    &
         &                            mat, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    
    
    
    ReadUnstrMatSection_i4 = err
  end function ReadUnstrMatSection_i4
  integer(kind=TIO_ERRK) function ReadUnstrMatSection_i8( fileID, materialID, sectionID, xfer, datatype, ncells, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    integer(kind=8), dimension(:), intent(out) :: mat
    


    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    


    
    
    n = 1
    call tio_read_unstrmat_section_c( fileID, materialID, sectionID, xfer, datatype, ncells,    &
         &                            mat, dtdum, dtdum, dtdum, lnmx, lnmc, idum, idum, idum, idum, n, err )
    
    
    
    
    ReadUnstrMatSection_i8 = err
  end function ReadUnstrMatSection_i8
  integer(kind=TIO_ERRK) function ReadUnstrMatSection_i4_i4_i4_r4( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ncells, mat,                                &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: mat
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    


    
    
    
    n = 2
    call tio_read_unstrmat_section_c( fileID, materialID, sectionID, xfer, datatype, ncells,     &
         &                            mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                            ncomp, ptr, mixmat, vf, n, err )
    
    
    
    ReadUnstrMatSection_i4_i4_i4_r4 = err
  end function ReadUnstrMatSection_i4_i4_i4_r4
  integer(kind=TIO_ERRK) function ReadUnstrMatSection_i4_i4_i4_r8( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ncells, mat,                                &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=4), dimension(:), intent(out) :: mat
    integer(kind=4), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    


    
    
    
    n = 2
    call tio_read_unstrmat_section_c( fileID, materialID, sectionID, xfer, datatype, ncells,     &
         &                            mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                            ncomp, ptr, mixmat, vf, n, err )
    
    
    
    ReadUnstrMatSection_i4_i4_i4_r8 = err
  end function ReadUnstrMatSection_i4_i4_i4_r8
  integer(kind=TIO_ERRK) function ReadUnstrMatSection_i8_i8_i8_r4( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ncells, mat,                                &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: mat
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=4),    dimension(:), intent(out) :: vf
    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    


    
    
    
    n = 2
    call tio_read_unstrmat_section_c( fileID, materialID, sectionID, xfer, datatype, ncells,     &
         &                            mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                            ncomp, ptr, mixmat, vf, n, err )
    
    
    
    ReadUnstrMatSection_i8_i8_i8_r4 = err
  end function ReadUnstrMatSection_i8_i8_i8_r4
  integer(kind=TIO_ERRK) function ReadUnstrMatSection_i8_i8_i8_r8( fileID, materialID, sectionID, xfer, datatype,   &
       &                                           ncells, mat,                                &
       &                                           ncomp_datatype, ptr_datatype, vf_datatype,  &
       &                                           nmixcell, nmixcomp, ncomp, ptr, mixmat, vf  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: ncells
    
    
    
    integer(kind=TIO_IPK),   intent(in)        :: ncomp_datatype, ptr_datatype, vf_datatype
    integer(kind=TIO_SIZEK), intent(out)       :: nmixcell, nmixcomp


    integer(kind=8), dimension(:), intent(out) :: mat
    integer(kind=8), dimension(:), intent(out) :: ncomp, ptr, mixmat
    real(kind=8),    dimension(:), intent(out) :: vf
    
    
    integer                 :: n, idum, dtdum
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    dtdum = TIO_DATATYPE_NULL_F
    
    


    
    
    
    n = 2
    call tio_read_unstrmat_section_c( fileID, materialID, sectionID, xfer, datatype, ncells,     &
         &                            mat, ncomp_datatype, ptr_datatype, vf_datatype, nmixcell, nmixcomp,   &
         &                            ncomp, ptr, mixmat, vf, n, err )
    
    
    
    ReadUnstrMatSection_i8_i8_i8_r8 = err
  end function ReadUnstrMatSection_i8_i8_i8_r8
  
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **           POINTMATS            ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_PointMat_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_PointMaterial_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_PointMat_Chunk_f( fileID, materialID, idx, xfer,
  ! <DATA>                                                             datatype, mat )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in) :: materialID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in) :: idx
  ! <DATA>   integer(kind=TIO_IPK),          intent(in) :: xfer, datatype
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(in) :: mat
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function WritePointMatChunk_i4( fileID, materialID, idx, xfer,  &
       &                                                 datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK),       intent(in) :: fileID
    integer(kind=TIO_OBJK),        intent(in) :: materialID
    integer(kind=TIO_SIZEK),       intent(in) :: idx
    integer(kind=TIO_IPK),         intent(in) :: xfer, datatype
    
    integer(kind=4), dimension(:), intent(in) :: mat
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_write_pointmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, err )
    
    WritePointMatChunk_i4 = err
  end function WritePointMatChunk_i4
  integer(kind=TIO_ERRK) function WritePointMatChunk_i8( fileID, materialID, idx, xfer,  &
       &                                                 datatype, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK),       intent(in) :: fileID
    integer(kind=TIO_OBJK),        intent(in) :: materialID
    integer(kind=TIO_SIZEK),       intent(in) :: idx
    integer(kind=TIO_IPK),         intent(in) :: xfer, datatype
    
    integer(kind=8), dimension(:), intent(in) :: mat
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_write_pointmat_chunk_c( fileID, materialID, idx, xfer, datatype, mat, err )
    
    WritePointMatChunk_i8 = err
  end function WritePointMatChunk_i8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointMat_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointMaterial_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointMat_Chunk_f( fileID, materialID, idx, xfer,
  ! <DATA>                                                            datatype, nghosts, nnodes, mat )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: xfer, datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(in)            :: nghosts
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: nnodes
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mat
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function ReadPointMatChunk_meta( fileID, materialID, idx, xfer,  &
       &                                                  datatype, nghosts, nnodes       )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    


    n = 0
    call tio_read_pointmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,  &
         &                          nnodes, idum, n, err )
        






    ReadPointMatChunk_meta = err
  end function ReadPointMatChunk_meta
  integer(kind=TIO_ERRK) function ReadPointMatChunk_i4(   fileID, materialID, idx, xfer,  &
       &                                                  datatype, nghosts, nnodes, mat  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    integer(kind=4), dimension(:), intent(out) :: mat
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    






    n = 1
    call tio_read_pointmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,  &
         &                          nnodes, mat, n, err )




    ReadPointMatChunk_i4 = err
  end function ReadPointMatChunk_i4
  integer(kind=TIO_ERRK) function ReadPointMatChunk_i8(   fileID, materialID, idx, xfer,  &
       &                                                  datatype, nghosts, nnodes, mat  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    integer(kind=8), dimension(:), intent(out) :: mat
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    






    n = 1
    call tio_read_pointmat_chunk_c( fileID, materialID, idx, xfer, datatype, nghosts,  &
         &                          nnodes, mat, n, err )




    ReadPointMatChunk_i8 = err
  end function ReadPointMatChunk_i8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointMat_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointMaterial_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointMat_All_f( fileID, materialID, datatype,
  ! <DATA>                                                          nnodes, mat )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: nnodes
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mat
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadPointMatAll_meta( fileID, materialID, datatype, nnodes )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    


    n = 0
    call tio_read_pointmat_all_c( fileID, materialID, datatype, nnodes, idum, n, err)
        




    


    ReadPointMatAll_meta = err
  end function ReadPointMatAll_meta
  integer(kind=TIO_ERRK) function ReadPointMatAll_i4( fileID, materialID, datatype, nnodes, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    integer(kind=4), dimension(:), intent(out) :: mat
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    






    n = 1
    call tio_read_pointmat_all_c( fileID, materialID, datatype, nnodes, mat, n, err)


    


    ReadPointMatAll_i4 = err
  end function ReadPointMatAll_i4
  integer(kind=TIO_ERRK) function ReadPointMatAll_i8( fileID, materialID, datatype, nnodes, mat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    integer(kind=8), dimension(:), intent(out) :: mat
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    






    n = 1
    call tio_read_pointmat_all_c( fileID, materialID, datatype, nnodes, mat, n, err)


    


    ReadPointMatAll_i8 = err
  end function ReadPointMatAll_i8
  
  


  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointMat_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointMaterial_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointMat_Section_f( fileID, materialID, sectionID,
  ! <DATA>                             xfer, datatype, nnodes, mat )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: materialID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: sectionID
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),          intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out)           :: nnodes
  ! <DATA>   [INTEGER CHOICE], dimension(:), intent(out), optional :: mat
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadPointMatSection_meta( fileID, materialID, sectionID, xfer, datatype,  &
       &                                                    nnodes )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
        
    
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err
    
    
    
    n = 0
    
    call tio_read_pointmat_section_c( fileID, materialID, sectionID, xfer, datatype, nnodes, idum, n, err )
    
    
    
    ReadPointMatSection_meta = err
  end function ReadPointMatSection_meta
  integer(kind=TIO_ERRK) function ReadPointMatSection_i4( fileID, materialID, sectionID, xfer, datatype, nnodes, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
        
    integer(kind=4), dimension(:), intent(out) :: mat
    
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_pointmat_section_c( fileID, materialID, sectionID, xfer, datatype, nnodes, mat, n, err )
    
    
    ReadPointMatSection_i4 = err
  end function ReadPointMatSection_i4
  integer(kind=TIO_ERRK) function ReadPointMatSection_i8( fileID, materialID, sectionID, xfer, datatype, nnodes, mat)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: materialID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
        
    integer(kind=8), dimension(:), intent(out) :: mat
    
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 1
    call tio_read_pointmat_section_c( fileID, materialID, sectionID, xfer, datatype, nnodes, mat, n, err )
    
    
    ReadPointMatSection_i8 = err
  end function ReadPointMatSection_i8
  
  
  ! ********************************************************************************************** !
  
  
end module TIO_Material_mod






!
!  EOF

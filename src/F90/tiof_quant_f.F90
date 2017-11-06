!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!
!-*-Mode: F90;-*-  emacs mode specifier


module TIO_Quant_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  use TIO_Utils_mod
  
  implicit none
  
  
  private
  public :: TIO_List_Quants_f
  public :: TIO_Create_Quant_f, TIO_Open_Quant_f, TIO_Open_Quant_idx_f
  public :: TIO_Get_Quant_Info_f
  public :: TIO_Set_Quant_Range_f, TIO_Get_Quant_Range_f
  public :: TIO_Close_Quant_f
  
  public :: TIO_Write_QuadQuant_Chunk_f, TIO_Read_QuadQuant_Chunk_f
  public :: TIO_Read_QuadQuant_All_f, TIO_Read_QuadQuant_Section_f
  
  public :: TIO_Write_UnstrQuant_Chunk_f, TIO_Read_UnstrQuant_Chunk_f
  public :: TIO_Read_UnstrQuant_All_f, TIO_Read_UnstrQuant_Section_f
  
  public :: TIO_Write_PointQuant_Chunk_f, TIO_Read_PointQuant_Chunk_f
  public :: TIO_Read_PointQuant_All_f, TIO_Read_PointQuant_Section_f
  
  
  interface TIO_Set_Quant_Range_f
    module procedure SetRange_i4, SetRange_i8, SetRange_r4, SetRange_r8, SetRange_l4
  end interface
  
  interface TIO_Get_Quant_Range_f
    module procedure GetRange_i4, GetRange_i8, GetRange_r4, GetRange_r8, GetRange_l4
  end interface
  
  
  interface TIO_Write_QuadQuant_Chunk_f
    module procedure WriteQuadQuantChunk_1D_i4    ! integer(kind=4), dimension(:)
    module procedure WriteQuadQuantChunk_1D_i8    ! integer(kind=8), dimension(:)
    module procedure WriteQuadQuantChunk_1D_r4    ! real(kind=4),    dimension(:)
    module procedure WriteQuadQuantChunk_1D_r8    ! real(kind=8),    dimension(:)
    module procedure WriteQuadQuantChunk_1D_l4    ! logical(kind=4), dimension(:)
    module procedure WriteQuadQuantChunk_2D_i4    ! integer(kind=4), dimension(:,:)
    module procedure WriteQuadQuantChunk_2D_i8    ! integer(kind=8), dimension(:,:)
    module procedure WriteQuadQuantChunk_2D_r4    ! real(kind=4),    dimension(:,:)
    module procedure WriteQuadQuantChunk_2D_r8    ! real(kind=8),    dimension(:,:)
    module procedure WriteQuadQuantChunk_2D_l4    ! logical(kind=4), dimension(,::)
    module procedure WriteQuadQuantChunk_3D_i4    ! integer(kind=4), dimension(:,:,:)
    module procedure WriteQuadQuantChunk_3D_i8    ! integer(kind=8), dimension(:,:,:)
    module procedure WriteQuadQuantChunk_3D_r4    ! real(kind=4),    dimension(:,:,:)
    module procedure WriteQuadQuantChunk_3D_r8    ! real(kind=8),    dimension(:,:,:)
    module procedure WriteQuadQuantChunk_3D_l4    ! logical(kind=4), dimension(:,:,:)
  end interface
  
  interface TIO_Read_QuadQuant_Chunk_f
    module procedure ReadQuadQuantChunk_meta
    module procedure ReadQuadQuantChunk_1D_i4
    module procedure ReadQuadQuantChunk_1D_i8
    module procedure ReadQuadQuantChunk_1D_r4
    module procedure ReadQuadQuantChunk_1D_r8
    module procedure ReadQuadQuantChunk_1D_l4
    module procedure ReadQuadQuantChunk_2D_i4
    module procedure ReadQuadQuantChunk_2D_i8
    module procedure ReadQuadQuantChunk_2D_r4
    module procedure ReadQuadQuantChunk_2D_r8
    module procedure ReadQuadQuantChunk_2D_l4
    module procedure ReadQuadQuantChunk_3D_i4
    module procedure ReadQuadQuantChunk_3D_i8
    module procedure ReadQuadQuantChunk_3D_r4
    module procedure ReadQuadQuantChunk_3D_r8
    module procedure ReadQuadQuantChunk_3D_l4
  end interface
  
  interface TIO_Read_QuadQuant_All_f
    module procedure ReadQuadQuantAll_meta     ! no data, just size metadata
    module procedure ReadQuadQuantAll_1D_i4
    module procedure ReadQuadQuantAll_1D_i8
    module procedure ReadQuadQuantAll_1D_r4
    module procedure ReadQuadQuantAll_1D_r8
    module procedure ReadQuadQuantAll_1D_l4
    module procedure ReadQuadQuantAll_2D_i4
    module procedure ReadQuadQuantAll_2D_i8
    module procedure ReadQuadQuantAll_2D_r4
    module procedure ReadQuadQuantAll_2D_r8
    module procedure ReadQuadQuantAll_2D_l4
    module procedure ReadQuadQuantAll_3D_i4
    module procedure ReadQuadQuantAll_3D_i8
    module procedure ReadQuadQuantAll_3D_r4
    module procedure ReadQuadQuantAll_3D_r8
    module procedure ReadQuadQuantAll_3D_l4
  end interface
  
  interface TIO_Read_QuadQuant_Section_f
    module procedure ReadQuadQuantSection_meta
    module procedure ReadQuadQuantSection_1D_i4
    module procedure ReadQuadQuantSection_1D_i8
    module procedure ReadQuadQuantSection_1D_r4
    module procedure ReadQuadQuantSection_1D_r8
    module procedure ReadQuadQuantSection_1D_l4
    module procedure ReadQuadQuantSection_2D_i4
    module procedure ReadQuadQuantSection_2D_i8
    module procedure ReadQuadQuantSection_2D_r4
    module procedure ReadQuadQuantSection_2D_r8
    module procedure ReadQuadQuantSection_2D_l4
    module procedure ReadQuadQuantSection_3D_i4
    module procedure ReadQuadQuantSection_3D_i8
    module procedure ReadQuadQuantSection_3D_r4
    module procedure ReadQuadQuantSection_3D_r8
    module procedure ReadQuadQuantSection_3D_l4
  end interface
  
  
  interface TIO_Write_UnstrQuant_Chunk_f
    module procedure WriteUnstrQuantChunk_i4   ! integer(kind=4) data
    module procedure WriteUnstrQuantChunk_i8   ! integer(kind=8) data
    module procedure WriteUnstrQuantChunk_r4   ! real(kind=4) data
    module procedure WriteUnstrQuantChunk_r8   ! real(kind=8) data
    module procedure WriteUnstrQuantChunk_l4   ! logical(kind=4) data
  end interface
  
  interface TIO_Read_UnstrQuant_Chunk_f
    module procedure ReadUnstrQuantChunk_meta
    module procedure ReadUnstrQuantChunk_i4
    module procedure ReadUnstrQuantChunk_i8
    module procedure ReadUnstrQuantChunk_r4
    module procedure ReadUnstrQuantChunk_r8
    module procedure ReadUnstrQuantChunk_l4
  end interface
  
  interface TIO_Read_UnstrQuant_All_f
    module procedure ReadUnstrQuantAll_meta    ! no data, just size metadata
    module procedure ReadUnstrQuantAll_i4
    module procedure ReadUnstrQuantAll_i8
    module procedure ReadUnstrQuantAll_r4
    module procedure ReadUnstrQuantAll_r8
    module procedure ReadUnstrQuantAll_l4
  end interface
  
  interface TIO_Read_UnstrQuant_Section_f
    module procedure ReadUnstrQuantSection_meta
    module procedure ReadUnstrQuantSection_i4
    module procedure ReadUnstrQuantSection_i8
    module procedure ReadUnstrQuantSection_r4
    module procedure ReadUnstrQuantSection_r8
    module procedure ReadUnstrQuantSection_l4
  end interface
  
  
  interface TIO_Write_PointQuant_Chunk_f
    module procedure WritePointQuantChunk_i4   ! integer(kind=4) data
    module procedure WritePointQuantChunk_i8   ! integer(kind=8) data
    module procedure WritePointQuantChunk_r4   ! real(kind=4) data
    module procedure WritePointQuantChunk_r8   ! real(kind=8) data
    module procedure WritePointQuantChunk_l4   ! real(kind=8) data
  end interface
  
  interface TIO_Read_PointQuant_Chunk_f
    module procedure ReadPointQuantChunk_meta
    module procedure ReadPointQuantChunk_i4
    module procedure ReadPointQuantChunk_i8
    module procedure ReadPointQuantChunk_r4
    module procedure ReadPointQuantChunk_r8
    module procedure ReadPointQuantChunk_l4
  end interface
  
  interface TIO_Read_PointQuant_All_f
    module procedure ReadPointQuantAll_meta       ! no data, just size metadata
    module procedure ReadPointQuantAll_i4
    module procedure ReadPointQuantAll_i8
    module procedure ReadPointQuantAll_r4
    module procedure ReadPointQuantAll_r8
    module procedure ReadPointQuantAll_l4
  end interface
  
  interface TIO_Read_PointQuant_Section_f
    module procedure ReadPointQuantSection_meta
    module procedure ReadPointQuantSection_i4
    module procedure ReadPointQuantSection_i8
    module procedure ReadPointQuantSection_r4
    module procedure ReadPointQuantSection_r8
    module procedure ReadPointQuantSection_l4
  end interface
  
  
contains
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_List_Quants_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_List_Quants
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_List_Quants_f( fileID, meshID, nobj, names )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
  ! <DATA>   character(len=*), dimension(:), intent(out), optional :: names
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_List_Quants_f( fileID, meshID, nobj, names )
    
    implicit none
    
    integer(kind=TIO_FILEK),        intent(in)            :: fileID
    integer(kind=TIO_OBJK),         intent(in)            :: meshID
    integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
    character(len=*), dimension(:), intent(out), optional :: names
    
    
    character :: cdummy
    
    integer(kind=TIO_SIZEK) :: ln
    integer(kind=TIO_ERRK)  :: err
    
    
    ln = -1
    call tio_list_quants_c(fileID, meshID, ln, cdummy, err)
    TIO_List_Quants_f = err
    if (TIO_List_Quants_f /= TIO_SUCCESS_F) return
    
    if (present(nobj)) nobj = ln
    
    if (present(names)) then
      TIO_List_Quants_f = listnames("TIO_List_Quants", LISTCLASS_QUANT, fileID, meshID, ln, names)
    end if
    
  end function TIO_List_Quants_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Create_Quant_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Create_Quant
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Create_Quant_f( fileID, meshID, name, quantID,
  ! <DATA>                                                     datatype, centring, nghosts,
  ! <DATA>                                                     ismixed, units )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: meshID
  ! <DATA>   character(len=*),        intent(in)  :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out) :: quantID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)  :: datatype, centring
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: nghosts
  ! <DATA>   logical(kind=TIO_LPK),   intent(in)  :: ismixed
  ! <DATA>   character(len=*),        intent(in)  :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Create_Quant_f( fileID, meshID, name, quantID,              &
       &                                              datatype, centring, nghosts, ismixed, units )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    character(len=*),        intent(in)  :: name
    integer(kind=TIO_OBJK),  intent(out) :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype, centring
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    logical(kind=TIO_LPK),   intent(in)  :: ismixed
    character(len=*),        intent(in)  :: units
    
    integer :: name_len, units_len
    integer :: lismixed
    integer(kind=TIO_ERRK) :: err
    


    
    TIO_Create_Quant_f = setlen("TIO_Create_Quant_f", "name", name, name_len)
    if (TIO_Create_Quant_f /= TIO_SUCCESS_F) return
    
    TIO_Create_Quant_f = setlen("TIO_Create_Quant_f", "units", units, units_len)
    if (TIO_Create_Quant_f /= TIO_SUCCESS_F) return
    
    if (ismixed) then
      lismixed = 1
    else
      lismixed = 0
    end if
    
    
    call tio_create_quant_c( fileID, meshID, name, name_len, quantID,                     &
         &                   datatype, centring, nghosts, lismixed, units, units_len, err )
    
    
    TIO_Create_Quant_f = err
    
  end function TIO_Create_Quant_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Quant_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Quant
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Quant_f( fileID, meshID, name, quantID,
  ! <DATA>                                                   datatype, centring, nghosts,
  ! <DATA>                                                   ismixed, units )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: meshID
  ! <DATA>   character(len=*),        intent(in)            :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out)           :: quantID
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: datatype, centring
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nghosts
  ! <DATA>   logical(kind=TIO_LPK),   intent(out), optional :: ismixed
  ! <DATA>   character(len=*),        intent(out), optional :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Quant_f( fileID, meshID, name, quantID,              &
       &                                            datatype, centring, nghosts, ismixed, units )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: meshID
    character(len=*),        intent(in)            :: name
    integer(kind=TIO_OBJK),  intent(out)           :: quantID
    integer(kind=TIO_IPK),   intent(out), optional :: datatype, centring
    integer(kind=TIO_SIZEK), intent(out), optional :: nghosts
    logical(kind=TIO_LPK),   intent(out), optional :: ismixed
    character(len=*),        intent(out), optional :: units
    
    
    integer                     :: name_len
    integer                     :: ldatatype, lcentring, lismixed
    integer(kind=TIO_SIZEK)     :: lnghosts
    character(len=TIO_STRLEN_F) :: lunits=""
    integer(kind=TIO_ERRK)      :: err
    
    
    TIO_Open_Quant_f = setlen("TIO_Open_Quant_f", "name", name, name_len)
    if (TIO_Open_Quant_f /= TIO_SUCCESS_F) return
    
    call tio_open_quant_c( fileID, meshID, -1, name, name_len, quantID,                       &
         &                 ldatatype, lcentring, lnghosts, lismixed, lunits, len(lunits), err )
    
    TIO_Open_Quant_f = err
    if (TIO_Open_Quant_f /= TIO_SUCCESS_F) return
    
    
    if (present(datatype)) datatype = ldatatype
    if (present(centring)) centring = lcentring
    if (present(nghosts))  nghosts  = lnghosts
    if (present(ismixed))  ismixed  = (lismixed > 0)
    
    if (present(units)) then
      TIO_Open_Quant_f = setstr("TIO_Open_Quant_f", "units", lunits, units)
      if (TIO_Open_Quant_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Quant_f(fileID, quantID)
        return
      end if
    end if
    
  end function TIO_Open_Quant_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Quant_idx_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Quant_idx
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Quant_idx_f( fileID, meshID, idx, name, quantID
  ! <DATA>                                                       datatype, centring, nghosts,
  ! <DATA>                                                       ismixed, units )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: idx
  ! <DATA>   character(len=*),        intent(out)           :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out)           :: quantID
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: datatype, centring
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nghosts
  ! <DATA>   logical(kind=TIO_LPK),   intent(out), optional :: ismixed
  ! <DATA>   character(len=*),        intent(out), optional :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Quant_idx_f( fileID, meshID, idx, name, quantID,       &
       &                                              datatype, centring, nghosts, ismixed, units )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: meshID
    integer(kind=TIO_SIZEK), intent(in)            :: idx
    character(len=*),        intent(out)           :: name
    integer(kind=TIO_OBJK),  intent(out)           :: quantID
    integer(kind=TIO_IPK),   intent(out), optional :: datatype, centring
    integer(kind=TIO_SIZEK), intent(out), optional :: nghosts
    logical(kind=TIO_LPK),   intent(out), optional :: ismixed
    character(len=*),        intent(out), optional :: units
    
    
    integer                     :: ldatatype, lcentring, lismixed
    integer(kind=TIO_SIZEK)     :: lnghosts
    character(len=TIO_STRLEN_F) :: lname="", lunits=""
    integer(kind=TIO_ERRK)      :: err
    
    
    call tio_open_quant_c( fileID, meshID, idx, lname, len(lname), quantID,                   &
         &                 ldatatype, lcentring, lnghosts, lismixed, lunits, len(lunits), err )
    
    TIO_Open_Quant_idx_f = err
    if (TIO_Open_Quant_idx_f /= TIO_SUCCESS_F) return
    
    TIO_Open_Quant_idx_f = setstr("TIO_Open_Quant_idx_f", "name", lname, name)
    if (TIO_Open_Quant_idx_f /= TIO_SUCCESS_F) then
      err = TIO_Close_Quant_f(fileID, quantID)
      return
    end if
    
    if (present(datatype)) datatype = ldatatype
    if (present(centring)) centring = lcentring
    if (present(nghosts))  nghosts  = lnghosts
    if (present(ismixed))  ismixed  = (lismixed > 0)
    
    if (present(units)) then
      TIO_Open_Quant_idx_f = setstr("TIO_Open_Quant_idx_f", "units", lunits, units)
      if (TIO_Open_Quant_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Quant_f(fileID, quantID)
        return
      end if
    end if
    
  end function TIO_Open_Quant_idx_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Quant_Info_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Quant_Info
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Quant_Info_f( fileID, quantID,
  ! <DATA>                                                       name, datatype, centring,
  ! <DATA>                                                       nghosts, ismixed, units )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID
  ! <DATA>   character(len=*),        intent(out), optional :: name
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: datatype, centring
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nghosts
  ! <DATA>   logical(kind=TIO_LPK),   intent(out), optional :: ismixed
  ! <DATA>   character(len=*),        intent(out), optional :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer function TIO_Get_Quant_Info_f( fileID, quantID, name, datatype, centring,  &
       &                                 nghosts, ismixed, units )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: quantID
    character(len=*),        intent(out), optional :: name
    integer(kind=TIO_IPK),   intent(out), optional :: datatype, centring
    integer(kind=TIO_SIZEK), intent(out), optional :: nghosts
    logical(kind=TIO_LPK),   intent(out), optional :: ismixed
    character(len=*),        intent(out), optional :: units
    
    integer                     :: ldatatype, lcentring, lismixed
    integer(kind=TIO_SIZEK)     :: lnghosts
    character(len=TIO_STRLEN_F) :: lname="", lunits=""
    integer(kind=TIO_ERRK)      :: err
    
    
    call tio_get_quant_info_c( fileID, quantID, lname, len(lname),   &
         &                     ldatatype, lcentring, lnghosts, lismixed, lunits, len(lunits), err )
    
    TIO_Get_Quant_Info_f = err
    if (TIO_Get_Quant_Info_f /= TIO_SUCCESS_F) return
    
    if (present(name)) then
      TIO_Get_Quant_Info_f = setstr("TIO_Get_Quant_Info_f", "name", lname, name)
      if (TIO_Get_Quant_Info_f /= TIO_SUCCESS_F) then
        return
      end if
    end if
    
    if (present(datatype)) datatype = ldatatype
    if (present(centring)) centring = lcentring
    if (present(nghosts))  nghosts  = lnghosts
    if (present(ismixed))  ismixed  = (lismixed > 0)
    
    if (present(units)) then
      TIO_Get_Quant_Info_f = setstr("TIO_Get_Quant_Info_f", "units", lunits, units)
      if (TIO_Get_Quant_Info_f /= TIO_SUCCESS_F) then
        return
      end if
    end if
    
  end function TIO_Get_Quant_Info_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Quant_Range_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Quant_Range
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Quant_Range_f( fileID, quantID, datatype,
  ! <DATA>                                                        qmin, qmax )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: quantID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in) :: datatype
  ! <DATA>   [CHOICE],                intent(in) :: qmin, qmax
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function SetRange_i4( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=4), intent(in) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_set_quant_range_c( fileID, quantID, datatype, qmin, qmax, err )
    
    
    SetRange_i4 = err
  end function SetRange_i4
  integer(kind=TIO_ERRK) function SetRange_i8( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=8), intent(in) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_set_quant_range_c( fileID, quantID, datatype, qmin, qmax, err )
    
    
    SetRange_i8 = err
  end function SetRange_i8
  integer(kind=TIO_ERRK) function SetRange_r4( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=4),    intent(in) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_set_quant_range_c( fileID, quantID, datatype, qmin, qmax, err )
    
    
    SetRange_r4 = err
  end function SetRange_r4
  integer(kind=TIO_ERRK) function SetRange_r8( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=8),    intent(in) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_set_quant_range_c( fileID, quantID, datatype, qmin, qmax, err )
    
    
    SetRange_r8 = err
  end function SetRange_r8
  integer(kind=TIO_ERRK) function SetRange_l4( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    logical(kind=4), intent(in) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_set_quant_range_c( fileID, quantID, datatype, qmin, qmax, err )
    
    
    SetRange_l4 = err
  end function SetRange_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Quant_Range_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Quant_Range
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Quant_Range_f( fileID, quantID, datatype,
  ! <DATA>                                                        qmin, qmax )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: quantID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)  :: datatype
  ! <DATA>   [CHOICE],                intent(out) :: qmin, qmax
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function GetRange_i4( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=4), intent(out) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_quant_range_c( fileID, quantID, datatype, qmin, qmax, err)
    
    
    GetRange_i4 = err
  end function GetRange_i4
  integer(kind=TIO_ERRK) function GetRange_i8( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=8), intent(out) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_quant_range_c( fileID, quantID, datatype, qmin, qmax, err)
    
    
    GetRange_i8 = err
  end function GetRange_i8
  integer(kind=TIO_ERRK) function GetRange_r4( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=4),    intent(out) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_quant_range_c( fileID, quantID, datatype, qmin, qmax, err)
    
    
    GetRange_r4 = err
  end function GetRange_r4
  integer(kind=TIO_ERRK) function GetRange_r8( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=8),    intent(out) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_quant_range_c( fileID, quantID, datatype, qmin, qmax, err)
    
    
    GetRange_r8 = err
  end function GetRange_r8
  integer(kind=TIO_ERRK) function GetRange_l4( fileID, quantID, datatype, qmin, qmax )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    logical(kind=4), intent(out) :: qmin, qmax
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_quant_range_c( fileID, quantID, datatype, qmin, qmax, err)
    
    
    GetRange_l4 = err
  end function GetRange_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Close_Quant_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Close_Quant
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Close_Quant_f( fileID, quantID )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: quantID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Close_Quant_f( fileID, quantID )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_close_quant_c(fileID, quantID, err)
    
    TIO_Close_Quant_f = err
    
  end function TIO_Close_Quant_f
  
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **          QUADQUANTS            ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_QuadQuant_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_QuadQuant_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_QuadQuant_Chunk_f( fileID, quantID, idx, xfer,
  ! <DATA>                                                              datatype, qdat, qmix )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)           :: quantID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)           :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)           :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)           :: datatype
  ! <DATA>   [CHOICE], [DIMENSION],   intent(in)           :: qdat
  ! <DATA>   [CHOICE], dimension(:),  intent(in), optional :: qmix
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_1D_i4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=4), dimension(:),     intent(in) :: qdat




    integer(kind=4), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_1D_i4 = err
  end function WriteQuadQuantChunk_1D_i4
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_1D_i8( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=8), dimension(:),     intent(in) :: qdat




    integer(kind=8), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_1D_i8 = err
  end function WriteQuadQuantChunk_1D_i8
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_1D_r4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=4),    dimension(:),     intent(in) :: qdat




    real(kind=4),    dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_1D_r4 = err
  end function WriteQuadQuantChunk_1D_r4
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_1D_r8( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=8),    dimension(:),     intent(in) :: qdat




    real(kind=8),    dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_1D_r8 = err
  end function WriteQuadQuantChunk_1D_r8
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_1D_l4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    logical(kind=4), dimension(:),     intent(in) :: qdat




    logical(kind=4), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_1D_l4 = err
  end function WriteQuadQuantChunk_1D_l4
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_2D_i4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=4), dimension(:,:),   intent(in) :: qdat




    integer(kind=4), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_2D_i4 = err
  end function WriteQuadQuantChunk_2D_i4
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_2D_i8( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=8), dimension(:,:),   intent(in) :: qdat




    integer(kind=8), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_2D_i8 = err
  end function WriteQuadQuantChunk_2D_i8
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_2D_r4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=4),    dimension(:,:),   intent(in) :: qdat




    real(kind=4),    dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_2D_r4 = err
  end function WriteQuadQuantChunk_2D_r4
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_2D_r8( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=8),    dimension(:,:),   intent(in) :: qdat




    real(kind=8),    dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_2D_r8 = err
  end function WriteQuadQuantChunk_2D_r8
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_2D_l4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    logical(kind=4), dimension(:,:),   intent(in) :: qdat




    logical(kind=4), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_2D_l4 = err
  end function WriteQuadQuantChunk_2D_l4
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_3D_i4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=4), dimension(:,:,:), intent(in) :: qdat




    integer(kind=4), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_3D_i4 = err
  end function WriteQuadQuantChunk_3D_i4
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_3D_i8( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=8), dimension(:,:,:), intent(in) :: qdat




    integer(kind=8), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_3D_i8 = err
  end function WriteQuadQuantChunk_3D_i8
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_3D_r4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=4),    dimension(:,:,:), intent(in) :: qdat




    real(kind=4),    dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_3D_r4 = err
  end function WriteQuadQuantChunk_3D_r4
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_3D_r8( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=8),    dimension(:,:,:), intent(in) :: qdat




    real(kind=8),    dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_3D_r8 = err
  end function WriteQuadQuantChunk_3D_r8
  integer(kind=TIO_ERRK) function WriteQuadQuantChunk_3D_l4( fileID, quantID, idx, xfer,  &
       &                                                     datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    logical(kind=4), dimension(:,:,:), intent(in) :: qdat




    logical(kind=4), dimension(:), intent(in), optional :: qmix
    
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_quadquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
    
    
    WriteQuadQuantChunk_3D_l4 = err
  end function WriteQuadQuantChunk_3D_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadQuant_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadQuant_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadQuant_Chunk_f( fileID, quantID, idx, xfer,
  ! <DATA>                         datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: nghosts
  ! <DATA>   integer(kind=TIO_IPK),   intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: ni, nj, nk
  ! <DATA>   [CHOICE], [DIMENSION],   intent(out), optional :: qdat
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qmix
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_meta(    fileID, quantID, idx, xfer,     &
       &                                       datatype, nghosts, ndims, ni, nj, nk, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 0
    call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
         &                           ndims, ni, nj, nk, idum, lnmx, idum, n, err     )
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    
    
    ReadQuadQuantChunk_meta = err
  end function ReadQuadQuantChunk_meta
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_1D_i4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=4), dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_1D_i4 = err
  end function ReadQuadQuantChunk_1D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_1D_i8(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=8), dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_1D_i8 = err
  end function ReadQuadQuantChunk_1D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_1D_r4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4),    dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_1D_r4 = err
  end function ReadQuadQuantChunk_1D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_1D_r8(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8),    dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_1D_r8 = err
  end function ReadQuadQuantChunk_1D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_1D_l4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    logical(kind=4), dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_1D_l4 = err
  end function ReadQuadQuantChunk_1D_l4
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_2D_i4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=4), dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_2D_i4 = err
  end function ReadQuadQuantChunk_2D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_2D_i8(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=8), dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_2D_i8 = err
  end function ReadQuadQuantChunk_2D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_2D_r4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4),    dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_2D_r4 = err
  end function ReadQuadQuantChunk_2D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_2D_r8(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8),    dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_2D_r8 = err
  end function ReadQuadQuantChunk_2D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_2D_l4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    logical(kind=4), dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_2D_l4 = err
  end function ReadQuadQuantChunk_2D_l4
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_3D_i4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=4), dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_3D_i4 = err
  end function ReadQuadQuantChunk_3D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_3D_i8(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=8), dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_3D_i8 = err
  end function ReadQuadQuantChunk_3D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_3D_r4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4),    dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_3D_r4 = err
  end function ReadQuadQuantChunk_3D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_3D_r8(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8),    dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_3D_r8 = err
  end function ReadQuadQuantChunk_3D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantChunk_3D_l4(   fileID, quantID, idx, xfer,     &
       &                           datatype, nghosts, ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    logical(kind=4), dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, qmix, n, err     )
      
    else
      
      n = 1
      call tio_read_quadquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
           &                           ndims, ni, nj, nk, qdat, lnmx, idum, n, err     )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantChunk_3D_l4 = err
  end function ReadQuadQuantChunk_3D_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadQuant_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadQuant_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadQuant_All_f( fileID, quantID, datatype,
  ! <DATA>                                                           ndims, ni, nj, nk,
  ! <DATA>                                                           qdat, nmixcomp, qmix )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_IPK),   intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: ni, nj, nk
  ! <DATA>   [CHOICE], [DIMENSION],   intent(out), optional :: qdat
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qmix
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadQuadQuantAll_meta(  fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    n = 0
    
    call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                         idum, lnmx, idum, n, err )
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    
    
    ReadQuadQuantAll_meta = err
  end function ReadQuadQuantAll_meta
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_1D_i4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=4), dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_1D_i4 = err
  end function ReadQuadQuantAll_1D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_1D_i8( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=8), dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_1D_i8 = err
  end function ReadQuadQuantAll_1D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_1D_r4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4),    dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_1D_r4 = err
  end function ReadQuadQuantAll_1D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_1D_r8( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8),    dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_1D_r8 = err
  end function ReadQuadQuantAll_1D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_1D_l4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    logical(kind=4), dimension(:),     intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_1D_l4 = err
  end function ReadQuadQuantAll_1D_l4
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_2D_i4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=4), dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_2D_i4 = err
  end function ReadQuadQuantAll_2D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_2D_i8( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=8), dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_2D_i8 = err
  end function ReadQuadQuantAll_2D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_2D_r4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4),    dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_2D_r4 = err
  end function ReadQuadQuantAll_2D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_2D_r8( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8),    dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_2D_r8 = err
  end function ReadQuadQuantAll_2D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_2D_l4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    logical(kind=4), dimension(:,:),   intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_2D_l4 = err
  end function ReadQuadQuantAll_2D_l4
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_3D_i4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=4), dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_3D_i4 = err
  end function ReadQuadQuantAll_3D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_3D_i8( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=8), dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_3D_i8 = err
  end function ReadQuadQuantAll_3D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_3D_r4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4),    dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_3D_r4 = err
  end function ReadQuadQuantAll_3D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_3D_r8( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8),    dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_3D_r8 = err
  end function ReadQuadQuantAll_3D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantAll_3D_l4( fileID, quantID, datatype,  &
       &                                                  ndims, ni, nj, nk, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    logical(kind=4), dimension(:,:,:), intent(out) :: qdat
    
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_all_c( fileID, quantID, datatype, ndims, ni, nj, nk,  &
         &                           qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantAll_3D_l4 = err
  end function ReadQuadQuantAll_3D_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadQuant_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadQuant_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadQuant_Section_f( fileID, quantID, sectionID,
  ! <DATA>                                                               xfer, datatype,
  ! <DATA>                                                               ndims, ni, nj, nk,
  ! <DATA>                                                               qdat, nmixcomp, qmix )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: xfer, datatype
  ! <DATA>   integer(kind=TIO_IPK),   intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: ni, nj, nk
  ! <DATA>   [CHOICE], [DIMENSION],   intent(out)           :: qdat
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qmix
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_meta(  fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, nmixcomp )
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    n = 0
    
    call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                             idum, lnmx, idum, n, err )
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    
    
    ReadQuadQuantSection_meta = err
  end function ReadQuadQuantSection_meta
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_1D_i4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    integer(kind=4), dimension(:),     intent(out)           :: qdat
    integer(kind=4), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_1D_i4 = err
  end function ReadQuadQuantSection_1D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_1D_i8( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    integer(kind=8), dimension(:),     intent(out)           :: qdat
    integer(kind=8), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_1D_i8 = err
  end function ReadQuadQuantSection_1D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_1D_r4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    real(kind=4), dimension(:),        intent(out)           :: qdat
    real(kind=4), dimension(:),        intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_1D_r4 = err
  end function ReadQuadQuantSection_1D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_1D_r8( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    real(kind=8), dimension(:),        intent(out)           :: qdat
    real(kind=8), dimension(:),        intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_1D_r8 = err
  end function ReadQuadQuantSection_1D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_1D_l4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    logical(kind=4), dimension(:),     intent(out)           :: qdat
    logical(kind=4), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_1D_l4 = err
  end function ReadQuadQuantSection_1D_l4
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_2D_i4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    integer(kind=4), dimension(:,:),   intent(out)           :: qdat
    integer(kind=4), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_2D_i4 = err
  end function ReadQuadQuantSection_2D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_2D_i8( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    integer(kind=8), dimension(:,:),   intent(out)           :: qdat
    integer(kind=8), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_2D_i8 = err
  end function ReadQuadQuantSection_2D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_2D_r4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    real(kind=4), dimension(:,:),      intent(out)           :: qdat
    real(kind=4), dimension(:),        intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_2D_r4 = err
  end function ReadQuadQuantSection_2D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_2D_r8( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    real(kind=8), dimension(:,:),      intent(out)           :: qdat
    real(kind=8), dimension(:),        intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_2D_r8 = err
  end function ReadQuadQuantSection_2D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_2D_l4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    logical(kind=4), dimension(:,:),   intent(out)           :: qdat
    logical(kind=4), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_2D_l4 = err
  end function ReadQuadQuantSection_2D_l4
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_3D_i4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    integer(kind=4), dimension(:,:,:), intent(out)           :: qdat
    integer(kind=4), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_3D_i4 = err
  end function ReadQuadQuantSection_3D_i4
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_3D_i8( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    integer(kind=8), dimension(:,:,:), intent(out)           :: qdat
    integer(kind=8), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_3D_i8 = err
  end function ReadQuadQuantSection_3D_i8
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_3D_r4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    real(kind=4), dimension(:,:,:),    intent(out)           :: qdat
    real(kind=4), dimension(:),        intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_3D_r4 = err
  end function ReadQuadQuantSection_3D_r4
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_3D_r8( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    real(kind=8), dimension(:,:,:),    intent(out)           :: qdat
    real(kind=8), dimension(:),        intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_3D_r8 = err
  end function ReadQuadQuantSection_3D_r8
  integer(kind=TIO_ERRK) function ReadQuadQuantSection_3D_l4( fileID, quantID, sectionID, xfer, datatype,  &
       &                                                      ndims, ni, nj, nk, qdat, nmixcomp, qmix )
    logical(kind=4), dimension(:,:,:), intent(out)           :: qdat
    logical(kind=4), dimension(:),     intent(out), optional :: qmix
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    integer(kind=TIO_SIZEK), optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err


    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_quadquant_section_c( fileID, quantID, sectionID, xfer, datatype, ndims, ni, nj, nk,  &
         &                               qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadQuadQuantSection_3D_l4 = err
  end function ReadQuadQuantSection_3D_l4
  
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **          UNSTRQUANTS           ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_UnstrQuant_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_UnstrQuant_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_UnstrQuant_Chunk_f( fileID, quantID, idx, xfer,
  ! <DATA>                                                               datatype, qdat, qmix )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)           :: quantID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)           :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)           :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)           :: datatype
  ! <DATA>   [CHOICE], dimension(:),  intent(in)           :: qdat
  ! <DATA>   [CHOICE], dimension(:),  intent(in), optional :: qmix
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function WriteUnstrQuantChunk_i4( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=4), dimension(:), intent(in)           :: qdat
    integer(kind=4), dimension(:), intent(in), optional :: qmix
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
        
    WriteUnstrQuantChunk_i4 = err
  end function WriteUnstrQuantChunk_i4
  integer(kind=TIO_ERRK) function WriteUnstrQuantChunk_i8( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=8), dimension(:), intent(in)           :: qdat
    integer(kind=8), dimension(:), intent(in), optional :: qmix
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
        
    WriteUnstrQuantChunk_i8 = err
  end function WriteUnstrQuantChunk_i8
  integer(kind=TIO_ERRK) function WriteUnstrQuantChunk_r4( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=4),    dimension(:), intent(in)           :: qdat
    real(kind=4),    dimension(:), intent(in), optional :: qmix
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
        
    WriteUnstrQuantChunk_r4 = err
  end function WriteUnstrQuantChunk_r4
  integer(kind=TIO_ERRK) function WriteUnstrQuantChunk_r8( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=8),    dimension(:), intent(in)           :: qdat
    real(kind=8),    dimension(:), intent(in), optional :: qmix
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
        
    WriteUnstrQuantChunk_r8 = err
  end function WriteUnstrQuantChunk_r8
  integer(kind=TIO_ERRK) function WriteUnstrQuantChunk_l4( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat, qmix         )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    logical(kind=4), dimension(:), intent(in)           :: qdat
    logical(kind=4), dimension(:), intent(in), optional :: qmix
    
    integer :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    if (present(qmix)) then
      n = 2
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, qmix, n, err)
    else
      n = 1
      call tio_write_unstrquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, idum, n, err)
    end if
        
    WriteUnstrQuantChunk_l4 = err
  end function WriteUnstrQuantChunk_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrQuant_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrQuant_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrQuant_Chunk_f( fileID, quantID, idx, xfer,
  ! <DATA>                                                              datatype, nghosts, nn, qdat,
  ! <DATA>                                                              nmixcomp, qmix )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: nghosts
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: nn
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qdat
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qmix
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadUnstrQuantChunk_meta( fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nn, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    n = 0
    call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
         &                            idum, lnmx, idum, n, err )
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    
    
    ReadUnstrQuantChunk_meta = err
  end function ReadUnstrQuantChunk_meta
  integer(kind=TIO_ERRK) function ReadUnstrQuantChunk_i4(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nn, qdat,    &
       &                                                    nmixcomp, qmix                  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    integer(kind=4), dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadUnstrQuantChunk_i4 = err
  end function ReadUnstrQuantChunk_i4
  integer(kind=TIO_ERRK) function ReadUnstrQuantChunk_i8(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nn, qdat,    &
       &                                                    nmixcomp, qmix                  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    integer(kind=8), dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadUnstrQuantChunk_i8 = err
  end function ReadUnstrQuantChunk_i8
  integer(kind=TIO_ERRK) function ReadUnstrQuantChunk_r4(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nn, qdat,    &
       &                                                    nmixcomp, qmix                  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    real(kind=4),    dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadUnstrQuantChunk_r4 = err
  end function ReadUnstrQuantChunk_r4
  integer(kind=TIO_ERRK) function ReadUnstrQuantChunk_r8(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nn, qdat,    &
       &                                                    nmixcomp, qmix                  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    real(kind=8),    dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadUnstrQuantChunk_r8 = err
  end function ReadUnstrQuantChunk_r8
  integer(kind=TIO_ERRK) function ReadUnstrQuantChunk_l4(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nn, qdat,    &
       &                                                    nmixcomp, qmix                  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    logical(kind=4), dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts, nn,  &
           &                            qdat, lnmx, idum, n, err )
      
    end if
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    
    ReadUnstrQuantChunk_l4 = err
  end function ReadUnstrQuantChunk_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrQuant_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrQuant_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrQuant_All_f( fileID, quantID, datatype, nn,
  ! <DATA>                                                            qdat, nmixcomp, qmix )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: nn
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qdat
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qmix
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadUnstrQuantAll_meta( fileID, quantID, datatype, nn, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    


    n = 0
    
    call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, idum, lnmx, idum, n, err )
    
    
    
    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantAll_meta = err
  end function ReadUnstrQuantAll_meta
  integer(kind=TIO_ERRK) function ReadUnstrQuantAll_i4(   fileID, quantID, datatype, nn, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    integer(kind=4), dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, idum, n, err )
      
    end if


    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantAll_i4 = err
  end function ReadUnstrQuantAll_i4
  integer(kind=TIO_ERRK) function ReadUnstrQuantAll_i8(   fileID, quantID, datatype, nn, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    integer(kind=8), dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, idum, n, err )
      
    end if


    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantAll_i8 = err
  end function ReadUnstrQuantAll_i8
  integer(kind=TIO_ERRK) function ReadUnstrQuantAll_r4(   fileID, quantID, datatype, nn, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    real(kind=4),    dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, idum, n, err )
      
    end if


    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantAll_r4 = err
  end function ReadUnstrQuantAll_r4
  integer(kind=TIO_ERRK) function ReadUnstrQuantAll_r8(   fileID, quantID, datatype, nn, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    real(kind=8),    dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, idum, n, err )
      
    end if


    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantAll_r8 = err
  end function ReadUnstrQuantAll_r8
  integer(kind=TIO_ERRK) function ReadUnstrQuantAll_l4(   fileID, quantID, datatype, nn, qdat,    &
       &                                                  nmixcomp, qmix )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nn
    
    logical(kind=4), dimension(:),           intent(out) :: qdat
    integer(kind=TIO_SIZEK),       optional, intent(out) :: nmixcomp
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err
    
    
    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_all_c( fileID, quantID, datatype, nn, qdat, lnmx, idum, n, err )
      
    end if


    
    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantAll_l4 = err
  end function ReadUnstrQuantAll_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrQuant_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrQuant_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrQuant_Section_f( fileID, quantID, sectionID,
  ! <DATA>                                                                xfer, datatype, nn, qdat,
  ! <DATA>                                                                nmixcomp, qmix )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: nn
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qdat
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qmix
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadUnstrQuantSection_meta( fileID, quantID, sectionID,  &
       &                                                      xfer, datatype, nn, nmixcomp )
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)            :: xfer
    integer(kind=TIO_IPK),   intent(in)            :: datatype
    integer(kind=TIO_SIZEK), intent(out)           :: nn
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp




    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err






    n = 0
    
    call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
         &                              nn, idum, lnmx, idum, n, err )
    
    
    


    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantSection_meta = err
  end function ReadUnstrQuantSection_meta
  integer(kind=TIO_ERRK) function ReadUnstrQuantSection_i4( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nn, qdat,    &
       &                                                    nmixcomp, qmix )
    integer(kind=4) :: i4
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)            :: xfer
    integer(kind=TIO_IPK),   intent(in)            :: datatype
    integer(kind=TIO_SIZEK), intent(out)           :: nn
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp




    integer(kind=4), dimension(:),           intent(out) :: qdat
    integer(kind=4), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err




    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, idum, n, err )
      
    end if
    


    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantSection_i4 = err
  end function ReadUnstrQuantSection_i4
  integer(kind=TIO_ERRK) function ReadUnstrQuantSection_i8( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nn, qdat,    &
       &                                                    nmixcomp, qmix )
    integer(kind=8) :: i8
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)            :: xfer
    integer(kind=TIO_IPK),   intent(in)            :: datatype
    integer(kind=TIO_SIZEK), intent(out)           :: nn
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp




    integer(kind=8), dimension(:),           intent(out) :: qdat
    integer(kind=8), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err




    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, idum, n, err )
      
    end if
    


    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantSection_i8 = err
  end function ReadUnstrQuantSection_i8
  integer(kind=TIO_ERRK) function ReadUnstrQuantSection_r4( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nn, qdat,    &
       &                                                    nmixcomp, qmix )
    real(kind=4)    :: r4
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)            :: xfer
    integer(kind=TIO_IPK),   intent(in)            :: datatype
    integer(kind=TIO_SIZEK), intent(out)           :: nn
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp




    real(kind=4),    dimension(:),           intent(out) :: qdat
    real(kind=4),    dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err




    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, idum, n, err )
      
    end if
    


    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantSection_r4 = err
  end function ReadUnstrQuantSection_r4
  integer(kind=TIO_ERRK) function ReadUnstrQuantSection_r8( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nn, qdat,    &
       &                                                    nmixcomp, qmix )
    real(kind=8)    :: r8
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)            :: xfer
    integer(kind=TIO_IPK),   intent(in)            :: datatype
    integer(kind=TIO_SIZEK), intent(out)           :: nn
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp




    real(kind=8),    dimension(:),           intent(out) :: qdat
    real(kind=8),    dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err




    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, idum, n, err )
      
    end if
    


    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantSection_r8 = err
  end function ReadUnstrQuantSection_r8
  integer(kind=TIO_ERRK) function ReadUnstrQuantSection_l4( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nn, qdat,    &
       &                                                    nmixcomp, qmix )
    logical(kind=4) :: l4
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)            :: xfer
    integer(kind=TIO_IPK),   intent(in)            :: datatype
    integer(kind=TIO_SIZEK), intent(out)           :: nn
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcomp




    logical(kind=4), dimension(:),           intent(out) :: qdat
    logical(kind=4), dimension(:), optional, intent(out) :: qmix
    
    integer                 :: n, idum
    integer(kind=TIO_SIZEK) :: lnmx
    integer(kind=TIO_ERRK)  :: err




    
    
    
    if (present(qmix)) then
      
      n = 2
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, qmix, n, err )
      
    else
      
      n = 1
      call tio_read_unstrquant_section_c( fileID, quantID, sectionID, xfer, datatype,   &
           &                              nn, qdat, lnmx, idum, n, err )
      
    end if
    


    if (present(nmixcomp)) nmixcomp = lnmx
    
    
    ReadUnstrQuantSection_l4 = err
  end function ReadUnstrQuantSection_l4
  
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **          POINTQUANTS           ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_PointQuant_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_PointQuant_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_PointQuant_Chunk_f( fileID, quantID, idx, xfer,
  ! <DATA>                                                               datatype, qdat )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: quantID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in) :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in) :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in) :: datatype
  ! <DATA>   [CHOICE], dimension(:),  intent(in) :: qdat
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function WritePointQuantChunk_i4( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat               )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=4), dimension(:), intent(in) :: qdat
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_write_pointquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, err)
    
    
    WritePointQuantChunk_i4 = err
  end function WritePointQuantChunk_i4
  integer(kind=TIO_ERRK) function WritePointQuantChunk_i8( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat               )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    integer(kind=8), dimension(:), intent(in) :: qdat
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_write_pointquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, err)
    
    
    WritePointQuantChunk_i8 = err
  end function WritePointQuantChunk_i8
  integer(kind=TIO_ERRK) function WritePointQuantChunk_r4( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat               )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=4),    dimension(:), intent(in) :: qdat
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_write_pointquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, err)
    
    
    WritePointQuantChunk_r4 = err
  end function WritePointQuantChunk_r4
  integer(kind=TIO_ERRK) function WritePointQuantChunk_r8( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat               )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    real(kind=8),    dimension(:), intent(in) :: qdat
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_write_pointquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, err)
    
    
    WritePointQuantChunk_r8 = err
  end function WritePointQuantChunk_r8
  integer(kind=TIO_ERRK) function WritePointQuantChunk_l4( fileID, quantID, idx, xfer,  &
       &                                                   datatype, qdat               )
    
    implicit none 
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: quantID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: xfer
    integer(kind=TIO_IPK),   intent(in) :: datatype
    
    logical(kind=4), dimension(:), intent(in) :: qdat
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_write_pointquant_chunk_c(fileID, quantID, idx, xfer, datatype, qdat, err)
    
    
    WritePointQuantChunk_l4 = err
  end function WritePointQuantChunk_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointQuant_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointQuant_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointQuant_Chunk_f( fileID, quantID, idx, xfer,
  ! <DATA>                                                              datatype, nghosts, nnodes,
  ! <DATA>                                                              qdat )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: nghosts
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: nnodes
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qdat
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadPointQuantChunk_meta( fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nnodes       )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    
    n = 0
    call tio_read_pointquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
         &                            nnodes, idum, n, err )
    
    
    
    
    ReadPointQuantChunk_meta = err
  end function ReadPointQuantChunk_meta
  integer(kind=TIO_ERRK) function ReadPointQuantChunk_i4(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    integer(kind=4), dimension(:), intent(out) :: qdat
    
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    n = 1
    call tio_read_pointquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
         &                            nnodes, qdat, n, err )
    
    
    
    ReadPointQuantChunk_i4 = err
  end function ReadPointQuantChunk_i4
  integer(kind=TIO_ERRK) function ReadPointQuantChunk_i8(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    integer(kind=8), dimension(:), intent(out) :: qdat
    
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    n = 1
    call tio_read_pointquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
         &                            nnodes, qdat, n, err )
    
    
    
    ReadPointQuantChunk_i8 = err
  end function ReadPointQuantChunk_i8
  integer(kind=TIO_ERRK) function ReadPointQuantChunk_r4(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    real(kind=4),    dimension(:), intent(out) :: qdat
    
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    n = 1
    call tio_read_pointquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
         &                            nnodes, qdat, n, err )
    
    
    
    ReadPointQuantChunk_r4 = err
  end function ReadPointQuantChunk_r4
  integer(kind=TIO_ERRK) function ReadPointQuantChunk_r8(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    real(kind=8),    dimension(:), intent(out) :: qdat
    
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    n = 1
    call tio_read_pointquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
         &                            nnodes, qdat, n, err )
    
    
    
    ReadPointQuantChunk_r8 = err
  end function ReadPointQuantChunk_r8
  integer(kind=TIO_ERRK) function ReadPointQuantChunk_l4(   fileID, quantID, idx, xfer,     &
       &                                                    datatype, nghosts, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    logical(kind=4), dimension(:), intent(out) :: qdat
    
    
    integer                :: n, idum
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    n = 1
    call tio_read_pointquant_chunk_c( fileID, quantID, idx, xfer, datatype, nghosts,  &
         &                            nnodes, qdat, n, err )
    
    
    
    ReadPointQuantChunk_l4 = err
  end function ReadPointQuantChunk_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointQuant_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointQuant_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointQuant_All_f( fileID, quantID, datatype,
  ! <DATA>                                                            nnodes, qdat )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: nnodes
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qdat
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadPointQuantAll_meta( fileID, quantID, datatype, nnodes )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    integer(kind=TIO_ERRK)  :: err
    integer :: n, idum
    
    
    
    n = 0
    call tio_read_pointquant_all_c( fileID, quantID, datatype, nnodes, idum, n, err )


    
    
    
    ReadPointQuantAll_meta = err
  end function ReadPointQuantAll_meta
  integer(kind=TIO_ERRK) function ReadPointQuantAll_i4(   fileID, quantID, datatype, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    integer(kind=4), dimension(:), intent(out) :: qdat
    
    integer(kind=TIO_ERRK)  :: err
    integer :: n, idum
    
    
    
    
    n = 1
    call tio_read_pointquant_all_c( fileID, quantID, datatype, nnodes, qdat, n, err )
    
    
    
    ReadPointQuantAll_i4 = err
  end function ReadPointQuantAll_i4
  integer(kind=TIO_ERRK) function ReadPointQuantAll_i8(   fileID, quantID, datatype, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    integer(kind=8), dimension(:), intent(out) :: qdat
    
    integer(kind=TIO_ERRK)  :: err
    integer :: n, idum
    
    
    
    
    n = 1
    call tio_read_pointquant_all_c( fileID, quantID, datatype, nnodes, qdat, n, err )
    
    
    
    ReadPointQuantAll_i8 = err
  end function ReadPointQuantAll_i8
  integer(kind=TIO_ERRK) function ReadPointQuantAll_r4(   fileID, quantID, datatype, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    real(kind=4),    dimension(:), intent(out) :: qdat
    
    integer(kind=TIO_ERRK)  :: err
    integer :: n, idum
    
    
    
    
    n = 1
    call tio_read_pointquant_all_c( fileID, quantID, datatype, nnodes, qdat, n, err )
    
    
    
    ReadPointQuantAll_r4 = err
  end function ReadPointQuantAll_r4
  integer(kind=TIO_ERRK) function ReadPointQuantAll_r8(   fileID, quantID, datatype, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    real(kind=8),    dimension(:), intent(out) :: qdat
    
    integer(kind=TIO_ERRK)  :: err
    integer :: n, idum
    
    
    
    
    n = 1
    call tio_read_pointquant_all_c( fileID, quantID, datatype, nnodes, qdat, n, err )
    
    
    
    ReadPointQuantAll_r8 = err
  end function ReadPointQuantAll_r8
  integer(kind=TIO_ERRK) function ReadPointQuantAll_l4(   fileID, quantID, datatype, nnodes, qdat )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    logical(kind=4), dimension(:), intent(out) :: qdat
    
    integer(kind=TIO_ERRK)  :: err
    integer :: n, idum
    
    
    
    
    n = 1
    call tio_read_pointquant_all_c( fileID, quantID, datatype, nnodes, qdat, n, err )
    
    
    
    ReadPointQuantAll_l4 = err
  end function ReadPointQuantAll_l4
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointQuant_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointQuant_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointQuant_Section_f( fileID, quantID, sectionID,
  ! <DATA>                                                                xfer, datatype, nnodes, qdat )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: quantID, sectionID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: nnodes
  ! <DATA>   [CHOICE], dimension(:),  intent(out), optional :: qdat
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadPointQuantSection_meta( fileID, quantID, sectionID,  &
       &                                                      xfer, datatype, nnodes )
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err






    n = 0
    
    call tio_read_pointquant_section_c( fileID, quantID, sectionID, xfer, datatype,  &
         &                              nnodes, idum, n, err )
    
    
    
    
    
    ReadPointQuantSection_meta = err
  end function ReadPointQuantSection_meta
  integer(kind=TIO_ERRK) function ReadPointQuantSection_i4( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nnodes, qdat )
    integer(kind=4) :: i4
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    integer(kind=4), dimension(:), intent(out) :: qdat
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err




    
    
    
    n = 1
    call tio_read_pointquant_section_c( fileID, quantID, sectionID, xfer, datatype,  &
         &                              nnodes, qdat, n, err )
    
    
    
    ReadPointQuantSection_i4 = err
  end function ReadPointQuantSection_i4
  integer(kind=TIO_ERRK) function ReadPointQuantSection_i8( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nnodes, qdat )
    integer(kind=8) :: i8
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    integer(kind=8), dimension(:), intent(out) :: qdat
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err




    
    
    
    n = 1
    call tio_read_pointquant_section_c( fileID, quantID, sectionID, xfer, datatype,  &
         &                              nnodes, qdat, n, err )
    
    
    
    ReadPointQuantSection_i8 = err
  end function ReadPointQuantSection_i8
  integer(kind=TIO_ERRK) function ReadPointQuantSection_r4( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nnodes, qdat )
    real(kind=4)    :: r4
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    real(kind=4),    dimension(:), intent(out) :: qdat
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err




    
    
    
    n = 1
    call tio_read_pointquant_section_c( fileID, quantID, sectionID, xfer, datatype,  &
         &                              nnodes, qdat, n, err )
    
    
    
    ReadPointQuantSection_r4 = err
  end function ReadPointQuantSection_r4
  integer(kind=TIO_ERRK) function ReadPointQuantSection_r8( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nnodes, qdat )
    real(kind=8)    :: r8
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    real(kind=8),    dimension(:), intent(out) :: qdat
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err




    
    
    
    n = 1
    call tio_read_pointquant_section_c( fileID, quantID, sectionID, xfer, datatype,  &
         &                              nnodes, qdat, n, err )
    
    
    
    ReadPointQuantSection_r8 = err
  end function ReadPointQuantSection_r8
  integer(kind=TIO_ERRK) function ReadPointQuantSection_l4( fileID, quantID, sectionID,  &
       &                                                    xfer, datatype, nnodes, qdat )
    logical(kind=4) :: l4
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: quantID, sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    logical(kind=4), dimension(:), intent(out) :: qdat
    
    integer                 :: n, idum
    integer(kind=TIO_ERRK)  :: err




    
    
    
    n = 1
    call tio_read_pointquant_section_c( fileID, quantID, sectionID, xfer, datatype,  &
         &                              nnodes, qdat, n, err )
    
    
    
    ReadPointQuantSection_l4 = err
  end function ReadPointQuantSection_l4
  
  
  
  
  ! ********************************************************************************************** !
  
  
end module TIO_Quant_mod






!
!  EOF

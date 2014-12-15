!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!
!-*-Mode: F90;-*-  emacs mode specifier

module TIO_Variable_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  use TIO_Utils_mod
  
  
  implicit none
  
  
  private
  
  public :: TIO_List_Variables_f
  public :: TIO_Create_Variable_f, TIO_Open_Variable_f, TIO_Open_Variable_idx_f,  &
       &    TIO_Close_Variable_f, TIO_Get_Variable_Info_f
  
  public :: TIO_Write_Variable_f, TIO_Read_Variable_f, TIO_Read_Variable_Section_f
  
  
  interface TIO_Write_Variable_f
    module procedure WriteVariable_0D_r4
    module procedure WriteVariable_0D_r8
    module procedure WriteVariable_0D_i4
    module procedure WriteVariable_0D_i8
    module procedure WriteVariable_0D_l4
    
    module procedure WriteVariable_1D_r4
    module procedure WriteVariable_1D_r8
    module procedure WriteVariable_1D_i4
    module procedure WriteVariable_1D_i8
    module procedure WriteVariable_1D_l4
    
    module procedure WriteVariable_2D_r4
    module procedure WriteVariable_2D_r8
    module procedure WriteVariable_2D_i4
    module procedure WriteVariable_2D_i8
    module procedure WriteVariable_2D_l4
    
    module procedure WriteVariable_3D_r4
    module procedure WriteVariable_3D_r8
    module procedure WriteVariable_3D_i4
    module procedure WriteVariable_3D_i8
    module procedure WriteVariable_3D_l4

    module procedure WriteVariable_0D_st
    module procedure WriteVariable_1D_st
    module procedure WriteVariable_2D_st
    module procedure WriteVariable_3D_st
  end interface
  
  interface TIO_Read_Variable_f
    module procedure ReadVariable_0D_r4
    module procedure ReadVariable_0D_r8
    module procedure ReadVariable_0D_i4
    module procedure ReadVariable_0D_i8
    module procedure ReadVariable_0D_l4
    
    module procedure ReadVariable_1D_r4
    module procedure ReadVariable_1D_r8
    module procedure ReadVariable_1D_i4
    module procedure ReadVariable_1D_i8
    module procedure ReadVariable_1D_l4
    
    module procedure ReadVariable_2D_r4
    module procedure ReadVariable_2D_r8
    module procedure ReadVariable_2D_i4
    module procedure ReadVariable_2D_i8
    module procedure ReadVariable_2D_l4
    
    module procedure ReadVariable_3D_r4
    module procedure ReadVariable_3D_r8
    module procedure ReadVariable_3D_i4
    module procedure ReadVariable_3D_i8
    module procedure ReadVariable_3D_l4
    
    module procedure ReadVariable_0D_st
    module procedure ReadVariable_1D_st
    module procedure ReadVariable_2D_st
    module procedure ReadVariable_3D_st
  end interface
  
  interface TIO_Read_Variable_Section_f
    module procedure ReadVariableSection_1D_r4
    module procedure ReadVariableSection_1D_r8
    module procedure ReadVariableSection_1D_i4
    module procedure ReadVariableSection_1D_i8
    module procedure ReadVariableSection_1D_l4
    
    module procedure ReadVariableSection_2D_r4
    module procedure ReadVariableSection_2D_r8
    module procedure ReadVariableSection_2D_i4
    module procedure ReadVariableSection_2D_i8
    module procedure ReadVariableSection_2D_l4
    
    module procedure ReadVariableSection_3D_r4
    module procedure ReadVariableSection_3D_r8
    module procedure ReadVariableSection_3D_i4
    module procedure ReadVariableSection_3D_i8
    module procedure ReadVariableSection_3D_l4
    
    module procedure ReadVariableSection_1D_st
    module procedure ReadVariableSection_2D_st
    module procedure ReadVariableSection_3D_st
  end interface
  
  
contains
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_List_Variables_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_List_Variables
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_List_Variables_f( fileID, objectID, nobj, names )
  ! <DATA>   
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: objectID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
  ! <DATA>   character(len=*), dimension(:), intent(out), optional :: names
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_List_Variables_f( fileID, objectID, nobj, names )
    
    implicit none
    
    integer(kind=TIO_FILEK),        intent(in)            :: fileID
    integer(kind=TIO_OBJK),         intent(in)            :: objectID
    integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
    character(len=*), dimension(:), intent(out), optional :: names
    
    
    character :: cdummy
    
    integer(kind=TIO_SIZEK) :: ln
    integer(kind=TIO_ERRK)  :: err
    
    
    ln = -1
    call tio_list_variables_c(fileID, objectID, ln, cdummy, err)
    TIO_List_Variables_f = err
    if (TIO_List_Variables_f /= TIO_SUCCESS_F) return
    
    if (present(nobj)) nobj = ln
    
    if (present(names)) then
      TIO_List_Variables_f = listnames("TIO_List_Variables", LISTCLASS_VARIABLE,  &
           &                           fileID, objectID, ln, names)
    end if
    
  end function TIO_List_Variables_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Create_Variable_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Create_Variable
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Create_Variable_f( fileID, objectID, name, varID,
  ! <DATA>                                                        datatype, ndims, dims, units )
  ! <DATA>   
  ! <DATA>   integer(kind=TIO_FILEK),               intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(in)  :: objectID
  ! <DATA>   character(len=*),                      intent(in)  :: name
  ! <DATA>   integer(kind=TIO_OBJK),                intent(out) :: varID
  ! <DATA>   integer(kind=TIO_IPK),                 intent(in)  :: datatype, ndims
  ! <DATA>   integer(kind=TIO_SIZEK), dimension(:), intent(in)  :: dims
  ! <DATA>   character(len=*),                      intent(in)  :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Create_Variable_f( fileID, objectID, name, varID,   &
       &                                                 datatype, ndims, dims, units )
    
    implicit none
    
    integer(kind=TIO_FILEK),               intent(in)  :: fileID
    integer(kind=TIO_OBJK),                intent(in)  :: objectID
    character(len=*),                      intent(in)  :: name
    integer(kind=TIO_OBJK),                intent(out) :: varID
    integer(kind=TIO_IPK),                 intent(in)  :: datatype, ndims
    integer(kind=TIO_SIZEK), dimension(:), intent(in)  :: dims
    character(len=*),                      intent(in)  :: units
    
    integer :: name_len, units_len
    integer(kind=TIO_ERRK) :: err
    
    
    TIO_Create_Variable_f = setlen("TIO_Create_Variable_f", "name", name, name_len)
    if (TIO_Create_Variable_f /= TIO_SUCCESS_F) return
    
    TIO_Create_Variable_f = setlen("TIO_Create_Variable_f", "units", units, units_len)
    if (TIO_Create_Variable_f /= TIO_SUCCESS_F) return
    
    call tio_create_variable_c( fileID, objectID, name, name_len, varID,   &
         &                      datatype, ndims, dims, units, units_len, err)
    
    TIO_Create_variable_f = err
    
  end function TIO_Create_Variable_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Variable_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Variable
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Variable_f( fileID, objectID, name, varID,
  ! <DATA>                                                      datatype, ndims, dims, units )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),               intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(in)            :: objectID
  ! <DATA>   character(len=*),                      intent(in)            :: name
  ! <DATA>   integer(kind=TIO_OBJK),                intent(out)           :: varID
  ! <DATA>   integer(kind=TIO_IPK),                 intent(out), optional :: datatype, ndims
  ! <DATA>   integer(kind=TIO_SIZEK), dimension(:), intent(out), optional :: dims
  ! <DATA>   character(len=*),                      intent(out), optional :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Variable_f( fileID, objectID, name, varID,   &
       &                                               datatype, ndims, dims, units )
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in)            :: fileID
    integer(kind=TIO_OBJK),                 intent(in)            :: objectID
    character(len=*),                       intent(in)            :: name
    integer(kind=TIO_OBJK),                 intent(out)           :: varID
    integer(kind=TIO_IPK),                  intent(out), optional :: datatype, ndims
    integer(kind=TIO_SIZEK), dimension(1:), intent(out), optional :: dims
    character(len=*),                       intent(out), optional :: units
    
    
    integer  :: name_len, ldatatype, lndims
    integer(kind=TIO_SIZEK), dimension(1:TIO_MAXDIMS_F) :: ldims
    character(len=TIO_STRLEN_F) :: lunits = ""
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    TIO_Open_Variable_f = setlen("TIO_Open_Variable_f", "name", name, name_len)
    if (TIO_Open_Variable_f /= TIO_SUCCESS_F) return
    
    call tio_open_variable_c( fileID, objectID, -1, name, name_len, varID,  &
         &                    ldatatype, lndims, ldims, lunits, len(lunits), err)
    
    TIO_Open_Variable_f = err
    if (TIO_Open_Variable_f /= TIO_SUCCESS_F) return
    
    if (present(datatype)) datatype = ldatatype
    if (present(ndims))    ndims    = lndims
    
    if (present(dims)) then
      if (ubound(dims,dim=1) < lndims) then
        call seterror("TIO_Open_Variable_f", "dims array is not big enough")
        TIO_Open_Variable_f = TIO_ERR_F90_API_F
        err = TIO_Close_Variable_f(fileID, varID)
        return
      end if
      dims(1:lndims) = ldims(1:lndims)
    end if
    
    if (present(units)) then
      TIO_Open_Variable_f = setstr("TIO_Open_Variable_f", "units", lunits, units)
      if (TIO_Open_Variable_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Variable_f(fileID, varID)
        return
      end if
    end if
    
  end function TIO_Open_Variable_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Variable_idx_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Variable_idx
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Variable_idx_f( fileID, objectID, idx, name, varID,
  ! <DATA>                                                          datatype, ndims, dims, units )
  ! <DATA>   
  ! <DATA>   integer(kind=TIO_FILEK),               intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(in)            :: objectID
  ! <DATA>   integer(kind=TIO_SIZEK),               intent(in)            :: idx
  ! <DATA>   character(len=*),                      intent(out)           :: name
  ! <DATA>   integer(kind=TIO_OBJK),                intent(out)           :: varID
  ! <DATA>   integer(kind=TIO_IPK),                 intent(out), optional :: datatype, ndims
  ! <DATA>   integer(kind=TIO_SIZEK), dimension(:), intent(out), optional :: dims
  ! <DATA>   character(len=*),                      intent(out), optional :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Variable_idx_f( fileID, objectID, idx, name, varID,  &
       &                                                   datatype, ndims, dims, units )
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in)            :: fileID
    integer(kind=TIO_OBJK),                 intent(in)            :: objectID
    integer(kind=TIO_SIZEK),                intent(in)            :: idx
    character(len=*),                       intent(out)           :: name
    integer(kind=TIO_OBJK),                 intent(out)           :: varID
    integer(kind=TIO_IPK),                  intent(out), optional :: datatype, ndims
    integer(kind=TIO_SIZEK), dimension(1:), intent(out), optional :: dims
    character(len=*),                       intent(out), optional :: units
    
    
    integer  :: ldatatype, lndims
    integer(kind=TIO_SIZEK), dimension(1:TIO_MAXDIMS_F) :: ldims
    character(len=TIO_STRLEN_F) :: lname = "", lunits = ""
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_open_variable_c( fileID, objectID, idx, lname, len(lname), varID,  &
         &                    ldatatype, lndims, ldims, lunits, len(lunits), err)
    
    TIO_Open_Variable_idx_f = err
    if (TIO_Open_Variable_idx_f /= TIO_SUCCESS_F) return
    
    TIO_Open_Variable_idx_f = setstr("TIO_Open_Variable_idx_f", "name", lname, name)
    if (TIO_Open_Variable_idx_f /= TIO_SUCCESS_F) then
      err = TIO_Close_Variable_f(fileID, varID)
      return
    end if
    
    if (present(datatype)) datatype = ldatatype
    if (present(ndims))    ndims    = lndims
    
    if (present(dims)) then
      if (ubound(dims,dim=1) < lndims) then
        call seterror("TIO_Open_Variable_idx_f", "dims array is not big enough")
        TIO_Open_Variable_idx_f = TIO_ERR_F90_API_F
        err = TIO_Close_Variable_f(fileID, varID)
        return
      end if
      dims(1:lndims) = ldims(1:lndims)
    end if
    
    if (present(units)) then
      TIO_Open_Variable_idx_f = setstr("TIO_Open_Variable_idx_f", "units", lunits, units)
      if (TIO_Open_Variable_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Variable_f(fileID, varID)
        return
      end if
    end if
    
  end function TIO_Open_Variable_idx_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Variable_Info_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Variable_Info
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Variable_Info_f( fileID, varID, name,
  ! <DATA>                                                          datatype, ndims, dims, units )
  ! <DATA>  
  ! <DATA>   integer(kind=TIO_FILEK),                intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                 intent(in)            :: varID
  ! <DATA>   character(len=*),                       intent(out), optional :: name
  ! <DATA>   integer(kind=TIO_IPK),                  intent(out), optional :: datatype, ndims
  ! <DATA>   integer(kind=TIO_SIZEK), dimension(1:), intent(out), optional :: dims
  ! <DATA>   character(len=*),                       intent(out), optional :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_Variable_Info_f( fileID, varID, name,   &
       &                                                   datatype, ndims, dims, units )
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in)            :: fileID
    integer(kind=TIO_OBJK),                 intent(in)            :: varID
    character(len=*),                       intent(out), optional :: name
    integer(kind=TIO_IPK),                  intent(out), optional :: datatype, ndims
    integer(kind=TIO_SIZEK), dimension(1:), intent(out), optional :: dims
    character(len=*),                       intent(out), optional :: units
    
    
    integer  :: ldatatype, lndims
    integer(kind=TIO_SIZEK), dimension(1:TIO_MAXDIMS_F) :: ldims
    character(len=TIO_STRLEN_F) :: lname = "", lunits = "" 
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_get_variable_info_c( fileID, varID, lname, len(lname), ldatatype, lndims, ldims,  &
         &                        lunits, len(lunits), err)
    
    TIO_Get_Variable_Info_f = err
    if (TIO_Get_Variable_Info_f /= TIO_SUCCESS_F) return
    
    if (present(name)) then
      TIO_Get_Variable_Info_f = setstr("TIO_Get_Variable_Info_f", "name", lname, name)
      if (TIO_Get_Variable_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(datatype)) datatype = ldatatype
    if (present(ndims))    ndims    = lndims
    
    if (present(dims)) then
      if (ubound(dims,dim=1) < lndims) then
        call seterror("TIO_Get_Variable_Info_f", "dims array is not big enough")
        TIO_Get_Variable_Info_f = TIO_ERR_F90_API_F
        return
      end if
      dims(1:lndims) = ldims(1:lndims)
    end if
    
    if (present(units)) then
      TIO_Get_Variable_Info_f = setstr("TIO_Get_Variable_Info_f", "units", lunits, units)
      if (TIO_Get_Variable_Info_f /= TIO_SUCCESS_F) return
    end if
    
  end function TIO_Get_Variable_Info_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Close_Variable_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Close_Variable
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Close_Variable_f( fileID, varID )
  ! <DATA>  
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: varID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Close_Variable_f( fileID, varID )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: varID
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_close_variable_c( fileID, varID, err )
    
    TIO_Close_Variable_f = err
    
  end function TIO_Close_Variable_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_Variable_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_Variable
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_Variable_f( fileID, varID, datatype, vdata )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: varID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in) :: datatype
  ! <DATA>   [CHOICE], [DIMENSION,]   intent(in) :: vdata
  ! </FAPI>
  ! <FDES>
  ! <DATA> <code>[CHOICE]</code> can be <code>real(kind=4)</code>, <code>real(kind=8)</code>,
  ! <DATA> <code>integer(kind=4)</code>, <code>integer(kind=8)</code>, <code>logical(kind=4)</code>
  ! <DATA> or <code>character(len=*)</code>
  ! <DATA> <code>[DIMENSION]</code> is optional so can be scalar (0D), 1D <code>(:)</code>, 
  ! <DATA> 2D <code>(:,:)</code> or 3D <code>(:,:,:)</code> as appropriate.
  ! </FDES>
  !
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
!TPP START 20

!TPP CASE 1
  integer(kind=TIO_ERRK) function WriteVariable_0D_r4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 2
  integer(kind=TIO_ERRK) function WriteVariable_0D_r8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 3
  integer(kind=TIO_ERRK) function WriteVariable_0D_i4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 4
  integer(kind=TIO_ERRK) function WriteVariable_0D_i8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 5
  integer(kind=TIO_ERRK) function WriteVariable_0D_l4( fileID, varID, datatype, vdata )
!TPP ESAC

    
!TPP CASE 6
  integer(kind=TIO_ERRK) function WriteVariable_1D_r4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 7
  integer(kind=TIO_ERRK) function WriteVariable_1D_r8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 8
  integer(kind=TIO_ERRK) function WriteVariable_1D_i4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 9
  integer(kind=TIO_ERRK) function WriteVariable_1D_i8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 10
  integer(kind=TIO_ERRK) function WriteVariable_1D_l4( fileID, varID, datatype, vdata )
!TPP ESAC
    
    
!TPP CASE 11
  integer(kind=TIO_ERRK) function WriteVariable_2D_r4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 12
  integer(kind=TIO_ERRK) function WriteVariable_2D_r8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 13
  integer(kind=TIO_ERRK) function WriteVariable_2D_i4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 14
  integer(kind=TIO_ERRK) function WriteVariable_2D_i8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 15
  integer(kind=TIO_ERRK) function WriteVariable_2D_l4( fileID, varID, datatype, vdata )
!TPP ESAC

    
!TPP CASE 16
  integer(kind=TIO_ERRK) function WriteVariable_3D_r4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 17
  integer(kind=TIO_ERRK) function WriteVariable_3D_r8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 18
  integer(kind=TIO_ERRK) function WriteVariable_3D_i4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 19
  integer(kind=TIO_ERRK) function WriteVariable_3D_i8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 20
  integer(kind=TIO_ERRK) function WriteVariable_3D_l4( fileID, varID, datatype, vdata )
!TPP ESAC

    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: varID
    integer(kind=TIO_IPK),   intent(in) :: datatype

!TPP CASE 1
    real(kind=4),     intent(in) :: vdata
!TPP ESAC
!TPP CASE 2
    real(kind=8),     intent(in) :: vdata
!TPP ESAC
!TPP CASE 3
    integer(kind=4),  intent(in) :: vdata
!TPP ESAC
!TPP CASE 4
    integer(kind=8),  intent(in) :: vdata
!TPP ESAC
!TPP CASE 5
    logical(kind=4),  intent(in) :: vdata
!TPP ESAC
    
!TPP CASE 6
    real(kind=4),     dimension(:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 7
    real(kind=8),     dimension(:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 8
    integer(kind=4),  dimension(:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 9
    integer(kind=8),  dimension(:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 10
    logical(kind=4),  dimension(:), intent(in) :: vdata
!TPP ESAC
    
!TPP CASE 11
    real(kind=4),     dimension(:,:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 12
    real(kind=8),     dimension(:,:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 13
    integer(kind=4),  dimension(:,:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 14
    integer(kind=8),  dimension(:,:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 15
    logical(kind=4),  dimension(:,:), intent(in) :: vdata
!TPP ESAC
    
!TPP CASE 16
    real(kind=4),     dimension(:,:,:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 17
    real(kind=8),     dimension(:,:,:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 18
    integer(kind=4),  dimension(:,:,:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 19
    integer(kind=8),  dimension(:,:,:), intent(in) :: vdata
!TPP ESAC
!TPP CASE 20
    logical(kind=4),  dimension(:,:,:), intent(in) :: vdata
!TPP ESAC
    
    integer(kind=TIO_ERRK) :: err
    integer                :: dummy
    
    
    call tio_write_variable_c( fileID, varID, datatype, vdata, dummy, err )
    
!TPP CASE 20
    WriteVariable_3D_l4 = err
  end function WriteVariable_3D_l4
!TPP ESAC
!TPP CASE 19
    WriteVariable_3D_i8 = err
  end function WriteVariable_3D_i8
!TPP ESAC
!TPP CASE 18
    WriteVariable_3D_i4 = err
  end function WriteVariable_3D_i4
!TPP ESAC
!TPP CASE 17
    WriteVariable_3D_r8 = err
  end function WriteVariable_3D_r8
!TPP ESAC
!TPP CASE 16
    WriteVariable_3D_r4 = err
  end function WriteVariable_3D_r4
!TPP ESAC

!TPP CASE 15
    WriteVariable_2D_l4 = err
  end function WriteVariable_2D_l4
!TPP ESAC
!TPP CASE 14
    WriteVariable_2D_i8 = err
  end function WriteVariable_2D_i8
!TPP ESAC
!TPP CASE 13
    WriteVariable_2D_i4 = err
  end function WriteVariable_2D_i4
!TPP ESAC
!TPP CASE 12
    WriteVariable_2D_r8 = err
  end function WriteVariable_2D_r8
!TPP ESAC
!TPP CASE 11
    WriteVariable_2D_r4 = err
  end function WriteVariable_2D_r4
!TPP ESAC

!TPP CASE 10
    WriteVariable_1D_l4 = err
  end function WriteVariable_1D_l4
!TPP ESAC
!TPP CASE 9
    WriteVariable_1D_i8 = err
  end function WriteVariable_1D_i8
!TPP ESAC
!TPP CASE 8
    WriteVariable_1D_i4 = err
  end function WriteVariable_1D_i4
!TPP ESAC
!TPP CASE 7
    WriteVariable_1D_r8 = err
  end function WriteVariable_1D_r8
!TPP ESAC
!TPP CASE 6
    WriteVariable_1D_r4 = err
  end function WriteVariable_1D_r4
!TPP ESAC

!TPP CASE 5
    WriteVariable_0D_l4 = err
  end function WriteVariable_0D_l4
!TPP ESAC
!TPP CASE 4
    WriteVariable_0D_i8 = err
  end function WriteVariable_0D_i8
!TPP ESAC
!TPP CASE 3
    WriteVariable_0D_i4 = err
  end function WriteVariable_0D_i4
!TPP ESAC
!TPP CASE 2
    WriteVariable_0D_r8 = err
  end function WriteVariable_0D_r8
!TPP ESAC
!TPP CASE 1
    WriteVariable_0D_r4 = err
  end function WriteVariable_0D_r4
!TPP ESAC

!TPP END
  
  
  ! String variable writes
  
  
  integer(kind=TIO_ERRK) function WriteVariable_0D_st( fileID, varID, datatype, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: varID
    integer(kind=TIO_IPK),   intent(in) :: datatype
    character(len=*),        intent(in) :: vdata
    
    
    character(len=TIO_STRLEN_F) :: ldata
    
    integer :: vdata_len
    integer(kind=TIO_ERRK) :: err
    
    vdata_len = len_trim(vdata)
    if (vdata_len > TIO_STRLEN_F) then
      call seterror("TIO_Write_Variable_f", "vdata string length must be <= TIO_STRLEN_F")
      WriteVariable_0D_st = TIO_ERR_F90_API_F
      return
    end if
    
    WriteVariable_0D_st = setlen("TIO_Write_Variable_f", "vdata", vdata, vdata_len)
    if (WriteVariable_0D_st /= TIO_SUCCESS_F) return
    
    ldata = trim(vdata)
    
    call tio_write_variable_c( fileID, varID, datatype, ldata, vdata_len, err )
    
    WriteVariable_0D_st = err
    
  end function WriteVariable_0D_st
  
  
  integer(kind=TIO_ERRK) function WriteVariable_1D_st( fileID, varID, datatype, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),         intent(in) :: fileID
    integer(kind=TIO_OBJK),          intent(in) :: varID
    integer(kind=TIO_IPK),           intent(in) :: datatype
    character(len=*), dimension(1:), intent(in) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:), allocatable :: ldata
    
    integer :: dummy
    integer(kind=TIO_SIZEK) :: ni, i
    integer(kind=TIO_ERRK)  :: err
    
    
    ni = size(vdata)
    
    allocate(ldata(ni))
    
    do i = 1, ni
      if (len_trim(vdata(i)) > TIO_STRLEN_F) then
        call seterror("TIO_Write_Variable_f", "vdata(:) string lengths must be <= TIO_STRLEN_F")
        WriteVariable_1D_st = TIO_ERR_F90_API_F
        return
      end if
      ldata(i) = trim(vdata(i))
    end do
    
    call tio_write_variable_c( fileID, varID, datatype, ldata, dummy, err )
    
    deallocate(ldata)
    
    WriteVariable_1D_st = err
    
  end function WriteVariable_1D_st

  
  integer(kind=TIO_ERRK) function WriteVariable_2D_st( fileID, varID, datatype, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),            intent(in) :: fileID
    integer(kind=TIO_OBJK),             intent(in) :: varID
    integer(kind=TIO_IPK),              intent(in) :: datatype
    character(len=*), dimension(1:,1:), intent(in) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:,:), allocatable :: ldata
    
    integer :: dummy
    integer(kind=TIO_SIZEK) :: ni, nj, i, j
    integer(kind=TIO_ERRK)  :: err
    
    
    ni = size(vdata, dim=1)
    nj = size(vdata, dim=2)
    
    allocate(ldata(ni,nj))
    
    do j = 1, nj
      do i = 1, ni
        if (len_trim(vdata(i,j)) > TIO_STRLEN_F) then
          call seterror("TIO_Write_Variable_f", "vdata(:,:) string lengths must be <= TIO_STRLEN_F")
          WriteVariable_2D_st = TIO_ERR_F90_API_F
          return
        end if
        ldata(i,j) = trim(vdata(i,j))
      end do
    end do
    
    call tio_write_variable_c( fileID, varID, datatype, ldata, dummy, err )
    
    deallocate(ldata)
    
    WriteVariable_2D_st = err
    
  end function WriteVariable_2D_st
  
  
  integer(kind=TIO_ERRK) function WriteVariable_3D_st( fileID, varID, datatype, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),               intent(in) :: fileID
    integer(kind=TIO_OBJK),                intent(in) :: varID
    integer(kind=TIO_IPK),                 intent(in) :: datatype
    character(len=*), dimension(1:,1:,1:), intent(in) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:,:,:), allocatable :: ldata
    
    integer :: dummy
    integer(kind=TIO_SIZEK) :: ni, nj, nk, i, j, k
    integer(kind=TIO_ERRK)  :: err
    
    
    ni = size(vdata, dim=1)
    nj = size(vdata, dim=2)
    nk = size(vdata, dim=3)
    
    allocate(ldata(ni,nj,nk))
    
    do k = 1, nk
      do j = 1, nj
        do i = 1, ni
          if (len_trim(vdata(i,j,k)) > TIO_STRLEN_F) then
            call seterror("TIO_Write_Variable_f", "vdata(:,:,:) string lengths must be <= TIO_STRLEN_F")
            WriteVariable_3D_st = TIO_ERR_F90_API_F
            return
          end if
          ldata(i,j,k) = trim(vdata(i,j,k))
        end do
      end do
    end do
    
    call tio_write_variable_c( fileID, varID, datatype, ldata, dummy, err )
    
    deallocate(ldata)
    
    WriteVariable_3D_st = err
    
  end function WriteVariable_3D_st
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_Variable_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_Variable
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_Variable_f( fileID, varID, datatype, vdata )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: varID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)  :: datatype
  ! <DATA>   [CHOICE], [DIMENSION,]   intent(out) :: vdata  
  ! </FAPI>
  ! <FDES>
  ! <DATA> <code>[CHOICE]</code> can be <code>real(kind=4)</code>, <code>real(kind=8)</code>,
  ! <DATA> <code>integer(kind=4)</code>, <code>integer(kind=8)</code>, <code>logical(kind=4)</code>
  ! <DATA> or <code>character(len=*)</code>
  ! <DATA> <code>[DIMENSION]</code> is optional so can be scalar (0D), 1D <code>(:)</code>, 
  ! <DATA> 2D <code>(:,:)</code> or 3D <code>(:,:,:)</code> as appropriate.
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
!TPP START 20

!TPP CASE 1
  integer(kind=TIO_ERRK) function ReadVariable_0D_r4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 2
  integer(kind=TIO_ERRK) function ReadVariable_0D_r8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 3
  integer(kind=TIO_ERRK) function ReadVariable_0D_i4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 4
  integer(kind=TIO_ERRK) function ReadVariable_0D_i8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 5
  integer(kind=TIO_ERRK) function ReadVariable_0D_l4( fileID, varID, datatype, vdata )
!TPP ESAC

    
!TPP CASE 6
  integer(kind=TIO_ERRK) function ReadVariable_1D_r4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 7
  integer(kind=TIO_ERRK) function ReadVariable_1D_r8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 8
  integer(kind=TIO_ERRK) function ReadVariable_1D_i4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 9
  integer(kind=TIO_ERRK) function ReadVariable_1D_i8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 10
  integer(kind=TIO_ERRK) function ReadVariable_1D_l4( fileID, varID, datatype, vdata )
!TPP ESAC
    
    
!TPP CASE 11
  integer(kind=TIO_ERRK) function ReadVariable_2D_r4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 12
  integer(kind=TIO_ERRK) function ReadVariable_2D_r8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 13
  integer(kind=TIO_ERRK) function ReadVariable_2D_i4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 14
  integer(kind=TIO_ERRK) function ReadVariable_2D_i8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 15
  integer(kind=TIO_ERRK) function ReadVariable_2D_l4( fileID, varID, datatype, vdata )
!TPP ESAC

    
!TPP CASE 16
  integer(kind=TIO_ERRK) function ReadVariable_3D_r4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 17
  integer(kind=TIO_ERRK) function ReadVariable_3D_r8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 18
  integer(kind=TIO_ERRK) function ReadVariable_3D_i4( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 19
  integer(kind=TIO_ERRK) function ReadVariable_3D_i8( fileID, varID, datatype, vdata )
!TPP ESAC

!TPP CASE 20
  integer(kind=TIO_ERRK) function ReadVariable_3D_l4( fileID, varID, datatype, vdata )
!TPP ESAC

    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: varID
    integer(kind=TIO_IPK),   intent(in) :: datatype

!TPP CASE 1
    real(kind=4),     intent(out) :: vdata
!TPP ESAC
!TPP CASE 2
    real(kind=8),     intent(out) :: vdata
!TPP ESAC
!TPP CASE 3
    integer(kind=4),  intent(out) :: vdata
!TPP ESAC
!TPP CASE 4
    integer(kind=8),  intent(out) :: vdata
!TPP ESAC
!TPP CASE 5
    logical(kind=4),  intent(out) :: vdata
!TPP ESAC
    
!TPP CASE 6
    real(kind=4),     dimension(:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 7
    real(kind=8),     dimension(:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 8
    integer(kind=4),  dimension(:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 9
    integer(kind=8),  dimension(:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 10
    logical(kind=4),  dimension(:), intent(out) :: vdata
!TPP ESAC
    
!TPP CASE 11
    real(kind=4),     dimension(:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 12
    real(kind=8),     dimension(:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 13
    integer(kind=4),  dimension(:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 14
    integer(kind=8),  dimension(:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 15
    logical(kind=4),  dimension(:,:), intent(out) :: vdata
!TPP ESAC
    
!TPP CASE 16
    real(kind=4),     dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 17
    real(kind=8),     dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 18
    integer(kind=4),  dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 19
    integer(kind=8),  dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 20
    logical(kind=4),  dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
    
    integer :: dummy
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_read_variable_c( fileID, varID, datatype, vdata, dummy, err )
    
!TPP CASE 20
    ReadVariable_3D_l4 = err
  end function ReadVariable_3D_l4
!TPP ESAC
!TPP CASE 19
    ReadVariable_3D_i8 = err
  end function ReadVariable_3D_i8
!TPP ESAC
!TPP CASE 18
    ReadVariable_3D_i4 = err
  end function ReadVariable_3D_i4
!TPP ESAC
!TPP CASE 17
    ReadVariable_3D_r8 = err
  end function ReadVariable_3D_r8
!TPP ESAC
!TPP CASE 16
    ReadVariable_3D_r4 = err
  end function ReadVariable_3D_r4
!TPP ESAC

!TPP CASE 15
    ReadVariable_2D_l4 = err
  end function ReadVariable_2D_l4
!TPP ESAC
!TPP CASE 14
    ReadVariable_2D_i8 = err
  end function ReadVariable_2D_i8
!TPP ESAC
!TPP CASE 13
    ReadVariable_2D_i4 = err
  end function ReadVariable_2D_i4
!TPP ESAC
!TPP CASE 12
    ReadVariable_2D_r8 = err
  end function ReadVariable_2D_r8
!TPP ESAC
!TPP CASE 11
    ReadVariable_2D_r4 = err
  end function ReadVariable_2D_r4
!TPP ESAC

!TPP CASE 10
    ReadVariable_1D_l4 = err
  end function ReadVariable_1D_l4
!TPP ESAC
!TPP CASE 9
    ReadVariable_1D_i8 = err
  end function ReadVariable_1D_i8
!TPP ESAC
!TPP CASE 8
    ReadVariable_1D_i4 = err
  end function ReadVariable_1D_i4
!TPP ESAC
!TPP CASE 7
    ReadVariable_1D_r8 = err
  end function ReadVariable_1D_r8
!TPP ESAC
!TPP CASE 6
    ReadVariable_1D_r4 = err
  end function ReadVariable_1D_r4
!TPP ESAC

!TPP CASE 5
    ReadVariable_0D_l4 = err
  end function ReadVariable_0D_l4
!TPP ESAC
!TPP CASE 4
    ReadVariable_0D_i8 = err
  end function ReadVariable_0D_i8
!TPP ESAC
!TPP CASE 3
    ReadVariable_0D_i4 = err
  end function ReadVariable_0D_i4
!TPP ESAC
!TPP CASE 2
    ReadVariable_0D_r8 = err
  end function ReadVariable_0D_r8
!TPP ESAC
!TPP CASE 1
    ReadVariable_0D_r4 = err
  end function ReadVariable_0D_r4
!TPP ESAC

!TPP END
  
  
  ! String variable reads
  
  
  integer(kind=TIO_ERRK) function ReadVariable_0D_st( fileID, varID, datatype, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: varID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    character(len=*),        intent(out) :: vdata
    
    character(len=TIO_STRLEN_F) :: ldata = ""
    
    integer                :: il
    integer(kind=TIO_ERRK) :: err
    
    
    il = TIO_STRLEN_F
    call tio_read_variable_c( fileID, varID, datatype, ldata, il, err )
    
    ReadVariable_0D_st = err
    if (ReadVariable_0D_st /= TIO_SUCCESS_F) return
    
    if (len_trim(ldata) > len(vdata)) then
      call seterror("TIO_Read_Variable_f", "vdata string is not long enough")
      ReadVariable_0D_st = TIO_ERR_F90_API_F
      return
    end if
    vdata = ldata
    
    
  end function ReadVariable_0D_st
  
  
  integer(kind=TIO_ERRK) function ReadVariable_1D_st( fileID, varID, datatype, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),         intent(in)  :: fileID
    integer(kind=TIO_OBJK),          intent(in)  :: varID
    integer(kind=TIO_IPK),           intent(in)  :: datatype
    character(len=*), dimension(1:), intent(out) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:), allocatable :: ldata
    
    integer :: dummy
    integer                :: ni, i, ld
    integer(kind=TIO_ERRK) :: err
    
    
    ni = size(vdata)
    
    allocate(ldata(ni))
    
    do i = 1, ni
      ldata(i) = ""
    end do
    
    call tio_read_variable_c( fileID, varID, datatype, ldata, dummy, err )
    
    ReadVariable_1D_st = err
    if (ReadVariable_1D_st /= TIO_SUCCESS_F) then
      deallocate(ldata)
      return
    end if
    
    ld = len(vdata(1))
    do i = 1, ni
      if (len_trim(ldata(i)) > ld) then
        call seterror("TIO_Read_Variable_f", "strings in data array are not long enough")
        ReadVariable_1D_st = TIO_ERR_F90_API_F
        return
      end if
      vdata(i) = ldata(i)
    end do
    
    deallocate(ldata)
    
  end function ReadVariable_1D_st
  
  
  integer(kind=TIO_ERRK) function ReadVariable_2D_st( fileID, varID, datatype, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),            intent(in)  :: fileID
    integer(kind=TIO_OBJK),             intent(in)  :: varID
    integer(kind=TIO_IPK),              intent(in)  :: datatype
    character(len=*), dimension(1:,1:), intent(out) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:,:), allocatable :: ldata
    
    integer :: dummy
    integer                :: ni, nj, i, j, ld
    integer(kind=TIO_ERRK) :: err
    
    
    ni = size(vdata, dim=1)
    nj = size(vdata, dim=2)
    
    allocate(ldata(ni,nj))
    
    do j = 1, nj
      do i = 1, ni
        ldata(i,j) = ""
      end do
    end do
    
    call tio_read_variable_c( fileID, varID, datatype, ldata, dummy, err )
    
    ReadVariable_2D_st = err
    if (ReadVariable_2D_st /= TIO_SUCCESS_F) then
      deallocate(ldata)
      return
    end if
    
    ld = len(vdata(1,1))
    do j = 1, nj
      do i = 1, ni
        if (len_trim(ldata(i,j)) > ld) then
          call seterror("TIO_Read_Variable_f", "strings in data array are not long enough")
          ReadVariable_2D_st = TIO_ERR_F90_API_F
          return
        end if
        vdata(i,j) = ldata(i,j)
      end do
    end do
    
    deallocate(ldata)
    
  end function ReadVariable_2D_st
  
  
  integer(kind=TIO_ERRK) function ReadVariable_3D_st( fileID, varID, datatype, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),               intent(in)  :: fileID
    integer(kind=TIO_OBJK),                intent(in)  :: varID
    integer(kind=TIO_IPK),                 intent(in)  :: datatype
    character(len=*), dimension(1:,1:,1:), intent(out) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:,:,:), allocatable :: ldata
    
    integer :: dummy
    integer                :: ni, nj, nk, i, j, k, ld
    integer(kind=TIO_ERRK) :: err
    
    
    ni = size(vdata, dim=1)
    nj = size(vdata, dim=2)
    nk = size(vdata, dim=3)
    
    allocate(ldata(ni,nj,nk))
    
    do k = 1, nk
      do j = 1, nj
        do i = 1, ni
          ldata(i,j,k) = ""
        end do
      end do
    end do
    
    call tio_read_variable_c( fileID, varID, datatype, ldata, dummy, err )
    
    ReadVariable_3D_st = err
    if (ReadVariable_3D_st /= TIO_SUCCESS_F) then
      deallocate(ldata)
      return
    end if
    
    ld = len(vdata(1,1,1))
    do k = 1, nk
      do j = 1, nj
        do i = 1, ni
          if (len_trim(ldata(i,j,k)) > ld) then
            call seterror("TIO_Read_Variable_f", "strings in data array are not long enough")
            ReadVariable_3D_st = TIO_ERR_F90_API_F
            return
          end if
          vdata(i,j,k) = ldata(i,j,k)
        end do
      end do
    end do
    
    deallocate(ldata)
    
  end function ReadVariable_3D_st
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_Variable_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_Variable_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_Variable_Section_f( fileID, varID, datatype,
  ! <DATA>                                                              ndims, lolimit, hilimit,
  ! <DATA>                                                              vdata )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),               intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(in)  :: varID
  ! <DATA>   integer(kind=TIO_IPK),                 intent(in)  :: datatype
  ! <DATA>   integer(kind=TIO_IPK),                 intent(in)  :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), dimension(:), intent(in)  :: lolimit, hilimit
  ! <DATA>   [CHOICE], [DIMENSION],                 intent(out) :: vdata
  ! </FAPI>
  ! <FDES>
  ! <DATA> <code>[CHOICE]</code> can be <code>real(kind=4)</code>, <code>real(kind=8)</code>,
  ! <DATA> <code>integer(kind=4)</code>, <code>integer(kind=8)</code>, <code>logical(kind=4)</code>
  ! <DATA> or <code>character(len=*)</code>
  ! <DATA> <code>[DIMENSION]</code> is optional so can be scalar (0D), 1D <code>(:)</code>, 
  ! <DATA> 2D <code>(:,:)</code> or 3D <code>(:,:,:)</code> as appropriate.
  ! <DATA>
  ! <DATA> Note - string datatypes are not supported for reading by section at present.
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
!TPP START 15

!TPP CASE 1
  integer(kind=TIO_ERRK) function ReadVariableSection_1D_r4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 2
  integer(kind=TIO_ERRK) function ReadVariableSection_1D_r8( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 3
  integer(kind=TIO_ERRK) function ReadVariableSection_1D_i4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 4
  integer(kind=TIO_ERRK) function ReadVariableSection_1D_i8( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 5
  integer(kind=TIO_ERRK) function ReadVariableSection_1D_l4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
    
!TPP CASE 6
  integer(kind=TIO_ERRK) function ReadVariableSection_2D_r4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 7
  integer(kind=TIO_ERRK) function ReadVariableSection_2D_r8( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 8
  integer(kind=TIO_ERRK) function ReadVariableSection_2D_i4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 9
  integer(kind=TIO_ERRK) function ReadVariableSection_2D_i8( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 10
  integer(kind=TIO_ERRK) function ReadVariableSection_2D_l4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
    
!TPP CASE 11
  integer(kind=TIO_ERRK) function ReadVariableSection_3D_r4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 12
  integer(kind=TIO_ERRK) function ReadVariableSection_3D_r8( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 13
  integer(kind=TIO_ERRK) function ReadVariableSection_3D_i4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 14
  integer(kind=TIO_ERRK) function ReadVariableSection_3D_i8( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
!TPP CASE 15
  integer(kind=TIO_ERRK) function ReadVariableSection_3D_l4( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
!TPP ESAC
    
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in)  :: fileID
    integer(kind=TIO_OBJK),                 intent(in)  :: varID
    integer(kind=TIO_IPK),                  intent(in)  :: datatype
    integer(kind=TIO_IPK),                  intent(in)  :: ndims
    integer(kind=TIO_SIZEK), dimension(1:), intent(in)  :: lolimit, hilimit

!TPP CASE 1
    real(kind=4),     dimension(:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 2
    real(kind=8),     dimension(:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 3
    integer(kind=4),  dimension(:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 4
    integer(kind=8),  dimension(:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 5
    logical(kind=4),  dimension(:), intent(out) :: vdata
!TPP ESAC
    
!TPP CASE 6
    real(kind=4),     dimension(:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 7
    real(kind=8),     dimension(:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 8
    integer(kind=4),  dimension(:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 9
    integer(kind=8),  dimension(:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 10
    logical(kind=4),  dimension(:,:), intent(out) :: vdata
!TPP ESAC
    
!TPP CASE 11
    real(kind=4),     dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 12
    real(kind=8),     dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 13
    integer(kind=4),  dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 14
    integer(kind=8),  dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
!TPP CASE 15
    logical(kind=4),  dimension(:,:,:), intent(out) :: vdata
!TPP ESAC
    
    integer(kind=TIO_ERRK) :: err
    integer                :: vdata_len
    
    call tio_read_variable_section_c( fileID, varID, datatype, ndims, lolimit, hilimit, vdata, vdata_len, err )
    
    
!TPP CASE 15
    ReadVariableSection_3D_l4 = err
  end function ReadVariableSection_3D_l4
!TPP ESAC
!TPP CASE 14
    ReadVariableSection_3D_i8 = err
  end function ReadVariableSection_3D_i8
!TPP ESAC
!TPP CASE 13
    ReadVariableSection_3D_i4 = err
  end function ReadVariableSection_3D_i4
!TPP ESAC
!TPP CASE 12
    ReadVariableSection_3D_r8 = err
  end function ReadVariableSection_3D_r8
!TPP ESAC
!TPP CASE 11
    ReadVariableSection_3D_r4 = err
  end function ReadVariableSection_3D_r4
!TPP ESAC

!TPP CASE 10
    ReadVariableSection_2D_l4 = err
  end function ReadVariableSection_2D_l4
!TPP ESAC
!TPP CASE 9
    ReadVariableSection_2D_i8 = err
  end function ReadVariableSection_2D_i8
!TPP ESAC
!TPP CASE 8
    ReadVariableSection_2D_i4 = err
  end function ReadVariableSection_2D_i4
!TPP ESAC
!TPP CASE 7
    ReadVariableSection_2D_r8 = err
  end function ReadVariableSection_2D_r8
!TPP ESAC
!TPP CASE 6
    ReadVariableSection_2D_r4 = err
  end function ReadVariableSection_2D_r4
!TPP ESAC

!TPP CASE 5
    ReadVariableSection_1D_l4 = err
  end function ReadVariableSection_1D_l4
!TPP ESAC
!TPP CASE 4
    ReadVariableSection_1D_i8 = err
  end function ReadVariableSection_1D_i8
!TPP ESAC
!TPP CASE 3
    ReadVariableSection_1D_i4 = err
  end function ReadVariableSection_1D_i4
!TPP ESAC
!TPP CASE 2
    ReadVariableSection_1D_r8 = err
  end function ReadVariableSection_1D_r8
!TPP ESAC
!TPP CASE 1
    ReadVariableSection_1D_r4 = err
  end function ReadVariableSection_1D_r4
!TPP ESAC

!TPP END
  
  
  ! String variable section reads
  
  
  integer(kind=TIO_ERRK) function ReadVariableSection_1D_st( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),         intent(in)  :: fileID
    integer(kind=TIO_OBJK),          intent(in)  :: varID
    integer(kind=TIO_IPK),           intent(in)  :: datatype
    integer(kind=TIO_IPK),           intent(in)  :: ndims
    integer(kind=TIO_SIZEK), dimension(1:), intent(in)  :: lolimit, hilimit
    character(len=*), dimension(1:), intent(out) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:), allocatable :: ldata
    
    integer                :: ni, i, ld
    integer(kind=TIO_ERRK) :: err
    integer                :: ldata_len
    
    ni = hilimit(1) - lolimit(1) + 1
    
    if (size(vdata) < ni) then
      call seterror("TIO_Read_Variable_Section_f", "vdata data array is not big enough")
      ReadVariableSection_1D_st = TIO_ERR_F90_API_F
      return
    end if
    
    allocate(ldata(ni))
    
    do i = 1, ni
      ldata(i) = ""
    end do
    
    
    call tio_read_variable_section_c( fileID, varID, datatype, ndims, lolimit, hilimit, ldata, ldata_len, err )
    
    ReadVariableSection_1D_st = err
    if (ReadVariableSection_1D_st /= TIO_SUCCESS_F) then
      deallocate(ldata)
      return
    end if
    
    ld = len(vdata(1))
    do i = 1, ni
      if (len_trim(ldata(i)) > ld) then
        call seterror("TIO_Read_Variable_Section_f", "strings in data array are not long enough")
        ReadVariableSection_1D_st = TIO_ERR_F90_API_F
        return
      end if
      vdata(i) = ldata(i)
    end do
    
    deallocate(ldata)
    
  end function ReadVariableSection_1D_st
  
  
  integer(kind=TIO_ERRK) function ReadVariableSection_2D_st( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),         intent(in)  :: fileID
    integer(kind=TIO_OBJK),          intent(in)  :: varID
    integer(kind=TIO_IPK),           intent(in)  :: datatype
    integer(kind=TIO_IPK),           intent(in)  :: ndims
    integer(kind=TIO_SIZEK), dimension(1:), intent(in)  :: lolimit, hilimit
    character(len=*), dimension(1:,1:), intent(out) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:,:), allocatable :: ldata
    
    integer                :: ni, nj, i, j, ld
    integer(kind=TIO_ERRK) :: err
    integer                :: ldata_len
    
    ni = hilimit(1) - lolimit(1) + 1
    nj = hilimit(2) - lolimit(2) + 1
    
    if ((size(vdata,dim=1) < ni) .or. (size(vdata,dim=2) < nj)) then
      call seterror("TIO_Read_Variable_Section_f", "vdata data array is not big enough")
      ReadVariableSection_2D_st = TIO_ERR_F90_API_F
      return
    end if
    
    allocate(ldata(ni,nj))
    
    do j = 1, nj
      do i = 1, ni
        ldata(i,j) = ""
      end do
    end do
    
    call tio_read_variable_section_c( fileID, varID, datatype, ndims, lolimit, hilimit, ldata, ldata_len, err )
    
    ReadVariableSection_2D_st = err
    if (ReadVariableSection_2D_st /= TIO_SUCCESS_F) then
      deallocate(ldata)
      return
    end if
    
    ld = len(vdata(1,1))
    do j = 1, nj
      do i = 1, ni
        if (len_trim(ldata(i,j)) > ld) then
          call seterror("TIO_Read_Variable_Section_f", "strings in data array are not long enough")
          ReadVariableSection_2D_st = TIO_ERR_F90_API_F
          return
        end if
        vdata(i,j) = ldata(i,j)
      end do
    end do
    
    deallocate(ldata)
    
  end function ReadVariableSection_2D_st
  
  
  integer(kind=TIO_ERRK) function ReadVariableSection_3D_st( fileID, varID, datatype, ndims,  &
       &                                                     lolimit, hilimit, vdata )
    
    implicit none
    
    integer(kind=TIO_FILEK),         intent(in)  :: fileID
    integer(kind=TIO_OBJK),          intent(in)  :: varID
    integer(kind=TIO_IPK),           intent(in)  :: datatype
    integer(kind=TIO_IPK),           intent(in)  :: ndims
    integer(kind=TIO_SIZEK), dimension(1:), intent(in)  :: lolimit, hilimit
    character(len=*), dimension(1:,1:,1:), intent(out) :: vdata
    
    character(len=TIO_STRLEN_F), dimension(:,:,:), allocatable :: ldata
    
    integer                :: ni, nj, nk, i, j, k, ld
    integer(kind=TIO_ERRK) :: err
    integer                :: ldata_len
    
    ni = hilimit(1) - lolimit(1) + 1
    nj = hilimit(2) - lolimit(2) + 1
    nk = hilimit(3) - lolimit(3) + 1
    
    if ((size(vdata,dim=1) < ni) .or. (size(vdata,dim=2) < nj) .or. (size(vdata,dim=3) < nk)) then
      call seterror("TIO_Read_Variable_Section_f", "vdata data array is not big enough")
      ReadVariableSection_3D_st = TIO_ERR_F90_API_F
      return
    end if
    
    allocate(ldata(ni,nj,nk))
    
    do k = 1, nk
      do j = 1, nj
        do i = 1, ni
          ldata(i,j,k) = ""
        end do
      end do
    end do
    
    
    call tio_read_variable_section_c( fileID, varID, datatype, ndims, lolimit, hilimit, ldata, ldata_len, err )
    
    ReadVariableSection_3D_st = err
    if (ReadVariableSection_3D_st /= TIO_SUCCESS_F) then
      deallocate(ldata)
      return
    end if
    
    ld = len(vdata(1,1,1))
    do k = 1, nk
      do j = 1, nj
        do i = 1, ni
          if (len_trim(ldata(i,j,k)) > ld) then
            call seterror("TIO_Read_Variable_Section_f", "strings in data array are not long enough")
            ReadVariableSection_3D_st = TIO_ERR_F90_API_F
            return
          end if
          vdata(i,j,k) = ldata(i,j,k)
        end do
      end do
    end do
    
    deallocate(ldata)
    
  end function ReadVariableSection_3D_st
  
  
  
  
  ! ********************************************************************************************** !
  
  
end module TIO_Variable_mod



!
!  EOF

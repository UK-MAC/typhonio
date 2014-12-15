!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!

module TIO_Utils_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  
  implicit none
  
  
  private
  public :: setlen, setstr, listnames
  
  integer, parameter, public :: LISTCLASS_VGROUP   = 1
  integer, parameter, public :: LISTCLASS_VARIABLE = 2
  integer, parameter, public :: LISTCLASS_STATE    = 3
  integer, parameter, public :: LISTCLASS_MESH     = 4
  integer, parameter, public :: LISTCLASS_MAT      = 5
  integer, parameter, public :: LISTCLASS_QUANT    = 6
  
contains
  
  
  ! ********************************************************************************************** !
  !
  ! Internal F90 API functions
  !
  ! ********************************************************************************************** !
  
  
  
  
  ! ********************************************************************************************** !
  !
  ! setlen
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function setlen(subname, strname, strin, strlen)
    
    implicit none
    
    character(len=*), intent(in)  :: subname
    character(len=*), intent(in)  :: strname
    character(len=*), intent(in)  :: strin
    integer,          intent(out) :: strlen
    
    
    strlen = len_trim(strin)
    
    if (strlen > TIO_STRLEN_F) then
      call seterror(subname, strname // " string length must be <= TIO_STRLEN_F")
      setlen = TIO_ERR_F90_API_F
    else
      setlen = TIO_SUCCESS_F
    end if
    
  end function setlen
  
  
  
  ! ********************************************************************************************** !
  !
  ! setstr
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function setstr(subname, strname, strin, strout)
    
    implicit none
    
    character(len=*), intent(in)  :: subname
    character(len=*), intent(in)  :: strname
    character(len=*), intent(in)  :: strin
    character(len=*), intent(out) :: strout
    
    
    if (len_trim(strin) > len(strout)) then
      call seterror(subname, strname // " string is not long enough")
      setstr = TIO_ERR_F90_API_F
    else
      strout = trim(strin)
      setstr = TIO_SUCCESS_F
    end if
    
  end function setstr
  
  
  
  ! ********************************************************************************************** !
  !
  ! listnames
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function listnames(subname, class, fileID, objectID, nobj, names)
    
    implicit none
    
    character(len=*),                intent(in)            :: subname
    integer,                         intent(in)            :: class
    integer(kind=TIO_FILEK),         intent(in)            :: fileID
    integer(kind=TIO_OBJK),          intent(in)            :: objectID
    integer(kind=TIO_SIZEK),         intent(in)            :: nobj
    character(len=*), dimension(1:), intent(out), optional :: names
    
    character(len=TIO_STRLEN_F), dimension(:), allocatable :: lnames
    
    integer   :: idummy
    character :: cdummy
    
    integer(kind=TIO_SIZEK) :: in, lm, lmax
    integer(kind=TIO_ERRK)  :: err
    
    
    if (nobj >= 0) then
      
      
      if (.not. present(names)) then
        call seterror(subname, "names not present")
        listnames = TIO_ERR_F90_API_F
        return
      end if
      
      if (nobj > ubound(names, dim=1)) then    ! dim=1 necessary because character(), dimension(:)
                                               ! is actually a 2D array!
        call seterror(subname, "names string array is not big enough")
        listnames = TIO_ERR_F90_API_F
        deallocate(lnames)
        return
      end if
      
      allocate(lnames(nobj))
      
      do in = 1, nobj
        lnames(in) = ""  ! Need to initialize or len_trim() fails to work!
      end do
      
      select case (class)
      case (LISTCLASS_VGROUP)
        call tio_list_vargroups_c(fileID, objectID, nobj, lnames, err)
      case (LISTCLASS_VARIABLE)
        call tio_list_variables_c(fileID, objectID, nobj, lnames, err)
      case (LISTCLASS_STATE)
        call tio_list_states_c(fileID, nobj, lnames, err)
      case (LISTCLASS_MESH)
        call tio_list_meshes_c(fileID, objectID, nobj, lnames, err)
      case (LISTCLASS_MAT)    ! Datatype will be ignored in next call
        call tio_get_mat_range_c(fileID, objectID, TIO_INTEGER4_F, nobj, idummy, lnames, err)
      case (LISTCLASS_QUANT)
        call tio_list_quants_c(fileID, objectID, nobj, lnames, err)

      case default
        call seterror(subname, "invalid class")
        listnames = TIO_ERR_F90_API_F
        deallocate(lnames)
        return
      end select
      
      lmax = 0
      do in = 1, nobj
        lm = len_trim(lnames(in))
        if (lm > lmax) lmax = lm
      end do
      
      if (lmax > len(names(1))) then          ! len(names(1)) gives length of all strings in array
        call seterror(subname, "strings in names array are not long enough")
        listnames = TIO_ERR_F90_API_F
        deallocate(lnames)
        return
      end if
      
      do in = 1, nobj
        names(in) = trim(lnames(in))
      end do
      
      deallocate(lnames)
      
      
    else
      
      
      select case (class)
      case default
        call listnames_c( class, fileID, objectID, nobj, cdummy, err )
      end select
      
      
    end if
    
    listnames = TIO_SUCCESS_F
    
  end function listnames
  
  
end module TIO_Utils_mod

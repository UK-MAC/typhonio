!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!

module TIO_Error_mod
  
  use TIO_Global_mod

#ifdef _TYPHIO_F2003_STDERR
  use ISO_FORTRAN_ENV, only : ERROR_UNIT   ! Gives us IO units
#endif

  implicit none
  
  type err_tp
    character(len=32)  :: subr
    character(len=256) :: desc
  end type err_tp
  
  type (err_tp), save :: ferr
  
#ifdef _TYPHIO_F2003_STDERR
  integer, parameter :: stderr = ERROR_UNIT
#else
  integer, parameter :: stderr = 0             ! Just use zero, which is usually stderr
#endif
  
  private
  public :: TIO_Get_Error_f, TIO_Set_Verbose_f, TIO_Get_Verbose_f
  public :: seterror
  
  
contains
  
  
  ! Internal routine to F90 API to log an error for later printing
  subroutine seterror(subr, desc)
    
    implicit none
    
    character(len=*), intent(in) :: subr, desc
    
    ferr%subr = subr
    ferr%desc = desc
    
  end subroutine seterror
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Error_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Error
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Error_f( err, str )
  ! <DATA>  
  ! <DATA>   integer(kind=TIO_ERRK), intent(in)  :: err
  ! <DATA>   character(len=*),       intent(out) :: str
  ! </FAPI>
  ! <FDES>
  ! <DATA> The returned error string will be truncated, if necessary, to the length of
  ! <DATA> <code>str</code>.
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_Error_f( err, str )
    
    implicit none
    
    integer(kind=TIO_ERRK), intent(in)  :: err
    character(len=*),       intent(out) :: str
    
    
    integer                :: vmode
    integer                :: str_len
    integer(kind=TIO_ERRK) :: l_err
    
    
    
    str_len = len(str)
    if (str_len < TIO_STRLEN_F) then
      call seterror("TIO_Get_Error_f", "len(str) < TIO_STRLEN_F")
      TIO_Get_Error_f = TIO_ERR_F90_API_F
      return
    end if
    
    
    call tio_get_error_c( err, str, str_len, l_err )
    
    TIO_Get_Error_f = l_err
    
    if (l_err /= TIO_SUCCESS_F) return
    
    
    if (err == TIO_ERR_F90_API_F) then
      
      call tio_get_verbose_c( vmode, l_err )
      
      if (vmode /= TIO_VERBOSE_OFF_F) then
        
        write (stderr,'("-- TyphonIO F90 error traceback:  ----------------------------")')
        write (stderr,*) "    ", trim(ferr%subr), ":"
        write (stderr,*) "    ", trim(ferr%desc)
        write (stderr,*) ""
        
      end if
      
    end if
    
    
    TIO_Get_Error_f = l_err
    
  end function TIO_Get_Error_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Verbose_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Verbose
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Verbose_f( mode )
  ! <DATA>  
  ! <DATA>   integer, intent(in) :: mode
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Set_Verbose_f( mode )
    
    implicit none
    
    integer, intent(in) :: mode
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_set_verbose_c( mode, err )
    
    TIO_Set_Verbose_f = err
    
  end function TIO_Set_Verbose_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Verbose_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Verbose
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Verbose_f( mode )
  ! <DATA>   
  ! <DATA>   integer, intent(out) :: mode
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_Verbose_f( mode )
    
    implicit none
    
    integer, intent(out) :: mode
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_get_verbose_c( mode, err )
    
    TIO_Get_Verbose_f = err
    
  end function TIO_Get_Verbose_f
  
  
  
  ! ********************************************************************************************** !
  
  
end module TIO_Error_mod



!
!  EOF

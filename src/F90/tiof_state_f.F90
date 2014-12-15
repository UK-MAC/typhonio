!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!

module TIO_State_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  use TIO_Utils_mod
  
  
  implicit none
  
  
  private
  public :: TIO_List_States_f
  public :: TIO_Create_State_f, TIO_Open_State_f, TIO_Open_State_idx_f
  public :: TIO_Get_State_Info_f
  public :: TIO_Close_State_f
  
    
contains
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_List_States_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_List_States
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_List_States_f( fileID, nobj, names )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
  ! <DATA>   character(len=*), dimension(:), intent(out), optional :: names
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_List_States_f(fileID, nobj, names)
    
    implicit none
    
    integer(kind=TIO_FILEK),        intent(in)            :: fileID
    integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
    character(len=*), dimension(:), intent(out), optional :: names
    
    
    character :: cdummy
    
    integer(kind=TIO_SIZEK) :: ln
    integer(kind=TIO_ERRK)  :: err
    
    
    ln = -1
    call tio_list_states_c(fileID, ln, cdummy, err)
    TIO_List_States_f = err
    if (TIO_List_States_f /= TIO_SUCCESS_F) return
    
    if (present(nobj)) nobj = ln
    
    if (present(names)) then
      TIO_List_States_f = listnames("TIO_List_States_f", LISTCLASS_STATE,  &
           &                        fileID, TIO_NULL_F, ln, names)
    end if
    
  end function TIO_List_States_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Create_State_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Create_State
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Create_State_f( fileID, name, stateID,
  !                                                            step, time, units )
  ! <DATA>  
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   character(len=*),        intent(in)  :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out) :: stateID
  ! <DATA>   integer(kind=TIO_STEPK), intent(in)  :: step
  ! <DATA>   real(kind=TIO_TIMEK),    intent(in)  :: time
  ! <DATA>   character(len=*),        intent(in)  :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Create_State_f( fileID, name, stateID, step, time, units )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    character(len=*),        intent(in)  :: name
    integer(kind=TIO_OBJK),  intent(out) :: stateID
    integer(kind=TIO_STEPK), intent(in)  :: step
    real(kind=TIO_TIMEK),    intent(in)  :: time
    character(len=*),        intent(in)  :: units
    
    
    integer :: name_len, units_len
    integer(kind=TIO_ERRK) :: err
    
    
    TIO_Create_State_f = setlen("TIO_Create_State_f", "name", name, name_len)
    if (TIO_Create_State_f /= TIO_SUCCESS_F) return
    
    TIO_Create_State_f = setlen("TIO_Create_State_f", "units", units, units_len)
    if (TIO_Create_State_f /= TIO_SUCCESS_F) return
    
    call tio_create_state_c(fileID, name, name_len, stateID, step, time, units, units_len, err)
    
    TIO_Create_State_f = err
    
  end function TIO_Create_State_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_State_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_State
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_State_f( fileID, name, stateID,
  ! <DATA>                                                   step, time, units )
  ! <DATA>   
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   character(len=*),        intent(in)            :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out)           :: stateID
  ! <DATA>   integer(kind=TIO_STEPK), intent(out), optional :: step
  ! <DATA>   real(kind=TIO_TIMEK),    intent(out), optional :: time
  ! <DATA>   character(len=*),        intent(out), optional :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_State_f( fileID, name, stateID, step, time, units )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    character(len=*),        intent(in)            :: name
    integer(kind=TIO_OBJK),  intent(out)           :: stateID
    integer(kind=TIO_STEPK), intent(out), optional :: step
    real(kind=TIO_TIMEK),    intent(out), optional :: time
    character(len=*),        intent(out), optional :: units
    
    integer                     :: name_len
    integer(kind=TIO_STEPK)     :: lstep
    real(kind=TIO_TIMEK)        :: ltime
    character(len=TIO_STRLEN_F) :: lunits = ""
    
    integer(kind=TIO_ERRK) :: err
    
    
    TIO_Open_State_f = setlen("TIO_Open_State_f", "name", name, name_len)
    if (TIO_Open_State_f /= TIO_SUCCESS_F) return
    
    call tio_open_state_c(fileID, -1, name, name_len, stateID,   &
         &                lstep, ltime, lunits, len(lunits), err)
    
    TIO_Open_State_f = err
    if (TIO_Open_State_f /= TIO_SUCCESS_F) return
    
    if (present(step))  step = lstep
    if (present(time))  time = ltime
    if (present(units)) then
      TIO_Open_State_f = setstr("TIO_Open_State_f", "units", lunits, units)
      if (TIO_Open_State_f /= TIO_SUCCESS_F) then
        err = TIO_Close_State_f(fileID, stateID)
        return
      end if
    end if
    
  end function TIO_Open_State_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_State_idx_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_State_idx
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_State_idx_f( fileID, idx, name, stateID,
  ! <DATA>                                                       step, time, units )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: idx
  ! <DATA>   character(len=*),        intent(out)           :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out)           :: stateID
  ! <DATA>   integer(kind=TIO_STEPK), intent(out), optional :: step
  ! <DATA>   real(kind=TIO_TIMEK),    intent(out), optional :: time
  ! <DATA>   character(len=*),        intent(out), optional :: units
  ! <DATA>   
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_State_idx_f(fileID, idx, name, stateID,   &
       &                                               step, time, units)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_SIZEK), intent(in)            :: idx
    character(len=*),        intent(out)           :: name
    integer(kind=TIO_OBJK),  intent(out)           :: stateID
    integer(kind=TIO_STEPK), intent(out), optional :: step
    real(kind=TIO_TIMEK),    intent(out), optional :: time
    character(len=*),        intent(out), optional :: units
    
    character(len=TIO_STRLEN_F) :: lname = ""
    integer(kind=TIO_STEPK)     :: lstep
    real(kind=TIO_TIMEK)        :: ltime
    character(len=TIO_STRLEN_F) :: lunits = ""
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_open_state_c(fileID, idx, lname, len(lname), stateID,   &
         &                lstep, ltime, lunits, len(lunits), err)
    
    TIO_Open_State_idx_f = err
    if (TIO_Open_State_idx_f /= TIO_SUCCESS_F) return
    
    TIO_Open_State_idx_f = setstr("TIO_Open_State_idx_f", "name", lname, name)
    if (TIO_Open_State_idx_f /= TIO_SUCCESS_F) then
      err = TIO_Close_State_f(fileID, stateID)
      return
    end if
    
    if (present(step))  step = lstep
    if (present(time))  time = ltime
    
    if (present(units)) then
      TIO_Open_State_idx_f = setstr("TIO_Open_State_idx_f", "units", lunits, units)
      if (TIO_Open_State_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_State_f(fileID, stateID)
        return
      end if
    end if
    
  end function TIO_Open_State_idx_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_State_Info_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_State_Info
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_State_Info_f( fileID, stateID,
  ! <DATA>                                                       name, step, time, units )
  ! <DATA> 
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: stateID
  ! <DATA>   character(len=*),        intent(out), optional :: name
  ! <DATA>   integer(kind=TIO_STEPK), intent(out), optional :: step
  ! <DATA>   real(kind=TIO_TIMEK),    intent(out), optional :: time
  ! <DATA>   character(len=*),        intent(out), optional :: units
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_State_Info_f(fileID, stateID, name, step, time, units)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: stateID
    character(len=*),        intent(out), optional :: name
    integer(kind=TIO_STEPK), intent(out), optional :: step
    real(kind=TIO_TIMEK),    intent(out), optional :: time
    character(len=*),        intent(out), optional :: units
    
    
    character(len=TIO_STRLEN_F) :: lname="", lunits=""
    integer(kind=TIO_STEPK)     :: lstep
    real(kind=TIO_TIMEK)        :: ltime
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_get_state_info_c(fileID, stateID, lname, len(lname),   &
         &                    lstep, ltime, lunits, len(lunits), err)
    
    TIO_Get_State_Info_f = err
    if (TIO_Get_State_Info_f /= TIO_SUCCESS_F) return
    
    if (present(name)) then
      TIO_Get_State_Info_f = setstr("TIO_Get_State_Info_f", "name", lname, name)
      if (TIO_Get_State_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(step)) step = lstep
    if (present(time)) time = ltime
    
    if (present(units)) then
      TIO_Get_State_Info_f = setstr("TIO_Get_State_Info_f", "units", lunits, units)
      if (TIO_Get_State_Info_f /= TIO_SUCCESS_F) return
    end if
    
  end function TIO_Get_State_Info_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Close_State_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Close_State
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Close_State_f
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: stateID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Close_State_f(fileID, stateID)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: stateID
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_close_state_c(fileID, stateID, err)
    
    TIO_Close_State_f = err
    
  end function TIO_Close_State_f
  
  
  
  ! ********************************************************************************************** !
  
  
end module TIO_State_mod



!
!  EOF

!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!

module TIO_Vargroup_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  use TIO_Utils_mod
    
  implicit none
  
  
  private
  public :: TIO_List_Vargroups_f
  public :: TIO_Create_Vargroup_f, TIO_Open_Vargroup_f, TIO_Open_Vargroup_idx_f
  public :: TIO_Get_Vargroup_Info_f
  public :: TIO_Close_Vargroup_f
  
  
contains
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_List_Vargroups_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_List_Vargroups
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_List_Vargroups_f( fileID, objectID, nobj, names )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: objectID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
  ! <DATA>   character(len=*), dimension(:), intent(out), optional :: names
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_List_Vargroups_f( fileID, objectID, nobj, names )
    
    implicit none
    
    integer(kind=TIO_FILEK),        intent(in)            :: fileID
    integer(kind=TIO_OBJK),         intent(in)            :: objectID
    integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
    character(len=*), dimension(:), intent(out), optional :: names
    
    
    character :: cdummy
    
    integer(kind=TIO_SIZEK) :: ln
    integer(kind=TIO_ERRK)  :: err
    
    
    ln = -1
    call tio_list_vargroups_c(fileID, objectID, ln, cdummy, err)
    TIO_List_Vargroups_f = err
    if (TIO_List_Vargroups_f /= TIO_SUCCESS_F) return
    
    if (present(nobj)) nobj = ln
    
    if (present(names)) then
      TIO_List_Vargroups_f = listnames("TIO_List_Vargroups", LISTCLASS_VGROUP,  &
           &                           fileID, objectID, ln, names)
    end if
    
  end function TIO_List_Vargroups_f  
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Create_Vargroup_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Create_Vargroup
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Create_Vargroup_f( fileID, objectID, name, vgroupID )
  ! <DATA>   
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: objectID
  ! <DATA>   character(len=*),        intent(in)  :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out) :: vgroupID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Create_Vargroup_f( fileID, objectID, name, vgroupID )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: objectID
    character(len=*),        intent(in)  :: name
    integer(kind=TIO_OBJK),  intent(out) :: vgroupID
    
    
    integer :: name_len
    integer(kind=TIO_ERRK) :: err
    
    
    TIO_Create_Vargroup_f = setlen("TIO_Create_Vargroup_f", "name", name, name_len)
    if (TIO_Create_Vargroup_f /= TIO_SUCCESS_F) return
    
    call tio_create_vargroup_c( fileID, objectID, name, name_len, vgroupID, err)
    
    TIO_Create_Vargroup_f = err
    
  end function TIO_Create_Vargroup_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Vargroup_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Vargroup
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Vargroup_f( fileID, objectID, name, vgroupID )
  ! <DATA>   
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: objectID
  ! <DATA>   character(len=*),        intent(in)  :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out) :: vgroupID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Vargroup_f( fileID, objectID, name, vgroupID )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: objectID
    character(len=*),        intent(in)  :: name
    integer(kind=TIO_OBJK),  intent(out) :: vgroupID
    
    integer :: name_len
    integer(kind=TIO_ERRK) :: err
    
    
    TIO_Open_Vargroup_f = setlen("TIO_Open_Vargroup_f", "name", name, name_len)
    if (TIO_Open_Vargroup_f /= TIO_SUCCESS_F) return
    
    call tio_open_vargroup_c( fileID, objectID, -1, name, name_len, vgroupID, err )
    
    TIO_Open_Vargroup_f = err
    
  end function TIO_Open_Vargroup_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Vargroup_idx_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Vargroup_idx
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Vargroup_idx_f( fileID, objectID, idx, name, vgroupID )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: objectID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: idx
  ! <DATA>   character(len=*),        intent(out) :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out) :: vgroupID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Vargroup_idx_f( fileID, objectID, idx, name, vgroupID )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: objectID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    character(len=*),        intent(out) :: name
    integer(kind=TIO_OBJK),  intent(out) :: vgroupID
    
    character(len=TIO_STRLEN_F) :: lname = ""
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_open_vargroup_c( fileID, objectID, idx, lname, len(lname), vgroupID, err )
    
    TIO_Open_Vargroup_idx_f = err
    if (TIO_Open_Vargroup_idx_f /= TIO_SUCCESS_F) return
    
    TIO_Open_Vargroup_idx_f = setstr("TIO_Open_Vargroup_idx_f", "name", lname, name)
    if (TIO_Open_Vargroup_idx_f /= TIO_SUCCESS_F) then
      err = TIO_Close_Vargroup_f(fileID, vgroupID)
      return
    end if
    
  end function TIO_Open_Vargroup_idx_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Vargroup_Info_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Vargroup_Info
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Vargroup_Info_f( fileID, vgroupID, name )
  ! <DATA>   
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: vgroupID
  ! <DATA>   character(len=*),        intent(out), optional :: name
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_Vargroup_Info_f( fileID, vgroupID, name )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: vgroupID
    character(len=*),        intent(out), optional :: name
    
    
    character(len=TIO_STRLEN_F) :: lname = ""
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_get_vargroup_info_c(fileID, vgroupID, lname, len(lname), err)
    
    TIO_Get_Vargroup_Info_f = err
    if (TIO_Get_Vargroup_Info_f /= TIO_SUCCESS_F) return
    
    if (present(name)) then
      TIO_Get_Vargroup_Info_f = setstr("TIO_Get_Vargroup_Info_f", "name", lname, name)
      if (TIO_Get_Vargroup_Info_f /= TIO_SUCCESS_F) return
    end if
    
  end function TIO_Get_Vargroup_Info_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Close_Vargroup_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Close_Vargroup
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Close_Vargroup_f( fileID, vgroupID )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: vgroupID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Close_Vargroup_f( fileID, vgroupID )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: vgroupID
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_close_vargroup_c( fileID, vgroupID, err )
    
    TIO_Close_Vargroup_f = err
    
  end function TIO_Close_Vargroup_f
  
  
  ! ********************************************************************************************** !
  
  
end module TIO_Vargroup_mod



!
!  EOF

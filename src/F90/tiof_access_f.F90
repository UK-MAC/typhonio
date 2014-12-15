!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!

module TIO_Access_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  use TIO_Utils_mod
  
  
  implicit none
  
  
  private
  public :: TIO_Get_LibVersion_f, TIO_Get_FileVersion_f
  public :: TIO_CheckFile_f, TIO_CheckObject_f
  public :: TIO_IsFileOpen_f, TIO_IsObjectOpen_f
  public :: TIO_Create_f, TIO_Open_f, TIO_Get_File_Info_f, TIO_Close_f, TIO_Flush_f
  
  
  integer, parameter :: MAX_FILENAME_LEN = 512
  
  
contains
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_LibVersion_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_LibVersion
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_LibVersion_f( maj, min, rel )
  ! <DATA>
  ! <DATA>   integer, intent(out) :: maj, min, rel
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_LibVersion_f( maj, min, rel )
    
    implicit none
    
    integer, intent(out) :: maj, min, rel
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_get_libversion_c( maj, min, rel, err )
    
    TIO_Get_LibVersion_f = err
    
  end function TIO_Get_LibVersion_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_FileVersion_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_FileVersion
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_FileVersion_f( filename, maj, min, rel )
  ! <DATA>
  ! <DATA>   character(len=*), intent(in)  :: filename
  ! <DATA>   integer,          intent(out) :: maj, min, rel
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_FileVersion_f( filename, maj, min, rel )
    
    implicit none
    
    character(len=*), intent(in)  :: filename
    integer,          intent(out) :: maj, min, rel
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_get_fileversion_c( filename, len_trim(filename),  &
         &                      maj, min, rel, err             )
    
    TIO_Get_FileVersion_f = err
    
  end function TIO_Get_FileVersion_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_CheckFile_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_CheckFile
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_CheckFile_f( filename )
  ! <DATA>
  ! <DATA>   character(len=*), intent(in) :: filename
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_CheckFile_f( filename )
    
    implicit none
    
    character(len=*), intent(in) :: filename
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_checkfile_c( filename, len_trim(filename), err )
    
    TIO_CheckFile_f = err
    
  end function TIO_CheckFile_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_CheckObject_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_CheckObject
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_CheckObject_f( fileID, parentID, name )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: parentID
  ! <DATA>   character(len=*),        intent(in) :: name
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_CheckObject_f( fileID, parentID, name )
    
    implicit none
    
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: parentID
    character(len=*),        intent(in) :: name
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_checkobject_c( fileID, parentID, name, len_trim(name), err )
    
    TIO_CheckObject_f = err
    
  end function TIO_CheckObject_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_IsFileOpen_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_IsFileOpen
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_IsFileOpen_f( filename )
  ! <DATA>
  ! <DATA>   character(len=*), intent(in) :: filename
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_IsFileOpen_f( filename )
    
    implicit none
    
    character(len=*), intent(in) :: filename
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_isfileopen_c( filename, len_trim(filename), err )
    
    TIO_IsFileOpen_f = err
    
  end function TIO_IsFileOpen_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_IsObjectOpen_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_IsObjectOpen
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_IsObjectOpen_f( fileID, parentID, name )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: parentID
  ! <DATA>   character(len=*),        intent(in) :: name
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_IsObjectOpen_f( fileID, parentID, name )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: parentID
    character(len=*),        intent(in) :: name
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_isobjectopen_c( fileID, parentID, name, len_trim(name), err )
    
    TIO_IsObjectOpen_f = err
    
  end function TIO_IsObjectOpen_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Create_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Create
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Create_f( filename, fileID, access,
  ! <DATA>                                               codename, version, date, title,
  ! <DATA>                                               comm, info, rank )
  ! <DATA>  
  ! <DATA>   character(len=*),        intent(in)           :: filename
  ! <DATA>   integer(kind=TIO_FILEK), intent(out)          :: fileID
  ! <DATA>   integer,                 intent(in)           :: access
  ! <DATA>   character(len=*),        intent(in)           :: codename
  ! <DATA>   character(len=*),        intent(in)           :: version
  ! <DATA>   character(len=*),        intent(in)           :: date
  ! <DATA>   character(len=*),        intent(in)           :: title
  ! <DATA>   integer,                 intent(in), optional :: comm, info, rank
  ! </FAPI>
  ! <FDES>
  ! <DATA> When using the parallel TyphonIO library, <code>comm</code>, <code>info</code>
  ! <DATA> and <code>rank</code> are not optional
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !

  integer(kind=TIO_ERRK) function TIO_Create_f( filename, fileID, access,         &
       &                                        codename, version, date, title,   &
       &                                        comm, info, rank )
    
    implicit none
    
    character(len=*),        intent(in)           :: filename
    integer(kind=TIO_FILEK), intent(out)          :: fileID
    integer,                 intent(in)           :: access
    character(len=*),        intent(in)           :: codename
    character(len=*),        intent(in)           :: version
    character(len=*),        intent(in)           :: date
    character(len=*),        intent(in)           :: title
    
#ifdef  _TYPHIO_PARALLEL
    integer,                 intent(in)           :: comm, info, rank
#else
    integer,                 intent(in), optional :: comm, info, rank
#endif
    
    
    integer                :: f_len, c_len, v_len, d_len, t_len
    integer                :: l_comm, l_info, l_rank
    integer(kind=TIO_ERRK) :: err
    
    
    
    f_len = len_trim(filename)
    
    TIO_Create_f = setlen("TIO_Create_f", "codename", codename, c_len)
    if (TIO_Create_f == TIO_ERR_F90_API_F) return
    
    TIO_Create_f = setlen("TIO_Create_f", "version", version, v_len)
    if (TIO_Create_f == TIO_ERR_F90_API_F) return
    
    TIO_Create_f = setlen("TIO_Create_f", "date", date, d_len)
    if (TIO_Create_f == TIO_ERR_F90_API_F) return
    
    TIO_Create_f = setlen("TIO_Create_f", "title", title, t_len)
    if (TIO_Create_f == TIO_ERR_F90_API_F) return
    

#ifdef _TYPHIO_PARALLEL
    
    l_comm = comm
    l_info = info
    l_rank = rank
    
#else
    
    if (present(comm)) then
      l_comm = comm
    else
      l_comm = TIO_NULL_F
    end if
    
    if (present(info)) then
      l_info = info
    else
      l_info = TIO_NULL_F
    end if
    
    if (present(rank)) then
      l_rank = rank
    else
      l_rank = 0
    end if
    
#endif
    
    
    call tio_create_c( filename, f_len,         &
         &             fileID,                  &
         &             access,                  &
         &             codename, c_len,         &
         &             version,  v_len,         &
         &             date,     d_len,         &
         &             title,    t_len,         &
         &             l_comm, l_info, l_rank,  &
         &             err                      )
    
    
    TIO_Create_f = err
    
  end function TIO_Create_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_f( filename, fileID, access,
  ! <DATA>                                             codename, version, date, title,
  ! <DATA>                                             comm, info, rank )
  ! <DATA>  
  ! <DATA>   character(len=*),        intent(in)           :: filename
  ! <DATA>   integer(kind=TIO_FILEK), intent(out)          :: fileID
  ! <DATA>   integer,                 intent(in)           :: access
  ! <DATA>   character(len=*),        intent(inout)        :: codename
  ! <DATA>   character(len=*),        intent(inout)        :: version
  ! <DATA>   character(len=*),        intent(inout)        :: date
  ! <DATA>   character(len=*),        intent(inout)        :: title
  ! <DATA>   integer,                 intent(in), optional :: comm, info, rank
  ! </FAPI>
  ! <FDES>
  ! <DATA> When using the parallel TyphonIO library, <code>comm</code>, <code>info</code>
  ! <DATA> and <code>rank</code> are not optional
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_f( filename, fileID, access,         &
       &                                      codename, version, date, title,   &
       &                                      comm, info, rank )
    
    implicit none
    
    character(len=*),        intent(in)           :: filename
    integer(kind=TIO_FILEK), intent(out)          :: fileID
    integer,                 intent(in)           :: access
    character(len=*),        intent(inout)        :: codename
    character(len=*),        intent(inout)        :: version
    character(len=*),        intent(inout)        :: date
    character(len=*),        intent(inout)        :: title
#ifdef  _TYPHIO_PARALLEL
    integer,                 intent(in)           :: comm, info, rank
#else
    integer,                 intent(in), optional :: comm, info, rank
#endif
    
    
    character(len=TIO_STRLEN_F) :: lc, lv, ld, lt
    
    integer                :: f_len, c_len, v_len, d_len, t_len
    integer                :: l_comm, l_info, l_rank
    integer(kind=TIO_ERRK) :: err
    
    
    
    f_len = len_trim(filename)
    
#ifdef _TYPHIO_PARALLEL
    
    l_comm = comm
    l_info = info
    l_rank = rank
    
#else
    
    if (present(comm)) then
      l_comm = comm
    else
      l_comm = TIO_NULL_F
    end if
    
    if (present(info)) then
      l_info = info
    else
      l_info = TIO_NULL_F
    end if
    
    if (present(rank)) then
      l_rank = rank
    else
      l_rank = 0
    end if

#endif
    
    
    select case (access)
      
    case (TIO_ACC_READWRITE_F)
      
      ! Use the file attributes to modify the file.  Use them, don't return them.
      TIO_Open_f = setlen("TIO_Open_f", "codename", codename, c_len)
      if (TIO_Open_f == TIO_ERR_F90_API_F) return
      
      TIO_Open_f = setlen("TIO_Open_f", "version", version, v_len)
      if (TIO_Open_f == TIO_ERR_F90_API_F) return
      
      TIO_Open_f = setlen("TIO_Open_f", "date", date, d_len)
      if (TIO_Open_f == TIO_ERR_F90_API_F) return
      
      TIO_Open_f = setlen("TIO_Open_f", "title", title, t_len)
      if (TIO_Open_f == TIO_ERR_F90_API_F) return
      
      
      call tio_open_c( filename, f_len,         &
           &           fileID,                  &
           &           access,                  &
           &           codename, c_len,         &
           &           version,  v_len,         &
           &           date,     d_len,         &
           &           title,    t_len,         &
           &           l_comm, l_info, l_rank,  &
           &           err                      )
      
      TIO_Open_f = err
      
    case (TIO_ACC_READWRITESTRICT_F)
     
      c_len = -1
      lc = ""
      lv = ""
      ld = ""
      lt = ""
      
      call tio_open_c( filename, f_len,         &
           &           fileID,                  &
           &           access,                  &
           &           lc, c_len,               &
           &           lv, c_len,               &
           &           ld, c_len,               &
           &           lt, c_len,               &
           &           l_comm, l_info, l_rank,  &
           &           err                      )

      TIO_Open_f = err 

    case (TIO_ACC_READONLY_F)
      
      ! Return the file attributes as they were stored.
      
      c_len = -1
      lc = ""
      lv = ""
      ld = ""
      lt = ""
      
      call tio_open_c( filename, f_len,         &
           &           fileID,                  &
           &           access,                  &
           &           lc, c_len,               &
           &           lv, c_len,               &
           &           ld, c_len,               &
           &           lt, c_len,               &
           &           l_comm, l_info, l_rank,  &
           &           err                      )
      
      TIO_Open_f = err
      if (TIO_Open_f /= TIO_SUCCESS_F) return
      
      
      TIO_Open_f = setstr("TIO_Open_f", "codename", lc, codename)
      if (TIO_Open_f /= TIO_SUCCESS_F) then
        err = TIO_Close_f(fileID)
        return
      end if
      
      TIO_Open_f = setstr("TIO_Open_f", "version", lv, version)
      if (TIO_Open_f /= TIO_SUCCESS_F) then
        err = TIO_Close_f(fileID)
        return
      end if
      
      TIO_Open_f = setstr("TIO_Open_f", "date", ld, date)
      if (TIO_Open_f /= TIO_SUCCESS_F) then
        err = TIO_Close_f(fileID)
        return
      end if
      
      TIO_Open_f = setstr("TIO_Open_f", "title", lt, title)
      if (TIO_Open_f /= TIO_SUCCESS_F) then
        err = TIO_Close_f(fileID)
        return
      end if
      
      
    case default
      
      ! C will pick up invalid access value.
      
      c_len = TIO_STRLEN_F
      lc = ""
      lv = ""
      ld = ""
      lt = ""
      
      call tio_open_c( filename, f_len,         &
           &           fileID,                  &
           &           access,                  &
           &           lc, c_len,               &
           &           lv, c_len,               &
           &           ld, c_len,               &
           &           lt, c_len,               &
           &           l_comm, l_info, l_rank,  &
           &           err                      )
      
      TIO_Open_f = err
      
    end select
    
  end function TIO_Open_f
  
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Close_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Close
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Close_f( fileID )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Close_f(fileID)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_close_c( fileID, err )
    
    TIO_Close_f = err
    
  end function TIO_Close_f
  
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Flush_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Flush
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Flush_f( fileID )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Flush_f( fileID )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_flush_c( fileID, err )
    
    TIO_Flush_f = err
    
  end function TIO_Flush_f
  
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_File_Info_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_File_Info
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_File_Info_f( fileID, filename,
  ! <DATA>                                                      codename, version, date, title )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   character(len=*),        intent(out), optional :: filename
  ! <DATA>   character(len=*),        intent(out), optional :: codename
  ! <DATA>   character(len=*),        intent(out), optional :: version
  ! <DATA>   character(len=*),        intent(out), optional :: date
  ! <DATA>   character(len=*),        intent(out), optional :: title
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_File_Info_f( fileID, filename,   &
       &                                               codename, version, date, title )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    character(len=*),        intent(out), optional :: filename
    character(len=*),        intent(out), optional :: codename
    character(len=*),        intent(out), optional :: version
    character(len=*),        intent(out), optional :: date
    character(len=*),        intent(out), optional :: title
    
    
    character(len=1024)         :: lfilename=""
    character(len=TIO_STRLEN_F) :: lcodename="", lversion="", ldate="", ltitle=""
    integer                     :: flen, llen
    integer(kind=TIO_ERRK)      :: err
    
    
    
    flen = 1024
    llen = TIO_STRLEN_F
    
    call tio_get_file_info_c( fileID,            &
         &                    lfilename, flen,   &
         &                    lcodename, llen,   &
         &                    lversion,  llen,   &
         &                    ldate,     llen,   &
         &                    ltitle,    llen,   &
         &                    err                )
    
    TIO_Get_File_Info_f = err
    if (TIO_Get_File_Info_f /= TIO_SUCCESS_F) return
    
    
    
    if (present(filename)) then
      TIO_Get_File_Info_f = setstr("TIO_Get_File_Info_f", "filename", lfilename, filename)
      if (TIO_Get_File_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(codename)) then
      TIO_Get_File_Info_f = setstr("TIO_Get_File_Info_f", "codename", lcodename, codename)
      if (TIO_Get_File_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(version)) then
      TIO_Get_File_Info_f = setstr("TIO_Get_File_Info_f", "version", lversion, version)
      if (TIO_Get_File_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(date)) then
      TIO_Get_File_Info_f = setstr("TIO_Get_File_Info_f", "date", ldate, date)
      if (TIO_Get_File_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(title)) then
      TIO_Get_File_Info_f = setstr("TIO_Get_File_Info_f", "title", ltitle, title)
      if (TIO_Get_File_Info_f /= TIO_SUCCESS_F) return
    end if
    
  end function TIO_Get_File_Info_f
  
  
  
  ! ********************************************************************************************** !
  
  
end module TIO_Access_mod



!
!  EOF

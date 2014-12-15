!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!

#define TIO_CHECK( X ) ierr = X; if (ierr .ne. TIO_SUCCESS_F) print *, 'Error:', __line__, ierr

program main
  use typhonio
  implicit none

  integer :: ierr
  integer :: i
  character(len=*), parameter :: file_name = "../tio_files/tio_vargroup.h5"
  integer(kind=TIO_FILEK) :: file_id
  integer(kind=TIO_OBJK) :: state_id
  integer(kind=TIO_OBJK) :: vargroup_id1, vargroup_id2, vargroup_id3
  real(kind=8), dimension(10,10,10) :: vdata

  TIO_CHECK( TIO_Create_f(file_name, file_id, TIO_ACC_REPLACE_F, "TestCode", "V0.0", "01-01-2000", "Test Title") )

  TIO_CHECK( TIO_Create_State_f( file_id, "State1", state_id, 1, 0.0_TIO_TIMEK, "ms" ) )

  TIO_CHECK( TIO_Create_Vargroup_f(file_id, state_id, "VarGroup1", vargroup_id1) )
  TIO_CHECK( TIO_Create_Vargroup_f(file_id, state_id, "VarGroup2", vargroup_id2) )
  TIO_CHECK( TIO_Create_Vargroup_f(file_id, state_id, "VarGroup3", vargroup_id3) )

  TIO_CHECK( TIO_Close_f(file_id) )

end program main

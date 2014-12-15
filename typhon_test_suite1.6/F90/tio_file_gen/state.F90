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
  integer :: i, j
  character(len=*), parameter :: file_name = "../tio_files/tio_state.h5"
  integer(kind=TIO_FILEK) :: file_id
  integer(kind=TIO_OBJK) :: state_id1, state_id2, state_id3

  TIO_CHECK( TIO_Create_f(file_name, file_id, TIO_ACC_REPLACE_F, "TestCode", "V0.0", "01-01-2000", "Test Title") )

  TIO_CHECK( TIO_Create_State_f( file_id, "State1", state_id1, 1, 0.0_TIO_TIMEK, "ms" ) )
  TIO_CHECK( TIO_Create_State_f( file_id, "State2", state_id2, 2, 1.0_TIO_TIMEK, "ms" ) )
  TIO_CHECK( TIO_Create_State_f( file_id, "State3", state_id3, 3, 2.0_TIO_TIMEK, "ms" ) )

  TIO_CHECK( TIO_Close_f(file_id) )

end program main

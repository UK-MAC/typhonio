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
  character(len=*), parameter :: file_name = "../tio_files/tio_chunk.h5"
  integer(kind=TIO_FILEK) :: file_id
  integer(kind=TIO_OBJK) :: state_id
  integer(kind=TIO_OBJK) :: mesh_id, quadmesh_id, unstrmesh_id, pointmesh_id
  integer(kind=TIO_OBJK) :: arrayq_id1, arrayq_id2, arrayq_id3
  real(kind=8), dimension(3,16,2,3) :: aqdat
  integer, dimension(1) :: dims1
  integer, dimension(3) :: dims2

  TIO_CHECK( TIO_Create_f(file_name, file_id, TIO_ACC_REPLACE_F, "TestCode", "V0.0", "01-01-2000", "Test Title") )

  TIO_CHECK( TIO_Create_State_f( file_id, "State1", state_id, 1, 0.0_TIO_TIMEK, "seconds" ) )
  
  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "Mesh", mesh_id,                          &
       &                        TIO_MESH_POINT_F, TIO_COORD_CARTESIAN_F, .false., "Group1", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_2D_F, 100, 0, 0, 0, 4,                &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "QuadMesh", quadmesh_id,                              &
       &                        TIO_MESH_QUAD_COLINEAR_F, TIO_COORD_CARTESIAN_F, .false., "Group2", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 100, 100, 100, 0, 1,               &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "UnstrMesh", unstrmesh_id,                        &
       &                        TIO_MESH_UNSTRUCT_F, TIO_COORD_CARTESIAN_F, .false., "Group4", 1,  &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 1331, 1000, 1, 8000, 1,     &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "PointMesh", pointmesh_id,                        &
       &                        TIO_MESH_POINT_F, TIO_COORD_CARTESIAN_F, .false., "Group4", 1,  &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 1000, 0, 0, 0, 1,     &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Set_Quad_Chunk_f(file_id, quadmesh_id, 1, TIO_3D_F, -10, 10, -20, 20, -30, 30, 5, 0) )
  TIO_CHECK( TIO_Set_Unstr_Chunk_f(file_id, unstrmesh_id, 1, TIO_3D_F, 36, 25, 1, 4, 1, 1, 1, 4, 2, 0) )
  TIO_CHECK( TIO_Set_Point_Chunk_f(file_id, pointmesh_id, 1, TIO_3D_F, 0, 10, 2) )

  TIO_CHECK( TIO_Close_f(file_id) )

end program main

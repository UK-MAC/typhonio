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
  character(len=*), parameter :: file_name = "../tio_files/tio_mesh.h5"
  integer(kind=TIO_FILEK) :: file_id
  integer(kind=TIO_OBJK) :: state_id
  integer(kind=TIO_OBJK) :: material_id
  integer(kind=TIO_OBJK) :: quadcomesh_id, quadnoncomesh_id, unstrmesh_id, pointmesh_id
  real(kind=8), dimension(100) :: idat, jdat, kdat
  integer(kind=TIO_SHAPEK), dimension(1) :: shapes
  integer(kind=4), dimension(9) :: nodeIDs
  integer(kind=4), dimension(4) :: cellIDs
  integer(kind=4), dimension(16) :: connectivity
  real(kind=8), dimension(9) :: icoords, jcoords

  TIO_CHECK( TIO_Create_f(file_name, file_id, TIO_ACC_REPLACE_F, "TestCode", "V0.0", "01-01-2000", "Test Title") )

  TIO_CHECK( TIO_Create_State_f( file_id, "State1", state_id, 1, 0.0_TIO_TIMEK, "seconds" ) )
 
!======================================================================================================================
! Quad Colinear

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "QuadCoMesh", quadcomesh_id,                        &
       &                        TIO_MESH_QUAD_COLINEAR_F, TIO_COORD_CARTESIAN_F, .false., "Group1", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 100, 100, 100, 0, 1,            &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Set_Mesh_Range_f( file_id, quadcomesh_id, TIO_REAL8_F,  TIO_3D_F, &
       &                           -10._8, 10._8, -20._8, 20._8, -30._8, 30._8 ) )

  TIO_CHECK( TIO_Set_Quad_Chunk_f(file_id, quadcomesh_id, 1, TIO_3D_F, 1, 100, 1, 100, 1, 100, 0, 0) )

  idat(:) = 1._8
  jdat(:) = 2._8
  kdat(:) = 3._8
  TIO_CHECK( TIO_Write_QuadMesh_All_f(file_id, quadcomesh_id, TIO_REAL8_F, idat, jdat, kdat ) )

!======================================================================================================================
! Quad NonColinear

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "QuadNonCoMesh", quadnoncomesh_id,                     &
       &                        TIO_MESH_QUAD_NONCOLINEAR_F, TIO_COORD_CARTESIAN_F, .false., "Group2", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 100, 100, 100, 2, 1,               &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )
       
!======================================================================================================================
! Unstructured

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "UnstrMesh", unstrmesh_id,                     &
       &                        TIO_MESH_UNSTRUCT_F, TIO_COORD_CARTESIAN_F, .false., "Group3", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_2D_F, 9, 4, 1, 16, 1,    &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Set_Unstr_Chunk_f(file_id, unstrmesh_id, 1, TIO_2D_F, 9, 4, 1, 16, 0, 0, 0, 0, 0, 0) )

  nodeIDs = (/ 1, 2, 3, 4, 5, 6, 7, 8, 9 /)
  cellIDs = (/ 1, 2, 3, 4 /)
  shapes  = (/ TIO_SHAPE_QUAD4_F /)
  connectivity(:) = 1
  icoords(:) = 1._8
  jcoords(:) = 1._8

  TIO_CHECK( TIO_Write_UnstrMesh_Chunk_f(file_id, unstrmesh_id, 1, TIO_XFER_COLLECTIVE_F, TIO_INTEGER4_F, &
       &                                TIO_REAL8_F, nodeIDs, cellIDs, shapes, (/ 4 /), connectivity, &
       &                                icoords, jcoords) )

!======================================================================================================================
! Point

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "PointMesh", pointmesh_id,                  &
       &                        TIO_MESH_POINT_F, TIO_COORD_CARTESIAN_F, .false., "Group4", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 100, 0, 0, 0, 1,      &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Set_Point_Chunk_f(file_id, pointmesh_id, 1, TIO_3D_F, 1, 100, 0) )
  
  idat(:) = 1._8
  jdat(:) = 2._8
  kdat(:) = 3._8

  TIO_CHECK( TIO_Write_PointMesh_Chunk_f(file_id, pointmesh_id, 1, TIO_XFER_COLLECTIVE_F, TIO_REAL8_F, idat, jdat, kdat) )

!======================================================================================================================

  TIO_CHECK( TIO_Close_f(file_id) )

end program main

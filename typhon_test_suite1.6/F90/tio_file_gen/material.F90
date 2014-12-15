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
  character(len=*), parameter :: file_name = "../tio_files/tio_material.h5"
  integer(kind=TIO_FILEK) :: file_id
  integer(kind=TIO_OBJK) :: state_id
  integer(kind=TIO_OBJK) :: material_id1, material_id2, material_id3
  integer(kind=TIO_OBJK) :: quadmesh_id, unstrmesh_id, pointmesh_id
  integer, dimension(1) :: dims1
  integer, dimension(3) :: dims2
  integer(kind=4), dimension(9,9) :: mat
  integer(kind=4), dimension(1) :: ncomp
  integer(kind=4), dimension(1) :: ptr
  integer(kind=4), dimension(2) :: mixmat
  real(kind=8), dimension(2)    :: vf
  integer(kind=TIO_SHAPEK), dimension(1) :: shapes
  integer(kind=4), dimension(1331) :: nodeIDs
  integer(kind=4), dimension(1000) :: cellIDs
  integer(kind=4), dimension(8000) :: connectivity
  real(kind=8), dimension(1331) :: icoords, jcoords
  integer :: i

  TIO_CHECK( TIO_Create_f(file_name, file_id, TIO_ACC_REPLACE_F, "TestCode", "V0.0", "01-01-2000", "Test Title") )

  TIO_CHECK( TIO_Create_State_f( file_id, "State1", state_id, 1, 0.0_TIO_TIMEK, "seconds" ) )
 
!======================================================================================================================
! QUAD MESH

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "QuadMesh", quadmesh_id,                                    &
       &                        TIO_MESH_QUAD_COLINEAR_F, TIO_COORD_CARTESIAN_F, .false., "Group1", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_2D_F, 10, 10, 0, 0, 1,                &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Set_Quad_Chunk_f(file_id, quadmesh_id, 1, TIO_2D_F, 1, 10, 1, 10, 0, 0, 1, 2) )

  TIO_CHECK( TIO_Create_Mat_f(file_id, quadmesh_id, "Material", material_id1, &
       &                      TIO_INTEGER4_F, 3, TIO_GHOSTS_NONE_F, .true.,  &
       &                      TIO_INTEGER4_F, TIO_INTEGER4_F, TIO_REAL8_F) )

  TIO_CHECK( TIO_Set_Mat_Range_f(file_id, material_id1, TIO_INTEGER4_F, 3, (/ 1, 2, 3 /), (/ "Mat1", "Mat2", "Mat3" /)) )

  mat(1:3,:) = 1; mat(4:6,:) = 2; mat(7,:) = -1; mat(8:9,:) = 3
  ncomp  = (/ 2 /)
  ptr    = (/ 1 /)
  mixmat = (/ 2, 3 /)
  vf     = (/ 0.4, 0.6 /)

  TIO_CHECK( TIO_Write_QuadMat_Chunk_f(file_id, material_id1, 1, TIO_XFER_COLLECTIVE_F, &
       &                               TIO_INTEGER4_F, mat, TIO_INTEGER4_F,            &
       &                               TIO_INTEGER4_F, TIO_REAL8_F, ncomp, ptr, mixmat, vf) )

!======================================================================================================================
! UNSTRUCTURED MESH

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "UnstrMesh", unstrmesh_id,                       &
       &                        TIO_MESH_UNSTRUCT_F, TIO_COORD_CARTESIAN_F, .false., "Group4", 1,  &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_2D_F, 9, 4, 1, 16, 1,             &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )
  
  
  TIO_CHECK( TIO_Set_Unstr_Chunk_f(file_id, unstrmesh_id, 1, TIO_2D_F, 9, 4, 1, 16, 0, 0, 0, 0, 1, 2) )
  
  TIO_CHECK( TIO_Create_Mat_f(file_id, unstrmesh_id, "Material", material_id2, &
       &                      TIO_INTEGER4_F, 3, 0, .true.,              &
       &                      TIO_INTEGER4_F, TIO_INTEGER4_F, TIO_REAL8_F) )

  nodeIDs = (/ (i,i=1,1331) /)
  cellIDs = (/ (i,i=1,1000) /)
  shapes  = (/ TIO_SHAPE_QUAD4_F /)
  connectivity(:) = 1
  icoords(:) = 1._8
  jcoords(:) = 1._8

  TIO_CHECK( TIO_Write_UnstrMesh_Chunk_f(file_id, unstrmesh_id, 1, TIO_XFER_COLLECTIVE_F, TIO_INTEGER4_F, &
       &                                TIO_REAL8_F, nodeIDs, cellIDs, shapes, (/ 1000 /), connectivity, &
       &                                icoords, jcoords) )

  ncomp  = (/ 2 /)
  ptr    = (/ 1 /)
  mixmat = (/ 2, 3 /)
  vf     = (/ 0.4, 0.6 /)
  
  TIO_CHECK( TIO_Write_UnstrMat_Chunk_f(file_id, material_id2, 1, TIO_XFER_COLLECTIVE_F, &
       &                               TIO_INTEGER4_F, (/ 1, 2, -1, 3 /), TIO_INTEGER4_F,            &
       &                               TIO_INTEGER4_F, TIO_REAL8_F, ncomp, ptr, mixmat, vf) )

!======================================================================================================================
! POINT MESH

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "PointMesh", pointmesh_id,                           &
       &                        TIO_MESH_POINT_F, TIO_COORD_CARTESIAN_F, .false., "Group4", 1,  &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 5, 0, 0, 0, 1,             &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )
  
  TIO_CHECK( TIO_Set_Point_Chunk_f(file_id, pointmesh_id, 1, TIO_3D_F, 1, 5, 0) )
  
  TIO_CHECK( TIO_Create_Mat_f(file_id, pointmesh_id, "Material", material_id3, &
       &                      TIO_INTEGER4_F, 3, 0, .false.) )

  TIO_CHECK( TIO_Write_PointMat_Chunk_f(file_id, material_id3, 1, TIO_XFER_COLLECTIVE_F, &
       &                               TIO_INTEGER4_F, (/ 1, 2, 2, 3, 3 /)) )

!======================================================================================================================

  TIO_CHECK( TIO_Close_f(file_id) )

end program main

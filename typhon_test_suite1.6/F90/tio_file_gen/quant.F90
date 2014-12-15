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
  character(len=*), parameter :: file_name = "../tio_files/tio_quant.h5"
  integer(kind=TIO_FILEK) :: file_id
  integer(kind=TIO_OBJK) :: state_id
  integer(kind=TIO_OBJK) :: material_id
  integer(kind=TIO_OBJK) :: quadcomesh_id, unstrmesh_id, pointmesh_id
  integer(kind=TIO_OBJK) :: quant_id1, quant_id2, quant_id3, quant_id4, quant_id5
  real(kind=8), dimension(10,10,10) :: qdat
  real(kind=8), dimension(9) :: qnode
  real(kind=8), dimension(100) :: qpoint
  integer(kind=TIO_SHAPEK), dimension(1) :: shapes
  integer(kind=4), dimension(9) :: nodeIDs
  integer(kind=4), dimension(4) :: cellIDs
  integer(kind=4), dimension(16) :: connectivity
  real(kind=8), dimension(9) :: icoords, jcoords

  TIO_CHECK( TIO_Create_f(file_name, file_id, TIO_ACC_REPLACE_F, "TestCode", "V0.0", "01-01-2000", "Test Title") )

  TIO_CHECK( TIO_Create_State_f( file_id, "State1", state_id, 1, 0.0_TIO_TIMEK, "seconds" ) )
 
!======================================================================================================================
! QUAD MESH

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "QuadMesh", quadcomesh_id,                        &
       &                        TIO_MESH_QUAD_COLINEAR_F, TIO_COORD_CARTESIAN_F, .false., "Group1", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 10, 10, 10, 0, 1,            &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Set_Quad_Chunk_f(file_id, quadcomesh_id, 1, TIO_3D_F, 1, 10, 1, 10, 1, 10, 0, 0) )

  TIO_CHECK( TIO_Create_Quant_f(file_id, quadcomesh_id, "Quant1", quant_id1, TIO_REAL8_F, &
                  TIO_CENTRE_NODE_F, 0, .false., "node") )

  TIO_CHECK( TIO_Create_Quant_f(file_id, quadcomesh_id, "Quant2", quant_id2, TIO_REAL4_F, &
                  TIO_CENTRE_CELL_F, 0, .false., "node") )

  TIO_CHECK( TIO_Create_Quant_f(file_id, quadcomesh_id, "Quant3", quant_id3, TIO_INTEGER4_F, &
                  TIO_CENTRE_CELL_F, 0, .false., "node") )

  TIO_CHECK( TIO_Set_Quant_Range_f(file_id, quant_id1, TIO_REAL8_F, 1._8, 1000._8) )
  TIO_CHECK( TIO_Set_Quant_Range_f(file_id, quant_id2, TIO_REAL4_F, -1._4, 1._4) )
  TIO_CHECK( TIO_Set_Quant_Range_f(file_id, quant_id3, TIO_INTEGER4_F, 0_4, 100_4) )

  qdat(:,:,:) = reshape( (/ (i,i=1,1000) /), (/ 10, 10, 10 /) )

  TIO_CHECK( TIO_Write_QuadQuant_Chunk_f( file_id, quant_id1, 1, TIO_XFER_COLLECTIVE_F, TIO_REAL8_F, qdat ) )

!======================================================================================================================
! UNSTRUCTURED MESH

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "UnstrMesh", unstrmesh_id,                          &
       &                        TIO_MESH_UNSTRUCT_F, TIO_COORD_CARTESIAN_F, .false., "Group1", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_2D_F, 9, 4, 1, 16, 1,                &
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

  TIO_CHECK( TIO_Create_Quant_f(file_id, unstrmesh_id, "Quant1", quant_id4, TIO_REAL8_F, &
                  TIO_CENTRE_NODE_F, 0, .false., "node") )

  qnode(:) = (/ 0., 1., 2., 3., 4., 5., 6., 7., 8. /)
  
  TIO_CHECK( TIO_Write_UnstrQuant_Chunk_f( file_id, quant_id4, 1, TIO_XFER_COLLECTIVE_F, TIO_REAL8_F, qnode ) )

!======================================================================================================================
! UNSTRUCTURED MESH

  TIO_CHECK( TIO_Create_Mesh_f( file_id, state_id, "PointMesh", pointmesh_id,                          &
       &                        TIO_MESH_POINT_F, TIO_COORD_CARTESIAN_F, .false., "Group1", 1, &
       &                        TIO_INTEGER4_F, TIO_REAL8_F, TIO_3D_F, 100, 0, 0, 0, 1,                &
       &                        "mm", "mm", "mm", "x", "y", "z" ) )

  TIO_CHECK( TIO_Set_Point_Chunk_f(file_id, pointmesh_id, 1, TIO_3D_F, 1, 100, 0) )
  
  TIO_CHECK( TIO_Create_Quant_f(file_id, pointmesh_id, "Quant1", quant_id5, TIO_REAL8_F, &
                  TIO_CENTRE_NODE_F, 0, .false., "node") )

  qpoint(:) = (/ (i,i=1,100) /)
  
  TIO_CHECK( TIO_Write_PointQuant_Chunk_f( file_id, quant_id5, 1, TIO_XFER_COLLECTIVE_F, TIO_REAL8_F, qpoint ) )

!======================================================================================================================

  TIO_CHECK( TIO_Close_f(file_id) )

end program main

!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!
module TIO_Chunk_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  use TIO_Utils_mod
  
  
  implicit none
  
  
  private
  public :: TIO_Get_nchunks_f
  public :: TIO_Set_Quad_Chunk_f,  TIO_Get_Quad_Chunk_f,  TIO_Set_Quad_Section_f
  public :: TIO_Set_Unstr_Chunk_f, TIO_Get_Unstr_Chunk_f, TIO_Set_Unstr_Section_f
  public :: TIO_Set_Point_Chunk_f, TIO_Get_Point_Chunk_f, TIO_Set_Point_Section_f
  
contains
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_nchunks_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_nchunks
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_nchunks_f( fileID, objectID, nchunks )
  ! <DATA>   
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: objectID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out) :: nchunks
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_nchunks_f( fileID, objectID, nchunks )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: objectID
    integer(kind=TIO_SIZEK), intent(out) :: nchunks
    
    
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_nchunks_c(fileID, objectID, nchunks, err)
    TIO_Get_nchunks_f = err
    
  end function TIO_Get_nchunks_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Quad_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Quad_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Quad_Chunk_f( fileID, meshID, idx, ndims,
  ! <DATA>                                                       il, ih, jl, jh, kl, kh,
  ! <DATA>                                                       nmixcell, nmixcomp )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in) :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in) :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in) :: il, ih, jl, jh, kl, kh
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in) :: nmixcell, nmixcomp
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Set_Quad_Chunk_f( fileID, meshID, idx, ndims,  &
       &                                                il, ih, jl, jh, kl, kh, nmixcell, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: ndims
    integer(kind=TIO_SIZEK), intent(in) :: il, ih, jl, jh, kl, kh
    integer(kind=TIO_SIZEK), intent(in) :: nmixcell, nmixcomp
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_set_quad_chunk_c(fileID, meshID, idx, ndims,  &
         &                    il, ih, jl, jh, kl, kh, nmixcell, nmixcomp, err)
    
    TIO_Set_Quad_Chunk_f = err
    
  end function TIO_Set_Quad_Chunk_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Quad_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Quad_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Quad_Chunk_f( fileID, objectID, idx, ndims,
  ! <DATA>                                                       il, ih, jl, jh, kl, kh,
  ! <DATA>                                                       nmixcell, nmixcomp )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: objectID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: il, ih, jl, jh, kl, kh
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_Quad_Chunk_f( fileID, objectID, idx, ndims,  &
       &                                                il, ih, jl, jh, kl, kh, nmixcell, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: objectID
    integer(kind=TIO_SIZEK), intent(in)            :: idx
    integer(kind=TIO_IPK),   intent(out)           :: ndims
    integer(kind=TIO_SIZEK), intent(out)           :: il, ih, jl, jh, kl, kh
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp
    
    integer(kind=TIO_SIZEK) :: lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_quad_chunk_c(fileID, objectID, idx, ndims,  &
         &                    il, ih, jl, jh, kl, kh, lnmx, lnmc, err)
    
    if (present(nmixcell)) nmixcell = lnmx
    if (present(nmixcomp)) nmixcomp = lnmc
    
    TIO_Get_Quad_Chunk_f = err
    
  end function TIO_Get_Quad_Chunk_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Quad_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Quad_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Quad_Section_f( fileID, meshID, sectionID, ndims,
  ! <DATA>                                                         il, ih, jl, jh, kl, kh )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: meshID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out) :: sectionID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)  :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: il, ih, jl, jh, kl, kh
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Set_Quad_Section_f( fileID, meshID, sectionID, ndims,  &
       &                                                  il, ih, jl, jh, kl, kh )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(out) :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: ndims
    integer(kind=TIO_SIZEK), intent(in)  :: il, ih, jl, jh, kl, kh
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_set_quad_section_c(fileID, meshID, sectionID, ndims, il, ih, jl, jh, kl, kh, err)
    
    TIO_Set_Quad_Section_f = err
    
  end function TIO_Set_Quad_Section_f
  
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Unstr_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Unstr_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Unstr_Chunk_f( fileID, meshID, idx, ndims,
  ! <DATA>                                                        nnodes, ncells, 
  ! <DATA>                                                        nshapes, nconnectivity,
  ! <DATA>                                                        nghost_nodes,  nghost_cells,
  ! <DATA>                                                        nghost_shapes, nghost_connectivity,
  ! <DATA>                                                        nmixcell, nmixcomp )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)  :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: nnodes, ncells, nshapes, nconnectivity
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: nghost_nodes,  nghost_cells
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: nghost_shapes, nghost_connectivity
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: nmixcell, nmixcomp
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Set_Unstr_Chunk_f( fileID, meshID, idx, ndims,              &
       &                                                 nnodes, ncells, nshapes, nconnectivity,  &
       &                                                 nghost_nodes,  nghost_cells,             &
       &                                                 nghost_shapes, nghost_connectivity,      &
       &                                                 nmixcell, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: ndims
    integer(kind=TIO_SIZEK), intent(in)  :: nnodes, ncells, nshapes, nconnectivity
    integer(kind=TIO_SIZEK), intent(in)  :: nghost_nodes,  nghost_cells
    integer(kind=TIO_SIZEK), intent(in)  :: nghost_shapes, nghost_connectivity
    integer(kind=TIO_SIZEK), intent(in)  :: nmixcell, nmixcomp
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_set_unstr_chunk_c(fileID, meshID, idx, ndims,  &
         &                     nnodes, ncells, nshapes, nconnectivity,  &
         &                     nghost_nodes,  nghost_cells, nghost_shapes, nghost_connectivity,  &
         &                     nmixcell, nmixcomp, err)
    
    TIO_Set_Unstr_Chunk_f = err
    
  end function TIO_Set_Unstr_Chunk_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Unstr_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Unstr_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Unstr_Chunk_f( fileID, objectID, idx, ndims,
  ! <DATA>                                                        nnodes, ncells, 
  ! <DATA>                                                        nshapes, nconnectivity,
  ! <DATA>                                                        nghost_nodes,  nghost_cells,
  ! <DATA>                                                        nghost_shapes, nghost_connectivity,
  ! <DATA>                                                        nmixcell, nmixcomp )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: objectID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: nnodes, ncells, nshapes, nconnectivity
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nghost_nodes,  nghost_cells
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nghost_shapes, nghost_connectivity
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_Unstr_Chunk_f( fileID, objectID, idx, ndims,            &
       &                                                 nnodes, ncells, nshapes, nconnectivity,  &
       &                                                 nghost_nodes,  nghost_cells,             &
       &                                                 nghost_shapes, nghost_connectivity,      &
       &                                                 nmixcell, nmixcomp )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: objectID
    integer(kind=TIO_SIZEK), intent(in)            :: idx
    integer(kind=TIO_IPK),   intent(out)           :: ndims
    integer(kind=TIO_SIZEK), intent(out)           :: nnodes, ncells, nshapes, nconnectivity
    integer(kind=TIO_SIZEK), intent(out), optional :: nghost_nodes,  nghost_cells
    integer(kind=TIO_SIZEK), intent(out), optional :: nghost_shapes, nghost_connectivity
    integer(kind=TIO_SIZEK), intent(out), optional :: nmixcell, nmixcomp
    
    integer(kind=TIO_SIZEK) :: lngn, lngc, lngs, lngv, lnmx, lnmc
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_unstr_chunk_c(fileID, objectID, idx, ndims,           &
         &                     nnodes, ncells, nshapes, nconnectivity, &
         &                     lngn, lngc, lngs, lngv, lnmx, lnmc, err)
    
    if (present(nghost_nodes))        nghost_nodes        = lngn
    if (present(nghost_cells))        nghost_cells        = lngc
    if (present(nghost_shapes))       nghost_shapes       = lngs
    if (present(nghost_connectivity)) nghost_connectivity = lngv
    
    if (present(nmixcell)) nmixcell = lnmx
    if (present(nmixcomp)) nmixcomp = lnmc
    
    TIO_Get_Unstr_Chunk_f = err
    
  end function TIO_Get_Unstr_Chunk_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Unstr_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Unstr_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Unstr_Section_f( fileID, meshID, sectionID, 
  ! <DATA>                                                          centring, nIDs, IDs )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),               intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(in)  :: meshID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(out) :: sectionID
  ! <DATA>   integer(kind=TIO_IPK),                 intent(in)  :: centring
  ! <DATA>   integer(kind=TIO_SIZEK),               intent(in)  :: nIDs
  ! <DATA>   integer(kind=TIO_SIZEK), dimension(:), intent(in)  :: IDs
  ! </FAPI>
  ! <FDES>
  ! <DATA> Different sections must be made for nodal and cell centred objects.
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Set_Unstr_Section_f( fileID, meshID, sectionID,  &
       &                                                   centring, nIDs, IDs )
    
    implicit none
    
    integer(kind=TIO_FILEK),               intent(in)  :: fileID
    integer(kind=TIO_OBJK),                intent(in)  :: meshID
    integer(kind=TIO_OBJK),                intent(out) :: sectionID
    integer(kind=TIO_IPK),                 intent(in)  :: centring
    integer(kind=TIO_SIZEK),               intent(in)  :: nIDs
    integer(kind=TIO_SIZEK), dimension(:), intent(in)  :: IDs
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_set_unstr_section_c(fileID, meshID, sectionID, centring, nIDs, IDs, err)
    
    TIO_Set_Unstr_Section_f = err
    
  end function TIO_Set_Unstr_Section_f
  
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Point_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Point_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Point_Chunk_f( fileID, meshID, idx, ndims,
  ! <DATA>                                                        nl, nh, nghost_nodes )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in) :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in) :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in) :: nl, nh, nghost_nodes
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Set_Point_Chunk_f( fileID, meshID, idx, ndims,  &
       &                                                 nl, nh, nghost_nodes )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer(kind=TIO_IPK),   intent(in) :: ndims
    integer(kind=TIO_SIZEK), intent(in) :: nl, nh, nghost_nodes
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_set_point_chunk_c(fileID, meshID, idx, ndims, nl, nh, nghost_nodes, err)
    
    TIO_Set_Point_Chunk_f = err
    
  end function TIO_Set_Point_Chunk_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Point_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Point_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Point_Chunk_f( fileID, objectID, idx, ndims,
  ! <DATA>                                                        nl, nh, nghost_nodes )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: objectID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out)           :: nl, nh
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: nghost_nodes
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Get_Point_Chunk_f( fileID, objectID, idx, ndims,  &
       &                                                 nl, nh, nghost_nodes )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: objectID
    integer(kind=TIO_SIZEK), intent(in)            :: idx
    integer(kind=TIO_IPK),   intent(out)           :: ndims
    integer(kind=TIO_SIZEK), intent(out)           :: nl, nh
    integer(kind=TIO_SIZEK), intent(out), optional :: nghost_nodes
    
    integer(kind=TIO_SIZEK) :: lngn
    integer(kind=TIO_ERRK)  :: err
    
    
    call tio_get_point_chunk_c(fileID, objectID, idx, ndims,  nl, nh, lngn, err)
    
    if (present(nghost_nodes)) nghost_nodes = lngn
    
    TIO_Get_Point_Chunk_f = err
    
  end function TIO_Get_Point_Chunk_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Point_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Point_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Point_Section_f( fileID, meshID, sectionID, 
  ! <DATA>                                                          nIDs, IDs )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),               intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(in)  :: meshID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(out) :: sectionID
  ! <DATA>   integer(kind=TIO_SIZEK),               intent(in)  :: nIDs
  ! <DATA>   integer(kind=TIO_SIZEK), dimension(:), intent(in)  :: IDs
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Set_Point_Section_f( fileID, meshID, sectionID, nIDs, IDs )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(out) :: sectionID
    integer(kind=TIO_SIZEK), intent(in)  :: nIDs
    integer(kind=TIO_SIZEK), dimension(:), intent(in) :: IDs
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_set_point_section_c(fileID, meshID, sectionID, nIDs, IDs, err)
    
    TIO_Set_Point_Section_f = err
    
  end function TIO_Set_Point_Section_f
  
  
end module TIO_Chunk_mod



!
!  EOF

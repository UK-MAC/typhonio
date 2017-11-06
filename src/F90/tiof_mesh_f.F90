!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!
!-*-Mode: F90;-*-  emacs mode specifier


module TIO_Mesh_mod
  
  use TIO_Global_mod
  use TIO_Error_mod
  use TIO_Utils_mod
  
  implicit none
  
  
  private
  public :: TIO_List_Meshes_f
  public :: TIO_Create_Mesh_f, TIO_Open_Mesh_f, TIO_Open_Mesh_idx_f
  public :: TIO_Close_Mesh_f
  public :: TIO_Get_Mesh_Info_f
  public :: TIO_Set_Mesh_Range_f, TIO_Get_Mesh_Range_f
  
  public :: TIO_Write_QuadMesh_Chunk_f, TIO_Write_QuadMesh_All_f
  public :: TIO_Read_QuadMesh_Chunk_f,  TIO_Read_QuadMesh_All_f, TIO_Read_QuadMesh_Section_f
  
  public :: TIO_Write_UnstrMesh_Chunk_f
  public :: TIO_Read_UnstrMesh_Chunk_f, TIO_Read_UnstrMesh_All_f, TIO_Read_UnstrMesh_Section_f
  
  public :: TIO_Write_PointMesh_Chunk_f
  public :: TIO_Read_PointMesh_Chunk_f, TIO_Read_PointMesh_All_f, TIO_Read_PointMesh_Section_f
  
  
  interface TIO_Set_Mesh_Range_f
    module procedure SetMeshRange_r4
    module procedure SetMeshRange_r8
  end interface
  
  interface TIO_Get_Mesh_Range_f
    module procedure GetMeshRange_r4
    module procedure GetMeshRange_r8
  end interface
  
  
  
  interface TIO_Write_QuadMesh_Chunk_f
    module procedure WriteQuadMeshChunk_1D_r4
    module procedure WriteQuadMeshChunk_1D_r8
    module procedure WriteQuadMeshChunk_2D_r4
    module procedure WriteQuadMeshChunk_2D_r8
    module procedure WriteQuadMeshChunk_3D_r4
    module procedure WriteQuadMeshChunk_3D_r8
  end interface
  
  interface TIO_Write_QuadMesh_All_f
    module procedure WriteQuadMeshAll_r4
    module procedure WriteQuadMeshAll_r8
  end interface
  
  interface TIO_Read_QuadMesh_Chunk_f
    module procedure ReadQuadMeshChunk_meta
    module procedure ReadQuadMeshChunk_1D_r4
    module procedure ReadQuadMeshChunk_1D_r8
    module procedure ReadQuadMeshChunk_2D_r4
    module procedure ReadQuadMeshChunk_2D_r8
    module procedure ReadQuadMeshChunk_3D_r4
    module procedure ReadQuadMeshChunk_3D_r8
  end interface
  
  interface TIO_Read_QuadMesh_All_f
    module procedure ReadQuadMeshAll_meta           ! no coord data, just size metadata
    module procedure ReadQuadMeshAll_1D_r4
    module procedure ReadQuadMeshAll_1D_r8
    module procedure ReadQuadMeshAll_2D_r4
    module procedure ReadQuadMeshAll_2D_r8
    module procedure ReadQuadMeshAll_3D_r4
    module procedure ReadQuadMeshAll_3D_r8
  end interface
  
  interface TIO_Read_QuadMesh_Section_f
    module procedure ReadQuadMeshSection_meta       ! no coord data, just size metadata
    module procedure ReadQuadMeshSection_1D_r4
    module procedure ReadQuadMeshSection_1D_r8
    module procedure ReadQuadMeshSection_2D_r4
    module procedure ReadQuadMeshSection_2D_r8
    module procedure ReadQuadMeshSection_3D_r4
    module procedure ReadQuadMeshSection_3D_r8
  end interface
  
  
  interface TIO_Write_UnstrMesh_Chunk_f
    module procedure WriteUnstrMeshChunk_i4g_r4c
    module procedure WriteUnstrMeshChunk_i8g_r4c
    module procedure WriteUnstrMeshChunk_i4g_r8c
    module procedure WriteUnstrMeshChunk_i8g_r8c
  end interface
  
  interface TIO_Read_UnstrMesh_Chunk_f
    module procedure ReadUnstrMeshChunk_meta
    module procedure ReadUnstrMeshChunk_i4g_r4c
    module procedure ReadUnstrMeshChunk_i8g_r4c
    module procedure ReadUnstrMeshChunk_i4g_r8c
    module procedure ReadUnstrMeshChunk_i8g_r8c
  end interface
  
  interface TIO_Read_UnstrMesh_All_f
    module procedure ReadUnstrMeshAll_meta          ! no graph or coord data, just size metadata
    module procedure ReadUnstrMeshAll_i4g_r4c
    module procedure ReadUnstrMeshAll_i8g_r4c
    module procedure ReadUnstrMeshAll_i4g_r8c
    module procedure ReadUnstrMeshAll_i8g_r8c
  end interface
  
  interface TIO_Read_UnstrMesh_Section_f
    module procedure ReadUnstrMeshSection_meta      ! no graph or coord data, just size metadata
    
    module procedure ReadUnstrMeshSection_r4c
    module procedure ReadUnstrMeshSection_r8c
    
!!$    module procedure ReadUnstrMeshSection_i4g_r4c
!!$    module procedure ReadUnstrMeshSection_i8g_r4c
!!$    module procedure ReadUnstrMeshSection_i4g_r8c
!!$    module procedure ReadUnstrMeshSection_i8g_r8c
  end interface
  
  
  interface TIO_Write_PointMesh_Chunk_f
    module procedure WritePointMeshChunk_r4
    module procedure WritePointMeshChunk_r8
  end interface
  
  interface TIO_Read_PointMesh_Chunk_f
    module procedure ReadPointMeshChunk_meta
    module procedure ReadPointMeshChunk_r4
    module procedure ReadPointMeshChunk_r8
  end interface
  
  interface TIO_Read_PointMesh_All_f
    module procedure ReadPointMeshAll_meta
    module procedure ReadPointMeshAll_r4
    module procedure ReadPointMeshAll_r8
  end interface
  
  interface TIO_Read_PointMesh_Section_f
    module procedure ReadPointMeshSection_meta
    module procedure ReadPointMeshSection_r4
    module procedure ReadPointMeshSection_r8
  end interface
  
  
contains
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_List_Meshes_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_List_Meshes
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_List_Meshes_f( fileID, stateID, nobj, names)
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),        intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),         intent(in)            :: stateID
  ! <DATA>   integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
  ! <DATA>   character(len=*), dimension(:), intent(out), optional :: names
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_List_Meshes_f( fileID, stateID, nobj, names)
    
    implicit none
    
    integer(kind=TIO_FILEK),        intent(in)            :: fileID
    integer(kind=TIO_OBJK),         intent(in)            :: stateID
    integer(kind=TIO_SIZEK),        intent(out), optional :: nobj
    character(len=*), dimension(:), intent(out), optional :: names
    
    
    character :: cdummy
    
    integer(kind=TIO_SIZEK) :: ln
    integer(kind=TIO_ERRK)  :: err
    
    
    ln = -1
    call tio_list_meshes_c(fileID, stateID, ln, cdummy, err)
    TIO_List_Meshes_f = err
    if (TIO_List_Meshes_f /= TIO_SUCCESS_F) return
    
    if (present(nobj)) nobj = ln
    
    if (present(names)) then
      TIO_List_Meshes_f = listnames("TIO_List_Meshes", LISTCLASS_MESH, fileID, stateID, ln, names)
    end if
    
  end function TIO_List_Meshes_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Create_Mesh_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Create_Mesh
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Create_Mesh_f( fileID, stateID, name, meshID,
  ! <DATA>                                                    meshtype, coordtype, isAMR,
  ! <DATA>                                                    group, order,
  ! <DATA>                                                    graph_datatype, coord_datatype,
  ! <DATA>                                                    ndims, n1, n2, n3, n4, nchunks,
  ! <DATA>                                                    iunits, junits, kunits,
  ! <DATA>                                                    ilabel, jlabel, klabel )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)           :: stateID
  ! <DATA>   character(len=*),        intent(in)           :: name
  ! <DATA>   integer(kind=TIO_OBJK),  intent(out)          :: meshID
  ! <DATA>   integer(kind=TIO_IPK) ,  intent(in)           :: meshtype, coordtype
  ! <DATA>   logical(kind=TIO_LPK),   intent(in)           :: isAMR
  ! <DATA>   character(len=*),        intent(in)           :: group
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)           :: order
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)           :: graph_datatype, coord_datatype, ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)           :: n1, n2, n3, n4, nchunks
  ! <DATA>   character(len=*),        intent(in), optional :: iunits, junits, kunits
  ! <DATA>   character(len=*),        intent(in), optional :: ilabel, jlabel, klabel
  ! </FAPI>
  ! <FDES>
  ! <DATA>  For colinear meshes:      n1=ni, n2=nj, n3=nk, n4=ignored                         <br>
  ! <DATA>  For non-colinear meshes:  n1=ni, n2=nj, n3=nk, n4=nghosts of mesh chunks          <br>
  ! <DATA>  For unstructured:         n1=nnodes, n2=ncells, n3=nshapes, n4=nconnectivity      <br>
  ! <DATA>  For point meshes:         n1=nnodes, n2,n3,n4=ignored
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Create_Mesh_f( fileID, stateID, name, meshID,             &
       &                                             meshtype, coordtype, isAMR, group, order,  &
       &                                             graph_datatype, coord_datatype,            &
       &                                             ndims, n1, n2, n3, n4, nchunks,            &
       &                                             iunits, junits, kunits,                    &
       &                                             ilabel, jlabel, klabel )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)           :: fileID
    integer(kind=TIO_OBJK),  intent(in)           :: stateID
    character(len=*),        intent(in)           :: name
    integer(kind=TIO_OBJK),  intent(out)          :: meshID
    integer(kind=TIO_IPK),   intent(in)           :: meshtype, coordtype
    logical(kind=TIO_LPK),   intent(in)           :: isAMR
    character(len=*),        intent(in)           :: group
    integer(kind=TIO_SIZEK), intent(in)           :: order
    integer(kind=TIO_IPK),   intent(in)           :: graph_datatype, coord_datatype, ndims
    integer(kind=TIO_SIZEK), intent(in)           :: n1, n2, n3, n4, nchunks
    character(len=*),        intent(in), optional :: iunits, junits, kunits
    character(len=*),        intent(in), optional :: ilabel, jlabel, klabel
    
    character(len=TIO_STRLEN_F) :: iu="", ju="", ku="", il="", jl="", kl=""
    integer :: name_len, group_len, iu_len, ju_len, ku_len, il_len, jl_len, kl_len
    integer :: lisamr
    integer(kind=TIO_ERRK) :: err
    
    
    TIO_Create_Mesh_f = setlen("TIO_Create_Mesh_f", "name", name, name_len)
    if (TIO_Create_Mesh_f /= TIO_SUCCESS_F) return
    
    if (isAMR) then
      lisamr = 1
    else
      lisamr = 0
    end if
    
    TIO_Create_Mesh_f = setlen("TIO_Create_Mesh_f", "group", group, group_len)
    if (TIO_Create_Mesh_f /= TIO_SUCCESS_F) return
    
    iu_len = 0
    ju_len = 0
    ku_len = 0
    il_len = 0
    jl_len = 0
    kl_len = 0
    
    if (present(iunits)) then
      TIO_Create_Mesh_f = setlen("TIO_Create_Mesh_f", "iunits", iunits, iu_len)
      if (TIO_Create_Mesh_f /= TIO_SUCCESS_F) return
      iu = iunits
    end if
    if (present(junits)) then
      TIO_Create_Mesh_f = setlen("TIO_Create_Mesh_f", "junits", junits, ju_len)
      if (TIO_Create_Mesh_f /= TIO_SUCCESS_F) return
      ju = junits
    end if
    if (present(kunits)) then
      TIO_Create_Mesh_f = setlen("TIO_Create_Mesh_f", "kunits", kunits, ku_len)
      if (TIO_Create_Mesh_f /= TIO_SUCCESS_F) return
      ku = kunits
    end if
    
    if (present(ilabel)) then
      TIO_Create_Mesh_f = setlen("TIO_Create_Mesh_f", "ilabel", ilabel, il_len)
      if (TIO_Create_Mesh_f /= TIO_SUCCESS_F) return
      il = ilabel
    end if
    if (present(jlabel)) then
      TIO_Create_Mesh_f = setlen("TIO_Create_Mesh_f", "jlabel", jlabel, jl_len)
      if (TIO_Create_Mesh_f /= TIO_SUCCESS_F) return
      jl = jlabel
    end if
    if (present(klabel)) then
      TIO_Create_Mesh_f = setlen("TIO_Create_Mesh_f", "klabel", klabel, kl_len)
      if (TIO_Create_Mesh_f /= TIO_SUCCESS_F) return
      kl = klabel
    end if
    
    
    call tio_create_mesh_c( fileID, stateID, name, name_len, meshID,               &
         &                  meshtype, coordtype, lisamr, group, group_len, order,  &
         &                  graph_datatype, coord_datatype,                        &
         &                  ndims, n1, n2, n3, n4, nchunks,                        &
         &                  iu, iu_len, ju, ju_len, ku, ku_len,                    &
         &                  il, il_len, jl, jl_len, kl, kl_len, err                )
    
    
    TIO_Create_Mesh_f = err
    
  end function TIO_Create_Mesh_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Mesh_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Mesh
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Mesh_f( fileID, stateID, name, meshID,
  ! <DATA>                                                  meshtype, coordtype, isAMR,
  ! <DATA>                                                  group, order,
  ! <DATA>                                                  graph_datatype, coord_datatype,
  ! <DATA>                                                  ndims, n1, n2, n3, n4, nchunks,
  ! <DATA>                                                  iunits, junits, kunits,
  ! <DATA>                                                  ilabel, jlabel, klabel )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),  intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),   intent(in)            :: stateID
  ! <DATA>   character(len=*),         intent(in)            :: name
  ! <DATA>   integer(kind=TIO_OBJK),   intent(out)           :: meshID
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: meshtype, coordtype
  ! <DATA>   logical(kind=TIO_LPK),    intent(out), optional :: isAMR
  ! <DATA>   character(len=*),         intent(out), optional :: group
  ! <DATA>   integer(kind=TIO_SIZEK),  intent(out), optional :: order
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: graph_datatype, coord_datatype
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),  intent(out), optional :: n1, n2, n3, n4, nchunks
  ! <DATA>   character(len=*),         intent(out), optional :: iunits, junits, kunits
  ! <DATA>   character(len=*),         intent(out), optional :: ilabel, jlabel, klabel
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Mesh_f( fileID, stateID, name, meshID,             &
       &                                           meshtype, coordtype, isAMR, group, order,  &
       &                                           graph_datatype, coord_datatype,            &
       &                                           ndims, n1, n2, n3, n4, nchunks,            &
       &                                           iunits, junits, kunits,                    &
       &                                           ilabel, jlabel, klabel )
    
    implicit none
    
    integer(kind=TIO_FILEK),  intent(in)            :: fileID
    integer(kind=TIO_OBJK),   intent(in)            :: stateID
    character(len=*),         intent(in)            :: name
    integer(kind=TIO_OBJK),   intent(out)           :: meshID
    integer(kind=TIO_IPK),    intent(out), optional :: meshtype, coordtype
    logical(kind=TIO_LPK),    intent(out), optional :: isAMR
    character(len=*),         intent(out), optional :: group
    integer(kind=TIO_SIZEK),  intent(out), optional :: order
    integer(kind=TIO_IPK),    intent(out), optional :: graph_datatype, coord_datatype, ndims
    integer(kind=TIO_SIZEK),  intent(out), optional :: n1, n2, n3, n4, nchunks
    character(len=*),         intent(out), optional :: iunits, junits, kunits
    character(len=*),         intent(out), optional :: ilabel, jlabel, klabel
    
    
    integer :: name_len
    integer :: lisAMR, lmeshtype, lcoordtype, lcdtype, lgdtype, lndims
    integer(kind=TIO_SIZEK) :: lorder, ln1, ln2, ln3, ln4, lnchunks
    
    character(len=TIO_STRLEN_F) :: lgroup="", iu="", ju="", ku="", il="", jl="", kl=""
    integer                     :: group_len, iu_len, ju_len, ku_len, il_len, jl_len, kl_len
    
    integer(kind=TIO_ERRK) :: err
    
    
    TIO_Open_Mesh_f = setlen("TIO_Open_Mesh_f", "name", name, name_len)
    if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) return
    
    call tio_open_mesh_c(fileID, stateID, -1, name, name_len, meshID,                 &
         &               lmeshtype, lcoordtype, lisAMR, lgroup, len(lgroup), lorder,  &
         &               lgdtype, lcdtype, lndims, ln1, ln2, ln3, ln4, lnchunks,      &
         &               iu, len(iu), ju, len(ju), ku, len(ku),                       &
         &               il, len(il), jl, len(jl), kl, len(kl), err)
    
    TIO_Open_Mesh_f = err
    if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) return
    
    if (present(meshtype))  meshtype  = lmeshtype
    if (present(coordtype)) coordtype = lcoordtype
    if (present(isAMR))     isAMR     = (lisAMR > 0)
    
    if (present(group)) then
      TIO_Open_Mesh_f = setstr("TIO_Open_Mesh_f", "group", lgroup, group)
      if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if


    if (present(order)) order = lorder
    
    if (present(graph_datatype)) graph_datatype = lgdtype
    if (present(coord_datatype)) coord_datatype = lcdtype
    
    if (present(ndims))   ndims   = lndims
    if (present(n1))      n1      = ln1
    if (present(n2))      n2      = ln2
    if (present(n3))      n3      = ln3
    if (present(n4))      n4      = ln4
    if (present(nchunks)) nchunks = lnchunks
    
    if (present(iunits)) then
      TIO_Open_Mesh_f = setstr("TIO_Open_Mesh_f", "iunits", iu, iunits)
      if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    if (present(junits)) then
      TIO_Open_Mesh_f = setstr("TIO_Open_Mesh_f", "junits", ju, junits)
      if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    if (present(kunits)) then
      TIO_Open_Mesh_f = setstr("TIO_Open_Mesh_f", "kunits", ku, kunits)
      if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    
    if (present(ilabel)) then
      TIO_Open_Mesh_f = setstr("TIO_Open_Mesh_f", "ilabel", il, ilabel)
      if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    if (present(jlabel)) then
      TIO_Open_Mesh_f = setstr("TIO_Open_Mesh_f", "jlabel", jl, jlabel)
      if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    if (present(klabel)) then
      TIO_Open_Mesh_f = setstr("TIO_Open_Mesh_f", "klabel", kl, klabel)
      if (TIO_Open_Mesh_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    
  end function TIO_Open_Mesh_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Open_Mesh_idx_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Open_Mesh_idx
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Open_Mesh_idx_f( fileID, stateID, idx, name, meshID,
  ! <DATA>                                                      meshtype, coordtype, isAMR,
  ! <DATA>                                                      group, order,
  ! <DATA>                                                      graph_datatype, coord_datatype,
  ! <DATA>                                                      ndims, n1, n2, n3, n4, nchunks,
  ! <DATA>                                                      iunits, junits, kunits,
  ! <DATA>                                                      ilabel, jlabel, klabel )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),  intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),   intent(in)            :: stateID
  ! <DATA>   integer(kind=TIO_SIZEK),  intent(in)            :: idx
  ! <DATA>   character(len=*),         intent(out)           :: name
  ! <DATA>   integer(kind=TIO_OBJK),   intent(out)           :: meshID
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: meshtype, coordtype
  ! <DATA>   logical(kind=TIO_LPK),    intent(out), optional :: isAMR
  ! <DATA>   character(len=*),         intent(out), optional :: group
  ! <DATA>   integer(kind=TIO_SIZEK),  intent(out), optional :: order
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: graph_datatype, coord_datatype
  ! <DATA>   integer(kind=TIO_IPK),    intent(out), optional :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),  intent(out), optional :: n1, n2, n3, n4, nchunks
  ! <DATA>   character(len=*),         intent(out), optional :: iunits, junits, kunits
  ! <DATA>   character(len=*),         intent(out), optional :: ilabel, jlabel, klabel
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Open_Mesh_idx_f( fileID, stateID, idx, name, meshID,        &
       &                                               meshtype, coordtype, isAMR, group, order,  &
       &                                               graph_datatype, coord_datatype,            &
       &                                               ndims, n1, n2, n3, n4, nchunks,            &
       &                                               iunits, junits, kunits,                    &
       &                                               ilabel, jlabel, klabel )
    
    implicit none
    
    integer(kind=TIO_FILEK),  intent(in)            :: fileID
    integer(kind=TIO_OBJK),   intent(in)            :: stateID
    integer(kind=TIO_SIZEK),  intent(in)            :: idx
    character(len=*),         intent(out)           :: name
    integer(kind=TIO_OBJK),   intent(out)           :: meshID
    integer(kind=TIO_IPK),    intent(out), optional :: meshtype, coordtype
    logical(kind=TIO_LPK),    intent(out), optional :: isAMR
    character(len=*),         intent(out), optional :: group
    integer(kind=TIO_SIZEK),  intent(out), optional :: order
    integer(kind=TIO_IPK),    intent(out), optional :: graph_datatype, coord_datatype, ndims
    integer(kind=TIO_SIZEK),  intent(out), optional :: n1, n2, n3, n4, nchunks
    character(len=*),         intent(out), optional :: iunits, junits, kunits
    character(len=*),         intent(out), optional :: ilabel, jlabel, klabel
    
    
    integer :: name_len
    integer :: lisAMR, lmeshtype, lcoordtype, lcdtype, lgdtype, lndims
    integer(kind=TIO_SIZEK) :: lorder, ln1, ln2, ln3, ln4, lnchunks
    
    character(len=TIO_STRLEN_F) :: lname="", lgroup="", iu="", ju="", ku="", il="", jl="", kl=""
    integer                     :: group_len, iu_len, ju_len, ku_len, il_len, jl_len, kl_len
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_open_mesh_c(fileID, stateID, idx, lname, len(name), meshID,              &
         &               lmeshtype, lcoordtype, lisAMR, lgroup, len(lgroup), lorder,  &
         &               lgdtype, lcdtype, lndims, ln1, ln2, ln3, ln4, lnchunks,      &
         &               iu, len(iu), ju, len(ju), ku, len(ku),                       &
         &               il, len(il), jl, len(jl), kl, len(kl), err)
    
    TIO_Open_Mesh_idx_f = err
    if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) return
    
    TIO_Open_Mesh_idx_f = setstr("TIO_Open_Mesh_idx_f", "name", lname, name)
    if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
      if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    
    if (present(meshtype))  meshtype  = lmeshtype
    if (present(coordtype)) coordtype = lcoordtype
    if (present(isAMR))     isAMR     = (lisAMR > 0)
    
    if (present(group)) then
      TIO_Open_Mesh_idx_f = setstr("TIO_Open_Mesh_idx_f", "group", lgroup, group)
      if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    
    if (present(order)) order = lorder
    
    if (present(graph_datatype)) graph_datatype = lgdtype
    if (present(coord_datatype)) coord_datatype = lcdtype
    
    if (present(ndims))   ndims   = lndims
    if (present(n1))      n1      = ln1
    if (present(n2))      n2      = ln2
    if (present(n3))      n3      = ln3
    if (present(n4))      n4      = ln4
    if (present(nchunks)) nchunks = lnchunks
    
    if (present(iunits)) then
      TIO_Open_Mesh_idx_f = setstr("TIO_Open_Mesh_idx_f", "iunits", iu, iunits)
      if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    if (present(junits)) then
      TIO_Open_Mesh_idx_f = setstr("TIO_Open_Mesh_idx_f", "junits", ju, junits)
      if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    if (present(kunits)) then
      TIO_Open_Mesh_idx_f = setstr("TIO_Open_Mesh_idx_f", "kunits", ku, kunits)
      if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    
    if (present(ilabel)) then
      TIO_Open_Mesh_idx_f = setstr("TIO_Open_Mesh_idx_f", "ilabel", il, ilabel)
      if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    if (present(jlabel)) then
      TIO_Open_Mesh_idx_f = setstr("TIO_Open_Mesh_idx_f", "jlabel", jl, jlabel)
      if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    if (present(klabel)) then
      TIO_Open_Mesh_idx_f = setstr("TIO_Open_Mesh_idx_f", "klabel", kl, klabel)
      if (TIO_Open_Mesh_idx_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
    
  end function TIO_Open_Mesh_idx_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Mesh_Info_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Mesh_Info
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Mesh_Info_f( fileID, meshID, name,
  ! <DATA>                                                      meshtype, coordtype, isAMR,
  ! <DATA>                                                      group, order,
  ! <DATA>                                                      graph_datatype, coord_datatype,
  ! <DATA>                                                      ndims, n1, n2, n3, n4, nchunks,
  ! <DATA>                                                      iunits, junits, kunits,
  ! <DATA>                                                      ilabel, jlabel, klabel )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: meshID
  ! <DATA>   character(len=*),        intent(out), optional :: name
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: meshtype, coordtype
  ! <DATA>   logical(kind=TIO_LPK),   intent(out), optional :: isAMR
  ! <DATA>   character(len=*),        intent(out), optional :: group
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: order
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: graph_datatype, coord_datatype
  ! <DATA>   integer(kind=TIO_IPK),   intent(out), optional :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out), optional :: n1, n2, n3, n4, nchunks
  ! <DATA>   character(len=*),        intent(out), optional :: iunits, junits, kunits
  ! <DATA>   character(len=*),        intent(out), optional :: ilabel, jlabel, klabel
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer function TIO_Get_Mesh_Info_f( fileID, meshID, name,                      &
       &                                meshtype, coordtype, isAMR, group, order,  &
       &                                graph_datatype, coord_datatype,            &
       &                                ndims, n1, n2, n3, n4, nchunks,            &
       &                                iunits, junits, kunits,                    &
       &                                ilabel, jlabel, klabel )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)            :: fileID
    integer(kind=TIO_OBJK),  intent(in)            :: meshID
    character(len=*),        intent(out), optional :: name
    integer(kind=TIO_IPK),   intent(out), optional :: meshtype, coordtype
    logical(kind=TIO_LPK),   intent(out), optional :: isAMR
    character(len=*),        intent(out), optional :: group
    integer(kind=TIO_SIZEK), intent(out), optional :: order
    integer(kind=TIO_IPK),   intent(out), optional :: graph_datatype, coord_datatype, ndims
    integer(kind=TIO_SIZEK), intent(out), optional :: n1, n2, n3, n4, nchunks
    character(len=*),        intent(out), optional :: iunits, junits, kunits
    character(len=*),        intent(out), optional :: ilabel, jlabel, klabel
    
    
    integer :: name_len
    integer :: lisAMR, lmeshtype, lcoordtype, lcdtype, lgdtype, lndims
    integer(kind=TIO_SIZEK) :: lorder, ln1, ln2, ln3, ln4, lnchunks
    
    character(len=TIO_STRLEN_F) :: lname="", lgroup="", iu="", ju="", ku="", il="", jl="", kl=""
    integer                     :: group_len, iu_len, ju_len, ku_len, il_len, jl_len, kl_len
    
    integer(kind=TIO_ERRK) :: err
    
    
    call tio_get_mesh_info_c( fileID, meshID, lname, len(lname),                           &
         &                    lmeshtype, lcoordtype, lisAMR, lgroup, len(lgroup), lorder,  &
         &                    lgdtype, lcdtype, lndims, ln1, ln2, ln3, ln4, lnchunks,      &
         &                    iu, len(iu), ju, len(ju), ku, len(ku),                       &
         &                    il, len(il), jl, len(jl), kl, len(kl), err)
    
    TIO_Get_Mesh_Info_f = err
    if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) return
    
    if (present(name)) then
      TIO_Get_Mesh_Info_f = setstr("TIO_Get_Mesh_Info_f", "name", lname, name)
      if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(meshtype))  meshtype  = lmeshtype
    if (present(coordtype)) coordtype = lcoordtype
    if (present(isAMR))     isAMR     = (lisAMR > 0)
    
    if (present(group)) then
      TIO_Get_Mesh_Info_f = setstr("TIO_Get_Mesh_Info_f", "group", lgroup, group)
      if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) then
        err = TIO_Close_Mesh_f(fileID, meshID)
        return
      end if
    end if
 
    if (present(order)) order = lorder
    
    if (present(graph_datatype)) graph_datatype = lgdtype
    if (present(coord_datatype)) coord_datatype = lcdtype
    
    if (present(ndims))   ndims   = lndims
    if (present(n1))      n1      = ln1
    if (present(n2))      n2      = ln2
    if (present(n3))      n3      = ln3
    if (present(n4))      n4      = ln4
    if (present(nchunks)) nchunks = lnchunks
    
    if (present(iunits)) then
      TIO_Get_Mesh_Info_f = setstr("TIO_Get_Mesh_Info_f", "iunits", iu, iunits)
      if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) return
    end if
    if (present(junits)) then
      TIO_Get_Mesh_Info_f = setstr("TIO_Get_Mesh_Info_f", "junits", ju, junits)
      if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) return
    end if
    if (present(kunits)) then
      TIO_Get_Mesh_Info_f = setstr("TIO_Get_Mesh_Info_f", "kunits", ku, kunits)
      if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) return
    end if
    
    if (present(ilabel)) then
      TIO_Get_Mesh_Info_f = setstr("TIO_Get_Mesh_Info_f", "ilabel", il, ilabel)
      if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) return
    end if
    if (present(jlabel)) then
      TIO_Get_Mesh_Info_f = setstr("TIO_Get_Mesh_Info_f", "jlabel", jl, jlabel)
      if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) return
    end if
    if (present(klabel)) then
      TIO_Get_Mesh_Info_f = setstr("TIO_Get_Mesh_Info_f", "klabel", kl, klabel)
      if (TIO_Get_Mesh_Info_f /= TIO_SUCCESS_F) return
    end if
    
  end function TIO_Get_Mesh_Info_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Close_Mesh_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Close_Mesh
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Close_Mesh_f( fileID, meshID )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in) :: meshID
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function TIO_Close_Mesh_f( fileID, meshID )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    
    integer(kind=TIO_ERRK) :: err
    
    
    
    call tio_close_mesh_c(fileID, meshID, err)
    
    TIO_Close_Mesh_f = err
    
  end function TIO_Close_Mesh_f
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Set_Mesh_Range_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Set_Mesh_Range
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Set_Mesh_Range_f( fileID, meshID, datatype, ndims,
  ! <DATA>                                                       xl, xh, yl, yh, zl, zh )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype, ndims
  ! <DATA>   [REAL CHOICE],           intent(in)            :: xl, xh
  ! <DATA>   [REAL CHOICE],           intent(in), optional  :: yl, yh, zl, zh
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function SetMeshRange_r4( fileID, meshID, datatype, ndims, &
       &                                           xl, xh, yl, yh, zl, zh )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer,                 intent(in) :: datatype, ndims
    
    real(kind=4), intent(in)            :: xl, xh
    real(kind=4), intent(in), optional  :: yl, yh, zl, zh
    
    real(kind=4) :: lyl, lyh, lzl, lzh
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    err = TIO_SUCCESS_F
    
    if (ndims > TIO_1D_F) then
      if (.not. present(yl)) then
        call seterror("TIO_Set_Mesh_Range_f", "yl,yh must be present if ndims > 1D")
        err = TIO_ERR_F90_API_F
      end if
      if (ndims > TIO_2D_F) then
        if (.not. present(zl)) then
          call seterror("TIO_Set_Mesh_Range_f", "zl,zh must be present if ndims > 2D")
          err = TIO_ERR_F90_API_F
        end if
      end if
    end if
    
    if (present(zl)) then
      if (.not. present(zh)) then
        call seterror("TIO_Set_Mesh_Range_f", "zh must be present if zl is present")
        err = TIO_ERR_F90_API_F
      end if
      
      lzl = zl
      lzh = zh
    end if
    
    if (present(yl)) then
      if (.not. present(yh)) then
        call seterror("TIO_Set_Mesh_Range_f", "yh must be present if yl is present")
        err = TIO_ERR_F90_API_F
      end if
      
      lyl = yl
      lyh = yh
    end if
    
    if (err == TIO_SUCCESS_F) then
      
      call tio_set_mesh_range_c( fileID, meshID, datatype, ndims, xl, xh, lyl, lyh, lzl, lzh, err )
      
    end if
        
    SetMeshRange_r4 = err
  end function SetMeshRange_r4
  integer(kind=TIO_ERRK) function SetMeshRange_r8( fileID, meshID, datatype, ndims, &
       &                                           xl, xh, yl, yh, zl, zh )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer,                 intent(in) :: datatype, ndims
    
    
    real(kind=8), intent(in)            :: xl, xh
    real(kind=8), intent(in), optional  :: yl, yh, zl, zh
    
    real(kind=8) :: lyl, lyh, lzl, lzh
    
    integer(kind=TIO_ERRK) :: err
    
    
    err = TIO_SUCCESS_F
    
    if (ndims > TIO_1D_F) then
      if (.not. present(yl)) then
        call seterror("TIO_Set_Mesh_Range_f", "yl,yh must be present if ndims > 1D")
        err = TIO_ERR_F90_API_F
      end if
      if (ndims > TIO_2D_F) then
        if (.not. present(zl)) then
          call seterror("TIO_Set_Mesh_Range_f", "zl,zh must be present if ndims > 2D")
          err = TIO_ERR_F90_API_F
        end if
      end if
    end if
    
    if (present(zl)) then
      if (.not. present(zh)) then
        call seterror("TIO_Set_Mesh_Range_f", "zh must be present if zl is present")
        err = TIO_ERR_F90_API_F
      end if
      
      lzl = zl
      lzh = zh
    end if
    
    if (present(yl)) then
      if (.not. present(yh)) then
        call seterror("TIO_Set_Mesh_Range_f", "yh must be present if yl is present")
        err = TIO_ERR_F90_API_F
      end if
      
      lyl = yl
      lyh = yh
    end if
    
    if (err == TIO_SUCCESS_F) then
      
      call tio_set_mesh_range_c( fileID, meshID, datatype, ndims, xl, xh, lyl, lyh, lzl, lzh, err )
      
    end if
        
    SetMeshRange_r8 = err
  end function SetMeshRange_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Get_Mesh_Range_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_Mesh_Range
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Get_Mesh_Range_f( fileID, meshID, datatype, ndims,
  ! <DATA>                                                       xl, xh, yl, yh, zl, zh )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_IPK),   intent(out)           :: ndims
  ! <DATA>   [REAL CHOICE],           intent(out)           :: xl, xh
  ! <DATA>   [REAL CHOICE],           intent(out), optional :: yl, yh, zl, zh
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function GetMeshRange_r4( fileID, meshID, datatype, ndims, &
       &                                           xl, xh, yl, yh, zl, zh )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    
    real(kind=4), intent(out)           :: xl, xh
    real(kind=4), intent(out), optional :: yl, yh, zl, zh
    
    real(kind=4) :: lyl, lyh, lzl, lzh
    
    
    integer(kind=TIO_ERRK) :: err
    
    
    err = TIO_SUCCESS_F
    
!!$    if (present(zl)) then
!!$      if (.not. present(zh)) then
!!$        call seterror("TIO_Get_Mesh_Range_f", "zh must be present if zl is present")
!!$        err = TIO_ERR_F90_API_F
!!$      end if
!!$    end if
!!$    
!!$    if (present(yl)) then
!!$      if (.not. present(yh)) then
!!$        call seterror("TIO_Get_Mesh_Range_f", "yh must be present if yl is present")
!!$        err = TIO_ERR_F90_API_F
!!$      end if
!!$    end if
    
    if (err == TIO_SUCCESS_F) then
      
      call tio_get_mesh_range_c( fileID, meshID, datatype, ndims, xl, xh, lyl, lyh, lzl, lzh, err )
      
      if (present(yl)) yl = lyl
      if (present(yh)) yh = lyh
      if (present(zl)) zl = lzl
      if (present(zh)) zh = lzh
    
    end if
    
!!$    if (ndims > TIO_1D_F) then
!!$      if (.not. present(yl)) then
!!$        call seterror("TIO_Get_Mesh_Range_f", "yl,yh must be present if ndims > 1D")
!!$        err = TIO_ERR_F90_API_F
!!$      end if
!!$      if (ndims > TIO_2D_F) then
!!$        if (.not. present(zl)) then
!!$          call seterror("TIO_Get_Mesh_Range_f", "zl,zh must be present if ndims > 2D")
!!$          err = TIO_ERR_F90_API_F
!!$        end if
!!$      end if
!!$    end if
    
    
    GetMeshRange_r4 = err
  end function GetMeshRange_r4
  integer(kind=TIO_ERRK) function GetMeshRange_r8( fileID, meshID, datatype, ndims, &
       &                                           xl, xh, yl, yh, zl, zh )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    
    
    real(kind=8), intent(out)           :: xl, xh
    real(kind=8), intent(out), optional :: yl, yh, zl, zh
    
    real(kind=8) :: lyl, lyh, lzl, lzh
    
    integer(kind=TIO_ERRK) :: err
    
    
    err = TIO_SUCCESS_F
    
!!$    if (present(zl)) then
!!$      if (.not. present(zh)) then
!!$        call seterror("TIO_Get_Mesh_Range_f", "zh must be present if zl is present")
!!$        err = TIO_ERR_F90_API_F
!!$      end if
!!$    end if
!!$    
!!$    if (present(yl)) then
!!$      if (.not. present(yh)) then
!!$        call seterror("TIO_Get_Mesh_Range_f", "yh must be present if yl is present")
!!$        err = TIO_ERR_F90_API_F
!!$      end if
!!$    end if
    
    if (err == TIO_SUCCESS_F) then
      
      call tio_get_mesh_range_c( fileID, meshID, datatype, ndims, xl, xh, lyl, lyh, lzl, lzh, err )
      
      if (present(yl)) yl = lyl
      if (present(yh)) yh = lyh
      if (present(zl)) zl = lzl
      if (present(zh)) zh = lzh
    
    end if
    
!!$    if (ndims > TIO_1D_F) then
!!$      if (.not. present(yl)) then
!!$        call seterror("TIO_Get_Mesh_Range_f", "yl,yh must be present if ndims > 1D")
!!$        err = TIO_ERR_F90_API_F
!!$      end if
!!$      if (ndims > TIO_2D_F) then
!!$        if (.not. present(zl)) then
!!$          call seterror("TIO_Get_Mesh_Range_f", "zl,zh must be present if ndims > 2D")
!!$          err = TIO_ERR_F90_API_F
!!$        end if
!!$      end if
!!$    end if
    
    
    GetMeshRange_r8 = err
  end function GetMeshRange_r8
  
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **          QUADMESHES            ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_QuadMesh_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_QuadMesh_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_QuadMesh_Chunk_f( fileID, meshID, idx, xfer,
  ! <DATA>                                                             datatype,
  ! <DATA>                                                             icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),    intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),     intent(in)           :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK),    intent(in)           :: idx
  ! <DATA>   integer(kind=TIO_IPK),      intent(in)           :: xfer
  ! <DATA>   integer(kind=TIO_IPK),      intent(in)           :: datatype
  ! <DATA>   [REAL CHOICE], [DIMENSION], intent(in)           :: icoords
  ! <DATA>   [REAL CHOICE], [DIMENSION], intent(in), optional :: jcoords, kcoords
  ! </FAPI>
  ! <FDES>
  ! <DATA> To be used with non-colinear quad mesh chunks.
  ! <DATA> For colinear meshes use TIO_Write_QuadMesh_All.
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function WriteQuadMeshChunk_1D_r4( fileID, meshID, idx, xfer, datatype,  &
  &                                                         icoords, jcoords, kcoords) result(res)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer,                 intent(in) :: xfer
    integer,                 intent(in) :: datatype


    real(kind=4),    dimension(:), intent(in)               :: icoords
    real(kind=4),    dimension(:), intent(in),     optional :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case (n)
    case (3)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, kcoords, n, err     )
    case (2)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, dummy, n, err       )
    case (1)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, dummy, dummy, n, err         )
    end select


    res = err


        
  end function WriteQuadMeshChunk_1D_r4
  integer(kind=TIO_ERRK) function WriteQuadMeshChunk_1D_r8( fileID, meshID, idx, xfer, datatype,   &
  &                                                         icoords, jcoords, kcoords) result(res)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer,                 intent(in) :: xfer
    integer,                 intent(in) :: datatype


    real(kind=8),    dimension(:), intent(in)               :: icoords
    real(kind=8),    dimension(:), intent(in),     optional :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case (n)
    case (3)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, kcoords, n, err     )
    case (2)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, dummy, n, err       )
    case (1)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, dummy, dummy, n, err         )
    end select


    res = err


        
  end function WriteQuadMeshChunk_1D_r8
  integer(kind=TIO_ERRK) function WriteQuadMeshChunk_2D_r4( fileID, meshID, idx, xfer, datatype,  &
  &                                                         icoords, jcoords, kcoords) result(res)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer,                 intent(in) :: xfer
    integer,                 intent(in) :: datatype


    real(kind=4),    dimension(:,:), intent(in)             :: icoords
    real(kind=4),    dimension(:,:), intent(in),   optional :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case (n)
    case (3)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, kcoords, n, err     )
    case (2)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, dummy, n, err       )
    case (1)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, dummy, dummy, n, err         )
    end select


    res = err


        
  end function WriteQuadMeshChunk_2D_r4
  integer(kind=TIO_ERRK) function WriteQuadMeshChunk_2D_r8( fileID, meshID, idx, xfer, datatype,   &
  &                                                         icoords, jcoords, kcoords) result(res)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer,                 intent(in) :: xfer
    integer,                 intent(in) :: datatype


    real(kind=8),    dimension(:,:), intent(in)             :: icoords
    real(kind=8),    dimension(:,:), intent(in),   optional :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case (n)
    case (3)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, kcoords, n, err     )
    case (2)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, dummy, n, err       )
    case (1)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, dummy, dummy, n, err         )
    end select


    res = err


        
  end function WriteQuadMeshChunk_2D_r8
  integer(kind=TIO_ERRK) function WriteQuadMeshChunk_3D_r4( fileID, meshID, idx, xfer, datatype,  &
  &                                                         icoords, jcoords, kcoords) result(res)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer,                 intent(in) :: xfer
    integer,                 intent(in) :: datatype


    real(kind=4),    dimension(:,:,:), intent(in)           :: icoords
    real(kind=4),    dimension(:,:,:), intent(in), optional :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case (n)
    case (3)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, kcoords, n, err     )
    case (2)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, dummy, n, err       )
    case (1)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, dummy, dummy, n, err         )
    end select


    res = err


        
  end function WriteQuadMeshChunk_3D_r4
  integer(kind=TIO_ERRK) function WriteQuadMeshChunk_3D_r8( fileID, meshID, idx, xfer, datatype,   &
  &                                                         icoords, jcoords, kcoords) result(res)
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in) :: fileID
    integer(kind=TIO_OBJK),  intent(in) :: meshID
    integer(kind=TIO_SIZEK), intent(in) :: idx
    integer,                 intent(in) :: xfer
    integer,                 intent(in) :: datatype


    real(kind=8),    dimension(:,:,:), intent(in)           :: icoords
    real(kind=8),    dimension(:,:,:), intent(in), optional :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case (n)
    case (3)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, kcoords, n, err     )
    case (2)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, jcoords, dummy, n, err       )
    case (1)
      call tio_write_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                           icoords, dummy, dummy, n, err         )
    end select


    res = err


        
  end function WriteQuadMeshChunk_3D_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_QuadMesh_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_QuadMesh_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_QuadMesh_All_f( fileID, meshID, datatype,
  ! <DATA>                                                           icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),     intent(in)           :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),      intent(in)           :: meshID
  ! <DATA>   integer(kind=TIO_IPK),       intent(in)           :: datatype
  ! <DATA>   [REAL CHOICE], dimension(:), intent(in)           :: icoords
  ! <DATA>   [REAL CHOICE], dimension(:), intent(in), optional :: jcoords, kcoords
  ! </FAPI>
  ! <FDES>
  ! <DATA> To be used with colinear quad mesh chunks.
  ! <DATA> For non-colinear meshes use TIO_Write_QuadMesh_Chunk.
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function WriteQuadMeshAll_r4( fileID, meshID, datatype,  &
  &                                                    icoords, jcoords, kcoords) result(res)
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in) :: fileID
    integer(kind=TIO_OBJK),                 intent(in) :: meshID
    integer,                                intent(in) :: datatype


    real(kind=4),    dimension(:), intent(in)            :: icoords
    real(kind=4),    dimension(:), intent(in), optional  :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_All_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case(n)
    case (3)
      call tio_write_quadmesh_all_c( fileID, meshID, datatype, icoords, jcoords, kcoords, n, err )
    case (2)
      call tio_write_quadmesh_all_c( fileID, meshID, datatype, icoords, jcoords, dummy, n, err )
    case(1)
      call tio_write_quadmesh_all_c( fileID, meshID, datatype, icoords, dummy, dummy, n, err )
    end select
    
    res = err


    
  end function WriteQuadMeshAll_r4
  integer(kind=TIO_ERRK) function WriteQuadMeshAll_r8( fileID, meshID, datatype,   &
  &                                                    icoords, jcoords, kcoords) result(res)
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in) :: fileID
    integer(kind=TIO_OBJK),                 intent(in) :: meshID
    integer,                                intent(in) :: datatype


    real(kind=8),    dimension(:), intent(in)            :: icoords
    real(kind=8),    dimension(:), intent(in), optional  :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_All_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case(n)
    case (3)
      call tio_write_quadmesh_all_c( fileID, meshID, datatype, icoords, jcoords, kcoords, n, err )
    case (2)
      call tio_write_quadmesh_all_c( fileID, meshID, datatype, icoords, jcoords, dummy, n, err )
    case(1)
      call tio_write_quadmesh_all_c( fileID, meshID, datatype, icoords, dummy, dummy, n, err )
    end select
    
    res = err


    
  end function WriteQuadMeshAll_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadMesh_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadMesh_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadMesh_Chunk_f( fileID, meshID, idx, xfer,
  ! <DATA>                                                            datatype, nghosts,
  ! <DATA>                                                            ndims, ni, nj, nk,
  ! <DATA>                                                            icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),    intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),     intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK),    intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),      intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),      intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),    intent(in)            :: nghosts
  ! <DATA>   integer(kind=TIO_IPK),      intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),    intent(out)           :: ni, nj, nk
  ! <DATA>   [REAL CHOICE], [DIMENSION], intent(out)           :: icoords
  ! <DATA>   [REAL CHOICE], [DIMENSION], intent(out), optional :: jcoords, kcoords
  ! </FAPI>
  ! <FDES>
  ! <DATA> DIMENSION is dimension(:) for colinear meshes, or dimension(:), dimension(:,:), or 
  ! <DATA> dimension(:,:,:) for non-colinear.
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadQuadMeshChunk_meta(  fileID, meshID, idx, xfer,            &
       &                                                   datatype, nghosts, ndims, ni, nj, nk  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    


    n = -1
    
    call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
         &                          nghosts, ndims, ni, nj, nk,           &
         &                          dummy, dummy, dummy, n, err           )


    
    
    
    
    ReadQuadMeshChunk_meta = err
  end function ReadQuadMeshChunk_meta
  integer(kind=TIO_ERRK) function ReadQuadMeshChunk_1D_r4( fileID, meshID, idx, xfer,            &
       &                                                   datatype, nghosts, ndims, ni, nj, nk, &
       &                                                   icoords, jcoords, kcoords             )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:),               intent(out) :: icoords
    real(kind=4), dimension(:),     optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, kcoords, n, err     )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, dummy, n, err       )
      
    else
      
      n = 1
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, dummy, dummy, n, err         )
      
    end if
    
    
    
    ReadQuadMeshChunk_1D_r4 = err
  end function ReadQuadMeshChunk_1D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshChunk_1D_r8( fileID, meshID, idx, xfer,            &
       &                                                   datatype, nghosts, ndims, ni, nj, nk, &
       &                                                   icoords, jcoords, kcoords             )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:),               intent(out) :: icoords
    real(kind=8), dimension(:),     optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, kcoords, n, err     )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, dummy, n, err       )
      
    else
      
      n = 1
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, dummy, dummy, n, err         )
      
    end if
    
    
    
    ReadQuadMeshChunk_1D_r8 = err
  end function ReadQuadMeshChunk_1D_r8
  integer(kind=TIO_ERRK) function ReadQuadMeshChunk_2D_r4( fileID, meshID, idx, xfer,            &
       &                                                   datatype, nghosts, ndims, ni, nj, nk, &
       &                                                   icoords, jcoords, kcoords             )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:,:),             intent(out) :: icoords
    real(kind=4), dimension(:,:),   optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, kcoords, n, err     )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, dummy, n, err       )
      
    else
      
      n = 1
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, dummy, dummy, n, err         )
      
    end if
    
    
    
    ReadQuadMeshChunk_2D_r4 = err
  end function ReadQuadMeshChunk_2D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshChunk_2D_r8( fileID, meshID, idx, xfer,            &
       &                                                   datatype, nghosts, ndims, ni, nj, nk, &
       &                                                   icoords, jcoords, kcoords             )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:,:),             intent(out) :: icoords
    real(kind=8), dimension(:,:),   optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, kcoords, n, err     )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, dummy, n, err       )
      
    else
      
      n = 1
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, dummy, dummy, n, err         )
      
    end if
    
    
    
    ReadQuadMeshChunk_2D_r8 = err
  end function ReadQuadMeshChunk_2D_r8
  integer(kind=TIO_ERRK) function ReadQuadMeshChunk_3D_r4( fileID, meshID, idx, xfer,            &
       &                                                   datatype, nghosts, ndims, ni, nj, nk, &
       &                                                   icoords, jcoords, kcoords             )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:,:,:),           intent(out) :: icoords
    real(kind=4), dimension(:,:,:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, kcoords, n, err     )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, dummy, n, err       )
      
    else
      
      n = 1
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, dummy, dummy, n, err         )
      
    end if
    
    
    
    ReadQuadMeshChunk_3D_r4 = err
  end function ReadQuadMeshChunk_3D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshChunk_3D_r8( fileID, meshID, idx, xfer,            &
       &                                                   datatype, nghosts, ndims, ni, nj, nk, &
       &                                                   icoords, jcoords, kcoords             )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:,:,:),           intent(out) :: icoords
    real(kind=8), dimension(:,:,:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, kcoords, n, err     )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, jcoords, dummy, n, err       )
      
    else
      
      n = 1
      call tio_read_quadmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                          nghosts, ndims, ni, nj, nk,           &
           &                          icoords, dummy, dummy, n, err         )
      
    end if
    
    
    
    ReadQuadMeshChunk_3D_r8 = err
  end function ReadQuadMeshChunk_3D_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadMesh_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadMesh_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadMesh_Section_f( fileID, meshID, sectionID,
  ! <DATA>                                                              xfer, datatype,
  ! <DATA>                                                              ndims, ni, nj, nk,
  ! <DATA>                                                              icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),    intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),     intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_OBJK),     intent(in)            :: sectionID
  ! <DATA>   integer(kind=TIO_IPK),      intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),      intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_IPK),      intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),    intent(out)           :: ni, nj, nk
  ! <DATA>   [REAL CHOICE], [DIMENSION], intent(out)           :: icoords
  ! <DATA>   [REAL CHOICE], [DIMENSION], intent(out), optional :: jcoords, kcoords
  ! </FAPI>
  ! <DATA> DIMENSION is dimension(:) for colinear meshes, or dimension(:), dimension(:,:), or 
  ! <DATA> dimension(:,:,:) for non-colinear.
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadQuadMeshSection_meta(  fileID, meshID, sectionID,          &
       &                                                     xfer, datatype, ndims, ni, nj, nk   )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    n = -1
    
    call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,     &
         &                            ndims, ni, nj, nk, dummy, dummy, dummy, n, err )
    
    
    
    
    
    ReadQuadMeshSection_meta = err
  end function ReadQuadMeshSection_meta
  integer(kind=TIO_ERRK) function ReadQuadMeshSection_1D_r4( fileID, meshID, sectionID,          &
       &                                                     xfer, datatype, ndims, ni, nj, nk,  &
       &                                                     icoords, jcoords, kcoords           )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:),               intent(out) :: icoords
    real(kind=4), dimension(:),     optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,           &
           &                            ndims, ni, nj, nk, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,         &
           &                            ndims, ni, nj, nk, icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                            ndims, ni, nj, nk, icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshSection_1D_r4 = err
  end function ReadQuadMeshSection_1D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshSection_1D_r8( fileID, meshID, sectionID,          &
       &                                                     xfer, datatype, ndims, ni, nj, nk,  &
       &                                                     icoords, jcoords, kcoords           )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:),               intent(out) :: icoords
    real(kind=8), dimension(:),     optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,           &
           &                            ndims, ni, nj, nk, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,         &
           &                            ndims, ni, nj, nk, icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                            ndims, ni, nj, nk, icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshSection_1D_r8 = err
  end function ReadQuadMeshSection_1D_r8
  integer(kind=TIO_ERRK) function ReadQuadMeshSection_2D_r4( fileID, meshID, sectionID,          &
       &                                                     xfer, datatype, ndims, ni, nj, nk,  &
       &                                                     icoords, jcoords, kcoords           )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:,:),             intent(out) :: icoords
    real(kind=4), dimension(:,:),   optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,           &
           &                            ndims, ni, nj, nk, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,         &
           &                            ndims, ni, nj, nk, icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                            ndims, ni, nj, nk, icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshSection_2D_r4 = err
  end function ReadQuadMeshSection_2D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshSection_2D_r8( fileID, meshID, sectionID,          &
       &                                                     xfer, datatype, ndims, ni, nj, nk,  &
       &                                                     icoords, jcoords, kcoords           )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:,:),             intent(out) :: icoords
    real(kind=8), dimension(:,:),   optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,           &
           &                            ndims, ni, nj, nk, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,         &
           &                            ndims, ni, nj, nk, icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                            ndims, ni, nj, nk, icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshSection_2D_r8 = err
  end function ReadQuadMeshSection_2D_r8
  integer(kind=TIO_ERRK) function ReadQuadMeshSection_3D_r4( fileID, meshID, sectionID,          &
       &                                                     xfer, datatype, ndims, ni, nj, nk,  &
       &                                                     icoords, jcoords, kcoords           )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:,:,:),           intent(out) :: icoords
    real(kind=4), dimension(:,:,:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,           &
           &                            ndims, ni, nj, nk, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,         &
           &                            ndims, ni, nj, nk, icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                            ndims, ni, nj, nk, icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshSection_3D_r4 = err
  end function ReadQuadMeshSection_3D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshSection_3D_r8( fileID, meshID, sectionID,          &
       &                                                     xfer, datatype, ndims, ni, nj, nk,  &
       &                                                     icoords, jcoords, kcoords           )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:,:,:),           intent(out) :: icoords
    real(kind=8), dimension(:,:,:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,           &
           &                            ndims, ni, nj, nk, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,         &
           &                            ndims, ni, nj, nk, icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                            ndims, ni, nj, nk, icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshSection_3D_r8 = err
  end function ReadQuadMeshSection_3D_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_QuadMesh_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_QuadMesh_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_QuadMesh_All_f( fileID, meshID, datatype,
  ! <DATA>                                                          ndims, ni, nj, nk,
  ! <DATA>                                                          icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),               intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                intent(in)  :: meshID
  ! <DATA>   integer(kind=TIO_IPK),                 intent(in)  :: datatype
  ! <DATA>   integer(kind=TIO_IPK),                 intent(out) :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),               intent(out) :: ni, nj, nk
  ! <DATA>   [REAL CHOICE], dimension(:),           intent(out) :: icoords
  ! <DATA>   [REAL CHOICE], dimension(:), optional, intent(out) :: jcoords, kcoords
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadQuadMeshAll_meta(  fileID, meshID, datatype, ndims,   &
       &                                                 ni, nj, nk                         )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    


    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    


    n = -1
    
    call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
         &                        dummy, dummy, dummy, n, err )
    
    
    
    
    
    ReadQuadMeshAll_meta = err
  end function ReadQuadMeshAll_meta
  integer(kind=TIO_ERRK) function ReadQuadMeshAll_1D_r4( fileID, meshID, datatype, ndims,   &
       &                                                 ni, nj, nk,                        &
       &                                                 icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:),               intent(out) :: icoords
    real(kind=4), dimension(:),     optional, intent(out) :: jcoords, kcoords


    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshAll_1D_r4 = err
  end function ReadQuadMeshAll_1D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshAll_1D_r8( fileID, meshID, datatype, ndims,   &
       &                                                 ni, nj, nk,                        &
       &                                                 icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:),               intent(out) :: icoords
    real(kind=8), dimension(:),     optional, intent(out) :: jcoords, kcoords


    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshAll_1D_r8 = err
  end function ReadQuadMeshAll_1D_r8
  integer(kind=TIO_ERRK) function ReadQuadMeshAll_2D_r4( fileID, meshID, datatype, ndims,   &
       &                                                 ni, nj, nk,                        &
       &                                                 icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:,:),             intent(out) :: icoords
    real(kind=4), dimension(:,:),   optional, intent(out) :: jcoords, kcoords


    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshAll_2D_r4 = err
  end function ReadQuadMeshAll_2D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshAll_2D_r8( fileID, meshID, datatype, ndims,   &
       &                                                 ni, nj, nk,                        &
       &                                                 icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:,:),             intent(out) :: icoords
    real(kind=8), dimension(:,:),   optional, intent(out) :: jcoords, kcoords


    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshAll_2D_r8 = err
  end function ReadQuadMeshAll_2D_r8
  integer(kind=TIO_ERRK) function ReadQuadMeshAll_3D_r4( fileID, meshID, datatype, ndims,   &
       &                                                 ni, nj, nk,                        &
       &                                                 icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=4), dimension(:,:,:),           intent(out) :: icoords
    real(kind=4), dimension(:,:,:), optional, intent(out) :: jcoords, kcoords


    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshAll_3D_r4 = err
  end function ReadQuadMeshAll_3D_r4
  integer(kind=TIO_ERRK) function ReadQuadMeshAll_3D_r8( fileID, meshID, datatype, ndims,   &
       &                                                 ni, nj, nk,                        &
       &                                                 icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: ni, nj, nk
    
    real(kind=8), dimension(:,:,:),           intent(out) :: icoords
    real(kind=8), dimension(:,:,:), optional, intent(out) :: jcoords, kcoords


    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_quadmesh_all_c( fileID, meshID, datatype, ndims, ni, nj, nk,  &
           &                        icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadQuadMeshAll_3D_r8 = err
  end function ReadQuadMeshAll_3D_r8
  
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **          UNSTRMESHES           ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_UnstrMesh_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_UnstrMesh_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_UnstrMesh_Chunk_f( fileID, meshID, idx, xfer,
  ! <DATA>                                                              graph_datatype, coord_datatype,
  ! <DATA>                                                              nodeIDs, cellIDs, shapes,
  ! <DATA>                                                              ncells_per_shape, connectivity,
  ! <DATA>                                                              icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),                intent(in) :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),                 intent(in) :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK),                intent(in) :: idx
  ! <DATA>   integer(kind=TIO_IPK),                  intent(in) :: xfer
  ! <DATA>   integer(kind=TIO_IPK),                  intent(in) :: graph_datatype, coord_datatype
  ! <DATA>   integer(kind=TIO_SHAPEK), dimension(:), intent(in) :: shapes
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(in) :: nodeIDs, cellIDs
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(in) :: ncells_per_shape
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(in) :: connectivity
  ! <DATA>   [REAL CHOICE], dimension(:), intent(in)            :: icoords
  ! <DATA>   [REAL CHOICE], dimension(:), intent(in), optional  :: jcoords, kcoords
  ! </FAPI>
  ! <FDES>
  ! <DATA> All [INTEGER CHOICE] must be the same, likewise for the [REAL CHOICE].
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function WriteUnstrMeshChunk_i4g_r4c( fileID, meshID, idx, xfer,   &
       &                                             graph_datatype, coord_datatype,           &
       &                                             nodeIDs, cellIDs, shapes,                 &
       &                                             ncells_per_shape, connectivity,           &
       &                                             icoords, jcoords, kcoords              )  &
       &                          result (res)
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in) :: fileID
    integer(kind=TIO_OBJK),                 intent(in) :: meshID
    integer(kind=TIO_SIZEK),                intent(in) :: idx
    integer(kind=TIO_IPK),                  intent(in) :: xfer
    integer(kind=TIO_IPK),                  intent(in) :: graph_datatype, coord_datatype
    integer(kind=TIO_SHAPEK), dimension(:), intent(in) :: shapes


    integer(kind=4), dimension(:), intent(in)            :: nodeIDs, cellIDs
    integer(kind=4), dimension(:), intent(in)            :: ncells_per_shape, connectivity


    real(kind=4),    dimension(:), intent(in)            :: icoords
    real(kind=4),    dimension(:), intent(in), optional  :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_UnstrMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case(n)
    case (3)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, jcoords, kcoords, n, err)
      
    case (2)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, jcoords, dummy, n, err)
    case (1)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, dummy, dummy, n, err)
    end select
    
    res = err


  end function WriteUnstrMeshChunk_i4g_r4c
  integer(kind=TIO_ERRK) function WriteUnstrMeshChunk_i8g_r4c( fileID, meshID, idx, xfer,   &
       &                                             graph_datatype, coord_datatype,           &
       &                                             nodeIDs, cellIDs, shapes,                 &
       &                                             ncells_per_shape, connectivity,           &
       &                                             icoords, jcoords, kcoords              )  &
       &                          result (res)
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in) :: fileID
    integer(kind=TIO_OBJK),                 intent(in) :: meshID
    integer(kind=TIO_SIZEK),                intent(in) :: idx
    integer(kind=TIO_IPK),                  intent(in) :: xfer
    integer(kind=TIO_IPK),                  intent(in) :: graph_datatype, coord_datatype
    integer(kind=TIO_SHAPEK), dimension(:), intent(in) :: shapes


    integer(kind=8), dimension(:), intent(in)            :: nodeIDs, cellIDs
    integer(kind=8), dimension(:), intent(in)            :: ncells_per_shape, connectivity


    real(kind=4),    dimension(:), intent(in)            :: icoords
    real(kind=4),    dimension(:), intent(in), optional  :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_UnstrMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case(n)
    case (3)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, jcoords, kcoords, n, err)
      
    case (2)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, jcoords, dummy, n, err)
    case (1)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, dummy, dummy, n, err)
    end select
    
    res = err


  end function WriteUnstrMeshChunk_i8g_r4c
  integer(kind=TIO_ERRK) function WriteUnstrMeshChunk_i4g_r8c( fileID, meshID, idx, xfer,   &
       &                                             graph_datatype, coord_datatype,           &
       &                                             nodeIDs, cellIDs, shapes,                 &
       &                                             ncells_per_shape, connectivity,           &
       &                                             icoords, jcoords, kcoords              )  &
       &                          result (res)
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in) :: fileID
    integer(kind=TIO_OBJK),                 intent(in) :: meshID
    integer(kind=TIO_SIZEK),                intent(in) :: idx
    integer(kind=TIO_IPK),                  intent(in) :: xfer
    integer(kind=TIO_IPK),                  intent(in) :: graph_datatype, coord_datatype
    integer(kind=TIO_SHAPEK), dimension(:), intent(in) :: shapes


    integer(kind=4), dimension(:), intent(in)            :: nodeIDs, cellIDs
    integer(kind=4), dimension(:), intent(in)            :: ncells_per_shape, connectivity


    real(kind=8),    dimension(:), intent(in)            :: icoords
    real(kind=8),    dimension(:), intent(in), optional  :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_UnstrMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case(n)
    case (3)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, jcoords, kcoords, n, err)
      
    case (2)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, jcoords, dummy, n, err)
    case (1)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, dummy, dummy, n, err)
    end select
    
    res = err


  end function WriteUnstrMeshChunk_i4g_r8c
  integer(kind=TIO_ERRK) function WriteUnstrMeshChunk_i8g_r8c( fileID, meshID, idx, xfer,      &
       &                                             graph_datatype, coord_datatype,           &
       &                                             nodeIDs, cellIDs, shapes,                 &
       &                                             ncells_per_shape, connectivity,           &
       &                                             icoords, jcoords, kcoords              )  &
       &                          result (res)
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in) :: fileID
    integer(kind=TIO_OBJK),                 intent(in) :: meshID
    integer(kind=TIO_SIZEK),                intent(in) :: idx
    integer(kind=TIO_IPK),                  intent(in) :: xfer
    integer(kind=TIO_IPK),                  intent(in) :: graph_datatype, coord_datatype
    integer(kind=TIO_SHAPEK), dimension(:), intent(in) :: shapes


    integer(kind=8), dimension(:), intent(in)            :: nodeIDs, cellIDs
    integer(kind=8), dimension(:), intent(in)            :: ncells_per_shape, connectivity


    real(kind=8),    dimension(:), intent(in)            :: icoords
    real(kind=8),    dimension(:), intent(in), optional  :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_UnstrMesh_Chunk_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case(n)
    case (3)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, jcoords, kcoords, n, err)
      
    case (2)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, jcoords, dummy, n, err)
    case (1)
      call tio_write_unstrmesh_chunk_c(fileID, meshID, idx, xfer,                                 &
           &                           graph_datatype, coord_datatype,                            &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                           icoords, dummy, dummy, n, err)
    end select
    
    res = err


  end function WriteUnstrMeshChunk_i8g_r8c
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrMesh_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrMesh_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrMesh_Chunk_f( fileID, meshID, idx, xfer,
  ! <DATA>                             graph_datatype, coord_datatype, nghosts,
  ! <DATA>                             ndims, nnodes, ncells, nshapes, nconnectivity,
  ! <DATA>                             nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,
  ! <DATA>                             icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: idx
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)  :: xfer
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
  ! <DATA>   integer(kind=TIO_SIZEK), intent(in)  :: nghosts
  ! <DATA>   integer(kind=TIO_IPK),   intent(out) :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(out)           :: nodeIDs, cellIDs
  ! <DATA>   integer(kind=TIO_SHAPEK), dimension(:), intent(out)           :: shapes
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(out)           :: ncells_per_shape
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(out)           :: connectivity
  ! <DATA>   [REAL CHOICE],            dimension(:), intent(out)           :: icoords
  ! <DATA>   [REAL CHOICE],            dimension(:), intent(out), optional :: jcoords, kcoords
  ! </FAPI>
  ! <FDES>
  ! <DATA> All [INTEGER CHOICE] must be the same, likewise for the [REAL CHOICE].
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadUnstrMeshChunk_meta(    fileID, meshID, idx, xfer,         &
       &                                         graph_datatype, coord_datatype, nghosts,        &
       &                                         ndims, nnodes, ncells, nshapes, nconnectivity   )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    


    n = -1
    
    call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
         &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
         &                           dummy, dummy, dummy, dummy, dummy,  dummy, dummy, dummy,    &
         &                           n, err )


    
    
    
    
    ReadUnstrMeshChunk_meta = err
  end function ReadUnstrMeshChunk_meta
  integer(kind=TIO_ERRK) function ReadUnstrMeshChunk_i4g_r4c( fileID, meshID, idx, xfer,         &
       &                                         graph_datatype, coord_datatype, nghosts,        &
       &                                         ndims, nnodes, ncells, nshapes, nconnectivity,  &
       &                                         nodeIDs, cellIDs, shapes,                       &
       &                                         ncells_per_shape, connectivity,                 &
       &                                         icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    integer(kind=4),          dimension(:),           intent(out) :: nodeIDs, cellIDs
    integer(kind=TIO_SHAPEK), dimension(:),           intent(out) :: shapes
    integer(kind=4),          dimension(:),           intent(out) :: ncells_per_shape, connectivity
    real(kind=4),             dimension(:),           intent(out) :: icoords
    real(kind=4),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadUnstrMeshChunk_i4g_r4c = err
  end function ReadUnstrMeshChunk_i4g_r4c
  integer(kind=TIO_ERRK) function ReadUnstrMeshChunk_i8g_r4c( fileID, meshID, idx, xfer,         &
       &                                         graph_datatype, coord_datatype, nghosts,        &
       &                                         ndims, nnodes, ncells, nshapes, nconnectivity,  &
       &                                         nodeIDs, cellIDs, shapes,                       &
       &                                         ncells_per_shape, connectivity,                 &
       &                                         icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    integer(kind=8),          dimension(:),           intent(out) :: nodeIDs, cellIDs
    integer(kind=TIO_SHAPEK), dimension(:),           intent(out) :: shapes
    integer(kind=8),          dimension(:),           intent(out) :: ncells_per_shape, connectivity
    real(kind=4),             dimension(:),           intent(out) :: icoords
    real(kind=4),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadUnstrMeshChunk_i8g_r4c = err
  end function ReadUnstrMeshChunk_i8g_r4c
  integer(kind=TIO_ERRK) function ReadUnstrMeshChunk_i4g_r8c( fileID, meshID, idx, xfer,         &
       &                                         graph_datatype, coord_datatype, nghosts,        &
       &                                         ndims, nnodes, ncells, nshapes, nconnectivity,  &
       &                                         nodeIDs, cellIDs, shapes,                       &
       &                                         ncells_per_shape, connectivity,                 &
       &                                         icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    integer(kind=4),          dimension(:),           intent(out) :: nodeIDs, cellIDs
    integer(kind=TIO_SHAPEK), dimension(:),           intent(out) :: shapes
    integer(kind=4),          dimension(:),           intent(out) :: ncells_per_shape, connectivity
    real(kind=8),             dimension(:),           intent(out) :: icoords
    real(kind=8),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadUnstrMeshChunk_i4g_r8c = err
  end function ReadUnstrMeshChunk_i4g_r8c
  integer(kind=TIO_ERRK) function ReadUnstrMeshChunk_i8g_r8c( fileID, meshID, idx, xfer,         &
       &                                         graph_datatype, coord_datatype, nghosts,        &
       &                                         ndims, nnodes, ncells, nshapes, nconnectivity,  &
       &                                         nodeIDs, cellIDs, shapes,                       &
       &                                         ncells_per_shape, connectivity,                 &
       &                                         icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    integer(kind=8),          dimension(:),           intent(out) :: nodeIDs, cellIDs
    integer(kind=TIO_SHAPEK), dimension(:),           intent(out) :: shapes
    integer(kind=8),          dimension(:),           intent(out) :: ncells_per_shape, connectivity
    real(kind=8),             dimension(:),           intent(out) :: icoords
    real(kind=8),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_unstrmesh_chunk_c( fileID, meshID, idx, xfer, graph_datatype, coord_datatype,  &
           &                           nghosts, ndims, nnodes, ncells, nshapes, nconnectivity,     &
           &                           nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,   &
           &                           icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadUnstrMeshChunk_i8g_r8c = err
  end function ReadUnstrMeshChunk_i8g_r8c
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrMesh_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrMesh_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrMesh_All_f( fileID, meshID,
  ! <DATA>                             graph_datatype, coord_datatype,
  ! <DATA>                             ndims, nnodes, ncells, nshapes, nconnectivity,
  ! <DATA>                             nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,
  ! <DATA>                             icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK), intent(in)  :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),  intent(in)  :: meshID
  ! <DATA>   integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
  ! <DATA>   integer(kind=TIO_IPK),   intent(out) :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(out)           :: nodeIDs, cellIDs
  ! <DATA>   integer(kind=TIO_SHAPEK), dimension(:), intent(out)           :: shapes
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(out)           :: ncells_per_shape
  ! <DATA>   [INTEGER CHOICE],         dimension(:), intent(out)           :: connectivity
  ! <DATA>   [REAL CHOICE],            dimension(:), intent(out)           :: icoords
  ! <DATA>   [REAL CHOICE],            dimension(:), intent(out), optional :: jcoords, kcoords
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function ReadUnstrMeshAll_meta( fileID, meshID,      &
       &                                                 graph_datatype, coord_datatype, ndims,   &
       &                                                 nnodes, ncells, nshapes, nconnectivity   )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    


    n = -1
    
    call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,           &
         &                         ndims, nnodes, ncells, nshapes, nconnectivity,            &
         &                         dummy, dummy, dummy, dummy, dummy,  dummy, dummy, dummy,  &
         &                         n, err )


    
    
    
    
    ReadUnstrMeshAll_meta = err
  end function ReadUnstrMeshAll_meta
  integer(kind=TIO_ERRK) function ReadUnstrMeshAll_i4g_r4c( fileID, meshID,   &
       &                                                graph_datatype, coord_datatype, ndims,   &
       &                                                nnodes, ncells, nshapes, nconnectivity,  &
       &                                                nodeIDs, cellIDs, shapes,                &
       &                                                ncells_per_shape, connectivity,          &
       &                                                icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    integer(kind=4),          dimension(:),           intent(out) :: nodeIDs, cellIDs
    integer(kind=TIO_SHAPEK), dimension(:),           intent(out) :: shapes
    integer(kind=4),          dimension(:),           intent(out) :: ncells_per_shape, connectivity
    real(kind=4),             dimension(:),           intent(out) :: icoords
    real(kind=4),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadUnstrMeshAll_i4g_r4c = err
  end function ReadUnstrMeshAll_i4g_r4c
  integer(kind=TIO_ERRK) function ReadUnstrMeshAll_i8g_r4c( fileID, meshID,   &
       &                                                graph_datatype, coord_datatype, ndims,   &
       &                                                nnodes, ncells, nshapes, nconnectivity,  &
       &                                                nodeIDs, cellIDs, shapes,                &
       &                                                ncells_per_shape, connectivity,          &
       &                                                icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    integer(kind=8),          dimension(:),           intent(out) :: nodeIDs, cellIDs
    integer(kind=TIO_SHAPEK), dimension(:),           intent(out) :: shapes
    integer(kind=8),          dimension(:),           intent(out) :: ncells_per_shape, connectivity
    real(kind=4),             dimension(:),           intent(out) :: icoords
    real(kind=4),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadUnstrMeshAll_i8g_r4c = err
  end function ReadUnstrMeshAll_i8g_r4c
  integer(kind=TIO_ERRK) function ReadUnstrMeshAll_i4g_r8c( fileID, meshID,   &
       &                                                graph_datatype, coord_datatype, ndims,   &
       &                                                nnodes, ncells, nshapes, nconnectivity,  &
       &                                                nodeIDs, cellIDs, shapes,                &
       &                                                ncells_per_shape, connectivity,          &
       &                                                icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    integer(kind=4),          dimension(:),           intent(out) :: nodeIDs, cellIDs
    integer(kind=TIO_SHAPEK), dimension(:),           intent(out) :: shapes
    integer(kind=4),          dimension(:),           intent(out) :: ncells_per_shape, connectivity
    real(kind=8),             dimension(:),           intent(out) :: icoords
    real(kind=8),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadUnstrMeshAll_i4g_r8c = err
  end function ReadUnstrMeshAll_i4g_r8c
  integer(kind=TIO_ERRK) function ReadUnstrMeshAll_i8g_r8c( fileID, meshID,   &
       &                                                graph_datatype, coord_datatype, ndims,   &
       &                                                nnodes, ncells, nshapes, nconnectivity,  &
       &                                                nodeIDs, cellIDs, shapes,                &
       &                                                ncells_per_shape, connectivity,          &
       &                                                icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: graph_datatype, coord_datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes, ncells, nshapes, nconnectivity
    
    integer(kind=8),          dimension(:),           intent(out) :: nodeIDs, cellIDs
    integer(kind=TIO_SHAPEK), dimension(:),           intent(out) :: shapes
    integer(kind=8),          dimension(:),           intent(out) :: ncells_per_shape, connectivity
    real(kind=8),             dimension(:),           intent(out) :: icoords


    real(kind=8),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, jcoords, dummy, n, err )
      
    else
      
      n = 1
      call tio_read_unstrmesh_all_c( fileID, meshID, graph_datatype, coord_datatype,            &
           &                         ndims, nnodes, ncells, nshapes, nconnectivity,             &
           &                         nodeIDs, cellIDs, shapes, ncells_per_shape, connectivity,  &
           &                         icoords, dummy, dummy, n, err )
      
    end if
    
    
    
    ReadUnstrMeshAll_i8g_r8c = err
  end function ReadUnstrMeshAll_i8g_r8c
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_UnstrMesh_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_UnstrMesh_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_UnstrMesh_Section_f( fileID, meshID, sectionID,
  ! <DATA>                                                               xfer, datatype, ndims, nnodes,
  ! <DATA>                                                               icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),     intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),      intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_OBJK),      intent(in)            :: sectionID
  ! <DATA>   integer(kind=TIO_IPK),       intent(in)            :: xfer, datatype
  ! <DATA>   integer(kind=TIO_IPK),       intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),     intent(out)           :: nnodes
  ! <DATA>   [REAL CHOICE], dimension(:), intent(out), optional :: icoords, jcoords, kcoords
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_meta( fileID, meshID, sectionID, xfer, datatype,  &
       & ndims, nnodes )


!!$!TPP CASE 4
!!$  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_i4g_r8c( fileID, meshID, sectionID, xfer, datatype,  &
!!$       & ndims, nnodes, icoords, jcoords, kcoords i4, r8)
!!$    integer(kind=4) :: i4
!!$    real(kind=8)    :: r8
!!$!TPP ESAC
!!$!TPP CASE 5
!!$  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_i8g_r8c( fileID, meshID, sectionID, xfer, datatype,  &
!!$       & ndims, nnodes, icoords, jcoords, kcoords i8, r8)
!!$    integer(kind=8) :: i8
!!$    real(kind=8)    :: r8
!!$!TPP ESAC
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err = TIO_FAIL_F
    
    
     


    n = -1
    
    call tio_read_unstrmesh_section_c( fileID, meshID, sectionID, xfer, datatype, &
         &                             ndims, nnodes, dummy, dummy, dummy, n, err )


    
    
    
    
!!$!TPP CASE 5
!!$    ReadUnstrMeshSection_i8g_r8c = err
!!$  end function ReadUnstrMeshSection_i8g_r8c
!!$!TPP ESAC  
!!$!TPP CASE 4
!!$    ReadUnstrMeshSection_i4g_r8c = err
!!$  end function ReadUnstrMeshSection_i4g_r8c
!!$!TPP ESAC  


    ReadUnstrMeshSection_meta = err
  end function ReadUnstrMeshSection_meta
  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_r4c( fileID, meshID, sectionID, xfer, datatype,  &
       & ndims, nnodes, icoords, jcoords, kcoords )
    
    real(kind=4), dimension(:), intent(out)           :: icoords
    real(kind=4), dimension(:), intent(out), optional :: jcoords, kcoords
    


!!$!TPP CASE 4
!!$  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_i4g_r8c( fileID, meshID, sectionID, xfer, datatype,  &
!!$       & ndims, nnodes, icoords, jcoords, kcoords i4, r8)
!!$    integer(kind=4) :: i4
!!$    real(kind=8)    :: r8
!!$!TPP ESAC
!!$!TPP CASE 5
!!$  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_i8g_r8c( fileID, meshID, sectionID, xfer, datatype,  &
!!$       & ndims, nnodes, icoords, jcoords, kcoords i8, r8)
!!$    integer(kind=8) :: i8
!!$    real(kind=8)    :: r8
!!$!TPP ESAC
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err = TIO_FAIL_F
    
    
     
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, jcoords, dummy, n, err   )
      
    else
      
      n = 1
      call tio_read_unstrmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, dummy, dummy, n, err     )
      
    end if
    
    
    
!!$!TPP CASE 5
!!$    ReadUnstrMeshSection_i8g_r8c = err
!!$  end function ReadUnstrMeshSection_i8g_r8c
!!$!TPP ESAC  
!!$!TPP CASE 4
!!$    ReadUnstrMeshSection_i4g_r8c = err
!!$  end function ReadUnstrMeshSection_i4g_r8c
!!$!TPP ESAC  


    ReadUnstrMeshSection_r4c = err
  end function ReadUnstrMeshSection_r4c
  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_r8c( fileID, meshID, sectionID, xfer, datatype,  &
       & ndims, nnodes, icoords, jcoords, kcoords )


    real(kind=8), dimension(:), intent(out)           :: icoords
    real(kind=8), dimension(:), intent(out), optional :: jcoords, kcoords
    


!!$!TPP CASE 4
!!$  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_i4g_r8c( fileID, meshID, sectionID, xfer, datatype,  &
!!$       & ndims, nnodes, icoords, jcoords, kcoords i4, r8)
!!$    integer(kind=4) :: i4
!!$    real(kind=8)    :: r8
!!$!TPP ESAC
!!$!TPP CASE 5
!!$  integer(kind=TIO_ERRK) function ReadUnstrMeshSection_i8g_r8c( fileID, meshID, sectionID, xfer, datatype,  &
!!$       & ndims, nnodes, icoords, jcoords, kcoords i8, r8)
!!$    integer(kind=8) :: i8
!!$    real(kind=8)    :: r8
!!$!TPP ESAC
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err = TIO_FAIL_F
    
    
     
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_unstrmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_unstrmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, jcoords, dummy, n, err   )
      
    else
      
      n = 1
      call tio_read_unstrmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, dummy, dummy, n, err     )
      
    end if
    
    
    
!!$!TPP CASE 5
!!$    ReadUnstrMeshSection_i8g_r8c = err
!!$  end function ReadUnstrMeshSection_i8g_r8c
!!$!TPP ESAC  
!!$!TPP CASE 4
!!$    ReadUnstrMeshSection_i4g_r8c = err
!!$  end function ReadUnstrMeshSection_i4g_r8c
!!$!TPP ESAC  


    ReadUnstrMeshSection_r8c = err
  end function ReadUnstrMeshSection_r8c
  
  
  
  
  ! ************************************ !
  ! ************************************ !
  ! **                                ** !
  ! **                                ** !
  ! **          POINTMESHES           ** !
  ! **                                ** !
  ! **                                ** !
  ! ************************************ !
  ! ************************************ !
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Write_PointMesh_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Write_PointMesh_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Write_PointMesh_Chunk_f( fileID, meshID, idx, xfer,
  ! <DATA>                                                    datatype, icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),     intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),      intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK),     intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),       intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),       intent(in)            :: datatype
  ! <DATA>   [REAL CHOICE], dimension(:), intent(in)            :: icoords
  ! <DATA>   [REAL CHOICE], dimension(:), intent(in), optional  :: jcoords, kcoords
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function WritePointMeshChunk_r4( fileID, meshID, idx, xfer,            &
       &                                                  datatype, icoords, jcoords, kcoords )  &
       &                          result (res)
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in) :: fileID
    integer(kind=TIO_OBJK),                 intent(in) :: meshID
    integer(kind=TIO_SIZEK),                intent(in) :: idx
    integer(kind=TIO_IPK),                  intent(in) :: xfer
    integer(kind=TIO_IPK),                  intent(in) :: datatype
    
    real(kind=4),    dimension(:), intent(in)            :: icoords
    real(kind=4),    dimension(:), intent(in), optional  :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_All_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case(n)
    case (3)
      call tio_write_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                            icoords, jcoords, kcoords, n, err     )
    case (2)
      call tio_write_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                            icoords, jcoords, dummy, n, err       )
    case (1)
      call tio_write_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                            icoords, dummy, dummy, n, err         )
    end select
    
    res = err
    
  end function WritePointMeshChunk_r4
  integer(kind=TIO_ERRK) function WritePointMeshChunk_r8( fileID, meshID, idx, xfer,             &
       &                                                  datatype, icoords, jcoords, kcoords )  &
       &                          result (res)
    
    implicit none
    
    integer(kind=TIO_FILEK),                intent(in) :: fileID
    integer(kind=TIO_OBJK),                 intent(in) :: meshID
    integer(kind=TIO_SIZEK),                intent(in) :: idx
    integer(kind=TIO_IPK),                  intent(in) :: xfer
    integer(kind=TIO_IPK),                  intent(in) :: datatype
    
    real(kind=8),    dimension(:), intent(in)            :: icoords
    real(kind=8),    dimension(:), intent(in), optional  :: jcoords, kcoords
    
    character              :: dummy
    integer(kind=TIO_IPK)  :: lndims
    integer                :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    err = TIO_Get_Mesh_Info_f(fileID, meshID, ndims=lndims)
    if (err /= TIO_SUCCESS_F) return
    
    if (present(kcoords)) then
      n = 3
    else if (present(jcoords)) then
      n = 2
    else
      n = 1
    end if
    
    ! If not enough coord arrays specified, error
    if (lndims > n) then
      call seterror("TIO_Write_QuadMesh_All_f", "Not enough optional coord arrays specified")
      res = TIO_ERR_F90_API_F
      return
    end if
    
    ! If too many specified, just ignore latter ones
    n = min(n,lndims)
    
    select case(n)
    case (3)
      call tio_write_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                            icoords, jcoords, kcoords, n, err     )
    case (2)
      call tio_write_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                            icoords, jcoords, dummy, n, err       )
    case (1)
      call tio_write_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,  &
           &                            icoords, dummy, dummy, n, err         )
    end select
    
    res = err
    
  end function WritePointMeshChunk_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointMesh_Chunk_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointMesh_Chunk
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointMesh_Chunk_f( fileID, meshID, idx, xfer,
  ! <DATA>                                                      datatype, nghosts, ndims, nnodes,
  ! <DATA>                                                      icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),     intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),      intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_SIZEK),     intent(in)            :: idx
  ! <DATA>   integer(kind=TIO_IPK),       intent(in)            :: xfer
  ! <DATA>   integer(kind=TIO_IPK),       intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_SIZEK),     intent(in)            :: nghosts
  ! <DATA>   integer(kind=TIO_IPK),       intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),     intent(out)           :: nnodes
  ! <DATA>   [REAL CHOICE], dimension(:), intent(out)           :: icoords
  ! <DATA>   [REAL CHOICE], dimension(:), intent(out), optional :: jcoords, kcoords
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !


  integer(kind=TIO_ERRK) function ReadPointMeshChunk_meta( fileID, meshID, idx, xfer,        &
       &                                                   datatype, nghosts, ndims, nnodes  )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    


    n = -1
    
    call tio_read_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,                &
         &                           nghosts, ndims, nnodes, dummy, dummy, dummy, n, err )


    
    
    
    
    ReadPointMeshChunk_meta = err
  end function ReadPointMeshChunk_meta
  integer(kind=TIO_ERRK) function ReadPointMeshChunk_r4(   fileID, meshID, idx, xfer,        &
       &                                                   datatype, nghosts, ndims, nnodes, &
       &                                                   icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    real(kind=4),             dimension(:),           intent(out) :: icoords
    real(kind=4),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,                      &
           &                           nghosts, ndims, nnodes, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,                      &
           &                           nghosts, ndims, nnodes, icoords, jcoords, dummy, n, err   )
      
    else
      
      n = 1
      call tio_read_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,                      &
           &                           nghosts, ndims, nnodes, icoords, dummy, dummy, n, err     )
      
    end if
    
    
    
    ReadPointMeshChunk_r4 = err
  end function ReadPointMeshChunk_r4
  integer(kind=TIO_ERRK) function ReadPointMeshChunk_r8(   fileID, meshID, idx, xfer,        &
       &                                                   datatype, nghosts, ndims, nnodes, &
       &                                                   icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_SIZEK), intent(in)  :: idx
    integer(kind=TIO_IPK),   intent(in)  :: xfer
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_SIZEK), intent(in)  :: nghosts
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    real(kind=8),             dimension(:),           intent(out) :: icoords
    real(kind=8),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,                      &
           &                           nghosts, ndims, nnodes, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,                      &
           &                           nghosts, ndims, nnodes, icoords, jcoords, dummy, n, err   )
      
    else
      
      n = 1
      call tio_read_pointmesh_chunk_c( fileID, meshID, idx, xfer, datatype,                      &
           &                           nghosts, ndims, nnodes, icoords, dummy, dummy, n, err     )
      
    end if
    
    
    
    ReadPointMeshChunk_r8 = err
  end function ReadPointMeshChunk_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointMesh_All_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointMesh_All
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointMeshAll_f( fileID, meshID, datatype,
  ! <DATA>                                                          ndims, nnodes,
  ! <DATA>                                                          icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),     intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),      intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_IPK),       intent(in)            :: datatype
  ! <DATA>   integer(kind=TIO_IPK),       intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),     intent(out)           :: nnodes
  ! <DATA>   [REAL CHOICE], dimension(:), intent(out)           :: icoords
  ! <DATA>   [REAL CHOICE], dimension(:), intent(out), optional :: jcoords, kcoords
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer(kind=TIO_ERRK) function ReadPointMeshAll_meta( fileID, meshID, datatype, ndims, nnodes )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    


    n = -1
    
    call tio_read_pointmesh_all_c( fileID, meshID, datatype, ndims, nnodes,  &
         &                         dummy, dummy, dummy, n, err               )


    
    
    
    
    ReadPointMeshAll_meta = err
  end function ReadPointMeshAll_meta
  integer(kind=TIO_ERRK) function ReadPointMeshAll_r4( fileID, meshID, datatype, ndims, nnodes,  &
       &                                               icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    real(kind=4),             dimension(:),           intent(out) :: icoords
    real(kind=4),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_pointmesh_all_c( fileID, meshID, datatype, ndims, nnodes,  &
           &                         icoords, jcoords, kcoords, n, err         )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_pointmesh_all_c( fileID, meshID, datatype, ndims, nnodes,  &
           &                         icoords, jcoords, dummy, n, err           )
      
    else
      
      n = 1
      call tio_read_pointmesh_all_c( fileID, meshID, datatype, ndims, nnodes,  &
           &                         icoords, dummy, dummy, n, err             )
      
    end if
    
    
    
    ReadPointMeshAll_r4 = err
  end function ReadPointMeshAll_r4
  integer(kind=TIO_ERRK) function ReadPointMeshAll_r8( fileID, meshID, datatype, ndims, nnodes,  &
       &                                               icoords, jcoords, kcoords )
    
    implicit none
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_IPK),   intent(in)  :: datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    real(kind=8),             dimension(:),           intent(out) :: icoords
    real(kind=8),             dimension(:), optional, intent(out) :: jcoords, kcoords
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err
    
    
    
    
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_pointmesh_all_c( fileID, meshID, datatype, ndims, nnodes,  &
           &                         icoords, jcoords, kcoords, n, err         )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_pointmesh_all_c( fileID, meshID, datatype, ndims, nnodes,  &
           &                         icoords, jcoords, dummy, n, err           )
      
    else
      
      n = 1
      call tio_read_pointmesh_all_c( fileID, meshID, datatype, ndims, nnodes,  &
           &                         icoords, dummy, dummy, n, err             )
      
    end if
    
    
    
    ReadPointMeshAll_r8 = err
  end function ReadPointMeshAll_r8
  
  
  
  ! ********************************************************************************************** !
  !
  ! TIO_Read_PointMesh_Section_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Read_PointMesh_Section
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer(kind=TIO_ERRK) function TIO_Read_PointMesh_Section_f( fileID, meshID, sectionID,
  ! <DATA>                                                               xfer, datatype, ndims, nnodes,
  ! <DATA>                                                               icoords, jcoords, kcoords )
  ! <DATA>
  ! <DATA>   integer(kind=TIO_FILEK),     intent(in)            :: fileID
  ! <DATA>   integer(kind=TIO_OBJK),      intent(in)            :: meshID
  ! <DATA>   integer(kind=TIO_OBJK),      intent(in)            :: sectionID
  ! <DATA>   integer(kind=TIO_IPK),       intent(in)            :: xfer, datatype
  ! <DATA>   integer(kind=TIO_IPK),       intent(out)           :: ndims
  ! <DATA>   integer(kind=TIO_SIZEK),     intent(out)           :: nnodes
  ! <DATA>   [REAL CHOICE], dimension(:), intent(out), optional :: icoords, jcoords, kcoords
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  
  integer(kind=TIO_ERRK) function ReadPointMeshSection_meta( fileID, meshID, sectionID, xfer, datatype,  &
       & ndims, nnodes )
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err = TIO_FAIL_F
    
    
     


    n = -1
    
    call tio_read_pointmesh_section_c( fileID, meshID, sectionID, xfer, datatype, &
         &                             ndims, nnodes, dummy, dummy, dummy, n, err )


    
    
    
    
    ReadPointMeshSection_meta = err
  end function ReadPointMeshSection_meta
  integer(kind=TIO_ERRK) function ReadPointMeshSection_r4( fileID, meshID, sectionID, xfer, datatype,  &
       & ndims, nnodes, icoords, jcoords, kcoords )
    
    real(kind=4), dimension(:), intent(out)           :: icoords
    real(kind=4), dimension(:), intent(out), optional :: jcoords, kcoords
    
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err = TIO_FAIL_F
    
    
     
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_pointmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_pointmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, jcoords, dummy, n, err   )
      
    else
      
      n = 1
      call tio_read_pointmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, dummy, dummy, n, err     )
      
    end if
    
    
    
    ReadPointMeshSection_r4 = err
  end function ReadPointMeshSection_r4
  integer(kind=TIO_ERRK) function ReadPointMeshSection_r8( fileID, meshID, sectionID, xfer, datatype,  &
       & ndims, nnodes, icoords, jcoords, kcoords )


    real(kind=8), dimension(:), intent(out)           :: icoords
    real(kind=8), dimension(:), intent(out), optional :: jcoords, kcoords
    
    
    integer(kind=TIO_FILEK), intent(in)  :: fileID
    integer(kind=TIO_OBJK),  intent(in)  :: meshID
    integer(kind=TIO_OBJK),  intent(in)  :: sectionID
    integer(kind=TIO_IPK),   intent(in)  :: xfer, datatype
    integer(kind=TIO_IPK),   intent(out) :: ndims
    integer(kind=TIO_SIZEK), intent(out) :: nnodes
    
    character :: dummy
    integer   :: n
    integer(kind=TIO_ERRK) :: err = TIO_FAIL_F
    
    
     
    
    
    
    if (present(kcoords)) then
      
      n = 3
      call tio_read_pointmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, jcoords, kcoords, n, err )
      
    else if (present(jcoords)) then
      
      n = 2
      call tio_read_pointmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, jcoords, dummy, n, err   )
      
    else
      
      n = 1
      call tio_read_pointmesh_section_c( fileID, meshID, sectionID, xfer, datatype,       &
           &                             ndims, nnodes, icoords, dummy, dummy, n, err     )
      
    end if
    
    
    
    ReadPointMeshSection_r8 = err
  end function ReadPointMeshSection_r8
  
  
  
  ! ********************************************************************************************** !
  
  
end module TIO_Mesh_mod






!
!  EOF

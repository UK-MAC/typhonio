!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO.
! Released under the BSD 3-clause license.
! For more details see license.txt
!
module TIO_Global_mod
  implicit none
  public
  ! ********************************************************************************************** !
  !
  ! Parameterized datatype kinds and properties:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Parameterized kinds:
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer, parameter :: TIO_FILEK
  ! <DATA> integer, parameter :: TIO_OBJK
  ! <DATA> integer, parameter :: TIO_ERRK
  ! <DATA> integer, parameter :: TIO_IPK
  ! <DATA> integer, parameter :: TIO_LPK
  ! <DATA> integer, parameter :: TIO_SIZEK
  ! <DATA> integer, parameter :: TIO_STEPK
  ! <DATA> integer, parameter :: TIO_TIMEK
  ! <DATA> integer, parameter :: TIO_SHAPEK
  ! <DATA> integer, parameter :: TIO_STRLEN_F
  ! <DATA> integer, parameter :: TIO_MAXDIMS_F
  ! <DATA> integer, parameter :: TIO_NULL_F
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_FILEK = 4 ! Must correspond with TIO_File_t
  integer, parameter :: TIO_OBJK = 4 ! Must correspond with TIO_Object_t
  integer, parameter :: TIO_ERRK = 4 ! Must correspond with TIO_t
  integer, parameter :: TIO_IPK = 4 ! Cast in C wrapper, for integer args
  integer, parameter :: TIO_LPK = 4 ! Cast in C wrapper, for logical/boolean args
  integer, parameter :: TIO_SIZEK = 4 ! Cast in C wrapper, so can pick 4 or 8 byte here
  integer, parameter :: TIO_STEPK = 4 ! Cast in C wrapper
  integer, parameter :: TIO_TIMEK = 8 ! Cast in C wrapper
  integer, parameter :: TIO_SHAPEK = 4 ! Must correspond with TIO_Shape_t
  integer, parameter :: TIO_STRLEN_F = 64 ! = TIO_STRLEN from typhio_global.h
  integer, parameter :: TIO_MAXDIMS_F = 7 ! = TIO_MAXDIMS from typhio_global.h
  integer, parameter :: TIO_NULL_F = -1 ! Generic null value
  ! ********************************************************************************************** !
  !
  ! Error return codes - copied straight out of tio_error.h:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer, parameter :: TIO_SUCCESS_F = 0
  ! <DATA> integer, parameter :: TIO_FAIL_F = -1
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_FILEID_F = -2
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_OBJECTID_F = -3
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_DATATYPE_F = -4
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_DIMS_F = -5
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_ARG_F = -6
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_RANGE_F = -7
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_INDEX_F = -8
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_NAME_F = -9
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_STRING_F = -10
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_OPERATION_F = -11
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_CONVERSION_F = -12
  ! <DATA> integer, parameter :: TIO_ERR_INVALID_SECTIONID_F = -13
  ! <DATA> integer, parameter :: TIO_ERR_FILE_CREATE_F = -20
  ! <DATA> integer, parameter :: TIO_ERR_FILE_OPEN_F = -21
  ! <DATA> integer, parameter :: TIO_ERR_FILE_CLOSE_F = -22
  ! <DATA> integer, parameter :: TIO_ERR_FILE_FLUSH_F = -23
  ! <DATA> integer, parameter :: TIO_ERR_FILE_NOT_EXIST_F = -24
  ! <DATA> integer, parameter :: TIO_ERR_FILE_NOT_WRITEABLE_F = -30
  ! <DATA> integer, parameter :: TIO_ERR_FILE_NOT_READABLE_F = -31
  ! <DATA> integer, parameter :: TIO_ERR_FILE_NOT_CLOSEABLE_F = -32
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_NOT_WRITEABLE_F = -33
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_NOT_READABLE_F = -34
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_NOT_CLOSEABLE_F = -35
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_CREATE_F = -40
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_OPEN_F = -41
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_CLOSE_F = -42
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_WRITE_F = -43
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_READ_F = -44
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_ALREADY_EXIST_F = -45
  ! <DATA> integer, parameter :: TIO_ERR_OBJECT_NOT_EXIST_F = -46
  ! <DATA> integer, parameter :: TIO_ERR_VERSION_MISMATCH_F = -80
  ! <DATA> integer, parameter :: TIO_ERR_LIBRARY_MISMATCH_F = -81
  ! <DATA> integer, parameter :: TIO_ERR_UNSUPPORTED_FILETYPE_F = -82
  ! <DATA> integer, parameter :: TIO_ERR_F90_API_F = -90
  ! <DATA> integer, parameter :: TIO_ERR_INT_F = -999
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_SUCCESS_F = 0
  integer, parameter :: TIO_FAIL_F = -1
  integer, parameter :: TIO_ERR_INVALID_FILEID_F = -2
  integer, parameter :: TIO_ERR_INVALID_OBJECTID_F = -3
  integer, parameter :: TIO_ERR_INVALID_DATATYPE_F = -4
  integer, parameter :: TIO_ERR_INVALID_DIMS_F = -5
  integer, parameter :: TIO_ERR_INVALID_ARG_F = -6
  integer, parameter :: TIO_ERR_INVALID_RANGE_F = -7
  integer, parameter :: TIO_ERR_INVALID_INDEX_F = -8
  integer, parameter :: TIO_ERR_INVALID_NAME_F = -9
  integer, parameter :: TIO_ERR_INVALID_STRING_F = -10
  integer, parameter :: TIO_ERR_INVALID_OPERATION_F = -11
  integer, parameter :: TIO_ERR_INVALID_CONVERSION_F = -12
  integer, parameter :: TIO_ERR_INVALID_SECTIONID_F = -13
  integer, parameter :: TIO_ERR_FILE_CREATE_F = -20
  integer, parameter :: TIO_ERR_FILE_OPEN_F = -21
  integer, parameter :: TIO_ERR_FILE_CLOSE_F = -22
  integer, parameter :: TIO_ERR_FILE_FLUSH_F = -23
  integer, parameter :: TIO_ERR_FILE_NOT_EXIST_F = -24
  integer, parameter :: TIO_ERR_FILE_NOT_WRITEABLE_F = -30
  integer, parameter :: TIO_ERR_FILE_NOT_READABLE_F = -31
  integer, parameter :: TIO_ERR_FILE_NOT_CLOSEABLE_F = -32
  integer, parameter :: TIO_ERR_OBJECT_NOT_WRITEABLE_F = -33
  integer, parameter :: TIO_ERR_OBJECT_NOT_READABLE_F = -34
  integer, parameter :: TIO_ERR_OBJECT_NOT_CLOSEABLE_F = -35
  integer, parameter :: TIO_ERR_OBJECT_CREATE_F = -40
  integer, parameter :: TIO_ERR_OBJECT_OPEN_F = -41
  integer, parameter :: TIO_ERR_OBJECT_CLOSE_F = -42
  integer, parameter :: TIO_ERR_OBJECT_WRITE_F = -43
  integer, parameter :: TIO_ERR_OBJECT_READ_F = -44
  integer, parameter :: TIO_ERR_OBJECT_ALREADY_EXIST_F = -45
  integer, parameter :: TIO_ERR_OBJECT_NOT_EXIST_F = -46
  integer, parameter :: TIO_ERR_VERSION_MISMATCH_F = -80
  integer, parameter :: TIO_ERR_LIBRARY_MISMATCH_F = -81
  integer, parameter :: TIO_ERR_UNSUPPORTED_FILETYPE_F = -82
  integer, parameter :: TIO_ERR_F90_API_F = -90
  integer, parameter :: TIO_ERR_INT_F = -999
  ! ********************************************************************************************** !
  !
  ! Datatypes:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Datatype specifiers:
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer, parameter :: TIO_DATATYPE_NULL_F
  ! <DATA> integer, parameter :: TIO_REAL4_F
  ! <DATA> integer, parameter :: TIO_REAL8_F
  ! <DATA> integer, parameter :: TIO_INTEGER4_F
  ! <DATA> integer, parameter :: TIO_INTEGER8_F
  ! <DATA> integer, parameter :: TIO_LOGICAL_F
  ! <DATA> integer, parameter :: TIO_CHAR_F
  ! <DATA> integer, parameter :: TIO_STRING_F
  ! <DATA> integer, parameter :: TIO_SIZE_F
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_DATATYPE_NULL_F = 1000
  integer, parameter :: TIO_REAL4_F = 1001
  integer, parameter :: TIO_REAL8_F = 1002
  integer, parameter :: TIO_INTEGER4_F = 1003
  integer, parameter :: TIO_INTEGER8_F = 1004
  integer, parameter :: TIO_LOGICAL_F = 1005 ! 4-byte logical = unsigned int
  integer, parameter :: TIO_CHAR_F = 1006
  integer, parameter :: TIO_STRING_F = 1007
  integer, parameter :: TIO_SIZE_F = 1008
  ! ********************************************************************************************** !
  !
  ! Type classes:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Datatype classes:
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer, parameter :: TIO_TYPECLASS_NULL_F
  ! <DATA> integer, parameter :: TIO_TYPECLASS_INTEGER_F
  ! <DATA> integer, parameter :: TIO_TYPECLASS_REAL_F
  ! <DATA> integer, parameter :: TIO_TYPECLASS_STRING_F
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_TYPECLASS_NULL_F = 2000
  integer, parameter :: TIO_TYPECLASS_REAL_F = 2001
  integer, parameter :: TIO_TYPECLASS_INTEGER_F = 2002
  integer, parameter :: TIO_TYPECLASS_STRING_F = 2003
  ! ********************************************************************************************** !
  !
  ! Dimensions (actual numeric values):
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Dimensions:
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer, parameter :: TIO_0D_F = 0
  ! <DATA> integer, parameter :: TIO_1D_F = 1
  ! <DATA> integer, parameter :: TIO_2D_F = 2
  ! <DATA> integer, parameter :: TIO_3D_F = 3
  ! <DATA> integer, parameter :: TIO_4D_F = 4
  ! <DATA> integer, parameter :: TIO_5D_F = 5
  ! <DATA> integer, parameter :: TIO_6D_F = 6
  ! <DATA> integer, parameter :: TIO_7D_F = 7
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_0D_F = 0
  integer, parameter :: TIO_1D_F = 1
  integer, parameter :: TIO_2D_F = 2
  integer, parameter :: TIO_3D_F = 3
  integer, parameter :: TIO_4D_F = 4
  integer, parameter :: TIO_5D_F = 5
  integer, parameter :: TIO_6D_F = 6
  integer, parameter :: TIO_7D_F = 7
  ! ********************************************************************************************** !
  !
  ! Ghosts (actual numeric values):
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_GHOSTS_NULL_F = -1
  integer, parameter :: TIO_GHOSTS_NONE_F = 0
  integer, parameter :: TIO_GHOSTS_ALL_F = -2
!!$ integer, parameter :: TIO_GHOSTS_INTERNAL_F = -3 ! Viz-only
!!$ integer, parameter :: TIO_GHOSTS_ONLY_F = -4 ! Viz-only
  ! ********************************************************************************************** !
  !
  ! Access parameters:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Access mode specifiers:
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer, parameter :: TIO_ACC_NULL_F = 90010
  ! <DATA> integer, parameter :: TIO_ACC_CREATE_F = 90011
  ! <DATA> integer, parameter :: TIO_ACC_REPLACE_F = 90012
  ! <DATA> integer, parameter :: TIO_ACC_READWRITE_F = 90013
  ! <DATA> integer, parameter :: TIO_ACC_READONLY_F = 90014
  ! <DATA> integer, parameter :: TIO_ACC_READWRITESTRICT_F = 90015
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_ACC_NULL_F = 90010
  integer, parameter :: TIO_ACC_CREATE_F = 90011
  integer, parameter :: TIO_ACC_REPLACE_F = 90012
  integer, parameter :: TIO_ACC_READWRITE_F = 90013
  integer, parameter :: TIO_ACC_READONLY_F = 90014
  integer, parameter :: TIO_ACC_READWRITESTRICT_F = 90015
  ! ********************************************************************************************** !
  !
  ! Meshtype parameters:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_MESH_NULL_F = 90020
  integer, parameter :: TIO_MESH_QUAD_F = 90021 ! = colinear
  integer, parameter :: TIO_MESH_QUAD_COLINEAR_F = 90021
  integer, parameter :: TIO_MESH_QUAD_NONCOLINEAR_F = 90022
  integer, parameter :: TIO_MESH_UNSTRUCT_F = 90023
  integer, parameter :: TIO_MESH_POINT_F = 90024
  ! ********************************************************************************************** !
  !
  ! Coordtype parameters:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_COORD_NULL_F = 90030
  integer, parameter :: TIO_COORD_CARTESIAN_F = 90031
  integer, parameter :: TIO_COORD_SPHERICAL_F = 90032
  integer, parameter :: TIO_COORD_CYLINDRICAL_F = 90033
  integer, parameter :: TIO_COORD_CYL_AXISYMMETRIC_F = 90034
  ! ********************************************************************************************** !
  !
  ! Shape parameters:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_SHAPE_NULL_F = 0 ! Values here muct match C
  integer, parameter :: TIO_SHAPE_POINT1_F = -10
  integer, parameter :: TIO_SHAPE_BAR2_F = -11
  integer, parameter :: TIO_SHAPE_TRI3_F = -12
  integer, parameter :: TIO_SHAPE_QUAD4_F = -13
  integer, parameter :: TIO_SHAPE_TET4_F = -14
  integer, parameter :: TIO_SHAPE_WEDGE6_F = -15
  integer, parameter :: TIO_SHAPE_HEX8_F = -16
  integer, parameter :: TIO_SHAPE_PYR5_F = -17
                                                                 ! More shapes may be
                                                                 ! added in future
  ! ********************************************************************************************** !
  !
  ! Centring parameters:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_CENTRE_NULL_F = 90040
  integer, parameter :: TIO_CENTRE_CELL_F = 90041
  integer, parameter :: TIO_CENTRE_EDGE_I_F = 90042
  integer, parameter :: TIO_CENTRE_EDGE_J_F = 90043
  integer, parameter :: TIO_CENTRE_EDGE_K_F = 90044
  integer, parameter :: TIO_CENTRE_FACE_I_F = 90045
  integer, parameter :: TIO_CENTRE_FACE_J_F = 90046
  integer, parameter :: TIO_CENTRE_FACE_K_F = 90047
  integer, parameter :: TIO_CENTRE_NODE_F = 90048
  ! ********************************************************************************************** !
  !
  ! ArrayQuant dimension tags:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_ARRAYQ_DIM1_F = -101
  integer, parameter :: TIO_ARRAYQ_DIM2_F = -102
  integer, parameter :: TIO_ARRAYQ_DIM3_F = -103
  integer, parameter :: TIO_ARRAYQ_CELL_F = -104
  integer, parameter :: TIO_ARRAYQ_NODE_F = -105
  ! ********************************************************************************************** !
  !
  ! Data transfer parameters:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_XFER_NULL_F = 90050
  integer, parameter :: TIO_XFER_INDEPENDENT_F = 90051
  integer, parameter :: TIO_XFER_COLLECTIVE_F = 90052
  ! ********************************************************************************************** !
  !
  ! Verbose mode parameters:
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> Error codes:
  ! </NAME>
  ! <FAPI>
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer, parameter :: TIO_VERBOSE_NULL_F = 90060
  integer, parameter :: TIO_VERBOSE_OFF_F = 90061
  integer, parameter :: TIO_VERBOSE_ALL_F = 90062
  integer, parameter :: TIO_VERBOSE_MASTER_F = 90063
contains
  ! ********************************************************************************************** !
  !
  ! TIO_SizeOf_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_SizeOf
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer function TIO_SizeOf_f( datatype )
  ! <DATA>
  ! <DATA> integer, intent(in) :: datatype
  ! </FAPI>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer function TIO_SizeOf_f( datatype )
    implicit none
    integer, intent(in) :: datatype
    integer :: val
    call tio_sizeof_c( datatype, val )
    TIO_SizeOf_f = val
  end function TIO_SizeOf_f
  ! ********************************************************************************************** !
  !
  ! TIO_Get_TypeClass_f
  !
  ! <ITEM>
  ! <NAME>
  ! <DATA> TIO_Get_TypeClass
  ! </NAME>
  ! <FAPI>
  ! <DATA> integer function TIO_Get_TypeClass_f( datatype, typeclass )
  ! <DATA>
  ! <DATA> integer, intent(in) :: datatype
  ! <DATA> integer, intent(out) :: typeclass
  ! </FAPI>
  ! <FDES>
  ! <DATA> The returned typeclass is given as one of the TypeClass parameters:
  ! <DATA> <blockquote>
  ! <DATA> <code>TIO_TYPECLASS_INTEGER_F</code><br>
  ! <DATA> <code>TIO_TYPECLASS_FLOAT_F</code><br>
  ! <DATA> <code>TIO_TYPECLASS_STRING_F</code><br>
  ! <DATA> </blockquote>
  ! </FDES>
  ! </ITEM>
  !
  ! ********************************************************************************************** !
  integer function TIO_Get_TypeClass_f( datatype, typeclass )
    implicit none
    integer, intent(in) :: datatype
    integer, intent(out) :: typeclass
    integer(kind=TIO_ERRK) :: err
    call tio_get_typeclass_c( datatype, typeclass, err )
    TIO_Get_TypeClass_f = err
  end function TIO_Get_TypeClass_f
end module TIO_Global_mod
!
! EOF


CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

PROJECT(TyphonIO C)

option(BUILD_FORTRAN_LIBRARY "BUILD_FORTRAN_LIBRARY" OFF)

include(CPack)

# HDF5

FIND_PACKAGE(HDF5 COMPONENTS C HL NO_MODULE REQUIRED shared)
#FIND_PACKAGE(HDF5 COMPONENTS C HL NO_MODULE REQUIRED static)
SET(LINK_LIBS ${LINK_LIBS} ${HDF5_C_LIBRARIES})

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_access.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_access.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_chunk.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_chunk.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_chunk_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_error.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_error.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_error_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_global.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_global.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_global_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_hdf5.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_hdf5_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_hierarchy.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_hierarchy_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_malloc.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_malloc_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_material.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_material.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_mesh.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_mesh.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_mesh_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_quant.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_quant.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_register.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_register_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_stack.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_stack_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_state.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_state.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_vargroup.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_vargroup.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_variable.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_variable.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_vizutils.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_xhdf5.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/tio_xhdf5_int.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/typhonio.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/typhonio_viz.h
)

ADD_LIBRARY(typhonio ${SOURCES})

INCLUDE_DIRECTORIES(typhonio
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${HDF5_INCLUDE_DIR}
)

TARGET_LINK_LIBRARIES(typhonio
	${LINK_LIBS}
)

ADD_DEFINITIONS(-D_TYPHIO_HDF5)

INSTALL(TARGETS typhonio DESTINATION lib)

INSTALL(FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_access.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_chunk.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_error.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_global.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_material.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_mesh.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_quant.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_state.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_vargroup.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tio_variable.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/typhonio.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/typhonio_viz.h"
	DESTINATION include
)

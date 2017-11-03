# Header Files
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Source Files
SET(TIO_SOURCES
	${CMAKE_SOURCE_DIR}/src/tio_access.c
	${CMAKE_SOURCE_DIR}/src/tio_access.h
	${CMAKE_SOURCE_DIR}/src/tio_chunk.c
	${CMAKE_SOURCE_DIR}/src/tio_chunk.h
	${CMAKE_SOURCE_DIR}/src/tio_chunk_int.h
	${CMAKE_SOURCE_DIR}/src/tio_error.c
	${CMAKE_SOURCE_DIR}/src/tio_error.h
	${CMAKE_SOURCE_DIR}/src/tio_error_int.h
	${CMAKE_SOURCE_DIR}/src/tio_global.c
	${CMAKE_SOURCE_DIR}/src/tio_global.h
	${CMAKE_SOURCE_DIR}/src/tio_global_int.h
	${CMAKE_SOURCE_DIR}/src/tio_hdf5.c
	${CMAKE_SOURCE_DIR}/src/tio_hdf5_int.h
	${CMAKE_SOURCE_DIR}/src/tio_hierarchy.c
	${CMAKE_SOURCE_DIR}/src/tio_hierarchy_int.h
	${CMAKE_SOURCE_DIR}/src/tio_malloc.c
	${CMAKE_SOURCE_DIR}/src/tio_malloc_int.h
	${CMAKE_SOURCE_DIR}/src/tio_material.c
	${CMAKE_SOURCE_DIR}/src/tio_material.h
	${CMAKE_SOURCE_DIR}/src/tio_mesh.c
	${CMAKE_SOURCE_DIR}/src/tio_mesh.h
	${CMAKE_SOURCE_DIR}/src/tio_mesh_int.h
	${CMAKE_SOURCE_DIR}/src/tio_quant.c
	${CMAKE_SOURCE_DIR}/src/tio_quant.h
	${CMAKE_SOURCE_DIR}/src/tio_register.c
	${CMAKE_SOURCE_DIR}/src/tio_register_int.h
	${CMAKE_SOURCE_DIR}/src/tio_stack.c
	${CMAKE_SOURCE_DIR}/src/tio_stack_int.h
	${CMAKE_SOURCE_DIR}/src/tio_state.c
	${CMAKE_SOURCE_DIR}/src/tio_state.h
	${CMAKE_SOURCE_DIR}/src/tio_vargroup.c
	${CMAKE_SOURCE_DIR}/src/tio_vargroup.h
	${CMAKE_SOURCE_DIR}/src/tio_variable.c
	${CMAKE_SOURCE_DIR}/src/tio_variable.h
	${CMAKE_SOURCE_DIR}/src/tio_vizutils.c
	${CMAKE_SOURCE_DIR}/src/tio_xhdf5.c
	${CMAKE_SOURCE_DIR}/src/tio_xhdf5_int.h
	${CMAKE_SOURCE_DIR}/src/typhonio.h
	${CMAKE_SOURCE_DIR}/src/typhonio_viz.h
)

SET(TIO_HEADERS
	"${CMAKE_SOURCE_DIR}/src/tio_access.h"
	"${CMAKE_SOURCE_DIR}/src/tio_chunk.h"
	"${CMAKE_SOURCE_DIR}/src/tio_error.h"
	"${CMAKE_SOURCE_DIR}/src/tio_global.h"
	"${CMAKE_SOURCE_DIR}/src/tio_material.h"
	"${CMAKE_SOURCE_DIR}/src/tio_mesh.h"
	"${CMAKE_SOURCE_DIR}/src/tio_quant.h"
	"${CMAKE_SOURCE_DIR}/src/tio_state.h"
	"${CMAKE_SOURCE_DIR}/src/tio_vargroup.h"
	"${CMAKE_SOURCE_DIR}/src/tio_variable.h"
	"${CMAKE_SOURCE_DIR}/src/typhonio.h"
	"${CMAKE_SOURCE_DIR}/src/typhonio_viz.h"
)

## Install Headers
INSTALL(FILES ${TIO_HEADERS} DESTINATION include)

# Definitions For Src
ADD_DEFINITIONS(-D_TYPHIO_HDF5)
ADD_DEFINITIONS(-D_TYPHIO_PARALLEL)

# Static Library
ADD_LIBRARY(typhonio STATIC ${TIO_SOURCES})
TARGET_LINK_LIBRARIES(typhonio ${TIO_EXTERNAL_LIBS})
SET_TARGET_PROPERTIES(typhonio PROPERTIES OUTPUT_NAME typhonio CLEAN_DIRECT_OUTPUT 1)
INSTALL(TARGETS typhonio DESTINATION lib)

# Shared Library
IF(BUILD_TIO_SHARED)
	ADD_LIBRARY(typhonio-shared SHARED ${TIO_SOURCES})
	TARGET_LINK_LIBRARIES(typhonio-shared ${TIO_EXTERNAL_LIBS})
	SET_TARGET_PROPERTIES(typhonio-shared PROPERTIES OUTPUT_NAME typhonio CLEAN_DIRECT_OUTPUT 1)
	INSTALL(TARGETS typhonio-shared DESTINATION lib)
ENDIF(BUILD_TIO_SHARED)
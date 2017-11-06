################################
## TyphonIO Fortran Libraries ##
################################

# Header Files
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_Fortran_MODULE_DIRECTORY lib)

SET(TIO_F_SOURCES
	${CMAKE_SOURCE_DIR}/src/F90/tiof_fkit.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_global_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_error_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_access_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_vargroup_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_variable_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_state_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_mesh_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_material_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_quant_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_chunk_c.c
	${CMAKE_SOURCE_DIR}/src/F90/tiof_global_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_error_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_utils_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_access_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_vargroup_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_variable_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_state_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_mesh_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_material_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_quant_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/tiof_chunk_f.F90
	${CMAKE_SOURCE_DIR}/src/F90/typhonio.F90
)

SET(TIO_F_HEADERS
	${CMAKE_SOURCE_DIR}/src/F90/tiof_fkit.h
	${CMAKE_SOURCE_DIR}/src/F90/tiof_errornum.inc
	${CMAKE_SOURCE_DIR}/src/F90/tiof_errornumapi.inc
)

SET(TIO_F_MODULES
	tio_access_mod.mod
	tio_chunk_mod.mod
	tio_error_mod.mod
	tio_global_mod.mod
	tio_material_mod.mod
	tio_mesh_mod.mod
	tio_quant_mod.mod
	tio_state_mod.mod
	tio_utils_mod.mod
	tio_vargroup_mod.mod
	tio_variable_mod.mod
	typhonio.mod
)

# INSTALL(FILES ${TIO_F_MODULES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

ADD_DEFINITIONS(-D_TYPHIO_F2003_STDERR)
ADD_DEFINITIONS(-DF2C_UNDERSCORE)
ADD_DEFINITIONS(-D_TYPHIO_PARALLEL)

ADD_LIBRARY(typhonio_f STATIC ${TIO_F_SOURCES})
SET_TARGET_PROPERTIES(typhonio_f PROPERTIES OUTPUT_NAME typhonio_f CLEAN_DIRECT_OUTPUT 1 
		LINKER_LANGUAGE Fortran)
SET(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/src/F90/lib)
INSTALL(DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
INSTALL(TARGETS typhonio_f DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

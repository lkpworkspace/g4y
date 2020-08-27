cmake_minimum_required(VERSION 3.5)

project(g4y)

set(G4Y_VERSION 0.1)

set(G4Y_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND CMAKE_MODULE_PATH ${G4Y_ROOT_PATH}/cmake)
set(BUILD_SHARED_LIBS TRUE)

include(G4YBuildSet)

include_directories(
	${G4Y_ROOT_PATH}/g4y
	${G4Y_ROOT_PATH}/g4y/gcom
	${G4Y_ROOT_PATH}/g4y/gcom/camera
	${G4Y_ROOT_PATH}/g4y/gcom/transform
	${G4Y_ROOT_PATH}/g4y/gcom/graphics
	${G4Y_ROOT_PATH}/g4y/gcom/shape
	${G4Y_ROOT_PATH}/g4y/gcom/physics
	${G4Y_ROOT_PATH}/examples
	${G4Y_ROOT_PATH}/3rd/include/imgui
	${G4Y_ROOT_PATH}/3rd/include/imgui/examples
)

include(${G4Y_ROOT_PATH}/g4y/CMakeLists.txt)

add_library(g4y SHARED ${G4Y_SRC} ${G4Y_COM_SRC})

use_g4y_compile_define(g4y)
use_g4y_compile_options(g4y)
use_g4y_libs_depend(g4y)

set_target_properties(g4y
    PROPERTIES
    VERSION "${G4Y_VERSION}"
)

add_custom_command(TARGET ${PROJECT_NAME} 
	POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E 
		make_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/"
	COMMAND ${CMAKE_COMMAND} -E 
	   copy
	   "${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/${PROJECT_NAME}.lib"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/" 
	COMMAND ${CMAKE_COMMAND} -E 
		copy 
		"${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/${PROJECT_NAME}.dll"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/" 
)


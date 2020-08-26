cmake_minimum_required(VERSION 3.5)
set(APP_NAME HelloCpp)

project(${APP_NAME})

set(G4Y_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND CMAKE_MODULE_PATH ${G4Y_ROOT_PATH}/cmake)

include(ProjBuildSet)

include_directories(
	${G4Y_ROOT_PATH}/g4y
	${G4Y_ROOT_PATH}/g4y/gcom
	${G4Y_ROOT_PATH}/g4y/gcom/camera
	${G4Y_ROOT_PATH}/g4y/gcom/transform
	${G4Y_ROOT_PATH}/g4y/gcom/graphics
	${G4Y_ROOT_PATH}/g4y/gcom/shape
	${G4Y_ROOT_PATH}/g4y/gcom/physics
	${G4Y_ROOT_PATH}/examples
	${G4Y_ROOT_PATH}/3rd/imgui
	${G4Y_ROOT_PATH}/3rd/imgui/examples
)

set(${APP_NAME}_SRC
    main.cpp
)

add_executable(${APP_NAME} ${${APP_NAME}_SRC})

use_g4y_compile_define(${APP_NAME})
use_g4y_compile_options(${APP_NAME})
use_g4y_libs_depend(${APP_NAME})

target_link_libraries(${APP_NAME} ${G4Y_ROOT_PATH}/libs/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/g4y.lib)

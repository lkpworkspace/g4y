cmake_minimum_required(VERSION 3.10)

project(gfy)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(gfy)
include_directories(gcom)
include_directories(3rd/glm)

set(GCOM_SRCS
    ${CMAKE_SOURCE_DIR}/gcom/GTransform.cpp
)

set(GFY_SRCS
    ${CMAKE_SOURCE_DIR}/gfy/GObj.cpp
    ${CMAKE_SOURCE_DIR}/gfy/GCom.cpp
    ${CMAKE_SOURCE_DIR}/gfy/GScene.cpp
    ${CMAKE_SOURCE_DIR}/gfy/GWorld.cpp
)

add_subdirectory(3rd/glm)
add_subdirectory(examples)
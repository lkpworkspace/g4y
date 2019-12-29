cmake_minimum_required(VERSION 2.8)
project(gfy)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# gl glew glm assimp imgui stb
find_package(glm)
find_package(assimp)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)
include_directories(${PROJECT_SOURCE_DIR}/3rd/imgui)
include_directories(${PROJECT_SOURCE_DIR}/3rd/stb)
set(IMGUI_SRCS
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/examples/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/examples/imgui_impl_opengl3.cpp
)

include_directories(gfy)
include_directories(gcom/transform)
include_directories(gcom/model)

set(GFY_SRCS
    ${PROJECT_SOURCE_DIR}/gfy/GObj.cpp
    ${PROJECT_SOURCE_DIR}/gfy/GCom.cpp
    ${PROJECT_SOURCE_DIR}/gfy/GScene.cpp
    ${PROJECT_SOURCE_DIR}/gfy/GWorld.cpp
)

set(GCOM_TRANSFORM_SRCS
    ${PROJECT_SOURCE_DIR}/gcom/transform/GTransform.cpp
)

set(GCOM_MODEL_SRCS
    ${PROJECT_SOURCE_DIR}/gcom/model/GModel.cpp
    ${PROJECT_SOURCE_DIR}/gcom/model/GMesh.cpp
    ${PROJECT_SOURCE_DIR}/gcom/model/GShader.cpp
)

add_subdirectory(examples)

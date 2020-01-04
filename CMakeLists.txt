cmake_minimum_required(VERSION 2.8)
project(g4y)

option(USE_GUI "use gui" ON)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# gl glew glm assimp imgui stb
find_package(glm)
if(USE_GUI)
    find_package(assimp)
    
    add_definitions(-DUSE_GUI)
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)
    
    include_directories(gcom/model)
    include_directories(${PROJECT_SOURCE_DIR}/3rd/imgui)
    include_directories(${PROJECT_SOURCE_DIR}/3rd/imgui/examples)
    include_directories(${PROJECT_SOURCE_DIR}/3rd/stb)

    set(GCOM_GUI_SRCS
        ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_demo.cpp
        ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/3rd/imgui/examples/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/3rd/imgui/examples/imgui_impl_opengl3.cpp

        ${PROJECT_SOURCE_DIR}/gcom/model/GModel.cpp
        ${PROJECT_SOURCE_DIR}/gcom/model/GMesh.cpp
        ${PROJECT_SOURCE_DIR}/gcom/model/GShader.cpp

        ${PROJECT_SOURCE_DIR}/gcom/shape/GGrid.cpp
    )
endif(USE_GUI)

include_directories(g4y)
include_directories(gcom/transform)
include_directories(gcom/camera)
include_directories(gcom/shape)

set(G4Y_SRCS
    ${PROJECT_SOURCE_DIR}/g4y/GObj.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GCom.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GScene.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GWorld.cpp
)

set(GCOM_SRCS
    ${PROJECT_SOURCE_DIR}/gcom/transform/GTransform.cpp
    ${PROJECT_SOURCE_DIR}/gcom/camera/GCamera.cpp
)

add_subdirectory(examples)

cmake_minimum_required(VERSION 2.8)
project(g4y)

option(USE_GUI "use gui" ON)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# boost lib
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# bullet
find_package(Bullet)
if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
endif()

include_directories(g4y)
include_directories(gcom/transform)
include_directories(gcom/camera)
include_directories(gcom/physics)
include_directories(gcom/msg)

set(G4Y_SRCS
    ${PROJECT_SOURCE_DIR}/g4y/GObj.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GCom.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GScene.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GWorld.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GPhyWorld.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GOpenGLView.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GShader.cpp
)

set(GCOM_SRCS
    ${PROJECT_SOURCE_DIR}/gcom/transform/GTransform.cpp
    ${PROJECT_SOURCE_DIR}/gcom/camera/GCamera.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GMeshCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GSphereCollider.cpp
)

# gl glew glm assimp imgui stb
find_package(glm)
if(USE_GUI)
    find_package(assimp)
    
    add_definitions(-DUSE_GUI)
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)
    
    include_directories(gcom/model)
    include_directories(gcom/shape)
    include_directories(gcom/light)
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

        ${PROJECT_SOURCE_DIR}/gcom/shape/GGrid.cpp
        ${PROJECT_SOURCE_DIR}/gcom/shape/GAxis.cpp
        ${PROJECT_SOURCE_DIR}/gcom/shape/GSkybox.cpp
        ${PROJECT_SOURCE_DIR}/gcom/shape/GCube.cpp

        ${PROJECT_SOURCE_DIR}/gcom/light/GLight.cpp
    )
endif(USE_GUI)

add_subdirectory(examples)

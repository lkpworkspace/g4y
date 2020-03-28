cmake_minimum_required(VERSION 2.8)
project(g4y)

option(USE_GUI "use gui" ON)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# boost lib
add_definitions(-DBOOST_LOG_DYN_LINK)
set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.65 COMPONENTS python3 date_time system log)
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
include_directories(gcom/msg/msgio)

set(G4Y_SRCS
    ${PROJECT_SOURCE_DIR}/g4y/G4Y.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GLog.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GObj.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GCom.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GScene.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GWorld.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GPhyWorld.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GOpenGLView.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GShader.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GResourceMgr.cpp
)

set(GCOM_SRCS
    ${PROJECT_SOURCE_DIR}/gcom/transform/GTransform.cpp
    ${PROJECT_SOURCE_DIR}/gcom/camera/GCamera.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GRigibody.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GMeshCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GSphereCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GBoxCollider.cpp
)

# gl glew glm assimp imgui stb
find_package(glm)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
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

if(USE_GUI)
set(G4Y_ALL_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS}
)
else()
set(G4Y_ALL_SRCS
    ${GCOM_SRCS}
    ${G4Y_SRCS}
)
endif(USE_GUI)

set(G4Y_LIBRARIES
    rt
    GL 
    GLEW 
    glm 
    glfw 
    assimp 
    pthread
    ${BULLET_LIBRARIES}
    ${Boost_LIBRARIES}
)

include_directories(examples)
add_subdirectory(pyg4y)
add_subdirectory(examples)
cmake_minimum_required(VERSION 3.5)

project(g4y)

option(USE_GRAPHICS "use graphics" ON)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_definitions(-DUSE_GRAPHICS)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)

set(G4Y_SRCS
    ${PROJECT_SOURCE_DIR}/g4y/G4Y.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GLog.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GObj.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GCom.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GScene.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GWorld.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GPhyWorld.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GOpenGLView.cpp
    ${PROJECT_SOURCE_DIR}/g4y/GResourceMgr.cpp
)

set(GCOM_SRCS
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/examples/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/examples/imgui_impl_opengl3.cpp

    ${PROJECT_SOURCE_DIR}/gcom/transform/GTransform.cpp
    ${PROJECT_SOURCE_DIR}/gcom/camera/GCamera.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GRigibody.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GMeshCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GSphereCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GBoxCollider.cpp

    ${PROJECT_SOURCE_DIR}/gcom/graphics/GMesh.cpp
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GShader.cpp
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GMaterial.cpp
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GTexture.cpp
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GMeshRenderer.cpp

    ${PROJECT_SOURCE_DIR}/gcom/shape/GGrid.cpp
    ${PROJECT_SOURCE_DIR}/gcom/shape/GAxis.cpp
    ${PROJECT_SOURCE_DIR}/gcom/shape/GSkybox.cpp
    ${PROJECT_SOURCE_DIR}/gcom/shape/GCube.cpp

    ${PROJECT_SOURCE_DIR}/gcom/light/GLight.cpp
)

set(G4Y_ALL_SRCS
    ${GCOM_SRCS}
    ${G4Y_SRCS}
)

# opengl
find_package(OpenGL REQUIRED)

# glew
find_package(GLEW REQUIRED)

# glm
find_package(glm REQUIRED)

# boost lib
add_definitions(-DBOOST_LOG_DYN_LINK)
set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost REQUIRED COMPONENTS python date_time system log)

# bullet
find_package(Bullet REQUIRED)

# assimp
find_package(assimp REQUIRED)


include_directories(g4y)
include_directories(examples)
include_directories(gcom/transform)
include_directories(gcom/camera)
include_directories(gcom/physics)
include_directories(gcom/msg)
include_directories(gcom/msg/msgio)
include_directories(gcom/shape)
include_directories(gcom/light)
include_directories(gcom/graphics)
include_directories(${PROJECT_SOURCE_DIR}/3rd/imgui)
include_directories(${PROJECT_SOURCE_DIR}/3rd/imgui/examples)
include_directories(${PROJECT_SOURCE_DIR}/3rd/stb)
include_directories(${GLM_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR} ${BULLET_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


if(WIN32)
    set(G4Y_LIBRARIES
        glfw
        GLEW::GLEW
        assimp::assimp
        ${OPENGL_LIBRARIES}
        ${GLM_LIBRARY_DIRS} 
        ${BULLET_LIBRARIES}
        ${Boost_LIBRARIES}
    )
elseif(UNIX)
    set(G4Y_LIBRARIES
        rt
        pthread
        glfw
        GLEW
        assimp
        ${OPENGL_LIBRARIES}
        ${GLM_LIBRARY_DIRS}
        ${BULLET_LIBRARIES}
        ${Boost_LIBRARIES}
    )
endif()

add_subdirectory(pyg4y)
add_subdirectory(examples)
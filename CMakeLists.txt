cmake_minimum_required(VERSION 3.5)

project(g4y)

#set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD            17)
set(CMAKE_CXX_STANDARD_REQUIRED   YES)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)
if(WIN32)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE)
endif()

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

# imgui
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/examples/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui/examples/imgui_impl_opengl3.cpp

# transform
    ${PROJECT_SOURCE_DIR}/gcom/transform/GTransform.cpp

# camera
    ${PROJECT_SOURCE_DIR}/gcom/camera/GCamera.cpp

# physics
    ${PROJECT_SOURCE_DIR}/gcom/physics/GCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GRigibody.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GMeshCollider.cpp
    ${PROJECT_SOURCE_DIR}/gcom/physics/GBoxCollider.cpp

# graphics
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GMesh.cpp
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GShader.cpp
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GMaterial.cpp
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GTexture.cpp
    ${PROJECT_SOURCE_DIR}/gcom/graphics/GMeshRenderer.cpp

# shape
    ${PROJECT_SOURCE_DIR}/gcom/shape/GGrid.cpp
    ${PROJECT_SOURCE_DIR}/gcom/shape/GAxis.cpp
    ${PROJECT_SOURCE_DIR}/gcom/shape/GSkybox.cpp
    ${PROJECT_SOURCE_DIR}/gcom/shape/GCube.cpp
)

set(G4Y_ALL_SRCS
    ${GCOM_SRCS}
    ${G4Y_SRCS}
)

# opengl
find_package(OpenGL REQUIRED)

# glew
find_package(GLEW REQUIRED)

# glfw
find_package(glfw3 REQUIRED)

# glm
find_package(glm REQUIRED)

# boost lib
add_definitions(-DBOOST_LOG_DYN_LINK)
set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
if(WIN32)
find_package(Boost REQUIRED COMPONENTS python37 date_time system log)
elseif(UNIX)
find_package(Boost REQUIRED COMPONENTS python3 date_time system log)
endif()
message(STATUS ${Boost_INCLUDE_DIRS})
message(STATUS ${Boost_LIBRARIES})

# bullet
find_package(Bullet REQUIRED)

# assimp
find_package(assimp REQUIRED)


include_directories(g4y)
include_directories(examples)
include_directories(gcom/transform)
include_directories(gcom/camera)
include_directories(gcom/physics)
include_directories(gcom/shape)
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

add_subdirectory(examples)
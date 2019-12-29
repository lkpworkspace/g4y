set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(GMODEL_SRCS 
    GModel.cpp
    GMesh.cpp
    GShader.cpp
)

add_library(gmodel SHARED ${GMODEL_SRCS})
target_link_libraries(gmodel GL GLEW glm)
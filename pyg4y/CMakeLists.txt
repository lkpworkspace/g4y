set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
# message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
# message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
# message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
include_directories(${PYTHON_INCLUDE_DIRS})

add_library(g4y SHARED pyg4y.cpp ${G4Y_ALL_SRCS})

target_link_libraries(g4y ${G4Y_LIBRARIES})
set(Python3_INCLUDE_DIR ${G4Y_ROOT_PATH}/3rd/include/python3.7)
set(Python3_LIBRARY
    ${G4Y_ROOT_PATH}/3rd/libs/Release/python37.lib
    #optimized ${CMAKE_SOURCE_DIR}/3rd/libs/Release/python37.lib
    #debug ${CMAKE_SOURCE_DIR}/3rd/libs/Debug/python37_d.lib
)

set(SSSIMP_INCLUDE_DIRS "${Python3_INCLUDE_DIR}")
set(Python3_LIBRARIES "${Python3_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Python3 DEFAULT_MSG Python3_LIBRARIES Python3_INCLUDE_DIR)

mark_as_advanced(Python3_INCLUDE_DIR Python3_LIBRARIES Python3_LIBRARY)
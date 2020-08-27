set(GLEW_INCLUDE_DIR ${G4Y_ROOT_PATH}/3rd/include)
set(GLEW_LIBRARY
    optimized ${G4Y_ROOT_PATH}/3rd/libs/Release/glew32.lib
    debug ${G4Y_ROOT_PATH}/3rd/libs/Debug/glew32d.lib
)

set(GLEW_INCLUDE_DIRS "${GLEW_INCLUDE_DIR}")
set(GLEW_LIBRARIES "${GLEW_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIBRARIES GLEW_INCLUDE_DIR)

mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARIES GLEW_LIBRARY)
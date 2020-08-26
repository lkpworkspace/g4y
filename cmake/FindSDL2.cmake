set(SDL2_INCLUDE_DIR ${G4Y_ROOT_PATH}/3rd/include/SDL2)
set(SDL2_LIBRARY
    optimized ${G4Y_ROOT_PATH}/3rd/libs/Release/SDL2.lib
    debug ${G4Y_ROOT_PATH}/3rd/libs/Debug/SDL2d.lib
)

set(SDL2_INCLUDE_DIRS "${SDL2_INCLUDE_DIR}")
set(SDL2_LIBRARIES "${SDL2_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARIES SDL2_INCLUDE_DIR)

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARIES SDL2_LIBRARY)
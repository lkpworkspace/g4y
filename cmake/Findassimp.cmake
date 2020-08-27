set(ASSIMP_INCLUDE_DIR ${G4Y_ROOT_PATH}/3rd/include)
set(ASSIMP_LIBRARY
    optimized ${G4Y_ROOT_PATH}/3rd/libs/Release/assimp-vc141-mt.lib
    debug ${G4Y_ROOT_PATH}/3rd/libs/Debug/assimp-vc141-mtd.lib
)

set(ASSIMP_INCLUDE_DIRS "${ASSIMP_INCLUDE_DIR}")
set(ASSIMP_LIBRARIES "${ASSIMP_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ASSIMP DEFAULT_MSG ASSIMP_LIBRARIES ASSIMP_INCLUDE_DIR)

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARIES ASSIMP_LIBRARY)
set(BULLET_INCLUDE_DIR ${G4Y_ROOT_PATH}/3rd/include/bullet)
set(BULLET_LIBRARY
    optimized ${G4Y_ROOT_PATH}/3rd/libs/Release/LinearMath.lib
    optimized ${G4Y_ROOT_PATH}/3rd/libs/Release/BulletCollision.lib
    optimized ${G4Y_ROOT_PATH}/3rd/libs/Release/BulletDynamics.lib
    optimized ${G4Y_ROOT_PATH}/3rd/libs/Release/BulletSoftBody.lib
    debug ${G4Y_ROOT_PATH}/3rd/libs/Debug/LinearMath_Debug.lib
    debug ${G4Y_ROOT_PATH}/3rd/libs/Debug/BulletCollision_Debug.lib
    debug ${G4Y_ROOT_PATH}/3rd/libs/Debug/BulletDynamics_Debug.lib
    debug ${G4Y_ROOT_PATH}/3rd/libs/Debug/BulletSoftBody_Debug.lib
)

set(BULLET_INCLUDE_DIRS "${BULLET_INCLUDE_DIR}")
set(BULLET_LIBRARIES "${BULLET_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BULLET DEFAULT_MSG BULLET_LIBRARIES BULLET_INCLUDE_DIR)

mark_as_advanced(BULLET_INCLUDE_DIR BULLET_LIBRARIES BULLET_LIBRARY)
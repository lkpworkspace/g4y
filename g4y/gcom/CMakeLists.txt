# gcom

include(${G4Y_ROOT_PATH}/g4y/gcom/camera/CMakeLists.txt)
include(${G4Y_ROOT_PATH}/g4y/gcom/graphics/CMakeLists.txt)
include(${G4Y_ROOT_PATH}/g4y/gcom/physics/CMakeLists.txt)
include(${G4Y_ROOT_PATH}/g4y/gcom/shape/CMakeLists.txt)
include(${G4Y_ROOT_PATH}/g4y/gcom/transform/CMakeLists.txt)

set(G4Y_COM_SRC
	${G4Y_CAMERA_SRC}
	${G4Y_GRAPHICS_SRC}
	${G4Y_PHYSICS_SRC}
	${G4Y_SHAPE_SRC}
	${G4Y_TRANSFORM_SRC}
)
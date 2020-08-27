# g4y

include(${G4Y_ROOT_PATH}/g4y/gcom/CMakeLists.txt)

set(G4Y_SRC
    ${G4Y_ROOT_PATH}/g4y/GConfig.hpp
	${G4Y_ROOT_PATH}/g4y/GCommon.h
	${G4Y_ROOT_PATH}/g4y/G4Y.h
    ${G4Y_ROOT_PATH}/g4y/G4Y.cpp
	${G4Y_ROOT_PATH}/g4y/GLog.h
    ${G4Y_ROOT_PATH}/g4y/GLog.cpp
	${G4Y_ROOT_PATH}/g4y/GObj.h
    ${G4Y_ROOT_PATH}/g4y/GObj.cpp
	${G4Y_ROOT_PATH}/g4y/GCom.h
    ${G4Y_ROOT_PATH}/g4y/GCom.cpp
	${G4Y_ROOT_PATH}/g4y/GScene.h
    ${G4Y_ROOT_PATH}/g4y/GScene.cpp
	${G4Y_ROOT_PATH}/g4y/GWorld.h
    ${G4Y_ROOT_PATH}/g4y/GWorld.cpp
	${G4Y_ROOT_PATH}/g4y/GPhyWorld.h
    ${G4Y_ROOT_PATH}/g4y/GPhyWorld.cpp
	${G4Y_ROOT_PATH}/g4y/GOpenGLView.h
    ${G4Y_ROOT_PATH}/g4y/GOpenGLView.cpp
	${G4Y_ROOT_PATH}/g4y/GResourceMgr.h
    ${G4Y_ROOT_PATH}/g4y/GResourceMgr.cpp
)
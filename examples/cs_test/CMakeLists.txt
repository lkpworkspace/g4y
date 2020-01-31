set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# protobuf lib
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS
    ${PROJECT_SOURCE_DIR}/gcom/msg/g4y.proto
)

set(CLIENT_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS}
    ${PROJECT_SOURCE_DIR}/gcom/msg/GMsg.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GPbTransformMsg.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GMsgMgr.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GCliMsgMgr.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GCliMsgProxy.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GSrvMsgMgr.cpp
    TClient.cpp
)

set(SERVER_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS} 
    ${PROJECT_SOURCE_DIR}/gcom/msg/GMsg.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GPbTransformMsg.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GMsgMgr.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GCliMsgMgr.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GCliMsgProxy.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GSrvMsgMgr.cpp
    TServer.cpp
)

ADD_EXECUTABLE(client.out ${PROTO_SRCS} ${PROTO_HDRS} ${CLIENT_TEST_SRCS} )

ADD_EXECUTABLE(server.out ${PROTO_SRCS} ${PROTO_HDRS} ${SERVER_TEST_SRCS} )

target_link_libraries(client.out GL GLEW glm glfw assimp ${PROTOBUF_LIBRARIES} ${BULLET_LIBRARIES})
target_link_libraries(server.out GL GLEW glm glfw assimp ${PROTOBUF_LIBRARIES} ${BULLET_LIBRARIES})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# protobuf lib
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS
    ${PROJECT_SOURCE_DIR}/gcom/msg/g4y.proto
)

set(MSG_SRC
    ${PROJECT_SOURCE_DIR}/gcom/msg/GMsg.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GPbTransformMsg.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GMsgMgr.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GCliMsgMgr.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GCliMsgProxy.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GSrvMsgMgr.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/GMsgNetIO.cpp
    ${PROJECT_SOURCE_DIR}/gcom/msg/msgio/GBoostMsgQueue.cpp
)

set(CLIENT_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS}
    ${MSG_SRC}
    TClient.cpp
)

set(SERVER_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS}
    ${MSG_SRC}
    TServer.cpp
)

ADD_EXECUTABLE(client.out ${PROTO_SRCS} ${PROTO_HDRS} ${CLIENT_TEST_SRCS} )

ADD_EXECUTABLE(server.out ${PROTO_SRCS} ${PROTO_HDRS} ${SERVER_TEST_SRCS} )

target_link_libraries(client.out rt GL GLEW glm glfw assimp ${PROTOBUF_LIBRARIES} ${BULLET_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(server.out rt GL GLEW glm glfw assimp ${PROTOBUF_LIBRARIES} ${BULLET_LIBRARIES} ${Boost_LIBRARIES})
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
)

set(CS_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS}
    ${MSG_SRC}
)

ADD_EXECUTABLE(client.out TClient.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${CS_TEST_SRCS} )
target_link_libraries(client.out ${PROTOBUF_LIBRARIES} ${G4Y_LIBRARIES})

ADD_EXECUTABLE(server.out TServer.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${CS_TEST_SRCS} )
target_link_libraries(server.out ${PROTOBUF_LIBRARIES} ${G4Y_LIBRARIES})

ADD_EXECUTABLE(client Client.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${CS_TEST_SRCS} )
target_link_libraries(client ${PROTOBUF_LIBRARIES} ${G4Y_LIBRARIES})

ADD_EXECUTABLE(server Server.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${CS_TEST_SRCS} )
target_link_libraries(server ${PROTOBUF_LIBRARIES} ${G4Y_LIBRARIES})
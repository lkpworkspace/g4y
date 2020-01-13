set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

set(BULLET_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS} 
    bullet_test.cpp
)

ADD_EXECUTABLE(bullet_test.out ${BULLET_TEST_SRCS})

target_link_libraries(bullet_test.out GL GLEW glm glfw assimp ${BULLET_LIBRARIES})
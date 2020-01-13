set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)


if(USE_GUI)
set(GFY_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS} 
    g4y_test.cpp
)
else()
set(GFY_TEST_SRCS
    ${GCOM_SRCS}
    ${G4Y_SRCS} 
    g4y_test.cpp
)
endif(USE_GUI)

ADD_EXECUTABLE(g4y_test.out ${GFY_TEST_SRCS})

if(USE_GUI)
    target_link_libraries(g4y_test.out GL GLEW glm glfw assimp ${BULLET_LIBRARIES})
endif(USE_GUI)
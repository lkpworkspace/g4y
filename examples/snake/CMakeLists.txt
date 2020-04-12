set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

set(GUI_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GRAPHICS_SRCS}
    ${G4Y_SRCS} 
    snake.cpp
)

ADD_EXECUTABLE(snake.out ${GUI_TEST_SRCS})

target_link_libraries(snake.out ${G4Y_LIBRARIES})
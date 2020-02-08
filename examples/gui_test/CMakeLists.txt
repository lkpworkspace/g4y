set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

set(GUI_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS} 
    gui_test.cpp
)

ADD_EXECUTABLE(gui_test.out ${GUI_TEST_SRCS})

target_link_libraries(gui_test.out ${G4Y_LIBRARIES})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)


if(USE_GRAPHICS)
set(GFY_TEST_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GRAPHICS_SRCS}
    ${G4Y_SRCS} 
    g4y_test.cpp
)
else()
set(GFY_TEST_SRCS
    ${GCOM_SRCS}
    ${G4Y_SRCS} 
    g4y_test.cpp
)
endif(USE_GRAPHICS)

ADD_EXECUTABLE(g4y_test.out ${GFY_TEST_SRCS})

if(USE_GRAPHICS)
    target_link_libraries(g4y_test.out ${G4Y_LIBRARIES})
endif(USE_GRAPHICS)
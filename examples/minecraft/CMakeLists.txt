set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

set(MINECRAFT_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GRAPHICS_SRCS}
    ${G4Y_SRCS} 
    main.cpp
)

ADD_EXECUTABLE(minecraft ${MINECRAFT_SRCS})

target_link_libraries(minecraft ${G4Y_LIBRARIES})
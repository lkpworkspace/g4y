set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

set(GFY_EDITOR_SRCS
    ${GCOM_SRCS} 
    ${GCOM_GUI_SRCS}
    ${G4Y_SRCS} 
    Panel.cpp
    PanelMgr.cpp
    main.cpp
)

ADD_EXECUTABLE(editor.out ${GFY_EDITOR_SRCS})
target_link_libraries(editor.out ${G4Y_LIBRARIES})
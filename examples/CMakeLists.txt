set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(g4y_test)

if(USE_GRAPHICS)
    add_subdirectory(gui_test)
    add_subdirectory(cs_test)
    add_subdirectory(snake)
    add_subdirectory(editor)
    add_subdirectory(minecraft)
endif(USE_GRAPHICS)

add_subdirectory(bullet_test)
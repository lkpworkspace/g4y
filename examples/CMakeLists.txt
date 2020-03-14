set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(g4y_test)

if(USE_GUI)
    add_subdirectory(gui_test)
    add_subdirectory(cs_test)
    add_subdirectory(snake)
endif(USE_GUI)

add_subdirectory(bullet_test)
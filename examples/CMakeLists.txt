add_subdirectory(g4y_test)

if(USE_GUI)
    add_subdirectory(gui_test)
    add_subdirectory(cs_test)
endif(USE_GUI)
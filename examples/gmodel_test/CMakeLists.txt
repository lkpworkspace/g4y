set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

ADD_EXECUTABLE(gmodel_test.out model_test.cpp ${GMODEL_SRCS} ${IMGUI_SRCS} ${STB_SRCS})
target_link_libraries(gmodel_test.out GL GLEW glm glfw assimp)

macro (g4y_depend)
	set(PLATFORM_SPECIFIC_LIBS)

	list(APPEND PLATFORM_SPECIFIC_LIBS OPENGL GLEW GLM Boost Python3 imgui SDL2 ASSIMP)

	# boost
	g4y_find_package(Boost Boost REQUIRED COMPONENTS python37 date_time system log)

	# opengl
	g4y_find_package(OpenGL OPENGL REQUIRED)

	# glew
	g4y_find_package(GLEW GLEW REQUIRED)

	# glm
	g4y_find_package(glm GLM REQUIRED)

	# python3
	g4y_find_package(Python3 Python3 REQUIRED COMPONENTS Interpreter Development)

	# sdl2
	g4y_find_package(SDL2 SDL2 REQUIRED)

	# imgui
	g4y_find_package(imgui imgui REQUIRED)

	# assimp
	g4y_find_package(assimp ASSIMP REQUIRED)

endmacro(g4y_depend)

macro(use_g4y_libs_depend target)
    g4y_depend()
    foreach(platform_lib ${PLATFORM_SPECIFIC_LIBS})
		g4y_use_pkg(${target} ${platform_lib})
    endforeach()
endmacro(use_g4y_libs_depend)


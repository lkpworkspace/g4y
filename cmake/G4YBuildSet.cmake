if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "using toolchain file:" ${CMAKE_TOOLCHAIN_FILE})
endif()

# include helper functions
include(G4YBuildHelpers)

# set common compiler options
# add target compile define function
# add target compile options function
include(G4YConfigDefine)

# config libraries dependence
include(G4YConfigDepend)
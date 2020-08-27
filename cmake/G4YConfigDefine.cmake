#WINDOWS 	= 	Windows Desktop
#WINRT 		= 	Windows RT
#WP8 	  	= 	Windows Phone 8
#ANDROID    =	Android
#IOS		=	iOS
#MACOSX		=	MacOS X
#LINUX      =   Linux

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if(WINRT)
    set(SYSTEM_STRING "Windows RT")
  elseif(WP8)
    set(SYSTEM_STRING "Windows Phone 8")
  else()
    set(WINDOWS TRUE)
    set(SYSTEM_STRING "Windows Desktop")
  endif()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if(ANDROID)
    set(SYSTEM_STRING "Android")
  else()
    set(LINUX TRUE)
    set(SYSTEM_STRING "Linux")
  endif()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  if(IOS)
    set(SYSTEM_STRING "IOS")
  else()
    set(MACOSX TRUE)
    set(APPLE TRUE)
    set(SYSTEM_STRING "Mac OSX")
  endif()
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(COMPILER_STRING ${CMAKE_CXX_COMPILER_ID})
  set(CLANG TRUE)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if(MINGW)
    set(COMPILER_STRING "Mingw GCC")
  else()
    set(COMPILER_STRING "GCC")
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  set(COMPILER_STRING "${CMAKE_CXX_COMPILER_ID} C++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(COMPILER_STRING "Visual Studio C++")
endif()

message(STATUS "System: ${SYSTEM_STRING}, Compiler: ${COMPILER_STRING}")

set(CMAKE_CXX_STANDARD            17)
set(CMAKE_CXX_STANDARD_REQUIRED   YES)

# boost defines
add_definitions(-DBOOST_LOG_DYN_LINK)
set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

# Compiler options
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS
					-wd4251 -wd4244 -wd4334 -wd4005 -wd4820 -wd4710
					-wd4514 -wd4056 -wd4996 -wd4099)
	add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

	# Use inline debug info (/Z7) format. Or internal error may occur.
	# Errors looks like: "xmemory0(592): error C3130: Internal Compiler Error: failed to write injected code block to PDB"
	foreach(lang C CXX)
	string(REGEX REPLACE "/Z[iI7]" "" CMAKE_${lang}_FLAGS_DEBUG "${CMAKE_${lang}_FLAGS_DEBUG}")
	set(CMAKE_${lang}_FLAGS_DEBUG "${CMAKE_${lang}_FLAGS_DEBUG} /Z7")
	endforeach()

endif(MSVC)

 # Set compiler options
 function(use_g4y_compile_options target)
    if(MSVC)
        target_compile_options(${target}
            PUBLIC /MP
        )
    endif()
 endfunction()

function(use_g4y_compile_define target)
  if(WINDOWS)
    target_compile_definitions(${target} 
        PUBLIC WIN32
        PUBLIC _WIN32
        PUBLIC _WINDOWS
        PUBLIC UNICODE
        PUBLIC _UNICODE
        PUBLIC _CRT_SECURE_NO_WARNINGS
        PUBLIC _SCL_SECURE_NO_WARNINGS
    )
    if(BUILD_SHARED_LIBS)
      target_compile_definitions(${target} 
        PUBLIC G4Y_EXPORT_DLL
      )
    endif()
  endif()
endfunction()

cmake_minimum_required(VERSION 3.0.0)

project (proto)

# set module path -------------------------------------
include (${CMAKE_CURRENT_LIST_DIR}/cmake/glob_code.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/cmake/system.cmake)
# -----------------------------------------------------

check_operating_system ()

# enable c++11 and disable mvc++ security warnings
if (NOT MSVC)
	if (CMAKE_GENERATOR STREQUAL Xcode)
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	else ()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")
	endif ()
else ()
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(PROTO_GL_BUILD "Include proto gl" ON)
option(PROTO_GL_DEMO_BUILD "Include proto gl demo" ON)

include (${CMAKE_CURRENT_LIST_DIR}/proto/CMakeLists.txt)

if (PROTO_GL_BUILD)
	include (${CMAKE_CURRENT_LIST_DIR}/proto_gl/CMakeLists.txt)
	if (PROTO_GL_DEMO_BUILD)
		include (${CMAKE_CURRENT_LIST_DIR}/demo/CMakeLists.txt)
	endif ()
endif()

project (proto)
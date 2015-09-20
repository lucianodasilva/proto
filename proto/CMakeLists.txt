cmake_minimum_required(VERSION 3.3)

set ( proto_path ${CMAKE_CURRENT_LIST_DIR})

include (${proto_path}/cmake/environment.cmake)
include (${proto_path}/cmake/modules.cmake)

# setup environment
check_environment ()

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
# -----------------

add_library_module (proto ${proto_path}/modules/proto)
add_library_module (proto_gl ${proto_path}/modules/proto_gl proto)

project (proto)

cmake_minimum_required(VERSION 3.0.0)

project (proto)

# enable c++11
if (NOT MSVC)
	if (CMAKE_GENERATOR STREQUAL Xcode)
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	else ()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")
	endif ()
else ()
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS")
endif ()

# set module path -------------------------------------
include (${CMAKE_CURRENT_LIST_DIR}/cmake/git.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/cmake/glob_code.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/cmake/system.cmake)
# -----------------------------------------------------

check_operating_system ()

include (${CMAKE_CURRENT_LIST_DIR}/proto/CMakeLists.txt)
include (${CMAKE_CURRENT_LIST_DIR}/demo/CMakeLists.txt)

project (proto)
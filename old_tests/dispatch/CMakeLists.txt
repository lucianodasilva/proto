cmake_minimum_required(VERSION 2.8.4)
project(dispatch)

file ( GLOB SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

file ( GLOB HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

add_executable(dispatch ${SOURCE_FILES} ${HEADER_FILES})

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

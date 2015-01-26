cmake_minimum_required(VERSION 2.8.4)
project(datadriven)

# enable c++11
if (NOT ${CMAKE_CXX_COMPILER_ID} EQUAL "MSVC")
	if (CMAKE_GENERATOR STREQUAL Xcode)
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	else ()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -g -Wall")
	endif ()
endif ()

file ( GLOB SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.*
)

include_directories(.)

add_executable(datadriven ${SOURCE_FILES})
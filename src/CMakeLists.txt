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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../build/")

set(SOURCE_FILES
    CMakeLists.txt
    imp_reference.h
    imp_datadriven.h
    harness.h
    harness_timer.h
    main.cpp
    test_math.h
    math.matrixes.h
    math.vectors.h
)

include_directories(.)

add_executable(datadriven ${SOURCE_FILES})
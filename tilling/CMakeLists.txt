cmake_minimum_required(VERSION 2.8.4)

project (tilling)

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

###############################################

set (FREEGLUT_BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set (FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set (FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)

#include_directories(deps/freeglut/include)
#add_subdirectory (deps/freeglut/)
include (deps/freeglut/CMakeLists.txt)
add_definitions(-DFREEGLUT_STATIC)

###############################################

include (deps/glew/CMakeLists.txt)
add_definitions(-DGLEW_STATIC)

###############################################

project(tilling)

file ( GLOB SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

file ( GLOB HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(ADDITIONAL_LIBRARIES ${OPENGL_LIBRARIES})

add_executable(tilling ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(tilling ${ADDITIONAL_LIBRARIES} glew freeglut_static)


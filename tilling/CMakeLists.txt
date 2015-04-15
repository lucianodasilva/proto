cmake_minimum_required(VERSION 2.8.4)

################################################
#option (BUILD_SHARED_LIBS OFF)
set (BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory (deps/SFML-2.2)

include_directories (deps/SFML-2.2/include/)

add_definitions(-DSFML_STATIC)
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
target_link_libraries(tilling ${ADDITIONAL_LIBRARIES} sfml-system sfml-graphics sfml-window glew_custom)

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


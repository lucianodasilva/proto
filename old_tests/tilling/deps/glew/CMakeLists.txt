cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(glew)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set (glew_dir ${CMAKE_CURRENT_LIST_DIR})

file ( GLOB glew_source
	${glew_dir}/src/*.c
	${glew_dir}/include/GL/*.h
)

add_library(glew STATIC ${glew_source})

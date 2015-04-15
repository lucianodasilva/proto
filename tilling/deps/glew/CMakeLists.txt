cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(glew_custom)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set (glew_dir ${CMAKE_CURRENT_LIST_DIR})

file ( GLOB glew_source
	${glew_dir}/src/*.c
	${glew_dir}/include/GL/*.h
)

add_library(glew_custom STATIC ${glew_source})

#target_link_libraries ( glew_custom ${ballistic_gl_dependencies})

#set_property (TARGET glew PROPERTY FOLDER "deps")
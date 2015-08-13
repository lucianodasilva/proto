cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(glew)

find_package (OpenGL REQUIRED)
set (GLEW_LIBRARIES ${OPENGL_LIBRARIES})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set (glew_dir ${CMAKE_CURRENT_LIST_DIR})

add_library(glew STATIC ${glew_dir}/src/glew.c)
set_target_properties (glew PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC;GLEW_MX")

set ( GLEW_LIBRARIES glew ${GLEW_LIBRARIES} )
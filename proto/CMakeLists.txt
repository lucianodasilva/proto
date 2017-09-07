cmake_minimum_required(VERSION 3.0)

project(proto)

set ( proto_path ${CMAKE_CURRENT_LIST_DIR})

include (${proto_path}/cmake/environment.cmake)
include (${proto_path}/cmake/cmake_modules/modules.cmake)

# setup environment
check_environment ()

# enable c++14
set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# core library module
add_static_module (proto 		${proto_path}/modules/proto)

# gl library module
add_static_module (proto_gl 	${proto_path}/modules/proto_gl proto)

# test executable
# add_executable_module (demo 	${proto_path}/modules/demo proto proto_gl)

project (proto)
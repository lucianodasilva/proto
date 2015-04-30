cmake_minimum_required(VERSION 3.0.0)

project (proto)

set ( proto_path ${CMAKE_CURRENT_LIST_DIR})

include ( ${proto_path}/deps/glew/CMakeLists.txt )
add_definitions(-DGLEW_STATIC)

###############################################

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

project(proto)

glob_code ( ${proto_path}/include header_files )
glob_code ( ${proto_path}/src source_files )

include_directories (${proto_path}/include)

add_library ( proto STATIC ${source_files} ${header_files})
target_link_libraries(proto ${OPENGL_LIBRARIES} glew)


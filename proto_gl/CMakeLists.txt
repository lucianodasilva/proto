cmake_minimum_required(VERSION 3.0.0)

project (proto_gl)

set ( proto_gl_path ${CMAKE_CURRENT_LIST_DIR})

# add dependencies ############################

# add glew
add_subdirectory ( ${proto_gl_path}/deps/glew/ )
include_directories ( ${proto_gl_path}/deps/glew/include )

set_property (TARGET glew PROPERTY FOLDER "deps")

###############################################

project(proto_gl)

glob_code ( ${proto_gl_path}/include header_files )
glob_code ( ${proto_gl_path}/src source_files )

include_directories (${proto_gl_path}/include)

add_library ( proto_gl STATIC ${source_files} ${header_files})

set ( PROTO_GL_LIBRARIES proto glew ${GLEW_LIBRARIES} )

target_link_libraries(proto_gl ${PROTO_GL_LIBRARIES} )


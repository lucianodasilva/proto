cmake_minimum_required(VERSION 3.0.0)

project (proto)

set ( proto_path ${CMAKE_CURRENT_LIST_DIR})

# add dependencies ############################

# add glew
add_subdirectory ( ${proto_path}/deps/glew/ )
include_directories ( ${proto_path}/deps/glew/include )
set_property (TARGET glew PROPERTY FOLDER "deps")

# preconfigure glfw
set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set (GLFW_DOCUMENT_INTERNALS OFF CACHE BOOL "" FORCE)
set (GLFW_INSTALL OFF CACHE BOOL "" FORCE)
#set (GLFW_LIBRARIES OFF CACHE BOOL "" FORCE)
set (GLFW_USE_DWM_SWAP_INTERVAL OFF CACHE BOOL "" FORCE)
set (GLFW_USE_EGL OFF CACHE BOOL "" FORCE)
set (GLFW_USE_OPTIMUS_HPG OFF CACHE BOOL "" FORCE)

# add glfw
add_subdirectory ( ${proto_path}/deps/glfw/ )
include_directories( ${proto_path}/deps/glfw/include)

set_property (TARGET glfw PROPERTY FOLDER "deps")

###############################################

project(proto)

glob_code ( ${proto_path}/include header_files )
glob_code ( ${proto_path}/src source_files )

include_directories (${proto_path}/include)

add_library ( proto STATIC ${source_files} ${header_files})

set ( PROTO_LIBRARIES glew ${GLEW_LIBRARIES} glfw ${GLFW_LIBRARIES} )

target_link_libraries(proto ${PROTO_LIBRARIES} )


cmake_minimum_required(VERSION 3.0.0)

project (proto)

set ( proto_path ${CMAKE_CURRENT_LIST_DIR})

include ( ${proto_path}/deps/glew/CMakeLists.txt )
add_definitions(-DGLEW_STATIC)

set_property (TARGET glew PROPERTY FOLDER "deps")

# setup sdl for "proto"
set (SDL_ATOMIC OFF CACHE BOOL "" FORCE)
set (SDL_AUDIO OFF CACHE BOOL "" FORCE)
set (SDL_HAPTIC OFF CACHE BOOL "" FORCE)
set (SDL_JOYSTICK OFF CACHE BOOL "" FORCE)
set (SDL_PTHREADS OFF CACHE BOOL "" FORCE)
set (SDL_THREADS OFF CACHE BOOL "" FORCE)
set (SDL_TIMERS OFF CACHE BOOL "" FORCE)

set (DIRECTX OFF CACHE BOOL "" FORCE)
set (RENDER_D3D OFF CACHE BOOL "" FORCE)

include ( ${proto_path}/deps/sdl2/CMakeLists.txt )

set_property (TARGET SDL2-static PROPERTY FOLDER "deps")

###############################################

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

project(proto)

glob_code ( ${proto_path}/include header_files )
glob_code ( ${proto_path}/src source_files )

include_directories (${proto_path}/include)

add_library ( proto STATIC ${source_files} ${header_files})

set ( proto_dependencies ${OPENGL_LIBRARIES} glew SDL2-static )


target_link_libraries(proto ${proto_dependencies} )


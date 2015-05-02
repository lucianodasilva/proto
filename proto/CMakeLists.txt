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

if (APPLE)
	set(CMAKE_C_FLAGS "-x objective-c")
	set(CMAKE_CXX_FLAGS "-x objective-c++")

	find_library(COCOA_LIBRARY Cocoa)
	find_library(CORE_VIDEO_LIBRARY CoreVideo)

	mark_as_advanced (
		COCOA_LIBRARY,
		CORE_VIDEO_LIBRARY
	)

	set ( proto_dependencies ${OPENGL_LIBRARIES} ${COCOA_LIBRARY} ${CORE_VIDEO_LIBRARY} glew )
else ()
	set ( proto_dependencies ${OPENGL_LIBRARIES} glew )
endif ()

target_link_libraries(proto ${proto_dependencies} )


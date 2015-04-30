cmake_minimum_required(VERSION 3.0.0)

project (tilling)

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

###############################################

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions (-DPROTO_OS_WINDOWS)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions (-DPROTO_OS_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions (-DPROTO_OS_DARWIN)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Sun")
	add_definitions (-DPROTO_OS_SUN)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
	add_definitions (-DPROTO_OS_FREEBSD)
endif ()

if (UNIX)
	add_definitions (-DPROTO_API_UNIX)
elseif (WIN32)
	add_definitions (-DPROTO_API_WIN32)
endif ()

if (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
	add_definitions (-DPROTO_COMPILER_MSVC)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	add_definitions (-DPROTO_COMPILER_CLANG)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
	add_definitions (-DPROTO_COMPILER_GNU)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")
	add_definitions (-DPROTO_COMPILER_SUNPRO)
endif ()

###############################################

include (deps/glew/CMakeLists.txt)
add_definitions(-DGLEW_STATIC)

###############################################

project(tilling)

file ( GLOB SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
${CMAKE_CURRENT_LIST_DIR}/*.mm
)

file ( GLOB HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.h
${CMAKE_CURRENT_LIST_DIR}/*.hpp
)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(ADDITIONAL_LIBRARIES ${OPENGL_LIBRARIES})

add_executable(tilling ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(tilling ${ADDITIONAL_LIBRARIES} glew)


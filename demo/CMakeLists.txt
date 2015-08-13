cmake_minimum_required(VERSION 3.0.0)

project (demo_resources)

file ( GLOB resource_files 
	${CMAKE_CURRENT_LIST_DIR}/resources/*
) 

add_custom_target(
    demo_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    	${CMAKE_CURRENT_LIST_DIR}/resources $<TARGET_FILE_DIR:demo>/resources
    SOURCES ${resource_files}
)

project (demo)

set ( demo_path ${CMAKE_CURRENT_LIST_DIR} )

include_directories(${demo_path})

glob_code ( ${demo_path} source_files )

add_executable(demo ${source_files})
target_link_libraries(demo proto ${PROTO_LIBRARIES})


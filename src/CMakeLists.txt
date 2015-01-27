cmake_minimum_required(VERSION 2.8.4)
project(datadriven)

file ( GLOB SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.*
)

include_directories(.)

add_executable(datadriven ${SOURCE_FILES})

# enable c++11
set_property(TARGET datadriven PROPERTY CXX_STANDARD 11)
set_property(TARGET datadriven PROPERTY CXX_STANDARD_REQUIRED ON)
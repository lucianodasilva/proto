set (proto_gl_module_path ${CMAKE_CURRENT_LIST_DIR})

# additional dependencies
include_directories (
  ${proto_gl_module_path}/deps/glm/include
  ${proto_gl_module_path}/deps/glad/include
)

# "extra" source file
add_module_source_files (proto_gl ${proto_gl_module_path}/deps/glad/src/glad.c)

# find system opengl library and include directory
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_module_dependencies(proto_gl ${OPENGL_LIBRARIES})
# --------
if (APPLE)
  find_library(COCOA_LIBRARY Cocoa)

  mark_as_advanced (
    COCOA_LIBRARY
  )

  add_module_dependencies (proto_gl ${COCOA_LIBRARY})
endif ()

set ( proto_gl_path ${CMAKE_CURRENT_LIST_DIR})

# additional dependencies ############################

# add glew
add_subdirectory ( ${proto_gl_path}/deps/glew/ )
include_directories ( ${proto_gl_path}/deps/glew/include )

set_property (TARGET glew PROPERTY FOLDER "deps")
# --------
if (APPLE)
  find_library(COCOA_LIBRARY Cocoa)
  mark_as_advanced (
    COCOA_LIBRARY
  )

  list( APPEND proto_gl_deps ${COCOA_LIBRARY} glew ${GLEW_LIBRARIES})
else ()
  list( APPEND proto_gl_deps glew ${GLEW_LIBRARIES})
endif ()

set ( proto_gl_path ${CMAKE_CURRENT_LIST_DIR})

# additional dependencies ############################

# --------
if (APPLE)
  find_library(COCOA_LIBRARY Cocoa)
  mark_as_advanced (
    COCOA_LIBRARY
  )

  #list( APPEND proto_gl_deps ${COCOA_LIBRARY} glew ${GLEW_LIBRARIES})
else ()
  #list( APPEND proto_gl_deps glew ${GLEW_LIBRARIES})
endif ()

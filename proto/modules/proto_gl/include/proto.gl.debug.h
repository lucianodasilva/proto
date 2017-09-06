#pragma once

#ifndef	 _proto_gl_debug_h_
#define  _proto_gl_debug_h_

#include <string>
#include <sstream>
#include <iostream>

#include <proto.h>
#include "proto.gl.details.h"

namespace proto {
	namespace debug {

		std::string gl_error_to_string (GLenum error);

#	ifdef _DEBUG

#		define gl_error_guard( scope_name ) auto_guard ([] \
		{ \
			GLenum error = glGetError (); \
			if (error != GL_NO_ERROR) \
				throw runtime_exception ("gl error guard " scope_name " failed with error code [" + proto::debug::gl_error_to_string (error) + "]"); \
		})

#	else

	#	define gl_error_guard( scope_name )

#	endif

	}

}

#endif
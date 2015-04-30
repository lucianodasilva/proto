#pragma once

#ifndef	 _proto_debug_h_
#define  _proto_debug_h_

#include <string>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

#include "proto.details.h"

namespace proto {
	namespace debug {

		std::string gl_error_to_string (GLenum error);

#	ifdef _DEBUG

		class debug_message {
		private:

			std::stringstream	_stream;
			bool				_active;

		public:

			// remove copy
			debug_message () = delete;
			debug_message (const debug_message &) = delete;
			debug_message operator = (const debug_message  &) = delete;

			// new instance
			template < class _t >
			inline debug_message (const _t & v) :
				_active (true),
				_stream ()
			{
				_stream << v;
			}

			// move operator
			inline debug_message (debug_message && o) :
				_active (o._active),
				_stream (std::move (o._stream))
			{
				o.dismiss ();
			}

			template < class _t >
			inline debug_message & operator << (const _t & v) {
				_stream << v;
				return *this;
			}

			inline ~debug_message () {
				if (_active) {
					std::cerr << _stream.str () << std::endl;
				}
			}

			inline void dismiss () { _active = false; }

		};

#		define debug_print proto::debug::debug_message ("[ " __FUNCTION__ " ] ")

#		define gl_error_guard( scope_name ) auto_guard ([] \
		{ \
			GLenum error = glGetError (); \
			if (error != GL_NO_ERROR) \
				debug_print << "gl error guard " scope_name " failed with error code [" << proto::debug::gl_error_to_string (error) << "]"; \
		})

#	else

		class debug_message {
		public:
			template < class _t >
			inline debug_message & operator << (const _t & v) { return *this; }

		};

	#	define debug_print proto::debug::debug_message ()

	#	define gl_error_guard( scope_name )

#	endif

	}

}

#endif
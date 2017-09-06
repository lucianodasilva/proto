#pragma once

#ifndef	_proto_gl_application_h_
#define _proto_gl_application_h_

#include "proto.gl.window.h"

namespace proto {
	namespace gl {

		class application_base : public proto::application_base {
		public:

			virtual expected < window * > create_window(std::string const & title, point const & size_v) = 0;

		};

	}
}

#endif
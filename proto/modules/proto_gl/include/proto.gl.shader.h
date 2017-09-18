#pragma once

#ifndef	_proto_gl_shader_h_
#define _proto_gl_shader_h_

#include <string>
#include <cinttypes>

#include "proto.gl.details.h"

using namespace std;

namespace proto {
	namespace gl {

		enum class shader_type : uint32_t {
			none = GL_NONE,
			vertex = GL_VERTEX_SHADER,
			pixel = GL_FRAGMENT_SHADER
		};

#define proto_shader_source(source) \
	#source

		class shader {
		private:

			GLuint			_shader_id;
			shader_type		_type;
			bool			_is_active;


		public:

			void swap(shader & s);

			shader();
			shader(shader && v);
			~shader();

			shader & operator = (shader && s);

			inline GLuint		id() const { return _shader_id; }
			inline shader_type	type() const { return _type; }

			inline bool			is_shader_ok() const {
				GLint state;
				glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &state);
				return _is_active && state == GL_TRUE;
			}

			static shader compile(shader_type type, const std::string & source);

		};
	}
}

#endif
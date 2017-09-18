#include "proto.gl.program.h"
#include "proto.gl.debug.h"

#include <utility>

namespace proto {
	namespace gl {

		void program::swap(program & p) {
			std::swap(_program_id, p._program_id);
			std::swap(_is_active, p._is_active);
			std::swap(_uniforms, p._uniforms);
		}

		program::program() : _program_id(0), _is_active(false) {}

		program::program(program && v) : program() {
			swap(v);
		}

		program::~program() {
			//gl_error_guard("program delete");
			if (_is_active)
				glDeleteProgram(_program_id);
		}

		program & program::operator = (program && p) {
			swap(p);
			return *this;
		}

		void program::bind() const {
			if (!_is_active)
				return;

			glUseProgram(_program_id);
		}

		program program::link(const shader & vertex, const shader & pixel) {
			//gl_error_guard("program create");

			if (!(vertex.is_shader_ok() && pixel.is_shader_ok())) {
				//debug_print << "cannot link to uncompiled shaders";
				return program();
			}

			program new_p;
			new_p._program_id = glCreateProgram();
			new_p._is_active = true;

			glAttachShader(new_p._program_id, vertex.id());
			glAttachShader(new_p._program_id, pixel.id());

			glLinkProgram(new_p._program_id);

			if (!new_p.is_program_ok()) {
				//debug_print << "program linking failed";
				return program();
			}

			// discover uniforms
			int total = -1;
			glGetProgramiv(new_p._program_id, GL_ACTIVE_UNIFORMS, &total);
			for (int i = 0; i < total; ++i) {
				int name_len = -1, num = -1;
				GLenum type = GL_ZERO;
				char name[100];
				glGetActiveUniform(new_p._program_id, GLuint(i), sizeof(name) - 1,
					&name_len, &num, &type, name);
				name[name_len] = 0;
				GLuint location = glGetUniformLocation(new_p._program_id, name);


#if _debug
				// this is a memory leak ( oh well )
				char * name_copy = new char[name_len + 1];
				std::strcpy(name_copy, +name);

				id_t id = make_id(name_copy);
#else
				id_t id = make_id(name);
#endif

				new_p._uniforms[id] = location;
			}

			return new_p;
		}

		uniform_t program::get_uniform(const id_t & id) const {
			auto it = _uniforms.find(id);

			if (it == _uniforms.cend())
				return std::numeric_limits < uint32_t >::max();

			return it->second;
		}

		void program::set_value(uniform_t location, uint32_t v) {
			//gl_error_guard("set uniform value");
			if (location != std::numeric_limits < uint32_t >::max())
				glUniform1ui(location, v);
		}

		void program::set_value(uniform_t location, int32_t v) {
			//gl_error_guard("set uniform value");
			if (location != std::numeric_limits < uint32_t >::max())
				glUniform1i(location, v);
		}

		void program::set_value(uniform_t location, float v) {
			//gl_error_guard("set uniform value");
			if (location != std::numeric_limits < uint32_t >::max())
				glUniform1f(location, v);
		}

		void program::set_value(uniform_t location, const vec2 & v) {
			//gl_error_guard("set uniform value");
			if (location != std::numeric_limits < uint32_t >::max())
				glUniform2f(location, v.x, v.y);
		}

		void program::set_value(uniform_t location, const vec3 & v) {
			//gl_error_guard("set uniform value");
			if (location != std::numeric_limits < uint32_t >::max())
				glUniform3f(location, v.x, v.y, v.z);
		}

		void program::set_value(uniform_t location, const vec4 & v) {
			//gl_error_guard("set uniform value");
			if (location != std::numeric_limits < uint32_t >::max())
				glUniform4f(location, v.x, v.y, v.z, v.w);
		}

		void program::set_value(uniform_t location, const mat4 & v) {
			//gl_error_guard("set uniform value");
			if (location != std::numeric_limits < uint32_t >::max())
				glUniformMatrix4fv(location, 1, false, (GLfloat *)&v);
		}
	}
}
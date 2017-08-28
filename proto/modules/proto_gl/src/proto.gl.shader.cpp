//#include "proto.gl.shader.h"
//#include "proto.gl.debug.h"
//
//#include <utility>
//
//namespace proto {
//	namespace gl {
//
//		void shader::swap(shader & s) {
//			std::swap(_shader_id, s._shader_id);
//			std::swap(_type, s._type);
//			std::swap(_is_active, s._is_active);
//		}
//
//		shader::shader() :
//			_shader_id(0),
//			_type(shader_type::none),
//			_is_active(false)
//		{}
//
//		shader::shader(shader && v) : shader()
//		{
//			swap(v);
//		}
//
//		shader::~shader() {
//			gl_error_guard("SHADER DELETE");
//			if (_is_active)
//				glDeleteShader(_shader_id);
//		}
//
//		shader & shader::operator = (shader && s) {
//			swap(s);
//			return *this;
//		}
//
//		shader shader::compile(shader_type type, const std::string & source) {
//			gl_error_guard("SHADER COMPILE");
//
//			shader new_s;
//
//			const char * source_ptr = source.c_str();
//			int32_t source_length = source.size();
//
//			new_s._shader_id = glCreateShader((GLenum)type);
//			new_s._type = type;
//			new_s._is_active = true;
//
//			glShaderSource(new_s._shader_id, 1, &source_ptr, &source_length);
//			glCompileShader(new_s._shader_id);
//
//			// evaluate shader compilation
//			GLint compiler_state;
//			glGetShaderiv(new_s._shader_id, GL_COMPILE_STATUS, &compiler_state);
//
//			if (compiler_state == GL_FALSE) {
//				// report compilation issues
//				int32_t
//					info_length,
//					writen_chars;
//
//				char * info_log;
//
//				glGetShaderiv(new_s._shader_id, GL_INFO_LOG_LENGTH, &info_length);
//				info_log = new char[info_length];
//
//				glGetShaderInfoLog(new_s._shader_id, info_length, &writen_chars, info_log);
//
//				string error(info_log, writen_chars);
//				debug_print << "shader " << new_s._shader_id << " compile failed with error: " << error;
//
//				delete[] info_log;
//			}
//			// ---------------------------
//
//			return new_s;
//		}
//	}
//}
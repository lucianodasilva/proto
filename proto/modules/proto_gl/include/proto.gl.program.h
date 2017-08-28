//#pragma once
//
//#ifndef	_proto_gl_program_h_
//#define _proto_gl_program_h_
//
//#include <map>
//
//#include <proto.h>
//#include "proto.gl.deps.h"
//#include "proto.gl.debug.h"
//#include "proto.gl.shader.h"
//
//namespace proto {
//	namespace gl {
//
//		using uniform_t = GLuint;
//
//		class program {
//		private:
//
//			GLuint	_program_id;
//			bool	_is_active;
//
//			std::map < id_t, uniform_t > _uniforms;
//
//
//		public:
//
//			void swap(program & p);
//
//			program();
//			program(program && v);
//			~program();
//
//			program & operator = (program && p);
//
//			inline GLuint	id() const { return _program_id; }
//
//			inline bool		is_program_ok() const {
//				GLint state;
//				glGetProgramiv(_program_id, GL_LINK_STATUS, &state);
//				return state == GL_TRUE;
//			}
//
//			void bind() const;
//
//			static program link(const shader & vertex, const shader & pixel);
//
//			uniform_t get_uniform(const id_t & id) const;
//
//			void set_value(uniform_t location, uint32_t v);
//
//			void set_value(uniform_t location, int32_t v);
//
//			void set_value(uniform_t location, float v);
//
//			void set_value(uniform_t location, const vec2 & v);
//
//			void set_value(uniform_t location, const vec3 & v);
//
//			void set_value(uniform_t location, const vec4 & v);
//
//			void set_value(uniform_t location, const mat4 & v);
//
//			template < class _t >
//			inline void set_value(const char * uniform_name, const _t & v) {
//				set_value(
//					get_uniform(make_id(uniform_name)),
//					v
//					);
//			}
//
//		};
//	}
//}
//
//#endif
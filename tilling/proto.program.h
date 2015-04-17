#pragma once

#ifndef	_proto_program_h_
#define _proto_program_h_

#include <GL/glew.h>
#include <map>

#include "proto.id.h"
#include "proto.shader.h"
#include "proto.math.h"

namespace proto {

	using uniform_t = GLuint;

	class program {
	private:

		GLuint	_program_id;
		bool	_is_active;

		std::map < id_t, uniform_t > _uniforms;

		program ();

	public:

		program (program && v);
		~program ();

		inline GLuint	id () const { return _program_id; }

		inline bool		is_program_ok () const {
			GLint state;
			glGetProgramiv (_program_id, GL_LINK_STATUS, &state);
			return state == GL_TRUE;	
		}

		void bind () const;

		static program link (const shader & vertex, const shader & pixel);

		uniform_t get_uniform (const id_t & id) const;

		inline void set_value (uniform_t location, uint32_t v) {
			if (location != std::numeric_limits < uint32_t >::max ())
				glUniform1ui (location, v);
		}

		inline void set_value (uniform_t location, int32_t v) {
			if (location != std::numeric_limits < uint32_t >::max ())
				glUniform1i (location, v);
		}

		inline void set_value (uniform_t location, float v) {
			if (location != std::numeric_limits < uint32_t >::max ())
				glUniform1f (location, v);
		}

		inline void set_value (uniform_t location, const vec2 & v) {
			if (location != std::numeric_limits < uint32_t >::max ())
				glUniform2f (location, v.x, v.y);
		}

		inline void set_value (uniform_t location, const vec3 & v) {
			if (location != std::numeric_limits < uint32_t >::max ())
				glUniform3f (location, v.x, v.y, v.z);
		}

		inline void set_value (uniform_t location, const vec4 & v) {
			if (location != std::numeric_limits < uint32_t >::max ())
				glUniform4f (location, v.x, v.y, v.z, v.w);
		}

		inline void set_value (uniform_t location, const mat4 & v) {
			if (location != std::numeric_limits < uint32_t >::max ())
				glUniformMatrix4fv (location, 1, false, (GLfloat *)&v);
		}

		template < class _t >
		inline void set_value (const char * uniform_name, const _t & v) {
			set_value (
				get_uniform (create_id (uniform_name)),
				v
			);
		}

	};

}

#endif
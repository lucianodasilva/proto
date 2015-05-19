#include "proto.program.h"
#include "proto.debug.h"

#include <utility>

namespace proto {

	void program::swap (program & p) {
		std::swap (_program_id, p._program_id);
		std::swap (_is_active, p._is_active);
		std::swap (_uniforms, p._uniforms);
	}

	program::program () : _program_id (0), _is_active (false) {}

	program::program (program && v) : program () {
		swap (v);
	}

	program::~program () {
		gl_error_guard ("PROGRAM DELETE");
		if (_is_active)
			glDeleteProgram (_program_id);
	}

	program & program::operator = (program && p) {
		swap (p);
		return *this;
	}

	void program::bind () const {
		if (!_is_active)
			return;

		glUseProgram (_program_id);
	}

	program program::link (const shader & vertex, const shader & pixel) {
		gl_error_guard ("PROGRAM CREATE");

		if (!(vertex.is_shader_ok () && pixel.is_shader_ok ())) {
			debug_print << "cannot link to uncompiled shaders";
			return program ();
		}

		program new_p;
		new_p._program_id = glCreateProgram ();
		new_p._is_active = true;

		glAttachShader (new_p._program_id, vertex.id ());
		glAttachShader (new_p._program_id, pixel.id ());

		glLinkProgram (new_p._program_id);

		if (!new_p.is_program_ok ()) {
			debug_print << "program linking failed";
			return program ();
		}

		// Discover uniforms
		int total = -1;
		glGetProgramiv (new_p._program_id, GL_ACTIVE_UNIFORMS, &total);
		for (int i = 0; i < total; ++i) {
			int name_len = -1, num = -1;
			GLenum type = GL_ZERO;
			char name[100];
			glGetActiveUniform (new_p._program_id, GLuint (i), sizeof (name) - 1,
								&name_len, &num, &type, name);
			name[name_len] = 0;
			GLuint location = glGetUniformLocation (new_p._program_id, name);
			

		#if _DEBUG
			// this is a memory leak ( oh well )
			char * name_copy = new char [name_len + 1];
			std::strcpy (name_copy, +name);

			id_t id = make_id (name_copy);
		#else
			id_t id = make_id (name);
		#endif

			new_p._uniforms [id] = location;
		}

		return new_p;
	}

	uniform_t program::get_uniform (const id_t & id) const {
		auto it = _uniforms.find (id);

		if (it == _uniforms.cend ())
			return std::numeric_limits < uint32_t >::max ();

		return it->second;
	}

}
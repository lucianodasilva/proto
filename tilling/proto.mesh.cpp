#include "proto.mesh.h"
#include "proto.debug.h"

namespace proto {

	size_t mesh::calc_stride (mesh_attributes attributes) {
		size_t stride = 0;

		if (attributes & mesh_attributes::position)
			stride += sizeof (vec3);
		if (attributes & mesh_attributes::uv)
			stride += sizeof (vec2);
		if (attributes & mesh_attributes::normal)
			stride += sizeof (vec3);

		return stride;
	}

	void mesh::define_attributes (mesh_attributes attributes) {
		size_t
			offset = 0,
			stride = calc_stride (attributes);

		if (attributes & mesh_attributes::position) {
			gl_error_guard ("MESH SET POSITION ATTRIBUTE");
			glVertexAttribPointer (
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				stride,
				(GLvoid *)offset
				);

			offset += 3 * sizeof (float);
			glEnableVertexAttribArray (1);
		}

		if (attributes & mesh_attributes::uv) {
			gl_error_guard ("MESH SET UV ATTRIBUTE");
			glVertexAttribPointer (
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				stride,
				(GLvoid *)offset
				);

			offset += 2 * sizeof (float);
			glEnableVertexAttribArray (2);
		}

		if (attributes & mesh_attributes::normal) {
			gl_error_guard ("MESH SET NORMAL ATTRIBUTE");
			glVertexAttribPointer (
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				stride,
				(GLvoid *)offset
				);

			offset += 3 * sizeof (float);
			glEnableVertexAttribArray (3);
		}

	}

    mesh::mesh() :
		_vertex_buffer_id (0),
		_index_buffer_id (0),
		_vbo_id (0),
		_is_active (false)
	{}

	mesh::mesh (mesh && v) {
		using namespace std;

		swap (_vertex_buffer_id, v._vertex_buffer_id);
		swap (_index_buffer_id, v._index_buffer_id);
		swap (_vbo_id, v._vbo_id);
		swap (_is_active, v._is_active);
	}

    mesh::~mesh() {
		gl_error_guard ("MESH DELETE");

		if (_is_active) {
			glBindVertexArray (0);

			glDeleteVertexArrays (1, &_vbo_id);
			glDeleteBuffers (2, &_vertex_buffer_id);
		}
    }

	void mesh::bind () const {
		gl_error_guard ("MESH BIND");
		glBindVertexArray (_vbo_id);
	}

	mesh mesh::create (
		const uint8_t *		vertex_buffer,
		uint32_t			vertex_buffer_size,
		const uint16_t *	index_buffer,
		uint32_t			index_buffer_size,
		mesh_attributes		attributes,
		bool				dynamic
	) {
		gl_error_guard ("MESH CREATE");
		
		mesh new_m;

		// Create vertex buffer	and index buffer
		glGenBuffers (2, &new_m._vertex_buffer_id);

		// Bind vertex buffer
		glBindBuffer (GL_ARRAY_BUFFER, new_m._vertex_buffer_id);

		// Set vertex buffer data
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size, (GLvoid*)vertex_buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		// Create index buffer
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, new_m._index_buffer_id);

		// Set index buffer data
		glBufferData (GL_ELEMENT_ARRAY_BUFFER, index_buffer_size * sizeof (uint16_t), (GLvoid *)index_buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		// Clear buffer bindings
		glBindBuffer (GL_ARRAY_BUFFER, 0);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

		// Create Vertex Array Object
		glGenVertexArrays (1, &new_m._vbo_id);
		glBindVertexArray (new_m._vbo_id);

		// Setup vertex array object
		glBindBuffer (GL_ARRAY_BUFFER, new_m._vertex_buffer_id);

		define_attributes (attributes);

		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, new_m._index_buffer_id);

		// Reset vbo bindings
		glBindVertexArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

		new_m._index_count = index_buffer_size;

		return new_m;
	}

	void mesh::update_vertex_buffer (
		const uint8_t *		vertex_buffer,
		uint32_t			vertex_buffer_size
	) const {
		gl_error_guard ("MESH UPDATE VERTEX BUFFER");

		glBindVertexArray (_vbo_id);
		glBindBuffer (GL_ARRAY_BUFFER, _vertex_buffer_id);

		glBufferSubData (GL_ARRAY_BUFFER, 0, vertex_buffer_size, (GLvoid *)vertex_buffer);
	}

	void mesh::update_index_buffer (
		const uint16_t *	index_buffer,
		uint32_t			index_buffer_size
	) {
		gl_error_guard ("MESH UPDATE INDEX BUFFER");

		glBindVertexArray (_vbo_id);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, _index_buffer_id);

		glBufferSubData (GL_ELEMENT_ARRAY_BUFFER, 0, index_buffer_size * sizeof (uint16_t), (GLvoid *)index_buffer);
		_index_count = index_buffer_size;
	}

	inline void calculate_quad (
		float * address, 
		const vec3 & center, 
		float size, 
		const vec3 & normal
	) {

		float hsize = size / 2.0F;

		vec3 v;

		// top left
		v = proto::math::cross ({ -1.F, 1.F, .0F }, normal) + center;
		// top right
		v = proto::math::cross ({ 1.F, 1.F, .0F }, normal) + center;
		// bottom left
		v = proto::math::cross ({ -1.F, -1.F, .0F }, normal) + center;
		// bottom right
		v = proto::math::cross ({ 1.F, -1.F, .0F }, normal) + center;

	}

	mesh mesh::create_quad (const vec3 & direction, float size) {
		
		



	}

	mesh mesh::create_cube ( float size ) {

		float hsize = size / 2.0F;

		// position | uv

		float vertex [] = {
		// front
			-hsize, hsize, -hsize, 	.0F, .0F,
			hsize, hsize, -hsize, 	1.F, .0F,
			hsize, -hsize, -hsize,	1.F, 1.F,
			-hsize, -hsize, -hsize, .0F, 1.0F,
		// left
			-hsize, hsize, hsize, 	.0F, .0F,
			-hsize, hsize, -hsize, 	1.F, .0F,
			-hsize, -hsize, -hsize,	1.F, 1.F,
			-hsize, -hsize, hsize, 	.0F, 1.0F,
		// right
			hsize, hsize, -hsize, 	.0F, .0F,
			hsize, hsize, hsize, 	1.F, .0F,
			hsize, -hsize, hsize,	1.F, 1.F,
			hsize, -hsize, -hsize,  .0F, 1.0F,
		// back
			hsize, hsize, hsize, 	.0F, .0F,
			-hsize, hsize, hsize, 	1.F, .0F,
			-hsize, -hsize, hsize,	1.F, 1.F,
			hsize, -hsize, hsize, 	.0F, 1.0F,
		// top
			-hsize, hsize, hsize, 	.0F, .0F,
			hsize, hsize, hsize, 	1.F, .0F,
			hsize, hsize, -hsize,	1.F, 1.F,
			-hsize, hsize, -hsize, 	.0F, 1.0F,
		//bottom
			-hsize, -hsize, -hsize, .0F, .0F,
			hsize, -hsize, -hsize, 	1.F, .0F,
			hsize, -hsize, hsize,	1.F, 1.F,
			-hsize, -hsize, hsize,  .0F, 1.0F
		};

		/*return create (
			static_cast < uint8_t * > (vertex),

		)*/

		mesh m;

		return m;
	}
}
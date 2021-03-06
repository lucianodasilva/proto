#include "proto.gl.mesh.h"
#include "proto.gl.mesh_builder.h"
#include "proto.gl.debug.h"

#include <utility>

namespace proto {
	namespace gl {

		size_t mesh::calc_stride(mesh_attributes attributes) {
			size_t stride = 0;

			if (attributes & mesh_attributes::position)
				stride += sizeof(vec3);
			if (attributes & mesh_attributes::uv)
				stride += sizeof(vec2);
			if (attributes & mesh_attributes::normal)
				stride += sizeof(vec3);

			return stride;
		}

		void mesh::define_attributes(mesh_attributes attributes) {
			size_t
				offset = 0,
				stride = calc_stride(attributes);

			if (attributes & mesh_attributes::position) {
				gl_error_guard("MESH SET POSITION ATTRIBUTE");
				glVertexAttribPointer(
					(GLuint)mesh_location::position,
					3,
					GL_FLOAT,
					GL_FALSE,
					stride,
					(GLvoid *)offset
					);

				offset += 3 * sizeof(float);
				glEnableVertexAttribArray(1);
			}

			if (attributes & mesh_attributes::uv) {
				gl_error_guard("MESH SET UV ATTRIBUTE");
				glVertexAttribPointer(
					(GLuint)mesh_location::uv,
					2,
					GL_FLOAT,
					GL_FALSE,
					stride,
					(GLvoid *)offset
					);

				offset += 2 * sizeof(float);
				glEnableVertexAttribArray(2);
			}

			if (attributes & mesh_attributes::normal) {
				gl_error_guard("MESH SET NORMAL ATTRIBUTE");
				glVertexAttribPointer(
					(GLuint)mesh_location::normal,
					3,
					GL_FLOAT,
					GL_FALSE,
					stride,
					(GLvoid *)offset
					);

				offset += 3 * sizeof(float);
				glEnableVertexAttribArray(3);
			}

		}

		void mesh::swap(mesh & m) {
			std::swap(_vertex_buffer_id, m._vertex_buffer_id);
			std::swap(_index_buffer_id, m._index_buffer_id);
			std::swap(_vbo_id, m._vbo_id);
			std::swap(_index_count, m._index_count);
			std::swap(_is_active, m._is_active);
		}

		mesh::mesh() :
			_vertex_buffer_id(0),
			_index_buffer_id(0),
			_vbo_id(0),
			_index_count(0),
			_is_active(false)
		{}

		mesh::mesh(mesh && v) : mesh() {
			swap(v);
		}

		mesh::~mesh() {
			gl_error_guard("MESH DELETE");

			if (_is_active) {
				glBindVertexArray(0);

				glDeleteVertexArrays(1, &_vbo_id);
				glDeleteBuffers(2, &_vertex_buffer_id);
			}
		}

		mesh & mesh::operator = (mesh && v) {
			swap(v);
			return *this;
		}

		void mesh::bind() const {
			gl_error_guard("MESH BIND");
			glBindVertexArray(_vbo_id);
		}

		mesh mesh::create(
			const uint8_t *		vertex_buffer,
			uint32_t			vertex_buffer_size,
			const uint16_t *	index_buffer,
			uint32_t			index_buffer_size,
			mesh_attributes		attributes,
			bool				dynamic
			) {
			gl_error_guard("MESH CREATE");

			mesh new_m;

			// Create vertex buffer
			glGenBuffers(1, &new_m._vertex_buffer_id);
			glBindBuffer(GL_ARRAY_BUFFER, new_m._vertex_buffer_id);

			// Set vertex buffer data
			glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, (GLvoid*)vertex_buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

			// Create index buffer
			glGenBuffers(1, &new_m._index_buffer_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_m._index_buffer_id);

			// Set index buffer data
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size * sizeof(uint16_t), (GLvoid *)index_buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

			// Clear buffer bindings
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// Create Vertex Array Object
			glGenVertexArrays(1, &new_m._vbo_id);
			glBindVertexArray(new_m._vbo_id);

			// Setup vertex array object
			glBindBuffer(GL_ARRAY_BUFFER, new_m._vertex_buffer_id);

			define_attributes(attributes);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_m._index_buffer_id);

			// Reset vbo bindings
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			new_m._index_count = index_buffer_size;
			new_m._is_active = true;

			return new_m;
		}

		void mesh::update_vertex_buffer(
			const uint8_t *		vertex_buffer,
			uint32_t			vertex_buffer_size
			) const {
			gl_error_guard("MESH UPDATE VERTEX BUFFER");

			glBindVertexArray(_vbo_id);
			glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);

			glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer_size, (GLvoid *)vertex_buffer);
		}

		void mesh::update_index_buffer(
			const uint16_t *	index_buffer,
			uint32_t			index_buffer_size
			) {
			gl_error_guard("MESH UPDATE INDEX BUFFER");

			glBindVertexArray(_vbo_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer_id);

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_buffer_size * sizeof(uint16_t), (GLvoid *)index_buffer);
			_index_count = index_buffer_size;
		}


		mesh mesh::create_quad(float size, const vec3 & direction) {
			mesh_builder builder;

			builder.add_quad(size, { .0F, .0F, .0F }, { .0F, 1.0F, .0F }, direction);
			return builder.make_mesh();
		}

		mesh mesh::create_cube(float size) {

			mesh_builder builder;

			vec3
				top = { .0F, 1.F, .0F },
				bottom = { .0F, -1.0F, .0F },
				left = { -1.F, .0F, .0F },
				right = { 1.F, .0F, .0F },
				back = { .0F, .0F, 1.F },
				front = { .0, .0F, -1.F };

			float hs = size / 2.0F;

			vec3 up = { .0F, 1.0F, .0F };

			builder.add_quad(size, top * hs, up, top);
			builder.add_quad(size, bottom * hs, up, bottom);
			builder.add_quad(size, left * hs, up, left);
			builder.add_quad(size, right * hs, up, right);
			builder.add_quad(size, back * hs, up, back);
			builder.add_quad(size, front * hs, up, front);

			return builder.make_mesh();
		}
	}
}
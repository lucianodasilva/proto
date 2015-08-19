#pragma once

#ifndef _proto_gl_mesh_h_
#define _proto_gl_mesh_h_

#include <cinttypes>

#include <proto.h>
#include "proto.gl.constants.h"

namespace proto {
	namespace gl {

		enum class mesh_location : uint32_t {
			position = 1,
			uv = 2,
			normal = 3
		};

		enum class mesh_attributes : uint32_t {
			position = 0x0001,
			uv = 0x0002,
			normal = 0x0004
		};

		inline bool operator & (mesh_attributes v1, mesh_attributes v2) {
			return ((size_t)v1 & (size_t)v2) != 0;
		}

		inline mesh_attributes operator | (mesh_attributes v1, mesh_attributes v2) {
			return (mesh_attributes)((size_t)v1 | (size_t)v2);
		}

		class mesh {
		private:

			static size_t calc_stride(mesh_attributes attributes);
			static void define_attributes(mesh_attributes attributes);

			GLuint
				_vertex_buffer_id,
				_index_buffer_id,
				_vbo_id;

			uint32_t _index_count;

			bool _is_active;

		public:

			void swap(mesh & m);

			uint32_t index_count() const { return _index_count; }

			mesh();
			mesh(mesh && v);
			~mesh();

			mesh & operator = (mesh && m);


			void bind() const;

			static mesh create(
				const uint8_t *		vertex_buffer,
				uint32_t			vertex_buffer_size,
				const uint16_t *	index_buffer,
				uint32_t			index_buffer_size,
				mesh_attributes		attributes,
				bool				dynamic = false
				);

			void update_vertex_buffer(
				const uint8_t *		vertex_buffer,
				uint32_t			vertex_buffer_size
				) const;

			void update_index_buffer(
				const uint16_t *	index_buffer,
				uint32_t			index_buffer_size
				);

			static mesh create_quad(
				float size,
				const vec3 & direction
				);

			static mesh create_cube(
				float size
				);

		};
	}
}

#endif
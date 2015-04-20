#pragma once

#ifndef _proto_mesh_h_
#define _proto_mesh_h_

#include <cinttypes>

#include "proto.formats.h"
#include "proto.math.h"

namespace proto {

	enum class mesh_attributes : uint32_t {
		position =	0x0001,
		uv =		0x0002,
		normal =	0x0004
	};

	bool operator & (mesh_attributes v1, mesh_attributes v2) {
		return ((uint32_t)v1 & (uint32_t)v2) != 0;
	}

    class mesh {
    private:

		static size_t calc_stride (mesh_attributes attributes);
		static void define_attributes (mesh_attributes attributes);

		GLuint
			_vertex_buffer_id,
			_index_buffer_id,
			_vbo_id;
		
		bool _is_active;

		mesh ();

    public:

		mesh (mesh && v);
		~mesh ();

		void bind () const;

		static mesh create (
			const uint8_t *		vertex_buffer,
			uint32_t			vertex_buffer_size,
			const uint16_t *	index_buffer,
			uint32_t			index_buffer_size,
			mesh_attributes		attributes,
			bool				dynamic = false
		);

		void update_vertex_buffer (
			const uint8_t *		vertex_buffer,
			uint32_t			vertex_buffer_size
		) const;

		void update_index_buffer (
				const uint16_t *	index_buffer,
				uint32_t			index_buffer_size
		) const;

		static mesh create_cube (
			float size
		);

    };

}

#endif
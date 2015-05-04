#pragma once

#ifndef _proto_mesh_builder_h_
#define _proto_mesh_builder_h_

#include "proto.formats.h"
#include "proto.math.h"
#include "proto.mesh.h"

#include <vector>

using namespace std;

namespace proto {

	class mesh_builder {
	public:

		struct element {
			vec3 position;
			vec2 uv;
			vec3 normal;
		};

		struct face {
			uint16_t
				v1,
				v2,
				v3;
		};

	private:

		vector < element > 	_elements;
		vector < face > 	_faces;

	public:

		mesh_builder ();
		mesh_builder ( mesh_builder && v );

		uint16_t add_element ( const element & e );
		void add_face ( const face & f );

		void add_quad ( float size, const vec3 & center, const vec3 & up, const vec3 & direction );

		void reset ();

		mesh make_mesh ();

	};

}

#endif
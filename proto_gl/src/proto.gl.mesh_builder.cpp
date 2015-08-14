#include "proto.gl.mesh_builder.h"
#include "proto.gl.debug.h"

namespace proto {
	namespace gl {

		mesh_builder::mesh_builder() {}

		mesh_builder::mesh_builder(mesh_builder && v) {
			using namespace std;
			swap(_elements, v._elements);
			swap(_faces, v._faces);
		}

		uint16_t mesh_builder::add_element(const element  & v) {
			uint16_t index = static_cast <uint16_t> (_elements.size());
			_elements.push_back(v);

			return index;
		}

		void mesh_builder::add_face(const face & f) {
			_faces.push_back(f);
		}

		void mesh_builder::add_quad(
			float size,
			const vec3 & center,
			const vec3 & up,
			const vec3 & direction
			) {
			using namespace proto::math;

			float hs = size / 2.0F;
			auto look_at = make_look_at(center, up, direction);

			vec3
				// top left
				v1 = look_at * vec3{ -hs, hs, .0F },
				// top right
				v2 = look_at * vec3{ hs, hs, .0F },
				// bottom left
				v3 = look_at * vec3{ hs, -hs, .0F },
				// bottom right
				v4 = look_at * vec3{ -hs, -hs, .0F };

			vec3 normal = normalize(direction - center);

			auto
				i1 = add_element({ v1, {.0F, .0F}, normal }),
				i2 = add_element({ v2, {1.F, .0F}, normal }),
				i3 = add_element({ v3, {1.F, 1.F}, normal }),
				i4 = add_element({ v4, {.0F, 1.F}, normal });

			add_face({ i1, i2, i3 });
			add_face({ i1, i3, i4 });
		}

		void mesh_builder::reset() {
			_elements.clear();
			_faces.clear();
		}

		mesh mesh_builder::make_mesh() {

			auto attributes = mesh_attributes::position | mesh_attributes::uv | mesh_attributes::normal;

			auto data = reinterpret_cast <uint8_t *> (_elements.data());
			auto data_size = _elements.size() * sizeof(element);

			auto index = reinterpret_cast <uint16_t *> (_faces.data());
			auto index_count = _faces.size() * 3;

			return mesh::create(data, data_size, index, index_count, attributes, false);

		}
	}
}
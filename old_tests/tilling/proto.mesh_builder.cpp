#include "proto.mesh_builder.h"
#include "proto.debug.h"

namespace proto {

    mesh_builder::mesh_builder() {}

    mesh_builder::mesh_builder ( mesh_builder && v ) {
        using namespace std;
        swap (_elements, v._elements);
        swap (_faces, v._faces);
    }

    uint16_t mesh_builder::add_element ( const element  & v ) {
		uint16_t index = static_cast < uint16_t > (_elements.size ());
        _elements.push_back (v);

        return index;
    }

    void mesh_builder::add_face ( const face & f ) {
        _faces.push_back (f);
    }

    void mesh_builder::add_quad (
            float size,
            const vec3 & center,
            const vec3 & normal
    ) {

        float hs = size / 2.0F;

        vec3
        // top left
			v1 = proto::math::cross ( vec3 { -hs, hs, .0F }, normal) + center,
        // top right
			v2 = proto::math::cross ( vec3 { hs, hs, .0F }, normal) + center,
        // bottom left
            v3 = proto::math::cross ( vec3 { -hs, -hs, .0F }, normal) + center,
        // bottom right
            v4 = proto::math::cross ( vec3 { hs, -hs, .0F }, normal) + center;

        auto
            i1 = add_element ({v1, {.0F, .0F}, normal}),
            i2 = add_element ({v2, {1.F, .0F}, normal}),
            i3 = add_element ({v3, {.0F, 1.F}, normal}),
            i4 = add_element ({v4, {1.F, 1.F}, normal});

        add_face ({i1, i2, i3});
        add_face ({i3, i2, i4});
    }

    void mesh_builder::reset () {
        _elements.clear();
        _faces.clear ();
    }

    mesh mesh_builder::make_mesh () {

        auto attributes = mesh_attributes::position | mesh_attributes::uv | mesh_attributes::normal;

        auto data = reinterpret_cast < uint8_t * > (_elements.data ());
        auto data_size = _elements.size () * sizeof (element);

        auto index = reinterpret_cast < uint16_t * > (_faces.data ());
        auto index_count = _faces.size () * 3;

        return mesh::create (data, data_size, index, index_count, attributes, false);

    }


}
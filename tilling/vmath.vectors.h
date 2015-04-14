#pragma once

#ifndef _vmath_vectors_h_
#define _vmath_vectors_h_

#include <string>

using namespace std;

namespace vmath {

	enum axis_index : uint32_t {
		axis_index_x = 0U,
		axis_index_y = 1U,
		axis_index_z = 2U,
		axis_index_w = 3U
	};

	namespace details {

		template < class value_t >
		struct vec2_t {

			static const uint32_t size = 2;

			union {
				value_t data [size];
				struct { value_t x, y; };
			};

		};

		template < class value_t >
		struct vec3_t {

			static const uint32_t size = 3;

			union {
				value_t data [size];
				struct { value_t x, y, z; };
				struct { value_t r, g, b; };

				vec2_t < value_t > xy;
			};

		};

		template < class value_t >
		struct vec4_t {

			static const uint32_t size = 4;

			union {
				value_t data [size];
				struct { value_t x, y, z, w; };
				struct { value_t r, g, b, a; };

				vec2_t < value_t > xy;
				vec3_t < value_t > xyz;

				vec3_t < value_t > rgb;
			};

		};

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator + (const struct_t < value_t > & v1, const struct_t < value_t > & v2) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = v1.data [i] + v2.data [i];

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator + (const struct_t < value_t > & v1, const value_t & v2) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = v1.data [i] + v2;

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator - (const struct_t < value_t > & v1, const struct_t < value_t > & v2) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = v1.data [i] - v2.data [i];

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator - (const struct_t < value_t > & v1, const value_t & v2) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = v1.data [i] - v2;

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator * (const struct_t < value_t > & v1, const struct_t < value_t > & v2) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = v1.data [i] * v2.data [i];

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator * (const struct_t < value_t > & v, const value_t & s) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = v.data [i] * s;

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator * (const value_t & s, const struct_t < value_t > & v) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = s * v.data [i];

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator / (const struct_t < value_t > & v1, const struct_t < value_t > & v2) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = v1.data [i] / v2.data [i];

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator / (const struct_t < value_t > & v, const value_t & s) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = v.data [i] / s;

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator / (const value_t & s, const struct_t < value_t > & v) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = s / v.data [i];

			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > operator - (const struct_t < value_t > & v) {
			struct_t < value_t > r;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = -v.data [i];

			return r;
		}

	}

}

using point = vmath::details::vec2_t < int32_t >;

using vec2 = vmath::details::vec2_t < float >;
using vec3 = vmath::details::vec3_t < float >;
using vec4 = vmath::details::vec4_t < float >;
	
using ivec2 = vmath::details::vec2_t < int32_t >;
using ivec3 = vmath::details::vec3_t < int32_t >;
using ivec4 = vmath::details::vec4_t < int32_t >;
	
using uvec2 = vmath::details::vec2_t < uint32_t >;
using uvec3 = vmath::details::vec3_t < uint32_t >;
using uvec4 = vmath::details::vec4_t < uint32_t >;
	
using color = vmath::details::vec3_t < float >;
using color4 = vmath::details::vec4_t < float >;

#endif
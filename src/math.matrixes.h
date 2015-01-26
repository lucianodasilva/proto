#ifndef	_math_matrixes_h_
#define _math_matrixes_h_

#include "math.vectors.h"

namespace proto {

#ifdef __clang__
#	pragma clang diagnostic ignored "-Wmissing-braces"
#endif

	namespace math {

		template < class value_t >
		struct mat4_t {

			typedef mat4_t < value_t > this_type;

			static const mat4_t < value_t > identity;

			static const uint32_t size = 16;
			static const uint32_t stride = 4;

			union {
				value_t data[size];
				value_t data_sqr[stride][stride];

				struct {
					value_t m00, m01, m02, m03;
					value_t m04, m05, m06, m07;
					value_t m08, m09, m10, m11;
					value_t m12, m13, m14, m15;
				};
			};

			inline this_type operator * (const value_t & v) const {
				this_type ret;

				for (int i = 0; i < size; ++i)
					ret.data[i] = data[i] * v;

				return ret;
			}

			inline this_type operator * (const this_type & v) const {
				this_type ret;

				for (int i = 0; i < stride; ++i) {
					for (int j = 0; j < stride; ++j) {
						ret.data_sqr[i][j] = value_t();
						for (int k = 0; k < stride; ++k)
							ret.data_sqr[i][j] += (data_sqr[i][k] * v.data_sqr[k][j]);
					}
				}

				return ret;
			}

			inline details::vec3_t < value_t > operator * (const details::vec3_t < value_t > & v) const {
				return{
					(v.data[0] * data[0]) + (v.data[1] * data[4]) + (v.data[2] * data[8]) + (data[12]),
					(v.data[0] * data[1]) + (v.data[1] * data[5]) + (v.data[2] * data[9]) + (data[13]),
					(v.data[0] * data[2]) + (v.data[1] * data[6]) + (v.data[2] * data[10]) + (data[14])
				};
			}

			inline details::vec4_t < value_t > operator * (const details::vec4_t < value_t > & v) const {
				return{
					(v.data[0] * data[0]) + (v.data[1] * data[4]) + (v.data[2] * data[8]) + (v.data[3] * data[12]),
					(v.data[0] * data[1]) + (v.data[1] * data[5]) + (v.data[2] * data[9]) + (v.data[3] * data[13]),
					(v.data[0] * data[2]) + (v.data[1] * data[6]) + (v.data[2] * data[10]) + (v.data[3] * data[14]),
					(v.data[0] * data[3]) + (v.data[1] * data[7]) + (v.data[2] * data[11]) + (v.data[3] * data[15])
				};
			}

			inline this_type operator + (const this_type & v) const {
				this_type ret;

				for (int i = 0; i < size; ++i)
					ret.data[i] = data[i] + v.data[i];

				return ret;
			}

			inline this_type transpose() const {
				return
				{
					data[0],
					data[4],
					data[8],
					data[12],
					data[1],
					data[5],
					data[9],
					data[13],
					data[2],
					data[6],
					data[10],
					data[14],
					data[3],
					data[7],
					data[11],
					data[15]
				};
			}

			inline this_type invert() const {
				value_t inv[16], det;

				inv[0] = data[5] * data[10] * data[15] -
					data[5] * data[11] * data[14] -
					data[9] * data[6] * data[15] +
					data[9] * data[7] * data[14] +
					data[13] * data[6] * data[11] -
					data[13] * data[7] * data[10];

				inv[4] = -data[4] * data[10] * data[15] +
					data[4] * data[11] * data[14] +
					data[8] * data[6] * data[15] -
					data[8] * data[7] * data[14] -
					data[12] * data[6] * data[11] +
					data[12] * data[7] * data[10];

				inv[8] = data[4] * data[9] * data[15] -
					data[4] * data[11] * data[13] -
					data[8] * data[5] * data[15] +
					data[8] * data[7] * data[13] +
					data[12] * data[5] * data[11] -
					data[12] * data[7] * data[9];

				inv[12] = -data[4] * data[9] * data[14] +
					data[4] * data[10] * data[13] +
					data[8] * data[5] * data[14] -
					data[8] * data[6] * data[13] -
					data[12] * data[5] * data[10] +
					data[12] * data[6] * data[9];

				inv[1] = -data[1] * data[10] * data[15] +
					data[1] * data[11] * data[14] +
					data[9] * data[2] * data[15] -
					data[9] * data[3] * data[14] -
					data[13] * data[2] * data[11] +
					data[13] * data[3] * data[10];

				inv[5] = data[0] * data[10] * data[15] -
					data[0] * data[11] * data[14] -
					data[8] * data[2] * data[15] +
					data[8] * data[3] * data[14] +
					data[12] * data[2] * data[11] -
					data[12] * data[3] * data[10];

				inv[9] = -data[0] * data[9] * data[15] +
					data[0] * data[11] * data[13] +
					data[8] * data[1] * data[15] -
					data[8] * data[3] * data[13] -
					data[12] * data[1] * data[11] +
					data[12] * data[3] * data[9];

				inv[13] = data[0] * data[9] * data[14] -
					data[0] * data[10] * data[13] -
					data[8] * data[1] * data[14] +
					data[8] * data[2] * data[13] +
					data[12] * data[1] * data[10] -
					data[12] * data[2] * data[9];

				inv[2] = data[1] * data[6] * data[15] -
					data[1] * data[7] * data[14] -
					data[5] * data[2] * data[15] +
					data[5] * data[3] * data[14] +
					data[13] * data[2] * data[7] -
					data[13] * data[3] * data[6];

				inv[6] = -data[0] * data[6] * data[15] +
					data[0] * data[7] * data[14] +
					data[4] * data[2] * data[15] -
					data[4] * data[3] * data[14] -
					data[12] * data[2] * data[7] +
					data[12] * data[3] * data[6];

				inv[10] = data[0] * data[5] * data[15] -
					data[0] * data[7] * data[13] -
					data[4] * data[1] * data[15] +
					data[4] * data[3] * data[13] +
					data[12] * data[1] * data[7] -
					data[12] * data[3] * data[5];

				inv[14] = -data[0] * data[5] * data[14] +
					data[0] * data[6] * data[13] +
					data[4] * data[1] * data[14] -
					data[4] * data[2] * data[13] -
					data[12] * data[1] * data[6] +
					data[12] * data[2] * data[5];

				inv[3] = -data[1] * data[6] * data[11] +
					data[1] * data[7] * data[10] +
					data[5] * data[2] * data[11] -
					data[5] * data[3] * data[10] -
					data[9] * data[2] * data[7] +
					data[9] * data[3] * data[6];

				inv[7] = data[0] * data[6] * data[11] -
					data[0] * data[7] * data[10] -
					data[4] * data[2] * data[11] +
					data[4] * data[3] * data[10] +
					data[8] * data[2] * data[7] -
					data[8] * data[3] * data[6];

				inv[11] = -data[0] * data[5] * data[11] +
					data[0] * data[7] * data[9] +
					data[4] * data[1] * data[11] -
					data[4] * data[3] * data[9] -
					data[8] * data[1] * data[7] +
					data[8] * data[3] * data[5];

				inv[15] = data[0] * data[5] * data[10] -
					data[0] * data[6] * data[9] -
					data[4] * data[1] * data[10] +
					data[4] * data[2] * data[9] +
					data[8] * data[1] * data[6] -
					data[8] * data[2] * data[5];

				det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];

				if (det == 0)
					return this_type();

				det = float(1) / det;

				return{
					inv[0] * det,
					inv[1] * det,
					inv[2] * det,
					inv[3] * det,
					inv[4] * det,
					inv[5] * det,
					inv[6] * det,
					inv[7] * det,
					inv[8] * det,
					inv[9] * det,
					inv[10] * det,
					inv[11] * det,
					inv[12] * det,
					inv[13] * det,
					inv[14] * det,
					inv[15] * det
				};
			}

			inline static this_type make_translation(const details::vec3_t < value_t > & p) {
				return{
					value_t(1), value_t(0), value_t(0), value_t(0),
					value_t(0), value_t(1), value_t(0), value_t(0),
					value_t(0), value_t(0), value_t(1), value_t(0),
					p.x, p.y, p.z, value_t(1)
				};
			}

			inline static this_type make_translation(const details::vec4_t < value_t > & p) {
				return{
					value_t(1), value_t(0), value_t(0), value_t(0),
					value_t(0), value_t(1), value_t(0), value_t(0),
					value_t(0), value_t(0), value_t(1), value_t(0),
					p.x, p.y, p.z, p.w
				};
			}

			inline static this_type make_rotation_x(value_t r) {
				value_t
					r_sin = sin(r),
					r_cos = cos(r);

				return{
					value_t(1), value_t(0), value_t(0), value_t(0),
					value_t(0), r_cos, r_sin, value_t(0),
					value_t(0), -r_sin, r_cos, value_t(0),
					value_t(0), value_t(0), value_t(0), value_t(1)
				};
			}

			inline static this_type make_rotation_y(value_t r) {
				value_t
					r_sin = sin(r),
					r_cos = cos(r);

				return{
					r_cos, value_t(0), -r_sin, value_t(0),
					value_t(0), value_t(1), value_t(0), value_t(0),
					r_sin, value_t(0), r_cos, value_t(0),
					value_t(0), value_t(0), value_t(0), value_t(1)
				};
			}

			inline static this_type make_rotation_z(value_t r) {
				value_t
					r_sin = sin(r),
					r_cos = cos(r);

				return{
					r_cos, r_sin, value_t(0), value_t(0),
					-r_sin, r_cos, value_t(0), value_t(0),
					value_t(0), value_t(0), value_t(1), value_t(0),
					value_t(0), value_t(0), value_t(0), value_t(1)
				};
			}

			inline static this_type make_scale(const details::vec3_t < value_t > & p) {
				return{
					p.x, value_t(0), value_t(0), value_t(0),
					value_t(0), p.y, value_t(0), value_t(0),
					value_t(0), value_t(0), p.z, value_t(0),
					value_t(0), value_t(0), value_t(0), value_t(1)
				};
			}

			inline static this_type make_scale(const details::vec4_t < value_t > & p) {
				return{
					p.x, value_t(0), value_t(0), value_t(0),
					value_t(0), p.y, value_t(0), value_t(0),
					value_t(0), value_t(0), p.z, value_t(0),
					value_t(0), value_t(0), value_t(0), p.w
				};
			}


		};

		template < class value_t >
		const mat4_t < value_t > mat4_t < value_t >::identity = {
			value_t(1), value_t(0), value_t(0), value_t(0),
			value_t(0), value_t(1), value_t(0), value_t(0),
			value_t(0), value_t(0), value_t(1), value_t(0),
			value_t(0), value_t(0), value_t(0), value_t(1)
		};

		using mat4 = math::mat4_t < float > ;
	}
}

#endif
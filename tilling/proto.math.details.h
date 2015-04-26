#pragma once

#ifndef	_proto_math_details_h_
#define _proto_math_details_h_

#include <cmath>
#include <limits>

#include "proto.math.vectors.h"
#include "proto.math.matrixes.h"

namespace proto {
	namespace math {

		float const pi =
		float (
		3.1415926535897932384626433832795);

		template < class _t >
		inline _t radians(const _t & degrees) {
			return degrees * (pi / float (180));
		}

		template < class _t, template < class > class _struct_t >
		inline _t dot (
				const _struct_t < _t > & v1,
				const _struct_t < _t > &v2
		) {
			// avoid branching by avoiding zero ( floating point precision exploit )
			// returns minimum positive number for min
			_t _res = std::numeric_limits < _t >::min();

			for (uint32_t i = 0; i < _struct_t < _t >::size; ++i)
				_res += v1.data[i] * v2.data[i];

			return _res;
		}

		template < class _t , template < class > class _struct_t >
		inline _t sqr_length ( const _struct_t < _t > &v ) {
			return dot < _t , _struct_t > (v, v);
		}

		template < class _t , template < class > class _struct_t >
		inline _t length ( const _struct_t < _t >
		&v) {
			return std::sqrt (sqr_length < _t, _struct_t > (v));
		}

		template < class _t, template < class > class _struct_t >
		inline _t length ( const _struct_t < _t >
		&v1,
		const _struct_t < _t >
		&v2) {
			return length < _t, _struct_t > (v2 - v1);
		}

		template < class _t >
		inline details::vec3_t < _t > cross(
				const details::vec3_t < _t > & v1,
				const details::vec3_t < _t > &v2
		){
			return {
					v1.y * v2.z - v1.z * v2.y,
					v1.z * v2.x - v1.x * v2.z,
					v1.x * v2.y - v1.y * v2.x
			};
		}

		template < class
		_t, template < class > class
		_struct_t >
		inline _struct_t <_t> normalize(const _struct_t <_t>
		&v ) {
			_t norm = _t(1) / length<_t, _struct_t>(v);
			return v * norm;
		}

		inline float lerp(float v1, float v2, float alpha) {
			return v1 + ((v2 - v1) * alpha);
		}

		template < class _t, template < class > class _struct_t >
		inline _struct_t < _t > lerp (
				const _struct_t <_t> & v1,
				const _struct_t <_t> & v2,
				float alpha
		) {
			_struct_t <_t> v;

			for (int i = 0; i < _struct_t<_t>::size; ++i)
				v.data[i] = lerp(v1.data[i], v2.data[i], alpha);

			return v;
		}

		template < class _t, template < class > class _struct_t >
		inline void lerp(
				const _struct_t <_t> & v1,
				const _struct_t <_t> & v2,
				float alpha,
				_struct_t<_t> & r
		) {
			for (int i = 0; i < _struct_t<_t>::size; ++i)
				r.data[i] = lerp(v1.data[i], v2.data[i], alpha);
		}

		// naive implementation
		template < class _t >
		inline _t clamp (
				const _t & v,
				const _t & min_v,
				const _t & max_v
		) {
			if (v < min_v) return min_v;
			if (v > max_v) return max_v;
			return v;
		}

		template < class _t >
		inline details::mat4_t < _t > make_translation(
				const details::vec3_t < _t > p
		) {
			return {
					_t(1), _t(0), _t(0), _t(0),
					_t(0), _t(1), _t(0), _t(0),
					_t(0), _t(0), _t(1), _t(0),
					p.x, p.y, p.z, _t(1)
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_translation(
				const details::vec4_t < _t > & p) {
			return {
					_t(1), _t(0), _t(0), _t(0),
					_t(0), _t(1), _t(0), _t(0),
					_t(0), _t(0), _t(1), _t(0),
					p.x, p.y, p.z, p.w
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_rotation_x( _t r ) {
			_t
					r_sin = sin(r),
					r_cos = cos(r);

			return {
					_t(1), _t(0), _t(0), _t(0),
					_t(0), r_cos, r_sin, _t(0),
					_t(0), -r_sin, r_cos, _t(0),
					_t(0), _t(0), _t(0), _t(1)
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_rotation_y (_t r) {
			_t
					r_sin = sin(r),
					r_cos = cos(r);

			return {
					r_cos, _t(0), -r_sin, _t(0),
					_t(0), _t(1), _t(0), _t(0),
					r_sin, _t(0), r_cos, _t(0),
					_t(0), _t(0), _t(0), _t(1)
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_rotation_z (_t r) {
			_t
					r_sin = sin(r),
					r_cos = cos(r);

			return {
					r_cos, r_sin, _t(0), _t(0),
					-r_sin, r_cos, _t(0), _t(0),
					_t(0), _t(0), _t(1), _t(0),
					_t(0), _t(0), _t(0), _t(1)
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_scale(
				const details::vec3_t < _t > & p
		) {
			return {
					p.x, _t(0), _t(0), _t(0),
					_t(0), p.y, _t(0), _t(0),
					_t(0), _t(0), p.z, _t(0),
					_t(0), _t(0), _t(0), _t(1)
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_scale(
				const details::vec4_t < _t > & p
		) {
			return {
					p.x, _t(0), _t(0), _t(0),
					_t(0), p.y, _t(0), _t(0),
					_t(0), _t(0), p.z, _t(0),
					_t(0), _t(0), _t(0), p.w
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_ortho (
				_t left,
				_t top,
				_t right,
				_t bottom,
				_t near_v,
				_t far_v
		) {
			return {
					_t (2) / (right - left), _t (0), _t (0), _t (0),
					_t (0), _t (2) / (top - bottom), _t (0), _t (0),
					_t (0), _t (0), _t (1) / (far_v - near_v), _t (0),
					_t (0), _t (0), near_v / (near_v - far_v), _t (1)
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_perspective (
				int width,
				int height,
				_t fovy,
				_t near_v,
				_t far_v
		) {

			_t aspect = _t (width) / _t (height);

			_t
					fov_r = proto::math::radians (fovy),
					range = std::tan (fov_r / _t (2)) * near_v,
					l = -range * aspect,
					r = range * aspect,
					b = -range,
					t = range;

			return {
					(_t (2) * near_v) / (r - l), _t (0), _t (0), _t (0),
					_t (0), (_t (2) * near_v) / (t - b), _t (0), _t (0),
					_t (0), _t (0), -(far_v + near_v) / (far_v - near_v), _t (-1),
					_t (0), _t (0), -(_t (2) * far_v * near_v) / (far_v - near_v), _t (0)
			};
		}

		template < class _t >
		inline details::mat4_t < _t > make_look_at (
				const details::vec3_t < _t > & position,
				const details::vec3_t < _t > & up,
				const details::vec3_t < _t > & target
		) {

			details::vec3_t < _t > zaxis = normalize (target - position);
			details::vec3_t < _t > yaxis = normalize (up);
			details::vec3_t < _t > xaxis = normalize (cross (zaxis, yaxis));

			yaxis = math::cross (xaxis, zaxis);

			return{
					xaxis.x, yaxis.x, -zaxis.x, _t (0),
					xaxis.y, yaxis.y, -zaxis.y, _t (0),
					xaxis.z, yaxis.z, -zaxis.z, _t (0),
					-dot (xaxis, position), -dot (yaxis, position), dot (zaxis, position), _t (1)
			};
		}

	}
}

#endif
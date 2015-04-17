#pragma once

#ifndef _proto_texture_h_
#define _proto_texture_h_

#include <cinttypes>

#include "proto.formats.h"

namespace proto {

    class texture {
    private:

		GLuint			_texture_id;
		bool			_is_active;

		texture_format	_format;

		texture_filter
			_mag_filter,
			_min_filter;

		texture_wrap
			_wrap_s,
			_wrap_t;

		uint32_t
			_width,
			_height;

		texture ();

    public:

		inline GLuint id () const { return _texture_id; }

		inline texture_filter mag_filter () const { return _mag_filter; }
		inline texture_filter min_filter () const { return _min_filter; }

		inline texture_wrap wrap_s () const { return _wrap_s; }
		inline texture_wrap wrap_t () const { return _wrap_t; }

		inline uint32_t width () const { return _width; }
		inline uint32_t height () const { return _height; }

		texture (texture && v);
		~texture ();

		inline void bind () const { glBindTexture (GL_TEXTURE_2D, _texture_id); }

		static texture create (
			const uint8_t * data,
			uint32_t width,
			uint32_t height,
			texture_format format,
			texture_wrap wrap_s			= texture_wrap::clamp_to_border,
			texture_wrap wrap_t			= texture_wrap::clamp_to_border,
			texture_filter mag_filter	= texture_filter::nearest,
			texture_filter min_filter	= texture_filter::nearest
		);

		void update (
			const uint8_t * data,
			uint32_t offset_x,
			uint32_t offset_y,
			uint32_t width,
			uint32_t height
		) const ;

    };

}

#endif
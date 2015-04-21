#pragma once

#ifndef _proto_texture_h_
#define _proto_texture_h_

#include <cinttypes>

#include "proto.formats.h"
#include "proto.math.h"

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


    public:

		inline GLuint id () const { return _texture_id; }

		inline texture_filter mag_filter () const { return _mag_filter; }
		inline texture_filter min_filter () const { return _min_filter; }

		inline texture_wrap wrap_s () const { return _wrap_s; }
		inline texture_wrap wrap_t () const { return _wrap_t; }

		inline uint32_t width () const { return _width; }
		inline uint32_t height () const { return _height; }

		void swap (texture & t);

		texture ();
		texture (texture && v);
		~texture ();

		texture & operator = (texture && t);

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

		static texture create_checkers (
			uint32_t width, uint32_t height, 
			uint32_t div_x, uint32_t div_y,
			color4 on_color = { 1.F, 1.F, .0F, 1.F },
			color4 off_color = { .0F, .0F, .0F, 1.F },
			texture_wrap wrap_s = texture_wrap::clamp_to_border,
			texture_wrap wrap_t = texture_wrap::clamp_to_border,
			texture_filter mag_filter = texture_filter::nearest,
			texture_filter min_filter = texture_filter::nearest
		);

    };

}

#endif
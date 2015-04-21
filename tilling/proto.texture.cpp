#include "proto.texture.h"
#include "proto.color.h"
#include "proto.debug.h"

#include <utility>

namespace proto {

	void texture::swap (texture & t) {
		std::swap (_texture_id, t._texture_id);
		std::swap (_is_active, t._is_active);
		std::swap (_format, t._format);
		std::swap (_mag_filter, t._mag_filter);
		std::swap (_min_filter, t._min_filter);
	}

    texture::texture() :
		_texture_id (0),
		_is_active (0),
		_width (0),
		_height (0)
	{}

	texture::texture (texture && v) : texture () {
		swap (v);
	}

    texture::~texture() {
		gl_error_guard ("TEXTURE DELETE");
		if (_is_active)
			glDeleteTextures (1, &_texture_id);
    }

	texture & texture::operator = (texture && t) {
		swap (t);
		return *this;
	}

	texture texture::create (
		const uint8_t * data,
		uint32_t width,
		uint32_t height,
		texture_format format,
		texture_wrap wrap_s,
		texture_wrap wrap_t,
		texture_filter mag_filter,
		texture_filter min_filter
	) {
		gl_error_guard ("TEXTURE CREATE");
		
		texture new_t;

		glGenTextures (1, &new_t._texture_id);
		
		new_t._is_active = true;
		new_t._format = format;
		new_t._width = width;
		new_t._height = height;
		new_t._mag_filter = mag_filter;
		new_t._min_filter = min_filter;
		new_t._wrap_s = wrap_s;
		new_t._wrap_t = wrap_t;
		
		glBindTexture (GL_TEXTURE_2D, new_t._texture_id);
		glTexImage2D (
			GL_TEXTURE_2D,
			0,
			(GLint)new_t._format,
			width,
			height,
			0,
			(GLint)new_t._format,
			GL_UNSIGNED_BYTE,
			data
		);

		glGenerateMipmap (GL_TEXTURE_2D);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)new_t._mag_filter);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)new_t._min_filter);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)new_t._wrap_s);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)new_t._wrap_t);

		// offset for better interpolation quality
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0F);

		return new_t;
	}

	void texture::update (
		const uint8_t * data,
		uint32_t offset_x,
		uint32_t offset_y,
		uint32_t width,
		uint32_t height
	) const {
		gl_error_guard ("TEXTURE UPDATE");

		glBindTexture (GL_TEXTURE_2D, _texture_id);
		glTexSubImage2D (
			GL_TEXTURE_2D,
			0,
			offset_x,
			offset_y,
			width,
			height,
			(GLint)_format,
			GL_UNSIGNED_BYTE,
			data
		);

		glGenerateMipmap (GL_TEXTURE_2D);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)_mag_filter);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)_min_filter);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)_wrap_s);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)_wrap_t);

		// offset for better interpolation quality
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0F);

	}

	texture texture::create_checkers (
		uint32_t width, uint32_t height,
		uint32_t div_x, uint32_t div_y,
		color4 on_color, color4 off_color,
		texture_wrap wrap_s = texture_wrap::clamp_to_border,
		texture_wrap wrap_t = texture_wrap::clamp_to_border,
		texture_filter mag_filter = texture_filter::nearest,
		texture_filter min_filter = texture_filter::nearest
	) {
		
		uint8_t on_c = color4_int::from_color (on_color);
		uint8_t off_c = color4_int::from_color (off_color);

		uint32_t size = width * height;
		
		uint32_t * texture_data = new uint32_t[size];

		uint32_t
			ix = 0,
			iy = 0,
			dx = width / div_x,
			dy = height / div_y;
		
		for (int i = 0; i < size; ++i) 
		{
			iy = i / width;
			ix = (i - (iy * width));

			iy /= dy;
			ix /= dx;

			if (ix % 2 ^ iy % 2) {
				texture_data[i] = on_c;
			} else {
				texture_data[i] = off_c;
			}
		}

		auto_guard ([texture_data] { delete[] texture_data; });

		return create (
			reinterpret_cast < uint8_t * > (texture_data),
			width, height, texture_format::rgba,
			wrap_s, wrap_t, mag_filter, min_filter
		);
	}

}
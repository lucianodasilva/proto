#include "proto.texture.h"
#include "proto.debug.h"

namespace proto {

    texture::texture() {
		gl_error_guard ("TEXTURE CREATE");
		glGenTextures (1, &_texture_id);

    }

    texture::~texture() {
		gl_error_guard ("TEXTURE DELETE");
		glDeleteTextures (1, &_texture_id);
    }

	void texture::set (
		const uint8_t * data,
		uint32_t width,
		uint32_t height,
		texture_format format,
		texture_wrap wrap_s,
		texture_wrap wrap_t,
		texture_filter mag_filter,
		texture_filter min_filter
	) {
		gl_error_guard ("TEXTURE SET");
		
		_format = format;

		_width = width;
		_height = height;

		_mag_filter = mag_filter;
		_min_filter = min_filter;

		_wrap_s = wrap_s;
		_wrap_t = wrap_t;
		
		glBindTexture (GL_TEXTURE_2D, _texture_id);
		glTexImage2D (
			GL_TEXTURE_2D,
			0,
			(GLint)_format,
			width,
			height,
			0,
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
}
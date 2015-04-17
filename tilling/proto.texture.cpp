#include "proto.texture.h"
#include "proto.debug.h"

namespace proto {

    texture::texture() :
		_texture_id (0),
		_is_active (0),
		_width (0),
		_height (0)
	{}

	texture::texture (texture && v) {
		using namespace std;

		swap (_texture_id, v._texture_id);
		swap (_is_active, v._is_active);
		swap (_format, v._format);
		swap (_mag_filter, v._mag_filter);
		swap (_min_filter, v._min_filter);
	}

    texture::~texture() {
		gl_error_guard ("TEXTURE DELETE");
		if (_is_active)
			glDeleteTextures (1, &_texture_id);
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
}
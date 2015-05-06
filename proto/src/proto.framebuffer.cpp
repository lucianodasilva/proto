#include "proto.framebuffer.h"
#include "proto.debug.h"

namespace proto {

	void framebuffer::swap(framebuffer & v) {
		std::swap(_framebuffer_id, v._framebuffer_id);
		std::swap(_auxiliar_id, v._auxiliar_id);
		std::swap(_is_active, v._is_active);
		std::swap(_has_renderbuffer, v._has_renderbuffer);
		std::swap(_has_no_backbuffer, v._has_no_backbuffer);
	}

	framebuffer::framebuffer () :
		_is_active (false),
		_has_renderbuffer (false),
		_has_no_backbuffer (false)
	{}

	framebuffer::framebuffer (framebuffer && v) {
		swap(v);
	}

	framebuffer::~framebuffer () {
		gl_error_guard ("FRAMEBUFFER DELETE");

		if (_is_active) {
			glDeleteFramebuffers (1, &_framebuffer_id);

			if (_has_renderbuffer)
				glDeleteRenderbuffers (1, &_auxiliar_id);
		}
	}

	framebuffer & framebuffer::operator = (framebuffer && v) {
		swap(v);
		return *this;
	}

	void framebuffer::bind () const {
		if (_is_active) {
			gl_error_guard("FRAMEBUFFER BIND");
			glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer_id);
		}
	}

	framebuffer framebuffer::create (const texture & color_buffer) {
		gl_error_guard ("FRAMEBUFFER COLOR BUFFER CREATE");

		framebuffer new_f;

		glGenFramebuffers (1, &new_f._framebuffer_id);
		
		new_f._is_active = true;
		new_f._has_renderbuffer = true;

		glBindFramebuffer(GL_FRAMEBUFFER, new_f._framebuffer_id);

		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer.id (), 0);

		// create depth buffer
		glGenRenderbuffers (1, &new_f._auxiliar_id);
		glBindRenderbuffer (GL_RENDERBUFFER, new_f._auxiliar_id);
		glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, color_buffer.width (), color_buffer.height ());

		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, new_f._auxiliar_id);

		auto status = glCheckFramebufferStatus (GL_FRAMEBUFFER);
		
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			debug_print << "failed creating framebuffer";
			return framebuffer ();
		}

		return new_f;
	}

	framebuffer framebuffer::create (const texture & color_buffer, const texture & depth_buffer) {
		gl_error_guard ("FRAMEBUFFER COLOR BUFFER CREATE");

		framebuffer new_f;

		glCreateFramebuffers (1, &new_f._framebuffer_id);

		new_f._is_active = true;
		new_f.bind ();

		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer.id (), 0);
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer.id (), 0);
		
		auto status = glCheckFramebufferStatus (GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE) {
			debug_print << "failed creating framebuffer";
			return framebuffer ();
		}

		return new_f;
	}

	framebuffer framebuffer::create_depth_only (const texture & depth_buffer) {
		gl_error_guard ("FRAMEBUFFER COLOR BUFFER CREATE");

		framebuffer new_f;

		glCreateFramebuffers (1, &new_f._framebuffer_id);

		new_f._is_active = true;
		new_f._has_no_backbuffer = true;
		new_f.bind ();

		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer.id (), 0);

		auto status = glCheckFramebufferStatus (GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE) {
			debug_print << "failed creating framebuffer";
			return framebuffer ();
		}

		return new_f;
	}
  
}
#include "proto.framebuffer.h"
#include "proto.debug.h"

namespace proto {

	framebuffer::framebuffer () :
		_uses_default (false)
	{
		gl_error_guard ("FRAMEBUFFER CREATE");
		glGenFramebuffers (1, &_framebuffer_id);
	}

	framebuffer::~framebuffer () {
		gl_error_guard ("FRAMEBUFFER DELETE");
		glDeleteFramebuffers (1, &_framebuffer_id);

		if (_uses_default)
			glDeleteRenderbuffers (1, &_default_depth_id);
	}

	void framebuffer::set_target (const texture & tex)
	{
		gl_error_guard ("FRAMEBUFFER SET COLOR TARGET");
		bind ();
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.id (), 0);
		
		// set draw buffers
		GLenum draw_buffer_type = GL_COLOR_ATTACHMENT0;
		glDrawBuffers (1, &draw_buffer_type);

		// set default depth buffer
		glGenRenderbuffers (1, &_default_depth_id);
		glBindRenderbuffer (GL_RENDERBUFFER, _default_depth_id);
		glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, tex.width (), tex.height ());

		_uses_default = true;

		// attach to framebuffer
		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _default_depth_id);

		auto status = glCheckFramebufferStatus (GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
			debug_print << "Invalid framebuffer configuration!";
	}
  
}
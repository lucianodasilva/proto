#pragma once

#ifndef _proto_framebuffer_h_
#define _proto_framebuffer_h_

#include <GL/glew.h>
#include "proto.texture.h"

namespace proto {

    class framebuffer {
    private:

		GLuint	_framebuffer_id;
		GLuint	_default_depth_id;
		bool	_uses_default;

    public:

		inline void bind () const { glBindFramebuffer (GL_FRAMEBUFFER, _framebuffer_id); }

		framebuffer ();
		~framebuffer ();

		void set_target (const texture & tex);

    };

}

#endif
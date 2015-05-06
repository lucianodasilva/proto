#pragma once

#ifndef _proto_framebuffer_h_
#define _proto_framebuffer_h_

#include <GL/glew.h>
#include "proto.texture.h"

namespace proto {

    class framebuffer {
    private:

		GLuint	_framebuffer_id;
		GLuint	_auxiliar_id;
		bool	_is_active;
		bool	_has_renderbuffer;
		bool	_has_no_backbuffer;

		framebuffer ();

    public:

		framebuffer (framebuffer && v);
		~framebuffer ();

		void bind () const;
		
		static framebuffer create (const texture & color_buffer);
		static framebuffer create (const texture & color_buffer, const texture & depth_buffer );
		static framebuffer create_depth_only (const texture & depth_buffer);

    };

}

#endif
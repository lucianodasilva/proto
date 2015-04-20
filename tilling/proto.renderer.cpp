//
// Created by Luciano on 15/04/2015.
//

#include "proto.renderer.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace proto {

	void renderer::set_framebuffer (framebuffer & f) {
		f.bind ();
	}

	void renderer::reset_framebuffer () {
		glBindFramebuffer (GL_FRAMEBUFFER, 0);
		glDrawBuffer (GL_BACK);
		glReadBuffer (GL_BACK);
	}

	void renderer::set_texture (texture & t) {
		t.bind ();
	}

	void renderer::set_mesh (mesh & m) {
		m.bind ();
	}

	void renderer::render_mesh (mesh & m) {
		glDrawElements (GL_TRIANGLES, m.index_count (), GL_UNSIGNED_SHORT, 0);
	}

	void renderer::set_program (program & p) {
		p.bind ();
	}

	void renderer::set_viewport (const ivec2 & top, const ivec2 & size) {
		glViewport (
			top.x,
			top.y,
			size.x,
			size.y
		);
	}

	void renderer::enable_depth (blend_function sfactor, blend_function dfactor) const {
		glEnable (GL_BLEND);
		glBlendFunc ((GLenum)sfactor, (GLenum)dfactor);
	}

	void renderer::disable_depth () const {
		glDisable (GL_BLEND);
	}

    void renderer::clear( const color & c, clear_mask mask ) {
		glClearColor (c.r, c.g, c.b, 1);
		glClear ((GLenum)mask);
    }

    void renderer::present() {
		glutSwapBuffers ();
    }

}
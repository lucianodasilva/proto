//
// Created by Luciano on 15/04/2015.
//

#include "proto.renderer.h"
#include "proto.debug.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace proto {

	void renderer::set_framebuffer (framebuffer & f) {
		gl_error_guard ("RENDERER SET FRAMEBUFFER");
		f.bind ();
	}

	void renderer::reset_framebuffer () {
		gl_error_guard ("RENDERER RESET FRAMEBUFFER");
		glBindFramebuffer (GL_FRAMEBUFFER, 0);
		glDrawBuffer (GL_BACK);
		glReadBuffer (GL_BACK);
	}

	void renderer::set_texture (texture & t) {
		gl_error_guard ("RENDERER SET TEXTURE");
		glActiveTexture (GL_TEXTURE0);
		t.bind ();
	}

	void renderer::set_mesh (mesh & m) {
		gl_error_guard ("RENDERER SET MESH");
		m.bind ();
	}

	void renderer::render_mesh (mesh & m) {
		gl_error_guard ("RENDERER RENDER MESH");
		glDrawElements (GL_TRIANGLES, m.index_count (), GL_UNSIGNED_SHORT, 0);
	}

	void renderer::set_program (program & p) {
		gl_error_guard ("RENDERER SET PROGRAM");
		p.bind ();
	}

	void renderer::set_viewport (int x, int y, int width, int height) {
		gl_error_guard ("RENDERER SET VIEWPORT");
		glViewport (
			x,
			y,
			width,
			height
		);
	}

	void renderer::enable_depth (blend_function sfactor, blend_function dfactor) const {
		gl_error_guard ("RENDERER ENABLE DEPTH");
		glEnable (GL_BLEND);
		glBlendFunc ((GLenum)sfactor, (GLenum)dfactor);
	}

	void renderer::disable_depth () const {
		gl_error_guard ("RENDERER DISABLE DEPTH");
		glDisable (GL_BLEND);
	}

    void renderer::clear( const color & c, clear_mask mask ) {
		gl_error_guard ("RENDERER CLEAR");
		glClearColor (c.r, c.g, c.b, 1);
		glClear ((GLenum)mask);
    }

    void renderer::present() {
		gl_error_guard ("RENDERER PRESENT");
		glutSwapBuffers ();
    }

}
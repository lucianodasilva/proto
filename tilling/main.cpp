
#include <GL/glew.h>

#include "proto.details.h"
#include "proto.debug.h"
#include "proto.mesh_builder.h"
#include "proto.renderer.h"
#include "proto.window.h"
#include "proto.window_manager.h"

//proto::renderer r;

proto::mesh quad;
proto::program prog;
proto::texture demo_tex;

proto::mat4
	model,
	view,
	projection;

void load_stuffs () {

	const char * vertex_source = proto_shader_source (
		//#version 330 core \n
																		
		uniform mat4 uni_t_model;										
		uniform mat4 uni_t_view;										
		uniform mat4 uni_t_proj;										
		uniform mat4 uni_t_normal;										
		uniform mat4 uni_t_mvp;											
																		
		layout (location = 1) in vec3 in_position;						
		layout (location = 2) in vec2 in_uv;							
		layout (location = 3) in vec3 in_normal;						
																		
		out vec2 var_uv;												
		out vec3 var_normal;											
																		
		void main () {													
			vec4 pos = uni_t_mvp * vec4 (in_position, 1.0);				
			var_normal = (uni_t_normal * vec4 (in_normal, 1.0)).xyz;	
																		
			var_uv = in_uv;												
																		
			gl_Position = pos;											
		}																
	);

	const char * pixel_source = proto_shader_source (
		//#version 330 core \n

		uniform sampler2D uni_texture;

		in vec3 var_normal;
		in vec2 var_uv;

		out vec4 out_color;

		void main () {
			vec4 texture_color = texture (uni_texture, var_uv);
			out_color = texture_color;
			out_color = vec4 (1.0F, .0F, .0F, 1.0F);
		}
	);

	auto vs = proto::shader::compile (proto::shader_type::vertex, vertex_source);
	auto ps = proto::shader::compile (proto::shader_type::pixel, pixel_source);

	prog = proto::program::link (vs, ps);

	//quad = proto::mesh::create_quad (1.0F, proto::vec3{ .0F, .0F, -1.F });

	proto::mesh_builder builder;

	uint16_t
		i1 = builder.add_element ({ {-.5F, -.5F, .0F}, {.0F, .0F}, { .0F, .0F, -1.F} }),
		i2 = builder.add_element ({ { .5F, -.5F, .0F },{ 1.F, .0F },{ .0F, .0F, -1.F } }),
		i3 = builder.add_element ({ { -.5F, .5F, .0F },{ .0F, 1.F },{ .0F, .0F, -1.F } }),
		i4 = builder.add_element ({ { .5F, .5F, .0F },{ 1.F, 1.F },{ .0F, .0F, -1.F } });

	builder.add_face ({ i1, i2, i3 });
	builder.add_face ({ i2, i4, i3 });

	quad = builder.make_mesh ();

	demo_tex = proto::texture::create_checkers (32, 32, 4, 4);

	model		= proto::mat4::identity;
	view		= proto::math::make_look_at (
		proto::vec3 { .0, .0, -1. }, 
		proto::vec3 { .0, 1., .0 }, 
		proto::vec3 { .0, .0, .0 }
	);

	projection  = proto::math::make_ortho (-1.0F, 1.0F, 1.0F, -1.0F, .0F, 100.0F);

	//r.set_viewport (0, 0, 512, 512);
}

void on_window_render ( proto::window & w, proto::renderer & r ) {
	r.clear ({ .0F, .2F, .3F }, proto::clear_mask::color | proto::clear_mask::depth);

	r.set_program (prog);
	r.set_texture (demo_tex);

	prog.set_value ("uni_t_model",	model);
	prog.set_value ("uni_t_view",	view);
	prog.set_value ("uni_t_proj",	projection);
	prog.set_value ("uni_t_normal", model.transpose ().invert ());
	prog.set_value ("uni_t_mvp",	model * view * projection);

	prog.set_value ("uni_texture",	0);

	r.set_mesh (quad);
	r.render_mesh (quad);

	r.present ();
}

void on_window_update (proto::window & sender) {

}

void on_window_close ( proto::window & sender ) {

}

void close_callback () {
	quad = proto::mesh ();
}

int main(int arg_c, char * arg_v[]) {

	auto w = proto::window::create ("Tilling Proto", { 512, 512 });

	w->on_window_render += on_window_render;
	w->on_window_update += on_window_update;
	w->on_window_close += on_window_close;

	w->make_active (); // this should not be called, needed for glew
	w->show ();

	// load graphics
	// glew init should be automatic somewhere within proto
	GLenum err = glewInit ();

	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf (stderr, "Error: %s\n", glewGetErrorString (err));
		return err;
	}

	glDisable	(GL_DEPTH_TEST);
	
	glDisable	(GL_CULL_FACE);
	glCullFace	(GL_BACK);

	load_stuffs ();

	proto::main_loop ();

    return 0;
}
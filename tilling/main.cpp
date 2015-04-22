
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "proto.details.h"
#include "proto.debug.h"
#include "proto.renderer.h"

proto::renderer r;

proto::mesh quad;
proto::program prog;
proto::texture demo_tex;

proto::mat4
	model,
	view,
	projection;

void load_stuffs () {

	const char * vertex_source = proto_shader_source (
		#version 330 core \n												
																		
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
		#version 330 core \n

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

	quad = proto::mesh::create_quad (1.0F, proto::vec3{ .0F, .0F, -1.F });

	demo_tex = proto::texture::create_checkers (32, 32, 4, 4);

	model		= proto::mat4::identity;
	view		= proto::mat4::make_look_at (
		proto::vec3 { .0, .0, -1. }, 
		proto::vec3 { .0, 1., .0 }, 
		proto::vec3 { .0, .0, .0 }
	);

	projection  = proto::mat4::make_ortho (-1.0F, 1.0F, 1.0F, -1.0F, .0F, 100.0F);

	r.set_viewport (0, 0, 100, 100);
}

void render_callback () {
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

void update_callback () {
	glutPostRedisplay ();
}

void close_callback () {
	quad = proto::mesh ();
}

int main(int arg_c, char * arg_v[]) {

	glutInit (&arg_c, arg_v);

    glutInitWindowPosition (-1, -1);
    glutInitWindowSize (600, 600);

    glutInitDisplayMode (GLUT_RGBA);

    glutCreateWindow ("Tilling Demo");
	glutDisplayFunc (&render_callback);
	glutIdleFunc (&update_callback);
	glutCloseFunc (&close_callback);

	// load graphics
	GLenum err = glewInit ();

	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf (stderr, "Error: %s\n", glewGetErrorString (err));
	}

	glDisable	(GL_DEPTH_TEST);
	
	glDisable	(GL_CULL_FACE);
	glCullFace	(GL_BACK);

	load_stuffs ();

    glutMainLoop();

    return 0;
}
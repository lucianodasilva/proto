
#include <GL/glew.h>

#include <proto.h>

proto::mesh quad;
proto::program prog;
proto::texture demo_tex;

proto::mat4
	model,
	view,
	projection;

void load_stuffs() {

	auto vs = proto::shader::compile(proto::shader_type::vertex, proto::get_file_content ("resources/basic_shader.vs"));
	auto ps = proto::shader::compile(proto::shader_type::pixel, proto::get_file_content("resources/basic_shader.ps"));

	prog = proto::program::link(vs, ps);

	quad = proto::mesh::create_quad (1.0F, proto::vec3{ .0F, .0F, -1.F });

	//proto::mesh_builder builder;

	//uint16_t
	//	i1 = builder.add_element({ { -.5F, .5F, .0F },{ .0F, .0F },{ .0F, .0F, -1.F } }),
	//	i2 = builder.add_element({ { .5F, .5F, .0F },{ 1.F, .0F },{ .0F, .0F, -1.F } }),
	//	i3 = builder.add_element({ { .5F, -.5F, .0F },{ .0F, 1.F },{ .0F, .0F, -1.F } }),
	//	i4 = builder.add_element({ { -.5F, -.5F, .0F },{ 1.F, 1.F },{ .0F, .0F, -1.F } });
	//
	//builder.add_face({ i1, i2, i3 });
	//builder.add_face({ i1, i3, i4 });
	//
	//quad = builder.make_mesh();

	demo_tex = proto::texture::create_checkers(32, 32, 4, 4);

	model = proto::mat4::identity;
	view = proto::math::make_look_at(
		proto::vec3{ .0, .0, -1. },
		proto::vec3{ .0, 1., .0 },
		proto::vec3{ .0, .0, .0 }
	);

	projection = proto::math::make_ortho(-1.0F, 1.0F, 1.0F, -1.0F, .0F, 100.0F);

	model = proto::mat4::identity;
	view = proto::mat4::identity;
	projection = proto::mat4::identity;

	//projection = proto::math::make_ortho(-1.0F, 1.0F, 1.0F, -1.0F, .0001F, 100.0F);
	//r.set_viewport (0, 0, 512, 512);
}

void on_window_render(proto::window & w, proto::renderer & r) {
	r.clear({ .0F, .2F, .3F }, proto::clear_mask::color | proto::clear_mask::depth);

	r.set_mesh(quad);
	r.set_program(prog);
	r.set_texture(demo_tex);

	auto mv = model * view;
	auto mvp = mv * projection;

	prog.set_value("uni_t_model", model);
	prog.set_value("uni_t_view", view);
	prog.set_value("uni_t_proj", projection);
	prog.set_value("uni_t_normal", mv.transpose().invert());
	prog.set_value("uni_t_mvp", mvp);

	prog.set_value("uni_texture", (int)demo_tex.id ());

	r.render_mesh(quad);

	r.present();
}

void on_window_update(proto::window & sender) {

}

void on_window_close(proto::window & sender) {

}

void close_callback() {
	//quad = proto::mesh();
}

int main(int arg_c, char * arg_v[]) {

	auto w = proto::window::create("Tilling Proto", { 512, 512 });

	w->on_window_render += on_window_render;
	w->on_window_update += on_window_update;
	w->on_window_close += on_window_close;

	w->make_active(); // this should not be called, needed for glew
	w->show();

	// load graphics
	// glew init should be automatic somewhere within proto
	glewExperimental = true;
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return err;
	} else {
		string gl_str_version = (const char *)glGetString(GL_VERSION);
		debug_print << "opengl version: " << gl_str_version;
	}

	{
		gl_error_guard("DEFAULT STARTUP FLAGS");
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	load_stuffs();

	proto::main_loop();

	return 0;
}
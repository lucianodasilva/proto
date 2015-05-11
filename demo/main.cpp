
#include <GL/glew.h>

#include <proto.h>

struct window_controller {

	proto::mesh quad;
	proto::program prog;
	proto::texture demo_tex;

	proto::texture color_buffer;
	proto::framebuffer mem_frame;

	proto::mat4
		model,
		view,
		projection;

	bool is_loaded = false;

	shared_ptr < proto::window > win_ref;

	void load_stuffs() {

		color_buffer = proto::texture::create(nullptr, 512, 512, proto::texture_format::rgba);

		mem_frame = proto::framebuffer::create(color_buffer);

		auto vs = proto::shader::compile(proto::shader_type::vertex, proto::get_file_content ("resources/basic_shader.vs"));
		auto ps = proto::shader::compile(proto::shader_type::pixel, proto::get_file_content("resources/basic_shader.ps"));

		prog = proto::program::link(vs, ps);

		quad = proto::mesh::create_quad (1.0F, proto::vec3{ .0F, .0F, -1.F });

		demo_tex = proto::texture::create_checkers( 32, 32, 4, 4 );

		projection = proto::math::make_ortho(-1.0F, 1.0F, 1.0F, -1.0F, .0F, 100.0F);

		model = proto::mat4::identity;
		view = proto::mat4::identity;
		projection = proto::mat4::identity;

	}

	void on_window_update(proto::window & w) {
		if (!is_loaded) {
			load_stuffs();
			is_loaded = true;
		}
	}

	void on_window_render(proto::window & w) {
		proto::renderer r;

		// swap framebuffer
		{
			r.set_framebuffer(mem_frame);
			r.clear({ .0F, 1.0F, .0F }, proto::clear_mask::color | proto::clear_mask::depth);
			r.set_mesh(quad);
			r.set_program(prog);
			r.set_texture(demo_tex);

			prog.set_value < int32_t >("uni_texture", 0);

			r.render_mesh(quad);
			r.present();
		}

		r.reset_framebuffer();
		r.clear({ .0F, .2F, .3F }, proto::clear_mask::color | proto::clear_mask::depth);

		r.set_mesh(quad);
		r.set_program(prog);
		r.set_texture(color_buffer);

		prog.set_value < int32_t > ("uni_texture", 0);

		r.render_mesh(quad);

		r.present();
	}

	window_controller(const shared_ptr < proto::window > & w) :
		win_ref(w) 
	{
		win_ref->on_window_render += [this](proto::window & w) { on_window_render(w); };
		win_ref->on_window_update += [this](proto::window & w) { on_window_update(w);};
	}

};

void close_callback() {
	//quad = proto::mesh();
}

int main(int arg_c, char * arg_v[]) {

	if (!proto::window_manager::initialize())
		return -1;

	auto w = proto::window::create("Tilling Proto", { 512, 512 });
	auto wc = window_controller(w);

	//w->make_active(); // this should not be called, needed for glew
	w->show();

	{
		gl_error_guard("DEFAULT STARTUP FLAGS");
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	proto::main_loop();

	return 0;
}
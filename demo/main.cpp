
#include <proto.h>
#include <proto.gl.h>

#include <thread>

//struct window_controller {

	//proto::gl::mesh quad;
	//proto::gl::program prog;
	//proto::gl::texture demo_tex;

	//proto::gl::texture color_buffer;
	//proto::gl::framebuffer mem_frame;

	//proto::mat4
	//	model,
	//	view,
	//	projection;

	//bool is_loaded = false;

	//shared_ptr < proto::gl::window > win_ref;

	//proto::expected < void > load_stuffs() {

	//	// define auto cleanup
	//	auto stuff_failed_cleanup = proto::scope_guard([&] {
	//		color_buffer = proto::gl::texture();
	//		mem_frame = proto::gl::framebuffer();
	//		prog = proto::gl::program();
	//		quad = proto::gl::mesh();
	//		demo_tex = proto::gl::texture();
	//	});

	//	color_buffer = proto::gl::texture::create(nullptr, 512, 512, proto::gl::texture_format::rgba);

	//	mem_frame = proto::gl::framebuffer::create(color_buffer);

	//	auto e_vs = proto::get_file_content("resources/basic_shared.vs");

	//	if (!e_vs)
	//		return e_vs.get_exception ();

	//	auto vs = proto::gl::shader::compile(proto::gl::shader_type::vertex, e_vs.get ());

	//	auto e_ps = proto::get_file_content("resources/basic_shader.ps");
	//	if (!e_ps)
	//		return e_ps.get_exception ();

	//	auto ps = proto::gl::shader::compile(proto::gl::shader_type::pixel, e_ps.get ());

	//	prog = proto::gl::program::link(vs, ps);

	//	quad = proto::gl::mesh::create_quad (1.0F, proto::vec3{ .0F, .0F, -1.F });

	//	demo_tex = proto::gl::texture::create_checkers( 32, 32, 4, 4 );

	//	projection = proto::math::make_ortho(-1.0F, 1.0F, 1.0F, -1.0F, .0F, 100.0F);

	//	model = proto::mat4::identity;
	//	view = proto::mat4::identity;
	//	projection = proto::mat4::identity;

	//	// All ok. Dismiss cleanup.
	//	stuff_failed_cleanup.dismiss();

	//	return{};
	//}

	//void on_window_update(proto::gl::window & w) {
	//}

	//void on_window_render(proto::gl::window & w) {
	//	if (!is_loaded) {
	//		load_stuffs();
	//		is_loaded = true;
	//	}

	//	proto::gl::renderer r;

	//	// swap framebuffer
	//	{
	//		r.set_framebuffer(mem_frame);
	//		r.clear({ .0F, 1.0F, .0F }, proto::gl::clear_mask::color | proto::gl::clear_mask::depth);
	//		r.set_mesh(quad);
	//		r.set_program(prog);
	//		r.set_texture(demo_tex);

	//		prog.set_value < int32_t >("uni_texture", 0);

	//		r.render_mesh(quad);
	//		r.present();
	//	}

	//	r.reset_framebuffer();
	//	r.clear({ .0F, .2F, .3F }, proto::gl::clear_mask::color | proto::gl::clear_mask::depth);

	//	r.set_mesh(quad);
	//	r.set_program(prog);
	//	r.set_texture(color_buffer);

	//	prog.set_value < int32_t > ("uni_texture", 0);

	//	r.render_mesh(quad);

	//	r.present();
	//}

	//window_controller(shared_ptr < proto::gl::window > win) :
	//	win_ref(win) 
	//{
	//	win_ref->on_window_render += [this](proto::gl::window & w) { on_window_render(w); };
	//	win_ref->on_window_update += [this](proto::gl::window & w) { on_window_update(w);};
	//}
//
//};

//void close_callback() {
//	//quad = proto::mesh();
//}

void error_callback(int error, const char * description) {
	std::cout << description << std::endl;
}

int main(int arg_c, char * arg_v[]) {

	//auto w1 = proto::gl::window::create("Tilling Proto 1", { 512, 512 });
	//auto wc1 = window_controller(w1);
	//
	//auto w2 = proto::gl::window::create("Tilling Proto 2", { 512, 512 });
	//auto wc2 = window_controller(w2);
	//
	//w1->show();
	//w2->show();
	//
	//{
	//	gl_error_guard("DEFAULT STARTUP FLAGS");
	//	glDisable(GL_DEPTH_TEST);
	//	glDisable(GL_CULL_FACE);
	//	glCullFace(GL_BACK);
	//}
	//
	//proto::run_scheduler().run([](proto::run_scheduler & s) {
	//	if (!proto::gl::window_manager::instance().handle_windows ()) {
	//		s.stop();
	//	}
	//});

	return 0;
}
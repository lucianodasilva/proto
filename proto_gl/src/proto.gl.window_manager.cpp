
#include "proto.gl.debug.h"
#include "proto.gl.window_manager.h"
#include "proto.gl.window.h"

using namespace std;

namespace proto {
	namespace gl {

		window_manager::window_manager() : singleton_base < window_manager >() {
			initialize();
		}

		window_manager::~window_manager() {}

		void window_manager::initialize() {

			//if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			//	debug_print << "failed to initialize SDL system";
			//	return;
			//}
			//
			//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
			//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			// make dummy window ( avoid deadlocking window instance )
			//_dummy_window = proto::gl::window::create_window_instance("", { 10, 10 });
			//register_window(_dummy_window);
			//
			//_dummy_window->make_current();

			glewExperimental = true;
			GLenum err = glewInit();

			if (GLEW_OK != err) {
				/* Problem: glewInit failed, something is seriously wrong. */
				debug_print << "failed to initialize glew with error: " << glewGetErrorString(err);
				return;
			}
			else {
				string gl_str_version = (const char *)glGetString(GL_VERSION);
				debug_print << "opengl version: " << gl_str_version;
			}

			// should perhaps setup VSync with the following code
			// SDL_GL_SetSwapInterval(1)
		}

		bool window_manager::handle_windows() {
			//SDL_Event e;
			//
			//// empty event pool
			//while (SDL_PollEvent(&e)) {
			//	for (auto & w : _windows) {
			//		w->handle_event(&e);
			//	}
			//}

			//for (
			//	auto w_it = _windows.begin();
			//	w_it != _windows.end();
			//	++w_it
			//	) {
			//
			//	auto & w_ptr = *w_it;
			//
			//	if (w_ptr->is_closed()) {
			//		_window_death_row.push_back(w_it);
			//		continue;
			//	}
			//
			//	w_ptr->on_window_update.sync_invoke(*w_ptr.get());
			//
			//	if (w_ptr->is_visible()) {
			//		w_ptr->make_current();
			//		w_ptr->on_window_render.sync_invoke(*w_ptr.get());
			//
			//		w_ptr->swap_context();
			//	}
			//}
			//
			//{
			//	for (auto & win_it : _window_death_row) {
			//		_windows.erase(win_it);
			//	}
			//
			//	_window_death_row.clear();
			//
			//	lock_guard < mutex > new_lock(_new_mutex);
			//
			//	for (auto & w_it : _new) {
			//		_windows.push_back(w_it);
			//	}
			//
			//	_new.clear();
			//}
			//
			//return _windows.size() > 1 /* cause of the dummy window */;
			return true;
		}

		void window_manager::register_window(shared_ptr < window > w) {
			//lock_guard < mutex > lock(_new_mutex);
			//_new.emplace_back(w);
		}

	}
}
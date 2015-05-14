
#include "proto.window_manager.h"
#include "proto.window.h"
#include "proto.debug.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glew.h>

using namespace std;

namespace proto {

	window_manager::~window_manager() {}

	bool window_manager::initialize() {
		window_manager & manager = instance();

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			debug_print << "failed to initialize SDL system";
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		
		// make dummy window
		manager._dummy_window = proto::window::create("", { 10, 10 });
		manager._dummy_window->make_current();

		glewExperimental = true;
		GLenum err = glewInit();

		if (GLEW_OK != err) {
			/* Problem: glewInit failed, something is seriously wrong. */
			debug_print << "failed to initialize glew with error: " << glewGetErrorString(err);
			return false;
		}
		else {
			string gl_str_version = (const char *)glGetString(GL_VERSION);
			debug_print << "opengl version: " << gl_str_version;
		}

		// should perhaps setup VSync with the following code
		// SDL_GL_SetSwapInterval(1)

		return true;
	}

	void window_manager::main_loop () {

		window_manager & manager = instance ();

		vector < vector < shared_ptr < window > >::iterator >
			death_row;

		do {

			SDL_Event e;

			// empty event pool
			while (SDL_PollEvent(&e)) {
				for (auto & w : manager._windows) {
					w->handle_event(&e);
				}
			}

			for (
				auto w_it = manager._windows.begin ();
				w_it != manager._windows.end ();
				++w_it
			) {

				auto & w_ptr = *w_it;

				if (w_ptr->is_closed ()) {
					death_row.push_back (w_it);
					continue;
				}

				w_ptr->on_window_update.sync_invoke (*w_ptr.get ());

				if (w_ptr->is_visible ()) {
					w_ptr->make_current ();
					w_ptr->on_window_render.sync_invoke (*w_ptr.get ());

					w_ptr->swap_context();
				}
			}

			{
				for (auto & win_it : death_row) {
					manager._windows.erase (win_it);
				}

				death_row.clear ();

				lock_guard < mutex > new_lock (manager._new_mutex);

				for (auto & w_it : manager._new) {
					manager._windows.push_back (w_it);
				}

				manager._new.clear ();
			}
		} while (manager._windows.size () > 1 /* cause of the dummy window */);
	}

	void window_manager::register_window (const shared_ptr < window > & w) {
		lock_guard < mutex > lock (_new_mutex);
		_new.emplace_back (w);
	}

}

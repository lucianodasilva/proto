#pragma once

#ifndef	_proto_gl_window_h_
#define _proto_gl_window_h_

#include <memory>

#include "proto.gl.window_events.h"

namespace proto {
	namespace gl {

		class window {
		public:

			window();

			mouse_down_event	on_mouse_down;
			mouse_up_event		on_mouse_up;
			mouse_move_event	on_mouse_move;

			key_down_event		on_key_down;
			key_up_event		on_key_up;

			window_render_event	on_window_render;
			window_update_event	on_window_update;

			window_load_event	on_window_load;

			window_show_event	on_window_show;
			window_hide_event	on_window_hide;

			window_close_event	on_window_close;

			window_resize_event on_window_resize;

			~window();

			void show();
			void hide();

			void close();

			bool is_visible() const;
			bool is_closed() const;

			point size() const;
			void size(const point & p);

			void make_current() const;

		private:

			std::unique_ptr < class window_imp > _implement;

			friend class window_manager;

			void * context() const;
			void swap_context() const;

			using external_event = void *;
			void handle_event(external_event e);

		};
	}
}

#endif
#pragma once

#ifndef	_proto_gl_window_h_
#define _proto_gl_window_h_

#include "proto.gl.window_events.h"

#include <memory>

namespace proto {
	namespace gl {

		class window {
		public:

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

			virtual ~window() = default;

			virtual void show() = 0;
			virtual void hide() = 0;

			virtual void close() = 0;

			virtual bool is_visible() const = 0;
			virtual bool is_closed() const = 0;

			virtual point size() const = 0;
			virtual void size(const point & p) = 0;

		};
	}
}

#endif
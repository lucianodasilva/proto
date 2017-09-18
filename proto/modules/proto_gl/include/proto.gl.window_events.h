#pragma once
#ifndef _proto_gl_window_event_h_
#define _proto_gl_window_event_h_

#include "proto.h"
#include "proto.gl.details.h"

namespace proto {
	namespace gl {

		template < class ... _argv_tv >
		using window_event = event < class window, _argv_tv ... >;

		enum class mouse_buttons {
			none,
			left,
			middle,
			right
		};

		struct mouse_event_args {
			point					location;
			mask < mouse_buttons >	button;
		};

		struct key_event_args {

		};

		using mouse_event = window_event < mouse_event_args >;

		using mouse_down_event = mouse_event;
		using mouse_up_event = mouse_event;
		using mouse_move_event = mouse_event;

		using key_down_event = window_event < key_event_args >;
		using key_up_event = window_event < key_event_args >;

		using window_load_event = window_event <>;
		using window_close_event = window_event <>;
		using window_show_event = window_event <>;
		using window_hide_event = window_event <>;

		using window_render_event = window_event <>;
		using window_update_event = window_event <>;

		using window_resize_event = window_event < point >;
	}
}

#endif
#pragma once

#ifndef	_proto_window_h_
#define _proto_window_h_

#include <memory>

#include "proto.details.h"
#include "proto.math.h"

namespace proto {

	class window;

	template < class ... _argv_tv >
	using window_event = event < window, _argv_tv ... >;

	enum class mouse_buttons {
		left,
		middle,
		right
	};

	struct mouse_down_event_args {
		uint32_t x, y;
		mouse_buttons button;
	};

	using mouse_down_event = window_event < mouse_down_event_args >;

	struct mouse_up_event_args {
		uint32_t x, y;
		mouse_buttons button;
	};

	using mouse_up_event = window_event < mouse_up_event_args >;

	struct mouse_move_event_args {
		uint32_t x, y;
	};

	using mouse_move_event = window_event < mouse_move_event_args >;
	using window_close_event = window_event < void >;

	class window {
	private:

		std::shared_ptr < class window_imp > _implement;

	public:

		mouse_down_event	on_mouse_down;
		mouse_up_event		on_mouse_up;
		mouse_move_event	on_mouse_move;
		window_close_event	on_window_close;

		void swap (window & w);

		window ();
		window (window && w);

		~window ();

		window & operator = (window && w);

		void show () const;
		void hide () const;

		point size () const;
		void size (const point & p);

		void do_event_loop ();

		static window create (const char * title, const point & size_v);

	};

}

#endif
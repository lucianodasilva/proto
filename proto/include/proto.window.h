#pragma once

#ifndef	_proto_window_h_
#define _proto_window_h_

#include <memory>

#include "proto.details.h"
#include "proto.event.h"
#include "proto.math.h"
#include "proto.renderer.h"

using namespace std;

namespace proto {

	class window;
	class window_manager;

	template < class ... _argv_tv >
	using window_event = event < window, _argv_tv ... >;

	enum class mouse_buttons {
		none,
		left,
		middle,
		right
	};

	inline mouse_buttons operator | (mouse_buttons m1, mouse_buttons m2) {
		return (mouse_buttons)((size_t)m1 | (size_t)m2);
	}

	inline bool operator & (mouse_buttons m1, mouse_buttons m2) {
		return ((size_t)m1 & (size_t)m2) != 0;
	}

	struct mouse_event_args {
		point location;
		mouse_buttons button;
	};

	using mouse_down_event = window_event < mouse_event_args >;
	using mouse_up_event = window_event < mouse_event_args >;
	using mouse_move_event = window_event < mouse_event_args >;

	using window_load_event = window_event < void >;
	using window_close_event = window_event < void >;
	using window_show_event = window_event < void >;
	using window_hide_event = window_event < void >;

	using window_render_event = window_event < void >;
	using window_update_event = window_event < void >;

	using window_resize_event = window_event < point >;

	class window {
	private:

		unique_ptr < class window_imp > _implement;
		renderer						_renderer;
		
		friend class window_manager;

		void * context() const;
		void swap_context () const;

		using external_event = void *;
		void handle_event(external_event e);

		static shared_ptr < window > create_window_instance(const char * title, const point & size_v);

	public:
		
		window ();

		mouse_down_event	on_mouse_down;
		mouse_up_event		on_mouse_up;
		mouse_move_event	on_mouse_move;

		window_render_event	on_window_render;
		window_update_event	on_window_update;

		window_load_event	on_window_load;

		window_show_event	on_window_show;
		window_hide_event	on_window_hide;

		window_close_event	on_window_close;

		window_resize_event on_window_resize;

		~window ();

		void show ();
		void hide ();

		void close ();

		bool is_visible () const;
		bool is_closed () const;

		point size () const;
		void size (const point & p);

		void make_current () const;

		static shared_ptr < window > create (const char * title, const point & size_v);

	};

}

#endif
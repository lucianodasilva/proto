#pragma once

#ifndef	_proto_window_h_
#define _proto_window_h_

#include <memory>

#include "proto.math.h"

namespace proto {

	class window_controller {
	public:
		virtual bool update () = 0;

	};

	class window {
	private:

		std::shared_ptr < class window_imp > _implement;

	public:

		void swap (window & w);

		window ();
		window (window && w);

		~window ();

		window & operator = (window && w);

		void show () const;
		void hide () const;

		point size () const;
		void size (const point & p);

		void do_event_loop (window_controller * controller);

		static window create (const char * title, const point & size_v);

	};

}

#endif
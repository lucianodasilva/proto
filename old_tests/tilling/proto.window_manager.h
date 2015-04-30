#pragma once

#ifndef	_proto_window_manager_h_
#define _proto_window_manager_h_

#include <memory>
#include <mutex>
#include <vector>

#include "proto.details.h"
#include "proto.math.h"


using namespace std;

namespace proto {

	class window;

	class window_manager {
	private:

		vector < shared_ptr < window > >	_windows;

		mutex								_new_mutex;
		vector < shared_ptr < window > >	_new;

		window_manager ();

	public:

		window_manager (const window_manager &) = delete;
		window_manager & operator = (const window_manager &) = delete;

		static window_manager & instance ();
		static void main_loop ();

		void register_window (const shared_ptr < window > & w);

	};

	inline void main_loop () {
		window_manager::instance ().main_loop ();
	}

}

#endif
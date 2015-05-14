#pragma once

#ifndef	_proto_window_manager_h_
#define _proto_window_manager_h_

#include <memory>
#include <mutex>
#include <vector>

#include "proto.details.h"
#include "proto.singleton.h"
#include "proto.math.h"


using namespace std;

namespace proto {

	class window;

	class window_manager : public singleton_base < window_manager > {
	private:

		shared_ptr < window >				_dummy_window;

		vector < shared_ptr < window > >	_windows;

		mutex								_new_mutex;
		vector < shared_ptr < window > >	_new;

		void register_window(const shared_ptr < window > & w);

		friend class window;

	public:

		~window_manager();

		static bool initialize();
		static void main_loop ();

	};

	inline void main_loop () {
		window_manager::instance ().main_loop ();
	}

}

#endif
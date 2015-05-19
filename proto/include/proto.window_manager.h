#pragma once

#ifndef	_proto_window_manager_h_
#define _proto_window_manager_h_

#include <memory>
#include <mutex>
#include <vector>

#include "proto.details.h"
#include "proto.singleton_base.h"
#include "proto.math.h"


using namespace std;

namespace proto {

	class window;

	class window_manager : public singleton_base < window_manager > {
	private:

		shared_ptr < window >				_dummy_window;

		vector < shared_ptr < window > >	_windows;
		vector < vector < shared_ptr < window > >::iterator >
											_window_death_row;


		mutex								_new_mutex;
		vector < shared_ptr < window > >	_new;

		void register_window(const shared_ptr < window > & w);

		friend class window;

	public:

		~window_manager();

		static bool initialize();

		bool handle_windows ();

	};


}

#endif
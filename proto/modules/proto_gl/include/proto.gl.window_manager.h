#pragma once

#ifndef	_proto_gl_window_manager_h_
#define _proto_gl_window_manager_h_

#include <memory>
#include <mutex>
#include <vector>

#include <proto.h>

using namespace std;

namespace proto {
	namespace gl {

		class window;

		class window_manager : public singleton_base < window_manager > {
		private:

			shared_ptr < window >				_dummy_window;

			vector < shared_ptr < window > >	_windows;
			vector < vector < shared_ptr < window > >::iterator >
				_window_death_row;

			mutex								_new_mutex;
			vector < shared_ptr < window > >	_new;

			void register_window(shared_ptr < window > w);

			friend class window;

		protected:

			friend class singleton_base < window_manager >;
			window_manager();

		public:

			~window_manager();

			void initialize();

			bool handle_windows();

		};

	}
}

#endif
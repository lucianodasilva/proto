#pragma once
#ifdef PROTO_API_WIN32

#ifndef	_proto_gl_application_win32_h_
#define _proto_gl_application_win32_h_

#include "proto.gl.application.h"

#include <memory>
#include <thread>
#include <vector>


namespace proto {
	namespace gl {

		class application_base_win32 : public gl::application_base {
		public:

			scheduler_base & render_scheduler();

			virtual expected < window * > create_window(std::string const & title, point const & size_v) override;

		protected:

			virtual expected < void > initialize() override;
			virtual void tick() override;

		private:

			std::vector < std::unique_ptr < class window_win32 > > 
						_windows;

			spin_mutex		_window_vector_mutex;

			sync_scheduler	_render_scheduler;
		

		};

	}
}

#endif
#endif
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

		class application_win32 : 
			public proto::gl::application_base
		{
		public:

			virtual ~application_win32();

			expected < window * > create_window(std::string const & title, point const & size_v);

			dispatcher_base * dispatcher() override;
			std::atomic < bool > const & is_running() const override;

			expected < void > run() override;
			void exit() override;

			dispatcher_base * render_dispatcher() override;

		protected:
		
			virtual expected < void > initialize() override;
		
		private:

			std::atomic < bool >	_running;
		
			std::vector < std::unique_ptr < class window_win32 > > 
									_windows;
			spin_mutex				_window_vector_mutex;

			proto::dispatcher <>	_logic_dispatcher;
			std::thread				_logic_thread;

			proto::dispatcher <>	_render_dispatcher;
		};

	}
}

#endif
#endif
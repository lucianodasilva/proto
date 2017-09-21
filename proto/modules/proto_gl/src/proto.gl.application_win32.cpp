#ifdef PROTO_API_WIN32

#include "proto.gl.application_win32.h"
#include "proto.gl.window_win32.h"

namespace proto {
	namespace gl {

		application_win32::~application_win32() {
			exit();
		}

		expected < window * > application_win32::create_window(std::string const & title, point const & size_v) {
			return nullptr;
		}

		dispatcher_base * application_win32::dispatcher() {
			return &_logic_dispatcher;
		}

		std::atomic < bool > const & application_win32::is_running() const {
			return _running;
		}

		expected < void > application_win32::run() {
			_running = true;

			auto_guard([this] {
				_running = false;
			});

			auto init_result = this->initialize();

			// if service initialization failed
			// return failure reason
			if (!init_result)
				return init_result;

			// loop as long as _running is true
			for (; _running;) {
				// update window/rendering generic tasks
				for (auto & w : _windows) {
					w->handle_events();
				}

				_render_dispatcher.consume_tasks();
				// update renderer
				
				// invoke render event
				for (auto & w : _windows)
					w->on_window_render.invoke(*w);
			}

			return {};
		}

		void application_win32::exit() {
			_running = false;

			if (_logic_thread.joinable())
				_logic_thread.join();
		}

		dispatcher_base * application_win32::render_dispatcher() {
			return &_render_dispatcher;
		}

		expected < void > application_win32::initialize() {
			// create a separate thread for the logic loop
			_logic_thread = std::thread (
				[this] {
					for (;this->_running;) {
						// consume logic tasks
						this->_logic_dispatcher.consume_tasks();
						// update logic callback
						this->update();
					}
				}
			);

			return {};
		}

	}
}

#endif
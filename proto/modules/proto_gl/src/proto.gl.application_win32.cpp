#ifdef PROTO_API_WIN32

#include "proto.gl.application_win32.h"
#include "proto.gl.window_win32.h"

namespace proto {
	namespace gl {

		application_win32::~application_win32() {
			exit();
		}

		expected < window * > application_win32::create_window(std::string const & title, point const & size_v) {
			return {};
		}

		dispatcher_base & application_win32::dispatcher() {
			return _logic_dispatcher;
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

			// loop as long as _is_running is true
			for (; _running;) {
				// update renderer
			}

			return {};
		}

		void application_win32::exit() {
			_running = false;

			if (_logic_thread.joinable())
				_logic_thread.join();
		}

		expected < void > application_win32::initialize() {
			// create a separate thread for the logic loop
			_logic_thread = std::thread (
				[this] {
					for (; this->is_running();) {
						for (auto & w : _windows) {
							w->handle_events();
						}

						this->update();
					}
				}
			);

			return {};
		}

	}
}

#endif
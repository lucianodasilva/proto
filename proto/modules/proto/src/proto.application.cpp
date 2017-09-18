#include "proto.application.h"
#include "proto.dispatcher.h"

namespace proto {

	expected < void > application_base::initialize() {
		return {};
	}

	application::application()
		: _running(false)
	{}

	application::~application() {
		this->exit();
	}

	dispatcher_base & application::dispatcher() {
		return _dispatcher;
	}

	std::atomic < bool > const & application::is_running() const {
		return _running;
	}

	expected < void > application::run() {
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
		for (;_running;) {
			_dispatcher.consume_tasks();
			this->update();
		}

		return {};
	}

	void application::exit() {
		_running = false;
	}

	namespace details {
		callback_application::callback_application(application_update_callback_t && callback)
			: _callback(callback) {}

		void callback_application::update() {
			if (_callback)
				_callback(*this);
		}
	}
}
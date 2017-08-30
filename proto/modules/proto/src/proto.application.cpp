#include "proto.application.h"

namespace proto {

	scheduler_base & application_base::scheduler() {
		return _scheduler;
	}

	service_manager & application_base::services() {
		return _services;
	}

	expected < void > application_base::run() {
		_is_running = true;

		auto init_result = _services.initialize();

		// if service initialization failed
		// return failure reason
		if (!init_result)
			return init_result;

		// loop as long as _is_running is true
		for (;_is_running;) {

			_scheduler.consume_tasks();
			_services.update();

			this->update();
		}

		return {};
	}

	void application_base::exit() {
		_is_running = false;
	}

}
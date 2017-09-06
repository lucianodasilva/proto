#include "proto.application.h"

namespace proto {

	scheduler_base & application_base::scheduler() {
		return _scheduler;
	}

	expected < void > application_base::run() {
		_is_running = true;

		auto init_result = this->initialize();

		// if service initialization failed
		// return failure reason
		if (!init_result)
			return init_result;

		// loop as long as _is_running is true
		for (;_is_running;) {
			_scheduler.consume_tasks();

			this->update();
		}

		return {};
	}

	expected < void > application_base::initialize(){
		return {};
	}

	void application_base::tick() {
		_scheduler.consume_tasks(); 

		this->update();
	}

	void application_base::exit() {
		_is_running = false;
	}

	application::application(std::function < void(application_base &) > const & update_callback)
		: _update_callback(update_callback) {}

	void application::update() {
		if (_update_callback)
			_update_callback(*this);
	}

}
#include "proto.application.h"

namespace proto {

	void application_base::run() {
		_is_running = true;

		for (;_is_running;) {
			_scheduler.consume_tasks();
			this->update();
		}
	}

	void application_base::exit() {
		_is_running = false;
	}

}
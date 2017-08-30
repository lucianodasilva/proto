#pragma once
#ifndef	_proto_application_h_
#define _proto_application_h_

#include <memory>

#include "proto.service.h"
#include "proto.sync_scheduler.h"

namespace proto {

	class application_base : public non_copyable  {
	public:

		scheduler_base & scheduler();
		service_manager & services();

		expected < void > run();
		void exit();

		virtual void update() = 0;

	private:

		std::atomic < bool >	_is_running = false;

		sync_scheduler			_scheduler;
		service_manager			_services;

	};

	class application : public application_base {
	public:

		application(std::function < void(application_base &) > const & update_callback)
			: _update_callback(update_callback) {}

		virtual inline void update() override {
			if (_update_callback)
				_update_callback(*this);
		}

	private:

		std::function <void(application_base &) > _update_callback;

	};

	inline void run_application(std::function < void(application_base &) > const & update_callback) {
		application app(update_callback);
		app.run();
	}

}

#endif

#pragma once
#ifndef	_proto_application_h_
#define _proto_application_h_

#include <memory>

#include "proto.sync_scheduler.h"

namespace proto {

	class application_base : public non_copyable  {
	public:

		scheduler_base & scheduler();

		expected < void > run();
		void exit();

		virtual void update() = 0;

	protected:

		virtual expected < void > initialize();
		virtual void tick();

	private:

		std::atomic < bool >	_is_running = false;

		sync_scheduler			_scheduler;

	};

	class application : public application_base {
	public:

		application(std::function < void(application_base &) > const & update_callback);

		virtual void update() override;

	private:

		std::function <void(application_base &) > _update_callback;

	};

	inline void run_application(std::function < void(application_base &) > const & update_callback) {
		application app(update_callback);
		app.run();
	}

}

#endif

#pragma once

#ifndef	_proto_run_scheduler_h_
#define _proto_run_scheduler_h_

#include "proto.scheduler.h"

namespace proto {

	class run_scheduler : public scheduler_base {
	public:

		virtual bool contains_thread(const std::thread::id & id) const;

		run_scheduler();
		virtual ~run_scheduler();

		void run(std::function < void(run_scheduler &) > const & callback );

		void stop();
	protected:

		void join();

	private:
		std::thread::id				_thread_id;

		void consume_tasks();
	};

}

#endif
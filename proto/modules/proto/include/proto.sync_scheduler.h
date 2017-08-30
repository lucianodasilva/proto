#pragma once

#ifndef	_proto_sync_scheduler_h_
#define _proto_sync_scheduler_h_

#include "proto.scheduler.h"

namespace proto {

	class sync_scheduler : public scheduler_base {
	public:

		virtual bool has_thread(const std::thread::id & id) const;

		sync_scheduler();
		virtual ~sync_scheduler();

		void run();
		void stop();

		void consume_tasks();

		virtual void join() override;

	private:
		std::thread::id	_thread_id;
	};

}

#endif
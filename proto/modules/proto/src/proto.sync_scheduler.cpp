#include "proto.sync_scheduler.h"

#include <memory>

namespace proto {

	bool sync_scheduler::has_thread(const std::thread::id & id) const {
		return id == _thread_id;
	}

	sync_scheduler::sync_scheduler() 
		: scheduler_base()
	{
		_scheduler_running = false;
	}

	sync_scheduler::~sync_scheduler() {
		if (_scheduler_running)
			join();
	}

	void sync_scheduler::run() {
		_thread_id = std::this_thread::get_id();
		_scheduler_running = true;

		for (;_scheduler_running;) {
			consume_tasks();
		}

	}

	void sync_scheduler::stop() {
		join();
	}

	void sync_scheduler::join() {
		scheduler_base::_scheduler_running = false;
		consume_tasks();
	}

	void sync_scheduler::consume_tasks() {

		// consume available tasks
		for (;;) {
			scheduler_task task = {};

			{
				std::lock_guard < std::mutex > lock(scheduler_base::_task_mutex);

				if (!scheduler_base::_scheduler_running && scheduler_base::_tasks.empty())
					return;

				if (!scheduler_base::_tasks.empty()) {
					task = move(scheduler_base::_tasks.front());
					scheduler_base::_tasks.pop();
				}
			}

			if (!task)
				break;

			task();
		}
	}

}
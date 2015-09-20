#include "proto.run_scheduler.h"

#include <memory>

namespace proto {

	bool run_scheduler::contains_thread(const thread::id & id) const {
		return id == _thread_id;
	}

	run_scheduler::run_scheduler() 
		: scheduler_base() 
	{}

	void run_scheduler::run( const function < void ( run_scheduler & ) > & callback ) {
		_thread_id = this_thread::get_id();
		_is_running = true;

		for (;;) {

			scheduler_task task;

			// consume available tasks
			for (;;) {
				{
					lock_guard < mutex > lock(scheduler_base::_task_mutex);

					if (!scheduler_base::_is_running && scheduler_base::_tasks.empty())
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

			callback(*this);
		}

	}

	void run_scheduler::stop() {
		scheduler_base::_is_running = false;
	}

}
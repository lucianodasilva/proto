#include "proto.run_scheduler.h"

#include <memory>

namespace proto {

	bool run_scheduler::contains_thread(const std::thread::id & id) const {
		return id == _thread_id;
	}

	run_scheduler::run_scheduler() 
		: scheduler_base()
	{
		_scheduler_running = false;
	}

	run_scheduler::~run_scheduler() {
		if (_scheduler_running)
			join();
	}

	void run_scheduler::run( std::function < void ( run_scheduler & ) > const & callback ) {
		_thread_id = std::this_thread::get_id();
		_scheduler_running = true;

		for (;_scheduler_running;) {
			consume_tasks();
			callback(*this);
		}

	}

	void run_scheduler::stop() {
		join();
	}

	void run_scheduler::join() {
		scheduler_base::_scheduler_running = false;
		consume_tasks();
	}

	void run_scheduler::consume_tasks() {

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
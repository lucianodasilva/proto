#include "proto.event_scheduler.h"

#include <memory>

namespace proto {

	event_scheduler::event_scheduler() 
		: scheduler_base() 
	{
		_thread = thread(
			[this]
		{
			for (;;) {
				scheduler_task task;

				{
					unique_lock < mutex > lock(scheduler_base::_task_mutex);
					scheduler_base::_condition.wait(
						lock,
						[this] { return !scheduler_base::_is_running || !scheduler_base::_tasks.empty();}
					);

					if (!scheduler_base::_is_running && scheduler_base::_tasks.empty())
						return;

					task = move(scheduler_base::_tasks.front());
					scheduler_base::_tasks.pop();
				}

				task();
			}
		});
	}

	bool event_scheduler::contains_thread(const thread::id & id) const {
		return _thread.get_id() == id;
	}

	event_scheduler::~event_scheduler() {
		scheduler_base::~scheduler_base();
		_thread.join();
	}

}
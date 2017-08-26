#include "proto.scheduler.h"

#include <memory>

namespace proto {

	void scheduler_base::notify_one() {}

	scheduler::~scheduler(){
		scheduler_base::_scheduler_running = false;
		
		_condition.notify_all();

		for (auto & worker : _workers)
			worker.join();
	}

	scheduler::scheduler(unsigned int thread_count) :
		scheduler_base()
	{

		for (
			unsigned int i = 0; i < thread_count; ++i
		) {
			_workers.emplace_back(
				[this]
			{
				for (;;) {
					scheduler_task task = {};

					{
						unique_lock < mutex > lock(scheduler_base::_task_mutex);
						_condition.wait(
							lock,
							[this] { return _scheduler_running && !scheduler_base::_tasks.empty();}
						);

						if (!_scheduler_running && scheduler_base::_tasks.empty())
							return;

						task = move(scheduler_base::_tasks.front());
						scheduler_base::_tasks.pop();
					}

					task();
				}
			});
		}
	}

	bool scheduler::contains_thread(const thread::id & id) const {
		for (auto & t : _workers)
			if (t.get_id() == id)
				return true;

		return false;
	}

	void scheduler::notify_one() {
		_condition.notify_one();
	}

}
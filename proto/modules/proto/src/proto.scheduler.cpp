#include "proto.scheduler.h"

#include <memory>

namespace proto {

	void scheduler_base::try_consume_task() {}

	scheduler::~scheduler(){
		join();
	}

	void scheduler::join() {
		if (scheduler_base::_scheduler_running) {
			scheduler_base::_scheduler_running = false;

			_condition.notify_all();

			for (auto & worker : _workers) {
				if (worker.joinable())
					worker.join();
			}
		}
	}

	void scheduler::run (unsigned int thread_count)
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
						std::unique_lock < std::mutex > lock(scheduler_base::_task_mutex);
						_condition.wait(
							lock,
							[this] { return _scheduler_running && !scheduler_base::_tasks.empty();}
						);

						if (!_scheduler_running && scheduler_base::_tasks.empty())
							return;

						task = std::move(scheduler_base::_tasks.front());
						scheduler_base::_tasks.pop();
					}

					task();
				}
			});
		}
	}

	bool scheduler::has_thread(const std::thread::id & id) const {
		for (auto & t : _workers)
			if (t.get_id() == id)
				return true;

		return false;
	}

	void scheduler::try_consume_task() {
		_condition.notify_one();
	}

}
#include "proto.scheduler.h"
#include "proto.window_manager.h"

#include <memory>

namespace proto {

	scheduler_base::scheduler_base()
	{
		_is_running = true;
	}

	scheduler_base::~scheduler_base() {
		_is_running = false;
		_condition.notify_all();
	}

	scheduler::~scheduler() {
		scheduler_base::_is_running = false;

		scheduler_base::_condition.notify_all();

		for (auto & worker : _workers)
			worker.join();
	}

	scheduler::scheduler() :
		singleton_base < scheduler >(),
		scheduler_base()
	{
		auto thread_count = std::thread::hardware_concurrency();

		for (
			decltype (thread_count) i = 0;
			i < thread_count;
			++i
			) {
			_workers.emplace_back(
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
	}

	bool scheduler::contains_thread(const thread::id & id) const {
		for (auto & t : _workers)
			if (t.get_id() == id)
				return true;

		return false;
	}

}
#include "proto.scheduler.h"
#include <memory>

namespace proto {

	scheduler::~scheduler() {
		_is_running = false;

		_condition.notify_all();

		for (auto & worker : _workers)
			worker.join();
	}

	void scheduler::initialize () {
		singleton_base::initialize();

		_is_running = true;

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
						task_t task;

						{
							unique_lock < mutex > lock(_task_mutex);
							_condition.wait(
								lock,
								[this] { return !_is_running || !_tasks.empty();}
							);

							if (!_is_running && _tasks.empty())
								return;

							task = move(_tasks.front());
							_tasks.pop();
						}

						task();
					}
				}
			);
		}
	}

}
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

	// event scheduler implementation
	scheduler::event_scheduler::event_scheduler() 
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

	scheduler::event_scheduler::~event_scheduler() {
		scheduler_base::~scheduler_base();
		_thread.join();
	}

	// main scheduler implementation
	scheduler::main_scheduler::main_scheduler()
		: scheduler_base()
	{}

	scheduler::main_scheduler::~main_scheduler() {
	}

	void scheduler::main_scheduler::run () {
		for (;;) {
			scheduler_task task;

			{
				unique_lock < mutex > lock(scheduler_base::_task_mutex);

				if (!scheduler_base::_is_running && scheduler_base::_tasks.empty())
					return;

				if (!scheduler_base::_tasks.empty()) {
					task = move(scheduler_base::_tasks.front());
					scheduler_base::_tasks.pop();
				}
			}

			if (task)
				task();

			if (_is_running && !window_manager::instance().handle_windows()) {
				_is_running = false;
			}
		}
	}
}
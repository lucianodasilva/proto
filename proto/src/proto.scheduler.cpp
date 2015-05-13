#include "proto.scheduler.h"

namespace proto {

	void thread_runner() {
		while (scheduler::is_running()) {
			scheduler::do_task();
		}
	}

	scheduler::~scheduler() {
		for (auto & t : _threads) {
			t.join();
		}
	}

	void scheduler::initialize () {
		singleton_base::initialize();

		_is_running = true;

		auto thread_count = std::thread::hardware_concurrency();

		_threads.reserve(thread_count);

		for (
			decltype (thread_count) i = 0; 
			i < thread_count; 
			++i
		) {
			_threads.push_back(thread(thread_runner));
		}

	}

	future < void > scheduler::add_task(function < void() > && f) {
		auto & s = instance();

		lock_guard < spin_mutex > work_list_lock(s._work_mutex);

		s._work_queue.push_back(task(f));
		return std::move(s._work_queue.back().get_future());
	}

	void scheduler::do_task() {

	}


}
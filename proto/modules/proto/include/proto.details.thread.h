#pragma once
#ifndef _proto_details_thread_h_
#define _proto_details_thread_h_

#include <atomic>
#include <mutex>

namespace proto {

	struct spin_mutex {
	private:
		std::atomic_flag _lockless_flag = ATOMIC_FLAG_INIT;
	public:
		inline void lock() {
			while (_lockless_flag.test_and_set(std::memory_order_acquire)) {}
		}

		inline void unlock() {
			_lockless_flag.clear(std::memory_order_release);
		}
	};

}

#endif
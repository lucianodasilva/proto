#ifndef	_proto_scheduler_h_
#define _proto_scheduler_h_

#include "proto.details.h"
#include "proto.singleton.h"

#include <vector>

#include <atomic>
#include <future>
#include <thread>

using namespace std;

namespace proto {

	enum struct task_type {
		rendering,
		main,
		work
	};

	class scheduler : public singleton_base < scheduler > {
	private:

		using task = std::packaged_task < void() >;

		vector < thread >	_threads;

		vector < task >		_work_queue;
		vector < task >		_active_work_list;

		spin_mutex			_work_mutex;

		atomic < bool >		_is_running;

	protected:

		virtual void initialize() override;

	public:

		virtual ~scheduler();

		static inline bool is_running() {
			return scheduler::instance()._is_running;
		}

		static future < void > add_task(function < void() > && f);

		static void do_task();

	};

}

#endif
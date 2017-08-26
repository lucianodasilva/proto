#ifndef	_proto_scheduler_h_
#define _proto_scheduler_h_

#include "proto.details.h"
#include "proto.id.h"
#include "proto.singleton_base.h"

#include <future>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

namespace proto {

	using scheduler_task = function < void() >;

	class scheduler_base : public non_copyable {
	public:

		virtual bool contains_thread (const thread::id & id) const = 0;

		virtual ~scheduler_base() = default;

		template < class _ft_t, class ... _args_t >
		inline auto enqueue(_ft_t && f, _args_t && ... args) 
			-> future < typename result_of < _ft_t(_args_t ...)>::type >
		{

			using ret_t = typename result_of < _ft_t(_args_t ...)>::type;

			auto task = make_shared < packaged_task < ret_t() > >(
				bind(forward < _ft_t >(f), forward < _args_t >(args) ...)
			);

			future < ret_t > res = task->get_future();
			{
				unique_lock < mutex > lock(_task_mutex);

				_tasks.emplace([task]() {
					(*task)();
				});
			}

			notify_one();
			return res;
		}

	protected:
		queue < scheduler_task >	_tasks;
		mutex						_task_mutex;
		atomic < bool >				_scheduler_running = true;

		virtual void notify_one();
	};

	class scheduler : public scheduler_base {
	public:

		scheduler(unsigned int thread_count = std::thread::hardware_concurrency());
		virtual ~scheduler();

		virtual bool contains_thread(const thread::id & id) const override;

	protected:

		virtual void notify_one() override;

	private:
		vector < thread >	_workers;
		condition_variable	_condition;
	};

}

#endif
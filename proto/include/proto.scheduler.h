#ifndef	_proto_scheduler_h_
#define _proto_scheduler_h_

#include "proto.details.h"
#include "proto.singleton.h"

#include <queue>
#include <vector>

#include <atomic>
#include <future>
#include <thread>

using namespace std;

namespace proto {

	class scheduler : public singleton_base < scheduler > {
	private:

		using task_t = std::function < void() >;

		vector < thread >	_workers;
		queue < task_t >	_tasks;

		mutex				_task_mutex;
		condition_variable	_condition;
		atomic < bool >		_is_running;

	protected:

		virtual void initialize() override;

	public:

		virtual ~scheduler();

		template < class _ft_t, class ... _args_t >
		static inline auto add_task(_ft_t && f, _args_t && ... args) 
			-> future < typename result_of < _ft_t (_args_t ...)>::type >
		{
			auto & s = instance();

			using ret_t = typename result_of < _ft_t(_args_t ...)>::type;

			auto task = make_shared < packaged_task < ret_t () > > (
				bind(forward < _ft_t >(f), forward < _args_t >(args) ...)
			);

			future < ret_t > res = task->get_future();
			{
				unique_lock < mutex > lock(s._task_mutex);

				if (!s._is_running)
					throw std::runtime_error("added tasks on stopped scheduler");

				s._tasks.emplace([task]() {
					(*task)();
				});
			}

			s._condition.notify_one();
			return res;
		}

	};

}

#endif
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

	class scheduler_base {
	protected:

		queue < scheduler_task >	_tasks;

		mutex						_task_mutex;
		condition_variable			_condition;
		atomic < bool >				_is_running;

		scheduler_base ();

	public:

		virtual ~scheduler_base();

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

				if (!_is_running)
					throw std::runtime_error("added tasks on stopped scheduler");

				_tasks.emplace([task]() {
					(*task)();
				});
			}

			_condition.notify_one();
			return res;
		}


	};

	class scheduler :
		public singleton_base < scheduler >,
		public scheduler_base
	{
	private:

		vector < thread >	_workers;

	protected:

		friend class singleton_base < scheduler >;
		scheduler();

	public:

		virtual ~scheduler();

		template < class _ft_t, class ... _args_t >
		static inline auto enqueue(_ft_t && f, _args_t && ... args)
			-> future < typename result_of < _ft_t(_args_t ...)>::type >
		{
			auto & s = instance();
			return static_cast <dispatcher_base &> (s).enqueue(f, args...);
		}

	};

}

#endif
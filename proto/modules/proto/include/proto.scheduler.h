#ifndef	_proto_scheduler_h_
#define _proto_scheduler_h_

#include "proto.details.h"
#include "proto.id.h"

#include <future>
#include <queue>
#include <thread>
#include <vector>

namespace proto {

	using scheduler_task = std::function < void() >;

	class scheduler_base : public non_copyable {
	public:

		virtual bool has_thread (const std::thread::id & id) const = 0;

		virtual ~scheduler_base() = default;

		virtual void join() = 0;

		template < class _ft_t, class ... _args_t >
		inline auto enqueue(_ft_t && f, _args_t && ... args) 
			-> std::future < std::result_of_t < _ft_t(_args_t ...) > >
		{

			using ret_t = std::result_of_t < _ft_t(_args_t ...) >;

			auto task = std::make_shared < std::packaged_task < ret_t() > >(
				std::bind(std::forward < _ft_t >(f), std::forward < _args_t >(args) ...)
			);

			std::future < ret_t > task_future = task->get_future();
			{
				std::unique_lock < std::mutex > lock(_task_mutex);

				_tasks.emplace([task]() {
					(*task)();
				});
			}

			try_consume_task();
			return task_future;
		}

	protected:
		std::queue < scheduler_task >	_tasks;
		std::mutex						_task_mutex;
		std::atomic < bool >			_scheduler_running;

		virtual void try_consume_task();
	};

	class scheduler : public scheduler_base {
	public:

		virtual bool has_thread(const std::thread::id & id) const override;

		void run (unsigned int thread_count = std::thread::hardware_concurrency());
		virtual ~scheduler();

		virtual void join() override;

	protected:

		virtual void try_consume_task() override;

	private:
		std::vector < std::thread >	_workers;
		std::condition_variable	_condition;
	};

	template < class _ft_t, class ... _args_t >
	inline auto scheduler_dispatch(
		scheduler_base & scheduler, 
		_ft_t && f, 
		_args_t && ... args
	){
		using ret_t = std::result_of_t < _ft_t(_args_t ...) >;

		if (scheduler.has_thread(std::this_thread::get_id())) {

			auto task = std::packaged_task < ret_t() >(
				std::bind(
					std::forward < _ft_t >(f), 
					std::forward < _args_t >(args) ...
				)
			);

			auto ret_future = task.get_future();

			task();

			return ret_future;
		} else {
			return scheduler.enqueue(f, args...);
		}
	}

}

#endif
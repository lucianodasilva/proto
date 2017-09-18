#pragma once
#ifndef _proto_dispatcher_h_
#define _proto_dispatcher_h_

#include "proto.debug.h"
#include "proto.details.h"
#include "proto.swap_locking.h"

#include <future>
#include <memory>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace proto {

	class dispatcher_base : public non_copyable {
	public:

		virtual bool has_thread(std::thread::id const & id) = 0;

		template < class _ft_t, class ... _args_t >
		inline auto enqueue(_ft_t && f, _args_t && ... args)
			-> std::future < std::result_of_t < _ft_t(_args_t ...) > >
		{
			auto task = make_task(
				std::forward < _ft_t >(f),
				std::forward < _args_t >(args)...
			);

			auto task_future = task.get_future();

			enqueue_task(details::make_proxy(std::move(task)));

			return task_future;
		}

		template < class _ft_t, class ... _args_t >
		inline auto run_or_enqueue(_ft_t && f, _args_t && ... args)
			-> std::future < std::result_of_t < _ft_t(_args_t ...) > >
		{
			auto task = make_task(
				std::forward < _ft_t >(f),
				std::forward < _args_t >(args)...
			);

			auto task_future = task.get_future();

			if (has_thread(std::this_thread::get_id()))
				task();
			else {
				enqueue_task(details::make_proxy(std::move(task)));
			}

			return task_future;
		}

	protected:

		virtual void enqueue_task(details::proxy_task && task) = 0;

	private:

		template < class _ft_t, class ... _args_t >
		inline static auto make_task(_ft_t && f, _args_t && ... args) {
			return std::packaged_task < typename std::result_of < _ft_t(_args_t...) >::type() >(
				std::bind(
					std::forward < _ft_t >(f),
					std::forward < _args_t >(args)...
				)
			);
		}

	};

	template < class _task_runner_t = details::sync_task_runner >
	class dispatcher :
		public dispatcher_base,
		public _task_runner_t
	{
	public:

		inline bool has_thread(std::thread::id const & id) override {
			return _task_runner_t::runner_has_thread(id);
		}

	protected:

		inline void enqueue_task(details::proxy_task && task) override {
			_task_runner_t::runner_enqueue_task(std::forward < details::proxy_task >(task));
		}

	};

}

#endif
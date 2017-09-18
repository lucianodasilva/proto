#pragma once
#ifndef _proto_details_dispatch_h_
#define _proto_details_dispatch_h_

#include "proto.swap_locking.h"

#include <future>
#include <memory>
#include <queue>
#include <vector>

namespace proto {
	namespace details {

		struct proxy_task {

			struct task_handle_base {
				virtual void invoke() = 0;
			};

			template < class _ft_t  >
			struct task_handle : public task_handle_base {

				inline task_handle(std::packaged_task < _ft_t > f)
					: task(std::move(f))
				{}

				std::packaged_task < _ft_t > task;

				virtual void invoke() override {
					task();
				}
			};

			std::unique_ptr < task_handle_base > handle;

			proxy_task() = default;

			proxy_task(proxy_task &&) = default;
			proxy_task & operator = (proxy_task &&) = default;

			proxy_task(std::unique_ptr < task_handle_base > handle)
				: handle(std::move(handle))
			{}

			inline void invoke() {
				if (handle)
					handle->invoke();
			}

		};

		template < class _ft_t >
		inline proxy_task make_proxy(std::packaged_task < _ft_t > && task) {
			auto task_handle = std::unique_ptr < proxy_task::task_handle_base >(
				new proxy_task::task_handle < _ft_t >(std::forward < decltype (task) >(task))
				);

			return proxy_task(std::move(task_handle));
		}

		class sync_task_runner {
		public:

			sync_task_runner();

			virtual ~sync_task_runner();

			void run();

			void stop();

			void consume_tasks();

		protected:

			bool runner_has_thread(std::thread::id const & id) const;

			void runner_enqueue_task(details::proxy_task && task);

		private:
			swap_locking < std::vector < details::proxy_task > >
				_tasks;

			std::atomic < bool >	_running;
			std::thread::id			_thread_id;
		};

		class async_task_runner {
		public:

			async_task_runner();

			virtual ~async_task_runner();

			void run(uint32_t worker_thread_count);

			void stop();

		protected:

			bool runner_has_thread(std::thread::id const & id) const;

			void runner_enqueue_task(details::proxy_task && task);

		private:

			std::vector < std::thread >			_workers;
			std::condition_variable				_consume_condition;

			std::queue < details::proxy_task >	_tasks;
			std::mutex							_task_mutex;
			std::atomic < bool >				_running;

		};

	}
}

#endif
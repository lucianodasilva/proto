#include "proto.details.dispatch.h"
#include "proto.debug.h"

namespace proto {
	namespace details {

		////////////////////////////////////
		// sync dispatcher implementation //
		////////////////////////////////////

		sync_task_runner::sync_task_runner() :
			_running(false)
		{}

		sync_task_runner::~sync_task_runner() {
			stop();
		}

		bool sync_task_runner::runner_has_thread(std::thread::id const & id) const {
			return id == _thread_id;
		}

		void sync_task_runner::run() {
			_thread_id = std::this_thread::get_id();

			_running = true;

			for (; _running;)
				consume_tasks();

			bool has_tasks = _tasks.lock()->empty();

			if (has_tasks)
				consume_tasks();
		}

		void sync_task_runner::stop() {
			_running = false;
		}

		void sync_task_runner::consume_tasks() {
			proto_thread_guard(_thread_id);

			auto & exec_list = _tasks.swap_active();

			for (auto & task : exec_list)
				task.invoke();

			exec_list.clear();
		}

		void sync_task_runner::runner_enqueue_task(details::proxy_task && task) {
			proto_debug_assert_break(_running);

			if (!_running)
				return;

			auto locked = _tasks.lock();
			locked->push_back(std::move(task));
		}

		/////////////////////////////////////
		// async dispatcher implementation //
		/////////////////////////////////////

		async_task_runner::async_task_runner() :
			_running(false)
		{}

		async_task_runner::~async_task_runner() {
			stop();
		}

		bool async_task_runner::runner_has_thread(std::thread::id const & id) const {
			bool found_thread = false;

			for (auto & worker : _workers) {
				found_thread |= (worker.get_id() == id);
			}

			return found_thread;
		}

		void async_task_runner::runner_enqueue_task(details::proxy_task && task) {
			proto_debug_assert_break(_running);

			if (!_running)
				return;

			std::lock_guard < std::mutex > lock(_task_mutex);
			_tasks.push(std::move(task));

			_consume_condition.notify_one();
		}

		void async_task_runner::run(uint32_t worker_thread_count) {
			_running = true;

			_workers.resize(worker_thread_count);

			for (size_t i = 0; i < worker_thread_count; ++i) {
				_workers[i] = std::thread([this] {
					for (;;) {

						details::proxy_task task;

						{
							std::unique_lock < std::mutex > lock(this->_task_mutex);

							this->_consume_condition.wait(
								lock,
								[this] { return this->_running && !this->_tasks.empty(); }
							);

							if (!this->_running && this->_tasks.empty())
								return;

							task = std::move(this->_tasks.front());
							this->_tasks.pop();
						}

						task.invoke();
					}
				});
			}
		}

		void async_task_runner::stop() {
			_running = false;

			for (auto & worker : _workers) {
				if (worker.joinable())
					worker.join();
			}
		}

	}
}
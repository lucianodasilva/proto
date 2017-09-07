
#include <proto.h>

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

#ifndef _DEBUG
#	define proto_dispatcher_guard(dispatcher)
#else
#	define proto_dispatcher_guard(dispatcher) \
	if (scheduler.has_thread (std::this_thread::get_id ())) { \
		proto_debug_break (); \
	}
#endif

#ifndef _DEBUG
#	define proto_thread_guard(thread_id)
#else
#	define proto_thread_guard(thread_id) \
	if (thread_id != std::this_thread::get_id ()) { \
		proto_debug_break (); \
	}
#endif

using namespace proto;

namespace proto {

	template < class _t, class _mutex_t = spin_mutex >
	class swap_locking {
	public:

		class lock_handle {
		public:

			inline lock_handle(_t & handle, _mutex_t & mutex) :
				_lock(mutex),
				_handle(&handle)
			{}

			lock_handle (lock_handle &&) = default; 
			lock_handle & operator = (lock_handle &&) = default;
			
			inline _t * operator -> () { return _handle; }
			inline _t const * operator -> () const { return _handle; }

		private:
			std::unique_lock < _mutex_t > 
					_lock;
			_t *	_handle = nullptr;
		};

		inline lock_handle lock() {
			return {_active, _mutex};
		}

		inline _t & swap_active() {
			{
				std::lock_guard < _mutex_t > lock(_mutex);
				std::swap(_active, _consumable);
			}
			return _consumable;
		}

	private:
		_t 
			_active,
			_consumable;

		_mutex_t
			_mutex;
	};

	namespace details {

		class proxy_task {
		public:

			proxy_task (proxy_task &&) = default;
			proxy_task & operator = (proxy_task &&) = default;

			template < class _ft_t >
			proxy_task(std::packaged_task < _ft_t > && task) :
				_task_handler(new task_handler < _ft_t >(std::move(task)))
			{}

			void invoke() {
				if (_task_handler)
					_task_handler->invoke();
			}

		private:

			struct task_handler_base {
				virtual void invoke() = 0;
			};
			
			template < class _ft_t  >
			struct task_handler : public task_handler_base {

				std::packaged_task < _ft_t > task;

				inline task_handler(std::packaged_task < _ft_t > && task) : 
					task(std::move(task))
				{}

				virtual void invoke() override {
					task();
				}
			};

			std::unique_ptr < task_handler_base > _task_handler;
		};
	}

	class dispatcher_base : private non_copyable {
	public:

		virtual ~dispatcher_base() = default;

		virtual bool has_thread(std::thread const & thread) const = 0;

		template < class _ft_t, class ... _args_t >
		inline auto enqueue(_ft_t && f, _args_t && ... args)
			-> std::future < std::result_of_t < _ft_t(_args_t ...) > >
		{
			auto task = make_task(
				std::forward(f),
				std::forward(args)...
			);

			auto task_future = task.get_future();

			enqueue_task(task);

			return task_future;
		}

		virtual void run() = 0;

		virtual void join() = 0;

	protected:
		virtual void enqueue_task(details::proxy_task && task) = 0;
	private:

		template < class _ft_t, class ... _args_t >
		inline static auto make_task (_ft_t && f, _args_t && ... args)
			-> std::packaged_task < std::result_of_t < _ft_t(_args_t ...) > () >
		{
			return {
				std::bind(
					std::forward(f),
					std::forward(args)...
				)
			};
		}

	};

	class dispatcher : public dispatcher_base {
	public:

		dispatcher () :
			_running (false)
		{}

		virtual ~dispatcher() {
			join();
		}

		virtual bool has_thread(std::thread const & thread) const override {
			return thread.get_id() == _thread_id;
		}

		virtual void run() override {
			_thread_id = std::this_thread::get_id();

			_running = true;

			for (; _running;)
				consume_tasks();

			bool has_tasks = _tasks.lock()->empty();
				
			if (has_tasks)
				consume_tasks();
		}

		virtual void join() override {
			_running = false;
		}

		void consume_tasks() {
			proto_thread_guard(_thread_id);

			auto & exec_list = _tasks.swap_active();

			for (auto & task : exec_list)
				task.invoke();

			exec_list.clear();
		}

	protected:

		virtual void enqueue_task(details::proxy_task && task) override {
			proto_debug_assert_break(_running);

			if (!_running)
				return;

			auto locked = _tasks.lock();
			locked->push_back(std::move(task));
		}

	private:
		swap_locking < std::vector < details::proxy_task > > 
								_tasks;

		std::atomic < bool >	_running;
		std::thread::id			_thread_id;
	};

	class async_dispatcher : public dispatcher_base {
	public:

		async_dispatcher () :
			_running (false)
		{}

		virtual ~dispatcher() {
			join();
		}

		virtual bool has_thread(std::thread const & thread) const override {
			return thread.get_id() == _thread_id;
		}

		virtual void run() override {
			_thread_id = std::this_thread::get_id();

			_running = true;

			for (; _running;)
				consume_tasks();

			bool has_tasks = _tasks.lock()->empty();
				
			if (has_tasks)
				consume_tasks();
		}

		virtual void join() override {
			_running = false;
		}

		void consume_tasks() {
			proto_thread_guard(_thread_id);

			auto & exec_list = _tasks.swap_active();

			for (auto & task : exec_list)
				task.invoke();

			exec_list.clear();
		}

	protected:

		virtual void enqueue_task(details::proxy_task && task) override {
			proto_debug_assert_break(_running);

			if (!_running)
				return;

			auto locked = _tasks.lock();
			locked->push_back(std::move(task));
		}

	private:
		swap_locking < std::vector < details::proxy_task > > 
								_tasks;

		std::atomic < bool >	_running;
		std::thread::id			_thread_id;
	};

}


int main(int arg_c, char * arg_v[]) {

	proto::swap_locking < std::vector < int > > swp_vector;

	{
		auto swp = swp_vector.lock();
		swp->push_back(3);
	}

	swp_vector.swap_active();

	return 0;
}
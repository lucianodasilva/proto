#pragma once

#ifndef _proto_event_h_
#define _proto_event_h_

#include "proto.details.h"
#include "proto.scheduler.h"

namespace proto {

	namespace details {

		template < class ... _argv_tv >
		struct event_invoker {
		public:
			using handler_t = std::function < void(_argv_tv & ...) >;
			using handler_vector_t = std::vector < std::shared_ptr < handler_t > >;

			template < class ... _invoke_args_tv >
			inline static void invoke(handler_vector_t const & handlers, _invoke_args_tv && ... args) {
				for (auto & h : handlers)
					h->operator ()(args...);
			}
		};

		template <>
		struct event_invoker < void > {
		public:
			using handler_t = std::function < void() >;
			using handler_vector_t = std::vector < std::shared_ptr < handler_t > >;

			inline static void invoke(handler_vector_t const & handlers) {
				for (auto & h : handlers)
					h->operator ()();
			}
		};

	}

	template < class ... _argv_tv >
	struct event : public non_copyable {
	public:

		using invoker_t = details::event_invoker < _argv_tv ... >;
		using handler_t = typename invoker_t::handler_t;

		template < class ... _invoke_args_tv >
		inline void invoke(_invoke_args_tv && ... argv) const {
			typename invoker_t::handler_vector_t handlers;

			{
				std::lock_guard < spin_mutex > lock(_handler_mutex);
				// copy handlers to avoid dead locks
				handlers = _handlers;
			}

			invoker_t::invoke(handlers, argv...);
		}

		template < class ... _invoke_args_tv >
		inline void scheduler_invoke(proto::scheduler_base & event_scheduler, _invoke_args_tv && ... argv) const {
			event_scheduler.enqueue ([this, &argv...] {
				invoke(argv...);
			});
		}

		inline void operator += (handler_t && handler) {
			std::lock_guard < spin_mutex > lock(_handler_mutex);
			_handlers.emplace_back(
				std::make_shared < handler_t > (std::move (handler))
			);
		}

		inline void operator -= (handler_t && handler) {
			std::lock_guard < spin_mutex > lock(_handler_mutex);
			auto it = find(
				_handlers.begin(),
				_handlers.end(),
				std::move (handler)
			);

			if (it != _handlers.end())
				_handlers.erase(it);
		}

	private:
		mutable spin_mutex						_handler_mutex;
		typename invoker_t::handler_vector_t	_handlers;
	};


}

#endif
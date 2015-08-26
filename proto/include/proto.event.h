#pragma once

#ifndef _proto_event_h_
#define _proto_event_h_

#include "proto.details.h"
#include "proto.event_scheduler.h"

namespace proto {

	template < class _sender_t, class ... _argv_tv >
	struct event {
	public:
		using handler_t = std::function < void(_sender_t &, _argv_tv & ...) >;
	private:
		mutable spin_mutex							_handler_mutex;
		std::vector < shared_ptr < handler_t > >	_handlers;
	public:

		// force single instance / no copy
		event(const event &) = delete;
		event & operator = (const event &) = delete;

		inline event() {}

		inline void sync_invoke(_sender_t & sender, _argv_tv & ... argv) const {
			std::vector < shared_ptr < handler_t > > handlers;

			{
				std::lock_guard < spin_mutex > lock(_handler_mutex);
				// copy handlers to avoid dead locks
				handlers = _handlers;
			}

			for (auto & h : handlers)
				h->operator ()(sender, argv ...);
		}

		inline void invoke(_sender_t & sender, _argv_tv & ... argv) const {
			event_scheduler::enqueue ([this, &sender, &argv...] {
				sync_invoke(sender, argv...);
			});
		}

		inline void operator += (handler_t && handler) {
			lock_guard < spin_mutex > lock(_handler_mutex);
			_handlers.emplace_back(
				make_shared < handler_t > (handler)
			);
		}

		inline void operator -= (handler_t && handler) {
			lock_guard < spin_mutex > lock(_handler_mutex);
			auto it = find(
				_handlers.begin(),
				_handlers.end(),
				forward (handler)
				);

			if (it != _handlers.end())
				_handlers.erase(it);
		}
	};

	template < class _sender_t >
	struct event < _sender_t, void > {
	public:
		using handler_t = std::function < void(_sender_t &) >;
	private:
		mutable spin_mutex					_handler_mutex;
		vector < shared_ptr < handler_t > >	_handlers;
	public:

		// force single instance / no copy
		event(const event &) = delete;
		event & operator = (const event &) = delete;

		inline event() {}

		inline void sync_invoke(_sender_t & sender) const {
			vector < shared_ptr < handler_t > > handlers;

			{
				lock_guard < spin_mutex > lock(_handler_mutex);
				handlers = _handlers;
			}

			for (auto & h : _handlers)
				h->operator ()(sender);
		}

		inline void invoke(_sender_t & sender) const {
			event_scheduler::enqueue ([this, &sender] {
				sync_invoke(sender);
			});
		}

		inline void operator += (handler_t && handler) {
			lock_guard < spin_mutex > lock(_handler_mutex);
			_handlers.emplace_back(
				make_shared < handler_t > (forward < handler_t >(handler))
			);
		}

		inline void operator -= (handler_t && handler) {
			lock_guard < spin_mutex > lock(_handler_mutex);
			auto it = find(
				_handlers.begin(),
				_handlers.end(),
				forward (handler)
				);

			if (it != _handlers.end())
				_handlers.erase(it);
		}
	};

}

#endif
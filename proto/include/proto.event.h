#pragma once

#ifndef _proto_event_h_
#define _proto_event_h_

#include "proto.details.h"
#include "proto.scheduler.h"

namespace proto {

	template < class _sender_t, class ... _argv_tv >
	struct event {
	public:
		using handler_t = std::function < void(_sender_t &, _argv_tv & ...) >;
	private:
		mutable spin_mutex			_handler_mutex;
		std::vector < handler_t >	_handlers;
	public:

		// force single instance / no copy
		event(const event &) = delete;
		event & operator = (const event &) = delete;

		inline event() {}

		inline void sync_invoke(_sender_t & sender, _argv_tv & ... argv) const {
			std::lock_guard < spin_mutex > lock(_handler_mutex);
			for (auto & h : _handlers)
				h(sender, argv ...);
		}

		inline void invoke(_sender_t & sender, _argv_tv & ... argv) const {
			scheduler::enqueue([this, &sender, &argv...] {
				sync_invoke(sender, argv...);
			});
		}

		inline void operator += (const handler_t & handler) {
			handlers.emplace_back(handler);
		}

		inline void operator -= (const handler_t & handler) {
			auto it = std::find(
				handlers.begin(),
				handlers.end(),
				handler
				);

			if (it != handlers.end())
				handlers.erase(it);
		}
	};

	template < class _sender_t >
	struct event < _sender_t, void > {
	public:
		using handler_t = std::function < void(_sender_t &) >;
	private:
		mutable spin_mutex			_handler_mutex;
		std::vector < handler_t >	_handlers;
	public:

		// force single instance / no copy
		event(const event &) = delete;
		event & operator = (const event &) = delete;

		inline event() {}

		inline void sync_invoke(_sender_t & sender) const {
			std::lock_guard < spin_mutex > lock(_handler_mutex);
			for (auto & h : _handlers)
				h(sender);
		}

		inline void invoke(_sender_t & sender) const {
			scheduler::enqueue([this, &sender] {
				sync_invoke(sender);
			});
		}

		inline void operator += (const handler_t & handler) {
			std::lock_guard < spin_mutex > lock(_handler_mutex);
			_handlers.emplace_back(handler);
		}

		inline void operator -= (const handler_t & handler) {
			std::lock_guard < spin_mutex > lock(_handler_mutex);
			auto it = std::find(
				_handlers.begin(),
				_handlers.end(),
				handler
				);

			if (it != _handlers.end())
				_handlers.erase(it);
		}
	};

}

#endif
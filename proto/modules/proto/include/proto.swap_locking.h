#pragma once
#ifndef _proto_swap_locking_h_
#define _proto_swap_locking_h_

#include "proto.details.thread.h"

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

			lock_handle(lock_handle &&) = default;
			lock_handle & operator = (lock_handle &&) = default;

			inline _t * operator -> () { return _handle; }
			inline _t const * operator -> () const { return _handle; }

		private:
			std::unique_lock < _mutex_t >
					_lock;
			_t *	_handle = nullptr;
		};

		inline lock_handle lock() {
			return { _active, _mutex };
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

}

#endif
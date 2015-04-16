#pragma once

#ifndef	_proto_details_h_
#define _proto_details_h_

namespace proto {

	namespace details {

		// scope guard implementation
		template < class _method_t >
		class scope_guard {
		private:

			bool		_active;
			_method_t	_method;

		public:

			// remove copy
			scope_guard () = delete;
			scope_guard (const scope_guard < _method_t > &) = delete;
			scope_guard < _method_t > operator = (const scope_guard < _method_t > &) = delete;

			// new instance
			inline scope_guard (_method_t m) :
				_active (true),
				_method (std::move (m))
			{}

			// move operator
			inline scope_guard (scope_guard < _method_t > && o) :
				_method (std::move (o._method)),
				_active (o._active)
			{
				o.dismiss ();
			}

			inline ~scope_guard () {
				trigger ();
			}

			inline void trigger () {
				if (_active)
					_method ();

				_active = false;
			}

			inline void dismiss () { _active = false; }

		};
	}

	template < class _method_t >
	inline details::scope_guard < _method_t > scope_guard (_method_t m) {
		return details::scope_guard < _method_t > (std::move (m));
	}

	template < class arg >
	inline void guard_dismiss (details::scope_guard < arg > & g) {
		g.dismiss ();
	}

	template < class arg_1, class ... _arg_v >
	inline void guard_dismiss (details::scope_guard < arg_1 > & g1, details::scope_guard < _arg_v > & ... gv) {
		guard_dismiss (g1);
		guard_dismiss (gv...);
	}

#	define auto_guard auto __scopeguard_##__COUNTER__ = scope_guard

}

#endif
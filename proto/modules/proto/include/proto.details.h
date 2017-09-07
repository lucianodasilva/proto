#pragma once

#ifndef	_proto_details_h_
#define _proto_details_h_

#include <atomic>
#include <fstream>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

#include "proto.details.expected.h"
#include "proto.details.mask.h"

namespace proto {

	class non_copyable {
	protected:
		constexpr non_copyable() = default;
		~non_copyable() = default;

		non_copyable(const non_copyable &) = delete;
		non_copyable & operator = (const non_copyable &) = delete;
	};

	struct spin_mutex {
	private:
		std::atomic_flag _lockless_flag = ATOMIC_FLAG_INIT;
	public:
		inline void lock() {
			while (_lockless_flag.test_and_set(std::memory_order_acquire)) {}
		}

		inline void unlock() {
			_lockless_flag.clear(std::memory_order_release);
		}
	};

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
				_active (o._active),
				_method (std::move (o._method))
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

#	define macro_cat_impl(x,y) x##y
#	define macro_cat(x,y) macro_cat_impl (x, y)
#	define auto_guard auto macro_cat (__scopeguard_, __COUNTER__) = proto::scope_guard

	inline expected < std::string > get_file_content(const std::string & filename) {
		std::ifstream in(filename, std::ios::in | std::ios::binary);

		if (in) {

			// find file size
			in.seekg(0, std::ios::end);
			auto file_length = in.tellg();

			// read file
			in.seekg(0, std::ios::beg);
			auto content = std::string(static_cast < size_t > (file_length), ' ');

			in.read(&content[0], content.size());
			in.close();

			return content;
		}

		return expected_failed < std::invalid_argument > ("Failed to read from \"" + filename + "\"");
	}

}

#endif
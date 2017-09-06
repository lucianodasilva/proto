#pragma once
#ifndef _proto_details_expected_h_
#define _proto_details_expected_h_

#include <exception>
#include <stdexcept>
#include <utility>

namespace proto {

	template < class _t >
	struct expected {
	private:

	#pragma warning (push)
	#pragma warning ( disable : 4624)

		union {
			_t					_value;
			std::exception_ptr	_error;
		};

	#pragma warning (pop)

		bool		_failed;

	public:

		inline expected() {}

		inline expected(const _t & v) :
			_value(v),
			_failed(false) {}

		inline expected(_t && v) :
			_value(std::move(v)),
			_failed(false) {}

		inline expected(const expected & v){
			this->operator =(v);
		}

		inline expected(expected && v) :
			_failed(v._failed) {
			if (_failed)
				new (&_error) std::exception_ptr(std::move(v._error));
			else
				new (&_value) _t(std::move(v._value));
		}

		inline expected(std::exception_ptr v) :
			_failed(true) {
			new (&_error) std::exception_ptr(v);
		}

		inline ~expected() {
			if (!_failed)
				_value.~_t();
			else
				_error.~exception_ptr();
		}

		inline expected & operator = (const expected & v) {
			_failed = v._failed;

			if (_failed)
				new (&_error) std::exception_ptr(v._error);
			else
				new (&_value) _t(v._value);

			return *this;
		}

		inline operator bool() {
			return !_failed;
		}

		inline _t & get() {
			if (_failed)
				std::rethrow_exception(_error);
			return _value;
		}

		inline const _t & get() const {
			if (_failed)
				std::rethrow_exception(_error);
			return _value;
		}

		inline std::exception_ptr get_exception() const {
			if (_failed)
				return _error;
			else
				return make_exception_ptr(std::runtime_error("Expected is not a failure"));
		}

		inline void rethrow() const {
			if (_failed)
				std::rethrow_exception(_error);
			else
				throw std::runtime_error("Expected is not a failure");
		}

		inline void swap(expected & v) {
			if (_failed) {
				if (v._failed) {
                    std::swap(_error, v._error);
					std::swap(_failed, v._failed);
				}
				else {
					v.swap(*this);
				}
			}
			else {
				if (v._failed) {
					auto t_error = std::move(v._error);
					new (&v._value) _t(std::move(_value));
					new (&v._error) std::exception_ptr(t_error);
					std::swap(_failed, v._failed);
				}
				else {
					using std::swap;
					swap(_value, v._value);
				}
			}
		}

	};

	template < >
	struct expected < void > {
	private:

		std::exception_ptr	_error;
		bool				_failed;

	public:

		inline expected() : _failed(false) {}

		inline expected(const expected & v)
		{
			this->operator=(v);
		}

		inline expected(expected && v) :
			_error(std::move(v._error)),
			_failed(v._failed)
		{}

		inline expected(std::exception_ptr v) :
			_error(v),
			_failed(true)
		{}

		inline expected & operator = (const expected & v) {
			_failed = v._failed;
			_error = v._error;

			return *this;
		}

		inline operator bool() {
			return !_failed;
		}

		inline std::exception_ptr get_exception() const {
			if (_failed)
				return _error;
			else
				return make_exception_ptr(std::runtime_error("Expected is not a failure"));
		}

		inline void rethrow() const {
			if (_failed)
				std::rethrow_exception(_error);
			else
				throw std::runtime_error("Expected is not a failure");
		}

		inline void swap(expected & v) {
			std::swap(_error, v._error);
			std::swap(_failed, v._failed);
		}

	};

	template < class _e, class ... _args_v >
	inline std::exception_ptr expected_failed(const _args_v & ... ex_args) {
		static_assert (std::is_assignable < std::exception, _e >::value, "Unsuported exception type found");
		return std::make_exception_ptr < _e >(_e(ex_args...));
	}

	inline std::exception_ptr expected_current_exception() {
		return std::make_exception_ptr(std::current_exception());
	}
}

#endif
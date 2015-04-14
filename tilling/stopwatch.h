#pragma once
#ifndef _harness_time_h_
#define _harness_time_h_

#include <chrono>

namespace stopwatch {

	using time_point = decltype (std::chrono::high_resolution_clock::now());

	inline static time_point time_now();

	/* returns a double representation of the elapsed seconds between both time points */
	inline static double time_elapsed(
		const time_point & start,
		const time_point & end
		);

	/* returns a double representation of the elapsed seconds between the starting point until the calling time */
	inline static double time_stop(const time_point & start);

	template < typename ... args_t >
	struct _timed_function_call {

		inline _timed_function_call(void(*func_address)(args_t ...)) :
			function_address(func_address) {}

		inline _timed_function_call(const _timed_function_call < args_t ... > & v)
			: function_address(v.function_address) {}

		void(*function_address)(args_t ...);

		inline double operator ()(args_t ... arg) {
			time_point start = time_now();
			function_address(arg...);
			return time_stop(start);
		}

	};

	template < typename ... args_t >
	struct _average_timed_function_call : public _timed_function_call < args_t... > {

		inline _average_timed_function_call(uint32_t calls, void(*func_address)(args_t ...)) :
			call_count(calls),
			_timed_function_call < args_t ... >(func_address) {}

		inline _average_timed_function_call(const _average_timed_function_call < args_t... > & v) :
			call_count(v.call_count), _timed_function_call < args_t ... >(v.function_address) {}

		uint32_t call_count;

		inline double operator ()(args_t ... arg) {
			double time_sum = 0.0;
			for (uint32_t i = 0; i < call_count; ++i)
				time_sum += _timed_function_call < args_t... >::operator () (arg...);

			return time_sum / double(call_count);
		}

	};

	template < typename ... args_t >
	_timed_function_call < args_t ... > timed_call(void(*function_address) (args_t ...)) {
		return _timed_function_call < args_t ... > {function_address};
	}

	template < typename ... args_t >
	_average_timed_function_call < args_t ... > timed_avg_call(uint32_t call_count, void(*function_address) (args_t ...)) {
		return _average_timed_function_call < args_t ... >(call_count, function_address);
	}

	inline static time_point time_now() {
		return std::chrono::high_resolution_clock::now();
	}

	inline static double time_elapsed_now(
		const time_point & start
		) {
		return std::chrono::duration_cast <std::chrono::milliseconds> (time_now() - start).count() / 1000.0;
	}

	/* returns a double representation of the elapsed seconds between both time points */
	inline static double time_elapsed(
		const time_point & start,
		const time_point & end
		) {
		return std::chrono::duration_cast <std::chrono::milliseconds> (end - start).count() / 1000.0;
	}

	/* returns a double representation of the elapsed seconds between the starting point until the calling time */
	inline static double time_stop(const time_point & start) {
		return time_elapsed(start, time_now());
	}

}


#endif
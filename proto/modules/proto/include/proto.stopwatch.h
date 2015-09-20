#pragma once
#ifndef _proto_stopwatch_h_
#define _proto_stopwatch_h_

#include <chrono>
#include <functional>

namespace proto {
	namespace stopwatch {

		using time_point = decltype(std::chrono::high_resolution_clock::now());

		inline static time_point time_now();

		/* returns a double representation of the elapsed seconds between both time points */
		inline static double time_elapsed(
				const time_point & start,
				const time_point & end
		);

		/* returns a double representation of the elapsed seconds between the starting point until the calling time */
		inline static double time_stop(const time_point & start);

		enum class call_result_type {
			average,
			total
		};

		namespace details {

			template < class _rt, class ... _args_tv >
			struct timed_function_call {

				using func_t = std::function < _rt (_args_tv ...) >;

				func_t function;

				inline timed_function_call (func_t function) :
					function (function) { }

				inline timed_function_call (const timed_function_call & v) :
					function (v.function) { }

				inline double operator()(_args_tv ... args) {
					time_point start = time_now ();
					function (args...);
					return time_stop (start);
				}

			};

			template < class _rt, class ... _args_tv >
			struct multiple_timed_function_call : public timed_function_call < _rt, _args_tv ... > {

				uint64_t call_count;

				inline multiple_timed_function_call (uint32_t call_count_v, typename timed_function_call < _rt, _args_tv ... >::func_t function) :
					timed_function_call < _rt, _args_tv ... > (function),
					call_count (call_count_v)
				{}

				inline multiple_timed_function_call (const multiple_timed_function_call & v) :
				timed_function_call < _rt, _args_tv ... >::function (v.function),
					call_count (v.call_count)
				{ }

				inline double operator()(call_result_type result, _args_tv ... args) {
					time_point start = time_now ();

					double time_sum = 0.0;
					for (uint32_t i = 0; i < call_count; ++i)
						timed_function_call < _rt, _args_tv ... >::function (args ...);

					time_sum = time_stop (start);

					if (result == call_result_type::total)
						return time_sum;
					else if (result == call_result_type::average)
						return time_sum / double (call_count);
				}

			};

		}

		template < class _rt, class ... _args_tv >
		inline details::timed_function_call < _rt, _args_tv ... > timed_call (
			typename details::timed_function_call < _rt, _args_tv ... >::func_t f
		) {
			return details::timed_function_call < _rt, _args_tv ... > ( f );
		}

		template < class _rt, class ... _args_tv >
		inline details::multiple_timed_function_call < _rt, _args_tv ... > multiple_timed_call (
			typename details::multiple_timed_function_call < _rt, _args_tv ... >::func_t f
		) {
			return details::multiple_timed_function_call < _rt, _args_tv ... > ( f );
		}

		inline static time_point time_now() {
			return std::chrono::high_resolution_clock::now();
		}

		inline static double time_elapsed_now(
				const time_point & start
		) {
			return std::chrono::duration_cast<std::chrono::milliseconds>(time_now() - start).count() / 1000.0;
		}

		/* returns a double representation of the elapsed seconds between both time points */
		inline static double time_elapsed ( 
			const time_point & start,
			const time_point & end
		) {
			return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
		}

		/* returns a double representation of the elapsed seconds between the starting point until the calling time */
		inline static double time_stop(
			const time_point & start
		) {
			return time_elapsed(start, time_now());
		}

	}
}


#endif
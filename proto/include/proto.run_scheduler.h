#pragma once

#ifndef	_proto_run_scheduler_h_
#define _proto_run_scheduler_h_

#include "proto.scheduler.h"

namespace proto {

	class run_scheduler : public scheduler_base {
	public:

		run_scheduler();

		template < class _ft_t, class ... _args_t >
		static inline auto enqueue(_ft_t && f, _args_t && ... args)
			-> future < typename result_of < _ft_t(_args_t ...)>::type >
		{
			auto & s = instance();
			return static_cast <scheduler_base &> (s).enqueue(f, args...);
		}

		void run(const function < void(run_scheduler &) > & callback );

		void stop();

	};

}

#endif
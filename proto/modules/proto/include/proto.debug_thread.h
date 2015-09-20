#ifndef _proto_debug_thread_h_
#define _proto_debug_thread_h_

#include "proto.debug_break.h"
#include "proto.debug_print.h"

#ifdef _DEBUG

#define proto_scheduler_guard(scheduler, message)

#else

#define proto_scheduler_guard(scheduler, message) \
if (scheduler.containd_thread (std::this_thread::get_id ())) { \
	debug_print << "scheduler break: " << message; \
	proto_debug_break (); \
}

#endif

#endif
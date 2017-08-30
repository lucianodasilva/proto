#pragma once
#ifndef _proto_debug_thread_h_
#define _proto_debug_thread_h_

#include "proto.debug_break.h"

#ifndef _DEBUG
#	define proto_scheduler_guard(scheduler, message)
#else
#	define proto_scheduler_guard(scheduler, message) \
	if (scheduler.has_thread (std::this_thread::get_id ())) { \
		proto_debug_break (); \
	}
#endif

#endif
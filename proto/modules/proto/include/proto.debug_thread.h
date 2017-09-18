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

#ifndef _DEBUG
#	define proto_dispatcher_guard(_dispatcher_)
#else
#	define proto_dispatcher_guard(_dispatcher_) proto_debug_assert_break(_dispatcher_.has_thread (std::this_thread::get_id ()))
#endif

#ifndef _DEBUG
#	define proto_thread_guard(_thread_id_)
#else
#	define proto_thread_guard(_thread_id_) proto_debug_assert_break (_thread_id_ != std::this_thread::get_id ())
#endif

#endif
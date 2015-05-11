#ifndef _proto_debug_thread_h_
#define _proto_debug_thread_h_

#include "proto.debug_break.h"
#include "proto.debug_print.h"

#ifdef _DEBUG

#define proto_thread_guard(thread, message)

#else

#define proto_thread_guard(thread, message) \
if (std::this_thread::get_id () != thread_instance.get_id()) { \
	debug_print << "thread break: " << message; \
	proto_debug_break (); \
}

#endif

#endif
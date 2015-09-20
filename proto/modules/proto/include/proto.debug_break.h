#pragma once
#ifndef _proto_debug_break_h_
#define _proto_debug_break_h_

#ifndef _DEBUG

#define proto_debug_break

#else

#include "proto.details.h"

#ifdef PROTO_COMPILER_MSVC

#define proto_debug_break() __debugbreak;

#else

#define proto_debug_break() __asm__ volatile("int $0x03")

#endif
#endif

#endif
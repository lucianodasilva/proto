
#ifndef _proto_debug_break_h_
#define _proto_debug_break_h_

#ifndef _DEBUG

#define proto_debug_break

#else

#include "proto.details.h"

#ifdef PROTO_COMPILER_MSVC

#define proto_debug_break __debugbreak

#else

#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>

#ifdef __cplusplus
extern "C" {
#endif
	
#	define DEBUG_BREAK_PREFER_BUILTIN_TRAP_TO_SIGTRAP 0
	
#if defined(__i386__) || defined(__x86_64__)
	
#	define HAVE_TRAP_INSTRUCTION 1
#	define trap_instruction __asm__ volatile("int $0x03")
	
#elif defined(__thumb__)
	
#	define HAVE_TRAP_INSTRUCTION 1
	
#	if 1
#		define trap_instruction __asm__ volatile(".inst 0xde01")
#	else
#		define trap_instruction __asm__ volatile(".inst.w 0xf7f0a000")
#	endif
	
#elif defined(__arm__) && !defined(__thumb__)
	
#	define HAVE_TRAP_INSTRUCTION 1
#	define trap_instruction __asm__ volatile(".inst 0xe7f001f0")
	
#else
	
#	define HAVE_TRAP_INSTRUCTION 0
	
#endif
	
#if HAVE_TRAP_INSTRUCTION
#	define proto_debug_break() trap_instruction
#else
#	if DEBUG_BREAK_PREFER_BUILTIN_TRAP_TO_SIGTRAP
#		define proto_debug_break() __builtin_trap ()
#	else
#		define proto_debug_break() raise(SIGTRAP)
#	endif
#endif
	
#ifdef __cplusplus
}
#endif

#endif

#undef DEBUG_BREAK_PREFER_BUILTIN_TRAP_TO_SIGTRAP
#undef HAVE_TRAP_INSTRUCTION

#endif
#endif
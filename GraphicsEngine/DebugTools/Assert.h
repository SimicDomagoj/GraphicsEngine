#pragma once

#define ASSERTIONS_ENABLED 1

void reportAssertionFailure(char* expr, char* file, int line);

#if ASSERTIONS_ENABLED
// define some inline assembly that causes a break
// into the debugger -- this will be different on each
// target CPU
//todo different debug breaks according to system
#define debugBreak() __asm { int 3 }
// check the expression and fail if it is false
#define ASSERT(expr) \
	if (expr) { } \
	else \
	{ \
		reportAssertionFailure(#expr, __FILE__, __LINE__); \
		debugBreak(); \
	}
#else
#define ASSERT(expr) // evaluates to nothing
#endif
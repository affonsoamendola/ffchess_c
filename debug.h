#ifndef FF_DEBUG_H
#define FF_DEBUG_H

//If is a debug build, enable debug features.
#ifdef _DEBUG
	#define _ASSERTIONS_ENABLED
	#define _DEBUG_BREAKPOINTS
#endif

//Defines code breakpoints
#ifdef _DEBUG_BREAKPOINTS
	
	#define DEBUG_BREAK() asm { int 3 }
#else
	
	#define DEBUG_BREAK()
#endif

//Defines Assertions
#ifdef _ASSERTIONS_ENABLED
	#define ASSERT(expr) \
		if(expr) { } \
		else \
		{ \
			debug_assertion_fail(#expr, __FILE__, __LINE__);\
			DEBUG_BREAK();\
		} \
#else

	#define ASSERT(expr)
#endif

#endif
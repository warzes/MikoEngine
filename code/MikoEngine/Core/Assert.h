#pragma once

// TODO: http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/

#if SE_PLATFORM_WINDOWS
#	define SE_DEBUG_BREAK __debugbreak()
#elif SE_PLATFORM_LINUX
#	define SE_DEBUG_BREAK __builtin_trap()
#else
#endif

#if SE_DEBUG
#	define SE_ASSERT(expression, message) SE_DEBUG_BREAK; assert((expression) && message);
#else
#	define SE_ASSERT(expression, message)
#endif
#pragma once

// TODO: переписать _STL_VERIFY на свой. Также обратить внимание на аргументы _CrtDbgReport 

#if SE_COMPILER_MSVC
#	define SE_ASSERT__(expression, message) _STL_VERIFY(expression, message)
#else
#	define SE_ASSERT__(expression, message) assert((expression) && message);
#endif

// TODO: http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
//=============================================================================
// Assert Macros
//=============================================================================
#if SE_DEBUG
#	define SE_ASSERT(expression, message) SE_DEBUG_BREAK; SE_ASSERT__(expression, message);
#	define SE_VERIFY(expression, message) SE_ASSERT(expression, message)
#	define SE_ASSUME(expression, message) SE_ASSERT(expression, message)
#else
#	define SE_ASSERT(expression, message)
#	define SE_VERIFY(expression, message) (expression)
#	define SE_ASSUME(expression, message) __assume(expression)
#endif
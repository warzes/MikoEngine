#pragma once

#include "Core/DefaultAssert.h"

// TODO: добавить еще один ассет, который можно выключить даже в дебаге - например его использовать в аллокаторе
// TODO: посмотреть на реализацию _STL_VERIFY в студии. Также _CrtDbgReport 
// TODO: http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
//=============================================================================
// Assert Macros
//=============================================================================
#if SE_DEBUG
#	define _ASSERT_FUNC_(expression, format, ...) DefaultAssert::Get().HandleAssert(#expression, __FILE__, static_cast<uint32_t>(__LINE__), format, ##__VA_ARGS__))

#	define SE_ASSERT(expression, format, ...)                                     \
		do                                                                        \
		{                                                                         \
			if (!(expression) && _ASSERT_FUNC_(expression, format, ##__VA_ARGS__) \
			{                                                                     \
				/*RHI_LOG(CRITICAL, format, ##__VA_ARGS__);*/                         \
				SE_DEBUG_BREAK;                                                   \
			}                                                                     \
		} while (0);

#	define SE_VERIFY(expression, format, ...) SE_ASSERT(expression, format, ##__VA_ARGS__)
#	define SE_ASSUME(expression, format, ...) SE_ASSERT(expression, format, ##__VA_ARGS__)
#else
#	define SE_ASSERT(expression, format, ...)
#	define SE_VERIFY(expression, format, ...) (expression)
#	define SE_ASSUME(expression, format, ...) __assume(expression)
#endif
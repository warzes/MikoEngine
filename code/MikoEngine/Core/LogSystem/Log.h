#pragma once

#include "Core/LogSystem/detail/CoreLogSystem.h"

#define SE_LOG(level, str)  {CoreLogSystem::Print(str, level);}
#define SE_LOG_ERROR(str)   {CoreLogSystem::Print(str, LogVerbosity::Error);}
#define SE_LOG_WARNING(str) {CoreLogSystem::Print(str, LogVerbosity::Warning);}
#define SE_LOG_INFO(str)    {CoreLogSystem::Print(str, LogVerbosity::Info);}


#if SE_PLATFORM_WINDOWS
#define MLOG(...) \
	{ \
		//char __str__buf__[2048]; \
		//auto log = DefaultLog.GetLog(SimpleLog::Level::Info); \
		//sprintf_s(__str__buf__, "%-40s:%-5d", __func__, __LINE__);   \
		//log << __str__buf__; \
		//sprintf_s(__str__buf__, __VA_ARGS__); \
		//log << __str__buf__; \
	}


#define MLOGE(...)  { char __str__buf__[2048]; sprintf_s(__str__buf__, "%-6s", "ERROR:"); OutputDebugStringA(__str__buf__); sprintf_s(__str__buf__, "%-40s:%-5d", __func__, __LINE__); OutputDebugStringA(__str__buf__); sprintf_s(__str__buf__, __VA_ARGS__); OutputDebugStringA(__str__buf__); sprintf_s(__str__buf__, "\n"); OutputDebugStringA(__str__buf__); }
#elif SE_PLATFORM_OSX || SE_PLATFORM_LINUX || SE_PLATFORM_IOS
#define MLOG(...)   { fprintf(stdout, "%-6s", "LOG:");    fprintf(stdout, "%-40s:%-5d", __func__, __LINE__); fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); }
#define MLOGE(...)  { fprintf(stdout, "%-6s", "ERROR:");  fprintf(stdout, "%-40s:%-5d", __func__, __LINE__); fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); }
#elif SE_PLATFORM_ANDROID
#include <android/log.h>
#define MLOG(FORMAT,...)    { __android_log_print(ANDROID_LOG_DEBUG, "t", FORMAT, ##__VA_ARGS__); }
#define MLOGE(FORMAT,...)   { __android_log_print(ANDROID_LOG_DEBUG, "t", FORMAT, ##__VA_ARGS__); }
#endif
#pragma once

#if SE_PLATFORM_WINDOWS
#	include "Platform/WindowsPlatformCoreLog.h"
#elif SE_PLATFORM_ANDROID
#	include "Platform/AndroidPlatformCoreLog.h"
#elif SE_PLATFORM_LINUX
#	include "Platform/LinuxPlatformCoreLog.h"
#elif SE_PLATFORM_EMSCRIPTEN
#	include "Platform/EmscriptenPlatformCoreLog.h"
#endif
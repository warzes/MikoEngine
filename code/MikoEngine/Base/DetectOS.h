#pragma once

// Platform OS
#define SE_PLATFORM_ANDROID        0
#define SE_PLATFORM_BSD            0
#define SE_PLATFORM_EMSCRIPTEN     0
#define SE_PLATFORM_HAIKU          0
#define SE_PLATFORM_HURD           0
#define SE_PLATFORM_IOS            0
#define SE_PLATFORM_LINUX          0
#define SE_PLATFORM_NX             0
#define SE_PLATFORM_OSX            0
#define SE_PLATFORM_RPI            0
#define SE_PLATFORM_WINDOWS        0
#define SE_PLATFORM_WINRT          0

//=============================================================================
// Platform
//=============================================================================
// https://github.com/Henrito/htpredef/blob/master/predef/platform.h
#if defined(__ANDROID__) || defined(ANDROID) // Android compiler defines __linux__
#	undef  SE_PLATFORM_ANDROID
#	define SE_PLATFORM_ANDROID __ANDROID_API__
#elif  defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__)  || defined(__OpenBSD__)  || defined(__DragonFly__)
#	undef  SE_PLATFORM_BSD
#	define SE_PLATFORM_BSD 1
#elif defined(__EMSCRIPTEN__)
#	undef  SE_PLATFORM_EMSCRIPTEN
#	define SE_PLATFORM_EMSCRIPTEN 1
#elif defined(__HAIKU__)
#	undef  SE_PLATFORM_HAIKU
#	define SE_PLATFORM_HAIKU 1
#elif defined(__GNU__)
#	undef  SE_PLATFORM_HURD
#	define SE_PLATFORM_HURD 1
#elif defined(__VCCOREVER__) // RaspberryPi compiler defines __linux__
#	undef  SE_PLATFORM_RPI
#	define SE_PLATFORM_RPI 1
#elif  defined(__linux__)
#	undef  SE_PLATFORM_LINUX
#	define SE_PLATFORM_LINUX 1
#elif defined(__NX__)
#	undef  SE_PLATFORM_NX
#	define SE_PLATFORM_NX 1
#elif  defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)  || defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#	undef  SE_PLATFORM_IOS
#	define SE_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#	undef  SE_PLATFORM_OSX
#	define SE_PLATFORM_OSX __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#elif defined(_WIN32) || defined(_WIN64)
#	undef  SE_PLATFORM_WINDOWS
#	define SE_PLATFORM_WINDOWS 1
#else
#	error Unknown platform.
#endif // SE_PLATFORM_*
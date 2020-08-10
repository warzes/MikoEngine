#pragma once

#if SE_PLATFORM_WINDOWS
#	if SE_ARCH_64BIT
typedef unsigned __int64 handle;	// Replacement for nasty Microsoft Windows stuff leading to header chaos
#	else
typedef unsigned __int32 handle;	// Replacement for nasty Microsoft Windows stuff leading to header chaos
#	endif
#elif SE_PLATFORM_LINUX
#	if SE_ARCH_64BIT
typedef uint64_t handle;
#	else
typedef uint32_t handle;
#	endif
#else
#	error "Unsupported platform"
#endif
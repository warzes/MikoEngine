#pragma once

// Build OS Version (name in VersionOSConst)
#if SE_PLATFORM_WINDOWS
#	define SE_WINDOWS_VERSION       SE_WINDOWS_10
#endif

// TODO:
#define SE_OPENGL 1
#define SE_VULKAN 1
#if SE_VULKAN
#	undef SE_OPENGL
#endif
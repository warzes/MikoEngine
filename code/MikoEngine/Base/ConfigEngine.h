#pragma once

#define TIME_IMPL_STL 0

// Build OS Version (name in VersionOSConst)
#if SE_PLATFORM_WINDOWS
#	define SE_WINDOWS_VERSION       SE_WINDOWS_10
#endif

// TODO:
#define SE_OPENGL 1
#define SE_VULKAN 0

// Set "SE_OPENGL_GLSLTOSPIRV" as preprocessor definition when building this library to add support for compiling GLSL into SPIR-V
#define SE_OPENGL_GLSLTOSPIRV 1

#define SE_STATISTICS 1
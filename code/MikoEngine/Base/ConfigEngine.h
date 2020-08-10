#pragma once

#define TIME_IMPL_STL 0

// Build OS Version (name in VersionOSConst)
#if SE_PLATFORM_WINDOWS
#	define SE_WINDOWS_VERSION       SE_WINDOWS_10
#endif

// TODO:
#define SE_RHINULL 1
#define SE_OPENGL 1
#define SE_OPENGLES 1
#define SE_VULKAN 1
#define SE_DIRECT3D11 1
#define SE_DIRECT3D12 1

// Set "SE_GLSLTOSPIRV" as preprocessor definition when building this library to add support for compiling GLSL into SPIR-V, increases the binary size around one MiB
#define SE_GLSLTOSPIRV 1

// Set "SE_STATISTICS" as preprocessor definition in order to enable the gathering of statistics (tiny binary size and tiny negative performance impact)
#define SE_STATISTICS 1
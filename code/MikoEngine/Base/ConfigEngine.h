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
#define SE_DIRECT3D11 0
#define SE_DIRECT3D12 1

// Set "SE_GLSLTOSPIRV" as preprocessor definition when building this library to add support for compiling GLSL into SPIR-V, increases the binary size around one MiB
#define SE_GLSLTOSPIRV 1

// Set "SE_STATISTICS" as preprocessor definition in order to enable the gathering of statistics (tiny binary size and tiny negative performance impact)
#define SE_STATISTICS 1

//If this RHI was compiled with "SE_OPENGL_STATE_CLEANUP" set as preprocessor definition, the previous OpenGL state will be restored after performing an operation (worse performance, increases the binary size slightly, might avoid unexpected behaviour when using OpenGL directly beside this RHI)
#define SE_OPENGL_STATE_CLEANUP 1

//- Set "RENDERER_GRAPHICS_DEBUGGER" as preprocessor definition to enable graphics debugger support
#define RENDERER_GRAPHICS_DEBUGGER 1
//- Set "RENDERER_PROFILER" as preprocessor definition to enable profiler support
#define RENDERER_PROFILER 1
//- Set "RENDERER_IMGUI" as preprocessor definition to enable ImGui support
#define RENDERER_IMGUI 1
//- Set "RENDERER_OPENVR" as preprocessor definition to enable OpenVR support
#define RENDERER_OPENVR 1
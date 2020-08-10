#pragma once

//-----------------------------------------------------------------------------
// Base Header
//-----------------------------------------------------------------------------
#include "Base/EngineVersionConst.h"
#include "Base/DetectCompiler.h"
#include "Base/DetectOS.h"
#include "Base/VersionOSConst.h"
#include "Base/ConfigEngine.h"
#include "Base/BaseMacros.h"
#include "Base/InlineFunc.h"
#include "Base/PlatformTypes.h"

//-----------------------------------------------------------------------------
// Header config
//-----------------------------------------------------------------------------
//#define _CRT_SECURE_NO_WARNINGS // TODO: удалить

//-----------------------------------------------------------------------------
// 3rd-party Header
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_LEVEL(0);

//#if SE_OPENGL
//#	include <glad.h>
//#endif

#if SE_OPENGL || SE_VULKAN
#	if SE_GLSLTOSPIRV
#		include <SPIRV/GlslangToSpv.h>
#		include <glslang/MachineIndependent/localintermediate.h>
#	endif
#	include <smol-v/smolv.h>
#endif

#if SE_DIRECT3D12
#	include <d3d12.h>
#	include <d3dx12.h>
#	include <dxgi1_5.h>
#	include <d3dcompiler.h>
#	include <pix_win.h>
#endif

//#if SE_OPENGL
//#	if SE_PLATFORM_WINDOWS
//#		include <GL/gl.h>
//#		include <GL/glext.h>	// Requires definitions from "gl.h"
//#		include <GL/wglext.h>	// Requires definitions from "gl.h"
//#	elif SE_PLATFORM_LINUX
//#		include <GL/glx.h>
//#		include <GL/glxext.h>
//#		include <GL/gl.h>
//#		include <GL/glext.h>	// Requires definitions from "gl.h"
//#		include <X11/Xlib.h>// TODO(co) Review which of the following headers can be removed
//#	endif
//#endif

#	define GLFW_INCLUDE_NONE
#if SE_VULKAN
//#	define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#if SE_PLATFORM_LINUX || SE_PLATFORM_BSD
#	if ENTRY_CONFIG_USE_WAYLAND
#		include <wayland-egl.h>
#		define GLFW_EXPOSE_NATIVE_WAYLAND
#	else
#		define GLFW_EXPOSE_NATIVE_X11
#		define GLFW_EXPOSE_NATIVE_GLX
#	endif
#elif SE_PLATFORM_OSX
#	define GLFW_EXPOSE_NATIVE_COCOA
#	define GLFW_EXPOSE_NATIVE_NSGL
#elif SE_PLATFORM_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#endif //
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
// GLM use the OpenGL depth range of -1.0 to 1.0 by default.
// We want range from 0.0 to 1.0 for Vulkan.
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <stb_image.h>

#include <tiny_obj_loader.h>

#include <imgui/imgui.h>

//#if SE_VULKAN
//#	include <vulkan/vulkan.h>
//#	define VMA_USE_STL_SHARED_MUTEX 0 // TODO
//#	include <vk_mem_alloc.h>
//#endif // SE_VULKAN

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

SE_PRAGMA_WARNING_POP

//-----------------------------------------------------------------------------
// Platform Header
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_LEVEL(0);
#if SE_PLATFORM_WINDOWS
#	include "Base/Win32PlatformDefinitions.h"
#	include <Windows.h>
#	include <VersionHelpers.h>
#	undef near
#	undef far
#else
#	include <sys/time.h>
#endif // SE_PLATFORM_*
SE_PRAGMA_WARNING_POP

//-----------------------------------------------------------------------------
// STD C++17 Header
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_LEVEL(0);

#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <stdexcept>

#include <algorithm>
#include <chrono>
#include <optional>
#include <sstream>
#include <fstream>
#include <iostream>

#include <vector>
#include <array>
#include <set>
#include <stack>
#include <unordered_map>

#include <atomic>
#include <mutex>
#include <thread>

SE_PRAGMA_WARNING_POP

//-----------------------------------------------------------------------------
// Disable warning
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_DISABLE_MSVC(4514); //
SE_PRAGMA_WARNING_DISABLE_MSVC(4820); //'4' bytes padding added after data member

// TEMP WARN DISABLE
SE_PRAGMA_WARNING_DISABLE_MSVC(4100); // unreferenced formal parameter
SE_PRAGMA_WARNING_DISABLE_MSVC(4625); // copy constructor was implicitly defined as deleted
SE_PRAGMA_WARNING_DISABLE_MSVC(4626); // assignment operator was implicitly defined as deleted
SE_PRAGMA_WARNING_DISABLE_MSVC(4996); // This function or variable may be unsafe.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
SE_PRAGMA_WARNING_DISABLE_MSVC(5026); // move constructor was implicitly defined as deleted
SE_PRAGMA_WARNING_DISABLE_MSVC(5027); // move assignment operator was implicitly defined as deleted
SE_PRAGMA_WARNING_DISABLE_MSVC(5039); // pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception.
SE_PRAGMA_WARNING_DISABLE_MSVC(5045); // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

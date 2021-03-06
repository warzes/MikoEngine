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
#define _STL_WARNING_LEVEL 3

#if SE_PLATFORM_WINDOWS
#	include "Base/Win32PlatformDefinitions.h"
#endif // SE_PLATFORM_WINDOWS

//-----------------------------------------------------------------------------
// Disable warning
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_DISABLE_MSVC(4514); // unreferenced inline function has been removed;
SE_PRAGMA_WARNING_DISABLE_MSVC(4710); // function not inlined;
SE_PRAGMA_WARNING_DISABLE_MSVC(4711); // function selected for inline expansion;
SE_PRAGMA_WARNING_DISABLE_MSVC(4820); // 'bytes' bytes padding added after construct 'member_name';

// TEMP WARN DISABLE
SE_PRAGMA_WARNING_DISABLE_MSVC(4100); // unreferenced formal parameter
SE_PRAGMA_WARNING_DISABLE_MSVC(4625); // copy constructor was implicitly defined as deleted
SE_PRAGMA_WARNING_DISABLE_MSVC(4626); // assignment operator was implicitly defined as deleted
SE_PRAGMA_WARNING_DISABLE_MSVC(4996); // This function or variable may be unsafe.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
SE_PRAGMA_WARNING_DISABLE_MSVC(5026); // move constructor was implicitly defined as deleted
SE_PRAGMA_WARNING_DISABLE_MSVC(5027); // move assignment operator was implicitly defined as deleted
SE_PRAGMA_WARNING_DISABLE_MSVC(5039); // pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception.
SE_PRAGMA_WARNING_DISABLE_MSVC(5045); // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

//-----------------------------------------------------------------------------
// STD C++17 Header
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_LEVEL(0)
SE_PRAGMA_WARNING_DISABLE_MSVC(4355); // 
SE_PRAGMA_WARNING_DISABLE_MSVC(5204); // 

#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <stdexcept>

#include <algorithm>
#include <chrono>
#include <optional>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <iostream>
#include <functional>

#include <queue>
#include <vector>
#include <array>
#include <set>
#include <stack>
#include <unordered_map>

#include <atomic>
#include <mutex>
#include <thread>
#include <future>

//SE_PRAGMA_WARNING_POP

//-----------------------------------------------------------------------------
// 3rd-party Header
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_LEVEL(0)

//-----------------------------------------------------------------------------
// Direct3D 11
#if SE_DIRECT3D11
#	include <d3d11.h>
#	include <d3d11_1.h>
#endif

//-----------------------------------------------------------------------------
// Direct3D 12
#if SE_DIRECT3D12
#	include <d3d12.h>
#	include <d3dx12.h>
#	include <dxgi1_6.h>
#	include <d3dcompiler.h>
#	include <pix_win.h>
#endif

//-----------------------------------------------------------------------------
// OpenGL ES 3
#if SE_OPENGLES
#	define GL_GLES_PROTOTYPES 0
#	include <GLES3/gl3.h>
#	include <GLES3/gl2ext.h>
#	include <EGL/egl.h>
#	include <EGL/eglext.h>
#endif

//-----------------------------------------------------------------------------
// OpenGL
#if SE_OPENGL
#	if SE_PLATFORM_WINDOWS
#		undef GL_GLEXT_PROTOTYPES
#		include <GL/gl.h>
#		include <GL/glext.h>	// Requires definitions from "gl.h"
#		include <GL/wglext.h>	// Requires definitions from "gl.h"
#	elif SE_PLATFORM_LINUX
#		include <GL/glx.h>
#		include <GL/glxext.h>
#		include <GL/gl.h>
#		include <GL/glext.h>	// Requires definitions from "gl.h"
#		include <X11/Xlib.h>// TODO(co) Review which of the following headers can be removed
#	endif
#endif

//-----------------------------------------------------------------------------
// Vulkan
#if SE_VULKAN
#	if SE_PLATFORM_WINDOWS
#		define VK_USE_PLATFORM_WIN32_KHR
#	elif defined(__ANDROID__)
#		define VK_USE_PLATFORM_ANDROID_KHR
#	elif defined(LINUX)
// TODO(sw) Make this optional which platform we support? For now we support xlib and Wayland.
#		define VK_USE_PLATFORM_XLIB_KHR
#		define VK_USE_PLATFORM_WAYLAND_KHR
#	endif
//#	define VK_NO_PROTOTYPES
#	include <vulkan/vulkan.h>
//#	define VMA_USE_STL_SHARED_MUTEX 0 // TODO
//#	include <vk_mem_alloc.h>
#endif

//-----------------------------------------------------------------------------
// GLM
// GLM use the OpenGL depth range of -1.0 to 1.0 by default. We want range from 0.0 to 1.0 for Vulkan.
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_CXX17
#define GLM_FORCE_INLINE
#define GLM_FORCE_SIMD_AVX2
#define GLM_FORCE_XYZW_ONLY
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/detail/setup.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/intersect.hpp>

//-----------------------------------------------------------------------------
// SPIRV
#if SE_OPENGL || SE_VULKAN
#	if SE_GLSLTOSPIRV
#		include <SPIRV/GlslangToSpv.h>
#		include <glslang/MachineIndependent/localintermediate.h>
#	endif
#	include <smol-v/smolv.h>
#endif

//-----------------------------------------------------------------------------
// GLFW
#define GLFW_INCLUDE_NONE
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

//-----------------------------------------------------------------------------
// Assimp
//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>

//-----------------------------------------------------------------------------
// STB
#include <stb_image.h>

//-----------------------------------------------------------------------------
// Tiny OBJ Loader
//#include <tiny_obj_loader.h>

//-----------------------------------------------------------------------------
// Imgui
#include <imgui/imgui.h>

SE_PRAGMA_WARNING_POP

//-----------------------------------------------------------------------------
// Platform Header
//-----------------------------------------------------------------------------
SE_PRAGMA_WARNING_LEVEL(0);
#if SE_PLATFORM_WINDOWS
#	include <Windows.h>
#	include <VersionHelpers.h>
#	include <wrl.h> // Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#	include <shellapi.h>
#	include <strsafe.h>
#	undef near
#	undef far
#else
#	include <sys/time.h>
#endif // SE_PLATFORM_*
SE_PRAGMA_WARNING_POP
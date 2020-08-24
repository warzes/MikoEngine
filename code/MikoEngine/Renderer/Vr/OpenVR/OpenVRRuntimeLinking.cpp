#include "stdafx.h"
#if RENDERER_OPENVR
#define OPENVR_DEFINERUNTIMELINKING
#include "Renderer/Vr/OpenVR/OpenVRRuntimeLinking.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Context.h"

#ifdef _WIN32
	
#elif defined LINUX
	// TODO(co) Review which of the following headers can be removed
	#include <X11/Xlib.h>

	#include <GL/glx.h>
	#include <GL/glxext.h>

	#include <dlfcn.h>
	#include <link.h>
	#include <iostream>
#else
	#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Macros                                                ]
	//[-------------------------------------------------------]
	// Define a helper macro
	#ifdef _WIN32
		#define IMPORT_FUNC(funcName)																																						\
			if (result)																																										\
			{																																												\
				void* symbol = ::GetProcAddress(static_cast<HMODULE>(mOpenVRSharedLibrary), #funcName);																						\
				if (nullptr != symbol)																																						\
				{																																											\
					*(reinterpret_cast<void**>(&(funcName))) = symbol;																														\
				}																																											\
				else																																										\
				{																																											\
					wchar_t moduleFilename[MAX_PATH];																																		\
					moduleFilename[0] = '\0';																																				\
					::GetModuleFileNameW(static_cast<HMODULE>(mOpenVRSharedLibrary), moduleFilename, MAX_PATH);																				\
					RHI_LOG(CRITICAL, "The renderer failed to locate the entry point \"%s\" within the OpenVR shared library \"%s\"", #funcName, moduleFilename)	\
					result = false;																																							\
				}																																											\
			}
	#elif defined LINUX
		#define IMPORT_FUNC(funcName)																																					\
			if (result)																																									\
			{																																											\
				void* symbol = ::dlsym(mOpenVRSharedLibrary, #funcName);																												\
				if (nullptr != symbol)																																					\
				{																																										\
					*(reinterpret_cast<void**>(&(funcName))) = symbol;																													\
				}																																										\
				else																																									\
				{																																										\
					link_map *linkMap = nullptr;																																		\
					const char* libraryName = "unknown";																																\
					if (dlinfo(mOpenVRSharedLibrary, RTLD_DI_LINKMAP, &linkMap))																										\
					{																																									\
						libraryName = linkMap->l_name;																																	\
					}																																									\
					RHI_LOG(CRITICAL, "The renderer failed to locate the entry point \"%s\" within the OpenVR shared library \"%s\"", #funcName, libraryName)	\
					result = false;																																						\
				}																																										\
			}
	#else
		#error "Unsupported platform"
	#endif


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	OpenVRRuntimeLinking::OpenVRRuntimeLinking(IRenderer& renderer) :
		mRenderer(renderer),
		mOpenVRSharedLibrary(nullptr),
		mEntryPointsRegistered(false),
		mInitialized(false)
	{
		// Nothing here
	}

	OpenVRRuntimeLinking::~OpenVRRuntimeLinking()
	{
		// Destroy the shared library instances
		#ifdef _WIN32
			if (nullptr != mOpenVRSharedLibrary)
			{
				::FreeLibrary(static_cast<HMODULE>(mOpenVRSharedLibrary));
			}
		#elif defined LINUX
			if (nullptr != mOpenVRSharedLibrary)
			{
				::dlclose(mOpenVRSharedLibrary);
			}
		#else
			#error "Unsupported platform"
		#endif
	}

	bool OpenVRRuntimeLinking::isOpenVRAvaiable()
	{
		// Already initialized?
		if (!mInitialized)
		{
			// We're now initialized
			mInitialized = true;

			// Load the shared libraries
			if (loadSharedLibraries())
			{
				// Load the OpenVR entry points
				mEntryPointsRegistered = loadOpenVREntryPoints();
			}
		}

		// Entry points successfully registered?
		return mEntryPointsRegistered;
	}

	bool OpenVRRuntimeLinking::loadSharedLibraries()
	{
		// Load the shared library
		#ifdef _WIN32
			mOpenVRSharedLibrary = ::LoadLibraryExA("openvr_api.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
			if (nullptr == mOpenVRSharedLibrary)
			{
				RHI_LOG(COMPATIBILITY_WARNING, "The renderer failed to load in the shared OpenVR library \"openvr_api.dll\", OpenVR support disabled")
			}
		#elif defined LINUX
			mOpenVRSharedLibrary = ::dlopen("libopenvr_api.so", RTLD_NOW);
			if (nullptr == mOpenVRSharedLibrary)
			{
				RHI_LOG(COMPATIBILITY_WARNING, "The renderer failed to load in the shared OpenVR library \"libopenvr_api.so\", OpenVR support disabled")
			}
		#else
			#error "Unsupported platform"
		#endif

		// Done
		return (nullptr != mOpenVRSharedLibrary);
	}

	bool OpenVRRuntimeLinking::loadOpenVREntryPoints()
	{
		bool result = true;	// Success by default

		// Load the entry points
		using namespace vr;
		IMPORT_FUNC(VR_IsHmdPresent);
		IMPORT_FUNC(VR_IsRuntimeInstalled);
		IMPORT_FUNC(VR_RuntimePath);
		IMPORT_FUNC(VR_GetVRInitErrorAsSymbol);
		IMPORT_FUNC(VR_GetVRInitErrorAsEnglishDescription);
		IMPORT_FUNC(VR_GetGenericInterface);
		IMPORT_FUNC(VR_IsInterfaceVersionValid);
		IMPORT_FUNC(VR_GetInitToken);
		IMPORT_FUNC(VR_InitInternal2);
		IMPORT_FUNC(VR_ShutdownInternal);

		// Done
		return result;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
#endif // RENDERER_OPENVR

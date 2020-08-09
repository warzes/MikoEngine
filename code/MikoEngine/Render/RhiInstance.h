#pragma once

#include <Render/Rhi.h>

#ifdef RHI_NULL
[[nodiscard]] extern Rhi::IRhi* createNullRhiInstance(const Rhi::Context&);
#endif
#ifdef RHI_VULKAN
[[nodiscard]] extern Rhi::IRhi* createVulkanRhiInstance(const Rhi::Context&);
#endif
#ifdef RHI_OPENGL
[[nodiscard]] extern Rhi::IRhi* createOpenGLRhiInstance(const Rhi::Context&);
#endif
#ifdef RHI_OPENGLES3
[[nodiscard]] extern Rhi::IRhi* createOpenGLES3RhiInstance(const Rhi::Context&);
#endif
#ifdef RHI_DIRECT3D11
[[nodiscard]] extern Rhi::IRhi* createDirect3D11RhiInstance(const Rhi::Context&);
#endif
#ifdef RHI_DIRECT3D12
[[nodiscard]] extern Rhi::IRhi* createDirect3D12RhiInstance(const Rhi::Context&);
#endif

namespace Rhi
{
	class RhiInstance final
	{
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhiName
		*    Case sensitive ASCII name of the RHI to instance, must be valid. Usually "Rhi::DEFAULT_RHI_NAME".
		*    Example RHI names: "Null", "Vulkan", "OpenGL", "OpenGLES3", "Direct3D9", "Direct3D10", "Direct3D11", "Direct3D12"
		*  @param[in] context
		*    RHI context, the RHI context instance must stay valid as long as the RHI instance exists
		*  @param[in] loadRhiApiSharedLibrary
		*    Indicates if the RHI instance should load the RHI API shared library (true) or not (false, default)
		*/
		RhiInstance(const char* rhiName, Context& context, bool loadRhiApiSharedLibrary = false)
		{
			// In order to keep it simple in this test project the supported RHI implementations are
			// fixed typed in. For a real system a dynamic plugin system would be a good idea.
			if (loadRhiApiSharedLibrary)
			{
				// User wants us to load the RHI API shared library
				loadOpenGLSharedLibraryInternal(rhiName);
				context.setRhiApiSharedLibrary(mOpenGLSharedLibrary);
			}

#ifdef RHI_NULL
			if (0 == strcmp(rhiName, "Null"))
				mRhi = createNullRhiInstance(context);
#endif
#ifdef RHI_VULKAN
			if (0 == strcmp(rhiName, "Vulkan"))
				mRhi = createVulkanRhiInstance(context);
#endif
#ifdef RHI_OPENGL
			if (0 == strcmp(rhiName, "OpenGL"))
				mRhi = createOpenGLRhiInstance(context);
#endif
#ifdef RHI_OPENGLES3
			if (0 == strcmp(rhiName, "OpenGLES3"))
				mRhi = createOpenGLES3RhiInstance(context);
#endif
#ifdef RHI_DIRECT3D11
			if (0 == strcmp(rhiName, "Direct3D11"))
				mRhi = createDirect3D11RhiInstance(context);
#endif
#ifdef RHI_DIRECT3D12
			if (0 == strcmp(rhiName, "Direct3D12"))
				mRhi = createDirect3D12RhiInstance(context);
#endif
		}

		~RhiInstance()
		{
			mRhi = nullptr;

#ifdef _WIN32
			if (nullptr != mOpenGLSharedLibrary)
				::FreeLibrary(static_cast<HMODULE>(mOpenGLSharedLibrary));
#elif defined LINUX
			if (nullptr != mOpenGLSharedLibrary)
				::dlclose(mOpenGLSharedLibrary);
#endif
		}

		[[nodiscard]] inline IRhi* getRhi() const
		{
			return mRhi;
		}

		inline void destroyRhi()
		{
			mRhi = nullptr;
		}
	private:
		void loadOpenGLSharedLibraryInternal([[maybe_unused]] const char* rhiName)
		{
			// TODO(sw) Currently this is only needed for OpenGL (libGL.so) under Linux. This interacts with the library libX11.
#ifdef LINUX
// Under Linux the OpenGL library (libGL.so) registers callbacks in libX11 when loaded, which gets called on XCloseDisplay
// When the OpenGL library gets unloaded before the XCloseDisplay call then the X11 library wants to call the callbacks registered by the OpenGL library -> crash
// So we load it here. The user must make sure that an instance of this class gets destroyed after XCloseDisplay was called
// See http://dri.sourceforge.net/doc/DRIuserguide.html "11.5 libGL.so and dlopen()"
			if (0 == strcmp(rhiName, "OpenGL"))
			{
				mOpenGLSharedLibrary = ::dlopen("libGL.so", RTLD_NOW | RTLD_GLOBAL);
			}
#endif
		}
	private:
		void* mRhiSharedLibrary = nullptr;    // Shared RHI library, can be a null pointer
		IRhiPtr mRhi;                         // RHI instance, can be a null pointer
		void* mOpenGLSharedLibrary = nullptr; // Shared OpenGL library, can be a null pointer
	};
} // Rhi
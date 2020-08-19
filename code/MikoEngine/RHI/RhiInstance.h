#pragma once

#include "RHI/Rhi.h"

#if SE_RHINULL
[[nodiscard]] extern Rhi::IRhi* createNullRhiInstance(const Rhi::Context&);
#endif
#if SE_VULKAN
[[nodiscard]] extern Rhi::IRhi* createVulkanRhiInstance(const Rhi::Context&);
#endif
#if SE_OPENGL
[[nodiscard]] extern Rhi::IRhi* createOpenGLRhiInstance(const Rhi::Context&);
#endif
#if SE_OPENGLES
[[nodiscard]] extern Rhi::IRhi* createOpenGLES3RhiInstance(const Rhi::Context&);
#endif
#if SE_DIRECT3D11
[[nodiscard]] extern Rhi::IRhi* createDirect3D11RhiInstance(const Rhi::Context&);
#endif
#if SE_DIRECT3D12
[[nodiscard]] extern Rhi::IRhi* createDirect3D12RhiInstance(const Rhi::Context&);
#endif

namespace Rhi
{
	class RhiInstance final
	{
	public:
		RhiInstance(const char* rhiName, Context& context)
		{
#if SE_RHINULL
			if (0 == strcmp(rhiName, "Null"))
				mRhi = createNullRhiInstance(context);
#endif
#if SE_VULKAN
			if (0 == strcmp(rhiName, "Vulkan"))
				mRhi = createVulkanRhiInstance(context);
#endif
#if SE_OPENGL
			if (0 == strcmp(rhiName, "OpenGL"))
				mRhi = createOpenGLRhiInstance(context);
#endif
#if SE_OPENGLES
			if (0 == strcmp(rhiName, "OpenGLES3"))
				mRhi = createOpenGLES3RhiInstance(context);
#endif
#if SE_DIRECT3D11
			if (0 == strcmp(rhiName, "Direct3D11"))
				mRhi = createDirect3D11RhiInstance(context);
#endif
#if SE_DIRECT3D12
			if (0 == strcmp(rhiName, "Direct3D12"))
				mRhi = createDirect3D12RhiInstance(context);
#endif
		}

		~RhiInstance()
		{
			mRhi = nullptr;
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
		IRhiPtr mRhi;                         // RHI instance, can be a null pointer
	};
} // Rhi
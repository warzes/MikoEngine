#pragma once

namespace Rhi
{
	class IRenderPass : public IResource
	{
	public:
		inline virtual ~IRenderPass() override
		{
#if SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfRenderPasses;
#endif
		}

	protected:
		inline explicit IRenderPass(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::RENDER_PASS, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedRenderPasses;
			++rhi.getStatistics().currentNumberOfRenderPasses;
#endif
		}

		explicit IRenderPass(const IRenderPass&) = delete;
		IRenderPass& operator=(const IRenderPass&) = delete;

	};

	typedef SmartRefCount<IRenderPass> IRenderPassPtr;
} // namespace Rhi
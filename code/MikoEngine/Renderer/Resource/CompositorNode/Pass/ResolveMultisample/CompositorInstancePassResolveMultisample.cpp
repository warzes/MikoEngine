#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/ResolveMultisample/CompositorInstancePassResolveMultisample.h"
#include "Renderer/Resource/CompositorNode/Pass/ResolveMultisample/CompositorResourcePassResolveMultisample.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceResourceManager.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
#include "Renderer/Core/Renderer/FramebufferManager.h"
#include "Renderer/Core/IProfiler.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Context.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassResolveMultisample::onFillCommandBuffer(const Rhi::IRenderTarget* renderTarget, const CompositorContextData&, Rhi::CommandBuffer& commandBuffer)
	{
		const IRenderer& renderer = getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer();

		// Sanity check
		RHI_ASSERT(nullptr != renderTarget, "The resolve multisample compositor instance pass needs a valid render target")

		// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
		RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(renderer.getContext(), commandBuffer, getCompositorResourcePass().getDebugName())

		// Resolve
		Rhi::IFramebuffer* framebuffer = renderer.getCompositorWorkspaceResourceManager().getFramebufferManager().getFramebufferByCompositorFramebufferId(static_cast<const CompositorResourcePassResolveMultisample&>(getCompositorResourcePass()).getSourceMultisampleCompositorFramebufferId());
		if (nullptr != framebuffer)
		{
			// TODO(co) Get rid of the evil const-cast
			Rhi::Command::ResolveMultisampleFramebuffer::create(commandBuffer, const_cast<Rhi::IRenderTarget&>(*renderTarget), *framebuffer);
		}
		else
		{
			// Error!
			RHI_ASSERT(false, "We should never end up in here")
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorInstancePassResolveMultisample::CompositorInstancePassResolveMultisample(const CompositorResourcePassResolveMultisample& compositorResourcePassResolveMultisample, const CompositorNodeInstance& compositorNodeInstance) :
		ICompositorInstancePass(compositorResourcePassResolveMultisample, compositorNodeInstance)
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

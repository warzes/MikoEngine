#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/Scene/CompositorInstancePassScene.h"
#include "Renderer/Resource/CompositorNode/Pass/Scene/CompositorResourcePassScene.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
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
	void CompositorInstancePassScene::onCompositorWorkspaceInstanceLoadingFinished()
	{
		// Cache render queue index range instance, we know it must exist when we're in here
		mRenderQueueIndexRange = getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderQueueIndexRangeByRenderQueueIndex(mRenderQueue.getMinimumRenderQueueIndex());
		RHI_ASSERT(nullptr != mRenderQueueIndexRange, "Invalid render queue index range")
		RHI_ASSERT(mRenderQueueIndexRange->minimumRenderQueueIndex <= mRenderQueue.getMinimumRenderQueueIndex(), "Invalid minimum render queue index")
		RHI_ASSERT(mRenderQueueIndexRange->maximumRenderQueueIndex >= mRenderQueue.getMaximumRenderQueueIndex(), "Invalid maximum render queue index")
	}

	void CompositorInstancePassScene::onFillCommandBuffer(const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer)
	{
		// Sanity check
		RHI_ASSERT(nullptr != renderTarget, "The scene compositor instance pass needs a valid render target")

		// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
		RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer().getContext(), commandBuffer, getCompositorResourcePass().getDebugName())

		// Fill command buffer
		RHI_ASSERT(nullptr != mRenderQueueIndexRange, "Invalid render queue index range")
		const MaterialTechniqueId materialTechniqueId = static_cast<const CompositorResourcePassScene&>(getCompositorResourcePass()).getMaterialTechniqueId();
		for (const RenderableManager* renderableManager : mRenderQueueIndexRange->renderableManagers)
		{
			// The render queue index range covered by this compositor instance pass scene might be smaller than the range of the
			// cached render queue index range. So, we could add a range check in here to reject renderable managers, but it's not
			// really worth to do so since the render queue only considers renderables inside the render queue range anyway.
			mRenderQueue.addRenderablesFromRenderableManager(*renderableManager, materialTechniqueId, compositorContextData);
		}
		if (mRenderQueue.getNumberOfDrawCalls() > 0)
		{
			mRenderQueue.fillGraphicsCommandBuffer(*renderTarget, compositorContextData, commandBuffer);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	CompositorInstancePassScene::CompositorInstancePassScene(const CompositorResourcePassScene& compositorResourcePassScene, const CompositorNodeInstance& compositorNodeInstance, bool positionOnlyPass) :
		ICompositorInstancePass(compositorResourcePassScene, compositorNodeInstance),
		mRenderQueue(compositorNodeInstance.getCompositorWorkspaceInstance().getRenderer().getMaterialBlueprintResourceManager().getIndirectBufferManager(), compositorResourcePassScene.getMinimumRenderQueueIndex(), compositorResourcePassScene.getMaximumRenderQueueIndex(), positionOnlyPass, compositorResourcePassScene.isTransparentPass(), true),
		mRenderQueueIndexRange(nullptr)
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

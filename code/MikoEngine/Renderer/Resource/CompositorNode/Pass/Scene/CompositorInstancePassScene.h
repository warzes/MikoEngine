#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/CompositorNode/Pass/ICompositorInstancePass.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
#include "Renderer/RenderQueue/RenderQueue.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class CompositorResourcePassScene;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorInstancePassScene : public ICompositorInstancePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onCompositorWorkspaceInstanceLoadingFinished() override;
		virtual void onFillCommandBuffer(const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer) override;

		inline virtual void onPostCommandBufferExecution() override
		{
			// Directly clear the render queue as soon as the frame rendering has been finished to avoid evil dangling pointers
			mRenderQueue.clear();
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		CompositorInstancePassScene(const CompositorResourcePassScene& compositorResourcePassScene, const CompositorNodeInstance& compositorNodeInstance, bool positionOnlyPass = false);

		inline virtual ~CompositorInstancePassScene() override
		{
			// Nothing here
		}

		explicit CompositorInstancePassScene(const CompositorInstancePassScene&) = delete;
		CompositorInstancePassScene& operator=(const CompositorInstancePassScene&) = delete;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		RenderQueue												  mRenderQueue;
		const CompositorWorkspaceInstance::RenderQueueIndexRange* mRenderQueueIndexRange;	// Cached render queue index range instance, can be a null pointer, don't destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

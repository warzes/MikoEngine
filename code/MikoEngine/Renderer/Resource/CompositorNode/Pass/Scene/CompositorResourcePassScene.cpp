#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/Scene/CompositorResourcePassScene.h"
#include "Renderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	void CompositorResourcePassScene::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		SE_ASSERT(sizeof(v1CompositorNode::PassScene) == numberOfBytes, "Invalid number of bytes")

		// Call the base implementation
		ICompositorResourcePass::deserialize(sizeof(v1CompositorNode::Pass), data);

		// Read data
		const v1CompositorNode::PassScene* passScene = reinterpret_cast<const v1CompositorNode::PassScene*>(data);
		mMinimumRenderQueueIndex = passScene->minimumRenderQueueIndex;
		mMaximumRenderQueueIndex = passScene->maximumRenderQueueIndex;
		mTransparentPass		 = passScene->transparentPass;
		mMaterialTechniqueId	 = passScene->materialTechniqueId;

		// Sanity check
		SE_ASSERT(mMaximumRenderQueueIndex >= mMinimumRenderQueueIndex, "Invalid maximum render queue index")
	}

	bool CompositorResourcePassScene::getRenderQueueIndexRange(uint8_t& minimumRenderQueueIndex, uint8_t& maximumRenderQueueIndex) const
	{
		// This compositor resource pass has a render queue range defined
		minimumRenderQueueIndex = mMinimumRenderQueueIndex;
		maximumRenderQueueIndex = mMaximumRenderQueueIndex;
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

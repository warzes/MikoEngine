#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/Copy/CompositorResourcePassCopy.h"
#include "Renderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	void CompositorResourcePassCopy::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		SE_ASSERT(sizeof(v1CompositorNode::PassCopy) == numberOfBytes, "Invalid number of bytes")

		// Call the base implementation
		ICompositorResourcePass::deserialize(sizeof(v1CompositorNode::Pass), data);

		// Read data
		const v1CompositorNode::PassCopy* passCopy = reinterpret_cast<const v1CompositorNode::PassCopy*>(data);
		mDestinationTextureAssetId = passCopy->destinationTextureAssetId;
		mSourceTextureAssetId = passCopy->sourceTextureAssetId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

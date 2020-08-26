#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/Clear/CompositorResourcePassClear.h"
#include "Renderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	void CompositorResourcePassClear::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		SE_ASSERT(sizeof(v1CompositorNode::PassClear) == numberOfBytes, "Invalid number of bytes")

		// Call the base implementation
		ICompositorResourcePass::deserialize(sizeof(v1CompositorNode::Pass), data);

		// Read data
		const v1CompositorNode::PassClear* passClear = reinterpret_cast<const v1CompositorNode::PassClear*>(data);
		mFlags = passClear->flags;
		memcpy(glm::value_ptr(mColor), passClear->color, sizeof(float) * 4);
		mZ = passClear->z;
		mStencil = passClear->stencil;

		// Sanity check
		SE_ASSERT(0 != mFlags, "The clear compositor resource pass flags must not be null")
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

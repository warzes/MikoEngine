#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/VrHiddenAreaMesh/CompositorResourcePassVrHiddenAreaMesh.h"
#include "Renderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	void CompositorResourcePassVrHiddenAreaMesh::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		RHI_ASSERT(sizeof(v1CompositorNode::PassVrHiddenAreaMesh) == numberOfBytes, "Invalid number of bytes")

		// Call the base implementation
		ICompositorResourcePass::deserialize(sizeof(v1CompositorNode::Pass), data);

		// Read data
		const v1CompositorNode::PassVrHiddenAreaMesh* passVrHiddenAreaMesh = reinterpret_cast<const v1CompositorNode::PassVrHiddenAreaMesh*>(data);
		mFlags = passVrHiddenAreaMesh->flags;
		mStencil = passVrHiddenAreaMesh->stencil;

		// Sanity checks
		RHI_ASSERT(0 != mFlags, "The VR hidden area mesh compositor resource pass flags must not be null")
		RHI_ASSERT((mFlags & Rhi::ClearFlag::COLOR) == 0, "The VR hidden area mesh compositor resource pass doesn't support the color flag")
		RHI_ASSERT((mFlags & Rhi::ClearFlag::STENCIL) == 0, "TODO(co) The VR hidden area mesh compositor resource pass doesn't support the stencil flag, yet")
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

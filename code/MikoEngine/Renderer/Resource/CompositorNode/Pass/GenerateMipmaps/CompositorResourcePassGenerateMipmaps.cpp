#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/GenerateMipmaps/CompositorResourcePassGenerateMipmaps.h"
#include "Renderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	void CompositorResourcePassGenerateMipmaps::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		RHI_ASSERT(sizeof(v1CompositorNode::PassGenerateMipmaps) == numberOfBytes, "Invalid number of bytes")

		// Call the base implementation
		ICompositorResourcePass::deserialize(sizeof(v1CompositorNode::Pass), data);

		// Read data
		const v1CompositorNode::PassGenerateMipmaps* passGenerateMipmaps = reinterpret_cast<const v1CompositorNode::PassGenerateMipmaps*>(data);
		mTextureAssetId = passGenerateMipmaps->textureAssetId;
		mMaterialBlueprintAssetId = passGenerateMipmaps->materialBlueprintAssetId;
		mTextureMaterialBlueprintProperty = passGenerateMipmaps->textureMaterialBlueprintProperty;

		// Sanity check
		RHI_ASSERT((IsInvalid(mMaterialBlueprintAssetId) && IsInvalid(mTextureMaterialBlueprintProperty)) || (IsValid(mMaterialBlueprintAssetId) && IsValid(mTextureMaterialBlueprintProperty)), "Invalid material blueprint asset")
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

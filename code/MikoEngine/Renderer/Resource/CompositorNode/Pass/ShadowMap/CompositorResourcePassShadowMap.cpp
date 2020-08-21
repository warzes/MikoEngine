#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/ShadowMap/CompositorResourcePassShadowMap.h"
#include "Renderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	void CompositorResourcePassShadowMap::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		RHI_ASSERT(sizeof(v1CompositorNode::PassShadowMap) == numberOfBytes, "Invalid number of bytes")

		// Call the base implementation
		CompositorResourcePassScene::deserialize(sizeof(v1CompositorNode::PassScene), data);

		// Read data
		const v1CompositorNode::PassShadowMap* passShadowMap = reinterpret_cast<const v1CompositorNode::PassShadowMap*>(data);
		mTextureAssetId = passShadowMap->textureAssetId;
		mDepthToExponentialVarianceMaterialBlueprintAssetId = passShadowMap->depthToExponentialVarianceMaterialBlueprintAssetId;
		mBlurMaterialBlueprintAssetId = passShadowMap->blurMaterialBlueprintAssetId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

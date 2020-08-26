#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Utility/StringId.h"
#include "Renderer/Core/Renderer/RenderTargetTextureSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;	// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compositor render target texture; used as part of compositor framebuffers
	*/
	class CompositorRenderTargetTexture final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline CompositorRenderTargetTexture(AssetId assetId, const RenderTargetTextureSignature& renderTargetTextureSignature) :
			mAssetId(assetId),
			mRenderTargetTextureSignature(renderTargetTextureSignature)
		{
			// Nothing here
		}

		inline explicit CompositorRenderTargetTexture(const CompositorRenderTargetTexture& compositorRenderTargetTexture) :
			mAssetId(compositorRenderTargetTexture.mAssetId),
			mRenderTargetTextureSignature(compositorRenderTargetTexture.mRenderTargetTextureSignature)
		{
			// Nothing here
		}

		inline ~CompositorRenderTargetTexture()
		{
			// Nothing here
		}

		[[nodiscard]] inline AssetId getAssetId() const
		{
			return mAssetId;
		}

		[[nodiscard]] inline const RenderTargetTextureSignature& getRenderTargetTextureSignature() const
		{
			return mRenderTargetTextureSignature;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		CompositorRenderTargetTexture& operator=(const CompositorRenderTargetTexture& compositorRenderTargetTexture) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetId						 mAssetId;
		RenderTargetTextureSignature mRenderTargetTextureSignature;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#pragma once

#include "Renderer/Core/Manager.h"
#include "Renderer/Core/StringId.h"
#include "Renderer/Core/Renderer/RenderTargetTextureSignature.h"

namespace Renderer
{
	class IRenderer;
}


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
	class RenderTargetTextureManager final : private Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct RenderTargetTextureElement final
		{
			AssetId						 assetId;
			RenderTargetTextureSignature renderTargetTextureSignature;
			Rhi::ITexture*				 texture;				// Can be a null pointer, no "Rhi::ITexturePtr" to not have overhead when internally reallocating
			uint32_t					 numberOfReferences;	// Number of texture references (don't misuse the RHI texture reference counter for this)

			inline RenderTargetTextureElement() :
				assetId(getInvalid<AssetId>()),
				texture(nullptr),
				numberOfReferences(0)
			{
				// Nothing here
			}

			inline explicit RenderTargetTextureElement(const RenderTargetTextureSignature& _renderTargetTextureSignature) :
				assetId(getInvalid<AssetId>()),
				renderTargetTextureSignature(_renderTargetTextureSignature),
				texture(nullptr),
				numberOfReferences(0)
			{
				// Nothing here
			}

			inline RenderTargetTextureElement(AssetId _assetId, const RenderTargetTextureSignature& _renderTargetTextureSignature) :
				assetId(_assetId),
				renderTargetTextureSignature(_renderTargetTextureSignature),
				texture(nullptr),
				numberOfReferences(0)
			{
				// Nothing here
			}

			inline RenderTargetTextureElement(AssetId _assetId, const RenderTargetTextureSignature& _renderTargetTextureSignature, Rhi::ITexture& _texture) :
				assetId(_assetId),
				renderTargetTextureSignature(_renderTargetTextureSignature),
				texture(&_texture),
				numberOfReferences(0)
			{
				// Nothing here
			}
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline explicit RenderTargetTextureManager(IRenderer& renderer) :
			mRenderer(renderer)
		{
			// Nothing here
		}

		inline ~RenderTargetTextureManager()
		{
			// Nothing here
		}

		explicit RenderTargetTextureManager(const RenderTargetTextureManager&) = delete;
		RenderTargetTextureManager& operator=(const RenderTargetTextureManager&) = delete;

		[[nodiscard]] inline IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		void clear();
		void clearRhiResources();
		void addRenderTargetTexture(AssetId assetId, const RenderTargetTextureSignature& renderTargetTextureSignature);
		[[nodiscard]] Rhi::ITexture* getTextureByAssetId(AssetId assetId, const Rhi::IRenderTarget& renderTarget, uint8_t numberOfMultisamples, float resolutionScale, const RenderTargetTextureSignature** outRenderTargetTextureSignature);
		void releaseRenderTargetTextureBySignature(const RenderTargetTextureSignature& renderTargetTextureSignature);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<RenderTargetTextureElement>						 SortedRenderTargetTextureVector;
		typedef std::unordered_map<uint32_t, RenderTargetTextureSignatureId> AssetIdToRenderTargetTextureSignatureId;	// Key = "Renderer::AssetId"
		typedef std::unordered_map<uint32_t, uint32_t>						 AssetIdToIndex;							// Key = "Renderer::AssetId"


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&								mRenderer;
		SortedRenderTargetTextureVector			mSortedRenderTargetTextureVector;
		AssetIdToRenderTargetTextureSignatureId	mAssetIdToRenderTargetTextureSignatureId;
		AssetIdToIndex							mAssetIdToIndex;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

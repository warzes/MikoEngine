#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/CompositorNode/Pass/Scene/CompositorResourcePassScene.h"


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
	*    Compositor resource pass shadow map
	*/
	class CompositorResourcePassShadowMap final : public CompositorResourcePassScene
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("ShadowMap");
		static constexpr uint32_t MAXIMUM_NUMBER_OF_SHADOW_CASCADES = 4;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline AssetId getTextureAssetId() const
		{
			return mTextureAssetId;
		}

		[[nodiscard]] inline AssetId getDepthToExponentialVarianceMaterialBlueprintAssetId() const
		{
			return mDepthToExponentialVarianceMaterialBlueprintAssetId;
		}

		[[nodiscard]] inline AssetId getBlurMaterialBlueprintAssetId() const
		{
			return mBlurMaterialBlueprintAssetId;
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual CompositorPassTypeId getTypeId() const override
		{
			return TYPE_ID;
		}

		virtual void deserialize(uint32_t numberOfBytes, const uint8_t* data) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit CompositorResourcePassShadowMap(const CompositorTarget& compositorTarget) :
			CompositorResourcePassScene(compositorTarget)
		{
			// Nothing here
		}

		inline virtual ~CompositorResourcePassShadowMap() override
		{
			// Nothing here
		}

		explicit CompositorResourcePassShadowMap(const CompositorResourcePassShadowMap&) = delete;
		CompositorResourcePassShadowMap& operator=(const CompositorResourcePassShadowMap&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetId	mTextureAssetId;										// Shadow map texture asset ID
		AssetId mDepthToExponentialVarianceMaterialBlueprintAssetId;	// Depth to exponential variance material blueprint asset ID
		AssetId mBlurMaterialBlueprintAssetId;							// Blur material blueprint asset ID


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

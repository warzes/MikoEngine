#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/CompositorNode/Pass/ICompositorResourcePass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;				// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef StringId MaterialPropertyId;	// Material property identifier, internally just a POD "uint32_t", result of hashing the property name


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorResourcePassGenerateMipmaps final : public ICompositorResourcePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("GenerateMipmaps");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline AssetId getTextureAssetId() const
		{
			return mTextureAssetId;
		}

		[[nodiscard]] inline AssetId getMaterialBlueprintAssetId() const
		{
			return mMaterialBlueprintAssetId;
		}

		[[nodiscard]] inline MaterialPropertyId getTextureMaterialBlueprintProperty() const
		{
			return mTextureMaterialBlueprintProperty;
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
		inline explicit CompositorResourcePassGenerateMipmaps(const CompositorTarget& compositorTarget) :
			ICompositorResourcePass(compositorTarget)
		{
			// Nothing here
		}

		inline virtual ~CompositorResourcePassGenerateMipmaps() override
		{
			// Nothing here
		}

		explicit CompositorResourcePassGenerateMipmaps(const CompositorResourcePassGenerateMipmaps&) = delete;
		CompositorResourcePassGenerateMipmaps& operator=(const CompositorResourcePassGenerateMipmaps&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetId			   mTextureAssetId;
		AssetId			   mMaterialBlueprintAssetId;
		MaterialPropertyId mTextureMaterialBlueprintProperty;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

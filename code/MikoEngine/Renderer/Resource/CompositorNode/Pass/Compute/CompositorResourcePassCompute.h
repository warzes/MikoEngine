#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Material/MaterialProperties.h"
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
	typedef uint32_t MaterialTechniqueId;	// Material technique identifier, result of hashing the material technique name via "Renderer::StringId"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compositor resource pass compute
	*
	*  @remarks
	*    A compositor resource pass compute instance is using a material resource for compute rendering. This material resource
	*    can be defined by providing an material asset ID. Since compositor material blueprints are usually highly
	*    specialized for a certain task, it would be annoying to have to define a material asset for each and every
	*    compositor material. So, it's also supported to define a material blueprint asset directly.
	*/
	class CompositorResourcePassCompute : public ICompositorResourcePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("Compute");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		CompositorResourcePassCompute(const CompositorTarget& compositorTarget, AssetId materialBlueprintAssetId, const MaterialProperties& materialProperties);

		inline virtual ~CompositorResourcePassCompute() override
		{
			// Nothing here
		}

		[[nodiscard]] inline bool isMaterialDefinitionMandatory() const
		{
			return mMaterialDefinitionMandatory;
		}

		[[nodiscard]] inline AssetId getMaterialAssetId() const
		{
			return mMaterialAssetId;
		}

		[[nodiscard]] inline MaterialTechniqueId getMaterialTechniqueId() const
		{
			return mMaterialTechniqueId;
		}

		[[nodiscard]] inline AssetId getMaterialBlueprintAssetId() const
		{
			return mMaterialBlueprintAssetId;
		}

		[[nodiscard]] inline const MaterialProperties& getMaterialProperties() const
		{
			return mMaterialProperties;
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
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline explicit CompositorResourcePassCompute(const CompositorTarget& compositorTarget) :
			ICompositorResourcePass(compositorTarget),
			mMaterialDefinitionMandatory(true),
			mMaterialTechniqueId(GetInvalid<MaterialTechniqueId>())
		{
			// Nothing here
		}

		inline CompositorResourcePassCompute(const CompositorTarget& compositorTarget, bool materialDefinitionMandatory) :
			ICompositorResourcePass(compositorTarget),
			mMaterialDefinitionMandatory(materialDefinitionMandatory),
			mMaterialTechniqueId(GetInvalid<MaterialTechniqueId>())
		{
			// Nothing here
		}

		explicit CompositorResourcePassCompute(const CompositorResourcePassCompute&) = delete;
		CompositorResourcePassCompute& operator=(const CompositorResourcePassCompute&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				mMaterialDefinitionMandatory;
		AssetId				mMaterialAssetId;			// If material blueprint asset ID is set, material asset ID must be invalid
		MaterialTechniqueId	mMaterialTechniqueId;		// Must always be valid
		AssetId				mMaterialBlueprintAssetId;	// If material asset ID is set, material blueprint asset ID must be invalid
		MaterialProperties	mMaterialProperties;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

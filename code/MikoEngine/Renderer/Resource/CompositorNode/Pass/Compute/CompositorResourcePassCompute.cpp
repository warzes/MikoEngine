#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/Compute/CompositorResourcePassCompute.h"
#include "Renderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CompositorResourcePassCompute::CompositorResourcePassCompute(const CompositorTarget& compositorTarget, AssetId materialBlueprintAssetId, const MaterialProperties& materialProperties) :
		ICompositorResourcePass(compositorTarget),
		mMaterialDefinitionMandatory(true),
		mMaterialTechniqueId(MaterialResourceManager::DEFAULT_MATERIAL_TECHNIQUE_ID),
		mMaterialBlueprintAssetId(materialBlueprintAssetId),
		mMaterialProperties(materialProperties)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	void CompositorResourcePassCompute::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		SE_ASSERT(sizeof(v1CompositorNode::PassCompute) <= numberOfBytes, "Invalid number of bytes")

		// Call the base implementation
		ICompositorResourcePass::deserialize(sizeof(v1CompositorNode::Pass), data);

		// Read data
		const v1CompositorNode::PassCompute* passCompute = reinterpret_cast<const v1CompositorNode::PassCompute*>(data);
		SE_ASSERT(sizeof(v1CompositorNode::PassCompute) + sizeof(MaterialProperty) * passCompute->numberOfMaterialProperties == numberOfBytes, "Invalid number of bytes")
		mMaterialAssetId = passCompute->materialAssetId;
		mMaterialTechniqueId = passCompute->materialTechniqueId;
		mMaterialBlueprintAssetId = passCompute->materialBlueprintAssetId;

		{ // Read material properties
			// TODO(co) Get rid of the evil const-cast
			MaterialProperties::SortedPropertyVector& sortedPropertyVector = const_cast<MaterialProperties::SortedPropertyVector&>(mMaterialProperties.getSortedPropertyVector());
			sortedPropertyVector.resize(passCompute->numberOfMaterialProperties);
			memcpy(reinterpret_cast<char*>(sortedPropertyVector.data()), data + sizeof(v1CompositorNode::PassCompute), sizeof(MaterialProperty) * passCompute->numberOfMaterialProperties);
		}

		// Sanity checks
		SE_ASSERT(!mMaterialDefinitionMandatory || IsValid(mMaterialAssetId) || IsValid(mMaterialBlueprintAssetId), "Invalid material asset")
		SE_ASSERT(!(IsValid(mMaterialAssetId) && IsValid(mMaterialBlueprintAssetId)), "Invalid material asset")
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

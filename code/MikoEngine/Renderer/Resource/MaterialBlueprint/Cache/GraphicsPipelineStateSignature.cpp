#include "stdafx.h"
#include "Renderer/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateSignature.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "Renderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "Renderer/Resource/ShaderBlueprint/ShaderBlueprintResource.h"
#include "Renderer/Core/Math/Math.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	ShaderCombinationId GraphicsPipelineStateSignature::generateShaderCombinationId(const ShaderBlueprintResource& shaderBlueprintResource, const ShaderProperties& shaderProperties)
	{
		ShaderCombinationId shaderCombinationId(Math::FNV1a_INITIAL_HASH_32);

		{ // Apply shader blueprint resource ID
			const ShaderBlueprintResourceId shaderBlueprintResourceId = shaderBlueprintResource.getId();
			shaderCombinationId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&shaderBlueprintResourceId), sizeof(uint32_t), shaderCombinationId);
		}

		// Apply shader properties
		const ShaderProperties& referencedShaderProperties = shaderBlueprintResource.getReferencedShaderProperties();
		for (const ShaderProperties::Property& property : shaderProperties.getSortedPropertyVector())
		{
			// Use the additional information to reduce the shader properties in order to generate fewer combinations
			const ShaderPropertyId shaderPropertyId = property.shaderPropertyId;
			if (referencedShaderProperties.hasPropertyValue(shaderPropertyId))
			{
				// No need to check for zero-value shader properties in here, already optimized out by "Renderer::MaterialBlueprintResource::optimizeShaderProperties()"

				// Apply shader property ID
				shaderCombinationId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&shaderPropertyId), sizeof(uint32_t), shaderCombinationId);

				// Apply shader property value
				shaderCombinationId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&property.value), sizeof(int32_t), shaderCombinationId);
			}
		}

		// Done
		return shaderCombinationId;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GraphicsPipelineStateSignature::GraphicsPipelineStateSignature(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature) :
		mMaterialBlueprintResourceId(graphicsPipelineStateSignature.mMaterialBlueprintResourceId),
		mSerializedGraphicsPipelineStateHash(graphicsPipelineStateSignature.mSerializedGraphicsPipelineStateHash),
		mShaderProperties(graphicsPipelineStateSignature.mShaderProperties),
		mGraphicsPipelineStateSignatureId(graphicsPipelineStateSignature.mGraphicsPipelineStateSignatureId),
		mShaderCombinationId{graphicsPipelineStateSignature.mShaderCombinationId[0], graphicsPipelineStateSignature.mShaderCombinationId[1], graphicsPipelineStateSignature.mShaderCombinationId[2], graphicsPipelineStateSignature.mShaderCombinationId[3], graphicsPipelineStateSignature.mShaderCombinationId[4]}
	{
		// Nothing here
	}

	GraphicsPipelineStateSignature& GraphicsPipelineStateSignature::operator=(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature)
	{
		mMaterialBlueprintResourceId = graphicsPipelineStateSignature.mMaterialBlueprintResourceId;
		mSerializedGraphicsPipelineStateHash = graphicsPipelineStateSignature.mSerializedGraphicsPipelineStateHash;
		mShaderProperties = graphicsPipelineStateSignature.mShaderProperties;
		mGraphicsPipelineStateSignatureId = graphicsPipelineStateSignature.mGraphicsPipelineStateSignatureId;
		for (uint8_t i = 0; i < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++i)
		{
			mShaderCombinationId[i] = graphicsPipelineStateSignature.mShaderCombinationId[i];
		}

		// Done
		return *this;
	}

	void GraphicsPipelineStateSignature::set(const MaterialBlueprintResource& materialBlueprintResource, uint32_t serializedGraphicsPipelineStateHash, const ShaderProperties& shaderProperties)
	{
		mMaterialBlueprintResourceId		 = materialBlueprintResource.getId();
		mSerializedGraphicsPipelineStateHash = serializedGraphicsPipelineStateHash;
		mShaderProperties					 = shaderProperties;
		mGraphicsPipelineStateSignatureId	 = Math::FNV1a_INITIAL_HASH_32;
		for (uint8_t i = 0; i < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++i)
		{
			mShaderCombinationId[i] = getInvalid<ShaderCombinationId>();
		}

		// Incorporate primitive hashes
		mGraphicsPipelineStateSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mMaterialBlueprintResourceId), sizeof(uint32_t), mGraphicsPipelineStateSignatureId);
		mGraphicsPipelineStateSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&mSerializedGraphicsPipelineStateHash), sizeof(uint32_t), mGraphicsPipelineStateSignatureId);

		// Incorporate shader related hashes
		const ShaderBlueprintResourceManager& shaderBlueprintResourceManager = materialBlueprintResource.getResourceManager<MaterialBlueprintResourceManager>().getRenderer().getShaderBlueprintResourceManager();
		for (uint8_t i = 0; i < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++i)
		{
			const ShaderBlueprintResource* shaderBlueprintResource = shaderBlueprintResourceManager.tryGetById(materialBlueprintResource.getGraphicsShaderBlueprintResourceId(static_cast<GraphicsShaderType>(i)));
			if (nullptr != shaderBlueprintResource)
			{
				const uint32_t hash = mShaderCombinationId[i] = generateShaderCombinationId(*shaderBlueprintResource, mShaderProperties);
				mGraphicsPipelineStateSignatureId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&hash), sizeof(uint32_t), mGraphicsPipelineStateSignatureId);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

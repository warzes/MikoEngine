#include "stdafx.h"
#include "Renderer/Resource/Material/MaterialTechnique.h"
#include "Renderer/Resource/Material/MaterialResource.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/MaterialBufferManager.h"
#include "Renderer/Resource/Texture/TextureResourceManager.h"
#include "Renderer/Resource/Texture/TextureResource.h"
#include "Renderer/Resource/RendererResourceManager.h"
#include "Math/Math.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		#define DEFINE_CONSTANT(name) static constexpr uint32_t name = SE_STRING_ID(#name);
			DEFINE_CONSTANT(CullMode)
			DEFINE_CONSTANT(AlphaToCoverageEnable)
		#undef DEFINE_CONSTANT


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MaterialTechnique::MaterialTechnique(MaterialTechniqueId materialTechniqueId, MaterialResource& materialResource, MaterialBlueprintResourceId materialBlueprintResourceId) :
		MaterialBufferSlot(materialResource),
		mMaterialTechniqueId(materialTechniqueId),
		mMaterialBlueprintResourceId(materialBlueprintResourceId),
		mSerializedGraphicsPipelineStateHash(GetInvalid<uint32_t>())
	{
		MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
		if (nullptr != materialBufferManager)
		{
			materialBufferManager->requestSlot(*this);
		}

		// Calculate FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
		calculateSerializedGraphicsPipelineStateHash();
	}

	MaterialTechnique::~MaterialTechnique()
	{
		// Due to hot-reloading it's possible that there's no assigned material slot, so we need to do a check here
		if (IsValid(getAssignedMaterialSlot()))
		{
			MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
			SE_ASSERT(nullptr != materialBufferManager, "Invalid material buffer manager")
			materialBufferManager->releaseSlot(*this);
		}
	}

	void MaterialTechnique::fillGraphicsCommandBuffer(const IRenderer& renderer, Rhi::CommandBuffer& commandBuffer, uint32_t& resourceGroupRootParameterIndex, Rhi::IResourceGroup** resourceGroup)
	{
		// Sanity check
		SE_ASSERT(IsValid(mMaterialBlueprintResourceId), "Invalid material blueprint resource ID")
		SE_ASSERT(nullptr != resourceGroup, "The RHI resource group pointer must be valid")

		{ // Bind the material buffer manager
			MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
			if (nullptr != materialBufferManager)
			{
				materialBufferManager->fillGraphicsCommandBuffer(*this, commandBuffer);
			}
		}

		// Set resource group
		fillCommandBuffer(renderer, resourceGroupRootParameterIndex, resourceGroup);
	}

	void MaterialTechnique::fillComputeCommandBuffer(const IRenderer& renderer, Rhi::CommandBuffer& commandBuffer, uint32_t& resourceGroupRootParameterIndex, Rhi::IResourceGroup** resourceGroup)
	{
		// Sanity check
		SE_ASSERT(IsValid(mMaterialBlueprintResourceId), "Invalid material blueprint resource ID")
		SE_ASSERT(nullptr != resourceGroup, "The RHI resource group pointer must be valid")

		{ // Bind the material buffer manager
			MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
			if (nullptr != materialBufferManager)
			{
				materialBufferManager->fillComputeCommandBuffer(*this, commandBuffer);
			}
		}

		// Set resource group
		fillCommandBuffer(renderer, resourceGroupRootParameterIndex, resourceGroup);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	void MaterialTechnique::onLoadingStateChange(const Renderer::IResource&)
	{
		makeResourceGroupDirty();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MaterialBufferManager* MaterialTechnique::getMaterialBufferManager() const
	{
		// It's valid if a material blueprint resource doesn't contain a material uniform buffer (usually the case for compositor material blueprint resources)
		const MaterialBlueprintResource* materialBlueprintResource = getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
		return (nullptr != materialBlueprintResource) ? materialBlueprintResource->getMaterialBufferManager() : nullptr;
	}

	const MaterialTechnique::Textures& MaterialTechnique::getTextures(const IRenderer& renderer)
	{
		// Need for gathering the textures now?
		if (mTextures.empty())
		{
			const MaterialBlueprintResource* materialBlueprintResource = renderer.getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
			if (nullptr != materialBlueprintResource)
			{
				const MaterialResource& materialResource = getMaterialResource();
				TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
				const MaterialBlueprintResource::Textures& textures = materialBlueprintResource->getTextures();
				const size_t numberOfTextures = textures.size();
				mTextures.reserve(numberOfTextures);
				for (size_t i = 0; i < numberOfTextures; ++i)
				{
					const MaterialBlueprintResource::Texture& blueprintTexture = textures[i];

					// Start with the material blueprint textures
					Texture texture;
					texture.rootParameterIndex = blueprintTexture.rootParameterIndex;
					texture.materialProperty   = blueprintTexture.materialProperty;
					texture.textureResourceId  = blueprintTexture.textureResourceId;

					// Apply material specific modifications
					const MaterialPropertyId materialPropertyId = texture.materialProperty.getMaterialPropertyId();
					if (IsValid(materialPropertyId))
					{
						// Figure out the material property value
						const MaterialProperty* materialProperty = materialResource.getPropertyById(materialPropertyId);
						if (nullptr != materialProperty)
						{
							// TODO(co) Error handling: Usage mismatch etc.
							texture.materialProperty = *materialProperty;
							textureResourceManager.loadTextureResourceByAssetId(texture.materialProperty.getTextureAssetIdValue(), blueprintTexture.fallbackTextureAssetId, texture.textureResourceId, this, blueprintTexture.rgbHardwareGammaCorrection);
						}
					}

					// Insert texture
					mTextures.push_back(texture);
				}
			}
		}
		return mTextures;
	}

	void MaterialTechnique::calculateSerializedGraphicsPipelineStateHash()
	{
		const MaterialBlueprintResource* materialBlueprintResource = getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
		if (nullptr != materialBlueprintResource)
		{
			// Start with the graphics pipeline state of the material blueprint resource
			Rhi::SerializedGraphicsPipelineState serializedGraphicsPipelineState = materialBlueprintResource->getGraphicsPipelineState();

			// Apply material properties
			// -> Renderer toolkit counterpart is "RendererToolkit::JsonMaterialBlueprintHelper::readPipelineStateObject()"
			const MaterialProperties& materialBlueprintMaterialProperties = materialBlueprintResource->getMaterialProperties();
			const MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector = getMaterialResource().getSortedPropertyVector();
			const size_t numberOfMaterialProperties = sortedMaterialPropertyVector.size();
			for (size_t i = 0; i < numberOfMaterialProperties; ++i)
			{
				// A material can have multiple material techniques, do only apply material properties which are known to the material blueprint resource
				const MaterialProperty& materialProperty = sortedMaterialPropertyVector[i];
				if (nullptr != materialBlueprintMaterialProperties.getPropertyById(materialProperty.getMaterialPropertyId()))
				{
					switch (materialProperty.getUsage())
					{
						case MaterialProperty::Usage::UNKNOWN:
						case MaterialProperty::Usage::STATIC:
						case MaterialProperty::Usage::SHADER_UNIFORM:
						case MaterialProperty::Usage::SHADER_COMBINATION:
							// Nothing here
							break;

						case MaterialProperty::Usage::RASTERIZER_STATE:
							// TODO(co) Implement all rasterizer state properties
							if (materialProperty.getMaterialPropertyId() == ::detail::CullMode)
							{
								serializedGraphicsPipelineState.rasterizerState.cullMode = materialProperty.getCullModeValue();
							}
							break;

						case MaterialProperty::Usage::DEPTH_STENCIL_STATE:
							// TODO(co) Implement all depth stencil state properties
							break;

						case MaterialProperty::Usage::BLEND_STATE:
							// TODO(co) Implement all blend state properties
							if (materialProperty.getMaterialPropertyId() == ::detail::AlphaToCoverageEnable)
							{
								serializedGraphicsPipelineState.blendState.alphaToCoverageEnable = materialProperty.getBooleanValue();
							}
							break;

						case MaterialProperty::Usage::SAMPLER_STATE:
						case MaterialProperty::Usage::TEXTURE_REFERENCE:
						case MaterialProperty::Usage::GLOBAL_REFERENCE:
						case MaterialProperty::Usage::UNKNOWN_REFERENCE:
						case MaterialProperty::Usage::PASS_REFERENCE:
						case MaterialProperty::Usage::MATERIAL_REFERENCE:
						case MaterialProperty::Usage::INSTANCE_REFERENCE:
						case MaterialProperty::Usage::GLOBAL_REFERENCE_FALLBACK:
							// Nothing here
							break;
					}
				}
			}

			// Calculate the FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
			mSerializedGraphicsPipelineStateHash = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&serializedGraphicsPipelineState), sizeof(Rhi::SerializedGraphicsPipelineState));

			// Register the FNV1a hash of "Rhi::SerializedGraphicsPipelineState" inside the material blueprint resource manager so it's sufficient to pass around the tiny hash instead the over 400 bytes full serialized pipeline state
			getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().addSerializedGraphicsPipelineState(mSerializedGraphicsPipelineStateHash, serializedGraphicsPipelineState);
		}
		else
		{
			SetInvalid(mSerializedGraphicsPipelineStateHash);
		}
	}

	void MaterialTechnique::scheduleForShaderUniformUpdate()
	{
		MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
		if (nullptr != materialBufferManager)
		{
			materialBufferManager->scheduleForUpdate(*this);
		}
	}

	void MaterialTechnique::fillCommandBuffer(const IRenderer& renderer, uint32_t& resourceGroupRootParameterIndex, Rhi::IResourceGroup** resourceGroup)
	{
		// Set textures
		const Textures& textures = getTextures(renderer);
		if (textures.empty())
		{
			SetInvalid(resourceGroupRootParameterIndex);
			*resourceGroup = nullptr;
		}
		else
		{
			// Create resource group, if needed
			if (nullptr == mResourceGroup)
			{
				// Check texture resources
				const size_t numberOfTextures = textures.size();
				const TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
				for (size_t i = 0; i < numberOfTextures; ++i)
				{
					// Due to background texture loading, some textures might not be ready, yet
					// -> But even in this situation there should be a decent fallback texture in place
					const Texture& texture = textures[i];
					TextureResource* textureResource = textureResourceManager.tryGetById(texture.textureResourceId);
					if (nullptr == textureResource)
					{
						// Maybe it's a dynamically created texture like a shadow map created by "Renderer::CompositorInstancePassShadowMap"
						// which might not have been ready yet when the material was originally loaded
						textureResource = textureResourceManager.getTextureResourceByAssetId(texture.materialProperty.getTextureAssetIdValue());
						if (nullptr != textureResource)
						{
							mTextures[i].textureResourceId = textureResource->getId();
						}
					}
					if (nullptr != textureResource)
					{
						// We also need to get informed in case e.g. dynamic compositor textures get changed in order to update the texture resource group accordantly
						textureResource->connectResourceListener(*this);
					}
				}

				// Get material blueprint resource
				const MaterialBlueprintResource* materialBlueprintResource = getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
				SE_ASSERT(nullptr != materialBlueprintResource, "Invalid material blueprint resource")

				// Create texture resource group
				std::vector<Rhi::IResource*> resources;
				std::vector<Rhi::ISamplerState*> samplerStates;
				uint32_t textureStartIndex = 0;
				if (nullptr != mStructuredBufferPtr)
				{
					// First entry is the structured buffer
					resources.resize(numberOfTextures + 1);
					samplerStates.resize(numberOfTextures + 1);
					resources[0] = mStructuredBufferPtr;
					samplerStates[0] = nullptr;
					textureStartIndex = 1;
				}
				else
				{
					resources.resize(numberOfTextures);
					samplerStates.resize(numberOfTextures);
				}
				const MaterialBlueprintResource::Textures& materialBlueprintResourceTextures = materialBlueprintResource->getTextures();
				const MaterialBlueprintResource::SamplerStates& materialBlueprintResourceSamplerStates = materialBlueprintResource->getSamplerStates();
				for (size_t i = 0; i < numberOfTextures; ++i)
				{
					// Set texture resource
					TextureResource* textureResource = textureResourceManager.tryGetById(textures[i].textureResourceId);
					SE_ASSERT(nullptr != textureResource, "Invalid texture resource")
					resources[i + textureStartIndex] = textureResource->getTexturePtr();
					SE_ASSERT(nullptr != resources[i + textureStartIndex], "Invalid resource")

					// Set sampler state, if there's one (e.g. texel fetch instead of sampling might be used)
					if (IsValid(materialBlueprintResourceTextures[i].samplerStateIndex))
					{
						SE_ASSERT(materialBlueprintResourceTextures[i].samplerStateIndex < materialBlueprintResourceSamplerStates.size(), "Invalid sampler state index")
						samplerStates[i + textureStartIndex] = materialBlueprintResourceSamplerStates[materialBlueprintResourceTextures[i].samplerStateIndex].samplerStatePtr;
					}
					else
					{
						samplerStates[i + textureStartIndex] = nullptr;
					}
				}
				// TODO(co) All resources need to be inside the same resource group, this needs to be guaranteed by design
				mResourceGroup = renderer.getRendererResourceManager().createResourceGroup(*materialBlueprintResource->getRootSignaturePtr(), textures[0].rootParameterIndex, static_cast<uint32_t>(resources.size()), resources.data(), samplerStates.data() RHI_RESOURCE_DEBUG_NAME("Material technique"));
			}

			// Tell the caller about the resource group
			resourceGroupRootParameterIndex = textures[0].rootParameterIndex;
			*resourceGroup = mResourceGroup;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

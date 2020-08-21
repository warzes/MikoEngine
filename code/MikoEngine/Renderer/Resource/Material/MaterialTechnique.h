#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResourceListener.h"
#include "Renderer/Resource/Material/MaterialProperty.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/MaterialBufferSlot.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IRenderer;
	class MaterialBufferManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t TextureResourceId;				// POD texture resource identifier
	typedef uint32_t MaterialTechniqueId;			// Material technique identifier, result of hashing the material technique name via "Renderer::StringId"
	typedef uint32_t MaterialBlueprintResourceId;	// POD material blueprint resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material technique
	*/
	class MaterialTechnique final : public MaterialBufferSlot, public IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialResource;					// Material technique owner
		friend class MaterialBlueprintResourceManager;	// Needs to be able to call "Renderer::MaterialTechnique::makeResourceGroupDirty()"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialTechniqueId
		*    Material technique ID
		*  @param[in] materialResource
		*    Owner material resource, only material resource manager and material resource ID will internally be stored
		*  @param[in] materialBlueprintResourceId
		*    Material blueprint resource ID
		*/
		MaterialTechnique(MaterialTechniqueId materialTechniqueId, MaterialResource& materialResource, MaterialBlueprintResourceId materialBlueprintResourceId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MaterialTechnique() override;

		/**
		*  @brief
		*    Return the material technique ID
		*
		*  @return
		*    The material technique ID
		*/
		[[nodiscard]] inline MaterialTechniqueId getMaterialTechniqueId() const
		{
			return mMaterialTechniqueId;
		}

		/**
		*  @brief
		*    Return the ID of the used material blueprint resource
		*
		*  @return
		*    The ID of the used material blueprint resource, can be invalid
		*/
		[[nodiscard]] inline MaterialBlueprintResourceId getMaterialBlueprintResourceId() const
		{
			return mMaterialBlueprintResourceId;
		}

		/**
		*  @brief
		*    Set structured buffer pointer
		*
		*  @param[in] structuredBufferPtr
		*    RHI structured buffer pointer
		*/
		inline void setStructuredBufferPtr(Rhi::IStructuredBufferPtr& structuredBufferPtr)
		{
			mStructuredBufferPtr = structuredBufferPtr;
		}

		/**
		*  @brief
		*    Return the FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
		*
		*  @return
		*    The FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
		*/
		[[nodiscard]] inline uint32_t getSerializedGraphicsPipelineStateHash() const
		{
			return mSerializedGraphicsPipelineStateHash;
		}

		/**
		*  @brief
		*    Bind the graphics material technique into the given command buffer
		*
		*  @param[in] renderer
		*    Renderer to use
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*  @param[out] resourceGroupRootParameterIndex
		*    Root parameter index to bind the resource group to, can be "Renderer::getInvalid<uint32_t>()"
		*  @param[out] resourceGroup
		*    RHI resource group to set, must be valid
		*/
		void fillGraphicsCommandBuffer(const IRenderer& renderer, Rhi::CommandBuffer& commandBuffer, uint32_t& resourceGroupRootParameterIndex, Rhi::IResourceGroup** resourceGroup);

		/**
		*  @brief
		*    Bind the compute material technique into the given command buffer
		*
		*  @param[in] renderer
		*    Renderer to use
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*  @param[out] resourceGroupRootParameterIndex
		*    Root parameter index to bind the resource group to, can be "Renderer::getInvalid<uint32_t>()"
		*  @param[out] resourceGroup
		*    RHI resource group to set, must be valid
		*/
		void fillComputeCommandBuffer(const IRenderer& renderer, Rhi::CommandBuffer& commandBuffer, uint32_t& resourceGroupRootParameterIndex, Rhi::IResourceGroup** resourceGroup);


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const Renderer::IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Texture final
		{
			uint32_t		  rootParameterIndex;
			MaterialProperty  materialProperty;
			TextureResourceId textureResourceId;
		};
		typedef std::vector<Texture> Textures;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit MaterialTechnique(const MaterialTechnique&) = delete;
		MaterialTechnique& operator=(const MaterialTechnique&) = delete;
		MaterialBufferManager* getMaterialBufferManager() const;

		inline void clearTextures()
		{
			mTextures.clear();
			makeResourceGroupDirty();
		}

		/**
		*  @brief
		*    Return the textures
		*
		*  @param[in] renderer
		*    Renderer to use
		*
		*  @return
		*    The textures
		*/
		[[nodiscard]] const Textures& getTextures(const IRenderer& renderer);

		inline void makeResourceGroupDirty()
		{
			// Forget about the resource group so it's rebuild
			// TODO(co) Optimization possibility: Allow it to update resource groups instead of always having to destroy and recreate them?
			mResourceGroup = nullptr;
		}

		/**
		*  @brief
		*    Calculate FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
		*/
		void calculateSerializedGraphicsPipelineStateHash();

		/**
		*  @brief
		*    Schedule the material slot for shader uniform update
		*/
		void scheduleForShaderUniformUpdate();

		/**
		*  @brief
		*    Bind the material technique into the given command buffer
		*
		*  @param[in] renderer
		*    Renderer to use
		*  @param[out] resourceGroupRootParameterIndex
		*    Root parameter index to bind the resource group to, can be "Renderer::getInvalid<uint32_t>()"
		*  @param[out] resourceGroup
		*    RHI resource group to set, must be valid
		*/
		void fillCommandBuffer(const IRenderer& renderer, uint32_t& resourceGroupRootParameterIndex, Rhi::IResourceGroup** resourceGroup);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialTechniqueId			mMaterialTechniqueId;					// Material technique ID
		MaterialBlueprintResourceId	mMaterialBlueprintResourceId;			// Material blueprint resource ID, can be set to invalid value
		Rhi::IStructuredBufferPtr	mStructuredBufferPtr;
		Textures					mTextures;
		uint32_t					mSerializedGraphicsPipelineStateHash;	// FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
		Rhi::IResourceGroupPtr		mResourceGroup;							// Resource group, can be a null pointer


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

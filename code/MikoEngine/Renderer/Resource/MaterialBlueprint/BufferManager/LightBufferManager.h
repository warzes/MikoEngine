#pragma once

#include "Renderer/Core/Manager.h"
#include "Utility/StringId.h"
#include "RHI/Rhi.h"

namespace Renderer
{
	class SceneResource;
	class IRenderer;
	class MaterialBlueprintResource;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t			 TextureResourceId;	// POD texture resource identifier
	typedef StringId			 AssetId;			// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef std::vector<AssetId> AssetIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Light buffer manager
	*/
	class LightBufferManager final : private Manager
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the asset IDs of automatically generated dynamic default texture assets
		*
		*  @param[out] assetIds
		*    Receives the asset IDs of automatically generated dynamic default texture assets, the list is not cleared before new entries are added
		*
		*  @remarks
		*    The light buffer manager automatically generates some dynamic default texture assets one can reference e.g. inside material blueprint resources:
		*    - "Engine/Texture/DynamicByCode/LightClustersMap3D"
		*/
		static void getDefaultTextureAssetIds(AssetIds& assetIds);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderer
		*    Renderer instance to use
		*/
		explicit LightBufferManager(IRenderer& renderer);

		/**
		*  @brief
		*    Destructor
		*/
		~LightBufferManager();

		/**
		*  @brief
		*    Fill the light buffer
		*
		*  @param[in] worldSpaceCameraPosition
		*    64 bit world space position of the camera for camera relative rendering
		*  @param[in] sceneResource
		*    Scene resource to use
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*/
		void fillBuffer(const glm::dvec3& worldSpaceCameraPosition, SceneResource& sceneResource, Rhi::CommandBuffer& commandBuffer);

		/**
		*  @brief
		*    Bind the light buffer manager into the given graphics command buffer
		*
		*  @param[in] materialBlueprintResource
		*    Graphics material blueprint resource
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*/
		void fillGraphicsCommandBuffer(const MaterialBlueprintResource& materialBlueprintResource, Rhi::CommandBuffer& commandBuffer);

		/**
		*  @brief
		*    Bind the light buffer manager into the given compute command buffer
		*
		*  @param[in] materialBlueprintResource
		*    Compute material blueprint resource
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*/
		void fillComputeCommandBuffer(const MaterialBlueprintResource& materialBlueprintResource, Rhi::CommandBuffer& commandBuffer);

		/**
		*  @brief
		*    Get light clusters scale
		*
		*  @return
		*    Light clusters scale
		*/
		[[nodiscard]] glm::vec3 getLightClustersScale() const;

		/**
		*  @brief
		*    Get light clusters bias
		*
		*  @return
		*    Light clusters bias
		*/
		[[nodiscard]] glm::vec3 getLightClustersBias() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit LightBufferManager(const LightBufferManager&) = delete;
		LightBufferManager& operator=(const LightBufferManager&) = delete;
		void fillTextureBuffer(const glm::dvec3& worldSpaceCameraPosition, SceneResource& sceneResource);	// 64 bit world space position of the camera
		void fillClusters3DTexture(SceneResource& sceneResource, Rhi::CommandBuffer& commandBuffer);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint8_t> ScratchBuffer;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&			 mRenderer;			// Renderer instance to use
		Rhi::ITextureBuffer* mTextureBuffer;	// RHI texture buffer instance, always valid
		ScratchBuffer		 mTextureScratchBuffer;
		TextureResourceId	 mClusters3DTextureResourceId;
		glm::vec3			 mLightClustersAabbMinimum;
		glm::vec3			 mLightClustersAabbMaximum;
		Rhi::IResourceGroup* mResourceGroup;	// RHI resource group instance, always valid


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

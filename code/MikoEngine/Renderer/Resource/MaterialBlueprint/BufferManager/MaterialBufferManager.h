#pragma once

namespace Rhi
{
	class CommandBuffer;
	class IUniformBuffer;
	class IBufferManager;
	class IResourceGroup;
}
namespace Renderer
{
	class IRenderer;
	class MaterialBufferSlot;
	class MaterialBlueprintResource;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material buffer manager
	*
	*  @note
	*    - For material batching
	*    - Concept basing on OGRE 2.1 "Ogre::ConstBufferPool", but more generic and simplified thanks to the material blueprint concept
	*/
	class MaterialBufferManager final
	{


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
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource
		*/
		MaterialBufferManager(IRenderer& renderer, const MaterialBlueprintResource& materialBlueprintResource);

		/**
		*  @brief
		*    Destructor
		*/
		~MaterialBufferManager();

		/**
		*  @brief
		*    Request a slot and fill the material slot; automatically schedules for update
		*/
		void requestSlot(MaterialBufferSlot& materialBufferSlot);

		/**
		*  @brief
		*    Release a slot requested with "Renderer::MaterialBufferManager::requestSlot()"
		*/
		void releaseSlot(MaterialBufferSlot& materialBufferSlot);

		/**
		*  @brief
		*    Schedule the slot of the given material slot for update
		*/
		void scheduleForUpdate(MaterialBufferSlot& materialBufferSlot);

		/**
		*  @brief
		*    Reset last graphics bound pool and update the dirty slots
		*/
		void resetLastGraphicsBoundPool();

		/**
		*  @brief
		*    Reset last compute bound pool and update the dirty slots
		*/
		void resetLastComputeBoundPool();

		/**
		*  @brief
		*    Fill slot to graphics command buffer
		*
		*  @param[in] materialBufferSlot
		*    Graphics material buffer slot to bind
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*/
		void fillGraphicsCommandBuffer(MaterialBufferSlot& materialBufferSlot, Rhi::CommandBuffer& commandBuffer);

		/**
		*  @brief
		*    Fill slot to compute command buffer
		*
		*  @param[in] materialBufferSlot
		*    Compute material buffer slot to bind
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*/
		void fillComputeCommandBuffer(MaterialBufferSlot& materialBufferSlot, Rhi::CommandBuffer& commandBuffer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit MaterialBufferManager(const MaterialBufferManager&) = delete;
		MaterialBufferManager& operator=(const MaterialBufferManager&) = delete;
		void uploadDirtySlots();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct BufferPool final
		{
			std::vector<uint32_t> freeSlots;
			Rhi::IUniformBuffer*  uniformBuffer;	// Memory is managed by this buffer pool instance
			Rhi::IResourceGroup*  resourceGroup;	// Memory is managed by this buffer pool instance

			BufferPool(uint32_t bufferSize, uint32_t slotsPerPool, Rhi::IBufferManager& bufferManager, const MaterialBlueprintResource& materialBlueprintResource);
			~BufferPool();
		};

		typedef std::vector<BufferPool*>		 BufferPools;
		typedef std::vector<MaterialBufferSlot*> MaterialBufferSlots;
		typedef std::vector<uint8_t>			 ScratchBuffer;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&						 mRenderer;
		const MaterialBlueprintResource& mMaterialBlueprintResource;
		BufferPools						 mBufferPools;
		uint32_t						 mSlotsPerPool;
		uint32_t						 mBufferSize;
		MaterialBufferSlots				 mDirtyMaterialBufferSlots;
		MaterialBufferSlots				 mMaterialBufferSlots;
		const BufferPool*				 mLastGraphicsBoundPool;
		const BufferPool*				 mLastComputeBoundPool;
		ScratchBuffer					 mScratchBuffer;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

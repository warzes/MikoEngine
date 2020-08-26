#pragma once

#include "RHI/Rhi.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class MaterialResource;
	class MaterialResourceManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialResourceId;	// POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material buffer slot
	*/
	class MaterialBufferSlot
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialBufferManager;	// Manages the slot instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialResource
		*    Owner material resource, only material resource manager and material resource ID will internally be stored
		*/
		explicit MaterialBufferSlot(MaterialResource& materialResource);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~MaterialBufferSlot()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Return the owner material resource manager
		*
		*  @return
		*    The owner material resource manager
		*/
		[[nodiscard]] inline MaterialResourceManager& getMaterialResourceManager() const
		{
			SE_ASSERT(nullptr != mMaterialResourceManager, "Invalid material resource manager")
			return *mMaterialResourceManager;
		}

		/**
		*  @brief
		*    Return the owner material resource ID
		*
		*  @return
		*    The owner material resource ID
		*/
		[[nodiscard]] inline MaterialResourceId getMaterialResourceId() const
		{
			return mMaterialResourceId;
		}

		/**
		*  @brief
		*    Return the owner material resource instance
		*
		*  @return
		*    The owner material resource instance
		*
		*  @note
		*    - Ease of use method
		*/
		[[nodiscard]] const MaterialResource& getMaterialResource() const;

		/**
		*  @brief
		*    Return the assigned material slot
		*
		*  @return
		*    The assigned material slot
		*/
		[[nodiscard]] inline uint32_t getAssignedMaterialSlot() const
		{
			return mAssignedMaterialSlot;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit MaterialBufferSlot(const MaterialBufferSlot&) = delete;
		MaterialBufferSlot& operator=(const MaterialBufferSlot&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialResourceManager* mMaterialResourceManager;	// Owner material resource manager, always valid
		MaterialResourceId		 mMaterialResourceId;		// Owner material resource ID, always valid
		void*					 mAssignedMaterialPool;		// "Renderer::MaterialBufferManager::BufferPool*", it's a private inner class which we can't forward declare, but we also don't want to expose too much details, so void* it is in here
		uint32_t				 mAssignedMaterialSlot;
		int						 mGlobalIndex;
		bool					 mDirty;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

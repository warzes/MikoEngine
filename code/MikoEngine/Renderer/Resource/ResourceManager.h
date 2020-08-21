#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResourceManager.h"


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
	*    Public abstract resource manager template
	*/
	template <class TYPE>
	class ResourceManager : public IResourceManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Ease-of-use access templates                          ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline TYPE& getByIndex(uint32_t index) const
		{
			return static_cast<TYPE&>(getResourceByIndex(index));
		}

		[[nodiscard]] inline TYPE& getById(ResourceId resourceId) const
		{
			return static_cast<TYPE&>(getResourceByResourceId(resourceId));
		}

		[[nodiscard]] inline TYPE* tryGetById(ResourceId resourceId) const
		{
			return static_cast<TYPE*>(tryGetResourceByResourceId(resourceId));
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline ResourceManager()
		{
			// Nothing here
		}

		inline virtual ~ResourceManager() override
		{
			// Nothing here
		}

		explicit ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

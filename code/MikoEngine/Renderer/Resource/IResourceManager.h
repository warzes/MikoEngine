#pragma once

#include "Renderer/Resource/IResource.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId ResourceLoaderTypeId;	// Resource loader type identifier, internally just a POD "uint32_t", usually created by hashing the file format extension (if the resource loader is processing file data in the first place)


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Public abstract resource manager interface
	*/
	class IResourceManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;		// Needs to be able to destroy resource manager instances
		friend class ResourceStreamer;	// Needs to be able to create resource loader instances


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual uint32_t getNumberOfResources() const = 0;
		[[nodiscard]] virtual IResource& getResourceByIndex(uint32_t index) const = 0;
		[[nodiscard]] virtual IResource& getResourceByResourceId(ResourceId resourceId) const = 0;
		[[nodiscard]] virtual IResource* tryGetResourceByResourceId(ResourceId resourceId) const = 0;
		virtual void reloadResourceByAssetId(AssetId assetId) = 0;
		virtual void update() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IResourceManager()
		{
			// Nothing here
		}

		inline virtual ~IResourceManager()
		{
			// Noting here
		}

		explicit IResourceManager(const IResourceManager&) = delete;
		IResourceManager& operator=(const IResourceManager&) = delete;

		inline void setResourceLoadingState(IResource& resource, IResource::LoadingState loadingState)
		{
			resource.setLoadingState(loadingState);
		}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] virtual IResourceLoader* createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId) = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

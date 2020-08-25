#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/PackedElementManager.h"
#include "Renderer/Asset/AssetManager.h"
#include "Renderer/Resource/ResourceStreamer.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IResourceManager;
	class IRenderer;
	class IResourceListener;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;				// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef StringId ResourceLoaderTypeId;	// Resource loader type identifier, internally just a POD "uint32_t", usually created by hashing the file format extension (if the resource loader is processing file data in the first place)


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Internal resource manager template; not public used to keep template instantiation overhead under control
	*/
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS>
	class ResourceManagerTemplate : private Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef PackedElementManager<TYPE, ID_TYPE, MAXIMUM_NUMBER_OF_ELEMENTS> Resources;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline ResourceManagerTemplate(IRenderer& renderer, IResourceManager& resourceManager) :
			mRenderer(renderer),
			mResourceManager(resourceManager)
		{
			// Nothing here
		}

		inline ~ResourceManagerTemplate()
		{
			// Nothing here
		}

		[[nodiscard]] inline IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		[[nodiscard]] inline IResourceManager& getResourceManager() const
		{
			return mResourceManager;
		}

		[[nodiscard]] inline LOADER_TYPE* createResourceLoaderInstance([[maybe_unused]] ResourceLoaderTypeId resourceLoaderTypeId)
		{
			// We only support our own format
			RHI_ASSERT(resourceLoaderTypeId == LOADER_TYPE::TYPE_ID, "Invalid resource loader type ID")
			return new LOADER_TYPE(mResourceManager, mRenderer);
		}

		[[nodiscard]] inline TYPE* getResourceByAssetId(AssetId assetId) const	// Considered to be inefficient, avoid method whenever possible
		{
			// TODO(co) Implement more efficient solution later on
			const uint32_t numberOfElements = mResources.getNumberOfElements();
			for (uint32_t i = 0; i < numberOfElements; ++i)
			{
				TYPE& resource = mResources.getElementByIndex(i);
				if (resource.getAssetId() == assetId)
				{
					return &resource;
				}
			}

			// There's no resource using the given asset ID
			return nullptr;
		}

		[[nodiscard]] inline TYPE& createEmptyResourceByAssetId(AssetId assetId)	// Resource is not allowed to exist, yet
		{
			// Sanity check
			RHI_ASSERT(nullptr == getResourceByAssetId(assetId), "The resource isn't allowed to exist, yet")

			// Create the resource instance
			TYPE& resource = mResources.addElement();
			resource.setResourceManager(&mResourceManager);
			resource.setAssetId(assetId);

			// Done
			return resource;
		}

		inline void loadResourceByAssetId(AssetId assetId, ID_TYPE& resourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)	// Asynchronous
		{
			// Choose default resource loader type ID, if necessary
			if (IsInvalid(resourceLoaderTypeId))
			{
				resourceLoaderTypeId = LOADER_TYPE::TYPE_ID;
			}

			// Get or create the instance
			TYPE* resource = getResourceByAssetId(assetId);

			// Create the resource instance
			const Asset* asset = mRenderer.getAssetManager().tryGetAssetByAssetId(assetId);
			RHI_ASSERT(nullptr != asset, "Unknown asset ID")
			bool load = (reload && nullptr != asset);
			if (nullptr == resource && nullptr != asset)
			{
				resource = &mResources.addElement();
				resource->setResourceManager(&mResourceManager);
				resource->setAssetId(assetId);
				resource->setResourceLoaderTypeId(resourceLoaderTypeId);
				load = true;
			}

			// Before connecting a resource listener, ensure we set the output resource ID at once so it can already directly be used inside the resource listener
			if (nullptr != resource)
			{
				resourceId = resource->getId();
				if (nullptr != resourceListener)
				{
					resource->connectResourceListener(*resourceListener);
				}
			}
			else
			{
				resourceId = GetInvalid<ID_TYPE>();
			}

			// Load the resource, if required
			if (load)
			{
				// Commit resource streamer asset load request
				mRenderer.getResourceStreamer().commitLoadRequest(ResourceStreamer::LoadRequest(*asset, resourceLoaderTypeId, reload, mResourceManager, resourceId));
			}
		}

		inline void reloadResourceByAssetId(AssetId assetId)
		{
			// TODO(co) Experimental implementation (take care of resource cleanup etc.)
			const uint32_t numberOfElements = mResources.getNumberOfElements();
			for (uint32_t i = 0; i < numberOfElements; ++i)
			{
				const TYPE& resource = mResources.getElementByIndex(i);
				if (resource.getAssetId() == assetId)
				{
					ID_TYPE resourceId = GetInvalid<ID_TYPE>();
					loadResourceByAssetId(assetId, resourceId, nullptr, true, resource.getResourceLoaderTypeId());
					break;
				}
			}
		}

		[[nodiscard]] inline Resources& getResources()
		{
			return mResources;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit ResourceManagerTemplate(const ResourceManagerTemplate&) = delete;
		ResourceManagerTemplate& operator=(const ResourceManagerTemplate&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&		  mRenderer;	// Renderer instance, do not destroy the instance
		IResourceManager& mResourceManager;
		Resources		  mResources;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

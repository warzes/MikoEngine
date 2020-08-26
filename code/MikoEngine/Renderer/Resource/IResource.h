#pragma once

#include "Utility/StringId.h"
#include "Core/GetInvalid.h"

namespace Renderer
{
	class IResourceManager;
	class IResourceListener;
}

namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;				// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef uint32_t ResourceId;			// POD resource identifier
	typedef StringId ResourceLoaderTypeId;	// Resource loader type identifier, internally just a POD "uint32_t", usually created by hashing the file format extension (if the resource loader is processing file data in the first place)


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ResourceStreamer;	// Is changing the resource loading state
		friend class IResourceManager;	// Is changing the resource loading state


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class LoadingState
		{
			UNLOADED,	// Not loaded
			LOADING,	// Loading is in progress
			LOADED,		// Fully loaded
			UNLOADING,	// Currently unloading	// TODO(co) Currently unused
			FAILED		// The last loading attempt failed
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		#if SE_DEBUG
			/**
			*  @brief
			*    Return the renderable debug name
			*
			*  @return
			*    The renderable debug name, never a null pointer and at least an empty string
			*
			*  @note
			*    - If possible, the renderable debug name should use the following convention: "<filename>?[<attribute 0>][<attribute n>]" (for "?" see "Renderer::IFileManager::INVALID_CHARACTER")
			*/
			[[nodiscard]] inline const char* getDebugName() const
			{
				return mDebugName;
			}

			inline void setDebugName(const char debugName[])
			{
				SE_ASSERT(strlen(debugName) < 256, "Renderable debug name is not allowed to exceed 255 characters")
				strncpy(mDebugName, debugName, 256);
				mDebugName[255] = '\0';
			}
		#endif

		[[nodiscard]] inline IResourceManager& getResourceManager() const
		{
			SE_ASSERT(nullptr != mResourceManager, "Invalid resource manager")
			return *mResourceManager;
		}

		template <typename T>
		[[nodiscard]] inline T& getResourceManager() const
		{
			SE_ASSERT(nullptr != mResourceManager, "Invalid resource manager")
			return *static_cast<T*>(mResourceManager);
		}

		[[nodiscard]] inline ResourceId getId() const
		{
			return mResourceId;
		}

		[[nodiscard]] inline AssetId getAssetId() const
		{
			return mAssetId;
		}

		[[nodiscard]] inline ResourceLoaderTypeId getResourceLoaderTypeId() const
		{
			return mResourceLoaderTypeId;
		}

		[[nodiscard]] inline LoadingState getLoadingState() const
		{
			return mLoadingState;
		}

		void connectResourceListener(IResourceListener& resourceListener);	// No guaranteed resource listener caller order, if already connected nothing happens (no double registration)
		void disconnectResourceListener(IResourceListener& resourceListener);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IResource() :
			#if SE_DEBUG
				mDebugName{},
			#endif
			mResourceManager(nullptr),
			mResourceId(GetInvalid<ResourceId>()),
			mAssetId(GetInvalid<AssetId>()),
			mResourceLoaderTypeId(GetInvalid<ResourceLoaderTypeId>()),
			mLoadingState(LoadingState::UNLOADED)
		{
			// Nothing here
		}

		inline virtual ~IResource()
		{
			// Sanity checks
			SE_ASSERT('\0' == mDebugName[0], "Invalid debug name")
			SE_ASSERT(nullptr == mResourceManager, "Invalid resource manager")
			SE_ASSERT(IsInvalid(mResourceId), "Invalid resource ID")
			SE_ASSERT(IsInvalid(mAssetId), "Invalid asset ID")
			SE_ASSERT(IsInvalid(mResourceLoaderTypeId), "Invalid resource loader type ID")
			SE_ASSERT(LoadingState::UNLOADED == mLoadingState || LoadingState::FAILED == mLoadingState, "Invalid loading state")
			SE_ASSERT(mSortedResourceListeners.empty(), "Invalid sorted resource listeners")
		}

		explicit IResource(const IResource&) = delete;
		IResource& operator=(const IResource&) = delete;
		IResource& operator=(IResource&& resource);

		inline void setResourceManager(IResourceManager* resourceManager)
		{
			mResourceManager = resourceManager;
		}

		inline void setAssetId(AssetId assetId)
		{
			mAssetId = assetId;
		}

		inline void setResourceLoaderTypeId(ResourceLoaderTypeId resourceLoaderTypeId)
		{
			mResourceLoaderTypeId = resourceLoaderTypeId;
		}

		void setLoadingState(LoadingState loadingState);

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(ResourceId resourceId)
		{
			// Sanity checks
			SE_ASSERT('\0' == mDebugName[0], "Invalid debug name")
			SE_ASSERT(nullptr == mResourceManager, "Invalid resource manager")
			SE_ASSERT(IsInvalid(mResourceId), "Invalid resource ID")
			SE_ASSERT(IsInvalid(mAssetId), "Invalid asset ID")
			SE_ASSERT(IsInvalid(mResourceLoaderTypeId), "Invalid resource loader type ID")
			SE_ASSERT(LoadingState::UNLOADED == mLoadingState, "Invalid loading state")
			SE_ASSERT(mSortedResourceListeners.empty(), "Invalid sorted resource listeners")

			// Set data
			mResourceId = resourceId;
		}

		void deinitializeElement();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<IResourceListener*> SortedResourceListeners;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		#if SE_DEBUG
			char				mDebugName[256];		// Debug name for easier resource identification when debugging, contains terminating zero, first member variable by intent to see it at once during introspection (debug memory layout change is no problem here)
		#endif
		IResourceManager*		mResourceManager;		// Owner resource manager, always valid
		ResourceId				mResourceId;			// Unique resource ID inside the resource manager
		AssetId					mAssetId;				// In case the resource is an instance of an asset, this is the ID of this asset
		ResourceLoaderTypeId	mResourceLoaderTypeId;
		LoadingState			mLoadingState;
		SortedResourceListeners mSortedResourceListeners;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

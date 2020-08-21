#include "stdafx.h"
#include "Renderer/Resource/IResource.h"
#include "Renderer/Resource/IResourceListener.h"

#include <algorithm>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline bool orderByResourceListener(Renderer::IResourceListener* left, Renderer::IResourceListener* right)
		{
			return (left < right);
		}


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
	void IResource::connectResourceListener(IResourceListener& resourceListener)
	{
		SortedResourceListeners::iterator iterator = std::lower_bound(mSortedResourceListeners.begin(), mSortedResourceListeners.end(), &resourceListener, ::detail::orderByResourceListener);
		if (iterator == mSortedResourceListeners.end() || *iterator != &resourceListener)
		{
			mSortedResourceListeners.insert(iterator, &resourceListener);
			resourceListener.mResourceConnections.emplace_back(mResourceManager, mResourceId);
			resourceListener.onLoadingStateChange(*this);
		}
	}

	void IResource::disconnectResourceListener(IResourceListener& resourceListener)
	{
		SortedResourceListeners::iterator iterator = std::lower_bound(mSortedResourceListeners.begin(), mSortedResourceListeners.end(), &resourceListener, ::detail::orderByResourceListener);
		if (iterator != mSortedResourceListeners.end() && *iterator == &resourceListener)
		{
			{ // TODO(co) If this turns out to be a performance problem, we might want to use e.g. a sorted vector
				const IResourceListener::ResourceConnection resourceConnection(mResourceManager, mResourceId);
				IResourceListener::ResourceConnections::iterator connectionIterator = std::find_if(resourceListener.mResourceConnections.begin(), resourceListener.mResourceConnections.end(),
					[resourceConnection](const IResourceListener::ResourceConnection& currentResourceConnection) { return (currentResourceConnection.resourceManager == resourceConnection.resourceManager && currentResourceConnection.resourceId == resourceConnection.resourceId); }
					);
				RHI_ASSERT(connectionIterator != resourceListener.mResourceConnections.end(), "Invalid connection iterator")
				resourceListener.mResourceConnections.erase(connectionIterator);
			}
			mSortedResourceListeners.erase(iterator);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	IResource& IResource::operator=(IResource&& resource)
	{
		// Swap data
		#if SE_DEBUG
			std::swap(mDebugName,			resource.mDebugName);
		#endif
		std::swap(mResourceManager,			resource.mResourceManager);
		std::swap(mResourceId,				resource.mResourceId);
		std::swap(mAssetId,					resource.mAssetId);
		std::swap(mResourceLoaderTypeId,	resource.mResourceLoaderTypeId);
		std::swap(mLoadingState,			resource.mLoadingState);
		std::swap(mSortedResourceListeners,	resource.mSortedResourceListeners);	// This is fine, resource listeners store a resource ID instead of a raw pointer

		// Done
		return *this;
	}

	void IResource::setLoadingState(LoadingState loadingState)
	{
		mLoadingState = loadingState;
		for (IResourceListener* resourceListener : mSortedResourceListeners)
		{
			resourceListener->onLoadingStateChange(*this);
		}
	}

	void IResource::deinitializeElement()
	{
		// Sanity check
		RHI_ASSERT(LoadingState::LOADING != mLoadingState || LoadingState::UNLOADING != mLoadingState, "Resource deinitialized while in-flight inside the resource streamer")

		// Update loading state, if necessary
		if (LoadingState::UNLOADED != mLoadingState && LoadingState::FAILED != mLoadingState)
		{
			setLoadingState(LoadingState::UNLOADED);
		}

		// Disconnect all resource listeners
		const IResourceListener::ResourceConnection resourceConnection(mResourceManager, mResourceId);
		for (IResourceListener* resourceListener : mSortedResourceListeners)
		{
			// TODO(co) If this turns out to be a performance problem, we might want to use e.g. a sorted vector
			IResourceListener::ResourceConnections::iterator connectionIterator = std::find_if(resourceListener->mResourceConnections.begin(), resourceListener->mResourceConnections.end(),
				[resourceConnection](const IResourceListener::ResourceConnection& currentResourceConnection) { return (currentResourceConnection.resourceManager == resourceConnection.resourceManager && currentResourceConnection.resourceId == resourceConnection.resourceId); }
				);
			RHI_ASSERT(connectionIterator != resourceListener->mResourceConnections.end(), "Invalid connection iterator")
			resourceListener->mResourceConnections.erase(connectionIterator);
		}

		// Reset everything
		#if SE_DEBUG
			mDebugName[0] = '\0';	// This isn't a security relevant use-case, so the most simple solution is sufficient here
		#endif
		mResourceManager = nullptr;
		setInvalid(mResourceId);
		setInvalid(mAssetId);
		setInvalid(mResourceLoaderTypeId);
		mSortedResourceListeners.clear();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#include "stdafx.h"
#include "Renderer/Resource/Skeleton/SkeletonResourceManager.h"
#include "Renderer/Resource/Skeleton/SkeletonResource.h"
#include "Renderer/Resource/Skeleton/Loader/SkeletonResourceLoader.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SkeletonResource* SkeletonResourceManager::getSkeletonResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	void SkeletonResourceManager::loadSkeletonResourceByAssetId(AssetId assetId, SkeletonResourceId& skeletonResourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, skeletonResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	SkeletonResourceId SkeletonResourceManager::createSkeletonResourceByAssetId(AssetId assetId)
	{
		SkeletonResource& skeletonResource = mInternalResourceManager->createEmptyResourceByAssetId(assetId);
		setResourceLoadingState(skeletonResource, IResource::LoadingState::LOADED);
		return skeletonResource.getId();
	}

	void SkeletonResourceManager::setInvalidResourceId(SkeletonResourceId& skeletonResourceId, IResourceListener& resourceListener) const
	{
		SkeletonResource* skeletonResource = tryGetById(skeletonResourceId);
		if (nullptr != skeletonResource)
		{
			skeletonResource->disconnectResourceListener(resourceListener);
		}
		SetInvalid(skeletonResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t SkeletonResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& SkeletonResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& SkeletonResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* SkeletonResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void SkeletonResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}

	void SkeletonResourceManager::update()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* SkeletonResourceManager::createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return mInternalResourceManager->createResourceLoaderInstance(resourceLoaderTypeId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	SkeletonResourceManager::SkeletonResourceManager(IRenderer& renderer)
	{
		mInternalResourceManager = new ResourceManagerTemplate<SkeletonResource, SkeletonResourceLoader, SkeletonResourceId, 2048>(renderer, *this);
	}

	SkeletonResourceManager::~SkeletonResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

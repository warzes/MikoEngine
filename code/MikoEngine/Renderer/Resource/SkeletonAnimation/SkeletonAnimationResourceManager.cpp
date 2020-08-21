#include "stdafx.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationResourceManager.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationResource.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationController.h"
#include "Renderer/Resource/SkeletonAnimation/Loader/SkeletonAnimationResourceLoader.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"
#include "Renderer/Core/Time/TimeManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SkeletonAnimationResource* SkeletonAnimationResourceManager::getSkeletonAnimationResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	void SkeletonAnimationResourceManager::loadSkeletonAnimationResourceByAssetId(AssetId assetId, SkeletonAnimationResourceId& skeletonAnimationResourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, skeletonAnimationResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	SkeletonAnimationResourceId SkeletonAnimationResourceManager::createSkeletonAnimationResourceByAssetId(AssetId assetId)
	{
		SkeletonAnimationResource& skeletonAnimationResource = mInternalResourceManager->createEmptyResourceByAssetId(assetId);
		setResourceLoadingState(skeletonAnimationResource, IResource::LoadingState::LOADED);
		return skeletonAnimationResource.getId();
	}

	void SkeletonAnimationResourceManager::setInvalidResourceId(SkeletonAnimationResourceId& skeletonAnimationResourceId, IResourceListener& resourceListener) const
	{
		SkeletonAnimationResource* skeletonAnimationResource = tryGetById(skeletonAnimationResourceId);
		if (nullptr != skeletonAnimationResource)
		{
			skeletonAnimationResource->disconnectResourceListener(resourceListener);
		}
		setInvalid(skeletonAnimationResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t SkeletonAnimationResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& SkeletonAnimationResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& SkeletonAnimationResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* SkeletonAnimationResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void SkeletonAnimationResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}

	void SkeletonAnimationResourceManager::update()
	{
		// Update skeleton animation controllers
		const float pastSecondsSinceLastFrame = mInternalResourceManager->getRenderer().getTimeManager().getPastSecondsSinceLastFrame();
		for (SkeletonAnimationController* skeletonAnimationController : mSkeletonAnimationControllers)
		{
			skeletonAnimationController->update(pastSecondsSinceLastFrame);
		}
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* SkeletonAnimationResourceManager::createResourceLoaderInstance([[maybe_unused]] ResourceLoaderTypeId resourceLoaderTypeId)
	{
		// We only support our own format
		RHI_ASSERT(resourceLoaderTypeId == SkeletonAnimationResourceLoader::TYPE_ID, "Invalid resource loader type ID")
		#if SE_DEBUG
			return new SkeletonAnimationResourceLoader(mInternalResourceManager->getResourceManager(), mInternalResourceManager->getRenderer());
		#else
			return new SkeletonAnimationResourceLoader(mInternalResourceManager->getResourceManager());
		#endif
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	SkeletonAnimationResourceManager::SkeletonAnimationResourceManager(IRenderer& renderer)
	{
		mInternalResourceManager = new ResourceManagerTemplate<SkeletonAnimationResource, SkeletonAnimationResourceLoader, SkeletonAnimationResourceId, 2048>(renderer, *this);
	}

	SkeletonAnimationResourceManager::~SkeletonAnimationResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

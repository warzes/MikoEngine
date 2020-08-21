#include "stdafx.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceResourceManager.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceResource.h"
#include "Renderer/Resource/CompositorWorkspace/Loader/CompositorWorkspaceResourceLoader.h"
#include "Renderer/Core/Renderer/RenderPassManager.h"
#include "Renderer/Core/Renderer/FramebufferManager.h"
#include "Renderer/Core/Renderer/RenderTargetTextureManager.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void CompositorWorkspaceResourceManager::loadCompositorWorkspaceResourceByAssetId(AssetId assetId, CompositorWorkspaceResourceId& compositorWorkspaceResourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, compositorWorkspaceResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	void CompositorWorkspaceResourceManager::setInvalidResourceId(CompositorWorkspaceResourceId& compositorWorkspaceResourceId, IResourceListener& resourceListener) const
	{
		CompositorWorkspaceResource* compositorWorkspaceResource = tryGetById(compositorWorkspaceResourceId);
		if (nullptr != compositorWorkspaceResource)
		{
			compositorWorkspaceResource->disconnectResourceListener(resourceListener);
		}
		setInvalid(compositorWorkspaceResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t CompositorWorkspaceResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& CompositorWorkspaceResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& CompositorWorkspaceResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* CompositorWorkspaceResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void CompositorWorkspaceResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* CompositorWorkspaceResourceManager::createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return mInternalResourceManager->createResourceLoaderInstance(resourceLoaderTypeId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorWorkspaceResourceManager::CompositorWorkspaceResourceManager(IRenderer& renderer) :
		mRenderTargetTextureManager(new RenderTargetTextureManager(renderer)),
		mRenderPassManager(new RenderPassManager(renderer.getRhi())),
		mFramebufferManager(new FramebufferManager(*mRenderTargetTextureManager, *mRenderPassManager))
	{
		mInternalResourceManager = new ResourceManagerTemplate<CompositorWorkspaceResource, CompositorWorkspaceResourceLoader, CompositorWorkspaceResourceId, 32>(renderer, *this);
	}

	CompositorWorkspaceResourceManager::~CompositorWorkspaceResourceManager()
	{
		delete mFramebufferManager;
		delete mRenderPassManager;
		delete mRenderTargetTextureManager;
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

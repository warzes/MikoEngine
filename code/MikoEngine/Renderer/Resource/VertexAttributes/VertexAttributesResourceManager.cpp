#include "stdafx.h"
#include "Renderer/Resource/VertexAttributes/VertexAttributesResourceManager.h"
#include "Renderer/Resource/VertexAttributes/VertexAttributesResource.h"
#include "Renderer/Resource/VertexAttributes/Loader/VertexAttributesResourceLoader.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	VertexAttributesResource* VertexAttributesResourceManager::getVertexAttributesResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	void VertexAttributesResourceManager::loadVertexAttributesResourceByAssetId(AssetId assetId, VertexAttributesResourceId& vertexAttributesResourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, vertexAttributesResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	VertexAttributesResourceId VertexAttributesResourceManager::createVertexAttributesResourceByAssetId(AssetId assetId)
	{
		VertexAttributesResource& vertexAttributesResource = mInternalResourceManager->createEmptyResourceByAssetId(assetId);
		setResourceLoadingState(vertexAttributesResource, IResource::LoadingState::LOADED);
		return vertexAttributesResource.getId();
	}

	void VertexAttributesResourceManager::setInvalidResourceId(VertexAttributesResourceId& vertexAttributesResourceId, IResourceListener& resourceListener) const
	{
		VertexAttributesResource* vertexAttributesResource = tryGetById(vertexAttributesResourceId);
		if (nullptr != vertexAttributesResource)
		{
			vertexAttributesResource->disconnectResourceListener(resourceListener);
		}
		setInvalid(vertexAttributesResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t VertexAttributesResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& VertexAttributesResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& VertexAttributesResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* VertexAttributesResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void VertexAttributesResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}

	void VertexAttributesResourceManager::update()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* VertexAttributesResourceManager::createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return mInternalResourceManager->createResourceLoaderInstance(resourceLoaderTypeId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	VertexAttributesResourceManager::VertexAttributesResourceManager(IRenderer& renderer)
	{
		mInternalResourceManager = new ResourceManagerTemplate<VertexAttributesResource, VertexAttributesResourceLoader, VertexAttributesResourceId, 32>(renderer, *this);
	}

	VertexAttributesResourceManager::~VertexAttributesResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

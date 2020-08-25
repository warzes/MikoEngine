#include "stdafx.h"
#include "Renderer/Resource/ShaderPiece/ShaderPieceResourceManager.h"
#include "Renderer/Resource/ShaderPiece/ShaderPieceResource.h"
#include "Renderer/Resource/ShaderPiece/Loader/ShaderPieceResourceLoader.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void ShaderPieceResourceManager::loadShaderPieceResourceByAssetId(AssetId assetId, ShaderPieceResourceId& shaderPieceResourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, shaderPieceResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	void ShaderPieceResourceManager::setInvalidResourceId(ShaderPieceResourceId& shaderPieceResourceId, IResourceListener& resourceListener) const
	{
		ShaderPieceResource* shaderPieceResource = tryGetById(shaderPieceResourceId);
		if (nullptr != shaderPieceResource)
		{
			shaderPieceResource->disconnectResourceListener(resourceListener);
		}
		SetInvalid(shaderPieceResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t ShaderPieceResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& ShaderPieceResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& ShaderPieceResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* ShaderPieceResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void ShaderPieceResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}

	void ShaderPieceResourceManager::update()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* ShaderPieceResourceManager::createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return mInternalResourceManager->createResourceLoaderInstance(resourceLoaderTypeId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	ShaderPieceResourceManager::ShaderPieceResourceManager(IRenderer& renderer) :
		mRenderer(renderer)
	{
		mInternalResourceManager = new ResourceManagerTemplate<ShaderPieceResource, ShaderPieceResourceLoader, ShaderPieceResourceId, 64>(renderer, *this);
	}

	ShaderPieceResourceManager::~ShaderPieceResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

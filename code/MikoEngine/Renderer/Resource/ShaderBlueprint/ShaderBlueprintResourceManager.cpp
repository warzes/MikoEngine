#include "stdafx.h"
#include "Renderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "Renderer/Resource/ShaderBlueprint/ShaderBlueprintResource.h"
#include "Renderer/Resource/ShaderBlueprint/Loader/ShaderBlueprintResourceLoader.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"


// Disable warnings
// TODO(co) See "Renderer::ShaderBlueprintResourceManager::ShaderBlueprintResourceManager()": How the heck should we avoid such a situation without using complicated solutions like a pointer to an instance? (= more individual allocations/deallocations)
SE_PRAGMA_WARNING_DISABLE_MSVC(4355)	// warning C4355: 'this': used in base member initializer list


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void ShaderBlueprintResourceManager::loadShaderBlueprintResourceByAssetId(AssetId assetId, ShaderBlueprintResourceId& shaderBlueprintResourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, shaderBlueprintResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	void ShaderBlueprintResourceManager::setInvalidResourceId(ShaderBlueprintResourceId& shaderBlueprintResourceId, IResourceListener& resourceListener) const
	{
		ShaderBlueprintResource* shaderBlueprintResource = tryGetById(shaderBlueprintResourceId);
		if (nullptr != shaderBlueprintResource)
		{
			shaderBlueprintResource->disconnectResourceListener(resourceListener);
		}
		setInvalid(shaderBlueprintResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t ShaderBlueprintResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& ShaderBlueprintResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& ShaderBlueprintResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* ShaderBlueprintResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void ShaderBlueprintResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* ShaderBlueprintResourceManager::createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return mInternalResourceManager->createResourceLoaderInstance(resourceLoaderTypeId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	ShaderBlueprintResourceManager::ShaderBlueprintResourceManager(IRenderer& renderer) :
		mRenderer(renderer),
		mShaderCacheManager(*this)
	{
		mInternalResourceManager = new ResourceManagerTemplate<ShaderBlueprintResource, ShaderBlueprintResourceLoader, ShaderBlueprintResourceId, 64>(renderer, *this);

		// Gather RHI shader properties
		// -> Write the RHI name as well as the shader language name into the shader properties so shaders can perform RHI specific handling if required
		// -> We really need both, usually shader language name is sufficient, but if more fine granular information is required it's accessible
		Rhi::IRhi& rhi = mRenderer.getRhi();
		const Rhi::Capabilities& capabilities = rhi.getCapabilities();
		mRhiShaderProperties.setPropertyValue(static_cast<uint32_t>(rhi.getNameId()), 1);
		mRhiShaderProperties.setPropertyValue(STRING_ID("ZeroToOneClipZ"), capabilities.zeroToOneClipZ ? 1 : 0);
		mRhiShaderProperties.setPropertyValue(STRING_ID("UpperLeftOrigin"), capabilities.upperLeftOrigin ? 1 : 0);
		mRhiShaderProperties.setPropertyValue(StringId(rhi.getDefaultShaderLanguage().getShaderLanguageName()), 1);
	}

	ShaderBlueprintResourceManager::~ShaderBlueprintResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

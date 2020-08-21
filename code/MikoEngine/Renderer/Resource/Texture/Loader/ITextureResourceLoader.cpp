#include "stdafx.h"
#include "Renderer/Resource/Texture/Loader/ITextureResourceLoader.h"
#include "Renderer/Resource/Texture/TextureResource.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void ITextureResourceLoader::initialize(const Asset& asset, bool reload, IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mTextureResource = static_cast<TextureResource*>(&resource);
	}

	bool ITextureResourceLoader::onDispatch()
	{
		// Create the RHI texture instance
		mTextureResource->mTexture = (mRenderer.getRhi().getCapabilities().nativeMultithreading ? mTexture : createRhiTexture());

		// Fully loaded
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

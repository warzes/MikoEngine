#include "stdafx.h"
#include "Renderer/Resource/Mesh/Loader/IMeshResourceLoader.h"
#include "Renderer/Resource/Mesh/MeshResource.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void IMeshResourceLoader::initialize(const Asset& asset, bool reload, IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mMeshResource = static_cast<MeshResource*>(&resource);
	}

	bool IMeshResourceLoader::isFullyLoaded()
	{
		// Fully loaded?
		const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
		const SubMeshes& subMeshes = mMeshResource->getSubMeshes();
		const uint32_t numberOfUsedSubMeshes = static_cast<uint32_t>(subMeshes.size());
		for (uint32_t i = 0; i < numberOfUsedSubMeshes; ++i)
		{
			if (IResource::LoadingState::LOADED != materialResourceManager.getResourceByResourceId(subMeshes[i].getMaterialResourceId()).getLoadingState())
			{
				// Not fully loaded
				return false;
			}
		}

		// Fully loaded
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

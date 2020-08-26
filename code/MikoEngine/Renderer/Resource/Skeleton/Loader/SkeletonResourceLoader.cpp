#include "stdafx.h"
#include "Renderer/Resource/Skeleton/Loader/SkeletonResourceLoader.h"
#include "Renderer/Resource/Skeleton/Loader/SkeletonFileFormat.h"
#include "Renderer/Resource/Skeleton/SkeletonResource.h"
#include "Core/File/IFile.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void SkeletonResourceLoader::initialize(const Asset& asset, bool reload, IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mSkeletonResource = static_cast<SkeletonResource*>(&resource);
	}

	bool SkeletonResourceLoader::onDeserialization(IFile&)
	{
		// TODO(co) Right now, there's no standalone skeleton asset, only the skeleton which is part of a mesh. When there's one, don't forget to use LZ4 compression.
		// Tell the memory mapped file about the LZ4 compressed data
		// mMemoryFile.loadLz4CompressedDataFromFile(v1Skeleton::FORMAT_TYPE, v1Skeleton::FORMAT_VERSION, file);

		// Done
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

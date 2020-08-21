#include "stdafx.h"
#include "Renderer/Resource/CompositorWorkspace/Loader/CompositorWorkspaceResourceLoader.h"
#include "Renderer/Resource/CompositorWorkspace/Loader/CompositorWorkspaceFileFormat.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceResource.h"


// TODO(co) Error handling


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
		void nodesDeserialization(Renderer::IFile& file, Renderer::CompositorWorkspaceResource& compositorWorkspaceResource)
		{
			// Read in the compositor workspace resource nodes
			Renderer::v1CompositorWorkspace::Nodes nodes;
			file.read(&nodes, sizeof(Renderer::v1CompositorWorkspace::Nodes));

			// Sanity check
			RHI_ASSERT(nodes.numberOfNodes > 0, "Invalid compositor workspace asset without any nodes detected")

			// Read in the compositor node asset IDs
			compositorWorkspaceResource.reserveCompositorNodes(nodes.numberOfNodes);
			// TODO(co) Get rid of the evil const-cast
			Renderer::CompositorWorkspaceResource::CompositorNodeAssetIds& compositorNodeAssetIds = const_cast<Renderer::CompositorWorkspaceResource::CompositorNodeAssetIds&>(compositorWorkspaceResource.getCompositorNodeAssetIds());
			compositorNodeAssetIds.resize(nodes.numberOfNodes);
			file.read(compositorNodeAssetIds.data(), sizeof(Renderer::AssetId) * nodes.numberOfNodes);
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
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void CompositorWorkspaceResourceLoader::initialize(const Asset& asset, bool reload, IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mCompositorWorkspaceResource = static_cast<CompositorWorkspaceResource*>(&resource);
	}

	bool CompositorWorkspaceResourceLoader::onDeserialization(IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1CompositorWorkspace::FORMAT_TYPE, v1CompositorWorkspace::FORMAT_VERSION, file);
	}

	void CompositorWorkspaceResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the compositor workspace header
		// TODO(co) Currently the compositor workspace header is unused
		v1CompositorWorkspace::CompositorWorkspaceHeader compositorWorkspaceHeader;
		mMemoryFile.read(&compositorWorkspaceHeader, sizeof(v1CompositorWorkspace::CompositorWorkspaceHeader));

		// Read in the compositor workspace resource nodes
		::detail::nodesDeserialization(mMemoryFile, *mCompositorWorkspaceResource);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

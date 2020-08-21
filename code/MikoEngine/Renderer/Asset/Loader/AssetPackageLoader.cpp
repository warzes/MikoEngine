#include "stdafx.h"
#include "Renderer/Asset/Loader/AssetPackageLoader.h"
#include "Renderer/Asset/Loader/AssetPackageFileFormat.h"
#include "Renderer/Asset/AssetPackage.h"
#include "Renderer/Core/File/MemoryFile.h"

namespace Renderer
{
	void AssetPackageLoader::loadAssetPackage(AssetPackage& assetPackage, IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data and decompress it at once
		MemoryFile memoryFile;
		if ( memoryFile.loadLz4CompressedDataFromFile(v1AssetPackage::FORMAT_TYPE, v1AssetPackage::FORMAT_VERSION, file) )
		{
			memoryFile.decompress();

			// Read in the asset package header
			v1AssetPackage::AssetPackageHeader assetPackageHeader;
			memoryFile.read(&assetPackageHeader, sizeof(v1AssetPackage::AssetPackageHeader));

			// Sanity check
			RHI_ASSERT(assetPackageHeader.numberOfAssets > 0, "Invalid empty asset package detected");

			// Read in the asset package content in one single burst
			AssetPackage::SortedAssetVector& sortedAssetVector = assetPackage.getWritableSortedAssetVector();
			sortedAssetVector.resize(assetPackageHeader.numberOfAssets);
			memoryFile.read(sortedAssetVector.data(), sizeof(Asset) * assetPackageHeader.numberOfAssets);
		}
	}
} // Renderer
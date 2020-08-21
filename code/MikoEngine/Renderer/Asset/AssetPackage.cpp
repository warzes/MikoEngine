#include "stdafx.h"
#include "Renderer/Asset/AssetPackage.h"
#include "Renderer/Core/Math/Math.h"
#include "Renderer/Context.h"

namespace
{
	namespace detail
	{
		struct OrderByAssetId final
		{
			[[nodiscard]] inline bool operator()(const Renderer::Asset& left, Renderer::AssetId right) const
			{
				return (left.assetId < right);
			}

			[[nodiscard]] inline bool operator()(Renderer::AssetId left, const Renderer::Asset& right) const
			{
				return (left < right.assetId);
			}
		};
	} // detail
}

namespace Renderer
{
	void AssetPackage::addAsset([[maybe_unused]] const Context& context, AssetId assetId, VirtualFilename virtualFilename)
	{
		RHI_ASSERT(nullptr == tryGetAssetByAssetId(assetId), "Renderer asset ID is already used");
		RHI_ASSERT(strlen(virtualFilename) < Asset::MAXIMUM_ASSET_FILENAME_LENGTH, "The renderer asset filename is too long");
		SortedAssetVector::const_iterator iterator = std::lower_bound(mSortedAssetVector.cbegin(), mSortedAssetVector.cend(), assetId, ::detail::OrderByAssetId());
		Asset& asset = *mSortedAssetVector.insert(iterator, Asset());
		asset.assetId = assetId;
		strncpy(asset.virtualFilename, virtualFilename, Asset::MAXIMUM_ASSET_FILENAME_LENGTH - 1);	// -1 not including the terminating zero
	}

	const Asset* AssetPackage::tryGetAssetByAssetId(AssetId assetId) const
	{
		SortedAssetVector::const_iterator iterator = std::lower_bound(mSortedAssetVector.cbegin(), mSortedAssetVector.cend(), assetId, ::detail::OrderByAssetId());
		return (iterator != mSortedAssetVector.cend() && iterator->assetId == assetId) ? &(*iterator) : nullptr;
	}

	bool AssetPackage::validateIntegrity(const IFileManager& fileManager) const
	{
		for ( const Asset& asset : mSortedAssetVector )
		{
			if ( Math::calculateFileFNV1a64ByVirtualFilename(fileManager, asset.virtualFilename) != asset.fileHash )
			{
				// Invalid integrity
				return false;
			}
		}

		// Valid integrity
		return true;
	}

	Asset* AssetPackage::tryGetWritableAssetByAssetId(AssetId assetId)
	{
		SortedAssetVector::iterator iterator = std::lower_bound(mSortedAssetVector.begin(), mSortedAssetVector.end(), assetId, ::detail::OrderByAssetId());
		return (iterator != mSortedAssetVector.cend() && iterator->assetId == assetId) ? &(*iterator) : nullptr;
	}
} // Renderer
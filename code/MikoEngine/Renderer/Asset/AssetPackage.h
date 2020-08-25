#pragma once


#include "Renderer/Asset/Asset.h"
#include "Core/GetInvalid.h"

namespace Renderer
{
	class Context;
	class IFileManager;
}

namespace Renderer
{
	typedef StringId AssetPackageId;		// Resource loader type identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset package name>"
	typedef const char* VirtualFilename;	// UTF-8 virtual filename, the virtual filename scheme is "<mount point = project name>/<asset directory>/<asset name>.<file extension>" (example "Example/Mesh/Monster/Squirrel.mesh"), never ever a null pointer and always finished by a terminating zero

	class AssetPackage final
	{
		friend class AssetManager;
	public:
		typedef std::vector<Asset> SortedAssetVector;

		inline AssetPackage() :
			mAssetPackageId(GetInvalid<AssetPackageId>())
		{
			// Nothing here
		}

		inline explicit AssetPackage(AssetPackageId assetPackageId) :
			mAssetPackageId(assetPackageId)
		{
			// Nothing here
		}

		[[nodiscard]] inline AssetPackageId getAssetPackageId() const
		{
			return mAssetPackageId;
		}

		inline void clear()
		{
			mSortedAssetVector.clear();
		}

		[[nodiscard]] inline const SortedAssetVector& getSortedAssetVector() const
		{
			return mSortedAssetVector;
		}

		void addAsset(const Context& context, AssetId assetId, VirtualFilename virtualFilename);
		[[nodiscard]] const Asset* tryGetAssetByAssetId(AssetId assetId) const;

		[[nodiscard]] inline VirtualFilename tryGetVirtualFilenameByAssetId(AssetId assetId) const
		{
			const Asset* asset = tryGetAssetByAssetId(assetId);
			return (nullptr != asset) ? asset->virtualFilename : nullptr;
		}

		[[nodiscard]] bool validateIntegrity(const IFileManager& fileManager) const;

		// For internal use only (exposed for API performance reasons)
		[[nodiscard]] inline SortedAssetVector& getWritableSortedAssetVector()
		{
			return mSortedAssetVector;
		}
		[[nodiscard]] Asset* tryGetWritableAssetByAssetId(AssetId assetId);

	private:
		explicit AssetPackage(const AssetPackage&) = delete;
		AssetPackage& operator=(const AssetPackage&) = delete;

		AssetPackageId	  mAssetPackageId;
		SortedAssetVector mSortedAssetVector;	// Sorted vector of assets
	};
} // Renderer
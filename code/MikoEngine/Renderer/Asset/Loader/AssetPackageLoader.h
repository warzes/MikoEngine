#pragma once

class IFile;

namespace Renderer
{
	class AssetPackage;
}

namespace Renderer
{
	class AssetPackageLoader final
	{
		friend class AssetManager;
	private:
		AssetPackageLoader() = default;
		~AssetPackageLoader() = default;

		explicit AssetPackageLoader(const AssetPackageLoader&) = delete;
		AssetPackageLoader& operator=(const AssetPackageLoader&) = delete;

		void loadAssetPackage(AssetPackage& assetPackage, IFile& file);
	};
} // Renderer
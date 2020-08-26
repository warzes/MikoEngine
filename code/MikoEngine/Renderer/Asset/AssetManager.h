#pragma once

#include "Renderer/Core/Manager.h"
#include "Renderer/Asset/Asset.h"

namespace Renderer
{
	class AssetPackage;
	class IRenderer;
}

namespace Renderer
{
	typedef StringId AssetPackageId;			// Resource loader type identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset package name>"
	typedef const char* VirtualFilename;		// UTF-8 virtual filename, the virtual filename scheme is "<mount point = project name>/<asset directory>/<asset name>.<file extension>" (example "Example/Mesh/Monster/Squirrel.mesh"), never ever a null pointer and always finished by a terminating zero
	typedef const char* AbsoluteDirectoryName;	// UTF-8 absolute directory name (example: "c:/MyProject"), without "/" at the end, never ever a null pointer and always finished by a terminating zero

	class AssetManager final : private Manager
	{
		friend class RendererImpl;
	public:
		typedef std::vector<AssetPackage*> AssetPackageVector;

		//[-------------------------------------------------------]
		//[ Asset package                                         ]
		//[-------------------------------------------------------]
		void clear();
		[[nodiscard]] AssetPackage& addAssetPackage(AssetPackageId assetPackageId);
		AssetPackage* mountAssetPackage(AbsoluteDirectoryName absoluteDirectoryName, const char* projectName);
		[[nodiscard]] AssetPackage& getAssetPackageById(AssetPackageId assetPackageId) const;
		[[nodiscard]] AssetPackage* tryGetAssetPackageById(AssetPackageId assetPackageId) const;
		void removeAssetPackage(AssetPackageId assetPackageId);

		//[-------------------------------------------------------]
		//[ Asset                                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] const Asset* tryGetAssetByAssetId(AssetId assetId) const;

		[[nodiscard]] inline const Asset& getAssetByAssetId(AssetId assetId) const
		{
			const Asset* asset = tryGetAssetByAssetId(assetId);
			SE_ASSERT(nullptr != asset, "Invalid asset")
				return *asset;
		}

		[[nodiscard]] inline VirtualFilename tryGetVirtualFilenameByAssetId(AssetId assetId) const
		{
			const Asset* asset = tryGetAssetByAssetId(assetId);
			return (nullptr != asset) ? asset->virtualFilename : nullptr;
		}

	private:
		inline explicit AssetManager(IRenderer& renderer) :
			mRenderer(renderer)
		{
			// Nothing here
		}

		inline ~AssetManager()
		{
			clear();
		}

		explicit AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;

		[[nodiscard]] AssetPackage* addAssetPackageByVirtualFilename(AssetPackageId assetPackageId, VirtualFilename virtualFilename);

		IRenderer&		   mRenderer;	// Renderer instance, do not destroy the instance
		AssetPackageVector mAssetPackageVector;
	};
} // Renderer
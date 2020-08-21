#pragma once

#include "Renderer/Core/StringId.h"

namespace Renderer
{
	// Asset package file format content:
	// - File format header
	// - Asset package header
	// - Assets
	namespace v1AssetPackage
	{
		static constexpr uint32_t FORMAT_TYPE = STRING_ID("AssetPackage");
		static constexpr uint32_t FORMAT_VERSION = 3;

#pragma pack(push)
#pragma pack(1)
		struct AssetPackageHeader final
		{
			uint32_t numberOfAssets;
		};
#pragma pack(pop)
	} // v1AssetPackage
} // Renderer
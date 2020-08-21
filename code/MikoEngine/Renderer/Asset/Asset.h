#pragma once

#include "Renderer/Core/StringId.h"

namespace Renderer
{
	typedef StringId AssetId;	// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"

	/**
	*  @brief
	*    Asset class
	*
	*  @remarks
	*    The asset reference table is always kept in memory so we have to implement it in an efficient way.
	*    No "std::string" by intent to be cache friendly and avoid memory trashing, which is important here.
	*    140 bytes per asset might sound not much, but when having e.g. 30.000 assets which is not unusual for a
	*    more complex project, you end up in having a 4 MiB asset reference table in memory.
	*/
	struct Asset final
	{
		static constexpr uint32_t MAXIMUM_ASSET_FILENAME_LENGTH = 127 + 1;	// +1 for the terminating zero

		AssetId  assetId;											// Asset ID
		uint64_t fileHash;											// 64-bit FNV-1a hash of the asset file
		char	 virtualFilename[MAXIMUM_ASSET_FILENAME_LENGTH];	// Virtual asset UTF-8 filename inside the asset package, including terminating zero
	};

} // Renderer

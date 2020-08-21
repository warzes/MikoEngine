#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/StringId.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;	// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"


	// Compositor workspace file format content:
	// - File format header
	// - Compositor workspace header
	namespace v1CompositorWorkspace
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = STRING_ID("CompositorWorkspace");
		static constexpr uint32_t FORMAT_VERSION = 2;

		#pragma pack(push)
		#pragma pack(1)
			struct CompositorWorkspaceHeader final
			{
				uint32_t unused;	// TODO(co) Currently the compositor workspace header is unused
			};

			struct Nodes final
			{
				uint32_t numberOfNodes;
			};

			struct Node final
			{
				AssetId compositorNodeAssetId;
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1CompositorWorkspace
} // Renderer

#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	// Rigid skeleton file format content:
	// - Skeleton header
	// - Skeleton data block
	namespace v1Skeleton
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = STRING_ID("Skeleton");
		static constexpr uint32_t FORMAT_VERSION = 1;

		#pragma pack(push)
		#pragma pack(1)
			struct Header final
			{
				uint32_t formatType;
				uint32_t formatVersion;
				// TODO(co) Right now, there's no standalone skeleton asset, only the skeleton which is part of a mesh. When there's one, don't forget to use LZ4 compression.
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1Skeleton
} // Renderer

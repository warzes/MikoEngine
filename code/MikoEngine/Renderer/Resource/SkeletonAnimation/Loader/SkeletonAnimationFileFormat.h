#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	// Rigid skeleton animation clip file format content:
	// - File format header
	// - Skeleton animation header
	// - Bone IDs
	// - ACL ( https://github.com/nfrechette/acl ) compressed skeleton animation clip
	namespace v1SkeletonAnimation
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = STRING_ID("SkeletonAnimation");
		static constexpr uint32_t FORMAT_VERSION = 3;

		#pragma pack(push)
		#pragma pack(1)
			struct SkeletonAnimationHeader final
			{
				uint8_t  numberOfChannels;		// The number of bone animation channels; each channel affects a single bone
				float	 durationInTicks;		// Duration of the animation in ticks
				float	 ticksPerSecond;		// Ticks per second; 0 if not specified in the imported file
				uint32_t aclCompressedClipSize;	// ACL compressed clip size in bytes
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1SkeletonAnimation
} // Renderer

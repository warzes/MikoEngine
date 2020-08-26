#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResource.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	template <class ELEMENT_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class PackedElementManager;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
	class SkeletonAnimationResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t SkeletonAnimationResourceId;	// POD skeleton animation resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Rigid skeleton animation clip resource
	*/
	class SkeletonAnimationResource final : public IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SkeletonAnimationResourceLoader;
		friend PackedElementManager<SkeletonAnimationResource, SkeletonAnimationResourceId, 2048>;										// Type definition of template class
		friend ResourceManagerTemplate<SkeletonAnimationResource, SkeletonAnimationResourceLoader, SkeletonAnimationResourceId, 2048>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline uint8_t getNumberOfChannels() const
		{
			return mNumberOfChannels;
		}

		[[nodiscard]] inline float getDurationInTicks() const
		{
			return mDurationInTicks;
		}

		[[nodiscard]] inline float getTicksPerSecond() const
		{
			return mTicksPerSecond;
		}

		[[nodiscard]] inline const std::vector<uint32_t>& getBoneIds() const
		{
			return mBoneIds;
		}

		[[nodiscard]] inline const std::vector<uint8_t>& getAclCompressedClip() const
		{
			return mAclCompressedClip;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline SkeletonAnimationResource() :
			mNumberOfChannels(0),
			mDurationInTicks(0.0f),
			mTicksPerSecond(0.0f)
		{
			// Nothing here
		}

		inline virtual ~SkeletonAnimationResource() override
		{
			// Sanity checks
			SE_ASSERT(0 == mNumberOfChannels, "Invalid number of channels")
			SE_ASSERT(0.0f == mDurationInTicks, "Invalid duration in ticks")
			SE_ASSERT(0.0f == mTicksPerSecond, "Invalid ticks per second")
		}

		explicit SkeletonAnimationResource(const SkeletonAnimationResource&) = delete;
		SkeletonAnimationResource& operator=(const SkeletonAnimationResource&) = delete;

		inline void clearSkeletonAnimationData()
		{
			mNumberOfChannels = 0;
			mDurationInTicks  = 0.0f;
			mTicksPerSecond   = 0.0f;
		}

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(SkeletonAnimationResourceId skeletonAnimationResourceId)
		{
			// Sanity checks
			SE_ASSERT(0 == mNumberOfChannels, "Invalid number of channels")
			SE_ASSERT(0.0f == mDurationInTicks, "Invalid duration in ticks")
			SE_ASSERT(0.0f == mTicksPerSecond, "Invalid ticks per second")

			// Call base implementation
			IResource::initializeElement(skeletonAnimationResourceId);
		}

		inline void deinitializeElement()
		{
			// Reset everything
			clearSkeletonAnimationData();

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint8_t				  mNumberOfChannels;	// The number of bone animation channels; each channel affects a single bone
		float				  mDurationInTicks;		// Duration of the animation in ticks
		float				  mTicksPerSecond;		// Ticks per second; 0 if not specified in the imported file
		std::vector<uint32_t> mBoneIds;
		std::vector<uint8_t>  mAclCompressedClip;	// ACL ( https://github.com/nfrechette/acl ) compressed skeleton animation clip


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#include "stdafx.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationEvaluator.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationResourceManager.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationResource.h"
#include "Renderer/Core/Math/Math.h"
#include "RHI/DefaultAllocator.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4061)	// warning C4061: enumerator 'rtm::mix4::b' in switch of enum 'rtm::mix4' is not explicitly handled by a case label
	SE_PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'initializing': conversion from 'int' to 'uint8_t', signed/unsigned mismatch
	SE_PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'rtm::rtm_impl::matrix_caster<rtm::matrix3x3f>': move assignment operator was implicitly defined as deleted
	#include <acl/algorithm/uniformly_sampled/decoder.h>
SE_PRAGMA_WARNING_POP

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/gtx/quaternion.hpp>
	#include <glm/gtc/matrix_transform.hpp>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		typedef acl::uniformly_sampled::DecompressionContext<acl::uniformly_sampled::DefaultDecompressionSettings> AclDecompressionContext;


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class AclAllocator final : public acl::IAllocator
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			inline explicit AclAllocator()
			{
				// Nothing here
			}

			explicit AclAllocator(const AclAllocator&) = delete;

			virtual ~AclAllocator() override
			{
				// Nothing here
			}

			AclAllocator& operator=(const AclAllocator&) = delete;


		//[-------------------------------------------------------]
		//[ Public virtual acl::IAllocator methods                ]
		//[-------------------------------------------------------]
		public:
			virtual void* allocate(size_t size, size_t alignment = k_default_alignment) override
			{
				return GetAllocator().reallocate(nullptr, 0, size, alignment);
			}

			virtual void deallocate(void* ptr, size_t size) override
			{
				GetAllocator().reallocate(ptr, size, 0, 1);
			}
	};


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SkeletonAnimationEvaluator::SkeletonAnimationEvaluator(SkeletonAnimationResourceManager& skeletonAnimationResourceManager, SkeletonAnimationResourceId skeletonAnimationResourceId) :
		mSkeletonAnimationResourceManager(skeletonAnimationResourceManager),
		mSkeletonAnimationResourceId(skeletonAnimationResourceId)
	{
		mAclAllocator = new ::detail::AclAllocator();
		mAclDecompressionContext = new ::detail::AclDecompressionContext();
//		mAclDecompressionContext = new ::detail::AclDecompressionContext(static_cast<acl::IAllocator*>(mAclAllocator));	// TODO(co) When using ACL decompression context with custom allocator there are reports about damaged blocks on destruction
		const SkeletonAnimationResource& skeletonAnimationResource = mSkeletonAnimationResourceManager.getById(mSkeletonAnimationResourceId);
		static_cast<::detail::AclDecompressionContext*>(mAclDecompressionContext)->initialize(*reinterpret_cast<const acl::CompressedClip*>(skeletonAnimationResource.getAclCompressedClip().data()));
		mBoneIds = skeletonAnimationResource.getBoneIds();
		mTransformMatrices.resize(skeletonAnimationResource.getNumberOfChannels());
	}

	SkeletonAnimationEvaluator::~SkeletonAnimationEvaluator()
	{
		delete static_cast<::detail::AclDecompressionContext*>(mAclDecompressionContext);
		delete static_cast<::detail::AclAllocator*>(mAclAllocator);
	}

	void SkeletonAnimationEvaluator::evaluate(float timeInSeconds)
	{
		const SkeletonAnimationResource& skeletonAnimationResource = mSkeletonAnimationResourceManager.getById(mSkeletonAnimationResourceId);
		const uint8_t numberOfChannels = skeletonAnimationResource.getNumberOfChannels();

		// Decompress the ACL compressed skeleton animation clip
		::detail::AclDecompressionContext* aclDecompressionContext = static_cast<::detail::AclDecompressionContext*>(mAclDecompressionContext);
		const float duration = skeletonAnimationResource.getDurationInTicks() / skeletonAnimationResource.getTicksPerSecond();
		while (timeInSeconds > duration)
		{
			timeInSeconds -= duration;
		}
		static_cast<::detail::AclDecompressionContext*>(mAclDecompressionContext)->seek(timeInSeconds, acl::sample_rounding_policy::none);
		for (uint8_t i = 0; i < numberOfChannels; ++i)
		{
			rtm::quatf rotation;
			rtm::vector4f translation;
			rtm::vector4f scale;
			aclDecompressionContext->decompress_bone(i, &rotation, &translation, &scale);

			// Build a transformation matrix from it
			// TODO(co) Handle case of no scale
			// TODO(co) Review temporary matrix instances on the C-runtime stack
			glm::quat presentRotation(rtm::quat_get_w(rotation), rtm::quat_get_x(rotation), rtm::quat_get_y(rotation), rtm::quat_get_z(rotation));
			glm::vec3 presentPosition(rtm::vector_get_x(translation), rtm::vector_get_y(translation), rtm::vector_get_z(translation));
			glm::vec3 presentScale(rtm::vector_get_x(scale), rtm::vector_get_y(scale), rtm::vector_get_z(scale));
			mTransformMatrices[i] = glm::translate(Math::MAT4_IDENTITY, presentPosition) * glm::toMat4(presentRotation) * glm::scale(Math::MAT4_IDENTITY, presentScale);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

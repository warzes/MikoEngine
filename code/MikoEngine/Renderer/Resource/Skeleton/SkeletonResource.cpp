#include "stdafx.h"
#include "Renderer/Resource/Skeleton/SkeletonResource.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	SE_PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/glm.hpp>
	#include <glm/gtx/dual_quaternion.hpp>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	uint32_t SkeletonResource::getBoneIndexByBoneId(uint32_t boneId) const
	{
		// TODO(co) Maybe it makes sense to store the bone IDs in some order to speed up the following
		for (uint8_t boneIndex = 0; boneIndex < mNumberOfBones; ++boneIndex)
		{
			if (mBoneIds[boneIndex] == boneId)
			{
				return boneIndex;
			}
		}
		return getInvalid<uint32_t>();
	}

	void SkeletonResource::localToGlobalPose()
	{
		// The root has no parent
		mGlobalBoneMatrices[0] = mLocalBoneMatrices[0];

		// Due to cache friendly depth-first rolled up bone hierarchy, the global parent bone pose is already up-to-date
		// TODO(co) Ensure that in the end SIMD intrinsics in GLM are used in here
		for (uint8_t i = 1; i < mNumberOfBones; ++i)
		{
			mGlobalBoneMatrices[i] = mGlobalBoneMatrices[mBoneParentIndices[i]] * mLocalBoneMatrices[i];
		}

		/*
		{ // Linear blend skinning (LBS) using matrices; there's no runtime switch by intent since dual quaternion skinning (DQS) is the way to go, don't remove this reference comment
			glm::mat3x4* boneSpaceMatrices = reinterpret_cast<glm::mat3x4*>(mBoneSpaceData);
			for (uint8_t i = 0; i < mNumberOfBones; ++i)
			{
				boneSpaceMatrices[i] = glm::transpose(mGlobalBoneMatrices[i] * mBoneOffsetMatrices[i]);
			}
		}
		*/

		{ // The dual quaternion skinning (DQS) implementation is basing on https://gamedev.stackexchange.com/questions/164423/help-with-dual-quaternion-skinning
			glm::dualquat* boneSpaceDualQuaternions = reinterpret_cast<glm::dualquat*>(mBoneSpaceData);
			for (uint8_t i = 0; i < mNumberOfBones; ++i)
			{
				const glm::mat4 boneSpaceMatrix = mGlobalBoneMatrices[i] * mBoneOffsetMatrices[i];
				const glm::quat rotationQuaternion = glm::quat_cast(boneSpaceMatrix);
				const glm::vec4& translation = boneSpaceMatrix[3];
				glm::dualquat& boneSpaceDualQuaternion = boneSpaceDualQuaternions[i];
				boneSpaceDualQuaternion[0] = rotationQuaternion;
				boneSpaceDualQuaternion[1] = glm::quat(0.0f, translation.x, translation.y, translation.z) * rotationQuaternion * 0.5f;
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

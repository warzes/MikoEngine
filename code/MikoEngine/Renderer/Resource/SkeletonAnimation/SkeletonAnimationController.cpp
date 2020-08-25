#include "stdafx.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationController.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationEvaluator.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationResourceManager.h"
#include "Renderer/Resource/Skeleton/SkeletonResourceManager.h"
#include "Renderer/Resource/Skeleton/SkeletonResource.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Context.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void SkeletonAnimationController::startSkeletonAnimationByResourceId(SkeletonAnimationResourceId skeletonAnimationResourceId)
	{
		clear();
		mSkeletonAnimationResourceId = skeletonAnimationResourceId;
		if (IsValid(skeletonAnimationResourceId))
		{
			mRenderer.getSkeletonAnimationResourceManager().getResourceByResourceId(skeletonAnimationResourceId).connectResourceListener(*this);
		}
	}

	void SkeletonAnimationController::startSkeletonAnimationByAssetId(AssetId skeletonAnimationAssetId)
	{
		clear();
		mRenderer.getSkeletonAnimationResourceManager().loadSkeletonAnimationResourceByAssetId(skeletonAnimationAssetId, mSkeletonAnimationResourceId, this);
	}

	void SkeletonAnimationController::clear()
	{
		if (IsValid(mSkeletonAnimationResourceId))
		{
			disconnectFromResourceById(mSkeletonAnimationResourceId);
			SetInvalid(mSkeletonAnimationResourceId);
		}
		destroySkeletonAnimationEvaluator();
		mTimeInSeconds = 0.0f;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	void SkeletonAnimationController::onLoadingStateChange(const IResource& resource)
	{
		if (resource.getLoadingState() == IResource::LoadingState::LOADED)
		{
			createSkeletonAnimationEvaluator();
		}
		else
		{
			destroySkeletonAnimationEvaluator();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SkeletonAnimationController::createSkeletonAnimationEvaluator()
	{
		RHI_ASSERT(nullptr == mSkeletonAnimationEvaluator, "No useless update calls, please")
		SkeletonAnimationResourceManager& skeletonAnimationResourceManager = mRenderer.getSkeletonAnimationResourceManager();
		mSkeletonAnimationEvaluator = new SkeletonAnimationEvaluator(skeletonAnimationResourceManager, mSkeletonAnimationResourceId);

		// Register skeleton animation controller
		skeletonAnimationResourceManager.mSkeletonAnimationControllers.push_back(this);
	}

	void SkeletonAnimationController::destroySkeletonAnimationEvaluator()
	{
		if (nullptr != mSkeletonAnimationEvaluator)
		{
			{ // Unregister skeleton animation controller
				SkeletonAnimationResourceManager::SkeletonAnimationControllers& skeletonAnimationControllers = mRenderer.getSkeletonAnimationResourceManager().mSkeletonAnimationControllers;
				SkeletonAnimationResourceManager::SkeletonAnimationControllers::iterator iterator = std::find(skeletonAnimationControllers.begin(), skeletonAnimationControllers.end(), this);
				RHI_ASSERT(iterator != skeletonAnimationControllers.end(), "Invalid skeleton animation controller")
				skeletonAnimationControllers.erase(iterator);
			}

			// Destroy skeleton animation evaluator
			delete mSkeletonAnimationEvaluator;
			mSkeletonAnimationEvaluator = nullptr;
		}
	}

	void SkeletonAnimationController::update(float pastSecondsSinceLastFrame)
	{
		// Sanity check
		RHI_ASSERT(pastSecondsSinceLastFrame > 0.0f, "No negative time, please")
		RHI_ASSERT(nullptr != mSkeletonAnimationEvaluator, "No useless update calls, please")

		// Advance time and evaluate state
		mTimeInSeconds += pastSecondsSinceLastFrame;
		mSkeletonAnimationEvaluator->evaluate(mTimeInSeconds);

		{ // Tell the controlled skeleton resource about the new state
			SkeletonResource& skeletonResource = mRenderer.getSkeletonResourceManager().getById(mSkeletonResourceId);
			const SkeletonAnimationEvaluator::BoneIds& boneIds = mSkeletonAnimationEvaluator->getBoneIds();
			const SkeletonAnimationEvaluator::TransformMatrices& transformMatrices = mSkeletonAnimationEvaluator->getTransformMatrices();
			glm::mat4* localBoneMatrices = skeletonResource.getLocalBoneMatrices();
			for (size_t i = 0; i < boneIds.size(); ++i)
			{
				const uint32_t boneIndex = skeletonResource.getBoneIndexByBoneId(boneIds[i]);
				if (IsValid(boneIndex))
				{
					localBoneMatrices[boneIndex] = transformMatrices[i];
				}
			}
			skeletonResource.localToGlobalPose();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

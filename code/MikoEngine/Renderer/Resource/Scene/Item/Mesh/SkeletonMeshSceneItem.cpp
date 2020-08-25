#include "stdafx.h"
#include "Renderer/Resource/Scene/Item/Mesh/SkeletonMeshSceneItem.h"
#include "Renderer/Resource/Scene/Loader/SceneFileFormat.h"
#include "Renderer/Resource/Scene/SceneResource.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationController.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"
#include "Renderer/Resource/Mesh/MeshResource.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SkeletonResourceId SkeletonMeshSceneItem::getSkeletonResourceId() const
	{
		const MeshResource* meshResource = getSceneResource().getRenderer().getMeshResourceManager().tryGetById(getMeshResourceId());
		return (nullptr != meshResource) ? meshResource->getSkeletonResourceId() : GetInvalid<SkeletonResourceId>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	void SkeletonMeshSceneItem::deserialize(uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		RHI_ASSERT(sizeof(v1Scene::SkeletonMeshItem) <= numberOfBytes, "Invalid number of bytes")
		const v1Scene::SkeletonMeshItem* skeletonMeshItem = reinterpret_cast<const v1Scene::SkeletonMeshItem*>(data);

		// Read data
		mSkeletonAnimationAssetId = skeletonMeshItem->skeletonAnimationAssetId;

		// Call base implementation
		MeshSceneItem::deserialize(numberOfBytes - sizeof(v1Scene::SkeletonMeshItem), data + sizeof(v1Scene::SkeletonMeshItem));
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	SkeletonMeshSceneItem::~SkeletonMeshSceneItem()
	{
		// Destroy the skeleton animation controller instance, if needed
		delete mSkeletonAnimationController;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	void SkeletonMeshSceneItem::onLoadingStateChange(const IResource& resource)
	{
		// Create/destroy skeleton animation controller
		if (resource.getId() == getMeshResourceId())
		{
			if (resource.getLoadingState() == IResource::LoadingState::LOADED)
			{
				if (IsValid(mSkeletonAnimationAssetId))
				{
					RHI_ASSERT(nullptr == mSkeletonAnimationController, "Invalid skeleton animation controller")
					mSkeletonAnimationController = new SkeletonAnimationController(getSceneResource().getRenderer(), static_cast<const MeshResource&>(resource).getSkeletonResourceId());
					mSkeletonAnimationController->startSkeletonAnimationByAssetId(mSkeletonAnimationAssetId);
				}
			}
			else if (nullptr != mSkeletonAnimationController)
			{
				delete mSkeletonAnimationController;
				mSkeletonAnimationController = nullptr;
			}
		}

		// Call the base implementation
		MeshSceneItem::onLoadingStateChange(resource);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#include "stdafx.h"
#include "Renderer/Resource/Scene/Item/Mesh/MeshSceneItem.h"
#include "Renderer/Resource/Scene/Loader/SceneFileFormat.h"
#include "Renderer/Resource/Scene/Culling/SceneItemSet.h"
#include "Renderer/Resource/Scene/SceneResource.h"
#include "Renderer/Resource/Scene/SceneNode.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"
#include "Renderer/Resource/Mesh/MeshResource.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/IRenderer.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/gtx/component_wise.hpp>
SE_PRAGMA_WARNING_POP

#include <algorithm>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void MeshSceneItem::setMeshResourceId(MeshResourceId meshResourceId)
	{
		if (IsValid(mMeshResourceId))
		{
			disconnectFromResourceById(mMeshResourceId);
		}
		mMeshResourceId = meshResourceId;
		if (IsValid(meshResourceId))
		{
			getSceneResource().getRenderer().getMeshResourceManager().getResourceByResourceId(meshResourceId).connectResourceListener(*this);
		}
	}

	void MeshSceneItem::setMeshResourceIdByAssetId(AssetId meshAssetId)
	{
		if (IsValid(mMeshResourceId))
		{
			disconnectFromResourceById(mMeshResourceId);
		}
		getSceneResource().getRenderer().getMeshResourceManager().loadMeshResourceByAssetId(meshAssetId, mMeshResourceId, this);
	}

	void MeshSceneItem::setMaterialResourceIdOfSubMeshLod(uint32_t subMeshIndex, uint8_t lodIndex, MaterialResourceId materialResourceId)
	{
		SE_ASSERT(subMeshIndex < getNumberOfSubMeshes(), "Invalid sub mesh index")
		SE_ASSERT(lodIndex < mRenderableManager.getNumberOfLods(), "Invalid LOD index")
		mRenderableManager.getRenderables()[subMeshIndex + lodIndex * getNumberOfSubMeshes()].setMaterialResourceId(getSceneResource().getRenderer().getMaterialResourceManager(), materialResourceId);
	}

	void MeshSceneItem::setMaterialResourceIdOfAllSubMeshesAndLods(MaterialResourceId materialResourceId)
	{
		// The renderables contain all LODs, hence in here we just need to iterate through all renderables to get the job done
		const MaterialResourceManager& materialResourceManager = getSceneResource().getRenderer().getMaterialResourceManager();
		for (Renderable& renderable : mRenderableManager.getRenderables())
		{
			renderable.setMaterialResourceId(materialResourceManager, materialResourceId);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	void MeshSceneItem::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity checks
		SE_ASSERT(sizeof(v1Scene::MeshItem) <= numberOfBytes, "Invalid number of bytes")
		const v1Scene::MeshItem* meshItem = reinterpret_cast<const v1Scene::MeshItem*>(data);
		SE_ASSERT((sizeof(v1Scene::MeshItem) + sizeof(AssetId) * meshItem->numberOfSubMeshMaterialAssetIds) == numberOfBytes, "Invalid number of bytes")

		// Read data
		setMeshResourceIdByAssetId(meshItem->meshAssetId);
		if (meshItem->numberOfSubMeshMaterialAssetIds > 0)
		{
			mSubMeshMaterialAssetIds.resize(meshItem->numberOfSubMeshMaterialAssetIds);
			memcpy(mSubMeshMaterialAssetIds.data(), data + sizeof(v1Scene::MeshItem), sizeof(AssetId) * meshItem->numberOfSubMeshMaterialAssetIds);
		}
		else
		{
			mSubMeshMaterialAssetIds.clear();
		}
	}

	void MeshSceneItem::onAttachedToSceneNode(SceneNode& sceneNode)
	{
		mRenderableManager.setTransform(&sceneNode.getGlobalTransform());

		// Call the base implementation
		ISceneItem::onAttachedToSceneNode(sceneNode);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	void MeshSceneItem::onLoadingStateChange(const IResource& resource)
	{
		if (resource.getLoadingState() == IResource::LoadingState::LOADED)
		{
			// If mesh resource loading has been finished, setup the renderable manager
			if (resource.getId() == mMeshResourceId)
			{
				const MeshResource& meshResource = static_cast<const MeshResource&>(resource);
				RenderableManager::Renderables& renderables = mRenderableManager.getRenderables();
				renderables.clear();

				// Set scene item set bounding data
				// TODO(co) The following is just for culling kickoff and won't stay this way
				if (nullptr != mSceneItemSet)
				{
					const SceneNode* parentSceneNode = getParentSceneNode();

					{ // Set minimum object space bounding box corner position
						const glm::vec3& minimumBoundingBoxPosition = meshResource.getMinimumBoundingBoxPosition();
						mSceneItemSet->minimumX[mSceneItemSetIndex] = minimumBoundingBoxPosition.x;
						mSceneItemSet->minimumY[mSceneItemSetIndex] = minimumBoundingBoxPosition.y;
						mSceneItemSet->minimumZ[mSceneItemSetIndex] = minimumBoundingBoxPosition.z;
					}

					{ // Set maximum object space bounding box corner position
						const glm::vec3& maximumBoundingBoxPosition = meshResource.getMaximumBoundingBoxPosition();
						mSceneItemSet->maximumX[mSceneItemSetIndex] = maximumBoundingBoxPosition.x;
						mSceneItemSet->maximumY[mSceneItemSetIndex] = maximumBoundingBoxPosition.y;
						mSceneItemSet->maximumZ[mSceneItemSetIndex] = maximumBoundingBoxPosition.z;
					}

					{ // Set world space center position of bounding sphere
						const glm::vec3& boundingSpherePosition = meshResource.getBoundingSpherePosition();
						if (nullptr != parentSceneNode)
						{
							const glm::vec3& position = parentSceneNode->getTransform().position;
							const glm::vec3& scale = parentSceneNode->getTransform().scale;
							mSceneItemSet->spherePositionX[mSceneItemSetIndex] = boundingSpherePosition.x * scale.x + position.x;
							mSceneItemSet->spherePositionY[mSceneItemSetIndex] = boundingSpherePosition.y * scale.y + position.y;
							mSceneItemSet->spherePositionZ[mSceneItemSetIndex] = boundingSpherePosition.z * scale.z + position.z;
						}
						else
						{
							mSceneItemSet->spherePositionX[mSceneItemSetIndex] = boundingSpherePosition.x;
							mSceneItemSet->spherePositionY[mSceneItemSetIndex] = boundingSpherePosition.y;
							mSceneItemSet->spherePositionZ[mSceneItemSetIndex] = boundingSpherePosition.z;
						}
					}

					{ // Set negative world space radius of bounding sphere
						float boundingSphereRadius = meshResource.getBoundingSphereRadius();
						if (nullptr != parentSceneNode)
						{
							boundingSphereRadius *= glm::compMax(parentSceneNode->getTransform().scale);
						}
						mSceneItemSet->negativeRadius[mSceneItemSetIndex] = -boundingSphereRadius;
					}
				}

				// Fill renderable manager
				MaterialResourceManager& materialResourceManager = getSceneResource().getRenderer().getMaterialResourceManager();
				{
					#if SE_DEBUG
						const char* debugName = meshResource.getDebugName();
						mRenderableManager.setDebugName(debugName);
					#endif
					const Rhi::IVertexArrayPtr& vertexArrayPtr = meshResource.getVertexArrayPtr();
					const Rhi::IVertexArrayPtr& positionOnlyVertexArrayPtr = meshResource.getPositionOnlyVertexArrayPtr();
					const SkeletonResourceId skeletonResourceId = meshResource.getSkeletonResourceId();
					const SubMeshes& subMeshes = meshResource.getSubMeshes();
					const size_t numberOfSubMeshes = subMeshes.size();
					renderables.reserve(numberOfSubMeshes);
					for (size_t i = 0; i < numberOfSubMeshes; ++i)
					{
						const SubMesh& subMesh = subMeshes[i];
						renderables.emplace_back(mRenderableManager, vertexArrayPtr, positionOnlyVertexArrayPtr, materialResourceManager, subMesh.getMaterialResourceId(), skeletonResourceId, true, subMesh.getStartIndexLocation(), subMesh.getNumberOfIndices(), 1 RHI_RESOURCE_DEBUG_NAME((std::string(debugName) + "[SubMesh" + std::to_string(i) + ']').c_str()));
					}
					mRenderableManager.setNumberOfLods(meshResource.getNumberOfLods());
				}

				// Handle overwritten sub-meshes
				// -> In case the overwritten material resource is not yet fully loaded, the original material resource of the sub-mesh is temporarily used
				// -> In case there are more overwritten sub-meshes as there are sub-meshes, be error tolerant here (mesh assets might have been changed, but not updated scene assets in use)
				if (!mSubMeshMaterialAssetIds.empty())
				{
					const uint32_t numberOfMaterials = static_cast<uint32_t>(std::min(mSubMeshMaterialAssetIds.size(), mRenderableManager.getRenderables().size()));
					for (size_t i = 0; i < numberOfMaterials; ++i)
					{
						if (IsValid(mSubMeshMaterialAssetIds[i]))
						{
							MaterialResourceId materialResourceId = GetInvalid<MaterialResourceId>();
							materialResourceManager.loadMaterialResourceByAssetId(mSubMeshMaterialAssetIds[i], materialResourceId, this);
						}
					}
				}

				// Finalize the renderable manager by updating cached renderables data
				mRenderableManager.updateCachedRenderablesData();
			}
			else
			{
				// Overwritten sub-mesh material loaded now?
				// -> In case there are more overwritten sub-meshes as there are sub-meshes, be error tolerant here (mesh assets might have been changed, but not updated scene assets in use)
				const uint32_t numberOfMaterials = static_cast<uint32_t>(std::min(mSubMeshMaterialAssetIds.size(), mRenderableManager.getRenderables().size()));
				bool updateCachedRenderablesDataRequired = false;
				for (uint32_t i = 0; i < numberOfMaterials; ++i)
				{
					if (resource.getAssetId() == mSubMeshMaterialAssetIds[i])
					{
						mRenderableManager.getRenderables()[i].setMaterialResourceId(getSceneResource().getRenderer().getMaterialResourceManager(), resource.getId());

						// Don't break, multiple sub-meshes might use one and the same material resource
						updateCachedRenderablesDataRequired = true;
					}
				}

				// Finalize the renderable manager by updating cached renderables data
				if (updateCachedRenderablesDataRequired)
				{
					mRenderableManager.updateCachedRenderablesData();
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

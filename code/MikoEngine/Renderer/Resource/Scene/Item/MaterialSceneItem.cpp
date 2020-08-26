#include "stdafx.h"
#include "Renderer/Resource/Scene/Item/MaterialSceneItem.h"
#include "Renderer/Resource/Scene/Loader/SceneFileFormat.h"
#include "Renderer/Resource/Scene/SceneResource.h"
#include "Renderer/Resource/Scene/SceneNode.h"
#include "Renderer/Resource/Material/MaterialResource.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	void MaterialSceneItem::deserialize([[maybe_unused]] uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		SE_ASSERT(sizeof(v1Scene::MaterialItem) <= numberOfBytes, "Invalid number of bytes")

		// Read data
		const v1Scene::MaterialItem* materialItem = reinterpret_cast<const v1Scene::MaterialItem*>(data);
		SE_ASSERT(sizeof(v1Scene::MaterialItem) + sizeof(MaterialProperty) * materialItem->numberOfMaterialProperties == numberOfBytes, "Invalid number of bytes")
		mMaterialAssetId = materialItem->materialAssetId;
		mMaterialTechniqueId = materialItem->materialTechniqueId;
		mMaterialBlueprintAssetId = materialItem->materialBlueprintAssetId;

		{ // Read material properties
			// TODO(co) Get rid of the evil const-cast
			MaterialProperties::SortedPropertyVector& sortedPropertyVector = const_cast<MaterialProperties::SortedPropertyVector&>(mMaterialProperties.getSortedPropertyVector());
			sortedPropertyVector.resize(materialItem->numberOfMaterialProperties);
			memcpy(reinterpret_cast<char*>(sortedPropertyVector.data()), data + sizeof(v1Scene::MaterialItem), sizeof(MaterialProperty) * materialItem->numberOfMaterialProperties);
		}

		// Sanity checks
		SE_ASSERT(IsValid(mMaterialAssetId) || IsValid(mMaterialBlueprintAssetId), "Invalid data")
		SE_ASSERT(!(IsValid(mMaterialAssetId) && IsValid(mMaterialBlueprintAssetId)), "Invalid data")
	}

	void MaterialSceneItem::onAttachedToSceneNode(SceneNode& sceneNode)
	{
		mRenderableManager.setTransform(&sceneNode.getGlobalTransform());

		// Call the base implementation
		ISceneItem::onAttachedToSceneNode(sceneNode);
	}

	const RenderableManager* MaterialSceneItem::getRenderableManager() const
	{
		if (!IsValid(getMaterialResourceId()))
		{
			// TODO(co) Get rid of the nasty delayed initialization in here, including the evil const-cast. For this, full asynchronous material blueprint loading must work. See "TODO(co) Currently material blueprint resource loading is a blocking process.".
			const_cast<MaterialSceneItem*>(this)->initialize();
		}
		return &mRenderableManager;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	void MaterialSceneItem::initialize()
	{
		// Sanity checks
		SE_ASSERT(IsValid(mMaterialAssetId) || IsValid(mMaterialBlueprintAssetId), "Invalid data")
		SE_ASSERT(!(IsValid(mMaterialAssetId) && IsValid(mMaterialBlueprintAssetId)), "Invalid data")

		// Get parent material resource ID and initiate creating the material resource
		MaterialResourceManager& materialResourceManager = getSceneResource().getRenderer().getMaterialResourceManager();
		if (IsValid(mMaterialAssetId))
		{
			// Get or load material resource
			MaterialResourceId materialResourceId = GetInvalid<MaterialResourceId>();
			materialResourceManager.loadMaterialResourceByAssetId(mMaterialAssetId, materialResourceId, this);
		}
		else
		{
			// Get or load material blueprint resource
			const AssetId materialBlueprintAssetId = mMaterialBlueprintAssetId;
			if (IsValid(materialBlueprintAssetId))
			{
				MaterialResourceId parentMaterialResourceId = materialResourceManager.getMaterialResourceIdByAssetId(materialBlueprintAssetId);
				if (IsInvalid(parentMaterialResourceId))
				{
					parentMaterialResourceId = materialResourceManager.createMaterialResourceByAssetId(materialBlueprintAssetId, materialBlueprintAssetId, mMaterialTechniqueId);
				}
				createMaterialResource(parentMaterialResourceId);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	void MaterialSceneItem::onLoadingStateChange(const IResource& resource)
	{
		SE_ASSERT(resource.getAssetId() == mMaterialAssetId, "Invalid asset ID")
		if (resource.getLoadingState() == IResource::LoadingState::LOADED)
		{
			mRenderableManager.getRenderables().clear();

			// Destroy the material resource we created
			if (IsValid(mMaterialResourceId))
			{
				getSceneResource().getRenderer().getMaterialResourceManager().destroyMaterialResource(mMaterialResourceId);
				SetInvalid(mMaterialResourceId);
			}

			// Create material resource
			createMaterialResource(resource.getId());
		}
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	MaterialSceneItem::~MaterialSceneItem()
	{
		if (IsValid(mMaterialResourceId))
		{
			// Clear the renderable manager right now
			mRenderableManager.getRenderables().clear();

			// Destroy the material resource we created
			getSceneResource().getRenderer().getMaterialResourceManager().destroyMaterialResource(mMaterialResourceId);
		}
	}

	void MaterialSceneItem::createMaterialResource(MaterialResourceId parentMaterialResourceId)
	{
		// Sanity checks
		SE_ASSERT(IsInvalid(mMaterialResourceId), "Invalid data")
		SE_ASSERT(IsValid(parentMaterialResourceId), "Invalid data")

		// Each material user instance must have its own material resource since material property values might vary
		MaterialResourceManager& materialResourceManager = getSceneResource().getRenderer().getMaterialResourceManager();
		mMaterialResourceId = materialResourceManager.createMaterialResourceByCloning(parentMaterialResourceId);

		{ // Set material properties
			const MaterialProperties::SortedPropertyVector& sortedPropertyVector = mMaterialProperties.getSortedPropertyVector();
			if (!sortedPropertyVector.empty())
			{
				MaterialResource& materialResource = materialResourceManager.getById(mMaterialResourceId);
				for (const MaterialProperty& materialProperty : sortedPropertyVector)
				{
					if (materialProperty.isOverwritten())
					{
						materialResource.setPropertyById(materialProperty.getMaterialPropertyId(), materialProperty);
					}
				}
			}
		}

		// Tell the world
		onMaterialResourceCreated();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

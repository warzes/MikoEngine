#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/Compute/CompositorInstancePassCompute.h"
#include "Renderer/Resource/CompositorNode/Pass/Compute/CompositorResourcePassCompute.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/Material/MaterialResource.h"
#include "Renderer/Core/IProfiler.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Resource/Material/MaterialTechnique.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CompositorInstancePassCompute::CompositorInstancePassCompute(const CompositorResourcePassCompute& compositorResourcePassCompute, const CompositorNodeInstance& compositorNodeInstance) :
		ICompositorInstancePass(compositorResourcePassCompute, compositorNodeInstance),
		mComputeMaterialBlueprint(true),
		mRenderQueue(compositorNodeInstance.getCompositorWorkspaceInstance().getRenderer().getMaterialBlueprintResourceManager().getIndirectBufferManager(), 0, 0, false, false, false),
		mMaterialResourceId(GetInvalid<MaterialResourceId>())
	{
		const IRenderer& renderer = compositorNodeInstance.getCompositorWorkspaceInstance().getRenderer();

		// Sanity checks
		RHI_ASSERT(!compositorResourcePassCompute.isMaterialDefinitionMandatory() || IsValid(compositorResourcePassCompute.getMaterialAssetId()) || IsValid(compositorResourcePassCompute.getMaterialBlueprintAssetId()), "Invalid compositor resource pass compute configuration")
		RHI_ASSERT(!(IsValid(compositorResourcePassCompute.getMaterialAssetId()) && IsValid(compositorResourcePassCompute.getMaterialBlueprintAssetId())), "Invalid compositor resource pass compute configuration")

		// Get parent material resource ID and initiate creating the compositor instance pass compute material resource
		MaterialResourceManager& materialResourceManager = renderer.getMaterialResourceManager();
		if (IsValid(compositorResourcePassCompute.getMaterialAssetId()))
		{
			// Get or load material resource
			MaterialResourceId materialResourceId = GetInvalid<MaterialResourceId>();
			materialResourceManager.loadMaterialResourceByAssetId(compositorResourcePassCompute.getMaterialAssetId(), materialResourceId, this);
		}
		else
		{
			// Get or load material blueprint resource
			const AssetId materialBlueprintAssetId = compositorResourcePassCompute.getMaterialBlueprintAssetId();
			if (IsValid(materialBlueprintAssetId))
			{
				MaterialResourceId parentMaterialResourceId = materialResourceManager.getMaterialResourceIdByAssetId(materialBlueprintAssetId);
				if (IsInvalid(parentMaterialResourceId))
				{
					parentMaterialResourceId = materialResourceManager.createMaterialResourceByAssetId(materialBlueprintAssetId, materialBlueprintAssetId, compositorResourcePassCompute.getMaterialTechniqueId());
				}
				createMaterialResource(parentMaterialResourceId);
			}
		}
	}

	CompositorInstancePassCompute::~CompositorInstancePassCompute()
	{
		if (IsValid(mMaterialResourceId))
		{
			// Clear the renderable manager
			mRenderableManager.getRenderables().clear();

			// Destroy the material resource the compositor instance pass compute created
			getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer().getMaterialResourceManager().destroyMaterialResource(mMaterialResourceId);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassCompute::onFillCommandBuffer(const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer)
	{
		if (IsValid(mMaterialResourceId))
		{
			// Sanity check
			RHI_ASSERT(!mRenderableManager.getRenderables().empty(), "No renderables")

			// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
			RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer().getContext(), commandBuffer, getCompositorResourcePass().getDebugName())

			// Fill command buffer depending on graphics or compute material blueprint
			mRenderQueue.addRenderablesFromRenderableManager(mRenderableManager, static_cast<const CompositorResourcePassCompute&>(getCompositorResourcePass()).getMaterialTechniqueId(), compositorContextData);
			if (mRenderQueue.getNumberOfDrawCalls() > 0)
			{
				if (mComputeMaterialBlueprint)
				{
					// Sanity check
					RHI_ASSERT(nullptr == renderTarget, "The compute compositor instance pass needs an invalid render target in case a compute material blueprint is used")

					// Fill command buffer using a compute material blueprint
					mRenderQueue.fillComputeCommandBuffer(compositorContextData, commandBuffer);
				}
				else
				{
					// Sanity check
					RHI_ASSERT(nullptr != renderTarget, "The compute compositor instance pass needs a valid render target in case a graphics material blueprint is used")

					// Fill command buffer using a graphics material blueprint
					mRenderQueue.fillGraphicsCommandBuffer(*renderTarget, compositorContextData, commandBuffer);
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassCompute::onLoadingStateChange(const IResource& resource)
	{
		RHI_ASSERT(resource.getId() == mMaterialResourceId, "Invalid material resource ID")
		createMaterialResource(resource.getId());
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::CompositorInstancePassCompute methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassCompute::createMaterialResource(MaterialResourceId parentMaterialResourceId)
	{
		const IRenderer& renderer = getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer();

		// Sanity checks
		RHI_ASSERT(IsInvalid(mMaterialResourceId), "Invalid material resource ID")
		RHI_ASSERT(IsValid(parentMaterialResourceId), "Invalid material resource ID")

		// Each compositor instance pass compute must have its own material resource since material property values might vary
		MaterialResourceManager& materialResourceManager = renderer.getMaterialResourceManager();
		mMaterialResourceId = materialResourceManager.createMaterialResourceByCloning(parentMaterialResourceId);

		// Graphics or compute material blueprint?
		mComputeMaterialBlueprint = true;
		MaterialResource& materialResource = materialResourceManager.getById(mMaterialResourceId);
		{
			const MaterialTechnique* materialTechnique = materialResource.getMaterialTechniqueById(MaterialResourceManager::DEFAULT_MATERIAL_TECHNIQUE_ID);
			RHI_ASSERT(nullptr != materialTechnique, "Invalid material technique")
			MaterialBlueprintResource* materialBlueprintResource = renderer.getMaterialBlueprintResourceManager().tryGetById(materialTechnique->getMaterialBlueprintResourceId());
			RHI_ASSERT(nullptr != materialBlueprintResource, "Invalid material blueprint resource")
			mComputeMaterialBlueprint = IsValid(materialBlueprintResource->getComputeShaderBlueprintResourceId());
		}

		{ // Set compositor resource pass compute material properties
			const MaterialProperties::SortedPropertyVector& sortedPropertyVector = static_cast<const CompositorResourcePassCompute&>(getCompositorResourcePass()).getMaterialProperties().getSortedPropertyVector();
			if (!sortedPropertyVector.empty())
			{
				for (const MaterialProperty& materialProperty : sortedPropertyVector)
				{
					if (materialProperty.isOverwritten())
					{
						materialResource.setPropertyById(materialProperty.getMaterialPropertyId(), materialProperty);
					}
				}
			}
		}

		// Setup renderable manager using attribute-less rendering
		#if SE_DEBUG
			mRenderableManager.setDebugName(materialResource.getDebugName());
		#endif
		mRenderableManager.getRenderables().emplace_back(mRenderableManager, Rhi::IVertexArrayPtr(), materialResourceManager, mMaterialResourceId, GetInvalid<SkeletonResourceId>(), false, 0, 3, 1 RHI_RESOURCE_DEBUG_NAME(materialResource.getDebugName()));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

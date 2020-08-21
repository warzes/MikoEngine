#include "stdafx.h"
#include "Renderer/Resource/Scene/Item/Particles/ParticlesSceneItem.h"
#include "Renderer/Resource/Scene/SceneResource.h"
#include "Renderer/Resource/Scene/SceneNode.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"
#include "Renderer/Resource/Material/MaterialPropertyValue.h"
#include "Renderer/Resource/Material/MaterialTechnique.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/Material/MaterialResource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ISceneItem methods        ]
	//[-------------------------------------------------------]
	void ParticlesSceneItem::onExecuteOnRendering([[maybe_unused]] const Rhi::IRenderTarget& renderTarget, [[maybe_unused]] const CompositorContextData& compositorContextData, [[maybe_unused]] Rhi::CommandBuffer& commandBuffer) const
	{
		// TODO(co) Implement me
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	void ParticlesSceneItem::initialize()
	{
		// Call the base implementation
		if (mMaximumNumberOfParticles > 0)
		{
			MaterialSceneItem::initialize();
		}
	}

	void ParticlesSceneItem::onMaterialResourceCreated()
	{
		// Setup renderable manager: Six vertices per particle, particle index = instance index
		#if SE_DEBUG
			const char* debugName = "Particles";
			mRenderableManager.setDebugName(debugName);
		#endif
		const IRenderer& renderer = getSceneResource().getRenderer();
		const MaterialResourceManager& materialResourceManager = renderer.getMaterialResourceManager();
		const MaterialResourceId materialResourceId = getMaterialResourceId();
		mRenderableManager.getRenderables().emplace_back(mRenderableManager, renderer.getMeshResourceManager().getDrawIdVertexArrayPtr(), materialResourceManager, materialResourceId, getInvalid<SkeletonResourceId>(), false, 0, 6, mMaximumNumberOfParticles RHI_RESOURCE_DEBUG_NAME(debugName));
		mRenderableManager.updateCachedRenderablesData();

		// Tell the used material resource about our structured buffer
		for (MaterialTechnique* materialTechnique : materialResourceManager.getById(materialResourceId).getSortedMaterialTechniqueVector())
		{
			materialTechnique->setStructuredBufferPtr(mStructuredBufferPtr);
		}

		// We need "Renderer::ISceneItem::onExecuteOnRendering()" calls during runtime
		setCallExecuteOnRendering(true);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	ParticlesSceneItem::ParticlesSceneItem(SceneResource& sceneResource) :
		MaterialSceneItem(sceneResource, false),	// TODO(co) Set bounding box
		mMaximumNumberOfParticles(8)				// TODO(co) Make this dynamic
	{
		// The RHI implementation must support structured buffers
		const IRenderer& renderer = getSceneResource().getRenderer();
		if (renderer.getRhi().getCapabilities().maximumStructuredBufferSize > 0)
		{
			// Create vertex array object (VAO)
			// Create the vertex buffer object (VBO)
			// TODO(co) Make this dynamic
			const ParticleDataStruct particlesData[8] =
			{
				{
					4.88f,  1.4f, -1.44f, 0.5f,
					 1.0f,  1.0f,   1.0f, 0.3f
				},
				{
					-6.2f,  1.4f, -1.44f, 0.5f,
					 1.0f,  1.0f,   1.0f, 0.3f
				},
				{
					4.88f,  1.4f,   2.2f, 0.5f,
					 1.0f,  1.0f,   1.0f, 0.3f
				},
				{
					-6.2f,  1.4f,   2.2f, 0.5f,
					 1.0f,  1.0f,   1.0f, 0.3f
				},
				{
					-12.0f, 1.39f,  -4.0f, 1.0f,
					  1.0f,  0.0f,   0.0f, 1.0f
				},
				{
					11.2f, 1.39f,  -4.0f, 1.0f,
					 0.0f,  1.0f,   0.0f, 1.0f
				},
				{
					-12.0f, 1.39f,   4.5f, 1.0f,
					  0.0f,  0.0f,   1.0f, 1.0f
				},
				{
					11.2f, 1.39f,   4.5f, 1.0f,
					 1.0f,  1.0f,   1.0f, 1.0f
				}
			};

			// Create the structured buffer
			mStructuredBufferPtr = renderer.getBufferManager().createStructuredBuffer(sizeof(ParticleDataStruct) * mMaximumNumberOfParticles, particlesData, Rhi::BufferFlag::SHADER_RESOURCE, Rhi::BufferUsage::STATIC_DRAW, sizeof(ParticleDataStruct) RHI_RESOURCE_DEBUG_NAME("Particles"));
		}
		else
		{
			mMaximumNumberOfParticles = 0;
			RHI_LOG_ONCE(COMPATIBILITY_WARNING, "The renderer particles scene item needs a RHI implementation with structured buffer support")
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

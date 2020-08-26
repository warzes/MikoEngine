#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/DebugGui/CompositorInstancePassDebugGui.h"
#include "Renderer/Resource/CompositorNode/Pass/DebugGui/CompositorResourcePassDebugGui.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorContextData.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
#include "Renderer/DebugGui/DebugGuiManager.h"
#include "Renderer/Core/IProfiler.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Context.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassDebugGui::onFillCommandBuffer([[maybe_unused]] const Rhi::IRenderTarget* renderTarget, [[maybe_unused]] const CompositorContextData& compositorContextData, [[maybe_unused]] Rhi::CommandBuffer& commandBuffer)
	{
		// Sanity check
		SE_ASSERT(nullptr != renderTarget, "The debug GUI compositor instance pass needs a valid render target")

		#ifdef RENDERER_IMGUI
			// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
			const IRenderer& renderer = getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer();
			RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(renderer.getContext(), commandBuffer, getCompositorResourcePass().getDebugName())

			// Fill command buffer
			DebugGuiManager& debugGuiManager = renderer.getDebugGuiManager();
			RenderableManager::Renderables& renderables = mRenderableManager.getRenderables();
			compositorContextData.resetCurrentlyBoundMaterialBlueprintResource();
			if (renderables.empty())
			{
				// Fill command buffer using fixed build in RHI configuration resources
				debugGuiManager.fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(commandBuffer);
			}
			else
			{
				// Fill command buffer, this sets the material resource blueprint
				{
					const Rhi::IVertexArrayPtr& vertexArrayPtr = debugGuiManager.getFillVertexArrayPtr();
					if (vertexArrayPtr != renderables[0].getVertexArrayPtr())
					{
						renderables[0].setVertexArrayPtr(vertexArrayPtr);
					}
				}
				mRenderQueue.addRenderablesFromRenderableManager(mRenderableManager, static_cast<const CompositorResourcePassDebugGui&>(getCompositorResourcePass()).getMaterialTechniqueId(), compositorContextData);
				if (mRenderQueue.getNumberOfDrawCalls() > 0)
				{
					mRenderQueue.fillGraphicsCommandBuffer(*renderTarget, compositorContextData, commandBuffer);

					// Fill command buffer using custom graphics material blueprint resource
					if (nullptr != compositorContextData.getCurrentlyBoundMaterialBlueprintResource())
					{
						debugGuiManager.fillGraphicsCommandBuffer(commandBuffer);
					}
				}
			}
		#else
			SE_ASSERT(false, "ImGui support is disabled")
		#endif
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::CompositorInstancePassCompute methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassDebugGui::createMaterialResource(MaterialResourceId parentMaterialResourceId)
	{
		// Call the base implementation
		CompositorInstancePassCompute::createMaterialResource(parentMaterialResourceId);

		// Inside this compositor pass implementation, the renderable only exists to set the material blueprint
		mRenderableManager.getRenderables()[0].setNumberOfIndices(0);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorInstancePassDebugGui::CompositorInstancePassDebugGui(const CompositorResourcePassDebugGui& compositorResourcePassDebugGui, const CompositorNodeInstance& compositorNodeInstance) :
		CompositorInstancePassCompute(compositorResourcePassDebugGui, compositorNodeInstance)
	{
		// Inside this compositor pass implementation, the renderable only exists to set the material blueprint
		RenderableManager::Renderables& renderables = mRenderableManager.getRenderables();
		if (!renderables.empty())
		{
			renderables[0].setNumberOfIndices(0);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

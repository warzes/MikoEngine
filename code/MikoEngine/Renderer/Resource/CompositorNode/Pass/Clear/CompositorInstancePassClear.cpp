#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/Clear/CompositorInstancePassClear.h"
#include "Renderer/Resource/CompositorNode/Pass/Clear/CompositorResourcePassClear.h"
#include "Renderer/Core/IProfiler.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
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
	void CompositorInstancePassClear::onFillCommandBuffer([[maybe_unused]] const Rhi::IRenderTarget* renderTarget, const CompositorContextData&, Rhi::CommandBuffer& commandBuffer)
	{
		// Sanity check
		SE_ASSERT(nullptr != renderTarget, "The clear compositor instance pass needs a valid render target")

		// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
		const CompositorResourcePassClear& compositorResourcePassClear = static_cast<const CompositorResourcePassClear&>(getCompositorResourcePass());
		RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer().getContext(), commandBuffer, compositorResourcePassClear.getDebugName())

		// Push the clear graphics command
		Rhi::Command::ClearGraphics::create(commandBuffer, compositorResourcePassClear.getFlags(), glm::value_ptr(compositorResourcePassClear.getClearColor()), compositorResourcePassClear.getZ(), compositorResourcePassClear.getStencil());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorInstancePassClear::CompositorInstancePassClear(const CompositorResourcePassClear& compositorResourcePassClear, const CompositorNodeInstance& compositorNodeInstance) :
		ICompositorInstancePass(compositorResourcePassClear, compositorNodeInstance)
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

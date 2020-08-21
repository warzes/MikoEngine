#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/CompositorNode/Pass/Compute/CompositorInstancePassCompute.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class CompositorResourcePassDebugGui;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorInstancePassDebugGui final : public CompositorInstancePassCompute
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onFillCommandBuffer(const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::CompositorInstancePassCompute methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void createMaterialResource(MaterialResourceId parentMaterialResourceId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		CompositorInstancePassDebugGui(const CompositorResourcePassDebugGui& compositorResourcePassDebugGui, const CompositorNodeInstance& compositorNodeInstance);

		inline virtual ~CompositorInstancePassDebugGui() override
		{
			// Nothing here
		}

		explicit CompositorInstancePassDebugGui(const CompositorInstancePassDebugGui&) = delete;
		CompositorInstancePassDebugGui& operator=(const CompositorInstancePassDebugGui&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

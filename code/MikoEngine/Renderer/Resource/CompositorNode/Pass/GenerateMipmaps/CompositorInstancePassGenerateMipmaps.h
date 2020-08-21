#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/CompositorNode/Pass/ICompositorInstancePass.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class CompositorResourcePassCompute;
	class CompositorInstancePassCompute;
	class CompositorResourcePassGenerateMipmaps;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorInstancePassGenerateMipmaps final : public ICompositorInstancePass
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
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		CompositorInstancePassGenerateMipmaps(const CompositorResourcePassGenerateMipmaps& compositorResourcePassGenerateMipmaps, const CompositorNodeInstance& compositorNodeInstance);
		virtual ~CompositorInstancePassGenerateMipmaps() override;
		explicit CompositorInstancePassGenerateMipmaps(const CompositorInstancePassGenerateMipmaps&) = delete;
		CompositorInstancePassGenerateMipmaps& operator=(const CompositorInstancePassGenerateMipmaps&) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<Rhi::IFramebufferPtr> FramebufferPtrs;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// For texture mipmap generation via custom material blueprint
		Rhi::CommandBuffer			   mCommandBuffer;
		CompositorResourcePassCompute* mCompositorResourcePassCompute;
		CompositorInstancePassCompute* mCompositorInstancePassCompute;
		uint32_t					   mRenderTargetWidth;
		uint32_t					   mRenderTargetHeight;
		FramebufferPtrs				   mFramebuffersPtrs;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

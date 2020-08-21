#pragma once

#include "RHI/Rhi.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class CompositorContextData;
	class CompositorNodeInstance;
	class ICompositorResourcePass;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class ICompositorInstancePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorNodeInstance;		// Needs to execute compositor node instances
		friend class CompositorWorkspaceInstance;	// Needs to be able to set the render target


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline const ICompositorResourcePass& getCompositorResourcePass() const
		{
			return mCompositorResourcePass;
		}

		[[nodiscard]] inline const CompositorNodeInstance& getCompositorNodeInstance() const
		{
			return mCompositorNodeInstance;
		}

		[[nodiscard]] inline Rhi::IRenderTarget* getRenderTarget() const
		{
			return mRenderTarget;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Method is called when the owner compositor workspace instance loading has been finished
		*
		*  @note
		*    - A compositor pass instance can e.g. prefetch a render queue index ranges instance in here to avoid repeating this during runtime
		*    - The default implementation is empty
		*/
		inline virtual void onCompositorWorkspaceInstanceLoadingFinished()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Fill the compositor pass into the given command buffer
		*
		*  @param[in] renderTarget
		*    RHI render target to render into, can be a null pointer (e.g. for compute shader or resource copy compositor passes)
		*  @param[in] compositorContextData
		*    Compositor context data
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*/
		virtual void onFillCommandBuffer(const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer) = 0;

		/**
		*  @brief
		*    Called post command buffer execution
		*
		*  @note
		*    - The default implementation is empty
		*/
		inline virtual void onPostCommandBufferExecution()
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline ICompositorInstancePass(const ICompositorResourcePass& compositorResourcePass, const CompositorNodeInstance& compositorNodeInstance) :
			mCompositorResourcePass(compositorResourcePass),
			mCompositorNodeInstance(compositorNodeInstance),
			mRenderTarget(nullptr),
			mNumberOfExecutionRequests(0)
		{
			// Nothing here
		}

		inline virtual ~ICompositorInstancePass()
		{
			// Nothing here
		}

		explicit ICompositorInstancePass(const ICompositorInstancePass&) = delete;
		ICompositorInstancePass& operator=(const ICompositorInstancePass&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const ICompositorResourcePass& mCompositorResourcePass;
		const CompositorNodeInstance&  mCompositorNodeInstance;
		Rhi::IRenderTarget*			   mRenderTarget;	/// Render target, can be a null pointer, don't destroy the instance
		uint32_t					   mNumberOfExecutionRequests;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#pragma once

namespace Rhi
{
	class IRenderTarget : public IResource
	{
	public:
		inline virtual ~IRenderTarget() override
		{
			mRenderPass.ReleaseReference();
		}

		[[nodiscard]] inline IRenderPass& getRenderPass() const
		{
			return mRenderPass;
		}

		// Public virtual Rhi::IRenderTarget methods
	public:
		/**
		*  @brief
		*    Return the width and height of the render target
		*
		*  @param[out] width
		*    Receives the width of the render target, guaranteed to be never ever zero
		*  @param[out] height
		*    Receives the height of the render target, guaranteed to be never ever zero
		*/
		virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const = 0;

		// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] renderPass
		*    Render pass to use, the render target keeps a reference to the render pass
		*/
		inline IRenderTarget(ResourceType resourceType, IRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, renderPass.getRhi() RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRenderPass(renderPass)
		{
			mRenderPass.AddReference();
		}

		explicit IRenderTarget(const IRenderTarget& source) = delete;
		IRenderTarget& operator =(const IRenderTarget& source) = delete;

		// Private data
	private:
		IRenderPass& mRenderPass;	// Render pass to use, the render target keeps a reference to the render pass

	};

	typedef SmartRefCount<IRenderTarget> IRenderTargetPtr;

} // namespace Rhi
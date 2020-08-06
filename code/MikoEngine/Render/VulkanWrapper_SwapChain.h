#pragma once

#if SE_VULKAN

#include "VulkanWrapper_Resource.h"

namespace vkWrapper
{

	class SwapChainResources
	{
	public:
		SwapChainResources(LogicalDevice* device, Surface* surface);
		~SwapChainResources();

		void Init();
		void Close();

		LogicalDevice* GetDevice()
		{
			return m_device;
		}
		Surface* GetSurface()
		{
			return m_surface;
		}
		SwapChain* GetSwapChain()
		{
			return m_swapchain;
		}
		RenderPass* GetRenderPass()
		{
			return m_renderpass;
		}
		FrameBuffer* GetFrameBuffer(uint32_t index)
		{
			return m_framebuffers[index];
		}
		uint32_t Count()
		{
			return m_swapchain->Count();
		}

	protected:
		LogicalDevice* m_device;
		Surface* m_surface;

		SwapChain* m_swapchain;
		std::vector<ImageView*> m_views;

		RenderPass* m_renderpass;
		Image* m_msaaImage;
		Image* m_depthImage;
		ImageView* m_msaaView;
		ImageView* m_depthView;

		std::vector<FrameBuffer*> m_framebuffers;
		std::vector<DrawCmdPool*> m_drawCmdPool;
	};

} // namespace vkWrapper
#endif // SE_VULKAN
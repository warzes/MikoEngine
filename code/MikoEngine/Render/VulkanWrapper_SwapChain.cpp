#include "stdafx.h"
#if SE_VULKAN
#include "VulkanWrapper_SwapChain.h"

namespace vkWrapper
{
	SwapChainResources::SwapChainResources(LogicalDevice* device, Surface* surface)
		: m_device(device)
		, m_surface(surface)
		, m_swapchain(nullptr)
		, m_renderpass(nullptr)
		, m_msaaImage(nullptr)
		, m_depthImage(nullptr)
		, m_msaaView(nullptr)
		, m_depthView(nullptr)
	{
	}

	SwapChainResources::~SwapChainResources()
	{
		Close();
	}

	void SwapChainResources::Init()
	{
		m_swapchain = new SwapChain(m_device, m_surface);
		m_swapchain->Init();

		uint32_t size = m_swapchain->Count();
		m_swap_chain_image_views.resize(size);
		m_swap_chain_framebuffers.resize(size);

		PhysicalDevice* physical = m_device->GetPhysical();


		m_swap_chain_depth_format = physical->FindDepthFormat();
		//VkFormat depthformat = physical->FindDepthFormat();







		for ( uint32_t i = 0; i < size; ++i )
		{
			m_swap_chain_image_views[i] = new ImageView(m_device, m_swapchain, i, VK_IMAGE_ASPECT_COLOR_BIT);
			m_swap_chain_image_views[i]->Init();
		}

		m_renderpass = new RenderPass(m_device, m_swapchain->GetFormat());
		m_renderpass->Init();

		
		VkExtent2D extent = m_swapchain->GetExtent();
		VkFormat colorformat = m_swapchain->GetFormat();
		
		// VkSampleCountFlagBits samples = m_device->msaaEnabled() ? physical->m_msaaSamples : VK_SAMPLE_COUNT_1_BIT;

		// create color ressource for msaa
		if ( m_device->MsaaEnabled() )
		{
			VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			m_msaaImage = new Image(m_device, extent.width, extent.height, 1, m_device->MsaaEnabled(), colorformat, usage);
			m_msaaImage->Init();
			m_msaaView = new ImageView(m_device, m_msaaImage, VK_IMAGE_ASPECT_COLOR_BIT);
			m_msaaView->Init();
		}

		// create depth ressource
		VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		m_depthImage = new Image(m_device, extent.width, extent.height, 1, m_device->MsaaEnabled(), m_swap_chain_depth_format, usage);
		m_depthImage->Init();
		m_depthImage->TransitionLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		m_depthView = new ImageView(m_device, m_depthImage, VK_IMAGE_ASPECT_DEPTH_BIT);
		m_depthView->Init();

		for ( uint32_t i = 0; i < size; ++i )
		{
			std::vector<ImageView*> attachments;
			if ( m_device->MsaaEnabled() ) attachments = { m_msaaView, m_depthView, m_swap_chain_image_views[i] };
			else attachments = { m_swap_chain_image_views[i], m_depthView };

			m_swap_chain_framebuffers[i] = new FrameBuffer(m_device, attachments, m_renderpass, extent.width, extent.height);
			m_swap_chain_framebuffers[i]->Init();
		}

		// draw cmd pools (one per frame)
		m_drawCmdPool.resize(size, nullptr);
		for ( uint32_t i = 0; i < size; ++i )
		{
			m_drawCmdPool[i] = new DrawCmdPool(m_device);
			m_drawCmdPool[i]->Init();
		}
	}

	void SwapChainResources::Close()
	{
		uint32_t size = m_swapchain->Count();
		for ( uint32_t i = 0; i < size; ++i ) delete m_drawCmdPool[i];

		if ( m_msaaView ) delete m_msaaView;
		if ( m_msaaImage ) delete m_msaaImage;

		if ( m_depthView ) delete m_depthView;
		if ( m_depthImage ) delete m_depthImage;

		for ( auto fb : m_swap_chain_framebuffers ) delete fb;
		m_swap_chain_framebuffers.clear();

		if ( m_renderpass ) delete m_renderpass;

		for ( auto iv : m_swap_chain_image_views ) delete iv;
		m_swap_chain_image_views.clear();

		if ( m_swapchain ) delete m_swapchain;
	}

} // namespace vkWrapper
#endif // SE_VULKAN
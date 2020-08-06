#pragma once

#if SE_VULKAN

#include "VulkanWrapper.h"

namespace vkWrapper
{
	class GraphicCmdPool;

	class CommandBuffer : VkDeviceObject
	{
	public:
		CommandBuffer(LogicalDevice* device, CommandPool* pool);
		CommandBuffer(LogicalDevice* device, CommandPool* pool, VkCommandBuffer buffer);
		~CommandBuffer();

		void Init() override;
		void Close() override;

		void BeginOneTime();
		void Begin();
		void End();
		void SubmitAndWait();

		void CopyBuffer(VkBuffer dst, VkBuffer src, uint32_t size);

		VkCommandBuffer& Get()
		{
			return m_commandbuffer;
		}

		bool IsValid()
		{
			return m_valid;
		}
		void Invalidate()
		{
			m_valid = false;
		}

	protected:
		VkCommandBuffer m_commandbuffer;
		CommandPool* m_pool;
		bool m_valid;
	};

	class CommandPool : VkDeviceObject
	{
	public:
		CommandPool(LogicalDevice* device, Queue* queue);
		~CommandPool();

		void Init() override;
		void Close() override;

		void Reset();

		CommandBuffer* Alloc();
		std::vector<CommandBuffer*> Alloc(uint32_t count);

		Queue* GetQueue()
		{
			return m_queue;
		}

		VkCommandPool& Get()
		{
			return m_pool;
		}

	protected:
		VkCommandPool m_pool;
		Queue* m_queue;
		std::vector<CommandBuffer*> m_allocated;
	};

	class GraphicCmdPool : public CommandPool
	{
	public:
		GraphicCmdPool(LogicalDevice* device);
		static GraphicCmdPool* instance()
		{
			return s_instance;
		}
	protected:
		static GraphicCmdPool* s_instance;
	};

	class DrawCmdPool : public CommandPool
	{
	public:
		DrawCmdPool(LogicalDevice* device);
		static DrawCmdPool* instance(int index)
		{
			return s_instances[index];
		}
	protected:
		static std::vector<DrawCmdPool*> s_instances;
	};

	class TransferCmdPool : public CommandPool
	{
	public:
		TransferCmdPool(LogicalDevice* device);
		static TransferCmdPool* instance()
		{
			return s_instance;
		}
	protected:
		static TransferCmdPool* s_instance;
	};

	class Buffer : VkDeviceObject
	{
	public:
		Buffer(LogicalDevice* device, uint32_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		~Buffer();

		void Init() override;
		void Close() override;

		void Copy(const void* data, size_t size);
		void Copy(Buffer* src);

		VkBuffer& Get()
		{
			return m_buffer;
		}
		VkDeviceMemory& GetMemory()
		{
			return m_memory;
		}
		uint32_t GetSize()
		{
			return m_size;
		}

	private:
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		VkBufferUsageFlags m_usage;
		VkMemoryPropertyFlags m_properties;

		uint32_t m_size;
	};

	class Image : VkDeviceObject
	{
	public:
		Image(LogicalDevice* device, uint32_t width, uint32_t height, uint32_t miplevels, bool enableMsaa, VkFormat format, VkImageUsageFlags usage);

		~Image();

		void Init() override;
		void Close() override;

		void TransitionLayout(VkImageLayout newLayout);
		void Copy(Buffer* buffer);
		void GenerateMipmaps();

		VkImage& Get()
		{
			return m_image;
		}
		VkImageLayout& GetLayout()
		{
			return m_layout;
		}
		VkFormat GetFormat()
		{
			return m_format;
		}

	private:
		VkImage m_image;
		VkImageLayout m_layout;
		VkDeviceMemory m_memory;

		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_mipLevels;
		bool m_msaaEnabled;
		VkFormat m_format;
		VkImageUsageFlags m_usage;
	};

	class ImageView : VkDeviceObject
	{
	public:
		ImageView(LogicalDevice* device, Image* image, VkImageAspectFlags aspect);
		ImageView(LogicalDevice* device, SwapChain* swapchain, uint32_t index, VkImageAspectFlags aspect);

		~ImageView();

		void Init() override;
		void Close() override;

		VkImageView& Get()
		{
			return m_view;
		}

	private:
		VkImageView m_view;
		VkImageAspectFlags m_aspect;

		Image* m_image;

		SwapChain* m_owner;
		uint32_t m_ownerIndex;
	};

	class RenderPass : VkDeviceObject
	{
	public:
		RenderPass(LogicalDevice* device, VkFormat format);
		~RenderPass();

		void Init() override;
		void Close() override;

		VkRenderPass& Get()
		{
			return m_renderPass;
		}

	private:
		VkRenderPass m_renderPass;
		VkFormat m_format;
	};

	class FrameBuffer : VkDeviceObject
	{
	public:
		FrameBuffer(LogicalDevice* device, std::vector<ImageView*>& attachments, RenderPass* renderpass, uint32_t width, uint32_t height);

		~FrameBuffer();

		void Init() override;
		void Close() override;

		VkFramebuffer& Get()
		{
			return m_framebuffer;
		}

	private:
		VkFramebuffer m_framebuffer;
		std::vector<ImageView*>& m_attachments;

		RenderPass* m_renderpass;

		uint32_t m_width;
		uint32_t m_height;
	};

} // namespace vkWrapper
#endif // SE_VULKAN
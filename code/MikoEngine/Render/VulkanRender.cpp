#include "stdafx.h"
#if SE_VULKAN
#include "VulkanRender.h"
#include "Core/Logger.h"
#include "Utility/Utility.h"
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace vk
{
	const std::vector<const char*> kValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

#    define MAX_DESCRIPTOR_POOL_THREADS 32
#    define MAX_COMMAND_THREADS 32
#    define MAX_THREAD_LOCAL_COMMAND_BUFFERS 8



#pragma region Image

	Image::Ptr Image::create(Backend::Ptr backend, VkImageType type, uint32_t width, uint32_t height, uint32_t depth, uint32_t mip_levels, uint32_t array_size, VkFormat format, VmaMemoryUsage memory_usage, VkImageUsageFlags usage, VkSampleCountFlagBits sample_count, VkImageLayout initial_layout, size_t size, void* data)
	{
		return std::shared_ptr<Image>(new Image(backend, type, width, height, depth, mip_levels, array_size, format, memory_usage, usage, sample_count, initial_layout, size, data));
	}

	Image::Ptr Image::create_from_swapchain(Backend::Ptr backend, VkImage image, VkImageType type, uint32_t width, uint32_t height, uint32_t depth, uint32_t mip_levels, uint32_t array_size, VkFormat format, VmaMemoryUsage memory_usage, VkImageUsageFlags usage, VkSampleCountFlagBits sample_count)
	{
		return std::shared_ptr<Image>(new Image(backend, image, type, width, height, depth, mip_levels, array_size, format, memory_usage, usage, sample_count));
	}

	Image::Ptr Image::create_from_file(Backend::Ptr backend, std::string path, bool flip_vertical, bool srgb)
	{
		int x, y, n;
		stbi_set_flip_vertically_on_load(flip_vertical);

		std::string ext = utility::file_extension(path);

		if (ext == "hdr")
		{
			float* data = stbi_loadf(path.c_str(), &x, &y, &n, 0);

			if (!data)
				return nullptr;

			Image::Ptr image = std::shared_ptr<Image>(new Image(backend, VK_IMAGE_TYPE_2D, (uint32_t)x, (uint32_t)y, 1, 0, 1, VK_FORMAT_R32G32B32_SFLOAT, VMA_MEMORY_USAGE_GPU_ONLY, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, x * y * sizeof(float) * 3, data));

			stbi_image_free(data);

			return image;
		}
		else
		{
			stbi_uc* data = stbi_load(path.c_str(), &x, &y, &n, 0);

			if (!data)
				return nullptr;

			if (n == 3)
			{
				stbi_image_free(data);
				data = stbi_load(path.c_str(), &x, &y, &n, 4);
				n = 4;
			}

			VkFormat format;

			if (n == 1)
				format = VK_FORMAT_R8_UNORM;
			else
			{
				if (srgb)
					format = VK_FORMAT_R8G8B8A8_SRGB;
				else
					format = VK_FORMAT_R8G8B8A8_UNORM;
			}

			Image::Ptr image = std::shared_ptr<Image>(new Image(backend, VK_IMAGE_TYPE_2D, (uint32_t)x, (uint32_t)y, 1, 0, 1, format, VMA_MEMORY_USAGE_GPU_ONLY, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, x * y * n, data));

			stbi_image_free(data);

			return image;
		}
	}

	Image::Image(Backend::Ptr backend, VkImageType type, uint32_t width, uint32_t height, uint32_t depth, uint32_t mip_levels, uint32_t array_size, VkFormat format, VmaMemoryUsage memory_usage, VkImageUsageFlags usage, VkSampleCountFlagBits sample_count, VkImageLayout initial_layout, size_t size, void* data) :
		Object(backend), m_type(type), m_width(width), m_height(height), m_depth(depth), m_mip_levels(mip_levels), m_array_size(array_size), m_format(format), m_memory_usage(memory_usage), m_sample_count(sample_count), m_usage(usage)
	{
		m_vma_allocator = backend->allocator();

		if (mip_levels == 0)
			m_mip_levels = static_cast<uint32_t>(std::floor(std::log2(std::max(m_width, m_height)))) + 1;

		VkImageCreateInfo image_info;
		SE_ZERO_MEMORY(image_info);

		image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_info.imageType = m_type;
		image_info.extent.width = m_width;
		image_info.extent.height = m_height;
		image_info.extent.depth = m_depth;
		image_info.mipLevels = m_mip_levels;
		image_info.arrayLayers = m_array_size;
		image_info.format = m_format;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.initialLayout = initial_layout;
		image_info.usage = m_usage;
		image_info.samples = m_sample_count;
		image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationInfo       alloc_info;
		VmaAllocationCreateInfo alloc_create_info;
		SE_ZERO_MEMORY(alloc_create_info);

		alloc_create_info.usage = memory_usage;
		alloc_create_info.flags = 0;

		if (vmaCreateImage(m_vma_allocator, &image_info, &alloc_create_info, &m_vk_image, &m_vma_allocation, &alloc_info) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Image.");
			throw std::runtime_error("(Vulkan) Failed to create Image.");
		}

		m_vk_device_memory = alloc_info.deviceMemory;

		if (data)
		{
			CommandBuffer::Ptr cmd_buf = backend->allocate_graphics_command_buffer(true);

			VkImageSubresourceRange subresource_range;
			SE_ZERO_MEMORY(subresource_range);

			subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresource_range.baseMipLevel = 0;
			subresource_range.levelCount = m_mip_levels;
			subresource_range.layerCount = m_array_size;
			subresource_range.baseArrayLayer = 0;

			// Image barrier for optimal image (target)
			// Optimal image will be used as destination for the copy
			utilities::set_image_layout(cmd_buf->handle(),
				m_vk_image,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				subresource_range);

			vkEndCommandBuffer(cmd_buf->handle());

			backend->flush_graphics({ cmd_buf });

			upload_data(0, 0, data, size, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

			if (m_mip_levels > 1)
				generate_mipmaps();
		}
	}



	Image::Image(Backend::Ptr backend, VkImage image, VkImageType type, uint32_t width, uint32_t height, uint32_t depth, uint32_t mip_levels, uint32_t array_size, VkFormat format, VmaMemoryUsage memory_usage, VkImageUsageFlags usage, VkSampleCountFlagBits sample_count) :
		Object(backend), m_vk_image(image), m_type(type), m_width(width), m_height(height), m_depth(depth), m_mip_levels(mip_levels), m_array_size(array_size), m_format(format), m_memory_usage(memory_usage), m_sample_count(sample_count)
	{
	}



	Image::~Image()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		if (m_vma_allocator && m_vma_allocation)
			vmaDestroyImage(m_vma_allocator, m_vk_image, m_vma_allocation);
	}



	void Image::upload_data(int array_index, int mip_level, void* data, size_t size, VkImageLayout src_layout, VkImageLayout dst_layout)
	{
		auto backend = m_vk_backend.lock();

		Buffer::Ptr staging = Buffer::create(backend, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, size, VMA_MEMORY_USAGE_CPU_ONLY, VMA_ALLOCATION_CREATE_MAPPED_BIT, data);

		VkBufferImageCopy buffer_copy_region;
		SE_ZERO_MEMORY(buffer_copy_region);

		buffer_copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		buffer_copy_region.imageSubresource.mipLevel = mip_level;
		buffer_copy_region.imageSubresource.baseArrayLayer = array_index;
		buffer_copy_region.imageSubresource.layerCount = 1;
		buffer_copy_region.imageExtent.width = m_width;
		buffer_copy_region.imageExtent.height = m_height;
		buffer_copy_region.imageExtent.depth = 1;
		buffer_copy_region.bufferOffset = 0;

		VkImageSubresourceRange subresource_range;
		SE_ZERO_MEMORY(subresource_range);

		subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresource_range.baseMipLevel = mip_level;
		subresource_range.levelCount = 1;
		subresource_range.layerCount = 1;
		subresource_range.baseArrayLayer = array_index;

		CommandBuffer::Ptr cmd_buf = backend->allocate_graphics_command_buffer(true);

		if (src_layout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			// Image barrier for optimal image (target)
			// Optimal image will be used as destination for the copy
			utilities::set_image_layout(cmd_buf->handle(),
				m_vk_image,
				src_layout,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				subresource_range);
		}

		// Copy mip levels from staging buffer
		vkCmdCopyBufferToImage(cmd_buf->handle(),
			staging->handle(),
			m_vk_image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&buffer_copy_region);

		if (dst_layout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			// Change texture image layout to shader read after all mip levels have been copied
			utilities::set_image_layout(cmd_buf->handle(),
				m_vk_image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				dst_layout,
				subresource_range);
		}

		vkEndCommandBuffer(cmd_buf->handle());

		backend->flush_graphics({ cmd_buf });
	}



	void Image::generate_mipmaps(VkImageLayout src_layout, VkImageLayout dst_layout)
	{
		auto backend = m_vk_backend.lock();

		CommandBuffer::Ptr cmd_buf = backend->allocate_graphics_command_buffer(true);

		VkImageSubresourceRange subresource_range;
		SE_ZERO_MEMORY(subresource_range);

		subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresource_range.levelCount = 1;
		subresource_range.layerCount = 1;

		int32_t mip_width = m_width;
		int32_t mip_height = m_height;

		for (int arr_idx = 0; arr_idx < m_array_size; arr_idx++)
		{
			for (int mip_idx = 1; mip_idx < m_mip_levels; mip_idx++)
			{
				subresource_range.baseMipLevel = mip_idx - 1;
				subresource_range.baseArrayLayer = arr_idx;

				VkImageLayout layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

				if (mip_idx == 1)
					layout = src_layout;

				utilities::set_image_layout(cmd_buf->handle(),
					m_vk_image,
					layout,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					subresource_range);

				VkImageBlit blit = {};
				blit.srcOffsets[0] = { 0, 0, 0 };
				blit.srcOffsets[1] = { mip_width, mip_height, 1 };
				blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.srcSubresource.mipLevel = mip_idx - 1;
				blit.srcSubresource.baseArrayLayer = arr_idx;
				blit.srcSubresource.layerCount = 1;
				blit.dstOffsets[0] = { 0, 0, 0 };
				blit.dstOffsets[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };
				blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.dstSubresource.mipLevel = mip_idx;
				blit.dstSubresource.baseArrayLayer = arr_idx;
				blit.dstSubresource.layerCount = 1;

				vkCmdBlitImage(cmd_buf->handle(),
					m_vk_image,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					m_vk_image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1,
					&blit,
					VK_FILTER_LINEAR);

				utilities::set_image_layout(cmd_buf->handle(),
					m_vk_image,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					dst_layout,
					subresource_range);

				if (mip_width > 1) mip_width /= 2;
				if (mip_height > 1) mip_height /= 2;
			}

			subresource_range.baseMipLevel = m_mip_levels - 1;

			utilities::set_image_layout(cmd_buf->handle(),
				m_vk_image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				dst_layout,
				subresource_range);
		}

		vkEndCommandBuffer(cmd_buf->handle());

		backend->flush_graphics({ cmd_buf });
	}

#pragma endregion

	ImageView::Ptr ImageView::create(Backend::Ptr backend, Image::Ptr image, VkImageViewType view_type, VkImageAspectFlags aspect_flags, uint32_t base_mip_level, uint32_t level_count, uint32_t base_array_layer, uint32_t layer_count)
	{
		return std::shared_ptr<ImageView>(new ImageView(backend, image, view_type, aspect_flags, base_mip_level, level_count, base_array_layer, layer_count));
	}



	ImageView::ImageView(Backend::Ptr backend, Image::Ptr image, VkImageViewType view_type, VkImageAspectFlags aspect_flags, uint32_t base_mip_level, uint32_t level_count, uint32_t base_array_layer, uint32_t layer_count) :
		Object(backend)
	{
		VkImageViewCreateInfo info;
		SE_ZERO_MEMORY(info);

		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = image->handle();
		info.viewType = view_type;
		info.format = image->format();
		info.subresourceRange.aspectMask = aspect_flags;
		info.subresourceRange.baseMipLevel = base_mip_level;
		info.subresourceRange.levelCount = level_count;
		info.subresourceRange.baseArrayLayer = base_array_layer;
		info.subresourceRange.layerCount = layer_count;

		if (vkCreateImageView(backend->device(), &info, nullptr, &m_vk_image_view) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Image View.");
			throw std::runtime_error("(Vulkan) Failed to create Image View.");
		}
	}



	ImageView::~ImageView()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyImageView(backend->device(), m_vk_image_view, nullptr);
	}



	RenderPass::Ptr RenderPass::create(Backend::Ptr backend, std::vector<VkAttachmentDescription> attachment_descs, std::vector<VkSubpassDescription> subpass_descs, std::vector<VkSubpassDependency> subpass_deps)
	{
		return std::shared_ptr<RenderPass>(new RenderPass(backend, attachment_descs, subpass_descs, subpass_deps));
	}



	RenderPass::RenderPass(Backend::Ptr backend, std::vector<VkAttachmentDescription> attachment_descs, std::vector<VkSubpassDescription> subpass_descs, std::vector<VkSubpassDependency> subpass_deps) :
		Object(backend)
	{
		VkRenderPassCreateInfo render_pass_info;
		SE_ZERO_MEMORY(render_pass_info);

		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = attachment_descs.size();
		render_pass_info.pAttachments = attachment_descs.data();
		render_pass_info.subpassCount = subpass_descs.size();
		render_pass_info.pSubpasses = subpass_descs.data();
		render_pass_info.dependencyCount = subpass_deps.size();
		render_pass_info.pDependencies = subpass_deps.data();

		if (vkCreateRenderPass(backend->device(), &render_pass_info, nullptr, &m_vk_render_pass) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Render Pass.");
			throw std::runtime_error("(Vulkan) Failed to create Render Pass.");
		}
	}



	RenderPass::~RenderPass()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyRenderPass(backend->device(), m_vk_render_pass, nullptr);
	}



	Framebuffer::Ptr Framebuffer::create(Backend::Ptr backend, RenderPass::Ptr render_pass, std::vector<ImageView::Ptr> views, uint32_t width, uint32_t height, uint32_t layers)
	{
		return std::shared_ptr<Framebuffer>(new Framebuffer(backend, render_pass, views, width, height, layers));
	}



	Framebuffer::Framebuffer(Backend::Ptr backend, RenderPass::Ptr render_pass, std::vector<ImageView::Ptr> views, uint32_t width, uint32_t height, uint32_t layers) :
		Object(backend)
	{
		std::vector<VkImageView> attachments(views.size());

		for (int i = 0; i < attachments.size(); i++)
			attachments[i] = views[i]->handle();

		VkFramebufferCreateInfo frameBuffer_create_info;
		SE_ZERO_MEMORY(frameBuffer_create_info);

		frameBuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBuffer_create_info.pNext = NULL;
		frameBuffer_create_info.renderPass = render_pass->handle();
		frameBuffer_create_info.attachmentCount = views.size();
		frameBuffer_create_info.pAttachments = attachments.data();
		frameBuffer_create_info.width = width;
		frameBuffer_create_info.height = height;
		frameBuffer_create_info.layers = layers;

		if (vkCreateFramebuffer(backend->device(), &frameBuffer_create_info, nullptr, &m_vk_framebuffer) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Framebuffer.");
			throw std::runtime_error("(Vulkan) Failed to create Framebuffer.");
		}
	}



	Framebuffer::~Framebuffer()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyFramebuffer(backend->device(), m_vk_framebuffer, nullptr);
	}



	Buffer::Ptr Buffer::create(Backend::Ptr backend, VkBufferUsageFlags usage, size_t size, VmaMemoryUsage memory_usage, VkFlags create_flags, void* data)
	{
		return std::shared_ptr<Buffer>(new Buffer(backend, usage, size, memory_usage, create_flags, data));
	}



	Buffer::Buffer(Backend::Ptr backend, VkBufferUsageFlags usage, size_t size, VmaMemoryUsage memory_usage, VkFlags create_flags, void* data) :
		Object(backend), m_size(size), m_vma_memory_usage(memory_usage)
	{
		m_vma_allocator = backend->allocator();

		VkBufferCreateInfo buffer_info;
		SE_ZERO_MEMORY(buffer_info);

		buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_info.size = size;
		buffer_info.usage = usage;
		buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		buffer_info.queueFamilyIndexCount = 0;
		buffer_info.pQueueFamilyIndices = nullptr;

		VkMemoryPropertyFlags memory_prop_flags = 0;
		VkBufferUsageFlags    usage_flags = usage;

		if (memory_usage == VMA_MEMORY_USAGE_CPU_ONLY)
		{
			memory_prop_flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			memory_prop_flags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			usage_flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}
		else if (memory_usage == VMA_MEMORY_USAGE_GPU_ONLY)
		{
			memory_prop_flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			usage_flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}
		else if (memory_usage == VMA_MEMORY_USAGE_CPU_TO_GPU)
		{
			memory_prop_flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			memory_prop_flags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}
		else if (memory_usage == VMA_MEMORY_USAGE_GPU_TO_CPU)
			memory_prop_flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

		m_vk_memory_property = memory_prop_flags;
		m_vk_usage_flags = usage_flags;

		buffer_info.usage = m_vk_usage_flags;

		VmaAllocationInfo vma_alloc_info;

		VmaAllocationCreateInfo alloc_create_info;
		SE_ZERO_MEMORY(alloc_create_info);

		alloc_create_info.usage = memory_usage;
		alloc_create_info.flags = create_flags;
		alloc_create_info.requiredFlags = memory_prop_flags;
		alloc_create_info.preferredFlags = 0;
		alloc_create_info.memoryTypeBits = 0;
		alloc_create_info.pool = VK_NULL_HANDLE;

		if (vmaCreateBuffer(m_vma_allocator, &buffer_info, &alloc_create_info, &m_vk_buffer, &m_vma_allocation, &vma_alloc_info) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Buffer.");
			throw std::runtime_error("(Vulkan) Failed to create Buffer.");
		}

		m_vk_device_memory = vma_alloc_info.deviceMemory;

		if (create_flags & VMA_ALLOCATION_CREATE_MAPPED_BIT)
			m_mapped_ptr = vma_alloc_info.pMappedData;

		if (data)
			upload_data(data, size, 0);
	}



	Buffer::~Buffer()
	{
		vmaDestroyBuffer(m_vma_allocator, m_vk_buffer, m_vma_allocation);
	}



	void Buffer::upload_data(void* data, size_t size, size_t offset)
	{
		auto backend = m_vk_backend.lock();

		if (m_vma_memory_usage == VMA_MEMORY_USAGE_GPU_ONLY)
		{
			// Create VMA_MEMORY_USAGE_CPU_ONLY staging buffer and perfom Buffer-to-Buffer copy
			Buffer::Ptr staging = Buffer::create(backend, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, size, VMA_MEMORY_USAGE_CPU_ONLY, VMA_ALLOCATION_CREATE_MAPPED_BIT, data);

			CommandBuffer::Ptr cmd_buf = backend->allocate_graphics_command_buffer();

			VkCommandBufferBeginInfo begin_info;
			SE_ZERO_MEMORY(begin_info);

			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			vkBeginCommandBuffer(cmd_buf->handle(), &begin_info);

			VkBufferCopy copy_region;
			SE_ZERO_MEMORY(copy_region);

			copy_region.dstOffset = offset;
			copy_region.size = size;

			vkCmdCopyBuffer(cmd_buf->handle(), staging->handle(), m_vk_buffer, 1, &copy_region);

			vkEndCommandBuffer(cmd_buf->handle());

			backend->flush_graphics({ cmd_buf });
		}
		else
		{
			if (!m_mapped_ptr)
				vkMapMemory(backend->device(), m_vk_device_memory, 0, size, 0, &m_mapped_ptr);

			memcpy(m_mapped_ptr, data, size);

			// If host coherency hasn't been requested, do a manual flush to make writes visible
			if ((m_vk_memory_property & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
			{
				VkMappedMemoryRange mapped_range;
				SE_ZERO_MEMORY(mapped_range);

				mapped_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				mapped_range.memory = m_vk_device_memory;
				mapped_range.offset = 0;
				mapped_range.size = VK_WHOLE_SIZE;

				vkFlushMappedMemoryRanges(backend->device(), 1, &mapped_range);
			}
		}
	}



	CommandPool::Ptr CommandPool::create(Backend::Ptr backend, uint32_t queue_family_index)
	{
		return std::shared_ptr<CommandPool>(new CommandPool(backend, queue_family_index));
	}



	CommandPool::CommandPool(Backend::Ptr backend, uint32_t queue_family_index) :
		Object(backend)
	{
		VkCommandPoolCreateInfo pool_info;
		SE_ZERO_MEMORY(pool_info);

		pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		pool_info.queueFamilyIndex = queue_family_index;

		if (vkCreateCommandPool(backend->device(), &pool_info, nullptr, &m_vk_pool) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Command Pool.");
			throw std::runtime_error("(Vulkan) Failed to create Command Pool.");
		}
	}



	CommandPool::~CommandPool()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyCommandPool(backend->device(), m_vk_pool, nullptr);
	}



	void CommandPool::reset()
	{
		auto backend = m_vk_backend.lock();

		if (vkResetCommandPool(backend->device(), m_vk_pool, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to reset Command Pool.");
			throw std::runtime_error("(Vulkan) Failed to reset Command Pool.");
		}
	}



	CommandBuffer::CommandBuffer(Backend::Ptr backend, CommandPool::Ptr pool) :
		Object(backend)
	{
		m_vk_pool = pool;

		VkCommandBufferAllocateInfo alloc_info;
		SE_ZERO_MEMORY(alloc_info);

		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandPool = pool->handle();
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(backend->device(), &alloc_info, &m_vk_command_buffer) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to allocate Command Buffer.");
			throw std::runtime_error("(Vulkan) Failed to allocate Command Buffer.");
		}
	}



	CommandBuffer::Ptr CommandBuffer::create(Backend::Ptr backend, CommandPool::Ptr pool)
	{
		return std::shared_ptr<CommandBuffer>(new CommandBuffer(backend, pool));
	}



	CommandBuffer::~CommandBuffer()
	{
		if (m_vk_backend.expired() || m_vk_pool.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();
		auto pool = m_vk_pool.lock();

		vkFreeCommandBuffers(backend->device(), pool->handle(), 1, &m_vk_command_buffer);
	}



	void CommandBuffer::reset()
	{
		vkResetCommandBuffer(m_vk_command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	}



	ShaderModule::Ptr ShaderModule::create_from_file(Backend::Ptr backend, std::string path)
	{
		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			throw std::runtime_error("Failed to open SPIRV shader!");

		size_t            file_size = (size_t)file.tellg();
		std::vector<char> buffer(file_size);

		file.seekg(0);
		file.read(buffer.data(), file_size);

		file.close();

		return std::shared_ptr<ShaderModule>(new ShaderModule(backend, buffer));
	}



	ShaderModule::Ptr ShaderModule::create(Backend::Ptr backend, std::vector<char> spirv)
	{
		return std::shared_ptr<ShaderModule>(new ShaderModule(backend, spirv));
	}



	ShaderModule::ShaderModule(Backend::Ptr backend, std::vector<char> spirv) :
		Object(backend)
	{
		VkShaderModuleCreateInfo create_info;
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = spirv.size();
		create_info.pCode = reinterpret_cast<const uint32_t*>(spirv.data());

		if (vkCreateShaderModule(backend->device(), &create_info, nullptr, &m_vk_module) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create shader module.");
			throw std::runtime_error("(Vulkan) Failed to create shader module.");
		}
	}



	ShaderModule::~ShaderModule()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyShaderModule(backend->device(), m_vk_module, nullptr);
	}



	VertexInputStateDesc::VertexInputStateDesc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		create_info.pVertexAttributeDescriptions = &attribute_desc[0];
		create_info.pVertexBindingDescriptions = &binding_desc[0];
	}



	VertexInputStateDesc& VertexInputStateDesc::add_binding_desc(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate)
	{
		binding_desc[create_info.vertexBindingDescriptionCount++] = { binding, stride, input_rate };
		return *this;
	}



	VertexInputStateDesc& VertexInputStateDesc::add_attribute_desc(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset)
	{
		attribute_desc[create_info.vertexAttributeDescriptionCount++] = { location, binding, format, offset };
		return *this;
	}



	InputAssemblyStateDesc::InputAssemblyStateDesc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	}



	InputAssemblyStateDesc& InputAssemblyStateDesc::set_flags(VkPipelineInputAssemblyStateCreateFlags flags)
	{
		create_info.flags = flags;
		return *this;
	}



	InputAssemblyStateDesc& InputAssemblyStateDesc::set_topology(VkPrimitiveTopology topology)
	{
		create_info.topology = topology;
		return *this;
	}



	InputAssemblyStateDesc& InputAssemblyStateDesc::set_primitive_restart_enable(bool primitive_restart_enable)
	{
		create_info.primitiveRestartEnable = primitive_restart_enable;
		return *this;
	}



	TessellationStateDesc::TessellationStateDesc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	}



	TessellationStateDesc& TessellationStateDesc::set_flags(VkPipelineTessellationStateCreateFlags flags)
	{
		create_info.flags = flags;
		return *this;
	}



	TessellationStateDesc& TessellationStateDesc::set_patch_control_points(uint32_t patch_control_points)
	{
		create_info.patchControlPoints = patch_control_points;
		return *this;
	}



	RasterizationStateDesc::RasterizationStateDesc()
	{
		SE_ZERO_MEMORY(create_info);
		SE_ZERO_MEMORY(conservative_raster_create_info);

		create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

		conservative_raster_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_depth_clamp(VkBool32 value)
	{
		create_info.depthClampEnable = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_rasterizer_discard_enable(VkBool32 value)
	{
		create_info.rasterizerDiscardEnable = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_polygon_mode(VkPolygonMode value)
	{
		create_info.polygonMode = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_cull_mode(VkCullModeFlags value)
	{
		create_info.cullMode = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_front_face(VkFrontFace value)
	{
		create_info.frontFace = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_depth_bias(VkBool32 value)
	{
		create_info.depthBiasEnable = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_depth_bias_constant_factor(float value)
	{
		create_info.depthBiasConstantFactor = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_depth_bias_clamp(float value)
	{
		create_info.depthBiasClamp = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_depth_bias_slope_factor(float value)
	{
		create_info.depthBiasSlopeFactor = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_line_width(float value)
	{
		create_info.lineWidth = value;
		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_conservative_raster_mode(VkConservativeRasterizationModeEXT value)
	{
		if (value != VK_CONSERVATIVE_RASTERIZATION_MODE_DISABLED_EXT)
			create_info.pNext = &conservative_raster_create_info;

		conservative_raster_create_info.conservativeRasterizationMode = value;

		return *this;
	}



	RasterizationStateDesc& RasterizationStateDesc::set_extra_primitive_overestimation_size(float value)
	{
		conservative_raster_create_info.extraPrimitiveOverestimationSize = value;

		return *this;
	}



	MultisampleStateDesc::MultisampleStateDesc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	}



	MultisampleStateDesc& MultisampleStateDesc::set_rasterization_samples(VkSampleCountFlagBits value)
	{
		create_info.rasterizationSamples = value;
		return *this;
	}



	MultisampleStateDesc& MultisampleStateDesc::set_sample_shading_enable(VkBool32 value)
	{
		create_info.sampleShadingEnable = value;
		return *this;
	}



	MultisampleStateDesc& MultisampleStateDesc::set_min_sample_shading(float value)
	{
		create_info.minSampleShading = value;
		return *this;
	}



	MultisampleStateDesc& MultisampleStateDesc::set_sample_mask(VkSampleMask* value)
	{
		create_info.pSampleMask = value;
		return *this;
	}



	MultisampleStateDesc& MultisampleStateDesc::set_alpha_to_coverage_enable(VkBool32 value)
	{
		create_info.alphaToCoverageEnable = value;
		return *this;
	}



	MultisampleStateDesc& MultisampleStateDesc::set_alpha_to_one_enable(VkBool32 value)
	{
		create_info.alphaToOneEnable = value;
		return *this;
	}



	StencilOpStateDesc& StencilOpStateDesc::set_fail_op(VkStencilOp value)
	{
		create_info.failOp = value;
		return *this;
	}



	StencilOpStateDesc& StencilOpStateDesc::set_pass_op(VkStencilOp value)
	{
		create_info.passOp = value;
		return *this;
	}



	StencilOpStateDesc& StencilOpStateDesc::set_depth_fail_op(VkStencilOp value)
	{
		create_info.depthFailOp = value;
		return *this;
	}



	StencilOpStateDesc& StencilOpStateDesc::set_compare_op(VkCompareOp value)
	{
		create_info.compareOp = value;
		return *this;
	}



	StencilOpStateDesc& StencilOpStateDesc::set_compare_mask(uint32_t value)
	{
		create_info.compareMask = value;
		return *this;
	}



	StencilOpStateDesc& StencilOpStateDesc::set_write_mask(uint32_t value)
	{
		create_info.writeMask = value;
		return *this;
	}



	StencilOpStateDesc& StencilOpStateDesc::set_reference(uint32_t value)
	{
		create_info.reference = value;
		return *this;
	}



	DepthStencilStateDesc::DepthStencilStateDesc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_depth_test_enable(VkBool32 value)
	{
		create_info.depthTestEnable = value;
		return *this;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_depth_write_enable(VkBool32 value)
	{
		create_info.depthWriteEnable = value;
		return *this;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_depth_compare_op(VkCompareOp value)
	{
		create_info.depthCompareOp = value;
		return *this;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_depth_bounds_test_enable(VkBool32 value)
	{
		create_info.depthBoundsTestEnable = value;
		return *this;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_stencil_test_enable(VkBool32 value)
	{
		create_info.stencilTestEnable = value;
		return *this;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_front(StencilOpStateDesc value)
	{
		create_info.front = value.create_info;
		return *this;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_back(StencilOpStateDesc value)
	{
		create_info.back = value.create_info;
		return *this;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_min_depth_bounds(float value)
	{
		create_info.minDepthBounds = value;
		return *this;
	}



	DepthStencilStateDesc& DepthStencilStateDesc::set_max_depth_bounds(float value)
	{
		create_info.maxDepthBounds = value;
		return *this;
	}



	ColorBlendAttachmentStateDesc::ColorBlendAttachmentStateDesc()
	{
		SE_ZERO_MEMORY(create_info);
	}



	ColorBlendAttachmentStateDesc& ColorBlendAttachmentStateDesc::set_blend_enable(VkBool32 value)
	{
		create_info.blendEnable = value;
		return *this;
	}



	ColorBlendAttachmentStateDesc& ColorBlendAttachmentStateDesc::set_src_color_blend_factor(VkBlendFactor value)
	{
		create_info.srcColorBlendFactor = value;
		return *this;
	}



	ColorBlendAttachmentStateDesc& ColorBlendAttachmentStateDesc::set_dst_color_blend_Factor(VkBlendFactor value)
	{
		create_info.dstColorBlendFactor = value;
		return *this;
	}



	ColorBlendAttachmentStateDesc& ColorBlendAttachmentStateDesc::set_color_blend_op(VkBlendOp value)
	{
		create_info.colorBlendOp = value;
		return *this;
	}



	ColorBlendAttachmentStateDesc& ColorBlendAttachmentStateDesc::set_src_alpha_blend_factor(VkBlendFactor value)
	{
		create_info.srcAlphaBlendFactor = value;
		return *this;
	}



	ColorBlendAttachmentStateDesc& ColorBlendAttachmentStateDesc::set_dst_alpha_blend_factor(VkBlendFactor value)
	{
		create_info.dstAlphaBlendFactor = value;
		return *this;
	}



	ColorBlendAttachmentStateDesc& ColorBlendAttachmentStateDesc::set_alpha_blend_op(VkBlendOp value)
	{
		create_info.alphaBlendOp = value;
		return *this;
	}



	ColorBlendAttachmentStateDesc& ColorBlendAttachmentStateDesc::set_color_write_mask(VkColorComponentFlags value)
	{
		create_info.colorWriteMask = value;
		return *this;
	}



	ColorBlendStateDesc::ColorBlendStateDesc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	}



	ColorBlendStateDesc& ColorBlendStateDesc::set_logic_op_enable(VkBool32 value)
	{
		create_info.logicOpEnable = value;
		return *this;
	}



	ColorBlendStateDesc& ColorBlendStateDesc::set_logic_op(VkLogicOp value)
	{
		create_info.logicOp = value;
		return *this;
	}



	ColorBlendStateDesc& ColorBlendStateDesc::add_attachment(ColorBlendAttachmentStateDesc att)
	{
		if (create_info.attachmentCount == 0)
			create_info.pAttachments = &attachments[0];

		attachments[create_info.attachmentCount++] = att.create_info;

		return *this;
	}



	ColorBlendStateDesc& ColorBlendStateDesc::set_blend_constants(float r, float g, float b, float a)
	{
		create_info.blendConstants[0] = r;
		create_info.blendConstants[1] = g;
		create_info.blendConstants[2] = b;
		create_info.blendConstants[3] = a;
		return *this;
	}



	ViewportStateDesc::ViewportStateDesc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

		for (int i = 0; i < 32; i++)
		{
			SE_ZERO_MEMORY(viewports[i]);
			SE_ZERO_MEMORY(scissors[i]);
		}
	}



	ViewportStateDesc& ViewportStateDesc::add_viewport(float x,
		float y,
		float width,
		float height,
		float min_depth,
		float max_depth)
	{
		if (viewport_count == 32)
		{
			SE_LOG_FATAL("(Vulkan) Max viewport count reached.");
			throw std::runtime_error("(Vulkan) Max viewport count reached.");
		}

		uint32_t idx = viewport_count++;

		viewports[idx].x = x;
		viewports[idx].y = y;
		viewports[idx].width = width;
		viewports[idx].height = height;
		viewports[idx].minDepth = min_depth;
		viewports[idx].maxDepth = max_depth;

		return *this;
	}



	ViewportStateDesc& ViewportStateDesc::add_scissor(int32_t  x,
		int32_t  y,
		uint32_t w,
		uint32_t h)
	{
		if (scissor_count == 32)
		{
			SE_LOG_FATAL("(Vulkan) Max scissor count reached.");
			throw std::runtime_error("(Vulkan) Max scissor count reached.");
		}

		uint32_t idx = scissor_count++;

		scissors[idx].extent.width = w;
		scissors[idx].extent.height = h;
		scissors[idx].offset.x = x;
		scissors[idx].offset.y = y;

		return *this;
	}



	GraphicsPipeline::Desc::Desc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		for (uint32_t i = 0; i < 6; i++)
		{
			SE_ZERO_MEMORY(shader_stages[i]);
			shader_stages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		}

		SE_ZERO_MEMORY(dynamic_state);

		dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state.pDynamicStates = nullptr;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::add_dynamic_state(const VkDynamicState& state)
	{
		if (dynamic_state_count == 32)
		{
			SE_LOG_FATAL("(Vulkan) Max dynamic state count reached.");
			throw std::runtime_error("(Vulkan) Max dynamic state count reached.");
		}

		dynamic_states[dynamic_state_count++] = state;

		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_viewport_state(ViewportStateDesc& state)
	{
		state.create_info.viewportCount = state.viewport_count;
		state.create_info.scissorCount = state.scissor_count;
		state.create_info.pScissors = state.scissors;
		state.create_info.pViewports = state.viewports;
		create_info.pViewportState = &state.create_info;

		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::add_shader_stage(const VkShaderStageFlagBits& stage, const ShaderModule::Ptr& shader_module, const std::string& name)
	{
		uint32_t idx = shader_stage_count++;

		shader_entry_names[idx] = name;
		shader_stages[idx].module = shader_module->handle();
		shader_stages[idx].pName = shader_entry_names[idx].c_str();
		shader_stages[idx].stage = stage;

		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_vertex_input_state(const VertexInputStateDesc& state)
	{
		create_info.pVertexInputState = &state.create_info;
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_input_assembly_state(const InputAssemblyStateDesc& state)
	{
		create_info.pInputAssemblyState = &state.create_info;
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_tessellation_state(const TessellationStateDesc& state)
	{
		create_info.pTessellationState = &state.create_info;
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_rasterization_state(const RasterizationStateDesc& state)
	{
		create_info.pRasterizationState = &state.create_info;
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_multisample_state(const MultisampleStateDesc& state)
	{
		create_info.pMultisampleState = &state.create_info;
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_depth_stencil_state(const DepthStencilStateDesc& state)
	{
		create_info.pDepthStencilState = &state.create_info;
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_color_blend_state(const ColorBlendStateDesc& state)
	{
		create_info.pColorBlendState = &state.create_info;
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_pipeline_layout(const std::shared_ptr<PipelineLayout>& layout)
	{
		create_info.layout = layout->handle();
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_render_pass(const RenderPass::Ptr& render_pass)
	{
		create_info.renderPass = render_pass->handle();
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_sub_pass(const uint32_t& subpass)
	{
		create_info.subpass = subpass;
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_base_pipeline(const GraphicsPipeline::Ptr& pipeline)
	{
		create_info.basePipelineHandle = pipeline->handle();
		return *this;
	}



	GraphicsPipeline::Desc& GraphicsPipeline::Desc::set_base_pipeline_index(const int32_t& index)
	{
		create_info.basePipelineIndex = index;
		return *this;
	}



	GraphicsPipeline::Ptr GraphicsPipeline::create_for_post_process(Backend::Ptr backend, std::string vs, std::string fs, std::shared_ptr<PipelineLayout> pipeline_layout, RenderPass::Ptr render_pass)
	{
		// ---------------------------------------------------------------------------
		// Create shader modules
		// ---------------------------------------------------------------------------

		vk::ShaderModule::Ptr vs_module = vk::ShaderModule::create_from_file(backend, vs);
		vk::ShaderModule::Ptr fs_module = vk::ShaderModule::create_from_file(backend, fs);

		vk::GraphicsPipeline::Desc pso_desc;

		pso_desc.add_shader_stage(VK_SHADER_STAGE_VERTEX_BIT, vs_module, "main")
			.add_shader_stage(VK_SHADER_STAGE_FRAGMENT_BIT, fs_module, "main");

		// ---------------------------------------------------------------------------
		// Vertex input state
		// ---------------------------------------------------------------------------

		VertexInputStateDesc vs_desc;

		pso_desc.set_vertex_input_state(vs_desc);

		// ---------------------------------------------------------------------------
		// Create pipeline input assembly state
		// ---------------------------------------------------------------------------

		vk::InputAssemblyStateDesc input_assembly_state_desc;

		input_assembly_state_desc.set_primitive_restart_enable(false)
			.set_topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		pso_desc.set_input_assembly_state(input_assembly_state_desc);

		// ---------------------------------------------------------------------------
		// Create viewport state
		// ---------------------------------------------------------------------------

		vk::ViewportStateDesc vp_desc;

		vp_desc.add_viewport(0.0f, 0.0f, 1, 1, 0.0f, 1.0f)
			.add_scissor(0, 0, 1, 1);

		pso_desc.set_viewport_state(vp_desc);

		// ---------------------------------------------------------------------------
		// Create rasterization state
		// ---------------------------------------------------------------------------

		vk::RasterizationStateDesc rs_state;

		rs_state.set_depth_clamp(VK_FALSE)
			.set_rasterizer_discard_enable(VK_FALSE)
			.set_polygon_mode(VK_POLYGON_MODE_FILL)
			.set_line_width(1.0f)
			.set_cull_mode(VK_CULL_MODE_NONE)
			.set_front_face(VK_FRONT_FACE_COUNTER_CLOCKWISE)
			.set_depth_bias(VK_FALSE);

		pso_desc.set_rasterization_state(rs_state);

		// ---------------------------------------------------------------------------
		// Create multisample state
		// ---------------------------------------------------------------------------

		vk::MultisampleStateDesc ms_state;

		ms_state.set_sample_shading_enable(VK_FALSE)
			.set_rasterization_samples(VK_SAMPLE_COUNT_1_BIT);

		pso_desc.set_multisample_state(ms_state);

		// ---------------------------------------------------------------------------
		// Create depth stencil state
		// ---------------------------------------------------------------------------

		vk::DepthStencilStateDesc ds_state;

		ds_state.set_depth_test_enable(VK_FALSE)
			.set_depth_write_enable(VK_FALSE)
			.set_depth_compare_op(VK_COMPARE_OP_LESS)
			.set_depth_bounds_test_enable(VK_FALSE)
			.set_stencil_test_enable(VK_FALSE);

		pso_desc.set_depth_stencil_state(ds_state);

		// ---------------------------------------------------------------------------
		// Create color blend state
		// ---------------------------------------------------------------------------

		vk::ColorBlendAttachmentStateDesc blend_att_desc;

		blend_att_desc.set_color_write_mask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
			.set_blend_enable(VK_FALSE);

		vk::ColorBlendStateDesc blend_state;

		blend_state.set_logic_op_enable(VK_FALSE)
			.set_logic_op(VK_LOGIC_OP_COPY)
			.set_blend_constants(0.0f, 0.0f, 0.0f, 0.0f)
			.add_attachment(blend_att_desc);

		pso_desc.set_color_blend_state(blend_state);

		// ---------------------------------------------------------------------------
		// Create pipeline layout
		// ---------------------------------------------------------------------------

		pso_desc.set_pipeline_layout(pipeline_layout);

		// ---------------------------------------------------------------------------
		// Create dynamic state
		// ---------------------------------------------------------------------------

		pso_desc.add_dynamic_state(VK_DYNAMIC_STATE_VIEWPORT)
			.add_dynamic_state(VK_DYNAMIC_STATE_SCISSOR);

		// ---------------------------------------------------------------------------
		// Create pipeline
		// ---------------------------------------------------------------------------

		pso_desc.set_render_pass(render_pass);

		return vk::GraphicsPipeline::create(backend, pso_desc);
	}



	GraphicsPipeline::Ptr GraphicsPipeline::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<GraphicsPipeline>(new GraphicsPipeline(backend, desc));
	}



	GraphicsPipeline::GraphicsPipeline(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		desc.create_info.pStages = &desc.shader_stages[0];
		desc.create_info.stageCount = desc.shader_stage_count;
		desc.dynamic_state.dynamicStateCount = desc.dynamic_state_count;
		desc.dynamic_state.pDynamicStates = &desc.dynamic_states[0];
		desc.create_info.pDynamicState = &desc.dynamic_state;

		if (vkCreateGraphicsPipelines(backend->device(), nullptr, 1, &desc.create_info, nullptr, &m_vk_pipeline) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Graphics Pipeline.");
			throw std::runtime_error("(Vulkan) Failed to create Graphics Pipeline.");
		}
	}



	GraphicsPipeline::~GraphicsPipeline()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyPipeline(backend->device(), m_vk_pipeline, nullptr);
	}



	ComputePipeline::Desc::Desc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		create_info.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	}



	ComputePipeline::Desc& ComputePipeline::Desc::set_shader_stage(ShaderModule::Ptr shader_module, std::string name)
	{
		shader_entry_name = name;
		create_info.stage.pName = shader_entry_name.c_str();
		create_info.stage.module = shader_module->handle();
		create_info.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;

		return *this;
	}



	ComputePipeline::Desc& ComputePipeline::Desc::set_pipeline_layout(std::shared_ptr<PipelineLayout> layout)
	{
		create_info.layout = layout->handle();
		return *this;
	}



	ComputePipeline::Desc& ComputePipeline::Desc::set_base_pipeline(ComputePipeline::Ptr pipeline)
	{
		create_info.basePipelineHandle = pipeline->handle();
		return *this;
	}



	ComputePipeline::Desc& ComputePipeline::Desc::set_base_pipeline_index(int32_t index)
	{
		create_info.basePipelineIndex = index;
		return *this;
	}



	ComputePipeline::Ptr ComputePipeline::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<ComputePipeline>(new ComputePipeline(backend, desc));
	}



	ComputePipeline::ComputePipeline(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		if (vkCreateComputePipelines(backend->device(), nullptr, 1, &desc.create_info, nullptr, &m_vk_pipeline) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Compute Pipeline.");
			throw std::runtime_error("(Vulkan) Failed to create Compute Pipeline.");
		}
	}



	ComputePipeline::~ComputePipeline()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyPipeline(backend->device(), m_vk_pipeline, nullptr);
	}



	ShaderBindingTable::Desc::Desc()
	{
		entry_point_names.reserve(32);
		ray_gen_stages.reserve(32);
		hit_stages.reserve(32);
		miss_stages.reserve(32);
		hit_groups.reserve(32);
	}



	ShaderBindingTable::Desc& ShaderBindingTable::Desc::add_ray_gen_group(ShaderModule::Ptr shader, const std::string& entry_point)
	{
		VkPipelineShaderStageCreateInfo stage;
		SE_ZERO_MEMORY(stage);

		entry_point_names.push_back(entry_point);

		stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stage.module = shader->handle();
		stage.stage = VK_SHADER_STAGE_RAYGEN_BIT_NV;
		stage.pName = entry_point_names.back().c_str();

		ray_gen_stages.push_back(stage);

		return *this;
	}



	ShaderBindingTable::Desc& ShaderBindingTable::Desc::add_hit_group(ShaderModule::Ptr  closest_hit_shader,
		const std::string& closest_hit_entry_point,
		ShaderModule::Ptr  any_hit_shader,
		const std::string& any_hit_entry_point,
		ShaderModule::Ptr  intersection_shader,
		const std::string& intersection_entry_point)
	{
		HitGroupDesc group_desc;

		VkPipelineShaderStageCreateInfo closest_hit_stage;
		SE_ZERO_MEMORY(closest_hit_stage);

		entry_point_names.push_back(closest_hit_entry_point);

		closest_hit_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		closest_hit_stage.module = closest_hit_shader->handle();
		closest_hit_stage.stage = VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV;
		closest_hit_stage.pName = entry_point_names.back().c_str();

		hit_stages.push_back(closest_hit_stage);

		group_desc.closest_hit_stage = &hit_stages.back();

		if (any_hit_shader)
		{
			VkPipelineShaderStageCreateInfo any_hit_stage;
			SE_ZERO_MEMORY(any_hit_stage);

			entry_point_names.push_back(any_hit_entry_point);

			any_hit_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			any_hit_stage.module = any_hit_shader->handle();
			any_hit_stage.stage = VK_SHADER_STAGE_ANY_HIT_BIT_NV;
			any_hit_stage.pName = entry_point_names.back().c_str();

			hit_stages.push_back(any_hit_stage);

			group_desc.any_hit_stage = &hit_stages.back();
		}

		if (intersection_shader)
		{
			VkPipelineShaderStageCreateInfo intersection_stage;
			SE_ZERO_MEMORY(intersection_stage);

			entry_point_names.push_back(intersection_entry_point);

			intersection_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			intersection_stage.module = intersection_shader->handle();
			intersection_stage.stage = VK_SHADER_STAGE_INTERSECTION_BIT_NV;
			intersection_stage.pName = entry_point_names.back().c_str();

			hit_stages.push_back(intersection_stage);

			group_desc.intersection_stage = &hit_stages.back();
		}

		hit_groups.push_back(group_desc);

		return *this;
	}



	ShaderBindingTable::Desc& ShaderBindingTable::Desc::add_miss_group(ShaderModule::Ptr shader, const std::string& entry_point)
	{
		VkPipelineShaderStageCreateInfo stage;
		SE_ZERO_MEMORY(stage);

		entry_point_names.push_back(entry_point);

		stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stage.module = shader->handle();
		stage.stage = VK_SHADER_STAGE_MISS_BIT_NV;
		stage.pName = entry_point_names.back().c_str();

		miss_stages.push_back(stage);

		return *this;
	}



	ShaderBindingTable::Ptr ShaderBindingTable::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<ShaderBindingTable>(new ShaderBindingTable(backend, desc));
	}



	VkDeviceSize ShaderBindingTable::hit_group_offset()
	{
		return m_ray_gen_size + m_miss_group_size;
	}



	VkDeviceSize ShaderBindingTable::miss_group_offset()
	{
		return m_ray_gen_size;
	}



	ShaderBindingTable::~ShaderBindingTable()
	{
	}



	ShaderBindingTable::ShaderBindingTable(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		m_entry_point_names.reserve(32);

		auto& rt_props = backend->m_physicalDevice.ray_tracing_properties();

		// Ray gen shaders
		for (auto& stage : desc.ray_gen_stages)
		{
			VkRayTracingShaderGroupCreateInfoNV group_info;
			SE_ZERO_MEMORY(group_info);

			m_entry_point_names.push_back(std::string(stage.pName));

			stage.pName = m_entry_point_names[m_entry_point_names.size() - 1].c_str();

			group_info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV;
			group_info.pNext = nullptr;
			group_info.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV;
			group_info.generalShader = m_stages.size();
			group_info.closestHitShader = VK_SHADER_UNUSED_NV;
			group_info.anyHitShader = VK_SHADER_UNUSED_NV;
			group_info.intersectionShader = VK_SHADER_UNUSED_NV;

			m_stages.push_back(stage);
			m_groups.push_back(group_info);
		}

		// Ray miss shaders
		for (auto& stage : desc.miss_stages)
		{
			VkRayTracingShaderGroupCreateInfoNV group_info;
			SE_ZERO_MEMORY(group_info);

			group_info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV;
			group_info.pNext = nullptr;
			group_info.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV;
			group_info.generalShader = m_stages.size();
			group_info.closestHitShader = VK_SHADER_UNUSED_NV;
			group_info.anyHitShader = VK_SHADER_UNUSED_NV;
			group_info.intersectionShader = VK_SHADER_UNUSED_NV;

			m_entry_point_names.push_back(std::string(stage.pName));
			stage.pName = m_entry_point_names[m_entry_point_names.size() - 1].c_str();

			m_stages.push_back(stage);
			m_groups.push_back(group_info);
		}

		// Ray hit shaders
		for (auto& group : desc.hit_groups)
		{
			if (!group.closest_hit_stage)
			{
				SE_LOG_FATAL("(Vulkan) Hit shader group does not have Closest Hit stage.");
				throw std::runtime_error("(Vulkan) Hit shader group does not have Closest Hit stage.");
			}

			VkRayTracingShaderGroupCreateInfoNV group_info;
			SE_ZERO_MEMORY(group_info);

			group_info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV;
			group_info.pNext = nullptr;
			group_info.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_NV;
			group_info.generalShader = VK_SHADER_UNUSED_NV;
			group_info.closestHitShader = m_stages.size();
			group_info.anyHitShader = VK_SHADER_UNUSED_NV;
			group_info.intersectionShader = VK_SHADER_UNUSED_NV;

			m_entry_point_names.push_back(std::string(group.closest_hit_stage->pName));
			group.closest_hit_stage->pName = m_entry_point_names[m_entry_point_names.size() - 1].c_str();

			m_stages.push_back(*group.closest_hit_stage);

			if (group.any_hit_stage)
			{
				group_info.anyHitShader = m_stages.size();

				m_entry_point_names.push_back(std::string(group.any_hit_stage->pName));
				group.any_hit_stage->pName = m_entry_point_names[m_entry_point_names.size() - 1].c_str();

				m_stages.push_back(*group.any_hit_stage);
			}

			if (group.intersection_stage)
			{
				group_info.intersectionShader = m_stages.size();

				m_entry_point_names.push_back(std::string(group.intersection_stage->pName));
				group.intersection_stage->pName = m_entry_point_names[m_entry_point_names.size() - 1].c_str();

				m_stages.push_back(*group.intersection_stage);
			}

			m_groups.push_back(group_info);
		}

		m_ray_gen_size = desc.ray_gen_stages.size() * rt_props.shaderGroupHandleSize;
		m_hit_group_size = desc.hit_groups.size() * rt_props.shaderGroupHandleSize;
		m_miss_group_size = desc.miss_stages.size() * rt_props.shaderGroupHandleSize;
	}



	RayTracingPipeline::Desc::Desc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
		create_info.pNext = nullptr;
	}



	RayTracingPipeline::Desc& RayTracingPipeline::Desc::set_shader_binding_table(ShaderBindingTable::Ptr table)
	{
		sbt = table;
		const std::vector<VkPipelineShaderStageCreateInfo>& stages = table->stages();
		const std::vector<VkRayTracingShaderGroupCreateInfoNV>& groups = table->groups();

		create_info.groupCount = groups.size();
		create_info.pGroups = groups.data();

		create_info.stageCount = stages.size();
		create_info.pStages = stages.data();

		return *this;
	}



	RayTracingPipeline::Desc& RayTracingPipeline::Desc::set_pipeline_layout(std::shared_ptr<PipelineLayout> layout)
	{
		create_info.layout = layout->handle();
		return *this;
	}



	RayTracingPipeline::Desc& RayTracingPipeline::Desc::set_recursion_depth(uint32_t depth)
	{
		create_info.maxRecursionDepth = depth;
		return *this;
	}



	RayTracingPipeline::Desc& RayTracingPipeline::Desc::set_base_pipeline(RayTracingPipeline::Ptr pipeline)
	{
		create_info.basePipelineHandle = pipeline->handle();
		return *this;
	}



	RayTracingPipeline::Desc& RayTracingPipeline::Desc::set_base_pipeline_index(int32_t index)
	{
		create_info.basePipelineIndex = index;
		return *this;
	}



	RayTracingPipeline::Ptr RayTracingPipeline::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<RayTracingPipeline>(new RayTracingPipeline(backend, desc));
	}



	RayTracingPipeline::~RayTracingPipeline()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		m_vk_buffer.reset();
		m_sbt.reset();
		vkDestroyPipeline(backend->device(), m_vk_pipeline, nullptr);
	}



	RayTracingPipeline::RayTracingPipeline(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		m_sbt = desc.sbt;

		desc.create_info.pGroups = m_sbt->groups().data();
		desc.create_info.pStages = m_sbt->stages().data();

		if (vkCreateRayTracingPipelinesNV(backend->device(), VK_NULL_HANDLE, 1, &desc.create_info, VK_NULL_HANDLE, &m_vk_pipeline) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Ray Tracing Pipeline.");
			throw std::runtime_error("(Vulkan) Failed to create Ray Tracing Pipeline.");
		}

		const auto& rt_props = backend->m_physicalDevice.ray_tracing_properties();

		size_t sbt_size = m_sbt->groups().size() * rt_props.shaderGroupHandleSize;

		m_vk_buffer = vk::Buffer::create(backend, VK_BUFFER_USAGE_RAY_TRACING_BIT_NV, sbt_size, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);

		if (vkGetRayTracingShaderGroupHandlesNV(backend->device(), m_vk_pipeline, 0, m_sbt->groups().size(), sbt_size, m_vk_buffer->mapped_ptr()) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to get Shader Group handles.");
			throw std::runtime_error("(Vulkan) Failed to get Shader Group handles.");
		}
	}



	AccelerationStructure::Desc::Desc()
	{
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV;
		create_info.pNext = nullptr;
		create_info.compactedSize = 0;

		create_info.info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV;
		create_info.info.pNext = nullptr;
		create_info.info.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_NV;
	}



	AccelerationStructure::Desc& AccelerationStructure::Desc::set_type(VkAccelerationStructureTypeNV type)
	{
		create_info.info.type = type;
		return *this;
	}



	AccelerationStructure::Desc& AccelerationStructure::Desc::set_geometries(std::vector<VkGeometryNV> geometry_vec)
	{
		create_info.info.geometryCount = geometry_vec.size();
		geometries = geometry_vec;
		create_info.info.pGeometries = geometries.data();
		return *this;
	}



	AccelerationStructure::Desc& AccelerationStructure::Desc::set_instance_count(uint32_t count)
	{
		create_info.info.instanceCount = count;
		return *this;
	}



	AccelerationStructure::Ptr AccelerationStructure::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<AccelerationStructure>(new AccelerationStructure(backend, desc));
	}



	AccelerationStructure::~AccelerationStructure()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyAccelerationStructureNV(backend->device(), m_vk_acceleration_structure, nullptr);
		vmaFreeMemory(backend->allocator(), m_vma_allocation);
	}



	AccelerationStructure::AccelerationStructure(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		m_vk_acceleration_structure_info = desc.create_info.info;

		if (vkCreateAccelerationStructureNV(backend->device(), &desc.create_info, nullptr, &m_vk_acceleration_structure) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Acceleration Structure.");
			throw std::runtime_error("(Vulkan) Failed to create Acceleration Structure.");
		}

		VkAccelerationStructureMemoryRequirementsInfoNV memory_requirements_info;
		SE_ZERO_MEMORY(memory_requirements_info);

		memory_requirements_info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV;
		memory_requirements_info.pNext = nullptr;
		memory_requirements_info.type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_OBJECT_NV;
		memory_requirements_info.accelerationStructure = m_vk_acceleration_structure;

		VkMemoryRequirements2 memory_requirements;
		vkGetAccelerationStructureMemoryRequirementsNV(backend->device(), &memory_requirements_info, &memory_requirements);

		VmaAllocationInfo       alloc_info;
		VmaAllocationCreateInfo alloc_create_info;
		SE_ZERO_MEMORY(alloc_create_info);

		alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		alloc_create_info.flags = 0;

		vmaAllocateMemory(backend->allocator(), &memory_requirements.memoryRequirements, &alloc_create_info, &m_vma_allocation, &alloc_info);

		VkBindAccelerationStructureMemoryInfoNV bind_info;
		bind_info.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV;
		bind_info.pNext = nullptr;
		bind_info.accelerationStructure = m_vk_acceleration_structure;
		bind_info.memory = alloc_info.deviceMemory;
		bind_info.memoryOffset = alloc_info.offset;
		bind_info.deviceIndexCount = 0;
		bind_info.pDeviceIndices = nullptr;

		if (vkBindAccelerationStructureMemoryNV(backend->device(), 1, &bind_info) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Acceleration Structure.");
			throw std::runtime_error("(Vulkan) Failed to create Acceleration Structure.");
		}

		if (vkGetAccelerationStructureHandleNV(backend->device(), m_vk_acceleration_structure, sizeof(uint64_t), &m_handle) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Acceleration Structure.");
			throw std::runtime_error("(Vulkan) Failed to create Acceleration Structure.");
		}
	}



	Sampler::Ptr Sampler::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<Sampler>(new Sampler(backend, desc));
	}



	Sampler::Sampler(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		VkSamplerCreateInfo info;
		SE_ZERO_MEMORY(info);

		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.flags = desc.flags;
		info.magFilter = desc.mag_filter;
		info.minFilter = desc.min_filter;
		info.mipmapMode = desc.mipmap_mode;
		info.addressModeU = desc.address_mode_u;
		info.addressModeV = desc.address_mode_v;
		info.addressModeW = desc.address_mode_w;
		info.mipLodBias = desc.mip_lod_bias;
		info.anisotropyEnable = desc.anisotropy_enable;
		info.maxAnisotropy = desc.max_anisotropy;
		info.compareEnable = desc.compare_enable;
		info.compareOp = desc.compare_op;
		info.minLod = desc.min_lod;
		info.maxLod = desc.max_lod;
		info.borderColor = desc.border_color;
		info.unnormalizedCoordinates = desc.unnormalized_coordinates;

		if (vkCreateSampler(backend->device(), &info, nullptr, &m_vk_sampler) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create sampler.");
			throw std::runtime_error("(Vulkan) Failed to create sampler.");
		}
	}



	Sampler::~Sampler()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroySampler(backend->device(), m_vk_sampler, nullptr);
	}



	DescriptorSetLayout::Desc& DescriptorSetLayout::Desc::set_next_ptr(void* pnext)
	{
		pnext_ptr = pnext;
		return *this;
	}

	DescriptorSetLayout::Desc& DescriptorSetLayout::Desc::add_binding(uint32_t binding, VkDescriptorType descriptor_type, uint32_t descriptor_count, VkShaderStageFlags stage_flags)
	{
		bindings.push_back({ binding, descriptor_type, descriptor_count, stage_flags, nullptr });
		return *this;
	}



	DescriptorSetLayout::Desc& DescriptorSetLayout::Desc::add_binding(uint32_t binding, VkDescriptorType descriptor_type, uint32_t descriptor_count, VkShaderStageFlags stage_flags, Sampler::Ptr samplers[])
	{
		for (int i = 0; i < descriptor_count; i++)
			binding_samplers[binding][i] = samplers[i]->handle();

		bindings.push_back({ binding, descriptor_type, descriptor_count, stage_flags, &binding_samplers[binding][0] });
		return *this;
	}



	DescriptorSetLayout::Ptr DescriptorSetLayout::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<DescriptorSetLayout>(new DescriptorSetLayout(backend, desc));
	}



	DescriptorSetLayout::DescriptorSetLayout(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		VkDescriptorSetLayoutCreateInfo layout_info;
		SE_ZERO_MEMORY(layout_info);

		layout_info.pNext = desc.pnext_ptr;
		layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout_info.bindingCount = desc.bindings.size();
		layout_info.pBindings = desc.bindings.data();

		if (vkCreateDescriptorSetLayout(backend->device(), &layout_info, nullptr, &m_vk_ds_layout) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Descriptor Set Layout.");
			throw std::runtime_error("(Vulkan) Failed to create Descriptor Set Layout.");
		}
	}



	DescriptorSetLayout::~DescriptorSetLayout()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyDescriptorSetLayout(backend->device(), m_vk_ds_layout, nullptr);
	}



	PipelineLayout::Desc& PipelineLayout::Desc::add_descriptor_set_layout(DescriptorSetLayout::Ptr layout)
	{
		layouts.push_back(layout);
		return *this;
	}



	PipelineLayout::Desc& PipelineLayout::Desc::add_push_constant_range(VkShaderStageFlags stage_flags, uint32_t offset, uint32_t size)
	{
		push_constant_ranges.push_back({ stage_flags, offset, size });
		return *this;
	}



	PipelineLayout::Ptr PipelineLayout::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<PipelineLayout>(new PipelineLayout(backend, desc));
	}



	PipelineLayout::PipelineLayout(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		std::vector<VkDescriptorSetLayout> vk_layouts(desc.layouts.size());

		for (uint32_t i = 0; i < desc.layouts.size(); i++)
			vk_layouts[i] = desc.layouts[i]->handle();

		VkPipelineLayoutCreateInfo info;
		SE_ZERO_MEMORY(info);

		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.pushConstantRangeCount = desc.push_constant_ranges.size();
		info.pPushConstantRanges = desc.push_constant_ranges.data();
		info.setLayoutCount = desc.layouts.size();
		info.pSetLayouts = vk_layouts.data();

		if (vkCreatePipelineLayout(backend->device(), &info, nullptr, &m_vk_pipeline_layout) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create pipeline layout.");
			throw std::runtime_error("(Vulkan) Failed to create pipeline layout.");
		}
	}



	PipelineLayout::~PipelineLayout()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyPipelineLayout(backend->device(), m_vk_pipeline_layout, nullptr);
	}



	DescriptorPool::Desc& DescriptorPool::Desc::set_max_sets(uint32_t num)
	{
		max_sets = num;
		return *this;
	}



	DescriptorPool::Desc& DescriptorPool::Desc::set_create_flags(VkDescriptorPoolCreateFlags flags)
	{
		create_flags = flags;
		return *this;
	}



	DescriptorPool::Desc& DescriptorPool::Desc::add_pool_size(VkDescriptorType type, uint32_t descriptor_count)
	{
		pool_sizes.push_back({ type, descriptor_count });
		return *this;
	}



	DescriptorPool::Ptr DescriptorPool::create(Backend::Ptr backend, Desc desc)
	{
		return std::shared_ptr<DescriptorPool>(new DescriptorPool(backend, desc));
	}



	DescriptorPool::DescriptorPool(Backend::Ptr backend, Desc desc) :
		Object(backend)
	{
		m_vk_create_flags = desc.create_flags;

		VkDescriptorPoolCreateInfo pool_info;
		SE_ZERO_MEMORY(pool_info);

		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount = static_cast<uint32_t>(desc.pool_sizes.size());
		pool_info.pPoolSizes = desc.pool_sizes.data();
		pool_info.maxSets = desc.max_sets;
		pool_info.flags = desc.create_flags;

		if (vkCreateDescriptorPool(backend->device(), &pool_info, nullptr, &m_vk_ds_pool) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create descriptor pool.");
			throw std::runtime_error("(Vulkan) Failed to create descriptor pool.");
		}
	}



	DescriptorPool::~DescriptorPool()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyDescriptorPool(backend->device(), m_vk_ds_pool, nullptr);
	}



	DescriptorSet::Ptr DescriptorSet::create(Backend::Ptr backend, DescriptorSetLayout::Ptr layout, DescriptorPool::Ptr pool)
	{
		return std::shared_ptr<DescriptorSet>(new DescriptorSet(backend, layout, pool));
	}



	DescriptorSet::DescriptorSet(Backend::Ptr backend, DescriptorSetLayout::Ptr layout, DescriptorPool::Ptr pool) :
		Object(backend)
	{
		m_vk_pool = pool;

		VkDescriptorSetAllocateInfo info;
		SE_ZERO_MEMORY(info);

		VkDescriptorSetLayout vk_layout = layout->handle();

		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorPool = pool->handle();
		info.descriptorSetCount = 1;
		info.pSetLayouts = &vk_layout;

		if ((pool->create_flags() & VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT) == VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			m_should_destroy = true;

		if (vkAllocateDescriptorSets(backend->device(), &info, &m_vk_ds) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to allocate descriptor set.");
			throw std::runtime_error("(Vulkan) Failed to allocate descriptor set.");
		}
	}



	DescriptorSet::~DescriptorSet()
	{
		if (m_vk_backend.expired() || m_vk_pool.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();
		auto pool = m_vk_pool.lock();

		if (m_should_destroy)
			vkFreeDescriptorSets(backend->device(), pool->handle(), 1, &m_vk_ds);
	}



	Fence::Ptr Fence::create(Backend::Ptr backend)
	{
		return std::shared_ptr<Fence>(new Fence(backend));
	}



	Fence::~Fence()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyFence(backend->device(), m_vk_fence, nullptr);
	}



	Fence::Fence(Backend::Ptr backend) :
		Object(backend)
	{
		VkFenceCreateInfo fence_info;
		SE_ZERO_MEMORY(fence_info);

		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateFence(backend->device(), &fence_info, nullptr, &m_vk_fence) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Fence.");
			throw std::runtime_error("(Vulkan) Failed to create Fence.");
		}
	}



	Semaphore::Ptr Semaphore::create(Backend::Ptr backend)
	{
		return std::shared_ptr<Semaphore>(new Semaphore(backend));
	}



	Semaphore::~Semaphore()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroySemaphore(backend->device(), m_vk_semaphore, nullptr);
	}



	Semaphore::Semaphore(Backend::Ptr backend) :
		Object(backend)
	{
		VkSemaphoreCreateInfo semaphore_info;
		SE_ZERO_MEMORY(semaphore_info);

		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(backend->device(), &semaphore_info, nullptr, &m_vk_semaphore) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Semaphore.");
			throw std::runtime_error("(Vulkan) Failed to create Semaphore.");
		}
	}



	QueryPool::Ptr QueryPool::create(Backend::Ptr backend, VkQueryType query_type, uint32_t query_count, VkQueryPipelineStatisticFlags pipeline_statistics)
	{
		return std::shared_ptr<QueryPool>(new QueryPool(backend, query_type, query_count, pipeline_statistics));
	}



	void QueryPool::results(uint32_t           first_query,
		uint32_t           query_count,
		size_t             data_size,
		void* ptr,
		VkDeviceSize       stride,
		VkQueryResultFlags flags)
	{
		auto backend = m_vk_backend.lock();

		vkGetQueryPoolResults(backend->device(), m_vk_query_pool, first_query, query_count, data_size, ptr, stride, flags);
	}



	QueryPool::~QueryPool()
	{
		if (m_vk_backend.expired())
		{
			SE_LOG_FATAL("(Vulkan) Destructing after Device.");
			throw std::runtime_error("(Vulkan) Destructing after Device.");
		}

		auto backend = m_vk_backend.lock();

		vkDestroyQueryPool(backend->device(), m_vk_query_pool, nullptr);
	}



	QueryPool::QueryPool(Backend::Ptr backend, VkQueryType query_type, uint32_t query_count, VkQueryPipelineStatisticFlags pipeline_statistics) :
		Object(backend)
	{
		VkQueryPoolCreateInfo query_pool_info;
		SE_ZERO_MEMORY(query_pool_info);

		query_pool_info.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		query_pool_info.queryType = query_type;
		query_pool_info.pipelineStatistics = pipeline_statistics;
		query_pool_info.queryCount = query_count;

		if (vkCreateQueryPool(backend->device(), &query_pool_info, nullptr, &m_vk_query_pool) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Query Pool.");
			throw std::runtime_error("(Vulkan) Failed to create Query Pool.");
		}
	}

	namespace utilities
	{
		void set_image_layout(VkCommandBuffer         cmdbuffer,
			VkImage                 image,
			VkImageLayout           oldImageLayout,
			VkImageLayout           newImageLayout,
			VkImageSubresourceRange subresourceRange,
			VkPipelineStageFlags    srcStageMask,
			VkPipelineStageFlags    dstStageMask)
		{
			// Create an image barrier object
			VkImageMemoryBarrier image_memory_barrier;
			SE_ZERO_MEMORY(image_memory_barrier);

			image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			image_memory_barrier.oldLayout = oldImageLayout;
			image_memory_barrier.newLayout = newImageLayout;
			image_memory_barrier.image = image;
			image_memory_barrier.subresourceRange = subresourceRange;

			// Source layouts (old)
			// Source access mask controls actions that have to be finished on the old layout
			// before it will be transitioned to the new layout
			switch (oldImageLayout)
			{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				// Image layout is undefined (or does not matter)
				// Only valid as initial layout
				// No flags required, listed only for completeness
				image_memory_barrier.srcAccessMask = 0;
				break;

			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				// Image is preinitialized
				// Only valid as initial layout for linear images, preserves memory contents
				// Make sure host writes have been finished
				image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				// Image is a color attachment
				// Make sure any writes to the color buffer have been finished
				image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				// Image is a depth/stencil attachment
				// Make sure any writes to the depth/stencil buffer have been finished
				image_memory_barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				// Image is a transfer source
				// Make sure any reads from the image have been finished
				image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				// Image is a transfer destination
				// Make sure any writes to the image have been finished
				image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				// Image is read by a shader
				// Make sure any shader reads from the image have been finished
				image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;
			default:
				// Other source layouts aren't handled (yet)
				break;
			}

			// Target layouts (new)
			// Destination access mask controls the dependency for the new image layout
			switch (newImageLayout)
			{
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				// Image will be used as a transfer destination
				// Make sure any writes to the image have been finished
				image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				// Image will be used as a transfer source
				// Make sure any reads from the image have been finished
				image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				// Image will be used as a color attachment
				// Make sure any writes to the color buffer have been finished
				image_memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				// Image layout will be used as a depth/stencil attachment
				// Make sure any writes to depth/stencil buffer have been finished
				image_memory_barrier.dstAccessMask = image_memory_barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				// Image will be read in a shader (sampler, input attachment)
				// Make sure any writes to the image have been finished
				if (image_memory_barrier.srcAccessMask == 0)
				{
					image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
				}
				image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;
			default:
				// Other source layouts aren't handled (yet)
				break;
			}

			// Put barrier inside setup command buffer
			vkCmdPipelineBarrier(
				cmdbuffer,
				srcStageMask,
				dstStageMask,
				0,
				0,
				nullptr,
				0,
				nullptr,
				1,
				&image_memory_barrier);
		}

		uint32_t get_memory_type(VkPhysicalDevice device, uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound)
		{
			VkPhysicalDeviceMemoryProperties prop;

			// Memory properties are used regularly for creating all kinds of buffers
			vkGetPhysicalDeviceMemoryProperties(device, &prop);

			for (uint32_t i = 0; i < prop.memoryTypeCount; i++)
			{
				if ((typeBits & 1) == 1)
				{
					if ((prop.memoryTypes[i].propertyFlags & properties) == properties)
					{
						if (memTypeFound)
						{
							*memTypeFound = true;
						}
						return i;
					}
				}
				typeBits >>= 1;
			}

			if (memTypeFound)
			{
				*memTypeFound = false;
				return 0;
			}
			else
			{
				throw std::runtime_error("Could not find a matching memory type");
			}
		}
	} // namespace utilities
} // namespace vk
#endif // SE_VULKAN
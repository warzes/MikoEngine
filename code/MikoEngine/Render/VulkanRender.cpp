#include "stdafx.h"
#if SE_VULKAN
#include "VulkanRender.h"
#include "Core/Logger.h"
#include "Utility/Utility.h"
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma region FuncLoad

struct RayTracingFunctionTable
{
	PFN_vkCreateAccelerationStructureNV                _vkCreateAccelerationStructureNV;
	PFN_vkDestroyAccelerationStructureNV               _vkDestroyAccelerationStructureNV;
	PFN_vkGetAccelerationStructureMemoryRequirementsNV _vkGetAccelerationStructureMemoryRequirementsNV;
	PFN_vkBindAccelerationStructureMemoryNV            _vkBindAccelerationStructureMemoryNV;
	PFN_vkCmdBuildAccelerationStructureNV              _vkCmdBuildAccelerationStructureNV;
	PFN_vkCmdCopyAccelerationStructureNV               _vkCmdCopyAccelerationStructureNV;
	PFN_vkCmdTraceRaysNV                               _vkCmdTraceRaysNV;
	PFN_vkCreateRayTracingPipelinesNV                  _vkCreateRayTracingPipelinesNV;
	PFN_vkGetRayTracingShaderGroupHandlesNV            _vkGetRayTracingShaderGroupHandlesNV;
	PFN_vkGetAccelerationStructureHandleNV             _vkGetAccelerationStructureHandleNV;
	PFN_vkCmdWriteAccelerationStructuresPropertiesNV   _vkCmdWriteAccelerationStructuresPropertiesNV;
	PFN_vkCompileDeferredNV                            _vkCompileDeferredNV;
};

static RayTracingFunctionTable g_ray_tracing_func_table;

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateAccelerationStructureNV(VkDevice                                   device,
	const VkAccelerationStructureCreateInfoNV* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkAccelerationStructureNV* pAccelerationStructure)
{
	return g_ray_tracing_func_table._vkCreateAccelerationStructureNV(device, pCreateInfo, pAllocator, pAccelerationStructure);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyAccelerationStructureNV(VkDevice                     device,
	VkAccelerationStructureNV    accelerationStructure,
	const VkAllocationCallbacks* pAllocator)
{
	return g_ray_tracing_func_table._vkDestroyAccelerationStructureNV(device, accelerationStructure, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV(
	VkDevice                                               device,
	const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo,
	VkMemoryRequirements2KHR* pMemoryRequirements)
{
	return g_ray_tracing_func_table._vkGetAccelerationStructureMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkBindAccelerationStructureMemoryNV(VkDevice                                       device,
	uint32_t                                       bindInfoCount,
	const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
	return g_ray_tracing_func_table._vkBindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
}

VKAPI_ATTR void VKAPI_CALL
vkCmdBuildAccelerationStructureNV(VkCommandBuffer                      commandBuffer,
	const VkAccelerationStructureInfoNV* pInfo,
	VkBuffer                             instanceData,
	VkDeviceSize                         instanceOffset,
	VkBool32                             update,
	VkAccelerationStructureNV            dst,
	VkAccelerationStructureNV            src,
	VkBuffer                             scratch,
	VkDeviceSize                         scratchOffset)
{
	return g_ray_tracing_func_table._vkCmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}



VKAPI_ATTR void VKAPI_CALL vkCmdCopyAccelerationStructureNV(VkCommandBuffer                   cmdBuf,
	VkAccelerationStructureNV         dst,
	VkAccelerationStructureNV         src,
	VkCopyAccelerationStructureModeNV mode)
{
	return g_ray_tracing_func_table._vkCmdCopyAccelerationStructureNV(cmdBuf, dst, src, mode);
}



VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysNV(VkCommandBuffer commandBuffer,
	VkBuffer        raygenShaderBindingTableBuffer,
	VkDeviceSize    raygenShaderBindingOffset,
	VkBuffer        missShaderBindingTableBuffer,
	VkDeviceSize    missShaderBindingOffset,
	VkDeviceSize    missShaderBindingStride,
	VkBuffer        hitShaderBindingTableBuffer,
	VkDeviceSize    hitShaderBindingOffset,
	VkDeviceSize    hitShaderBindingStride,
	VkBuffer        callableShaderBindingTableBuffer,
	VkDeviceSize    callableShaderBindingOffset,
	VkDeviceSize    callableShaderBindingStride,
	uint32_t        width,
	uint32_t        height,
	uint32_t        depth)
{
	return g_ray_tracing_func_table._vkCmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}



VKAPI_ATTR VkResult VKAPI_CALL
vkCreateRayTracingPipelinesNV(VkDevice                                device,
	VkPipelineCache                         pipelineCache,
	uint32_t                                createInfoCount,
	const VkRayTracingPipelineCreateInfoNV* pCreateInfos,
	const VkAllocationCallbacks* pAllocator,
	VkPipeline* pPipelines)
{
	return g_ray_tracing_func_table._vkCreateRayTracingPipelinesNV(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}



VKAPI_ATTR VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV(VkDevice   device,
	VkPipeline pipeline,
	uint32_t   firstGroup,
	uint32_t   groupCount,
	size_t     dataSize,
	void* pData)
{
	return g_ray_tracing_func_table._vkGetRayTracingShaderGroupHandlesNV(device, pipeline, firstGroup, groupCount, dataSize, pData);
}



VKAPI_ATTR VkResult VKAPI_CALL
vkGetAccelerationStructureHandleNV(VkDevice                  device,
	VkAccelerationStructureNV accelerationStructure,
	size_t                    dataSize,
	void* pData)
{
	return g_ray_tracing_func_table._vkGetAccelerationStructureHandleNV(device, accelerationStructure, dataSize, pData);
}



VKAPI_ATTR void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV(
	VkCommandBuffer                  commandBuffer,
	uint32_t                         accelerationStructureCount,
	const VkAccelerationStructureNV* pAccelerationStructures,
	VkQueryType                      queryType,
	VkQueryPool                      queryPool,
	uint32_t                         firstQuery)
{
	return g_ray_tracing_func_table._vkCmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}



VKAPI_ATTR VkResult VKAPI_CALL vkCompileDeferredNV(VkDevice   device,
	VkPipeline pipeline,
	uint32_t   shader)
{
	return g_ray_tracing_func_table._vkCompileDeferredNV(device, pipeline, shader);
}

#pragma endregion

namespace vk
{
	const std::vector<const char*> kValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

#    define MAX_DESCRIPTOR_POOL_THREADS 32
#    define MAX_COMMAND_THREADS 32
#    define MAX_THREAD_LOCAL_COMMAND_BUFFERS 8

	struct ThreadLocalCommandBuffers
	{
		CommandPool::Ptr   command_pool[Backend::kMaxFramesInFlight];
		CommandBuffer::Ptr command_buffers[Backend::kMaxFramesInFlight][MAX_THREAD_LOCAL_COMMAND_BUFFERS];
		uint32_t           allocated_buffers = 0;

		ThreadLocalCommandBuffers(Backend::Ptr backend, uint32_t queue_family)
		{
			for (int i = 0; i < Backend::kMaxFramesInFlight; i++)
			{
				command_pool[i] = CommandPool::create(backend, queue_family);

				for (int j = 0; j < MAX_THREAD_LOCAL_COMMAND_BUFFERS; j++)
					command_buffers[i][j] = CommandBuffer::create(backend, command_pool[i]);
			}
		}

		~ThreadLocalCommandBuffers()
		{
		}

		void reset(uint32_t frame_index)
		{
			allocated_buffers = 0;
			command_pool[frame_index]->reset();
		}

		CommandBuffer::Ptr allocate(uint32_t frame_index, bool begin)
		{
			if (allocated_buffers >= MAX_THREAD_LOCAL_COMMAND_BUFFERS)
			{
				SE_LOG_FATAL("(Vulkan) Max thread local command buffer count reached!");
				throw std::runtime_error("(Vulkan) Max thread local command buffer count reached!");
			}

			auto cmd_buf = command_buffers[frame_index][allocated_buffers++];

			if (begin)
			{
				VkCommandBufferBeginInfo begin_info;
				SE_ZERO_MEMORY(begin_info);

				begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

				vkBeginCommandBuffer(cmd_buf->handle(), &begin_info);
			}

			return cmd_buf;
		}
	};

	std::atomic<uint32_t>                                   g_thread_counter = 0;
	thread_local uint32_t                                   g_thread_idx = g_thread_counter++;
	thread_local std::shared_ptr<ThreadLocalCommandBuffers> g_graphics_command_buffers[MAX_COMMAND_THREADS];
	thread_local std::shared_ptr<ThreadLocalCommandBuffers> g_compute_command_buffers[MAX_COMMAND_THREADS];
	thread_local std::shared_ptr<ThreadLocalCommandBuffers> g_transfer_command_buffers[MAX_COMMAND_THREADS];
	thread_local DescriptorPool::Ptr                        g_descriptor_pools[MAX_DESCRIPTOR_POOL_THREADS];

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

		auto& rt_props = backend->ray_tracing_properties();

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

		const auto& rt_props = backend->ray_tracing_properties();

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



	Backend::Ptr Backend::create(GLFWwindow* window, bool enable_validation_layers, bool require_ray_tracing, std::vector<const char*> additional_device_extensions, void* pnext)
	{
		std::shared_ptr<Backend> backend = std::shared_ptr<Backend>(new Backend(window, enable_validation_layers, require_ray_tracing, additional_device_extensions, pnext));
		backend->initialize();

		return backend;
	}



	Backend::Backend(GLFWwindow* window, bool enable_validation_layers, bool require_ray_tracing, std::vector<const char*> additional_device_extensions, void* pnext) :
		m_window(window)
	{
		m_ray_tracing_enabled = require_ray_tracing;

		m_instance = new vkWrapper::Instance();
		m_instance->Init();
		m_debugMessenger = m_instance->CreateDebugMessenger();
		m_surface = m_instance->CreateSurface(window);

		std::vector<const char*> device_extensions;
		if (m_ray_tracing_enabled)
		{
			device_extensions.push_back(VK_NV_RAY_TRACING_EXTENSION_NAME);
			device_extensions.push_back(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
			device_extensions.push_back(VK_KHR_MAINTENANCE3_EXTENSION_NAME);
			device_extensions.push_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);

			SE_ZERO_MEMORY(m_indexing_features);

			m_indexing_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
			m_indexing_features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
			m_indexing_features.runtimeDescriptorArray = VK_TRUE;
			m_indexing_features.descriptorBindingVariableDescriptorCount = VK_TRUE;
			m_indexing_features.shaderStorageBufferArrayNonUniformIndexing = VK_TRUE;
		}
		for (auto ext : additional_device_extensions)
			device_extensions.push_back(ext);

		m_physicalDevice = m_instance->PickPhysicalDevice(m_surface, device_extensions);

		// TODO: ��������� ������� - ���� ���������� �� device_extensions (������� ����� � ��� ����������)
		device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		if (!create_logical_device(device_extensions, pnext))
		{
			SE_LOG_FATAL("(Vulkan) Failed to create logical device.");
			throw std::runtime_error("(Vulkan) Failed to create logical device.");
		}

		VmaAllocatorCreateInfo allocator_info = {};
		allocator_info.physicalDevice = m_physicalDevice.Get();
		allocator_info.device = m_vk_device;

		if (vmaCreateAllocator(&allocator_info, &m_vma_allocator) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Allocator.");
			throw std::runtime_error("(Vulkan) Failed to create Allocator.");
		}

		if (require_ray_tracing)
			load_ray_tracing_funcs();
	}



	Backend::~Backend()
	{
		for (int i = 0; i < MAX_COMMAND_THREADS; i++)
		{
			g_graphics_command_buffers[i].reset();
			g_compute_command_buffers[i].reset();
			g_transfer_command_buffers[i].reset();
		}

		for (int i = 0; i < MAX_DESCRIPTOR_POOL_THREADS; i++)
			g_descriptor_pools[i].reset();

		for (int i = 0; i < m_swap_chain_images.size(); i++)
		{
			m_swap_chain_framebuffers[i].reset();
			m_swap_chain_image_views[i].reset();
		}

		for (int i = 0; i < m_in_flight_fences.size(); i++)
			m_in_flight_fences[i].reset();

		m_swap_chain_render_pass.reset();
		m_swap_chain_depth_view.reset();
		m_swap_chain_depth.reset();

		delete m_debugMessenger; m_debugMessenger = nullptr;
		delete m_surface; m_surface = nullptr;

		if (m_vk_swap_chain)
		{
			vkDestroySwapchainKHR(m_vk_device, m_vk_swap_chain, nullptr);
			m_vk_swap_chain = nullptr;
		}

		if (m_vma_allocator)
		{
			vmaDestroyAllocator(m_vma_allocator);
			m_vma_allocator = nullptr;
		}

		if (m_vk_device)
		{
			vkDestroyDevice(m_vk_device, nullptr);
			m_vk_device = nullptr;
		}

		delete m_instance; m_instance = nullptr;
	}



	void Backend::initialize()
	{
		create_swapchain();

		// Create Descriptor Pools
		DescriptorPool::Desc dp_desc;

		dp_desc.set_max_sets(512)
			.add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 32)
			.add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 4)
			.add_pool_size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 256)
			.add_pool_size(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 32)
			.add_pool_size(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 32)
			.add_pool_size(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 16)
			.add_pool_size(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV, 16);

		for (int i = 0; i < MAX_DESCRIPTOR_POOL_THREADS; i++)
			g_descriptor_pools[i] = DescriptorPool::create(shared_from_this(), dp_desc);

		for (int i = 0; i < MAX_COMMAND_THREADS; i++)
		{
			g_graphics_command_buffers[i] = std::make_shared<ThreadLocalCommandBuffers>(shared_from_this(), m_instance->queue_infos().graphics_queue_index);
			g_compute_command_buffers[i] = std::make_shared<ThreadLocalCommandBuffers>(shared_from_this(), m_instance->queue_infos().compute_queue_index);
			g_transfer_command_buffers[i] = std::make_shared<ThreadLocalCommandBuffers>(shared_from_this(), m_instance->queue_infos().transfer_queue_index);
		}
	}



	void Backend::load_ray_tracing_funcs()
	{
		g_ray_tracing_func_table._vkCreateAccelerationStructureNV = reinterpret_cast<PFN_vkCreateAccelerationStructureNV>(vkGetDeviceProcAddr(m_vk_device, "vkCreateAccelerationStructureNV"));
		g_ray_tracing_func_table._vkDestroyAccelerationStructureNV = reinterpret_cast<PFN_vkDestroyAccelerationStructureNV>(vkGetDeviceProcAddr(m_vk_device, "vkDestroyAccelerationStructureNV"));
		g_ray_tracing_func_table._vkGetAccelerationStructureMemoryRequirementsNV = reinterpret_cast<PFN_vkGetAccelerationStructureMemoryRequirementsNV>(vkGetDeviceProcAddr(m_vk_device, "vkGetAccelerationStructureMemoryRequirementsNV"));
		g_ray_tracing_func_table._vkBindAccelerationStructureMemoryNV = reinterpret_cast<PFN_vkBindAccelerationStructureMemoryNV>(vkGetDeviceProcAddr(m_vk_device, "vkBindAccelerationStructureMemoryNV"));
		g_ray_tracing_func_table._vkCmdBuildAccelerationStructureNV = reinterpret_cast<PFN_vkCmdBuildAccelerationStructureNV>(vkGetDeviceProcAddr(m_vk_device, "vkCmdBuildAccelerationStructureNV"));
		g_ray_tracing_func_table._vkCmdCopyAccelerationStructureNV = reinterpret_cast<PFN_vkCmdCopyAccelerationStructureNV>(vkGetDeviceProcAddr(m_vk_device, "vkCmdCopyAccelerationStructureNV"));
		g_ray_tracing_func_table._vkCmdTraceRaysNV = reinterpret_cast<PFN_vkCmdTraceRaysNV>(vkGetDeviceProcAddr(m_vk_device, "vkCmdTraceRaysNV"));
		g_ray_tracing_func_table._vkCreateRayTracingPipelinesNV = reinterpret_cast<PFN_vkCreateRayTracingPipelinesNV>(vkGetDeviceProcAddr(m_vk_device, "vkCreateRayTracingPipelinesNV"));
		g_ray_tracing_func_table._vkGetRayTracingShaderGroupHandlesNV = reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesNV>(vkGetDeviceProcAddr(m_vk_device, "vkGetRayTracingShaderGroupHandlesNV"));
		g_ray_tracing_func_table._vkGetAccelerationStructureHandleNV = reinterpret_cast<PFN_vkGetAccelerationStructureHandleNV>(vkGetDeviceProcAddr(m_vk_device, "vkGetAccelerationStructureHandleNV"));
		g_ray_tracing_func_table._vkCmdWriteAccelerationStructuresPropertiesNV = reinterpret_cast<PFN_vkCmdWriteAccelerationStructuresPropertiesNV>(vkGetDeviceProcAddr(m_vk_device, "vkCmdWriteAccelerationStructuresPropertiesNV"));
		g_ray_tracing_func_table._vkCompileDeferredNV = reinterpret_cast<PFN_vkCompileDeferredNV>(vkGetDeviceProcAddr(m_vk_device, "vkCompileDeferredNV"));
	}



	std::shared_ptr<DescriptorSet> Backend::allocate_descriptor_set(std::shared_ptr<DescriptorSetLayout> layout)
	{
		return DescriptorSet::create(shared_from_this(), layout, g_descriptor_pools[g_thread_idx]);
	}



	std::shared_ptr<CommandBuffer> Backend::allocate_graphics_command_buffer(bool begin)
	{
		return g_graphics_command_buffers[g_thread_idx]->allocate(m_current_frame, begin);
	}



	std::shared_ptr<CommandBuffer> Backend::allocate_compute_command_buffer(bool begin)
	{
		return g_compute_command_buffers[g_thread_idx]->allocate(m_current_frame, begin);
	}



	std::shared_ptr<CommandBuffer> Backend::allocate_transfer_command_buffer(bool begin)
	{
		return g_transfer_command_buffers[g_thread_idx]->allocate(m_current_frame, begin);
	}



	std::shared_ptr<CommandPool> Backend::thread_local_graphics_command_pool()
	{
		return g_graphics_command_buffers[g_thread_idx]->command_pool[m_current_frame];
	}



	std::shared_ptr<CommandPool> Backend::thread_local_compute_command_pool()
	{
		return g_compute_command_buffers[g_thread_idx]->command_pool[m_current_frame];
	}



	std::shared_ptr<CommandPool> Backend::thread_local_transfer_command_pool()
	{
		return g_transfer_command_buffers[g_thread_idx]->command_pool[m_current_frame];
	}



	std::shared_ptr<DescriptorPool> Backend::thread_local_descriptor_pool()
	{
		return g_descriptor_pools[g_thread_idx];
	}



	void Backend::submit_graphics(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs,
		const std::vector<std::shared_ptr<Semaphore>>& wait_semaphores,
		const std::vector<VkPipelineStageFlags>& wait_stages,
		const std::vector<std::shared_ptr<Semaphore>>& signal_semaphores)
	{
		submit(m_vk_graphics_queue, cmd_bufs, wait_semaphores, wait_stages, signal_semaphores);
	}



	void Backend::submit_compute(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs,
		const std::vector<std::shared_ptr<Semaphore>>& wait_semaphores,
		const std::vector<VkPipelineStageFlags>& wait_stages,
		const std::vector<std::shared_ptr<Semaphore>>& signal_semaphores)
	{
		submit(m_vk_compute_queue, cmd_bufs, wait_semaphores, wait_stages, signal_semaphores);
	}



	void Backend::submit_transfer(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs,
		const std::vector<std::shared_ptr<Semaphore>>& wait_semaphores,
		const std::vector<VkPipelineStageFlags>& wait_stages,
		const std::vector<std::shared_ptr<Semaphore>>& signal_semaphores)
	{
		submit(m_vk_transfer_queue, cmd_bufs, wait_semaphores, wait_stages, signal_semaphores);
	}



	void Backend::flush_graphics(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs)
	{
		flush(m_vk_graphics_queue, cmd_bufs);

		for (int i = 0; i < MAX_COMMAND_THREADS; i++)
			g_graphics_command_buffers[i]->reset(m_current_frame);
	}



	void Backend::flush_compute(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs)
	{
		flush(m_vk_compute_queue, cmd_bufs);

		for (int i = 0; i < MAX_COMMAND_THREADS; i++)
			g_compute_command_buffers[i]->reset(m_current_frame);
	}



	void Backend::flush_transfer(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs)
	{
		flush(m_vk_transfer_queue, cmd_bufs);

		for (int i = 0; i < MAX_COMMAND_THREADS; i++)
			g_transfer_command_buffers[i]->reset(m_current_frame);
	}



	void Backend::submit(VkQueue                                            queue,
		const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs,
		const std::vector<std::shared_ptr<Semaphore>>& wait_semaphores,
		const std::vector<VkPipelineStageFlags>& wait_stages,
		const std::vector<std::shared_ptr<Semaphore>>& signal_semaphores)
	{
		VkSemaphore vk_wait_semaphores[16];

		for (int i = 0; i < wait_semaphores.size(); i++)
			vk_wait_semaphores[i] = wait_semaphores[i]->handle();

		VkSemaphore vk_signal_semaphores[16];

		for (int i = 0; i < signal_semaphores.size(); i++)
			vk_signal_semaphores[i] = signal_semaphores[i]->handle();

		VkCommandBuffer vk_cmd_bufs[32];

		for (int i = 0; i < cmd_bufs.size(); i++)
			vk_cmd_bufs[i] = cmd_bufs[i]->handle();

		VkPipelineStageFlags vk_wait_stages[16];

		for (int i = 0; i < wait_semaphores.size(); i++)
			vk_wait_stages[i] = wait_stages[i];

		VkSubmitInfo submit_info;
		SE_ZERO_MEMORY(submit_info);

		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		submit_info.waitSemaphoreCount = wait_semaphores.size();
		submit_info.pWaitSemaphores = vk_wait_semaphores;
		submit_info.pWaitDstStageMask = vk_wait_stages;

		submit_info.commandBufferCount = cmd_bufs.size();
		submit_info.pCommandBuffers = &vk_cmd_bufs[0];

		submit_info.signalSemaphoreCount = signal_semaphores.size();
		submit_info.pSignalSemaphores = vk_signal_semaphores;

		vkResetFences(m_vk_device, 1, &m_in_flight_fences[m_current_frame]->handle());

		if (vkQueueSubmit(queue, 1, &submit_info, m_in_flight_fences[m_current_frame]->handle()) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to submit command buffer!");
			throw std::runtime_error("(Vulkan) Failed to submit command buffer!");
		}
	}



	void Backend::flush(VkQueue queue, const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs)
	{
		VkCommandBuffer vk_cmd_bufs[32];

		for (int i = 0; i < cmd_bufs.size(); i++)
			vk_cmd_bufs[i] = cmd_bufs[i]->handle();

		VkSubmitInfo submit_info;
		SE_ZERO_MEMORY(submit_info);

		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &vk_cmd_bufs[0];

		// Create fence to ensure that the command buffer has finished executing
		VkFenceCreateInfo fence_info;
		SE_ZERO_MEMORY(fence_info);

		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		VkFence fence;
		vkCreateFence(m_vk_device, &fence_info, nullptr, &fence);

		// Submit to the queue
		vkQueueSubmit(queue, 1, &submit_info, fence);

		// Wait for the fence to signal that command buffer has finished executing
		vkWaitForFences(m_vk_device, 1, &fence, VK_TRUE, 100000000000);

		vkDestroyFence(m_vk_device, fence, nullptr);
	}



	void Backend::acquire_next_swap_chain_image(const std::shared_ptr<Semaphore>& semaphore)
	{
		vkWaitForFences(m_vk_device, 1, &m_in_flight_fences[m_current_frame]->handle(), VK_TRUE, UINT64_MAX);

		for (int i = 0; i < MAX_COMMAND_THREADS; i++)
		{
			g_graphics_command_buffers[i]->reset(m_current_frame);
			g_compute_command_buffers[i]->reset(m_current_frame);
			g_transfer_command_buffers[i]->reset(m_current_frame);
		}

		VkResult result = vkAcquireNextImageKHR(m_vk_device, m_vk_swap_chain, UINT64_MAX, semaphore->handle(), VK_NULL_HANDLE, &m_image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreate_swapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			SE_LOG_FATAL("(Vulkan) Failed to acquire swap chain image!");
			throw std::runtime_error("(Vulkan) Failed to acquire swap chain image!");
		}
	}



	void Backend::present(const std::vector<std::shared_ptr<Semaphore>>& semaphores)
	{
		VkSemaphore signal_semaphores[16];

		for (int i = 0; i < semaphores.size(); i++)
			signal_semaphores[i] = semaphores[i]->handle();

		VkPresentInfoKHR present_info;
		SE_ZERO_MEMORY(present_info);

		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = semaphores.size();
		present_info.pWaitSemaphores = signal_semaphores;

		VkSwapchainKHR swap_chains[] = { m_vk_swap_chain };
		present_info.swapchainCount = 1;
		present_info.pSwapchains = swap_chains;
		present_info.pImageIndices = &m_image_index;

		if (vkQueuePresentKHR(m_vk_presentation_queue, &present_info) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to submit draw command buffer!");
			throw std::runtime_error("failed to present swap chain image!");
		}

		m_current_frame = (m_current_frame + 1) % kMaxFramesInFlight;
	}



	Image::Ptr Backend::swapchain_image()
	{
		return m_swap_chain_images[m_current_frame];
	}



	ImageView::Ptr Backend::swapchain_image_view()
	{
		return m_swap_chain_image_views[m_current_frame];
	}



	Framebuffer::Ptr Backend::swapchain_framebuffer()
	{
		return m_swap_chain_framebuffers[m_current_frame];
	}



	RenderPass::Ptr Backend::swapchain_render_pass()
	{
		return m_swap_chain_render_pass;
	}



	void Backend::wait_idle()
	{
		vkDeviceWaitIdle(m_vk_device);
	}



	uint32_t Backend::swap_image_count()
	{
		return m_swap_chain_images.size();
	}



	VkInstance& Backend::instance()
	{
		return m_instance->Get();
	}



	VkQueue Backend::graphics_queue()
	{
		return m_vk_graphics_queue;
	}



	VkQueue Backend::transfer_queue()
	{
		return m_vk_transfer_queue;
	}



	VkQueue Backend::compute_queue()
	{
		return m_vk_compute_queue;
	}



	VkDevice Backend::device()
	{
		return m_vk_device;
	}



	VkPhysicalDevice Backend::physical_device()
	{
		return m_physicalDevice.Get();
	}



	VmaAllocator_T* Backend::allocator()
	{
		return m_vma_allocator;
	}



	size_t Backend::min_dynamic_ubo_alignment()
	{
		return m_physicalDevice.GetProperties().limits.minUniformBufferOffsetAlignment;
	}



	size_t Backend::aligned_dynamic_ubo_size(size_t size)
	{
		size_t min_ubo_alignment = m_physicalDevice.GetProperties().limits.minUniformBufferOffsetAlignment;
		size_t aligned_size = size;

		if (min_ubo_alignment > 0)
			aligned_size = (aligned_size + min_ubo_alignment - 1) & ~(min_ubo_alignment - 1);

		return aligned_size;
	}



	VkFormat Backend::find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_physicalDevice.Get(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		throw std::runtime_error("Failed to find supported format!");
	}



	VkFormat Backend::find_depth_format()
	{
		return find_supported_format({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}



	void Backend::query_swap_chain_support(VkPhysicalDevice device, vkWrapper::SwapChainSupportDetails& details)
	{
		// Get surface capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface->Get(), &details.capabilities);

		uint32_t present_mode_count;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface->Get(), &present_mode_count, nullptr);

		if (present_mode_count != 0)
		{
			details.presentModes.resize(present_mode_count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface->Get(), &present_mode_count, &details.presentModes[0]);
		}

		uint32_t format_count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface->Get(), &format_count, nullptr);

		if (format_count != 0)
		{
			details.formats.resize(format_count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface->Get(), &format_count, &details.formats[0]);
		}
	}



	bool Backend::check_validation_layer_support(std::vector<const char*> layers)
	{
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		VkLayerProperties available_layers[32];
		vkEnumerateInstanceLayerProperties(&layer_count, &available_layers[0]);

		for (const char* layer_name : layers)
		{
			bool layer_found = false;

			for (const auto& layer_properties : available_layers)
			{
				if (std::string(layer_name) == std::string(layer_properties.layerName))
				{
					layer_found = true;
					break;
				}
			}

			if (!layer_found)
			{
				SE_LOG_FATAL("(Vulkan) Validation Layer not available: " + std::string(layer_name));
				return false;
			}
		}

		return true;
	}



	bool Backend::create_logical_device(std::vector<const char*> extensions, void* pnext)
	{
		VkPhysicalDeviceFeatures supported_features;
		SE_ZERO_MEMORY(supported_features);

		VkDeviceCreateInfo device_info;
		SE_ZERO_MEMORY(device_info);

		device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.pQueueCreateInfos = &m_instance->queue_infos().infos[0];
		device_info.queueCreateInfoCount = static_cast<uint32_t>(m_instance->queue_infos().queue_count);
		device_info.enabledExtensionCount = extensions.size();
		device_info.ppEnabledExtensionNames = extensions.data();
		device_info.pEnabledFeatures = &supported_features;
		device_info.pNext = nullptr;

		VkPhysicalDeviceFeatures2 physical_device_features_2;
		SE_ZERO_MEMORY(physical_device_features_2);

		if (pnext)
		{
			physical_device_features_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			physical_device_features_2.features = supported_features;
			physical_device_features_2.pNext = pnext;
			device_info.pEnabledFeatures = nullptr;
			device_info.pNext = &physical_device_features_2;
		}
		else if (m_ray_tracing_enabled)
		{
			pnext = &m_indexing_features;

			physical_device_features_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			physical_device_features_2.features = supported_features;
			physical_device_features_2.pNext = pnext;
			device_info.pEnabledFeatures = nullptr;
			device_info.pNext = &physical_device_features_2;
		}

		if (m_debugMessenger)
		{
			device_info.enabledLayerCount = kValidationLayers.size();
			device_info.ppEnabledLayerNames = &kValidationLayers[0];
		}
		else
			device_info.enabledLayerCount = 0;

		float priority = 1.0f;

		for (int i = 0; i < m_instance->queue_infos().queue_count; i++)
			m_instance->queue_infos().infos[i].pQueuePriorities = &priority;

		if (vkCreateDevice(m_physicalDevice.Get(), &device_info, nullptr, &m_vk_device) != VK_SUCCESS)
			return false;

		// Get presentation queue
		vkGetDeviceQueue(m_vk_device, m_instance->queue_infos().presentation_queue_index, 0, &m_vk_presentation_queue);

		// Get graphics queue
		if (m_instance->queue_infos().graphics_queue_index == m_instance->queue_infos().presentation_queue_index)
			m_vk_graphics_queue = m_vk_presentation_queue;
		else
			vkGetDeviceQueue(m_vk_device, m_instance->queue_infos().graphics_queue_index, 0, &m_vk_graphics_queue);

		// Get compute queue
		if (m_instance->queue_infos().compute_queue_index == m_instance->queue_infos().presentation_queue_index)
			m_vk_compute_queue = m_vk_presentation_queue;
		else if (m_instance->queue_infos().compute_queue_index == m_instance->queue_infos().graphics_queue_index)
			m_vk_compute_queue = m_vk_graphics_queue;
		else
			vkGetDeviceQueue(m_vk_device, m_instance->queue_infos().compute_queue_index, 0, &m_vk_compute_queue);

		// Get transfer queue
		if (m_instance->queue_infos().transfer_queue_index == m_instance->queue_infos().presentation_queue_index)
			m_vk_transfer_queue = m_vk_presentation_queue;
		else if (m_instance->queue_infos().transfer_queue_index == m_instance->queue_infos().graphics_queue_index)
			m_vk_transfer_queue = m_vk_graphics_queue;
		else if (m_instance->queue_infos().transfer_queue_index == m_instance->queue_infos().compute_queue_index)
			m_vk_transfer_queue = m_vk_transfer_queue;
		else
			vkGetDeviceQueue(m_vk_device, m_instance->queue_infos().transfer_queue_index, 0, &m_vk_transfer_queue);

		return true;
	}



	bool Backend::create_swapchain()
	{
		m_current_frame = 0;
		VkSurfaceFormatKHR surface_format = choose_swap_surface_format(m_instance->GetSwapChainDetails().formats);
		VkPresentModeKHR   present_mode = choose_swap_present_mode(m_instance->GetSwapChainDetails().presentModes);
		VkExtent2D         extent = choose_swap_extent(m_instance->GetSwapChainDetails().capabilities);

		uint32_t image_count = m_instance->GetSwapChainDetails().capabilities.minImageCount + 1;

		if (m_instance->GetSwapChainDetails().capabilities.maxImageCount > 0 && image_count > m_instance->GetSwapChainDetails().capabilities.maxImageCount)
			image_count = m_instance->GetSwapChainDetails().capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR create_info;
		SE_ZERO_MEMORY(create_info);

		create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.surface = m_surface->Get();
		create_info.minImageCount = image_count;
		create_info.imageFormat = surface_format.format;
		create_info.imageColorSpace = surface_format.colorSpace;
		create_info.imageExtent = extent;
		create_info.imageArrayLayers = 1;
		create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		m_swap_chain_image_format = surface_format.format;
		m_swap_chain_extent = extent;

		uint32_t queue_family_indices[] = { (uint32_t)m_instance->queue_infos().graphics_queue_index, (uint32_t)m_instance->queue_infos().presentation_queue_index };

		if (m_instance->queue_infos().presentation_queue_index != m_instance->queue_infos().graphics_queue_index)
		{
			create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			create_info.queueFamilyIndexCount = 2;
			create_info.pQueueFamilyIndices = queue_family_indices;
		}
		else
		{
			create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			create_info.queueFamilyIndexCount = 0;
			create_info.pQueueFamilyIndices = nullptr;
		}

		create_info.preTransform = m_instance->GetSwapChainDetails().capabilities.currentTransform;
		create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		create_info.presentMode = present_mode;
		create_info.clipped = VK_TRUE;
		create_info.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_vk_device, &create_info, nullptr, &m_vk_swap_chain) != VK_SUCCESS)
			return false;

		uint32_t swap_image_count = 0;
		vkGetSwapchainImagesKHR(m_vk_device, m_vk_swap_chain, &swap_image_count, nullptr);
		m_swap_chain_images.resize(swap_image_count);
		m_swap_chain_image_views.resize(swap_image_count);
		m_swap_chain_framebuffers.resize(swap_image_count);

		VkImage images[32];

		if (vkGetSwapchainImagesKHR(m_vk_device, m_vk_swap_chain, &swap_image_count, &images[0]) != VK_SUCCESS)
			return false;

		m_swap_chain_depth_format = find_depth_format();

		m_swap_chain_depth = Image::create(shared_from_this(),
			VK_IMAGE_TYPE_2D,
			m_swap_chain_extent.width,
			m_swap_chain_extent.height,
			1,
			1,
			1,
			m_swap_chain_depth_format,
			VMA_MEMORY_USAGE_GPU_ONLY,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_SAMPLE_COUNT_1_BIT);

		m_swap_chain_depth_view = ImageView::create(shared_from_this(), m_swap_chain_depth, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT);

		create_render_pass();

		std::vector<ImageView::Ptr> views(2);

		views[1] = m_swap_chain_depth_view;

		for (int i = 0; i < swap_image_count; i++)
		{
			m_swap_chain_images[i] = Image::create_from_swapchain(shared_from_this(), images[i], VK_IMAGE_TYPE_2D, m_swap_chain_extent.width, m_swap_chain_extent.height, 1, 1, 1, m_swap_chain_image_format, VMA_MEMORY_USAGE_UNKNOWN, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SAMPLE_COUNT_1_BIT);
			m_swap_chain_image_views[i] = ImageView::create(shared_from_this(), m_swap_chain_images[i], VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);

			views[0] = m_swap_chain_image_views[i];

			m_swap_chain_framebuffers[i] = Framebuffer::create(shared_from_this(), m_swap_chain_render_pass, views, m_swap_chain_extent.width, m_swap_chain_extent.height, 1);
		}

		m_in_flight_fences.resize(kMaxFramesInFlight);

		for (size_t i = 0; i < kMaxFramesInFlight; i++)
			m_in_flight_fences[i] = Fence::create(shared_from_this());

		return true;
	}



	void Backend::recreate_swapchain()
	{
		vkDeviceWaitIdle(m_vk_device);

		// Destroy existing swap chain resources
		for (int i = 0; i < m_swap_chain_images.size(); i++)
		{
			m_swap_chain_images[i].reset();
			m_swap_chain_framebuffers[i].reset();
			m_swap_chain_image_views[i].reset();
		}

		vkDestroySwapchainKHR(m_vk_device, m_vk_swap_chain, nullptr);

		if (!create_swapchain())
		{
			SE_LOG_FATAL("(Vulkan) Failed to create swap chain!");
			throw std::runtime_error("(Vulkan) Failed to create swap chain!");
		}
	}



	void Backend::create_render_pass()
	{
		std::vector<VkAttachmentDescription> attachments(2);

		// Color attachment
		attachments[0].format = m_swap_chain_image_format;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Depth attachment
		attachments[1].format = m_swap_chain_depth_format;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference color_reference;
		color_reference.attachment = 0;
		color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depth_reference;
		depth_reference.attachment = 1;
		depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		std::vector<VkSubpassDescription> subpass_description(1);

		subpass_description[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass_description[0].colorAttachmentCount = 1;
		subpass_description[0].pColorAttachments = &color_reference;
		subpass_description[0].pDepthStencilAttachment = &depth_reference;
		subpass_description[0].inputAttachmentCount = 0;
		subpass_description[0].pInputAttachments = nullptr;
		subpass_description[0].preserveAttachmentCount = 0;
		subpass_description[0].pPreserveAttachments = nullptr;
		subpass_description[0].pResolveAttachments = nullptr;

		// Subpass dependencies for layout transitions
		std::vector<VkSubpassDependency> dependencies(2);

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		m_swap_chain_render_pass = RenderPass::create(shared_from_this(), attachments, subpass_description, dependencies);
	}



	VkSurfaceFormatKHR Backend::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats)
	{
		if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED)
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		for (const auto& available_format : available_formats)
		{
			if (available_format.format == VK_FORMAT_B8G8R8A8_SNORM && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return available_format;
		}

		return available_formats[0];
	}



	VkPresentModeKHR Backend::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_modes)
	{
		VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& available_mode : available_modes)
		{
			if (available_mode == VK_PRESENT_MODE_MAILBOX_KHR)
				best_mode = available_mode;
			else if (available_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				best_mode = available_mode;
		}

		return best_mode;
	}



	VkExtent2D Backend::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		VkSurfaceCapabilitiesKHR caps;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice.Get(), m_surface->Get(), &caps);
		return caps.maxImageExtent;
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
#include "stdafx.h"
#if SE_VULKAN
#include "VulkanWrapper_Resource.h"

namespace vkWrapper
{
	CommandBuffer::CommandBuffer(LogicalDevice* device, CommandPool* pool)
		: VkDeviceObject(device)
		, m_commandbuffer(VK_NULL_HANDLE)
		, m_pool(pool)
		, m_valid(true)
	{
	}

	CommandBuffer::CommandBuffer(LogicalDevice* device, CommandPool* pool, VkCommandBuffer buffer)
		: VkDeviceObject(device)
		, m_commandbuffer(buffer)
		, m_pool(pool)
		, m_valid(true)
	{
	}

	CommandBuffer::~CommandBuffer()
	{
		Close();
	}

	void CommandBuffer::Init()
	{
		if ( m_commandbuffer != VK_NULL_HANDLE ) return;

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_pool->Get();
		allocInfo.commandBufferCount = 1;

		vkAllocateCommandBuffers(m_parent->Get(), &allocInfo, &m_commandbuffer);
	}

	void CommandBuffer::Close()
	{
		vkFreeCommandBuffers(m_parent->Get(), m_pool->Get(), 1, &m_commandbuffer);
	}

	// CommandBuffer::reset
	// the pool must have the flag VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT

	void CommandBuffer::BeginOneTime()
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(m_commandbuffer, &beginInfo);
	}

	void CommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if ( vkBeginCommandBuffer(m_commandbuffer, &beginInfo) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}
	}

	void CommandBuffer::End()
	{
		if ( vkEndCommandBuffer(m_commandbuffer) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	void CommandBuffer::SubmitAndWait()
	{
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandbuffer;

		Queue* queue = m_pool->GetQueue();
		vkQueueSubmit(queue->Get(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(queue->Get());
	}

	void CommandBuffer::CopyBuffer(VkBuffer dst, VkBuffer src, uint32_t size)
	{
		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(m_commandbuffer, src, dst, 1, &copyRegion);
	}

	CommandPool::CommandPool(LogicalDevice* device, Queue* queue)
		: VkDeviceObject(device)
		, m_queue(queue)
	{
	}

	CommandPool::~CommandPool()
	{
		Close();
	}

	void CommandPool::Init()
	{
		PhysicalDevice* physical = m_parent->GetPhysical();
		auto indices = physical->m_indices;

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = m_queue->GetFamilyIndex();
		poolInfo.flags = 0; // Optional
		if ( vkCreateCommandPool(m_parent->Get(), &poolInfo, nullptr, &m_pool) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to create command pool!");
		}
	}

	void CommandPool::Close()
	{
		vkDestroyCommandPool(m_parent->Get(), m_pool, nullptr);
	}

	void CommandPool::Reset()
	{
		VkCommandPoolResetFlags flags = 0; // see VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT
		vkResetCommandPool(m_parent->Get(), m_pool, flags);
		for ( auto it = m_allocated.begin(); it != m_allocated.end(); ++it )
		{
			(*it)->Invalidate();
			delete *it;
		}
		m_allocated.clear();
	}

	CommandBuffer* CommandPool::Alloc()
	{
		CommandBuffer* cmd = new CommandBuffer(m_parent, this);
		cmd->Init();
		m_allocated.push_back(cmd);
		return cmd;
	}

	std::vector<CommandBuffer*> CommandPool::Alloc(uint32_t count)
	{
		std::vector<VkCommandBuffer> buffers(count);

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_pool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = buffers.size();

		if ( vkAllocateCommandBuffers(m_parent->Get(), &allocInfo, buffers.data()) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}

		std::vector<CommandBuffer*> cmds(count);
		for ( uint32_t i = 0; i < count; ++i )
		{
			cmds[i] = new CommandBuffer(m_parent, this, buffers[i]);
			m_allocated.push_back(cmds[i]);
		}
		return cmds;
	}


	GraphicCmdPool* GraphicCmdPool::s_instance = nullptr;
	TransferCmdPool* TransferCmdPool::s_instance = nullptr;
	std::vector<DrawCmdPool*> DrawCmdPool::s_instances;


	GraphicCmdPool::GraphicCmdPool(LogicalDevice* device)
		: CommandPool(device, device->GetGraphicQueue())
	{
		s_instance = this;
	}

	DrawCmdPool::DrawCmdPool(LogicalDevice* device)
		: CommandPool(device, device->GetGraphicQueue())
	{
		s_instances.push_back(this);
	}
	TransferCmdPool::TransferCmdPool(LogicalDevice* device)
		: CommandPool(device, device->GetTransferQueue())
	{
		s_instance = this;
	}


	Buffer::Buffer(LogicalDevice* device, uint32_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
		: VkDeviceObject(device)
		, m_usage(usage)
		, m_properties(properties)
		, m_size(size)
	{
	}

	Buffer::~Buffer()
	{
		Close();
	}

	void Buffer::Init()
	{
		PhysicalDevice* physical = m_parent->GetPhysical();
		QueueFamilyIndices indices = physical->m_indices;

		// declaration
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = m_size;
		bufferInfo.usage = m_usage;
		// bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.transferFamily.value() };
		bufferInfo.queueFamilyIndexCount = 2; // Optional
		bufferInfo.pQueueFamilyIndices = queueFamilyIndices; // Optional

		if ( vkCreateBuffer(m_parent->Get(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to create buffer!");
		}

		// requirements
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_parent->Get(), m_buffer, &memRequirements);

		// allocation
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = physical->FindMemoryType(memRequirements.memoryTypeBits, m_properties);

		if ( vkAllocateMemory(m_parent->Get(), &allocInfo, nullptr, &m_memory) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		// binding
		vkBindBufferMemory(m_parent->Get(), m_buffer, m_memory, 0);
	}

	void Buffer::Close()
	{
		vkDestroyBuffer(m_parent->Get(), m_buffer, nullptr);
		vkFreeMemory(m_parent->Get(), m_memory, nullptr);
	}

	void Buffer::Copy(const void* data, size_t size)
	{
		void* mapped;
		vkMapMemory(m_parent->Get(), m_memory, 0, size, 0, &mapped);
		{
			memcpy(mapped, data, size);
		}
		vkUnmapMemory(m_parent->Get(), m_memory);
	}

	void Buffer::Copy(Buffer* src)
	{
		CommandBuffer* command = TransferCmdPool::instance()->Alloc();
		command->BeginOneTime();
		command->CopyBuffer(m_buffer, src->Get(), m_size);
		command->End();
		command->SubmitAndWait();
		delete command;
	}

	Image::Image(LogicalDevice* device, uint32_t width, uint32_t height,
		uint32_t miplevels, bool enableMsaa, VkFormat format, VkImageUsageFlags usage)
		: VkDeviceObject(device)
		, m_width(width)
		, m_height(height)
		, m_mipLevels(miplevels)
		, m_msaaEnabled(enableMsaa)
		, m_format(format)
		, m_usage(usage)
	{
	}

	//--------------------------------------------------------------
	Image::~Image()
	{
		Close();
	}

	//--------------------------------------------------------------
	void Image::Init()
	{
		PhysicalDevice* physical = m_parent->GetPhysical();

		m_layout = VK_IMAGE_LAYOUT_UNDEFINED; // only 2 possibles values here. Not usable by the GPU

		// image definition
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_width;
		imageInfo.extent.height = m_height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = m_mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = m_format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = m_layout;
		imageInfo.usage = m_usage;
		imageInfo.samples = m_msaaEnabled ? physical->m_msaaSamples : VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if ( vkCreateImage(m_parent->Get(), &imageInfo, nullptr, &m_image) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to create image!");
		}

		// memory alloc
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_parent->Get(), m_image, &memRequirements);

		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = physical->FindMemoryType(memRequirements.memoryTypeBits, properties);

		if ( vkAllocateMemory(m_parent->Get(), &allocInfo, nullptr, &m_vk_device_memory) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to allocate image memory!");
		}

		// bind
		vkBindImageMemory(m_parent->Get(), m_image, m_vk_device_memory, 0);
	}

	//--------------------------------------------------------------
	void Image::Close()
	{
		vkDestroyImage(m_parent->Get(), m_image, nullptr);
		vkFreeMemory(m_parent->Get(), m_vk_device_memory, nullptr);
	}

	//--------------------------------------------------------------
	void Image::TransitionLayout(VkImageLayout newLayout)
	{
		bool isGraphic = (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			|| newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		PhysicalDevice* physical = m_parent->GetPhysical();

		CommandBuffer* cmd = isGraphic ? GraphicCmdPool::instance()->Alloc() : TransferCmdPool::instance()->Alloc();
		cmd->BeginOneTime();

		VkCommandBuffer commandBuffer = cmd->Get();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = m_layout;
		barrier.newLayout = newLayout;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.image = m_image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = m_mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		// handle transitions :
		// Undefined -> transfer destination: transfer writes that don't need to wait on anything
		// Transfer destination -> shader reading: shader reads should wait on transfer writes
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if ( m_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
			newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL )
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if ( m_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
			newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if ( m_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
			newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL )
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask =
				VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			throw std::invalid_argument("unsupported layout transition!");
		}


		// case of depth stencil attachment
		if ( newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL )
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if ( physical->HasStencilComponent(m_format) )
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}


		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		cmd->End();
		cmd->SubmitAndWait();
		m_layout = newLayout;
		delete cmd;
	}

	//--------------------------------------------------------------
	void Image::Copy(Buffer* buffer)
	{
		CommandBuffer* cmd = TransferCmdPool::instance()->Alloc();
		cmd->BeginOneTime();
		VkCommandBuffer commandBuffer = cmd->Get();

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { m_width, m_height, 1 };

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer->Get(),
			m_image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		cmd->End();
		cmd->SubmitAndWait();
		delete cmd;
	}

	//--------------------------------------------------------------
	void Image::GenerateMipmaps(VkImageLayout src_layout, VkImageLayout dst_layout)
	{
		std::cout << "generate mipmap " << m_mipLevels << std::endl;
		PhysicalDevice* physical = m_parent->GetPhysical();

		// Check if image format supports linear blitting
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(physical->Get(), m_format, &formatProperties);
		if ( !(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) )
		{
			throw std::runtime_error("texture image format does not support linear blitting!");
		}

		CommandBuffer* cmd = GraphicCmdPool::instance()->Alloc();
		cmd->BeginOneTime();
		VkCommandBuffer commandBuffer = cmd->Get();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = m_image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = m_width;
		int32_t mipHeight = m_height;

		for ( uint32_t i = 1; i < m_mipLevels; i++ )
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit = {};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer,
				m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if ( mipWidth > 1 ) mipWidth /= 2;
			if ( mipHeight > 1 ) mipHeight /= 2;
		}

		// handle the last mip level
		barrier.subresourceRange.baseMipLevel = m_mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		cmd->End();
		cmd->SubmitAndWait();
		delete cmd;
	}

} // namespace vkWrapper
#endif // SE_VULKAN
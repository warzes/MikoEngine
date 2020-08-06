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

} // namespace vkWrapper
#endif // SE_VULKAN
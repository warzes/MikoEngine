#include "stdafx.h"
#if SE_VULKAN
#include "VulkanRender.h"
#include "Core/Logger.h"
#include "Utility/Utility.h"

#    define MAX_COMMAND_THREADS 32
#    define MAX_DESCRIPTOR_POOL_THREADS 32
#    define MAX_THREAD_LOCAL_COMMAND_BUFFERS 8


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
	struct ThreadLocalCommandBuffers
	{
		CommandPool::Ptr   command_pool[Backend::kMaxFramesInFlight];
		CommandBuffer::Ptr command_buffers[Backend::kMaxFramesInFlight][MAX_THREAD_LOCAL_COMMAND_BUFFERS];
		uint32_t           allocated_buffers = 0;

		ThreadLocalCommandBuffers(Backend::Ptr backend, uint32_t queue_family)
		{
			for ( int i = 0; i < Backend::kMaxFramesInFlight; i++ )
			{
				command_pool[i] = CommandPool::create(backend, queue_family);

				for ( int j = 0; j < MAX_THREAD_LOCAL_COMMAND_BUFFERS; j++ )
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
			if ( allocated_buffers >= MAX_THREAD_LOCAL_COMMAND_BUFFERS )
			{
				SE_LOG_FATAL("(Vulkan) Max thread local command buffer count reached!");
				throw std::runtime_error("(Vulkan) Max thread local command buffer count reached!");
			}

			auto cmd_buf = command_buffers[frame_index][allocated_buffers++];

			if ( begin )
			{
				VkCommandBufferBeginInfo begin_info;
				SE_ZERO_MEMORY(begin_info);

				begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

				vkBeginCommandBuffer(cmd_buf->handle(), &begin_info);
			}

			return cmd_buf;
		}
	};

	thread_local std::shared_ptr<ThreadLocalCommandBuffers> g_graphics_command_buffers[MAX_COMMAND_THREADS];
	thread_local std::shared_ptr<ThreadLocalCommandBuffers> g_compute_command_buffers[MAX_COMMAND_THREADS];
	thread_local std::shared_ptr<ThreadLocalCommandBuffers> g_transfer_command_buffers[MAX_COMMAND_THREADS];
	thread_local DescriptorPool::Ptr                        g_descriptor_pools[MAX_DESCRIPTOR_POOL_THREADS];

	std::atomic<uint32_t>                                   g_thread_counter = 0;
	thread_local uint32_t                                   g_thread_idx = g_thread_counter++;

	Backend::Ptr Backend::create(GLFWwindow* window, bool enable_validation_layers, bool require_ray_tracing)
	{
		std::shared_ptr<Backend> backend = std::shared_ptr<Backend>(new Backend(window, enable_validation_layers, require_ray_tracing));
		backend->initialize();
		return backend;
	}

	Backend::Backend(GLFWwindow* window, bool enable_validation_layers, bool require_ray_tracing) :
		m_window(window)
	{
		m_ray_tracing_enabled = require_ray_tracing;

		SE_LOG_INFO("start init vulkan");
		m_instance = new vkWrapper::Instance();
		m_instance->Init();
		m_debugMessenger = m_instance->CreateDebugMessenger(); // TODO: in Instance
		m_surface = m_instance->CreateSurface(window);
		m_physicalDevice = m_instance->PickPhysicalDevice(m_surface, m_ray_tracing_enabled);
		m_device = new vkWrapper::LogicalDevice(&m_physicalDevice);
		m_device->Init(m_ray_tracing_enabled);

		VmaAllocatorCreateInfo allocator_info = {};
		allocator_info.physicalDevice = m_physicalDevice.Get();
		allocator_info.device = m_device->Get();
		if ( vmaCreateAllocator(&allocator_info, &m_vma_allocator) != VK_SUCCESS )
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Allocator.");
			throw std::runtime_error("(Vulkan) Failed to create Allocator.");
		}

		if ( require_ray_tracing )
			load_ray_tracing_funcs();
	}

	Backend::~Backend()
	{
		for ( int i = 0; i < MAX_COMMAND_THREADS; i++ )
		{
			g_graphics_command_buffers[i].reset();
			g_compute_command_buffers[i].reset();
			g_transfer_command_buffers[i].reset();
		}

		for ( int i = 0; i < MAX_DESCRIPTOR_POOL_THREADS; i++ )
			g_descriptor_pools[i].reset();

		for ( int i = 0; i < m_swap_chain_images.size(); i++ )
		{
			m_swap_chain_framebuffers[i].reset();
			m_swap_chain_image_views[i].reset();
		}

		for ( int i = 0; i < m_in_flight_fences.size(); i++ )
			m_in_flight_fences[i].reset();

		m_swap_chain_render_pass.reset();
		m_swap_chain_depth_view.reset();
		m_swap_chain_depth.reset();

		if ( m_vk_swap_chain )
		{
			vkDestroySwapchainKHR(m_device->Get(), m_vk_swap_chain, nullptr);
			m_vk_swap_chain = nullptr;
		}

		if ( m_vma_allocator )
		{
			vmaDestroyAllocator(m_vma_allocator);
			m_vma_allocator = nullptr;
		}

		delete m_debugMessenger; m_debugMessenger = nullptr;
		delete m_surface; m_surface = nullptr;
		delete m_device; m_device = nullptr;
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

		for ( int i = 0; i < MAX_DESCRIPTOR_POOL_THREADS; i++ )
			g_descriptor_pools[i] = DescriptorPool::create(shared_from_this(), dp_desc);

		for ( int i = 0; i < MAX_COMMAND_THREADS; i++ )
		{
			g_graphics_command_buffers[i] = std::make_shared<ThreadLocalCommandBuffers>(shared_from_this(), m_physicalDevice.queue_infos().graphics_queue_index);
			g_compute_command_buffers[i] = std::make_shared<ThreadLocalCommandBuffers>(shared_from_this(), m_physicalDevice.queue_infos().compute_queue_index);
			g_transfer_command_buffers[i] = std::make_shared<ThreadLocalCommandBuffers>(shared_from_this(), m_physicalDevice.queue_infos().transfer_queue_index);
		}
	}

	void Backend::load_ray_tracing_funcs()
	{
		g_ray_tracing_func_table._vkCreateAccelerationStructureNV = reinterpret_cast<PFN_vkCreateAccelerationStructureNV>(vkGetDeviceProcAddr(m_device->Get(), "vkCreateAccelerationStructureNV"));
		g_ray_tracing_func_table._vkDestroyAccelerationStructureNV = reinterpret_cast<PFN_vkDestroyAccelerationStructureNV>(vkGetDeviceProcAddr(m_device->Get(), "vkDestroyAccelerationStructureNV"));
		g_ray_tracing_func_table._vkGetAccelerationStructureMemoryRequirementsNV = reinterpret_cast<PFN_vkGetAccelerationStructureMemoryRequirementsNV>(vkGetDeviceProcAddr(m_device->Get(), "vkGetAccelerationStructureMemoryRequirementsNV"));
		g_ray_tracing_func_table._vkBindAccelerationStructureMemoryNV = reinterpret_cast<PFN_vkBindAccelerationStructureMemoryNV>(vkGetDeviceProcAddr(m_device->Get(), "vkBindAccelerationStructureMemoryNV"));
		g_ray_tracing_func_table._vkCmdBuildAccelerationStructureNV = reinterpret_cast<PFN_vkCmdBuildAccelerationStructureNV>(vkGetDeviceProcAddr(m_device->Get(), "vkCmdBuildAccelerationStructureNV"));
		g_ray_tracing_func_table._vkCmdCopyAccelerationStructureNV = reinterpret_cast<PFN_vkCmdCopyAccelerationStructureNV>(vkGetDeviceProcAddr(m_device->Get(), "vkCmdCopyAccelerationStructureNV"));
		g_ray_tracing_func_table._vkCmdTraceRaysNV = reinterpret_cast<PFN_vkCmdTraceRaysNV>(vkGetDeviceProcAddr(m_device->Get(), "vkCmdTraceRaysNV"));
		g_ray_tracing_func_table._vkCreateRayTracingPipelinesNV = reinterpret_cast<PFN_vkCreateRayTracingPipelinesNV>(vkGetDeviceProcAddr(m_device->Get(), "vkCreateRayTracingPipelinesNV"));
		g_ray_tracing_func_table._vkGetRayTracingShaderGroupHandlesNV = reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesNV>(vkGetDeviceProcAddr(m_device->Get(), "vkGetRayTracingShaderGroupHandlesNV"));
		g_ray_tracing_func_table._vkGetAccelerationStructureHandleNV = reinterpret_cast<PFN_vkGetAccelerationStructureHandleNV>(vkGetDeviceProcAddr(m_device->Get(), "vkGetAccelerationStructureHandleNV"));
		g_ray_tracing_func_table._vkCmdWriteAccelerationStructuresPropertiesNV = reinterpret_cast<PFN_vkCmdWriteAccelerationStructuresPropertiesNV>(vkGetDeviceProcAddr(m_device->Get(), "vkCmdWriteAccelerationStructuresPropertiesNV"));
		g_ray_tracing_func_table._vkCompileDeferredNV = reinterpret_cast<PFN_vkCompileDeferredNV>(vkGetDeviceProcAddr(m_device->Get(), "vkCompileDeferredNV"));
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
		submit(m_device->m_vk_graphics_queue, cmd_bufs, wait_semaphores, wait_stages, signal_semaphores);
	}



	void Backend::submit_compute(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs,
		const std::vector<std::shared_ptr<Semaphore>>& wait_semaphores,
		const std::vector<VkPipelineStageFlags>& wait_stages,
		const std::vector<std::shared_ptr<Semaphore>>& signal_semaphores)
	{
		submit(m_device->m_vk_compute_queue, cmd_bufs, wait_semaphores, wait_stages, signal_semaphores);
	}



	void Backend::submit_transfer(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs,
		const std::vector<std::shared_ptr<Semaphore>>& wait_semaphores,
		const std::vector<VkPipelineStageFlags>& wait_stages,
		const std::vector<std::shared_ptr<Semaphore>>& signal_semaphores)
	{
		submit(m_device->m_vk_transfer_queue, cmd_bufs, wait_semaphores, wait_stages, signal_semaphores);
	}



	void Backend::flush_graphics(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs)
	{
		flush(m_device->m_vk_graphics_queue, cmd_bufs);

		for ( int i = 0; i < MAX_COMMAND_THREADS; i++ )
			g_graphics_command_buffers[i]->reset(m_current_frame);
	}



	void Backend::flush_compute(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs)
	{
		flush(m_device->m_vk_compute_queue, cmd_bufs);

		for ( int i = 0; i < MAX_COMMAND_THREADS; i++ )
			g_compute_command_buffers[i]->reset(m_current_frame);
	}



	void Backend::flush_transfer(const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs)
	{
		flush(m_device->m_vk_transfer_queue, cmd_bufs);

		for ( int i = 0; i < MAX_COMMAND_THREADS; i++ )
			g_transfer_command_buffers[i]->reset(m_current_frame);
	}



	void Backend::submit(VkQueue                                            queue,
		const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs,
		const std::vector<std::shared_ptr<Semaphore>>& wait_semaphores,
		const std::vector<VkPipelineStageFlags>& wait_stages,
		const std::vector<std::shared_ptr<Semaphore>>& signal_semaphores)
	{
		VkSemaphore vk_wait_semaphores[16];

		for ( int i = 0; i < wait_semaphores.size(); i++ )
			vk_wait_semaphores[i] = wait_semaphores[i]->handle();

		VkSemaphore vk_signal_semaphores[16];

		for ( int i = 0; i < signal_semaphores.size(); i++ )
			vk_signal_semaphores[i] = signal_semaphores[i]->handle();

		VkCommandBuffer vk_cmd_bufs[32];

		for ( int i = 0; i < cmd_bufs.size(); i++ )
			vk_cmd_bufs[i] = cmd_bufs[i]->handle();

		VkPipelineStageFlags vk_wait_stages[16];

		for ( int i = 0; i < wait_semaphores.size(); i++ )
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

		vkResetFences(m_device->Get(), 1, &m_in_flight_fences[m_current_frame]->handle());

		if ( vkQueueSubmit(queue, 1, &submit_info, m_in_flight_fences[m_current_frame]->handle()) != VK_SUCCESS )
		{
			SE_LOG_FATAL("(Vulkan) Failed to submit command buffer!");
			throw std::runtime_error("(Vulkan) Failed to submit command buffer!");
		}
	}

	void Backend::flush(VkQueue queue, const std::vector<std::shared_ptr<CommandBuffer>>& cmd_bufs)
	{
		VkCommandBuffer vk_cmd_bufs[32];

		for ( int i = 0; i < cmd_bufs.size(); i++ )
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
		vkCreateFence(m_device->Get(), &fence_info, nullptr, &fence);

		// Submit to the queue
		vkQueueSubmit(queue, 1, &submit_info, fence);

		// Wait for the fence to signal that command buffer has finished executing
		vkWaitForFences(m_device->Get(), 1, &fence, VK_TRUE, 100000000000);

		vkDestroyFence(m_device->Get(), fence, nullptr);
	}



	void Backend::acquire_next_swap_chain_image(const std::shared_ptr<Semaphore>& semaphore)
	{
		vkWaitForFences(m_device->Get(), 1, &m_in_flight_fences[m_current_frame]->handle(), VK_TRUE, UINT64_MAX);

		for ( int i = 0; i < MAX_COMMAND_THREADS; i++ )
		{
			g_graphics_command_buffers[i]->reset(m_current_frame);
			g_compute_command_buffers[i]->reset(m_current_frame);
			g_transfer_command_buffers[i]->reset(m_current_frame);
		}

		VkResult result = vkAcquireNextImageKHR(m_device->Get(), m_vk_swap_chain, UINT64_MAX, semaphore->handle(), VK_NULL_HANDLE, &m_image_index);

		if ( result == VK_ERROR_OUT_OF_DATE_KHR )
		{
			recreate_swapchain();
			return;
		}
		else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
		{
			SE_LOG_FATAL("(Vulkan) Failed to acquire swap chain image!");
			throw std::runtime_error("(Vulkan) Failed to acquire swap chain image!");
		}
	}



	void Backend::present(const std::vector<std::shared_ptr<Semaphore>>& semaphores)
	{
		VkSemaphore signal_semaphores[16];

		for ( int i = 0; i < semaphores.size(); i++ )
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

		if ( vkQueuePresentKHR(m_device->m_vk_presentation_queue, &present_info) != VK_SUCCESS )
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
		vkDeviceWaitIdle(m_device->Get());
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
		return m_device->m_vk_graphics_queue;
	}



	VkQueue Backend::transfer_queue()
	{
		return m_device->m_vk_transfer_queue;
	}



	VkQueue Backend::compute_queue()
	{
		return m_device->m_vk_compute_queue;
	}



	VkDevice Backend::device()
	{
		return m_device->Get();
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

		if ( min_ubo_alignment > 0 )
			aligned_size = (aligned_size + min_ubo_alignment - 1) & ~(min_ubo_alignment - 1);

		return aligned_size;
	}



	VkFormat Backend::find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for ( VkFormat format : candidates )
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_physicalDevice.Get(), format, &props);

			if ( tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features )
			{
				return format;
			}
			else if ( tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features )
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

		if ( present_mode_count != 0 )
		{
			details.presentModes.resize(present_mode_count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface->Get(), &present_mode_count, &details.presentModes[0]);
		}

		uint32_t format_count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface->Get(), &format_count, nullptr);

		if ( format_count != 0 )
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

		for ( const char* layer_name : layers )
		{
			bool layer_found = false;

			for ( const auto& layer_properties : available_layers )
			{
				if ( std::string(layer_name) == std::string(layer_properties.layerName) )
				{
					layer_found = true;
					break;
				}
			}

			if ( !layer_found )
			{
				SE_LOG_FATAL("(Vulkan) Validation Layer not available: " + std::string(layer_name));
				return false;
			}
		}

		return true;
	}

	bool Backend::create_swapchain()
	{
		m_current_frame = 0;
		VkSurfaceFormatKHR surface_format = choose_swap_surface_format(m_physicalDevice.GetSwapChainDetails().formats);
		VkPresentModeKHR   present_mode = choose_swap_present_mode(m_physicalDevice.GetSwapChainDetails().presentModes);
		VkExtent2D         extent = choose_swap_extent(m_physicalDevice.GetSwapChainDetails().capabilities);

		uint32_t image_count = m_physicalDevice.GetSwapChainDetails().capabilities.minImageCount + 1;

		if ( m_physicalDevice.GetSwapChainDetails().capabilities.maxImageCount > 0 && image_count > m_physicalDevice.GetSwapChainDetails().capabilities.maxImageCount )
			image_count = m_physicalDevice.GetSwapChainDetails().capabilities.maxImageCount;

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

		uint32_t queue_family_indices[] = { (uint32_t)m_physicalDevice.queue_infos().graphics_queue_index, (uint32_t)m_physicalDevice.queue_infos().presentation_queue_index };

		if ( m_physicalDevice.queue_infos().presentation_queue_index != m_physicalDevice.queue_infos().graphics_queue_index )
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

		create_info.preTransform = m_physicalDevice.GetSwapChainDetails().capabilities.currentTransform;
		create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		create_info.presentMode = present_mode;
		create_info.clipped = VK_TRUE;
		create_info.oldSwapchain = VK_NULL_HANDLE;

		if ( vkCreateSwapchainKHR(m_device->Get(), &create_info, nullptr, &m_vk_swap_chain) != VK_SUCCESS )
			return false;

		uint32_t swap_image_count = 0;
		vkGetSwapchainImagesKHR(m_device->Get(), m_vk_swap_chain, &swap_image_count, nullptr);
		m_swap_chain_images.resize(swap_image_count);
		m_swap_chain_image_views.resize(swap_image_count);
		m_swap_chain_framebuffers.resize(swap_image_count);

		VkImage images[32];

		if ( vkGetSwapchainImagesKHR(m_device->Get(), m_vk_swap_chain, &swap_image_count, &images[0]) != VK_SUCCESS )
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

		for ( int i = 0; i < swap_image_count; i++ )
		{
			m_swap_chain_images[i] = Image::create_from_swapchain(shared_from_this(), images[i], VK_IMAGE_TYPE_2D, m_swap_chain_extent.width, m_swap_chain_extent.height, 1, 1, 1, m_swap_chain_image_format, VMA_MEMORY_USAGE_UNKNOWN, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SAMPLE_COUNT_1_BIT);
			m_swap_chain_image_views[i] = ImageView::create(shared_from_this(), m_swap_chain_images[i], VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);

			views[0] = m_swap_chain_image_views[i];

			m_swap_chain_framebuffers[i] = Framebuffer::create(shared_from_this(), m_swap_chain_render_pass, views, m_swap_chain_extent.width, m_swap_chain_extent.height, 1);
		}

		m_in_flight_fences.resize(kMaxFramesInFlight);

		for ( size_t i = 0; i < kMaxFramesInFlight; i++ )
			m_in_flight_fences[i] = Fence::create(shared_from_this());

		return true;
	}



	void Backend::recreate_swapchain()
	{
		vkDeviceWaitIdle(m_device->Get());

		// Destroy existing swap chain resources
		for ( int i = 0; i < m_swap_chain_images.size(); i++ )
		{
			m_swap_chain_images[i].reset();
			m_swap_chain_framebuffers[i].reset();
			m_swap_chain_image_views[i].reset();
		}

		vkDestroySwapchainKHR(m_device->Get(), m_vk_swap_chain, nullptr);

		if ( !create_swapchain() )
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
		if ( available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED )
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		for ( const auto& available_format : available_formats )
		{
			if ( available_format.format == VK_FORMAT_B8G8R8A8_SNORM && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
				return available_format;
		}

		return available_formats[0];
	}



	VkPresentModeKHR Backend::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_modes)
	{
		VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

		for ( const auto& available_mode : available_modes )
		{
			if ( available_mode == VK_PRESENT_MODE_MAILBOX_KHR )
				best_mode = available_mode;
			else if ( available_mode == VK_PRESENT_MODE_IMMEDIATE_KHR )
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
} // namespace vk
#endif // SE_VULKAN
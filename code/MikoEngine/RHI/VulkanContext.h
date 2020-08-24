#pragma once

namespace VulkanRhi
{
	class VulkanContext final
	{
	public:
		explicit VulkanContext(VulkanRhi &vulkanRhi) 
			: mVulkanRhi(vulkanRhi)
			, mVkPhysicalDevice(VK_NULL_HANDLE)
			, mVkDevice(VK_NULL_HANDLE)
			, mGraphicsQueueFamilyIndex(~0u)
			, mPresentQueueFamilyIndex(~0u)
			, mGraphicsVkQueue(VK_NULL_HANDLE)
			, mPresentVkQueue(VK_NULL_HANDLE)
			, mVkCommandPool(VK_NULL_HANDLE)
			, mVkCommandBuffer(VK_NULL_HANDLE)
		{
			const VulkanRuntimeLinking &vulkanRuntimeLinking = mVulkanRhi.getVulkanRuntimeLinking();

			// Get the physical Vulkan device this context should use
			bool enableDebugMarker = true;	// TODO(co) Make it possible to setup from the outside whether or not the "VK_EXT_debug_marker"-extension should be used (e.g. retail shipped games might not want to have this enabled)
			{
				detail::VkPhysicalDevices vkPhysicalDevices;
				::detail::enumeratePhysicalDevices(vulkanRuntimeLinking.getVkInstance(), vkPhysicalDevices);
				if ( !vkPhysicalDevices.empty() )
				{
					mVkPhysicalDevice = ::detail::selectPhysicalDevice(vkPhysicalDevices, vulkanRhi.getVulkanRuntimeLinking().isValidationEnabled(), enableDebugMarker);
				}
			}

			// Create the logical Vulkan device instance
			if ( VK_NULL_HANDLE != mVkPhysicalDevice )
			{
				mVkDevice = ::detail::createVkDevice(mVulkanRhi.getVkAllocationCallbacks(), mVkPhysicalDevice, vulkanRuntimeLinking.isValidationEnabled(), enableDebugMarker, mGraphicsQueueFamilyIndex, mPresentQueueFamilyIndex);
				if ( VK_NULL_HANDLE != mVkDevice )
				{
					{
						// Get the Vulkan device graphics queue that command buffers are submitted to
						vkGetDeviceQueue(mVkDevice, mGraphicsQueueFamilyIndex, 0, &mGraphicsVkQueue);
						if ( VK_NULL_HANDLE != mGraphicsVkQueue )
						{
							// Get the Vulkan device present queue
							vkGetDeviceQueue(mVkDevice, mPresentQueueFamilyIndex, 0, &mPresentVkQueue);
							if ( VK_NULL_HANDLE != mPresentVkQueue )
							{
								// Create Vulkan command pool instance
								mVkCommandPool = ::detail::createVkCommandPool(mVulkanRhi.getVkAllocationCallbacks(), mVkDevice, mGraphicsQueueFamilyIndex);
								if ( VK_NULL_HANDLE != mVkCommandPool )
								{
									// Create Vulkan command buffer instance
									mVkCommandBuffer = ::detail::createVkCommandBuffer(mVkDevice, mVkCommandPool);
								}
								else
								{
									// Error!
									RHI_LOG(CRITICAL, "Failed to create Vulkan command pool instance")
								}
							}
						}
						else
						{
							// Error!
							RHI_LOG(CRITICAL, "Failed to get the Vulkan device graphics queue that command buffers are submitted to")
						}
					}
				}
			}
		}

		~VulkanContext()
		{
			if ( VK_NULL_HANDLE != mVkDevice )
			{
				if ( VK_NULL_HANDLE != mVkCommandPool )
				{
					if ( VK_NULL_HANDLE != mVkCommandBuffer )
					{
						vkFreeCommandBuffers(mVkDevice, mVkCommandPool, 1, &mVkCommandBuffer);
					}
					vkDestroyCommandPool(mVkDevice, mVkCommandPool, mVulkanRhi.getVkAllocationCallbacks());
				}
				vkDeviceWaitIdle(mVkDevice);
				vkDestroyDevice(mVkDevice, mVulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return whether or not the content is initialized
		*
		*  @return
		*    "true" if the context is initialized, else "false"
		*/
		[[nodiscard]] inline bool isInitialized() const
		{
			return (VK_NULL_HANDLE != mVkCommandBuffer);
		}

		/**
		*  @brief
		*    Return the owner Vulkan RHI instance
		*
		*  @return
		*    Owner Vulkan RHI instance
		*/
		[[nodiscard]] inline VulkanRhi& getVulkanRhi() const
		{
			return mVulkanRhi;
		}

		/**
		*  @brief
		*    Return the Vulkan physical device this context is using
		*
		*  @return
		*    The Vulkan physical device this context is using
		*/
		[[nodiscard]] inline VkPhysicalDevice getVkPhysicalDevice() const
		{
			return mVkPhysicalDevice;
		}

		/**
		*  @brief
		*    Return the Vulkan device this context is using
		*
		*  @return
		*    The Vulkan device this context is using
		*/
		[[nodiscard]] inline VkDevice getVkDevice() const
		{
			return mVkDevice;
		}

		/**
		*  @brief
		*    Return the used graphics queue family index
		*
		*  @return
		*    Graphics queue family index, ~0u if invalid
		*/
		[[nodiscard]] inline uint32_t getGraphicsQueueFamilyIndex() const
		{
			return mGraphicsQueueFamilyIndex;
		}

		/**
		*  @brief
		*    Return the used present queue family index
		*
		*  @return
		*    Present queue family index, ~0u if invalid
		*/
		[[nodiscard]] inline uint32_t getPresentQueueFamilyIndex() const
		{
			return mPresentQueueFamilyIndex;
		}

		/**
		*  @brief
		*    Return the handle to the Vulkan device graphics queue that command buffers are submitted to
		*
		*  @return
		*    Handle to the Vulkan device graphics queue that command buffers are submitted to
		*/
		[[nodiscard]] inline VkQueue getGraphicsVkQueue() const
		{
			return mGraphicsVkQueue;
		}

		/**
		*  @brief
		*    Return the handle to the Vulkan device present queue
		*
		*  @return
		*    Handle to the Vulkan device present queue
		*/
		[[nodiscard]] inline VkQueue getPresentVkQueue() const
		{
			return mPresentVkQueue;
		}

		/**
		*  @brief
		*    Return the used Vulkan command buffer pool instance
		*
		*  @return
		*    The used Vulkan command buffer pool instance
		*/
		[[nodiscard]] inline VkCommandPool getVkCommandPool() const
		{
			return mVkCommandPool;
		}

		/**
		*  @brief
		*    Return the Vulkan command buffer instance
		*
		*  @return
		*    The Vulkan command buffer instance
		*/
		[[nodiscard]] inline VkCommandBuffer getVkCommandBuffer() const
		{
			return mVkCommandBuffer;
		}

		// TODO(co) Trivial implementation to have something to start with. Need to use more clever memory management and stating buffers later on.
		[[nodiscard]] uint32_t findMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags vkMemoryPropertyFlags) const
		{
			VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;
			vkGetPhysicalDeviceMemoryProperties(mVkPhysicalDevice, &vkPhysicalDeviceMemoryProperties);
			for ( uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; ++i )
			{
				if ( (typeFilter & (1 << i)) && (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & vkMemoryPropertyFlags) == vkMemoryPropertyFlags )
				{
					return i;
				}
			}

			// Error!
			RHI_LOG(CRITICAL, "Failed to find suitable Vulkan memory type");
			return ~0u;
		}

		[[nodiscard]] inline VkCommandBuffer createVkCommandBuffer() const
		{
			return ::detail::createVkCommandBuffer(mVkDevice, mVkCommandPool);
		}

		void destroyVkCommandBuffer(VkCommandBuffer vkCommandBuffer) const
		{
			if ( VK_NULL_HANDLE != mVkCommandBuffer )
			{
				vkFreeCommandBuffers(mVkDevice, mVkCommandPool, 1, &vkCommandBuffer);
			}
		}

	protected:
		explicit VulkanContext(const VulkanContext&) = delete;
		VulkanContext& operator=(const VulkanContext&) = delete;

	private:
		VulkanRhi&		 mVulkanRhi;				// Owner Vulkan RHI instance
		VkPhysicalDevice mVkPhysicalDevice;			// Vulkan physical device this context is using
		VkDevice		 mVkDevice;					// Vulkan device instance this context is using (equivalent of a OpenGL context or Direct3D 11 device)
		uint32_t		 mGraphicsQueueFamilyIndex;	// Graphics queue family index, ~0u if invalid
		uint32_t		 mPresentQueueFamilyIndex;	// Present queue family index, ~0u if invalid
		VkQueue			 mGraphicsVkQueue;			// Handle to the Vulkan device graphics queue that command buffers are submitted to
		VkQueue			 mPresentVkQueue;			// Handle to the Vulkan device present queue
		VkCommandPool	 mVkCommandPool;			// Vulkan command buffer pool instance
		VkCommandBuffer  mVkCommandBuffer;			// Vulkan command buffer instance
	};
} // namespace VulkanRhi
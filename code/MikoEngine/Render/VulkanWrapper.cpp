#include "stdafx.h"
#if SE_VULKAN
#include "VulkanWrapper.h"
#include "Core/Logger.h"
//-----------------------------------------------------------------------------
namespace vkWrapper
{
#pragma region Extensions
	//-------------------------------------------------------------------------
	// Extensions
	//-------------------------------------------------------------------------
	void Extensions::Enable(const std::string& extname)
	{
		auto it = std::find(m_exts.begin(), m_exts.end(), extname);
		if (it == m_exts.end())
		{
			m_exts.push_back(extname);
			updateBuf();
		}
	}

	bool Extensions::IsEnabled(const std::string& extname) const
	{
		if (std::find(m_exts.begin(), m_exts.end(), extname) == m_exts.end())
			return false;
		return true;
	}

	void Extensions::Disable(const std::string& extname)
	{
		auto it = std::find(m_exts.begin(), m_exts.end(), extname);
		if (it != m_exts.end())
		{
			m_exts.erase(it);
			updateBuf();
		}
	}

	bool Extensions::CheckSupport()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExts(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExts.data());

		for (const auto& extName : m_extBuf)
		{
			bool layerFound = false;

			for (const auto& extProperties : availableExts)
			{
				if (strcmp(extName, extProperties.extensionName) == 0) 
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound) return false;
		}

		return true;
	}

	bool Extensions::CheckDeviceSupport(VkPhysicalDevice& device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> required(m_exts.begin(), m_exts.end());

		for (const auto& ext : availableExtensions)
			required.erase(ext.extensionName);

		return required.empty();
	}

	void Extensions::operator+=(const Extensions& that)
	{
		for (auto it : that.m_exts) Enable(it);
		updateBuf();
	}

	uint32_t Extensions::Count() const
	{
		return m_extBuf.size();
	}
		
	const char* const* Extensions::Data() const
	{
		return m_extBuf.data();
	}

	Extensions Extensions::Default() // передавать enableValidationLayers аргументов
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		Extensions extensions;
		for (uint32_t ext = 0; ext < glfwExtensionCount; ++ext) extensions.Enable(glfwExtensions[ext]);

		if (enableValidationLayers) extensions.Enable(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	void Extensions::updateBuf()
	{
		m_extBuf.resize(m_exts.size());
		for (uint32_t i = 0; i < m_exts.size(); ++i)
		{
			m_extBuf[i] = m_exts[i].data();
		}
	}
#pragma endregion

#pragma region ValidationLayers
	//-------------------------------------------------------------------------
	// ValidationLayers
	//-------------------------------------------------------------------------
	void ValidationLayers::Enable(const std::string& layername)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layername);
		if (it == m_layers.end())
		{
			m_layers.push_back(layername);
			updateBuf();
		}
	}

	bool ValidationLayers::IsEnabled(const std::string& layername) const
	{
		if (std::find(m_layers.begin(), m_layers.end(), layername) == m_layers.end())
			return false;
		return true;
	}

	void ValidationLayers::Disable(const std::string& layername)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layername);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			updateBuf();
		}
	}

	bool ValidationLayers::CheckSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : m_layersBuf)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) 
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound) return false;
		}

		return true;
	}

	uint32_t ValidationLayers::Count() const
	{
		return m_layersBuf.size();
	}

	const char* const* ValidationLayers::Data() const
	{
		return m_layersBuf.data();
	}

	ValidationLayers ValidationLayers::Default()
	{
		ValidationLayers layers;
		layers.Enable("VK_LAYER_KHRONOS_validation");
		return layers;
	}

	void ValidationLayers::updateBuf()
	{
		m_layersBuf.resize(m_layers.size());
		for (uint32_t i = 0; i < m_layers.size(); ++i)
		{
			m_layersBuf[i] = m_layers[i].data();
		}
	}

#pragma endregion

#pragma region PhysicalDevice
	//-------------------------------------------------------------------------
	// PhysicalDevice
	//-------------------------------------------------------------------------

	const char* kVendorNames[] =
	{
	  "Unknown",
	  "AMD",
	  "IMAGINATION",
	  "NVIDIA",
	  "ARM",
	  "QUALCOMM",
	  "INTEL"
	};

	const char* get_vendor_name(uint32_t id)
	{
		switch ( id )
		{
		case 0x1002:
			return kVendorNames[1];
		case 0x1010:
			return kVendorNames[2];
		case 0x10DE:
			return kVendorNames[3];
		case 0x13B5:
			return kVendorNames[4];
		case 0x5143:
			return kVendorNames[5];
		case 0x8086:
			return kVendorNames[6];
		default:
			return kVendorNames[0];
		}
	}

	const char* kDeviceTypes[] = 
	{
		"VK_PHYSICAL_DEVICE_TYPE_OTHER",
		"VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU",
		"VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU",
		"VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU",
		"VK_PHYSICAL_DEVICE_TYPE_CPU"
	};

	bool is_queue_compatible(VkQueueFlags current_queue_flags, int32_t graphics, int32_t compute, int32_t transfer)
	{
		if ( graphics == 1 )
		{
			// If you need graphics, and queue doesn't have it...
			if ( !(current_queue_flags & VK_QUEUE_GRAPHICS_BIT) )
				return false;
		}
		else if ( graphics == 0 )
		{
			// If you don't need graphics, but queue has it...
			if ( current_queue_flags & VK_QUEUE_GRAPHICS_BIT )
				return false;
		}

		if ( compute == 1 )
		{
			// If you need compute, and queue doesn't have it...
			if ( !(current_queue_flags & VK_QUEUE_COMPUTE_BIT) )
				return false;
		}
		else if ( compute == 0 )
		{
			// If you don't need compute, but queue has it...
			if ( current_queue_flags & VK_QUEUE_COMPUTE_BIT )
				return false;
		}

		if ( transfer == 1 )
		{
			// If you need transfer, and queue doesn't have it...
			if ( !(current_queue_flags & VK_QUEUE_TRANSFER_BIT) )
				return false;
		}
		else if ( transfer == 0 )
		{
			// If you don't need transfer, but queue has it...
			if ( current_queue_flags & VK_QUEUE_TRANSFER_BIT )
				return false;
		}

		return true;
	}

	PhysicalDevice::PhysicalDevice(VkPhysicalDevice& physical)
		: m_physical(physical)
	{
	}

	bool PhysicalDevice::IsDeviceSuitable(VkSurfaceKHR& surface, VkPhysicalDeviceType type, bool requireRayRracing)
	{
		vkGetPhysicalDeviceProperties(m_physical, &m_device_properties);
		if (m_device_properties.deviceType == type)
		{
			m_deviceExtensions = DefaultExtensions();
			if (requireRayRracing)
			{
				m_deviceExtensions.Enable(VK_NV_RAY_TRACING_EXTENSION_NAME);
				m_deviceExtensions.Enable(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
				m_deviceExtensions.Enable(VK_KHR_MAINTENANCE3_EXTENSION_NAME);
				m_deviceExtensions.Enable(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
			}

			bool requires_ray_tracing = false;
			if (m_deviceExtensions.IsEnabled(VK_NV_RAY_TRACING_EXTENSION_NAME))
				requires_ray_tracing = true;

			bool extensionsSupported = m_deviceExtensions.CheckDeviceSupport(m_physical);

			m_swapChainSupportDetails = QuerySwapChainSupport(surface);
			if (m_swapChainSupportDetails.formats.size() > 0 && m_swapChainSupportDetails.presentModes.size() > 0 && extensionsSupported)
			{
				SE_LOG_INFO("(Vulkan) Vendor : " + std::string(get_vendor_name(m_device_properties.vendorID)));
				SE_LOG_INFO("(Vulkan) Name   : " + std::string(m_device_properties.deviceName));
				SE_LOG_INFO("(Vulkan) Type   : " + std::string(kDeviceTypes[m_device_properties.deviceType]));
				SE_LOG_INFO("(Vulkan) Driver : " + std::to_string(m_device_properties.driverVersion));

				if (requires_ray_tracing)
				{
					m_ray_tracing_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV;
					m_ray_tracing_properties.pNext = nullptr;
					m_ray_tracing_properties.maxRecursionDepth = 0;
					m_ray_tracing_properties.shaderGroupHandleSize = 0;

					VkPhysicalDeviceProperties2 properties;
					properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
					properties.pNext = &m_ray_tracing_properties;
					SE_ZERO_MEMORY(properties.properties);
					vkGetPhysicalDeviceProperties2(m_physical, &properties);
				}

				return FindQueueFamilies(surface, m_selected_queues);
			}
		}

		return false;
	}

	SwapChainSupportDetails PhysicalDevice::QuerySwapChainSupport(VkSurfaceKHR& surface)
	{
		SwapChainSupportDetails details;

		// Get surface capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical, surface, &details.capabilities);

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical, surface, &presentModeCount, details.presentModes.data());
		}

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical, surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical, surface, &formatCount, details.formats.data());
		}
		return details;
	}

	bool vkWrapper::PhysicalDevice::FindQueueFamilies(VkSurfaceKHR& surface, QueueInfos& infos)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queueFamilyCount, nullptr);
		SE_LOG_INFO("(Vulkan) Number of Queue families: " + std::to_string(queueFamilyCount));

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queueFamilyCount, queueFamilies.data());

		for (uint32_t i = 0; i < queueFamilyCount; i++)
		{
			VkQueueFlags bits = queueFamilies[i].queueFlags;

			SE_LOG_INFO("(Vulkan) Family " + std::to_string(i));
			SE_LOG_INFO("(Vulkan) Supported Bits: ");
			SE_LOG_INFO("(Vulkan) VK_QUEUE_GRAPHICS_BIT: " + std::to_string((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) > 0));
			SE_LOG_INFO("(Vulkan) VK_QUEUE_COMPUTE_BIT: " + std::to_string((queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) > 0));
			SE_LOG_INFO("(Vulkan) VK_QUEUE_TRANSFER_BIT: " + std::to_string((queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) > 0));
			SE_LOG_INFO("(Vulkan) Number of Queues: " + std::to_string(queueFamilies[i].queueCount));

			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physical, i, surface, &present_support);

			// Look for Presentation Queue
			if (present_support && infos.presentation_queue_index == -1)
				infos.presentation_queue_index = i;

			// Look for a graphics queue if the ideal one isn't found yet.
			if (infos.graphics_queue_quality != 3)
			{
				if (is_queue_compatible(bits, 1, 1, 1))
				{
					// Ideally, a queue that supports everything.
					infos.graphics_queue_index = i;
					infos.graphics_queue_quality = 3;
				}
				else if (is_queue_compatible(bits, 1, -1, -1))
				{
					// If not, a queue that supports at least graphics.
					infos.graphics_queue_index = i;
					infos.graphics_queue_quality = 1;
				}
			}

			// Look for a compute queue if the ideal one isn't found yet.
			if (infos.compute_queue_quality != 3)
			{
				if (is_queue_compatible(bits, 0, 1, 0))
				{
					// Ideally, a queue that only supports compute (for asynchronous compute).
					infos.compute_queue_index = i;
					infos.compute_queue_quality = 3;
				}
				else if (is_queue_compatible(bits, 0, 1, 1))
				{
					// Else, a queue that supports compute and transfer only (might allow asynchronous compute. Have to check).
					infos.compute_queue_index = i;
					infos.compute_queue_quality = 2;
				}
				else if (is_queue_compatible(bits, -1, 1, -1) && infos.compute_queue_quality == 0)
				{
					// If not, a queue that supports at least compute
					infos.compute_queue_index = i;
					infos.compute_queue_quality = 1;
				}
			}

			// Look for a Transfer queue if the ideal one isn't found yet.
			if (infos.transfer_queue_quality != 3)
			{
				if (is_queue_compatible(bits, 0, 0, 1))
				{
					// Ideally, a queue that only supports transfer (for DMA).
					infos.transfer_queue_index = i;
					infos.transfer_queue_quality = 3;
				}
				else if (is_queue_compatible(bits, 0, 1, 1))
				{
					// Else, a queue that supports compute and transfer only.
					infos.transfer_queue_index = i;
					infos.transfer_queue_quality = 2;
				}
				else if (is_queue_compatible(bits, -1, -1, 1) && infos.transfer_queue_quality == 0)
				{
					// If not, a queue that supports at least graphics
					infos.transfer_queue_index = i;
					infos.transfer_queue_quality = 1;
				}
			}
		}

		if (infos.presentation_queue_index == -1)
		{
			SE_LOG_INFO("(Vulkan) No Presentation Queue Found");
			return false;
		}

		if (infos.graphics_queue_quality == 0)

		{
			SE_LOG_INFO("(Vulkan) No Graphics Queue Found");
			return false;
		}

		if (infos.compute_queue_quality == 0 || infos.transfer_queue_quality == 0)
		{
			SE_LOG_INFO("(Vulkan) No Queues supporting Compute or Transfer found");
			return false;
		}

		VkDeviceQueueCreateInfo presentation_queue_info;
		SE_ZERO_MEMORY(presentation_queue_info);

		presentation_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		presentation_queue_info.queueFamilyIndex = infos.presentation_queue_index;
		presentation_queue_info.queueCount = 1;

		VkDeviceQueueCreateInfo graphics_queue_info;
		SE_ZERO_MEMORY(graphics_queue_info);

		graphics_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphics_queue_info.queueFamilyIndex = infos.graphics_queue_index;
		graphics_queue_info.queueCount = 1;

		VkDeviceQueueCreateInfo compute_queue_info;
		SE_ZERO_MEMORY(compute_queue_info);

		compute_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		compute_queue_info.queueFamilyIndex = infos.compute_queue_index;
		compute_queue_info.queueCount = 1;

		VkDeviceQueueCreateInfo transfer_queue_info;
		SE_ZERO_MEMORY(transfer_queue_info);

		transfer_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		transfer_queue_info.queueFamilyIndex = infos.transfer_queue_index;
		transfer_queue_info.queueCount = 1;

		infos.infos[infos.queue_count++] = presentation_queue_info;

		if (infos.graphics_queue_index != infos.presentation_queue_index)
			infos.infos[infos.queue_count++] = graphics_queue_info;

		if (infos.compute_queue_index != infos.presentation_queue_index && infos.compute_queue_index != infos.graphics_queue_index)
			infos.infos[infos.queue_count++] = compute_queue_info;

		if (infos.transfer_queue_index != infos.presentation_queue_index && infos.transfer_queue_index != infos.graphics_queue_index && infos.transfer_queue_index != infos.compute_queue_index)
			infos.infos[infos.queue_count++] = transfer_queue_info;

		return true;
	}




	//void PhysicalDevice::FindSupportDetails(VkSurfaceKHR& surface)
	//{
	//	m_indices = FindQueueFamilies(surface);
	//	m_swapChainSupport = QuerySwapChainSupport(surface);
	//	m_msaaSamples = GetMaxUsableSampleCount();
	//}

	VkSampleCountFlagBits PhysicalDevice::GetMaxUsableSampleCount()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_physical, &physicalDeviceProperties);

		VkSampleCountFlags counts =
			physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
		if ( counts & VK_SAMPLE_COUNT_64_BIT )
			return VK_SAMPLE_COUNT_64_BIT;
		if ( counts & VK_SAMPLE_COUNT_32_BIT )
			return VK_SAMPLE_COUNT_32_BIT;
		if ( counts & VK_SAMPLE_COUNT_16_BIT )
			return VK_SAMPLE_COUNT_16_BIT;
		if ( counts & VK_SAMPLE_COUNT_8_BIT )
			return VK_SAMPLE_COUNT_8_BIT;
		if ( counts & VK_SAMPLE_COUNT_4_BIT )
			return VK_SAMPLE_COUNT_4_BIT;
		if ( counts & VK_SAMPLE_COUNT_2_BIT )
			return VK_SAMPLE_COUNT_2_BIT;

		return VK_SAMPLE_COUNT_1_BIT;
	}

	VkFormat PhysicalDevice::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for ( VkFormat format : candidates )
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_physical, format, &props);

			if ( tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features )
				return format;
			else if ( tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features )
				return format;
		}

		throw std::runtime_error("failed to find supported format!");
	}

	VkFormat PhysicalDevice::FindDepthFormat()
	{
		return FindSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	bool PhysicalDevice::HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	

	

	

	//bool PhysicalDevice::CheckDeviceExtensionSupport()
	//{
	//	Extensions deviceExtensions = DefaultExtensions();
	//	return deviceExtensions.CheckDeviceSupport(m_physical);
	//}

	//int PhysicalDevice::RateDeviceSuitability()
	//{
	//	VkPhysicalDeviceProperties deviceProperties;
	//	vkGetPhysicalDeviceProperties(m_physical, &deviceProperties);

	//	VkPhysicalDeviceFeatures deviceFeatures;
	//	vkGetPhysicalDeviceFeatures(m_physical, &deviceFeatures);

	//	int score = 0;

	//	 Discrete GPUs have a significant performance advantage
	//	if ( deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
	//	{
	//		score += 1000;
	//	}

	//	 Maximum possible size of textures affects graphics quality
	//	score += deviceProperties.limits.maxImageDimension2D;

	//	 Application can't function without geometry shaders
	//	if ( !deviceFeatures.geometryShader )
	//	{
	//		return 0;
	//	}

	//	return score;
	//}

	uint32_t PhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_physical, &memProperties);

		for ( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ )
		{
			if ( (typeFilter & (1 << i))
				&& (memProperties.memoryTypes[i].propertyFlags & properties) == properties )
			{
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	Extensions PhysicalDevice::DefaultExtensions()
	{
		Extensions deviceExtensions;
		deviceExtensions.Enable(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		return deviceExtensions;
	}

	VkPhysicalDevice PhysicalDevice::Get()
	{
		return m_physical;
	}

#pragma endregion

#pragma region Instance
	//-------------------------------------------------------------------------
	// Instance
	//-------------------------------------------------------------------------
	void Instance::Init()
	{
		m_validationLayers = ValidationLayers::Default();
		if (enableValidationLayers && !m_validationLayers.CheckSupport())
		{
			throw std::runtime_error("validation layers requested, but not available!");
		}

		m_extensions = Extensions::Default();
		//extensions.Enable(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME); // устарело в 1.1??
		m_extensions.CheckSupport();

		VkApplicationInfo appInfo = {};
		appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName   = "Game";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName        = "Miko Engine";
		appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion         = VK_API_VERSION_1_1;
		appInfo.pNext              = nullptr;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = m_extensions.Count();
		createInfo.ppEnabledExtensionNames = m_extensions.Data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (enableValidationLayers)
		{
			SE_LOG_INFO("validationLayers is enabled");
			createInfo.enabledLayerCount = m_validationLayers.Count();
			createInfo.ppEnabledLayerNames = m_validationLayers.Data();

			DebugMessenger::PopulateCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) 
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Vulkan instance.");
			throw std::runtime_error("(Vulkan) Failed to create Vulkan instance.");
		}
	}

	void Instance::Close()
	{
		vkDestroyInstance(m_instance, nullptr);
	}

	Surface* Instance::CreateSurface(GLFWwindow* window)
	{
		Surface *s = new Surface(this, window);
		s->Init();
		return s;
	}

	void Instance::GetPhysicalDevices(std::vector<PhysicalDevice>& physicals)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
		if ( deviceCount == 0 )
		{
			SE_LOG_FATAL("(Vulkan) Failed to find GPUs with Vulkan support!");
			throw std::runtime_error("(Vulkan) Failed to find GPUs with Vulkan support!");
		}

		physicals.resize(deviceCount);

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

		for (uint32_t i = 0; i < deviceCount; ++i)
			physicals[i] = PhysicalDevice(devices[i]);
	}

	PhysicalDevice Instance::PickPhysicalDevice(Surface* surface, bool require_ray_tracing)
	{
		std::vector<PhysicalDevice> devices;
		GetPhysicalDevices(devices);

		// Try to find a discrete GPU...
		for ( auto& device : devices )
		{
			if ( device.IsDeviceSuitable(surface->Get(), VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, require_ray_tracing) )
			{
				return device;
			}
		}

		// ...If not, try to find an integrated GPU...
		for ( auto& device : devices )
		{
			if ( device.IsDeviceSuitable(surface->Get(), VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, require_ray_tracing) )
			{
				return device;
			}
		}

		SE_LOG_FATAL("(Vulkan) Failed to find a suitable GPU.");
		throw std::runtime_error("(Vulkan) Failed to find a suitable GPU.");
#if 0
				
		// Use an ordered map to automatically sort candidates by increasing score
		std::multimap<int, PhysicalDevice> candidates;
		for (auto& device : devices)
		{
			device.FindSupportDetails(surface->Get());
			int score = device.RateDeviceSuitability();
			candidates.insert(std::make_pair(score, device));
		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0)
		{
			return candidates.rbegin()->second;
		}
		else
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}
#endif
	}

	VkInstance& Instance::Get()
	{
		return m_instance;
	}

	DebugMessenger* Instance::CreateDebugMessenger()
	{
		if (!enableValidationLayers) return nullptr;

		DebugMessenger* debug = new DebugMessenger(this);
		debug->Init();
		return debug;
	}

#pragma endregion

#pragma region DebugMessenger
	//-------------------------------------------------------------------------
	// DebugMessenger
	//-------------------------------------------------------------------------
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		else 
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
			func(instance, debugMessenger, pAllocator);
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger::DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT /*messageSeverity*/,
		VkDebugUtilsMessageTypeFlagsEXT /*messageType*/,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* /*pUserData*/) 
	{
		SE_LOG_ERROR("(Vulkan) Validation Layer: " + std::string(pCallbackData->pMessage));
		return VK_FALSE;
	}

	DebugMessenger::DebugMessenger(Instance *vkinstance)
		: VkInstanceObject(vkinstance)
	{
	}

	DebugMessenger::~DebugMessenger()
	{
		Close();
	}

	void DebugMessenger::Init()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(m_parent->Get(), &createInfo, nullptr, &m_messenger) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Vulkan debug messenger.");
		}
	}

	void DebugMessenger::Close()
	{
		DestroyDebugUtilsMessengerEXT(m_parent->Get(), m_messenger, nullptr);
	}

	void DebugMessenger::PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info)
	{
		info = {};
		info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		info.pfnUserCallback = DebugCallback;
		info.pUserData = nullptr; // Optional
	}

#pragma endregion

#pragma region Surface
	//-------------------------------------------------------------------------
	// Surface
	//-------------------------------------------------------------------------

	Surface::Surface(Instance *vkinstance, GLFWwindow* window)
		: VkInstanceObject(vkinstance)
		, m_window(window)
	{
	}

	Surface::~Surface()
	{
		Close();
	}

	void Surface::Init()
	{
		if (glfwCreateWindowSurface(m_parent->Get(), m_window, nullptr, &m_surface) != VK_SUCCESS)
		{
			SE_LOG_FATAL("(Vulkan) Failed to create Vulkan surface.");
			throw std::runtime_error("(Vulkan) Failed to create Vulkan surface.");
		}
	}

	void Surface::Close()
	{
		vkDestroySurfaceKHR(m_parent->Get(), m_surface, nullptr);
	}

	VkSurfaceKHR& Surface::Get()
	{
		return m_surface;
	}

	void Surface::GetSize(uint32_t& width, uint32_t& height)
	{
		int w, h;
		glfwGetFramebufferSize(m_window, &w, &h);
		width = w; height = h;
	}
#pragma endregion

#pragma region SwapChain
	//-------------------------------------------------------------------------
	// SwapChain
	//-------------------------------------------------------------------------
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		VkFormat expectedFormat = VK_FORMAT_R8G8B8A8_UNORM;
		VkColorSpaceKHR expectedColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

		for ( const auto& candidate : availableFormats )
		{
			if ( candidate.format == expectedFormat &&
				candidate.colorSpace == expectedColorSpace )
				return candidate;
		}

		// default
		return availableFormats[0];
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		VkPresentModeKHR expected = VK_PRESENT_MODE_MAILBOX_KHR;

		for ( const auto& candidate : availablePresentModes )
		{
			if ( candidate == expected ) return candidate;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	uint32_t clamp(uint32_t v, uint32_t mn, uint32_t mx)
	{
		return std::max(mn, std::min(mx, v));
	}

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height)
	{
		VkExtent2D actualExtent = { width,height };

		if ( capabilities.currentExtent.width != UINT32_MAX )
		{
			actualExtent = capabilities.currentExtent;
		}
		else
		{
			const VkExtent2D& minExtent = capabilities.minImageExtent;
			const VkExtent2D& maxExtent = capabilities.maxImageExtent;
			actualExtent.width = clamp(actualExtent.width, minExtent.width, maxExtent.width);
			actualExtent.height = clamp(actualExtent.height, minExtent.height, maxExtent.height);
		}

		return actualExtent;
	}

	SwapChain::SwapChain(LogicalDevice* device, Surface* surface)
		: VkDeviceObject(device)
		, m_surface(surface)
	{
	}

	SwapChain::~SwapChain()
	{
		Close();
	}

	void SwapChain::Init()
	{
		PhysicalDevice* physical = m_parent->GetPhysical();
		SwapChainSupportDetails swapChainSupport = physical->m_swapChainSupportDetails;

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

		uint32_t width, height;
		m_surface->GetSize(width, height);
		m_extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if ( swapChainSupport.capabilities.maxImageCount > 0
			&& imageCount > swapChainSupport.capabilities.maxImageCount )
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_surface->Get();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = physical->m_indices;
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if ( indices.graphicsFamily != indices.presentFamily )
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if ( vkCreateSwapchainKHR(m_parent->Get(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to create swap chain!");
		}

		// retrieve handles to the swap chain vkImage(s)
		vkGetSwapchainImagesKHR(m_parent->Get(), m_swapChain, &imageCount, nullptr);
		m_images.resize(imageCount);
		vkGetSwapchainImagesKHR(m_parent->Get(), m_swapChain, &imageCount, m_images.data());
		m_imageFormat = surfaceFormat.format;

		std::cout << "swapchain image count = " << imageCount << std::endl;

	}

	void SwapChain::Close()
	{
		vkDestroySwapchainKHR(m_parent->Get(), m_swapChain, nullptr);
	}

	uint32_t SwapChain::AcquireNextImage(VkSemaphore semaphore)
	{
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(m_parent->Get(), m_swapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);
		if ( result == VK_ERROR_OUT_OF_DATE_KHR )
		{
			throw std::runtime_error("need to recreate swapchain");
		}
		else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		return imageIndex;
	}

#pragma endregion

#pragma region Queue
	//-------------------------------------------------------------------------
	// Queue
	//-------------------------------------------------------------------------
	Queue::Queue(LogicalDevice* device, uint32_t familyIndex)
		: VkDeviceObject(device)
		, m_familyIndex(familyIndex)
	{
	}

	Queue::~Queue()
	{
		Close();
	}

	void Queue::Init()
	{
		vkGetDeviceQueue(m_parent->Get(), m_familyIndex, 0, &m_queue);
	}

	void Queue::Close()
	{
		m_queue = VK_NULL_HANDLE;
	}
	void Queue::Wait()
	{
		vkQueueWaitIdle(m_queue);
	}

	void Queue::Present(VkSemaphore semaphore, SwapChain* swapchain, uint32_t imageIndex)
	{
		VkSemaphore signalSemaphores[] = { semaphore };

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapchain->Get() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional

		VkResult result = vkQueuePresentKHR(m_queue, &presentInfo);

		if ( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR )
		{
			throw std::runtime_error("framebuffer need to be resized");
		}
		else if ( result != VK_SUCCESS )
		{
			throw std::runtime_error("failed to present swap chain image!");
		}
	}
#pragma endregion

#pragma region LogicalDevice
	//-------------------------------------------------------------------------
	// LogicalDevice
	//-------------------------------------------------------------------------

	LogicalDevice::LogicalDevice(PhysicalDevice* physical)
		: m_device(VK_NULL_HANDLE)
		, m_physicalDevice(physical)
		, m_msaaEnabled(true)
	{
	}

	LogicalDevice::~LogicalDevice()
	{
		Close();
	}

	void LogicalDevice::Init()
	{
		QueueFamilyIndices indices = m_physicalDevice->m_indices;

		float queuePriority = 1.0f;
		std::set<uint32_t> queueFamilies = indices.GetQueueFamilySet();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for ( uint32_t queueFamily : queueFamilies )
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// enable features
		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.sampleRateShading = m_msaaEnabled ? VK_TRUE : VK_FALSE;


		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = queueCreateInfos.size();
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;


		m_extensions = PhysicalDevice::DefaultExtensions();
		createInfo.enabledExtensionCount = m_extensions.Count();
		createInfo.ppEnabledExtensionNames = m_extensions.Data();

		if ( enableValidationLayers )
		{
			m_validationLayers = ValidationLayers::Default();
			createInfo.enabledLayerCount = m_validationLayers.Count();
			createInfo.ppEnabledLayerNames = m_validationLayers.Data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if ( vkCreateDevice(m_physicalDevice->Get(), &createInfo, nullptr, &m_device) != VK_SUCCESS )
		{
			throw std::runtime_error("failed to create logical device!");
		}

		// get device queues
		m_graphicsQueue = new Queue(this, indices.graphicsFamily.value());
		m_presentQueue = new Queue(this, indices.presentFamily.value());
		m_transferQueue = new Queue(this, indices.transferFamily.value());
		m_graphicsQueue->Init();
		m_presentQueue->Init();
		m_transferQueue->Init();
	}

	void LogicalDevice::Close()
	{
		vkDestroyDevice(m_device, nullptr);
	}

	VkDevice& LogicalDevice::Get()
	{
		return m_device;
	}

	PhysicalDevice* LogicalDevice::GetPhysical()
	{
		return m_physicalDevice;
	}

#pragma endregion

#pragma region Synchronization
	//-------------------------------------------------------------------------
	// Synchronization
	//-------------------------------------------------------------------------
	Synchronization::Synchronization(LogicalDevice* device, uint32_t framesInFlight)
		: VkDeviceObject(device)
		, m_framesInFlight(framesInFlight)
		, m_currentFrame(0)
	{
	}

	Synchronization::~Synchronization()
	{
		Close();
	}

	void Synchronization::Init()
	{
		m_imgAvailableSemaphores.resize(m_framesInFlight);
		m_renderFinishedSemaphores.resize(m_framesInFlight);
		m_inFlightFences.resize(m_framesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for ( size_t i = 0; i < m_framesInFlight; i++ )
		{
			if ( vkCreateSemaphore(m_parent->Get(), &semaphoreInfo, nullptr, &m_imgAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(m_parent->Get(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(m_parent->Get(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS )
			{
				throw std::runtime_error("failed to create synchronization objects for a frame!");
			}
		}
	}

	void Synchronization::Close()
	{
		for ( size_t i = 0; i < m_framesInFlight; i++ )
		{
			vkDestroySemaphore(m_parent->Get(), m_imgAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(m_parent->Get(), m_renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(m_parent->Get(), m_inFlightFences[i], nullptr);
		}
		m_imgAvailableSemaphores.clear();
		m_renderFinishedSemaphores.clear();
		m_inFlightFences.clear();
	}

	void Synchronization::WaitForFence()
	{
		vkWaitForFences(m_parent->Get(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
	}

	void Synchronization::ResetFence()
	{
		vkResetFences(m_parent->Get(), 1, &m_inFlightFences[m_currentFrame]);
	}

	void Synchronization::NextFrame()
	{
		m_currentFrame = (m_currentFrame + 1) % m_framesInFlight;
	}

	VkSemaphore& Synchronization::GetImgAvailableSemaphore()
	{
		return m_imgAvailableSemaphores[m_currentFrame];
	}

	VkSemaphore& Synchronization::GetRenderFinishedSemaphore()
	{
		return m_renderFinishedSemaphores[m_currentFrame];
	}

	VkFence& Synchronization::GetFence()
	{
		return m_inFlightFences[m_currentFrame];
	}
#pragma endregion



} // namespace vkWrapper
#endif // SE_VULKAN
#include "stdafx.h"
#if SE_VULKAN
#include "VulkanWrapper.h"
#include "Core/Logger.h"
//-----------------------------------------------------------------------------
namespace vkWrapper
{
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

	//-------------------------------------------------------------------------
	// PhysicalDevice
	//-------------------------------------------------------------------------

	PhysicalDevice::PhysicalDevice(VkPhysicalDevice& physical)
		: m_physical(physical)
	{
	}

	void PhysicalDevice::FindSupportDetails(VkSurfaceKHR& surface)
	{
		m_indices = FindQueueFamilies(surface);
		m_swapChainSupport = QuerySwapChainSupport(surface);
		m_msaaSamples = GetMaxUsableSampleCount();
	}

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

	QueueFamilyIndices PhysicalDevice::FindQueueFamilies(VkSurfaceKHR& surface)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queueFamilyCount, queueFamilies.data());
		int i = 0;

		for ( const auto& queueFamily : queueFamilies )
		{
			if ( queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT )
			{
				indices.graphicsFamily = i;
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physical, i, surface, &presentSupport);
			if ( queueFamily.queueCount > 0 && presentSupport )
			{
				indices.presentFamily = i;
			}

			if ( queueFamily.queueCount > 0 &&
				!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
				(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) )
			{
				indices.transferFamily = i;
			}

			if ( indices.IsComplete() )
			{
				break;
			}

			i++;
		}
		return indices;
	}

	SwapChainSupportDetails PhysicalDevice::QuerySwapChainSupport(VkSurfaceKHR& surface)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical, surface, &formatCount, nullptr);

		if ( formatCount != 0 )
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical, surface, &presentModeCount, nullptr);

		if ( presentModeCount != 0 )
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	bool PhysicalDevice::IsDeviceSuitable(VkSurfaceKHR& surface)
	{
		QueueFamilyIndices indices = FindQueueFamilies(surface);

		bool extensionsSupported = CheckDeviceExtensionSupport();

		bool swapChainAdequate = false;
		if ( extensionsSupported )
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(m_physical, &supportedFeatures);

		return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool PhysicalDevice::CheckDeviceExtensionSupport()
	{
		Extensions deviceExtensions = DefaultExtensions();
		return deviceExtensions.CheckDeviceSupport(m_physical);
	}

	int PhysicalDevice::RateDeviceSuitability()
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(m_physical, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(m_physical, &deviceFeatures);

		int score = 0;

		// Discrete GPUs have a significant performance advantage
		if ( deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
		{
			score += 1000;
		}

		// Maximum possible size of textures affects graphics quality
		score += deviceProperties.limits.maxImageDimension2D;

		// Application can't function without geometry shaders
		if ( !deviceFeatures.geometryShader )
		{
			return 0;
		}

		return score;
	}

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

		auto extensions = Extensions::Default();
		//extensions.Enable(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME); // устарело в 1.1??
		extensions.CheckSupport();

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
		createInfo.enabledExtensionCount = extensions.Count();
		createInfo.ppEnabledExtensionNames = extensions.Data();

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
		Surface* s = new Surface(this, window);
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

	PhysicalDevice Instance::PickPhysicalDevice(Surface* surface)
	{
		std::vector<PhysicalDevice> devices;
		GetPhysicalDevices(devices);
				
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

} // namespace vkWrapper
#endif // SE_VULKAN
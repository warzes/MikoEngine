#pragma once

namespace VulkanRhi
{
	// Vulkan runtime linking for creating and managing the Vulkan instance ("VkInstance")
	class VulkanRuntimeLinking final
	{
	public:
		inline VulkanRuntimeLinking(VulkanRhi& vulkanRhi, bool enableValidation) 
			: mVulkanRhi(vulkanRhi)
			, mValidationEnabled(enableValidation)
			, mVkInstance(VK_NULL_HANDLE)
			, mVkDebugReportCallbackEXT(VK_NULL_HANDLE)
			, mInstanceLevelFunctionsRegistered(false)
			, mInitialized(false)
		{
		}

		~VulkanRuntimeLinking()
		{
			if ( VK_NULL_HANDLE != mVkDebugReportCallbackEXT )
				vkDestroyDebugReportCallbackEXT(mVkInstance, mVkDebugReportCallbackEXT, mVulkanRhi.getVkAllocationCallbacks());

			if ( VK_NULL_HANDLE != mVkInstance )
				vkDestroyInstance(mVkInstance, mVulkanRhi.getVkAllocationCallbacks());
		}

		[[nodiscard]] inline bool isValidationEnabled() const
		{
			return mValidationEnabled;
		}

		[[nodiscard]] bool isVulkanAvaiable()
		{
			if ( !mInitialized )
			{
				mInitialized = true;

				// Create the Vulkan instance
				const VkResult vkResult = createVulkanInstance(mValidationEnabled);
				if ( VK_SUCCESS == vkResult )
				{
					// Load instance based instance level Vulkan function pointers
					mInstanceLevelFunctionsRegistered = loadInstanceLevelVulkanEntryPoints();

					// Setup debug callback
					if ( mInstanceLevelFunctionsRegistered && mValidationEnabled )
					{
						setupDebugCallback();
					}
				}
				else
				{
					// Error!
					RHI_LOG(CRITICAL, "Failed to create the Vulkan instance");
				}
			}

			// Entry points successfully registered?
			return ( (VK_NULL_HANDLE != mVkInstance) && mInstanceLevelFunctionsRegistered);
		}

		[[nodiscard]] inline VkInstance getVkInstance() const
		{
			return mVkInstance;
		}

	private:
		explicit VulkanRuntimeLinking(const VulkanRuntimeLinking&) = delete;
		VulkanRuntimeLinking& operator=(const VulkanRuntimeLinking&) = delete;

		[[nodiscard]] VkResult createVulkanInstance(bool enableValidation)
		{
			// Enable surface extensions depending on OS
			std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
#ifdef VK_USE_PLATFORM_WIN32_KHR
			enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined VK_USE_PLATFORM_ANDROID_KHR
			#warning "TODO(co) Not tested"
				enabledExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined VK_USE_PLATFORM_XLIB_KHR || defined VK_USE_PLATFORM_WAYLAND_KHR
#if defined VK_USE_PLATFORM_XLIB_KHR
			enabledExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif
#if defined VK_USE_PLATFORM_WAYLAND_KHR
			enabledExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif
#elif defined VK_USE_PLATFORM_XCB_KHR
			#warning "TODO(co) Not tested"
				enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#else
#error "Unsupported platform"
#endif
			if ( enableValidation )
				enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

			{ // Ensure the extensions we need are supported
				uint32_t propertyCount = 0;
				if ( (vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr) != VK_SUCCESS) || (0 == propertyCount) )
				{
					RHI_LOG(CRITICAL, "Failed to enumerate Vulkan instance extension properties");
					return VK_ERROR_EXTENSION_NOT_PRESENT;
				}
				::detail::VkExtensionPropertiesVector vkExtensionPropertiesVector(propertyCount);
				if ( vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, &vkExtensionPropertiesVector[0]) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to enumerate Vulkan instance extension properties");
					return VK_ERROR_EXTENSION_NOT_PRESENT;
				}
				for ( const char* enabledExtension : enabledExtensions )
				{
					if ( !::detail::isExtensionAvailable(enabledExtension, vkExtensionPropertiesVector) )
					{
						RHI_LOG(CRITICAL, "Couldn't find Vulkan instance extension named \"%s\"", enabledExtension);
						return VK_ERROR_EXTENSION_NOT_PRESENT;
					}
				}
			}

			// TODO(co) Make it possible for the user to provide application related information?
			static constexpr VkApplicationInfo vkApplicationInfo =
			{
				VK_STRUCTURE_TYPE_APPLICATION_INFO,	// sType (VkStructureType)
				nullptr,							// pNext (const void*)
				"Miko Application",					// pApplicationName (const char*)
				VK_MAKE_VERSION(0, 0, 0),			// applicationVersion (uint32_t)
				"MikoEngine",						// pEngineName (const char*)
				VK_MAKE_VERSION(0, 0, 0),			// engineVersion (uint32_t)
				VK_API_VERSION_1_2					// apiVersion (uint32_t)
			};

			const VkInstanceCreateInfo vkInstanceCreateInfo =
			{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,							// sType (VkStructureType)
				nullptr,														// pNext (const void*)
				0,																// flags (VkInstanceCreateFlags)
				&vkApplicationInfo,												// pApplicationInfo (const VkApplicationInfo*)
				enableValidation ? ::detail::NUMBER_OF_VALIDATION_LAYERS : 0,	// enabledLayerCount (uint32_t)
				enableValidation ? ::detail::VALIDATION_LAYER_NAMES : nullptr,	// ppEnabledLayerNames (const char* const*)
				static_cast<uint32_t>(enabledExtensions.size()),				// enabledExtensionCount (uint32_t)
				enabledExtensions.data()										// ppEnabledExtensionNames (const char* const*)
			};
			VkResult vkResult = vkCreateInstance(&vkInstanceCreateInfo, mVulkanRhi.getVkAllocationCallbacks(), &mVkInstance);
			if ( VK_ERROR_LAYER_NOT_PRESENT == vkResult && enableValidation )
			{
				// Error! Since the show must go on, try creating a Vulkan instance without validation enabled...
				RHI_LOG(WARNING, "Failed to create the Vulkan instance with validation enabled, layer is not present. Install e.g. the LunarG Vulkan SDK and see e.g. https://vulkan.lunarg.com/doc/view/1.0.51.0/windows/layers.html .");
				mValidationEnabled = false;
				vkResult = createVulkanInstance(mValidationEnabled);
			}

			// Done
			return vkResult;
		}

		[[nodiscard]] bool loadInstanceLevelVulkanEntryPoints() const
		{
			bool result = true;	// Success by default

			// Define a helper macro
#define IMPORT_FUNC(funcName) \
				if (result) \
				{ \
					funcName = (PFN_##funcName)(vkGetInstanceProcAddr(mVkInstance, #funcName));	 \
					if (nullptr == funcName) \
					{ \
						RHI_LOG(CRITICAL, "Failed to load instance based Vulkan function pointer \"%s\"", #funcName); \
						result = false; \
					} \
				}

				if ( mValidationEnabled )
				{
					// "VK_EXT_debug_report"-extension
					IMPORT_FUNC(vkCreateDebugReportCallbackEXT);
					IMPORT_FUNC(vkDestroyDebugReportCallbackEXT);
				}
#undef IMPORT_FUNC

				// Done
				return result;
		}

		void setupDebugCallback()
		{
			// Sanity check
			RHI_ASSERT(mValidationEnabled, "Do only call this Vulkan method if validation is enabled");

			// The report flags determine what type of messages for the layers will be displayed
			// -> Use "VK_DEBUG_REPORT_FLAG_BITS_MAX_ENUM_EXT" to get everything, quite verbose
			static constexpr VkDebugReportFlagsEXT vkDebugReportFlagsEXT = (VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT);

			// Setup debug callback
			const VkDebugReportCallbackCreateInfoEXT vkDebugReportCallbackCreateInfoEXT =
			{
				VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,	// sType (VkStructureType)
				nullptr,													// pNext (const void*)
				vkDebugReportFlagsEXT,										// flags (VkDebugReportFlagsEXT)
				::detail::debugReportCallback,								// pfnCallback (PFN_vkDebugReportCallbackEXT)
				nullptr			// pUserData (void*)
			};
			if ( vkCreateDebugReportCallbackEXT(mVkInstance, &vkDebugReportCallbackCreateInfoEXT, mVulkanRhi.getVkAllocationCallbacks(), &mVkDebugReportCallbackEXT) != VK_SUCCESS )
			{
				RHI_LOG(WARNING, "Failed to create the Vulkan debug report callback");
			}
		}

	private:
		VulkanRhi&				 mVulkanRhi;						// Owner Vulkan RHI instance
		bool					 mValidationEnabled;				// Validation enabled?
		VkInstance				 mVkInstance;						// Vulkan instance, stores all per-application states
		VkDebugReportCallbackEXT mVkDebugReportCallbackEXT;			// Vulkan debug report callback, can be a null handle
		bool					 mInstanceLevelFunctionsRegistered;	// Instance level Vulkan function pointers registered?
		bool					 mInitialized;						// Already initialized?
	};
} // namespace VulkanRhi
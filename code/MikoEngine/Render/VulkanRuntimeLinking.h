#pragma once

namespace VulkanRhi
{
	/**
	*  @brief
	*    Vulkan runtime linking for creating and managing the Vulkan instance ("VkInstance")
	*/
	class VulkanRuntimeLinking final
	{
	public:
		inline VulkanRuntimeLinking(VulkanRhi& vulkanRhi, bool enableValidation) :
			mVulkanRhi(vulkanRhi),
			mValidationEnabled(enableValidation),
			mVulkanSharedLibrary(nullptr),
			mEntryPointsRegistered(false),
			mVkInstance(VK_NULL_HANDLE),
			mVkDebugReportCallbackEXT(VK_NULL_HANDLE),
			mInstanceLevelFunctionsRegistered(false),
			mInitialized(false)
		{
		}

		~VulkanRuntimeLinking()
		{
			// Destroy the Vulkan debug report callback
			if ( VK_NULL_HANDLE != mVkDebugReportCallbackEXT )
			{
				vkDestroyDebugReportCallbackEXT(mVkInstance, mVkDebugReportCallbackEXT, mVulkanRhi.getVkAllocationCallbacks());
			}

			// Destroy the Vulkan instance
			if ( VK_NULL_HANDLE != mVkInstance )
			{
				vkDestroyInstance(mVkInstance, mVulkanRhi.getVkAllocationCallbacks());
			}

			// Destroy the shared library instances
#if SE_PLATFORM_WINDOWS
			if ( nullptr != mVulkanSharedLibrary )
			{
				::FreeLibrary(static_cast<HMODULE>(mVulkanSharedLibrary));
			}
#elif defined LINUX
			if ( nullptr != mVulkanSharedLibrary )
			{
				::dlclose(mVulkanSharedLibrary);
			}
#else
#error "Unsupported platform"
#endif
		}

		/**
		*  @brief
		*    Return whether or not validation is enabled
		*
		*  @return
		*    "true" if validation is enabled, else "false"
		*/
		[[nodiscard]] inline bool isValidationEnabled() const
		{
			return mValidationEnabled;
		}

		/**
		*  @brief
		*    Return whether or not Vulkan is available
		*
		*  @return
		*    "true" if Vulkan is available, else "false"
		*/
		[[nodiscard]] bool isVulkanAvaiable()
		{
			// Already initialized?
			if ( !mInitialized )
			{
				// We're now initialized
				mInitialized = true;

				// Load the shared libraries
				if ( loadSharedLibraries() )
				{
					// Load the global level Vulkan function entry points
					mEntryPointsRegistered = loadGlobalLevelVulkanEntryPoints();
					if ( mEntryPointsRegistered )
					{
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
							RHI_LOG(CRITICAL, "Failed to create the Vulkan instance")
						}
					}
				}
			}

			// Entry points successfully registered?
			return (mEntryPointsRegistered && (VK_NULL_HANDLE != mVkInstance) && mInstanceLevelFunctionsRegistered);
		}

		/**
		*  @brief
		*    Return the Vulkan instance
		*
		*  @return
		*    Vulkan instance
		*/
		[[nodiscard]] inline VkInstance getVkInstance() const
		{
			return mVkInstance;
		}

		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VulkanRuntimeLinking(const VulkanRuntimeLinking&) = delete;
		VulkanRuntimeLinking& operator =(const VulkanRuntimeLinking&) = delete;

		/**
		*  @brief
		*    Load the shared libraries
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] bool loadSharedLibraries()
		{
//			// Load the shared library
//#if SE_PLATFORM_WINDOWS
//			mVulkanSharedLibrary = ::LoadLibraryExA("vulkan-1.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
//			if ( nullptr == mVulkanSharedLibrary )
//			{
//				RHI_LOG(CRITICAL, "Failed to load in the shared Vulkan library \"vulkan-1.dll\"")
//			}
//#elif defined LINUX
//			mVulkanSharedLibrary = ::dlopen("libvulkan.so", RTLD_NOW);
//			if ( nullptr == mVulkanSharedLibrary )
//			{
//				RHI_LOG(CRITICAL, "Failed to load in the shared Vulkan library \"libvulkan-1.so\"")
//			}
//#else
//#error "Unsupported platform"
//#endif
//
//// Done
//			return (nullptr != mVulkanSharedLibrary);
			return true;
		}

		/**
		*  @brief
		*    Load the global level Vulkan function entry points
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] bool loadGlobalLevelVulkanEntryPoints() const
		{
			bool result = true;	// Success by default

//			// Define a helper macro
//#if SE_PLATFORM_WINDOWS
//#define IMPORT_FUNC(funcName)																																			\
//					if (result)																																							\
//					{																																									\
//						void* symbol = ::GetProcAddress(static_cast<HMODULE>(mVulkanSharedLibrary), #funcName);																			\
//						if (nullptr != symbol)																																			\
//						{																																								\
//							*(reinterpret_cast<void**>(&(funcName))) = symbol;																											\
//						}																																								\
//						else																																							\
//						{																																								\
//							wchar_t moduleFilename[MAX_PATH];																															\
//							moduleFilename[0] = '\0';																																	\
//							::GetModuleFileNameW(static_cast<HMODULE>(mVulkanSharedLibrary), moduleFilename, MAX_PATH);																	\
//							RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the shared Vulkan library \"%s\"", #funcName, moduleFilename)	\
//							result = false;																																				\
//						}																																								\
//					}
//#elif defined(__ANDROID__)
//#define IMPORT_FUNC(funcName)																																				\
//					if (result)																																								\
//					{																																										\
//						void* symbol = ::dlsym(mVulkanSharedLibrary, #funcName);																											\
//						if (nullptr != symbol)																																				\
//						{																																									\
//							*(reinterpret_cast<void**>(&(funcName))) = symbol;																												\
//						}																																									\
//						else																																								\
//						{																																									\
//							const char* libraryName = "unknown";																															\
//							RHI_LOG(CRITICAL, "Failed to locate the Vulkan entry point \"%s\" within the Vulkan shared library \"%s\"", #funcName, libraryName)	\
//							result = false;																																					\
//						}																																									\
//					}
//#elif defined LINUX
//#define IMPORT_FUNC(funcName)																																		\
//					if (result)																																						\
//					{																																								\
//						void* symbol = ::dlsym(mVulkanSharedLibrary, #funcName);																									\
//						if (nullptr != symbol)																																		\
//						{																																							\
//							*(reinterpret_cast<void**>(&(funcName))) = symbol;																										\
//						}																																							\
//						else																																						\
//						{																																							\
//							link_map *linkMap = nullptr;																															\
//							const char* libraryName = "unknown";																													\
//							if (dlinfo(mVulkanSharedLibrary, RTLD_DI_LINKMAP, &linkMap))																							\
//							{																																						\
//								libraryName = linkMap->l_name;																														\
//							}																																						\
//							RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the shared Vulkan library \"%s\"", #funcName, libraryName)	\
//							result = false;																																			\
//						}																																							\
//					}
//#else
//#error "Unsupported platform"
//#endif
//
//// Load the Vulkan global level function entry points
//			IMPORT_FUNC(vkGetInstanceProcAddr)
//				IMPORT_FUNC(vkGetDeviceProcAddr)
//				IMPORT_FUNC(vkEnumerateInstanceExtensionProperties)
//				IMPORT_FUNC(vkEnumerateInstanceLayerProperties)
//				IMPORT_FUNC(vkCreateInstance)

				// Undefine the helper macro
#undef IMPORT_FUNC

// Done
return result;
		}

		/**
		*  @brief
		*    Create the Vulkan instance
		*
		*  @param[in] enableValidation
		*    Enable validation layer? (don't do this for shipped products)
		*
		*  @return
		*    Vulkan instance creation result
		*/
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
			{
				enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			}

			{ // Ensure the extensions we need are supported
				uint32_t propertyCount = 0;
				if ( (vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr) != VK_SUCCESS) || (0 == propertyCount) )
				{
					RHI_LOG(CRITICAL, "Failed to enumerate Vulkan instance extension properties")
						return VK_ERROR_EXTENSION_NOT_PRESENT;
				}
				::detail::VkExtensionPropertiesVector vkExtensionPropertiesVector(propertyCount);
				if ( vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, &vkExtensionPropertiesVector[0]) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to enumerate Vulkan instance extension properties")
						return VK_ERROR_EXTENSION_NOT_PRESENT;
				}
				for ( const char* enabledExtension : enabledExtensions )
				{
					if ( !::detail::isExtensionAvailable(enabledExtension, vkExtensionPropertiesVector) )
					{
						RHI_LOG(CRITICAL, "Couldn't find Vulkan instance extension named \"%s\"", enabledExtension)
							return VK_ERROR_EXTENSION_NOT_PRESENT;
					}
				}
			}

			// TODO(co) Make it possible for the user to provide application related information?
			static constexpr VkApplicationInfo vkApplicationInfo =
			{
				VK_STRUCTURE_TYPE_APPLICATION_INFO,	// sType (VkStructureType)
				nullptr,							// pNext (const void*)
				"Unrimp Application",				// pApplicationName (const char*)
				VK_MAKE_VERSION(0, 0, 0),			// applicationVersion (uint32_t)
				"Unrimp",							// pEngineName (const char*)
				VK_MAKE_VERSION(0, 0, 0),			// engineVersion (uint32_t)
				VK_API_VERSION_1_0					// apiVersion (uint32_t)
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
				RHI_LOG(WARNING, "Failed to create the Vulkan instance with validation enabled, layer is not present. Install e.g. the LunarG Vulkan SDK and see e.g. https://vulkan.lunarg.com/doc/view/1.0.51.0/windows/layers.html .")
					mValidationEnabled = false;
				vkResult = createVulkanInstance(mValidationEnabled);
			}

			// Done
			return vkResult;
		}

		/**
		*  @brief
		*    Load the instance level Vulkan function entry points
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
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
						RHI_LOG(CRITICAL, "Failed to load instance based Vulkan function pointer \"%s\"", #funcName) \
						result = false; \
					} \
				}

//			// Load the Vulkan instance level function entry points
//				IMPORT_FUNC(vkDestroyInstance)
//				IMPORT_FUNC(vkEnumeratePhysicalDevices)
//				IMPORT_FUNC(vkEnumerateDeviceLayerProperties)
//				IMPORT_FUNC(vkEnumerateDeviceExtensionProperties)
//				IMPORT_FUNC(vkGetPhysicalDeviceQueueFamilyProperties)
//				IMPORT_FUNC(vkGetPhysicalDeviceFeatures)
//				IMPORT_FUNC(vkGetPhysicalDeviceFormatProperties)
//				IMPORT_FUNC(vkGetPhysicalDeviceMemoryProperties)
//				IMPORT_FUNC(vkGetPhysicalDeviceProperties)
//				IMPORT_FUNC(vkCreateDevice)
				if ( mValidationEnabled )
				{
					// "VK_EXT_debug_report"-extension
					IMPORT_FUNC(vkCreateDebugReportCallbackEXT);
					IMPORT_FUNC(vkDestroyDebugReportCallbackEXT);
				}
//			// "VK_KHR_surface"-extension
//			IMPORT_FUNC(vkDestroySurfaceKHR)
//				IMPORT_FUNC(vkGetPhysicalDeviceSurfaceSupportKHR)
//				IMPORT_FUNC(vkGetPhysicalDeviceSurfaceFormatsKHR)
//				IMPORT_FUNC(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
//				IMPORT_FUNC(vkGetPhysicalDeviceSurfacePresentModesKHR)
//#ifdef VK_USE_PLATFORM_WIN32_KHR
//				// "VK_KHR_win32_surface"-extension
//				IMPORT_FUNC(vkCreateWin32SurfaceKHR)
//#elif defined VK_USE_PLATFORM_ANDROID_KHR
//				// "VK_KHR_android_surface"-extension
//				#warning "TODO(co) Not tested"
//				IMPORT_FUNC(vkCreateAndroidSurfaceKHR)
//#elif defined VK_USE_PLATFORM_XLIB_KHR || defined VK_USE_PLATFORM_WAYLAND_KHR
//#if defined VK_USE_PLATFORM_XLIB_KHR
//				// "VK_KHR_xlib_surface"-extension
//				IMPORT_FUNC(vkCreateXlibSurfaceKHR)
//#endif
//#if defined VK_USE_PLATFORM_WAYLAND_KHR
//				// "VK_KHR_wayland_surface"-extension
//				IMPORT_FUNC(vkCreateWaylandSurfaceKHR)
//#endif
//#elif defined VK_USE_PLATFORM_XCB_KHR
//				// "VK_KHR_xcb_surface"-extension
//				#warning "TODO(co) Not tested"
//				IMPORT_FUNC(vkCreateXcbSurfaceKHR)
//#else
//#error "Unsupported platform"
//#endif
//
				// Undefine the helper macro
#undef IMPORT_FUNC

				// Done
				return result;
		}

		/**
		*  @brief
		*    Setup debug callback
		*/
		void setupDebugCallback()
		{
			// Sanity check
			RHI_ASSERT(mValidationEnabled, "Do only call this Vulkan method if validation is enabled")

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
				const_cast<Rhi::Context*>(&mVulkanRhi.getContext())			// pUserData (void*)
			};
			if ( vkCreateDebugReportCallbackEXT(mVkInstance, &vkDebugReportCallbackCreateInfoEXT, mVulkanRhi.getVkAllocationCallbacks(), &mVkDebugReportCallbackEXT) != VK_SUCCESS )
			{
				RHI_LOG(WARNING, "Failed to create the Vulkan debug report callback")
			}
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VulkanRhi&				 mVulkanRhi;						// Owner Vulkan RHI instance
		bool					 mValidationEnabled;				// Validation enabled?
		void*					 mVulkanSharedLibrary;				// Vulkan shared library, can be a null pointer
		bool					 mEntryPointsRegistered;			// Entry points successfully registered?
		VkInstance				 mVkInstance;						// Vulkan instance, stores all per-application states
		VkDebugReportCallbackEXT mVkDebugReportCallbackEXT;			// Vulkan debug report callback, can be a null handle
		bool					 mInstanceLevelFunctionsRegistered;	// Instance level Vulkan function pointers registered?
		bool					 mInitialized;						// Already initialized?
	};
} // namespace VulkanRhi
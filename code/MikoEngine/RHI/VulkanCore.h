#pragma once

namespace VulkanRhi
{
	class VertexArray;
	class RootSignature;
	class VulkanContext;
	class VulkanRuntimeLinking;
}

//[-------------------------------------------------------]
//[ Macros & definitions                                  ]
//[-------------------------------------------------------]
#if SE_DEBUG
	/*
	*  @brief
	*    Check whether or not the given resource is owned by the given RHI
	*/
#define RHI_MATCH_CHECK(rhiReference, resourceReference) \
		RHI_ASSERT(&rhiReference == &(resourceReference).getRhi(), "Vulkan error: The given resource is owned by another RHI instance")
#else
	/*
	*  @brief
	*    Check whether or not the given resource is owned by the given RHI
	*/
#define RHI_MATCH_CHECK(rhiReference, resourceReference)
#endif

#ifndef FNPTR
#define FNPTR(name) PFN_##name name;
#endif

namespace VulkanRhi
{
	// "VK_EXT_debug_report"-extension
	FNPTR(vkCreateDebugReportCallbackEXT);
	FNPTR(vkDestroyDebugReportCallbackEXT);

	// "VK_EXT_debug_marker"-extension
	FNPTR(vkDebugMarkerSetObjectTagEXT)
	FNPTR(vkDebugMarkerSetObjectNameEXT)
	FNPTR(vkCmdDebugMarkerBeginEXT)
	FNPTR(vkCmdDebugMarkerEndEXT)
	FNPTR(vkCmdDebugMarkerInsertEXT)
}

//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr const char* GLSL_NAME = "GLSL";	// ASCII name of this shader language, always valid (do not free the memory the returned pointer is pointing to)
		typedef std::vector<VkPhysicalDevice> VkPhysicalDevices;
		typedef std::vector<VkExtensionProperties> VkExtensionPropertiesVector;
		typedef std::array<VkPipelineShaderStageCreateInfo, 5> VkPipelineShaderStageCreateInfos;
#ifdef __ANDROID__
		// On Android we need to explicitly select all layers
		#warning "TODO(co) Not tested"
			static constexpr uint32_t NUMBER_OF_VALIDATION_LAYERS = 6;
		static constexpr const char* VALIDATION_LAYER_NAMES[] =
		{
			"VK_LAYER_GOOGLE_threading",
			"VK_LAYER_LUNARG_parameter_validation",
			"VK_LAYER_LUNARG_object_tracker",
			"VK_LAYER_LUNARG_core_validation",
			"VK_LAYER_LUNARG_swapchain",
			"VK_LAYER_GOOGLE_unique_objects"
		};
#else
		// On desktop the LunarG loaders exposes a meta layer that contains all layers
		static constexpr uint32_t NUMBER_OF_VALIDATION_LAYERS = 1;
		static constexpr const char* VALIDATION_LAYER_NAMES[] =
		{
			"VK_LAYER_LUNARG_standard_validation"
		};
#endif

#ifdef SE_GLSLTOSPIRV
		static bool GlslangInitialized = false;

		// Settings from "glslang/StandAlone/ResourceLimits.cpp"
		static constexpr TBuiltInResource DefaultTBuiltInResource =
		{
			32,		// MaxLights
			6,		// MaxClipPlanes
			32,		// MaxTextureUnits
			32,		// MaxTextureCoords
			64,		// MaxVertexAttribs
			4096,	// MaxVertexUniformComponents
			64,		// MaxVaryingFloats
			32,		// MaxVertexTextureImageUnits
			80,		// MaxCombinedTextureImageUnits
			32,		// MaxTextureImageUnits
			4096,	// MaxFragmentUniformComponents
			32,		// MaxDrawBuffers
			128,	// MaxVertexUniformVectors
			8,		// MaxVaryingVectors
			16,		// MaxFragmentUniformVectors
			16,		// MaxVertexOutputVectors
			15,		// MaxFragmentInputVectors
			-8,		// MinProgramTexelOffset
			7,		// MaxProgramTexelOffset
			8,		// MaxClipDistances
			65535,	// MaxComputeWorkGroupCountX
			65535,	// MaxComputeWorkGroupCountY
			65535,	// MaxComputeWorkGroupCountZ
			1024,	// MaxComputeWorkGroupSizeX
			1024,	// MaxComputeWorkGroupSizeY
			64,		// MaxComputeWorkGroupSizeZ
			1024,	// MaxComputeUniformComponents
			16,		// MaxComputeTextureImageUnits
			8,		// MaxComputeImageUniforms
			8,		// MaxComputeAtomicCounters
			1,		// MaxComputeAtomicCounterBuffers
			60,		// MaxVaryingComponents
			64,		// MaxVertexOutputComponents
			64,		// MaxGeometryInputComponents
			128,	// MaxGeometryOutputComponents
			128,	// MaxFragmentInputComponents
			8,		// MaxImageUnits
			8,		// MaxCombinedImageUnitsAndFragmentOutputs
			8,		// MaxCombinedShaderOutputResources
			0,		// MaxImageSamples
			0,		// MaxVertexImageUniforms
			0,		// MaxTessControlImageUniforms
			0,		// MaxTessEvaluationImageUniforms
			0,		// MaxGeometryImageUniforms
			8,		// MaxFragmentImageUniforms
			8,		// MaxCombinedImageUniforms
			16,		// MaxGeometryTextureImageUnits
			256,	// MaxGeometryOutputVertices
			1024,	// MaxGeometryTotalOutputComponents
			1024,	// MaxGeometryUniformComponents
			64,		// MaxGeometryVaryingComponents
			128,	// MaxTessControlInputComponents
			128,	// MaxTessControlOutputComponents
			16,		// MaxTessControlTextureImageUnits
			1024,	// MaxTessControlUniformComponents
			4096,	// MaxTessControlTotalOutputComponents
			128,	// MaxTessEvaluationInputComponents
			128,	// MaxTessEvaluationOutputComponents
			16,		// MaxTessEvaluationTextureImageUnits
			1024,	// MaxTessEvaluationUniformComponents
			120,	// MaxTessPatchComponents
			32,		// MaxPatchVertices
			64,		// MaxTessGenLevel
			16,		// MaxViewports
			0,		// MaxVertexAtomicCounters
			0,		// MaxTessControlAtomicCounters
			0,		// MaxTessEvaluationAtomicCounters
			0,		// MaxGeometryAtomicCounters
			8,		// MaxFragmentAtomicCounters
			8,		// MaxCombinedAtomicCounters
			1,		// MaxAtomicCounterBindings
			0,		// MaxVertexAtomicCounterBuffers
			0,		// MaxTessControlAtomicCounterBuffers
			0,		// MaxTessEvaluationAtomicCounterBuffers
			0,		// MaxGeometryAtomicCounterBuffers
			1,		// MaxFragmentAtomicCounterBuffers
			1,		// MaxCombinedAtomicCounterBuffers
			16384,	// MaxAtomicCounterBufferSize
			4,		// MaxTransformFeedbackBuffers
			64,		// MaxTransformFeedbackInterleavedComponents
			8,		// MaxCullDistances
			8,		// MaxCombinedClipAndCullDistances
			4,		// MaxSamples
			256,	// MaxMeshOutputVerticesNV
			512,	// MaxMeshOutputPrimitivesNV
			32,		// MaxMeshWorkGroupSizeX_NV
			1,		// MaxMeshWorkGroupSizeY_NV
			1,		// MaxMeshWorkGroupSizeZ_NV
			32,		// MaxTaskWorkGroupSizeX_NV
			1,		// MaxTaskWorkGroupSizeY_NV
			1,		// MaxTaskWorkGroupSizeZ_NV
			4,		// MaxMeshViewCountNV
			{		// limits
				1,	// nonInductiveForLoops
				1,	// whileLoops
				1,	// doWhileLoops
				1,	// generalUniformIndexing
				1,	// generalAttributeMatrixVectorIndexing
				1,	// generalVaryingIndexing
				1,	// generalSamplerIndexing
				1,	// generalVariableIndexing
				1,	// generalConstantMatrixVectorIndexing
			}
		};
#endif


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		void updateWidthHeight(uint32_t mipmapIndex, uint32_t textureWidth, uint32_t textureHeight, uint32_t& width, uint32_t& height)
		{
			Rhi::ITexture::getMipmapSize(mipmapIndex, textureWidth, textureHeight);
			if ( width > textureWidth )
			{
				width = textureWidth;
			}
			if ( height > textureHeight )
			{
				height = textureHeight;
			}
		}

		void addVkPipelineShaderStageCreateInfo(VkShaderStageFlagBits vkShaderStageFlagBits, VkShaderModule vkShaderModule, VkPipelineShaderStageCreateInfos& vkPipelineShaderStageCreateInfos, uint32_t stageCount)
		{
			VkPipelineShaderStageCreateInfo& vkPipelineShaderStageCreateInfo = vkPipelineShaderStageCreateInfos[stageCount];
			vkPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;	// sType (VkStructureType)
			vkPipelineShaderStageCreateInfo.pNext = nullptr;												// pNext (const void*)
			vkPipelineShaderStageCreateInfo.flags = 0;													// flags (VkPipelineShaderStageCreateFlags)
			vkPipelineShaderStageCreateInfo.stage = vkShaderStageFlagBits;								// stage (VkShaderStageFlagBits)
			vkPipelineShaderStageCreateInfo.module = vkShaderModule;										// module (VkShaderModule)
			vkPipelineShaderStageCreateInfo.pName = "main";												// pName (const char*)
			vkPipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;												// pSpecializationInfo (const VkSpecializationInfo*)
		}

		void enumeratePhysicalDevices(const Rhi::Context& context, VkInstance vkInstance, VkPhysicalDevices& vkPhysicalDevices)
		{
			// Get the number of available physical devices
			uint32_t physicalDeviceCount = 0;
			VkResult vkResult = vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, nullptr);
			if ( VK_SUCCESS == vkResult )
			{
				if ( physicalDeviceCount > 0 )
				{
					// Enumerate physical devices
					vkPhysicalDevices.resize(physicalDeviceCount);
					vkResult = vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, vkPhysicalDevices.data());
					if ( VK_SUCCESS != vkResult )
					{
						// Error!
						RHI_LOG(CRITICAL, "Failed to enumerate physical Vulkan devices")
					}
				}
				else
				{
					// Error!
					RHI_LOG(CRITICAL, "There are no physical Vulkan devices")
				}
			}
			else
			{
				// Error!
				RHI_LOG(CRITICAL, "Failed to get the number of physical Vulkan devices")
			}
		}

		[[nodiscard]] bool isExtensionAvailable(const char* extensionName, const VkExtensionPropertiesVector& vkExtensionPropertiesVector)
		{
			for ( const VkExtensionProperties& vkExtensionProperties : vkExtensionPropertiesVector )
			{
				if ( strcmp(vkExtensionProperties.extensionName, extensionName) == 0 )
				{
					// The extension is available
					return true;
				}
			}

			// The extension isn't available
			return false;
		}

		[[nodiscard]] VkPhysicalDevice selectPhysicalDevice(const Rhi::Context& context, const VkPhysicalDevices& vkPhysicalDevices, bool validationEnabled, bool& enableDebugMarker)
		{
			// TODO(co) I'am sure this selection can be improved (rating etc.)
			for ( const VkPhysicalDevice& vkPhysicalDevice : vkPhysicalDevices )
			{
				// Get of device extensions
				uint32_t propertyCount = 0;
				if ( (vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &propertyCount, nullptr) != VK_SUCCESS) || (0 == propertyCount) )
				{
					// Reject physical Vulkan device
					continue;
				}
				VkExtensionPropertiesVector vkExtensionPropertiesVector(propertyCount);
				if ( vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &propertyCount, vkExtensionPropertiesVector.data()) != VK_SUCCESS )
				{
					// Reject physical Vulkan device
					continue;
				}

				{ // Reject physical Vulkan devices basing on swap chain support
					// Check device extensions
					static constexpr std::array<const char*, 2> deviceExtensions =
					{
						VK_KHR_SWAPCHAIN_EXTENSION_NAME,
						VK_KHR_MAINTENANCE1_EXTENSION_NAME,	// We want to be able to specify a negative viewport height, this way we don't have to apply "<output position>.y = -<output position>.y" inside vertex shaders to compensate for the Vulkan coordinate system
						// VK_NV_MESH_SHADER_EXTENSION_NAME,	// TODO(co) Mesh shader support must be optional
					};
					bool rejectDevice = false;
					for ( const char* deviceExtension : deviceExtensions )
					{
						if ( !isExtensionAvailable(deviceExtension, vkExtensionPropertiesVector) )
						{
							rejectDevice = true;
							break;
						}
					}
					if ( rejectDevice )
					{
						// Reject physical Vulkan device
						continue;
					}
				}

				{ // Reject physical Vulkan devices basing on supported API version and some basic limits
					VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
					vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkPhysicalDeviceProperties);
					const uint32_t majorVersion = VK_VERSION_MAJOR(vkPhysicalDeviceProperties.apiVersion);
					if ( (majorVersion < 1) || (vkPhysicalDeviceProperties.limits.maxImageDimension2D < 4096) )
					{
						// Reject physical Vulkan device
						continue;
					}
				}

				// Reject physical Vulkan devices basing on supported queue family
				uint32_t queueFamilyPropertyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertyCount, nullptr);
				if ( 0 == queueFamilyPropertyCount )
				{
					// Reject physical Vulkan device
					continue;
				}
				std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());
				for ( uint32_t i = 0; i < queueFamilyPropertyCount; ++i )
				{
					if ( (queueFamilyProperties[i].queueCount > 0) && (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) )
					{
						// Check whether or not the "VK_EXT_debug_marker"-extension is available
						// -> The "VK_EXT_debug_marker"-extension is only available when the application gets started by tools like RenderDoc ( https://renderdoc.org/ )
						// -> See "Offline debugging in Vulkan with VK_EXT_debug_marker and RenderDoc" - https://www.saschawillems.de/?page_id=2017
						if ( enableDebugMarker )
						{
							// Check whether or not the "VK_EXT_debug_marker"-extension is available
							if ( isExtensionAvailable(VK_EXT_DEBUG_MARKER_EXTENSION_NAME, vkExtensionPropertiesVector) )
							{
								// TODO(co) Currently, when trying to use RenderDoc ( https://renderdoc.org/ ) while having Vulkan debug layers enabled, RenderDoc crashes
								// -> Windows 10 x64
								// -> Radeon software 17.7.2
								// -> GPU: AMD 290X
								// -> LunarG® Vulkan™ SDK 1.0.54.0
								// -> Tried RenderDoc v0.91 as well as "Nightly v0.x @ 2017-08-21 (Win x64)" ("RenderDoc_2017_08_21_177d595d_64.zip")
								if ( validationEnabled )
								{
									enableDebugMarker = false;
									RHI_LOG(WARNING, "Vulkan validation layers are enabled: If you want to use debug markers (\"VK_EXT_debug_marker\"-extension) please disable the validation layers")
								}
							}
							else
							{
								// Silently disable debug marker
								enableDebugMarker = false;
							}
						}

						// Select physical Vulkan device
						return vkPhysicalDevice;
					}
				}
			}

			// Error!
			RHI_LOG(CRITICAL, "Failed to select a physical Vulkan device")
				return VK_NULL_HANDLE;
		}

		[[nodiscard]] VkResult createVkDevice(const Rhi::Context& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkPhysicalDevice vkPhysicalDevice, const VkDeviceQueueCreateInfo& vkDeviceQueueCreateInfo, bool enableValidation, bool enableDebugMarker, VkDevice& vkDevice)
		{
			// See http://vulkan.gpuinfo.org/listfeatures.php to check out GPU hardware capabilities
			static constexpr std::array<const char*, 3> enabledExtensions =
			{
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
				VK_KHR_MAINTENANCE1_EXTENSION_NAME,		// We want to be able to specify a negative viewport height, this way we don't have to apply "<output position>.y = -<output position>.y" inside vertex shaders to compensate for the Vulkan coordinate system
				// VK_NV_MESH_SHADER_EXTENSION_NAME,	// TODO(co) Mesh shader support must be optional
				VK_EXT_DEBUG_MARKER_EXTENSION_NAME
			};

			// This will only be used if meshShadingSupported=true (see below)
			VkPhysicalDeviceMeshShaderFeaturesNV vkPhysicalDeviceMeshShaderFeaturesNV = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV };
			vkPhysicalDeviceMeshShaderFeaturesNV.taskShader = true;
			vkPhysicalDeviceMeshShaderFeaturesNV.meshShader = true;
			static constexpr VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures =
			{
				VK_FALSE,	// robustBufferAccess (VkBool32)
				VK_FALSE,	// fullDrawIndexUint32 (VkBool32)
				VK_TRUE,	// imageCubeArray (VkBool32)
				VK_FALSE,	// independentBlend (VkBool32)
				VK_TRUE,	// geometryShader (VkBool32)
				VK_TRUE,	// tessellationShader (VkBool32)
				VK_FALSE,	// sampleRateShading (VkBool32)
				VK_FALSE,	// dualSrcBlend (VkBool32)
				VK_FALSE,	// logicOp (VkBool32)
				VK_TRUE,	// multiDrawIndirect (VkBool32)
				VK_FALSE,	// drawIndirectFirstInstance (VkBool32)
				VK_TRUE,	// depthClamp (VkBool32)
				VK_FALSE,	// depthBiasClamp (VkBool32)
				VK_TRUE,	// fillModeNonSolid (VkBool32)
				VK_FALSE,	// depthBounds (VkBool32)
				VK_FALSE,	// wideLines (VkBool32)
				VK_FALSE,	// largePoints (VkBool32)
				VK_FALSE,	// alphaToOne (VkBool32)
				VK_FALSE,	// multiViewport (VkBool32)
				VK_TRUE,	// samplerAnisotropy (VkBool32)
				VK_FALSE,	// textureCompressionETC2 (VkBool32)
				VK_FALSE,	// textureCompressionASTC_LDR (VkBool32)
				VK_TRUE,	// textureCompressionBC (VkBool32)
				VK_TRUE,	// occlusionQueryPrecise (VkBool32)
				VK_TRUE,	// pipelineStatisticsQuery (VkBool32)
				VK_FALSE,	// vertexPipelineStoresAndAtomics (VkBool32)
				VK_FALSE,	// fragmentStoresAndAtomics (VkBool32)
				VK_FALSE,	// shaderTessellationAndGeometryPointSize (VkBool32)
				VK_FALSE,	// shaderImageGatherExtended (VkBool32)
				VK_FALSE,	// shaderStorageImageExtendedFormats (VkBool32)
				VK_FALSE,	// shaderStorageImageMultisample (VkBool32)
				VK_FALSE,	// shaderStorageImageReadWithoutFormat (VkBool32)
				VK_FALSE,	// shaderStorageImageWriteWithoutFormat (VkBool32)
				VK_FALSE,	// shaderUniformBufferArrayDynamicIndexing (VkBool32)
				VK_FALSE,	// shaderSampledImageArrayDynamicIndexing (VkBool32)
				VK_FALSE,	// shaderStorageBufferArrayDynamicIndexing (VkBool32)
				VK_FALSE,	// shaderStorageImageArrayDynamicIndexing (VkBool32)
				VK_FALSE,	// shaderClipDistance (VkBool32)
				VK_FALSE,	// shaderCullDistance (VkBool32)
				VK_FALSE,	// shaderFloat64 (VkBool32)
				VK_FALSE,	// shaderInt64 (VkBool32)
				VK_FALSE,	// shaderInt16 (VkBool32)
				VK_FALSE,	// shaderResourceResidency (VkBool32)
				VK_FALSE,	// shaderResourceMinLod (VkBool32)
				VK_FALSE,	// sparseBinding (VkBool32)
				VK_FALSE,	// sparseResidencyBuffer (VkBool32)
				VK_FALSE,	// sparseResidencyImage2D (VkBool32)
				VK_FALSE,	// sparseResidencyImage3D (VkBool32)
				VK_FALSE,	// sparseResidency2Samples (VkBool32)
				VK_FALSE,	// sparseResidency4Samples (VkBool32)
				VK_FALSE,	// sparseResidency8Samples (VkBool32)
				VK_FALSE,	// sparseResidency16Samples (VkBool32)
				VK_FALSE,	// sparseResidencyAliased (VkBool32)
				VK_FALSE,	// variableMultisampleRate (VkBool32)
				VK_FALSE	// inheritedQueries (VkBool32)
			};
			const VkDeviceCreateInfo vkDeviceCreateInfo =
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,							// sType (VkStructureType)
				&vkPhysicalDeviceMeshShaderFeaturesNV,							// pNext (const void*)
				0,																// flags (VkDeviceCreateFlags)
				1,																// queueCreateInfoCount (uint32_t)
				&vkDeviceQueueCreateInfo,										// pQueueCreateInfos (const VkDeviceQueueCreateInfo*)
				enableValidation ? NUMBER_OF_VALIDATION_LAYERS : 0,				// enabledLayerCount (uint32_t)
				enableValidation ? VALIDATION_LAYER_NAMES : nullptr,			// ppEnabledLayerNames (const char* const*)
				//enableDebugMarker ? 4u : 3u,									// enabledExtensionCount (uint32_t)
				enableDebugMarker ? 3u : 2u,									// enabledExtensionCount (uint32_t)
				enabledExtensions.empty() ? nullptr : enabledExtensions.data(),	// ppEnabledExtensionNames (const char* const*)
				&vkPhysicalDeviceFeatures										// pEnabledFeatures (const VkPhysicalDeviceFeatures*)
			};
			const VkResult vkResult = vkCreateDevice(vkPhysicalDevice, &vkDeviceCreateInfo, vkAllocationCallbacks, &vkDevice);
			if ( VK_SUCCESS == vkResult && enableDebugMarker )
			{
				// Get "VK_EXT_debug_marker"-extension function pointers

				// Define a helper macro
#define IMPORT_FUNC(funcName)																					\
					VulkanRhi::funcName = reinterpret_cast<PFN_##funcName>(vkGetDeviceProcAddr(vkDevice, #funcName));						\
					if (nullptr == VulkanRhi::funcName)																					\
					{																											\
						RHI_LOG( CRITICAL, "Failed to load instance based Vulkan function pointer \"%s\"", #funcName)	\
					}																											\

				// "VK_EXT_debug_marker"-extension
					IMPORT_FUNC(vkDebugMarkerSetObjectTagEXT)
					IMPORT_FUNC(vkDebugMarkerSetObjectNameEXT)
					IMPORT_FUNC(vkCmdDebugMarkerBeginEXT)
					IMPORT_FUNC(vkCmdDebugMarkerEndEXT)
					IMPORT_FUNC(vkCmdDebugMarkerInsertEXT)

					// Undefine the helper macro
#undef IMPORT_FUNC
			}

			// Done
			return vkResult;
		}

		[[nodiscard]] VkDevice createVkDevice(const Rhi::Context& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkPhysicalDevice vkPhysicalDevice, bool enableValidation, bool enableDebugMarker, uint32_t& graphicsQueueFamilyIndex, uint32_t& presentQueueFamilyIndex)
		{
			VkDevice vkDevice = VK_NULL_HANDLE;

			// Get physical device queue family properties
			uint32_t queueFamilyPropertyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertyCount, nullptr);
			if ( queueFamilyPropertyCount > 0 )
			{
				std::vector<VkQueueFamilyProperties> vkQueueFamilyProperties;
				vkQueueFamilyProperties.resize(queueFamilyPropertyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertyCount, vkQueueFamilyProperties.data());

				// Find a queue that supports graphics operations
				uint32_t graphicsQueueIndex = 0;
				for ( ; graphicsQueueIndex < queueFamilyPropertyCount; ++graphicsQueueIndex )
				{
					if ( vkQueueFamilyProperties[graphicsQueueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT )
					{
						// Create logical Vulkan device instance
						static constexpr std::array<float, 1> queuePriorities = { 0.0f };
						const VkDeviceQueueCreateInfo vkDeviceQueueCreateInfo =
						{
							VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,	// sType (VkStructureType)
							nullptr,									// pNext (const void*)
							0,											// flags (VkDeviceQueueCreateFlags)
							graphicsQueueIndex,							// queueFamilyIndex (uint32_t)
							1,											// queueCount (uint32_t)
							queuePriorities.data()						// pQueuePriorities (const float*)
						};
						VkResult vkResult = createVkDevice(context, vkAllocationCallbacks, vkPhysicalDevice, vkDeviceQueueCreateInfo, enableValidation, enableDebugMarker, vkDevice);
						if ( VK_ERROR_LAYER_NOT_PRESENT == vkResult && enableValidation )
						{
							// Error! Since the show must go on, try creating a Vulkan device instance without validation enabled...
							RHI_LOG(WARNING, "Failed to create the Vulkan device instance with validation enabled, layer is not present")
								vkResult = createVkDevice(context, vkAllocationCallbacks, vkPhysicalDevice, vkDeviceQueueCreateInfo, false, enableDebugMarker, vkDevice);
						}
						// TODO(co) Error handling: Evaluate "vkResult"?
						graphicsQueueFamilyIndex = graphicsQueueIndex;
						presentQueueFamilyIndex = graphicsQueueIndex;	// TODO(co) Handle the case of the graphics queue doesn't support present

						// We're done, get us out of the loop
						graphicsQueueIndex = queueFamilyPropertyCount;
					}
				}
			}
			else
			{
				// Error!
				RHI_LOG(CRITICAL, "Failed to get physical Vulkan device queue family properties")
			}

			// Done
			return vkDevice;
		}

		[[nodiscard]] VkCommandPool createVkCommandPool(const Rhi::Context& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, uint32_t graphicsQueueFamilyIndex)
		{
			VkCommandPool vkCommandPool = VK_NULL_HANDLE;

			// Create Vulkan command pool instance
			const VkCommandPoolCreateInfo vkCommandPoolCreateInfo =
			{
				VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,			// sType (VkStructureType)
				nullptr,											// pNext (const void*)
				VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,	// flags (VkCommandPoolCreateFlags)
				graphicsQueueFamilyIndex,							/// queueFamilyIndex (uint32_t)
			};
			const VkResult vkResult = vkCreateCommandPool(vkDevice, &vkCommandPoolCreateInfo, vkAllocationCallbacks, &vkCommandPool);
			if ( VK_SUCCESS != vkResult )
			{
				// Error!
				RHI_LOG(CRITICAL, "Failed to create Vulkan command pool instance")
			}

			// Done
			return vkCommandPool;
		}

		[[nodiscard]] VkCommandBuffer createVkCommandBuffer(const Rhi::Context& context, VkDevice vkDevice, VkCommandPool vkCommandPool)
		{
			VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;

			// Create Vulkan command buffer instance
			const VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo =
			{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,	// sType (VkStructureType)
				nullptr,										// pNext (const void*)
				vkCommandPool,									// commandPool (VkCommandPool)
				VK_COMMAND_BUFFER_LEVEL_PRIMARY,				// level (VkCommandBufferLevel)
				1												// commandBufferCount (uint32_t)
			};
			VkResult vkResult = vkAllocateCommandBuffers(vkDevice, &vkCommandBufferAllocateInfo, &vkCommandBuffer);
			if ( VK_SUCCESS != vkResult )
			{
				// Error!
				RHI_LOG(CRITICAL, "Failed to create Vulkan command buffer instance")
			}

			// Done
			return vkCommandBuffer;
		}

		[[nodiscard]] bool hasVkFormatStencilComponent(VkFormat vkFormat)
		{
			return (VK_FORMAT_D32_SFLOAT_S8_UINT == vkFormat || VK_FORMAT_D24_UNORM_S8_UINT == vkFormat);
		}

		[[nodiscard]] const char* vkDebugReportObjectTypeToString(VkDebugReportObjectTypeEXT vkDebugReportObjectTypeEXT)
		{
			// Define helper macro
#define VALUE(value) case value: return #value;

// Evaluate
			switch ( vkDebugReportObjectTypeEXT )
			{
				VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_EXT)
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV_EXT)
					//	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT)	not possible
					//	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT)	not possible
					//	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_KHR_EXT)	not possible
					//	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR_EXT)	not possible
					//	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_BEGIN_RANGE_EXT)	not possible
					//	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_END_RANGE_EXT)	not possible
					VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_MAX_ENUM_EXT)
			}

			// Undefine helper macro
#undef VALUE

// Error!
			return nullptr;
		}

		[[nodiscard]] VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
		{
			const Rhi::Context* context = static_cast<const Rhi::Context*>(pUserData);

			// TODO(co) Inside e.g. the "InstancedCubes"-example the log gets currently flooded with
			//          "Warning: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT" Object: "120" Location: "5460" Message code: "0" Layer prefix: "DS" Message: "DescriptorSet 0x78 previously bound as set #0 is incompatible with set 0xc82f498 newly bound as set #0 so set #1 and any subsequent sets were disturbed by newly bound pipelineLayout (0x8b)" ".
			//          It's a known Vulkan API issue regarding validation. See https://github.com/KhronosGroup/Vulkan-Docs/issues/305 - "vkCmdBindDescriptorSets should be able to take NULL sets. #305".
			//          Currently I see no other way then ignoring this message.
			if ( VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT == objectType && 5460 == location && 0 == messageCode )
			{
				// The Vulkan call should not be aborted to have the same behavior with and without validation layers enabled
				return VK_FALSE;
			}

			// TODO(co) File "source\rhi\private\vulkanrhi\vulkanrhi.cpp" | Line 1029 | Critical: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT" Object: "4963848" Location: "0" Message code: "0" Layer prefix: "Loader Message" Message: "loader_create_device_chain: Failed to find 'vkGetInstanceProcAddr' in layer C:\Program Files (x86)\Steam\.\SteamOverlayVulkanLayer.dll.  Skipping layer." 
			if ( VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT == objectType && object && 0 == location && 0 == messageCode && nullptr != strstr(pMessage, "SteamOverlayVulkanLayer.dll") )
			{
				return VK_FALSE;
			}

			// Get log message type
			// -> Vulkan is using a flags combination, map it to our log message type enumeration
			DefaultLog::Type type = DefaultLog::Type::TRACE;
			if ( (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0 )
			{
				type = DefaultLog::Type::CRITICAL;
			}
			else if ( (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0 )
			{
				type = DefaultLog::Type::WARNING;
			}
			else if ( (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0 )
			{
				type = DefaultLog::Type::PERFORMANCE_WARNING;
			}
			else if ( (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0 )
			{
				type = DefaultLog::Type::INFORMATION;
			}
			else if ( (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0 )
			{
				type = DefaultLog::Type::DEBUG;
			}

			// Construct the log message
			std::stringstream message;
			message << "Vulkan debug report callback: ";
			message << "Object type: \"" << vkDebugReportObjectTypeToString(objectType) << "\" ";
			message << "Object: \"" << object << "\" ";
			message << "Location: \"" << location << "\" ";
			message << "Message code: \"" << messageCode << "\" ";
			message << "Layer prefix: \"" << pLayerPrefix << "\" ";
			message << "Message: \"" << pMessage << "\" ";

			// Print log message
			if ( GetLog().print(type, nullptr, __FILE__, static_cast<uint32_t>(__LINE__), message.str().c_str()) )
			{
				SE_DEBUG_BREAK;
			}

			// The Vulkan call should not be aborted to have the same behavior with and without validation layers enabled
			return VK_FALSE;
		}

		[[nodiscard]] VkSurfaceKHR createPresentationSurface(const Rhi::Context& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkInstance vkInstance, VkPhysicalDevice vkPhysicalDevice, uint32_t graphicsQueueFamilyIndex, Rhi::WindowHandle windoInfo)
		{
			VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE;

#ifdef VK_USE_PLATFORM_WIN32_KHR
			const VkWin32SurfaceCreateInfoKHR vkWin32SurfaceCreateInfoKHR =
			{
				VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,																		// sType (VkStructureType)
				nullptr,																												// pNext (const void*)
				0,																														// flags (VkWin32SurfaceCreateFlagsKHR)
				reinterpret_cast<HINSTANCE>(::GetWindowLongPtr(reinterpret_cast<HWND>(windoInfo.nativeWindowHandle), GWLP_HINSTANCE)),	// hinstance (HINSTANCE)
				reinterpret_cast<HWND>(windoInfo.nativeWindowHandle)																	// hwnd (HWND)
			};
			if ( vkCreateWin32SurfaceKHR(vkInstance, &vkWin32SurfaceCreateInfoKHR, vkAllocationCallbacks, &vkSurfaceKHR) != VK_SUCCESS )
			{
				// TODO(co) Can we ensure "vkSurfaceKHR" doesn't get touched by "vkCreateWin32SurfaceKHR()" in case of failure?
				vkSurfaceKHR = VK_NULL_HANDLE;
			}
#elif defined VK_USE_PLATFORM_ANDROID_KHR
			#warning "TODO(co) Not tested"
				const VkAndroidSurfaceCreateInfoKHR vkAndroidSurfaceCreateInfoKHR =
			{
				VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,				// sType (VkStructureType)
				nullptr,														// pNext (const void*)
				0,																// flags (VkAndroidSurfaceCreateFlagsKHR)
				reinterpret_cast<ANativeWindow*>(windoInfo.nativeWindowHandle)	// window (ANativeWindow*)
			};
			if ( vkCreateAndroidSurfaceKHR(vkInstance, &vkAndroidSurfaceCreateInfoKHR, vkAllocationCallbacks, &vkSurfaceKHR) != VK_SUCCESS )
			{
				// TODO(co) Can we ensure "vkSurfaceKHR" doesn't get touched by "vkCreateAndroidSurfaceKHR()" in case of failure?
				vkSurfaceKHR = VK_NULL_HANDLE;
			}
#elif defined VK_USE_PLATFORM_XLIB_KHR || defined VK_USE_PLATFORM_WAYLAND_KHR
			RHI_ASSERT(context.getType() == Rhi::Context::ContextType::X11 || context.getType() == Rhi::Context::ContextType::WAYLAND, "Invalid Vulkan context type")

				// If the given RHI context is an X11 context use the display connection object provided by the context
				if ( context.getType() == Rhi::Context::ContextType::X11 )
				{
					const Rhi::X11Context& x11Context = static_cast<const Rhi::X11Context&>(context);
					const VkXlibSurfaceCreateInfoKHR vkXlibSurfaceCreateInfoKHR =
					{
						VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,	// sType (VkStructureType)
						nullptr,										// pNext (const void*)
						0,												// flags (VkXlibSurfaceCreateFlagsKHR)
						x11Context.getDisplay(),						// dpy (Display*)
						windoInfo.nativeWindowHandle					// window (Window)
					};
					if ( vkCreateXlibSurfaceKHR(vkInstance, &vkXlibSurfaceCreateInfoKHR, vkAllocationCallbacks, &vkSurfaceKHR) != VK_SUCCESS )
					{
						// TODO(co) Can we ensure "vkSurfaceKHR" doesn't get touched by "vkCreateXlibSurfaceKHR()" in case of failure?
						vkSurfaceKHR = VK_NULL_HANDLE;
					}
				}
				else if ( context.getType() == Rhi::Context::ContextType::WAYLAND )
				{
					const Rhi::WaylandContext& waylandContext = static_cast<const Rhi::WaylandContext&>(context);
					const VkWaylandSurfaceCreateInfoKHR vkWaylandSurfaceCreateInfoKHR =
					{
						VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,	// sType (VkStructureType)
						nullptr,											// pNext (const void*)
						0,													// flags (VkWaylandSurfaceCreateInfoKHR)
						waylandContext.getDisplay(),						// display (wl_display*)
						windoInfo.waylandSurface							// surface (wl_surface*)
					};
					if ( vkCreateWaylandSurfaceKHR(vkInstance, &vkWaylandSurfaceCreateInfoKHR, vkAllocationCallbacks, &vkSurfaceKHR) != VK_SUCCESS )
					{
						// TODO(co) Can we ensure "vkSurfaceKHR" doesn't get touched by "vkCreateWaylandSurfaceKHR()" in case of failure?
						vkSurfaceKHR = VK_NULL_HANDLE;
					}
				}
#elif defined VK_USE_PLATFORM_XCB_KHR
#error "TODO(co) Complete implementation"
			const VkXcbSurfaceCreateInfoKHR vkXcbSurfaceCreateInfoKHR =
			{
				VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,	// sType (VkStructureType)
				nullptr,										// pNext (const void*)
				0,												// flags (VkXcbSurfaceCreateFlagsKHR)
				TODO(co)										// connection (xcb_connection_t*)
				TODO(co)										// window (xcb_window_t)
			};
			if ( vkCreateXcbSurfaceKHR(vkInstance, &vkXcbSurfaceCreateInfoKHR, vkAllocationCallbacks, &vkSurfaceKHR) != VK_SUCCESS )
			{
				// TODO(co) Can we ensure "vkSurfaceKHR" doesn't get touched by "vkCreateXcbSurfaceKHR()" in case of failure?
				vkSurfaceKHR = VK_NULL_HANDLE;
			}
#else
#error "Unsupported platform"
#endif

			{ // Sanity check: Does the physical Vulkan device support the Vulkan presentation surface?
			  // TODO(co) Inside our RHI implementation the swap chain is physical device independent, which is a nice thing usability wise.
			  //          On the other hand, the sanity check here can only detect issues but it would be better to not get into such issues in the first place.
				VkBool32 queuePresentSupport = VK_FALSE;
				vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, graphicsQueueFamilyIndex, vkSurfaceKHR, &queuePresentSupport);
				if ( VK_FALSE == queuePresentSupport )
				{
					RHI_LOG(CRITICAL, "The created Vulkan presentation surface has no queue present support")
				}
			}

			// Done
			return vkSurfaceKHR;
		}

		[[nodiscard]] uint32_t getNumberOfSwapChainImages(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR)
		{
			// Set of images defined in a swap chain may not always be available for application to render to:
			// - One may be displayed and one may wait in a queue to be presented
			// - If application wants to use more images at the same time it must ask for more images
			uint32_t numberOfImages = vkSurfaceCapabilitiesKHR.minImageCount + 1;
			if ( (vkSurfaceCapabilitiesKHR.maxImageCount > 0) && (numberOfImages > vkSurfaceCapabilitiesKHR.maxImageCount) )
			{
				numberOfImages = vkSurfaceCapabilitiesKHR.maxImageCount;
			}
			return numberOfImages;
		}

		[[nodiscard]] VkSurfaceFormatKHR getSwapChainFormat(const Rhi::Context& context, VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurfaceKHR)
		{
			uint32_t surfaceFormatCount = 0;
			if ( (vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurfaceKHR, &surfaceFormatCount, nullptr) != VK_SUCCESS) || (0 == surfaceFormatCount) )
			{
				RHI_LOG(CRITICAL, "Failed to get physical Vulkan device surface formats")
					return { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_MAX_ENUM_KHR };
			}

			std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
			if ( vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurfaceKHR, &surfaceFormatCount, surfaceFormats.data()) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to get physical Vulkan device surface formats")
					return { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_MAX_ENUM_KHR };
			}

			// If the list contains only one entry with undefined format it means that there are no preferred surface formats and any can be chosen
			if ( (surfaceFormats.size() == 1) && (VK_FORMAT_UNDEFINED == surfaceFormats[0].format) )
			{
				return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
			}

			// Check if list contains most widely used R8 G8 B8 A8 format with nonlinear color space
			// -> Not all implementations support RGBA8, some only support BGRA8 formats (e.g. xlib surface under Linux with RADV), so check for both
			for ( const VkSurfaceFormatKHR& surfaceFormat : surfaceFormats )
			{
				if ( VK_FORMAT_R8G8B8A8_UNORM == surfaceFormat.format || VK_FORMAT_B8G8R8A8_UNORM == surfaceFormat.format )
				{
					return surfaceFormat;
				}
			}

			// Return the first format from the list
			return surfaceFormats[0];
		}

		[[nodiscard]] VkExtent2D getSwapChainExtent(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR)
		{
			// Special value of surface extent is width == height == -1
			// -> If this is so we define the size by ourselves but it must fit within defined confines, else it's already set to the operation window dimension
			if ( -1 == vkSurfaceCapabilitiesKHR.currentExtent.width )
			{
				VkExtent2D swapChainExtent = { 640, 480 };
				if ( swapChainExtent.width < vkSurfaceCapabilitiesKHR.minImageExtent.width )
				{
					swapChainExtent.width = vkSurfaceCapabilitiesKHR.minImageExtent.width;
				}
				if ( swapChainExtent.height < vkSurfaceCapabilitiesKHR.minImageExtent.height )
				{
					swapChainExtent.height = vkSurfaceCapabilitiesKHR.minImageExtent.height;
				}
				if ( swapChainExtent.width > vkSurfaceCapabilitiesKHR.maxImageExtent.width )
				{
					swapChainExtent.width = vkSurfaceCapabilitiesKHR.maxImageExtent.width;
				}
				if ( swapChainExtent.height > vkSurfaceCapabilitiesKHR.maxImageExtent.height )
				{
					swapChainExtent.height = vkSurfaceCapabilitiesKHR.maxImageExtent.height;
				}
				return swapChainExtent;
			}

			// Most of the cases we define size of the swap chain images equal to current window's size
			return vkSurfaceCapabilitiesKHR.currentExtent;
		}

		[[nodiscard]] VkImageUsageFlags getSwapChainUsageFlags(const Rhi::Context& context, const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR)
		{
			// Color attachment flag must always be supported. We can define other usage flags but we always need to check if they are supported.
			if ( vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT )
			{
				return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			}

			// Construct the log message
			std::stringstream message;
			message << "VK_IMAGE_USAGE_TRANSFER_DST image usage is not supported by the swap chain: Supported swap chain image usages include:\n";
			message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) ? "  VK_IMAGE_USAGE_TRANSFER_SRC\n" : "");
			message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) ? "  VK_IMAGE_USAGE_TRANSFER_DST\n" : "");
			message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT) ? "  VK_IMAGE_USAGE_SAMPLED\n" : "");
			message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT) ? "  VK_IMAGE_USAGE_STORAGE\n" : "");
			message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) ? "  VK_IMAGE_USAGE_COLOR_ATTACHMENT\n" : "");
			message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) ? "  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT\n" : "");
			message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) ? "  VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT\n" : "");
			message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) ? "  VK_IMAGE_USAGE_INPUT_ATTACHMENT" : "");

			// Print log message
			RHI_LOG(CRITICAL, message.str().c_str())

				// Error!
				return static_cast<VkImageUsageFlags>(-1);
		}

		[[nodiscard]] VkSurfaceTransformFlagBitsKHR getSwapChainTransform(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR)
		{
			// - Sometimes images must be transformed before they are presented (i.e. due to device's orientation being other than default orientation)
			// - If the specified transform is other than current transform, presentation engine will transform image during presentation operation; this operation may hit performance on some platforms
			// - Here we don't want any transformations to occur so if the identity transform is supported use it otherwise just use the same transform as current transform
			return (vkSurfaceCapabilitiesKHR.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR : vkSurfaceCapabilitiesKHR.currentTransform;
		}

		[[nodiscard]] VkPresentModeKHR getSwapChainPresentMode(const Rhi::Context& context, VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurfaceKHR)
		{
			uint32_t presentModeCount = 0;
			if ( (vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSurfaceKHR, &presentModeCount, nullptr) != VK_SUCCESS) || (0 == presentModeCount) )
			{
				RHI_LOG(CRITICAL, "Failed to get physical Vulkan device surface present modes")
					return VK_PRESENT_MODE_MAX_ENUM_KHR;
			}

			std::vector<VkPresentModeKHR> presentModes(presentModeCount);
			if ( vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSurfaceKHR, &presentModeCount, presentModes.data()) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to get physical Vulkan device surface present modes")
					return VK_PRESENT_MODE_MAX_ENUM_KHR;
			}

			// - FIFO present mode is always available
			// - MAILBOX is the lowest latency V-Sync enabled mode (something like triple-buffering) so use it if available
			for ( const VkPresentModeKHR& presentMode : presentModes )
			{
				if ( VK_PRESENT_MODE_MAILBOX_KHR == presentMode )
				{
					return presentMode;
				}
			}
			for ( const VkPresentModeKHR& presentMode : presentModes )
			{
				if ( VK_PRESENT_MODE_FIFO_KHR == presentMode )
				{
					return presentMode;
				}
			}

			// Error!
			RHI_LOG(CRITICAL, "FIFO present mode is not supported by the Vulkan swap chain")
				return VK_PRESENT_MODE_MAX_ENUM_KHR;
		}

		[[nodiscard]] VkRenderPass createRenderPass(const Rhi::Context& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, VkFormat colorVkFormat, VkFormat depthVkFormat, VkSampleCountFlagBits vkSampleCountFlagBits)
		{
			const bool hasDepthStencilAttachment = (VK_FORMAT_UNDEFINED != depthVkFormat);

			// Render pass configuration
			const std::array<VkAttachmentDescription, 2> vkAttachmentDescriptions =
			{ {
				{
					0,									// flags (VkAttachmentDescriptionFlags)
					colorVkFormat,						// format (VkFormat)
					vkSampleCountFlagBits,				// samples (VkSampleCountFlagBits)
					VK_ATTACHMENT_LOAD_OP_CLEAR,		// loadOp (VkAttachmentLoadOp)
					VK_ATTACHMENT_STORE_OP_STORE,		// storeOp (VkAttachmentStoreOp)
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,	// stencilLoadOp (VkAttachmentLoadOp)
					VK_ATTACHMENT_STORE_OP_DONT_CARE,	// stencilStoreOp (VkAttachmentStoreOp)
					VK_IMAGE_LAYOUT_UNDEFINED,			// initialLayout (VkImageLayout)
					VK_IMAGE_LAYOUT_PRESENT_SRC_KHR		// finalLayout (VkImageLayout)
				},
				{
					0,													// flags (VkAttachmentDescriptionFlags)
					depthVkFormat,										// format (VkFormat)
					vkSampleCountFlagBits,								// samples (VkSampleCountFlagBits)
					VK_ATTACHMENT_LOAD_OP_CLEAR,						// loadOp (VkAttachmentLoadOp)
					VK_ATTACHMENT_STORE_OP_STORE,						// storeOp (VkAttachmentStoreOp)
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,					// stencilLoadOp (VkAttachmentLoadOp)
					VK_ATTACHMENT_STORE_OP_DONT_CARE,					// stencilStoreOp (VkAttachmentStoreOp)
					VK_IMAGE_LAYOUT_UNDEFINED,							// initialLayout (VkImageLayout)
					VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL	// finalLayout (VkImageLayout)
				}
			} };
			static constexpr VkAttachmentReference colorVkAttachmentReference =
			{
				0,											// attachment (uint32_t)
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL	// layout (VkImageLayout)
			};
			static constexpr VkAttachmentReference depthVkAttachmentReference =
			{
				1,													// attachment (uint32_t)
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL	// layout (VkImageLayout)
			};
			const VkSubpassDescription vkSubpassDescription =
			{
				0,																	// flags (VkSubpassDescriptionFlags)
				VK_PIPELINE_BIND_POINT_GRAPHICS,									// pipelineBindPoint (VkPipelineBindPoint)
				0,																	// inputAttachmentCount (uint32_t)
				nullptr,															// pInputAttachments (const VkAttachmentReference*)
				1,																	// colorAttachmentCount (uint32_t)
				&colorVkAttachmentReference,										// pColorAttachments (const VkAttachmentReference*)
				nullptr,															// pResolveAttachments (const VkAttachmentReference*)
				hasDepthStencilAttachment ? &depthVkAttachmentReference : nullptr,	// pDepthStencilAttachment (const VkAttachmentReference*)
				0,																	// preserveAttachmentCount (uint32_t)
				nullptr																// pPreserveAttachments (const uint32_t*)
			};
			static constexpr VkSubpassDependency vkSubpassDependency =
			{
				VK_SUBPASS_EXTERNAL,														// srcSubpass (uint32_t)
				0,																			// dstSubpass (uint32_t)
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// srcStageMask (VkPipelineStageFlags)
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// dstStageMask (VkPipelineStageFlags)
				0,																			// srcAccessMask (VkAccessFlags)
				VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,	// dstAccessMask (VkAccessFlags)
				0																			// dependencyFlags (VkDependencyFlags)
			};
			const VkRenderPassCreateInfo vkRenderPassCreateInfo =
			{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,	// sType (VkStructureType)
				nullptr,									// pNext (const void*)
				0,											// flags (VkRenderPassCreateFlags)
				hasDepthStencilAttachment ? 2u : 1u,		// attachmentCount (uint32_t)
				vkAttachmentDescriptions.data(),			// pAttachments (const VkAttachmentDescription*)
				1,											// subpassCount (uint32_t)
				&vkSubpassDescription,						// pSubpasses (const VkSubpassDescription*)
				1,											// dependencyCount (uint32_t)
				&vkSubpassDependency						// pDependencies (const VkSubpassDependency*)
			};

			// Create render pass
			VkRenderPass vkRenderPass = VK_NULL_HANDLE;
			if ( vkCreateRenderPass(vkDevice, &vkRenderPassCreateInfo, vkAllocationCallbacks, &vkRenderPass) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to create Vulkan render pass")
			}

			// Done
			return vkRenderPass;
		}

		[[nodiscard]] VkFormat findSupportedVkFormat(VkPhysicalDevice vkPhysicalDevice, const std::vector<VkFormat>& vkFormatCandidates, VkImageTiling vkImageTiling, VkFormatFeatureFlags vkFormatFeatureFlags)
		{
			for ( VkFormat vkFormat : vkFormatCandidates )
			{
				VkFormatProperties vkFormatProperties;
				vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, vkFormat, &vkFormatProperties);
				if ( VK_IMAGE_TILING_LINEAR == vkImageTiling && (vkFormatProperties.linearTilingFeatures & vkFormatFeatureFlags) == vkFormatFeatureFlags )
				{
					return vkFormat;
				}
				else if ( VK_IMAGE_TILING_OPTIMAL == vkImageTiling && (vkFormatProperties.optimalTilingFeatures & vkFormatFeatureFlags) == vkFormatFeatureFlags )
				{
					return vkFormat;
				}
			}

			// Failed to find supported Vulkan depth format
			return VK_FORMAT_UNDEFINED;
		}

		/**
		*  @brief
		*    Create Vulkan shader module from bytecode
		*
		*  @param[in] context
		*    RHI context
		*  @param[in] vkAllocationCallbacks
		*    Vulkan allocation callbacks
		*  @param[in] vkDevice
		*    Vulkan device
		*  @param[in] shaderBytecode
		*    Shader SPIR-V bytecode compressed via SMOL-V
		*
		*  @return
		*    The Vulkan shader module, null handle on error
		*/
		[[nodiscard]] VkShaderModule createVkShaderModuleFromBytecode(const Rhi::Context& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, const Rhi::ShaderBytecode& shaderBytecode)
		{
			// Decode from SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
			// -> https://github.com/aras-p/smol-v
			// -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
			const size_t spirvOutputBufferSize = smolv::GetDecodedBufferSize(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
			// TODO(co) Try to avoid new/delete by trying to use the C-runtime stack if there aren't too many bytes
			uint8_t* spirvOutputBuffer = RHI_MALLOC_TYPED(uint8_t, spirvOutputBufferSize);
			smolv::Decode(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes(), spirvOutputBuffer, spirvOutputBufferSize);

			// Create the Vulkan shader module
			const VkShaderModuleCreateInfo vkShaderModuleCreateInfo =
			{
				VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,			// sType (VkStructureType)
				nullptr,												// pNext (const void*)
				0,														// flags (VkShaderModuleCreateFlags)
				spirvOutputBufferSize,									// codeSize (size_t)
				reinterpret_cast<const uint32_t*>(spirvOutputBuffer)	// pCode (const uint32_t*)
			};
			VkShaderModule vkShaderModule = VK_NULL_HANDLE;
			if ( vkCreateShaderModule(vkDevice, &vkShaderModuleCreateInfo, vkAllocationCallbacks, &vkShaderModule) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to create the Vulkan shader module")
			}

			// Done
			RHI_FREE(spirvOutputBuffer);
			return vkShaderModule;
		}

		/**
		*  @brief
		*    Create Vulkan shader module from source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] vkShaderStageFlagBits
		*    Vulkan shader stage flag bits (only a single set bit allowed)
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be a valid pointer
		*  @param[out] shaderBytecode
		*    If not a null pointer, this receives the shader SPIR-V bytecode compressed via SMOL-V
		*
		*  @return
		*    The Vulkan shader module, null handle on error
		*/
		// TODO(co) Visual Studio 2017 compile settings: For some reasons I need to disable optimization for the following method or else "glslang::TShader::parse()" will output the error "ERROR: 0:1: '€' : unexpected token" (glslang (latest commit c325f4364666eedb94c20a13670df058a38a14ab - April 20, 2018), Visual Studio 2017 15.7.1)
#ifdef _MSC_VER
#pragma optimize("", off)
#endif
		[[nodiscard]] VkShaderModule createVkShaderModuleFromSourceCode(const Rhi::Context& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, VkShaderStageFlagBits vkShaderStageFlagBits, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode)
		{
#ifdef SE_GLSLTOSPIRV
			// Initialize glslang, if necessary
			if ( !GlslangInitialized )
			{
				glslang::InitializeProcess();
				GlslangInitialized = true;
			}

			// GLSL to intermediate
			// -> OpenGL 4.5
			static constexpr int glslVersion = 450;
			EShLanguage shLanguage = EShLangCount;
			if ( (vkShaderStageFlagBits & VK_SHADER_STAGE_VERTEX_BIT) != 0 )
			{
				shLanguage = EShLangVertex;
			}
			else if ( (vkShaderStageFlagBits & VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT) != 0 )
			{
				shLanguage = EShLangTessControl;
			}
			else if ( (vkShaderStageFlagBits & VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) != 0 )
			{
				shLanguage = EShLangTessEvaluation;
			}
			else if ( (vkShaderStageFlagBits & VK_SHADER_STAGE_GEOMETRY_BIT) != 0 )
			{
				shLanguage = EShLangGeometry;
			}
			else if ( (vkShaderStageFlagBits & VK_SHADER_STAGE_FRAGMENT_BIT) != 0 )
			{
				shLanguage = EShLangFragment;
			}
			else if ( (vkShaderStageFlagBits & VK_SHADER_STAGE_TASK_BIT_NV) != 0 )
			{
				shLanguage = EShLangTaskNV;
			}
			else if ( (vkShaderStageFlagBits & VK_SHADER_STAGE_MESH_BIT_NV) != 0 )
			{
				shLanguage = EShLangMeshNV;
			}
			else if ( (vkShaderStageFlagBits & VK_SHADER_STAGE_COMPUTE_BIT) != 0 )
			{
				shLanguage = EShLangCompute;
			}
			else
			{
				RHI_ASSERT(false, "Invalid Vulkan shader stage flag bits")
			}
			glslang::TShader shader(shLanguage);
			shader.setEnvInput(glslang::EShSourceGlsl, shLanguage, glslang::EShClientVulkan, glslVersion);
			shader.setEntryPoint("main");
			{
				const char* sourcePointers[] = { sourceCode };
				shader.setStrings(sourcePointers, 1);
			}
			static constexpr EShMessages shMessages = static_cast<EShMessages>(EShMsgDefault | EShMsgSpvRules | EShMsgVulkanRules);
			if ( shader.parse(&DefaultTBuiltInResource, glslVersion, false, shMessages) )
			{
				glslang::TProgram program;
				program.addShader(&shader);
				if ( program.link(shMessages) )
				{
					// Intermediate to SPIR-V
					const glslang::TIntermediate* intermediate = program.getIntermediate(shLanguage);
					if ( nullptr != intermediate )
					{
						std::vector<unsigned int> spirv;
						glslang::GlslangToSpv(*intermediate, spirv);

						// Optional shader bytecode output
						if ( nullptr != shaderBytecode )
						{
							// Encode to SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
							// -> https://github.com/aras-p/smol-v
							// -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
							// -> Don't apply "spv::spirvbin_t::remap()" or the SMOL-V result will be bigger
							smolv::ByteArray byteArray;
							smolv::Encode(spirv.data(), sizeof(unsigned int) * spirv.size(), byteArray, smolv::kEncodeFlagStripDebugInfo);

							// Done
							shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(byteArray.size()), reinterpret_cast<uint8_t*>(byteArray.data()));
						}

						// Create the Vulkan shader module
						const VkShaderModuleCreateInfo vkShaderModuleCreateInfo =
						{
							VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	// sType (VkStructureType)
							nullptr,										// pNext (const void*)
							0,												// flags (VkShaderModuleCreateFlags)
							sizeof(unsigned int) * spirv.size(),			// codeSize (size_t)
							spirv.data()									// pCode (const uint32_t*)
						};
						VkShaderModule vkShaderModule = VK_NULL_HANDLE;
						if ( vkCreateShaderModule(vkDevice, &vkShaderModuleCreateInfo, vkAllocationCallbacks, &vkShaderModule) != VK_SUCCESS )
						{
							RHI_LOG(CRITICAL, "Failed to create the Vulkan shader module")
						}
						return vkShaderModule;
					}
				}
				else
				{
					// Failed to link the program
					if ( GetLog().print(DefaultLog::Type::CRITICAL, sourceCode, __FILE__, static_cast<uint32_t>(__LINE__), "Failed to link the GLSL program: %s", program.getInfoLog()) )
					{
						SE_DEBUG_BREAK;
					}
				}
			}
			else
			{
				// Failed to parse the shader source code
				if ( GetLog().print(DefaultLog::Type::CRITICAL, sourceCode, __FILE__, static_cast<uint32_t>(__LINE__), "Failed to parse the GLSL shader source code: %s", shader.getInfoLog()) )
				{
					SE_DEBUG_BREAK;
				}
			}
#endif

			// Error!
			return VK_NULL_HANDLE;
		}
		// TODO(co) Visual Studio 2017 compile settings: For some reasons I need to disable optimization for the following method or else "glslang::TShader::parse()" will output the error "ERROR: 0:1: '€' : unexpected token" (glslang (latest commit c325f4364666eedb94c20a13670df058a38a14ab - April 20, 2018), Visual Studio 2017 15.7.1)
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}
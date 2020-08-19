#pragma once

namespace VulkanRhi
{
	//[-------------------------------------------------------]
	//[ VulkanRhi/Mapping.h                                   ]
	//[-------------------------------------------------------]
	class Mapping final
	{
		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Rhi::FilterMode                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::FilterMode" to Vulkan magnification filter mode
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] filterMode
		*    "Rhi::FilterMode" to map
		*
		*  @return
		*    Vulkan magnification filter mode
		*/
		[[nodiscard]] static VkFilter getVulkanMagFilterMode([[maybe_unused]] const Rhi::Context& context, Rhi::FilterMode filterMode)
		{
			switch ( filterMode )
			{
			case Rhi::FilterMode::MIN_MAG_MIP_POINT:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::MIN_MAG_MIP_LINEAR:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::ANISOTROPIC:
				return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::COMPARISON_ANISOTROPIC:
				return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

			case Rhi::FilterMode::UNKNOWN:
				RHI_ASSERT(false, "Vulkan filter mode must not be unknown")
					return VK_FILTER_NEAREST;

			default:
				return VK_FILTER_NEAREST;	// We should never be in here
			}
		}

		/**
		*  @brief
		*    "Rhi::FilterMode" to Vulkan minification filter mode
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] filterMode
		*    "Rhi::FilterMode" to map
		*
		*  @return
		*    Vulkan minification filter mode
		*/
		[[nodiscard]] static VkFilter getVulkanMinFilterMode([[maybe_unused]] const Rhi::Context& context, Rhi::FilterMode filterMode)
		{
			switch ( filterMode )
			{
			case Rhi::FilterMode::MIN_MAG_MIP_POINT:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::MIN_MAG_MIP_LINEAR:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::ANISOTROPIC:
				return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
				return VK_FILTER_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
				return VK_FILTER_LINEAR;

			case Rhi::FilterMode::COMPARISON_ANISOTROPIC:
				return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

			case Rhi::FilterMode::UNKNOWN:
				RHI_ASSERT(false, "Vulkan filter mode must not be unknown")
					return VK_FILTER_NEAREST;

			default:
				return VK_FILTER_NEAREST;	// We should never be in here
			}
		}

		/**
		*  @brief
		*    "Rhi::FilterMode" to Vulkan sampler mipmap mode
		*
		*  @param[in] context
		*    Rhi context to use
		*  @param[in] filterMode
		*    "Rhi::FilterMode" to map
		*
		*  @return
		*    Vulkan sampler mipmap mode
		*/
		[[nodiscard]] static VkSamplerMipmapMode getVulkanMipmapMode([[maybe_unused]] const Rhi::Context& context, Rhi::FilterMode filterMode)
		{
			switch ( filterMode )
			{
			case Rhi::FilterMode::MIN_MAG_MIP_POINT:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			case Rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;

			case Rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			case Rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;

			case Rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			case Rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;

			case Rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			case Rhi::FilterMode::MIN_MAG_MIP_LINEAR:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;

			case Rhi::FilterMode::ANISOTROPIC:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;	// There's no special setting in Vulkan

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;

			case Rhi::FilterMode::COMPARISON_ANISOTROPIC:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;	// There's no special setting in Vulkan

			case Rhi::FilterMode::UNKNOWN:
				RHI_ASSERT(false, "Vulkan filter mode must not be unknown")
					return VK_SAMPLER_MIPMAP_MODE_NEAREST;

			default:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;	// We should never be in here
			}
		}

		//[-------------------------------------------------------]
		//[ Rhi::TextureAddressMode                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::TextureAddressMode" to Vulkan texture address mode
		*
		*  @param[in] textureAddressMode
		*    "Rhi::TextureAddressMode" to map
		*
		*  @return
		*    Vulkan texture address mode
		*/
		[[nodiscard]] static VkSamplerAddressMode getVulkanTextureAddressMode(Rhi::TextureAddressMode textureAddressMode)
		{
			static constexpr VkSamplerAddressMode MAPPING[] =
			{
				VK_SAMPLER_ADDRESS_MODE_REPEAT,					// Rhi::TextureAddressMode::WRAP
				VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,		// Rhi::TextureAddressMode::MIRROR
				VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,			// Rhi::TextureAddressMode::CLAMP
				VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,		// Rhi::TextureAddressMode::BORDER
				VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE	// Rhi::TextureAddressMode::MIRROR_ONCE
			};
			return MAPPING[static_cast<int>(textureAddressMode) - 1];	// Lookout! The "Rhi::TextureAddressMode"-values start with 1, not 0
		}

		//[-------------------------------------------------------]
		//[ Rhi::Blend                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::Blend" to Vulkan blend factor
		*
		*  @param[in] blend
		*    "Rhi::Blend" to map
		*
		*  @return
		*    Vulkan blend factor
		*/
		[[nodiscard]] static VkBlendFactor getVulkanBlendFactor(Rhi::Blend blend)
		{
			static constexpr VkBlendFactor MAPPING[] =
			{
				VK_BLEND_FACTOR_ZERO,						// Rhi::Blend::ZERO				 = 1
				VK_BLEND_FACTOR_ONE,						// Rhi::Blend::ONE				 = 2
				VK_BLEND_FACTOR_SRC_COLOR,					// Rhi::Blend::SRC_COLOR		 = 3
				VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,		// Rhi::Blend::INV_SRC_COLOR	 = 4
				VK_BLEND_FACTOR_SRC_ALPHA,					// Rhi::Blend::SRC_ALPHA		 = 5
				VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,		// Rhi::Blend::INV_SRC_ALPHA	 = 6
				VK_BLEND_FACTOR_DST_ALPHA,					// Rhi::Blend::DEST_ALPHA		 = 7
				VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,		// Rhi::Blend::INV_DEST_ALPHA	 = 8
				VK_BLEND_FACTOR_DST_COLOR,					// Rhi::Blend::DEST_COLOR		 = 9
				VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,		// Rhi::Blend::INV_DEST_COLOR	 = 10
				VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,			// Rhi::Blend::SRC_ALPHA_SAT	 = 11
				VK_BLEND_FACTOR_MAX_ENUM,					// <undefined>					 = 12 !
				VK_BLEND_FACTOR_MAX_ENUM,					// <undefined>					 = 13 !
				VK_BLEND_FACTOR_CONSTANT_COLOR,				// Rhi::Blend::BLEND_FACTOR		 = 14
				VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,	// Rhi::Blend::INV_BLEND_FACTOR  = 15
				VK_BLEND_FACTOR_SRC1_COLOR,					// Rhi::Blend::SRC_1_COLOR		 = 16
				VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,		// Rhi::Blend::INV_SRC_1_COLOR	 = 17
				VK_BLEND_FACTOR_SRC1_ALPHA,					// Rhi::Blend::SRC_1_ALPHA		 = 18
				VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA		// Rhi::Blend::INV_SRC_1_ALPHA	 = 19
			};
			return MAPPING[static_cast<int>(blend) - 1];	// Lookout! The "Rhi::Blend"-values start with 1, not 0, there are also holes
		}

		/**
		*  @brief
		*    "Rhi::BlendOp" to Vulkan blend operation
		*
		*  @param[in] blendOp
		*    "Rhi::BlendOp" to map
		*
		*  @return
		*    Vulkan blend operation
		*/
		[[nodiscard]] static VkBlendOp getVulkanBlendOp(Rhi::BlendOp blendOp)
		{
			static constexpr VkBlendOp MAPPING[] =
			{
				VK_BLEND_OP_ADD,				// Rhi::BlendOp::ADD
				VK_BLEND_OP_SUBTRACT,			// Rhi::BlendOp::SUBTRACT
				VK_BLEND_OP_REVERSE_SUBTRACT,	// Rhi::BlendOp::REV_SUBTRACT
				VK_BLEND_OP_MIN,				// Rhi::BlendOp::MIN
				VK_BLEND_OP_MAX					// Rhi::BlendOp::MAX
			};
			return MAPPING[static_cast<int>(blendOp) - 1];	// Lookout! The "Rhi::Blend"-values start with 1, not 0
		}

		//[-------------------------------------------------------]
		//[ Rhi::ComparisonFunc                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::ComparisonFunc" to Vulkan comparison function
		*
		*  @param[in] comparisonFunc
		*    "Rhi::ComparisonFunc" to map
		*
		*  @return
		*    Vulkan comparison function
		*/
		[[nodiscard]] static VkCompareOp getVulkanComparisonFunc(Rhi::ComparisonFunc comparisonFunc)
		{
			static constexpr VkCompareOp MAPPING[] =
			{
				VK_COMPARE_OP_NEVER,			// Rhi::ComparisonFunc::NEVER
				VK_COMPARE_OP_LESS,				// Rhi::ComparisonFunc::LESS
				VK_COMPARE_OP_EQUAL,			// Rhi::ComparisonFunc::EQUAL
				VK_COMPARE_OP_LESS_OR_EQUAL,	// Rhi::ComparisonFunc::LESS_EQUAL
				VK_COMPARE_OP_GREATER,			// Rhi::ComparisonFunc::GREATER
				VK_COMPARE_OP_NOT_EQUAL,		// Rhi::ComparisonFunc::NOT_EQUAL
				VK_COMPARE_OP_GREATER_OR_EQUAL,	// Rhi::ComparisonFunc::GREATER_EQUAL
				VK_COMPARE_OP_ALWAYS			// Rhi::ComparisonFunc::ALWAYS
			};
			return MAPPING[static_cast<int>(comparisonFunc) - 1];	// Lookout! The "Rhi::ComparisonFunc"-values start with 1, not 0
		}

		//[-------------------------------------------------------]
		//[ Rhi::VertexAttributeFormat and semantic               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::VertexAttributeFormat" to Vulkan format
		*
		*  @param[in] vertexAttributeFormat
		*    "Rhi::VertexAttributeFormat" to map
		*
		*  @return
		*    Vulkan format
		*/
		[[nodiscard]] static VkFormat getVulkanFormat(Rhi::VertexAttributeFormat vertexAttributeFormat)
		{
			static constexpr VkFormat MAPPING[] =
			{
				VK_FORMAT_R32_SFLOAT,			// Rhi::VertexAttributeFormat::FLOAT_1
				VK_FORMAT_R32G32_SFLOAT,		// Rhi::VertexAttributeFormat::FLOAT_2
				VK_FORMAT_R32G32B32_SFLOAT,		// Rhi::VertexAttributeFormat::FLOAT_3
				VK_FORMAT_R32G32B32A32_SFLOAT,	// Rhi::VertexAttributeFormat::FLOAT_4
				VK_FORMAT_R8G8B8A8_UNORM,		// Rhi::VertexAttributeFormat::R8G8B8A8_UNORM
				VK_FORMAT_R8G8B8A8_UINT,		// Rhi::VertexAttributeFormat::R8G8B8A8_UINT
				VK_FORMAT_R16G16_SINT,			// Rhi::VertexAttributeFormat::SHORT_2
				VK_FORMAT_R16G16B16A16_SINT,	// Rhi::VertexAttributeFormat::SHORT_4
				VK_FORMAT_R32_UINT				// Rhi::VertexAttributeFormat::UINT_1
			};
			return MAPPING[static_cast<int>(vertexAttributeFormat)];
		}

		//[-------------------------------------------------------]
		//[ Rhi::IndexBufferFormat                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::IndexBufferFormat" to Vulkan type
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] indexBufferFormat
		*    "Rhi::IndexBufferFormat" to map
		*
		*  @return
		*    Vulkan index type
		*/
		[[nodiscard]] static VkIndexType getVulkanType([[maybe_unused]] const Rhi::Context& context, Rhi::IndexBufferFormat::Enum indexBufferFormat)
		{
			RHI_ASSERT(Rhi::IndexBufferFormat::UNSIGNED_CHAR != indexBufferFormat, "One byte per element index buffer format isn't supported by Vulkan")
				static constexpr VkIndexType MAPPING[] =
			{
				VK_INDEX_TYPE_MAX_ENUM,	// Rhi::IndexBufferFormat::UNSIGNED_CHAR  - One byte per element, uint8_t (may not be supported by each API) - Not supported by Vulkan
				VK_INDEX_TYPE_UINT16,	// Rhi::IndexBufferFormat::UNSIGNED_SHORT - Two bytes per element, uint16_t
				VK_INDEX_TYPE_UINT32	// Rhi::IndexBufferFormat::UNSIGNED_INT   - Four bytes per element, uint32_t (may not be supported by each API)
			};
			return MAPPING[indexBufferFormat];
		}

		//[-------------------------------------------------------]
		//[ Rhi::PrimitiveTopology                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::PrimitiveTopology" to Vulkan type
		*
		*  @param[in] primitiveTopology
		*    "Rhi::PrimitiveTopology" to map
		*
		*  @return
		*    Vulkan type
		*/
		[[nodiscard]] static VkPrimitiveTopology getVulkanType(Rhi::PrimitiveTopology primitiveTopology)
		{
			// Tessellation support: Up to 32 vertices per patch are supported "Rhi::PrimitiveTopology::PATCH_LIST_1" ... "Rhi::PrimitiveTopology::PATCH_LIST_32"
			if ( primitiveTopology >= Rhi::PrimitiveTopology::PATCH_LIST_1 )
			{
				// Use tessellation
				return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
			}
			else
			{
				static constexpr VkPrimitiveTopology MAPPING[] =
				{
					VK_PRIMITIVE_TOPOLOGY_POINT_LIST,		// Rhi::PrimitiveTopology::POINT_LIST
					VK_PRIMITIVE_TOPOLOGY_LINE_LIST,		// Rhi::PrimitiveTopology::LINE_LIST
					VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,		// Rhi::PrimitiveTopology::LINE_STRIP
					VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,	// Rhi::PrimitiveTopology::TRIANGLE_LIST
					VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP	// Rhi::PrimitiveTopology::TRIANGLE_STRIP
				};
				return MAPPING[static_cast<int>(primitiveTopology) - 1];	// Lookout! The "Rhi::PrimitiveTopology"-values start with 1, not 0
			}
		}

		//[-------------------------------------------------------]
		//[ Rhi::TextureFormat                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::TextureFormat" to Vulkan format
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to map
		*
		*  @return
		*    Vulkan format
		*/
		[[nodiscard]] static VkFormat getVulkanFormat(Rhi::TextureFormat::Enum textureFormat)
		{
			static constexpr VkFormat MAPPING[] =
			{
				VK_FORMAT_R8_UNORM,					// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				VK_FORMAT_R8G8B8_UNORM,				// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				VK_FORMAT_R8G8B8A8_UNORM,			// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				VK_FORMAT_R8G8B8A8_SRGB,			// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				VK_FORMAT_B8G8R8A8_UNORM,			// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				VK_FORMAT_B10G11R11_UFLOAT_PACK32,	// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				VK_FORMAT_R16G16B16A16_SFLOAT,		// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				VK_FORMAT_R32G32B32A32_SFLOAT,		// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				VK_FORMAT_BC1_RGB_UNORM_BLOCK,		// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
				VK_FORMAT_BC1_RGB_SRGB_BLOCK,		// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				VK_FORMAT_BC2_UNORM_BLOCK,			// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				VK_FORMAT_BC2_SRGB_BLOCK,			// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				VK_FORMAT_BC3_UNORM_BLOCK,			// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				VK_FORMAT_BC3_SRGB_BLOCK,			// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				VK_FORMAT_BC4_UNORM_BLOCK,			// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
				VK_FORMAT_BC5_UNORM_BLOCK,			// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
				VK_FORMAT_UNDEFINED,				// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in Direct3D 11 - TODO(co) Check for Vulkan format
				VK_FORMAT_R16_UNORM,				// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				VK_FORMAT_R32_UINT,					// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				VK_FORMAT_R32_SFLOAT,				// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				VK_FORMAT_D32_SFLOAT,				// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				VK_FORMAT_R16G16_UNORM,				// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				VK_FORMAT_R16G16_SFLOAT,			// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				VK_FORMAT_UNDEFINED					// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		/**
		*  @brief
		*    Number of multisamples to Vulkan sample count flag bits
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*
		*  @return
		*    Vulkan sample count flag bits
		*/
		[[nodiscard]] static VkSampleCountFlagBits getVulkanSampleCountFlagBits([[maybe_unused]] const Rhi::Context& context, uint8_t numberOfMultisamples)
		{
			RHI_ASSERT(numberOfMultisamples <= 8, "Invalid number of Vulkan multisamples")
				static constexpr VkSampleCountFlagBits MAPPING[] =
			{
				VK_SAMPLE_COUNT_1_BIT,
				VK_SAMPLE_COUNT_2_BIT,
				VK_SAMPLE_COUNT_4_BIT,
				VK_SAMPLE_COUNT_8_BIT
			};
			return MAPPING[numberOfMultisamples - 1];	// Lookout! The "numberOfMultisamples"-values start with 1, not 0
		}


	};


	//[-------------------------------------------------------]
	//[ VulkanRhi/Helper.h                                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan helper
	*/
	class Helper final
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Command                                               ]
		//[-------------------------------------------------------]
		[[nodiscard]] static VkCommandBuffer beginSingleTimeCommands(const VulkanRhi& vulkanRhi)
		{
			// Create and begin Vulkan command buffer
			VkCommandBuffer vkCommandBuffer = vulkanRhi.getVulkanContext().createVkCommandBuffer();
			static constexpr VkCommandBufferBeginInfo vkCommandBufferBeginInfo =
			{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	// sType (VkStructureType)
				nullptr,										// pNext (const void*)
				0,												// flags (VkCommandBufferUsageFlags)
				nullptr											// pInheritanceInfo (const VkCommandBufferInheritanceInfo*)
			};
			if ( vkBeginCommandBuffer(vkCommandBuffer, &vkCommandBufferBeginInfo) == VK_SUCCESS )
			{
				// Done
				return vkCommandBuffer;
			}
			else
			{
				// Error!
				RHI_LOG(CRITICAL, "Failed to begin Vulkan command buffer instance")
					return VK_NULL_HANDLE;
			}
		}

		static void endSingleTimeCommands(const VulkanRhi& vulkanRhi, VkCommandBuffer vkCommandBuffer)
		{
			const VulkanContext& vulkanContext = vulkanRhi.getVulkanContext();
			const VkQueue vkQueue = vulkanContext.getGraphicsVkQueue();

			// End Vulkan command buffer
			vkEndCommandBuffer(vkCommandBuffer);

			// Submit Vulkan command buffer
			const VkSubmitInfo vkSubmitInfo =
			{
				VK_STRUCTURE_TYPE_SUBMIT_INFO,	// sType (VkStructureType)
				nullptr,						// pNext (const void*)
				0,								// waitSemaphoreCount (uint32_t)
				nullptr,						// pWaitSemaphores (const VkSemaphore*)
				nullptr,						// pWaitDstStageMask (const VkPipelineStageFlags*)
				1,								// commandBufferCount (uint32_t)
				&vkCommandBuffer,				// pCommandBuffers (const VkCommandBuffer*)
				0,								// signalSemaphoreCount (uint32_t)
				nullptr							// pSignalSemaphores (const VkSemaphore*)
			};
			if ( vkQueueSubmit(vkQueue, 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS )
			{
				// Error!
				RHI_LOG(CRITICAL, "Vulkan queue submit failed")
					return;
			}
			if ( vkQueueWaitIdle(vkQueue) != VK_SUCCESS )
			{
				// Error!
				RHI_LOG(CRITICAL, "Vulkan Queue wait idle failed")
					return;
			}

			// Destroy Vulkan command buffer
			vulkanContext.destroyVkCommandBuffer(vkCommandBuffer);
		}

		//[-------------------------------------------------------]
		//[ Transition                                            ]
		//[-------------------------------------------------------]
		static void transitionVkImageLayout(const VulkanRhi& vulkanRhi, VkImage vkImage, VkImageAspectFlags vkImageAspectFlags, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout)
		{
			// Create and begin Vulkan command buffer
			VkCommandBuffer vkCommandBuffer = beginSingleTimeCommands(vulkanRhi);

			// Vulkan image memory barrier
			transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, 1, 1, oldVkImageLayout, newVkImageLayout);

			// End and destroy Vulkan command buffer
			endSingleTimeCommands(vulkanRhi, vkCommandBuffer);
		}

		static void transitionVkImageLayout(const VulkanRhi& vulkanRhi, VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageAspectFlags vkImageAspectFlags, uint32_t levelCount, uint32_t layerCount, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout)
		{
			VkImageMemoryBarrier vkImageMemoryBarrier =
			{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,	// sType (VkStructureType)
				nullptr,								// pNext (const void*)
				0,										// srcAccessMask (VkAccessFlags)
				0,										// dstAccessMask (VkAccessFlags)
				oldVkImageLayout,						// oldLayout (VkImageLayout)
				newVkImageLayout,						// newLayout (VkImageLayout)
				VK_QUEUE_FAMILY_IGNORED,				// srcQueueFamilyIndex (uint32_t)
				VK_QUEUE_FAMILY_IGNORED,				// dstQueueFamilyIndex (uint32_t)
				vkImage,								// image (VkImage)
				{ // subresourceRange (VkImageSubresourceRange)
					vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
					0,					// baseMipLevel (uint32_t)
					levelCount,			// levelCount (uint32_t)
					0,					// baseArrayLayer (uint32_t)
					layerCount			// layerCount (uint32_t)
				}
			};

			// "srcAccessMask" and "dstAccessMask" configuration
			VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			if ( VK_IMAGE_LAYOUT_PREINITIALIZED == oldVkImageLayout && VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == newVkImageLayout )
			{
				vkImageMemoryBarrier.srcAccessMask = 0;
				vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if ( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == oldVkImageLayout && VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL == newVkImageLayout )
			{
				vkImageMemoryBarrier.srcAccessMask = 0;
				vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else if ( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == oldVkImageLayout && VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL == newVkImageLayout )
			{
				vkImageMemoryBarrier.srcAccessMask = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				vkImageMemoryBarrier.dstAccessMask = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				srcStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
				dstStageMask = VK_PIPELINE_STAGE_HOST_BIT;
			}
			else if ( VK_IMAGE_LAYOUT_UNDEFINED == oldVkImageLayout && VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL == newVkImageLayout )
			{
				vkImageMemoryBarrier.srcAccessMask = 0;
				vkImageMemoryBarrier.dstAccessMask = (VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
			}
			else
			{
				RHI_LOG(CRITICAL, "Unsupported Vulkan image layout transition")
			}

			// Create Vulkan pipeline barrier command
			vkCmdPipelineBarrier(vkCommandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &vkImageMemoryBarrier);
		}

		static void transitionVkImageLayout(const VulkanRhi& vulkanRhi, VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout, VkImageSubresourceRange vkImageSubresourceRange, VkPipelineStageFlags sourceVkPipelineStageFlags, VkPipelineStageFlags destinationVkPipelineStageFlags)
		{
			// Basing on https://github.com/SaschaWillems/Vulkan/tree/master

			// Create an image barrier object
			VkImageMemoryBarrier vkImageMemoryBarrier =
			{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,	// sType (VkStructureType)
				nullptr,								// pNext (const void*)
				0,										// srcAccessMask (VkAccessFlags)
				0,										// dstAccessMask (VkAccessFlags)
				oldVkImageLayout,						// oldLayout (VkImageLayout)
				newVkImageLayout,						// newLayout (VkImageLayout)
				VK_QUEUE_FAMILY_IGNORED,				// srcQueueFamilyIndex (uint32_t)
				VK_QUEUE_FAMILY_IGNORED,				// dstQueueFamilyIndex (uint32_t)
				vkImage,								// image (VkImage)
				vkImageSubresourceRange					// subresourceRange (VkImageSubresourceRange)
			};

			// Source layouts (old)
			// -> Source access mask controls actions that have to be finished on the old layout before it will be transitioned to the new layout
			switch ( oldVkImageLayout )
			{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				// Image layout is undefined (or does not matter)
				// Only valid as initial layout
				// No flags required, listed only for completeness
				vkImageMemoryBarrier.srcAccessMask = 0;
				break;

			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				// Image is preinitialized
				// Only valid as initial layout for linear images, preserves memory contents
				// Make sure host writes have been finished
				vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				// Image is a color attachment
				// Make sure any writes to the color buffer have been finished
				vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				// Image is a depth/stencil attachment
				// Make sure any writes to the depth/stencil buffer have been finished
				vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				// Image is a transfer source 
				// Make sure any reads from the image have been finished
				vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				// Image is a transfer destination
				// Make sure any writes to the image have been finished
				vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				// Image is read by a shader
				// Make sure any shader reads from the image have been finished
				vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_GENERAL:
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
			case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
			case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
			case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
			case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
			case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
				// case VK_IMAGE_LAYOUT_BEGIN_RANGE:	not possible
				// case VK_IMAGE_LAYOUT_END_RANGE:		not possible
			case VK_IMAGE_LAYOUT_MAX_ENUM:
			default:
				// Other source layouts aren't handled (yet)
				RHI_LOG(CRITICAL, "Unsupported Vulkan image old layout transition")
					break;
			}

			// Target layouts (new)
			// -> Destination access mask controls the dependency for the new image layout
			switch ( newVkImageLayout )
			{
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				// Image will be used as a transfer destination
				// Make sure any writes to the image have been finished
				vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				// Image will be used as a transfer source
				// Make sure any reads from the image have been finished
				vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				// Image will be used as a color attachment
				// Make sure any writes to the color buffer have been finished
				vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				// Image layout will be used as a depth/stencil attachment
				// Make sure any writes to depth/stencil buffer have been finished
				vkImageMemoryBarrier.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				// Image will be read in a shader (sampler, input attachment)
				// Make sure any writes to the image have been finished
				if ( vkImageMemoryBarrier.srcAccessMask == 0 )
				{
					vkImageMemoryBarrier.srcAccessMask = (VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
				}
				vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_UNDEFINED:
			case VK_IMAGE_LAYOUT_GENERAL:
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			case VK_IMAGE_LAYOUT_PREINITIALIZED:
			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
			case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
			case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
			case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
			case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
			case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
				// case VK_IMAGE_LAYOUT_BEGIN_RANGE:	not possible
				// case VK_IMAGE_LAYOUT_END_RANGE:		not possible
			case VK_IMAGE_LAYOUT_MAX_ENUM:
			default:
				// Other source layouts aren't handled (yet)
				RHI_LOG(CRITICAL, "Unsupported Vulkan image new layout transition")
					break;
			}

			// Put barrier inside setup command buffer
			// -> "Table 4. Supported access types": https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#synchronization-access-types-supported
			vkCmdPipelineBarrier(vkCommandBuffer, sourceVkPipelineStageFlags, destinationVkPipelineStageFlags, 0, 0, nullptr, 0, nullptr, 1, &vkImageMemoryBarrier);
		}

		//[-------------------------------------------------------]
		//[ Buffer                                                ]
		//[-------------------------------------------------------]
		// TODO(co) Trivial implementation to have something to start with. Need to use more clever memory management and stating buffers later on.
		static void createAndAllocateVkBuffer(const VulkanRhi& vulkanRhi, VkBufferUsageFlagBits vkBufferUsageFlagBits, VkMemoryPropertyFlags vkMemoryPropertyFlags, VkDeviceSize numberOfBytes, const void* data, VkBuffer& vkBuffer, VkDeviceMemory& vkDeviceMemory)
		{
			const VulkanContext& vulkanContext = vulkanRhi.getVulkanContext();
			const VkDevice vkDevice = vulkanContext.getVkDevice();

			// Create the Vulkan buffer
			const VkBufferCreateInfo vkBufferCreateInfo =
			{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,					// sType (VkStructureType)
				nullptr,												// pNext (const void*)
				0,														// flags (VkBufferCreateFlags)
				numberOfBytes,											// size (VkDeviceSize)
				static_cast<VkBufferUsageFlags>(vkBufferUsageFlagBits),	// usage (VkBufferUsageFlags)
				VK_SHARING_MODE_EXCLUSIVE,								// sharingMode (VkSharingMode)
				0,														// queueFamilyIndexCount (uint32_t)
				nullptr													// pQueueFamilyIndices (const uint32_t*)
			};
			if ( vkCreateBuffer(vkDevice, &vkBufferCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkBuffer) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to create the Vulkan buffer")
			}

			// Allocate memory for the Vulkan buffer
			VkMemoryRequirements vkMemoryRequirements = {};
			vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &vkMemoryRequirements);
			const VkMemoryAllocateInfo vkMemoryAllocateInfo =
			{
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,															// sType (VkStructureType)
				nullptr,																						// pNext (const void*)
				vkMemoryRequirements.size,																		// allocationSize (VkDeviceSize)
				vulkanContext.findMemoryTypeIndex(vkMemoryRequirements.memoryTypeBits, vkMemoryPropertyFlags)	// memoryTypeIndex (uint32_t)
			};
			if ( vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkDeviceMemory) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to allocate the Vulkan buffer memory")
			}

			// Bind and fill memory
			vkBindBufferMemory(vkDevice, vkBuffer, vkDeviceMemory, 0);
			if ( nullptr != data )
			{
				void* mappedData = nullptr;
				if ( vkMapMemory(vkDevice, vkDeviceMemory, 0, vkBufferCreateInfo.size, 0, &mappedData) == VK_SUCCESS )
				{
					memcpy(mappedData, data, static_cast<size_t>(vkBufferCreateInfo.size));
					vkUnmapMemory(vkDevice, vkDeviceMemory);
				}
				else
				{
					RHI_LOG(CRITICAL, "Failed to map the Vulkan memory")
				}
			}
		}

		static void destroyAndFreeVkBuffer(const VulkanRhi& vulkanRhi, VkBuffer& vkBuffer, VkDeviceMemory& vkDeviceMemory)
		{
			if ( VK_NULL_HANDLE != vkBuffer )
			{
				const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				vkDestroyBuffer(vkDevice, vkBuffer, vulkanRhi.getVkAllocationCallbacks());
				if ( VK_NULL_HANDLE != vkDeviceMemory )
				{
					vkFreeMemory(vkDevice, vkDeviceMemory, vulkanRhi.getVkAllocationCallbacks());
				}
			}
		}

		//[-------------------------------------------------------]
		//[ Image                                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] static VkImageLayout getVkImageLayoutByTextureFlags(uint32_t textureFlags)
		{
			if ( textureFlags & Rhi::TextureFlag::RENDER_TARGET )
			{
				return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			}
			else if ( textureFlags & Rhi::TextureFlag::UNORDERED_ACCESS )
			{
				return VK_IMAGE_LAYOUT_GENERAL;
			}
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}

		// TODO(co) Trivial implementation to have something to start with. Need to use more clever memory management and stating buffers later on.
		static VkFormat createAndFillVkImage(const VulkanRhi& vulkanRhi, VkImageType vkImageType, VkImageViewType vkImageViewType, const VkExtent3D& vkExtent3D, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags, uint8_t numberOfMultisamples, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory, VkImageView& vkImageView)
		{
			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? Rhi::ITexture::getNumberOfMipmaps(vkExtent3D.width, vkExtent3D.height) : 1;

			// Get Vulkan image usage flags
			RHI_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "Vulkan render target textures can't be filled using provided data")
				const bool isDepthTextureFormat = Rhi::TextureFormat::isDepth(textureFormat);
			VkImageUsageFlags vkImageUsageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			if ( textureFlags & Rhi::TextureFlag::SHADER_RESOURCE )
			{
				vkImageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;
			}
			if ( textureFlags & Rhi::TextureFlag::UNORDERED_ACCESS )
			{
				vkImageUsageFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
			}
			if ( textureFlags & Rhi::TextureFlag::RENDER_TARGET )
			{
				if ( isDepthTextureFormat )
				{
					vkImageUsageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				}
				else
				{
					vkImageUsageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				}
			}
			if ( generateMipmaps )
			{
				vkImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			}

			// Get Vulkan format
			const VkFormat vkFormat = Mapping::getVulkanFormat(textureFormat);
			const bool     layered = (VK_IMAGE_VIEW_TYPE_2D_ARRAY == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE_ARRAY == vkImageViewType);
			const uint32_t layerCount = layered ? vkExtent3D.depth : 1;
			const uint32_t depth = layered ? 1 : vkExtent3D.depth;
			const VkSampleCountFlagBits vkSampleCountFlagBits = Mapping::getVulkanSampleCountFlagBits(vulkanRhi.getContext(), numberOfMultisamples);
			VkImageAspectFlags vkImageAspectFlags = isDepthTextureFormat ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
			if ( ::detail::hasVkFormatStencilComponent(vkFormat) )
			{
				vkImageAspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}

			// Calculate the number of bytes
			uint32_t numberOfBytes = 0;
			if ( dataContainsMipmaps )
			{
				uint32_t currentWidth = vkExtent3D.width;
				uint32_t currentHeight = vkExtent3D.height;
				uint32_t currentDepth = depth;
				for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
				{
					numberOfBytes += Rhi::TextureFormat::getNumberOfBytesPerSlice(static_cast<Rhi::TextureFormat::Enum>(textureFormat), currentWidth, currentHeight) * currentDepth;
					currentWidth = Rhi::ITexture::getHalfSize(currentWidth);
					currentHeight = Rhi::ITexture::getHalfSize(currentHeight);
					currentDepth = Rhi::ITexture::getHalfSize(currentDepth);
				}
				numberOfBytes *= vkExtent3D.depth;
			}
			else
			{
				numberOfBytes = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, vkExtent3D.width, vkExtent3D.height) * vkExtent3D.depth;
			}

			{ // Create and fill Vulkan image
				const VkImageCreateFlags vkImageCreateFlags = (VK_IMAGE_VIEW_TYPE_CUBE == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE_ARRAY == vkImageViewType) ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u;
				createAndAllocateVkImage(vulkanRhi, vkImageCreateFlags, vkImageType, VkExtent3D{ vkExtent3D.width, vkExtent3D.height, depth }, numberOfMipmaps, layerCount, vkFormat, vkSampleCountFlagBits, VK_IMAGE_TILING_OPTIMAL, vkImageUsageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkImage, vkDeviceMemory);
			}

			// Create the Vulkan image view
			if ( (textureFlags & Rhi::TextureFlag::SHADER_RESOURCE) != 0 || (textureFlags & Rhi::TextureFlag::RENDER_TARGET) != 0 || (textureFlags & Rhi::TextureFlag::UNORDERED_ACCESS) != 0 )
			{
				createVkImageView(vulkanRhi, vkImage, vkImageViewType, numberOfMipmaps, layerCount, vkFormat, vkImageAspectFlags, vkImageView);
			}

			// Upload all mipmaps
			if ( nullptr != data )
			{
				// Create Vulkan staging buffer
				VkBuffer stagingVkBuffer = VK_NULL_HANDLE;
				VkDeviceMemory stagingVkDeviceMemory = VK_NULL_HANDLE;
				createAndAllocateVkBuffer(vulkanRhi, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, stagingVkBuffer, stagingVkDeviceMemory);

				{ // Upload all mipmaps
					const uint32_t numberOfUploadedMipmaps = generateMipmaps ? 1 : numberOfMipmaps;

					// Create and begin Vulkan command buffer
					VkCommandBuffer vkCommandBuffer = beginSingleTimeCommands(vulkanRhi);
					transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, numberOfUploadedMipmaps, layerCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

					// Upload all mipmaps
					uint32_t bufferOffset = 0;
					uint32_t currentWidth = vkExtent3D.width;
					uint32_t currentHeight = vkExtent3D.height;
					uint32_t currentDepth = depth;

					// Allocate list of VkBufferImageCopy and setup VkBufferImageCopy data for each mipmap level
					std::vector<VkBufferImageCopy> vkBufferImageCopyList;
					vkBufferImageCopyList.reserve(numberOfUploadedMipmaps);
					for ( uint32_t mipmap = 0; mipmap < numberOfUploadedMipmaps; ++mipmap )
					{
						vkBufferImageCopyList.push_back({
							bufferOffset,									// bufferOffset (VkDeviceSize)
							0,												// bufferRowLength (uint32_t)
							0,												// bufferImageHeight (uint32_t)
							{ // imageSubresource (VkImageSubresourceLayers)
								vkImageAspectFlags,							// aspectMask (VkImageAspectFlags)
								mipmap,										// mipLevel (uint32_t)
								0,											// baseArrayLayer (uint32_t)
								layerCount									// layerCount (uint32_t)
							},
							{ 0, 0, 0 },									// imageOffset (VkOffset3D)
							{ currentWidth, currentHeight, currentDepth }	// imageExtent (VkExtent3D)
							});

						// Move on to the next mipmap
						bufferOffset += Rhi::TextureFormat::getNumberOfBytesPerSlice(static_cast<Rhi::TextureFormat::Enum>(textureFormat), currentWidth, currentHeight) * currentDepth;
						currentWidth = Rhi::ITexture::getHalfSize(currentWidth);
						currentHeight = Rhi::ITexture::getHalfSize(currentHeight);
						currentDepth = Rhi::ITexture::getHalfSize(currentDepth);
					}

					// Copy Vulkan buffer to Vulkan image
					vkCmdCopyBufferToImage(vkCommandBuffer, stagingVkBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(vkBufferImageCopyList.size()), vkBufferImageCopyList.data());

					// End and destroy Vulkan command buffer
					if ( generateMipmaps )
					{
						const VkImageSubresourceRange vkImageSubresourceRange =
						{
							vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
							0,					// baseMipLevel (uint32_t)
							1,					// levelCount (uint32_t)
							0,					// baseArrayLayer (uint32_t)
							layerCount			// layerCount (uint32_t)
						};
						transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImageSubresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
					}
					else
					{
						transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, numberOfUploadedMipmaps, layerCount, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
					}
					endSingleTimeCommands(vulkanRhi, vkCommandBuffer);
				}

				// Destroy Vulkan staging buffer
				destroyAndFreeVkBuffer(vulkanRhi, stagingVkBuffer, stagingVkDeviceMemory);

				// Generate a complete texture mip-chain at runtime from a base image using image blits and proper image barriers
				// -> Basing on https://github.com/SaschaWillems/Vulkan/tree/master/examples/texturemipmapgen and "Mipmap generation : Transfers, transition layout" by Antoine MORRIER published January 12, 2017 at http://cpp-rendering.io/mipmap-generation/
				// -> We copy down the whole mip chain doing a blit from mip-1 to mip. An alternative way would be to always blit from the first mip level and sample that one down.
				// TODO(co) Some GPUs also offer "asynchronous transfer queues" (check for queue families with only the "VK_QUEUE_TRANSFER_BIT" set) that may be used to speed up such operations
				if ( generateMipmaps )
				{
#if SE_DEBUG
					{
						// Get device properties for the requested Vulkan texture format
						VkFormatProperties vkFormatProperties;
						vkGetPhysicalDeviceFormatProperties(vulkanRhi.getVulkanContext().getVkPhysicalDevice(), vkFormat, &vkFormatProperties);

						// Mip-chain generation requires support for blit source and destination
						RHI_ASSERT(vkFormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT, "Invalid Vulkan optimal tiling features")
							RHI_ASSERT(vkFormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT, "Invalid Vulkan optimal tiling features")
					}
#endif

					// Create and begin Vulkan command buffer
					VkCommandBuffer vkCommandBuffer = beginSingleTimeCommands(vulkanRhi);

					// Copy down mips from n-1 to n
					for ( uint32_t i = 1; i < numberOfMipmaps; ++i )
					{
						const VkImageBlit VkImageBlit =
						{
							{ // srcSubresource (VkImageSubresourceLayers)
								vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
								i - 1,				// mipLevel (uint32_t)
								0,					// baseArrayLayer (uint32_t)
								layerCount			// layerCount (uint32_t)
							},
							{ // srcOffsets[2] (VkOffset3D)
								{ 0, 0, 0 },
								{ std::max(int32_t(vkExtent3D.width >> (i - 1)), 1), std::max(int32_t(vkExtent3D.height >> (i - 1)), 1), 1 }
							},
							{ // dstSubresource (VkImageSubresourceLayers)
								vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
								i,					// mipLevel (uint32_t)
								0,					// baseArrayLayer (uint32_t)
								layerCount			// layerCount (uint32_t)
							},
							{ // dstOffsets[2] (VkOffset3D)
								{ 0, 0, 0 },
								{ std::max(int32_t(vkExtent3D.width >> i), 1), std::max(int32_t(vkExtent3D.height >> i), 1), 1 }
							}
						};
						const VkImageSubresourceRange vkImageSubresourceRange =
						{
							vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
							i,					// baseMipLevel (uint32_t)
							1,					// levelCount (uint32_t)
							0,					// baseArrayLayer (uint32_t)
							layerCount			// layerCount (uint32_t)
						};

						// Transition current mip level to transfer destination
						transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vkImageSubresourceRange, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

						// Blit from previous level
						vkCmdBlitImage(vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &VkImageBlit, VK_FILTER_LINEAR);

						// Transition current mip level to transfer source for read in next iteration
						transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImageSubresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
					}

					{ // After the loop, all mip layers are in "VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL"-layout, so transition all to "VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL"-layout
						const VkImageSubresourceRange vkImageSubresourceRange =
						{
							vkImageAspectFlags,		// aspectMask (VkImageAspectFlags)
							0,						// baseMipLevel (uint32_t)
							numberOfMipmaps,		// levelCount (uint32_t)
							0,						// baseArrayLayer (uint32_t)
							layerCount				// layerCount (uint32_t)
						};
						transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, vkImageSubresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
					}

					// End and destroy Vulkan command buffer
					endSingleTimeCommands(vulkanRhi, vkCommandBuffer);
				}
			}

			// Done
			return vkFormat;
		}

		static void createAndAllocateVkImage(const VulkanRhi& vulkanRhi, VkImageCreateFlags vkImageCreateFlags, VkImageType vkImageType, const VkExtent3D& vkExtent3D, uint32_t mipLevels, uint32_t arrayLayers, VkFormat vkFormat, VkSampleCountFlagBits vkSampleCountFlagBits, VkImageTiling vkImageTiling, VkImageUsageFlags vkImageUsageFlags, VkMemoryPropertyFlags vkMemoryPropertyFlags, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory)
		{
			const VulkanContext& vulkanContext = vulkanRhi.getVulkanContext();
			const VkDevice vkDevice = vulkanContext.getVkDevice();

			{ // Create Vulkan image
				const VkImageCreateInfo vkImageCreateInfo =
				{
					VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,	// sType (VkStructureType)
					nullptr,								// pNext (const void*)
					vkImageCreateFlags,						// flags (VkImageCreateFlags)
					vkImageType,							// imageType (VkImageType)
					vkFormat,								// format (VkFormat)
					vkExtent3D,								// extent (VkExtent3D)
					mipLevels,								// mipLevels (uint32_t)
					arrayLayers,							// arrayLayers (uint32_t)
					vkSampleCountFlagBits,					// samples (VkSampleCountFlagBits)
					vkImageTiling,							// tiling (VkImageTiling)
					vkImageUsageFlags,						// usage (VkImageUsageFlags)
					VK_SHARING_MODE_EXCLUSIVE,				// sharingMode (VkSharingMode)
					0,										// queueFamilyIndexCount (uint32_t)
					nullptr,								// pQueueFamilyIndices (const uint32_t*)
					VK_IMAGE_LAYOUT_PREINITIALIZED			// initialLayout (VkImageLayout)
				};
				if ( vkCreateImage(vkDevice, &vkImageCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkImage) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to create the Vulkan image")
				}
			}

			{ // Allocate Vulkan memory
				VkMemoryRequirements vkMemoryRequirements = {};
				vkGetImageMemoryRequirements(vkDevice, vkImage, &vkMemoryRequirements);
				const VkMemoryAllocateInfo vkMemoryAllocateInfo =
				{
					VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,															// sType (VkStructureType)
					nullptr,																						// pNext (const void*)
					vkMemoryRequirements.size,																		// allocationSize (VkDeviceSize)
					vulkanContext.findMemoryTypeIndex(vkMemoryRequirements.memoryTypeBits, vkMemoryPropertyFlags)	// memoryTypeIndex (uint32_t)
				};
				if ( vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkDeviceMemory) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to allocate the Vulkan memory")
				}
				if ( vkBindImageMemory(vkDevice, vkImage, vkDeviceMemory, 0) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to bind the Vulkan image memory")
				}
			}
		}

		static void destroyAndFreeVkImage(const VulkanRhi& vulkanRhi, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory)
		{
			if ( VK_NULL_HANDLE != vkImage )
			{
				const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				vkDestroyImage(vkDevice, vkImage, vulkanRhi.getVkAllocationCallbacks());
				vkImage = VK_NULL_HANDLE;
				if ( VK_NULL_HANDLE != vkDeviceMemory )
				{
					vkFreeMemory(vkDevice, vkDeviceMemory, vulkanRhi.getVkAllocationCallbacks());
					vkDeviceMemory = VK_NULL_HANDLE;
				}
			}
		}

		static void destroyAndFreeVkImage(const VulkanRhi& vulkanRhi, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory, VkImageView& vkImageView)
		{
			if ( VK_NULL_HANDLE != vkImageView )
			{
				vkDestroyImageView(vulkanRhi.getVulkanContext().getVkDevice(), vkImageView, vulkanRhi.getVkAllocationCallbacks());
				vkImageView = VK_NULL_HANDLE;
			}
			destroyAndFreeVkImage(vulkanRhi, vkImage, vkDeviceMemory);
		}

		static void createVkImageView(const VulkanRhi& vulkanRhi, VkImage vkImage, VkImageViewType vkImageViewType, uint32_t levelCount, uint32_t layerCount, VkFormat vkFormat, VkImageAspectFlags vkImageAspectFlags, VkImageView& vkImageView)
		{
			const VkImageViewCreateInfo vkImageViewCreateInfo =
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	// sType (VkStructureType)
				nullptr,									// pNext (const void*)
				0,											// flags (VkImageViewCreateFlags)
				vkImage,									// image (VkImage)
				vkImageViewType,							// viewType (VkImageViewType)
				vkFormat,									// format (VkFormat)
				{ // components (VkComponentMapping)
					VK_COMPONENT_SWIZZLE_IDENTITY,			// r (VkComponentSwizzle)
					VK_COMPONENT_SWIZZLE_IDENTITY,			// g (VkComponentSwizzle)
					VK_COMPONENT_SWIZZLE_IDENTITY,			// b (VkComponentSwizzle)
					VK_COMPONENT_SWIZZLE_IDENTITY			// a (VkComponentSwizzle)
				},
				{ // subresourceRange (VkImageSubresourceRange)
					vkImageAspectFlags,						// aspectMask (VkImageAspectFlags)
					0,										// baseMipLevel (uint32_t)
					levelCount,								// levelCount (uint32_t)
					0,										// baseArrayLayer (uint32_t)
					layerCount								// layerCount (uint32_t)
				}
			};
			if ( vkCreateImageView(vulkanRhi.getVulkanContext().getVkDevice(), &vkImageViewCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkImageView) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to create Vulkan image view")
			}
		}

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
#if SE_DEBUG
		static void setDebugObjectName(VkDevice vkDevice, VkDebugReportObjectTypeEXT vkDebugReportObjectTypeEXT, uint64_t object, const char* objectName)
		{
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				const VkDebugMarkerObjectNameInfoEXT vkDebugMarkerObjectNameInfoEXT =
				{
					VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT,	// sType (VkStructureType)
					nullptr,												// pNext (const void*)
					vkDebugReportObjectTypeEXT,								// objectType (VkDebugReportObjectTypeEXT)
					object,													// object (uint64_t)
					objectName												// pObjectName (const char*)
				};
				vkDebugMarkerSetObjectNameEXT(vkDevice, &vkDebugMarkerObjectNameInfoEXT);
			}
		}
#endif


	};

	//[-------------------------------------------------------]
	//[ VulkanRhi/RootSignature.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan root signature ("pipeline layout" in Vulkan terminology) class
	*/
	class RootSignature final : public Rhi::IRootSignature
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] rootSignature
		*    Root signature to use
		*/
		RootSignature(VulkanRhi& vulkanRhi, const Rhi::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IRootSignature(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRootSignature(rootSignature),
			mVkPipelineLayout(VK_NULL_HANDLE),
			mVkDescriptorPool(VK_NULL_HANDLE)
		{
			static constexpr uint32_t maxSets = 4242;	// TODO(co) We probably need to get this provided from the outside

			// Copy the parameter data
			const Rhi::Context& context = vulkanRhi.getContext();
			const uint32_t numberOfRootParameters = mRootSignature.numberOfParameters;
			if ( numberOfRootParameters > 0 )
			{
				mRootSignature.parameters = RHI_MALLOC_TYPED(context, Rhi::RootParameter, numberOfRootParameters);
				Rhi::RootParameter* destinationRootParameters = const_cast<Rhi::RootParameter*>(mRootSignature.parameters);
				memcpy(destinationRootParameters, rootSignature.parameters, sizeof(Rhi::RootParameter) * numberOfRootParameters);

				// Copy the descriptor table data
				for ( uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex )
				{
					Rhi::RootParameter& destinationRootParameter = destinationRootParameters[rootParameterIndex];
					const Rhi::RootParameter& sourceRootParameter = rootSignature.parameters[rootParameterIndex];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == destinationRootParameter.parameterType )
					{
						const uint32_t numberOfDescriptorRanges = destinationRootParameter.descriptorTable.numberOfDescriptorRanges;
						destinationRootParameter.descriptorTable.descriptorRanges = reinterpret_cast<uintptr_t>(RHI_MALLOC_TYPED(context, Rhi::DescriptorRange, numberOfDescriptorRanges));
						memcpy(reinterpret_cast<Rhi::DescriptorRange*>(destinationRootParameter.descriptorTable.descriptorRanges), reinterpret_cast<const Rhi::DescriptorRange*>(sourceRootParameter.descriptorTable.descriptorRanges), sizeof(Rhi::DescriptorRange) * numberOfDescriptorRanges);
					}
				}
			}

			{ // Copy the static sampler data
				const uint32_t numberOfStaticSamplers = mRootSignature.numberOfStaticSamplers;
				if ( numberOfStaticSamplers > 0 )
				{
					mRootSignature.staticSamplers = RHI_MALLOC_TYPED(context, Rhi::StaticSampler, numberOfStaticSamplers);
					memcpy(const_cast<Rhi::StaticSampler*>(mRootSignature.staticSamplers), rootSignature.staticSamplers, sizeof(Rhi::StaticSampler) * numberOfStaticSamplers);
				}
			}

			// Create the Vulkan descriptor set layout
			const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
			VkDescriptorSetLayouts vkDescriptorSetLayouts;
			uint32_t numberOfUniformTexelBuffers = 0;	// "VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER"
			uint32_t numberOfStorageTexelBuffers = 0;	// "VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER"
			uint32_t numberOfStorageImage = 0;			// "VK_DESCRIPTOR_TYPE_STORAGE_IMAGE"
			uint32_t numberOfStorageBuffers = 0;		// "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER"
			uint32_t numberOfUniformBuffers = 0;		// "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER"
			uint32_t numberOfCombinedImageSamplers = 0;	// "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
			if ( numberOfRootParameters > 0 )
			{
				// Fill the Vulkan descriptor set layout bindings
				vkDescriptorSetLayouts.reserve(numberOfRootParameters);
				mVkDescriptorSetLayouts.resize(numberOfRootParameters);
				std::fill(mVkDescriptorSetLayouts.begin(), mVkDescriptorSetLayouts.end(), static_cast<VkDescriptorSetLayout>(VK_NULL_HANDLE));	// TODO(co) Get rid of this
				typedef std::vector<VkDescriptorSetLayoutBinding> VkDescriptorSetLayoutBindings;
				VkDescriptorSetLayoutBindings vkDescriptorSetLayoutBindings;
				vkDescriptorSetLayoutBindings.reserve(numberOfRootParameters);
				for ( uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex )
				{
					vkDescriptorSetLayoutBindings.clear();

					// TODO(co) For now we only support descriptor tables
					const Rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
					{
						// Process descriptor ranges
						const Rhi::DescriptorRange* descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges);
						for ( uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < rootParameter.descriptorTable.numberOfDescriptorRanges; ++descriptorRangeIndex, ++descriptorRange )
						{
							// Evaluate parameter type
							VkDescriptorType vkDescriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
							switch ( descriptorRange->resourceType )
							{
							case Rhi::ResourceType::TEXTURE_BUFFER:
								RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange->rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange->rangeType, "Vulkan RHI implementation: Invalid descriptor range type")
									if ( Rhi::DescriptorRangeType::SRV == descriptorRange->rangeType )
									{
										vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
										++numberOfUniformTexelBuffers;
									}
									else
									{
										vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
										++numberOfStorageTexelBuffers;
									}
								break;

							case Rhi::ResourceType::VERTEX_BUFFER:
							case Rhi::ResourceType::INDEX_BUFFER:
							case Rhi::ResourceType::STRUCTURED_BUFFER:
							case Rhi::ResourceType::INDIRECT_BUFFER:
								RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange->rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange->rangeType, "Vulkan RHI implementation: Invalid descriptor range type")
									vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
								++numberOfStorageBuffers;
								break;

							case Rhi::ResourceType::UNIFORM_BUFFER:
								RHI_ASSERT(Rhi::DescriptorRangeType::UBV == descriptorRange->rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange->rangeType, "Vulkan RHI implementation: Invalid descriptor range type")
									vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
								++numberOfUniformBuffers;
								break;

							case Rhi::ResourceType::TEXTURE_1D:
							case Rhi::ResourceType::TEXTURE_1D_ARRAY:
							case Rhi::ResourceType::TEXTURE_2D:
							case Rhi::ResourceType::TEXTURE_2D_ARRAY:
							case Rhi::ResourceType::TEXTURE_3D:
							case Rhi::ResourceType::TEXTURE_CUBE:
							case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
								RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange->rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange->rangeType, "Vulkan RHI implementation: Invalid descriptor range type")
									if ( Rhi::DescriptorRangeType::SRV == descriptorRange->rangeType )
									{
										vkDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
										++numberOfCombinedImageSamplers;
									}
									else
									{
										vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
										++numberOfStorageImage;
									}
								break;

							case Rhi::ResourceType::SAMPLER_STATE:
								// Nothing here due to usage of "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
								RHI_ASSERT(Rhi::DescriptorRangeType::SAMPLER == descriptorRange->rangeType, "Vulkan RHI implementation: Invalid descriptor range type")
									break;

							case Rhi::ResourceType::ROOT_SIGNATURE:
							case Rhi::ResourceType::RESOURCE_GROUP:
							case Rhi::ResourceType::GRAPHICS_PROGRAM:
							case Rhi::ResourceType::VERTEX_ARRAY:
							case Rhi::ResourceType::RENDER_PASS:
							case Rhi::ResourceType::QUERY_POOL:
							case Rhi::ResourceType::SWAP_CHAIN:
							case Rhi::ResourceType::FRAMEBUFFER:
							case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
							case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
							case Rhi::ResourceType::VERTEX_SHADER:
							case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
							case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
							case Rhi::ResourceType::GEOMETRY_SHADER:
							case Rhi::ResourceType::FRAGMENT_SHADER:
							case Rhi::ResourceType::TASK_SHADER:
							case Rhi::ResourceType::MESH_SHADER:
							case Rhi::ResourceType::COMPUTE_SHADER:
								RHI_ASSERT(false, "Vulkan RHI implementation: Invalid resource type")
									break;
							}

							// Evaluate shader visibility
							VkShaderStageFlags vkShaderStageFlags = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
							switch ( descriptorRange->shaderVisibility )
							{
							case Rhi::ShaderVisibility::ALL:
								vkShaderStageFlags = VK_SHADER_STAGE_ALL;
								break;

							case Rhi::ShaderVisibility::VERTEX:
								vkShaderStageFlags = VK_SHADER_STAGE_VERTEX_BIT;
								break;

							case Rhi::ShaderVisibility::TESSELLATION_CONTROL:
								vkShaderStageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
								break;

							case Rhi::ShaderVisibility::TESSELLATION_EVALUATION:
								vkShaderStageFlags = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
								break;

							case Rhi::ShaderVisibility::GEOMETRY:
								vkShaderStageFlags = VK_SHADER_STAGE_GEOMETRY_BIT;
								break;

							case Rhi::ShaderVisibility::FRAGMENT:
								vkShaderStageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
								break;

							case Rhi::ShaderVisibility::TASK:
								vkShaderStageFlags = VK_SHADER_STAGE_TASK_BIT_NV;
								break;

							case Rhi::ShaderVisibility::MESH:
								vkShaderStageFlags = VK_SHADER_STAGE_MESH_BIT_NV;
								break;

							case Rhi::ShaderVisibility::COMPUTE:
								vkShaderStageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
								break;

							case Rhi::ShaderVisibility::ALL_GRAPHICS:
								vkShaderStageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
								break;
							}

							// Add the Vulkan descriptor set layout binding
							if ( VK_DESCRIPTOR_TYPE_MAX_ENUM != vkDescriptorType )
							{
								const VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding =
								{
									descriptorRangeIndex,	// binding (uint32_t)
									vkDescriptorType,		// descriptorType (VkDescriptorType)
									1,						// descriptorCount (uint32_t)
									vkShaderStageFlags,		// stageFlags (VkShaderStageFlags)
									nullptr					// pImmutableSamplers (const VkSampler*)
								};
								vkDescriptorSetLayoutBindings.push_back(vkDescriptorSetLayoutBinding);
							}
						}
					}

					// Create the Vulkan descriptor set layout
					if ( !vkDescriptorSetLayoutBindings.empty() )
					{
						const VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo =
						{
							VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,			// sType (VkStructureType)
							nullptr,														// pNext (const void*)
							0,																// flags (VkDescriptorSetLayoutCreateFlags)
							static_cast<uint32_t>(vkDescriptorSetLayoutBindings.size()),	// bindingCount (uint32_t)
							vkDescriptorSetLayoutBindings.data()							// pBindings (const VkDescriptorSetLayoutBinding*)
						};
						if ( vkCreateDescriptorSetLayout(vkDevice, &vkDescriptorSetLayoutCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkDescriptorSetLayouts[rootParameterIndex]) != VK_SUCCESS )
						{
							RHI_LOG(CRITICAL, "Failed to create the Vulkan descriptor set layout")
						}
						vkDescriptorSetLayouts.push_back(mVkDescriptorSetLayouts[rootParameterIndex]);
					}
				}
			}

			{ // Create the Vulkan pipeline layout
				const VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo =
				{
					VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,								// sType (VkStructureType)
					nullptr,																	// pNext (const void*)
					0,																			// flags (VkPipelineLayoutCreateFlags)
					static_cast<uint32_t>(vkDescriptorSetLayouts.size()),						// setLayoutCount (uint32_t)
					vkDescriptorSetLayouts.empty() ? nullptr : vkDescriptorSetLayouts.data(),	// pSetLayouts (const VkDescriptorSetLayout*)
					0,																			// pushConstantRangeCount (uint32_t)
					nullptr																		// pPushConstantRanges (const VkPushConstantRange*)
				};
				if ( vkCreatePipelineLayout(vkDevice, &vkPipelineLayoutCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkPipelineLayout) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to create the Vulkan pipeline layout")
				}
			}

			{ // Create the Vulkan descriptor pool
				typedef std::array<VkDescriptorPoolSize, 3> VkDescriptorPoolSizes;
				VkDescriptorPoolSizes vkDescriptorPoolSizes;
				uint32_t numberOfVkDescriptorPoolSizes = 0;

				// "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
				if ( numberOfCombinedImageSamplers > 0 )
				{
					VkDescriptorPoolSize& vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
					vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;	// type (VkDescriptorType)
					vkDescriptorPoolSize.descriptorCount = maxSets * numberOfCombinedImageSamplers;		// descriptorCount (uint32_t)
					++numberOfVkDescriptorPoolSizes;
				}

				// "VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER"
				if ( numberOfUniformTexelBuffers > 0 )
				{
					VkDescriptorPoolSize& vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
					vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;	// type (VkDescriptorType)
					vkDescriptorPoolSize.descriptorCount = maxSets * numberOfUniformTexelBuffers;	// descriptorCount (uint32_t)
					++numberOfVkDescriptorPoolSizes;
				}

				// "VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER"
				if ( numberOfStorageTexelBuffers > 0 )
				{
					VkDescriptorPoolSize& vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
					vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;	// type (VkDescriptorType)
					vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageTexelBuffers;	// descriptorCount (uint32_t)
					++numberOfVkDescriptorPoolSizes;
				}

				// "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER"
				if ( numberOfUniformBuffers > 0 )
				{
					VkDescriptorPoolSize& vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
					vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;	// type (VkDescriptorType)
					vkDescriptorPoolSize.descriptorCount = maxSets * numberOfUniformBuffers;	// descriptorCount (uint32_t)
					++numberOfVkDescriptorPoolSizes;
				}

				// "VK_DESCRIPTOR_TYPE_STORAGE_IMAGE"
				if ( numberOfStorageImage > 0 )
				{
					VkDescriptorPoolSize& vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
					vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;	// type (VkDescriptorType)
					vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageImage;		// descriptorCount (uint32_t)
					++numberOfVkDescriptorPoolSizes;
				}

				// "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER"
				if ( numberOfStorageBuffers > 0 )
				{
					VkDescriptorPoolSize& vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
					vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;	// type (VkDescriptorType)
					vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageBuffers;	// descriptorCount (uint32_t)
					++numberOfVkDescriptorPoolSizes;
				}

				// Create the Vulkan descriptor pool
				if ( numberOfVkDescriptorPoolSizes > 0 )
				{
					const VkDescriptorPoolCreateInfo VkDescriptorPoolCreateInfo =
					{
						VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,		// sType (VkStructureType)
						nullptr,											// pNext (const void*)
						VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,	// flags (VkDescriptorPoolCreateFlags)
						maxSets,											// maxSets (uint32_t)
						numberOfVkDescriptorPoolSizes,						// poolSizeCount (uint32_t)
						vkDescriptorPoolSizes.data()						// pPoolSizes (const VkDescriptorPoolSize*)
					};
					if ( vkCreateDescriptorPool(vkDevice, &VkDescriptorPoolCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkDescriptorPool) != VK_SUCCESS )
					{
						RHI_LOG(CRITICAL, "Failed to create the Vulkan descriptor pool")
					}
				}
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Root signature", 17)	// 17 = "Root signature: " including terminating zero
					for ( VkDescriptorSetLayout vkDescriptorSetLayout : mVkDescriptorSetLayouts )
					{
						Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT, (uint64_t)vkDescriptorSetLayout, detailedDebugName);
					}
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT, (uint64_t)mVkPipelineLayout, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT, (uint64_t)mVkDescriptorPool, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RootSignature() override
		{
			const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();

			// Destroy the Vulkan descriptor pool
			if ( VK_NULL_HANDLE != mVkDescriptorPool )
			{
				vkDestroyDescriptorPool(vkDevice, mVkDescriptorPool, vulkanRhi.getVkAllocationCallbacks());
			}

			// Destroy the Vulkan pipeline layout
			if ( VK_NULL_HANDLE != mVkPipelineLayout )
			{
				vkDestroyPipelineLayout(vkDevice, mVkPipelineLayout, vulkanRhi.getVkAllocationCallbacks());
			}

			// Destroy the Vulkan descriptor set layout
			for ( VkDescriptorSetLayout vkDescriptorSetLayout : mVkDescriptorSetLayouts )
			{
				if ( VK_NULL_HANDLE != vkDescriptorSetLayout )
				{
					vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout, vulkanRhi.getVkAllocationCallbacks());
				}
			}

			// Destroy the root signature data
			const Rhi::Context& context = vulkanRhi.getContext();
			if ( nullptr != mRootSignature.parameters )
			{
				for ( uint32_t rootParameterIndex = 0; rootParameterIndex < mRootSignature.numberOfParameters; ++rootParameterIndex )
				{
					const Rhi::RootParameter& rootParameter = mRootSignature.parameters[rootParameterIndex];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
					{
						RHI_FREE(context, reinterpret_cast<Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges));
					}
				}
				RHI_FREE(context, const_cast<Rhi::RootParameter*>(mRootSignature.parameters));
			}
			RHI_FREE(context, const_cast<Rhi::StaticSampler*>(mRootSignature.staticSamplers));
		}

		/**
		*  @brief
		*    Return the root signature data
		*
		*  @return
		*    The root signature data
		*/
		[[nodiscard]] inline const Rhi::RootSignature& getRootSignature() const
		{
			return mRootSignature;
		}

		/**
		*  @brief
		*    Return the Vulkan pipeline layout
		*
		*  @return
		*    The Vulkan pipeline layout
		*/
		[[nodiscard]] inline VkPipelineLayout getVkPipelineLayout() const
		{
			return mVkPipelineLayout;
		}

		/**
		*  @brief
		*    Return the Vulkan descriptor pool
		*
		*  @return
		*    The Vulkan descriptor pool
		*/
		[[nodiscard]] inline VkDescriptorPool getVkDescriptorPool() const
		{
			return mVkDescriptorPool;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRootSignature methods            ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual Rhi::IResourceGroup* createResourceGroup(uint32_t rootParameterIndex, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), RootSignature, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit RootSignature(const RootSignature&) = delete;
		RootSignature& operator =(const RootSignature&) = delete;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
	private:
		typedef std::vector<VkDescriptorSetLayout> VkDescriptorSetLayouts;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::RootSignature	   mRootSignature;
		VkDescriptorSetLayouts mVkDescriptorSetLayouts;
		VkPipelineLayout	   mVkPipelineLayout;
		VkDescriptorPool	   mVkDescriptorPool;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/VertexBuffer.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan vertex buffer object (VBO) interface
	*/
	class VertexBuffer final : public Rhi::IVertexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the vertex buffer, must be valid
		*  @param[in] data
		*    Vertex buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		VertexBuffer(VulkanRhi& vulkanRhi, uint32_t numberOfBytes, const void* data, uint32_t bufferFlags, [[maybe_unused]] Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IVertexBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkBuffer(VK_NULL_HANDLE),
			mVkDeviceMemory(VK_NULL_HANDLE)
		{
			int vkBufferUsageFlagBits = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			if ( (bufferFlags & Rhi::BufferFlag::UNORDERED_ACCESS) != 0 || (bufferFlags & Rhi::BufferFlag::SHADER_RESOURCE) != 0 )
			{
				vkBufferUsageFlagBits |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
			}
			Helper::createAndAllocateVkBuffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(vkBufferUsageFlagBits), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VBO", 6)	// 6 = "VBO: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (uint64_t)mVkBuffer, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexBuffer() override
		{
			Helper::destroyAndFreeVkBuffer(static_cast<const VulkanRhi&>(getRhi()), mVkBuffer, mVkDeviceMemory);
		}

		/**
		*  @brief
		*    Return the Vulkan vertex buffer
		*
		*  @return
		*    The Vulkan vertex buffer
		*/
		[[nodiscard]] inline VkBuffer getVkBuffer() const
		{
			return mVkBuffer;
		}

		/**
		*  @brief
		*    Return the Vulkan device memory
		*
		*  @return
		*    The Vulkan device memory
		*/
		[[nodiscard]] inline VkDeviceMemory getVkDeviceMemory() const
		{
			return mVkDeviceMemory;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), VertexBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator =(const VertexBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkBuffer	   mVkBuffer;		// Vulkan vertex buffer
		VkDeviceMemory mVkDeviceMemory;	// Vulkan vertex memory


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/IndexBuffer.h                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan index buffer object (IBO) interface
	*/
	class IndexBuffer final : public Rhi::IIndexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the index buffer, must be valid
		*  @param[in] data
		*    Index buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] indexBufferFormat
		*    Index buffer data format
		*/
		IndexBuffer(VulkanRhi& vulkanRhi, uint32_t numberOfBytes, const void* data, uint32_t bufferFlags, [[maybe_unused]] Rhi::BufferUsage bufferUsage, Rhi::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IIndexBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkIndexType(Mapping::getVulkanType(vulkanRhi.getContext(), indexBufferFormat)),
			mVkBuffer(VK_NULL_HANDLE),
			mVkDeviceMemory(VK_NULL_HANDLE)
		{
			int vkBufferUsageFlagBits = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			if ( (bufferFlags & Rhi::BufferFlag::UNORDERED_ACCESS) != 0 || (bufferFlags & Rhi::BufferFlag::SHADER_RESOURCE) != 0 )
			{
				vkBufferUsageFlagBits |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
			}
			Helper::createAndAllocateVkBuffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(vkBufferUsageFlagBits), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IBO", 6)	// 6 = "IBO: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (uint64_t)mVkBuffer, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IndexBuffer() override
		{
			Helper::destroyAndFreeVkBuffer(static_cast<const VulkanRhi&>(getRhi()), mVkBuffer, mVkDeviceMemory);
		}

		/**
		*  @brief
		*    Return the Vulkan index type
		*
		*  @return
		*    The Vulkan index type
		*/
		[[nodiscard]] inline VkIndexType getVkIndexType() const
		{
			return mVkIndexType;
		}

		/**
		*  @brief
		*    Return the Vulkan index buffer
		*
		*  @return
		*    The Vulkan index buffer
		*/
		[[nodiscard]] inline VkBuffer getVkBuffer() const
		{
			return mVkBuffer;
		}

		/**
		*  @brief
		*    Return the Vulkan device memory
		*
		*  @return
		*    The Vulkan device memory
		*/
		[[nodiscard]] inline VkDeviceMemory getVkDeviceMemory() const
		{
			return mVkDeviceMemory;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), IndexBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator =(const IndexBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkIndexType	   mVkIndexType;	// Vulkan vertex type
		VkBuffer	   mVkBuffer;		// Vulkan vertex buffer
		VkDeviceMemory mVkDeviceMemory;	// Vulkan vertex memory


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/VertexArray.h                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan vertex array interface
	*/
	class VertexArray final : public Rhi::IVertexArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] numberOfVertexBuffers
		*    Number of vertex buffers, having zero vertex buffers is valid
		*  @param[in] vertexBuffers
		*    At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		VertexArray(VulkanRhi& vulkanRhi, const Rhi::VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IVertexArray(static_cast<Rhi::IRhi&>(vulkanRhi), id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mIndexBuffer(indexBuffer),
			mNumberOfSlots(numberOfVertexBuffers),
			mVertexVkBuffers(nullptr),
			mStrides(nullptr),
			mOffsets(nullptr),
			mVertexBuffers(nullptr)
		{
			// Add a reference to the given index buffer
			if ( nullptr != mIndexBuffer )
			{
				mIndexBuffer->addReference();
			}

			// Add a reference to the used vertex buffers
			if ( mNumberOfSlots > 0 )
			{
				const Rhi::Context& context = vulkanRhi.getContext();
				mVertexVkBuffers = RHI_MALLOC_TYPED(context, VkBuffer, mNumberOfSlots);
				mStrides = RHI_MALLOC_TYPED(context, uint32_t, mNumberOfSlots);
				mOffsets = RHI_MALLOC_TYPED(context, VkDeviceSize, mNumberOfSlots);
				memset(mOffsets, 0, sizeof(VkDeviceSize) * mNumberOfSlots);	// Vertex buffer offset is not supported by OpenGL, so our RHI implementation doesn't support it either, set everything to zero
				mVertexBuffers = RHI_MALLOC_TYPED(context, VertexBuffer*, mNumberOfSlots);

				{ // Loop through all vertex buffers
					VkBuffer* currentVertexVkBuffer = mVertexVkBuffers;
					VertexBuffer** currentVertexBuffer = mVertexBuffers;
					const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + mNumberOfSlots;
					for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer, ++currentVertexVkBuffer, ++currentVertexBuffer )
					{
						// TODO(co) Add security check: Is the given resource one of the currently used RHI?
						*currentVertexBuffer = static_cast<VertexBuffer*>(vertexBuffer->vertexBuffer);
						*currentVertexVkBuffer = (*currentVertexBuffer)->getVkBuffer();
						(*currentVertexBuffer)->addReference();
					}
				}

				{ // Gather slot related data
					const Rhi::VertexAttribute* attribute = vertexAttributes.attributes;
					const Rhi::VertexAttribute* attributesEnd = attribute + vertexAttributes.numberOfAttributes;
					for ( ; attribute < attributesEnd; ++attribute )
					{
						mStrides[attribute->inputSlot] = attribute->strideInBytes;
					}
				}
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VertexArray() override
		{
			// Release the index buffer reference
			if ( nullptr != mIndexBuffer )
			{
				mIndexBuffer->releaseReference();
			}

			// Cleanup Vulkan input slot data
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			const Rhi::Context& context = vulkanRhi.getContext();
			if ( mNumberOfSlots > 0 )
			{
				RHI_FREE(context, mVertexVkBuffers);
				RHI_FREE(context, mStrides);
				RHI_FREE(context, mOffsets);
			}

			// Release the reference to the used vertex buffers
			if ( nullptr != mVertexBuffers )
			{
				// Release references
				VertexBuffer** vertexBuffersEnd = mVertexBuffers + mNumberOfSlots;
				for ( VertexBuffer** vertexBuffer = mVertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer )
				{
					(*vertexBuffer)->releaseReference();
				}

				// Cleanup
				RHI_FREE(context, mVertexBuffers);
			}

			// Free the unique compact vertex array ID
			vulkanRhi.VertexArrayMakeId.DestroyID(getId());
		}

		/**
		*  @brief
		*    Return the used index buffer
		*
		*  @return
		*    The used index buffer, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IndexBuffer* getIndexBuffer() const
		{
			return mIndexBuffer;
		}

		/**
		*  @brief
		*    Bind Vulkan buffers
		*
		*  @param[in] vkCommandBuffer
		*    Vulkan command buffer to write into
		*/
		void bindVulkanBuffers(VkCommandBuffer vkCommandBuffer) const
		{
			// Set the Vulkan vertex buffers
			if ( nullptr != mVertexVkBuffers )
			{
				vkCmdBindVertexBuffers(vkCommandBuffer, 0, mNumberOfSlots, mVertexVkBuffers, mOffsets);
			}
			else
			{
				// Do nothing since the Vulkan specification says "bindingCount must be greater than 0"
				// vkCmdBindVertexBuffers(vkCommandBuffer, 0, 0, nullptr, nullptr);
			}

			// Set the used index buffer
			// -> In case of no index buffer we don't set null indices, there's not really a point in it
			if ( nullptr != mIndexBuffer )
			{
				vkCmdBindIndexBuffer(vkCommandBuffer, mIndexBuffer->getVkBuffer(), 0, mIndexBuffer->getVkIndexType());
			}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), VertexArray, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexArray(const VertexArray&) = delete;
		VertexArray& operator =(const VertexArray&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		IndexBuffer*   mIndexBuffer;		// Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		// Vulkan input slots
		uint32_t	   mNumberOfSlots;		// Number of used Vulkan input slots
		VkBuffer*	   mVertexVkBuffers;	// Vulkan vertex buffers
		uint32_t*	   mStrides;			// Strides in bytes, if "mVertexVkBuffers" is no null pointer this is no null pointer as well
		VkDeviceSize*  mOffsets;			// Offsets in bytes, if "mVertexVkBuffers" is no null pointer this is no null pointer as well
		// For proper vertex buffer reference counter behaviour
		VertexBuffer** mVertexBuffers;		// Vertex buffers (we keep a reference to it) used by this vertex array, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/TextureBuffer.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan texture buffer object (TBO) interface
	*/
	class TextureBuffer final : public Rhi::ITextureBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the texture buffer, must be valid
		*  @param[in] data
		*    Texture buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] textureFormat
		*    Texture buffer data format
		*/
		TextureBuffer(VulkanRhi& vulkanRhi, uint32_t numberOfBytes, const void* data, uint32_t bufferFlags, [[maybe_unused]] Rhi::BufferUsage bufferUsage, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITextureBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkBuffer(VK_NULL_HANDLE),
			mVkDeviceMemory(VK_NULL_HANDLE),
			mVkBufferView(VK_NULL_HANDLE)
		{
			// Sanity check
			RHI_ASSERT((numberOfBytes % Rhi::TextureFormat::getNumberOfBytesPerElement(textureFormat)) == 0, "The Vulkan texture buffer size must be a multiple of the selected texture format bytes per texel")

				// Create the texture buffer
				uint32_t vkBufferUsageFlagBits = 0;
			if ( bufferFlags & Rhi::BufferFlag::SHADER_RESOURCE )
			{
				vkBufferUsageFlagBits |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
			}
			if ( bufferFlags & Rhi::BufferFlag::UNORDERED_ACCESS )
			{
				vkBufferUsageFlagBits |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
			}
			Helper::createAndAllocateVkBuffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(vkBufferUsageFlagBits), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

			// Create Vulkan buffer view
			if ( (bufferFlags & Rhi::BufferFlag::SHADER_RESOURCE) != 0 || (bufferFlags & Rhi::BufferFlag::UNORDERED_ACCESS) != 0 )
			{
				const VkBufferViewCreateInfo vkBufferViewCreateInfo =
				{
					VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,	// sType (VkStructureType)
					nullptr,									// pNext (const void*)
					0,											// flags (VkBufferViewCreateFlags)
					mVkBuffer,									// buffer (VkBuffer)
					Mapping::getVulkanFormat(textureFormat),	// format (VkFormat)
					0,											// offset (VkDeviceSize)
					VK_WHOLE_SIZE								// range (VkDeviceSize)
				};
				if ( vkCreateBufferView(vulkanRhi.getVulkanContext().getVkDevice(), &vkBufferViewCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkBufferView) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to create the Vulkan buffer view")
				}
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TBO", 6)	// 6 = "TBO: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (uint64_t)mVkBuffer, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT, (uint64_t)mVkBufferView, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureBuffer() override
		{
			const VulkanRhi& vulkanRhi = static_cast<const VulkanRhi&>(getRhi());
			if ( VK_NULL_HANDLE != mVkBufferView )
			{
				vkDestroyBufferView(vulkanRhi.getVulkanContext().getVkDevice(), mVkBufferView, vulkanRhi.getVkAllocationCallbacks());
			}
			Helper::destroyAndFreeVkBuffer(vulkanRhi, mVkBuffer, mVkDeviceMemory);
		}

		/**
		*  @brief
		*    Return the Vulkan texture buffer
		*
		*  @return
		*    The Vulkan texture buffer
		*/
		[[nodiscard]] inline VkBuffer getVkBuffer() const
		{
			return mVkBuffer;
		}

		/**
		*  @brief
		*    Return the Vulkan device memory
		*
		*  @return
		*    The Vulkan device memory
		*/
		[[nodiscard]] inline VkDeviceMemory getVkDeviceMemory() const
		{
			return mVkDeviceMemory;
		}

		/**
		*  @brief
		*    Return the Vulkan buffer view
		*
		*  @return
		*    The Vulkan buffer view
		*/
		[[nodiscard]] inline VkBufferView getVkBufferView() const
		{
			return mVkBufferView;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), TextureBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureBuffer(const TextureBuffer&) = delete;
		TextureBuffer& operator =(const TextureBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkBuffer	   mVkBuffer;		// Vulkan uniform texel buffer
		VkDeviceMemory mVkDeviceMemory;	// Vulkan uniform texel memory
		VkBufferView   mVkBufferView;	// Vulkan buffer view


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/StructuredBuffer.h                          ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan structured buffer object (SBO) interface
	*/
	class StructuredBuffer final : public Rhi::IStructuredBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the structured buffer, must be valid
		*  @param[in] data
		*    Structured buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] numberOfStructureBytes
		*    Number of structure bytes
		*/
		StructuredBuffer(VulkanRhi& vulkanRhi, uint32_t numberOfBytes, const void* data, [[maybe_unused]] Rhi::BufferUsage bufferUsage, [[maybe_unused]] uint32_t numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IStructuredBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkBuffer(VK_NULL_HANDLE),
			mVkDeviceMemory(VK_NULL_HANDLE)
		{
			// Sanity checks
			RHI_ASSERT((numberOfBytes % numberOfStructureBytes) == 0, "The Vulkan structured buffer size must be a multiple of the given number of structure bytes")
				RHI_ASSERT((numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The Vulkan structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

				// Create the structured buffer
				Helper::createAndAllocateVkBuffer(vulkanRhi, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "SBO", 6)	// 6 = "SBO: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (uint64_t)mVkBuffer, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StructuredBuffer() override
		{
			Helper::destroyAndFreeVkBuffer(static_cast<const VulkanRhi&>(getRhi()), mVkBuffer, mVkDeviceMemory);
		}

		/**
		*  @brief
		*    Return the Vulkan structured buffer
		*
		*  @return
		*    The Vulkan structured buffer
		*/
		[[nodiscard]] inline VkBuffer getVkBuffer() const
		{
			return mVkBuffer;
		}

		/**
		*  @brief
		*    Return the Vulkan device memory
		*
		*  @return
		*    The Vulkan device memory
		*/
		[[nodiscard]] inline VkDeviceMemory getVkDeviceMemory() const
		{
			return mVkDeviceMemory;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), StructuredBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit StructuredBuffer(const StructuredBuffer&) = delete;
		StructuredBuffer& operator =(const StructuredBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkBuffer	   mVkBuffer;		// Vulkan uniform texel buffer
		VkDeviceMemory mVkDeviceMemory;	// Vulkan uniform texel memory


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/IndirectBuffer.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan indirect buffer object interface
	*/
	class IndirectBuffer final : public Rhi::IIndirectBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the indirect buffer, must be valid
		*  @param[in] data
		*    Indirect buffer data, can be a null pointer (empty buffer)
		*  @param[in] indirectBufferFlags
		*    Indirect buffer flags, see "Rhi::IndirectBufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		IndirectBuffer(VulkanRhi& vulkanRhi, uint32_t numberOfBytes, const void* data, uint32_t indirectBufferFlags, [[maybe_unused]] Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IIndirectBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkBuffer(VK_NULL_HANDLE),
			mVkDeviceMemory(VK_NULL_HANDLE)
		{
			// Sanity checks
			RHI_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 || (indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0, "Invalid Vulkan flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" is missing")
				RHI_ASSERT(!((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 && (indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0), "Invalid Vulkan flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" must be set, but not both at one and the same time")
				RHI_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) == 0 || (numberOfBytes % sizeof(Rhi::DrawArguments)) == 0, "Vulkan indirect buffer element type flags specification is \"DRAW_ARGUMENTS\" but the given number of bytes don't align to this")
				RHI_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) == 0 || (numberOfBytes % sizeof(Rhi::DrawIndexedArguments)) == 0, "Vulkan indirect buffer element type flags specification is \"DRAW_INDEXED_ARGUMENTS\" but the given number of bytes don't align to this")

				// Create indirect buffer
				int vkBufferUsageFlagBits = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
			if ( (indirectBufferFlags & Rhi::IndirectBufferFlag::UNORDERED_ACCESS) != 0 || (indirectBufferFlags & Rhi::IndirectBufferFlag::SHADER_RESOURCE) != 0 )
			{
				vkBufferUsageFlagBits |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
			}
			Helper::createAndAllocateVkBuffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(vkBufferUsageFlagBits), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IndirectBufferObject", 23)	// 23 = "IndirectBufferObject: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (uint64_t)mVkBuffer, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IndirectBuffer() override
		{
			Helper::destroyAndFreeVkBuffer(static_cast<const VulkanRhi&>(getRhi()), mVkBuffer, mVkDeviceMemory);
		}

		/**
		*  @brief
		*    Return the Vulkan indirect buffer
		*
		*  @return
		*    The Vulkan indirect buffer
		*/
		[[nodiscard]] inline VkBuffer getVkBuffer() const
		{
			return mVkBuffer;
		}

		/**
		*  @brief
		*    Return the Vulkan device memory
		*
		*  @return
		*    The Vulkan device memory
		*/
		[[nodiscard]] inline VkDeviceMemory getVkDeviceMemory() const
		{
			return mVkDeviceMemory;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IIndirectBuffer methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const uint8_t* getEmulationData() const override
		{
			return nullptr;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), IndirectBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit IndirectBuffer(const IndirectBuffer&) = delete;
		IndirectBuffer& operator =(const IndirectBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkBuffer	   mVkBuffer;		// Vulkan indirect buffer
		VkDeviceMemory mVkDeviceMemory;	// Vulkan indirect memory


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/UniformBuffer.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan uniform buffer object (UBO, "constant buffer" in Direct3D terminology) interface
	*/
	class UniformBuffer final : public Rhi::IUniformBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the uniform buffer, must be valid
		*  @param[in] data
		*    Uniform buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		UniformBuffer(VulkanRhi& vulkanRhi, uint32_t numberOfBytes, const void* data, [[maybe_unused]] Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IUniformBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkBuffer(VK_NULL_HANDLE),
			mVkDeviceMemory(VK_NULL_HANDLE)
		{
			Helper::createAndAllocateVkBuffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "UBO", 6)	// 6 = "UBO: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (uint64_t)mVkBuffer, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~UniformBuffer() override
		{
			Helper::destroyAndFreeVkBuffer(static_cast<const VulkanRhi&>(getRhi()), mVkBuffer, mVkDeviceMemory);
		}

		/**
		*  @brief
		*    Return the Vulkan uniform buffer
		*
		*  @return
		*    The Vulkan uniform buffer
		*/
		[[nodiscard]] inline VkBuffer getVkBuffer() const
		{
			return mVkBuffer;
		}

		/**
		*  @brief
		*    Return the Vulkan device memory
		*
		*  @return
		*    The Vulkan device memory
		*/
		[[nodiscard]] inline VkDeviceMemory getVkDeviceMemory() const
		{
			return mVkDeviceMemory;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), UniformBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator =(const UniformBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkBuffer	   mVkBuffer;		// Vulkan uniform buffer
		VkDeviceMemory mVkDeviceMemory;	// Vulkan uniform memory


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/BufferManager.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan buffer manager interface
	*/
	class BufferManager final : public Rhi::IBufferManager
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*/
		inline explicit BufferManager(VulkanRhi& vulkanRhi) :
			IBufferManager(vulkanRhi)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BufferManager() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IBufferManager methods            ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual Rhi::IVertexBuffer* createVertexBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = 0, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), VertexBuffer)(vulkanRhi, numberOfBytes, data, bufferFlags, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IIndexBuffer* createIndexBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = 0, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW, Rhi::IndexBufferFormat::Enum indexBufferFormat = Rhi::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), IndexBuffer)(vulkanRhi, numberOfBytes, data, bufferFlags, bufferUsage, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IVertexArray* createVertexArray(const Rhi::VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, Rhi::IIndexBuffer* indexBuffer = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity checks
#if SE_DEBUG
			{
				const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
				for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer )
				{
					RHI_ASSERT(&vulkanRhi == &vertexBuffer->vertexBuffer->getRhi(), "Vulkan error: The given vertex buffer resource is owned by another RHI instance")
				}
			}
#endif
			RHI_ASSERT(nullptr == indexBuffer || &vulkanRhi == &indexBuffer->getRhi(), "Vulkan error: The given index buffer resource is owned by another RHI instance")

				// Create vertex array
				uint16_t id = 0;
			if ( vulkanRhi.VertexArrayMakeId.CreateID(id) )
			{
				return RHI_NEW(vulkanRhi.getContext(), VertexArray)(vulkanRhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}

			// Error: Ensure a correct reference counter behaviour
			const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
			for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer )
			{
				vertexBuffer->vertexBuffer->addReference();
				vertexBuffer->vertexBuffer->releaseReference();
			}
			if ( nullptr != indexBuffer )
			{
				indexBuffer->addReference();
				indexBuffer->releaseReference();
			}
			return nullptr;
		}

		[[nodiscard]] inline virtual Rhi::ITextureBuffer* createTextureBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = Rhi::BufferFlag::SHADER_RESOURCE, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW, Rhi::TextureFormat::Enum textureFormat = Rhi::TextureFormat::R32G32B32A32F RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), TextureBuffer)(vulkanRhi, numberOfBytes, data, bufferFlags, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IStructuredBuffer* createStructuredBuffer(uint32_t numberOfBytes, const void* data, [[maybe_unused]] uint32_t bufferFlags, Rhi::BufferUsage bufferUsage, uint32_t numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), StructuredBuffer)(vulkanRhi, numberOfBytes, data, bufferUsage, numberOfStructureBytes RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IIndirectBuffer* createIndirectBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t indirectBufferFlags = 0, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), IndirectBuffer)(vulkanRhi, numberOfBytes, data, indirectBufferFlags, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IUniformBuffer* createUniformBuffer(uint32_t numberOfBytes, const void* data = nullptr, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
			// -> Inside GLSL "layout(binding = 0, std140) writeonly uniform OutputUniformBuffer" will result in the GLSL compiler error "Failed to parse the GLSL shader source code: ERROR: 0:85: 'assign' :  l-value required "anon@6" (can't modify a uniform)"
			// -> Inside GLSL "layout(binding = 0, std430) writeonly buffer  OutputUniformBuffer" will work in OpenGL but will fail in Vulkan with "Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT" Object: "0" Location: "0" Message code: "13" Layer prefix: "Validation" Message: "Object: VK_NULL_HANDLE (Type = 0) | Type mismatch on descriptor slot 0.0 (used as type `ptr to uniform struct of (vec4 of float32)`) but descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER""
			// RHI_ASSERT((bufferFlags & Rhi::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid Vulkan buffer flags, uniform buffer can't be used for unordered access")
			// RHI_ASSERT((bufferFlags & Rhi::BufferFlag::SHADER_RESOURCE) != 0, "Invalid Vulkan buffer flags, uniform buffer must be used as shader resource")

			// Create the uniform buffer
			return RHI_NEW(vulkanRhi.getContext(), UniformBuffer)(vulkanRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), BufferManager, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit BufferManager(const BufferManager&) = delete;
		BufferManager& operator =(const BufferManager&) = delete;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Texture/Texture1D.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan 1D texture interface
	*/
	class Texture1D final : public Rhi::ITexture1D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture1D(VulkanRhi& vulkanRhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture1D(vulkanRhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkImage(VK_NULL_HANDLE),
			mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
			mVkDeviceMemory(VK_NULL_HANDLE),
			mVkImageView(VK_NULL_HANDLE)
		{
			Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_1D, VK_IMAGE_VIEW_TYPE_1D, { width, 1, 1 }, textureFormat, data, textureFlags, 1, mVkImage, mVkDeviceMemory, mVkImageView);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture", 13)	// 13 = "1D texture: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (uint64_t)mVkImage, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (uint64_t)mVkImageView, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture1D() override
		{
			Helper::destroyAndFreeVkImage(static_cast<VulkanRhi&>(getRhi()), mVkImage, mVkDeviceMemory, mVkImageView);
		}

		/**
		*  @brief
		*    Return the Vulkan image view
		*
		*  @return
		*    The Vulkan image view
		*/
		[[nodiscard]] inline VkImageView getVkImageView() const
		{
			return mVkImageView;
		}

		/**
		*  @brief
		*    Return the Vulkan image layout
		*
		*  @return
		*    The Vulkan image layout
		*/
		[[nodiscard]] inline VkImageLayout getVkImageLayout() const
		{
			return mVkImageLayout;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), Texture1D, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture1D(const Texture1D&) = delete;
		Texture1D& operator =(const Texture1D&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkImage		   mVkImage;
		VkImageLayout  mVkImageLayout;
		VkDeviceMemory mVkDeviceMemory;
		VkImageView	   mVkImageView;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Texture/Texture1DArray.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan 1D array texture interface
	*/
	class Texture1DArray final : public Rhi::ITexture1DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture1DArray(VulkanRhi& vulkanRhi, uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture1DArray(vulkanRhi, width, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkImage(VK_NULL_HANDLE),
			mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
			mVkDeviceMemory(VK_NULL_HANDLE),
			mVkImageView(VK_NULL_HANDLE),
			mVkFormat(Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_1D, VK_IMAGE_VIEW_TYPE_1D_ARRAY, { width, 1, numberOfSlices }, textureFormat, data, textureFlags, 1, mVkImage, mVkDeviceMemory, mVkImageView))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture array", 19)	// 19 = "1D texture array: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (uint64_t)mVkImage, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (uint64_t)mVkImageView, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture1DArray() override
		{
			Helper::destroyAndFreeVkImage(static_cast<VulkanRhi&>(getRhi()), mVkImage, mVkDeviceMemory, mVkImageView);
		}

		/**
		*  @brief
		*    Return the Vulkan image view
		*
		*  @return
		*    The Vulkan image view
		*/
		[[nodiscard]] inline VkImageView getVkImageView() const
		{
			return mVkImageView;
		}

		/**
		*  @brief
		*    Return the Vulkan image layout
		*
		*  @return
		*    The Vulkan image layout
		*/
		[[nodiscard]] inline VkImageLayout getVkImageLayout() const
		{
			return mVkImageLayout;
		}

		/**
		*  @brief
		*    Return the Vulkan format
		*
		*  @return
		*    The Vulkan format
		*/
		[[nodiscard]] inline VkFormat getVkFormat() const
		{
			return mVkFormat;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), Texture1DArray, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture1DArray(const Texture1DArray&) = delete;
		Texture1DArray& operator =(const Texture1DArray&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkImage		   mVkImage;
		VkImageLayout  mVkImageLayout;
		VkDeviceMemory mVkDeviceMemory;
		VkImageView	   mVkImageView;
		VkFormat	   mVkFormat;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Texture/Texture2D.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenVR-support: Data required for passing Vulkan textures to IVRCompositor::Submit; Be sure to call OpenVR_Shutdown before destroying these resources
	*
	*  @note
	*    - From OpenVR SDK 1.0.7 "openvr.h"-header
	*/
	struct VRVulkanTextureData_t
	{
		VkImage			 m_nImage;
		VkDevice		 m_pDevice;
		VkPhysicalDevice m_pPhysicalDevice;
		VkInstance		 m_pInstance;
		VkQueue			 m_pQueue;
		uint32_t		 m_nQueueFamilyIndex;
		uint32_t		 m_nWidth;
		uint32_t		 m_nHeight;
		VkFormat		 m_nFormat;
		uint32_t		 m_nSampleCount;
	};

	/**
	*  @brief
	*    Vulkan 2D texture interface
	*/
	class Texture2D final : public Rhi::ITexture2D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*/
		Texture2D(VulkanRhi& vulkanRhi, uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture2D(vulkanRhi, width, height RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVrVulkanTextureData{},
			mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
			mVkDeviceMemory(VK_NULL_HANDLE),
			mVkImageView(VK_NULL_HANDLE)
		{
			mVrVulkanTextureData.m_nFormat = Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, { width, height, 1 }, textureFormat, data, textureFlags, numberOfMultisamples, mVrVulkanTextureData.m_nImage, mVkDeviceMemory, mVkImageView);

			// Fill the rest of the "VRVulkanTextureData_t"-structure
			const VulkanContext& vulkanContext = vulkanRhi.getVulkanContext();
			const VulkanRuntimeLinking& vulkanRuntimeLinking = vulkanRhi.getVulkanRuntimeLinking();
			// m_nImage (VkImage) was set by "VulkanRhi::Helper::createAndFillVkImage()" above
			mVrVulkanTextureData.m_pDevice = vulkanContext.getVkDevice();					// m_pDevice (VkDevice)
			mVrVulkanTextureData.m_pPhysicalDevice = vulkanContext.getVkPhysicalDevice();			// m_pPhysicalDevice (VkPhysicalDevice)
			mVrVulkanTextureData.m_pInstance = vulkanRuntimeLinking.getVkInstance();		// m_pInstance (VkInstance)
			mVrVulkanTextureData.m_pQueue = vulkanContext.getGraphicsVkQueue();			// m_pQueue (VkQueue)
			mVrVulkanTextureData.m_nQueueFamilyIndex = vulkanContext.getGraphicsQueueFamilyIndex();	// m_nQueueFamilyIndex (uint32_t)
			mVrVulkanTextureData.m_nWidth = width;										// m_nWidth (uint32_t)
			mVrVulkanTextureData.m_nHeight = height;										// m_nHeight (uint32_t)
																									// m_nFormat (VkFormat)  was set by "VulkanRhi::Helper::createAndFillVkImage()" above
			mVrVulkanTextureData.m_nSampleCount = numberOfMultisamples;						// m_nSampleCount (uint32_t)

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)	// 13 = "2D texture: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (uint64_t)mVrVulkanTextureData.m_nImage, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (uint64_t)mVkImageView, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture2D() override
		{
			Helper::destroyAndFreeVkImage(static_cast<VulkanRhi&>(getRhi()), mVrVulkanTextureData.m_nImage, mVkDeviceMemory, mVkImageView);
		}

		/**
		*  @brief
		*    Return the Vulkan image view
		*
		*  @return
		*    The Vulkan image view
		*/
		[[nodiscard]] inline VkImageView getVkImageView() const
		{
			return mVkImageView;
		}

		/**
		*  @brief
		*    Return the Vulkan image layout
		*
		*  @return
		*    The Vulkan image layout
		*/
		[[nodiscard]] inline VkImageLayout getVkImageLayout() const
		{
			return mVkImageLayout;
		}

		/**
		*  @brief
		*    Return the Vulkan format
		*
		*  @return
		*    The Vulkan format
		*/
		[[nodiscard]] inline VkFormat getVkFormat() const
		{
			return mVrVulkanTextureData.m_nFormat;
		}

		/**
		*  @brief
		*    Set minimum maximum mipmap index
		*
		*  @param[in] minimumMipmapIndex
		*    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
		*  @param[in] maximumMipmapIndex
		*    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
		*/
		inline void setMinimumMaximumMipmapIndex([[maybe_unused]] uint32_t minimumMipmapIndex, [[maybe_unused]] uint32_t maximumMipmapIndex)
		{
			// TODO(co) Implement me
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IResource methods                 ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual void* getInternalResourceHandle() const override
		{
			return reinterpret_cast<void*>(const_cast<VRVulkanTextureData_t*>(&mVrVulkanTextureData));
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), Texture2D, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture2D(const Texture2D&) = delete;
		Texture2D& operator =(const Texture2D&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VRVulkanTextureData_t mVrVulkanTextureData;
		VkImageLayout		  mVkImageLayout;
		VkDeviceMemory		  mVkDeviceMemory;
		VkImageView			  mVkImageView;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Texture/Texture2DArray.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan 2D array texture interface
	*/
	class Texture2DArray final : public Rhi::ITexture2DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture2DArray(VulkanRhi& vulkanRhi, uint32_t width, uint32_t height, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture2DArray(vulkanRhi, width, height, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkImage(VK_NULL_HANDLE),
			mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
			mVkDeviceMemory(VK_NULL_HANDLE),
			mVkImageView(VK_NULL_HANDLE),
			mVkFormat(Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D_ARRAY, { width, height, numberOfSlices }, textureFormat, data, textureFlags, 1, mVkImage, mVkDeviceMemory, mVkImageView))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture array", 19)	// 19 = "2D texture array: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (uint64_t)mVkImage, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (uint64_t)mVkImageView, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture2DArray() override
		{
			Helper::destroyAndFreeVkImage(static_cast<VulkanRhi&>(getRhi()), mVkImage, mVkDeviceMemory, mVkImageView);
		}

		/**
		*  @brief
		*    Return the Vulkan image view
		*
		*  @return
		*    The Vulkan image view
		*/
		[[nodiscard]] inline VkImageView getVkImageView() const
		{
			return mVkImageView;
		}

		/**
		*  @brief
		*    Return the Vulkan image layout
		*
		*  @return
		*    The Vulkan image layout
		*/
		[[nodiscard]] inline VkImageLayout getVkImageLayout() const
		{
			return mVkImageLayout;
		}

		/**
		*  @brief
		*    Return the Vulkan format
		*
		*  @return
		*    The Vulkan format
		*/
		[[nodiscard]] inline VkFormat getVkFormat() const
		{
			return mVkFormat;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), Texture2DArray, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture2DArray(const Texture2DArray&) = delete;
		Texture2DArray& operator =(const Texture2DArray&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkImage		   mVkImage;
		VkImageLayout  mVkImageLayout;
		VkDeviceMemory mVkDeviceMemory;
		VkImageView	   mVkImageView;
		VkFormat	   mVkFormat;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Texture/Texture3D.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan 3D texture interface
	*/
	class Texture3D final : public Rhi::ITexture3D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] depth
		*    Texture depth, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture3D(VulkanRhi& vulkanRhi, uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture3D(vulkanRhi, width, height, depth RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkImage(VK_NULL_HANDLE),
			mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
			mVkDeviceMemory(VK_NULL_HANDLE),
			mVkImageView(VK_NULL_HANDLE)
		{
			Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_3D, VK_IMAGE_VIEW_TYPE_3D, { width, height, depth }, textureFormat, data, textureFlags, 1, mVkImage, mVkDeviceMemory, mVkImageView);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "3D texture", 13)	// 13 = "3D texture: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (uint64_t)mVkImage, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (uint64_t)mVkImageView, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture3D() override
		{
			Helper::destroyAndFreeVkImage(static_cast<VulkanRhi&>(getRhi()), mVkImage, mVkDeviceMemory, mVkImageView);
		}

		/**
		*  @brief
		*    Return the Vulkan image view
		*
		*  @return
		*    The Vulkan image view
		*/
		[[nodiscard]] inline VkImageView getVkImageView() const
		{
			return mVkImageView;
		}

		/**
		*  @brief
		*    Return the Vulkan image layout
		*
		*  @return
		*    The Vulkan image layout
		*/
		[[nodiscard]] inline VkImageLayout getVkImageLayout() const
		{
			return mVkImageLayout;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), Texture3D, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture3D(const Texture3D&) = delete;
		Texture3D& operator =(const Texture3D&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkImage		   mVkImage;
		VkImageLayout  mVkImageLayout;
		VkDeviceMemory mVkDeviceMemory;
		VkImageView	   mVkImageView;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Texture/TextureCube.h                       ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan cube texture interface
	*/
	class TextureCube final : public Rhi::ITextureCube
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		TextureCube(VulkanRhi& vulkanRhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITextureCube(vulkanRhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkImage(VK_NULL_HANDLE),
			mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
			mVkDeviceMemory(VK_NULL_HANDLE),
			mVkImageView(VK_NULL_HANDLE)
		{
			Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_CUBE, { width, width, 6 }, textureFormat, data, textureFlags, 1, mVkImage, mVkDeviceMemory, mVkImageView);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Cube texture", 15)	// 15 = "Cube texture: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (uint64_t)mVkImage, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (uint64_t)mVkImageView, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureCube() override
		{
			Helper::destroyAndFreeVkImage(static_cast<VulkanRhi&>(getRhi()), mVkImage, mVkDeviceMemory, mVkImageView);
		}

		/**
		*  @brief
		*    Return the Vulkan image view
		*
		*  @return
		*    The Vulkan image view
		*/
		[[nodiscard]] inline VkImageView getVkImageView() const
		{
			return mVkImageView;
		}

		/**
		*  @brief
		*    Return the Vulkan image layout
		*
		*  @return
		*    The Vulkan image layout
		*/
		[[nodiscard]] inline VkImageLayout getVkImageLayout() const
		{
			return mVkImageLayout;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), TextureCube, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureCube(const TextureCube&) = delete;
		TextureCube& operator =(const TextureCube&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkImage		   mVkImage;
		VkImageLayout  mVkImageLayout;
		VkDeviceMemory mVkDeviceMemory;
		VkImageView	   mVkImageView;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Texture/TextureCubeArray.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan cube texture array interface
	*/
	class TextureCubeArray final : public Rhi::ITextureCubeArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		TextureCubeArray(VulkanRhi& vulkanRhi, uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITextureCubeArray(vulkanRhi, width, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkImage(VK_NULL_HANDLE),
			mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
			mVkDeviceMemory(VK_NULL_HANDLE),
			mVkImageView(VK_NULL_HANDLE)
		{
			Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_CUBE_ARRAY, { width, width, numberOfSlices * 6 }, textureFormat, data, textureFlags, 1, mVkImage, mVkDeviceMemory, mVkImageView);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Cube texture array", 21)	// 21 = "Cube texture array: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (uint64_t)mVkImage, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
				Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (uint64_t)mVkImageView, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureCubeArray() override
		{
			Helper::destroyAndFreeVkImage(static_cast<VulkanRhi&>(getRhi()), mVkImage, mVkDeviceMemory, mVkImageView);
		}

		/**
		*  @brief
		*    Return the Vulkan image view
		*
		*  @return
		*    The Vulkan image view
		*/
		[[nodiscard]] inline VkImageView getVkImageView() const
		{
			return mVkImageView;
		}

		/**
		*  @brief
		*    Return the Vulkan image layout
		*
		*  @return
		*    The Vulkan image layout
		*/
		[[nodiscard]] inline VkImageLayout getVkImageLayout() const
		{
			return mVkImageLayout;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), TextureCubeArray, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureCubeArray(const TextureCubeArray&) = delete;
		TextureCubeArray& operator =(const TextureCubeArray&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkImage		   mVkImage;
		VkImageLayout  mVkImageLayout;
		VkDeviceMemory mVkDeviceMemory;
		VkImageView	   mVkImageView;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Texture/TextureManager.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan texture manager interface
	*/
	class TextureManager final : public Rhi::ITextureManager
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*/
		inline explicit TextureManager(VulkanRhi& vulkanRhi) :
			ITextureManager(vulkanRhi)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureManager() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::ITextureManager methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual Rhi::ITexture1D* createTexture1D(uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(width > 0, "Vulkan create texture 1D was called with invalid parameters")

				// Create 1D texture resource
				// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
				return RHI_NEW(vulkanRhi.getContext(), Texture1D)(vulkanRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITexture1DArray* createTexture1DArray(uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(width > 0 && numberOfSlices > 0, "Vulkan create texture 1D array was called with invalid parameters")

				// Create 1D texture array resource
				// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
				return RHI_NEW(vulkanRhi.getContext(), Texture1DArray)(vulkanRhi, width, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITexture2D* createTexture2D(uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT, uint8_t numberOfMultisamples = 1, [[maybe_unused]] const Rhi::OptimizedTextureClearValue* optimizedTextureClearValue = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(width > 0 && height > 0, "Vulkan create texture 2D was called with invalid parameters")

				// Create 2D texture resource
				// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
				return RHI_NEW(vulkanRhi.getContext(), Texture2D)(vulkanRhi, width, height, textureFormat, data, textureFlags, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITexture2DArray* createTexture2DArray(uint32_t width, uint32_t height, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(width > 0 && height > 0 && numberOfSlices > 0, "Vulkan create texture 2D array was called with invalid parameters")

				// Create 2D texture array resource
				// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
				return RHI_NEW(vulkanRhi.getContext(), Texture2DArray)(vulkanRhi, width, height, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITexture3D* createTexture3D(uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(width > 0 && height > 0 && depth > 0, "Vulkan create texture 3D was called with invalid parameters")

				// Create 3D texture resource
				// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
				return RHI_NEW(vulkanRhi.getContext(), Texture3D)(vulkanRhi, width, height, depth, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITextureCube* createTextureCube(uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(width > 0, "Vulkan create texture cube was called with invalid parameters")

				// Create cube texture resource
				// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
				return RHI_NEW(vulkanRhi.getContext(), TextureCube)(vulkanRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITextureCubeArray* createTextureCubeArray(uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(width > 0 && numberOfSlices > 0, "Vulkan create texture cube was called with invalid parameters")

				// Create cube texture resource
				// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
				return RHI_NEW(vulkanRhi.getContext(), TextureCubeArray)(vulkanRhi, width, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), TextureManager, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureManager(const TextureManager&) = delete;
		TextureManager& operator =(const TextureManager&) = delete;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/State/SamplerState.h                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan sampler state interface
	*/
	class SamplerState final : public Rhi::ISamplerState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] samplerState
		*    Sampler state to use
		*/
		SamplerState(VulkanRhi& vulkanRhi, const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ISamplerState(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkSampler(VK_NULL_HANDLE)
		{
			// Sanity checks
			RHI_ASSERT(samplerState.filter != Rhi::FilterMode::UNKNOWN, "Vulkan filter mode must not be unknown")
				RHI_ASSERT(samplerState.maxAnisotropy <= vulkanRhi.getCapabilities().maximumAnisotropy, "Maximum Vulkan anisotropy value violated")

				// TODO(co) Map "Rhi::SamplerState" to VkSamplerCreateInfo
				const bool anisotropyEnable = (Rhi::FilterMode::ANISOTROPIC == samplerState.filter || Rhi::FilterMode::COMPARISON_ANISOTROPIC == samplerState.filter);
			const VkSamplerCreateInfo vkSamplerCreateInfo =
			{
				VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,											// sType (VkStructureType)
				nullptr,																		// pNext (const void*)
				0,																				// flags (VkSamplerCreateFlags)
				Mapping::getVulkanMagFilterMode(vulkanRhi.getContext(), samplerState.filter),	// magFilter (VkFilter)
				Mapping::getVulkanMinFilterMode(vulkanRhi.getContext(), samplerState.filter),	// minFilter (VkFilter)
				Mapping::getVulkanMipmapMode(vulkanRhi.getContext(), samplerState.filter),		// mipmapMode (VkSamplerMipmapMode)
				Mapping::getVulkanTextureAddressMode(samplerState.addressU),					// addressModeU (VkSamplerAddressMode)
				Mapping::getVulkanTextureAddressMode(samplerState.addressV),					// addressModeV (VkSamplerAddressMode)
				Mapping::getVulkanTextureAddressMode(samplerState.addressW),					// addressModeW (VkSamplerAddressMode)
				samplerState.mipLodBias,														// mipLodBias (float)
				static_cast<VkBool32>(anisotropyEnable),										// anisotropyEnable (VkBool32)
				static_cast<float>(samplerState.maxAnisotropy),									// maxAnisotropy (float)
				VK_FALSE,																		// compareEnable (VkBool32)
				VK_COMPARE_OP_ALWAYS,															// compareOp (VkCompareOp)
				samplerState.minLod,															// minLod (float)
				samplerState.maxLod,															// maxLod (float)
				VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,										// borderColor (VkBorderColor)
				VK_FALSE																		// unnormalizedCoordinates (VkBool32)
			};
			if ( vkCreateSampler(vulkanRhi.getVulkanContext().getVkDevice(), &vkSamplerCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkSampler) == VK_SUCCESS )
			{
				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				if ( nullptr != vkDebugMarkerSetObjectNameEXT )
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Sampler state", 16)	// 16 = "Sampler state: " including terminating zero
						Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT, (uint64_t)mVkSampler, detailedDebugName);
				}
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Vulkan sampler instance")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SamplerState() override
		{
			if ( VK_NULL_HANDLE != mVkSampler )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroySampler(vulkanRhi.getVulkanContext().getVkDevice(), mVkSampler, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan sampler
		*
		*  @return
		*    The Vulkan sampler
		*/
		[[nodiscard]] inline VkSampler getVkSampler() const
		{
			return mVkSampler;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), SamplerState, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SamplerState(const SamplerState&) = delete;
		SamplerState& operator =(const SamplerState&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkSampler mVkSampler;	// Vulkan sampler instance, "VK_NULL_HANDLE" in case of error


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/RenderTarget/RenderPass.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan render pass interface
	*/
	class RenderPass final : public Rhi::IRenderPass
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] numberOfColorAttachments
		*    Number of color render target textures, must be <="Rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets"
		*  @param[in] colorAttachmentTextureFormats
		*    The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "numberOfColorAttachments" textures in the provided C-array of pointers
		*  @param[in] depthStencilAttachmentTextureFormat
		*    The optional depth stencil render target texture format, can be a "Rhi::TextureFormat::UNKNOWN" if there should be no depth buffer
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*/
		RenderPass(VulkanRhi& vulkanRhi, uint32_t numberOfColorAttachments, const Rhi::TextureFormat::Enum* colorAttachmentTextureFormats, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IRenderPass(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkRenderPass(VK_NULL_HANDLE),
			mNumberOfColorAttachments(numberOfColorAttachments),
			mDepthStencilAttachmentTextureFormat(depthStencilAttachmentTextureFormat),
			mVkSampleCountFlagBits(Mapping::getVulkanSampleCountFlagBits(vulkanRhi.getContext(), numberOfMultisamples))
		{
			const bool hasDepthStencilAttachment = (Rhi::TextureFormat::Enum::UNKNOWN != depthStencilAttachmentTextureFormat);

			// Vulkan attachment descriptions
			std::vector<VkAttachmentDescription> vkAttachmentDescriptions;
			vkAttachmentDescriptions.resize(mNumberOfColorAttachments + (hasDepthStencilAttachment ? 1u : 0u));
			uint32_t currentVkAttachmentDescriptionIndex = 0;

			// Handle color attachments
			typedef std::vector<VkAttachmentReference> VkAttachmentReferences;
			VkAttachmentReferences colorVkAttachmentReferences;
			if ( mNumberOfColorAttachments > 0 )
			{
				colorVkAttachmentReferences.resize(mNumberOfColorAttachments);
				for ( uint32_t i = 0; i < mNumberOfColorAttachments; ++i )
				{
					{ // Setup Vulkan color attachment references
						VkAttachmentReference& vkAttachmentReference = colorVkAttachmentReferences[currentVkAttachmentDescriptionIndex];
						vkAttachmentReference.attachment = currentVkAttachmentDescriptionIndex;			// attachment (uint32_t)
						vkAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;	// layout (VkImageLayout)
					}

					{ // Setup Vulkan color attachment description
						VkAttachmentDescription& vkAttachmentDescription = vkAttachmentDescriptions[currentVkAttachmentDescriptionIndex];
						vkAttachmentDescription.flags = 0;																// flags (VkAttachmentDescriptionFlags)
						vkAttachmentDescription.format = Mapping::getVulkanFormat(colorAttachmentTextureFormats[i]);	// format (VkFormat)
						vkAttachmentDescription.samples = mVkSampleCountFlagBits;										// samples (VkSampleCountFlagBits)
						vkAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;									// loadOp (VkAttachmentLoadOp)
						vkAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;									// storeOp (VkAttachmentStoreOp)
						vkAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;								// stencilLoadOp (VkAttachmentLoadOp)
						vkAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;								// stencilStoreOp (VkAttachmentStoreOp)
						vkAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;										// initialLayout (VkImageLayout)
						vkAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;						// finalLayout (VkImageLayout)
					}

					// Advance current Vulkan attachment description index
					++currentVkAttachmentDescriptionIndex;
				}
			}

			// Handle depth stencil attachments
			const VkAttachmentReference depthVkAttachmentReference =
			{
				currentVkAttachmentDescriptionIndex,				// attachment (uint32_t)
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL	// layout (VkImageLayout)
			};
			if ( hasDepthStencilAttachment )
			{
				// Setup Vulkan depth attachment description
				VkAttachmentDescription& vkAttachmentDescription = vkAttachmentDescriptions[currentVkAttachmentDescriptionIndex];
				vkAttachmentDescription.flags = 0;																// flags (VkAttachmentDescriptionFlags)
				vkAttachmentDescription.format = Mapping::getVulkanFormat(depthStencilAttachmentTextureFormat);	// format (VkFormat)
				vkAttachmentDescription.samples = mVkSampleCountFlagBits;										// samples (VkSampleCountFlagBits)
				vkAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;									// loadOp (VkAttachmentLoadOp)
				vkAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;									// storeOp (VkAttachmentStoreOp)
				vkAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;								// stencilLoadOp (VkAttachmentLoadOp)
				vkAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;								// stencilStoreOp (VkAttachmentStoreOp)
				vkAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;										// initialLayout (VkImageLayout)
				vkAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;				// finalLayout (VkImageLayout)
				// ++currentVkAttachmentDescriptionIndex;	// Not needed since we're the last
			}

			// Create Vulkan create render pass
			const VkSubpassDescription vkSubpassDescription =
			{
				0,																				// flags (VkSubpassDescriptionFlags)
				VK_PIPELINE_BIND_POINT_GRAPHICS,												// pipelineBindPoint (VkPipelineBindPoint)
				0,																				// inputAttachmentCount (uint32_t)
				nullptr,																		// pInputAttachments (const VkAttachmentReference*)
				mNumberOfColorAttachments,														// colorAttachmentCount (uint32_t)
				(mNumberOfColorAttachments > 0) ? colorVkAttachmentReferences.data() : nullptr,	// pColorAttachments (const VkAttachmentReference*)
				nullptr,																		// pResolveAttachments (const VkAttachmentReference*)
				hasDepthStencilAttachment ? &depthVkAttachmentReference : nullptr,				// pDepthStencilAttachment (const VkAttachmentReference*)
				0,																				// preserveAttachmentCount (uint32_t)
				nullptr																			// pPreserveAttachments (const uint32_t*)
			};
			static constexpr std::array<VkSubpassDependency, 2> vkSubpassDependencies =
			{ {
				{
					VK_SUBPASS_EXTERNAL,														// srcSubpass (uint32_t)
					0,																			// dstSubpass (uint32_t)
					VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,										// srcStageMask (VkPipelineStageFlags)
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// dstStageMask (VkPipelineStageFlags)
					VK_ACCESS_MEMORY_READ_BIT,													// srcAccessMask (VkAccessFlags)
					VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,	// dstAccessMask (VkAccessFlags)
					VK_DEPENDENCY_BY_REGION_BIT													// dependencyFlags (VkDependencyFlags)
				},
				{
					0,																			// srcSubpass (uint32_t)
					VK_SUBPASS_EXTERNAL,														// dstSubpass (uint32_t)
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// srcStageMask (VkPipelineStageFlags)
					VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,										// dstStageMask (VkPipelineStageFlags)
					VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,	// srcAccessMask (VkAccessFlags)
					VK_ACCESS_MEMORY_READ_BIT,													// dstAccessMask (VkAccessFlags)
					VK_DEPENDENCY_BY_REGION_BIT													// dependencyFlags (VkDependencyFlags)
				}
			} };
			const VkRenderPassCreateInfo vkRenderPassCreateInfo =
			{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,				// sType (VkStructureType)
				nullptr,												// pNext (const void*)
				0,														// flags (VkRenderPassCreateFlags)
				static_cast<uint32_t>(vkAttachmentDescriptions.size()),	// attachmentCount (uint32_t)
				vkAttachmentDescriptions.data(),						// pAttachments (const VkAttachmentDescription*)
				1,														// subpassCount (uint32_t)
				&vkSubpassDescription,									// pSubpasses (const VkSubpassDescription*)
				static_cast<uint32_t>(vkSubpassDependencies.size()),	// dependencyCount (uint32_t)
				vkSubpassDependencies.data()							// pDependencies (const VkSubpassDependency*)
			};
			if ( vkCreateRenderPass(vulkanRhi.getVulkanContext().getVkDevice(), &vkRenderPassCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkRenderPass) == VK_SUCCESS )
			{
				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				if ( nullptr != vkDebugMarkerSetObjectNameEXT )
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Render pass", 14)	// 14 = "Render pass: " including terminating zero
						Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, (uint64_t)mVkRenderPass, detailedDebugName);
				}
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Vulkan render pass")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RenderPass() override
		{
			// Destroy Vulkan render pass instance
			if ( VK_NULL_HANDLE != mVkRenderPass )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyRenderPass(vulkanRhi.getVulkanContext().getVkDevice(), mVkRenderPass, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan render pass
		*
		*  @return
		*    The Vulkan render pass
		*/
		[[nodiscard]] inline VkRenderPass getVkRenderPass() const
		{
			return mVkRenderPass;
		}

		/**
		*  @brief
		*    Return the number of color render target textures
		*
		*  @return
		*    The number of color render target textures
		*/
		[[nodiscard]] inline uint32_t getNumberOfColorAttachments() const
		{
			return mNumberOfColorAttachments;
		}

		/**
		*  @brief
		*    Return the number of render target textures (color and depth stencil)
		*
		*  @return
		*    The number of render target textures (color and depth stencil)
		*/
		[[nodiscard]] inline uint32_t getNumberOfAttachments() const
		{
			return (mDepthStencilAttachmentTextureFormat != Rhi::TextureFormat::Enum::UNKNOWN) ? (mNumberOfColorAttachments + 1) : mNumberOfColorAttachments;
		}

		/**
		*  @brief
		*    Return the depth stencil attachment texture format
		*
		*  @return
		*    The depth stencil attachment texture format
		*/
		[[nodiscard]] inline Rhi::TextureFormat::Enum getDepthStencilAttachmentTextureFormat() const
		{
			return mDepthStencilAttachmentTextureFormat;
		}

		/**
		*  @brief
		*    Return the Vulkan sample count flag bits
		*
		*  @return
		*    The Vulkan sample count flag bits
		*/
		[[nodiscard]] inline VkSampleCountFlagBits getVkSampleCountFlagBits() const
		{
			return mVkSampleCountFlagBits;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), RenderPass, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit RenderPass(const RenderPass&) = delete;
		RenderPass& operator =(const RenderPass&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkRenderPass			 mVkRenderPass;							// Vulkan render pass instance, can be a null handle
		uint32_t				 mNumberOfColorAttachments;				// Number of color render target textures
		Rhi::TextureFormat::Enum mDepthStencilAttachmentTextureFormat;	// The depth stencil attachment texture format
		VkSampleCountFlagBits	 mVkSampleCountFlagBits;				// Vulkan sample count flag bits


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/QueryPool.h                                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan asynchronous query pool interface
	*/
	class QueryPool final : public Rhi::IQueryPool
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] queryType
		*    Query type
		*  @param[in] numberOfQueries
		*    Number of queries
		*/
		QueryPool(VulkanRhi& vulkanRhi, Rhi::QueryType queryType, uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IQueryPool(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mQueryType(queryType),
			mVkQueryPool(VK_NULL_HANDLE)
		{
			// Get Vulkan query pool create information
			VkQueryPoolCreateInfo vkQueryPoolCreateInfo;
			vkQueryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;	// VkStructureType
			vkQueryPoolCreateInfo.pNext = nullptr;										// const void*
			vkQueryPoolCreateInfo.flags = 0;											// VkQueryPoolCreateFlags
			vkQueryPoolCreateInfo.queryCount = numberOfQueries;								// uint32_t
			switch ( queryType )
			{
			case Rhi::QueryType::OCCLUSION:
				vkQueryPoolCreateInfo.queryType = VK_QUERY_TYPE_OCCLUSION;	// VkQueryType
				vkQueryPoolCreateInfo.pipelineStatistics = 0;						// VkQueryPipelineStatisticFlags
				break;

			case Rhi::QueryType::PIPELINE_STATISTICS:
				// This setup results in the same structure layout as used by "D3D11_QUERY_DATA_PIPELINE_STATISTICS" which we use for "Rhi::PipelineStatisticsQueryResult"
				vkQueryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;	// VkQueryType
				vkQueryPoolCreateInfo.pipelineStatistics = 										// VkQueryPipelineStatisticFlags
					VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
					VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
					VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
					VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT |
					VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT |
					VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
					VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
					VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
					VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT |
					VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT |
					VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;
				break;

			case Rhi::QueryType::TIMESTAMP:
				vkQueryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;	// VkQueryType
				vkQueryPoolCreateInfo.pipelineStatistics = 0;						// VkQueryPipelineStatisticFlags
				break;
			}

			// Create Vulkan query pool
			if ( vkCreateQueryPool(vulkanRhi.getVulkanContext().getVkDevice(), &vkQueryPoolCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkQueryPool) == VK_SUCCESS )
			{
				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				if ( nullptr != vkDebugMarkerSetObjectNameEXT )
				{
					switch ( queryType )
					{
					case Rhi::QueryType::OCCLUSION:
						{
							RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Occlusion query", 18)	// 18 = "Occlusion query: " including terminating zero
								Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT, (uint64_t)mVkQueryPool, detailedDebugName);
							break;
						}

					case Rhi::QueryType::PIPELINE_STATISTICS:
						{
							RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Pipeline statistics query", 28)	// 28 = "Pipeline statistics query: " including terminating zero
								Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT, (uint64_t)mVkQueryPool, detailedDebugName);
							break;
						}

					case Rhi::QueryType::TIMESTAMP:
						{
							RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Timestamp query", 18)	// 18 = "Timestamp query: " including terminating zero
								Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT, (uint64_t)mVkQueryPool, detailedDebugName);
							break;
						}
					}
				}
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Vulkan query pool")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~QueryPool() override
		{
			// Destroy Vulkan query pool instance
			if ( VK_NULL_HANDLE != mVkQueryPool )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyQueryPool(vulkanRhi.getVulkanContext().getVkDevice(), mVkQueryPool, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the query type
		*
		*  @return
		*    The query type
		*/
		[[nodiscard]] inline Rhi::QueryType getQueryType() const
		{
			return mQueryType;
		}

		/**
		*  @brief
		*    Return the Vulkan query pool
		*
		*  @return
		*    The Vulkan query pool
		*/
		[[nodiscard]] inline VkQueryPool getVkQueryPool() const
		{
			return mVkQueryPool;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), QueryPool, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit QueryPool(const QueryPool&) = delete;
		QueryPool& operator =(const QueryPool&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::QueryType mQueryType;
		VkQueryPool	   mVkQueryPool;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/RenderTarget/SwapChain.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan swap chain class
	*
	*  @todo
	*    - TODO(co) Add support for debug name (not that important while at the same time more complex to implement here, but lets keep the TODO here to know there's room for improvement)
	*/
	class SwapChain final : public Rhi::ISwapChain
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] static VkFormat findColorVkFormat(const Rhi::Context& context, VkInstance vkInstance, const VulkanContext& vulkanContext)
		{
			const VkPhysicalDevice vkPhysicalDevice = vulkanContext.getVkPhysicalDevice();
			const VkSurfaceKHR vkSurfaceKHR = detail::createPresentationSurface(context, vulkanContext.getVulkanRhi().getVkAllocationCallbacks(), vkInstance, vkPhysicalDevice, vulkanContext.getGraphicsQueueFamilyIndex(), Rhi::WindowHandle{ context.getNativeWindowHandle(), nullptr });
			const VkSurfaceFormatKHR desiredVkSurfaceFormatKHR = ::detail::getSwapChainFormat(context, vkPhysicalDevice, vkSurfaceKHR);
			vkDestroySurfaceKHR(vkInstance, vkSurfaceKHR, vulkanContext.getVulkanRhi().getVkAllocationCallbacks());
			return desiredVkSurfaceFormatKHR.format;
		}

		[[nodiscard]] inline static VkFormat findDepthVkFormat(VkPhysicalDevice vkPhysicalDevice)
		{
			return detail::findSupportedVkFormat(vkPhysicalDevice, { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
		}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] windowHandle
		*    Information about the window to render into
		*/
		SwapChain(Rhi::IRenderPass& renderPass, Rhi::WindowHandle windowHandle RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ISwapChain(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			// Operation system window
			mNativeWindowHandle(windowHandle.nativeWindowHandle),
			mRenderWindow(windowHandle.renderWindow),
			// Vulkan presentation surface
			mVkSurfaceKHR(VK_NULL_HANDLE),
			// Vulkan swap chain and color render target related
			mVkSwapchainKHR(VK_NULL_HANDLE),
			mVkRenderPass(VK_NULL_HANDLE),
			mImageAvailableVkSemaphore(VK_NULL_HANDLE),
			mRenderingFinishedVkSemaphore(VK_NULL_HANDLE),
			mCurrentImageIndex(~0u),
			// Depth render target related
			mDepthVkFormat(Mapping::getVulkanFormat(static_cast<RenderPass&>(renderPass).getDepthStencilAttachmentTextureFormat())),
			mDepthVkImage(VK_NULL_HANDLE),
			mDepthVkDeviceMemory(VK_NULL_HANDLE),
			mDepthVkImageView(VK_NULL_HANDLE)
		{
			// Create the Vulkan presentation surface instance depending on the operation system
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(renderPass.getRhi());
			const VulkanContext&   vulkanContext = vulkanRhi.getVulkanContext();
			const VkInstance	   vkInstance = vulkanRhi.getVulkanRuntimeLinking().getVkInstance();
			const VkPhysicalDevice vkPhysicalDevice = vulkanContext.getVkPhysicalDevice();
			mVkSurfaceKHR = detail::createPresentationSurface(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vkInstance, vkPhysicalDevice, vulkanContext.getGraphicsQueueFamilyIndex(), windowHandle);
			if ( VK_NULL_HANDLE != mVkSurfaceKHR )
			{
				// Create the Vulkan swap chain
				createVulkanSwapChain();
			}
			else
			{
				// Error!
				RHI_LOG(CRITICAL, "The swap chain failed to create the Vulkan presentation surface")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SwapChain() override
		{
			if ( VK_NULL_HANDLE != mVkSurfaceKHR )
			{
				destroyVulkanSwapChain();
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroySurfaceKHR(vulkanRhi.getVulkanRuntimeLinking().getVkInstance(), mVkSurfaceKHR, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan render pass
		*
		*  @return
		*    The Vulkan render pass
		*/
		[[nodiscard]] inline VkRenderPass getVkRenderPass() const
		{
			return mVkRenderPass;
		}

		/**
		*  @brief
		*    Return the current Vulkan image to render color into
		*
		*  @return
		*    The current Vulkan image to render color into
		*/
		[[nodiscard]] inline VkImage getColorCurrentVkImage() const
		{
			RHI_ASSERT(~0u != mCurrentImageIndex, "Invalid index of the current Vulkan swap chain image to render into (Vulkan swap chain creation failed?)")
				RHI_ASSERT(mCurrentImageIndex < mSwapChainBuffer.size(), "Out-of-bounds index of the current Vulkan swap chain image to render into")
				return mSwapChainBuffer[mCurrentImageIndex].vkImage;
		}

		/**
		*  @brief
		*    Return the Vulkan image to render depth into
		*
		*  @return
		*    The Vulkan image to render depth into
		*/
		[[nodiscard]] inline VkImage getDepthVkImage() const
		{
			return mDepthVkImage;
		}

		/**
		*  @brief
		*    Return the current Vulkan framebuffer to render into
		*
		*  @return
		*    The current Vulkan framebuffer to render into
		*/
		[[nodiscard]] inline VkFramebuffer getCurrentVkFramebuffer() const
		{
			RHI_ASSERT(~0u != mCurrentImageIndex, "Invalid index of the current Vulkan swap chain image to render into (Vulkan swap chain creation failed?)")
				RHI_ASSERT(mCurrentImageIndex < mSwapChainBuffer.size(), "Out-of-bounds index of the current Vulkan swap chain image to render into")
				return mSwapChainBuffer[mCurrentImageIndex].vkFramebuffer;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRenderTarget methods             ]
		//[-------------------------------------------------------]
	public:
		virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const override
		{
			// Return stored width and height when both valid
			if ( nullptr != mRenderWindow )
			{
				mRenderWindow->getWidthAndHeight(width, height);
				return;
			}
#if SE_PLATFORM_WINDOWS
			// Is there a valid native OS window?
			if ( SE_NULL_HANDLE != mNativeWindowHandle )
			{
				// Get the width and height
				long swapChainWidth = 1;
				long swapChainHeight = 1;
				{
					// Get the client rectangle of the native output window
					// -> Don't use the width and height stored in "DXGI_SWAP_CHAIN_DESC" -> "DXGI_MODE_DESC"
					//    because it might have been modified in order to avoid zero values
					RECT rect;
					::GetClientRect(reinterpret_cast<HWND>(mNativeWindowHandle), &rect);

					// Get the width and height...
					swapChainWidth = rect.right - rect.left;
					swapChainHeight = rect.bottom - rect.top;

					// ... and ensure that none of them is ever zero
					if ( swapChainWidth < 1 )
					{
						swapChainWidth = 1;
					}
					if ( swapChainHeight < 1 )
					{
						swapChainHeight = 1;
					}
				}

				// Write out the width and height
				width = static_cast<UINT>(swapChainWidth);
				height = static_cast<UINT>(swapChainHeight);
			}
			else
#elif defined(__ANDROID__)
			if ( SE_NULL_HANDLE != mNativeWindowHandle )
			{
				// TODO(co) Get size on Android
				width = height = 1;
			}
			else
#elif defined LINUX
			if ( SE_NULL_HANDLE != mNativeWindowHandle )
			{
				VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				const Rhi::Context& context = vulkanRhi.getContext();
				RHI_ASSERT(context.getType() == Rhi::Context::ContextType::X11, "Invalid Vulkan context type")

					// If the given RHI context is an X11 context use the display connection object provided by the context
					if ( context.getType() == Rhi::Context::ContextType::X11 )
					{
						const Rhi::X11Context& x11Context = static_cast<const Rhi::X11Context&>(context);
						Display* display = x11Context.getDisplay();

						// Get the width and height...
						::Window rootWindow = 0;
						int positionX = 0, positionY = 0;
						unsigned int unsignedWidth = 0, unsignedHeight = 0, border = 0, depth = 0;
						if ( nullptr != display )
						{
							XGetGeometry(display, mNativeWindowHandle, &rootWindow, &positionX, &positionY, &unsignedWidth, &unsignedHeight, &border, &depth);
						}

						// ... and ensure that none of them is ever zero
						if ( unsignedWidth < 1 )
						{
							unsignedWidth = 1;
						}
						if ( unsignedHeight < 1 )
						{
							unsignedHeight = 1;
						}

						// Done
						width = unsignedWidth;
						height = unsignedHeight;
					}
			}
			else
#else
#error "Unsupported platform"
#endif
			{
				// Set known default return values
				width = 1;
				height = 1;
			}
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::ISwapChain methods                ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual handle getNativeWindowHandle() const override
		{
			return mNativeWindowHandle;
		}

		inline virtual void setVerticalSynchronizationInterval(uint32_t) override
		{
			// TODO(co) Implement usage of "synchronizationInterval"
		}

		virtual void present() override
		{
			// TODO(co) "Rhi::IRenderWindow::present()" support
			/*
			if (nullptr != mRenderWindow)
			{
				mRenderWindow->present();
				return;
			}
			*/

			// Get the Vulkan context
			const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			const VulkanContext& vulkanContext = vulkanRhi.getVulkanContext();

			{ // Queue submit
				const VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				const VkCommandBuffer vkCommandBuffer = vulkanContext.getVkCommandBuffer();
				const VkSubmitInfo vkSubmitInfo =
				{
					VK_STRUCTURE_TYPE_SUBMIT_INFO,	// sType (VkStructureType)
					nullptr,						// pNext (const void*)
					1,								// waitSemaphoreCount (uint32_t)
					&mImageAvailableVkSemaphore,	// pWaitSemaphores (const VkSemaphore*)
					&waitDstStageMask,				// pWaitDstStageMask (const VkPipelineStageFlags*)
					1,								// commandBufferCount (uint32_t)
					&vkCommandBuffer,				// pCommandBuffers (const VkCommandBuffer*)
					1,								// signalSemaphoreCount (uint32_t)
					&mRenderingFinishedVkSemaphore	// pSignalSemaphores (const VkSemaphore*)
				};
				if ( vkQueueSubmit(vulkanContext.getGraphicsVkQueue(), 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS )
				{
					// Error!
					RHI_LOG(CRITICAL, "Vulkan queue submit failed")
						return;
				}
			}

			{ // Queue present
				const VkPresentInfoKHR vkPresentInfoKHR =
				{
					VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,	// sType (VkStructureType)
					nullptr,							// pNext (const void*)
					1,									// waitSemaphoreCount (uint32_t)
					&mRenderingFinishedVkSemaphore,		// pWaitSemaphores (const VkSemaphore*)
					1,									// swapchainCount (uint32_t)
					&mVkSwapchainKHR,					// pSwapchains (const VkSwapchainKHR*)
					&mCurrentImageIndex,				// pImageIndices (const uint32_t*)
					nullptr								// pResults (VkResult*)
				};
				const VkResult vkResult = vkQueuePresentKHR(vulkanContext.getPresentVkQueue(), &vkPresentInfoKHR);
				if ( VK_SUCCESS != vkResult )
				{
					if ( VK_ERROR_OUT_OF_DATE_KHR == vkResult || VK_SUBOPTIMAL_KHR == vkResult )
					{
						// Recreate the Vulkan swap chain
						createVulkanSwapChain();
						return;
					}
					else
					{
						// Error!
						RHI_LOG(CRITICAL, "Failed to present Vulkan queue")
							return;
					}
				}
				vkQueueWaitIdle(vulkanContext.getPresentVkQueue());
			}

			// Acquire next image
			acquireNextImage(true);
		}

		inline virtual void resizeBuffers() override
		{
			// Recreate the Vulkan swap chain
			createVulkanSwapChain();
		}

		[[nodiscard]] inline virtual bool getFullscreenState() const override
		{
			// TODO(co) Implement me
			return false;
		}

		inline virtual void setFullscreenState(bool) override
		{
			// TODO(co) Implement me
		}

		inline virtual void setRenderWindow(Rhi::IRenderWindow* renderWindow) override
		{
			mRenderWindow = renderWindow;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), SwapChain, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SwapChain(const SwapChain&) = delete;
		SwapChain& operator =(const SwapChain&) = delete;

		void createVulkanSwapChain()
		{
			// Get the Vulkan physical device
			const VulkanRhi&	   vulkanRhi = static_cast<const VulkanRhi&>(getRhi());
			const Rhi::Context&	   context = vulkanRhi.getContext();
			const VulkanContext&   vulkanContext = vulkanRhi.getVulkanContext();
			const VkPhysicalDevice vkPhysicalDevice = vulkanContext.getVkPhysicalDevice();
			const VkDevice		   vkDevice = vulkanContext.getVkDevice();

			// Sanity checks
			RHI_ASSERT(VK_NULL_HANDLE != vkPhysicalDevice, "Invalid physical Vulkan device")
				RHI_ASSERT(VK_NULL_HANDLE != vkDevice, "Invalid Vulkan device")

				// Wait for the Vulkan device to become idle
				vkDeviceWaitIdle(vkDevice);

			// Get Vulkan surface capabilities
			VkSurfaceCapabilitiesKHR vkSurfaceCapabilitiesKHR;
			if ( vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, mVkSurfaceKHR, &vkSurfaceCapabilitiesKHR) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to get physical Vulkan device surface capabilities")
					return;
			}

			// Get Vulkan swap chain settings
			const uint32_t                      desiredNumberOfImages = ::detail::getNumberOfSwapChainImages(vkSurfaceCapabilitiesKHR);
			const VkSurfaceFormatKHR            desiredVkSurfaceFormatKHR = ::detail::getSwapChainFormat(context, vkPhysicalDevice, mVkSurfaceKHR);
			const VkExtent2D                    desiredVkExtent2D = ::detail::getSwapChainExtent(vkSurfaceCapabilitiesKHR);
			const VkImageUsageFlags             desiredVkImageUsageFlags = ::detail::getSwapChainUsageFlags(context, vkSurfaceCapabilitiesKHR);
			const VkSurfaceTransformFlagBitsKHR desiredVkSurfaceTransformFlagBitsKHR = ::detail::getSwapChainTransform(vkSurfaceCapabilitiesKHR);
			const VkPresentModeKHR              desiredVkPresentModeKHR = ::detail::getSwapChainPresentMode(context, vkPhysicalDevice, mVkSurfaceKHR);

			// Validate Vulkan swap chain settings
			if ( -1 == static_cast<int>(desiredVkImageUsageFlags) )
			{
				RHI_LOG(CRITICAL, "Invalid desired Vulkan image usage flags")
					return;
			}
			if ( VK_PRESENT_MODE_MAX_ENUM_KHR == desiredVkPresentModeKHR )
			{
				RHI_LOG(CRITICAL, "Invalid desired Vulkan presentation mode")
					return;
			}
			if ( (0 == desiredVkExtent2D.width) || (0 == desiredVkExtent2D.height) )
			{
				// Current surface size is (0, 0) so we can't create a swap chain and render anything (CanRender == false)
				// But we don't wont to kill the application as this situation may occur i.e. when window gets minimized
				destroyVulkanSwapChain();
				return;
			}

			{ // Create Vulkan swap chain
				VkSwapchainKHR newVkSwapchainKHR = VK_NULL_HANDLE;
				const VkSwapchainCreateInfoKHR vkSwapchainCreateInfoKHR =
				{
					VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,	// sType (VkStructureType)
					nullptr,										// pNext (const void*)
					0,												// flags (VkSwapchainCreateFlagsKHR)
					mVkSurfaceKHR,									// surface (VkSurfaceKHR)
					desiredNumberOfImages,							// minImageCount (uint32_t)
					desiredVkSurfaceFormatKHR.format,				// imageFormat (VkFormat)
					desiredVkSurfaceFormatKHR.colorSpace,			// imageColorSpace (VkColorSpaceKHR)
					desiredVkExtent2D,								// imageExtent (VkExtent2D)
					1,												// imageArrayLayers (uint32_t)
					desiredVkImageUsageFlags,						// imageUsage (VkImageUsageFlags)
					VK_SHARING_MODE_EXCLUSIVE,						// imageSharingMode (VkSharingMode)
					0,												// queueFamilyIndexCount (uint32_t)
					nullptr,										// pQueueFamilyIndices (const uint32_t*)
					desiredVkSurfaceTransformFlagBitsKHR,			// preTransform (VkSurfaceTransformFlagBitsKHR)
					VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,				// compositeAlpha (VkCompositeAlphaFlagBitsKHR)
					desiredVkPresentModeKHR,						// presentMode (VkPresentModeKHR)
					VK_TRUE,										// clipped (VkBool32)
					mVkSwapchainKHR									// oldSwapchain (VkSwapchainKHR)
				};
				if ( vkCreateSwapchainKHR(vkDevice, &vkSwapchainCreateInfoKHR, vulkanRhi.getVkAllocationCallbacks(), &newVkSwapchainKHR) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to create Vulkan swap chain")
						return;
				}
				destroyVulkanSwapChain();
				mVkSwapchainKHR = newVkSwapchainKHR;
			}

			// Create depth render target
			createDepthRenderTarget(desiredVkExtent2D);

			// Create render pass
			mVkRenderPass = ::detail::createRenderPass(context, vulkanRhi.getVkAllocationCallbacks(), vkDevice, desiredVkSurfaceFormatKHR.format, mDepthVkFormat, static_cast<RenderPass&>(getRenderPass()).getVkSampleCountFlagBits());

			// Vulkan swap chain image handling
			if ( VK_NULL_HANDLE != mVkRenderPass )
			{
				// Get the swap chain images
				uint32_t swapchainImageCount = 0;
				if ( vkGetSwapchainImagesKHR(vkDevice, mVkSwapchainKHR, &swapchainImageCount, nullptr) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to get Vulkan swap chain images")
						return;
				}
				std::vector<VkImage> vkImages(swapchainImageCount);
				if ( vkGetSwapchainImagesKHR(vkDevice, mVkSwapchainKHR, &swapchainImageCount, vkImages.data()) != VK_SUCCESS )
				{
					RHI_LOG(CRITICAL, "Failed to get Vulkan swap chain images")
						return;
				}

				// Get the swap chain buffers containing the image and image view
				mSwapChainBuffer.resize(swapchainImageCount);
				const bool hasDepthStencilAttachment = (VK_FORMAT_UNDEFINED != mDepthVkFormat);
				for ( uint32_t i = 0; i < swapchainImageCount; ++i )
				{
					SwapChainBuffer& swapChainBuffer = mSwapChainBuffer[i];
					swapChainBuffer.vkImage = vkImages[i];

					// Create the Vulkan image view
					Helper::createVkImageView(vulkanRhi, swapChainBuffer.vkImage, VK_IMAGE_VIEW_TYPE_2D, 1, 1, desiredVkSurfaceFormatKHR.format, VK_IMAGE_ASPECT_COLOR_BIT, swapChainBuffer.vkImageView);

					{ // Create the Vulkan framebuffer
						const std::array<VkImageView, 2> vkImageViews =
						{
							swapChainBuffer.vkImageView,
							mDepthVkImageView
						};
						const VkFramebufferCreateInfo vkFramebufferCreateInfo =
						{
							VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,	// sType (VkStructureType)
							nullptr,									// pNext (const void*)
							0,											// flags (VkFramebufferCreateFlags)
							mVkRenderPass,								// renderPass (VkRenderPass)
							hasDepthStencilAttachment ? 2u : 1u,		// attachmentCount (uint32_t)
							vkImageViews.data(),						// pAttachments (const VkImageView*)
							desiredVkExtent2D.width,					// width (uint32_t)
							desiredVkExtent2D.height,					// height (uint32_t)
							1											// layers (uint32_t)
						};
						if ( vkCreateFramebuffer(vkDevice, &vkFramebufferCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &swapChainBuffer.vkFramebuffer) != VK_SUCCESS )
						{
							RHI_LOG(CRITICAL, "Failed to create Vulkan framebuffer")
						}
					}
				}
			}

			{ // Create the Vulkan semaphores
				static constexpr VkSemaphoreCreateInfo vkSemaphoreCreateInfo =
				{
					VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,	// sType (VkStructureType)
					nullptr,									// pNext (const void*)
					0											// flags (VkSemaphoreCreateFlags)
				};
				if ( (vkCreateSemaphore(vkDevice, &vkSemaphoreCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mImageAvailableVkSemaphore) != VK_SUCCESS) ||
					(vkCreateSemaphore(vkDevice, &vkSemaphoreCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mRenderingFinishedVkSemaphore) != VK_SUCCESS) )
				{
					RHI_LOG(CRITICAL, "Failed to create Vulkan semaphore")
				}
			}

			// Acquire next image
			acquireNextImage(false);
		}

		void destroyVulkanSwapChain()
		{
			// Destroy Vulkan swap chain
			if ( VK_NULL_HANDLE != mVkRenderPass || !mSwapChainBuffer.empty() || VK_NULL_HANDLE != mVkSwapchainKHR )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
				vkDeviceWaitIdle(vkDevice);
				if ( VK_NULL_HANDLE != mVkRenderPass )
				{
					vkDestroyRenderPass(vkDevice, mVkRenderPass, vulkanRhi.getVkAllocationCallbacks());
					mVkRenderPass = VK_NULL_HANDLE;
				}
				if ( !mSwapChainBuffer.empty() )
				{
					for ( const SwapChainBuffer& swapChainBuffer : mSwapChainBuffer )
					{
						vkDestroyFramebuffer(vkDevice, swapChainBuffer.vkFramebuffer, vulkanRhi.getVkAllocationCallbacks());
						vkDestroyImageView(vkDevice, swapChainBuffer.vkImageView, vulkanRhi.getVkAllocationCallbacks());
					}
					mSwapChainBuffer.clear();
				}
				if ( VK_NULL_HANDLE != mVkSwapchainKHR )
				{
					vkDestroySwapchainKHR(vkDevice, mVkSwapchainKHR, vulkanRhi.getVkAllocationCallbacks());
					mVkSwapchainKHR = VK_NULL_HANDLE;
				}
				if ( VK_NULL_HANDLE != mImageAvailableVkSemaphore )
				{
					vkDestroySemaphore(vulkanRhi.getVulkanContext().getVkDevice(), mImageAvailableVkSemaphore, vulkanRhi.getVkAllocationCallbacks());
					mImageAvailableVkSemaphore = VK_NULL_HANDLE;
				}
				if ( VK_NULL_HANDLE != mRenderingFinishedVkSemaphore )
				{
					vkDestroySemaphore(vulkanRhi.getVulkanContext().getVkDevice(), mRenderingFinishedVkSemaphore, vulkanRhi.getVkAllocationCallbacks());
					mRenderingFinishedVkSemaphore = VK_NULL_HANDLE;
				}
			}

			// Destroy depth render target
			destroyDepthRenderTarget();
		}

		void acquireNextImage(bool recreateSwapChainIfNeeded)
		{
			const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			const VkResult vkResult = vkAcquireNextImageKHR(vulkanRhi.getVulkanContext().getVkDevice(), mVkSwapchainKHR, UINT64_MAX, mImageAvailableVkSemaphore, VK_NULL_HANDLE, &mCurrentImageIndex);
			if ( VK_SUCCESS != vkResult && VK_SUBOPTIMAL_KHR != vkResult )
			{
				if ( VK_ERROR_OUT_OF_DATE_KHR == vkResult )
				{
					// Recreate the Vulkan swap chain
					if ( recreateSwapChainIfNeeded )
					{
						createVulkanSwapChain();
					}
				}
				else
				{
					// Error!
					RHI_LOG(CRITICAL, "Failed to acquire next Vulkan image from swap chain")
				}
			}
		}

		void createDepthRenderTarget(const VkExtent2D& vkExtent2D)
		{
			if ( VK_FORMAT_UNDEFINED != mDepthVkFormat )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				Helper::createAndAllocateVkImage(vulkanRhi, 0, VK_IMAGE_TYPE_2D, { vkExtent2D.width, vkExtent2D.height, 1 }, 1, 1, mDepthVkFormat, static_cast<RenderPass&>(getRenderPass()).getVkSampleCountFlagBits(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mDepthVkImage, mDepthVkDeviceMemory);
				Helper::createVkImageView(vulkanRhi, mDepthVkImage, VK_IMAGE_VIEW_TYPE_2D, 1, 1, mDepthVkFormat, VK_IMAGE_ASPECT_DEPTH_BIT, mDepthVkImageView);
				// TODO(co) File "unrimp\source\rhi\private\vulkanrhi\vulkanrhi.cpp" | Line 1036 | Critical: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT" Object: "103612336" Location: "0" Message code: "461375810" Layer prefix: "Validation" Message: " [ VUID-vkCmdPipelineBarrier-pMemoryBarriers-01185 ] Object: 0x62cffb0 (Type = 6) | vkCmdPipelineBarrier(): pImageMemBarriers[0].dstAccessMask (0x600) is not supported by dstStageMask (0x1). The spec valid usage text states 'Each element of pMemoryBarriers, pBufferMemoryBarriers and pImageMemoryBarriers must not have any access flag included in its dstAccessMask member if that bit is not supported by any of the pipeline stages in dstStageMask, as specified in the table of supported access types.' (https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#VUID-vkCmdPipelineBarrier-pMemoryBarriers-01185)" 
				//Helper::transitionVkImageLayout(vulkanRhi, mDepthVkImage, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
			}
		}

		void destroyDepthRenderTarget()
		{
			if ( VK_NULL_HANDLE != mDepthVkImage )
			{
				RHI_ASSERT(VK_NULL_HANDLE != mDepthVkDeviceMemory, "Invalid Vulkan depth device memory")
					RHI_ASSERT(VK_NULL_HANDLE != mDepthVkImageView, "Invalid Vulkan depth image view")
					Helper::destroyAndFreeVkImage(static_cast<VulkanRhi&>(getRhi()), mDepthVkImage, mDepthVkDeviceMemory, mDepthVkImageView);
			}
		}


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
	private:
		struct SwapChainBuffer
		{
			VkImage		  vkImage = VK_NULL_HANDLE;	// Vulkan image, don't destroy since we don't own it
			VkImageView   vkImageView = VK_NULL_HANDLE;	// Vulkan image view, destroy if no longer needed
			VkFramebuffer vkFramebuffer = VK_NULL_HANDLE;	// Vulkan framebuffer, destroy if no longer needed
		};
		typedef std::vector<SwapChainBuffer> SwapChainBuffers;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		// Operation system window
		handle			mNativeWindowHandle;	// Native window handle window, can be a null handle
		Rhi::IRenderWindow* mRenderWindow;			// Render window instance, can be a null pointer, don't destroy the instance since we don't own it
		// Vulkan presentation surface
		VkSurfaceKHR mVkSurfaceKHR;	// Vulkan presentation surface, destroy if no longer needed
		// Vulkan swap chain and color render target related
		VkSwapchainKHR	 mVkSwapchainKHR;				// Vulkan swap chain, destroy if no longer needed
		VkRenderPass	 mVkRenderPass;					// Vulkan render pass, destroy if no longer needed (due to "VK_IMAGE_LAYOUT_PRESENT_SRC_KHR" we need an own Vulkan render pass instance)
		SwapChainBuffers mSwapChainBuffer;				// Swap chain buffer for managing the color render targets
		VkSemaphore		 mImageAvailableVkSemaphore;	// Vulkan semaphore, destroy if no longer needed
		VkSemaphore		 mRenderingFinishedVkSemaphore;	// Vulkan semaphore, destroy if no longer needed
		uint32_t		 mCurrentImageIndex;			// The index of the current Vulkan swap chain image to render into, ~0 if invalid
		// Depth render target related
		VkFormat		mDepthVkFormat;	// Can be "VK_FORMAT_UNDEFINED" if no depth stencil buffer is needed
		VkImage			mDepthVkImage;
		VkDeviceMemory  mDepthVkDeviceMemory;
		VkImageView		mDepthVkImageView;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/RenderTarget/Framebuffer.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan framebuffer interface
	*/
	class Framebuffer final : public Rhi::IFramebuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] colorFramebufferAttachments
		*    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "Rhi::IRenderPass::getNumberOfColorAttachments()" textures in the provided C-array of pointers
		*  @param[in] depthStencilFramebufferAttachment
		*    The depth stencil render target texture, can be a null pointer
		*
		*  @note
		*    - The framebuffer keeps a reference to the provided texture instances
		*/
		Framebuffer(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfColorTextures(static_cast<RenderPass&>(renderPass).getNumberOfColorAttachments()),
			mColorTextures(nullptr),	// Set below
			mDepthStencilTexture(nullptr),
			mWidth(UINT_MAX),
			mHeight(UINT_MAX),
			mVkRenderPass(static_cast<RenderPass&>(renderPass).getVkRenderPass()),
			mVkFramebuffer(VK_NULL_HANDLE)
		{
			const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(renderPass.getRhi());

			// Vulkan attachment descriptions and views to fill
			std::vector<VkImageView> vkImageViews;
			vkImageViews.resize(mNumberOfColorTextures + ((nullptr != depthStencilFramebufferAttachment) ? 1u : 0u));
			uint32_t currentVkAttachmentDescriptionIndex = 0;

			// Add a reference to the used color textures
			if ( mNumberOfColorTextures > 0 )
			{
				mColorTextures = RHI_MALLOC_TYPED(vulkanRhi.getContext(), Rhi::ITexture*, mNumberOfColorTextures);

				// Loop through all color textures
				Rhi::ITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
				for ( Rhi::ITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture, ++colorFramebufferAttachments )
				{
					// Sanity check
					RHI_ASSERT(nullptr != colorFramebufferAttachments->texture, "Invalid Vulkan color framebuffer attachment texture")

						// TODO(co) Add security check: Is the given resource one of the currently used RHI?
						*colorTexture = colorFramebufferAttachments->texture;
					(*colorTexture)->addReference();

					// Evaluate the color texture type
					VkImageView vkImageView = VK_NULL_HANDLE;
					switch ( (*colorTexture)->getResourceType() )
					{
					case Rhi::ResourceType::TEXTURE_2D:
						{
							const Texture2D* texture2D = static_cast<Texture2D*>(*colorTexture);

							// Sanity checks
							RHI_ASSERT(colorFramebufferAttachments->mipmapIndex < Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid Vulkan color framebuffer attachment mipmap index")
								RHI_ASSERT(0 == colorFramebufferAttachments->layerIndex, "Invalid Vulkan color framebuffer attachment layer index")

								// Update the framebuffer width and height if required
								vkImageView = texture2D->getVkImageView();
							::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2D->getWidth(), texture2D->getHeight(), mWidth, mHeight);
							break;
						}

					case Rhi::ResourceType::TEXTURE_2D_ARRAY:
						{
							// Update the framebuffer width and height if required
							const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(*colorTexture);
							vkImageView = texture2DArray->getVkImageView();
							::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2DArray->getWidth(), texture2DArray->getHeight(), mWidth, mHeight);
							break;
						}

					case Rhi::ResourceType::ROOT_SIGNATURE:
					case Rhi::ResourceType::RESOURCE_GROUP:
					case Rhi::ResourceType::GRAPHICS_PROGRAM:
					case Rhi::ResourceType::VERTEX_ARRAY:
					case Rhi::ResourceType::RENDER_PASS:
					case Rhi::ResourceType::QUERY_POOL:
					case Rhi::ResourceType::SWAP_CHAIN:
					case Rhi::ResourceType::FRAMEBUFFER:
					case Rhi::ResourceType::VERTEX_BUFFER:
					case Rhi::ResourceType::INDEX_BUFFER:
					case Rhi::ResourceType::TEXTURE_BUFFER:
					case Rhi::ResourceType::STRUCTURED_BUFFER:
					case Rhi::ResourceType::INDIRECT_BUFFER:
					case Rhi::ResourceType::UNIFORM_BUFFER:
					case Rhi::ResourceType::TEXTURE_1D:
					case Rhi::ResourceType::TEXTURE_1D_ARRAY:
					case Rhi::ResourceType::TEXTURE_3D:
					case Rhi::ResourceType::TEXTURE_CUBE:
					case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
					case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
					case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
					case Rhi::ResourceType::SAMPLER_STATE:
					case Rhi::ResourceType::VERTEX_SHADER:
					case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
					case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
					case Rhi::ResourceType::GEOMETRY_SHADER:
					case Rhi::ResourceType::FRAGMENT_SHADER:
					case Rhi::ResourceType::TASK_SHADER:
					case Rhi::ResourceType::MESH_SHADER:
					case Rhi::ResourceType::COMPUTE_SHADER:
					default:
						// Nothing here
						break;
					}

					// Remember the Vulkan image view
					vkImageViews[currentVkAttachmentDescriptionIndex] = vkImageView;

					// Advance current Vulkan attachment description index
					++currentVkAttachmentDescriptionIndex;
				}
			}

			// Add a reference to the used depth stencil texture
			if ( nullptr != depthStencilFramebufferAttachment )
			{
				mDepthStencilTexture = depthStencilFramebufferAttachment->texture;
				RHI_ASSERT(nullptr != mDepthStencilTexture, "Invalid Vulkan depth stencil framebuffer attachment texture")
					mDepthStencilTexture->addReference();

				// Evaluate the depth stencil texture type
				VkImageView vkImageView = VK_NULL_HANDLE;
				switch ( mDepthStencilTexture->getResourceType() )
				{
				case Rhi::ResourceType::TEXTURE_2D:
					{
						const Texture2D* texture2D = static_cast<Texture2D*>(mDepthStencilTexture);

						// Sanity checks
						RHI_ASSERT(depthStencilFramebufferAttachment->mipmapIndex < Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid Vulkan depth stencil framebuffer attachment mipmap index")
							RHI_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex, "Invalid Vulkan depth stencil framebuffer attachment layer index")

							// Update the framebuffer width and height if required
							vkImageView = texture2D->getVkImageView();
						::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2D->getWidth(), texture2D->getHeight(), mWidth, mHeight);
						break;
					}

				case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					{
						// Update the framebuffer width and height if required
						const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(mDepthStencilTexture);
						vkImageView = texture2DArray->getVkImageView();
						::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2DArray->getWidth(), texture2DArray->getHeight(), mWidth, mHeight);
						break;
					}

				case Rhi::ResourceType::ROOT_SIGNATURE:
				case Rhi::ResourceType::RESOURCE_GROUP:
				case Rhi::ResourceType::GRAPHICS_PROGRAM:
				case Rhi::ResourceType::VERTEX_ARRAY:
				case Rhi::ResourceType::RENDER_PASS:
				case Rhi::ResourceType::QUERY_POOL:
				case Rhi::ResourceType::SWAP_CHAIN:
				case Rhi::ResourceType::FRAMEBUFFER:
				case Rhi::ResourceType::VERTEX_BUFFER:
				case Rhi::ResourceType::INDEX_BUFFER:
				case Rhi::ResourceType::TEXTURE_BUFFER:
				case Rhi::ResourceType::STRUCTURED_BUFFER:
				case Rhi::ResourceType::INDIRECT_BUFFER:
				case Rhi::ResourceType::UNIFORM_BUFFER:
				case Rhi::ResourceType::TEXTURE_1D:
				case Rhi::ResourceType::TEXTURE_1D_ARRAY:
				case Rhi::ResourceType::TEXTURE_3D:
				case Rhi::ResourceType::TEXTURE_CUBE:
				case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
				case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
				case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
				case Rhi::ResourceType::SAMPLER_STATE:
				case Rhi::ResourceType::VERTEX_SHADER:
				case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
				case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
				case Rhi::ResourceType::GEOMETRY_SHADER:
				case Rhi::ResourceType::FRAGMENT_SHADER:
				case Rhi::ResourceType::TASK_SHADER:
				case Rhi::ResourceType::MESH_SHADER:
				case Rhi::ResourceType::COMPUTE_SHADER:
				default:
					// Nothing here
					break;
				}

				// Remember the Vulkan image view
				vkImageViews[currentVkAttachmentDescriptionIndex] = vkImageView;
			}

			// Validate the framebuffer width and height
			if ( 0 == mWidth || UINT_MAX == mWidth )
			{
				RHI_ASSERT(false, "Invalid Vulkan framebuffer width")
					mWidth = 1;
			}
			if ( 0 == mHeight || UINT_MAX == mHeight )
			{
				RHI_ASSERT(false, "Invalid Vulkan framebuffer height")
					mHeight = 1;
			}

			// Create Vulkan framebuffer
			const VkFramebufferCreateInfo vkFramebufferCreateInfo =
			{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,	// sType (VkStructureType)
				nullptr,									// pNext (const void*)
				0,											// flags (VkFramebufferCreateFlags)
				mVkRenderPass,								// renderPass (VkRenderPass)
				static_cast<uint32_t>(vkImageViews.size()),	// attachmentCount (uint32_t)
				vkImageViews.data(),						// pAttachments (const VkImageView*)
				mWidth,										// width (uint32_t)
				mHeight,									// height (uint32_t
				1											// layers (uint32_t)
			};
			if ( vkCreateFramebuffer(vulkanRhi.getVulkanContext().getVkDevice(), &vkFramebufferCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkFramebuffer) == VK_SUCCESS )
			{
				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				if ( nullptr != vkDebugMarkerSetObjectNameEXT )
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
						const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
					Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, (uint64_t)mVkRenderPass, detailedDebugName);
					Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT, (uint64_t)mVkFramebuffer, detailedDebugName);
				}
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Vulkan framebuffer")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Framebuffer() override
		{
			const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();

			// Destroy Vulkan framebuffer instance
			if ( VK_NULL_HANDLE != mVkFramebuffer )
			{
				vkDestroyFramebuffer(vkDevice, mVkFramebuffer, vulkanRhi.getVkAllocationCallbacks());
			}

			// Release the reference to the used color textures
			if ( nullptr != mColorTextures )
			{
				// Release references
				Rhi::ITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
				for ( Rhi::ITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture )
				{
					(*colorTexture)->releaseReference();
				}

				// Cleanup
				RHI_FREE(vulkanRhi.getContext(), mColorTextures);
			}

			// Release the reference to the used depth stencil texture
			if ( nullptr != mDepthStencilTexture )
			{
				// Release reference
				mDepthStencilTexture->releaseReference();
			}
		}

		/**
		*  @brief
		*    Return the Vulkan render pass
		*
		*  @return
		*    The Vulkan render pass
		*/
		[[nodiscard]] inline VkRenderPass getVkRenderPass() const
		{
			return mVkRenderPass;
		}

		/**
		*  @brief
		*    Return the Vulkan framebuffer to render into
		*
		*  @return
		*    The Vulkan framebuffer to render into
		*/
		[[nodiscard]] inline VkFramebuffer getVkFramebuffer() const
		{
			return mVkFramebuffer;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRenderTarget methods             ]
		//[-------------------------------------------------------]
	public:
		inline virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const override
		{
			// No fancy implementation in here, just copy over the internal information
			width = mWidth;
			height = mHeight;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), Framebuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator =(const Framebuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		uint32_t		mNumberOfColorTextures;	// Number of color render target textures
		Rhi::ITexture** mColorTextures;			// The color render target textures (we keep a reference to it), can be a null pointer or can contain null pointers, if not a null pointer there must be at least "mNumberOfColorTextures" textures in the provided C-array of pointers
		Rhi::ITexture*  mDepthStencilTexture;	// The depth stencil render target texture (we keep a reference to it), can be a null pointer
		uint32_t		mWidth;					// The framebuffer width
		uint32_t		mHeight;				// The framebuffer height
		VkRenderPass	mVkRenderPass;			// Vulkan render pass instance, can be a null handle, we don't own it
		VkFramebuffer	mVkFramebuffer;			// Vulkan framebuffer instance, can be a null handle


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/VertexShaderGlsl.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL vertex shader class
	*/
	class VertexShaderGlsl final : public Rhi::IVertexShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a vertex shader from shader bytecode
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		VertexShaderGlsl(VulkanRhi& vulkanRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IVertexShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VS", 5)	// 5 = "VS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a vertex shader from shader source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		VertexShaderGlsl(VulkanRhi& vulkanRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IVertexShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_VERTEX_BIT, sourceCode, shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VS", 5)	// 5 = "VS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VertexShaderGlsl() override
		{
			if ( VK_NULL_HANDLE != mVkShaderModule )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan shader module
		*
		*  @return
		*    The Vulkan shader module
		*/
		[[nodiscard]] inline VkShaderModule getVkShaderModule() const
		{
			return mVkShaderModule;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), VertexShaderGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexShaderGlsl(const VertexShaderGlsl&) = delete;
		VertexShaderGlsl& operator =(const VertexShaderGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkShaderModule mVkShaderModule;	// Vulkan shader module, destroy it if you no longer need it


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/TessellationControlShaderGlsl.h      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL tessellation control shader ("hull shader" in Direct3D terminology) class
	*/
	class TessellationControlShaderGlsl final : public Rhi::ITessellationControlShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a tessellation control shader ("hull shader" in Direct3D terminology) shader from shader bytecode
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		TessellationControlShaderGlsl(VulkanRhi& vulkanRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationControlShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TCS", 6)	// 6 = "TCS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a tessellation control shader ("hull shader" in Direct3D terminology) shader from shader source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		TessellationControlShaderGlsl(VulkanRhi& vulkanRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationControlShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, sourceCode, shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TCS", 6)	// 6 = "TCS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TessellationControlShaderGlsl() override
		{
			if ( VK_NULL_HANDLE != mVkShaderModule )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan shader module
		*
		*  @return
		*    The Vulkan shader module
		*/
		[[nodiscard]] inline VkShaderModule getVkShaderModule() const
		{
			return mVkShaderModule;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), TessellationControlShaderGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TessellationControlShaderGlsl(const TessellationControlShaderGlsl&) = delete;
		TessellationControlShaderGlsl& operator =(const TessellationControlShaderGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkShaderModule mVkShaderModule;	// Vulkan shader module, destroy it if you no longer need it


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/TessellationEvaluationShaderGlsl.h   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL tessellation evaluation shader ("domain shader" in Direct3D terminology) class
	*/
	class TessellationEvaluationShaderGlsl final : public Rhi::ITessellationEvaluationShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a tessellation evaluation shader ("domain shader" in Direct3D terminology) shader from shader bytecode
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		TessellationEvaluationShaderGlsl(VulkanRhi& vulkanRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationEvaluationShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TES", 6)	// 6 = "TES: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a tessellation evaluation shader ("domain shader" in Direct3D terminology) shader from shader source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		TessellationEvaluationShaderGlsl(VulkanRhi& vulkanRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationEvaluationShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, sourceCode, shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TES", 6)	// 6 = "TES: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TessellationEvaluationShaderGlsl() override
		{
			if ( VK_NULL_HANDLE != mVkShaderModule )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan shader module
		*
		*  @return
		*    The Vulkan shader module
		*/
		[[nodiscard]] inline VkShaderModule getVkShaderModule() const
		{
			return mVkShaderModule;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), TessellationEvaluationShaderGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TessellationEvaluationShaderGlsl(const TessellationEvaluationShaderGlsl&) = delete;
		TessellationEvaluationShaderGlsl& operator =(const TessellationEvaluationShaderGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkShaderModule mVkShaderModule;	// Vulkan shader module, destroy it if you no longer need it


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/GeometryShaderGlsl.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL geometry shader class
	*/
	class GeometryShaderGlsl final : public Rhi::IGeometryShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a geometry shader from shader bytecode
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*  @param[in] gsInputPrimitiveTopology
		*    Geometry shader input primitive topology
		*  @param[in] gsOutputPrimitiveTopology
		*    Geometry shader output primitive topology
		*  @param[in] numberOfOutputVertices
		*    Number of output vertices
		*/
		// TODO(co) Remove unused parameters
		GeometryShaderGlsl(VulkanRhi& vulkanRhi, const Rhi::ShaderBytecode& shaderBytecode, [[maybe_unused]] Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, [[maybe_unused]] Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, [[maybe_unused]] uint32_t numberOfOutputVertices RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGeometryShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a geometry shader from shader source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*  @param[in] gsInputPrimitiveTopology
		*    Geometry shader input primitive topology
		*  @param[in] gsOutputPrimitiveTopology
		*    Geometry shader output primitive topology
		*  @param[in] numberOfOutputVertices
		*    Number of output vertices
		*/
		// TODO(co) Remove unused parameters
		GeometryShaderGlsl(VulkanRhi& vulkanRhi, const char* sourceCode, [[maybe_unused]] Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, [[maybe_unused]] Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, [[maybe_unused]] uint32_t numberOfOutputVertices, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGeometryShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_GEOMETRY_BIT, sourceCode, shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GeometryShaderGlsl() override
		{
			if ( VK_NULL_HANDLE != mVkShaderModule )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan shader module
		*
		*  @return
		*    The Vulkan shader module
		*/
		[[nodiscard]] inline VkShaderModule getVkShaderModule() const
		{
			return mVkShaderModule;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), GeometryShaderGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GeometryShaderGlsl(const GeometryShaderGlsl&) = delete;
		GeometryShaderGlsl& operator =(const GeometryShaderGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkShaderModule mVkShaderModule;	// Vulkan shader module, destroy it if you no longer need it


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/FragmentShaderGlsl.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL fragment shader (FS, "pixel shader" in Direct3D terminology) class
	*/
	class FragmentShaderGlsl final : public Rhi::IFragmentShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a fragment shader from shader bytecode
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		FragmentShaderGlsl(VulkanRhi& vulkanRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IFragmentShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FS", 5)	// 5 = "FS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a fragment shader from shader source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		FragmentShaderGlsl(VulkanRhi& vulkanRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IFragmentShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_FRAGMENT_BIT, sourceCode, shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FS", 5)	// 5 = "FS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FragmentShaderGlsl() override
		{
			if ( VK_NULL_HANDLE != mVkShaderModule )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan shader module
		*
		*  @return
		*    The Vulkan shader module
		*/
		[[nodiscard]] inline VkShaderModule getVkShaderModule() const
		{
			return mVkShaderModule;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), FragmentShaderGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit FragmentShaderGlsl(const FragmentShaderGlsl&) = delete;
		FragmentShaderGlsl& operator =(const FragmentShaderGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkShaderModule mVkShaderModule;	// Vulkan shader module, destroy it if you no longer need it


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/TaskShaderGlsl.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL task shader (TS, "amplification shader" in Direct3D terminology) class
	*/
	class TaskShaderGlsl final : public Rhi::ITaskShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a task shader from shader bytecode
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		TaskShaderGlsl(VulkanRhi& vulkanRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITaskShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TS", 5)	// 5 = "TS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a task shader from shader source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		TaskShaderGlsl(VulkanRhi& vulkanRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITaskShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_TASK_BIT_NV, sourceCode, shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TS", 5)	// 5 = "TS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TaskShaderGlsl() override
		{
			if ( VK_NULL_HANDLE != mVkShaderModule )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan shader module
		*
		*  @return
		*    The Vulkan shader module
		*/
		[[nodiscard]] inline VkShaderModule getVkShaderModule() const
		{
			return mVkShaderModule;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), TaskShaderGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TaskShaderGlsl(const TaskShaderGlsl&) = delete;
		TaskShaderGlsl& operator =(const TaskShaderGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkShaderModule mVkShaderModule;	// Vulkan shader module, destroy it if you no longer need it


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/MeshShaderGlsl.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL mesh shader (MS) class
	*/
	class MeshShaderGlsl final : public Rhi::IMeshShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a mesh shader from shader bytecode
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		MeshShaderGlsl(VulkanRhi& vulkanRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IMeshShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "MS", 5)	// 5 = "MS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a mesh shader from shader source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		MeshShaderGlsl(VulkanRhi& vulkanRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IMeshShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_MESH_BIT_NV, sourceCode, shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "MS", 5)	// 5 = "MS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MeshShaderGlsl() override
		{
			if ( VK_NULL_HANDLE != mVkShaderModule )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan shader module
		*
		*  @return
		*    The Vulkan shader module
		*/
		[[nodiscard]] inline VkShaderModule getVkShaderModule() const
		{
			return mVkShaderModule;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), MeshShaderGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit MeshShaderGlsl(const MeshShaderGlsl&) = delete;
		MeshShaderGlsl& operator =(const MeshShaderGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkShaderModule mVkShaderModule;	// Vulkan shader module, destroy it if you no longer need it


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/ComputeShaderGlsl.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL compute shader (CS) class
	*/
	class ComputeShaderGlsl final : public Rhi::IComputeShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a compute shader from shader bytecode
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		ComputeShaderGlsl(VulkanRhi& vulkanRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IComputeShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "CS", 5)	// 5 = "CS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a compute shader from shader source code
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		ComputeShaderGlsl(VulkanRhi& vulkanRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IComputeShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_COMPUTE_BIT, sourceCode, shaderBytecode))
		{
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "CS", 5)	// 5 = "CS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (uint64_t)mVkShaderModule, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComputeShaderGlsl() override
		{
			if ( VK_NULL_HANDLE != mVkShaderModule )
			{
				const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
				vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
			}
		}

		/**
		*  @brief
		*    Return the Vulkan shader module
		*
		*  @return
		*    The Vulkan shader module
		*/
		[[nodiscard]] inline VkShaderModule getVkShaderModule() const
		{
			return mVkShaderModule;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), ComputeShaderGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ComputeShaderGlsl(const ComputeShaderGlsl&) = delete;
		ComputeShaderGlsl& operator =(const ComputeShaderGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		VkShaderModule mVkShaderModule;	// Vulkan shader module, destroy it if you no longer need it


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/GraphicsProgramGlsl.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL graphics program class
	*/
	class GraphicsProgramGlsl final : public Rhi::IGraphicsProgram
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for traditional graphics program
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] vertexShaderGlsl
		*    Vertex shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationControlShaderGlsl
		*    Tessellation control shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationEvaluationShaderGlsl
		*    Tessellation evaluation shader the graphics program is using, can be a null pointer
		*  @param[in] geometryShaderGlsl
		*    Geometry shader the graphics program is using, can be a null pointer
		*  @param[in] fragmentShaderGlsl
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		GraphicsProgramGlsl(VulkanRhi& vulkanRhi, VertexShaderGlsl* vertexShaderGlsl, TessellationControlShaderGlsl* tessellationControlShaderGlsl, TessellationEvaluationShaderGlsl* tessellationEvaluationShaderGlsl, GeometryShaderGlsl* geometryShaderGlsl, FragmentShaderGlsl* fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IGraphicsProgram(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			// Traditional graphics program
			mVertexShaderGlsl(vertexShaderGlsl),
			mTessellationControlShaderGlsl(tessellationControlShaderGlsl),
			mTessellationEvaluationShaderGlsl(tessellationEvaluationShaderGlsl),
			mGeometryShaderGlsl(geometryShaderGlsl),
			// Both graphics programs
			mFragmentShaderGlsl(fragmentShaderGlsl),
			// Task and mesh shader based graphics program
			mTaskShaderGlsl(nullptr),
			mMeshShaderGlsl(nullptr)
		{
			// Add references to the provided shaders
			if ( nullptr != mVertexShaderGlsl )
			{
				mVertexShaderGlsl->addReference();
			}
			if ( nullptr != mTessellationControlShaderGlsl )
			{
				mTessellationControlShaderGlsl->addReference();
			}
			if ( nullptr != mTessellationEvaluationShaderGlsl )
			{
				mTessellationEvaluationShaderGlsl->addReference();
			}
			if ( nullptr != mGeometryShaderGlsl )
			{
				mGeometryShaderGlsl->addReference();
			}
			if ( nullptr != mFragmentShaderGlsl )
			{
				mFragmentShaderGlsl->addReference();
			}
		}

		/**
		*  @brief
		*    Constructor for task and mesh shader based graphics program
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] taskShaderGlsl
		*    Task shader the graphics program is using, can be a null pointer
		*  @param[in] meshShaderGlsl
		*    Mesh shader the graphics program is using
		*  @param[in] fragmentShaderGlsl
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		GraphicsProgramGlsl(VulkanRhi& vulkanRhi, TaskShaderGlsl* taskShaderGlsl, MeshShaderGlsl& meshShaderGlsl, FragmentShaderGlsl* fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IGraphicsProgram(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			// Traditional graphics program
			mVertexShaderGlsl(nullptr),
			mTessellationControlShaderGlsl(nullptr),
			mTessellationEvaluationShaderGlsl(nullptr),
			mGeometryShaderGlsl(nullptr),
			// Both graphics programs
			mFragmentShaderGlsl(fragmentShaderGlsl),
			// Task and mesh shader based graphics program
			mTaskShaderGlsl(taskShaderGlsl),
			mMeshShaderGlsl(&meshShaderGlsl)
		{
			// Add references to the provided shaders
			if ( nullptr != mTaskShaderGlsl )
			{
				mTaskShaderGlsl->addReference();
			}
			mMeshShaderGlsl->addReference();
			if ( nullptr != mFragmentShaderGlsl )
			{
				mFragmentShaderGlsl->addReference();
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GraphicsProgramGlsl() override
		{
			// Release the shader references
			if ( nullptr != mVertexShaderGlsl )
			{
				mVertexShaderGlsl->releaseReference();
			}
			if ( nullptr != mTessellationControlShaderGlsl )
			{
				mTessellationControlShaderGlsl->releaseReference();
			}
			if ( nullptr != mTessellationEvaluationShaderGlsl )
			{
				mTessellationEvaluationShaderGlsl->releaseReference();
			}
			if ( nullptr != mGeometryShaderGlsl )
			{
				mGeometryShaderGlsl->releaseReference();
			}
			if ( nullptr != mFragmentShaderGlsl )
			{
				mFragmentShaderGlsl->releaseReference();
			}
			if ( nullptr != mTaskShaderGlsl )
			{
				mTaskShaderGlsl->releaseReference();
			}
			if ( nullptr != mMeshShaderGlsl )
			{
				mMeshShaderGlsl->releaseReference();
			}
		}

		//[-------------------------------------------------------]
		//[ Traditional graphics program                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the GLSL vertex shader the graphics program is using
		*
		*  @return
		*    The GLSL vertex shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline VertexShaderGlsl* getVertexShaderGlsl() const
		{
			return mVertexShaderGlsl;
		}

		/**
		*  @brief
		*    Return the GLSL tessellation control shader the graphics program is using
		*
		*  @return
		*    The GLSL tessellation control shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline TessellationControlShaderGlsl* getTessellationControlShaderGlsl() const
		{
			return mTessellationControlShaderGlsl;
		}

		/**
		*  @brief
		*    Return the GLSL tessellation evaluation shader the graphics program is using
		*
		*  @return
		*    The GLSL tessellation evaluation shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline TessellationEvaluationShaderGlsl* getTessellationEvaluationShaderGlsl() const
		{
			return mTessellationEvaluationShaderGlsl;
		}

		/**
		*  @brief
		*    Return the GLSL geometry shader the graphics program is using
		*
		*  @return
		*    The GLSL geometry shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline GeometryShaderGlsl* getGeometryShaderGlsl() const
		{
			return mGeometryShaderGlsl;
		}

		//[-------------------------------------------------------]
		//[ Both graphics programs                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the GLSL fragment shader the graphics program is using
		*
		*  @return
		*    The GLSL fragment shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline FragmentShaderGlsl* getFragmentShaderGlsl() const
		{
			return mFragmentShaderGlsl;
		}

		//[-------------------------------------------------------]
		//[ Task and mesh shader based graphics program           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the GLSL task shader the graphics program is using
		*
		*  @return
		*    The GLSL task shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline TaskShaderGlsl* getTaskShaderGlsl() const
		{
			return mTaskShaderGlsl;
		}

		/**
		*  @brief
		*    Return the GLSL mesh shader the graphics program is using
		*
		*  @return
		*    The GLSL mesh shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline MeshShaderGlsl* getMeshShaderGlsl() const
		{
			return mMeshShaderGlsl;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), GraphicsProgramGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GraphicsProgramGlsl(const GraphicsProgramGlsl&) = delete;
		GraphicsProgramGlsl& operator =(const GraphicsProgramGlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		// Traditional graphics program
		VertexShaderGlsl*				  mVertexShaderGlsl;					// Vertex shader the graphics program is using (we keep a reference to it), can be a null pointer
		TessellationControlShaderGlsl*	  mTessellationControlShaderGlsl;		// Tessellation control shader the graphics program is using (we keep a reference to it), can be a null pointer
		TessellationEvaluationShaderGlsl* mTessellationEvaluationShaderGlsl;	// Tessellation evaluation shader the graphics program is using (we keep a reference to it), can be a null pointer
		GeometryShaderGlsl*				  mGeometryShaderGlsl;					// Geometry shader the graphics program is using (we keep a reference to it), can be a null pointer
		// Both graphics programs
		FragmentShaderGlsl* mFragmentShaderGlsl;	// Fragment shader the graphics program is using (we keep a reference to it), can be a null pointer
		// Task and mesh shader based graphics program
		TaskShaderGlsl* mTaskShaderGlsl;	// Task shader the graphics program is using (we keep a reference to it), can be a null pointer
		MeshShaderGlsl* mMeshShaderGlsl;	// Mesh shader the graphics program is using (we keep a reference to it), can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/Shader/ShaderLanguageGlsl.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    GLSL shader language class
	*/
	class ShaderLanguageGlsl final : public Rhi::IShaderLanguage
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*/
		inline explicit ShaderLanguageGlsl(VulkanRhi& vulkanRhi) :
			IShaderLanguage(vulkanRhi)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ShaderLanguageGlsl() override
		{
			// De-initialize glslang, if necessary
#ifdef SE_GLSLTOSPIRV
			if ( ::detail::GlslangInitialized )
			{
				// TODO(co) Fix glslang related memory leaks. See also
				//		    - "Fix a few memory leaks #916" - https://github.com/KhronosGroup/glslang/pull/916
				//		    - "FreeGlobalPools is never called in glslang::FinalizeProcess()'s path. #928" - https://github.com/KhronosGroup/glslang/issues/928
				glslang::FinalizeProcess();
				::detail::GlslangInitialized = false;
			}
#endif
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShaderLanguage methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}

		[[nodiscard]] inline virtual Rhi::IVertexShader* createVertexShaderFromBytecode([[maybe_unused]] const Rhi::VertexAttributes& vertexAttributes, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan vertex shader bytecode is invalid")

				// Create shader instance
				return RHI_NEW(vulkanRhi.getContext(), VertexShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IVertexShader* createVertexShaderFromSourceCode([[maybe_unused]] const Rhi::VertexAttributes& vertexAttributes, const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), VertexShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITessellationControlShader* createTessellationControlShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan tessellation control shader bytecode is invalid")

				// Create shader instance
				return RHI_NEW(vulkanRhi.getContext(), TessellationControlShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITessellationControlShader* createTessellationControlShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), TessellationControlShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITessellationEvaluationShader* createTessellationEvaluationShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan tessellation evaluation shader bytecode is invalid")

				// Create shader instance
				return RHI_NEW(vulkanRhi.getContext(), TessellationEvaluationShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITessellationEvaluationShader* createTessellationEvaluationShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), TessellationEvaluationShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IGeometryShader* createGeometryShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode, Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan geometry shader bytecode is invalid")

				// Create shader instance
				return RHI_NEW(vulkanRhi.getContext(), GeometryShaderGlsl)(vulkanRhi, shaderBytecode, gsInputPrimitiveTopology, gsOutputPrimitiveTopology, numberOfOutputVertices RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IGeometryShader* createGeometryShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), GeometryShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, gsInputPrimitiveTopology, gsOutputPrimitiveTopology, numberOfOutputVertices, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IFragmentShader* createFragmentShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan fragment shader bytecode is invalid")

				// Create shader instance
				return RHI_NEW(vulkanRhi.getContext(), FragmentShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IFragmentShader* createFragmentShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), FragmentShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITaskShader* createTaskShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan task shader bytecode is invalid")

				// Create shader instance
				return RHI_NEW(vulkanRhi.getContext(), TaskShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITaskShader* createTaskShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), TaskShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IMeshShader* createMeshShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan mesh shader bytecode is invalid")

				// Create shader instance
				return RHI_NEW(vulkanRhi.getContext(), MeshShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IMeshShader* createMeshShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), MeshShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IComputeShader* createComputeShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity check
			RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan compute shader bytecode is invalid")

				// Create shader instance
				return RHI_NEW(vulkanRhi.getContext(), ComputeShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IComputeShader* createComputeShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			return RHI_NEW(vulkanRhi.getContext(), ComputeShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::IGraphicsProgram* createGraphicsProgram([[maybe_unused]] const Rhi::IRootSignature& rootSignature, [[maybe_unused]] const Rhi::VertexAttributes& vertexAttributes, Rhi::IVertexShader* vertexShader, Rhi::ITessellationControlShader* tessellationControlShader, Rhi::ITessellationEvaluationShader* tessellationEvaluationShader, Rhi::IGeometryShader* geometryShader, Rhi::IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity checks
			// -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
			// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
			//    the name is safe because we know that we always reference to one and the same name address
			// TODO(co) Add security check: Is the given resource one of the currently used RHI?
			RHI_ASSERT(nullptr == vertexShader || vertexShader->getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan vertex shader language mismatch")
				RHI_ASSERT(nullptr == tessellationControlShader || tessellationControlShader->getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan tessellation control shader language mismatch")
				RHI_ASSERT(nullptr == tessellationEvaluationShader || tessellationEvaluationShader->getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan tessellation evaluation shader language mismatch")
				RHI_ASSERT(nullptr == geometryShader || geometryShader->getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan geometry shader language mismatch")
				RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan fragment shader language mismatch")

				// Create the graphics program
				return RHI_NEW(vulkanRhi.getContext(), GraphicsProgramGlsl)(vulkanRhi, static_cast<VertexShaderGlsl*>(vertexShader), static_cast<TessellationControlShaderGlsl*>(tessellationControlShader), static_cast<TessellationEvaluationShaderGlsl*>(tessellationEvaluationShader), static_cast<GeometryShaderGlsl*>(geometryShader), static_cast<FragmentShaderGlsl*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::IGraphicsProgram* createGraphicsProgram([[maybe_unused]] const Rhi::IRootSignature& rootSignature, Rhi::ITaskShader* taskShader, Rhi::IMeshShader& meshShader, Rhi::IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
		{
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());

			// Sanity checks
			// -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
			// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
			//    the name is safe because we know that we always reference to one and the same name address
			// TODO(co) Add security check: Is the given resource one of the currently used RHI?
			RHI_ASSERT(nullptr == taskShader || taskShader->getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan task shader language mismatch")
				RHI_ASSERT(meshShader.getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan mesh shader language mismatch")
				RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan fragment shader language mismatch")

				// Create the graphics program
				return RHI_NEW(vulkanRhi.getContext(), GraphicsProgramGlsl)(vulkanRhi, static_cast<TaskShaderGlsl*>(taskShader), static_cast<MeshShaderGlsl&>(meshShader), static_cast<FragmentShaderGlsl*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), ShaderLanguageGlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ShaderLanguageGlsl(const ShaderLanguageGlsl&) = delete;
		ShaderLanguageGlsl& operator =(const ShaderLanguageGlsl&) = delete;


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/State/GraphicsPipelineState.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan graphics pipeline state class
	*/
	class GraphicsPipelineState final : public Rhi::IGraphicsPipelineState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] graphicsPipelineState
		*    Graphics pipeline state to use
		*  @param[in] id
		*    The unique compact graphics pipeline state ID
		*/
		GraphicsPipelineState(VulkanRhi& vulkanRhi, const Rhi::GraphicsPipelineState& graphicsPipelineState, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGraphicsPipelineState(vulkanRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRootSignature(graphicsPipelineState.rootSignature),
			mGraphicsProgram(graphicsPipelineState.graphicsProgram),
			mRenderPass(graphicsPipelineState.renderPass),
			mVkPipeline(VK_NULL_HANDLE)
		{
			// Add a reference to the referenced RHI resources
			mRootSignature->addReference();
			mGraphicsProgram->addReference();
			mRenderPass->addReference();

			// Our pipeline state needs to be independent of concrete render targets, so we're using dynamic viewport ("VK_DYNAMIC_STATE_VIEWPORT") and scissor ("VK_DYNAMIC_STATE_SCISSOR") states
			static constexpr uint32_t WIDTH = 42;
			static constexpr uint32_t HEIGHT = 42;

			// Shaders
			GraphicsProgramGlsl* graphicsProgramGlsl = static_cast<GraphicsProgramGlsl*>(mGraphicsProgram);
			const bool hasMeshShader = (nullptr != graphicsProgramGlsl->getMeshShaderGlsl());
			uint32_t stageCount = 0;
			::detail::VkPipelineShaderStageCreateInfos vkPipelineShaderStageCreateInfos;
			{
				// Define helper macro
#define SHADER_STAGE(vkShaderStageFlagBits, shaderGlsl) \
					if (nullptr != shaderGlsl) \
					{ \
						::detail::addVkPipelineShaderStageCreateInfo(vkShaderStageFlagBits, shaderGlsl->getVkShaderModule(), vkPipelineShaderStageCreateInfos, stageCount); \
						++stageCount; \
					}

				// Shader stages
				SHADER_STAGE(VK_SHADER_STAGE_TASK_BIT_NV, graphicsProgramGlsl->getTaskShaderGlsl())
					SHADER_STAGE(VK_SHADER_STAGE_MESH_BIT_NV, graphicsProgramGlsl->getMeshShaderGlsl())
					SHADER_STAGE(VK_SHADER_STAGE_VERTEX_BIT, graphicsProgramGlsl->getVertexShaderGlsl())
					SHADER_STAGE(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, graphicsProgramGlsl->getTessellationControlShaderGlsl())
					SHADER_STAGE(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, graphicsProgramGlsl->getTessellationEvaluationShaderGlsl())
					SHADER_STAGE(VK_SHADER_STAGE_GEOMETRY_BIT, graphicsProgramGlsl->getGeometryShaderGlsl())
					SHADER_STAGE(VK_SHADER_STAGE_FRAGMENT_BIT, graphicsProgramGlsl->getFragmentShaderGlsl())

					// Undefine helper macro
#undef SHADER_STAGE
			}

			// Vertex attributes
			const uint32_t numberOfAttributes = graphicsPipelineState.vertexAttributes.numberOfAttributes;
			std::vector<VkVertexInputBindingDescription> vkVertexInputBindingDescriptions;
			std::vector<VkVertexInputAttributeDescription> vkVertexInputAttributeDescriptions(numberOfAttributes);
			for ( uint32_t attribute = 0; attribute < numberOfAttributes; ++attribute )
			{
				const Rhi::VertexAttribute* attributes = &graphicsPipelineState.vertexAttributes.attributes[attribute];
				const uint32_t inputSlot = attributes->inputSlot;

				{ // Map to Vulkan vertex input binding description
					if ( vkVertexInputBindingDescriptions.size() <= inputSlot )
					{
						vkVertexInputBindingDescriptions.resize(inputSlot + 1);
					}
					VkVertexInputBindingDescription& vkVertexInputBindingDescription = vkVertexInputBindingDescriptions[inputSlot];
					vkVertexInputBindingDescription.binding = inputSlot;
					vkVertexInputBindingDescription.stride = attributes->strideInBytes;
					vkVertexInputBindingDescription.inputRate = (attributes->instancesPerElement > 0) ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
				}

				{ // Map to Vulkan vertex input attribute description
					VkVertexInputAttributeDescription& vkVertexInputAttributeDescription = vkVertexInputAttributeDescriptions[attribute];
					vkVertexInputAttributeDescription.location = attribute;
					vkVertexInputAttributeDescription.binding = inputSlot;
					vkVertexInputAttributeDescription.format = Mapping::getVulkanFormat(attributes->vertexAttributeFormat);
					vkVertexInputAttributeDescription.offset = attributes->alignedByteOffset;
				}
			}

			// Create the Vulkan graphics pipeline
			// TODO(co) Implement the rest of the value mappings
			const VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,			// sType (VkStructureType)
				nullptr,															// pNext (const void*)
				0,																	// flags (VkPipelineVertexInputStateCreateFlags)
				static_cast<uint32_t>(vkVertexInputBindingDescriptions.size()),		// vertexBindingDescriptionCount (uint32_t)
				vkVertexInputBindingDescriptions.data(),							// pVertexBindingDescriptions (const VkVertexInputBindingDescription*)
				static_cast<uint32_t>(vkVertexInputAttributeDescriptions.size()),	// vertexAttributeDescriptionCount (uint32_t)
				vkVertexInputAttributeDescriptions.data()							// pVertexAttributeDescriptions (const VkVertexInputAttributeDescription*)
			};
			const VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,		// sType (VkStructureType)
				nullptr,															// pNext (const void*)
				0,																	// flags (VkPipelineInputAssemblyStateCreateFlags)
				Mapping::getVulkanType(graphicsPipelineState.primitiveTopology),	// topology (VkPrimitiveTopology)
				VK_FALSE															// primitiveRestartEnable (VkBool32)
			};
			const VkViewport vkViewport =
			{
				0.0f,						// x (float)
				0.0f,						// y (float)
				static_cast<float>(WIDTH),	// width (float)
				static_cast<float>(HEIGHT),	// height (float)
				0.0f,						// minDepth (float)
				1.0f						// maxDepth (float)
			};
			const VkRect2D scissorVkRect2D =
			{
				{ // offset (VkOffset2D)
					0,	// x (int32_t)
					0	// y (int32_t)
				},
				{ // extent (VkExtent2D)
					WIDTH,	// width (uint32_t)
					HEIGHT	// height (uint32_t)
				}
			};
			const VkPipelineTessellationStateCreateInfo vkPipelineTessellationStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,																																									// sType (VkStructureType)
				nullptr,																																																					// pNext (const void*)
				0,																																																							// flags (VkPipelineTessellationStateCreateFlags)
				(graphicsPipelineState.primitiveTopology >= Rhi::PrimitiveTopology::PATCH_LIST_1) ? static_cast<uint32_t>(graphicsPipelineState.primitiveTopology) - static_cast<uint32_t>(Rhi::PrimitiveTopology::PATCH_LIST_1) + 1 : 1	// patchControlPoints (uint32_t)
			};
			const VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,	// sType (VkStructureType)
				nullptr,												// pNext (const void*)
				0,														// flags (VkPipelineViewportStateCreateFlags)
				1,														// viewportCount (uint32_t)
				&vkViewport,											// pViewports (const VkViewport*)
				1,														// scissorCount (uint32_t)
				&scissorVkRect2D										// pScissors (const VkRect2D*)
			};
			const float depthBias = static_cast<float>(graphicsPipelineState.rasterizerState.depthBias);
			const float depthBiasClamp = graphicsPipelineState.rasterizerState.depthBiasClamp;
			const float slopeScaledDepthBias = graphicsPipelineState.rasterizerState.slopeScaledDepthBias;
			const VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,																		// sType (VkStructureType)
				nullptr,																														// pNext (const void*)
				0,																																// flags (VkPipelineRasterizationStateCreateFlags)
				static_cast<VkBool32>(graphicsPipelineState.rasterizerState.depthClipEnable),													// depthClampEnable (VkBool32)
				VK_FALSE,																														// rasterizerDiscardEnable (VkBool32)
				(Rhi::FillMode::WIREFRAME == graphicsPipelineState.rasterizerState.fillMode) ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL,		// polygonMode (VkPolygonMode)
				static_cast<VkCullModeFlags>(static_cast<int>(graphicsPipelineState.rasterizerState.cullMode) - 1),								// cullMode (VkCullModeFlags)
				(1 == graphicsPipelineState.rasterizerState.frontCounterClockwise) ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE,	// frontFace (VkFrontFace)
				static_cast<VkBool32>(0.0f != depthBias || 0.0f != depthBiasClamp || 0.0f != slopeScaledDepthBias),								// depthBiasEnable (VkBool32)
				depthBias,																														// depthBiasConstantFactor (float)
				depthBiasClamp,																													// depthBiasClamp (float)
				slopeScaledDepthBias,																											// depthBiasSlopeFactor (float)
				1.0f																															// lineWidth (float)
			};
			const RenderPass* renderPass = static_cast<const RenderPass*>(mRenderPass);
			const VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,	// sType (VkStructureType)
				nullptr,													// pNext (const void*)
				0,															// flags (VkPipelineMultisampleStateCreateFlags)
				renderPass->getVkSampleCountFlagBits(),						// rasterizationSamples (VkSampleCountFlagBits)
				VK_FALSE,													// sampleShadingEnable (VkBool32)
				0.0f,														// minSampleShading (float)
				nullptr,													// pSampleMask (const VkSampleMask*)
				VK_FALSE,													// alphaToCoverageEnable (VkBool32)
				VK_FALSE													// alphaToOneEnable (VkBool32)
			};
			const VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,													// sType (VkStructureType)
				nullptr,																									// pNext (const void*)
				0,																											// flags (VkPipelineDepthStencilStateCreateFlags)
				static_cast<VkBool32>(0 != graphicsPipelineState.depthStencilState.depthEnable),							// depthTestEnable (VkBool32)
				static_cast<VkBool32>(Rhi::DepthWriteMask::ALL == graphicsPipelineState.depthStencilState.depthWriteMask),	// depthWriteEnable (VkBool32)
				Mapping::getVulkanComparisonFunc(graphicsPipelineState.depthStencilState.depthFunc),						// depthCompareOp (VkCompareOp)
				VK_FALSE,																									// depthBoundsTestEnable (VkBool32)
				static_cast<VkBool32>(0 != graphicsPipelineState.depthStencilState.stencilEnable),							// stencilTestEnable (VkBool32)
				{ // front (VkStencilOpState)
					VK_STENCIL_OP_KEEP,																						// failOp (VkStencilOp)
					VK_STENCIL_OP_KEEP,																						// passOp (VkStencilOp)
					VK_STENCIL_OP_KEEP,																						// depthFailOp (VkStencilOp)
					VK_COMPARE_OP_NEVER,																					// compareOp (VkCompareOp)
					0,																										// compareMask (uint32_t)
					0,																										// writeMask (uint32_t)
					0																										// reference (uint32_t)
				},
				{ // back (VkStencilOpState)
					VK_STENCIL_OP_KEEP,																						// failOp (VkStencilOp)
					VK_STENCIL_OP_KEEP,																						// passOp (VkStencilOp)
					VK_STENCIL_OP_KEEP,																						// depthFailOp (VkStencilOp)
					VK_COMPARE_OP_NEVER,																					// compareOp (VkCompareOp)
					0,																										// compareMask (uint32_t)
					0,																										// writeMask (uint32_t)
					0																										// reference (uint32_t)
				},
				0.0f,																										// minDepthBounds (float)
				1.0f																										// maxDepthBounds (float)
			};
			const uint32_t numberOfColorAttachments = renderPass->getNumberOfColorAttachments();
			RHI_ASSERT(numberOfColorAttachments < 8, "Invalid number of Vulkan color attachments")
				RHI_ASSERT(numberOfColorAttachments == graphicsPipelineState.numberOfRenderTargets, "Invalid number of Vulkan color attachments")
				std::array<VkPipelineColorBlendAttachmentState, 8> vkPipelineColorBlendAttachmentStates;
			for ( uint8_t i = 0; i < numberOfColorAttachments; ++i )
			{
				const Rhi::RenderTargetBlendDesc& renderTargetBlendDesc = graphicsPipelineState.blendState.renderTarget[i];
				VkPipelineColorBlendAttachmentState& vkPipelineColorBlendAttachmentState = vkPipelineColorBlendAttachmentStates[i];
				vkPipelineColorBlendAttachmentState.blendEnable = static_cast<VkBool32>(renderTargetBlendDesc.blendEnable);				// blendEnable (VkBool32)
				vkPipelineColorBlendAttachmentState.srcColorBlendFactor = Mapping::getVulkanBlendFactor(renderTargetBlendDesc.srcBlend);		// srcColorBlendFactor (VkBlendFactor)
				vkPipelineColorBlendAttachmentState.dstColorBlendFactor = Mapping::getVulkanBlendFactor(renderTargetBlendDesc.destBlend);		// dstColorBlendFactor (VkBlendFactor)
				vkPipelineColorBlendAttachmentState.colorBlendOp = Mapping::getVulkanBlendOp(renderTargetBlendDesc.blendOp);				// colorBlendOp (VkBlendOp)
				vkPipelineColorBlendAttachmentState.srcAlphaBlendFactor = Mapping::getVulkanBlendFactor(renderTargetBlendDesc.srcBlendAlpha);	// srcAlphaBlendFactor (VkBlendFactor)
				vkPipelineColorBlendAttachmentState.dstAlphaBlendFactor = Mapping::getVulkanBlendFactor(renderTargetBlendDesc.destBlendAlpha);	// dstAlphaBlendFactor (VkBlendFactor)
				vkPipelineColorBlendAttachmentState.alphaBlendOp = Mapping::getVulkanBlendOp(renderTargetBlendDesc.blendOpAlpha);		// alphaBlendOp (VkBlendOp)
				vkPipelineColorBlendAttachmentState.colorWriteMask = renderTargetBlendDesc.renderTargetWriteMask;							// colorWriteMask (VkColorComponentFlags)
			}
			const VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,	// sType (VkStructureType)
				nullptr,													// pNext (const void*)
				0,															// flags (VkPipelineColorBlendStateCreateFlags)
				VK_FALSE,													// logicOpEnable (VkBool32)
				VK_LOGIC_OP_COPY,											// logicOp (VkLogicOp)
				numberOfColorAttachments,									// attachmentCount (uint32_t)
				vkPipelineColorBlendAttachmentStates.data(),				// pAttachments (const VkPipelineColorBlendAttachmentState*)
				{ 0.0f, 0.0f, 0.0f, 0.0f }									// blendConstants[4] (float)
			};
			static constexpr std::array<VkDynamicState, 2> vkDynamicStates =
			{
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};
			static const VkPipelineDynamicStateCreateInfo vkPipelineDynamicStateCreateInfo =
			{
				VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,	// sType (VkStructureType)
				nullptr,												// pNext (const void*)
				0,														// flags (VkPipelineDynamicStateCreateFlags)
				static_cast<uint32_t>(vkDynamicStates.size()),			// dynamicStateCount (uint32_t)
				vkDynamicStates.data()									// pDynamicStates (const VkDynamicState*)
			};
			const VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo =
			{
				VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,					// sType (VkStructureType)
				nullptr,															// pNext (const void*)
				0,																	// flags (VkPipelineCreateFlags)
				stageCount,															// stageCount (uint32_t)
				vkPipelineShaderStageCreateInfos.data(),							// pStages (const VkPipelineShaderStageCreateInfo*)
				&vkPipelineVertexInputStateCreateInfo,								// pVertexInputState (const VkPipelineVertexInputStateCreateInfo*)
				hasMeshShader ? nullptr : &vkPipelineInputAssemblyStateCreateInfo,	// pInputAssemblyState (const VkPipelineInputAssemblyStateCreateInfo*)
				hasMeshShader ? nullptr : &vkPipelineTessellationStateCreateInfo,	// pTessellationState (const VkPipelineTessellationStateCreateInfo*)
				&vkPipelineViewportStateCreateInfo,									// pViewportState (const VkPipelineViewportStateCreateInfo*)
				&vkPipelineRasterizationStateCreateInfo,							// pRasterizationState (const VkPipelineRasterizationStateCreateInfo*)
				&vkPipelineMultisampleStateCreateInfo,								// pMultisampleState (const VkPipelineMultisampleStateCreateInfo*)
				&vkPipelineDepthStencilStateCreateInfo,								// pDepthStencilState (const VkPipelineDepthStencilStateCreateInfo*)
				&vkPipelineColorBlendStateCreateInfo,								// pColorBlendState (const VkPipelineColorBlendStateCreateInfo*)
				&vkPipelineDynamicStateCreateInfo,									// pDynamicState (const VkPipelineDynamicStateCreateInfo*)
				static_cast<RootSignature*>(mRootSignature)->getVkPipelineLayout(),	// layout (VkPipelineLayout)
				renderPass->getVkRenderPass(),										// renderPass (VkRenderPass)
				0,																	// subpass (uint32_t)
				VK_NULL_HANDLE,														// basePipelineHandle (VkPipeline)
				0																	// basePipelineIndex (int32_t)
			};
			if ( vkCreateGraphicsPipelines(vulkanRhi.getVulkanContext().getVkDevice(), VK_NULL_HANDLE, 1, &vkGraphicsPipelineCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkPipeline) == VK_SUCCESS )
			{
				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				if ( nullptr != vkDebugMarkerSetObjectNameEXT )
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics PSO", 15)	// 15 = "Graphics PSO: " including terminating zero
						Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, (uint64_t)mVkPipeline, detailedDebugName);
				}
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create the Vulkan graphics pipeline")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GraphicsPipelineState() override
		{
			// Destroy the Vulkan graphics pipeline
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			if ( VK_NULL_HANDLE != mVkPipeline )
			{
				vkDestroyPipeline(vulkanRhi.getVulkanContext().getVkDevice(), mVkPipeline, vulkanRhi.getVkAllocationCallbacks());
			}

			// Release referenced RHI resources
			mRootSignature->releaseReference();
			mGraphicsProgram->releaseReference();
			mRenderPass->releaseReference();

			// Free the unique compact graphics pipeline state ID
			vulkanRhi.GraphicsPipelineStateMakeId.DestroyID(getId());
		}

		/**
		*  @brief
		*    Return the Vulkan graphics pipeline
		*
		*  @return
		*    The Vulkan graphics pipeline
		*/
		[[nodiscard]] inline VkPipeline getVkPipeline() const
		{
			return mVkPipeline;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), GraphicsPipelineState, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GraphicsPipelineState(const GraphicsPipelineState&) = delete;
		GraphicsPipelineState& operator =(const GraphicsPipelineState&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::IRootSignature*   mRootSignature;
		Rhi::IGraphicsProgram* mGraphicsProgram;
		Rhi::IRenderPass*	   mRenderPass;
		VkPipeline			   mVkPipeline;	// The Vulkan graphics pipeline


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/State/ComputePipelineState.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan compute pipeline state class
	*/
	class ComputePipelineState final : public Rhi::IComputePipelineState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vulkanRhi
		*    Owner Vulkan RHI instance
		*  @param[in] rootSignature
		*    Root signature to use
		*  @param[in] computeShader
		*    Compute shader to use
		*  @param[in] id
		*    The unique compact compute pipeline state ID
		*/
		ComputePipelineState(VulkanRhi& vulkanRhi, Rhi::IRootSignature& rootSignature, Rhi::IComputeShader& computeShader, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IComputePipelineState(vulkanRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRootSignature(rootSignature),
			mComputeShader(computeShader),
			mVkPipeline(VK_NULL_HANDLE)
		{
			// Add a reference to the given root signature and compute shader
			rootSignature.addReference();
			computeShader.addReference();

			// Create the Vulkan compute pipeline
			const VkComputePipelineCreateInfo vkComputePipelineCreateInfo =
			{
				VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,							// sType (VkStructureType)
				nullptr,																// pNext (const void*)
				0,																		// flags (VkPipelineCreateFlags)
				{																		// stage (VkPipelineShaderStageCreateInfo)
					VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType (VkStructureType)
					nullptr,															// pNext (const void*)
					0,																	// flags (VkPipelineShaderStageCreateFlags)
					VK_SHADER_STAGE_COMPUTE_BIT,										// stage (VkShaderStageFlagBits)
					static_cast<ComputeShaderGlsl&>(computeShader).getVkShaderModule(),	// module (VkShaderModule)
					"main",																// pName (const char*)
					nullptr																// pSpecializationInfo (const VkSpecializationInfo*)
				},
				static_cast<RootSignature&>(rootSignature).getVkPipelineLayout(),		// layout (VkPipelineLayout)
				VK_NULL_HANDLE,															// basePipelineHandle (VkPipeline)
				0																		// basePipelineIndex (int32_t)
			};
			if ( vkCreateComputePipelines(vulkanRhi.getVulkanContext().getVkDevice(), VK_NULL_HANDLE, 1, &vkComputePipelineCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkPipeline) == VK_SUCCESS )
			{
				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				if ( nullptr != vkDebugMarkerSetObjectNameEXT )
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Compute PSO", 14)	// 14 = "Compute PSO: " including terminating zero
						Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, (uint64_t)mVkPipeline, detailedDebugName);
				}
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create the Vulkan compute pipeline")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComputePipelineState() override
		{
			// Destroy the Vulkan compute pipeline
			VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			if ( VK_NULL_HANDLE != mVkPipeline )
			{
				vkDestroyPipeline(vulkanRhi.getVulkanContext().getVkDevice(), mVkPipeline, vulkanRhi.getVkAllocationCallbacks());
			}

			// Release the root signature and compute shader reference
			mRootSignature.releaseReference();
			mComputeShader.releaseReference();

			// Free the unique compact compute pipeline state ID
			vulkanRhi.ComputePipelineStateMakeId.DestroyID(getId());
		}

		/**
		*  @brief
		*    Return the Vulkan compute pipeline
		*
		*  @return
		*    The Vulkan compute pipeline
		*/
		[[nodiscard]] inline VkPipeline getVkPipeline() const
		{
			return mVkPipeline;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), ComputePipelineState, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ComputePipelineState(const ComputePipelineState&) = delete;
		ComputePipelineState& operator =(const ComputePipelineState&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::IRootSignature& mRootSignature;
		Rhi::IComputeShader& mComputeShader;
		VkPipeline			 mVkPipeline;		// The Vulkan compute pipeline


	};




	//[-------------------------------------------------------]
	//[ VulkanRhi/ResourceGroup.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vulkan resource group class
	*/
	class ResourceGroup final : public Rhi::IResourceGroup
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] rootParameterIndex
		*    Root parameter index
		*  @param[in] vkDescriptorSet
		*    Wrapped Vulkan descriptor set
		*  @param[in] numberOfResources
		*    Number of resources, having no resources is invalid
		*  @param[in] resources
		*    At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
		*  @param[in] samplerStates
		*    If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
		*/
		ResourceGroup(RootSignature& rootSignature, uint32_t rootParameterIndex, VkDescriptorSet vkDescriptorSet, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IResourceGroup(rootSignature.getRhi() RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRootSignature(rootSignature),
			mVkDescriptorSet(vkDescriptorSet),
			mNumberOfResources(numberOfResources),
			mResources(RHI_MALLOC_TYPED(rootSignature.getRhi().getContext(), Rhi::IResource*, mNumberOfResources)),
			mSamplerStates(nullptr)
		{
			mRootSignature.addReference();

			// Process all resources and add our reference to the RHI resource
			const VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
			const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
			if ( nullptr != samplerStates )
			{
				mSamplerStates = RHI_MALLOC_TYPED(vulkanRhi.getContext(), Rhi::ISamplerState*, mNumberOfResources);
				for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
				{
					Rhi::ISamplerState* samplerState = mSamplerStates[resourceIndex] = samplerStates[resourceIndex];
					if ( nullptr != samplerState )
					{
						samplerState->addReference();
					}
				}
			}
			for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources )
			{
				Rhi::IResource* resource = *resources;
				RHI_ASSERT(nullptr != resource, "Invalid Vulkan resource")
					mResources[resourceIndex] = resource;
				resource->addReference();

				// Check the type of resource to set
				// TODO(co) Some additional resource type root signature security checks in debug build?
				const Rhi::ResourceType resourceType = resource->getResourceType();
				switch ( resourceType )
				{
				case Rhi::ResourceType::VERTEX_BUFFER:
					{
						const VkDescriptorBufferInfo vkDescriptorBufferInfo =
						{
							static_cast<VertexBuffer*>(resource)->getVkBuffer(),	// buffer (VkBuffer)
							0,														// offset (VkDeviceSize)
							VK_WHOLE_SIZE											// range (VkDeviceSize)
						};
						const VkWriteDescriptorSet vkWriteDescriptorSet =
						{
							VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,		// sType (VkStructureType)
							nullptr,									// pNext (const void*)
							mVkDescriptorSet,							// dstSet (VkDescriptorSet)
							resourceIndex,								// dstBinding (uint32_t)
							0,											// dstArrayElement (uint32_t)
							1,											// descriptorCount (uint32_t)
							VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,			// descriptorType (VkDescriptorType)
							nullptr,									// pImageInfo (const VkDescriptorImageInfo*)
							&vkDescriptorBufferInfo,					// pBufferInfo (const VkDescriptorBufferInfo*)
							nullptr										// pTexelBufferView (const VkBufferView*)
						};
						vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
						break;
					}

				case Rhi::ResourceType::INDEX_BUFFER:
					{
						const VkDescriptorBufferInfo vkDescriptorBufferInfo =
						{
							static_cast<IndexBuffer*>(resource)->getVkBuffer(),	// buffer (VkBuffer)
							0,													// offset (VkDeviceSize)
							VK_WHOLE_SIZE										// range (VkDeviceSize)
						};
						const VkWriteDescriptorSet vkWriteDescriptorSet =
						{
							VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,		// sType (VkStructureType)
							nullptr,									// pNext (const void*)
							mVkDescriptorSet,							// dstSet (VkDescriptorSet)
							resourceIndex,								// dstBinding (uint32_t)
							0,											// dstArrayElement (uint32_t)
							1,											// descriptorCount (uint32_t)
							VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,			// descriptorType (VkDescriptorType)
							nullptr,									// pImageInfo (const VkDescriptorImageInfo*)
							&vkDescriptorBufferInfo,					// pBufferInfo (const VkDescriptorBufferInfo*)
							nullptr										// pTexelBufferView (const VkBufferView*)
						};
						vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
						break;
					}

				case Rhi::ResourceType::TEXTURE_BUFFER:
					{
						const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootSignature.getRootSignature().parameters[rootParameterIndex].descriptorTable.descriptorRanges)[resourceIndex];
						RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "Vulkan texture buffer must bound at SRV or UAV descriptor range type")
							const VkBufferView vkBufferView = static_cast<TextureBuffer*>(resource)->getVkBufferView();
						const VkWriteDescriptorSet vkWriteDescriptorSet =
						{
							VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,																												// sType (VkStructureType)
							nullptr,																																			// pNext (const void*)
							mVkDescriptorSet,																																	// dstSet (VkDescriptorSet)
							resourceIndex,																																		// dstBinding (uint32_t)
							0,																																					// dstArrayElement (uint32_t)
							1,																																					// descriptorCount (uint32_t)
							(Rhi::DescriptorRangeType::SRV == descriptorRange.rangeType) ? VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER : VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,	// descriptorType (VkDescriptorType)
							nullptr,																																			// pImageInfo (const VkDescriptorImageInfo*)
							nullptr,																																			// pBufferInfo (const VkDescriptorBufferInfo*)
							&vkBufferView																																		// pTexelBufferView (const VkBufferView*)
						};
						vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
						break;
					}

				case Rhi::ResourceType::STRUCTURED_BUFFER:
					{
						[[maybe_unused]] const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootSignature.getRootSignature().parameters[rootParameterIndex].descriptorTable.descriptorRanges)[resourceIndex];
						RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "Vulkan structured buffer must bound at SRV or UAV descriptor range type")
							const VkDescriptorBufferInfo vkDescriptorBufferInfo =
						{
							static_cast<StructuredBuffer*>(resource)->getVkBuffer(),	// buffer (VkBuffer)
							0,															// offset (VkDeviceSize)
							VK_WHOLE_SIZE												// range (VkDeviceSize)
						};
						const VkWriteDescriptorSet vkWriteDescriptorSet =
						{
							VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,	// sType (VkStructureType)
							nullptr,								// pNext (const void*)
							mVkDescriptorSet,						// dstSet (VkDescriptorSet)
							resourceIndex,							// dstBinding (uint32_t)
							0,										// dstArrayElement (uint32_t)
							1,										// descriptorCount (uint32_t)
							VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,		// descriptorType (VkDescriptorType)
							nullptr,								// pImageInfo (const VkDescriptorImageInfo*)
							&vkDescriptorBufferInfo,				// pBufferInfo (const VkDescriptorBufferInfo*)
							nullptr									// pTexelBufferView (const VkBufferView*)
						};
						vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
						break;
					}

				case Rhi::ResourceType::INDIRECT_BUFFER:
					{
						const VkDescriptorBufferInfo vkDescriptorBufferInfo =
						{
							static_cast<IndirectBuffer*>(resource)->getVkBuffer(),	// buffer (VkBuffer)
							0,														// offset (VkDeviceSize)
							VK_WHOLE_SIZE											// range (VkDeviceSize)
						};
						const VkWriteDescriptorSet vkWriteDescriptorSet =
						{
							VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,		// sType (VkStructureType)
							nullptr,									// pNext (const void*)
							mVkDescriptorSet,							// dstSet (VkDescriptorSet)
							resourceIndex,								// dstBinding (uint32_t)
							0,											// dstArrayElement (uint32_t)
							1,											// descriptorCount (uint32_t)
							VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,			// descriptorType (VkDescriptorType)
							nullptr,									// pImageInfo (const VkDescriptorImageInfo*)
							&vkDescriptorBufferInfo,					// pBufferInfo (const VkDescriptorBufferInfo*)
							nullptr										// pTexelBufferView (const VkBufferView*)
						};
						vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
						break;
					}

				case Rhi::ResourceType::UNIFORM_BUFFER:
					{
						const VkDescriptorBufferInfo vkDescriptorBufferInfo =
						{
							static_cast<UniformBuffer*>(resource)->getVkBuffer(),	// buffer (VkBuffer)
							0,														// offset (VkDeviceSize)
							VK_WHOLE_SIZE											// range (VkDeviceSize)
						};
						const VkWriteDescriptorSet vkWriteDescriptorSet =
						{
							VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,	// sType (VkStructureType)
							nullptr,								// pNext (const void*)
							mVkDescriptorSet,						// dstSet (VkDescriptorSet)
							resourceIndex,							// dstBinding (uint32_t)
							0,										// dstArrayElement (uint32_t)
							1,										// descriptorCount (uint32_t)
							VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,		// descriptorType (VkDescriptorType)
							nullptr,								// pImageInfo (const VkDescriptorImageInfo*)
							&vkDescriptorBufferInfo,				// pBufferInfo (const VkDescriptorBufferInfo*)
							nullptr									// pTexelBufferView (const VkBufferView*)
						};
						vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
						break;
					}

				case Rhi::ResourceType::TEXTURE_1D:
				case Rhi::ResourceType::TEXTURE_1D_ARRAY:
				case Rhi::ResourceType::TEXTURE_2D:
				case Rhi::ResourceType::TEXTURE_2D_ARRAY:
				case Rhi::ResourceType::TEXTURE_3D:
				case Rhi::ResourceType::TEXTURE_CUBE:
				case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
					{
						// Evaluate the texture type and get the Vulkan image view
						VkImageView vkImageView = VK_NULL_HANDLE;
						VkImageLayout vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
						switch ( resourceType )
						{
						case Rhi::ResourceType::TEXTURE_1D:
							{
								const Texture1D* texture1D = static_cast<Texture1D*>(resource);
								vkImageView = texture1D->getVkImageView();
								vkImageLayout = texture1D->getVkImageLayout();
								break;
							}

						case Rhi::ResourceType::TEXTURE_1D_ARRAY:
							{
								const Texture1DArray* texture1DArray = static_cast<Texture1DArray*>(resource);
								vkImageView = texture1DArray->getVkImageView();
								vkImageLayout = texture1DArray->getVkImageLayout();
								break;
							}

						case Rhi::ResourceType::TEXTURE_2D:
							{
								const Texture2D* texture2D = static_cast<Texture2D*>(resource);
								vkImageView = texture2D->getVkImageView();
								vkImageLayout = texture2D->getVkImageLayout();
								break;
							}

						case Rhi::ResourceType::TEXTURE_2D_ARRAY:
							{
								const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(resource);
								vkImageView = texture2DArray->getVkImageView();
								vkImageLayout = texture2DArray->getVkImageLayout();
								break;
							}

						case Rhi::ResourceType::TEXTURE_3D:
							{
								const Texture3D* texture3D = static_cast<Texture3D*>(resource);
								vkImageView = texture3D->getVkImageView();
								vkImageLayout = texture3D->getVkImageLayout();
								break;
							}

						case Rhi::ResourceType::TEXTURE_CUBE:
							{
								const TextureCube* textureCube = static_cast<TextureCube*>(resource);
								vkImageView = textureCube->getVkImageView();
								vkImageLayout = textureCube->getVkImageLayout();
								break;
							}

						case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
							{
								const TextureCubeArray* textureCubeArray = static_cast<TextureCubeArray*>(resource);
								vkImageView = textureCubeArray->getVkImageView();
								vkImageLayout = textureCubeArray->getVkImageLayout();
								break;
							}

						case Rhi::ResourceType::ROOT_SIGNATURE:
						case Rhi::ResourceType::RESOURCE_GROUP:
						case Rhi::ResourceType::GRAPHICS_PROGRAM:
						case Rhi::ResourceType::VERTEX_ARRAY:
						case Rhi::ResourceType::RENDER_PASS:
						case Rhi::ResourceType::QUERY_POOL:
						case Rhi::ResourceType::SWAP_CHAIN:
						case Rhi::ResourceType::FRAMEBUFFER:
						case Rhi::ResourceType::VERTEX_BUFFER:
						case Rhi::ResourceType::INDEX_BUFFER:
						case Rhi::ResourceType::INDIRECT_BUFFER:
						case Rhi::ResourceType::TEXTURE_BUFFER:
						case Rhi::ResourceType::STRUCTURED_BUFFER:
						case Rhi::ResourceType::UNIFORM_BUFFER:
						case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
						case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
						case Rhi::ResourceType::SAMPLER_STATE:
						case Rhi::ResourceType::VERTEX_SHADER:
						case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
						case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
						case Rhi::ResourceType::GEOMETRY_SHADER:
						case Rhi::ResourceType::FRAGMENT_SHADER:
						case Rhi::ResourceType::TASK_SHADER:
						case Rhi::ResourceType::MESH_SHADER:
						case Rhi::ResourceType::COMPUTE_SHADER:
							RHI_LOG(CRITICAL, "Invalid Vulkan RHI implementation resource type")
								break;
						}

						// Get the sampler state
						const SamplerState* samplerState = (nullptr != mSamplerStates) ? static_cast<const SamplerState*>(mSamplerStates[resourceIndex]) : nullptr;

						// Update Vulkan descriptor sets
						const VkDescriptorImageInfo vkDescriptorImageInfo =
						{
							(nullptr != samplerState) ? samplerState->getVkSampler() : VK_NULL_HANDLE,	// sampler (VkSampler)
							vkImageView,																// imageView (VkImageView)
							vkImageLayout																// imageLayout (VkImageLayout)
						};
						const VkWriteDescriptorSet vkWriteDescriptorSet =
						{
							VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,																		// sType (VkStructureType)
							nullptr,																									// pNext (const void*)
							mVkDescriptorSet,																							// dstSet (VkDescriptorSet)
							resourceIndex,																								// dstBinding (uint32_t)
							0,																											// dstArrayElement (uint32_t)
							1,																											// descriptorCount (uint32_t)
							(nullptr != samplerState) ? VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER : VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,	// descriptorType (VkDescriptorType)
							&vkDescriptorImageInfo,																						// pImageInfo (const VkDescriptorImageInfo*)
							nullptr,																									// pBufferInfo (const VkDescriptorBufferInfo*)
							nullptr																										// pTexelBufferView (const VkBufferView*)
						};
						vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
						break;
					}

				case Rhi::ResourceType::SAMPLER_STATE:
					// Nothing to do in here, Vulkan is using combined image samplers
					break;

				case Rhi::ResourceType::ROOT_SIGNATURE:
				case Rhi::ResourceType::RESOURCE_GROUP:
				case Rhi::ResourceType::GRAPHICS_PROGRAM:
				case Rhi::ResourceType::VERTEX_ARRAY:
				case Rhi::ResourceType::RENDER_PASS:
				case Rhi::ResourceType::QUERY_POOL:
				case Rhi::ResourceType::SWAP_CHAIN:
				case Rhi::ResourceType::FRAMEBUFFER:
				case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
				case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
				case Rhi::ResourceType::VERTEX_SHADER:
				case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
				case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
				case Rhi::ResourceType::GEOMETRY_SHADER:
				case Rhi::ResourceType::FRAGMENT_SHADER:
				case Rhi::ResourceType::TASK_SHADER:
				case Rhi::ResourceType::MESH_SHADER:
				case Rhi::ResourceType::COMPUTE_SHADER:
					RHI_LOG(CRITICAL, "Invalid Vulkan RHI implementation resource type")
						break;
				}
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( nullptr != vkDebugMarkerSetObjectNameEXT )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Resource group", 17)	// 17 = "Resource group: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, (uint64_t)mVkDescriptorSet, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ResourceGroup() override
		{
			// Remove our reference from the RHI resources
			const Rhi::Context& context = getRhi().getContext();
			if ( nullptr != mSamplerStates )
			{
				for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
				{
					Rhi::ISamplerState* samplerState = mSamplerStates[resourceIndex];
					if ( nullptr != samplerState )
					{
						samplerState->releaseReference();
					}
				}
				RHI_FREE(context, mSamplerStates);
			}
			for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
			{
				mResources[resourceIndex]->releaseReference();
			}
			RHI_FREE(context, mResources);

			// Free Vulkan descriptor set
			if ( VK_NULL_HANDLE != mVkDescriptorSet )
			{
				vkFreeDescriptorSets(static_cast<VulkanRhi&>(mRootSignature.getRhi()).getVulkanContext().getVkDevice(), mRootSignature.getVkDescriptorPool(), 1, &mVkDescriptorSet);
			}
			mRootSignature.releaseReference();
		}

		/**
		*  @brief
		*    Return the Vulkan descriptor set
		*
		*  @return
		*    The Vulkan descriptor set, can be a null handle
		*/
		[[nodiscard]] inline VkDescriptorSet getVkDescriptorSet() const
		{
			return mVkDescriptorSet;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual Rhi::RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
		{
			RHI_DELETE(getRhi().getContext(), ResourceGroup, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ResourceGroup(const ResourceGroup&) = delete;
		ResourceGroup& operator =(const ResourceGroup&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		RootSignature&		 mRootSignature;		// Root signature
		VkDescriptorSet		 mVkDescriptorSet;		// "mVkDescriptorPool" of the root signature is the owner which manages the memory, can be a null handle (e.g. for a sampler resource group)
		uint32_t			 mNumberOfResources;	// Number of resources this resource group groups together
		Rhi::IResource**	 mResources;			// RHI resource, we keep a reference to it
		Rhi::ISamplerState** mSamplerStates;		// Sampler states, we keep a reference to it


	};

	// TODO(co) Try to somehow simplify the internal dependencies to be able to put this method directly into the class
	Rhi::IResourceGroup* RootSignature::createResourceGroup(uint32_t rootParameterIndex, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		VulkanRhi& vulkanRhi = static_cast<VulkanRhi&>(getRhi());
		const Rhi::Context& context = vulkanRhi.getContext();

		// Sanity checks
		RHI_ASSERT(VK_NULL_HANDLE != mVkDescriptorPool, "The Vulkan descriptor pool instance must be valid")
			RHI_ASSERT(rootParameterIndex < mVkDescriptorSetLayouts.size(), "The Vulkan root parameter index is out-of-bounds")
			RHI_ASSERT(numberOfResources > 0, "The number of Vulkan resources must not be zero")
			RHI_ASSERT(nullptr != resources, "The Vulkan resource pointers must be valid")

			// Allocate Vulkan descriptor set
			VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;
		if ( (*resources)->getResourceType() != Rhi::ResourceType::SAMPLER_STATE )
		{
			const VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo =
			{
				VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,	// sType (VkStructureType)
				nullptr,										// pNext (const void*)
				mVkDescriptorPool,								// descriptorPool (VkDescriptorPool)
				1,												// descriptorSetCount (uint32_t)
				&mVkDescriptorSetLayouts[rootParameterIndex]	// pSetLayouts (const VkDescriptorSetLayout*)
			};
			if ( vkAllocateDescriptorSets(vulkanRhi.getVulkanContext().getVkDevice(), &vkDescriptorSetAllocateInfo, &vkDescriptorSet) != VK_SUCCESS )
			{
				RHI_LOG(CRITICAL, "Failed to allocate the Vulkan descriptor set")
			}
		}

		// Create resource group
		return RHI_NEW(context, ResourceGroup)(*this, rootParameterIndex, vkDescriptorSet, numberOfResources, resources, samplerStates RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}




	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
} // VulkanRhi

//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		// Implementation from "08/02/2015 Better array 'countof' implementation with C++ 11 (updated)" - https://www.g-truc.net/post-0708.html
		template<typename T, std::size_t N>
		[[nodiscard]] constexpr std::size_t countof(T const (&)[N])
		{
			return N;
		}

		[[nodiscard]] VKAPI_ATTR void* VKAPI_CALL vkAllocationFunction(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope)
		{
			return GetAllocator().reallocate(nullptr, 0, size, alignment);
		}

		[[nodiscard]] VKAPI_ATTR void* VKAPI_CALL vkReallocationFunction(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope)
		{
			return GetAllocator().reallocate(pOriginal, 0, size, alignment);
		}

		VKAPI_ATTR void VKAPI_CALL vkFreeFunction(void* pUserData, void* pMemory)
		{
			GetAllocator().reallocate(pMemory, 0, 0, 1);
		}

		namespace ImplementationDispatch
		{


			//[-------------------------------------------------------]
			//[ Command buffer                                        ]
			//[-------------------------------------------------------]
			void ExecuteCommandBuffer(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ExecuteCommandBuffer* realData = static_cast<const Rhi::Command::ExecuteCommandBuffer*>(data);
				RHI_ASSERT(nullptr != realData->commandBufferToExecute, "The Vulkan command buffer to execute must be valid")
					rhi.submitCommandBuffer(*realData->commandBufferToExecute);
			}

			//[-------------------------------------------------------]
			//[ Graphics states                                       ]
			//[-------------------------------------------------------]
			void SetGraphicsRootSignature(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsRootSignature* realData = static_cast<const Rhi::Command::SetGraphicsRootSignature*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setGraphicsRootSignature(realData->rootSignature);
			}

			void SetGraphicsPipelineState(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsPipelineState* realData = static_cast<const Rhi::Command::SetGraphicsPipelineState*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setGraphicsPipelineState(realData->graphicsPipelineState);
			}

			void SetGraphicsResourceGroup(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsResourceGroup* realData = static_cast<const Rhi::Command::SetGraphicsResourceGroup*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setGraphicsResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
			}

			void SetGraphicsVertexArray(const void* data, Rhi::IRhi& rhi)
			{
				// Input-assembler (IA) stage
				const Rhi::Command::SetGraphicsVertexArray* realData = static_cast<const Rhi::Command::SetGraphicsVertexArray*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setGraphicsVertexArray(realData->vertexArray);
			}

			void SetGraphicsViewports(const void* data, Rhi::IRhi& rhi)
			{
				// Rasterizer (RS) stage
				const Rhi::Command::SetGraphicsViewports* realData = static_cast<const Rhi::Command::SetGraphicsViewports*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setGraphicsViewports(realData->numberOfViewports, (nullptr != realData->viewports) ? realData->viewports : reinterpret_cast<const Rhi::Viewport*>(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData)));
			}

			void SetGraphicsScissorRectangles(const void* data, Rhi::IRhi& rhi)
			{
				// Rasterizer (RS) stage
				const Rhi::Command::SetGraphicsScissorRectangles* realData = static_cast<const Rhi::Command::SetGraphicsScissorRectangles*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setGraphicsScissorRectangles(realData->numberOfScissorRectangles, (nullptr != realData->scissorRectangles) ? realData->scissorRectangles : reinterpret_cast<const Rhi::ScissorRectangle*>(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData)));
			}

			void SetGraphicsRenderTarget(const void* data, Rhi::IRhi& rhi)
			{
				// Output-merger (OM) stage
				const Rhi::Command::SetGraphicsRenderTarget* realData = static_cast<const Rhi::Command::SetGraphicsRenderTarget*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setGraphicsRenderTarget(realData->renderTarget);
			}

			void ClearGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ClearGraphics* realData = static_cast<const Rhi::Command::ClearGraphics*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).clearGraphics(realData->clearFlags, realData->color, realData->z, realData->stencil);
			}

			void DrawGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawGraphics* realData = static_cast<const Rhi::Command::DrawGraphics*>(data);
				if ( nullptr != realData->indirectBuffer )
				{
					static_cast<VulkanRhi::VulkanRhi&>(rhi).drawGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<VulkanRhi::VulkanRhi&>(rhi).drawGraphicsEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			void DrawIndexedGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawIndexedGraphics* realData = static_cast<const Rhi::Command::DrawIndexedGraphics*>(data);
				if ( nullptr != realData->indirectBuffer )
				{
					static_cast<VulkanRhi::VulkanRhi&>(rhi).drawIndexedGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<VulkanRhi::VulkanRhi&>(rhi).drawIndexedGraphicsEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			void DrawMeshTasks(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawMeshTasks* realData = static_cast<const Rhi::Command::DrawMeshTasks*>(data);
				if ( nullptr != realData->indirectBuffer )
				{
					static_cast<VulkanRhi::VulkanRhi&>(rhi).drawMeshTasks(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<VulkanRhi::VulkanRhi&>(rhi).drawMeshTasksEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			//[-------------------------------------------------------]
			//[ Compute                                               ]
			//[-------------------------------------------------------]
			void SetComputeRootSignature(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputeRootSignature* realData = static_cast<const Rhi::Command::SetComputeRootSignature*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setComputeRootSignature(realData->rootSignature);
			}

			void SetComputePipelineState(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputePipelineState* realData = static_cast<const Rhi::Command::SetComputePipelineState*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setComputePipelineState(realData->computePipelineState);
			}

			void SetComputeResourceGroup(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputeResourceGroup* realData = static_cast<const Rhi::Command::SetComputeResourceGroup*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setComputeResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
			}

			void DispatchCompute(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DispatchCompute* realData = static_cast<const Rhi::Command::DispatchCompute*>(data);
				vkCmdDispatch(static_cast<VulkanRhi::VulkanRhi&>(rhi).getVulkanContext().getVkCommandBuffer(), realData->groupCountX, realData->groupCountY, realData->groupCountZ);
			}

			//[-------------------------------------------------------]
			//[ Resource                                              ]
			//[-------------------------------------------------------]
			void SetTextureMinimumMaximumMipmapIndex(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetTextureMinimumMaximumMipmapIndex* realData = static_cast<const Rhi::Command::SetTextureMinimumMaximumMipmapIndex*>(data);
				if ( realData->texture->getResourceType() == Rhi::ResourceType::TEXTURE_2D )
				{
					static_cast<VulkanRhi::Texture2D*>(realData->texture)->setMinimumMaximumMipmapIndex(realData->minimumMipmapIndex, realData->maximumMipmapIndex);
				}
				else
				{
					RHI_LOG(CRITICAL, "Unsupported Vulkan texture resource type")
				}
			}

			void ResolveMultisampleFramebuffer(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ResolveMultisampleFramebuffer* realData = static_cast<const Rhi::Command::ResolveMultisampleFramebuffer*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).resolveMultisampleFramebuffer(*realData->destinationRenderTarget, *realData->sourceMultisampleFramebuffer);
			}

			void CopyResource(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::CopyResource* realData = static_cast<const Rhi::Command::CopyResource*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).copyResource(*realData->destinationResource, *realData->sourceResource);
			}

			void GenerateMipmaps(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::GenerateMipmaps* realData = static_cast<const Rhi::Command::GenerateMipmaps*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).generateMipmaps(*realData->resource);
			}

			//[-------------------------------------------------------]
			//[ Query                                                 ]
			//[-------------------------------------------------------]
			void ResetQueryPool(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ResetQueryPool* realData = static_cast<const Rhi::Command::ResetQueryPool*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).resetQueryPool(*realData->queryPool, realData->firstQueryIndex, realData->numberOfQueries);
			}

			void BeginQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::BeginQuery* realData = static_cast<const Rhi::Command::BeginQuery*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).beginQuery(*realData->queryPool, realData->queryIndex, realData->queryControlFlags);
			}

			void EndQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::EndQuery* realData = static_cast<const Rhi::Command::EndQuery*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).endQuery(*realData->queryPool, realData->queryIndex);
			}

			void WriteTimestampQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::WriteTimestampQuery* realData = static_cast<const Rhi::Command::WriteTimestampQuery*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).writeTimestampQuery(*realData->queryPool, realData->queryIndex);
			}

			//[-------------------------------------------------------]
			//[ Debug                                                 ]
			//[-------------------------------------------------------]
#if SE_DEBUG
			void SetDebugMarker(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetDebugMarker* realData = static_cast<const Rhi::Command::SetDebugMarker*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).setDebugMarker(realData->name);
			}

			void BeginDebugEvent(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::BeginDebugEvent* realData = static_cast<const Rhi::Command::BeginDebugEvent*>(data);
				static_cast<VulkanRhi::VulkanRhi&>(rhi).beginDebugEvent(realData->name);
			}

			void EndDebugEvent(const void*, Rhi::IRhi& rhi)
			{
				static_cast<VulkanRhi::VulkanRhi&>(rhi).endDebugEvent();
			}
#else
			void SetDebugMarker(const void*, Rhi::IRhi&)
			{
				// Nothing here
			}

			void BeginDebugEvent(const void*, Rhi::IRhi&)
			{
				// Nothing here
			}

			void EndDebugEvent(const void*, Rhi::IRhi&)
			{
				// Nothing here
			}
#endif


		}

		void beginVulkanRenderPass(const Rhi::IRenderTarget& renderTarget, VkRenderPass vkRenderPass, VkFramebuffer vkFramebuffer, uint32_t numberOfAttachments, const VulkanRhi::VulkanRhi::VkClearValues& vkClearValues, VkCommandBuffer vkCommandBuffer)
		{
			// Get render target dimension
			uint32_t width = 1;
			uint32_t height = 1;
			renderTarget.getWidthAndHeight(width, height);

			// Begin Vulkan render pass
			const VkRenderPassBeginInfo vkRenderPassBeginInfo =
			{
				VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,	// sType (VkStructureType)
				nullptr,									// pNext (const void*)
				vkRenderPass,								// renderPass (VkRenderPass)
				vkFramebuffer,								// framebuffer (VkFramebuffer)
				{ // renderArea (VkRect2D)
					{ 0, 0 },								// offset (VkOffset2D)
					{ width, height }						// extent (VkExtent2D)
				},
				numberOfAttachments,						// clearValueCount (uint32_t)
				vkClearValues.data()						// pClearValues (const VkClearValue*)
			};
			vkCmdBeginRenderPass(vkCommandBuffer, &vkRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		}


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr Rhi::ImplementationDispatchFunction DISPATCH_FUNCTIONS[static_cast<uint8_t>(Rhi::CommandDispatchFunctionIndex::NUMBER_OF_FUNCTIONS)] =
		{
			// Command buffer
			&ImplementationDispatch::ExecuteCommandBuffer,
			// Graphics
			&ImplementationDispatch::SetGraphicsRootSignature,
			&ImplementationDispatch::SetGraphicsPipelineState,
			&ImplementationDispatch::SetGraphicsResourceGroup,
			&ImplementationDispatch::SetGraphicsVertexArray,		// Input-assembler (IA) stage
			&ImplementationDispatch::SetGraphicsViewports,			// Rasterizer (RS) stage
			&ImplementationDispatch::SetGraphicsScissorRectangles,	// Rasterizer (RS) stage
			&ImplementationDispatch::SetGraphicsRenderTarget,		// Output-merger (OM) stage
			&ImplementationDispatch::ClearGraphics,
			&ImplementationDispatch::DrawGraphics,
			&ImplementationDispatch::DrawIndexedGraphics,
			&ImplementationDispatch::DrawMeshTasks,
			// Compute
			&ImplementationDispatch::SetComputeRootSignature,
			&ImplementationDispatch::SetComputePipelineState,
			&ImplementationDispatch::SetComputeResourceGroup,
			&ImplementationDispatch::DispatchCompute,
			// Resource
			&ImplementationDispatch::SetTextureMinimumMaximumMipmapIndex,
			&ImplementationDispatch::ResolveMultisampleFramebuffer,
			&ImplementationDispatch::CopyResource,
			&ImplementationDispatch::GenerateMipmaps,
			// Query
			&ImplementationDispatch::ResetQueryPool,
			&ImplementationDispatch::BeginQuery,
			&ImplementationDispatch::EndQuery,
			&ImplementationDispatch::WriteTimestampQuery,
			// Debug
			&ImplementationDispatch::SetDebugMarker,
			&ImplementationDispatch::BeginDebugEvent,
			&ImplementationDispatch::EndDebugEvent
		};


		//[-------------------------------------------------------]
		//[ Anonymous detail namespace                            ]
		//[-------------------------------------------------------]
	} // detail
}
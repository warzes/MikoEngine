#pragma once

namespace OpenGLRhi
{
	class Mapping final
	{
	public:
		//[-------------------------------------------------------]
		//[ Rhi::FilterMode                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::FilterMode" to OpenGL magnification filter mode
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] filterMode
		*    "Rhi::FilterMode" to map
		*
		*  @return
		*    OpenGL magnification filter mode
		*/
		[[nodiscard]] static GLint getOpenGLMagFilterMode([[maybe_unused]] const Rhi::Context& context, Rhi::FilterMode filterMode)
		{
			switch ( filterMode )
			{
			case Rhi::FilterMode::MIN_MAG_MIP_POINT:
				return GL_NEAREST;

			case Rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
				return GL_NEAREST;

			case Rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
				return GL_LINEAR;

			case Rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
				return GL_LINEAR;

			case Rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
				return GL_NEAREST;

			case Rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return GL_NEAREST;

			case Rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
				return GL_LINEAR;

			case Rhi::FilterMode::MIN_MAG_MIP_LINEAR:
				return GL_LINEAR;

			case Rhi::FilterMode::ANISOTROPIC:
				return GL_LINEAR;	// There's no special setting in OpenGL

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
				return GL_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
				return GL_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
				return GL_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
				return GL_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
				return GL_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return GL_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
				return GL_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
				return GL_LINEAR;

			case Rhi::FilterMode::COMPARISON_ANISOTROPIC:
				return GL_LINEAR;	// There's no special setting in OpenGL

			case Rhi::FilterMode::UNKNOWN:
				RHI_ASSERT(false, "OpenGL filter mode must not be unknown")
					return GL_NEAREST;

			default:
				return GL_NEAREST;	// We should never be in here
			}
		}

		/**
		*  @brief
		*    "Rhi::FilterMode" to OpenGL minification filter mode
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] filterMode
		*    "Rhi::FilterMode" to map
		*  @param[in] hasMipmaps
		*    Are mipmaps available?
		*
		*  @return
		*    OpenGL minification filter mode
		*/
		[[nodiscard]] static GLint getOpenGLMinFilterMode([[maybe_unused]] const Rhi::Context& context, Rhi::FilterMode filterMode, bool hasMipmaps)
		{
			switch ( filterMode )
			{
			case Rhi::FilterMode::MIN_MAG_MIP_POINT:
				return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

			case Rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
				return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

			case Rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
				return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

			case Rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
				return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

			case Rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
				return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

			case Rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

			case Rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
				return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

			case Rhi::FilterMode::MIN_MAG_MIP_LINEAR:
				return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

			case Rhi::FilterMode::ANISOTROPIC:
				return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;	// There's no special setting in OpenGL

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
				return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
				return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
				return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
				return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
				return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
				return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
				return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

			case Rhi::FilterMode::COMPARISON_ANISOTROPIC:
				return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;	// There's no special setting in OpenGL

			case Rhi::FilterMode::UNKNOWN:
				RHI_ASSERT(false, "OpenGL filter mode must not be unknown")
					return GL_NEAREST;

			default:
				return GL_NEAREST;	// We should never be in here
			}
		}

		/**
		*  @brief
		*    "Rhi::FilterMode" to OpenGL compare mode
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] filterMode
		*    "Rhi::FilterMode" to map
		*
		*  @return
		*    OpenGL compare mode
		*/
		[[nodiscard]] static GLint getOpenGLCompareMode([[maybe_unused]] const Rhi::Context& context, Rhi::FilterMode filterMode)
		{
			switch ( filterMode )
			{
			case Rhi::FilterMode::MIN_MAG_MIP_POINT:
			case Rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
			case Rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
			case Rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
			case Rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
			case Rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			case Rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
			case Rhi::FilterMode::MIN_MAG_MIP_LINEAR:
			case Rhi::FilterMode::ANISOTROPIC:
				return GL_NONE;

			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
			case Rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
			case Rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
			case Rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			case Rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
			case Rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
			case Rhi::FilterMode::COMPARISON_ANISOTROPIC:
				return GL_COMPARE_REF_TO_TEXTURE;

			case Rhi::FilterMode::UNKNOWN:
				RHI_ASSERT(false, "OpenGL filter mode must not be unknown")
					return GL_NEAREST;

			default:
				return GL_NEAREST;	// We should never be in here
			}
		}

		//[-------------------------------------------------------]
		//[ Rhi::TextureAddressMode                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::TextureAddressMode" to OpenGL texture address mode
		*
		*  @param[in] textureAddressMode
		*    "Rhi::TextureAddressMode" to map
		*
		*  @return
		*    OpenGL texture address mode
		*/
		[[nodiscard]] static GLint getOpenGLTextureAddressMode(Rhi::TextureAddressMode textureAddressMode)
		{
			static constexpr GLint MAPPING[] =
			{
				GL_REPEAT,			// Rhi::TextureAddressMode::WRAP
				GL_MIRRORED_REPEAT,	// Rhi::TextureAddressMode::MIRROR
				GL_CLAMP_TO_EDGE,	// Rhi::TextureAddressMode::CLAMP
				GL_CLAMP_TO_BORDER,	// Rhi::TextureAddressMode::BORDER
				GL_MIRRORED_REPEAT	// Rhi::TextureAddressMode::MIRROR_ONCE	// TODO(co) OpenGL equivalent? GL_ATI_texture_mirror_once ?
			};
			return MAPPING[static_cast<int>(textureAddressMode) - 1];	// Lookout! The "Rhi::TextureAddressMode"-values start with 1, not 0
		}

		//[-------------------------------------------------------]
		//[ Rhi::ComparisonFunc                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::ComparisonFunc" to OpenGL comparison function
		*
		*  @param[in] comparisonFunc
		*    "Rhi::ComparisonFunc" to map
		*
		*  @return
		*    OpenGL comparison function
		*/
		[[nodiscard]] static GLenum getOpenGLComparisonFunc(Rhi::ComparisonFunc comparisonFunc)
		{
			static constexpr GLenum MAPPING[] =
			{
				GL_NEVER,		// Rhi::ComparisonFunc::NEVER
				GL_LESS,		// Rhi::ComparisonFunc::LESS
				GL_EQUAL,		// Rhi::ComparisonFunc::EQUAL
				GL_LEQUAL,		// Rhi::ComparisonFunc::LESS_EQUAL
				GL_GREATER,		// Rhi::ComparisonFunc::GREATER
				GL_NOTEQUAL,	// Rhi::ComparisonFunc::NOT_EQUAL
				GL_GEQUAL,		// Rhi::ComparisonFunc::GREATER_EQUAL
				GL_ALWAYS		// Rhi::ComparisonFunc::ALWAYS
			};
			return MAPPING[static_cast<int>(comparisonFunc) - 1];	// Lookout! The "Rhi::ComparisonFunc"-values start with 1, not 0
		}

		//[-------------------------------------------------------]
		//[ Rhi::VertexAttributeFormat                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::VertexAttributeFormat" to OpenGL size (number of elements)
		*
		*  @param[in] vertexAttributeFormat
		*    "Rhi::VertexAttributeFormat" to map
		*
		*  @return
		*    OpenGL size
		*/
		[[nodiscard]] static GLint getOpenGLSize(Rhi::VertexAttributeFormat vertexAttributeFormat)
		{
			static constexpr GLint MAPPING[] =
			{
				1,	// Rhi::VertexAttributeFormat::FLOAT_1
				2,	// Rhi::VertexAttributeFormat::FLOAT_2
				3,	// Rhi::VertexAttributeFormat::FLOAT_3
				4,	// Rhi::VertexAttributeFormat::FLOAT_4
				4,	// Rhi::VertexAttributeFormat::R8G8B8A8_UNORM
				4,	// Rhi::VertexAttributeFormat::R8G8B8A8_UINT
				2,	// Rhi::VertexAttributeFormat::SHORT_2
				4,	// Rhi::VertexAttributeFormat::SHORT_4
				1	// Rhi::VertexAttributeFormat::UINT_1
			};
			return MAPPING[static_cast<int>(vertexAttributeFormat)];
		}

		/**
		*  @brief
		*    "Rhi::VertexAttributeFormat" to OpenGL type
		*
		*  @param[in] vertexAttributeFormat
		*    "Rhi::VertexAttributeFormat" to map
		*
		*  @return
		*    OpenGL type
		*/
		[[nodiscard]] static GLenum getOpenGLType(Rhi::VertexAttributeFormat vertexAttributeFormat)
		{
			static constexpr GLenum MAPPING[] =
			{
				GL_FLOAT,			// Rhi::VertexAttributeFormat::FLOAT_1
				GL_FLOAT,			// Rhi::VertexAttributeFormat::FLOAT_2
				GL_FLOAT,			// Rhi::VertexAttributeFormat::FLOAT_3
				GL_FLOAT,			// Rhi::VertexAttributeFormat::FLOAT_4
				GL_UNSIGNED_BYTE,	// Rhi::VertexAttributeFormat::R8G8B8A8_UNORM
				GL_UNSIGNED_BYTE,	// Rhi::VertexAttributeFormat::R8G8B8A8_UINT
				GL_SHORT,			// Rhi::VertexAttributeFormat::SHORT_2
				GL_SHORT,			// Rhi::VertexAttributeFormat::SHORT_4
				GL_UNSIGNED_INT		// Rhi::VertexAttributeFormat::UINT_1
			};
			return MAPPING[static_cast<int>(vertexAttributeFormat)];
		}

		/**
		*  @brief
		*    Return whether or not "Rhi::VertexAttributeFormat" is a normalized format
		*
		*  @param[in] vertexAttributeFormat
		*    "Rhi::VertexAttributeFormat" to check
		*
		*  @return
		*    "GL_TRUE" if the format is normalized, else "GL_FALSE"
		*/
		[[nodiscard]] static GLboolean isOpenGLVertexAttributeFormatNormalized(Rhi::VertexAttributeFormat vertexAttributeFormat)
		{
			static constexpr GLboolean MAPPING[] =
			{
				GL_FALSE,	// Rhi::VertexAttributeFormat::FLOAT_1
				GL_FALSE,	// Rhi::VertexAttributeFormat::FLOAT_2
				GL_FALSE,	// Rhi::VertexAttributeFormat::FLOAT_3
				GL_FALSE,	// Rhi::VertexAttributeFormat::FLOAT_4
				GL_TRUE,	// Rhi::VertexAttributeFormat::R8G8B8A8_UNORM
				GL_FALSE,	// Rhi::VertexAttributeFormat::R8G8B8A8_UINT
				GL_FALSE,	// Rhi::VertexAttributeFormat::SHORT_2
				GL_FALSE,	// Rhi::VertexAttributeFormat::SHORT_4
				GL_FALSE	// Rhi::VertexAttributeFormat::UINT_1
			};
			return MAPPING[static_cast<int>(vertexAttributeFormat)];
		}

		/**
		*  @brief
		*    Return whether or not "Rhi::VertexAttributeFormat" is an integer format
		*
		*  @param[in] vertexAttributeFormat
		*    "Rhi::VertexAttributeFormat" to check
		*
		*  @return
		*    "GL_TRUE" if the format is integer, else "GL_FALSE"
		*/
		[[nodiscard]] static GLboolean isOpenGLVertexAttributeFormatInteger(Rhi::VertexAttributeFormat vertexAttributeFormat)
		{
			static constexpr GLboolean MAPPING[] =
			{
				GL_FALSE,	// Rhi::VertexAttributeFormat::FLOAT_1
				GL_FALSE,	// Rhi::VertexAttributeFormat::FLOAT_2
				GL_FALSE,	// Rhi::VertexAttributeFormat::FLOAT_3
				GL_FALSE,	// Rhi::VertexAttributeFormat::FLOAT_4
				GL_FALSE,	// Rhi::VertexAttributeFormat::R8G8B8A8_UNORM
				GL_TRUE,	// Rhi::VertexAttributeFormat::R8G8B8A8_UINT
				GL_TRUE,	// Rhi::VertexAttributeFormat::SHORT_2
				GL_TRUE,	// Rhi::VertexAttributeFormat::SHORT_4
				GL_TRUE		// Rhi::VertexAttributeFormat::UINT_1
			};
			return MAPPING[static_cast<int>(vertexAttributeFormat)];
		}

		//[-------------------------------------------------------]
		//[ Rhi::IndexBufferFormat                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::IndexBufferFormat" to OpenGL type
		*
		*  @param[in] indexBufferFormat
		*    "Rhi::IndexBufferFormat" to map
		*
		*  @return
		*    OpenGL type
		*/
		[[nodiscard]] static GLenum getOpenGLType(Rhi::IndexBufferFormat::Enum indexBufferFormat)
		{
			static constexpr GLenum MAPPING[] =
			{
				GL_UNSIGNED_BYTE,	// Rhi::IndexBufferFormat::UNSIGNED_CHAR  - One byte per element, uint8_t (may not be supported by each API)
				GL_UNSIGNED_SHORT,	// Rhi::IndexBufferFormat::UNSIGNED_SHORT - Two bytes per element, uint16_t
				GL_UNSIGNED_INT		// Rhi::IndexBufferFormat::UNSIGNED_INT   - Four bytes per element, uint32_t (may not be supported by each API)
			};
			return MAPPING[indexBufferFormat];
		}

		//[-------------------------------------------------------]
		//[ Rhi::TextureFormat                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::TextureFormat" to OpenGL internal format
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to map
		*
		*  @return
		*    OpenGL internal format
		*/
		[[nodiscard]] static GLuint getOpenGLInternalFormat(Rhi::TextureFormat::Enum textureFormat)
		{
			static constexpr GLuint MAPPING[] =
			{
				GL_R8,										// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				GL_RGB8,									// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				GL_RGBA8,									// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				GL_SRGB8_ALPHA8,							// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_RGBA8,									// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				GL_R11F_G11F_B10F_EXT,						// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - "GL_EXT_packed_float" OpenGL extension
				GL_RGBA16F_ARB,								// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				GL_RGBA32F_ARB,								// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha),
				GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,			// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT,		// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,			// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT,		// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,			// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT,		// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_COMPRESSED_LUMINANCE_LATC1_EXT,			// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
				GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,	// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
				0,											// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in OpenGL
				GL_R16,										// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				GL_R32UI,									// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				GL_R32F,									// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				GL_DEPTH_COMPONENT32F,						// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				GL_RG16_SNORM,								// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				GL_RG16F,									// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				0											// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		/**
		*  @brief
		*    "Rhi::TextureFormat" to OpenGL format
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to map
		*
		*  @return
		*    OpenGL format
		*/
		[[nodiscard]] static GLuint getOpenGLFormat(Rhi::TextureFormat::Enum textureFormat)
		{
			static constexpr GLuint MAPPING[] =
			{
				GL_RED,										// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				GL_RGB,										// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				GL_RGBA,									// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				GL_RGBA,									// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_BGRA,									// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				GL_RGB,										// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - "GL_EXT_packed_float" OpenGL extension
				GL_RGBA,									// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				GL_RGBA,									// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,			// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT,		// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,			// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT,		// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,			// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT,		// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_COMPRESSED_LUMINANCE_LATC1_EXT,			// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
				GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,	// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
				0,											// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in OpenGL
				GL_RED,										// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				GL_RED_INTEGER,								// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				GL_RED,										// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				GL_DEPTH_COMPONENT,							// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				GL_RG,										// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				GL_RG,										// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				0											// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		/**
		*  @brief
		*    "Rhi::TextureFormat" to OpenGL type
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to map
		*
		*  @return
		*    OpenGL type
		*/
		[[nodiscard]] static GLenum getOpenGLType(Rhi::TextureFormat::Enum textureFormat)
		{
			static constexpr GLenum MAPPING[] =
			{
				GL_UNSIGNED_BYTE,						// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				GL_UNSIGNED_BYTE,						// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				GL_UNSIGNED_BYTE,						// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				GL_UNSIGNED_BYTE,						// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				GL_UNSIGNED_BYTE,						// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				GL_UNSIGNED_INT_10F_11F_11F_REV_EXT,	// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - "GL_EXT_packed_float" OpenGL extension
				GL_HALF_FLOAT_ARB,						// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				GL_FLOAT,								// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				0,										// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
				0,										// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				0,										// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				0,										// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				0,										// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				0,										// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				0,										// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
				0,										// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
				0,										// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in OpenGL
				GL_UNSIGNED_SHORT,						// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				GL_UNSIGNED_INT,						// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				GL_FLOAT,								// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				GL_FLOAT,								// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				GL_BYTE,								// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				GL_FLOAT,								// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				0										// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		//[-------------------------------------------------------]
		//[ Rhi::PrimitiveTopology                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::PrimitiveTopology" to OpenGL type
		*
		*  @param[in] primitiveTopology
		*    "Rhi::PrimitiveTopology" to map
		*
		*  @return
		*    OpenGL type
		*/
		[[nodiscard]] static GLenum getOpenGLType(Rhi::PrimitiveTopology primitiveTopology)
		{
			static constexpr GLenum MAPPING[] =
			{
				GL_POINTS,			// Rhi::PrimitiveTopology::POINT_LIST
				GL_LINES,			// Rhi::PrimitiveTopology::LINE_LIST
				GL_LINE_STRIP,		// Rhi::PrimitiveTopology::LINE_STRIP
				GL_TRIANGLES,		// Rhi::PrimitiveTopology::TRIANGLE_LIST
				GL_TRIANGLE_STRIP	// Rhi::PrimitiveTopology::TRIANGLE_STRIP
			};
			return MAPPING[static_cast<int>(primitiveTopology) - 1];	// Lookout! The "Rhi::PrimitiveTopology"-values start with 1, not 0
		}

		//[-------------------------------------------------------]
		//[ Rhi::MapType                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::MapType" to OpenGL type
		*
		*  @param[in] mapType
		*    "Rhi::MapType" to map
		*
		*  @return
		*    OpenGL type
		*/
		[[nodiscard]] static GLenum getOpenGLMapType(Rhi::MapType mapType)
		{
			static constexpr GLenum MAPPING[] =
			{
				GL_READ_ONLY,	// Rhi::MapType::READ
				GL_WRITE_ONLY,	// Rhi::MapType::WRITE
				GL_READ_WRITE,	// Rhi::MapType::READ_WRITE
				GL_WRITE_ONLY,	// Rhi::MapType::WRITE_DISCARD
				GL_WRITE_ONLY	// Rhi::MapType::WRITE_NO_OVERWRITE
			};
			return MAPPING[static_cast<int>(mapType) - 1];	// Lookout! The "Rhi::MapType"-values start with 1, not 0
		}

		//[-------------------------------------------------------]
		//[ Rhi::MapType                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::Blend" to OpenGL type
		*
		*  @param[in] blend
		*    "Rhi::Blend" to map
		*
		*  @return
		*    OpenGL type
		*/
		[[nodiscard]] static GLenum getOpenGLBlendType(Rhi::Blend blend)
		{
			if ( blend <= Rhi::Blend::SRC_ALPHA_SAT )
			{
				static constexpr GLenum MAPPING[] =
				{
					GL_ZERO,				// Rhi::Blend::ZERO
					GL_ONE,					// Rhi::Blend::ONE
					GL_SRC_COLOR,			// Rhi::Blend::SRC_COLOR
					GL_ONE_MINUS_SRC_COLOR,	// Rhi::Blend::INV_SRC_COLOR
					GL_SRC_ALPHA,			// Rhi::Blend::SRC_ALPHA
					GL_ONE_MINUS_SRC_ALPHA,	// Rhi::Blend::INV_SRC_ALPHA
					GL_DST_ALPHA,			// Rhi::Blend::DEST_ALPHA
					GL_ONE_MINUS_DST_ALPHA,	// Rhi::Blend::INV_DEST_ALPHA
					GL_DST_COLOR,			// Rhi::Blend::DEST_COLOR
					GL_ONE_MINUS_DST_COLOR,	// Rhi::Blend::INV_DEST_COLOR
					GL_SRC_ALPHA_SATURATE	// Rhi::Blend::SRC_ALPHA_SAT
				};
				return MAPPING[static_cast<int>(blend) - static_cast<int>(Rhi::Blend::ZERO)];
			}
			else
			{
				static constexpr GLenum MAPPING[] =
				{
					GL_SRC_COLOR,				// Rhi::Blend::BLEND_FACTOR		TODO(co) Mapping "Rhi::Blend::BLEND_FACTOR" to OpenGL possible?
					GL_ONE_MINUS_SRC_COLOR,		// Rhi::Blend::INV_BLEND_FACTOR	TODO(co) Mapping "Rhi::Blend::INV_BLEND_FACTOR" to OpenGL possible?
					GL_SRC1_COLOR,				// Rhi::Blend::SRC_1_COLOR
					GL_ONE_MINUS_SRC1_COLOR,	// Rhi::Blend::INV_SRC_1_COLOR
					GL_SRC1_ALPHA,				// Rhi::Blend::SRC_1_ALPHA
					GL_ONE_MINUS_SRC1_ALPHA,	// Rhi::Blend::INV_SRC_1_ALPHA
				};
				return MAPPING[static_cast<int>(blend) - static_cast<int>(Rhi::Blend::BLEND_FACTOR)];
			}
		}
	};
} // namespace OpenGLRhi
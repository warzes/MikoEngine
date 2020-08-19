#pragma once

namespace Rhi
{
	struct TextureFormat final
	{
		enum Enum : uint8_t
		{
			R8 = 0,	// 8-bit pixel format, all bits red
			R8G8B8 = 1,	// 24-bit pixel format, 8 bits for red, green and blue
			R8G8B8A8 = 2,	// 32-bit pixel format, 8 bits for red, green, blue and alpha
			R8G8B8A8_SRGB = 3,	// 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
			B8G8R8A8 = 4,	// 32-bit pixel format, 8 bits for red, green, blue and alpha
			R11G11B10F = 5,	// 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent, see e.g. https://www.khronos.org/opengl/wiki/Small_Float_Formats#R11F_G11F_B10F -> "Small Float Formats" -> "Numeric limits and precision" and "Small float formats – R11G11B10F precision" by Bart Wronski ( https://bartwronski.com/2017/04/02/small-float-formats-r11g11b10f-precision/ )
			R16G16B16A16F = 6,	// 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
			R32G32B32A32F = 7,	// 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
			BC1 = 8,	// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
			BC1_SRGB = 9,	// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
			BC2 = 10,	// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			BC2_SRGB = 11,	// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
			BC3 = 12,	// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			BC3_SRGB = 13,	// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
			BC4 = 14,	// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
			BC5 = 15,	// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
			ETC1 = 16,	// 3 component texture compression meant for mobile devices
			R16_UNORM = 17,	// 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
			R32_UINT = 18,	// 32-bit unsigned integer format
			R32_FLOAT = 19,	// 32-bit float format
			D32_FLOAT = 20,	// 32-bit float depth format
			R16G16_SNORM = 21,	// A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
			R16G16_FLOAT = 22,	// A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
			UNKNOWN = 23,	// Unknown
			NUMBER_OF_FORMATS = 24	// Number of texture formats
		};

		[[nodiscard]] static inline bool isCompressed(Enum textureFormat)
		{
			static constexpr bool MAPPING[] =
			{
				false,	// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				false,	// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				false,	// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				false,	// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				false,	// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				false,	// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				false,	// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				true,	// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				true,	// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				true,	// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				true,	// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices
				false,	// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				false,	// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				false,	// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				false,	// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				false,	// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				false,	// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				false	// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		[[nodiscard]] static inline bool isDepth(Enum textureFormat)
		{
			static constexpr bool MAPPING[] =
			{
				false,	// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				false,	// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				false,	// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				false,	// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				false,	// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				false,	// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				false,	// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				false,	// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices
				false,	// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				false,	// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				false,	// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				true,	// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				false,	// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				false,	// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				false	// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		[[nodiscard]] static inline uint32_t getNumberOfBytesPerElement(Enum textureFormat)
		{
			static constexpr uint32_t MAPPING[] =
			{
				sizeof(uint8_t),		// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				sizeof(uint8_t) * 3,	// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				sizeof(float),			// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				sizeof(float) * 2,		// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				sizeof(float) * 4,		// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				sizeof(uint8_t) * 3,	// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 3,	// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				sizeof(uint8_t) * 1,	// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 2,	// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 3,	// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - when being uncompressed
				sizeof(uint16_t),		// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				sizeof(uint32_t),		// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				sizeof(float),			// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				sizeof(float),			// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				sizeof(uint32_t),		// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				sizeof(float),			// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				0						// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		[[nodiscard]] static inline uint32_t getNumberOfBytesPerRow(Enum textureFormat, uint32_t width)
		{
			switch ( textureFormat )
			{
				// 8-bit pixel format, all bits red
			case R8:
				return width;

				// 24-bit pixel format, 8 bits for red, green and blue
			case R8G8B8:
				return 3 * width;

				// 32-bit pixel format, 8 bits for red, green, blue and alpha
			case R8G8B8A8:
			case R8G8B8A8_SRGB:
			case B8G8R8A8:
				return 4 * width;

				// 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
			case R11G11B10F:
				return 4 * width;

				// 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
			case R16G16B16A16F:
				return 8 * width;

				// 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
			case R32G32B32A32F:
				return 16 * width;

				// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
			case BC1:
			case BC1_SRGB:
				return ((width + 3) >> 2) * 8;

				// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			case BC2:
			case BC2_SRGB:
				return ((width + 3) >> 2) * 16;

				// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			case BC3:
			case BC3_SRGB:
				return ((width + 3) >> 2) * 16;

				// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
			case BC4:
				return ((width + 3) >> 2) * 8;

				// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
			case BC5:
				return ((width + 3) >> 2) * 16;

				// 3 component texture compression meant for mobile devices
			case ETC1:
				return (width >> 1);

				// 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
			case R16_UNORM:
				return sizeof(uint16_t) * width;

				// 32-bit unsigned integer format
			case R32_UINT:
				return sizeof(uint32_t) * width;

				// 32-bit float red/depth format
			case R32_FLOAT:
			case D32_FLOAT:
				return sizeof(float) * width;

				// A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
			case R16G16_SNORM:
				return sizeof(uint32_t) * width;

				// A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
			case R16G16_FLOAT:
				return sizeof(float) * width;

				// Unknown
			case UNKNOWN:
			case NUMBER_OF_FORMATS:
				return 0;

			default:
				return 0;
			}
		}

		[[nodiscard]] static inline uint32_t getNumberOfBytesPerSlice(Enum textureFormat, uint32_t width, uint32_t height)
		{
			switch ( textureFormat )
			{
				// 8-bit pixel format, all bits red
			case R8:
				return width * height;

				// 24-bit pixel format, 8 bits for red, green and blue
			case R8G8B8:
				return 3 * width * height;

				// 32-bit pixel format, 8 bits for red, green, blue and alpha
			case R8G8B8A8:
			case R8G8B8A8_SRGB:
			case B8G8R8A8:
				return 4 * width * height;

				// 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
			case R11G11B10F:
				return 4 * width * height;

				// 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
			case R16G16B16A16F:
				return 8 * width * height;

				// 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
			case R32G32B32A32F:
				return 16 * width * height;

				// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
			case BC1:
			case BC1_SRGB:
				return ((width + 3) >> 2) * ((height + 3) >> 2) * 8;

				// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			case BC2:
			case BC2_SRGB:
				return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

				// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			case BC3:
			case BC3_SRGB:
				return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

				// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
			case BC4:
				return ((width + 3) >> 2) * ((height + 3) >> 2) * 8;

				// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
			case BC5:
				return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

				// 3 component texture compression meant for mobile devices
			case ETC1:
				{
					const uint32_t numberOfBytesPerSlice = (width * height) >> 1;
					return (numberOfBytesPerSlice > 8) ? numberOfBytesPerSlice : 8;
				}

				// 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
			case R16_UNORM:
				return sizeof(uint16_t) * width * height;

				// 32-bit unsigned integer format
			case R32_UINT:
				return sizeof(uint32_t) * width * height;

				// 32-bit float depth format
			case R32_FLOAT:
			case D32_FLOAT:
				return sizeof(float) * width * height;

				// A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
			case R16G16_SNORM:
				return sizeof(uint32_t) * width * height;

				// A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
			case R16G16_FLOAT:
				return sizeof(float) * width * height;

				// Unknown
			case UNKNOWN:
			case NUMBER_OF_FORMATS:
				return 0;

			default:
				return 0;
			}
		}

	};

	struct TextureFlag final
	{
		enum Enum : uint32_t
		{
			UNORDERED_ACCESS = 1 << 0,	// This texture can be used for unordered access which is needed for compute shader read/write textures (when using Direct3D 11 a unordered access view (UAV) will be generated)
			SHADER_RESOURCE = 1 << 1,	// This texture can be used as shader resource (when using Direct3D 11 a shader resource view (SRV) will be generated)
			RENDER_TARGET = 1 << 2,	// This texture can be used as framebuffer object (FBO) attachment render target
			DATA_CONTAINS_MIPMAPS = 1 << 3,	// The user provided data containing mipmaps from 0-n down to 1x1 linearly in memory
			GENERATE_MIPMAPS = 1 << 4	// Allocate mipmap chain, automatically generate mipmaps if static data has been provided (avoid this if you can, will be ignored in case the "DATA_CONTAINS_MIPMAPS"-flag is set), for depth textures the mipmaps can only be allocated but not automatically be generated
		};
	};

	enum class TextureUsage
	{
		DEFAULT = 0,	// A resource that requires read and write access by the GPU. This is likely to be the most common usage choice.
		IMMUTABLE = 1,	// A resource that can only be read by the GPU. It cannot be written by the GPU, and cannot be accessed at all by the CPU. This type of resource must be initialized when it is created, since it cannot be changed after creation.
		DYNAMIC = 2,	// A resource that is accessible by both the GPU (read only) and the CPU (write only). A dynamic resource is a good choice for a resource that will be updated by the CPU at least once per frame. To update a dynamic resource, use a map method.
		STAGING = 3	// A resource that supports data transfer (copy) from the GPU to the CPU.
	};

	// "ID3D12Device::CreateCommittedResource method" documentation at https://msdn.microsoft.com/de-de/library/windows/desktop/dn899178%28v=vs.85%29.aspx
	struct OptimizedTextureClearValue final
	{
		union
		{
			float color[4];
			struct
			{
				float depth;
				uint8_t stencil;
			} DepthStencil;
		};
	};
} // namespace Rhi
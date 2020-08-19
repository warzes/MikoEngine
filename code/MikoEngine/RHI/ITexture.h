#pragma once

namespace Rhi
{
	class ITexture : public IResource
	{

		// Public static methods
	public:
		/**
		*  @brief
		*    Calculate the number of mipmaps for a 1D texture
		*
		*  @param[in] width
		*    Texture width
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width)
		{
			// If "std::log2()" wouldn't be supported, we could write the following: "return static_cast<uint32_t>(1 + std::floor(std::log(width) / 0.69314718055994529));"
			// -> log2(x) = log(x) / log(2)
			// -> log(2) = 0.69314718055994529
			return static_cast<uint32_t>(1 + std::floor(std::log2(width)));
		}

		/**
		*  @brief
		*    Calculate the number of mipmaps for a 2D texture
		*
		*  @param[in] width
		*    Texture width
		*  @param[in] height
		*    Texture height
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width, uint32_t height)
		{
			return getNumberOfMipmaps((width > height) ? width : height);
		}

		/**
		*  @brief
		*    Calculate the number of mipmaps for a 3D texture
		*
		*  @param[in] width
		*    Texture width
		*  @param[in] height
		*    Texture height
		*  @param[in] depth
		*    Texture depth
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width, uint32_t height, uint32_t depth)
		{
			return getNumberOfMipmaps(width, (height > depth) ? height : depth);
		}

		/**
		*  @brief
		*    Calculate the half size
		*
		*  @param[in] size
		*    Size
		*
		*  @return
		*    Half size, 1 as minimum
		*/
		[[nodiscard]] static inline uint32_t getHalfSize(uint32_t size)
		{
			size = (size >> 1);	// /= 2
			return (0u == size) ? 1u : size;
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width)
		{
			if ( 0u != mipmapIndex )
			{
				width = static_cast<uint32_t>(static_cast<float>(width) / std::exp2f(static_cast<float>(mipmapIndex)));
				if ( 0u == width )
				{
					width = 1u;
				}
			}
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*  @param[in,out] height
		*    Texture height
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width, uint32_t& height)
		{
			getMipmapSize(mipmapIndex, width);
			getMipmapSize(mipmapIndex, height);
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*  @param[in,out] height
		*    Texture height
		*  @param[in,out] depth
		*    Texture depth
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width, uint32_t& height, uint32_t& depth)
		{
			getMipmapSize(mipmapIndex, width);
			getMipmapSize(mipmapIndex, height);
			getMipmapSize(mipmapIndex, depth);
		}

		// Public methods
	public:
		inline virtual ~ITexture() override
		{
		}

		// Protected methods
	protected:
		inline ITexture(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		explicit ITexture(const ITexture& source) = delete;
		ITexture& operator =(const ITexture& source) = delete;

	};

	typedef SmartRefCount<ITexture> ITexturePtr;
} // namespace Rhi
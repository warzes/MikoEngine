#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract 2D array texture interface
*
*  @remarks
*    Common use cases for 2D texture arrays:
*    - The cascade 2D textures of shadow mapping
*    - The detail layer 2D textures (grass, stone etc.) of a terrain
*    - Decal 2D textures
*/
	class ITexture2DArray : public ITexture
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture2DArray() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTexture2DArrays;
#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the height of the texture
		*
		*  @return
		*    The height of the texture
		*/
		[[nodiscard]] inline uint32_t getHeight() const
		{
			return mHeight;
		}

		/**
		*  @brief
		*    Return the number of slices
		*
		*  @return
		*    The number of slices
		*/
		[[nodiscard]] inline uint32_t getNumberOfSlices() const
		{
			return mNumberOfSlices;
		}

		// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*/
		inline ITexture2DArray(IRhi& rhi, uint32_t width, uint32_t height, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_2D_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height),
			mNumberOfSlices(numberOfSlices)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTexture2DArrays;
			++rhi.getStatistics().currentNumberOfTexture2DArrays;
#endif
		}

		explicit ITexture2DArray(const ITexture2DArray& source) = delete;
		ITexture2DArray& operator =(const ITexture2DArray& source) = delete;

		// Private data
	private:
		uint32_t mWidth;			// The width of the texture
		uint32_t mHeight;			// The height of the texture
		uint32_t mNumberOfSlices;	// The number of slices

	};

	typedef SmartRefCount<ITexture2DArray> ITexture2DArrayPtr;
} // namespace Rhi
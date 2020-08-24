#pragma once

namespace Rhi
{
	/**
	*  @brief
	*    Abstract 1D array texture interface
	*
	*  @note
	*    - Common use case for 1D texture array: An 1D array texture is for example useful for storing multiple 1D Illuminating Engineering Society (IES) light profiles packed together in a single texture in order to avoid RHI state changes
	*/
	class ITexture1DArray : public ITexture
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture1DArray() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTexture1DArrays;
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
		*  @param[in] numberOfSlices
		*    The number of slices
		*/
		inline ITexture1DArray(IRhi& rhi, uint32_t width, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_1D_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mNumberOfSlices(numberOfSlices)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTexture1DArrays;
			++rhi.getStatistics().currentNumberOfTexture1DArrays;
#endif
		}

		explicit ITexture1DArray(const ITexture1DArray& source) = delete;
		ITexture1DArray& operator =(const ITexture1DArray& source) = delete;

		// Private data
	private:
		uint32_t mWidth;			// The width of the texture
		uint32_t mNumberOfSlices;	// The number of slices

	};

	typedef SmartRefCount<ITexture1DArray> ITexture1DArrayPtr;
} // namespace Rhi
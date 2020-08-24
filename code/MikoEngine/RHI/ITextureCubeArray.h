#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract cube texture array interface
*
*  @note
*    - Common use case for cube texture array: Multiple environment cube maps (created e.g. via environment probes)
*/
	class ITextureCubeArray : public ITexture
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureCubeArray() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTextureCubeArrays;
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
		inline ITextureCubeArray(IRhi& rhi, uint32_t width, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_CUBE_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mNumberOfSlices(numberOfSlices)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTextureCubeArrays;
			++rhi.getStatistics().currentNumberOfTextureCubeArrays;
#endif
		}

		explicit ITextureCubeArray(const ITextureCubeArray& source) = delete;
		ITextureCubeArray& operator =(const ITextureCubeArray& source) = delete;

		// Private data
	private:
		uint32_t mWidth;			// The width of the texture
		uint32_t mNumberOfSlices;	// The number of slices

	};

	typedef SmartRefCount<ITextureCubeArray> ITextureCubeArrayPtr;
} // namespace Rhi
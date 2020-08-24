#pragma once

namespace Rhi
{
	/**
	*  @brief
	*    Abstract 1D texture interface
	*
	*  @remarks
	*    Common use cases for 1D textures:
	*    - Illuminating Engineering Society (IES) light profile as 1D texture
	*    - 1D lens color map for screen space lens flares
	*    - 1D screen space ambient occlusion sample kernel map
	*    - 1D light gradient map for cel—shading (a style of cartoon rendering)
	*/
	class ITexture1D : public ITexture
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture1D() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTexture1Ds;
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
		*/
		inline ITexture1D(IRhi& rhi, uint32_t width RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_1D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTexture1Ds;
			++rhi.getStatistics().currentNumberOfTexture1Ds;
#endif
		}

		explicit ITexture1D(const ITexture1D& source) = delete;
		ITexture1D& operator =(const ITexture1D& source) = delete;

		// Private data
	private:
		uint32_t mWidth;	// The width of the texture

	};

	typedef SmartRefCount<ITexture1D> ITexture1DPtr;
} // namespace Rhi
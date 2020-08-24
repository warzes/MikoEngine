#pragma once

namespace Rhi
{
	/**
	*  @brief
	*    Abstract 2D texture interface
	*
	*  @note
	*    - Common use case for 2D texture: Pretty much every standard 2D texture
	*/
	class ITexture2D : public ITexture
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture2D() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTexture2Ds;
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
		*/
		inline ITexture2D(IRhi& rhi, uint32_t width, uint32_t height RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_2D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTexture2Ds;
			++rhi.getStatistics().currentNumberOfTexture2Ds;
#endif
		}

		explicit ITexture2D(const ITexture2D& source) = delete;
		ITexture2D& operator =(const ITexture2D& source) = delete;

		// Private data
	private:
		uint32_t mWidth;	// The width of the texture
		uint32_t mHeight;	// The height of the texture

	};

	typedef SmartRefCount<ITexture2D> ITexture2DPtr;
} // namespace Rhi
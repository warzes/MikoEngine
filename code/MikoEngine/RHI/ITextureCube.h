#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract cube texture interface
*
*  @note
*    - Common use case for cube texture: Environment cube maps
*/
	class ITextureCube : public ITexture
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureCube() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTextureCubes;
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
		inline ITextureCube(IRhi& rhi, uint32_t width RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_CUBE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTextureCubes;
			++rhi.getStatistics().currentNumberOfTextureCubes;
#endif
		}

		explicit ITextureCube(const ITextureCube& source) = delete;
		ITextureCube& operator =(const ITextureCube& source) = delete;

		// Private data
	private:
		uint32_t mWidth;	// The width of the texture

	};

	typedef SmartRefCount<ITextureCube> ITextureCubePtr;
} // namespace Rhi
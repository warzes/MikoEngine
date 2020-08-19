#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract 3D texture interface
*
*  @remarks
*    Common use cases for 3D textures:
*    - 3D color correction lookup table (LUT)
*    - Volume rendering (medical area and nowadays in games as well)
*    - Light clusters 3D map
*/
	class ITexture3D : public ITexture
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture3D() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTexture3Ds;
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
		*    Return the depth of the texture
		*
		*  @return
		*    The depth of the texture
		*/
		[[nodiscard]] inline uint32_t getDepth() const
		{
			return mDepth;
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
		*  @param[in] depth
		*    The depth of the texture
		*/
		inline ITexture3D(IRhi& rhi, uint32_t width, uint32_t height, uint32_t depth RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_3D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height),
			mDepth(depth)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTexture3Ds;
			++rhi.getStatistics().currentNumberOfTexture3Ds;
#endif
		}

		explicit ITexture3D(const ITexture3D& source) = delete;
		ITexture3D& operator =(const ITexture3D& source) = delete;

		// Private data
	private:
		uint32_t mWidth;	// The width of the texture
		uint32_t mHeight;	// The height of the texture
		uint32_t mDepth;	// The depth of the texture

	};

	typedef SmartRefCount<ITexture3D> ITexture3DPtr;
} // namespace Rhi
#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract texture manager interface
*
*  @remarks
*    The texture manager is responsible for managing fine granular instances of
*    - 1D texture ("Rhi::ITexture1D")
*    - 1D texture array ("Rhi::ITexture1DArray")
*    - 2D texture ("Rhi::ITexture2D")
*    - 2D texture array ("Rhi::ITexture2DArray")
*    - 3D texture ("Rhi::ITexture3D")
*    - Cube texture ("Rhi::ITextureCube")
*    - Cube texture array ("Rhi::ITextureCubeArray")
*/
	class ITextureManager : public RefCount<ITextureManager>
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureManager() override
		{
		}

		/**
		*  @brief
		*    Return the owner RHI instance
		*
		*  @return
		*    The owner RHI instance, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IRhi& getRhi() const
		{
			return mRhi;
		}

		// Public virtual Rhi::ITextureManager methods
	public:
		//[-------------------------------------------------------]
		//[ Resource creation                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a 1D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 1D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The following texture data layout is expected: Mip0, Mip1, Mip2, Mip3 ...
		*/
		[[nodiscard]] virtual ITexture1D* createTexture1D(uint32_t width, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 1D array texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 1D array texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture array data consists of a sequence of texture slices. Each the texture slice data of a single texture slice has to
		*    be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOf1DTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITexture1DArray* createTexture1DArray(uint32_t width, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 2D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture data format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*  @param[in] optimizedTextureClearValue
		*    Optional optimized texture clear value
		*
		*  @return
		*    The created 2D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The following texture data layout is expected: Mip0, Mip1, Mip2, Mip3 ...
		*/
		[[nodiscard]] virtual ITexture2D* createTexture2D(uint32_t width, uint32_t height, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT, uint8_t numberOfMultisamples = 1, const OptimizedTextureClearValue* optimizedTextureClearValue = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 2D array texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 2D array texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture array data consists of a sequence of texture slices. Each the texture slice data of a single texture slice has to
		*    be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5 (= positive X, negative X, positive Y, negative Y, positive Z, negative Z)
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOf2DTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITexture2DArray* createTexture2DArray(uint32_t width, uint32_t height, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 3D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] depth
		*    Texture depth, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 3D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5 (= positive X, negative X, positive Y, negative Y, positive Z, negative Z)
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*/
		[[nodiscard]] virtual ITexture3D* createTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a cube texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created cube texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Face0, Face1, Face2, Face3, Face4, Face5
		*    - Mip1: Face0, Face1, Face2, Face3, Face4, Face5
		*    (DDS-texture layout is using face-major order)
		*/
		[[nodiscard]] virtual ITextureCube* createTextureCube(uint32_t width, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a cube texture array instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created cube texture array instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Face0, Face1, Face2, Face3, Face4, Face5
		*    - Mip1: Face0, Face1, Face2, Face3, Face4, Face5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfCubeTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITextureCubeArray* createTextureCubeArray(uint32_t width, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ITextureManager(IRhi& rhi) :
			mRhi(rhi)
		{
		}

		explicit ITextureManager(const ITextureManager& source) = delete;
		ITextureManager& operator =(const ITextureManager& source) = delete;

		// Private data
	private:
		IRhi& mRhi;	// The owner RHI instance

	};

	typedef SmartRefCount<ITextureManager> ITextureManagerPtr;
} // namespace Rhi
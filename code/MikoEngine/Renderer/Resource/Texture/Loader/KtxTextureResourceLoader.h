#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Texture/Loader/ITextureResourceLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class KtxTextureResourceLoader final : public ITextureResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TextureResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("ktx");


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] virtual bool onDeserialization(IFile& file) override;

		inline virtual void onProcessing() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Protected Renderer::ITextureResourceLoader methods    ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual Rhi::ITexture* createRhiTexture() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline KtxTextureResourceLoader(IResourceManager& resourceManager, IRenderer& renderer) :
			ITextureResourceLoader(resourceManager, renderer),
			mWidth(0),
			mHeight(0),
			mTextureFormat(0),
			mCubeMap(false),
			mDataContainsMipmaps(false),
			mNumberOfImageDataBytes(0),
			mNumberOfUsedImageDataBytes(0),
			mImageData(nullptr)
		{
			// Nothing here
		}

		inline virtual ~KtxTextureResourceLoader() override
		{
			delete [] mImageData;
		}

		explicit KtxTextureResourceLoader(const KtxTextureResourceLoader&) = delete;
		KtxTextureResourceLoader& operator=(const KtxTextureResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Temporary data
		uint32_t mWidth;
		uint32_t mHeight;
		uint8_t  mTextureFormat;	// "Rhi::TextureFormat", don't want to include the header in here
		bool	 mCubeMap;
		bool	 mDataContainsMipmaps;

		// Temporary image data
		uint32_t mNumberOfImageDataBytes;
		uint32_t mNumberOfUsedImageDataBytes;
		uint8_t* mImageData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

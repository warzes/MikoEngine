#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Texture/Loader/ITextureResourceLoader.h"
#include "Renderer/Core/File/MemoryFile.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class Lz4DdsTextureResourceLoader final : public ITextureResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TextureResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID		 = STRING_ID("lz4dds");
		static constexpr uint32_t FORMAT_TYPE	 = TYPE_ID;
		static constexpr uint32_t FORMAT_VERSION = 1;


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] inline virtual bool onDeserialization(IFile& file) override
		{
			// Tell the memory mapped file about the LZ4 compressed data
			return mMemoryFile.loadLz4CompressedDataFromFile(FORMAT_TYPE, FORMAT_VERSION, file);
		}

		virtual void onProcessing() override;


	//[-------------------------------------------------------]
	//[ Protected Renderer::ITextureResourceLoader methods    ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual Rhi::ITexture* createRhiTexture() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline Lz4DdsTextureResourceLoader(IResourceManager& resourceManager, IRenderer& renderer) :
			ITextureResourceLoader(resourceManager, renderer),
			mWidth(0),
			mHeight(0),
			mDepth(0),
			mNumberOfSlices(0),
			mTextureFormat(0),
			mDataContainsMipmaps(false),
			mNumberOfImageDataBytes(0),
			mNumberOfUsedImageDataBytes(0),
			mImageData(nullptr)
		{
			// Nothing here
		}

		inline virtual ~Lz4DdsTextureResourceLoader() override
		{
			delete [] mImageData;
		}

		explicit Lz4DdsTextureResourceLoader(const Lz4DdsTextureResourceLoader&) = delete;
		Lz4DdsTextureResourceLoader& operator=(const Lz4DdsTextureResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Temporary data
		MemoryFile mMemoryFile;
		uint32_t   mWidth;
		uint32_t   mHeight;
		uint32_t   mDepth;
		uint32_t   mNumberOfSlices;
		uint8_t    mTextureFormat;	// "Rhi::TextureFormat", don't want to include the header in here
		bool	   mDataContainsMipmaps;

		// Temporary image data
		uint32_t mNumberOfImageDataBytes;
		uint32_t mNumberOfUsedImageDataBytes;
		uint8_t* mImageData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

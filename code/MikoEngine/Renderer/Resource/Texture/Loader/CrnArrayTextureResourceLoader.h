#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Texture/Loader/CrnTextureResourceLoader.h"
#include "Core/File/MemoryFile.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CrnArrayTextureResourceLoader final : public CrnTextureResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TextureResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = SE_STRING_ID("crn_array");


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] virtual bool onDeserialization(IFile& file) override;
		virtual void onProcessing() override;


	//[-------------------------------------------------------]
	//[ Protected Renderer::ITextureResourceLoader methods    ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual Rhi::ITexture* createRhiTexture() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline CrnArrayTextureResourceLoader(IResourceManager& resourceManager, IRenderer& renderer) :
			CrnTextureResourceLoader(resourceManager, renderer),
			mNumberOfSlices(0)
		{
			// Nothing here
		}

		inline virtual ~CrnArrayTextureResourceLoader() override
		{
			// Nothing here
		}

		explicit CrnArrayTextureResourceLoader(const CrnArrayTextureResourceLoader&) = delete;
		CrnArrayTextureResourceLoader& operator=(const CrnArrayTextureResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct SliceFileMetadata final
		{
			const Asset& asset;
			uint32_t	 offset;
			uint32_t	 numberOfBytes;

			inline SliceFileMetadata(const Asset& _asset, uint32_t _offset, uint32_t _numberOfBytes) :
				asset(_asset),
				offset(_offset),
				numberOfBytes(_numberOfBytes)
			{};
			SliceFileMetadata& operator =(const SliceFileMetadata& sliceFile) = delete;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Temporary data
		uint32_t mNumberOfSlices;

		// Temporary file data
		MemoryFile					   mMemoryFile;
		std::vector<AssetId>		   mAssetIds;
		std::vector<SliceFileMetadata> mSliceFileMetadata;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

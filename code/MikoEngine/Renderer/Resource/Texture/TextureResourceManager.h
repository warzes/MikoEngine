#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/ResourceManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rhi
{
	class ITexture;
}
namespace Renderer
{
	class TextureResource;
	class IRenderer;
	class ITextureResourceLoader;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t			 TextureResourceId;	// POD texture resource identifier
	typedef StringId			 AssetId;			// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef std::vector<AssetId> AssetIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Texture resource manager class
	*/
	class TextureResourceManager final : public ResourceManager<TextureResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the asset IDs of automatically generated dynamic default texture assets
		*
		*  @param[out] assetIds
		*    Receives the asset IDs of automatically generated dynamic default texture assets, the list is not cleared before new entries are added
		*
		*  @remarks
		*    The texture manager automatically generates some dynamic default texture assets one can reference e.g. inside material blueprint resources:
		*    - "Engine/Texture/DynamicByCode/WhiteMap1D"
		*    - "Engine/Texture/DynamicByCode/WhiteMap2D"
		*    - "Engine/Texture/DynamicByCode/WhiteMapCube"
		*    - "Engine/Texture/DynamicByCode/WhiteMap3D"
		*    - "Engine/Texture/DynamicByCode/BlackMap1D"
		*    - "Engine/Texture/DynamicByCode/BlackMap2D"
		*    - "Engine/Texture/DynamicByCode/BlackMapCube"
		*    - "Engine/Texture/DynamicByCode/BlackMap3D"
		*    - "Engine/Texture/DynamicByCode/IdentityAlbedoMap2D"
		*    - "Engine/Texture/DynamicByCode/IdentityAlphaMap2D"
		*    - "Engine/Texture/DynamicByCode/IdentityNormalMap2D"
		*    - "Engine/Texture/DynamicByCode/IdentityRoughnessMap2D"
		*    - "Engine/Texture/DynamicByCode/DielectricMetallicMap2D"
		*    - "Engine/Texture/DynamicByCode/IdentityEmissiveMap2D"
		*    - "Engine/Texture/DynamicByCode/Identity_argb_nxa2D"
		*    - "Engine/Texture/DynamicByCode/Identity_hr_rg_mb_nya2D"
		*/
		static void getDefaultTextureAssetIds(AssetIds& assetIds);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline uint8_t getNumberOfTopMipmapsToRemove() const
		{
			return mNumberOfTopMipmapsToRemove;
		}

		void setNumberOfTopMipmapsToRemove(uint8_t numberOfTopMipmapsToRemove);
		[[nodiscard]] TextureResource* getTextureResourceByAssetId(AssetId assetId) const;		// Considered to be inefficient, avoid method whenever possible
		[[nodiscard]] TextureResourceId getTextureResourceIdByAssetId(AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void loadTextureResourceByAssetId(AssetId assetId, AssetId fallbackTextureAssetId, TextureResourceId& textureResourceId, IResourceListener* resourceListener = nullptr, bool rgbHardwareGammaCorrection = false, bool reload = false, ResourceLoaderTypeId resourceLoaderTypeId = GetInvalid<ResourceLoaderTypeId>());	// Asynchronous
		TextureResourceId createTextureResourceByAssetId(AssetId assetId, Rhi::ITexture& texture, bool rgbHardwareGammaCorrection = false);	// Texture resource is not allowed to exist, yet
		void destroyTextureResource(TextureResourceId textureResourceId);
		void setInvalidResourceId(TextureResourceId& textureResourceId, IResourceListener& resourceListener) const;


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual uint32_t getNumberOfResources() const override;
		[[nodiscard]] virtual IResource& getResourceByIndex(uint32_t index) const override;
		[[nodiscard]] virtual IResource& getResourceByResourceId(ResourceId resourceId) const override;
		[[nodiscard]] virtual IResource* tryGetResourceByResourceId(ResourceId resourceId) const override;
		virtual void reloadResourceByAssetId(AssetId assetId) override;

		inline virtual void update() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] virtual IResourceLoader* createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit TextureResourceManager(IRenderer& renderer);
		virtual ~TextureResourceManager() override;
		explicit TextureResourceManager(const TextureResourceManager&) = delete;
		TextureResourceManager& operator=(const TextureResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint8_t mNumberOfTopMipmapsToRemove;	// The number of top mipmaps to remove while loading textures for efficient texture quality reduction. By setting this to e.g. two a 4096x4096 texture will become 1024x1024.

		// Internal resource manager implementation
		ResourceManagerTemplate<TextureResource, ITextureResourceLoader, TextureResourceId, 2048>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

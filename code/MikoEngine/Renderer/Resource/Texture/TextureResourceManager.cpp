#include "stdafx.h"
#include "Renderer/Resource/Texture/TextureResourceManager.h"
#include "Renderer/Resource/Texture/TextureResource.h"
#include "Renderer/Resource/Texture/Loader/CrnTextureResourceLoader.h"
#include "Renderer/Resource/Texture/Loader/CrnArrayTextureResourceLoader.h"
#include "Renderer/Resource/Texture/Loader/Lz4DdsTextureResourceLoader.h"
#include "Renderer/Resource/Texture/Loader/KtxTextureResourceLoader.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"
#ifdef RENDERER_OPENVR
	#include "Renderer/Vr/OpenVR/Loader/OpenVRTextureResourceLoader.h"
#endif


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		void createDefaultDynamicTextureAssets(Renderer::IRenderer& renderer, Renderer::TextureResourceManager& textureResourceManager)
		{
			Rhi::ITextureManager& textureManager = renderer.getTextureManager();

			// White RGBA texture
			const uint8_t whiteRgbaData[] = {
				255, 255, 255, 255,	// Face 0
				255, 255, 255, 255,	// Face 1
				255, 255, 255, 255,	// Face 2
				255, 255, 255, 255,	// Face 3
				255, 255, 255, 255,	// Face 4
				255, 255, 255, 255	// Face 5
			};
			Rhi::ITexturePtr whiteRgba1DTexturePtr(textureManager.createTexture1D(1, Rhi::TextureFormat::R8G8B8A8, whiteRgbaData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("White RGBA")));
			Rhi::ITexturePtr whiteRgba2DTexturePtr(textureManager.createTexture2D(1, 1, Rhi::TextureFormat::R8G8B8A8, whiteRgbaData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("White RGBA")));
			Rhi::ITexturePtr whiteRgbaCubeTexturePtr(textureManager.createTextureCube(1, Rhi::TextureFormat::R8G8B8A8, whiteRgbaData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("White RGBA")));
			Rhi::ITexturePtr whiteRgba3DTexturePtr(textureManager.createTexture3D(1, 1, 1, Rhi::TextureFormat::R8G8B8A8, whiteRgbaData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("White RGBA")));

			// Normal map identity texture
			const uint8_t normalMapIdentityData[] = { 128, 128, 255, 255 };
			Rhi::ITexturePtr normalMapIdentity2DTexturePtr(textureManager.createTexture2D(1, 1, Rhi::TextureFormat::R8G8B8A8, normalMapIdentityData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Normal map identity")));

			// White alpha texture
			const uint8_t whiteAData[] = { 255 };
			Rhi::ITexturePtr whiteA2DTexturePtr(textureManager.createTexture2D(1, 1, Rhi::TextureFormat::R8, whiteAData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("White alpha")));

			// Black RGBA texture
			const uint8_t blackRgbaData[] = {
				0, 0, 0, 0,	// Face 0
				0, 0, 0, 0,	// Face 1
				0, 0, 0, 0,	// Face 2
				0, 0, 0, 0,	// Face 3
				0, 0, 0, 0,	// Face 4
				0, 0, 0, 0	// Face 5
			};
			Rhi::ITexturePtr blackRgba1DTexturePtr(textureManager.createTexture1D(1, Rhi::TextureFormat::R8G8B8A8, blackRgbaData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("Black RGBA")));
			Rhi::ITexturePtr blackRgba2DTexturePtr(textureManager.createTexture2D(1, 1, Rhi::TextureFormat::R8G8B8A8, blackRgbaData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Black RGBA")));
			Rhi::ITexturePtr blackRgbaCubeTexturePtr(textureManager.createTextureCube(1, Rhi::TextureFormat::R8G8B8A8, blackRgbaData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("Black RGBA")));
			Rhi::ITexturePtr blackRgba3DTexturePtr(textureManager.createTexture3D(1, 1, 1, Rhi::TextureFormat::R8G8B8A8, blackRgbaData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("Black RGBA")));

			// Black alpha texture
			const uint8_t blackAData[] = { 0 };
			Rhi::ITexturePtr blackA2DTexturePtr(textureManager.createTexture2D(1, 1, Rhi::TextureFormat::R8, blackAData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Black alpha")));

			// "_argb_nxa"-texture
			// -> Must be white so e.g. albedo color can be multiplied in
			const uint8_t _argb_nxaIdentityData[] = { 255, 255, 255, 128 };
			Rhi::ITexturePtr _argb_nxaIdentity2DTexturePtr(textureManager.createTexture2D(1, 1, Rhi::TextureFormat::R8G8B8A8, _argb_nxaIdentityData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("_argb_nxa identity")));

			// "_hr_rg_mb_nya"-texture
			const uint8_t _hr_rg_mb_nyaIdentityData[] = { 0, 255, 0, 128 };
			Rhi::ITexturePtr _hr_rg_mb_nyaIdentity2DTexturePtr(textureManager.createTexture2D(1, 1, Rhi::TextureFormat::R8G8B8A8, _hr_rg_mb_nyaIdentityData, Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("_hr_rg_mb_nya identity")));

			// Define helper macro
			#define CREATE_TEXTURE(name, texturePtr) textureResourceManager.createTextureResourceByAssetId(ASSET_ID("Engine/Texture/DynamicByCode/"#name), *texturePtr);

			// Create default dynamic texture assets
			CREATE_TEXTURE(WhiteMap1D,				whiteRgba1DTexturePtr)
			CREATE_TEXTURE(WhiteMap2D,				whiteRgba2DTexturePtr)
			CREATE_TEXTURE(WhiteMapCube,			whiteRgbaCubeTexturePtr)
			CREATE_TEXTURE(WhiteMap3D,				whiteRgba3DTexturePtr)
			CREATE_TEXTURE(BlackMap1D,				blackRgba1DTexturePtr)
			CREATE_TEXTURE(BlackMap2D,				blackRgba2DTexturePtr)
			CREATE_TEXTURE(BlackMapCube,			blackRgbaCubeTexturePtr)
			CREATE_TEXTURE(BlackMap3D,				blackRgba3DTexturePtr)
			CREATE_TEXTURE(IdentityAlbedoMap2D,		whiteRgba2DTexturePtr)	// Must be white so e.g. albedo color can be multiplied in
			CREATE_TEXTURE(IdentityAlphaMap2D,		whiteA2DTexturePtr)
			CREATE_TEXTURE(IdentityNormalMap2D,		normalMapIdentity2DTexturePtr)
			CREATE_TEXTURE(IdentityRoughnessMap2D,	whiteA2DTexturePtr)
			CREATE_TEXTURE(DielectricMetallicMap2D,	blackA2DTexturePtr)
			CREATE_TEXTURE(IdentityEmissiveMap2D,	blackRgba2DTexturePtr)
			CREATE_TEXTURE(Identity_argb_nxa2D,		_argb_nxaIdentity2DTexturePtr)
			CREATE_TEXTURE(Identity_hr_rg_mb_nya2D,	_hr_rg_mb_nyaIdentity2DTexturePtr)

			// Undefine helper macro
			#undef CREATE_TEXTURE
		}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void TextureResourceManager::getDefaultTextureAssetIds(AssetIds& assetIds)
	{
		// Define helper macros
		#define ADD_ASSET_ID(name) assetIds.push_back(ASSET_ID(name));

		// Add asset IDs
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/WhiteMap1D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/WhiteMap2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/WhiteMapCube")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/WhiteMap3D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/BlackMap1D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/BlackMap2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/BlackMapCube")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/BlackMap3D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/IdentityAlbedoMap2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/IdentityAlphaMap2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/IdentityNormalMap2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/IdentityRoughnessMap2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/DielectricMetallicMap2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/IdentityEmissiveMap2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/Identity_argb_nxa2D")
		ADD_ASSET_ID("Engine/Texture/DynamicByCode/Identity_hr_rg_mb_nya2D")

		// Undefine helper macros
		#undef ADD_ASSET_ID
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void TextureResourceManager::setNumberOfTopMipmapsToRemove(uint8_t numberOfTopMipmapsToRemove)
	{
		if (mNumberOfTopMipmapsToRemove != numberOfTopMipmapsToRemove)
		{
			mNumberOfTopMipmapsToRemove = numberOfTopMipmapsToRemove;

			// Update the already loaded textures, but really only textures loaded from texture assets and not e.g. textures dynamically created during runtime
			const AssetManager& assetManager = mInternalResourceManager->getRenderer().getAssetManager();
			const uint32_t numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
			for (uint32_t i = 0; i < numberOfElements; ++i)
			{
				const TextureResource& textureResource = mInternalResourceManager->getResources().getElementByIndex(i);
				const AssetId assetId = textureResource.getAssetId();
				if (nullptr != assetManager.tryGetAssetByAssetId(assetId) && textureResource.getLoadingState() == Renderer::IResource::LoadingState::LOADED)
				{
					TextureResourceId textureResourceId = GetInvalid<TextureResourceId>();
					loadTextureResourceByAssetId(assetId, GetInvalid<AssetId>(), textureResourceId, nullptr, textureResource.isRgbHardwareGammaCorrection(), true);
				}
			}
		}
	}

	TextureResource* TextureResourceManager::getTextureResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	TextureResourceId TextureResourceManager::getTextureResourceIdByAssetId(AssetId assetId) const
	{
		const TextureResource* textureResource = getTextureResourceByAssetId(assetId);
		return (nullptr != textureResource) ? textureResource->getId() : GetInvalid<TextureResourceId>();
	}

	void TextureResourceManager::loadTextureResourceByAssetId(AssetId assetId, AssetId fallbackTextureAssetId, TextureResourceId& textureResourceId, IResourceListener* resourceListener, bool rgbHardwareGammaCorrection, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)
	{
		// Check whether or not the texture resource already exists
		TextureResource* textureResource = getTextureResourceByAssetId(assetId);

		// Create the resource instance
		// -> In case the texture asset ID is unknown it might be a runtime dynamic created texture which will be created by someone later one
		// -> Please note that the fallback texture asset ID is intentionally only used if the texture asset ID is valid, it's a fallback as long as the real texture data has not been loaded yet
		const IRenderer& renderer = mInternalResourceManager->getRenderer();
		const Asset* asset = renderer.getAssetManager().tryGetAssetByAssetId(assetId);
		// RHI_ASSERT(nullptr != asset, "Unknown asset ID")	// For texture assets there's no assert by intent since it's not unusual that e.g. referenced compositor texture assets get created later on
		bool load = (reload && nullptr != asset);
		if (nullptr == textureResource && nullptr != asset)
		{
			textureResource = &mInternalResourceManager->getResources().addElement();
			textureResource->setResourceManager(this);
			textureResource->setAssetId(assetId);
			textureResource->setResourceLoaderTypeId(resourceLoaderTypeId);
			textureResource->mRgbHardwareGammaCorrection = rgbHardwareGammaCorrection;
			load = true;
		}

		// Before connecting a resource listener, ensure we set the output resource ID at once so it can already directly be used inside the resource listener
		if (nullptr != textureResource)
		{
			textureResourceId = textureResource->getId();
			if (nullptr != resourceListener)
			{
				textureResource->connectResourceListener(*resourceListener);
			}
		}
		else
		{
			textureResourceId = GetInvalid<TextureResourceId>();
		}

		// Load the resource, if required
		if (load)
		{
			// Prepare the resource loader
			if (IsInvalid(resourceLoaderTypeId))
			{
				// The totally primitive texture resource loader type detection is sufficient for now
				const char* filenameExtension = strrchr(&asset->virtualFilename[0], '.');
				if (nullptr != filenameExtension)
				{
					resourceLoaderTypeId = StringId(filenameExtension + 1);
				}
				else
				{
					// Error!
					RHI_ASSERT(false, "We should never ever be able to be in here, it's the renderer toolkit responsible to ensure the renderer only works with sane data")
				}
			}
			if (IsValid(resourceLoaderTypeId))
			{
				// Commit resource streamer asset load request
				renderer.getResourceStreamer().commitLoadRequest(ResourceStreamer::LoadRequest(*asset, resourceLoaderTypeId, reload, *this, textureResourceId));

				// Since it might take a moment to load the texture resource, we'll use a fallback placeholder RHI texture resource so we don't have to wait until the real thing is there
				// -> In case there's already a RHI texture, keep that as long as possible (for example there might be a change in the number of top mipmaps to remove)
				if (nullptr == textureResource->mTexture)
				{
					if (IsValid(fallbackTextureAssetId))
					{
						const TextureResource* fallbackTextureResource = getTextureResourceByAssetId(fallbackTextureAssetId);
						if (nullptr != fallbackTextureResource)
						{
							textureResource->mTexture = fallbackTextureResource->getTexturePtr();
							textureResource->setLoadingState(IResource::LoadingState::LOADED);
						}
						else
						{
							// Error! 
							RHI_ASSERT(false, "Fallback texture asset ID not found")
						}
					}
					else
					{
						// Hiccups / lags warning
						RHI_ASSERT(false, "There should always be a fallback texture asset ID (better be safe than sorry)")
					}
				}
			}
			else
			{
				// Error!
				RHI_ASSERT(false, "We should never ever be able to be in here, it's the renderer toolkit responsible to ensure the renderer only works with sane data")
			}
		}
	}

	TextureResourceId TextureResourceManager::createTextureResourceByAssetId(AssetId assetId, Rhi::ITexture& texture, bool rgbHardwareGammaCorrection)
	{
		// Sanity check
		RHI_ASSERT(nullptr == getTextureResourceByAssetId(assetId), "The texture resource isn't allowed to exist, yet")

		// Create the texture resource instance
		TextureResource& textureResource = mInternalResourceManager->getResources().addElement();
		textureResource.setResourceManager(this);
		textureResource.setAssetId(assetId);
		textureResource.mRgbHardwareGammaCorrection = rgbHardwareGammaCorrection;	// TODO(co) We might need to extend "Rhi::ITexture" so we can readback the texture format
		textureResource.mTexture = &texture;

		// Done
		setResourceLoadingState(textureResource, IResource::LoadingState::LOADED);
		return textureResource.getId();
	}

	void TextureResourceManager::destroyTextureResource(TextureResourceId textureResourceId)
	{
		mInternalResourceManager->getResources().removeElement(textureResourceId);
	}

	void TextureResourceManager::setInvalidResourceId(TextureResourceId& textureResourceId, IResourceListener& resourceListener) const
	{
		TextureResource* textureResource = tryGetById(textureResourceId);
		if (nullptr != textureResource)
		{
			textureResource->disconnectResourceListener(resourceListener);
		}
		SetInvalid(textureResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t TextureResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& TextureResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& TextureResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* TextureResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void TextureResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		// TODO(co) Experimental implementation (take care of resource cleanup etc.)
		const uint32_t numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (uint32_t i = 0; i < numberOfElements; ++i)
		{
			const TextureResource& textureResource = mInternalResourceManager->getResources().getElementByIndex(i);
			if (textureResource.getAssetId() == assetId)
			{
				TextureResourceId textureResourceId = GetInvalid<TextureResourceId>();
				loadTextureResourceByAssetId(assetId, GetInvalid<AssetId>(), textureResourceId, nullptr, textureResource.isRgbHardwareGammaCorrection(), true, textureResource.getResourceLoaderTypeId());
				break;
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* TextureResourceManager::createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId)
	{
		switch (resourceLoaderTypeId)
		{
			case CrnTextureResourceLoader::TYPE_ID:
				return new CrnTextureResourceLoader(*this, mInternalResourceManager->getRenderer());
			case CrnArrayTextureResourceLoader::TYPE_ID:
				return new CrnArrayTextureResourceLoader(*this, mInternalResourceManager->getRenderer());

			case Lz4DdsTextureResourceLoader::TYPE_ID:
				return new Lz4DdsTextureResourceLoader(*this, mInternalResourceManager->getRenderer());

			case KtxTextureResourceLoader::TYPE_ID:
				return new KtxTextureResourceLoader(*this, mInternalResourceManager->getRenderer());

			#ifdef RENDERER_OPENVR
				case OpenVRTextureResourceLoader::TYPE_ID:
					return new OpenVRTextureResourceLoader(*this, mInternalResourceManager->getRenderer());
			#endif

			default:
				// TODO(co) Error handling
				RHI_ASSERT(false, "Invalid resource loader type ID")
				return nullptr;
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	TextureResourceManager::TextureResourceManager(IRenderer& renderer) :
		mNumberOfTopMipmapsToRemove(0)
	{
		mInternalResourceManager = new ResourceManagerTemplate<TextureResource, ITextureResourceLoader, TextureResourceId, 2048>(renderer, *this);
		::detail::createDefaultDynamicTextureAssets(renderer, *this);
	}

	TextureResourceManager::~TextureResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

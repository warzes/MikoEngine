#include "stdafx.h"
#if RENDERER_OPENVR
#include "Renderer/Vr/OpenVR/Loader/OpenVRTextureResourceLoader.h"
#include "Renderer/Resource/Texture/TextureResource.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Context.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(5204)	// warning C5204: 'crnlib::task_pool::executable_task': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
	#include <OpenVR/openvr.h>
SE_PRAGMA_WARNING_POP

#include <chrono>
#include <thread>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void OpenVRTextureResourceLoader::onProcessing()
	{
		// Load the render model texture
		vr::IVRRenderModels* vrRenderModels = vr::VRRenderModels();
		vr::EVRRenderModelError vrRenderModelError = vr::VRRenderModelError_Loading;
		const vr::TextureID_t albedoTextureId = static_cast<vr::TextureID_t>(std::atoi(getAsset().virtualFilename));
		while (vrRenderModelError == vr::VRRenderModelError_Loading)
		{
			vrRenderModelError = vrRenderModels->LoadTexture_Async(albedoTextureId, &mVrRenderModelTextureMap);
			if (vrRenderModelError == vr::VRRenderModelError_Loading)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(1ms);
			}
		}
		if (vr::VRRenderModelError_None != vrRenderModelError)
		{
			RHI_LOG(CRITICAL, "The renderer was unable to load OpenVR albedo texture %d: %s", albedoTextureId, vrRenderModels->GetRenderModelErrorNameFromEnum(vrRenderModelError))
			return;
		}

		{ // The "_argb_nxa" texture channel packing stores the x channel of a normal map inside the alpha channel, set identity normal map x value
			uint8_t* rubTextureMapData = const_cast<uint8_t*>(mVrRenderModelTextureMap->rubTextureMapData);	// Evil const-cast since I don't want to copy the data
			const uint8_t* rubTextureMapDataEnd = rubTextureMapData + mVrRenderModelTextureMap->unWidth * mVrRenderModelTextureMap->unHeight * 4;
			for (; rubTextureMapData < rubTextureMapDataEnd; rubTextureMapData += 4)
			{
				rubTextureMapData[3] = 128;
			}
		}

		// Can we create the RHI resource asynchronous as well?
		if (mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			mTexture = createRhiTexture();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected Renderer::ITextureResourceLoader methods    ]
	//[-------------------------------------------------------]
	Rhi::ITexture* OpenVRTextureResourceLoader::createRhiTexture()
	{
		Rhi::ITexture2D* texture2D = nullptr;
		if (nullptr != mVrRenderModelTextureMap)
		{
			// Create the RHI texture instance
			const bool rgbHardwareGammaCorrection = true;	// TODO(co) It must be possible to set the property name from the outside: Ask the material blueprint whether or not hardware gamma correction should be used
			texture2D = mRenderer.getTextureManager().createTexture2D(mVrRenderModelTextureMap->unWidth, mVrRenderModelTextureMap->unHeight, rgbHardwareGammaCorrection ? Rhi::TextureFormat::R8G8B8A8_SRGB : Rhi::TextureFormat::R8G8B8A8, static_cast<const void*>(mVrRenderModelTextureMap->rubTextureMapData), Rhi::TextureFlag::GENERATE_MIPMAPS | Rhi::TextureFlag::SHADER_RESOURCE, Rhi::TextureUsage::DEFAULT, 1, nullptr RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename));

			// Free the render model texture
			vr::VRRenderModels()->FreeTexture(mVrRenderModelTextureMap);
			mVrRenderModelTextureMap = nullptr;
		}

		// Done
		return texture2D;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#endif // RENDERER_OPENVR

#include "stdafx.h"
#include "Renderer/Core/Renderer/FramebufferManager.h"
#include "Renderer/Core/Renderer/RenderPassManager.h"
#include "Renderer/Core/Renderer/RenderTargetTextureManager.h"
#include "Renderer/IRenderer.h"

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
		[[nodiscard]] inline bool orderFramebufferElementByFramebufferSignatureId(const Renderer::FramebufferManager::FramebufferElement& left, const Renderer::FramebufferManager::FramebufferElement& right)
		{
			return (left.framebufferSignature.getFramebufferSignatureId() < right.framebufferSignature.getFramebufferSignatureId());
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
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void FramebufferManager::clear()
	{
		clearRhiResources();
		mSortedFramebufferVector.clear();
		mCompositorFramebufferIdToFramebufferSignatureId.clear();
	}

	void FramebufferManager::clearRhiResources()
	{
		for (FramebufferElement& framebufferElement : mSortedFramebufferVector)
		{
			if (nullptr != framebufferElement.framebuffer)
			{
				framebufferElement.framebuffer->ReleaseReference();
				framebufferElement.framebuffer = nullptr;
			}
		}
	}

	void FramebufferManager::addFramebuffer(CompositorFramebufferId compositorFramebufferId, const FramebufferSignature& framebufferSignature)
	{
		FramebufferElement framebufferElement(framebufferSignature);
		SortedFramebufferVector::iterator iterator = std::lower_bound(mSortedFramebufferVector.begin(), mSortedFramebufferVector.end(), framebufferElement, ::detail::orderFramebufferElementByFramebufferSignatureId);
		if (iterator == mSortedFramebufferVector.end() || iterator->framebufferSignature.getFramebufferSignatureId() != framebufferElement.framebufferSignature.getFramebufferSignatureId())
		{
			// Add new framebuffer

			// Register the new framebuffer element
			++framebufferElement.numberOfReferences;
			mSortedFramebufferVector.insert(iterator, framebufferElement);
		}
		else
		{
			// Just increase the number of references
			++iterator->numberOfReferences;
		}
		mCompositorFramebufferIdToFramebufferSignatureId.emplace(compositorFramebufferId, framebufferSignature.getFramebufferSignatureId());
	}

	Rhi::IFramebuffer* FramebufferManager::getFramebufferByCompositorFramebufferId(CompositorFramebufferId compositorFramebufferId) const
	{
		Rhi::IFramebuffer* framebuffer = nullptr;

		// Map compositor framebuffer ID to framebuffer signature ID
		CompositorFramebufferIdToFramebufferSignatureId::const_iterator iterator = mCompositorFramebufferIdToFramebufferSignatureId.find(compositorFramebufferId);
		if (mCompositorFramebufferIdToFramebufferSignatureId.cend() != iterator)
		{
			// TODO(co) Is there need for a more efficient search?
			const FramebufferSignatureId framebufferSignatureId = iterator->second;
			for (const FramebufferElement& framebufferElement : mSortedFramebufferVector)
			{
				const FramebufferSignature& framebufferSignature = framebufferElement.framebufferSignature;
				if (framebufferSignature.getFramebufferSignatureId() == framebufferSignatureId)
				{
					framebuffer = framebufferElement.framebuffer;
					break;
				}
			}
			SE_ASSERT(nullptr != framebuffer, "Invalid framebuffer")
		}
		else
		{
			// Error!
			SE_ASSERT(false, "Unknown compositor framebuffer ID, this shouldn't have happened")
		}

		// Done
		return framebuffer;
	}

	Rhi::IFramebuffer* FramebufferManager::getFramebufferByCompositorFramebufferId(CompositorFramebufferId compositorFramebufferId, const Rhi::IRenderTarget& renderTarget, uint8_t numberOfMultisamples, float resolutionScale)
	{
		Rhi::IFramebuffer* framebuffer = nullptr;

		// Map compositor framebuffer ID to framebuffer signature ID
		CompositorFramebufferIdToFramebufferSignatureId::const_iterator iterator = mCompositorFramebufferIdToFramebufferSignatureId.find(compositorFramebufferId);
		if (mCompositorFramebufferIdToFramebufferSignatureId.cend() != iterator)
		{
			// TODO(co) Is there need for a more efficient search?
			const FramebufferSignatureId framebufferSignatureId = iterator->second;
			for (FramebufferElement& framebufferElement : mSortedFramebufferVector)
			{
				const FramebufferSignature& framebufferSignature = framebufferElement.framebufferSignature;
				if (framebufferSignature.getFramebufferSignatureId() == framebufferSignatureId)
				{
					// Do we need to create the RHI framebuffer instance right now?
					if (nullptr == framebufferElement.framebuffer)
					{
						// Get the color texture instances
						Rhi::TextureFormat::Enum colorTextureFormats[8] = { Rhi::TextureFormat::Enum::UNKNOWN, Rhi::TextureFormat::Enum::UNKNOWN, Rhi::TextureFormat::Enum::UNKNOWN, Rhi::TextureFormat::Enum::UNKNOWN, Rhi::TextureFormat::Enum::UNKNOWN, Rhi::TextureFormat::Enum::UNKNOWN, Rhi::TextureFormat::Enum::UNKNOWN, Rhi::TextureFormat::Enum::UNKNOWN };
						const uint8_t numberOfColorAttachments = framebufferSignature.getNumberOfColorAttachments();
						SE_ASSERT(numberOfColorAttachments < 8, "Invalid number of color attachments")
						Rhi::FramebufferAttachment colorFramebufferAttachments[8];
						uint8_t usedNumberOfMultisamples = 0;
						for (uint8_t i = 0; i < numberOfColorAttachments; ++i)
						{
							const FramebufferSignatureAttachment& framebufferSignatureAttachment = framebufferSignature.getColorFramebufferSignatureAttachment(i);
							const AssetId colorTextureAssetId = framebufferSignatureAttachment.textureAssetId;
							const RenderTargetTextureSignature* colorRenderTargetTextureSignature = nullptr;
							Rhi::FramebufferAttachment& framebufferAttachment = colorFramebufferAttachments[i];
							framebufferAttachment.texture = IsValid(colorTextureAssetId) ? mRenderTargetTextureManager.getTextureByAssetId(colorTextureAssetId, renderTarget, numberOfMultisamples, resolutionScale, &colorRenderTargetTextureSignature) : nullptr;
							SE_ASSERT(nullptr != framebufferAttachment.texture, "Invalid framebuffer attachment texture")
							framebufferAttachment.mipmapIndex = framebufferSignatureAttachment.mipmapIndex;
							framebufferAttachment.layerIndex = framebufferSignatureAttachment.layerIndex;
							SE_ASSERT(nullptr != colorRenderTargetTextureSignature, "Invalid color render target texture signature")
							if (0 == usedNumberOfMultisamples)
							{
								usedNumberOfMultisamples = ((colorRenderTargetTextureSignature->getFlags() & RenderTargetTextureSignature::Flag::ALLOW_MULTISAMPLE) != 0) ? numberOfMultisamples : 1u;
							}
							else
							{
								SE_ASSERT(1 == usedNumberOfMultisamples || ((colorRenderTargetTextureSignature->getFlags() & RenderTargetTextureSignature::Flag::ALLOW_MULTISAMPLE) != 0), "Invalid number of multisamples")
							}
							colorTextureFormats[i] = colorRenderTargetTextureSignature->getTextureFormat();
						}

						// Get the depth stencil texture instances
						const FramebufferSignatureAttachment& depthStencilFramebufferSignatureAttachment = framebufferSignature.getDepthStencilFramebufferSignatureAttachment();
						const RenderTargetTextureSignature* depthStencilRenderTargetTextureSignature = nullptr;
						Rhi::FramebufferAttachment depthStencilFramebufferAttachment(IsValid(depthStencilFramebufferSignatureAttachment.textureAssetId) ? mRenderTargetTextureManager.getTextureByAssetId(depthStencilFramebufferSignatureAttachment.textureAssetId, renderTarget, numberOfMultisamples, resolutionScale, &depthStencilRenderTargetTextureSignature) : nullptr, depthStencilFramebufferSignatureAttachment.mipmapIndex, depthStencilFramebufferSignatureAttachment.layerIndex);
						if (nullptr != depthStencilRenderTargetTextureSignature)
						{
							if (0 == usedNumberOfMultisamples)
							{
								usedNumberOfMultisamples = ((depthStencilRenderTargetTextureSignature->getFlags() & RenderTargetTextureSignature::Flag::ALLOW_MULTISAMPLE) != 0) ? numberOfMultisamples : 1u;
							}
							else
							{
								SE_ASSERT(1 == usedNumberOfMultisamples || ((depthStencilRenderTargetTextureSignature->getFlags() & RenderTargetTextureSignature::Flag::ALLOW_MULTISAMPLE) != 0), "Invalid number of multisamples")
							}
						}
						const Rhi::TextureFormat::Enum depthStencilTextureFormat = (nullptr != depthStencilRenderTargetTextureSignature) ? depthStencilRenderTargetTextureSignature->getTextureFormat() : Rhi::TextureFormat::Enum::UNKNOWN;

						// Get or create the managed render pass
						Rhi::IRenderPass* renderPass = mRenderPassManager.getOrCreateRenderPass(numberOfColorAttachments, colorTextureFormats, depthStencilTextureFormat, usedNumberOfMultisamples);
						SE_ASSERT(nullptr != renderPass, "Invalid render pass")

						// Create the framebuffer object (FBO) instance
						// -> The framebuffer automatically adds a reference to the provided textures
						framebufferElement.framebuffer = mRenderTargetTextureManager.getRenderer().getRhi().createFramebuffer(*renderPass, colorFramebufferAttachments, ((nullptr != depthStencilFramebufferAttachment.texture) ? &depthStencilFramebufferAttachment : nullptr) RHI_RESOURCE_DEBUG_NAME("Framebuffer manager"));
						framebufferElement.framebuffer->AddReference();
					}
					framebuffer = framebufferElement.framebuffer;
					break;
				}
			}
			SE_ASSERT(nullptr != framebuffer, "Invalid framebuffer")
		}
		else
		{
			// Error!
			SE_ASSERT(false, "Unknown compositor framebuffer ID, this shouldn't have happened.")
		}

		// Done
		return framebuffer;
	}

	void FramebufferManager::releaseFramebufferBySignature(const FramebufferSignature& framebufferSignature)
	{
		const FramebufferElement framebufferElement(framebufferSignature);
		SortedFramebufferVector::iterator iterator = std::lower_bound(mSortedFramebufferVector.begin(), mSortedFramebufferVector.end(), framebufferElement, ::detail::orderFramebufferElementByFramebufferSignatureId);
		if (iterator != mSortedFramebufferVector.end() && iterator->framebufferSignature.getFramebufferSignatureId() == framebufferElement.framebufferSignature.getFramebufferSignatureId())
		{
			// Was this the last reference?
			if (1 == iterator->numberOfReferences)
			{
				if (nullptr != iterator->framebuffer)
				{
					iterator->framebuffer->ReleaseReference();
				}
				mSortedFramebufferVector.erase(iterator);
			}
			else
			{
				--iterator->numberOfReferences;
			}
		}
		else
		{
			// Error!
			SE_ASSERT(false, "Framebuffer signature isn't registered")
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

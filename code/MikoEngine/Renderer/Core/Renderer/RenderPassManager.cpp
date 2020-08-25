#include "stdafx.h"
#include "Renderer/Core/Renderer/RenderPassManager.h"
#include "Renderer/Core/Math/Math.h"

namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RenderPassManager::~RenderPassManager()
	{
		for (auto& renderPassPair : mRenderPasses)
		{
			renderPassPair.second->ReleaseReference();
		}
	}

	Rhi::IRenderPass* RenderPassManager::getOrCreateRenderPass(uint32_t numberOfColorAttachments, const Rhi::TextureFormat::Enum* colorAttachmentTextureFormats, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, uint8_t numberOfMultisamples)
	{
		// Calculate the render pass signature
		// TODO(co) Tiny performance optimization: It should be possible to pre-calculate a partial render pass signature using "numberOfColorAttachments", "colorAttachmentTextureFormats" and "depthStencilAttachmentTextureFormat" inside the renderer toolkit for the normal use-cases
		uint32_t renderPassSignature = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&numberOfColorAttachments), sizeof(uint32_t), Math::FNV1a_INITIAL_HASH_32);
		for (uint32_t i = 0; i < numberOfColorAttachments; ++i)
		{
			renderPassSignature = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&colorAttachmentTextureFormats[i]), sizeof(Rhi::TextureFormat::Enum), renderPassSignature);
		}
		renderPassSignature = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&depthStencilAttachmentTextureFormat), sizeof(Rhi::TextureFormat::Enum), renderPassSignature);
		renderPassSignature = Math::calculateFNV1a32(&numberOfMultisamples, sizeof(uint8_t), renderPassSignature);

		// Does the render pass instance already exist?
		const RenderPasses::const_iterator iterator = mRenderPasses.find(renderPassSignature);
		if (mRenderPasses.cend() != iterator)
		{
			// The render pass instance already exists
			return iterator->second;
		}
		else
		{
			// Create the render pass instance
			Rhi::IRenderPass* renderPass = mRhi.createRenderPass(numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_NAME("Render pass manager"));
			renderPass->AddReference();
			mRenderPasses.emplace(renderPassSignature, renderPass);
			return renderPass;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#pragma once

#include "Renderer/Core/Manager.h"
#include "Renderer/Core/Renderer/FramebufferSignature.h"
#include "RHI/Rhi.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class RenderPassManager;
	class RenderTargetTextureManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId CompositorFramebufferId;	// Compositor framebuffer identifier, internally just a POD "uint32_t"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class FramebufferManager final : private Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct FramebufferElement final
		{
			FramebufferSignature framebufferSignature;
			Rhi::IFramebuffer*	 framebuffer;			// Can be a null pointer, no "Rhi::IFramebufferPtr" to not have overhead when internally reallocating
			uint32_t			 numberOfReferences;	// Number of framebuffer references (don't misuse the RHI framebuffer reference counter for this)

			inline FramebufferElement() :
				framebuffer(nullptr),
				numberOfReferences(0)
			{
				// Nothing here
			}

			inline explicit FramebufferElement(const FramebufferSignature& _framebufferSignature) :
				framebufferSignature(_framebufferSignature),
				framebuffer(nullptr),
				numberOfReferences(0)
			{
				// Nothing here
			}

			inline FramebufferElement(const FramebufferSignature& _framebufferSignature, Rhi::IFramebuffer& _framebuffer) :
				framebufferSignature(_framebufferSignature),
				framebuffer(&_framebuffer),
				numberOfReferences(0)
			{
				// Nothing here
			}
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline FramebufferManager(RenderTargetTextureManager& renderTargetTextureManager, RenderPassManager& renderPassManager) :
			mRenderTargetTextureManager(renderTargetTextureManager),
			mRenderPassManager(renderPassManager)
		{
			// Nothing here
		}

		inline ~FramebufferManager()
		{
			// Nothing here
		}

		explicit FramebufferManager(const FramebufferManager&) = delete;
		FramebufferManager& operator=(const FramebufferManager&) = delete;
		void clear();
		void clearRhiResources();
		void addFramebuffer(CompositorFramebufferId compositorFramebufferId, const FramebufferSignature& framebufferSignature);
		[[nodiscard]] Rhi::IFramebuffer* getFramebufferByCompositorFramebufferId(CompositorFramebufferId compositorFramebufferId) const;
		[[nodiscard]] Rhi::IFramebuffer* getFramebufferByCompositorFramebufferId(CompositorFramebufferId compositorFramebufferId, const Rhi::IRenderTarget& mainRenderTarget, uint8_t numberOfMultisamples, float resolutionScale);
		void releaseFramebufferBySignature(const FramebufferSignature& framebufferSignature);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<FramebufferElement> SortedFramebufferVector;
		typedef std::unordered_map<uint32_t, FramebufferSignatureId> CompositorFramebufferIdToFramebufferSignatureId;	// Key = "Renderer::CompositorFramebufferId"


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RenderTargetTextureManager&						mRenderTargetTextureManager;	// Render target texture manager, just shared so don't destroy the instance
		RenderPassManager&								mRenderPassManager;				// Render pass manager, just shared so don't destroy the instance
		SortedFramebufferVector							mSortedFramebufferVector;
		CompositorFramebufferIdToFramebufferSignatureId	mCompositorFramebufferIdToFramebufferSignatureId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

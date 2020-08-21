#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResource.h"
#include "Renderer/Resource/CompositorNode/CompositorTarget.h"
#include "Renderer/Resource/CompositorNode/CompositorFramebuffer.h"
#include "Renderer/Resource/CompositorNode/CompositorRenderTargetTexture.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	template <class ELEMENT_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class PackedElementManager;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
	class CompositorNodeResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t CompositorNodeResourceId;	// POD compositor node resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorNodeResource final : public IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorNodeResourceLoader;
		friend PackedElementManager<CompositorNodeResource, CompositorNodeResourceId, 32>;									// Type definition of template class
		friend ResourceManagerTemplate<CompositorNodeResource, CompositorNodeResourceLoader, CompositorNodeResourceId, 32>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<CompositorChannelId>		   CompositorChannels;	// TODO(co) Get rid of "std::vector" and dynamic memory handling in here? (need to introduce a maximum number of input channels for this)
		typedef std::vector<CompositorRenderTargetTexture> CompositorRenderTargetTextures;
		typedef std::vector<CompositorFramebuffer>		   CompositorFramebuffers;
		typedef std::vector<CompositorTarget>			   CompositorTargets;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		// TODO(co) Asynchronous loading completion, we might want to move this into "Renderer::IResource"
		void enforceFullyLoaded();

		//[-------------------------------------------------------]
		//[ Input channels                                        ]
		//[-------------------------------------------------------]
		inline void reserveInputChannels(uint32_t numberOfInputChannels)
		{
			mInputChannels.reserve(numberOfInputChannels);
		}

		inline void addInputChannel(CompositorChannelId compositorChannelId)
		{
			mInputChannels.push_back(compositorChannelId);
		}

		[[nodiscard]] inline const CompositorChannels& getInputChannels() const
		{
			return mInputChannels;
		}

		//[-------------------------------------------------------]
		//[ Render target textures                                ]
		//[-------------------------------------------------------]
		inline void reserveRenderTargetTextures(uint32_t numberOfRenderTargetTextures)
		{
			mCompositorRenderTargetTextures.reserve(numberOfRenderTargetTextures);
		}

		inline void addRenderTargetTexture(AssetId assetId, const RenderTargetTextureSignature& renderTargetTextureSignature)
		{
			mCompositorRenderTargetTextures.emplace_back(assetId, renderTargetTextureSignature);
		}

		[[nodiscard]] inline const CompositorRenderTargetTextures& getRenderTargetTextures() const
		{
			return mCompositorRenderTargetTextures;
		}

		//[-------------------------------------------------------]
		//[ Framebuffers                                          ]
		//[-------------------------------------------------------]
		inline void reserveFramebuffers(uint32_t numberOfFramebuffers)
		{
			mCompositorFramebuffers.reserve(numberOfFramebuffers);
		}

		inline void addFramebuffer(CompositorFramebufferId compositorFramebufferId, const FramebufferSignature& framebufferSignature)
		{
			mCompositorFramebuffers.emplace_back(compositorFramebufferId, framebufferSignature);
		}

		[[nodiscard]] inline const CompositorFramebuffers& getFramebuffers() const
		{
			return mCompositorFramebuffers;
		}

		//[-------------------------------------------------------]
		//[ Targets                                               ]
		//[-------------------------------------------------------]
		inline void reserveCompositorTargets(uint32_t numberOfCompositorTargets)
		{
			mCompositorTargets.reserve(numberOfCompositorTargets);
		}

		[[nodiscard]] inline CompositorTarget& addCompositorTarget(CompositorChannelId compositorChannelId, CompositorFramebufferId compositorFramebufferId)
		{
			mCompositorTargets.emplace_back(compositorChannelId, compositorFramebufferId);
			return mCompositorTargets.back();
		}

		[[nodiscard]] inline const CompositorTargets& getCompositorTargets() const
		{
			return mCompositorTargets;
		}

		//[-------------------------------------------------------]
		//[ Output channels                                       ]
		//[-------------------------------------------------------]
		inline void reserveOutputChannels(uint32_t numberOfOutputChannels)
		{
			mOutputChannels.reserve(numberOfOutputChannels);
		}

		inline void addOutputChannel(CompositorChannelId compositorChannelId)
		{
			mOutputChannels.push_back(compositorChannelId);
		}

		[[nodiscard]] inline const CompositorChannels& getOutputChannels() const
		{
			return mOutputChannels;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline CompositorNodeResource()
		{
			// Nothing here
		}

		inline virtual ~CompositorNodeResource() override
		{
			// Sanity checks
			RHI_ASSERT(mInputChannels.empty(), "Invalid input channels")
			RHI_ASSERT(mCompositorRenderTargetTextures.empty(), "Invalid compositor render target textures")
			RHI_ASSERT(mCompositorFramebuffers.empty(), "Invalid compositor framebuffers")
			RHI_ASSERT(mCompositorTargets.empty(), "Invalid compositor targets")
			RHI_ASSERT(mOutputChannels.empty(), "Invalid output channels")
		}

		explicit CompositorNodeResource(const CompositorNodeResource&) = delete;
		CompositorNodeResource& operator=(const CompositorNodeResource&) = delete;

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(CompositorNodeResourceId compositorNodeResourceId)
		{
			// Sanity checks
			RHI_ASSERT(mInputChannels.empty(), "Invalid input channels")
			RHI_ASSERT(mCompositorRenderTargetTextures.empty(), "Invalid compositor render target textures")
			RHI_ASSERT(mCompositorFramebuffers.empty(), "Invalid compositor framebuffers")
			RHI_ASSERT(mCompositorTargets.empty(), "Invalid compositor targets")
			RHI_ASSERT(mOutputChannels.empty(), "Invalid output channels")

			// Call base implementation
			IResource::initializeElement(compositorNodeResourceId);
		}

		void deinitializeElement();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CompositorChannels			   mInputChannels;
		CompositorRenderTargetTextures mCompositorRenderTargetTextures;
		CompositorFramebuffers		   mCompositorFramebuffers;
		CompositorTargets			   mCompositorTargets;
		CompositorChannels			   mOutputChannels;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

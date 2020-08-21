#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/ResourceManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IRenderer;
	class RenderPassManager;
	class FramebufferManager;
	class RenderTargetTextureManager;
	class CompositorWorkspaceResource;
	class CompositorWorkspaceResourceLoader;
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
	typedef uint32_t CompositorWorkspaceResourceId;	// POD compositor workspace resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorWorkspaceResourceManager final : public ResourceManager<CompositorWorkspaceResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		void loadCompositorWorkspaceResourceByAssetId(AssetId assetId, CompositorWorkspaceResourceId& compositorWorkspaceResourceId, IResourceListener* resourceListener = nullptr, bool reload = false, ResourceLoaderTypeId resourceLoaderTypeId = getInvalid<ResourceLoaderTypeId>());	// Asynchronous
		void setInvalidResourceId(CompositorWorkspaceResourceId& compositorWorkspaceResourceId, IResourceListener& resourceListener) const;

		[[nodiscard]] inline RenderTargetTextureManager& getRenderTargetTextureManager()
		{
			RHI_ASSERT(nullptr != mRenderTargetTextureManager, "Invalid render target texture manager")
			return *mRenderTargetTextureManager;
		}

		[[nodiscard]] inline RenderPassManager& getRenderPassManager()
		{
			RHI_ASSERT(nullptr != mRenderPassManager, "Invalid render pass manager")
			return *mRenderPassManager;
		}

		[[nodiscard]] inline FramebufferManager& getFramebufferManager()
		{
			RHI_ASSERT(nullptr != mFramebufferManager, "Invalid framebuffer manager")
			return *mFramebufferManager;
		}


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
		explicit CompositorWorkspaceResourceManager(IRenderer& renderer);
		virtual ~CompositorWorkspaceResourceManager() override;
		explicit CompositorWorkspaceResourceManager(const CompositorWorkspaceResourceManager&) = delete;
		CompositorWorkspaceResourceManager& operator=(const CompositorWorkspaceResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RenderTargetTextureManager*	mRenderTargetTextureManager;	// Render target texture manager, always valid, we're responsible for destroying it if we no longer need it
		RenderPassManager*			mRenderPassManager;				// Render pass manager, always valid, we're responsible for destroying it if we no longer need it
		FramebufferManager*			mFramebufferManager;			// Framebuffer manager, always valid, we're responsible for destroying it if we no longer need it

		// Internal resource manager implementation
		ResourceManagerTemplate<CompositorWorkspaceResource, CompositorWorkspaceResourceLoader, CompositorWorkspaceResourceId, 32>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

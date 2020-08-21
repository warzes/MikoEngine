#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResourceListener.h"
#include "Renderer/Resource/CompositorNode/Pass/ICompositorInstancePass.h"
#include "Renderer/RenderQueue/RenderableManager.h"
#include "Renderer/RenderQueue/RenderQueue.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class CompositorResourcePassCompute;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialResourceId;	// POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compositor instance pass compute via compute or graphics pipeline state
	*
	*  @remarks
	*    Graphics material blueprint: Using a screen covering triangle as discussed at e.g.
	*    - https://web.archive.org/web/20140719063725/http://www.altdev.co/2011/08/08/interesting-vertex-shader-trick/
	*    - "Vertex Shader Tricks by Bill Bilodeau - AMD at GDC14" - http://de.slideshare.net/DevCentralAMD/vertex-shader-tricks-bill-bilodeau
	*    - Attribute-less rendering: "Rendering a Screen Covering Triangle in OpenGL (with no buffers)" - https://rauwendaal.net/2014/06/14/rendering-a-screen-covering-triangle-in-opengl/
	*
	*  @note
	*    - A compute pipeline state has less overhead as a graphics pipeline state, for best performance try to stick to compute pipeline state
	*/
	class CompositorInstancePassCompute : public ICompositorInstancePass, public IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		CompositorInstancePassCompute(const CompositorResourcePassCompute& compositorResourcePassCompute, const CompositorNodeInstance& compositorNodeInstance);
		virtual ~CompositorInstancePassCompute() override;

		[[nodiscard]] inline MaterialResourceId getMaterialResourceId() const
		{
			return mMaterialResourceId;
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	public:
		virtual void onFillCommandBuffer(const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer) override;

		inline virtual void onPostCommandBufferExecution() override
		{
			// Directly clear the render queue as soon as the frame rendering has been finished to avoid evil dangling pointers
			mRenderQueue.clear();
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::CompositorInstancePassCompute methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void createMaterialResource(MaterialResourceId parentMaterialResourceId);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		explicit CompositorInstancePassCompute(const CompositorInstancePassCompute&) = delete;
		CompositorInstancePassCompute& operator=(const CompositorInstancePassCompute&) = delete;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		bool			   mComputeMaterialBlueprint;	// "true" if this compositor instance pass is using a compute material blueprint, if "false" a graphics material blueprint is used
		RenderQueue		   mRenderQueue;
		MaterialResourceId mMaterialResourceId;
		RenderableManager  mRenderableManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

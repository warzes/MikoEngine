#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeResource.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeResourceManager.h"
#include "Renderer/Resource/ResourceStreamer.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void CompositorNodeResource::enforceFullyLoaded()
	{
		// TODO(co) Implement more efficient solution: We need to extend "Renderer::ResourceStreamer" to request emergency immediate processing of requested resources
		ResourceStreamer& resourceStreamer = getResourceManager<CompositorNodeResourceManager>().getRenderer().getResourceStreamer();
		while (IResource::LoadingState::LOADED != getLoadingState())
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
			resourceStreamer.dispatch();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CompositorNodeResource::deinitializeElement()
	{
		mInputChannels.clear();
		mCompositorRenderTargetTextures.clear();
		mCompositorFramebuffers.clear();
		mCompositorTargets.clear();
		mOutputChannels.clear();

		// Call base implementation
		IResource::deinitializeElement();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

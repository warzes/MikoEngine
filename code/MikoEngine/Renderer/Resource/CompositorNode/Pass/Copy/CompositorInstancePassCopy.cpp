#include "stdafx.h"
#include "Renderer/Resource/CompositorNode/Pass/Copy/CompositorInstancePassCopy.h"
#include "Renderer/Resource/CompositorNode/Pass/Copy/CompositorResourcePassCopy.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceResourceManager.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
#include "Renderer/Resource/Texture/TextureResourceManager.h"
#include "Renderer/Resource/Texture/TextureResource.h"
#include "Renderer/Core/IProfiler.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Context.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassCopy::onFillCommandBuffer([[maybe_unused]] const Rhi::IRenderTarget* renderTarget, const CompositorContextData&, Rhi::CommandBuffer& commandBuffer)
	{
		const CompositorResourcePassCopy& compositorResourcePassCopy = static_cast<const CompositorResourcePassCopy&>(getCompositorResourcePass());
		const IRenderer& renderer = getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer();

		// Sanity check
		RHI_ASSERT(nullptr == renderTarget, "The copy compositor instance pass needs an invalid render target")

		// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
		RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(renderer.getContext(), commandBuffer, compositorResourcePassCopy.getDebugName())

		// Get destination and source texture resources
		// TODO(co) "Renderer::TextureResourceManager::getTextureResourceByAssetId()" is considered to be inefficient, don't use it in here
		const TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
		const TextureResource* destinationTextureResource = textureResourceManager.getTextureResourceByAssetId(compositorResourcePassCopy.getDestinationTextureAssetId());
		const TextureResource* sourceTextureResource = textureResourceManager.getTextureResourceByAssetId(compositorResourcePassCopy.getSourceTextureAssetId());
		if (nullptr != destinationTextureResource && nullptr != sourceTextureResource)
		{
			const Rhi::ITexturePtr& destinationTexturePtr = destinationTextureResource->getTexturePtr();
			const Rhi::ITexturePtr& sourceTexturePtr = sourceTextureResource->getTexturePtr();
			if (nullptr != destinationTexturePtr && nullptr != sourceTexturePtr)
			{
				Rhi::Command::CopyResource::create(commandBuffer, *destinationTexturePtr, *sourceTexturePtr);
			}
			else
			{
				// Error!
				RHI_ASSERT(false, "We should never end up in here")
			}
		}
		else
		{
			// Error!
			RHI_ASSERT(false, "We should never end up in here")
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorInstancePassCopy::CompositorInstancePassCopy(const CompositorResourcePassCopy& compositorResourcePassCopy, const CompositorNodeInstance& compositorNodeInstance) :
		ICompositorInstancePass(compositorResourcePassCopy, compositorNodeInstance)
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

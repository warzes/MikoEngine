#pragma once

#include "RHI/Rhi.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rhi
{
	class IResource;
	class ISamplerState;
	class IRootSignature;
	class IResourceGroup;
}
namespace Renderer
{
	class IRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Renderer resource manager
	*/
	class RendererResourceManager final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] Rhi::IResourceGroup* createResourceGroup(Rhi::IRootSignature& rootSignature, uint32_t rootParameterIndex, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER);
		void garbageCollection();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit RendererResourceManager(IRenderer& renderer) :
			mRenderer(renderer),
			mGarbageCollectionCounter(0)
		{
			// Nothing here
		}

		~RendererResourceManager();
		explicit RendererResourceManager(const RendererResourceManager&) = delete;
		RendererResourceManager& operator=(const RendererResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint32_t, Rhi::IResourceGroup*> ResourceGroups;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&	   mRenderer;	// Renderer instance, do not destroy the instance
		ResourceGroups mResourceGroups;
		uint32_t	   mGarbageCollectionCounter;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

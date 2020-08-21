#pragma once

#include "RHI/Rhi.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class CompositorContextData;
	class ICompositorInstancePass;
	class CompositorWorkspaceInstance;
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
	class CompositorNodeInstance final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorWorkspaceInstance;	// Needs to create and execute compositor node instances


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<ICompositorInstancePass*> CompositorInstancePasses;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline CompositorNodeResourceId getCompositorNodeResourceId() const
		{
			return mCompositorNodeResourceId;
		}

		[[nodiscard]] inline const CompositorWorkspaceInstance& getCompositorWorkspaceInstance() const
		{
			return mCompositorWorkspaceInstance;
		}

		[[nodiscard]] inline const CompositorInstancePasses& getCompositorInstancePasses() const
		{
			return mCompositorInstancePasses;
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline CompositorNodeInstance(CompositorNodeResourceId compositorNodeResourceId, const CompositorWorkspaceInstance& compositorWorkspaceInstance) :
			mCompositorNodeResourceId(compositorNodeResourceId),
			mCompositorWorkspaceInstance(compositorWorkspaceInstance)
		{
			// Nothing here
		}

		~CompositorNodeInstance();
		explicit CompositorNodeInstance(const CompositorNodeInstance&) = delete;
		CompositorNodeInstance& operator=(const CompositorNodeInstance&) = delete;
		void compositorWorkspaceInstanceLoadingFinished() const;
		[[nodiscard]] Rhi::IRenderTarget& fillCommandBuffer(Rhi::IRenderTarget& renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer) const;

		/**
		*  @brief
		*    Called post command buffer execution
		*/
		void onPostCommandBufferExecution() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CompositorNodeResourceId		   mCompositorNodeResourceId;
		const CompositorWorkspaceInstance& mCompositorWorkspaceInstance;
		CompositorInstancePasses		   mCompositorInstancePasses;	// Owns the compositor instance passes and has to destroy them if no longer needed


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

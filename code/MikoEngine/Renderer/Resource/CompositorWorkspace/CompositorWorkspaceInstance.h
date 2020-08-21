#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/StringId.h"
#include "Renderer/Resource/IResourceListener.h"
#include "RHI/Rhi.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class LightSceneItem;
	class CameraSceneItem;
	class IRenderer;
	class ISceneItem;
	class RenderableManager;
	class CompositorNodeInstance;
	class ICompositorInstancePass;
	class CompositorInstancePassShadowMap;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;						// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef uint32_t CompositorWorkspaceResourceId;	// POD compositor workspace resource identifier
	typedef StringId CompositorPassTypeId;			// Compositor pass type identifier, internally just a POD "uint32_t"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compositor workspace instance
	*
	*  @remarks
	*    Compositors can get quite complex with a lot of individual compositor passes which several of them rendering portions of the scene.
	*    We really only want to perform expensive culling once for a compositor workspace frame rendering. Some renderable managers might never
	*    get rendered because none of the renderables is inside a render queue index range touched by the compositor passes. As a result,
	*    an compositor workspace instance keeps a list of render queue index ranges covered by the compositor instance passes. Before compositor
	*    instance passes are executed, a culling step is performed gathering all renderable managers which should currently be taken into account
	*    during rendering. The result of this culling step is that each render queue index range has renderable managers to consider assigned to them.
	*    Executed compositor instances passes only access this prepared render queue index information to fill their render queues.
	*/
	class CompositorWorkspaceInstance final : protected IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<RenderableManager*> RenderableManagers;
		typedef std::vector<CompositorNodeInstance*> CompositorNodeInstances;

		struct RenderQueueIndexRange final
		{
			uint8_t			   minimumRenderQueueIndex;	// Fixed during runtime
			uint8_t			   maximumRenderQueueIndex;	// Fixed during runtime
			RenderableManagers renderableManagers;		// Dynamic during runtime

			inline RenderQueueIndexRange(uint8_t _minimumRenderQueueIndex, uint8_t _maximumRenderQueueIndex) :
				minimumRenderQueueIndex(_minimumRenderQueueIndex),
				maximumRenderQueueIndex(_maximumRenderQueueIndex)
			{}
		};
		typedef std::vector<RenderQueueIndexRange> RenderQueueIndexRanges;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		CompositorWorkspaceInstance(IRenderer& renderer, AssetId compositorWorkspaceAssetId);
		virtual ~CompositorWorkspaceInstance() override;

		[[nodiscard]] inline const IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		[[nodiscard]] inline uint8_t getNumberOfMultisamples() const
		{
			return mNumberOfMultisamples;
		}

		void setNumberOfMultisamples(uint8_t numberOfMultisamples);	// The number of multisamples per pixel (valid values: 1, 2, 4, 8); Changes are considered to be expensive since internal RHI resources might need to be updated when rendering the next time

		[[nodiscard]] inline float getResolutionScale() const
		{
			return mResolutionScale;
		}

		inline void setResolutionScale(float resolutionScale)	// Changes are considered to be expensive since internal RHI resources might need to be updated when rendering the next time
		{
			mResolutionScale = resolutionScale;
		}

		[[nodiscard]] inline const RenderQueueIndexRanges& getRenderQueueIndexRanges() const	// Renderable manager pointers are only considered to be safe directly after the "Renderer::CompositorWorkspaceInstance::execute()" call
		{
			return mRenderQueueIndexRanges;
		}

		[[nodiscard]] const RenderQueueIndexRange* getRenderQueueIndexRangeByRenderQueueIndex(uint8_t renderQueueIndex) const;	// Can be a null pointer, don't destroy the instance
		[[nodiscard]] const ICompositorInstancePass* getFirstCompositorInstancePassByCompositorPassTypeId(CompositorPassTypeId compositorPassTypeId) const;
		void executeVr(Rhi::IRenderTarget& renderTarget, CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem);	// If "Renderer::IVrManager::isRunning()" is true, virtual reality rendering is used, don't use this method if you want to render e.g. into a texture for other purposes
		void execute(Rhi::IRenderTarget& renderTarget, const CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem, bool singlePassStereoInstancing = false);

		[[nodiscard]] inline const CompositorNodeInstances& getSequentialCompositorNodeInstances() const
		{
			return mSequentialCompositorNodeInstances;
		}

		[[nodiscard]] inline Rhi::IRenderTarget* getExecutionRenderTarget() const	// Only valid during compositor workspace instance execution
		{
			return mExecutionRenderTarget;
		}

		[[nodiscard]] inline const Rhi::CommandBuffer& getCommandBuffer() const
		{
			return mCommandBuffer;
		}

		#ifdef RHI_STATISTICS
			[[nodiscard]] inline const Rhi::PipelineStatisticsQueryResult& getPipelineStatisticsQueryResult() const
			{
				return mPipelineStatisticsQueryResult;
			}
		#endif


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		CompositorWorkspaceInstance() = delete;
		explicit CompositorWorkspaceInstance(const CompositorWorkspaceInstance&) = delete;
		CompositorWorkspaceInstance& operator=(const CompositorWorkspaceInstance&) = delete;
		void destroySequentialCompositorNodeInstances();
		void createFramebuffersAndRenderTargetTextures(const Rhi::IRenderTarget& mainRenderTarget);
		void destroyFramebuffersAndRenderTargetTextures(bool clearManagers = false);
		void clearRenderQueueIndexRangesRenderableManagers();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&						 mRenderer;
		uint8_t							 mNumberOfMultisamples;
		uint8_t							 mCurrentlyUsedNumberOfMultisamples;
		float							 mResolutionScale;
		uint32_t						 mRenderTargetWidth;
		uint32_t						 mRenderTargetHeight;
		CompositorWorkspaceResourceId	 mCompositorWorkspaceResourceId;
		CompositorNodeInstances			 mSequentialCompositorNodeInstances;	// We're responsible to destroy the compositor node instances if we no longer need them
		bool							 mFramebufferManagerInitialized;
		RenderQueueIndexRanges			 mRenderQueueIndexRanges;				// The render queue index ranges layout is fixed during runtime

		// The rest is temporary "CompositorWorkspaceInstance::execute()" data to e.g. avoid reallocations
		Rhi::IRenderTarget*				 mExecutionRenderTarget;				// Only valid during compositor workspace instance execution
		std::vector<ISceneItem*>		 mExecuteOnRenderingSceneItems;			// Scene items which requested an execute call on rendering, no duplicates allowed
		Rhi::CommandBuffer				 mCommandBuffer;						// RHI command buffer
		CompositorInstancePassShadowMap* mCompositorInstancePassShadowMap;		// Can be a null pointer, don't destroy the instance
		#ifdef RHI_STATISTICS
			Rhi::IQueryPoolPtr				   mPipelineStatisticsQueryPoolPtr;					// Double buffered asynchronous pipeline statistics query pool, can be a null pointer
			uint32_t						   mPreviousCurrentPipelineStatisticsQueryIndex;	// Can be "Renderer::getInvalid<uint32_t>()"
			uint32_t						   mCurrentPipelineStatisticsQueryIndex;			// Toggles between 0 or 1
			Rhi::PipelineStatisticsQueryResult mPipelineStatisticsQueryResult;					// Due to double buffered asynchronous pipeline statistics query pool, this is the pipeline statistics query result of the previous frame
		#endif


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

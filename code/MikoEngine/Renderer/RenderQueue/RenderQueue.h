#pragma once

#include "Renderer/Resource/ShaderBlueprint/Cache/ShaderProperties.h"
#include "RHI/Rhi.h"

namespace Renderer
{
	class Renderable;
	class IRenderer;
	class MaterialResource;
	class MaterialTechnique;
	class RenderableManager;
	class CompositorContextData;
	class IndirectBufferManager;
	class MaterialBlueprintResource;
}

namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialTechniqueId;	// Material technique identifier, result of hashing the material technique name via "Renderer::StringId"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Render queue
	*
	*  @remarks
	*    Using layered rendering (aka bucketized rendering) concept described in
	*    - "realtimecollisiondetection.net – the blog" - "Order your graphics draw calls around!" by Christer Ericson from October 3, 2008 - http://realtimecollisiondetection.net/blog/?p=86
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering – Part 1" by Stefan Reinalter from November 6, 2014 - https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
	*
	*    The sole purpose of the render queue is to fill sorted commands into a given command buffer.
	*/
	class RenderQueue final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] indirectBufferManager
		*    Indirect buffer manager to use, must stay valid as long as the indirect buffer manager instance exists
		*  @param[in] minimumRenderQueueIndex
		*    Minimum render queue index (inclusive)
		*  @param[in] maximumRenderQueueIndex
		*    Maximum render queue index (inclusive)
		*  @param[in] positionOnlyPass
		*    "true" if this render queue is used for a position-only render pass, else "false"
		*  @param[in] transparentPass
		*    "true" if this render queue is used for a transparent render pass, else "false" for opaque render pass (influences the renderables sorting)
		*  @param[in] doSort
		*    Sort renderables?
		*/
		RenderQueue(IndirectBufferManager& indirectBufferManager, uint8_t minimumRenderQueueIndex, uint8_t maximumRenderQueueIndex, bool positionOnlyPass, bool transparentPass, bool doSort);

		inline ~RenderQueue()
		{
			// Nothing here
		}

		[[nodiscard]] inline uint32_t getNumberOfDrawCalls() const
		{
			return mNumberOfNullDrawCalls + mNumberOfDrawIndexedCalls + mNumberOfDrawCalls;
		}

		[[nodiscard]] inline uint8_t getMinimumRenderQueueIndex() const
		{
			return mMinimumRenderQueueIndex;
		}

		[[nodiscard]] inline uint8_t getMaximumRenderQueueIndex() const
		{
			return mMaximumRenderQueueIndex;
		}

		void clear();
		void addRenderablesFromRenderableManager(const RenderableManager& renderableManager, MaterialTechniqueId materialTechniqueId, const CompositorContextData& compositorContextData, bool castShadows = false);
		void fillGraphicsCommandBuffer(const Rhi::IRenderTarget& renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer);
		void fillComputeCommandBuffer(const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit RenderQueue(const RenderQueue&) = delete;
		RenderQueue& operator=(const RenderQueue&) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct QueuedRenderable final
		{
			const Renderable*		   renderable;					// Always valid, don't destroy the instance
			const MaterialResource*	   materialResource;			// Always valid, don't destroy the instance
			MaterialTechnique*		   materialTechnique;			// Always valid, don't destroy the instance
			MaterialBlueprintResource* materialBlueprintResource;	// Always valid, don't destroy the instance
			Rhi::IPipelineState*	   foundPipelineState;			// Always valid, don't destroy the instance
			uint64_t				   sortingKey;					// Key used for sorting

			inline QueuedRenderable() :
				renderable(nullptr),
				materialResource(nullptr),
				materialTechnique(nullptr),
				materialBlueprintResource(nullptr),
				foundPipelineState(nullptr),
				sortingKey(0)
			{}
			inline QueuedRenderable(const Renderable& _renderable, const MaterialResource& _materialResource, MaterialTechnique& _materialTechnique, MaterialBlueprintResource& _materialBlueprintResource, Rhi::IPipelineState& _foundPipelineState, uint64_t _sortingKey) :
				renderable(&_renderable),
				materialResource(&_materialResource),
				materialTechnique(&_materialTechnique),
				materialBlueprintResource(&_materialBlueprintResource),
				foundPipelineState(&_foundPipelineState),
				sortingKey(_sortingKey)
			{}
			[[nodiscard]] inline bool operator < (const QueuedRenderable& queuedRenderable) const
			{
				return (sortingKey < queuedRenderable.sortingKey);
			}
		};
		typedef std::vector<QueuedRenderable> QueuedRenderables;

		struct Queue final
		{
			QueuedRenderables queuedRenderables;
			bool			  sorted = false;
		};
		typedef std::vector<Queue> Queues;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const IRenderer&		mRenderer;					// Renderer instance, we don't own the instance so don't delete it
		IndirectBufferManager&	mIndirectBufferManager;		// Indirect buffer manager instance, we don't own the instance so don't delete it
		Queues					mQueues;
		uint32_t				mNumberOfNullDrawCalls;
		uint32_t				mNumberOfDrawIndexedCalls;
		uint32_t				mNumberOfDrawCalls;
		uint8_t					mMinimumRenderQueueIndex;	// Inclusive
		uint8_t					mMaximumRenderQueueIndex;	// Inclusive
		bool					mPositionOnlyPass;
		bool					mTransparentPass;
		bool					mDoSort;
		// Scratch buffers to reduce dynamic memory allocations
		Rhi::CommandBuffer		mScratchCommandBuffer;
		ShaderProperties		mScratchShaderProperties;
		ShaderProperties		mScratchOptimizedShaderProperties;
	};
} // Renderer
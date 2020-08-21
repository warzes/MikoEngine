#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateSignature.h"
#include "RHI/Rhi.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class GraphicsPipelineStateCache final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class GraphicsPipelineStateCompiler;		// Needs to be able to set "mGraphicsPipelineStateObjectPtr"
		friend class GraphicsPipelineStateCacheManager;	// Is creating and managing graphics pipeline state cache instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the graphics pipeline state signature of the cache
		*
		*  @return
		*    The graphics pipeline state signature of the cache
		*/
		[[nodiscard]] inline const GraphicsPipelineStateSignature& getGraphicsPipelineStateSignature() const
		{
			return mGraphicsPipelineStateSignature;
		}

		/**
		*  @brief
		*    Return graphics pipeline state object
		*
		*  @return
		*    The graphics pipeline state object
		*/
		[[nodiscard]] inline const Rhi::IGraphicsPipelineStatePtr& getGraphicsPipelineStateObjectPtr() const
		{
			return mGraphicsPipelineStateObjectPtr;
		}

		/**
		*  @brief
		*    Return whether or not the graphics pipeline state cache is currently using fallback data due to asynchronous compilation
		*
		*  @return
		*    If "true", this graphics pipeline state cache is currently using fallback data because it's in asynchronous compilation
		*/
		[[nodiscard]] inline bool isUsingFallback() const
		{
			return mIsUsingFallback;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit GraphicsPipelineStateCache(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature) :
			mGraphicsPipelineStateSignature(graphicsPipelineStateSignature),
			mIsUsingFallback(false)
		{
			// Nothing here
		}

		inline ~GraphicsPipelineStateCache()
		{
			// Nothing here
		}

		explicit GraphicsPipelineStateCache(const GraphicsPipelineStateCache&) = delete;
		GraphicsPipelineStateCache& operator=(const GraphicsPipelineStateCache&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GraphicsPipelineStateSignature mGraphicsPipelineStateSignature;
		Rhi::IGraphicsPipelineStatePtr mGraphicsPipelineStateObjectPtr;
		bool						   mIsUsingFallback;					// If "true", this graphics pipeline state cache is currently using fallback data because it's in asynchronous compilation


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

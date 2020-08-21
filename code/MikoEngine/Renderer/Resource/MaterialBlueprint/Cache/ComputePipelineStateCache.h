#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/MaterialBlueprint/Cache/ComputePipelineStateSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class ComputePipelineStateCache final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ComputePipelineStateCompiler;		// Needs to be able to set "mComputePipelineStateObjectPtr"
		friend class ComputePipelineStateCacheManager;	// Is creating and managing compute pipeline state cache instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the compute pipeline state signature of the cache
		*
		*  @return
		*    The compute pipeline state signature of the cache
		*/
		[[nodiscard]] inline const ComputePipelineStateSignature& getComputePipelineStateSignature() const
		{
			return mComputePipelineStateSignature;
		}

		/**
		*  @brief
		*    Return compute pipeline state object
		*
		*  @return
		*    The compute pipeline state object
		*/
		[[nodiscard]] inline const Rhi::IComputePipelineStatePtr& getComputePipelineStateObjectPtr() const
		{
			return mComputePipelineStateObjectPtr;
		}

		/**
		*  @brief
		*    Return whether or not the compute pipeline state cache is currently using fallback data due to asynchronous compilation
		*
		*  @return
		*    If "true", this compute pipeline state cache is currently using fallback data because it's in asynchronous compilation
		*/
		[[nodiscard]] inline bool isUsingFallback() const
		{
			return mIsUsingFallback;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit ComputePipelineStateCache(const ComputePipelineStateSignature& computePipelineStateSignature) :
			mComputePipelineStateSignature(computePipelineStateSignature),
			mIsUsingFallback(false)
		{
			// Nothing here
		}

		inline ~ComputePipelineStateCache()
		{
			// Nothing here
		}

		explicit ComputePipelineStateCache(const ComputePipelineStateCache&) = delete;
		ComputePipelineStateCache& operator=(const ComputePipelineStateCache&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ComputePipelineStateSignature mComputePipelineStateSignature;
		Rhi::IComputePipelineStatePtr mComputePipelineStateObjectPtr;
		bool						  mIsUsingFallback;					// If "true", this compute pipeline state cache is currently using fallback data because it's in asynchronous compilation


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

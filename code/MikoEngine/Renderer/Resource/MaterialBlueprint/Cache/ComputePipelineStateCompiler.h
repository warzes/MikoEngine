#pragma once

#include "RHI/Rhi.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class ShaderCache;
	class IRenderer;
	class MaterialBlueprintResource;
	class ComputePipelineStateCache;
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
	*    Compute pipeline state compiler class
	*
	*  @remarks
	*    A compute pipeline state must master the following stages in order to archive the inner wisdom:
	*    1. Asynchronous shader building
	*    2. Asynchronous shader compilation
	*    3. Synchronous RHI implementation dispatch TODO(co) Asynchronous RHI implementation dispatch if supported by the RHI implementation
	*
	*  @note
	*    - Takes care of asynchronous compute pipeline state compilation
	*/
	class ComputePipelineStateCompiler final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;
		friend class ComputePipelineStateCacheManager;	// Only the compute pipeline state cache manager is allowed to commit compiler requests


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline bool isAsynchronousCompilationEnabled() const
		{
			return mAsynchronousCompilationEnabled;
		}

		void setAsynchronousCompilationEnabled(bool enabled);

		[[nodiscard]] inline uint32_t getNumberOfCompilerThreads() const
		{
			return mNumberOfCompilerThreads;
		}

		void setNumberOfCompilerThreads(uint32_t numberOfCompilerThreads);

		[[nodiscard]] inline uint32_t getNumberOfInFlightCompilerRequests() const
		{
			return mNumberOfInFlightCompilerRequests;
		}

		inline void flushBuilderQueue()
		{
			flushQueue(mBuilderMutex, mBuilderQueue);
		}

		inline void flushCompilerQueue()
		{
			flushQueue(mCompilerMutex, mCompilerQueue);
		}

		inline void flushAllQueues()
		{
			flushBuilderQueue();
			flushCompilerQueue();
		}

		void dispatch();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct CompilerRequest final
		{
			// Input
			ComputePipelineStateCache&	computePipelineStateCache;
			// Internal
			ShaderCache*				shaderCache;
			std::string					shaderSourceCode;
			Rhi::IComputePipelineState* computePipelineStateObject;

			inline explicit CompilerRequest(ComputePipelineStateCache& _computePipelineStateCache) :
				computePipelineStateCache(_computePipelineStateCache),
				shaderCache(nullptr),
				computePipelineStateObject(nullptr)
			{
				// Nothing here
			}
			inline explicit CompilerRequest(const CompilerRequest& compilerRequest) :
				computePipelineStateCache(compilerRequest.computePipelineStateCache),
				shaderCache(compilerRequest.shaderCache),
				shaderSourceCode(compilerRequest.shaderSourceCode),
				computePipelineStateObject(compilerRequest.computePipelineStateObject)
			{
				// Nothing here
			}
			CompilerRequest& operator=(const CompilerRequest&) = delete;
		};

		typedef std::vector<std::thread> CompilerThreads;
		typedef std::deque<CompilerRequest> CompilerRequests;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit ComputePipelineStateCompiler(IRenderer& renderer);
		explicit ComputePipelineStateCompiler(const ComputePipelineStateCompiler&) = delete;
		~ComputePipelineStateCompiler();
		ComputePipelineStateCompiler& operator=(const ComputePipelineStateCompiler&) = delete;
		void addAsynchronousCompilerRequest(ComputePipelineStateCache& computePipelineStateCache);
		void instantSynchronousCompilerRequest(MaterialBlueprintResource& materialBlueprintResource, ComputePipelineStateCache& computePipelineStateCache);
		void flushQueue(std::mutex& mutex, const CompilerRequests& compilerRequests);
		void builderThreadWorker();
		void compilerThreadWorker();
		[[nodiscard]] Rhi::IComputePipelineState* createComputePipelineState(const MaterialBlueprintResource& materialBlueprintResource, Rhi::IShader& shader) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&			  mRenderer;	// Renderer instance, do not destroy the instance
		bool				  mAsynchronousCompilationEnabled;
		uint32_t			  mNumberOfCompilerThreads;
		std::atomic<uint32_t> mNumberOfInFlightCompilerRequests;

		// Asynchronous building (moderate cost)
		std::atomic<bool>		mShutdownBuilderThread;
		std::mutex				mBuilderMutex;
		std::condition_variable	mBuilderConditionVariable;
		CompilerRequests		mBuilderQueue;
		std::thread				mBuilderThread;

		// Asynchronous compilation (nuts cost)
		std::atomic<bool>		mShutdownCompilerThread;
		std::mutex				mCompilerMutex;
		std::condition_variable	mCompilerConditionVariable;
		CompilerRequests		mCompilerQueue;
		CompilerThreads			mCompilerThreads;

		// Synchronous dispatch
		std::mutex		 mDispatchMutex;
		CompilerRequests mDispatchQueue;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

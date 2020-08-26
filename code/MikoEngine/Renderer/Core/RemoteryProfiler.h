#pragma once
#ifndef RENDERER_PROFILER
	#error "Do only include this header if the RENDERER_PROFILER preprocessor definition is set"
#endif


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/IProfiler.h"

#include "Rhi/Rhi.h"

#include <Remotery/Remotery.h>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] void* RmtMalloc(void* mm_context, rmtU32 size)
		{
			return static_cast<DefaultAllocator*>(mm_context)->reallocate(nullptr, 0, size, 1);
		}

		[[nodiscard]] void* RmtRealloc(void* mm_context, void* ptr, rmtU32 size)
		{
			return static_cast<DefaultAllocator*>(mm_context)->reallocate(ptr, 0, size, 1);
		}

		void RmtFreePtr(void* mm_context, void* ptr)
		{
			static_cast<DefaultAllocator*>(mm_context)->reallocate(ptr, 0, 0, 1);
		}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
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
	*    Remotery ( https://github.com/Celtoys/Remotery ) profiler implementation class one can use
	*
	*  @note
	*    - Designed to be instanced and used inside a single C++ file
	*    - Remotery also supports GPU profiling, else we would have to implement it as e.g. described at "GPU Profiling 101" by Nathan Reed ( http://reedbeta.com/blog/gpu-profiling-101/ )
	*/
	class RemoteryProfiler final : public IProfiler
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		// TODO(co) Remotery GPU profiling: Disabled by default since it might take some shutdown time due to "rmt_UnbindOpenGL blocks indefinitely #112" - https://github.com/Celtoys/Remotery/issues/112
		inline RemoteryProfiler(Rhi::IRhi& rhi, [[maybe_unused]] bool enableGpuProfiling = false) :
			mRemotery(nullptr),
			mUseD3D11(false),
			mUseOpenGL(false)
		{
			// Sanity check
			#if !defined(RHI_DEBUG) && !defined(SHARED_LIBRARIES)
				RHI_LOG(PERFORMANCE_WARNING, "Reminder: You might not want to ship products with enabled Remotery CPU and GPU profiling")
			#endif

			{ // Set Remotery settings
				rmtSettings* settings = _rmt_Settings();

				// Tell Remotery about our allocator
				settings->malloc	 = &::detail::RmtMalloc;
				settings->realloc	 = &::detail::RmtRealloc;
				settings->free		 = &::detail::RmtFreePtr;
				settings->mm_context = &GetAllocator();
			}

			// Create global Remotery instance
			rmt_CreateGlobalInstance(&mRemotery);

			// Bind Remotery RHI implementation
			SE_ASSERT(nullptr != mRemotery, "Failed to create global Remotery profiler instance")
			#if RMT_USE_D3D11
				if (enableGpuProfiling && rhi.getD3D11DevicePointer() != nullptr && rhi.getD3D11ImmediateContextPointer() != nullptr)
				{
					_rmt_BindD3D11(rhi.getD3D11DevicePointer(), rhi.getD3D11ImmediateContextPointer());
					mUseD3D11 = true;
				}
			#endif
			#if RMT_USE_OPENGL
				if (enableGpuProfiling && rhi.getNameId() == Rhi::NameId::OPENGL)
				{
					_rmt_BindOpenGL();
					mUseOpenGL = true;
				}
			#endif
		}

		inline virtual ~RemoteryProfiler() override
		{
			SE_ASSERT(0 == mNumberOfCurrentlyBegunCpuSamples && 0 == mNumberOfCurrentlyBegunGpuSamples, "Profiler sampling leak detected, not all begun samples were ended")
			#if RMT_USE_D3D11
				if (mUseD3D11)
				{
					_rmt_UnbindD3D11();
				}
			#endif
			#if RMT_USE_OPENGL
				if (mUseOpenGL)
				{
					_rmt_UnbindOpenGL();
				}
			#endif
			rmt_DestroyGlobalInstance(mRemotery);
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IProfiler methods            ]
	//[-------------------------------------------------------]
	public:
		virtual void beginCpuSample(const char* name, uint32_t* hashCache) override
		{
			#if SE_DEBUG
				++mNumberOfCurrentlyBegunCpuSamples;
			#endif
			_rmt_BeginCPUSample(name, RMTSF_Aggregate, hashCache);
		}

		virtual void endCpuSample() override
		{
			_rmt_EndCPUSample();
			#if SE_DEBUG
				--mNumberOfCurrentlyBegunCpuSamples;
			#endif
		}

		virtual void beginGpuSample([[maybe_unused]] const char* name, [[maybe_unused]] uint32_t* hashCache) override
		{
			#if SE_DEBUG
				++mNumberOfCurrentlyBegunGpuSamples;
			#endif
			#if RMT_USE_D3D11
				if (mUseD3D11)
				{
					_rmt_BeginD3D11Sample(name, hashCache);
				}
			#endif
			#if RMT_USE_OPENGL
				if (mUseOpenGL)
				{
					_rmt_BeginOpenGLSample(name, hashCache);
				}
			#endif
		}

		virtual void endGpuSample() override
		{
			#if RMT_USE_D3D11
				if (mUseD3D11)
				{
					_rmt_EndD3D11Sample();
				}
			#endif
			#if RMT_USE_OPENGL
				if (mUseOpenGL)
				{
					_rmt_EndOpenGLSample();
				}
			#endif
			#if SE_DEBUG
				--mNumberOfCurrentlyBegunGpuSamples;
			#endif
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit RemoteryProfiler(const RemoteryProfiler&) = delete;
		RemoteryProfiler& operator=(const RemoteryProfiler&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Remotery* mRemotery;
		bool	  mUseD3D11;
		bool	  mUseOpenGL;
		#if SE_DEBUG
			int mNumberOfCurrentlyBegunCpuSamples = 0;	// For leak detection
			int mNumberOfCurrentlyBegunGpuSamples = 0;	// For leak detection
		#endif


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#pragma once

#include "RHI/Rhi.h"

namespace Renderer
{
	class IFileManager;
	#ifdef RENDERER_GRAPHICS_DEBUGGER
		class IGraphicsDebugger;
	#endif
	#ifdef RENDERER_PROFILER
		class IProfiler;
	#endif
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
	*    Context class encapsulating all embedding related wirings
	*/
	class Context final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    RHI instance to use, the render instance must stay valid as long as the renderer instance exists
		*  @param[in] fileManager
		*    File manager instance to use, the file manager instance must stay valid as long as the renderer instance exists
		*  @param[in] graphicsDebugger
		*    Graphics debugger instance to use, the graphics debugger instance must stay valid as long as the renderer instance exists
		*  @param[in] profiler
		*    Profiler instance to use, the profiler instance must stay valid as long as the renderer instance exists
		*/
		#if defined(RENDERER_GRAPHICS_DEBUGGER) && defined(RENDERER_PROFILER)
			Context(Rhi::IRhi& rhi, IFileManager& fileManager, IGraphicsDebugger& graphicsDebugger, IProfiler& profiler);
		#elif defined RENDERER_GRAPHICS_DEBUGGER
			Context(Rhi::IRhi& rhi, IFileManager& fileManager, IGraphicsDebugger& graphicsDebugger);
		#elif defined RENDERER_PROFILER
			Context(Rhi::IRhi& rhi, IFileManager& fileManager, IProfiler& profiler);
		#else
			Context(Rhi::IRhi& rhi, IFileManager& fileManager);
		#endif

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Context()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Return the used RHI instance
		*
		*  @return
		*    The used RHI instance
		*/
		[[nodiscard]] inline Rhi::IRhi& getRhi() const
		{
			return mRhi;
		}

		/**
		*  @brief
		*    Return the used file manager instance
		*
		*  @return
		*    The used file manager instance
		*/
		[[nodiscard]] inline IFileManager& getFileManager() const
		{
			return mFileManager;
		}

		#ifdef RENDERER_GRAPHICS_DEBUGGER
			/**
			*  @brief
			*    Return the used graphics debugger instance
			*
			*  @return
			*    The used graphics debugger instance
			*/
			[[nodiscard]] inline IGraphicsDebugger& getGraphicsDebugger() const
			{
				return mGraphicsDebugger;
			}
		#endif

		#ifdef RENDERER_PROFILER
			/**
			*  @brief
			*    Return the used profiler instance
			*
			*  @return
			*    The used profiler instance
			*/
			[[nodiscard]] inline IProfiler& getProfiler() const
			{
				return mProfiler;
			}
		#endif


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:		
		Rhi::IRhi&		 mRhi;
		IFileManager&	 mFileManager;
		#ifdef RENDERER_GRAPHICS_DEBUGGER
			IGraphicsDebugger& mGraphicsDebugger;
		#endif
		#ifdef RENDERER_PROFILER
			IProfiler& mProfiler;
		#endif


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

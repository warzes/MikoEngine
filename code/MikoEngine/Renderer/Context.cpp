#include "stdafx.h"
#include "Renderer/Context.h"
#include "Rhi/Rhi.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	#if defined(RENDERER_GRAPHICS_DEBUGGER) && defined(RENDERER_PROFILER)
		Context::Context(Rhi::IRhi& rhi, IFileManager& fileManager, IGraphicsDebugger& graphicsDebugger, IProfiler& profiler) :
	#elif defined RENDERER_GRAPHICS_DEBUGGER
		Context::Context(Rhi::IRhi& rhi, IFileManager& fileManager, IGraphicsDebugger& graphicsDebugger) :
	#elif defined RENDERER_PROFILER
		Context::Context(Rhi::IRhi& rhi, IFileManager& fileManager, IProfiler& profiler) :
	#else
		Context::Context(Rhi::IRhi& rhi, IFileManager& fileManager) :
	#endif
		mRhi(rhi),
		mFileManager(fileManager)
		#ifdef RENDERER_GRAPHICS_DEBUGGER
			, mGraphicsDebugger(graphicsDebugger)
		#endif
		#ifdef RENDERER_PROFILER
			, mProfiler(profiler)
		#endif
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

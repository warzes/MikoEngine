#pragma once

#if RENDERER_OPENVR

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]


// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(5204)	// warning C5204: 'crnlib::task_pool::executable_task': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
	#include <OpenVR/openvr.h>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
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
	*    OpenVR runtime linking
	*/
	class OpenVRRuntimeLinking final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class VrManagerOpenVR;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderer
		*    Renderer instance to use
		*/
		explicit OpenVRRuntimeLinking(IRenderer& renderer);

		/**
		*  @brief
		*    Destructor
		*/
		~OpenVRRuntimeLinking();

		explicit OpenVRRuntimeLinking(const OpenVRRuntimeLinking&) = delete;
		OpenVRRuntimeLinking& operator=(const OpenVRRuntimeLinking&) = delete;

		/**
		*  @brief
		*    Return whether or not OpenVR is available
		*
		*  @return
		*    "true" if OpenVR is available, else "false"
		*/
		[[nodiscard]] bool isOpenVRAvaiable();

		/**
		*  @brief
		*    Load the shared libraries
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] bool loadSharedLibraries();

		/**
		*  @brief
		*    Load the OpenVR entry points
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] bool loadOpenVREntryPoints();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer& mRenderer;				// Renderer instance to use
		void*	   mOpenVRSharedLibrary;	// OpenVR shared library, can be a null pointer
		bool	   mEntryPointsRegistered;	// Entry points successfully registered?
		bool	   mInitialized;			// Already initialized?


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#endif // RENDERER_OPENVR

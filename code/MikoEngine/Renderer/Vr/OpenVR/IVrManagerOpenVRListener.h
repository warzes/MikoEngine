#pragma once


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
	class SceneNode;
	class SceneResource;
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
	*    Abstract OpenVR manager listener interface
	*/
	class IVrManagerOpenVRListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class VrManagerOpenVR;	// Is calling the protected interface methods


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IVrManagerOpenVRListener()
		{
			// Nothing here
		}

		inline virtual ~IVrManagerOpenVRListener()
		{
			// Nothing here
		}

		explicit IVrManagerOpenVRListener(const IVrManagerOpenVRListener&) = delete;
		IVrManagerOpenVRListener& operator=(const IVrManagerOpenVRListener&) = delete;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IVrManagerOpenVRListener methods ]
	//[-------------------------------------------------------]
	protected:
		inline virtual void onVrEvent([[maybe_unused]] const vr::VREvent_t& vrVrEvent)
		{
			// Nothing here
		}

		inline virtual void onSceneNodeCreated([[maybe_unused]] vr::TrackedDeviceIndex_t trackedDeviceIndex, [[maybe_unused]] SceneResource& sceneResource, [[maybe_unused]] SceneNode& sceneNode)
		{
			// Nothing here
		}


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

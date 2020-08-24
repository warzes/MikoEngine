#pragma once

namespace Renderer
{
	class CameraSceneItem;
}

class IController
{
public:

	inline virtual ~IController()
	{
		// Nothing here
	}

	[[nodiscard]] inline Renderer::CameraSceneItem& getCameraSceneItem()
	{
		return mCameraSceneItem;
	}

	/**
	*  @brief
	*    Return whether or not mouse control is currently actively used (e.g. for looking around)
	*
	*  @return
	*    "true" if the mouse control is currently actively used (e.g. for looking around), else "false"
	*
	*  @note
	*    - This can be used to avoid that while looking around with the mouse the mouse is becoming considered hovering over an GUI element
	*/
	[[nodiscard]] inline bool isMouseControlInProgress() const
	{
		return mMouseControlInProgress;
	}


	//[-------------------------------------------------------]
	//[ Public virtual IController methods                    ]
	//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Called on update request
	*
	*  @param[in] pastSecondsSinceLastFrame
	*    Past seconds since last frame
	*  @param[in] inputEnabled
	*    "true" if input is enabled, else "false"
	*/
	virtual void onUpdate(float pastSecondsSinceLastFrame, bool inputEnabled) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
protected:
	/**
	*  @brief
	*    Constructor
	*
	*  @param[in] cameraSceneItem
	*    Camera scene item to control, instance must stay valid as long as this controller instance exists
	*/
	inline explicit IController(Renderer::CameraSceneItem& cameraSceneItem) :
		mCameraSceneItem(cameraSceneItem),
		mMouseControlInProgress(false)
	{
		// Nothing here
	}

	explicit IController(const IController&) = delete;
	IController& operator=(const IController&) = delete;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
protected:
	Renderer::CameraSceneItem& mCameraSceneItem;
	bool					   mMouseControlInProgress;
};

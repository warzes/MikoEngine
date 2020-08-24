#pragma once

#include "IController.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace DeviceInput
{
	class InputManager;
	class VirtualStandardController;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Free camera controller
*/
class FreeCameraController final : public IController
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Constructor
	*
	*  @param[in] inputManager
	*    Owner input manager
	*  @param[in] cameraSceneItem
	*    Camera scene item to control, instance must stay valid as long as this controller instance exists
	*/
	FreeCameraController(DeviceInput::InputManager& inputManager, Renderer::CameraSceneItem& cameraSceneItem);

	/**
	*  @brief
	*    Destructor
	*/
	virtual ~FreeCameraController() override;


	//[-------------------------------------------------------]
	//[ Public virtual IController methods                    ]
	//[-------------------------------------------------------]
public:
	virtual void onUpdate(float pastSecondsSinceLastFrame, bool inputEnabled) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
private:
	explicit FreeCameraController(const FreeCameraController&) = delete;
	FreeCameraController& operator=(const FreeCameraController&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
private:
	DeviceInput::VirtualStandardController* mVirtualStandardController;
	float									mOriginalFovY;


};
#pragma once

#include "IController.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class LightSceneItem;
	class IRenderer;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual reality controller
*
*  @remarks
*    - Two controllers required
*    - The first VR controller is used for teleporting. A green light indicates the position one will end up. When pressing the trigger button one teleports to this position.
*    - The GUI is placed over the second VR controller
*/
class VrController final : public IController
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Constructor
	*
	*  @param[in] cameraSceneItem
	*    Camera scene item to control, instance must stay valid as long as this controller instance exists
	*/
	explicit VrController(Renderer::CameraSceneItem& cameraSceneItem);

	/**
	*  @brief
	*    Destructor
	*/
	virtual ~VrController() override;

	[[nodiscard]] const Renderer::LightSceneItem& getTeleportIndicationLightSceneItemSafe() const;


	//[-------------------------------------------------------]
	//[ Public virtual IController methods                    ]
	//[-------------------------------------------------------]
public:
	virtual void onUpdate(float pastSecondsSinceLastFrame, bool inputEnabled) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
private:
	explicit VrController(const VrController&) = delete;
	VrController& operator=(const VrController&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
private:
	const Renderer::IRenderer& mRenderer;
	Renderer::LightSceneItem*  mTeleportIndicationLightSceneItem;


};

#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/Manager.h"
#include "Renderer/Core/StringId.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rhi
{
	class IRenderTarget;
}
namespace Renderer
{
	class LightSceneItem;
	class CameraSceneItem;
	class CompositorWorkspaceInstance;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;			// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef StringId VrManagerTypeId;	// VR manager identifier, internally just a POD "uint32_t"
	typedef uint32_t SceneResourceId;	// POD scene resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class IVrManager : private Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;
		friend class CompositorWorkspaceInstance;	// Calls "Renderer::IVrManager::executeCompositorWorkspaceInstance()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class VrEye : int8_t
		{
			LEFT  =  0,
			RIGHT =  1
		};


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IVrManager methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual VrManagerTypeId getVrManagerTypeId() const = 0;

		/**
		*  @brief
		*    Check whether or not a head-mounted display (HMD) is present
		*
		*  @return
		*    "true" if a HMD is present, else "false" (OpenVR shared library not there? OpenVR runtime not installed? HMD not connected?)
		*
		*  @note
		*    - The VR manager is using OpenVR with runtime linking, so you need to ensure the OpenVR shared library
		*      can be loaded ("openvr_api.dll" under Microsoft Windows, "libopenvr_api.so" under Linux)
		*    - Method can also be used when the VR manager is not running
		*/
		[[nodiscard]] virtual bool isHmdPresent() const = 0;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual void setSceneResourceId(SceneResourceId sceneResourceId) = 0;
		[[nodiscard]] virtual bool startup(AssetId vrDeviceMaterialAssetId) = 0;	// If invalid material asset ID, no VR devices will be rendered
		[[nodiscard]] virtual bool isRunning() const = 0;
		virtual void shutdown() = 0;

		//[-------------------------------------------------------]
		//[ Transform (only valid if manager is running)          ]
		//[-------------------------------------------------------]
		virtual void updateHmdMatrixPose(CameraSceneItem* cameraSceneItem) = 0;
		[[nodiscard]] virtual glm::mat4 getHmdViewSpaceToClipSpaceMatrix(VrEye vrEye, float nearZ, float farZ) const = 0;
		[[nodiscard]] virtual glm::mat4 getHmdEyeSpaceToHeadSpaceMatrix(VrEye vrEye) const = 0;
		[[nodiscard]] virtual const glm::mat4& getHmdHeadSpaceToWorldSpaceMatrix() const = 0;
		[[nodiscard]] virtual const glm::mat4& getPreviousHmdHeadSpaceToWorldSpaceMatrix() const = 0;


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IVrManager methods          ]
	//[-------------------------------------------------------]
	private:
		//[-------------------------------------------------------]
		//[ Render (only valid if manager is running)             ]
		//[-------------------------------------------------------]
		virtual void executeCompositorWorkspaceInstance(CompositorWorkspaceInstance& compositorWorkspaceInstance, Rhi::IRenderTarget& renderTarget, CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IVrManager()
		{
			// Nothing here
		}

		inline virtual ~IVrManager()
		{
			// Nothing here
		}

		explicit IVrManager(const IVrManager&) = delete;
		IVrManager& operator=(const IVrManager&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

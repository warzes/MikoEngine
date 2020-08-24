#pragma once

#if RENDERER_OPENVR

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Vr/IVrManager.h"
#include "Renderer/Resource/IResourceListener.h"

#include "Rhi/Rhi.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(5204)	// warning C5204: 'crnlib::task_pool::executable_task': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
	#include <OpenVR/openvr.h>
SE_PRAGMA_WARNING_POP

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4774)	// warning C4774: 'sprintf_s' : format string expected in argument 3 is not a string literal
	#include <string>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class SceneNode;
	class IRenderer;
	class OpenVRRuntimeLinking;
	class IVrManagerOpenVRListener;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialResourceId;	// POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class VrManagerOpenVR final : public IVrManager, public IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("VrManagerOpenVR");
		typedef std::vector<std::string> RenderModelNames;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] static AssetId albedoTextureIdToAssetId(vr::TextureID_t albedoTextureId);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline IVrManagerOpenVRListener& getVrManagerOpenVRListener() const
		{
			// We know this pointer must always be valid
			RHI_ASSERT(nullptr != mVrManagerOpenVRListener, "Invalid VR manager OpenVR listener")
			return *mVrManagerOpenVRListener;
		}

		void setVrManagerOpenVRListener(IVrManagerOpenVRListener* vrManagerOpenVRListener);	// Does not take over the control of the memory

		[[nodiscard]] inline vr::IVRSystem* getVrSystem() const
		{
			return mVrSystem;
		}

		[[nodiscard]] inline MaterialResourceId getVrDeviceMaterialResourceId() const
		{
			return mVrDeviceMaterialResourceId;
		}

		[[nodiscard]] inline const RenderModelNames& getRenderModelNames() const
		{
			return mRenderModelNames;
		}

		[[nodiscard]] inline const vr::TrackedDevicePose_t& getVrTrackedDevicePose(vr::TrackedDeviceIndex_t trackedDeviceIndex) const
		{
			RHI_ASSERT(trackedDeviceIndex < vr::k_unMaxTrackedDeviceCount, "Maximum tracked device count exceeded")
			return mVrTrackedDevicePose[trackedDeviceIndex];
		}

		[[nodiscard]] inline const glm::mat4& getDevicePoseMatrix(vr::TrackedDeviceIndex_t trackedDeviceIndex) const
		{
			RHI_ASSERT(trackedDeviceIndex < vr::k_unMaxTrackedDeviceCount, "Maximum tracked device count exceeded")
			return mDevicePoseMatrix[trackedDeviceIndex];
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IVrManager methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual VrManagerTypeId getVrManagerTypeId() const override;
		[[nodiscard]] virtual bool isHmdPresent() const override;
		virtual void setSceneResourceId(SceneResourceId sceneResourceId) override;
		[[nodiscard]] virtual bool startup(AssetId vrDeviceMaterialAssetId) override;

		[[nodiscard]] inline virtual bool isRunning() const override
		{
			return (nullptr != mVrSystem);
		}

		virtual void shutdown() override;
		virtual void updateHmdMatrixPose(CameraSceneItem* cameraSceneItem) override;
		[[nodiscard]] virtual glm::mat4 getHmdViewSpaceToClipSpaceMatrix(VrEye vrEye, float nearZ, float farZ) const override;
		[[nodiscard]] virtual glm::mat4 getHmdEyeSpaceToHeadSpaceMatrix(VrEye vrEye) const override;

		[[nodiscard]] inline virtual const glm::mat4& getHmdHeadSpaceToWorldSpaceMatrix() const override
		{
			return mHmdHeadSpaceToWorldSpaceMatrix;
		}

		[[nodiscard]] inline virtual const glm::mat4& getPreviousHmdHeadSpaceToWorldSpaceMatrix() const override
		{
			return mPreviousHmdHeadSpaceToWorldSpaceMatrix;
		}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IVrManager methods          ]
	//[-------------------------------------------------------]
	private:
		virtual void executeCompositorWorkspaceInstance(CompositorWorkspaceInstance& compositorWorkspaceInstance, Rhi::IRenderTarget& renderTarget, CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem) override;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit VrManagerOpenVR(IRenderer& renderer);
		virtual ~VrManagerOpenVR() override;
		explicit VrManagerOpenVR(const VrManagerOpenVR&) = delete;
		VrManagerOpenVR& operator=(const VrManagerOpenVR&) = delete;
		void setupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Component final
		{
			std::string name;
			SceneNode* sceneNode = nullptr;	// TODO(co) No crazy raw-pointers
			Component(const std::string& _name, SceneNode* _sceneNode) :
				name(_name),
				sceneNode(_sceneNode)
			{};
		};
		typedef std::vector<Component> Components;
		struct TrackedDeviceInformation final
		{
			std::string renderModelName;
			Components  components;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&				   mRenderer;					// Renderer instance, do not destroy the instance
		IVrManagerOpenVRListener*  mVrManagerOpenVRListener;	// OpenVR manager listener, always valid, do not destroy the instance
		bool					   mVrDeviceMaterialResourceLoaded;
		MaterialResourceId		   mVrDeviceMaterialResourceId;
		SceneResourceId			   mSceneResourceId;
		SceneNode*				   mSceneNodes[vr::k_unMaxTrackedDeviceCount];	// TODO(co) No crazy raw-pointers
		TrackedDeviceInformation   mTrackedDeviceInformation[vr::k_unMaxTrackedDeviceCount];
		OpenVRRuntimeLinking*	   mOpenVRRuntimeLinking;
		vr::ETextureType		   mVrTextureType;
		vr::IVRSystem*			   mVrSystem;
		vr::IVRRenderModels*	   mVrRenderModels;
		RenderModelNames		   mRenderModelNames;
		bool					   mShowRenderModels;
		// Transform
		vr::TrackedDevicePose_t	mVrTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
		glm::mat4				mDevicePoseMatrix[vr::k_unMaxTrackedDeviceCount];
		uint32_t				mNumberOfValidDevicePoses;
		glm::mat4				mHmdHeadSpaceToWorldSpaceMatrix;
		glm::mat4				mPreviousHmdHeadSpaceToWorldSpaceMatrix;
		// RHI resources
		Rhi::ITexture2DPtr	 mColorTexture2D;	// Color 2D texture, can be a null pointer
		Rhi::IFramebufferPtr mFramebuffer;		// Framebuffer object (FBO), can be a null pointer


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#endif // RENDERER_OPENVR

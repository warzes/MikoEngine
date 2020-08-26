#include "stdafx.h"
#if RENDERER_OPENVR
#include "Renderer/Vr/OpenVR/VrManagerOpenVR.h"
#include "Renderer/Vr/OpenVR/OpenVRRuntimeLinking.h"
#include "Renderer/Vr/OpenVR/IVrManagerOpenVRListener.h"
#include "Renderer/Vr/OpenVR/Loader/OpenVRMeshResourceLoader.h"
#include "Renderer/Resource/Scene/SceneNode.h"
#include "Renderer/Resource/Scene/SceneResource.h"
#include "Renderer/Resource/Scene/SceneResourceManager.h"
#include "Renderer/Resource/Scene/Item/Mesh/MeshSceneItem.h"
#include "Renderer/Resource/Scene/Item/Camera/CameraSceneItem.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Asset/AssetPackage.h"
#include "Renderer/Asset/AssetManager.h"
#include "Renderer/Core/Math/Transform.h"
#include "Renderer/Core/Math/Math.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Context.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	SE_PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	#include <glm/gtx/matrix_decompose.hpp>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class VrManagerOpenVRListener : public Renderer::IVrManagerOpenVRListener
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			VrManagerOpenVRListener()
			{
				// Nothing here
			}

			virtual ~VrManagerOpenVRListener() override
			{
				// Nothing here
			}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			explicit VrManagerOpenVRListener(const VrManagerOpenVRListener&) = delete;
			VrManagerOpenVRListener& operator=(const VrManagerOpenVRListener&) = delete;


		};


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr uint32_t ASSET_PACKAGE_ID = SE_STRING_ID("Engine/OpenVR");


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		static VrManagerOpenVRListener defaultVrManagerOpenVRListener;


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] std::string getTrackedDeviceString(vr::IVRSystem& vrSystem, vr::TrackedDeviceIndex_t trackedDeviceIndex, vr::TrackedDeviceProperty trackedDeviceProperty, vr::TrackedPropertyError* trackedPropertyError = nullptr)
		{
			uint32_t requiredBufferLength = vrSystem.GetStringTrackedDeviceProperty(trackedDeviceIndex, trackedDeviceProperty, nullptr, 0, trackedPropertyError);
			if (0 == requiredBufferLength)
			{
				return "";
			}

			char* temp = new char[requiredBufferLength];
			vrSystem.GetStringTrackedDeviceProperty(trackedDeviceIndex, trackedDeviceProperty, temp, requiredBufferLength, trackedPropertyError);
			std::string result = temp;
			delete [] temp;
			return result;
		}

		[[nodiscard]] std::string getRenderModelName(uint32_t renderModelIndex)
		{
			vr::IVRRenderModels* vrRenderModels = vr::VRRenderModels();

			uint32_t requiredBufferLength = vrRenderModels->GetRenderModelName(renderModelIndex, nullptr, 0);
			if (0 == requiredBufferLength)
			{
				return "";
			}

			char* temp = new char[requiredBufferLength];
			vrRenderModels->GetRenderModelName(renderModelIndex, temp, requiredBufferLength);
			std::string result = temp;
			delete [] temp;
			return result;
		}

		[[nodiscard]] std::string getRenderModelComponentName(const std::string& renderModelName, uint32_t componentIndex)
		{
			vr::IVRRenderModels* vrRenderModels = vr::VRRenderModels();

			uint32_t requiredBufferLength = vrRenderModels->GetComponentName(renderModelName.c_str(), componentIndex, nullptr, 0);
			if (0 == requiredBufferLength)
			{
				return "";
			}

			char* temp = new char[requiredBufferLength];
			vrRenderModels->GetComponentName(renderModelName.c_str(), componentIndex, temp, requiredBufferLength);
			std::string result = temp;
			delete [] temp;
			return result;
		}

		[[nodiscard]] std::string getRenderModelComponentRenderModelName(const std::string& renderModelName, const std::string& componentName)
		{
			vr::IVRRenderModels* vrRenderModels = vr::VRRenderModels();

			uint32_t requiredBufferLength = vrRenderModels->GetComponentRenderModelName(renderModelName.c_str(), componentName.c_str(), nullptr, 0);
			if (0 == requiredBufferLength)
			{
				return "";
			}

			char* temp = new char[requiredBufferLength];
			vrRenderModels->GetComponentRenderModelName(renderModelName.c_str(), componentName.c_str(), temp, requiredBufferLength);
			std::string result = temp;
			delete [] temp;
			return result;
		}

		void registerRenderModel(const Renderer::Context& context, const std::string& renderModelName, Renderer::VrManagerOpenVR::RenderModelNames& renderModelNames, Renderer::AssetPackage& assetPackage)
		{
			// Some render models might share component render models, so we need to ensure the render model asset ID isn't registered, yet
			const Renderer::AssetId assetId(renderModelName.c_str());
			if (nullptr == assetPackage.tryGetAssetByAssetId(assetId))
			{
				assetPackage.addAsset(context, assetId, std::to_string(renderModelNames.size()).c_str());
				renderModelNames.push_back(renderModelName);
			}
		}

		[[nodiscard]] glm::mat4 convertOpenVrMatrixToGlmMat34(const vr::HmdMatrix34_t& vrHmdMatrix34)
		{
			// Transform the OpenGL style transform matrix into a Direct3D style transform matrix as described at http://cv4mar.blogspot.de/2009/03/transformation-matrices-between-opengl.html
			// -> Direct3D: Left-handed coordinate system
			// -> OpenGL without "GL_ARB_clip_control"-extension: Right-handed coordinate system
			return glm::mat4(
				 vrHmdMatrix34.m[0][0],  vrHmdMatrix34.m[1][0], -vrHmdMatrix34.m[2][0], 0.0f,
				 vrHmdMatrix34.m[0][1],  vrHmdMatrix34.m[1][1], -vrHmdMatrix34.m[2][1], 0.0f,
				-vrHmdMatrix34.m[0][2], -vrHmdMatrix34.m[1][2],  vrHmdMatrix34.m[2][2], 0.0f,
				 vrHmdMatrix34.m[0][3],  vrHmdMatrix34.m[1][3], -vrHmdMatrix34.m[2][3], 1.0f
				);
		}

		void createMeshSceneItem(Renderer::SceneResource& sceneResource, Renderer::SceneNode& sceneNode, const std::string& renderModelName)
		{
			// Check whether or not we need to generate the mesh asset right now
			Renderer::MeshResourceId meshResourceId = Renderer::getInvalid<Renderer::MeshResourceId>();
			sceneResource.getRenderer().getMeshResourceManager().loadMeshResourceByAssetId(Renderer::AssetId(renderModelName.c_str()), meshResourceId, nullptr, false, Renderer::OpenVRMeshResourceLoader::TYPE_ID);

			// Create mesh scene item
			Renderer::MeshSceneItem* meshSceneItem = sceneResource.createSceneItem<Renderer::MeshSceneItem>(sceneNode);
			if (nullptr != meshSceneItem)
			{
				meshSceneItem->setMeshResourceId(meshResourceId);
			}
		}

		void setSceneNodesVisible(Renderer::SceneNode* sceneNodes[vr::k_unMaxTrackedDeviceCount], bool visible)
		{
			for (uint32_t i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i)
			{
				Renderer::SceneNode* sceneNode = sceneNodes[i];
				if (nullptr != sceneNode)
				{
					sceneNode->setVisible(visible);
				}
			}
		}

		/**
		*  @brief
		*    Split the Vulkan extension string given by OpenVR; the extension names are separated by space
		*/
		void splitOpenVrVulkanExtensionString(const std::vector<char>& extensionString, std::vector<std::string>& outInstanceExtensionList)
		{
			// Break up the space separated list into entries
			std::string currentExtensionString;
			size_t index = 0;
			const size_t bufferSize = extensionString.size();
			while ((index < bufferSize) && 0 != extensionString[index])
			{
				if (' ' == extensionString[index])
				{
					// Do move instead of a copy we don't need the content anymore
					outInstanceExtensionList.emplace_back(std::move(currentExtensionString));
				}
				else
				{
					currentExtensionString += extensionString[index];
				}
				++index;
			}
			if (!currentExtensionString.empty())
			{
				// Do move instead of a copy we don't need the content anymore
				outInstanceExtensionList.emplace_back(std::move(currentExtensionString));
			}
		}

		/**
		*  @brief
		*    Ask OpenVR for the list of Vulkan instance extensions required
		*
		*  @note
		*    - Basing on https://github.com/ValveSoftware/openvr/blob/master/samples/hellovr_vulkan/hellovr_vulkan_main.cpp
		*    - See also https://github.com/ValveSoftware/openvr/wiki/Vulkan
		*/
		[[nodiscard]] bool getVulkanInstanceExtensionsRequired(std::vector<std::string>& outInstanceExtensionList)
		{
			if (!vr::VRCompositor())
			{
				// Error!
				return false;
			}

			outInstanceExtensionList.clear();
			const uint32_t bufferSize = vr::VRCompositor()->GetVulkanInstanceExtensionsRequired(nullptr, 0);
			if (bufferSize > 0)
			{
				// Allocate memory for the space separated list and query for it
				// TODO(sw) with c++17 we could use std::string which contains a non const data method like std::vector
				std::vector<char> extensionString;
				extensionString.resize(bufferSize, 0);
				vr::VRCompositor()->GetVulkanInstanceExtensionsRequired(extensionString.data(), bufferSize);

				// Break up the space separated list into entries
				splitOpenVrVulkanExtensionString(extensionString, outInstanceExtensionList);
			}

			// Done
			return true;
		}

		/**
		*  @brief
		*    Ask OpenVR for the list of Vulkan device extensions required
		*
		*  @note
		*    - Basing on https://github.com/ValveSoftware/openvr/blob/master/samples/hellovr_vulkan/hellovr_vulkan_main.cpp
		*    - See also https://github.com/ValveSoftware/openvr/wiki/Vulkan
		*/
		[[nodiscard]] bool getVulkanDeviceExtensionsRequired(VkPhysicalDevice_T* vkPhysicalDevice_T, std::vector<std::string>& outDeviceExtensionList)
		{
			if (!vr::VRCompositor())
			{
				// Error!
				return false;
			}

			outDeviceExtensionList.clear();
			const uint32_t bufferSize = vr::VRCompositor()->GetVulkanDeviceExtensionsRequired(vkPhysicalDevice_T, nullptr, 0);
			if (bufferSize > 0)
			{
				// Allocate memory for the space separated list and query for it
				// TODO(sw) with c++17 we could use std::string which contains a non const data method like std::vector
				std::vector<char> extensionString;
				extensionString.resize(bufferSize, 0);
				vr::VRCompositor()->GetVulkanDeviceExtensionsRequired(vkPhysicalDevice_T, extensionString.data(), bufferSize);

				// Break up the space separated list into entries
				splitOpenVrVulkanExtensionString(extensionString, outDeviceExtensionList);
			}

			// Done
			return true;
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
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	AssetId VrManagerOpenVR::albedoTextureIdToAssetId(vr::TextureID_t albedoTextureId)
	{
		const std::string albedoTextureName = "OpenVR_" + std::to_string(albedoTextureId);
		return StringId(albedoTextureName.c_str());
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void VrManagerOpenVR::setVrManagerOpenVRListener(IVrManagerOpenVRListener* vrManagerOpenVRListener)
	{
		// There must always be a valid VR manager OpenVR listener instance
		mVrManagerOpenVRListener = (nullptr != vrManagerOpenVRListener) ? vrManagerOpenVRListener : &::detail::defaultVrManagerOpenVRListener;
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IVrManager methods           ]
	//[-------------------------------------------------------]
	VrManagerTypeId VrManagerOpenVR::getVrManagerTypeId() const
	{
		return TYPE_ID;
	}

	bool VrManagerOpenVR::isHmdPresent() const
	{
		return (mOpenVRRuntimeLinking->isOpenVRAvaiable() && vr::VR_IsRuntimeInstalled() && vr::VR_IsHmdPresent());
	}

	void VrManagerOpenVR::setSceneResourceId(SceneResourceId sceneResourceId)
	{
		// TODO(co) Decent implementation so it's no problem to change the scene resource at any time
		mSceneResourceId = sceneResourceId;
	}

	bool VrManagerOpenVR::startup(AssetId vrDeviceMaterialAssetId)
	{
		SE_ASSERT(nullptr == mVrSystem, "The VR system is already running")
		if (nullptr == mVrSystem)
		{
			// TODO(co) Add support for "vr::TextureType_DirectX12" as soon as the RHI implementation is ready
			Rhi::IRhi& rhi = mRenderer.getRhi();
			switch (rhi.getNameId())
			{
				case Rhi::NameId::VULKAN:
					mVrTextureType = vr::TextureType_Vulkan;
					break;

				case Rhi::NameId::OPENGL:
					mVrTextureType = vr::TextureType_OpenGL;
					break;

				case Rhi::NameId::DIRECT3D11:
					mVrTextureType = vr::TextureType_DirectX;
					break;

				case Rhi::NameId::DIRECT3D12:
				case Rhi::NameId::OPENGLES3:
				case Rhi::NameId::NULL_DUMMY:
				default:
					// Error!
					RHI_LOG(CRITICAL, "The renderer VR OpenVR manager currently only supports Vulkan, OpenGL and Direct3D 11")
					return false;
			}

			// Initialize the OpenVR system
			vr::EVRInitError vrInitError = vr::VRInitError_None;
			mVrSystem = vr::VR_Init(&vrInitError, vr::VRApplication_Scene);
			if (vr::VRInitError_None != vrInitError)
			{
				// Error!
				RHI_LOG(CRITICAL, "The renderer was unable to initialize OpenVR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(vrInitError))
				return false;
			}
			if (vr::TextureType_Vulkan == mVrTextureType)
			{
				// TODO(co) Vulkan support: Have a look into "GetVulkanInstanceExtensionsRequired()" and "GetVulkanDeviceExtensionsRequired()",
				//          see https://github.com/ValveSoftware/openvr/wiki/Vulkan and https://github.com/ValveSoftware/openvr/blob/master/samples/hellovr_vulkan/hellovr_vulkan_main.cpp
				//          The stuff here is currently just for debugging to check whether or not locally extensions are needed.
				std::vector<std::string> outInstanceExtensionList;
				if (!::detail::getVulkanInstanceExtensionsRequired(outInstanceExtensionList) || !outInstanceExtensionList.empty())
				{
					RHI_LOG(CRITICAL, "OpenVR needs Vulkan instance extensions which are currently not supported")
					outInstanceExtensionList.clear();
				}
				/*
				VkPhysicalDevice_T* vkPhysicalDevice_T = TODO(co);
				::detail::getVulkanDeviceExtensionsRequired(vkPhysicalDevice_T, outInstanceExtensionList);
				if (!outInstanceExtensionList.empty())
				{
					RHI_LOG(CRITICAL, "OpenVR needs Vulkan device extensions which are currently not supported")
				}
				*/
			}

			// Get the OpenVR render models interface
			mVrRenderModels = static_cast<vr::IVRRenderModels*>(vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &vrInitError));
			if (nullptr == mVrRenderModels)
			{
				// De-initialize the OpenVR system
				vr::VR_Shutdown();
				mVrSystem = nullptr;

				// Error!
				RHI_LOG(CRITICAL, "The renderer was unable to retrieve the OpenVR render models interface: %s", vr::VR_GetVRInitErrorAsEnglishDescription(vrInitError))
				return false;
			}

			// Try to load the device material resource material
			mVrDeviceMaterialResourceLoaded = false;
			mVrDeviceMaterialResourceId = getInvalid<MaterialResourceId>();
			if (isValid(vrDeviceMaterialAssetId))
			{
				mRenderer.getMaterialResourceManager().loadMaterialResourceByAssetId(vrDeviceMaterialAssetId, mVrDeviceMaterialResourceId, this);
			}

			{ // Create RHI resources
				// Create the texture instance
				uint32_t width = 0;
				uint32_t height = 0;
				mVrSystem->GetRecommendedRenderTargetSize(&width, &height);
				width *= 2;	// Twice the width for single pass stereo rendering via instancing as described in "High Performance Stereo Rendering For VR", Timothy Wilson, San Diego, Virtual Reality Meetup
				const Rhi::TextureFormat::Enum textureFormat = Rhi::TextureFormat::Enum::R8G8B8A8;
				mColorTexture2D = mRenderer.getTextureManager().createTexture2D(width, height, textureFormat, nullptr, Rhi::TextureFlag::SHADER_RESOURCE | Rhi::TextureFlag::RENDER_TARGET, Rhi::TextureUsage::DEFAULT, 1, nullptr RHI_RESOURCE_DEBUG_NAME("OpenVR color render target"));
				Rhi::ITexture* depthStencilTexture2D = mRenderer.getTextureManager().createTexture2D(width, height, Rhi::TextureFormat::D32_FLOAT, nullptr, Rhi::TextureFlag::SHADER_RESOURCE | Rhi::TextureFlag::RENDER_TARGET, Rhi::TextureUsage::DEFAULT, 1, nullptr RHI_RESOURCE_DEBUG_NAME("OpenVR depth stencil render target"));

				// Create the framebuffer object (FBO) instance
				const Rhi::FramebufferAttachment colorFramebufferAttachment(mColorTexture2D);
				const Rhi::FramebufferAttachment depthStencilFramebufferAttachment(depthStencilTexture2D);
				mFramebuffer = rhi.createFramebuffer(*rhi.createRenderPass(1, &textureFormat, Rhi::TextureFormat::UNKNOWN, 1 RHI_RESOURCE_DEBUG_NAME("OpenVR")), &colorFramebufferAttachment, &depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME("OpenVR"));
			}

			{ // Add dynamic OpenVR asset package
				AssetPackage& assetPackage = mRenderer.getAssetManager().addAssetPackage(::detail::ASSET_PACKAGE_ID);
				const Context& context = mRenderer.getContext();

				// Register render models
				// -> OpenVR render model names can get awful long due to absolute path information, so, we need to store them inside a separate list and tell the asset just about the render model name index
				const uint32_t renderModelCount = mVrRenderModels->GetRenderModelCount();
				for (uint32_t renderModelIndex = 0; renderModelIndex < renderModelCount; ++renderModelIndex)
				{
					std::string renderModelName = ::detail::getRenderModelName(renderModelIndex);
					const uint32_t componentCount = mVrRenderModels->GetComponentCount(renderModelName.c_str());
					if (componentCount > 0)
					{
						for (uint32_t componentIndex = 0; componentIndex < componentCount; ++componentIndex)
						{
							const std::string componentName = ::detail::getRenderModelComponentName(renderModelName, componentIndex);
							const std::string componentRenderModelName = ::detail::getRenderModelComponentRenderModelName(renderModelName, componentName);
							if (!componentRenderModelName.empty())
							{
								::detail::registerRenderModel(context, componentRenderModelName, mRenderModelNames, assetPackage);
							}
						}
					}
					else
					{
						::detail::registerRenderModel(context, renderModelName, mRenderModelNames, assetPackage);
					}
				}
				SE_ASSERT(assetPackage.getSortedAssetVector().size() == mRenderModelNames.size(), "Size mismatch")

				// Register render model textures
				// -> Sadly, there's no way to determine all available albedo texture IDs upfront without loading the render models
				// -> We assume, that albedo texture IDs are linear
				for (uint32_t renderModelIndex = 0; renderModelIndex < renderModelCount; ++renderModelIndex)
				{
					assetPackage.addAsset(context, VrManagerOpenVR::albedoTextureIdToAssetId(static_cast<vr::TextureID_t>(renderModelIndex)), std::to_string(renderModelIndex).c_str());
				}
			}

			// TODO(co) Optionally mirror the result on the given render target
			vr::VRCompositor()->ShowMirrorWindow();
		}

		// Done
		return true;
	}

	void VrManagerOpenVR::shutdown()
	{
		if (nullptr != mVrSystem)
		{
			// Remove dynamic OpenVR asset package
			mRenderer.getAssetManager().removeAssetPackage(::detail::ASSET_PACKAGE_ID);
			mRenderModelNames.clear();

			// De-initialize the OpenVR system
			vr::VR_Shutdown();
			mVrSystem = nullptr;

			// Release RHI resources
			mFramebuffer = nullptr;
			mColorTexture2D = nullptr;
		}
	}

	void VrManagerOpenVR::updateHmdMatrixPose(CameraSceneItem* cameraSceneItem)
	{
		// Sanity check
		SE_ASSERT(nullptr != mVrSystem, "The VR system isn't running")

		// Process OpenVR events
		if (mVrDeviceMaterialResourceLoaded)
		{
			vr::VREvent_t vrVrEvent;
			while (mVrSystem->PollNextEvent(&vrVrEvent, sizeof(vr::VREvent_t)))
			{
				switch (vrVrEvent.eventType)
				{
					case vr::VREvent_TrackedDeviceActivated:
						setupRenderModelForTrackedDevice(vrVrEvent.trackedDeviceIndex);
						break;

					// Sent to the scene application to request hiding render models temporarily
					case vr::VREvent_HideRenderModels:
						mShowRenderModels = false;
						::detail::setSceneNodesVisible(mSceneNodes, mShowRenderModels);
						break;

					// Sent to the scene application to request restoring render model visibility
					case vr::VREvent_ShowRenderModels:
						mShowRenderModels = true;
						::detail::setSceneNodesVisible(mSceneNodes, mShowRenderModels);
						break;
				}

				// Tell the world
				mVrManagerOpenVRListener->onVrEvent(vrVrEvent);
			}
		}

		// Request poses from OpenVR
		vr::VRCompositor()->WaitGetPoses(mVrTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

		// Everything must be relative to the 64 bit world space position of the camera
		const glm::dvec3& cameraPosition = (nullptr != cameraSceneItem && nullptr != cameraSceneItem->getParentSceneNode()) ? cameraSceneItem->getParentSceneNode()->getGlobalTransform().position : Math::DVEC3_ZERO;

		// Don't draw controllers if somebody else has input focus
		const bool showControllers = (mShowRenderModels && mVrSystem->IsInputAvailable());

		// Gather all valid poses
		mNumberOfValidDevicePoses = 0;
		for (uint32_t deviceIndex = 0; deviceIndex < vr::k_unMaxTrackedDeviceCount; ++deviceIndex)
		{
			if (mVrTrackedDevicePose[deviceIndex].bPoseIsValid)
			{
				++mNumberOfValidDevicePoses;
				const glm::mat4& devicePoseMatrix = mDevicePoseMatrix[deviceIndex] = ::detail::convertOpenVrMatrixToGlmMat34(mVrTrackedDevicePose[deviceIndex].mDeviceToAbsoluteTracking);
				SceneNode* sceneNode = mSceneNodes[deviceIndex];
				if (nullptr != sceneNode)
				{
					// Transform data
					glm::vec3 scale;
					glm::quat rotation;
					glm::vec3 translation;
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(devicePoseMatrix, scale, rotation, translation, skew, perspective);

					// Tell the scene node about the new position and rotation, scale doesn't change
					sceneNode->setPositionRotation(glm::dvec3(translation) + cameraPosition, rotation);

					// Show/hide scene node
					sceneNode->setVisible(showControllers);
				}
			}
		}

		// Update render model components so we can see e.g. controller trigger animations
		for (uint32_t deviceIndex = 0; deviceIndex < vr::k_unMaxTrackedDeviceCount; ++deviceIndex)
		{
			const TrackedDeviceInformation& trackedDeviceInformation = mTrackedDeviceInformation[deviceIndex];
			if (!trackedDeviceInformation.renderModelName.empty() && !trackedDeviceInformation.components.empty())
			{
				vr::VRControllerState_t vrControllerState;
				if (mVrSystem->GetControllerState(deviceIndex, &vrControllerState, sizeof(vr::VRControllerState_t)))
				{
					vr::IVRRenderModels* vrRenderModels = vr::VRRenderModels();
					for (const Component& component : trackedDeviceInformation.components)
					{
						SE_ASSERT(!component.name.empty(), "The component name mustn't be empty")
						SceneNode* sceneNode = component.sceneNode;
						SE_ASSERT(nullptr != sceneNode, "Invalid scene node")
						vr::RenderModel_ControllerMode_State_t renderModelControllerModeState;
						renderModelControllerModeState.bScrollWheelVisible = false;
						vr::RenderModel_ComponentState_t renderModelComponentState;
						if (vrRenderModels->GetComponentState(trackedDeviceInformation.renderModelName.c_str(), component.name.c_str(), &vrControllerState, &renderModelControllerModeState, &renderModelComponentState))
						{
							sceneNode->setTransform(Transform(::detail::convertOpenVrMatrixToGlmMat34(renderModelComponentState.mTrackingToComponentRenderModel)));
							sceneNode->setVisible((renderModelComponentState.uProperties & vr::VRComponentProperty_IsVisible) != 0);
						}
					}
				}
			}
		}

		// Backup head space to world space
		if (mVrTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
		{
			mPreviousHmdHeadSpaceToWorldSpaceMatrix = mHmdHeadSpaceToWorldSpaceMatrix;	// First update isn't correct, but probably no one will notice
			mHmdHeadSpaceToWorldSpaceMatrix = mDevicePoseMatrix[vr::k_unTrackedDeviceIndex_Hmd];
		}

		// Update camera scene node transform and hide the HMD scene node in case it's currently used as the camera scene node (we don't want to see the HMD mesh from the inside)
		SceneNode* hmdSceneNode = mSceneNodes[vr::k_unTrackedDeviceIndex_Hmd];
		if (nullptr != hmdSceneNode)
		{
			bool hmdSceneNodeVisible = true;
			if (nullptr != cameraSceneItem)
			{
				SceneNode* sceneNode = cameraSceneItem->getParentSceneNode();
				if (nullptr != sceneNode)
				{
				//	sceneNode->setTransform(hmdSceneNode->getGlobalTransform());	// TODO(co)
					hmdSceneNodeVisible = false;
				}
			}
			hmdSceneNode->setVisible(hmdSceneNodeVisible);
		}
	}

	glm::mat4 VrManagerOpenVR::getHmdViewSpaceToClipSpaceMatrix(VrEye vrEye, float nearZ, float farZ) const
	{
		// Sanity check
		SE_ASSERT(nullptr != mVrSystem, "The VR system isn't running")

		// Transform the OpenGL style projection matrix into a Direct3D style projection matrix as described at http://cv4mar.blogspot.de/2009/03/transformation-matrices-between-opengl.html
		// -> Direct3D: Left-handed coordinate system with clip space depth value range 0..1
		// -> OpenGL without "GL_ARB_clip_control"-extension: Right-handed coordinate system with clip space depth value range -1..1
		const vr::HmdMatrix44_t vrHmdMatrix34 = mVrSystem->GetProjectionMatrix(static_cast<vr::Hmd_Eye>(vrEye), nearZ, farZ);
		return glm::mat4(
			 vrHmdMatrix34.m[0][0],  vrHmdMatrix34.m[1][0],  vrHmdMatrix34.m[2][0],  vrHmdMatrix34.m[3][0],
			 vrHmdMatrix34.m[0][1],  vrHmdMatrix34.m[1][1],  vrHmdMatrix34.m[2][1],  vrHmdMatrix34.m[3][1],
			-vrHmdMatrix34.m[0][2], -vrHmdMatrix34.m[1][2], -vrHmdMatrix34.m[2][2], -vrHmdMatrix34.m[3][2],
			 vrHmdMatrix34.m[0][3],  vrHmdMatrix34.m[1][3],  vrHmdMatrix34.m[2][3],  vrHmdMatrix34.m[3][3]
			);
	}

	glm::mat4 VrManagerOpenVR::getHmdEyeSpaceToHeadSpaceMatrix(VrEye vrEye) const
	{
		SE_ASSERT(nullptr != mVrSystem, "The VR system isn't running")
		return ::detail::convertOpenVrMatrixToGlmMat34(mVrSystem->GetEyeToHeadTransform(static_cast<vr::Hmd_Eye>(vrEye)));
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IVrManager methods          ]
	//[-------------------------------------------------------]
	void VrManagerOpenVR::executeCompositorWorkspaceInstance(CompositorWorkspaceInstance& compositorWorkspaceInstance, Rhi::IRenderTarget&, CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem)
	{
		// Sanity check
		SE_ASSERT(nullptr != mVrSystem, "The VR system isn't running")

		// Execute the compositor workspace instance
		// -> Using single pass stereo rendering via instancing as described in "High Performance Stereo Rendering For VR", Timothy Wilson, San Diego, Virtual Reality Meetup
		compositorWorkspaceInstance.execute(*mFramebuffer, cameraSceneItem, lightSceneItem, true);

		{ // Submit the rendered texture to the OpenVR compositor
			const vr::Texture_t vrTexture = { mColorTexture2D->getInternalResourceHandle(), mVrTextureType, vr::ColorSpace_Auto };
			static constexpr vr::VRTextureBounds_t leftEyeVrTextureBounds{0.0f, 0.0f, 0.5f, 1.0f};
			vr::VRCompositor()->Submit(vr::Eye_Left, &vrTexture, &leftEyeVrTextureBounds);
			static constexpr vr::VRTextureBounds_t rightEyeVrTextureBounds{0.5f, 0.0f, 1.0f, 1.0f};
			vr::VRCompositor()->Submit(vr::Eye_Right, &vrTexture, &rightEyeVrTextureBounds);
		}

		// Tell the compositor to begin work immediately instead of waiting for the next "vr::IVRCompositor::WaitGetPoses()" call
		vr::VRCompositor()->PostPresentHandoff();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	void VrManagerOpenVR::onLoadingStateChange(const IResource& resource)
	{
		if (mVrDeviceMaterialResourceId == resource.getId() && resource.getLoadingState() == IResource::LoadingState::LOADED)
		{
			mVrDeviceMaterialResourceLoaded = true;

			// Setup all render models for tracked devices
			for (uint32_t trackedDeviceIndex = vr::k_unTrackedDeviceIndex_Hmd; trackedDeviceIndex < vr::k_unMaxTrackedDeviceCount; ++trackedDeviceIndex)
			{
				if (mVrSystem->IsTrackedDeviceConnected(trackedDeviceIndex))
				{
					setupRenderModelForTrackedDevice(trackedDeviceIndex);
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	VrManagerOpenVR::VrManagerOpenVR(IRenderer& renderer) :
		mRenderer(renderer),
		mVrManagerOpenVRListener(&::detail::defaultVrManagerOpenVRListener),
		mVrDeviceMaterialResourceLoaded(false),
		mVrDeviceMaterialResourceId(getInvalid<MaterialResourceId>()),
		mSceneResourceId(getInvalid<SceneResourceId>()),
		mSceneNodes{},
		mOpenVRRuntimeLinking(new OpenVRRuntimeLinking(renderer)),
		mVrTextureType(vr::TextureType_OpenGL),
		mVrSystem(nullptr),
		mVrRenderModels(nullptr),
		mShowRenderModels(true),
		mNumberOfValidDevicePoses(0),
		mHmdHeadSpaceToWorldSpaceMatrix(Math::MAT4_IDENTITY),
		mPreviousHmdHeadSpaceToWorldSpaceMatrix(Math::MAT4_IDENTITY)
	{
		for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i)
		{
			mDevicePoseMatrix[i] = Math::MAT4_IDENTITY;
		}
	}

	VrManagerOpenVR::~VrManagerOpenVR()
	{
		shutdown();
		delete mOpenVRRuntimeLinking;
	}

	void VrManagerOpenVR::setupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t trackedDeviceIndex)
	{
		SE_ASSERT(trackedDeviceIndex < vr::k_unMaxTrackedDeviceCount, "Maximum tracked device count exceeded")

		// Create and setup scene node with mesh item, this is what's controlled during runtime
		SceneResource* sceneResource = mRenderer.getSceneResourceManager().tryGetById(mSceneResourceId);
		if (nullptr != sceneResource)
		{
			// Create scene node
			SceneNode* sceneNode = mSceneNodes[trackedDeviceIndex] = sceneResource->createSceneNode(Transform::IDENTITY);
			if (nullptr != sceneNode)
			{
				TrackedDeviceInformation& trackedDeviceInformation = mTrackedDeviceInformation[trackedDeviceIndex];
				trackedDeviceInformation.components.clear();

				// Get the render model name
				trackedDeviceInformation.renderModelName = ::detail::getTrackedDeviceString(*mVrSystem, trackedDeviceIndex, vr::Prop_RenderModelName_String);
				const std::string& renderModelName = trackedDeviceInformation.renderModelName;

				// In case the render model has components, don't use the render model directly, use its components instead so we can animate e.g. the controller trigger
				vr::IVRRenderModels* vrRenderModels = vr::VRRenderModels();
				const uint32_t componentCount = vrRenderModels->GetComponentCount(renderModelName.c_str());
				vr::VRControllerState_t vrControllerState;
				if (componentCount > 0 && mVrSystem->GetControllerState(trackedDeviceIndex, &vrControllerState, sizeof(vr::VRControllerState_t)))
				{
					for (uint32_t componentIndex = 0; componentIndex < componentCount; ++componentIndex)
					{
						const std::string componentName = ::detail::getRenderModelComponentName(renderModelName, componentIndex);
						const std::string componentRenderModelName = ::detail::getRenderModelComponentRenderModelName(renderModelName, componentName);
						if (!componentRenderModelName.empty())
						{
							// Get component state
							vr::RenderModel_ControllerMode_State_t renderModelControllerModeState;
							renderModelControllerModeState.bScrollWheelVisible = false;
							vr::RenderModel_ComponentState_t renderModelComponentState;
							if (vrRenderModels->GetComponentState(renderModelName.c_str(), componentName.c_str(), &vrControllerState, &renderModelControllerModeState, &renderModelComponentState))
							{
								// Create the scene node of the component
								SceneNode* componentSceneNode = sceneResource->createSceneNode(Transform(::detail::convertOpenVrMatrixToGlmMat34(renderModelComponentState.mTrackingToComponentRenderModel)));
								if (nullptr != componentSceneNode)
								{
									sceneNode->attachSceneNode(*componentSceneNode);
									::detail::createMeshSceneItem(*sceneResource, *componentSceneNode, componentRenderModelName);
									componentSceneNode->setVisible((renderModelComponentState.uProperties & vr::VRComponentProperty_IsVisible) != 0);
									trackedDeviceInformation.components.emplace_back(std::move(componentName), componentSceneNode);
								}
							}
						}
					}
				}
				else
				{
					::detail::createMeshSceneItem(*sceneResource, *sceneNode, renderModelName);
				}

				// Tell the world
				mVrManagerOpenVRListener->onSceneNodeCreated(trackedDeviceIndex, *sceneResource, *sceneNode);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
#endif // RENDERER_OPENVR

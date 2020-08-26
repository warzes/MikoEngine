#pragma once

#include <MikoEngine/MikoHeader.h>
#include <Math/Transform.h>
#include <Renderer/DebugGui/DebugGuiHelper.h>
#include <Renderer/Resource/IResourceListener.h>
#include "MikoEngine/Renderer/Core/Time/Stopwatch.h"
#include "MikoEngine/Renderer/Resource/IResourceListener.h"
#include "MikoEngine/Core/File/IFileManager.h"
#include "MikoEngine/Renderer/RendererInstance.h"
#include "MikoEngine/Renderer/Core/IProfiler.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class IController;
typedef struct ini_t ini_t;
namespace DeviceInput
{
	class InputManager;
}
namespace Renderer
{
	class ImGuiLog;
	class SceneNode;
	class CameraSceneItem;
	class SunlightSceneItem;
	class SkeletonMeshSceneItem;
	class CompositorWorkspaceInstance;
}

//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
namespace Renderer
{
	typedef uint32_t SceneResourceId;		///< POD scene resource identifier
	typedef uint32_t MaterialResourceId;	///< POD material resource identifier
}

/**
*  @brief
*    Scene example
*
*  @remarks
*    Demonstrates:
*    - Compositor
*    - Scene
*    - Virtual reality (VR)
*/
class Scene : public Application, public Renderer::IResourceListener
{
public:
	Scene();
	~Scene();
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
	Renderer::IRenderer* getRenderer() const;

	virtual void onLoadingStateChange(const Renderer::IResource& resource) override;

	void loadIni();
	void saveIni();
	void destroyIni();
	void applyCurrentSettings(Rhi::IRenderTarget& mainRenderTarget);
	void createCompositorWorkspace();
	void createDebugGui(Rhi::IRenderTarget& mainRenderTarget);
	void trySetCustomMaterialResource();
	void onDraw();

	IFileManager*		mFileManager;
#ifdef RENDERER_PROFILER
	Renderer::IProfiler*	mProfiler;			///< Profiler instance, can be a null pointer
#endif
	Renderer::Context*			mRendererContext;	///< Renderer context instance, can be a null pointer
	Renderer::RendererInstance* mRendererInstance;	///< Renderer instance, can be a null pointer
#ifdef RENDERER_GRAPHICS_DEBUGGER
	Renderer::IGraphicsDebugger* mGraphicsDebugger;	///< Graphics debugger instance, can be a null pointer
#endif


	enum class Msaa
	{
		NONE,
		TWO,
		FOUR,
		EIGHT
	};
	enum class Compositor
	{
		DEBUG,
		FORWARD,
		DEFERRED,
		VR
	};
	enum class ShadowQuality
	{
		NONE,
		LOW,
		MEDIUM,
		HIGH,
		ULTRA,
		EPIC
	};
	enum class TextureFiltering
	{
		POINT,
		BILINEAR,
		TRILINEAR,
		ANISOTROPIC_2,
		ANISOTROPIC_4,
		ANISOTROPIC_8,
		ANISOTROPIC_16
	};

	DeviceInput::InputManager*			   mInputManager;
	Renderer::ImGuiLog*					   mImGuiLog;
	Renderer::CompositorWorkspaceInstance* mCompositorWorkspaceInstance;
	bool								   mFirstFrame;
	Renderer::SceneResourceId			   mSceneResourceId;
	Renderer::MaterialResourceId		   mMaterialResourceId;
	Renderer::MaterialResourceId		   mCloneMaterialResourceId;
	bool								   mCustomMaterialResourceSet;
	IController*						   mController;
	// Crazy raw-pointers to point-of-interest scene stuff
	Renderer::CameraSceneItem*		 mCameraSceneItem;
	Renderer::SunlightSceneItem*	 mSunlightSceneItem;
	Renderer::SkeletonMeshSceneItem* mSkeletonMeshSceneItem;
	Renderer::SceneNode*			 mSceneNode;
	// States for runtime-editing
	Renderer::DebugGuiHelper::GizmoSettings mGizmoSettings;
	// Video
	bool  mFullscreen;
	bool  mCurrentFullscreen;
	float mResolutionScale;
	bool  mUseVerticalSynchronization;
	bool  mCurrentUseVerticalSynchronization;
	int	  mCurrentMsaa;
	// Graphics
	Compositor	  mInstancedCompositor;
	int			  mCurrentCompositor;
	ShadowQuality mShadowQuality;
	int			  mCurrentShadowQuality;
	bool		  mHighQualityRendering;
	bool		  mHighQualityLighting;
	bool		  mSoftParticles;
	int			  mCurrentTextureFiltering;
	int			  mNumberOfTopTextureMipmapsToRemove;
	int			  mNumberOfTopMeshLodsToRemove;
	int			  mTerrainTessellatedTriangleWidth;
	// Environment
	float mCloudsIntensity;
	float mWindSpeed;
	float mWetSurfaces[4];	// x=wet level, y=hole/cracks flood level, z=puddle flood level, w=rain intensity
	// Post processing
	bool  mPerformFxaa;
	bool  mPerformSharpen;
	bool  mPerformChromaticAberration;
	bool  mPerformOldCrtEffect;
	bool  mPerformFilmGrain;
	bool  mPerformSepiaColorCorrection;
	bool  mPerformVignette;
	float mDepthOfFieldBlurrinessCutoff;
	// Selected material properties
	bool  mUseEmissiveMap;
	float mAlbedoColor[3];
	// Selected scene item
	float mRotationSpeed;
	bool  mShowSkeleton;
	// Scene hot-reloading memory
	bool				mHasCameraTransformBackup;
	Transform mCameraTransformBackup;
	// Ini settings indices
	std::vector<char> mIniFileContent;	// Defined here to avoid reallocations
	ini_t*			  mIni;
	int				  mMainWindowPositionSizeIniProperty;
	int				  mCameraPositionRotationIniProperty;
	int				  mOpenMetricsWindowIniProperty;
};
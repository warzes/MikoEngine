#pragma once

#include <MikoEngine/MikoHeader.h>
#include <DeviceInput/DeviceInput.h>
#include "ICubeRenderer.h"
#include "MikoEngine/Renderer/Resource/IResourceListener.h"
#include "MikoEngine/Core/File/IFileManager.h"
#include "MikoEngine/Renderer/RendererInstance.h"
#include "MikoEngine/Renderer/Core/IProfiler.h"

/**
*  @brief
*    Cube instancing application class
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Index buffer object (IBO)
*    - Vertex array object (VAO)
*    - Texture buffer object (TBO)
*    - Uniform buffer object (UBO)
*    - 2D texture
*    - 2D texture array
*    - Sampler state object (SO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
*    - Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
*/
class InstancedCubes : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    static constexpr uint32_t NUMBER_OF_CHANGED_CUBES = 10000;	///< Number of changed cubes on key interaction
    static constexpr uint32_t NUMBER_OF_TEXTURES = 8;		///< Number of textures
    static constexpr uint32_t SCENE_RADIUS = 10;		///< Scene radius

	DeviceInput::InputManager*	mInputManager;			///< Input manager instance, always valid
	ICubeRenderer*				mCubeRenderer;			///< Cube renderer instance, can be a null pointer
	uint32_t					mNumberOfCubeInstances;	///< Number of cube instances
	Rhi::CommandBuffer			mCommandBuffer;			///< Command buffer

	float mGlobalTimer;			///< Global timer
	float mGlobalScale;			///< Global scale
	bool  mDisplayStatistics;	///< Display statistics?
	float mFPSUpdateTimer;		///< Timer for FPS update
	int   mFramesSinceCheck;	///< Number of frames since last FPS update
	float  mFramesPerSecond;	///< Current frames per second

	// TODO: TEMP===>
	Renderer::IRenderer* getRenderer() const;

	IFileManager*		mFileManager;
#ifdef RENDERER_PROFILER
	Renderer::IProfiler*	mProfiler;			///< Profiler instance, can be a null pointer
#endif
	Renderer::Context*			mRendererContext;	///< Renderer context instance, can be a null pointer
	Renderer::RendererInstance* mRendererInstance;	///< Renderer instance, can be a null pointer
#ifdef RENDERER_GRAPHICS_DEBUGGER
	Renderer::IGraphicsDebugger* mGraphicsDebugger;	///< Graphics debugger instance, can be a null pointer
#endif
};
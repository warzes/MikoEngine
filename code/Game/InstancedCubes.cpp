#include "InstancedCubes.h"
#include "Color4.h"
#include "CubeRendererDrawInstanced.h"
#include "CubeRendererInstancedArrays.h"
#include <Renderer/IRenderer.h>
#include <Renderer/DebugGui/DebugGuiManager.h>
#include <Renderer/DebugGui/DebugGuiHelper.h>
#include "MikoEngine/Renderer/Core/GetInvalid.h"
#include "MikoEngine/Renderer/IRenderer.h"
#include "MikoEngine/Renderer/Core/File/PhysicsFSFileManager.h"
#include "MikoEngine/Renderer/Core/RemoteryProfiler.h"
#include "MikoEngine/Renderer/Asset/AssetManager.h"
#include "MikoEngine/Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "MikoEngine/Renderer/Resource/Mesh/MeshResourceManager.h"
#include "MikoEngine/Renderer/Resource/Texture/TextureResourceManager.h"
#include "MikoEngine/Renderer/Resource/Mesh/MeshResource.h"
#include "MikoEngine/Renderer/Resource/Texture/TextureResource.h"

bool InstancedCubes::init(int argc, const char * argv[])
{
	// TODO: TEMP===>
		// Create the renderer instance
#ifdef __ANDROID__
	struct android_app androidApp;	// TODO(co) Get Android app instance
	RHI_ASSERT(rhi->getContext(), nullptr != androidApp.activity->assetManager, "Invalid Android asset manager instance")
		mFileManager = new Renderer::AndroidFileManager(rhi->getContext().getLog(), rhi->getContext().getAssert(), rhi->getContext().getAllocator(), std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string(), *androidApp.activity->assetManager);
#else
	mFileManager = new Renderer::PhysicsFSFileManager(std::filesystem::canonical(std::filesystem::current_path() / "..").generic_string());
#endif
#if defined(RENDERER_GRAPHICS_DEBUGGER) && defined(RENDERER_PROFILER)
	mProfiler = new Renderer::RemoteryProfiler(*rhi);
	mRendererContext = new Renderer::Context(*rhi, *mFileManager, *mGraphicsDebugger, *mProfiler);
#elif defined RENDERER_GRAPHICS_DEBUGGER
	mRendererContext = new Renderer::Context(*rhi, *mFileManager, *mGraphicsDebugger);
#elif defined RENDERER_PROFILER
	mProfiler = new Renderer::RemoteryProfiler(*rhi);
	mRendererContext = new Renderer::Context(*rhi, *mFileManager, *mProfiler);
#else
	mRendererContext = new Renderer::Context(*rhi, *mFileManager);
#endif
	mRendererInstance = new Renderer::RendererInstance(*mRendererContext);

	{
		Renderer::IRenderer* renderer = getRenderer();
		if ( nullptr != renderer )
		{
			// Mount asset package
			bool mountAssetPackageResult = false;
			Renderer::AssetManager& assetManager = renderer->getAssetManager();
			bool rhiIsOpenGLES = (rhi->getNameId() == Rhi::NameId::OPENGLES3);
			if ( rhiIsOpenGLES )
			{
				// Handy fallback for development: If the mobile data isn't there, use the PC data
				mountAssetPackageResult = (assetManager.mountAssetPackage("../DataMobile/Example/Content", "Example") != nullptr);
				if ( !mountAssetPackageResult )
				{
					RHI_LOG(COMPATIBILITY_WARNING, "The examples application failed to find \"../DataMobile/Example/Content\", using \"../DataPc/Example/Content\" as fallback");
					mountAssetPackageResult = (assetManager.mountAssetPackage("../DataPc/Example/Content", "Example") != nullptr);
					rhiIsOpenGLES = false;
				}
			}
			else
			{
				mountAssetPackageResult = (assetManager.mountAssetPackage("../DataPc/Example/Content", "Example") != nullptr);
			}
			if ( !mountAssetPackageResult )
			{
				// Error!
				//showUrgentMessage("Please start \"ExampleProjectCompiler\" before starting \"Examples\" for the first time");
				//deinitialization();
				return false;
			}
			renderer->loadPipelineStateObjectCache();

			// Load renderer toolkit project to enable hot-reloading in case of asset changes
#ifdef RENDERER_TOOLKIT
			{
				RendererToolkit::IRendererToolkit* rendererToolkit = getRendererToolkit();
				if ( nullptr != rendererToolkit )
				{
					// The renderer toolkit project startup is done inside a background thread to not block the main thread
					mRendererToolkitProjectStartupThread = std::thread(&IApplicationRenderer::rendererToolkitProjectStartupThreadWorker, this, renderer, rendererToolkit, rhiIsOpenGLES);
				}
			}
#endif
		}
	}
	// TODO: TEMP<===

	mInputManager = new DeviceInput::InputManager();
	mCubeRenderer = nullptr;
	mNumberOfCubeInstances = 500;
	mGlobalTimer = 0.0f;
	mGlobalScale = 1.0f;
	mDisplayStatistics = true;
	mFPSUpdateTimer = 0.0f;
	mFramesSinceCheck = 0;
	mFramesPerSecond = 0.0f;

	// Create the cube renderer instance
		// -> Evaluate the feature set of the used RHI
		// TODO(co) This example doesn't support texture buffer emulation, which for OpenGL ES 3 is currently used
	const Rhi::Capabilities& capabilities = rhi->getCapabilities();
	if ( capabilities.drawInstanced && capabilities.maximumNumberOf2DTextureArraySlices > 0 && capabilities.maximumTextureBufferSize > 0 && rhi->getNameId() != Rhi::NameId::OPENGLES3 )
	{
		// Render cubes by using draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
		mCubeRenderer = new CubeRendererDrawInstanced(*rhi, getMainRenderTarget()->getRenderPass(), NUMBER_OF_TEXTURES, SCENE_RADIUS);
	}
	else if ( capabilities.instancedArrays )
	{
		// Render cubes by using instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
		mCubeRenderer = new CubeRendererInstancedArrays(*rhi, getMainRenderTarget()->getRenderPass(), NUMBER_OF_TEXTURES, SCENE_RADIUS);
	}

	// Tell the cube renderer about the number of cubes
	if ( nullptr != mCubeRenderer )
	{
		mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
	}


	return true;
}

void InstancedCubes::update(double delta)
{
	Renderer::IRenderer* renderer = getRenderer();
	if ( nullptr != renderer )
	{
		renderer->update();
	}



	// Update the global timer (FPS independent movement)
	const float timeDifference = delta;
	mGlobalTimer += timeDifference;

	// Calculate the current FPS
	++mFramesSinceCheck;
	mFPSUpdateTimer += timeDifference;
	if ( mFPSUpdateTimer > 1000.0f )
	{
		mFramesPerSecond = static_cast<float>(mFramesSinceCheck / (mFPSUpdateTimer / 1000.0f));
		mFPSUpdateTimer -= 1000.0f;
		mFramesSinceCheck = 0;
	}

	{ // Input
		DeviceInput::Keyboard* keyboard = mInputManager->GetKeyboard();
		if ( keyboard->HasChanged() )
		{
			// Add a fixed number of cubes
			if ( keyboard->NumpadAdd.checkHitAndReset() || keyboard->Add.checkHitAndReset() )
			{
				// Upper limit, just in case someone tries something nasty
				if ( mNumberOfCubeInstances < UINT_MAX - NUMBER_OF_CHANGED_CUBES )
				{
					// Update the number of cubes
					mNumberOfCubeInstances += NUMBER_OF_CHANGED_CUBES;

					// Tell the cube renderer about the number of cubes
					if ( nullptr != mCubeRenderer )
					{
						mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
					}
				}
			}

			// Subtract a fixed number of cubes
			if ( keyboard->NumpadSubtract.checkHitAndReset() || keyboard->Subtract.checkHitAndReset() )
			{
				// Lower limit
				if ( mNumberOfCubeInstances > 1 )
				{
					// Update the number of cubes
					if ( mNumberOfCubeInstances > NUMBER_OF_CHANGED_CUBES )
					{
						mNumberOfCubeInstances -= NUMBER_OF_CHANGED_CUBES;
					}
					else
					{
						mNumberOfCubeInstances = 1;
					}

					// Tell the cube renderer about the number of cubes
					if ( nullptr != mCubeRenderer )
					{
						mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
					}
				}
			}

			// Scale cubes up (change the size of all cubes at the same time)
			if ( keyboard->Up.checkHitAndReset() )
			{
				mGlobalScale += 0.1f;
			}

			// Scale cubes down (change the size of all cubes at the same time)
			if ( keyboard->Down.checkHitAndReset() )
			{
				mGlobalScale -= 0.1f;	// No need to check for negative values, results in entertaining inversed backface culling
			}

			// Show/hide statistics
			if ( keyboard->Space.checkHitAndReset() )
			{
				// Toggle display of statistics
				mDisplayStatistics = !mDisplayStatistics;
			}
		}
		mInputManager->Update();
	}

	// Clear the graphics color buffer of the current render target with blue, do also clear the depth buffer
	Rhi::Command::ClearGraphics::create(mCommandBuffer, Rhi::ClearFlag::COLOR_DEPTH, Color4::MIKO);

	// Draw the cubes
	if ( nullptr != mCubeRenderer )
	{
		mCubeRenderer->fillCommandBuffer(mGlobalTimer, mGlobalScale, sin(mGlobalTimer * 0.001f) * SCENE_RADIUS, sin(mGlobalTimer * 0.0005f) * SCENE_RADIUS, cos(mGlobalTimer * 0.0008f) * SCENE_RADIUS, mCommandBuffer);
	}

	// Display statistics
//#ifdef RENDERER_IMGUI
	if ( mDisplayStatistics && nullptr != getMainRenderTarget()  )
	{
		Renderer::DebugGuiManager& debugGuiManager = getRenderer()->getDebugGuiManager();
		debugGuiManager.newFrame(*getMainRenderTarget());

		// Is there a cube renderer instance?
		if ( nullptr != mCubeRenderer )
		{
			char text[128];

			// Number of cubes
			snprintf(text, GLM_COUNTOF(text), "Number of cubes: %u", mNumberOfCubeInstances);
			Renderer::DebugGuiHelper::drawText(text, 10.0f, 10.0f);

			// Frames per second
			snprintf(text, GLM_COUNTOF(text), "Frames per second: %.2f", mFramesPerSecond);
			Renderer::DebugGuiHelper::drawText(text, 10.0f, 40.0f);

			// Cubes per second
			// -> In every frame we draw n-cubes...
			// -> TODO(co) This number can get huge... had over 1 million cubes with >25 FPS... million cubes at ~2.4 FPS...
			//snprintf(text, GLM_COUNTOF(text), "Cubes per second: %u", static_cast<uint32_t>(mFramesPerSecond) * mNumberOfCubeInstances);
			//Renderer::DebugGuiHelper::drawText(text, 10.0f, 70.0f);
		}
		else
		{
			Renderer::DebugGuiHelper::drawText("No cube renderer instance", 10.0f, 10.0f);
		}
		debugGuiManager.fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(mCommandBuffer);
	}
//#endif

	// Submit command buffer to the RHI implementation
	mCommandBuffer.submitToRhiAndClear(*rhi);
}

void InstancedCubes::shutdown()
{
	if ( nullptr != mCubeRenderer )
	{
		delete mCubeRenderer;
		mCubeRenderer = nullptr;
	}
	delete mInputManager;

	delete mRendererInstance;
	mRendererInstance = nullptr;
	delete mRendererContext;
	mRendererContext = nullptr;
#ifdef RENDERER_PROFILER
	delete static_cast<Renderer::RemoteryProfiler*>(mProfiler);
#endif
#ifdef __ANDROID__
	delete static_cast<Renderer::AndroidFileManager*>(mFileManager);
#else
	delete static_cast<Renderer::PhysicsFSFileManager*>(mFileManager);
#endif
	mFileManager = nullptr;
#ifdef RENDERER_TOOLKIT
	{
		mRendererToolkitProjectStartupThread.join();
		std::lock_guard<std::mutex> projectMutexLock(mProjectMutex);
		if ( nullptr != mProject )
		{
			delete mProject;
			mProject = nullptr;
		}
	}
	if ( nullptr != mRendererToolkitInstance )
	{
		delete mRendererToolkitInstance;
		mRendererToolkitInstance = nullptr;
	}
	if ( nullptr != mRendererToolkitContext )
	{
		delete mRendererToolkitContext;
		mRendererToolkitContext = nullptr;
	}
	if ( nullptr != mRendererToolkitFileManager )
	{
		delete static_cast<Renderer::DefaultFileManager*>(mRendererToolkitFileManager);
		mRendererToolkitFileManager = nullptr;
	}
#endif
}

ApplicationSetting InstancedCubes::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "InstancedCubes";
	settings.vsync = false;
	settings.refresh_rate = 1200;
	settings.rhiApi = RHIApi::OpenGL;
	return settings;
}

void InstancedCubes::window_resized(int width, int height)
{
}

Renderer::IRenderer* InstancedCubes::getRenderer() const
{
	return (nullptr != mRendererInstance) ? mRendererInstance->getRenderer() : nullptr;
}
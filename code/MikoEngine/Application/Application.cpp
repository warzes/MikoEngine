#include "stdafx.h"
#include "Application.h"
#include "Core/Logger.h"
//-----------------------------------------------------------------------------
extern "C"
{
	// NVIDIA: Force usage of NVidia GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> See "Enabling High Performance Graphics Rendering on Optimus Systems" http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

	// AMD: Force usage of AMD GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> Named "Dynamic Switchable Graphics", found no official documentation, only https://community.amd.com/message/1307599#comment-1307599 - "Can an OpenGL app default to the discrete GPU on an Enduro system?"
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
//-----------------------------------------------------------------------------
#if SE_RHINULL
[[nodiscard]] extern Rhi::IRhi* createNullRhiInstance(const handle&);
#endif
#if SE_VULKAN
[[nodiscard]] extern Rhi::IRhi* createVulkanRhiInstance(const handle&);
#endif
#if SE_OPENGL
[[nodiscard]] extern Rhi::IRhi* createOpenGLRhiInstance(const handle&);
#endif
#if SE_OPENGLES
[[nodiscard]] extern Rhi::IRhi* createOpenGLES3RhiInstance(const handle&);
#endif
#if SE_DIRECT3D11
[[nodiscard]] extern Rhi::IRhi* createDirect3D11RhiInstance(const handle&);
#endif
#if SE_DIRECT3D12
[[nodiscard]] extern Rhi::IRhi* createDirect3D12RhiInstance(const handle&);
#endif
//-----------------------------------------------------------------------------
handle glfwNativeWindowHandle(GLFWwindow* _window)
{
#	if SE_PLATFORM_LINUX || SE_PLATFORM_BSD
# 		if ENTRY_CONFIG_USE_WAYLAND
	wl_egl_window *win_impl = (wl_egl_window*)glfwGetWindowUserPointer(_window);
	if ( !win_impl )
	{
		int width, height;
		glfwGetWindowSize(_window, &width, &height);
		struct wl_surface* surface = (struct wl_surface*)glfwGetWaylandWindow(_window);
		if ( !surface )
			return nullptr;
		win_impl = wl_egl_window_create(surface, width, height);
		glfwSetWindowUserPointer(_window, (void*)(uintptr_t)win_impl);
	}
	return (void*)(uintptr_t)win_impl;
#		else
	return (void*)(uintptr_t)glfwGetX11Window(_window);
#		endif
#	elif SE_PLATFORM_OSX
	return glfwGetCocoaWindow(_window);
#	elif SE_PLATFORM_WINDOWS
	return reinterpret_cast<handle>(glfwGetWin32Window(_window));
#	endif // SE_PLATFORM_
}
//-----------------------------------------------------------------------------
int Application::Run(int argc, const char *argv[])
{
	if ( !init_base(argc, argv) )
		return 1;

#if SE_PLATFORM_EMSCRIPTEN
	emscripten_set_main_loop_arg(RunFrame, this, 0, 1);
#else
	while (!exit_requested())
	{
		update_base(m_delta);
	}
#endif
	shutdown_base();
	return 0;
}
//-----------------------------------------------------------------------------
#if SE_PLATFORM_EMSCRIPTEN
void Application::RunFrame(void* arg)
{
	Application* app = (Application*)arg;
	app->update_base(app->m_delta);
}
#endif
//-----------------------------------------------------------------------------
bool Application::init([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[])
{
	return true;
}
//-----------------------------------------------------------------------------
void Application::update([[maybe_unused]] double delta)
{
}
//-----------------------------------------------------------------------------
void Application::shutdown()
{
}
//-----------------------------------------------------------------------------
bool Application::init_base(int argc, const char * argv[])
{
#if SE_DEBUG && SE_PLATFORM_WINDOWS
	// "_CrtDumpMemoryLeaks()" reports false positive memory leak with static variables, so use a memory difference instead
	_CrtMemCheckpoint(&m_crtMemState);
#endif

	logger::Init();

	std::string config;

	// Defaults
	ApplicationSetting settings = intial_app_settings();
	bool resizable = settings.resizable;
	bool maximized = settings.maximized;
	int refresh_rate = settings.refresh_rate;
	m_width = settings.width;
	m_height = settings.height;
	m_title = settings.title;

	if ( glfwInit() != GLFW_TRUE )
	{
		SE_LOG_FATAL("Failed to initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_MAXIMIZED, maximized);
	glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	if ( !m_window )
	{
		SE_LOG_FATAL("Failed to create GLFW window!");
		return false;
	}

	glfwSetKeyCallback(m_window, key_callback_glfw);
	glfwSetCursorPosCallback(m_window, mouse_callback_glfw);
	glfwSetScrollCallback(m_window, scroll_callback_glfw);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback_glfw);
	glfwSetCharCallback(m_window, char_callback_glfw);
	glfwSetWindowSizeCallback(m_window, window_size_callback_glfw);
	glfwSetWindowUserPointer(m_window, this);

	glfwMakeContextCurrent(m_window); // TODO: only opengl? or?

#if SE_RHINULL
	if ( settings.rhiApi == RHIApi::Null )
		rhi = createNullRhiInstance(glfwNativeWindowHandle(m_window));
#endif
#if SE_VULKAN
	if ( settings.rhiApi == RHIApi::Vulkan )
		rhi = createVulkanRhiInstance(glfwNativeWindowHandle(m_window));
#endif
#if SE_OPENGL
	if ( settings.rhiApi == RHIApi::OpenGL )
		rhi = createOpenGLRhiInstance(glfwNativeWindowHandle(m_window));
#endif
#if SE_OPENGLES
	if ( settings.rhiApi == RHIApi::OpenGLES )
		rhi = createOpenGLES3RhiInstance(glfwNativeWindowHandle(m_window));
#endif
#if SE_DIRECT3D11
	if ( settings.rhiApi == RHIApi::Direct3D11 )
		rhi = createDirect3D11RhiInstance(glfwNativeWindowHandle(m_window));
#endif
#if SE_DIRECT3D12
	if ( settings.rhiApi == RHIApi::Direct3D12 )
		rhi = createDirect3D12RhiInstance(glfwNativeWindowHandle(m_window));
#endif

	if ( nullptr == rhi && !rhi->isInitialized() )
	{
		rhi = nullptr;
		return 0;
	}

	// Create render pass using the preferred swap chain texture format
	const Rhi::Capabilities& capabilities = rhi->getCapabilities();

	Rhi::IRenderPass* renderPass = rhi->createRenderPass(1, &capabilities.preferredSwapChainColorTextureFormat, capabilities.preferredSwapChainDepthStencilTextureFormat, 1 RHI_RESOURCE_DEBUG_NAME("Main"));

	// Create a main swap chain instance
	mainSwapChain = rhi->createSwapChain(
		*renderPass,
		Rhi::WindowHandle{ (handle)glfwNativeWindowHandle(m_window), nullptr },
		false
		RHI_RESOURCE_DEBUG_NAME("Main"));
	mainSwapChain->AddReference();	// Internal RHI reference // TODO:???
	
	SE_LOG_INFO("Successfully initialized platform!");
	if ( !init(argc, argv) )
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void Application::update_base(double delta)
{
	begin_frame();

	if ( doesCompleteOwnDrawing() )
	{
		update(delta);
	}
	else
	{
		if ( rhi->beginScene() )
		{
			{ // Scene rendering
				// Scoped debug event
				COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer);

				// Make the graphics main swap chain to the current render target
				Rhi::Command::SetGraphicsRenderTarget::create(commandBuffer, mainSwapChain);

				{ // Since Direct3D 12 is command list based, the viewport and scissor rectangle must be set in every draw call to work with all supported RHI implementations
					// Get the window size
					uint32_t width = 1;
					uint32_t height = 1;
					mainSwapChain->getWidthAndHeight(width, height);

					// Set the graphics viewport and scissor rectangle
					Rhi::Command::SetGraphicsViewportAndScissorRectangle::create(commandBuffer, 0, 0, width, height);
				}

				// Submit command buffer to the RHI implementation
				commandBuffer.submitToRhiAndClear(*rhi);

				// Call the draw method
				update(delta);
			}

			// Submit command buffer to the RHI implementation
			commandBuffer.submitToRhiAndClear(*rhi);

			// End scene rendering
			rhi->endScene();
		}

		// Present the content of the current back buffer
		mainSwapChain->present();
	}

	end_frame();
}
//-----------------------------------------------------------------------------
void Application::shutdown_base()
{
	// Execute user-side shutdown method.
	shutdown();

	// Delete the RHI instance
	if ( nullptr != mainSwapChain )
	{
		mainSwapChain->ReleaseReference();
		mainSwapChain = nullptr;
	}
	rhi = nullptr;

	// Shutdown GLFW.
	glfwDestroyWindow(m_window);
	glfwTerminate();

	// Close logger.
	logger::Close();

	// For memory leak detection
#if SE_DEBUG && SE_PLATFORM_WINDOWS
	_CrtMemDumpAllObjectsSince(&m_crtMemState);
#endif
}
//-----------------------------------------------------------------------------
void Application::begin_frame()
{
	m_timer.Start();

	glfwPollEvents();

	m_mouse_delta_x = m_mouse_x - m_last_mouse_x;
	m_mouse_delta_y = m_mouse_y - m_last_mouse_y;
	m_last_mouse_x = m_mouse_x;
	m_last_mouse_y = m_mouse_y;

	if ( m_window_resized && mainSwapChain )
	{
		mainSwapChain->resizeBuffers();
		m_window_resized = false;
	}

	// Toggle the fullscreen state
	//mMainSwapChain->setFullscreenState(!mMainSwapChain->getFullscreenState());
}
//-----------------------------------------------------------------------------
void Application::end_frame()
{
	m_timer.Stop();
	m_delta = m_timer.ElapsedTimeMilisec();
	m_delta_seconds = m_timer.ElapsedTimeSec();
}
//-----------------------------------------------------------------------------
void Application::request_exit() const
{
	glfwSetWindowShouldClose(m_window, true);
}
//-----------------------------------------------------------------------------
bool Application::exit_requested() const
{
	return glfwWindowShouldClose(m_window);
}
//-----------------------------------------------------------------------------
ApplicationSetting Application::intial_app_settings()
{
	return ApplicationSetting();
}
//-----------------------------------------------------------------------------
void Application::window_resized(int width, int height)
{
}
//-----------------------------------------------------------------------------
void Application::key_pressed(int code)
{
}
//-----------------------------------------------------------------------------
void Application::key_released(int code)
{
}
//-----------------------------------------------------------------------------
void Application::mouse_scrolled(double xoffset, double yoffset)
{
}
//-----------------------------------------------------------------------------
void Application::mouse_pressed(int code)
{
}
//-----------------------------------------------------------------------------
void Application::mouse_released(int code)
{
}
//-----------------------------------------------------------------------------
void Application::mouse_move(double x, double y, double deltaX, double deltaY)
{
}
//-----------------------------------------------------------------------------
void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose(window, 1);
	if ( key >= 0 && key < MAX_KEYS )
	{
		if ( action == GLFW_PRESS )
		{
			key_pressed(key);
			m_keys[key] = true;
		}
		else if ( action == GLFW_RELEASE )
		{
			key_released(key);
			m_keys[key] = false;
		}
	}
}
//-----------------------------------------------------------------------------
void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	m_mouse_x = xpos;
	m_mouse_y = ypos;
	mouse_move(xpos, ypos, m_mouse_delta_x, m_mouse_delta_y);
}
//-----------------------------------------------------------------------------
void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouse_scrolled(xoffset, yoffset);
}
//-----------------------------------------------------------------------------
void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if ( button >= 0 && button < MAX_MOUSE_BUTTONS )
	{
		if ( action == GLFW_PRESS )
		{
			mouse_pressed(button);
			m_mouse_buttons[button] = true;
		}
		else if ( action == GLFW_RELEASE )
		{
			mouse_released(button);
			m_mouse_buttons[button] = false;
		}
	}
}
//-----------------------------------------------------------------------------
void Application::window_size_callback(GLFWwindow* window, int width, int height)
{
	m_width = width;
	m_height = height;
	m_window_resized = true;
	window_resized(width, height);
}
//-----------------------------------------------------------------------------
void Application::key_callback_glfw(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->key_callback(window, key, scancode, action, mode);
}
//-----------------------------------------------------------------------------
void Application::mouse_callback_glfw(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->mouse_callback(window, xpos, ypos);
}
//-----------------------------------------------------------------------------
void Application::scroll_callback_glfw(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->scroll_callback(window, xoffset, yoffset);
}
//-----------------------------------------------------------------------------
void Application::mouse_button_callback_glfw(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->mouse_button_callback(window, button, action, mods);
}
//-----------------------------------------------------------------------------
void Application::char_callback_glfw(GLFWwindow* window, unsigned int c)
{
}
//-----------------------------------------------------------------------------
void Application::window_size_callback_glfw(GLFWwindow* window, int width, int height)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->window_size_callback(window, width, height);
}
//-----------------------------------------------------------------------------
[[nodiscard]] Rhi::IRenderTarget* Application::getMainRenderTarget() const
{
	return mainSwapChain;
}
//-----------------------------------------------------------------------------
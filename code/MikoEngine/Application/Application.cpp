#include "stdafx.h"
#include "Application.h"
#include "Core/Logger.h"
#include <Render/DefaultLog.h>
#include <Render/DefaultAssert.h>
#include <Render/DefaultAllocator.h>
#include <Render/RhiInstance.h>
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
void* glfwNativeWindowHandle(GLFWwindow* _window)
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
	return glfwGetWin32Window(_window);
#	endif // SE_PLATFORM_
}
//-----------------------------------------------------------------------------
int Application::Run(int argc, const char *argv[])
{
	if ( !init_base(argc, argv) )
		return 1;

	Rhi::DefaultLog defaultLog;
	Rhi::DefaultAssert defaultAssert;
	Rhi::DefaultAllocator defaultAllocator;
#ifdef _WIN32
	Rhi::Context rhiContext(defaultLog, defaultAssert, defaultAllocator, (Rhi::handle)glfwNativeWindowHandle(m_window));
	const bool loadRhiApiSharedLibrary = false;
#elif LINUX
	// Under Linux the OpenGL library interacts with the library from X11 so we need to load the library ourself instead letting it be loaded by the RHI instance
	// -> See http://dri.sourceforge.net/doc/DRIuserguide.html "11.5 libGL.so and dlopen()"
	const bool loadRhiApiSharedLibrary = true;
	Rhi::X11Context rhiContext(defaultLog, defaultAssert, defaultAllocator, getX11Display(), getNativeWindowHandle(*sdlWindow));
#endif
	Rhi::RhiInstance rhiInstance((argc > 1) ? argv[1] : Rhi::DEFAULT_RHI_NAME, rhiContext, loadRhiApiSharedLibrary);
	Rhi::IRhiPtr rhi = rhiInstance.getRhi();
	if (nullptr == rhi && !rhi->isInitialized())
		return 0;

	// Create RHI resources
	Rhi::ISwapChainPtr mainSwapChain;
	Rhi::IBufferManagerPtr bufferManager;
	Rhi::IRootSignaturePtr rootSignature;
	Rhi::IGraphicsPipelineStatePtr graphicsPipelineState;
	Rhi::IVertexArrayPtr vertexArray;
	{
		{ // Create RHI swap chain instance
			const Rhi::Capabilities& capabilities = rhi->getCapabilities();
			mainSwapChain = rhi->createSwapChain(
				*rhi->createRenderPass(1, &capabilities.preferredSwapChainColorTextureFormat, capabilities.preferredSwapChainDepthStencilTextureFormat, 1 RHI_RESOURCE_DEBUG_NAME("Main")),
				Rhi::WindowHandle{ (Rhi::handle)glfwNativeWindowHandle(m_window), nullptr, nullptr },	// TODO(co) Linux Wayland support
				rhi->getContext().isUsingExternalContext()
				RHI_RESOURCE_DEBUG_NAME("Main"));
		}

		// Create the buffer manager
		bufferManager = rhi->createBufferManager();

		{ // Create the root signature
			// Setup
			Rhi::RootSignatureBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(0, nullptr, 0, nullptr, Rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			rootSignature = rhi->createRootSignature(rootSignatureBuilder RHI_RESOURCE_DEBUG_NAME("Triangle"));
		}

		// Vertex input layout
		static constexpr Rhi::VertexAttribute vertexAttributesLayout[] =
		{
			{ // Attribute 0
				// Data destination
				Rhi::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
				"Position",								// name[32] (char)
				"POSITION",								// semanticName[32] (char)
				0,										// semanticIndex (uint32_t)
				// Data source
				0,										// inputSlot (uint32_t)
				0,										// alignedByteOffset (uint32_t)
				sizeof(float) * 2,						// strideInBytes (uint32_t)
				0										// instancesPerElement (uint32_t)
			}
		};
		const Rhi::VertexAttributes vertexAttributes(sizeof(vertexAttributesLayout) / sizeof(Rhi::VertexAttribute), vertexAttributesLayout);

		{ // Create vertex array object (VAO)
			// Create the vertex buffer object (VBO)
			// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
			static constexpr float VERTEX_POSITION[] =
			{					// Vertex ID	Triangle on screen
				 0.0f, 1.0f,	// 0				0
				 1.0f, 0.0f,	// 1			   .   .
				-0.5f, 0.0f		// 2			  2.......1
			};
			Rhi::IVertexBufferPtr vertexBuffer(bufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, 0, Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME("Triangle")));

			// Create vertex array object (VAO)
			// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
			// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
			// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
			//    reference of the used vertex buffer objects (VBO). If the reference counter of a
			//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
			const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
			vertexArray = bufferManager->createVertexArray(vertexAttributes, sizeof(vertexArrayVertexBuffers) / sizeof(Rhi::VertexArrayVertexBuffer), vertexArrayVertexBuffers, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle"));
		}

		{
			// Create the graphics program
			Rhi::IGraphicsProgramPtr graphicsProgram;
			{
				// Get the shader source code (outsourced to keep an overview)
				const char* vertexShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
#include "Example_GLSL_450.h"	// For Vulkan
#include "Example_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "Example_GLSL_ES3.h"
#include "Example_HLSL_D3D9_D3D10_D3D11_D3D12.h"
#include "Example_Null.h"

				// Create the graphics program
				Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
				graphicsProgram = shaderLanguage.createGraphicsProgram(
					*rootSignature,
					vertexAttributes,
					shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle")),
					shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle"))
					RHI_RESOURCE_DEBUG_NAME("Triangle"));
			}

			// Create the graphics pipeline state object (PSO)
			if (nullptr != graphicsProgram)
			{
				graphicsPipelineState = rhi->createGraphicsPipelineState(Rhi::GraphicsPipelineStateBuilder(rootSignature, graphicsProgram, vertexAttributes, mainSwapChain->getRenderPass()) RHI_RESOURCE_DEBUG_NAME("Triangle"));
			}
		}
	}

	// Record RHI command buffer
	Rhi::CommandBuffer commandBuffer;
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer)

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

		{ // Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
			const float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
			Rhi::Command::ClearGraphics::create(commandBuffer, Rhi::ClearFlag::COLOR_DEPTH, color);
		}

		// Set the used graphics root signature
		Rhi::Command::SetGraphicsRootSignature::create(commandBuffer, rootSignature);

		// Set the used graphics pipeline state object (PSO)
		Rhi::Command::SetGraphicsPipelineState::create(commandBuffer, graphicsPipelineState);

		// Input assembly (IA): Set the used vertex array
		Rhi::Command::SetGraphicsVertexArray::create(commandBuffer, vertexArray);

		// Set debug marker
		// -> Debug methods: When using Direct3D <11.1, these methods map to the Direct3D 9 PIX functions
		//    (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)
		COMMAND_SET_DEBUG_MARKER(commandBuffer, "Everyone ready for the upcoming triangle?")

		{
			// Scoped debug event
			COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, "Drawing the fancy triangle")

				// Render the specified geometric primitive, based on an array of vertices
				Rhi::Command::DrawGraphics::create(commandBuffer, 3);
		}
	}

#if SE_PLATFORM_EMSCRIPTEN
	emscripten_set_main_loop_arg(RunFrame, this, 0, 1);
#else
	while (!exit_requested())
	{
		update_base(m_delta);

		// Begin scene rendering
		if (rhi->beginScene())
		{
			// Submit command buffer to the RHI implementation
			commandBuffer.submitToRhi(*rhi);

			// End scene rendering
			rhi->endScene();
		}

		// Present the content of the current back buffer
		mainSwapChain->present();

		// Inform the swap chain that the size of the native window was changed
		// -> Required for Direct3D 9, Direct3D 10, Direct3D 11
		// -> Not required for OpenGL and OpenGL ES 3
		//mainSwapChain->resizeBuffers();
		//if (SDLK_RETURN == sdlEvent.key.keysym.sym && (sdlEvent.key.keysym.mod & KMOD_ALT) != 0)
		//	mainSwapChain->setFullscreenState(!mainSwapChain->getFullscreenState());
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
bool Application::init(int argc, const char *argv[])
{
	return true;
}
//-----------------------------------------------------------------------------
void Application::update(double delta)
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

	//WindowHandle = glfwNativeWindowHandle(m_window);

	glfwSetKeyCallback(m_window, key_callback_glfw);
	glfwSetCursorPosCallback(m_window, mouse_callback_glfw);
	glfwSetScrollCallback(m_window, scroll_callback_glfw);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback_glfw);
	glfwSetCharCallback(m_window, char_callback_glfw);
	glfwSetWindowSizeCallback(m_window, window_size_callback_glfw);
	glfwSetWindowUserPointer(m_window, this);

	glfwMakeContextCurrent(m_window); // TODO: only opengl? or?

	SE_LOG_INFO("Successfully initialized platform!");

	if ( !init(argc, argv) )
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void Application::update_base(double delta)
{
	begin_frame();
	update(delta);
	end_frame();
}
//-----------------------------------------------------------------------------
void Application::shutdown_base()
{
	// Execute user-side shutdown method.
	shutdown();

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
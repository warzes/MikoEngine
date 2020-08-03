#include "stdafx.h"
#include "Application.h"
#include "Core/Logger.h"
#include "Core/Profiler.h"
#include "Graphics/Material.h"
#include <imgui/examples/imgui_impl_glfw.h>
#if SE_VULKAN
#    include <imgui/examples/imgui_impl_vulkan.h>
#else
#    include <imgui/examples/imgui_impl_opengl3.h>
#endif
//-----------------------------------------------------------------------------
#if SE_VULKAN
void imgui_vulkan_error_check(VkResult err)
{
	if ( err == 0 )
		return;

	SE_LOG_ERROR("(Vulkan) Error " + std::to_string(err));

	if ( err < 0 )
		abort();
}
#else
static void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
{
	std::string msg_source;

	switch ( source )
	{
	case GL_DEBUG_SOURCE_API:
		msg_source = "API";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		msg_source = "SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		msg_source = "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		msg_source = "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		msg_source = "OTHER";
		break;
	}

	std::string msg_type;

	switch ( type )
	{
	case GL_DEBUG_TYPE_ERROR:
		msg_type = "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		msg_type = "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		msg_type = "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		msg_type = "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		msg_type = "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		msg_type = "OTHER";
		break;
	}

	std::string msg_severity = "DEFAULT";

	switch ( severity )
	{
	case GL_DEBUG_SEVERITY_LOW:
		msg_severity = "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		msg_severity = "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		msg_severity = "HIGH";
		break;
	}

	std::string log_msg = "glDebugMessage: " + std::string(message) + ", type = " + msg_type + ", source = " + msg_source + ", severity = " + msg_severity;

	if ( type == GL_DEBUG_TYPE_ERROR )
		SE_LOG_ERROR(log_msg);
	else
		SE_LOG_WARNING(log_msg);
}
#endif
//-----------------------------------------------------------------------------
int Application::Run(int argc, const char *argv[])
{
	if ( !init_base(argc, argv) )
		return 1;

#if defined(__EMSCRIPTEN__)
	emscripten_set_main_loop_arg(run_frame, this, 0, 1);
#else
	while ( !exit_requested() )
		update_base(m_delta);
#endif

#if SE_VULKAN
	m_vk_backend->wait_idle();
#endif

	shutdown_base();
	return 0;
}
//-----------------------------------------------------------------------------
#if defined(__EMSCRIPTEN__)
void Application::run_frame(void* arg)
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
	logger::initialize();
	logger::open_console_stream();
	logger::open_file_stream();

	std::string config;

	// Defaults
	ApplicationSetting settings = intial_app_settings();
	bool resizable = settings.resizable;
	bool maximized = settings.maximized;
	int  refresh_rate = settings.refresh_rate;
	m_width = settings.width;
	m_height = settings.height;
	m_title = settings.title;

	int major_ver = 4;
#if defined(__APPLE__)
	int         minor_ver = 1;
	const char* imgui_glsl_version = "#version 150";
#elif defined(__EMSCRIPTEN__)
	major_ver = 3;
	int         minor_ver = 0;
	const char* imgui_glsl_version = "#version 150";
#else
	int         minor_ver = 5;
	const char* imgui_glsl_version = "#version 130";
#endif

	if ( glfwInit() != GLFW_TRUE )
	{
		SE_LOG_FATAL("Failed to initialize GLFW");
		return false;
	}

#if SE_VULKAN
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#else
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

#    if !defined(__EMSCRIPTEN__)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
#    endif

#    if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#    endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_ver);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_ver);

#endif

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

	SE_LOG_INFO("Successfully initialized platform!");

#if SE_VULKAN
	m_vk_backend = vk::Backend::create(m_window,
#	if SE_DEBUG
		true
#	else
		false
#	endif
		,
		settings.ray_tracing,
		settings.device_extensions,
		settings.device_pnext);

	m_image_available_semaphores.resize(vk::Backend::kMaxFramesInFlight);
	m_render_finished_semaphores.resize(vk::Backend::kMaxFramesInFlight);

	for ( size_t i = 0; i < vk::Backend::kMaxFramesInFlight; i++ )
	{
		m_image_available_semaphores[i] = vk::Semaphore::create(m_vk_backend);
		m_render_finished_semaphores[i] = vk::Semaphore::create(m_vk_backend);
	}

	Material::initialize_common_resources(m_vk_backend);
#else
#    if !defined(__EMSCRIPTEN__)
	if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) )
		return false;
#    endif

	if ( settings.enable_debug_callback )
	{
		glDebugMessageCallback(glDebugCallback, NULL);
		glEnable(GL_DEBUG_OUTPUT);
	}

#endif

	ImGui::CreateContext();

#if SE_VULKAN
	ImGui_ImplGlfw_InitForVulkan(m_window, false);

	ImGui_ImplVulkan_InitInfo init_info = {};

	init_info.Instance = m_vk_backend->instance();
	init_info.PhysicalDevice = m_vk_backend->physical_device();
	init_info.Device = m_vk_backend->device();
	init_info.QueueFamily = m_vk_backend->queue_infos().graphics_queue_index;
	init_info.Queue = m_vk_backend->graphics_queue();
	init_info.PipelineCache = nullptr;
	init_info.DescriptorPool = m_vk_backend->thread_local_descriptor_pool()->handle();
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = m_vk_backend->swap_image_count();
	init_info.CheckVkResultFn = nullptr;

	ImGui_ImplVulkan_Init(&init_info, m_vk_backend->swapchain_render_pass()->handle());

	vk::CommandBuffer::Ptr cmd_buf = m_vk_backend->allocate_graphics_command_buffer();

	VkCommandBufferBeginInfo begin_info;
	SE_ZERO_MEMORY(begin_info);

	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	vkBeginCommandBuffer(cmd_buf->handle(), &begin_info);

	ImGui_ImplVulkan_CreateFontsTexture(cmd_buf->handle());

	vkEndCommandBuffer(cmd_buf->handle());

	m_vk_backend->flush_graphics({ cmd_buf });

	ImGui_ImplVulkan_DestroyFontUploadObjects();
#else
	ImGui_ImplGlfw_InitForOpenGL(m_window, false);
	ImGui_ImplOpenGL3_Init(imgui_glsl_version);
#endif

	ImGui::StyleColorsDark();

	float xscale, yscale;
	GLFWmonitor* primary = glfwGetPrimaryMonitor();	
	glfwGetMonitorContentScale(primary, &xscale, &yscale);

	ImGuiStyle* style = &ImGui::GetStyle();
	style->ScaleAllSizes(xscale > yscale ? xscale : yscale);
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = xscale > yscale ? xscale : yscale;

	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	m_width = display_w;
	m_height = display_h;

	if ( !m_debug_draw.init(
#if SE_VULKAN
		m_vk_backend, m_vk_backend->swapchain_render_pass()
#endif
	) )
		return false;

	profiler::initialize(
#if SE_VULKAN
		m_vk_backend
#endif
	);

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

#if SE_VULKAN
	// Shutdown debug draw.

	// Shutdown profiler.
	profiler::shutdown();

	m_debug_draw.shutdown();
	Material::shutdown_common_resources();

	// Shutdown ImGui.
	ImGui_ImplVulkan_Shutdown();

	for ( int i = 0; i < vk::Backend::kMaxFramesInFlight; i++ )
	{
		m_image_available_semaphores[i].reset();
		m_render_finished_semaphores[i].reset();
	}

	m_vk_backend->~Backend();
#else
	// Shutdown debug draw.
	m_debug_draw.shutdown();

	ImGui_ImplOpenGL3_Shutdown();
#endif

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Shutdown GLFW.
	glfwDestroyWindow(m_window);
	glfwTerminate();

	// Close logger streams.
	logger::close_file_stream();
	logger::close_console_stream();
}
//-----------------------------------------------------------------------------
#if SE_VULKAN
void Application::render_gui(vk::CommandBuffer::Ptr cmd_buf)
{
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd_buf->handle());
}
#endif
//-----------------------------------------------------------------------------
#if SE_VULKAN
void Application::submit_and_present(const std::vector<vk::CommandBuffer::Ptr>& cmd_bufs)
{
	m_vk_backend->submit_graphics(cmd_bufs,
		{ m_image_available_semaphores[m_vk_backend->current_frame_idx()] },
		{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT },
		{ m_render_finished_semaphores[m_vk_backend->current_frame_idx()] });

	m_vk_backend->present({ m_render_finished_semaphores[m_vk_backend->current_frame_idx()] });
}
#endif
//-----------------------------------------------------------------------------
void Application::begin_frame()
{
	m_timer.Start();

	glfwPollEvents();

#if SE_VULKAN
	if ( m_should_recreate_swap_chain )
	{
		m_vk_backend->recreate_swapchain();
		m_should_recreate_swap_chain = false;
	}

	m_vk_backend->acquire_next_swap_chain_image(m_image_available_semaphores[m_vk_backend->current_frame_idx()]);

	ImGui_ImplVulkan_NewFrame();
#else
	ImGui_ImplOpenGL3_NewFrame();
#endif

	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	m_mouse_delta_x = m_mouse_x - m_last_mouse_x;
	m_mouse_delta_y = m_mouse_y - m_last_mouse_y;

	m_last_mouse_x = m_mouse_x;
	m_last_mouse_y = m_mouse_y;

	profiler::begin_frame();
}
//-----------------------------------------------------------------------------
void Application::end_frame()
{
	profiler::end_frame();

#if !SE_VULKAN
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_window);
#endif

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
		glfwSetWindowShouldClose(window, GL_TRUE);
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

#if SE_VULKAN
	m_should_recreate_swap_chain = true;
#endif

	window_resized(width, height);
}
//-----------------------------------------------------------------------------
void Application::key_callback_glfw(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);
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
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->scroll_callback(window, xoffset, yoffset);
}
//-----------------------------------------------------------------------------
void Application::mouse_button_callback_glfw(GLFWwindow* window, int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->mouse_button_callback(window, button, action, mods);
}
//-----------------------------------------------------------------------------
void Application::char_callback_glfw(GLFWwindow* window, unsigned int c)
{
	ImGui_ImplGlfw_CharCallback(window, c);
}
//-----------------------------------------------------------------------------
void Application::window_size_callback_glfw(GLFWwindow* window, int width, int height)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->window_size_callback(window, width, height);
}
//-----------------------------------------------------------------------------
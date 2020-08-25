#pragma once

#include "ApplicationSettings.h"
#include "Utility/Timer.h"
#include "RHI/Rhi.h"

// Key and Mouse button limits.
#define MAX_KEYS 1024
#define MAX_MOUSE_BUTTONS 5

class Application
{
public:
	Application() = default;
	virtual ~Application() = default;

	int Run(int argc, const char* argv[]);

#if SE_PLATFORM_EMSCRIPTEN
	static void RunFrame(void* arg);
#endif

	// Internal callbacks used by GLFW static callback functions.
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void window_size_callback(GLFWwindow* window, int width, int height);

	// GLFW static callback functions. Called from within GLFW's event loop.
	static void key_callback_glfw(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_callback_glfw(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback_glfw(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_button_callback_glfw(GLFWwindow* window, int button, int action, int mods);
	static void char_callback_glfw(GLFWwindow* window, unsigned int c);
	static void window_size_callback_glfw(GLFWwindow* window, int width, int height);

protected:
	// Intial app settings. Override this to set defaults.
	virtual ApplicationSetting intial_app_settings();

	// Window event callbacks. Override these!
	virtual void window_resized(int width, int height);
	virtual void key_pressed(int code);
	virtual void key_released(int code);
	virtual void mouse_scrolled(double xoffset, double yoffset);
	virtual void mouse_pressed(int code);
	virtual void mouse_released(int code);
	virtual void mouse_move(double x, double y, double deltaX, double deltaY);

	// Application exit related-methods. Self-explanatory.
	void request_exit() const;
	bool exit_requested() const;

	// Life cycle hooks. Override these!
	virtual bool init(int argc, const char* argv[]);
	virtual void update(double delta);
	virtual void shutdown();

	// Return if the examples does the drawing completely on its own; thus no draw handling in frontend 
	[[nodiscard]] inline virtual bool doesCompleteOwnDrawing() const
	{
		// Default implementation does not complete own drawing
		return false;
	}

	[[nodiscard]] Rhi::IRenderTarget* getMainRenderTarget() const;

	uint32_t                            m_width = 0;
	uint32_t                            m_height = 0;
	double                              m_mouse_x = 0.0;
	double                              m_mouse_y = 0.0;
	double                              m_last_mouse_x = 0.0;
	double                              m_last_mouse_y = 0.0;
	double                              m_mouse_delta_x = 0.0;
	double                              m_mouse_delta_y = 0.0;
	double                              m_delta = 0.0;
	double                              m_delta_seconds = 0.0;
	std::string                         m_title;
	std::array<bool, MAX_KEYS>          m_keys;
	std::array<bool, MAX_MOUSE_BUTTONS> m_mouse_buttons;
	GLFWwindow*                         m_window = nullptr;
	Timer                               m_timer;

#if SE_DEBUG && SE_PLATFORM_WINDOWS
	_CrtMemState m_crtMemState = { };
#endif

	Rhi::IRhiPtr rhi = nullptr;
	Rhi::ISwapChainPtr mainSwapChain = nullptr;
	Rhi::CommandBuffer commandBuffer;	// Command buffer

	bool m_window_resized = false;
	bool m_endFrame = true;

private:
	// Pre, Post frame methods for ImGUI updates, presentations etc.
	void begin_frame();
	void end_frame();

	// Internal lifecycle methods
	bool init_base(int argc, const char* argv[]);
	void update_base(double delta);
	void shutdown_base();
};
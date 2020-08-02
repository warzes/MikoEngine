#include "stdafx.h"
#include "lWindow.h"
//-----------------------------------------------------------------------------
static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto wnd = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	wnd->m_framebufferResized = true;
}
//-----------------------------------------------------------------------------
void Window::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(800, 600, "Mico Engine", nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}
//-----------------------------------------------------------------------------
GLFWwindow* Window::GetWindow()
{
	return m_window;
}
//-----------------------------------------------------------------------------
void Window::Close()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
//-----------------------------------------------------------------------------
bool Window::IsClose()
{
	return glfwWindowShouldClose(m_window);
}
//-----------------------------------------------------------------------------
void Window::Events()
{
	glfwPollEvents();
}
//-----------------------------------------------------------------------------
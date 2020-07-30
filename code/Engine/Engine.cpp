#include "stdafx.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}
//-----------------------------------------------------------------------------
void Engine::Run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}
//-----------------------------------------------------------------------------
void Engine::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(800, 600, "Mico Engine", nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}
//-----------------------------------------------------------------------------
void Engine::initVulkan()
{
}
//-----------------------------------------------------------------------------
void Engine::mainLoop()
{
}
//-----------------------------------------------------------------------------
void Engine::cleanup()
{
}
//-----------------------------------------------------------------------------
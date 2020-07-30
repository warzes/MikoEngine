#pragma once

class Engine
{
	friend static void framebufferResizeCallback(GLFWwindow*, int, int);
public:
	void Run();

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();
	GLFWwindow *m_window = nullptr;

	bool framebufferResized = false;
};
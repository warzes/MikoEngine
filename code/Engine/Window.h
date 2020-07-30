#pragma once

class Window
{
	friend static void framebufferResizeCallback(GLFWwindow*, int, int);
public:
	void Init();

	GLFWwindow* GetWindow();
private:
	GLFWwindow* m_window = nullptr;
	bool m_framebufferResized = false;
};

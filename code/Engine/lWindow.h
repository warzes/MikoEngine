#pragma once

class Window
{
	friend static void framebufferResizeCallback(GLFWwindow*, int, int);
public:
	void Init();
	void Close();

	GLFWwindow* GetWindow();

	bool IsClose();
	void Events();

	bool& GetFramebufferResized() { return m_framebufferResized; }
private:
	GLFWwindow* m_window = nullptr;
	bool m_framebufferResized = false;
};

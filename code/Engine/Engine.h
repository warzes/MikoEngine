#pragma once

#include "Window.h"
#include "RenderSystem.h"

class Engine
{
public:
	void Run();

private:
	void mainLoop();
	void cleanup();

	Window m_window;
	RenderSystem m_renderSystem;
};
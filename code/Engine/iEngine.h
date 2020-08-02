#pragma once

#include "lWindow.h"
#include "lRenderSystem.h"

class lEngine
{
public:
	void Run();

private:
	void mainLoop();
	void cleanup();

	Window m_window;
	iRenderSystem m_renderSystem;
};
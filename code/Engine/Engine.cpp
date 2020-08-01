#include "stdafx.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
void Engine::Run()
{
	m_window.Init();
	m_renderSystem.Init(m_window.GetWindow());
	mainLoop();
	cleanup();
}
//-----------------------------------------------------------------------------
void Engine::mainLoop()
{
	while (!m_window.IsClose())
	{
		m_window.Events();
		m_renderSystem.drawFrame(m_window.GetFramebufferResized());
	}
}
//-----------------------------------------------------------------------------
void Engine::cleanup()
{
	m_renderSystem.Close();
	m_window.Close();
}
//-----------------------------------------------------------------------------
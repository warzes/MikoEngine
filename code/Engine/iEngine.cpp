#include "stdafx.h"
#include "iEngine.h"
//-----------------------------------------------------------------------------
void lEngine::Run()
{
	m_window.Init();
	m_renderSystem.Init(m_window.GetWindow());
	mainLoop();
	cleanup();
}
//-----------------------------------------------------------------------------
void lEngine::mainLoop()
{
	while (!m_window.IsClose())
	{
		m_window.Events();
		m_renderSystem.drawFrame(m_window.GetFramebufferResized());
	}
}
//-----------------------------------------------------------------------------
void lEngine::cleanup()
{
	m_renderSystem.Close();
	m_window.Close();
}
//-----------------------------------------------------------------------------
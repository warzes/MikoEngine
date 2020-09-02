#include "stdafx.h"
#include "CoreLogSystem.h"
#include "PlatformCoreLog.h"
//-----------------------------------------------------------------------------
CoreLogSystem::CoreLogSystem()
#if !SE_PLATFORM_EMSCRIPTEN
	: CoreLogThread(CoreLogSystem::logString)
#endif
{
	m_instance = this;
#if !SE_PLATFORM_EMSCRIPTEN
	m_thread = std::thread(&CoreLogThread::LogLoop, this);
#endif
}
//-----------------------------------------------------------------------------
CoreLogSystem::~CoreLogSystem()
{
	m_instance = nullptr;
}
//-----------------------------------------------------------------------------
void CoreLogSystem::Print(std::string_view str, const LogVerbosity verbosity)
{
	assert(m_instance);
#if SE_PLATFORM_EMSCRIPTEN
	m_instance->logString(str, verbosity);
#else
	m_instance->print(str.data(), verbosity);
#endif
}
//-----------------------------------------------------------------------------
void CoreLogSystem::logString(std::string_view str, const LogVerbosity verbosity)
{
	PlatformLogPrint(str, verbosity);
}
//-----------------------------------------------------------------------------
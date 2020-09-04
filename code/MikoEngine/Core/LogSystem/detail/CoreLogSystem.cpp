#include "stdafx.h"
#include "CoreLogSystem.h"
#include "PlatformCoreLog.h"
#include "Core/AssertMacros.h"
//-----------------------------------------------------------------------------
static CoreLogSystem logsystem;
//-----------------------------------------------------------------------------
CoreLogSystem::CoreLogSystem()
{
	m_instance = this;
}
//-----------------------------------------------------------------------------
void CoreLogSystem::Print(std::string_view str, const LogVerbosity verbosity)
{	
#if SE_PLATFORM_EMSCRIPTEN
	PlatformLogPrint(str, verbosity);
#else
	SE_ASSERT(m_instance, "CoreLogSystem no create!!!");
	m_instance->print(str.data(), verbosity);
#endif
}
//-----------------------------------------------------------------------------
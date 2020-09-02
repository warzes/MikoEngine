#pragma once

#if !SE_PLATFORM_EMSCRIPTEN
#	include "CoreLogThread.h"
#endif

class CoreLogSystem
#if !SE_PLATFORM_EMSCRIPTEN
	: public CoreLogThread
#endif
{
public:
	CoreLogSystem();
	~CoreLogSystem();

	static void Print(std::string_view str, const LogVerbosity verbosity = LogVerbosity::Info);

private:
	CoreLogSystem(const CoreLogSystem&) = delete;
	CoreLogSystem operator=(const CoreLogSystem&) = delete;

	static void logString(std::string_view str, const LogVerbosity verbosity);

	static inline CoreLogSystem *m_instance = nullptr;
};
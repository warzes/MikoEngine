#pragma once

#if !SE_PLATFORM_EMSCRIPTEN

#include "LogVerbosity.h"

class CoreLogThread
{
public:
	CoreLogThread();
	~CoreLogThread();

	void LogLoop();

protected:
	void close();
	void print(const std::string &str, const LogVerbosity verbosity = LogVerbosity::Info) const;

	struct commandLog
	{
		commandLog(const LogVerbosity initVerbosity, const std::string &initString) noexcept
			: verbosity(initVerbosity)
			, str(initString)
		{
		}

		LogVerbosity verbosity;
		std::string  str;
	};

	mutable std::condition_variable m_queueCondition;
	mutable std::mutex m_queueMutex;
	mutable std::queue<commandLog> m_logQueue;
	std::thread m_thread;

	bool m_isClose = false;
	bool m_quitCommand = false;
};

#endif
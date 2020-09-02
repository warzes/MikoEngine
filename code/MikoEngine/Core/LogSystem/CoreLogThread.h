#pragma once

#if !SE_PLATFORM_EMSCRIPTEN

#include "LogVerbosity.h"

class CoreLogThread
{
	typedef void logFlushImpl(std::string_view str, const LogVerbosity verbosity);
public:
	CoreLogThread(logFlushImpl *flushFunc);
	~CoreLogThread();

	void LogLoop();

protected:
	void close();
	void print(const std::string &str, const LogVerbosity verbosity = LogVerbosity::Info) const;

	struct command
	{
		enum class Type
		{
			LogString,
			Quit
		};

		explicit command(const Type initType) : type(initType)
		{
		}
		command(const Type initType, const LogVerbosity initVerbosity, const std::string &initString)
			: type(initType)
			, verbosity(initVerbosity)
			, str(initString)
		{
		}

		Type type;
		LogVerbosity verbosity;
		std::string str;
	};

	mutable std::condition_variable m_queueCondition;
	mutable std::mutex m_queueMutex;
	mutable std::queue<command> m_commandQueue;
	std::thread m_thread;

	logFlushImpl *m_logFlushImpl;
	bool m_isClose = false;
};

#endif
#pragma once

// TODO: логгер взять старый, с многопоточностью

#define SE_LOG_INFO(x)    logger::Log(x, std::string(__FILE__), __LINE__, logger::LEVEL_INFO)
#define SE_LOG_WARNING(x) logger::Log(x, std::string(__FILE__), __LINE__, logger::LEVEL_WARNING)
#define SE_LOG_ERROR(x)   logger::Log(x, std::string(__FILE__), __LINE__, logger::LEVEL_ERR)
#define SE_LOG_FATAL(x)   logger::Log(x, std::string(__FILE__), __LINE__, logger::LEVEL_FATAL)

namespace logger
{
	enum LogLevel
	{
		LEVEL_INFO    = 0,
		LEVEL_WARNING = 1,
		LEVEL_ERR     = 2,
		LEVEL_FATAL   = 3
	};

	enum LogVerbosity
	{
		VERBOSITY_BASIC     = 0x00,
		VERBOSITY_TIMESTAMP = 0x01,
		VERBOSITY_LEVEL     = 0x02,
		VERBOSITY_FILE      = 0x04,
		VERBOSITY_LINE      = 0x08,
		VERBOSITY_ALL       = 0x0f
	};

	void Init(int flags = VERBOSITY_ALL);
	void Close();

	// Main log method. File, line and level are required in addition to log message.
	void Log(const std::string &text, const std::string &file, int line, LogLevel level);

	// Explicitly flush all streams.
	void Flush();
} // namespace logger
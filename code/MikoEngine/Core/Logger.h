#pragma once

#define SE_LOG_INFO(x)    logger::log(x, std::string(__FILE__), __LINE__, logger::LEVEL_INFO)
#define SE_LOG_WARNING(x) logger::log(x, std::string(__FILE__), __LINE__, logger::LEVEL_WARNING)
#define SE_LOG_ERROR(x)   logger::log(x, std::string(__FILE__), __LINE__, logger::LEVEL_ERR)
#define SE_LOG_FATAL(x)   logger::log(x, std::string(__FILE__), __LINE__, logger::LEVEL_FATAL)

namespace logger
{
	enum LogLevel
	{
		LEVEL_INFO = 0,
		LEVEL_WARNING = 1,
		LEVEL_ERR = 2,
		LEVEL_FATAL = 3
	};

	enum LogVerbosity
	{
		VERBOSITY_BASIC = 0x00,
		VERBOSITY_TIMESTAMP = 0x01,
		VERBOSITY_LEVEL = 0x02,
		VERBOSITY_FILE = 0x04,
		VERBOSITY_LINE = 0x08,
		VERBOSITY_ALL = 0x0f
	};

	// Custom stream callback type. Use to implement your own logging stream such as through a network etc.
	typedef void (*CustomStreamCallback)(std::string, LogLevel);

	extern void initialize();
	extern void set_verbosity(int flags);

	// Open streams.
	extern void open_file_stream();
	extern void open_console_stream();
	extern void open_custom_stream(CustomStreamCallback callback);

	// Close streams.
	extern void close_file_stream();
	extern void close_console_stream();
	extern void close_custom_stream();

	// Debug mode. These will flush the stream immediately after each log.
	extern void enable_debug_mode();
	extern void disable_debug_mode();

	// Main log method. File, line and level are required in addition to log message.
	extern void log(std::string text, std::string file, int line, LogLevel level);

	// Simplified API.
	extern void log_info(std::string text);
	extern void log_error(std::string text);
	extern void log_warning(std::string text);
	extern void log_fatal(std::string text);

	// Explicitly flush all streams.
	extern void flush();
} // namespace logger
#include "stdafx.h"
#include "Logger.h"

#define FILE_STREAM_INDEX    0
#define CONSOLE_STREAM_INDEX 1
#define CUSTOM_STREAM_INDEX  2

#define LOG_SEPERATOR                                                            \
    "**************************************************************************" \
    "******************************\n"

namespace logger
{
	struct LoggerState
	{
		bool                 _open_streams[3];
		std::mutex           _log_mutex;
		std::ofstream        _stream;
		std::time_t          _rawtime;
		std::tm*             _timeinfo;
		int                  _verbosity;
		char                 _temp_buffer[80];
		bool                 _debug;
	};

	LoggerState g_logger;

	void Init(int flags)
	{
		for ( int i = 0; i < 3; i++ )
			g_logger._open_streams[i] = false;

		g_logger._verbosity = flags;
#if SE_DEBUG
		g_logger._debug = true;
#else
		g_logger._debug = false;
#endif
		// Open Console Stream
		g_logger._open_streams[CONSOLE_STREAM_INDEX] = true;
		std::time(&g_logger._rawtime);
		std::cout << LOG_SEPERATOR;
		std::cout << std::ctime(&g_logger._rawtime) << "Log Started.\n";
		std::cout << LOG_SEPERATOR;

		// Open File Stream
		g_logger._open_streams[FILE_STREAM_INDEX] = true;
		g_logger._stream.open("log.txt", std::ios::app | std::ofstream::out);
		std::time(&g_logger._rawtime);
		g_logger._stream << LOG_SEPERATOR;
		g_logger._stream << std::ctime(&g_logger._rawtime) << "Log Started.\n";
		g_logger._stream << LOG_SEPERATOR;
	}

	void Close()
	{
		// Close file stream
		g_logger._open_streams[FILE_STREAM_INDEX] = false;

		std::time(&g_logger._rawtime);
		g_logger._stream << LOG_SEPERATOR;
		g_logger._stream << std::ctime(&g_logger._rawtime) << "Log Ended.\n";
		g_logger._stream << LOG_SEPERATOR;

		g_logger._stream.close();

		// Close console stream
		g_logger._open_streams[CONSOLE_STREAM_INDEX] = false;
		std::time(&g_logger._rawtime);
		std::cout << LOG_SEPERATOR;
		std::cout << std::ctime(&g_logger._rawtime) << "Log Ended.\n";
		std::cout << LOG_SEPERATOR;
	}

	void Log(const std::string &text, const std::string &file, int line, LogLevel level)
	{
		std::lock_guard<std::mutex> lock(g_logger._log_mutex);

		std::string file_with_extension = file.substr(file.find_last_of("/\\") + 1);
		std::time(&g_logger._rawtime);
		g_logger._timeinfo = std::localtime(&g_logger._rawtime);
		std::strftime(g_logger._temp_buffer, 80, "%H:%M:%S", g_logger._timeinfo);

		std::string log_level_string;

		switch ( level )
		{
		case LEVEL_INFO:
			{
				log_level_string = "INFO   ";
				break;
			}
		case LEVEL_WARNING:
			{
				log_level_string = "WARNING";
				break;
			}
		case LEVEL_ERR:
			{
				log_level_string = "ERROR  ";
				break;
			}
		case LEVEL_FATAL:
			{
				log_level_string = "FATAL  ";
				break;
			}
		}

		std::string output;

		if ( (g_logger._verbosity & VERBOSITY_TIMESTAMP) || (g_logger._verbosity & VERBOSITY_LEVEL) )
		{
			output = "[ ";

			if ( g_logger._verbosity & VERBOSITY_TIMESTAMP )
				output += g_logger._temp_buffer;

			if ( (g_logger._verbosity & VERBOSITY_TIMESTAMP) && (g_logger._verbosity & VERBOSITY_LEVEL) )
				output += " | ";

			if ( g_logger._verbosity & VERBOSITY_LEVEL )
				output += log_level_string;

			output += " ] : ";
		}

		output += text;

		if ( g_logger._verbosity & VERBOSITY_FILE )
		{
			output += " , FILE : ";
			output += file_with_extension;
		}

		if ( g_logger._verbosity & VERBOSITY_LINE )
		{
			output += " , LINE : ";
			output += std::to_string(line);
		}

		if ( g_logger._open_streams[FILE_STREAM_INDEX] )
		{
			g_logger._stream << output << "\n";
		}

		if ( g_logger._open_streams[CONSOLE_STREAM_INDEX] )
		{
			std::cout << output << "\n";
		}

		// Flush stream if error
		if ( level == LEVEL_ERR || level == LEVEL_FATAL || g_logger._debug )
			Flush();
	}

	void Flush()
	{
		if ( g_logger._open_streams[FILE_STREAM_INDEX] )
		{
			g_logger._stream.flush();
		}
	}
} // namespace logger
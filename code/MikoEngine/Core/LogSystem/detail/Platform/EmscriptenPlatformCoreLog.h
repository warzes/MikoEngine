#pragma once

inline void PlatformLogPrint(std::string_view str, const LogVerbosity verbosity) noexcept
{
	int flags = EM_LOG_CONSOLE;
	switch ( level )
	{
	case Log::Level::Error:   flags |= EM_LOG_ERROR; break;
	case Log::Level::Warning: flags |= EM_LOG_WARN;  break;
	case Log::Level::Info:                           break;
	case Log::Level::All:                            break;
	default:                                         return;
	}
	emscripten_log(flags, "%s", str.data());
}
#pragma once

inline void PlatformLogPrint(std::string_view str, const LogVerbosity verbosity) noexcept
{
	int priority = 0;
	switch ( level )
	{
	case Log::Level::Error:   priority = ANDROID_LOG_ERROR; break;
	case Log::Level::Warning: priority = ANDROID_LOG_WARN;  break;
	case Log::Level::Info:    priority = ANDROID_LOG_INFO;  break;
	case Log::Level::All:     priority = ANDROID_LOG_DEBUG; break;
	default:                                                return;
	}
	__android_log_print(priority, "Sapphire", "%s", str.data());
}
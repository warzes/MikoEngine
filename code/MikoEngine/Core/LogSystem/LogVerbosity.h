#pragma once

enum class LogVerbosity : uint8_t
{
	Fatal,
	Error,
	Warning,
	Info,
	Debug,
};

inline constexpr const char* LogTypeToString(LogVerbosity type)
{
	switch ( type )
	{
	case LogVerbosity::Fatal:   return "Fatal";
	case LogVerbosity::Error:   return "Error";
	case LogVerbosity::Warning: return "Warning";
	case LogVerbosity::Info:    return "Info";
	case LogVerbosity::Debug:   return "Debug";
	default:                    return "";
	}
}
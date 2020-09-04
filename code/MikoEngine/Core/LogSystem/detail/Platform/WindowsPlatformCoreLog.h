#pragma once

inline void PlatformLogPrint(std::string_view str, const LogVerbosity verbosity) noexcept
{
#if SE_DEBUG
	OutputDebugStringA(str.data());

	HANDLE handle;
	switch ( verbosity )
	{
	case LogVerbosity::Error:
	case LogVerbosity::Warning:
		handle = GetStdHandle(STD_ERROR_HANDLE);
		break;
	case LogVerbosity::Info:
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		break;
	default: return;
	}

	DWORD bytesWritten;
	WriteConsoleA(handle, str.data(), str.size(), &bytesWritten, nullptr);
#endif
}
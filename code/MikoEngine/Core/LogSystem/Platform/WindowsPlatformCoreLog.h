#pragma once

inline void PlatformLogPrint(std::string_view str, const LogVerbosity verbosity)
{
	const int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, nullptr, 0);
	if ( bufferSize == 0 )
		return;

	std::vector<WCHAR> buffer(bufferSize + 1); // +1 for the newline
	if ( MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, buffer.data(), static_cast<int>(buffer.size())) == 0 )
		return;

	if ( FAILED(StringCchCatW(buffer.data(), buffer.size(), L"\n")) )
		return;

	OutputDebugStringW(buffer.data());
#if SE_DEBUG
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
	WriteConsoleW(handle, buffer.data(), static_cast<DWORD>(wcslen(buffer.data())), &bytesWritten, nullptr);
#endif
}
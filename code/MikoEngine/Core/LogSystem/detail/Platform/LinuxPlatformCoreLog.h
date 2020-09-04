#pragma once

inline void PlatformLogPrint(std::string_view str, const LogVerbosity verbosity) noexcept
{
	int fd = 0;
	switch ( level )
	{
	case Log::Level::Error:
	case Log::Level::Warning:
		fd = STDERR_FILENO;
		break;
	case Log::Level::Info:
	case Log::Level::All:
		fd = STDOUT_FILENO;
		break;
	default: return;
	}

	std::vector<char> output(str.begin(), str.end());
	output.push_back('\n');

	size_t offset = 0;
	while ( offset < output.size() )
	{
		const ssize_t written = write(fd, output.data() + offset, output.size() - offset);
		if ( written == -1 )
			return;

		offset += static_cast<size_t>(written);
	}
}
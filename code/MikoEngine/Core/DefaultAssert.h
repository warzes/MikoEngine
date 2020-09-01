#pragma once

/**
*  @brief
*    Abstract assert interface
*
*  @note
*    - The implementation must be multithreading safe since the engine is allowed to internally use multiple threads
*
*  @note
*    - Example: SE_ASSERT(isInitialized, "Direct3D 11 RHI implementation assert failed")
*    - On Microsoft Windows it will print to the Visual Studio output console and the debugger will break
*    - On Linux it will print on the console
*/
class DefaultAssert
{
public:
	DefaultAssert() noexcept = default;

	[[nodiscard]] bool HandleAssert(const char* expression, const char* file, uint32_t line, const char* format, ...) noexcept
	{
		bool requestDebugBreak = false;

		// Get the required buffer length, does not include the terminating zero character
		va_list vaList;
		va_start(vaList, format);
		const uint32_t textLength = static_cast<uint32_t>(vsnprintf(nullptr, 0, format, vaList));
		va_end(vaList);
		if ( 256 > textLength )
		{
			// Fast path: C-runtime stack

			// Construct the formatted text
			char formattedText[256];	// 255 +1 for the terminating zero character
			va_start(vaList, format);
			vsnprintf(formattedText, 256, format, vaList);
			va_end(vaList);

			// Internal processing
			requestDebugBreak = handleAssertInternal(expression, file, line, formattedText, textLength);
		}
		else
		{
			// Slow path: Heap
			// -> No reused scratch buffer in order to reduce memory allocation/deallocations in here to not make things more complex and to reduce the mutex locked scope

			// Construct the formatted text
			char* formattedText = new char[textLength + 1];	// 1+ for the terminating zero character
			va_start(vaList, format);
			vsnprintf(formattedText, textLength + 1, format, vaList);
			va_end(vaList);

			// Internal processing
			requestDebugBreak = handleAssertInternal(expression, file, line, formattedText, textLength);

			// Cleanup
			delete[] formattedText;
		}

		// Done
		return requestDebugBreak;
	}

	static DefaultAssert& Get() noexcept
	{
		static DefaultAssert assert;
		return assert;
	}

private:
	explicit DefaultAssert(const DefaultAssert&) = delete;
	DefaultAssert& operator=(const DefaultAssert&) = delete;

	[[nodiscard]] bool handleAssertInternal(const char* expression, const char* file, uint32_t line, const char* message, uint32_t) noexcept
	{
		std::lock_guard<std::mutex> mutexLock(m_mutex);
		bool requestDebugBreak = false;

		// Construct the full UTF-8 message text
		const std::string fullMessage = "Assert message \"" + std::string(message) + "\" | Expression \"" + std::string(expression) + "\" | File \"" + std::string(file) + "\" | Line " + std::to_string(line) + '\n';

		// Platform specific handling
#if SE_PLATFORM_WINDOWS
		{
			// Write into standard output stream
			std::cerr << fullMessage.c_str();
			// On Microsoft Windows, ensure the output can be seen inside the Visual Studio output window as well
			::OutputDebugStringA(fullMessage.c_str());
			if ( ::IsDebuggerPresent() )
			{
				requestDebugBreak = true;
			}
		}
#elif SE_PLATFORM_ANDROID
		__android_log_write(ANDROID_LOG_DEBUG, "MikoEngine", fullMessage.c_str());	// TODO(co) Might make sense to make the app-name customizable
		requestDebugBreak = true;
#elif SE_PLATFORM_LINUX
	// Write into standard output stream
		std::cerr << fullMessage.c_str();
		requestDebugBreak = true;
#else
#	error "Unsupported platform"
#endif
// Done
		return requestDebugBreak;
	}

	std::mutex m_mutex;
};
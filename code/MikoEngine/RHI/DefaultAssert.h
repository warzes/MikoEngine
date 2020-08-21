#pragma once

/**
*  @brief
*    Abstract assert interface
*
*  @note
*    - The implementation must be multithreading safe since the RHI is allowed to internally use multiple threads
*/
/**
*  @brief
*    Default assert implementation class one can use
*
*  @note
*    - Example: RHI_ASSERT(isInitialized, "Direct3D 11 RHI implementation assert failed")
*    - Designed to be instanced and used inside a single C++ file
*    - On Microsoft Windows it will print to the Visual Studio output console and the debugger will break
*    - On Linux it will print on the console
*/
class DefaultAssert
{
public:
	DefaultAssert() = default;
	~DefaultAssert() = default;
	/**
	*  @brief
	*    Handle assert
	*
	*  @param[in] expression
	*    Expression as ASCII string
	*  @param[in] file
	*    File as ASCII string
	*  @param[in] line
	*    Line number
	*  @param[in] format
	*    "snprintf"-style formatted UTF-8 assert message
	*
	*  @return
	*    "true" to request debug break, else "false"
	*/
	[[nodiscard]] bool handleAssert(const char* expression, const char* file, uint32_t line, const char* format, ...)
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

	// Protected methods
private:
	explicit DefaultAssert(const DefaultAssert&) = delete;
	DefaultAssert& operator=(const DefaultAssert&) = delete;

	/*
	*  @brief
	*    Receives an already formatted message for further processing
	*
	*  @param[in] expression
	*    Expression as ASCII string
	*  @param[in] file
	*    File as ASCII string
	*  @param[in] line
	*    Line number
	*  @param[in] message
	*    UTF-8 message
	*  @param[in] numberOfCharacters
	*    Number of characters inside the message, does not include the terminating zero character
	*
	*  @return
	*    "true" to request debug break, else "false"
	*/
	[[nodiscard]] bool handleAssertInternal(const char* expression, const char* file, uint32_t line, const char* message, uint32_t)
	{
		std::lock_guard<std::mutex> mutexLock(mMutex);
		bool requestDebugBreak = false;

		// Construct the full UTF-8 message text
		std::string fullMessage = "Assert message \"" + std::string(message) + "\" | Expression \"" + std::string(expression) + "\" | File \"" + std::string(file) + "\" | Line " + std::to_string(line);
		if ( '\n' != fullMessage.back() )
		{
			fullMessage += '\n';
		}

		// Platform specific handling
#if SE_PLATFORM_WINDOWS
		{
			// Convert UTF-8 string to UTF-16
			std::wstring utf16Line;
			utf16Line.resize(static_cast<std::wstring::size_type>(::MultiByteToWideChar(CP_UTF8, 0, fullMessage.c_str(), static_cast<int>(fullMessage.size()), nullptr, 0)));
			::MultiByteToWideChar(CP_UTF8, 0, fullMessage.c_str(), static_cast<int>(fullMessage.size()), utf16Line.data(), static_cast<int>(utf16Line.size()));

			// Write into standard output stream
			std::wcerr << utf16Line.c_str();

			// On Microsoft Windows, ensure the output can be seen inside the Visual Studio output window as well
			::OutputDebugStringW(utf16Line.c_str());
			if ( ::IsDebuggerPresent() )
			{
				requestDebugBreak = true;
			}
		}
#elif __ANDROID__
		__android_log_write(ANDROID_LOG_DEBUG, "MikoEngine", fullMessage.c_str());	// TODO(co) Might make sense to make the app-name customizable
		requestDebugBreak = true;
#elif LINUX
	// Write into standard output stream
		std::cerr << fullMessage.c_str();
		requestDebugBreak = true;
#else
#error "Unsupported platform"
#endif

// Done
		return requestDebugBreak;
	}

	std::mutex mMutex;
};

DefaultAssert& GetAssert();

/**
*  @brief
*    Ease-of-use assert macro
*
*  @param[in] context
*    RHI context to ask for the assert interface
*  @param[in] expression
*    Expression which must be true, else the assert triggers
*  @param[in] format
*    "snprintf"-style formatted UTF-8 assert message
*
*  @note
*    - Example: RHI_ASSERT(isInitialized, "Direct3D 11 RHI implementation assert failed")
*    - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { … } while(0)." for background information about the do-while wrap
*/
#if SE_DEBUG
#	define RHI_ASSERT(expression, format, ...) \
		do \
		{ \
			if (!(expression) && GetAssert().handleAssert(#expression, __FILE__, static_cast<uint32_t>(__LINE__), format, ##__VA_ARGS__)) \
			{ \
				SE_DEBUG_BREAK; \
			} \
		} while (0);
#else
#	define RHI_ASSERT(expression, format, ...)
#endif
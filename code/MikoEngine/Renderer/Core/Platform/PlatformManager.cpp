#include "stdafx.h"
#include "Renderer/Core/Platform/PlatformManager.h"
#include "RHI/DefaultAssert.h"

//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Windows                                               ]
		//[-------------------------------------------------------]
		#ifdef _WIN32
			const DWORD MS_VC_EXCEPTION = 0x406D1388;

			#pragma pack(push, 8)
				typedef struct tagTHREADNAME_INFO
				{
					DWORD  dwType;		// Must be 0x1000
					LPCSTR szName;		// Pointer to name (in user address space)
					DWORD  dwThreadID;	// Thread ID (-1 = caller thread)
					DWORD  dwFlags;		// Reserved for future use, must be zero
				} THREADNAME_INFO;
			#pragma pack(pop)

			void setThreadName(uint32_t dwThreadID, const char* name)
			{
				THREADNAME_INFO info;
				info.dwType		= 0x1000;
				info.szName		= name;
				info.dwThreadID = dwThreadID;
				info.dwFlags	= 0;
				__try
				{
					::RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					// Nothing here
				}
			}
		#elif LINUX
			// Nothing special here
		#else
			#error "Unsupported platform"
		#endif


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void PlatformManager::setCurrentThreadName([[maybe_unused]] const char* shortName, [[maybe_unused]] const char* descriptiveName)
	{
		// "pthread_setname_np()" support only up to 16 characters (including the terminating zero), so this is our limiting factor
		RHI_ASSERT((strlen(shortName) + 1) <= 16, "Invalid short name")	// +1 for the terminating zero
		RHI_ASSERT(strlen(descriptiveName) >= strlen(shortName), "Invalid descriptive name")

		// Platform specific part
		#ifdef _WIN32
			::detail::setThreadName(::GetCurrentThreadId(), descriptiveName);
		#elif LINUX
			::pthread_setname_np(pthread_self(), shortName);
		#elif __APPLE__
			#warning "Mac OS X: Renderer::PlatformManager::setCurrentThreadName() is untested"	// TODO(co) Not tested
			::pthread_setname_np(shortName);
		#else
			#error "Unsupported platform"
		#endif
	}

	bool PlatformManager::execute([[maybe_unused]] const char* command, [[maybe_unused]] const char* parameters, [[maybe_unused]] AbsoluteDirectoryName workingDirectory)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != command, "Invalid command")
		RHI_ASSERT(strlen(command) != 0, "Invalid command")
		RHI_ASSERT(nullptr != parameters, "Invalid parameters")
		RHI_ASSERT(nullptr != workingDirectory, "Invalid working directory")

		// Platform specific part
		#ifdef _WIN32
			// Define a helper macro: Convert UTF-8 string to UTF-16
			#define UTF8_TO_UTF16(utf8String, utf16String) \
				std::wstring utf16String; \
				utf16String.resize(static_cast<std::wstring::size_type>(::MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, nullptr , 0))); \
				::MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, utf16String.data(), static_cast<int>(utf16String.size()));

			// Execute command
			UTF8_TO_UTF16(command, utf16Command)
			UTF8_TO_UTF16(parameters, utf16Parameters)
			UTF8_TO_UTF16(workingDirectory, utf16WorkingDirectory)
			const HINSTANCE result = ::ShellExecuteW(nullptr, L"open", utf16Command.c_str(), utf16Parameters.c_str(), utf16WorkingDirectory.c_str(), SW_SHOWDEFAULT);

			// Undefine the helper macro
			#undef UTF8_TO_UTF16

			// Has the execution been successful?
			return (result > HINSTANCE(32));
		#elif LINUX
			#warning "Renderer::PlatformManager::execute() isn't implemented"

			// Error!
			return false;
		#else
			#error "Unsupported platform"
		#endif
	}

	bool PlatformManager::openUrlExternal([[maybe_unused]] const char* url)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != url, "Invalid URL");
		RHI_ASSERT(strlen(url) != 0, "Invalid URL");

		// Platform specific part
		#ifdef _WIN32
			// Execute command
			return execute("explorer", url);
		#elif LINUX
			#warning "Renderer::PlatformManager::openUrlExternal() isn't implemented"

			// Error!
			return false;
		#else
			#error "Unsupported platform"
		#endif
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

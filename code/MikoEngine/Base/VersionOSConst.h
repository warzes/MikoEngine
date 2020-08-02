#pragma once

#if SE_PLATFORM_WINDOWS
#	define SE_WINDOWS_XP          (1)
#	define SE_WINDOWS_7           (SE_WINDOWS_XP + 1)
#	define SE_WINDOWS_8           (SE_WINDOWS_7  + 1)
#	define SE_WINDOWS_10          (SE_WINDOWS_8  + 1)
#endif

#if SE_PLATFORM_LINUX
#	error "no impl!!!"
#endif

#if SE_PLATFORM_ANDROID
#	error "no impl!!!"
#endif
//#define _GLFW_BUILD_DLL           // To build shared version
//http://www.glfw.org/docs/latest/compile.html#compile_manual

#if defined(_WIN32)
#define _GLFW_WIN32
#endif
#if defined(__linux__)
#if !defined(_GLFW_WAYLAND)     // Required for Wayland windowing
#define _GLFW_X11
#endif
#endif
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#define _GLFW_X11
#endif
#if defined(__APPLE__)
#define _GLFW_COCOA
#define _GLFW_USE_MENUBAR       // To create and populate the menu bar when the first window is created
#define _GLFW_USE_RETINA        // To have windows use the full resolution of Retina displays
#endif
#if defined(__TINYC__)
#define _WIN32_WINNT_WINXP      0x0501
#endif

// NOTE: _GLFW_MIR experimental platform not supported at this moment

#include "GLFW/src/context.c"
#include "GLFW/src/init.c"
#include "GLFW/src/input.c"
#include "GLFW/src/monitor.c"
#include "GLFW/src/vulkan.c"
#include "GLFW/src/window.c"

#if defined(_WIN32)
#include "GLFW/src/win32_init.c"
#include "GLFW/src/win32_joystick.c"
#include "GLFW/src/win32_monitor.c"
#include "GLFW/src/win32_time.c"
#include "GLFW/src/win32_thread.c"
#include "GLFW/src/win32_window.c"
#include "GLFW/src/wgl_context.c"
#include "GLFW/src/egl_context.c"
#include "GLFW/src/osmesa_context.c"
#endif

#if defined(__linux__)
#if defined(_GLFW_WAYLAND)
#include "GLFW/src/wl_init.c"
#include "GLFW/src/wl_monitor.c"
#include "GLFW/src/wl_window.c"
#include "GLFW/src/wayland-pointer-constraints-unstable-v1-client-protocol.c"
#include "GLFW/src/wayland-relative-pointer-unstable-v1-client-protocol.c"
#endif
#if defined(_GLFW_X11)
#include "GLFW/src/x11_init.c"
#include "GLFW/src/x11_monitor.c"
#include "GLFW/src/x11_window.c"
#include "GLFW/src/glx_context.c"
#endif

#include "GLFW/src/linux_joystick.c"
#include "GLFW/src/posix_thread.c"
#include "GLFW/src/posix_time.c"
#include "GLFW/src/xkb_unicode.c"
#include "GLFW/src/egl_context.c"
#include "GLFW/src/osmesa_context.c"
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined( __NetBSD__) || defined(__DragonFly__)
#include "GLFW/src/x11_init.c"
#include "GLFW/src/x11_monitor.c"
#include "GLFW/src/x11_window.c"
#include "GLFW/src/xkb_unicode.c"
// TODO: Joystick implementation
#include "GLFW/src/null_joystick.c"
#include "GLFW/src/posix_time.c"
#include "GLFW/src/posix_thread.c"
#include "GLFW/src/glx_context.c"
#include "GLFW/src/egl_context.c"
#include "GLFW/src/osmesa_context.c"
#endif

#if defined(__APPLE__)
#include "GLFW/src/cocoa_init.m"
#include "GLFW/src/cocoa_joystick.m"
#include "GLFW/src/cocoa_monitor.m"
#include "GLFW/src/cocoa_window.m"
#include "GLFW/src/cocoa_time.c"
#include "GLFW/src/posix_thread.c"
#include "GLFW/src/nsgl_context.m"
#include "GLFW/src/egl_context.c"
#include "GLFW/src/osmesa_context.c"
#endif
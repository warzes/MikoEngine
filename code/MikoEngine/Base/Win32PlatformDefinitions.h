#pragma once

#ifdef _WIN32_WINDOWS
#	undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
#	undef _WIN32_WINNT
#endif
#ifdef WINVER
#	undef WINVER
#endif

#if   SE_WINDOWS_VERSION == SE_WINDOWS_XP
#	define _WIN32_WINNT 0x0501
#elif SE_WINDOWS_VERSION == SE_WINDOWS_7
#	define _WIN32_WINNT 0x0601
#elif SE_WINDOWS_VERSION == SE_WINDOWS_8
#	define _WIN32_WINNT 0x0602
#elif SE_WINDOWS_VERSION == SE_WINDOWS_10
#	define _WIN32_WINNT 0x0A00
#else
#	error "Error Select Windows Build!"
#endif

#define _WIN32_WINDOWS _WIN32_WINNT
#define WINVER _WIN32_WINNT
#define DIRECTINPUT_VERSION 0x0800

#define WIN32_LEAN_AND_MEAN      // 
#define WIN32_EXTRA_LEAN         // 
#define VC_EXTRALEAN             // 
#define NOIME                    // 
#define NOWINRES                 // 
#define NOGDICAPMASKS            // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES        // VK_*
#define NOWINMESSAGES            // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES              // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS             // SM_*
#define NOMENUS                  // MF_*
#define NOICONS                  // IDI_*
#define NOKEYSTATES              // MK_*
#define NOSYSCOMMANDS            // SC_*
#define NORASTEROPS              // Binary and Tertiary raster ops
#define NOSHOWWINDOW             // SW_*
#define OEMRESOURCE              // OEM Resource values
#define NOATOM                   // Atom Manager routines
#define NOCLIPBOARD              // Clipboard routines
#define NOCOLOR                  // Screen colors
#define NOCTLMGR                 // Control and Dialog routines
#define NODRAWTEXT               // DrawText() and DT_*
#define NOGDI                    // All GDI defines and routines
#define NOBITMAP                 // 
#define NOUSER                   // All USER defines and routines
#define NOMB                     // MB_* and MessageBox()
#define NOMEMMGR                 // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE               // typedef METAFILEPICT
#define NOMINMAX                 // Macros min(a, b) and max(a, b)
#define NOMSG                    // typedef MSG and associated routines
#define NOOPENFILE               // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL                 // SB_* and scrolling routines
#define NOSERVICE                // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND                  // Sound driver routines
#define NOTEXTMETRIC             // typedef TEXTMETRIC and associated routines
#define NOWH                     // SetWindowsHook and WH_*
#define NOWINOFFSETS             // GWL_*, GCL_*, associated routines
#define NOCOMM                   // COMM driver routines
#define NOKANJI                  // Kanji support stuff.
#define NOHELP                   // Help engine interface. (WinHelp is deprecated)
#define NOPROFILER               // Profiler interface.
#define NODEFERWINDOWPOS         // DeferWindowPos routines
#define NOMCX                    // Modem Configuration Extensions
#define NOPROXYSTUB              // 
#define NOIMAGE                  // 
#define NO                       // 
#define NOTAPE                   // 
#define ANSI_ONLY                // 
#define NOCRYPT                  // 
#define NOKERNEL                 // All KERNEL defines and routines
#define NONLS                    // All NLS defines and routines
#pragma once

// TODO: rename

namespace Rhi
{
	class Context
	{
	public:

		inline Context(handle nativeWindowHandle = 0) : mNativeWindowHandle(nativeWindowHandle)
		{
		}

		[[nodiscard]] inline handle getNativeWindowHandle() const
		{
			return mNativeWindowHandle;
		}

	private:
		explicit Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

		handle mNativeWindowHandle;
	};

	// Holds information about an window into which the rendering should be done
	struct WindowHandle final
	{
		handle nativeWindowHandle; // The native window handle
		IRenderWindow *renderWindow; // A pointer to an "Rhi::IRenderWindow"-instance, can be a null pointer
	};
} // namespace Rhi
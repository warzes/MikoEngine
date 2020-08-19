#pragma once

namespace OpenGLRhi
{
/**
*  @brief
*    Abstract OpenGL context interface
*
*  @remarks
*    While the OpenGL specification is platform independent, creating an OpenGL context is not.
*
*  @note
*    - Every native OS window needs its own context instance
*/
	class IOpenGLContext
	{

		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IOpenGLContext()
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual IOpenGLContext methods                 ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return whether or not the content is initialized
		*
		*  @return
		*    "true" if the context is initialized, else "false"
		*/
		[[nodiscard]] virtual bool isInitialized() const = 0;

		/**
		*  @brief
		*    Make the context current
		*/
		virtual void makeCurrent() const = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRuntimeLinking
		*    OpenGL runtime linking instance, if null pointer this isn't a primary context
		*/
		inline explicit IOpenGLContext(OpenGLRuntimeLinking* openGLRuntimeLinking) :
			mOpenGLRuntimeLinking(openGLRuntimeLinking)
		{
		}

		explicit IOpenGLContext(const IOpenGLContext&) = delete;
		IOpenGLContext& operator =(const IOpenGLContext&) = delete;

		/**
		*  @brief
		*    Load the >= OpenGL 3.0 entry points
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - This method is only allowed to be called after an >= OpenGL context has been created and set
		*/
		[[nodiscard]] inline bool loadOpenGL3EntryPoints() const
		{
			return (nullptr != mOpenGLRuntimeLinking) ? mOpenGLRuntimeLinking->loadOpenGL3EntryPoints() : true;
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		OpenGLRuntimeLinking* mOpenGLRuntimeLinking;	// OpenGL runtime linking instance, if null pointer this isn't a primary context


	};

#if SE_PLATFORM_WINDOWS
	//[-------------------------------------------------------]
	//[ OpenGLRhi/OpenGLContextWindows.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Windows OpenGL context class
	*/
	class OpenGLContextWindows : public IOpenGLContext
	{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class OpenGLRhi;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] depthStencilAttachmentTextureFormat
		*    Depth stencil attachment texture format
		*  @param[in] nativeWindowHandle
		*    Optional native main window handle, can be a null handle
		*  @param[in] shareContextWindows
		*    Optional share context, can be a null pointer
		*/
		inline OpenGLContextWindows(Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, handle nativeWindowHandle, const OpenGLContextWindows* shareContextWindows = nullptr) :
			OpenGLContextWindows(nullptr, depthStencilAttachmentTextureFormat, nativeWindowHandle, shareContextWindows)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OpenGLContextWindows() override
		{
			// Release the device context of the OpenGL window
			if ( SE_NULL_HANDLE != mWindowDeviceContext )
			{
				// Is the device context of the OpenGL window is the currently active OpenGL device context?
				if ( wglGetCurrentDC() == mWindowDeviceContext )
				{
					wglMakeCurrent(nullptr, nullptr);
				}

				// Destroy the render context of the OpenGL window
				if ( SE_NULL_HANDLE != mWindowRenderContext && mOwnsRenderContext )
				{
					wglDeleteContext(mWindowRenderContext);
				}

				// Release the device context of the OpenGL window
				if ( SE_NULL_HANDLE != mNativeWindowHandle )
				{
					::ReleaseDC(reinterpret_cast<HWND>(mNativeWindowHandle), mWindowDeviceContext);
				}
			}

			// Destroy the OpenGL dummy window, in case there's one
			if ( SE_NULL_HANDLE != mDummyWindow )
			{
				// Destroy the OpenGL dummy window
				::DestroyWindow(reinterpret_cast<HWND>(mDummyWindow));

				// Unregister the window class for the OpenGL dummy window
				::UnregisterClass(TEXT("OpenGLDummyWindow"), ::GetModuleHandle(nullptr));
			}
		}

		/**
		*  @brief
		*    Return the primary device context
		*
		*  @return
		*    The primary device context, null pointer on error
		*/
		[[nodiscard]] inline HDC getDeviceContext() const
		{
			return mWindowDeviceContext;
		}

		/**
		*  @brief
		*    Return the primary render context
		*
		*  @return
		*    The primary render context, null pointer on error
		*/
		[[nodiscard]] inline HGLRC getRenderContext() const
		{
			return mWindowRenderContext;
		}


		//[-------------------------------------------------------]
		//[ Public virtual OpenGLRhi::IOpenGLContext methods      ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual bool isInitialized() const override
		{
			return (nullptr != mWindowRenderContext);
		}

		inline virtual void makeCurrent() const override
		{
			wglMakeCurrent(mWindowDeviceContext, mWindowRenderContext);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit OpenGLContextWindows(const OpenGLContextWindows&) = delete;
		OpenGLContextWindows& operator =(const OpenGLContextWindows&) = delete;

		/**
		*  @brief
		*    Constructor for primary context
		*
		*  @param[in] openGLRuntimeLinking
		*    OpenGL runtime linking instance, if null pointer this isn't a primary context
		*  @param[in] depthStencilAttachmentTextureFormat
		*    Depth stencil attachment texture format
		*  @param[in] nativeWindowHandle
		*    Optional native main window handle, can be a null handle
		*  @param[in] shareContextWindows
		*    Optional share context, can be a null pointer
		*/
		OpenGLContextWindows(OpenGLRuntimeLinking* openGLRuntimeLinking, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, handle nativeWindowHandle, const OpenGLContextWindows* shareContextWindows = nullptr) :
			IOpenGLContext(openGLRuntimeLinking),
			mNativeWindowHandle(nativeWindowHandle),
			mDummyWindow(SE_NULL_HANDLE),
			mWindowDeviceContext(SE_NULL_HANDLE),
			mWindowRenderContext(SE_NULL_HANDLE),
			mOwnsRenderContext(true)
		{
			// Create a OpenGL dummy window?
			// -> Under Microsoft Windows, a OpenGL context is always coupled to a window... even if we're not going to render into a window at all...
			if ( SE_NULL_HANDLE == mNativeWindowHandle )
			{
				// Setup and register the window class for the OpenGL dummy window
				WNDCLASS windowDummyClass;
				windowDummyClass.hInstance = ::GetModuleHandle(nullptr);
				windowDummyClass.lpszClassName = TEXT("OpenGLDummyWindow");
				windowDummyClass.lpfnWndProc = DefWindowProc;
				windowDummyClass.style = 0;
				windowDummyClass.hIcon = nullptr;
				windowDummyClass.hCursor = nullptr;
				windowDummyClass.lpszMenuName = nullptr;
				windowDummyClass.cbClsExtra = 0;
				windowDummyClass.cbWndExtra = 0;
				windowDummyClass.hbrBackground = nullptr;
				::RegisterClass(&windowDummyClass);

				// Create the OpenGL dummy window
				mNativeWindowHandle = mDummyWindow = reinterpret_cast<handle>(::CreateWindow(TEXT("OpenGLDummyWindow"), TEXT("PFormat"), WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 8, 8, HWND_DESKTOP, nullptr, ::GetModuleHandle(nullptr), nullptr));
			}

			// Is there a valid window handle?
			if ( SE_NULL_HANDLE != mNativeWindowHandle )
			{
				// Get the device context of the OpenGL window
				mWindowDeviceContext = ::GetDC(reinterpret_cast<HWND>(mNativeWindowHandle));
				if ( SE_NULL_HANDLE != mWindowDeviceContext )
				{
					// Get the color depth of the desktop
					int bits = 32;
					{
						HDC deskTopDC = ::GetDC(nullptr);
						bits = ::GetDeviceCaps(deskTopDC, BITSPIXEL);
						::ReleaseDC(nullptr, deskTopDC);
					}

					// Get the first best pixel format
					// TODO(co) Use more detailed color and depth/stencil information from render pass
					const BYTE depthBufferBits = (Rhi::TextureFormat::Enum::UNKNOWN == depthStencilAttachmentTextureFormat) ? 0u : 24u;
					const PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
					{
						sizeof(PIXELFORMATDESCRIPTOR),	// Size of this pixel format descriptor
						1,								// Version number
						PFD_DRAW_TO_WINDOW |			// Format must support window
						PFD_SUPPORT_OPENGL |			// Format must support OpenGL
						PFD_DOUBLEBUFFER,				// Must support double buffering
						PFD_TYPE_RGBA,					// Request an RGBA format
						static_cast<UCHAR>(bits),		// Select our color depth
						0, 0, 0, 0, 0, 0,				// Color bits ignored
						0,								// No alpha buffer
						0,								// Shift bit ignored
						0,								// No accumulation buffer
						0, 0, 0, 0,						// Accumulation bits ignored
						depthBufferBits,				// Z-buffer (depth buffer)
						0,								// No stencil buffer
						0,								// No auxiliary buffer
						PFD_MAIN_PLANE,					// Main drawing layer
						0,								// Reserved
						0, 0, 0							// Layer masks ignored
					};
					const int pixelFormat = ::ChoosePixelFormat(mWindowDeviceContext, &pixelFormatDescriptor);
					if ( 0 != pixelFormat )
					{
						// Set the pixel format
						::SetPixelFormat(mWindowDeviceContext, pixelFormat, &pixelFormatDescriptor);

						// Lookout! OpenGL context sharing chaos: https://www.opengl.org/wiki/OpenGL_Context
						// "State" objects are not shared between contexts, including but not limited to:
						// - Vertex Array Objects (VAOs)
						// - Framebuffer Objects (FBOs)
						// -> Keep away from "wglShareLists()" and the share context parameter of "wglCreateContextAttribsARB()" and just share the OpenGL render context instead
						if ( nullptr != shareContextWindows )
						{
							mWindowRenderContext = shareContextWindows->getRenderContext();
							mOwnsRenderContext = false;
						}
						else
						{
							// Create a legacy OpenGL render context
							HGLRC legacyRenderContext = wglCreateContext(mWindowDeviceContext);
							if ( SE_NULL_HANDLE != legacyRenderContext )
							{
								// Make the legacy OpenGL render context to the current one
								wglMakeCurrent(mWindowDeviceContext, legacyRenderContext);

								// Load the >= OpenGL 3.0 entry points
								if ( loadOpenGL3EntryPoints() )
								{
									// Create the render context of the OpenGL window
									mWindowRenderContext = createOpenGLContext(nullptr);

									// Destroy the legacy OpenGL render context
									wglMakeCurrent(nullptr, nullptr);
									wglDeleteContext(legacyRenderContext);

									// If there's an OpenGL context, do some final initialization steps
									if ( SE_NULL_HANDLE != mWindowRenderContext )
									{
										// Make the OpenGL context to the current one
										// TODO(co) Review this, might cause issues when creating a context while a program is running
										wglMakeCurrent(mWindowDeviceContext, mWindowRenderContext);
									}
								}
								else
								{
									// Error, failed to load >= OpenGL 3 entry points!
								}
							}
							else
							{
								// Error, failed to create a legacy OpenGL render context!
							}
						}
					}
					else
					{
						// Error, failed to choose a pixel format!
					}
				}
				else
				{
					// Error, failed to obtain the device context of the OpenGL window!
				}
			}
			else
			{
				// Error, failed to create the OpenGL window!
			}
		}

		/**
		*  @brief
		*    Create a OpenGL context
		*
		*  @param[in] shareContextWindows
		*    Optional share context, can be a null pointer
		*
		*  @return
		*    The created OpenGL context, null pointer on error
		*/
		[[nodiscard]] HGLRC createOpenGLContext(const OpenGLContextWindows* shareContextWindows)
		{
			// Disable the following warning, we can't do anything to resolve this warning
			SE_PRAGMA_WARNING_PUSH
				SE_PRAGMA_WARNING_DISABLE_MSVC(4191)	// warning C4191: 'reinterpret_cast' : unsafe conversion from 'PROC' to '<x>'

				// Get the OpenGL extension wglGetExtensionsStringARB function pointer, we need it to check for further supported OpenGL extensions
				PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARBLocal = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(wglGetProcAddress("wglGetExtensionsStringARB"));
			if ( nullptr != wglGetExtensionsStringARBLocal )
			{
				// Get the available WGL extensions as string
				const char* extensions = wglGetExtensionsStringARBLocal(mWindowDeviceContext);

				// Check whether or not "WGL_ARB_create_context" is a substring of the WGL extension string meaning that this OpenGL extension is supported
				if ( nullptr != strstr(extensions, "WGL_ARB_create_context") )
				{
					// Get the OpenGL extension "wglCreateContextAttribsARB" function pointer
					PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
					if ( nullptr != wglCreateContextAttribsARB )
					{
						// Create the OpenGL context
						// -> OpenGL 4.1 (the best OpenGL version Mac OS X 10.11 supports, so lowest version we have to support)
						static const int ATTRIBUTES[] =
						{
							// We want an OpenGL context
							WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
							WGL_CONTEXT_MINOR_VERSION_ARB, 1,
							WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
							#if SE_DEBUG
							// "WGL_CONTEXT_DEBUG_BIT_ARB" comes from the "GL_ARB_debug_output"-extension
							WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
						#else
							// "WGL_ARB_create_context_no_error" and "GL_KHR_no_error"-extension
							((nullptr != strstr(extensions, "WGL_ARB_create_context_no_error")) ? WGL_CONTEXT_OPENGL_NO_ERROR_ARB : 0), 1,
						#endif
							// Done
							0
						};

						// Lookout! OpenGL context sharing chaos: https://www.opengl.org/wiki/OpenGL_Context
						// "State" objects are not shared between contexts, including but not limited to:
						// - Vertex Array Objects (VAOs)
						// - Framebuffer Objects (FBOs)
						// -> Practically, this makes a second OpenGL context only useful for resource background loading
						const HGLRC hglrc = wglCreateContextAttribsARB(mWindowDeviceContext, (nullptr != shareContextWindows) ? shareContextWindows->getRenderContext() : nullptr, ATTRIBUTES);
						if ( nullptr != hglrc )
						{
							// Done
							return hglrc;
						}
						else
						{
							// Error, context creation failed!
							return SE_NULL_HANDLE;
						}
					}
					else
					{
						// Error, failed to obtain the "wglCreateContextAttribsARB" function pointer (wow, something went terrible wrong!)
						return SE_NULL_HANDLE;
					}
				}
				else
				{
					// Error, the OpenGL extension "WGL_ARB_create_context" is not supported... as a result we can't create an OpenGL context!
					return SE_NULL_HANDLE;
				}
			}
			else
			{
				// Error, failed to obtain the "wglGetExtensionsStringARB" function pointer (wow, something went terrible wrong!)
				return SE_NULL_HANDLE;
			}

			// Restore the previous warning configuration
			SE_PRAGMA_WARNING_POP
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		handle mNativeWindowHandle;	// OpenGL window, can be a null pointer (HWND)
		handle mDummyWindow;			// OpenGL dummy window, can be a null pointer (HWND)
		HDC			mWindowDeviceContext;	// The device context of the OpenGL dummy window, can be a null pointer
		HGLRC		mWindowRenderContext;	// The render context of the OpenGL dummy window, can be a null pointer
		bool		mOwnsRenderContext;		// Does this context owns the OpenGL render context?


	};
#elif LINUX
	//[-------------------------------------------------------]
	//[ OpenGLRhi/OpenGLContextLinux.h                        ]
	//[-------------------------------------------------------]
	// TODO(co) Cleanup
	static bool ctxErrorOccurred = false;
	[[nodiscard]] static int ctxErrorHandler(Display*, XErrorEvent*)
	{
		ctxErrorOccurred = true;
		return 0;
	}

	/**
	*  @brief
	*    Linux OpenGL context class
	*/
	class OpenGLContextLinux final : public IOpenGLContext
	{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class OpenGLRhi;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] depthStencilAttachmentTextureFormat
		*    Depth stencil attachment texture format
		*  @param[in] nativeWindowHandle
		*    Optional native main window handle, can be a null handle
		*  @param[in] useExternalContext
		*    When true an own OpenGL context won't be created and the context pointed by "shareContextLinux" is ignored
		*  @param[in] shareContextLinux
		*    Optional share context, can be a null pointer
		*/
		inline OpenGLContextLinux(OpenGLRhi& openGLRhi, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, handle nativeWindowHandle, bool useExternalContext, const OpenGLContextLinux* shareContextLinux = nullptr) :
			OpenGLContextLinux(openGLRhi, nullptr, depthStencilAttachmentTextureFormat, nativeWindowHandle, useExternalContext, shareContextLinux)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OpenGLContextLinux() override
		{
			// Release the device context of the OpenGL window
			if ( nullptr != mDisplay )
			{
				// Is the device context of the OpenGL window is the currently active OpenGL device context?
				if ( glXGetCurrentContext() == mWindowRenderContext )
				{
					glXMakeCurrent(mDisplay, None, nullptr);
				}

				// Destroy the render context of the OpenGL window
				if ( SE_NULL_HANDLE != mWindowRenderContext && mOwnsRenderContext )
				{
					glXDestroyContext(mDisplay, mWindowRenderContext);
				}

				if ( mOwnsX11Display )
				{
					XCloseDisplay(mDisplay);
				}
			}
		}

		/**
		*  @brief
		*    Return the primary device context
		*
		*  @return
		*    The primary device context, null pointer on error
		*/
		[[nodiscard]] inline Display* getDisplay() const
		{
			return mDisplay;
		}

		/**
		*  @brief
		*    Return the primary render context
		*
		*  @return
		*    The primary render context, null pointer on error
		*/
		[[nodiscard]] inline GLXContext getRenderContext() const
		{
			return mWindowRenderContext;
		}


		//[-------------------------------------------------------]
		//[ Public virtual OpenGLRhi::IOpenGLContext methods      ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual bool isInitialized() const override
		{
			return (nullptr != mWindowRenderContext || mUseExternalContext);
		}

		virtual void makeCurrent() const override
		{
			// Only do something when have created our RHI context and don't use a external RHI context
			if ( !mUseExternalContext )
			{
				glXMakeCurrent(getDisplay(), mNativeWindowHandle, getRenderContext());
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit OpenGLContextLinux(const OpenGLContextLinux&) = delete;
		OpenGLContextLinux& operator =(const OpenGLContextLinux&) = delete;

		/**
		*  @brief
		*    Constructor for primary context
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] openGLRuntimeLinking
		*    OpenGL runtime linking instance, if null pointer this isn't a primary context
		*  @param[in] depthStencilAttachmentTextureFormat
		*    Depth stencil attachment texture format
		*  @param[in] nativeWindowHandle
		*    Optional native main window handle, can be a null handle
		*  @param[in] useExternalContext
		*    When true an own OpenGL context won't be created and the context pointed by "shareContextLinux" is ignored
		*  @param[in] shareContextLinux
		*    Optional share context, can be a null pointer
		*/
		OpenGLContextLinux(OpenGLRhi& openGLRhi, OpenGLRuntimeLinking* openGLRuntimeLinking, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, handle nativeWindowHandle, bool useExternalContext, const OpenGLContextLinux* shareContextLinux = nullptr) :
			IOpenGLContext(openGLRuntimeLinking),
			mOpenGLRhi(openGLRhi),
			mNativeWindowHandle(nativeWindowHandle),
			mDisplay(nullptr),
			mOwnsX11Display(true),
			mWindowRenderContext(SE_NULL_HANDLE),
			mUseExternalContext(useExternalContext),
			mOwnsRenderContext(true)
		{
			if ( mUseExternalContext )
			{
				// We use an external context so just load the OpenGL 3 entry points
				[[maybe_unused]] const bool result = loadOpenGL3EntryPoints();
			}
			else
			{
				const Rhi::Context& context = openGLRhi.getContext();
				RHI_ASSERT(context.getType() == Rhi::Context::ContextType::X11, "Invalid OpenGL context type")

					// If the given RHI context is an X11 context use the display connection object provided by the context
					if ( context.getType() == Rhi::Context::ContextType::X11 )
					{
						mDisplay = static_cast<const Rhi::X11Context&>(context).getDisplay();
						mOwnsX11Display = mDisplay == nullptr;
					}

				if ( mOwnsX11Display )
				{
					mDisplay = XOpenDisplay(0);
				}
			}
			if ( nullptr != mDisplay )
			{
				// Lookout! OpenGL context sharing chaos: https://www.opengl.org/wiki/OpenGL_Context
				// "State" objects are not shared between contexts, including but not limited to:
				// - Vertex Array Objects (VAOs)
				// - Framebuffer Objects (FBOs)
				// -> Keep away from the share context parameter of "glxCreateContextAttribsARB()" and just share the OpenGL render context instead
				if ( nullptr != shareContextLinux )
				{
					mWindowRenderContext = shareContextLinux->getRenderContext();
					mOwnsRenderContext = false;
				}
				else
				{
					// TODO(sw) We don't need a dummy context to load gl/glx entry points see "Misconception #2" from https://dri.freedesktop.org/wiki/glXGetProcAddressNeverReturnsNULL/
					// Load the >= OpenGL 3.0 entry points
					if ( loadOpenGL3EntryPoints() )
					{
						// Create the render context of the OpenGL window
						mWindowRenderContext = createOpenGLContext(depthStencilAttachmentTextureFormat);

						// If there's an OpenGL context, do some final initialization steps
						if ( SE_NULL_HANDLE != mWindowRenderContext )
						{
							// Make the OpenGL context to the current one, native window handle can be zero -> thus only offscreen rendering is supported/wanted
							const int result = glXMakeCurrent(mDisplay, mNativeWindowHandle, mWindowRenderContext);
							RHI_LOG(DEBUG, "Make new OpenGL context current: %d", result)
							{
								int major = 0;
								glGetIntegerv(GL_MAJOR_VERSION, &major);

								int minor = 0;
								glGetIntegerv(GL_MINOR_VERSION, &minor);

								GLint profile = 0;
								glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

								RHI_LOG(DEBUG, "OpenGL context version: %d.%d %s", major, minor, ((profile & GL_CONTEXT_CORE_PROFILE_BIT) ? "core" : "noncore"))
									int numberOfExtensions = 0;
								glGetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);
								RHI_LOG(DEBUG, "Number of supported OpenGL extensions: %d", numberOfExtensions)
									for ( GLuint extensionIndex = 0; extensionIndex < static_cast<GLuint>(numberOfExtensions); ++extensionIndex )
									{
										RHI_LOG(DEBUG, "%s", glGetStringi(GL_EXTENSIONS, extensionIndex))
									}
							}
						}
					}
					else
					{
						// Error, failed to load >= OpenGL 3 entry points!
					}
				}
			}
			else
			{
				// Error, failed to get display!
			}
		}

		/**
		*  @brief
		*    Create a OpenGL context
		*
		*  @param[in] depthStencilAttachmentTextureFormat
		*    Depth stencil attachment texture format
		*
		*  @return
		*    The created OpenGL context, null pointer on error
		*/
		[[nodiscard]] GLXContext createOpenGLContext(Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat)
		{
#define GLX_CONTEXT_MAJOR_VERSION_ARB	0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB	0x2092

			// Get the available GLX extensions as string
			const char* extensions = glXQueryExtensionsString(mDisplay, XDefaultScreen(mDisplay));

			// Check whether or not "GLX_ARB_create_context" is a substring of the GLX extension string meaning that this OpenGL extension is supported
			if ( nullptr != strstr(extensions, "GLX_ARB_create_context") )
			{
				// Get the OpenGL extension "glXCreateContextAttribsARB" function pointer
				typedef GLXContext(*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
				GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = reinterpret_cast<GLXCREATECONTEXTATTRIBSARBPROC>(glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB"));
				if ( nullptr != glXCreateContextAttribsARB )
				{
					// TODO(co) Cleanup
					ctxErrorOccurred = false;
					int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

					// Create the OpenGL context
					// -> OpenGL 4.1 (the best OpenGL version Mac OS X 10.11 supports, so lowest version we have to support)
					// TODO(co) Add support for the "GL_KHR_no_error"-extension
					int ATTRIBUTES[] =
					{
						// We want an OpenGL context
						GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
						GLX_CONTEXT_MINOR_VERSION_ARB, 1,
						// -> "GLX_CONTEXT_DEBUG_BIT_ARB" comes from the "GL_ARB_debug_output"-extension
						GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
						// TODO(co) Make it possible to activate "GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB" from the outside
						#if SE_DEBUG
						//	GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
						//	GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,	// Error messages like "Implicit version number 110 not supported by GL3 forward compatible context" might occur
						#else
						//	GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,	// Error messages like "Implicit version number 110 not supported by GL3 forward compatible context" might occur
						#endif
						// Done
						0
					};

					// TODO(co) Use more detailed color and depth/stencil information from render pass
					const int depthBufferBits = 24;//(Rhi::TextureFormat::Enum::UNKNOWN == depthStencilAttachmentTextureFormat) ? 24 : 24;
					int numberOfElements = 0;
					int visualAttributes[] =
					{
						GLX_RENDER_TYPE,	GLX_RGBA_BIT,
						GLX_DOUBLEBUFFER,	true,
						GLX_RED_SIZE,		8,
						GLX_GREEN_SIZE,		8,
						GLX_BLUE_SIZE,		8,
						GLX_ALPHA_SIZE,		8,
						GLX_DEPTH_SIZE,		depthBufferBits,
						GLX_STENCIL_SIZE,	8,
						None
					};
					GLXFBConfig* fbc = glXChooseFBConfig(mDisplay, DefaultScreen(mDisplay), visualAttributes, &numberOfElements);
					RHI_LOG(DEBUG, "Got %d of OpenGL GLXFBConfig", numberOfElements)
						GLXContext glxContext = glXCreateContextAttribsARB(mDisplay, *fbc, 0, true, ATTRIBUTES);

					XSync(mDisplay, False);

					// TODO(sw) make this fallback optional (via an option)
					if ( ctxErrorOccurred )
					{
						RHI_LOG(DEBUG, "Could not create OpenGL 3+ context try creating pre 3+ context")
							ctxErrorOccurred = false;

						// GLX_CONTEXT_MAJOR_VERSION_ARB = 1
						ATTRIBUTES[1] = 1;
						// GLX_CONTEXT_MINOR_VERSION_ARB = 0
						ATTRIBUTES[3] = 0;
						glxContext = glXCreateContextAttribsARB(mDisplay, *fbc, 0, true, ATTRIBUTES);

						// Synchronize to ensure any errors generated are processed
						XSync(mDisplay, False);

						// Restore the original error handler
						XSetErrorHandler(oldHandler);
					}

					if ( nullptr != glxContext )
					{
						// Done
						RHI_LOG(DEBUG, "OpenGL context with glXCreateContextAttribsARB created")
							return glxContext;
					}
					else
					{
						// Error, context creation failed!
						RHI_LOG(CRITICAL, "Could not create OpenGL context with glXCreateContextAttribsARB")
							return SE_NULL_HANDLE;
					}
				}
				else
				{
					// Error, failed to obtain the "GLX_ARB_create_context" function pointer (wow, something went terrible wrong!)
					RHI_LOG(CRITICAL, "Could not find OpenGL glXCreateContextAttribsARB")
						return SE_NULL_HANDLE;
				}
			}
			else
			{
				// Error, the OpenGL extension "GLX_ARB_create_context" is not supported... as a result we can't create an OpenGL context!
				RHI_LOG(CRITICAL, "OpenGL GLX_ARB_create_context not supported")
					return SE_NULL_HANDLE;
			}

#undef GLX_CONTEXT_MAJOR_VERSION_ARB
#undef GLX_CONTEXT_MINOR_VERSION_ARB
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		OpenGLRhi&	mOpenGLRhi;				// Owner OpenGL RHI instance
		handle mNativeWindowHandle;	// OpenGL window, can be a null pointer (Window)
		Display*	mDisplay;				// The X11 display connection, can be a null pointer
		bool		mOwnsX11Display;		// Indicates if this instance owns the X11 display
		GLXContext	mWindowRenderContext;	// The render context of the OpenGL dummy window, can be a null pointer
		bool		mUseExternalContext;
		bool		mOwnsRenderContext;		// Does this context own the OpenGL render context?


	};
#else
#error "Unsupported platform"
#endif
} // namespace OpenGLRhi
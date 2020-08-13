#pragma once

namespace OpenGLRhi
{
	//[-------------------------------------------------------]
//[ OpenGLRhi/OpenGLRuntimeLinking.h                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL runtime linking
*/
	class OpenGLRuntimeLinking final
	{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class IOpenGLContext;


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
		*/
		inline explicit OpenGLRuntimeLinking(OpenGLRhi& openGLRhi) :
			mOpenGLRhi(openGLRhi),
			mOwnsOpenGLSharedLibrary(nullptr == mOpenGLSharedLibrary),	// We can do this here because "mOpenGLSharedLibrary" lays before this variable
			mEntryPointsRegistered(false),
			mInitialized(false)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OpenGLRuntimeLinking()
		{
			if ( mOwnsOpenGLSharedLibrary )
			{
				// Destroy the shared library instances
#if SE_PLATFORM_WINDOWS
				if ( nullptr != mOpenGLSharedLibrary )
				{
					::FreeLibrary(static_cast<HMODULE>(mOpenGLSharedLibrary));
				}
#elif LINUX
				if ( nullptr != mOpenGLSharedLibrary )
				{
					::dlclose(mOpenGLSharedLibrary);
				}
#else
#error "Unsupported platform"
#endif
			}
		}

		/**
		*  @brief
		*    Return whether or not OpenGL is available
		*
		*  @return
		*    "true" if OpenGL is available, else "false"
		*/
		[[nodiscard]] bool isOpenGLAvaiable()
		{
			// Already initialized?
			if ( !mInitialized )
			{
				// We're now initialized
				mInitialized = true;

				// Load the shared libraries
				if ( loadSharedLibraries() )
				{
					// Load the OpenGL entry points
					mEntryPointsRegistered = loadOpenGLEntryPoints();
				}
			}

			// Entry points successfully registered?
			return mEntryPointsRegistered;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit OpenGLRuntimeLinking(const OpenGLRuntimeLinking&) = delete;
		OpenGLRuntimeLinking& operator =(const OpenGLRuntimeLinking&) = delete;

		/**
		*  @brief
		*    Load the shared libraries
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] bool loadSharedLibraries()
		{
			if ( mOwnsOpenGLSharedLibrary )
			{
				// Load the shared library
#if SE_PLATFORM_WINDOWS
				mOpenGLSharedLibrary = ::LoadLibraryExA("opengl32.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
				if ( nullptr == mOpenGLSharedLibrary )
				{
					RHI_LOG(CRITICAL, "Failed to load in the shared OpenGL library \"opengl32.dll\"")
				}
#elif LINUX
				mOpenGLSharedLibrary = ::dlopen("libGL.so", RTLD_NOW | RTLD_GLOBAL);
				if ( nullptr == mOpenGLSharedLibrary )
				{
					RHI_LOG(CRITICAL, "Failed to load in the shared OpenGL library \"libGL.so\"")
				}
#else
#error "Unsupported platform"
#endif
			}

			// Done
			return (nullptr != mOpenGLSharedLibrary);
		}

		/**
		*  @brief
		*    Load the OpenGL entry points
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] bool loadOpenGLEntryPoints()
		{
			bool result = true;	// Success by default

			// Load the entry points
			IMPORT_FUNC(glGetString)
				IMPORT_FUNC(glGetIntegerv)
				IMPORT_FUNC(glBindTexture)
				IMPORT_FUNC(glClear)
				IMPORT_FUNC(glClearStencil)
				IMPORT_FUNC(glClearDepth)
				IMPORT_FUNC(glClearColor)
				IMPORT_FUNC(glDrawArrays)
				IMPORT_FUNC(glDrawElements)
				IMPORT_FUNC(glColor4f)
				IMPORT_FUNC(glEnable)
				IMPORT_FUNC(glDisable)
				IMPORT_FUNC(glBlendFunc)
				IMPORT_FUNC(glFrontFace)
				IMPORT_FUNC(glCullFace)
				IMPORT_FUNC(glPolygonMode)
				IMPORT_FUNC(glTexParameteri)
				IMPORT_FUNC(glGenTextures)
				IMPORT_FUNC(glDeleteTextures)
				IMPORT_FUNC(glTexImage1D)
				IMPORT_FUNC(glTexImage2D)
				IMPORT_FUNC(glPixelStorei)
				IMPORT_FUNC(glDepthFunc)
				IMPORT_FUNC(glDepthMask)
				IMPORT_FUNC(glViewport)
				IMPORT_FUNC(glDepthRange)
				IMPORT_FUNC(glScissor)
				IMPORT_FUNC(glFlush)
				IMPORT_FUNC(glFinish)
#if SE_PLATFORM_WINDOWS
				IMPORT_FUNC(wglGetCurrentDC)
				IMPORT_FUNC(wglGetProcAddress)
				IMPORT_FUNC(wglCreateContext)
				IMPORT_FUNC(wglDeleteContext)
				IMPORT_FUNC(wglMakeCurrent)
#elif LINUX
				IMPORT_FUNC(glXMakeCurrent)
				IMPORT_FUNC(glXGetProcAddress)
				IMPORT_FUNC(glXGetProcAddressARB)
				IMPORT_FUNC(glXChooseVisual)
				IMPORT_FUNC(glXCreateContext)
				IMPORT_FUNC(glXDestroyContext)
				IMPORT_FUNC(glXGetCurrentContext)
				IMPORT_FUNC(glXQueryExtensionsString)
				IMPORT_FUNC(glXChooseFBConfig)
				IMPORT_FUNC(glXSwapBuffers)
				IMPORT_FUNC(glXGetClientString)
#else
#error "Unsupported platform"
#endif

				// Done
				return result;
		}

		/**
		*  @brief
		*    Load the >= OpenGL 3.0 entry points
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - This method is only allowed to be called after an >= OpenGL context has been created and set
		*/
		[[nodiscard]] bool loadOpenGL3EntryPoints()
		{
			bool result = true;	// Success by default

			// Optional >= OpenGL 4.5: Load the entry points
			IMPORT_FUNC(glCreateQueries)

				// Mandatory >= OpenGL 3.0: Load the entry points
				result = true;	// Success by default
			IMPORT_FUNC(glGetStringi)

			// Done
			return result;
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		OpenGLRhi&	mOpenGLRhi;					// Owner OpenGL RHI instance
		void*		mOpenGLSharedLibrary = nullptr;		// OpenGL shared library, can be a null pointer
		bool		mOwnsOpenGLSharedLibrary;	// Indicates if the OpenGL shared library was loaded from ourself or provided from external
		bool		mEntryPointsRegistered;		// Entry points successfully registered?
		bool		mInitialized;				// Already initialized?


	};

	// Undefine the helper macro
#undef IMPORT_FUNC
} // namespace OpenGLRhi
#pragma once

namespace OpenGLRhi
{
	/**
	*  @brief
	*    Supported OpenGL graphic card extensions
	*
	*  @remarks
	*    You must check if the extension is supported by the current hardware before
	*    you use it. If the extension isn't available you should offer an alternative
	*    technique aka fallback.
	*
	*  @see
	*    - OpenGL extension registry at http://oss.sgi.com/projects/ogl-sample/registry/ for more information about
	*      the different extensions
	*/
	class Extensions final
	{


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
		*  @param[in] openGLContext
		*    Owner OpenGL context
		*/
		inline Extensions(OpenGLRhi& openGLRhi, IOpenGLContext& openGLContext) :
			mOpenGLRhi(openGLRhi),
			mOpenGLContext(&openGLContext),
			mInitialized(false)
		{
			// Reset extensions
			resetExtensions();
		}

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~Extensions()
		{
		}

		/**
		*  @brief
		*    Returns whether the extensions are initialized or not
		*
		*  @return
		*    "true" if the extension are initialized, else "false"
		*/
		[[nodiscard]] inline bool isInitialized() const
		{
			return mInitialized;
		}

		///////////////////////////////////////////////////////////
		// Returns whether an extension is supported or not
		///////////////////////////////////////////////////////////
		// WGL (Windows only)
		[[nodiscard]] inline bool isWGL_ARB_extensions_string() const
		{
			return mWGL_ARB_extensions_string;
		}

		[[nodiscard]] inline bool isWGL_EXT_swap_control() const
		{
			return mWGL_EXT_swap_control;
		}

		[[nodiscard]] inline bool isWGL_EXT_swap_control_tear() const
		{
			return mWGL_EXT_swap_control_tear;
		}

		// NV
		[[nodiscard]] inline bool isGL_NV_mesh_shader() const
		{
			return mGL_NV_mesh_shader;
		}

		// EXT
		[[nodiscard]] inline bool isGL_EXT_texture_lod_bias() const
		{
			return mGL_EXT_texture_lod_bias;
		}

		[[nodiscard]] inline bool isGL_EXT_texture_filter_anisotropic() const
		{
			return mGL_EXT_texture_filter_anisotropic;
		}

		[[nodiscard]] inline bool isGL_EXT_texture_array() const
		{
			return mGL_EXT_texture_array;
		}

		[[nodiscard]] inline bool isGL_EXT_texture3D() const
		{
			return mGL_EXT_texture3D;
		}

		[[nodiscard]] inline bool isGL_EXT_direct_state_access() const
		{
			return mGL_EXT_direct_state_access;
		}

		[[nodiscard]] inline bool isGL_EXT_shader_image_load_store() const
		{
			return mGL_EXT_shader_image_load_store;
		}

		// KHR
		[[nodiscard]] inline bool isGL_KHR_debug() const
		{
			return mGL_KHR_debug;
		}

		// ARB
		[[nodiscard]] inline bool isGL_ARB_framebuffer_object() const
		{
			return mGL_ARB_framebuffer_object;
		}

		[[nodiscard]] inline bool isGL_ARB_multitexture() const
		{
			return mGL_ARB_multitexture;
		}

		[[nodiscard]] inline bool isGL_ARB_texture_multisample() const
		{
			return mGL_ARB_texture_multisample;
		}

		[[nodiscard]] inline bool isGL_ARB_vertex_buffer_object() const
		{
			return mGL_ARB_vertex_buffer_object;
		}

		[[nodiscard]] inline bool isGL_ARB_texture_compression() const
		{
			return mGL_ARB_texture_compression;
		}

		[[nodiscard]] inline bool isGL_ARB_draw_buffers() const
		{
			return mGL_ARB_draw_buffers;
		}

		[[nodiscard]] inline bool isGL_ARB_shader_objects() const
		{
			return mGL_ARB_shader_objects;
		}

		[[nodiscard]] inline bool isGL_ARB_separate_shader_objects() const
		{
			return mGL_ARB_separate_shader_objects;
		}

		[[nodiscard]] inline bool isGL_ARB_get_program_binary() const
		{
			return mGL_ARB_get_program_binary;
		}

		[[nodiscard]] inline bool isGL_ARB_uniform_buffer_object() const
		{
			return mGL_ARB_uniform_buffer_object;
		}

		[[nodiscard]] inline bool isGL_ARB_texture_buffer_object() const
		{
			return mGL_ARB_texture_buffer_object;
		}

		[[nodiscard]] inline bool isGL_ARB_draw_indirect() const
		{
			return mGL_ARB_draw_indirect;
		}

		[[nodiscard]] inline bool isGL_ARB_multi_draw_indirect() const
		{
			return mGL_ARB_multi_draw_indirect;
		}

		[[nodiscard]] inline bool isGL_ARB_vertex_shader() const
		{
			return mGL_ARB_vertex_shader;
		}

		[[nodiscard]] inline bool isGL_ARB_vertex_program() const
		{
			return mGL_ARB_vertex_program;
		}

		[[nodiscard]] inline bool isGL_ARB_tessellation_shader() const
		{
			return mGL_ARB_tessellation_shader;
		}

		[[nodiscard]] inline bool isGL_ARB_geometry_shader4() const
		{
			return mGL_ARB_geometry_shader4;
		}

		[[nodiscard]] inline bool isGL_ARB_fragment_shader() const
		{
			return mGL_ARB_fragment_shader;
		}

		[[nodiscard]] inline bool isGL_ARB_fragment_program() const
		{
			return mGL_ARB_fragment_program;
		}

		[[nodiscard]] inline bool isGL_ARB_compute_shader() const
		{
			return mGL_ARB_compute_shader;
		}

		[[nodiscard]] inline bool isGL_ARB_draw_instanced() const
		{
			return mGL_ARB_draw_instanced;
		}

		[[nodiscard]] inline bool isGL_ARB_base_instance() const
		{
			return mGL_ARB_base_instance;
		}

		[[nodiscard]] inline bool isGL_ARB_instanced_arrays() const
		{
			return mGL_ARB_instanced_arrays;
		}

		[[nodiscard]] inline bool isGL_ARB_vertex_array_object() const
		{
			return mGL_ARB_vertex_array_object;
		}

		[[nodiscard]] inline bool isGL_ARB_sampler_objects() const
		{
			return mGL_ARB_sampler_objects;
		}

		[[nodiscard]] inline bool isGL_ARB_draw_elements_base_vertex() const
		{
			return mGL_ARB_draw_elements_base_vertex;
		}

		[[nodiscard]] inline bool isGL_ARB_debug_output() const
		{
			return mGL_ARB_debug_output;
		}

		[[nodiscard]] inline bool isGL_ARB_direct_state_access() const
		{
			return mGL_ARB_direct_state_access;
		}

		[[nodiscard]] inline bool isGL_ARB_texture_storage() const
		{
			return mGL_ARB_texture_storage;
		}

		[[nodiscard]] inline bool isGL_ARB_shader_storage_buffer_object() const
		{
			return mGL_ARB_shader_storage_buffer_object;
		}

		[[nodiscard]] inline bool isGL_ARB_copy_image() const
		{
			return mGL_ARB_copy_image;
		}

		[[nodiscard]] inline bool isGL_ARB_gl_spirv() const
		{
			return mGL_ARB_gl_spirv;
		}

		[[nodiscard]] inline bool isGL_ARB_clip_control() const
		{
			return mGL_ARB_clip_control;
		}

		[[nodiscard]] inline bool isGL_ARB_occlusion_query() const
		{
			return mGL_ARB_occlusion_query;
		}

		[[nodiscard]] inline bool isGL_ARB_pipeline_statistics_query() const
		{
			return mGL_ARB_pipeline_statistics_query;
		}

		[[nodiscard]] inline bool isGL_ARB_timer_query() const
		{
			return mGL_ARB_timer_query;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	public:
		explicit Extensions(const Extensions&) = delete;
		Extensions& operator =(const Extensions&) = delete;

		/**
		*  @brief
		*    Checks whether an extension is supported by the given hardware or not
		*
		*  @param[in] extension
		*    ASCII name of the extension, if a null pointer, nothing happens
		*
		*  @return
		*    "true" if the extensions is supported, else "false"
		*/
		[[nodiscard]] bool isSupported(const char* extension) const
		{
			// Check whether or not the given extension string pointer is valid
			if ( nullptr != extension )
			{
				// Is the extension supported by the hardware?
				if ( checkExtension(extension) )
				{
					// Extension is supported!
					return true;
				}
			}

			// Extension isn't supported!
			return false;
		}

		/**
		*  @brief
		*    Checks whether an extension is supported by the given hardware or not
		*
		*  @param[in] extension
		*    ASCII name of the extension, if a null pointer, nothing happens
		*
		*  @return
		*    "true" if the extensions is supported, else "false"
		*/
		[[nodiscard]] bool checkExtension(const char* extension) const
		{
			// Check whether or not the given extension string pointer is valid
			if ( nullptr != extension )
			{
				// Under Windows all available extensions can be received via one additional function
				// but under Linux there are two additional functions for this
#if SE_PLATFORM_WINDOWS
					// "glGetString()" & "wglGetExtensionsStringARB()"
				const int numberOfLoops = 2;
#elif APPLE
	// On Mac OS X, only "glGetString(GL_EXTENSIONS)" is required
				const int numberOfLoops = 1;
#elif LINUX
	// "glGetString()" & "glXQueryExtensionsString()" & "glXGetClientString()"
				const int numberOfLoops = 3;
#else
#error "Unsupported platform"
#endif
				const char* extensions = nullptr;
				for ( int loopIndex = 0; loopIndex < numberOfLoops; ++loopIndex )
				{
					// Extension names should not have spaces
					const char* where = strchr(extension, ' ');
					if ( nullptr != where || '\0' == *extension )
					{
						return false; // Extension not found
					}

					// Advanced extensions
					// TODO(sw) Move the query for advanced extensions (via platform specific methods) to the context?
					if ( loopIndex > 0 )
					{
#if SE_PLATFORM_WINDOWS
						// WGL extensions
						if ( !mWGL_ARB_extensions_string )
						{
							// Extension not found
							return false;
						}
						extensions = static_cast<const char*>(wglGetExtensionsStringARB(wglGetCurrentDC()));
#elif APPLE
						// On Mac OS X, only "glGetString(GL_EXTENSIONS)" is required
#elif LINUX
						// Get the X server display connection
						Display* display = static_cast<OpenGLContextLinux&>(*mOpenGLContext).getDisplay();
						if ( nullptr != display )
						{
							if ( 2 == loopIndex )
							{
								extensions = static_cast<const char*>(glXQueryExtensionsString(display, XDefaultScreen(display)));
							}
							else
							{
								extensions = static_cast<const char*>(glXGetClientString(display, GLX_EXTENSIONS));
							}
						}
#else
#error "Unsupported platform"
#endif

						// Normal extensions
					}
					else
					{
						extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
						if ( nullptr == extensions )
						{
							// "glGetString(GL_EXTENSIONS)" is not available in core profiles, we have to use "glGetStringi()" 
							int numberOfExtensions = 0;
							glGetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);
							for ( GLuint extensionIndex = 0; extensionIndex < static_cast<GLuint>(numberOfExtensions); ++extensionIndex )
							{
								if ( 0 == strcmp(extension, reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, extensionIndex))) )
								{
									// Extension found
									return true;
								}
							}
						}
					}
					if ( nullptr != extensions )
					{
						// It takes a bit of care to be fool-proof about parsing the
						// OpenGL extensions string. Don't be fooled by substrings,
						// etc:
						const char* start = extensions;
						where = strstr(start, extension);
						while ( nullptr != where )
						{
							const char* terminator = where + strlen(extension);
							if ( (where == start || ' ' == *(where - 1)) && (' ' == *terminator || '\0' == *terminator) )
							{
								// Extension found
								return true;
							}
							start = terminator;
							where = strstr(start, extension);
						}
					}
				}
			}

			// Extension not found
			return false;
		}

		/**
		*  @brief
		*    Resets the extensions
		*/
		void resetExtensions()
		{
			mInitialized = false;

			// Extensions
			// WGL (Windows only)
			mWGL_ARB_extensions_string = false;
			mWGL_EXT_swap_control = false;
			mWGL_EXT_swap_control_tear = false;
			// EXT
			mGL_NV_mesh_shader = false;
			// EXT
			mGL_EXT_texture_lod_bias = false;
			mGL_EXT_texture_filter_anisotropic = false;
			mGL_EXT_texture_array = false;
			mGL_EXT_texture3D = false;
			mGL_EXT_direct_state_access = false;
			mGL_EXT_shader_image_load_store = false;
			// KHR
			mGL_KHR_debug = false;
			// ARB
			mGL_ARB_framebuffer_object = false;
			mGL_ARB_multitexture = false;
			mGL_ARB_texture_multisample = false;
			mGL_ARB_vertex_buffer_object = false;
			mGL_ARB_texture_compression = false;
			mGL_ARB_draw_buffers = false;
			mGL_ARB_shader_objects = false;
			mGL_ARB_separate_shader_objects = false;
			mGL_ARB_get_program_binary = false;
			mGL_ARB_uniform_buffer_object = false;
			mGL_ARB_texture_buffer_object = false;
			mGL_ARB_draw_indirect = false;
			mGL_ARB_multi_draw_indirect = false;
			mGL_ARB_vertex_shader = false;
			mGL_ARB_vertex_program = false;
			mGL_ARB_tessellation_shader = false;
			mGL_ARB_geometry_shader4 = false;
			mGL_ARB_fragment_shader = false;
			mGL_ARB_fragment_program = false;
			mGL_ARB_compute_shader = false;
			mGL_ARB_draw_instanced = false;
			mGL_ARB_base_instance = false;
			mGL_ARB_instanced_arrays = false;
			mGL_ARB_vertex_array_object = false;
			mGL_ARB_sampler_objects = false;
			mGL_ARB_draw_elements_base_vertex = false;
			mGL_ARB_debug_output = false;
			mGL_ARB_direct_state_access = false;
			mGL_ARB_texture_storage = false;
			mGL_ARB_shader_storage_buffer_object = false;
			mGL_ARB_copy_image = false;
			mGL_ARB_gl_spirv = false;
			mGL_ARB_clip_control = false;
			mGL_ARB_occlusion_query = false;
			mGL_ARB_pipeline_statistics_query = false;
			mGL_ARB_timer_query = false;
		}

		/**
		*  @brief
		*    Initialize the supported extensions
		*
		*  @param[in] useExtensions
		*    Use extensions?
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Platform dependent implementation
		*/
#if SE_PLATFORM_WINDOWS
		[[nodiscard]] bool initialize(bool useExtensions = true)
		{
			// Disable the following warning, we can't do anything to resolve this warning
			SE_PRAGMA_WARNING_PUSH
				SE_PRAGMA_WARNING_DISABLE_MSVC(4191)	// warning C4191: 'reinterpret_cast' : unsafe conversion from 'PROC' to '<x>'

				// Should the extensions be used?
				if ( useExtensions )
				{
					mInitialized = true;
				}
				else
				{
					resetExtensions();
					mInitialized = true;

					// Done
					return true;
				}


			//[-------------------------------------------------------]
			//[ WGL (Windows only) definitions                        ]
			//[-------------------------------------------------------]
			// WGL_ARB_extensions_string
			wglGetExtensionsStringARB = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(wglGetProcAddress("wglGetExtensionsStringARB"));
			mWGL_ARB_extensions_string = (nullptr != wglGetExtensionsStringARB);

			// WGL_EXT_swap_control
			mWGL_EXT_swap_control = isSupported("WGL_EXT_swap_control");
			if ( mWGL_EXT_swap_control )
			{
				wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
				mWGL_EXT_swap_control = (nullptr != wglGetExtensionsStringARB);
			}

			// WGL_EXT_swap_control_tear
			mWGL_EXT_swap_control_tear = isSupported("WGL_EXT_swap_control_tear");


			// Restore the previous warning configuration
			SE_PRAGMA_WARNING_POP

				// Initialize the supported universal extensions
				return initializeUniversal();
		}
#elif LINUX
		[[nodiscard]] bool initialize(bool useExtensions)
		{
			// Disable the following warning, we can't do anything to resolve this warning
			// Should the extensions be used?
			if ( useExtensions )
			{
				mInitialized = true;
			}
			else
			{
				resetExtensions();
				mInitialized = true;

				// Done
				return true;
			}

			// Initialize the supported universal extensions
			return initializeUniversal();
		}
#else
#error "Unsupported platform"
#endif

		/**
		*  @brief
		*    Initialize the supported universal extensions
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Platform independent implementation
		*/
		[[nodiscard]] bool initializeUniversal()
		{
			// Define a platform dependent helper macro
#if SE_PLATFORM_WINDOWS
#define IMPORT_FUNC(funcName)																													\
					if (result)																																	\
					{																																			\
						void* symbol = wglGetProcAddress(#funcName);																							\
						if (nullptr != symbol)																													\
						{																																		\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																					\
						}																																		\
						else																																	\
						{																																		\
							RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the OpenGL shared library", #funcName)	\
							result = false;																														\
						}																																		\
					}
#elif APPLE
	// For OpenGL extension handling, Apple provides several documents like
	// - "Technical Note TN2080 Understanding and Detecting OpenGL Functionality" (http://developer.apple.com/library/mac/#technotes/tn2080/_index.html)
	// - "Cross-Development Programming Guide" (http://www.filibeto.org/unix/macos/lib/dev/documentation/DeveloperTools/Conceptual/cross_development/cross_development.pdf)
	// -> All referencing to "QA1188: GetProcAdress and OpenGL Entry Points" (http://developer.apple.com/qa/qa2001/qa1188.html).
	//    Sadly, it appears that this site no longer exists.
	// -> It appears that for Mac OS X v10.6 >, the "dlopen"-way is recommended.
#define IMPORT_FUNC(funcName)																													\
					if (result)																																	\
					{																																			\
						void* symbol = m_pOpenGLSharedLibrary ? dlsym(mOpenGLSharedLibrary, #funcName) : nullptr;												\
						if (nullptr != symbol)																													\
						{																																		\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																					\
						}																																		\
						else																																	\
						{																																		\
							RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the OpenGL shared library", #funcName)	\
							result = false;																														\
						}																																		\
					}
#elif LINUX
			typedef void (*GLfunction)();
#define IMPORT_FUNC(funcName)																													\
					if (result)																																	\
					{																																			\
						GLfunction symbol = glXGetProcAddressARB(reinterpret_cast<const GLubyte*>(#funcName));													\
						if (nullptr != symbol)																													\
						{																																		\
							*(reinterpret_cast<GLfunction*>(&(funcName))) = symbol;																				\
						}																																		\
						else																																	\
						{																																		\
							RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the OpenGL shared library", #funcName)	\
							result = false;																														\
						}																																		\
					}
#else
#error "Unsupported platform"
#endif

// One thing about OpenGL versions and extensions: In case we're using a certain OpenGL core profile, graphics driver implementations might
// decide to not list OpenGL extensions which are a part of this OpenGL core profile. Such a behavior was first noted using Linux Mesa 3D.
// When not taking this into account, horrible things will happen.
			GLint profile = 0;
			glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
			const bool isCoreProfile = (profile & GL_CONTEXT_CORE_PROFILE_BIT);


			//[-------------------------------------------------------]
			//[ NV                                                    ]
			//[-------------------------------------------------------]
			// GL_NV_mesh_shader
			mGL_NV_mesh_shader = isSupported("GL_NV_mesh_shader");
			if ( mGL_NV_mesh_shader )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDrawMeshTasksNV)
					mGL_NV_mesh_shader = result;
			}


			//[-------------------------------------------------------]
			//[ EXT                                                   ]
			//[-------------------------------------------------------]
			// GL_EXT_texture_lod_bias - Is core since OpenGL 1.4
			mGL_EXT_texture_lod_bias = isCoreProfile ? true : isSupported("GL_EXT_texture_lod_bias");

			// GL_EXT_texture_filter_anisotropic
			mGL_EXT_texture_filter_anisotropic = isSupported("GL_EXT_texture_filter_anisotropic");

			// GL_EXT_texture_array - Is core since OpenGL 3.0
			mGL_EXT_texture_array = isCoreProfile ? true : isSupported("GL_EXT_texture_array");

			// GL_EXT_texture3D - Is core since OpenGL 1.2
			mGL_EXT_texture3D = isCoreProfile ? true : isSupported("GL_EXT_texture3D");
			if ( mGL_EXT_texture3D )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glTexImage3DEXT)
					IMPORT_FUNC(glTexSubImage3DEXT)
					mGL_EXT_texture3D = result;
			}

			// GL_EXT_direct_state_access - Is core since OpenGL 2.1
			mGL_EXT_direct_state_access = isCoreProfile ? true : isSupported("GL_EXT_direct_state_access");
			if ( mGL_EXT_direct_state_access )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glNamedBufferDataEXT)
					IMPORT_FUNC(glNamedBufferSubDataEXT)
					IMPORT_FUNC(glMapNamedBufferEXT)
					IMPORT_FUNC(glUnmapNamedBufferEXT)
					IMPORT_FUNC(glProgramUniform1iEXT)
					IMPORT_FUNC(glProgramUniform1uiEXT)
					IMPORT_FUNC(glProgramUniform1fEXT)
					IMPORT_FUNC(glProgramUniform2fvEXT)
					IMPORT_FUNC(glProgramUniform3fvEXT)
					IMPORT_FUNC(glProgramUniform4fvEXT)
					IMPORT_FUNC(glProgramUniformMatrix3fvEXT)
					IMPORT_FUNC(glProgramUniformMatrix4fvEXT)
					IMPORT_FUNC(glTextureImage1DEXT)
					IMPORT_FUNC(glTextureImage2DEXT)
					IMPORT_FUNC(glTextureImage3DEXT)
					IMPORT_FUNC(glTextureSubImage3DEXT)
					IMPORT_FUNC(glTextureParameteriEXT)
					IMPORT_FUNC(glGenerateTextureMipmapEXT)
					IMPORT_FUNC(glCompressedTextureImage1DEXT)
					IMPORT_FUNC(glCompressedTextureImage2DEXT)
					IMPORT_FUNC(glCompressedTextureImage3DEXT)
					IMPORT_FUNC(glVertexArrayVertexAttribOffsetEXT)
					IMPORT_FUNC(glEnableVertexArrayAttribEXT)
					IMPORT_FUNC(glBindMultiTextureEXT)
					IMPORT_FUNC(glNamedFramebufferTexture2DEXT)
					IMPORT_FUNC(glNamedFramebufferTextureLayerEXT)
					IMPORT_FUNC(glCheckNamedFramebufferStatusEXT)
					IMPORT_FUNC(glNamedRenderbufferStorageEXT)
					IMPORT_FUNC(glNamedFramebufferRenderbufferEXT)
					mGL_EXT_direct_state_access = result;
			}

			// GL_EXT_shader_image_load_store
			mGL_EXT_shader_image_load_store = isSupported("GL_EXT_shader_image_load_store");
			if ( mGL_EXT_shader_image_load_store )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glBindImageTextureEXT)
					IMPORT_FUNC(glMemoryBarrierEXT)
					mGL_EXT_shader_image_load_store = result;
			}


			//[-------------------------------------------------------]
			//[ KHR                                                   ]
			//[-------------------------------------------------------]
			// GL_KHR_debug
			mGL_KHR_debug = isSupported("GL_KHR_debug");
			if ( mGL_KHR_debug )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDebugMessageInsert)
					IMPORT_FUNC(glPushDebugGroup)
					IMPORT_FUNC(glPopDebugGroup)
					IMPORT_FUNC(glObjectLabel)
					mGL_KHR_debug = result;
			}


			//[-------------------------------------------------------]
			//[ ARB                                                   ]
			//[-------------------------------------------------------]
			// GL_ARB_framebuffer_object
			mGL_ARB_framebuffer_object = isSupported("GL_ARB_framebuffer_object");
			if ( mGL_ARB_framebuffer_object )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glBindRenderbuffer)
					IMPORT_FUNC(glDeleteRenderbuffers)
					IMPORT_FUNC(glGenRenderbuffers)
					IMPORT_FUNC(glRenderbufferStorage)
					IMPORT_FUNC(glBindFramebuffer)
					IMPORT_FUNC(glDeleteFramebuffers)
					IMPORT_FUNC(glGenFramebuffers)
					IMPORT_FUNC(glCheckFramebufferStatus)
					IMPORT_FUNC(glFramebufferTexture2D)
					IMPORT_FUNC(glFramebufferTextureLayer)
					IMPORT_FUNC(glFramebufferRenderbuffer)
					IMPORT_FUNC(glBlitFramebuffer)
					IMPORT_FUNC(glGenerateMipmap)
					mGL_ARB_framebuffer_object = result;
			}

			// GL_ARB_multitexture - Is core feature since OpenGL 1.3
			mGL_ARB_multitexture = isCoreProfile ? true : isSupported("GL_ARB_multitexture");
			if ( mGL_ARB_multitexture )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glActiveTextureARB)
					mGL_ARB_multitexture = result;
			}

			// GL_ARB_texture_multisample - Is core feature since OpenGL 3.2
			mGL_ARB_texture_multisample = isCoreProfile ? true : isSupported("GL_ARB_texture_multisample");
			if ( mGL_ARB_texture_multisample )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glTexImage2DMultisample)
					mGL_ARB_texture_multisample = result;
			}

			// GL_ARB_vertex_buffer_object - Is core feature since OpenGL 1.5
			mGL_ARB_vertex_buffer_object = isCoreProfile ? true : isSupported("GL_ARB_vertex_buffer_object");
			if ( mGL_ARB_vertex_buffer_object )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glBindBufferARB)
					IMPORT_FUNC(glDeleteBuffersARB)
					IMPORT_FUNC(glGenBuffersARB)
					IMPORT_FUNC(glBufferDataARB)
					IMPORT_FUNC(glBufferSubDataARB)
					IMPORT_FUNC(glMapBufferARB)
					IMPORT_FUNC(glUnmapBufferARB)
					mGL_ARB_vertex_buffer_object = result;
			}

			// GL_ARB_texture_compression - Is core since OpenGL 1.3
			mGL_ARB_texture_compression = isCoreProfile ? true : isSupported("GL_ARB_texture_compression");
			if ( mGL_ARB_texture_compression )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glCompressedTexImage1DARB)
					IMPORT_FUNC(glCompressedTexImage2DARB)
					IMPORT_FUNC(glCompressedTexImage3DARB)
					mGL_ARB_texture_compression = result;
			}

			// GL_ARB_draw_buffers - Is core since OpenGL 2.0
			mGL_ARB_draw_buffers = isCoreProfile ? true : isSupported("GL_ARB_draw_buffers");
			if ( mGL_ARB_draw_buffers )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDrawBuffersARB)
					mGL_ARB_draw_buffers = result;
			}

			// GL_ARB_shader_objects - Is core since OpenGL 2.0
			mGL_ARB_shader_objects = isCoreProfile ? true : isSupported("GL_ARB_shader_objects");
			if ( mGL_ARB_shader_objects )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDeleteShader)
					IMPORT_FUNC(glGetHandleARB)
					IMPORT_FUNC(glDetachShader)
					IMPORT_FUNC(glCreateShader)
					IMPORT_FUNC(glShaderSource)
					IMPORT_FUNC(glCompileShader)
					IMPORT_FUNC(glCreateProgram)
					IMPORT_FUNC(glAttachShader)
					IMPORT_FUNC(glLinkProgram)
					IMPORT_FUNC(glUseProgram)
					IMPORT_FUNC(glUniform1f)
					IMPORT_FUNC(glUniform1i)
					IMPORT_FUNC(glUniform2fv)
					IMPORT_FUNC(glUniform3fv)
					IMPORT_FUNC(glUniform4fv)
					IMPORT_FUNC(glUniformMatrix3fv)
					IMPORT_FUNC(glUniformMatrix4fv)
					IMPORT_FUNC(glGetShaderiv)
					IMPORT_FUNC(glGetProgramiv)
					IMPORT_FUNC(glGetShaderInfoLog)
					IMPORT_FUNC(glGetProgramInfoLog)
					IMPORT_FUNC(glGetUniformLocation)
					mGL_ARB_shader_objects = result;
			}

			// GL_ARB_separate_shader_objects - Is core since OpenGL 4.1
			mGL_ARB_separate_shader_objects = isCoreProfile ? true : isSupported("GL_ARB_separate_shader_objects");
			if ( mGL_ARB_separate_shader_objects )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glCreateShaderProgramv)
					IMPORT_FUNC(glDeleteProgram)
					IMPORT_FUNC(glGenProgramPipelines)
					IMPORT_FUNC(glDeleteProgramPipelines)
					IMPORT_FUNC(glBindProgramPipeline)
					IMPORT_FUNC(glUseProgramStages)
					IMPORT_FUNC(glValidateProgramPipeline)
					IMPORT_FUNC(glGetProgramPipelineiv)
					IMPORT_FUNC(glGetProgramPipelineInfoLog)
					IMPORT_FUNC(glActiveShaderProgram)
					mGL_ARB_separate_shader_objects = result;
			}

			// GL_ARB_get_program_binary - Is core since OpenGL 4.1
			mGL_ARB_get_program_binary = isCoreProfile ? true : isSupported("GL_ARB_get_program_binary");
			if ( mGL_ARB_get_program_binary )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glProgramParameteri)
					mGL_ARB_get_program_binary = result;
			}

			// GL_ARB_uniform_buffer_object - Is core since OpenGL 3.1
			mGL_ARB_uniform_buffer_object = isCoreProfile ? true : isSupported("GL_ARB_uniform_buffer_object");
			if ( mGL_ARB_uniform_buffer_object )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glGetUniformBlockIndex)
					IMPORT_FUNC(glUniformBlockBinding)
					IMPORT_FUNC(glBindBufferBase)
					mGL_ARB_uniform_buffer_object = result;
			}

			// GL_ARB_texture_buffer_object - Is core since OpenGL 3.1
			mGL_ARB_texture_buffer_object = isCoreProfile ? true : isSupported("GL_ARB_texture_buffer_object");
			if ( mGL_ARB_texture_buffer_object )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glTexBufferARB)
					mGL_ARB_texture_buffer_object = result;
			}

			// GL_ARB_draw_indirect - Is core since OpenGL 4.0
			mGL_ARB_draw_indirect = isCoreProfile ? true : isSupported("GL_ARB_draw_indirect");
			if ( mGL_ARB_draw_indirect )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDrawArraysIndirect)
					IMPORT_FUNC(glDrawElementsIndirect)
					mGL_ARB_draw_indirect = result;
			}

			// GL_ARB_multi_draw_indirect - Is core since OpenGL 4.3
			mGL_ARB_multi_draw_indirect = isSupported("GL_ARB_multi_draw_indirect");
			if ( mGL_ARB_multi_draw_indirect )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glMultiDrawArraysIndirect)
					IMPORT_FUNC(glMultiDrawElementsIndirect)
					mGL_ARB_multi_draw_indirect = result;
			}

			// GL_ARB_vertex_shader - Is core since OpenGL 2.0
			mGL_ARB_vertex_shader = isCoreProfile ? true : isSupported("GL_ARB_vertex_shader");
			if ( mGL_ARB_vertex_shader )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glBindAttribLocation)
					mGL_ARB_vertex_shader = result;
			}

			// GL_ARB_vertex_program
			mGL_ARB_vertex_program = isCoreProfile ? true : isSupported("GL_ARB_vertex_program");
			if ( mGL_ARB_vertex_program )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glVertexAttribPointerARB)
					IMPORT_FUNC(glVertexAttribIPointer)	// GL_NV_vertex_program4
					IMPORT_FUNC(glEnableVertexAttribArrayARB)
					IMPORT_FUNC(glDisableVertexAttribArrayARB)
					mGL_ARB_vertex_program = result;
			}

			// GL_ARB_tessellation_shader - Is core since OpenGL 4.0
			mGL_ARB_tessellation_shader = isCoreProfile ? true : isSupported("GL_ARB_tessellation_shader");
			if ( mGL_ARB_tessellation_shader )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glPatchParameteri)
					mGL_ARB_tessellation_shader = result;
			}

			// GL_ARB_geometry_shader4
			// TODO(sw) This extension was promoted to core feature but heavily modified source: https://www.khronos.org/opengl/wiki/History_of_OpenGL#OpenGL_3.2_.282009.29
			// TODO(sw) But this extension doesn't show up with mesa 3D either with an old OpenGL context (max OpenGL 3.3) or with an profile context (with OpenGL 4.3)
			mGL_ARB_geometry_shader4 = isSupported("GL_ARB_geometry_shader4");
			if ( mGL_ARB_geometry_shader4 )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glProgramParameteriARB)
					mGL_ARB_geometry_shader4 = result;
			}

			// GL_ARB_fragment_shader - Is core since OpenGL 2.0
			mGL_ARB_fragment_shader = isCoreProfile ? true : isSupported("GL_ARB_fragment_shader");

			// GL_ARB_fragment_program (we do not need any of the functions this extension provides)
			mGL_ARB_fragment_program = isCoreProfile ? true : isSupported("GL_ARB_fragment_program");

			// GL_ARB_compute_shader - Is core since OpenGL 4.3
			mGL_ARB_compute_shader = isSupported("GL_ARB_compute_shader");
			if ( mGL_ARB_compute_shader )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDispatchCompute)
					mGL_ARB_compute_shader = result;
			}

			// GL_ARB_draw_instanced - Is core since OpenGL 3.1
			mGL_ARB_draw_instanced = isCoreProfile ? true : isSupported("GL_ARB_draw_instanced");
			if ( mGL_ARB_draw_instanced )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDrawArraysInstancedARB)
					IMPORT_FUNC(glDrawElementsInstancedARB)
					mGL_ARB_draw_instanced = result;
			}

			// GL_ARB_base_instance - Is core since OpenGL 4.3
			mGL_ARB_base_instance = isSupported("GL_ARB_base_instance");
			if ( mGL_ARB_base_instance )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDrawArraysInstancedBaseInstance)
					IMPORT_FUNC(glDrawElementsInstancedBaseInstance)
					IMPORT_FUNC(glDrawElementsInstancedBaseVertexBaseInstance)
					mGL_ARB_base_instance = result;
			}

			// GL_ARB_instanced_arrays - Is core since OpenGL 3.3
			mGL_ARB_instanced_arrays = isCoreProfile ? true : isSupported("GL_ARB_instanced_arrays");
			if ( mGL_ARB_instanced_arrays )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glVertexAttribDivisorARB)
					mGL_ARB_instanced_arrays = result;
			}

			// GL_ARB_vertex_array_object
			mGL_ARB_vertex_array_object = isSupported("GL_ARB_vertex_array_object");
			if ( mGL_ARB_vertex_array_object )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glBindVertexArray)
					IMPORT_FUNC(glDeleteVertexArrays)
					IMPORT_FUNC(glGenVertexArrays)
					mGL_ARB_vertex_array_object = result;
			}

			// GL_ARB_sampler_objects - Is core since OpenGL 3.3
			mGL_ARB_sampler_objects = isCoreProfile ? true : isSupported("GL_ARB_sampler_objects");
			if ( mGL_ARB_sampler_objects )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glGenSamplers)
					IMPORT_FUNC(glDeleteSamplers)
					IMPORT_FUNC(glBindSampler)
					IMPORT_FUNC(glSamplerParameteri)
					IMPORT_FUNC(glSamplerParameterf)
					IMPORT_FUNC(glSamplerParameterfv)
					mGL_ARB_sampler_objects = result;
			}

			// GL_ARB_draw_elements_base_vertex - Is core since OpenGL 3.2
			mGL_ARB_draw_elements_base_vertex = isCoreProfile ? true : isSupported("GL_ARB_draw_elements_base_vertex");
			if ( mGL_ARB_draw_elements_base_vertex )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDrawElementsBaseVertex)
					IMPORT_FUNC(glDrawElementsInstancedBaseVertex)
					mGL_ARB_draw_elements_base_vertex = result;
			}

			// GL_ARB_debug_output - Is core since OpenGL 4.3
			mGL_ARB_debug_output = isSupported("GL_ARB_debug_output");
			if ( mGL_ARB_debug_output )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glDebugMessageCallbackARB)
					IMPORT_FUNC(glDebugMessageControlARB)
					mGL_ARB_debug_output = result;
			}

			// GL_ARB_direct_state_access - Is core since OpenGL 4.5
			mGL_ARB_direct_state_access = isSupported("GL_ARB_direct_state_access");
			if ( mGL_ARB_direct_state_access )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glCreateBuffers)
					IMPORT_FUNC(glCreateFramebuffers)
					IMPORT_FUNC(glCreateTextures)
					IMPORT_FUNC(glCreateVertexArrays)
					IMPORT_FUNC(glNamedBufferData)
					IMPORT_FUNC(glNamedBufferSubData)
					IMPORT_FUNC(glMapNamedBuffer)
					IMPORT_FUNC(glUnmapNamedBuffer)
					IMPORT_FUNC(glProgramUniform1i)
					IMPORT_FUNC(glProgramUniform1ui)
					IMPORT_FUNC(glProgramUniform1f)
					IMPORT_FUNC(glProgramUniform2fv)
					IMPORT_FUNC(glProgramUniform3fv)
					IMPORT_FUNC(glProgramUniform4fv)
					IMPORT_FUNC(glProgramUniformMatrix3fv)
					IMPORT_FUNC(glProgramUniformMatrix4fv)
					IMPORT_FUNC(glTextureParameteri)
					IMPORT_FUNC(glGenerateTextureMipmap)
					IMPORT_FUNC(glEnableVertexArrayAttrib)
					IMPORT_FUNC(glCheckNamedFramebufferStatus)
					IMPORT_FUNC(glNamedRenderbufferStorage)
					IMPORT_FUNC(glNamedFramebufferRenderbuffer)
					IMPORT_FUNC(glNamedFramebufferTexture)
					IMPORT_FUNC(glNamedFramebufferTextureLayer)
					IMPORT_FUNC(glTextureBuffer)
					IMPORT_FUNC(glBindTextureUnit)
					IMPORT_FUNC(glCompressedTextureSubImage1D)
					IMPORT_FUNC(glCompressedTextureSubImage2D)
					IMPORT_FUNC(glCompressedTextureSubImage3D)
					IMPORT_FUNC(glTextureSubImage1D)
					IMPORT_FUNC(glTextureSubImage2D)
					IMPORT_FUNC(glTextureSubImage3D)
					IMPORT_FUNC(glVertexArrayAttribFormat)
					IMPORT_FUNC(glVertexArrayAttribIFormat)
					IMPORT_FUNC(glVertexArrayAttribBinding)
					IMPORT_FUNC(glVertexArrayVertexBuffer)
					IMPORT_FUNC(glVertexArrayBindingDivisor)
					IMPORT_FUNC(glVertexArrayElementBuffer)
					mGL_ARB_direct_state_access = result;
			}

			// GL_ARB_texture_storage - Is core since OpenGL 4.5
			mGL_ARB_texture_storage = isSupported("GL_ARB_texture_storage");
			if ( mGL_ARB_texture_storage )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glTextureStorage1D)
					IMPORT_FUNC(glTextureStorage2D)
					IMPORT_FUNC(glTextureStorage3D)
					IMPORT_FUNC(glTextureStorage2DMultisample)
					mGL_ARB_texture_storage = result;
			}

			// GL_ARB_shader_storage_buffer_object - Is core since OpenGL 4.3
			mGL_ARB_shader_storage_buffer_object = isSupported("GL_ARB_shader_storage_buffer_object");

			// GL_ARB_copy_image - Is core since OpenGL 4.3
			mGL_ARB_copy_image = isSupported("GL_ARB_copy_image");
			if ( mGL_ARB_copy_image )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glCopyImageSubData)
					mGL_ARB_copy_image = result;
			}

			// GL_ARB_gl_spirv
			mGL_ARB_gl_spirv = isSupported("GL_ARB_gl_spirv");
			if ( mGL_ARB_gl_spirv )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glSpecializeShaderARB)
					mGL_ARB_gl_spirv = result;

				// TODO(co) "GL_ARB_gl_spirv"-support is under construction
				mGL_ARB_gl_spirv = false;
			}

			// GL_ARB_clip_control
			mGL_ARB_clip_control = isSupported("GL_ARB_clip_control");
			if ( mGL_ARB_clip_control )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glClipControl)
					mGL_ARB_clip_control = result;
			}

			// GL_ARB_occlusion_query - Is core since OpenGL 1.5
			mGL_ARB_occlusion_query = isCoreProfile ? true : isSupported("GL_ARB_occlusion_query");
			if ( mGL_ARB_occlusion_query )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glGenQueriesARB)
					IMPORT_FUNC(glDeleteQueriesARB)
					IMPORT_FUNC(glBeginQueryARB)
					IMPORT_FUNC(glEndQueryARB)
					IMPORT_FUNC(glGetQueryObjectuivARB)
					mGL_ARB_occlusion_query = result;
			}

			// GL_ARB_pipeline_statistics_query
			mGL_ARB_pipeline_statistics_query = isSupported("GL_ARB_pipeline_statistics_query");

			// GL_ARB_timer_query - Is core since OpenGL 3.3
			mGL_ARB_timer_query = isCoreProfile ? true : isSupported("GL_ARB_timer_query");
			if ( mGL_ARB_timer_query )
			{
				// Load the entry points
				bool result = true;	// Success by default
				IMPORT_FUNC(glQueryCounter)
					mGL_ARB_timer_query = result;
			}


			//[-------------------------------------------------------]
			//[ Core (OpenGL version dependent)                       ]
			//[-------------------------------------------------------]
			{
				[[maybe_unused]] bool result = true;	// Success by default
				IMPORT_FUNC(glShaderBinary)	// OpenGL 4.1
			}


			// Undefine the helper macro
#undef IMPORT_FUNC

// Done
			return true;
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		OpenGLRhi&		mOpenGLRhi;		// Owner OpenGL RHI instance
		IOpenGLContext* mOpenGLContext;	// Owner OpenGL context, always valid!
		bool			mInitialized;	// Are the extensions initialized?

		// Supported extensions
		// WGL (Windows only)
		bool mWGL_ARB_extensions_string;
		bool mWGL_EXT_swap_control;
		bool mWGL_EXT_swap_control_tear;
		// EXT
		bool mGL_NV_mesh_shader;
		// EXT
		bool mGL_EXT_texture_lod_bias;
		bool mGL_EXT_texture_filter_anisotropic;
		bool mGL_EXT_texture_array;
		bool mGL_EXT_texture3D;
		bool mGL_EXT_direct_state_access;
		bool mGL_EXT_shader_image_load_store;
		// KHR
		bool mGL_KHR_debug;
		// ARB
		bool mGL_ARB_framebuffer_object;
		bool mGL_ARB_multitexture;
		bool mGL_ARB_texture_multisample;
		bool mGL_ARB_vertex_buffer_object;
		bool mGL_ARB_texture_compression;
		bool mGL_ARB_draw_buffers;
		bool mGL_ARB_shader_objects;
		bool mGL_ARB_separate_shader_objects;
		bool mGL_ARB_get_program_binary;
		bool mGL_ARB_uniform_buffer_object;
		bool mGL_ARB_texture_buffer_object;
		bool mGL_ARB_draw_indirect;
		bool mGL_ARB_multi_draw_indirect;
		bool mGL_ARB_vertex_shader;
		bool mGL_ARB_vertex_program;
		bool mGL_ARB_tessellation_shader;
		bool mGL_ARB_geometry_shader4;
		bool mGL_ARB_fragment_shader;
		bool mGL_ARB_fragment_program;
		bool mGL_ARB_compute_shader;
		bool mGL_ARB_draw_instanced;
		bool mGL_ARB_base_instance;
		bool mGL_ARB_instanced_arrays;
		bool mGL_ARB_vertex_array_object;
		bool mGL_ARB_sampler_objects;
		bool mGL_ARB_draw_elements_base_vertex;
		bool mGL_ARB_debug_output;
		bool mGL_ARB_direct_state_access;
		bool mGL_ARB_texture_storage;
		bool mGL_ARB_shader_storage_buffer_object;
		bool mGL_ARB_copy_image;
		bool mGL_ARB_gl_spirv;
		bool mGL_ARB_clip_control;
		bool mGL_ARB_occlusion_query;
		bool mGL_ARB_pipeline_statistics_query;
		bool mGL_ARB_timer_query;
	};
} // namespace OpenGLRhi
#pragma once

//[-------------------------------------------------------]
//[ OpenGL functions                                      ]
//[-------------------------------------------------------]
#define FNDEF_GL(retType, funcName, args) retType (GLAPIENTRY *funcPtr_##funcName) args
FNDEF_GL(GLubyte*, glGetString, (GLenum));
FNDEF_GL(void, glGetIntegerv, (GLenum, GLint*));
FNDEF_GL(void, glBindTexture, (GLenum, GLuint));
FNDEF_GL(void, glClear, (GLbitfield));
FNDEF_GL(void, glClearStencil, (GLint));
FNDEF_GL(void, glClearDepth, (GLclampd));
FNDEF_GL(void, glClearColor, (GLclampf, GLclampf, GLclampf, GLclampf));
FNDEF_GL(void, glDrawArrays, (GLenum, GLint, GLsizei));
FNDEF_GL(void, glDrawElements, (GLenum, GLsizei, GLenum, const GLvoid*));
FNDEF_GL(void, glColor4f, (GLfloat, GLfloat, GLfloat, GLfloat));
FNDEF_GL(void, glEnable, (GLenum));
FNDEF_GL(void, glDisable, (GLenum));
FNDEF_GL(void, glBlendFunc, (GLenum, GLenum));
FNDEF_GL(void, glFrontFace, (GLenum));
FNDEF_GL(void, glCullFace, (GLenum));
FNDEF_GL(void, glPolygonMode, (GLenum, GLenum));
FNDEF_GL(void, glTexParameteri, (GLenum, GLenum, GLint));
FNDEF_GL(void, glGenTextures, (GLsizei, GLuint*));
FNDEF_GL(void, glDeleteTextures, (GLsizei, const GLuint*));
FNDEF_GL(void, glTexImage1D, (GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid*));
FNDEF_GL(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*));
FNDEF_GL(void, glPixelStorei, (GLenum, GLint));
FNDEF_GL(void, glDepthFunc, (GLenum func));
FNDEF_GL(void, glDepthMask, (GLboolean));
FNDEF_GL(void, glViewport, (GLint, GLint, GLsizei, GLsizei));
FNDEF_GL(void, glDepthRange, (GLclampd, GLclampd));
FNDEF_GL(void, glScissor, (GLint, GLint, GLsizei, GLsizei));
FNDEF_GL(void, glFlush, (void));
FNDEF_GL(void, glFinish, (void));

// >= OpenGL 3.0
FNDEF_GL(GLubyte *, glGetStringi, (GLenum, GLuint));

// >= OpenGL 4.5
FNDEF_GL(void, glCreateQueries, (GLenum, GLsizei, GLuint*));

// Platform specific
#if SE_PLATFORM_WINDOWS
FNDEF_GL(HDC, wglGetCurrentDC, (VOID));
FNDEF_GL(PROC, wglGetProcAddress, (LPCSTR));
FNDEF_GL(HGLRC, wglCreateContext, (HDC));
FNDEF_GL(BOOL, wglDeleteContext, (HGLRC));
FNDEF_GL(BOOL, wglMakeCurrent, (HDC, HGLRC));
#elif LINUX
FNDEF_GL(Bool, glXMakeCurrent, (Display*, GLXDrawable, GLXContext));
FNDEF_GL(XVisualInfo*, glXChooseVisual, (Display*, int, int*));
FNDEF_GL(GLXContext, glXCreateContext, (Display*, XVisualInfo*, GLXContext, int));
FNDEF_GL(void, glXDestroyContext, (Display*, GLXContext));
FNDEF_GL(GLXContext, glXGetCurrentContext, (void));
FNDEF_GL(const char*, glXQueryExtensionsString, (Display*, int));
FNDEF_GL(__GLXextFuncPtr, glXGetProcAddress, (const GLubyte*));
FNDEF_GL(__GLXextFuncPtr, glXGetProcAddressARB, (const GLubyte*));
FNDEF_GL(GLXFBConfig*, glXChooseFBConfig, (Display*, int, const int*, int*));
FNDEF_GL(void, glXSwapBuffers, (Display*, GLXDrawable));
FNDEF_GL(const char*, glXGetClientString, (Display*, int));
#else
#error "Unsupported platform"
#endif

// Redirect gl* function calls to funcPtr_gl*
#ifndef FNPTR
#define FNPTR(name) funcPtr_##name
#endif

// OpenGL
#define glGetString			FNPTR(glGetString)
#define glGetIntegerv		FNPTR(glGetIntegerv)
#define glBindTexture		FNPTR(glBindTexture)
#define glClear				FNPTR(glClear)
#define glClearStencil		FNPTR(glClearStencil)
#define glClearDepth		FNPTR(glClearDepth)
#define glClearColor		FNPTR(glClearColor)
#define glDrawArrays		FNPTR(glDrawArrays)
#define glDrawElements		FNPTR(glDrawElements)
#define glColor4f			FNPTR(glColor4f)
#define glEnable			FNPTR(glEnable)
#define glDisable			FNPTR(glDisable)
#define glBlendFunc			FNPTR(glBlendFunc)
#define glFrontFace			FNPTR(glFrontFace)
#define glCullFace			FNPTR(glCullFace)
#define glPolygonMode		FNPTR(glPolygonMode)
#define glTexParameteri		FNPTR(glTexParameteri)
#define glGenTextures		FNPTR(glGenTextures)
#define glDeleteTextures	FNPTR(glDeleteTextures)
#define glTexImage1D		FNPTR(glTexImage1D)
#define glTexImage2D		FNPTR(glTexImage2D)
#define glPixelStorei		FNPTR(glPixelStorei)
#define glDepthFunc			FNPTR(glDepthFunc)
#define glDepthMask			FNPTR(glDepthMask)
#define glViewport			FNPTR(glViewport)
#define glDepthRange		FNPTR(glDepthRange)
#define glScissor			FNPTR(glScissor)
#define glFlush				FNPTR(glFlush)
#define glFinish			FNPTR(glFinish)

// >= OpenGL 3.0
#define glGetStringi FNPTR(glGetStringi)

// >= OpenGL 4.5
#define glCreateQueries FNPTR(glCreateQueries)

// Platform specific
#if SE_PLATFORM_WINDOWS
#define wglGetCurrentDC		FNPTR(wglGetCurrentDC)
#define wglGetProcAddress	FNPTR(wglGetProcAddress)
#define wglCreateContext	FNPTR(wglCreateContext)
#define wglDeleteContext	FNPTR(wglDeleteContext)
#define wglMakeCurrent		FNPTR(wglMakeCurrent)
#elif LINUX
#define glXMakeCurrent				FNPTR(glXMakeCurrent)
#define glXGetProcAddress			FNPTR(glXGetProcAddress)
#define glXGetProcAddressARB		FNPTR(glXGetProcAddressARB)
#define glXChooseVisual				FNPTR(glXChooseVisual)
#define glXCreateContext			FNPTR(glXCreateContext)
#define glXDestroyContext			FNPTR(glXDestroyContext)
#define glXGetCurrentContext		FNPTR(glXGetCurrentContext)
#define glXQueryExtensionsString	FNPTR(glXQueryExtensionsString)
#define glXChooseFBConfig			FNPTR(glXChooseFBConfig)
#define glXSwapBuffers				FNPTR(glXSwapBuffers)
#define glXGetClientString			FNPTR(glXGetClientString)
#else
#error "Unsupported platform"
#endif

// Define a helper macro
#if SE_PLATFORM_WINDOWS
#define IMPORT_FUNC(funcName)																																			\
		if (result)																																							\
		{																																									\
			void* symbol = ::GetProcAddress(static_cast<HMODULE>(mOpenGLSharedLibrary), #funcName);																			\
			if (nullptr == symbol)																																			\
			{																																								\
				symbol = wglGetProcAddress(#funcName);																														\
			}																																								\
			if (nullptr != symbol)																																			\
			{																																								\
				*(reinterpret_cast<void**>(&(funcName))) = symbol;																											\
			}																																								\
			else																																							\
			{																																								\
				wchar_t moduleFilename[MAX_PATH];																															\
				moduleFilename[0] = '\0';																																	\
				::GetModuleFileNameW(static_cast<HMODULE>(mOpenGLSharedLibrary), moduleFilename, MAX_PATH);																	\
				RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the OpenGL shared library \"%s\"", #funcName, moduleFilename)	\
				result = false;																																				\
			}																																								\
		}
#elif LINUX
#define IMPORT_FUNC(funcName)																																		\
		if (result)																																						\
		{																																								\
			void* symbol = ::dlsym(mOpenGLSharedLibrary, #funcName);																									\
			if (nullptr != symbol)																																		\
			{																																							\
				*(reinterpret_cast<void**>(&(funcName))) = symbol;																										\
			}																																							\
			else																																						\
			{																																							\
				link_map *linkMap = nullptr;																															\
				const char* libraryName = "unknown";																													\
				if (dlinfo(mOpenGLSharedLibrary, RTLD_DI_LINKMAP, &linkMap))																							\
				{																																						\
					libraryName = linkMap->l_name;																														\
				}																																						\
				RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the OpenGL shared library \"%s\"", #funcName, libraryName)	\
				result = false;																																			\
			}																																							\
		}
#else
#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ OpenGL extension functions                            ]
//[-------------------------------------------------------]
#define FNDEF_EX(funcName, funcTypedef) funcTypedef funcName = nullptr

//[-------------------------------------------------------]
//[ WGL (Windows only) definitions                        ]
//[-------------------------------------------------------]
#if SE_PLATFORM_WINDOWS
	// WGL_ARB_extensions_string
FNDEF_EX(wglGetExtensionsStringARB, PFNWGLGETEXTENSIONSSTRINGARBPROC);

// WGL_EXT_swap_control
FNDEF_EX(wglSwapIntervalEXT, PFNWGLSWAPINTERVALEXTPROC);
#endif

//[-------------------------------------------------------]
//[ NV                                                    ]
//[-------------------------------------------------------]
// GL_NV_mesh_shader
FNDEF_EX(glDrawMeshTasksNV, PFNGLDRAWMESHTASKSNVPROC);

//[-------------------------------------------------------]
//[ EXT                                                   ]
//[-------------------------------------------------------]
// GL_EXT_texture3D
FNDEF_EX(glTexImage3DEXT, PFNGLTEXIMAGE3DEXTPROC);
FNDEF_EX(glTexSubImage3DEXT, PFNGLTEXSUBIMAGE3DEXTPROC);

// GL_EXT_direct_state_access
FNDEF_EX(glNamedBufferDataEXT, PFNGLNAMEDBUFFERDATAEXTPROC);
FNDEF_EX(glNamedBufferSubDataEXT, PFNGLNAMEDBUFFERSUBDATAEXTPROC);
FNDEF_EX(glMapNamedBufferEXT, PFNGLMAPNAMEDBUFFEREXTPROC);
FNDEF_EX(glUnmapNamedBufferEXT, PFNGLUNMAPNAMEDBUFFEREXTPROC);
FNDEF_EX(glProgramUniform1iEXT, PFNGLPROGRAMUNIFORM1IEXTPROC);
FNDEF_EX(glProgramUniform1uiEXT, PFNGLPROGRAMUNIFORM1UIEXTPROC);
FNDEF_EX(glProgramUniform1fEXT, PFNGLPROGRAMUNIFORM1FEXTPROC);
FNDEF_EX(glProgramUniform2fvEXT, PFNGLPROGRAMUNIFORM2FVEXTPROC);
FNDEF_EX(glProgramUniform3fvEXT, PFNGLPROGRAMUNIFORM3FVEXTPROC);
FNDEF_EX(glProgramUniform4fvEXT, PFNGLPROGRAMUNIFORM4FVEXTPROC);
FNDEF_EX(glProgramUniformMatrix3fvEXT, PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC);
FNDEF_EX(glProgramUniformMatrix4fvEXT, PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC);
FNDEF_EX(glTextureImage1DEXT, PFNGLTEXTUREIMAGE1DEXTPROC);
FNDEF_EX(glTextureImage2DEXT, PFNGLTEXTUREIMAGE2DEXTPROC);
FNDEF_EX(glTextureImage3DEXT, PFNGLTEXTUREIMAGE3DEXTPROC);
FNDEF_EX(glTextureSubImage3DEXT, PFNGLTEXTURESUBIMAGE3DEXTPROC);
FNDEF_EX(glTextureParameteriEXT, PFNGLTEXTUREPARAMETERIEXTPROC);
FNDEF_EX(glGenerateTextureMipmapEXT, PFNGLGENERATETEXTUREMIPMAPEXTPROC);
FNDEF_EX(glCompressedTextureImage1DEXT, PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC);
FNDEF_EX(glCompressedTextureImage2DEXT, PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC);
FNDEF_EX(glCompressedTextureImage3DEXT, PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC);
FNDEF_EX(glVertexArrayVertexAttribOffsetEXT, PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC);
FNDEF_EX(glEnableVertexArrayAttribEXT, PFNGLENABLEVERTEXARRAYATTRIBEXTPROC);
FNDEF_EX(glBindMultiTextureEXT, PFNGLBINDMULTITEXTUREEXTPROC);
FNDEF_EX(glNamedFramebufferTexture2DEXT, PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC);
FNDEF_EX(glNamedFramebufferTextureLayerEXT, PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC);
FNDEF_EX(glCheckNamedFramebufferStatusEXT, PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC);
FNDEF_EX(glNamedRenderbufferStorageEXT, PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC);
FNDEF_EX(glNamedFramebufferRenderbufferEXT, PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC);

// GL_EXT_shader_image_load_store
FNDEF_EX(glBindImageTextureEXT, PFNGLBINDIMAGETEXTUREEXTPROC);
FNDEF_EX(glMemoryBarrierEXT, PFNGLMEMORYBARRIEREXTPROC);

//[-------------------------------------------------------]
//[ KHR                                                   ]
//[-------------------------------------------------------]
// GL_KHR_debug
FNDEF_EX(glDebugMessageInsert, PFNGLDEBUGMESSAGEINSERTPROC);
FNDEF_EX(glPushDebugGroup, PFNGLPUSHDEBUGGROUPPROC);
FNDEF_EX(glPopDebugGroup, PFNGLPOPDEBUGGROUPPROC);
FNDEF_EX(glObjectLabel, PFNGLOBJECTLABELPROC);

//[-------------------------------------------------------]
//[ ARB                                                   ]
//[-------------------------------------------------------]
// GL_ARB_framebuffer_object
FNDEF_EX(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC);
FNDEF_EX(glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC);
FNDEF_EX(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC);
FNDEF_EX(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC);
FNDEF_EX(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC);
FNDEF_EX(glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC);
FNDEF_EX(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC);
FNDEF_EX(glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC);
FNDEF_EX(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DPROC);
FNDEF_EX(glFramebufferTextureLayer, PFNGLFRAMEBUFFERTEXTURELAYERPROC);
FNDEF_EX(glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC);
FNDEF_EX(glBlitFramebuffer, PFNGLBLITFRAMEBUFFERPROC);
FNDEF_EX(glGenerateMipmap, PFNGLGENERATEMIPMAPPROC);

// GL_ARB_multitexture
FNDEF_EX(glActiveTextureARB, PFNGLACTIVETEXTUREARBPROC);

// GL_ARB_texture_multisample
FNDEF_EX(glTexImage2DMultisample, PFNGLTEXIMAGE2DMULTISAMPLEPROC);

// GL_ARB_vertex_buffer_object
FNDEF_EX(glBindBufferARB, PFNGLBINDBUFFERARBPROC);
FNDEF_EX(glDeleteBuffersARB, PFNGLDELETEBUFFERSARBPROC);
FNDEF_EX(glGenBuffersARB, PFNGLGENBUFFERSARBPROC);
FNDEF_EX(glBufferDataARB, PFNGLBUFFERDATAARBPROC);
FNDEF_EX(glBufferSubDataARB, PFNGLBUFFERSUBDATAARBPROC);
FNDEF_EX(glMapBufferARB, PFNGLMAPBUFFERARBPROC);
FNDEF_EX(glUnmapBufferARB, PFNGLUNMAPBUFFERARBPROC);

// GL_ARB_texture_compression
FNDEF_EX(glCompressedTexImage1DARB, PFNGLCOMPRESSEDTEXIMAGE1DARBPROC);
FNDEF_EX(glCompressedTexImage2DARB, PFNGLCOMPRESSEDTEXIMAGE2DARBPROC);
FNDEF_EX(glCompressedTexImage3DARB, PFNGLCOMPRESSEDTEXIMAGE3DARBPROC);

// GL_ARB_vertex_program
FNDEF_EX(glVertexAttribPointerARB, PFNGLVERTEXATTRIBPOINTERARBPROC);
FNDEF_EX(glVertexAttribIPointer, PFNGLVERTEXATTRIBIPOINTERPROC);	// GL_NV_vertex_program4
FNDEF_EX(glEnableVertexAttribArrayARB, PFNGLENABLEVERTEXATTRIBARRAYARBPROC);
FNDEF_EX(glDisableVertexAttribArrayARB, PFNGLDISABLEVERTEXATTRIBARRAYARBPROC);

// GL_ARB_draw_buffers
FNDEF_EX(glDrawBuffersARB, PFNGLDRAWBUFFERSARBPROC);

// GL_ARB_shader_objects
FNDEF_EX(glDeleteShader, PFNGLDELETESHADERPROC);			// glDeleteObjectARB
FNDEF_EX(glGetHandleARB, PFNGLGETHANDLEARBPROC);
FNDEF_EX(glDetachShader, PFNGLDETACHSHADERPROC);			// glDetachObjectARB
FNDEF_EX(glCreateShader, PFNGLCREATESHADERPROC);			// glCreateShaderObjectARB
FNDEF_EX(glShaderSource, PFNGLSHADERSOURCEPROC);			// glShaderSourceARB
FNDEF_EX(glCompileShader, PFNGLCOMPILESHADERPROC);		// glCompileShaderARB
FNDEF_EX(glCreateProgram, PFNGLCREATEPROGRAMPROC);		// glCreateProgramObjectARB
FNDEF_EX(glAttachShader, PFNGLATTACHSHADERPROC);			// glAttachObjectARB
FNDEF_EX(glLinkProgram, PFNGLLINKPROGRAMPROC);			// glLinkProgramARB
FNDEF_EX(glUseProgram, PFNGLUSEPROGRAMPROC);			// glUseProgramObjectARB
FNDEF_EX(glUniform1f, PFNGLUNIFORM1FPROC);			// glUniform1fARB
FNDEF_EX(glUniform1i, PFNGLUNIFORM1IPROC);			// glUniform1iARB
FNDEF_EX(glUniform2fv, PFNGLUNIFORM2FVPROC);			// glUniform2fvARB
FNDEF_EX(glUniform3fv, PFNGLUNIFORM3FVPROC);			// glUniform3fvARB
FNDEF_EX(glUniform4fv, PFNGLUNIFORM4FVPROC);			// glUniform4fvARB
FNDEF_EX(glUniformMatrix3fv, PFNGLUNIFORMMATRIX3FVPROC);		// glUniformMatrix3fvARB
FNDEF_EX(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC);		// glUniformMatrix4fvARB
FNDEF_EX(glGetShaderiv, PFNGLGETSHADERIVPROC);			// glGetObjectParameterivARB
FNDEF_EX(glGetProgramiv, PFNGLGETPROGRAMIVPROC);			// glGetObjectParameterivARB
FNDEF_EX(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC);		// glGetInfoLogARB
FNDEF_EX(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC);	// glGetInfoLogARB
FNDEF_EX(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC);	// glGetUniformLocationARB

// GL_ARB_separate_shader_objects
FNDEF_EX(glCreateShaderProgramv, PFNGLCREATESHADERPROGRAMVPROC);
FNDEF_EX(glDeleteProgram, PFNGLDELETEPROGRAMPROC);
FNDEF_EX(glGenProgramPipelines, PFNGLGENPROGRAMPIPELINESPROC);
FNDEF_EX(glDeleteProgramPipelines, PFNGLDELETEPROGRAMPIPELINESPROC);
FNDEF_EX(glBindProgramPipeline, PFNGLBINDPROGRAMPIPELINEPROC);
FNDEF_EX(glUseProgramStages, PFNGLUSEPROGRAMSTAGESPROC);
FNDEF_EX(glValidateProgramPipeline, PFNGLVALIDATEPROGRAMPIPELINEPROC);
FNDEF_EX(glGetProgramPipelineiv, PFNGLGETPROGRAMPIPELINEIVPROC);
FNDEF_EX(glGetProgramPipelineInfoLog, PFNGLGETPROGRAMPIPELINEINFOLOGPROC);
FNDEF_EX(glActiveShaderProgram, PFNGLACTIVESHADERPROGRAMPROC);

// GL_ARB_get_program_binary
FNDEF_EX(glProgramParameteri, PFNGLPROGRAMPARAMETERIPROC);

// GL_ARB_uniform_buffer_object
FNDEF_EX(glGetUniformBlockIndex, PFNGLGETUNIFORMBLOCKINDEXPROC);
FNDEF_EX(glUniformBlockBinding, PFNGLUNIFORMBLOCKBINDINGPROC);
FNDEF_EX(glBindBufferBase, PFNGLBINDBUFFERBASEPROC);

// GL_ARB_texture_buffer_object
FNDEF_EX(glTexBufferARB, PFNGLTEXBUFFERARBPROC);

// GL_ARB_draw_indirect
FNDEF_EX(glDrawArraysIndirect, PFNGLDRAWARRAYSINDIRECTPROC);
FNDEF_EX(glDrawElementsIndirect, PFNGLDRAWELEMENTSINDIRECTPROC);

// GL_ARB_multi_draw_indirect
FNDEF_EX(glMultiDrawArraysIndirect, PFNGLMULTIDRAWARRAYSINDIRECTPROC);
FNDEF_EX(glMultiDrawElementsIndirect, PFNGLMULTIDRAWELEMENTSINDIRECTPROC);

// GL_ARB_vertex_shader
FNDEF_EX(glBindAttribLocation, PFNGLBINDATTRIBLOCATIONPROC);	// glBindAttribLocationARB

// GL_ARB_tessellation_shader
FNDEF_EX(glPatchParameteri, PFNGLPATCHPARAMETERIPROC);

// GL_ARB_geometry_shader4
FNDEF_EX(glProgramParameteriARB, PFNGLPROGRAMPARAMETERIARBPROC);

// GL_ARB_compute_shader
FNDEF_EX(glDispatchCompute, PFNGLDISPATCHCOMPUTEPROC);

// GL_ARB_draw_instanced
FNDEF_EX(glDrawArraysInstancedARB, PFNGLDRAWARRAYSINSTANCEDARBPROC);
FNDEF_EX(glDrawElementsInstancedARB, PFNGLDRAWELEMENTSINSTANCEDARBPROC);

// GL_ARB_base_instance
FNDEF_EX(glDrawArraysInstancedBaseInstance, PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC);
FNDEF_EX(glDrawElementsInstancedBaseInstance, PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC);
FNDEF_EX(glDrawElementsInstancedBaseVertexBaseInstance, PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC);

// GL_ARB_instanced_arrays
FNDEF_EX(glVertexAttribDivisorARB, PFNGLVERTEXATTRIBDIVISORARBPROC);

// GL_ARB_vertex_array_object
FNDEF_EX(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC);
FNDEF_EX(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC);
FNDEF_EX(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC);

// GL_ARB_sampler_objects
FNDEF_EX(glGenSamplers, PFNGLGENSAMPLERSPROC);
FNDEF_EX(glDeleteSamplers, PFNGLDELETESAMPLERSPROC);
FNDEF_EX(glBindSampler, PFNGLBINDSAMPLERPROC);
FNDEF_EX(glSamplerParameteri, PFNGLSAMPLERPARAMETERIPROC);
FNDEF_EX(glSamplerParameterf, PFNGLSAMPLERPARAMETERFPROC);
FNDEF_EX(glSamplerParameterfv, PFNGLSAMPLERPARAMETERFVPROC);

// GL_ARB_draw_elements_base_vertex
FNDEF_EX(glDrawElementsBaseVertex, PFNGLDRAWELEMENTSBASEVERTEXPROC);
FNDEF_EX(glDrawElementsInstancedBaseVertex, PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC);

// GL_ARB_debug_output
FNDEF_EX(glDebugMessageCallbackARB, PFNGLDEBUGMESSAGECALLBACKARBPROC);
FNDEF_EX(glDebugMessageControlARB, PFNGLDEBUGMESSAGECONTROLARBPROC);

// GL_ARB_direct_state_access
FNDEF_EX(glCreateBuffers, PFNGLCREATEBUFFERSPROC);
FNDEF_EX(glCreateFramebuffers, PFNGLCREATEFRAMEBUFFERSPROC);
FNDEF_EX(glCreateTextures, PFNGLCREATETEXTURESPROC);
FNDEF_EX(glCreateVertexArrays, PFNGLCREATEVERTEXARRAYSPROC);
FNDEF_EX(glNamedBufferData, PFNGLNAMEDBUFFERDATAPROC);
FNDEF_EX(glNamedBufferSubData, PFNGLNAMEDBUFFERSUBDATAPROC);
FNDEF_EX(glMapNamedBuffer, PFNGLMAPNAMEDBUFFERPROC);
FNDEF_EX(glUnmapNamedBuffer, PFNGLUNMAPNAMEDBUFFERPROC);
FNDEF_EX(glProgramUniform1i, PFNGLPROGRAMUNIFORM1IPROC);
FNDEF_EX(glProgramUniform1ui, PFNGLPROGRAMUNIFORM1UIPROC);
FNDEF_EX(glProgramUniform1f, PFNGLPROGRAMUNIFORM1FPROC);
FNDEF_EX(glProgramUniform2fv, PFNGLPROGRAMUNIFORM2FVPROC);
FNDEF_EX(glProgramUniform3fv, PFNGLPROGRAMUNIFORM3FVPROC);
FNDEF_EX(glProgramUniform4fv, PFNGLPROGRAMUNIFORM4FVPROC);
FNDEF_EX(glProgramUniformMatrix3fv, PFNGLPROGRAMUNIFORMMATRIX3FVPROC);
FNDEF_EX(glProgramUniformMatrix4fv, PFNGLPROGRAMUNIFORMMATRIX4FVPROC);
FNDEF_EX(glTextureParameteri, PFNGLTEXTUREPARAMETERIPROC);
FNDEF_EX(glGenerateTextureMipmap, PFNGLGENERATETEXTUREMIPMAPPROC);
FNDEF_EX(glEnableVertexArrayAttrib, PFNGLENABLEVERTEXARRAYATTRIBPROC);
FNDEF_EX(glCheckNamedFramebufferStatus, PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC);
FNDEF_EX(glNamedRenderbufferStorage, PFNGLNAMEDRENDERBUFFERSTORAGEPROC);
FNDEF_EX(glNamedFramebufferRenderbuffer, PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC);
FNDEF_EX(glNamedFramebufferTexture, PFNGLNAMEDFRAMEBUFFERTEXTUREPROC);
FNDEF_EX(glNamedFramebufferTextureLayer, PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC);
FNDEF_EX(glTextureBuffer, PFNGLTEXTUREBUFFERPROC);
FNDEF_EX(glBindTextureUnit, PFNGLBINDTEXTUREUNITPROC);
FNDEF_EX(glCompressedTextureSubImage1D, PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC);
FNDEF_EX(glCompressedTextureSubImage2D, PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC);
FNDEF_EX(glCompressedTextureSubImage3D, PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC);
FNDEF_EX(glTextureSubImage1D, PFNGLTEXTURESUBIMAGE1DPROC);
FNDEF_EX(glTextureSubImage2D, PFNGLTEXTURESUBIMAGE2DPROC);
FNDEF_EX(glTextureSubImage3D, PFNGLTEXTURESUBIMAGE3DPROC);
FNDEF_EX(glVertexArrayAttribFormat, PFNGLVERTEXARRAYATTRIBFORMATPROC);
FNDEF_EX(glVertexArrayAttribIFormat, PFNGLVERTEXARRAYATTRIBIFORMATPROC);
FNDEF_EX(glVertexArrayAttribBinding, PFNGLVERTEXARRAYATTRIBBINDINGPROC);
FNDEF_EX(glVertexArrayVertexBuffer, PFNGLVERTEXARRAYVERTEXBUFFERPROC);
FNDEF_EX(glVertexArrayBindingDivisor, PFNGLVERTEXARRAYBINDINGDIVISORPROC);
FNDEF_EX(glVertexArrayElementBuffer, PFNGLVERTEXARRAYELEMENTBUFFERPROC);

// GL_ARB_texture_storage
FNDEF_EX(glTextureStorage1D, PFNGLTEXTURESTORAGE1DPROC);
FNDEF_EX(glTextureStorage2D, PFNGLTEXTURESTORAGE2DPROC);
FNDEF_EX(glTextureStorage3D, PFNGLTEXTURESTORAGE3DPROC);
FNDEF_EX(glTextureStorage2DMultisample, PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC);

// GL_ARB_copy_image
FNDEF_EX(glCopyImageSubData, PFNGLCOPYIMAGESUBDATAPROC);

// GL_ARB_gl_spirv
FNDEF_EX(glSpecializeShaderARB, PFNGLSPECIALIZESHADERARBPROC);

// GL_ARB_clip_control
FNDEF_EX(glClipControl, PFNGLCLIPCONTROLPROC);

// GL_ARB_occlusion_query
FNDEF_EX(glGenQueriesARB, PFNGLGENQUERIESARBPROC);
FNDEF_EX(glDeleteQueriesARB, PFNGLDELETEQUERIESARBPROC);
FNDEF_EX(glBeginQueryARB, PFNGLBEGINQUERYARBPROC);
FNDEF_EX(glEndQueryARB, PFNGLENDQUERYARBPROC);
FNDEF_EX(glGetQueryObjectuivARB, PFNGLGETQUERYOBJECTUIVARBPROC);

// GL_ARB_timer_query
FNDEF_EX(glQueryCounter, PFNGLQUERYCOUNTERPROC);


//[-------------------------------------------------------]
//[ Core (OpenGL version dependent)                       ]
//[-------------------------------------------------------]
FNDEF_EX(glShaderBinary, PFNGLSHADERBINARYPROC);	// OpenGL 4.1

//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef FNDEF_EX
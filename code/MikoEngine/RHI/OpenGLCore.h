#pragma once

namespace OpenGLRhi
{
	class Extensions;
	class VertexArray;
	class RootSignature;
	class IOpenGLContext;
	class OpenGLRuntimeLinking;
	class ComputePipelineState;
	class GraphicsPipelineState;
}

#if SE_DEBUG
// Check whether or not the given resource is owned by the given RHI
#define RHI_MATCH_CHECK(rhiReference, resourceReference) \
		RHI_ASSERT(&rhiReference == &(resourceReference).getRhi(), "OpenGL error: The given resource is owned by another RHI instance")

/**
*  @brief
*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
*
*  @param[in] debugName
*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
*/
#define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER , [[maybe_unused]] const char debugName[] = ""
#else
// Check whether or not the given resource is owned by the given RHI
#define RHI_MATCH_CHECK(rhiReference, resourceReference)

/**
*  @brief
*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
*
*  @param[in] debugName
*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
*/
#define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER
#endif



//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{
		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr const char* GLSL_NAME = "GLSL";	// ASCII name of this shader language, always valid (do not free the memory the returned pointer is pointing to)

#ifdef SE_GLSLTOSPIRV
		static bool GlslangInitialized = false;

		// Settings from "glslang/StandAlone/ResourceLimits.cpp"
		static constexpr TBuiltInResource DefaultTBuiltInResource =
		{
			32,		// MaxLights
			6,		// MaxClipPlanes
			32,		// MaxTextureUnits
			32,		// MaxTextureCoords
			64,		// MaxVertexAttribs
			4096,	// MaxVertexUniformComponents
			64,		// MaxVaryingFloats
			32,		// MaxVertexTextureImageUnits
			80,		// MaxCombinedTextureImageUnits
			32,		// MaxTextureImageUnits
			4096,	// MaxFragmentUniformComponents
			32,		// MaxDrawBuffers
			128,	// MaxVertexUniformVectors
			8,		// MaxVaryingVectors
			16,		// MaxFragmentUniformVectors
			16,		// MaxVertexOutputVectors
			15,		// MaxFragmentInputVectors
			-8,		// MinProgramTexelOffset
			7,		// MaxProgramTexelOffset
			8,		// MaxClipDistances
			65535,	// MaxComputeWorkGroupCountX
			65535,	// MaxComputeWorkGroupCountY
			65535,	// MaxComputeWorkGroupCountZ
			1024,	// MaxComputeWorkGroupSizeX
			1024,	// MaxComputeWorkGroupSizeY
			64,		// MaxComputeWorkGroupSizeZ
			1024,	// MaxComputeUniformComponents
			16,		// MaxComputeTextureImageUnits
			8,		// MaxComputeImageUniforms
			8,		// MaxComputeAtomicCounters
			1,		// MaxComputeAtomicCounterBuffers
			60,		// MaxVaryingComponents
			64,		// MaxVertexOutputComponents
			64,		// MaxGeometryInputComponents
			128,	// MaxGeometryOutputComponents
			128,	// MaxFragmentInputComponents
			8,		// MaxImageUnits
			8,		// MaxCombinedImageUnitsAndFragmentOutputs
			8,		// MaxCombinedShaderOutputResources
			0,		// MaxImageSamples
			0,		// MaxVertexImageUniforms
			0,		// MaxTessControlImageUniforms
			0,		// MaxTessEvaluationImageUniforms
			0,		// MaxGeometryImageUniforms
			8,		// MaxFragmentImageUniforms
			8,		// MaxCombinedImageUniforms
			16,		// MaxGeometryTextureImageUnits
			256,	// MaxGeometryOutputVertices
			1024,	// MaxGeometryTotalOutputComponents
			1024,	// MaxGeometryUniformComponents
			64,		// MaxGeometryVaryingComponents
			128,	// MaxTessControlInputComponents
			128,	// MaxTessControlOutputComponents
			16,		// MaxTessControlTextureImageUnits
			1024,	// MaxTessControlUniformComponents
			4096,	// MaxTessControlTotalOutputComponents
			128,	// MaxTessEvaluationInputComponents
			128,	// MaxTessEvaluationOutputComponents
			16,		// MaxTessEvaluationTextureImageUnits
			1024,	// MaxTessEvaluationUniformComponents
			120,	// MaxTessPatchComponents
			32,		// MaxPatchVertices
			64,		// MaxTessGenLevel
			16,		// MaxViewports
			0,		// MaxVertexAtomicCounters
			0,		// MaxTessControlAtomicCounters
			0,		// MaxTessEvaluationAtomicCounters
			0,		// MaxGeometryAtomicCounters
			8,		// MaxFragmentAtomicCounters
			8,		// MaxCombinedAtomicCounters
			1,		// MaxAtomicCounterBindings
			0,		// MaxVertexAtomicCounterBuffers
			0,		// MaxTessControlAtomicCounterBuffers
			0,		// MaxTessEvaluationAtomicCounterBuffers
			0,		// MaxGeometryAtomicCounterBuffers
			1,		// MaxFragmentAtomicCounterBuffers
			1,		// MaxCombinedAtomicCounterBuffers
			16384,	// MaxAtomicCounterBufferSize
			4,		// MaxTransformFeedbackBuffers
			64,		// MaxTransformFeedbackInterleavedComponents
			8,		// MaxCullDistances
			8,		// MaxCombinedClipAndCullDistances
			4,		// MaxSamples
			256,	// MaxMeshOutputVerticesNV
			512,	// MaxMeshOutputPrimitivesNV
			32,		// MaxMeshWorkGroupSizeX_NV
			1,		// MaxMeshWorkGroupSizeY_NV
			1,		// MaxMeshWorkGroupSizeZ_NV
			32,		// MaxTaskWorkGroupSizeX_NV
			1,		// MaxTaskWorkGroupSizeY_NV
			1,		// MaxTaskWorkGroupSizeZ_NV
			4,		// MaxMeshViewCountNV
			{		// limits
				1,	// nonInductiveForLoops
				1,	// whileLoops
				1,	// doWhileLoops
				1,	// generalUniformIndexing
				1,	// generalAttributeMatrixVectorIndexing
				1,	// generalVaryingIndexing
				1,	// generalSamplerIndexing
				1,	// generalVariableIndexing
				1,	// generalConstantMatrixVectorIndexing
			}
		};
#endif

		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		void updateWidthHeight(uint32_t mipmapIndex, uint32_t textureWidth, uint32_t textureHeight, uint32_t& width, uint32_t& height)
		{
			Rhi::ITexture::getMipmapSize(mipmapIndex, textureWidth, textureHeight);
			if ( width > textureWidth )
			{
				width = textureWidth;
			}
			if ( height > textureHeight )
			{
				height = textureHeight;
			}
		}

		void printOpenGLShaderInformationIntoLog(const Rhi::Context& context, GLuint openGLShader)
		{
			// Get the length of the information (including a null termination)
			GLint informationLength = 0;
			glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
			if ( informationLength > 1 )
			{
				// Allocate memory for the information
				char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

				// Get the information
				glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

				// Output the debug string
				RHI_LOG(CRITICAL, informationLog);

					// Cleanup information memory
				RHI_FREE(informationLog);
			}
		}

		void printOpenGLShaderInformationIntoLog(const Rhi::Context& context, GLuint openGLShader, const char* sourceCode)
		{
			// Get the length of the information (including a null termination)
			GLint informationLength = 0;
			glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
			if ( informationLength > 1 )
			{
				// Allocate memory for the information
				char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

				// Get the information
				glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

				// Output the debug string
				if ( GetLog().print(DefaultLog::Type::CRITICAL, sourceCode, __FILE__, static_cast<uint32_t>(__LINE__), informationLog) )
				{
					SE_DEBUG_BREAK;
				}

				// Cleanup information memory
				RHI_FREE(informationLog);
			}
		}

		void printOpenGLProgramInformationIntoLog(const Rhi::Context& context, GLuint openGLProgram)
		{
			// Get the length of the information (including a null termination)
			GLint informationLength = 0;
			glGetProgramiv(openGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
			if ( informationLength > 1 )
			{
				// Allocate memory for the information
				char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

				// Get the information
				glGetProgramInfoLog(openGLProgram, informationLength, nullptr, informationLog);

				// Output the debug string
				RHI_LOG(CRITICAL, informationLog)

					// Cleanup information memory
					RHI_FREE(informationLog);
			}
		}

		void printOpenGLProgramInformationIntoLog(const Rhi::Context& context, GLuint openGLProgram, const char* sourceCode)
		{
			// Get the length of the information (including a null termination)
			GLint informationLength = 0;
			glGetProgramiv(openGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
			if ( informationLength > 1 )
			{
				// Allocate memory for the information
				char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

				// Get the information
				glGetProgramInfoLog(openGLProgram, informationLength, nullptr, informationLog);

				// Output the debug string
				if ( GetLog().print(DefaultLog::Type::CRITICAL, sourceCode, __FILE__, static_cast<uint32_t>(__LINE__), informationLog) )
				{
					SE_DEBUG_BREAK;
				}

				// Cleanup information memory
				RHI_FREE(informationLog);
			}
		}

		/**
		*  @brief
		*    Create and load a shader from bytecode
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] shaderType
		*    Shader type (for example "GL_VERTEX_SHADER_ARB")
		*  @param[in] shaderBytecode
		*    Shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
		*
		*  @return
		*    The OpenGL shader, 0 on error, destroy the resource if you no longer need it
		*/
		[[nodiscard]] GLuint loadShaderFromBytecode(const Rhi::Context& context, GLenum shaderType, const Rhi::ShaderBytecode& shaderBytecode)
		{
			// Create the shader object
			const GLuint openGLShader = glCreateShader(shaderType);

			// Load the SPIR-V module into the shader object
			// -> "glShaderBinary" is OpenGL 4.1
			{
				// Decode from SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
				// -> https://github.com/aras-p/smol-v
				// -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
				const size_t spirvOutputBufferSize = smolv::GetDecodedBufferSize(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
				uint8_t* spirvOutputBuffer = RHI_MALLOC_TYPED(uint8_t, spirvOutputBufferSize);
				smolv::Decode(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes(), spirvOutputBuffer, spirvOutputBufferSize);
				glShaderBinary(1, &openGLShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, spirvOutputBuffer, static_cast<GLsizei>(spirvOutputBufferSize));
				RHI_FREE(spirvOutputBuffer);
			}

			// Done
			return openGLShader;
		}

		/**
		*  @brief
		*    Create and load a shader program from bytecode
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] shaderType
		*    Shader type (for example "GL_VERTEX_SHADER_ARB")
		*  @param[in] shaderBytecode
		*    Shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
		*
		*  @return
		*    The OpenGL shader program, 0 on error, destroy the resource if you no longer need it
		*/
		[[nodiscard]] GLuint loadShaderProgramFromBytecode(const Rhi::Context& context, GLenum shaderType, const Rhi::ShaderBytecode& shaderBytecode)
		{
			// Create and load the shader object
			const GLuint openGLShader = loadShaderFromBytecode(context, shaderType, shaderBytecode);

			// Specialize the shader
			// -> Before this shader the isn't compiled, after this shader is supposed to be compiled
			glSpecializeShaderARB(openGLShader, "main", 0, nullptr, nullptr);

			// Check the compile status
			GLint compiled = GL_FALSE;
			glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
			if ( GL_TRUE == compiled )
			{
				// All went fine, create and return the program
				const GLuint openGLProgram = glCreateProgram();
				glProgramParameteri(openGLProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
				glAttachShader(openGLProgram, openGLShader);
				glLinkProgram(openGLProgram);
				glDetachShader(openGLProgram, openGLShader);
				glDeleteShader(openGLShader);

				// Check the link status
				GLint linked = GL_FALSE;
				glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
				if ( GL_TRUE != linked )
				{
					// Error, program link failed!
					printOpenGLProgramInformationIntoLog(context, openGLProgram, nullptr);
				}

				// Done
				return openGLProgram;
			}
			else
			{
				// Error, failed to compile the shader!
				printOpenGLShaderInformationIntoLog(context, openGLShader, nullptr);

				// Destroy the OpenGL shader
				// -> A value of 0 for shader will be silently ignored
				glDeleteShader(openGLShader);

				// Error!
				return 0u;
			}
		}

		/**
		*  @brief
		*    Create, load and compile a shader program from source code
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] shaderType
		*    Shader type (for example "GL_VERTEX_SHADER_ARB")
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be a valid pointer
		*
		*  @return
		*    The OpenGL shader program, 0 on error, destroy the resource if you no longer need it
		*/
		[[nodiscard]] GLuint loadShaderProgramFromSourceCode(const Rhi::Context& context, GLenum shaderType, const GLchar* sourceCode)
		{
			// Create the shader program
			const GLuint openGLProgram = glCreateShaderProgramv(shaderType, 1, &sourceCode);

			// Check the link status
			GLint linked = GL_FALSE;
			glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
			if ( GL_TRUE == linked )
			{
				// All went fine, return the program
				return openGLProgram;
			}
			else
			{
				// Error, failed to compile the shader!
				printOpenGLProgramInformationIntoLog(context, openGLProgram, sourceCode);

				// Destroy the program
				// -> A value of 0 for shader will be silently ignored
				glDeleteProgram(openGLProgram);

				// Error!
				return 0u;
			}
		}

		// Basing on the implementation from https://www.opengl.org/registry/specs/ARB/separate_shader_objects.txt
		[[nodiscard]] GLuint createShaderProgramObject(const Rhi::Context& context, GLuint openGLShader, const Rhi::VertexAttributes& vertexAttributes)
		{
			if ( openGLShader > 0 )
			{
				// Create the OpenGL program
				const GLuint openGLProgram = glCreateProgram();
				if ( openGLProgram > 0 )
				{
					glProgramParameteri(openGLProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

					// Attach the shader to the program
					glAttachShader(openGLProgram, openGLShader);

					// Define the vertex array attribute binding locations ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 & 12 terminology)
					// -> Crucial code that glCreateShaderProgram doesn't do
					{
						const uint32_t numberOfVertexAttributes = vertexAttributes.numberOfAttributes;
						for ( uint32_t vertexAttribute = 0; vertexAttribute < numberOfVertexAttributes; ++vertexAttribute )
						{
							glBindAttribLocation(openGLProgram, vertexAttribute, vertexAttributes.attributes[vertexAttribute].name);
						}
					}

					// Link the program
					glLinkProgram(openGLProgram);

					// Detach the shader from the program
					glDetachShader(openGLProgram, openGLShader);
				}

				// Destroy the OpenGL shader
				glDeleteShader(openGLShader);

				// Check the link status
				if ( openGLProgram > 0 )
				{
					GLint linked = GL_FALSE;
					glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
					if ( GL_TRUE == linked )
					{
						// Done
						return openGLProgram;
					}
					else
					{
						// Error, program link failed!
						printOpenGLProgramInformationIntoLog(context, openGLProgram);
					}
				}
			}

			// Error!
			return 0;
		}

		[[nodiscard]] GLuint loadShaderProgramFromBytecode(const Rhi::Context& context, const Rhi::VertexAttributes& vertexAttributes, GLenum shaderType, const Rhi::ShaderBytecode& shaderBytecode)
		{
			// Create and load the shader object
			const GLuint openGLShader = loadShaderFromBytecode(context, shaderType, shaderBytecode);

			// Specialize the shader
			// -> Before this shader the isn't compiled, after this shader is supposed to be compiled
			glSpecializeShaderARB(openGLShader, "main", 0, nullptr, nullptr);

			// Check the compile status
			GLint compiled = GL_FALSE;
			glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
			if ( GL_TRUE == compiled )
			{
				// All went fine, create and return the program
				return createShaderProgramObject(context, openGLShader, vertexAttributes);
			}
			else
			{
				// Error, failed to compile the shader!
				printOpenGLShaderInformationIntoLog(context, openGLShader);

				// Destroy the OpenGL shader
				// -> A value of 0 for shader will be silently ignored
				glDeleteShader(openGLShader);

				// Error!
				return 0;
			}
		}

		/**
		*  @brief
		*    Creates, loads and compiles a shader from source code
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] shaderType
		*    Shader type (for example "GL_VERTEX_SHADER_ARB")
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be a valid pointer
		*
		*  @return
		*    The OpenGL shader, 0 on error, destroy the resource if you no longer need it
		*/
		[[nodiscard]] GLuint loadShaderFromSourcecode(const Rhi::Context& context, GLenum shaderType, const GLchar* sourceCode)
		{
			// Create the shader object
			const GLuint openGLShader = glCreateShader(shaderType);

			// Load the shader source
			glShaderSource(openGLShader, 1, &sourceCode, nullptr);

			// Compile the shader
			glCompileShader(openGLShader);

			// Check the compile status
			GLint compiled = GL_FALSE;
			glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
			if ( GL_TRUE == compiled )
			{
				// All went fine, return the shader
				return openGLShader;
			}
			else
			{
				// Error, failed to compile the shader!

				{ // Get the length of the information
					GLint informationLength = 0;
					glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
					if ( informationLength > 1 )
					{
						// Allocate memory for the information
						GLchar* informationLog = RHI_MALLOC_TYPED(GLchar, informationLength);

						// Get the information
						glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

						// Output the debug string
						if ( GetLog().print(DefaultLog::Type::CRITICAL, sourceCode, __FILE__, static_cast<uint32_t>(__LINE__), informationLog) )
						{
							SE_DEBUG_BREAK;
						}

						// Cleanup information memory
						RHI_FREE(informationLog);
					}
				}

				// Destroy the shader
				// -> A value of 0 for shader will be silently ignored
				glDeleteShader(openGLShader);

				// Error!
				return 0u;
			}
		}

		[[nodiscard]] GLuint loadShaderProgramFromSourcecode(const Rhi::Context& context, const Rhi::VertexAttributes& vertexAttributes, GLenum type, const char* sourceCode)
		{
			return createShaderProgramObject(context, loadShaderFromSourcecode(context, type, sourceCode), vertexAttributes);
		}

		/**
		*  @brief
		*    Compile shader source code to shader bytecode
		*
		*  @param[in] context
		*    RHI context to use
		*  @param[in] shaderType
		*    Shader type (for example "GL_VERTEX_SHADER_ARB")
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be a valid pointer
		*  @param[out] shaderBytecode
		*    Receives the shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
		*/
		// TODO(co) Visual Studio 2017 compile settings: For some reasons I need to disable optimization for the following method or else "glslang::TShader::parse()" will output the error "ERROR: 0:1: '€' : unexpected token" (glslang (latest commit c325f4364666eedb94c20a13670df058a38a14ab - April 20, 2018), Visual Studio 2017 15.7.1)
#ifdef _MSC_VER
#pragma optimize("", off)
#endif
		void shaderSourceCodeToShaderBytecode(const Rhi::Context& context, GLenum shaderType, const GLchar* sourceCode, Rhi::ShaderBytecode& shaderBytecode)
		{
#ifdef SE_GLSLTOSPIRV
			// Initialize glslang, if necessary
			if ( !GlslangInitialized )
			{
				glslang::InitializeProcess();
				GlslangInitialized = true;
			}

			// GLSL to intermediate
			// -> OpenGL 4.1 (the best OpenGL version Mac OS X 10.11 supports, so lowest version we have to support)
			// TODO(co) OpenGL GLSL 430 instead of 410 for e.g. "GL_ARB_shader_image_load_store" build in support. Apply dropped OpenGL support so we can probably drop Apple support. If at once point Unrimp should run on Apple hardware, we probably will use MoltenVK for Vulkan (yet another RHI API Metal just for Apple hardware is probably to much work for a spare time project).
			// const int glslVersion = 410;
			const int glslVersion = 430;
			EShLanguage shLanguage = EShLangCount;
			switch ( shaderType )
			{
			case GL_VERTEX_SHADER_ARB:
				shLanguage = EShLangVertex;
				break;

			case GL_TESS_CONTROL_SHADER:
				shLanguage = EShLangTessControl;
				break;

			case GL_TESS_EVALUATION_SHADER:
				shLanguage = EShLangTessEvaluation;
				break;

			case GL_GEOMETRY_SHADER_ARB:
				shLanguage = EShLangGeometry;
				break;

			case GL_FRAGMENT_SHADER_ARB:
				shLanguage = EShLangFragment;
				break;

			case GL_COMPUTE_SHADER:
				shLanguage = EShLangCompute;
				break;
			}
			glslang::TShader shader(shLanguage);
			shader.setEnvInput(glslang::EShSourceGlsl, shLanguage, glslang::EShClientOpenGL, glslVersion);
			shader.setEntryPoint("main");
			{
				const char* sourcePointers[] = { sourceCode };
				shader.setStrings(sourcePointers, 1);
			}
			const EShMessages shMessages = static_cast<EShMessages>(EShMsgDefault);
			if ( shader.parse(&DefaultTBuiltInResource, glslVersion, false, shMessages) )
			{
				glslang::TProgram program;
				program.addShader(&shader);
				if ( program.link(shMessages) )
				{
					// Intermediate to SPIR-V
					const glslang::TIntermediate* intermediate = program.getIntermediate(shLanguage);
					if ( nullptr != intermediate )
					{
						std::vector<unsigned int> spirv;
						glslang::GlslangToSpv(*intermediate, spirv);

						// Encode to SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
						// -> https://github.com/aras-p/smol-v
						// -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
						// -> Don't apply "spv::spirvbin_t::remap()" or the SMOL-V result will be bigger
						smolv::ByteArray byteArray;
						smolv::Encode(spirv.data(), sizeof(unsigned int) * spirv.size(), byteArray, smolv::kEncodeFlagStripDebugInfo);

						// Done
						shaderBytecode.setBytecodeCopy(static_cast<uint32_t>(byteArray.size()), reinterpret_cast<uint8_t*>(byteArray.data()));
					}
				}
				else
				{
					// Failed to link the program
					if ( GetLog().print(DefaultLog::Type::CRITICAL, sourceCode, __FILE__, static_cast<uint32_t>(__LINE__), "Failed to link the GLSL program: %s", program.getInfoLog()) )
					{
						SE_DEBUG_BREAK;
					}
				}
			}
			else
			{
				// Failed to parse the shader source code
				if ( GetLog().print(DefaultLog::Type::CRITICAL, sourceCode, __FILE__, static_cast<uint32_t>(__LINE__), "Failed to parse the GLSL shader source code: %s", shader.getInfoLog()) )
				{
					SE_DEBUG_BREAK;
				}
			}
#endif
		}
		// TODO(co) Visual Studio 2017 compile settings: For some reasons I need to disable optimization for the following method or else "glslang::TShader::parse()" will output the error "ERROR: 0:1: '€' : unexpected token" (glslang (latest commit c325f4364666eedb94c20a13670df058a38a14ab - April 20, 2018), Visual Studio 2017 15.7.1)
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

		void bindUniformBlock(const Rhi::DescriptorRange& descriptorRange, uint32_t openGLProgram, uint32_t uniformBlockBindingIndex)
		{
			// Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension,
			// for backward compatibility, ask for the uniform block index
			const GLuint uniformBlockIndex = glGetUniformBlockIndex(openGLProgram, descriptorRange.baseShaderRegisterName);
			if ( GL_INVALID_INDEX != uniformBlockIndex )
			{
				// Associate the uniform block with the given binding point
				glUniformBlockBinding(openGLProgram, uniformBlockIndex, uniformBlockBindingIndex);
			}
		}

		void bindUniformLocation(const Rhi::DescriptorRange& descriptorRange, uint32_t openGLProgramPipeline, uint32_t openGLProgram)
		{
			const GLint uniformLocation = glGetUniformLocation(openGLProgram, descriptorRange.baseShaderRegisterName);
			if ( uniformLocation >= 0 )
			{
				// OpenGL/GLSL is not automatically assigning texture units to samplers, so, we have to take over this job
				// -> When using OpenGL or OpenGL ES 3 this is required
				// -> OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension supports explicit binding points ("layout(binding = 0)"
				//    in GLSL shader) , for backward compatibility we don't use it in here
				// -> When using Direct3D 9, 10, 11 or 12, the texture unit
				//    to use is usually defined directly within the shader by using the "register"-keyword
				// -> Use the "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" extension if possible to not change OpenGL states
				if ( nullptr != glProgramUniform1i )
				{
					glProgramUniform1i(openGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
				}
				else if ( nullptr != glProgramUniform1iEXT )
				{
					glProgramUniform1iEXT(openGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
				}
				else
				{
					// TODO(co) There's room for binding API call related optimization in here (will certainly be no huge overall efficiency gain)
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently used OpenGL program
					GLint openGLProgramBackup = 0;
					glGetProgramPipelineiv(openGLProgramPipeline, GL_ACTIVE_PROGRAM, &openGLProgramBackup);
					if ( static_cast<uint32_t>(openGLProgramBackup) == openGLProgram )
					{
						// Set uniform, please note that for this our program must be the currently used one
						glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
					}
					else
					{
						// Set uniform, please note that for this our program must be the currently used one
						glActiveShaderProgram(openGLProgramPipeline, openGLProgram);
						glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));

						// Be polite and restore the previous used OpenGL program
						glActiveShaderProgram(openGLProgramPipeline, static_cast<GLuint>(openGLProgramBackup));
					}
#else
					glActiveShaderProgram(openGLProgramPipeline, openGLProgram);
					glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
#endif
				}
			}
		}

		//[-------------------------------------------------------]
		//[ Anonymous detail namespace                            ]
		//[-------------------------------------------------------]
	} // detail
}
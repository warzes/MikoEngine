#include "stdafx.h"
#if SE_OPENGL
#include "Rhi.h"
#include "MakeID.h"
#include "OpenGLFunc.h"
#include "OpenGLCore.h"
#include "OpenGLRhi.h"
#include "OpenGLRuntimeLinking.h"
#include "OpenGLExtensions.h"
#include "IOpenGLContext.h"
#include "OpenGLMapping.h"
#include "ResourceGroup.h"
#include "RootSignature.h"

#include "OpenGLUnsort.h" // не распред

#pragma comment(lib, "OpenGL32.lib")

//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{
		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]

		[[nodiscard]] bool mapBuffer(const OpenGLRhi::Extensions& extensions, GLenum target, [[maybe_unused]] GLenum bindingTarget, GLuint openGLBuffer, Rhi::MapType mapType, Rhi::MappedSubresource& mappedSubresource)
		{
			// TODO(co) This buffer update isn't efficient, use e.g. persistent buffer mapping

			// Is "GL_ARB_direct_state_access" there?
			if (extensions.isGL_ARB_direct_state_access())
			{
				// Effective direct state access (DSA)
				mappedSubresource.data		 = glMapNamedBuffer(openGLBuffer, OpenGLRhi::Mapping::getOpenGLMapType(mapType));
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;
			}
			// Is "GL_EXT_direct_state_access" there?
			else if (extensions.isGL_EXT_direct_state_access())
			{
				// Effective direct state access (DSA)
				mappedSubresource.data		 = glMapNamedBufferEXT(openGLBuffer, OpenGLRhi::Mapping::getOpenGLMapType(mapType));
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;
			}
			else
			{
				// Traditional bind version

				#if SE_OPENGL_STATE_CLEANUP
					// Backup the currently bound OpenGL buffer
					GLint openGLBufferBackup = 0;
					glGetIntegerv(bindingTarget, &openGLBufferBackup);
				#endif

				// Bind this OpenGL buffer
				glBindBufferARB(target, openGLBuffer);

				// Map
				mappedSubresource.data		 = glMapBufferARB(target, OpenGLRhi::Mapping::getOpenGLMapType(mapType));
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;

				#if SE_OPENGL_STATE_CLEANUP
					// Be polite and restore the previous bound OpenGL buffer
					glBindBufferARB(target, static_cast<GLuint>(openGLBufferBackup));
				#endif
			}

			// Done
			RHI_ASSERT(nullptr != mappedSubresource.data, "Mapping of OpenGL buffer failed")
			return (nullptr != mappedSubresource.data);
		}

		void unmapBuffer(const OpenGLRhi::Extensions& extensions, GLenum target, [[maybe_unused]] GLenum bindingTarget, GLuint openGLBuffer)
		{
			// Is "GL_ARB_direct_state_access" there?
			if (extensions.isGL_ARB_direct_state_access())
			{
				// Effective direct state access (DSA)
				glUnmapNamedBuffer(openGLBuffer);
			}
			// Is "GL_EXT_direct_state_access" there?
			else if (extensions.isGL_EXT_direct_state_access())
			{
				// Effective direct state access (DSA)
				glUnmapNamedBufferEXT(openGLBuffer);
			}
			else
			{
				// Traditional bind version

				#if SE_OPENGL_STATE_CLEANUP
					// Backup the currently bound OpenGL buffer
					GLint openGLBufferBackup = 0;
					glGetIntegerv(bindingTarget, &openGLBufferBackup);
				#endif

				// Bind this OpenGL buffer
				glBindBufferARB(target, openGLBuffer);

				// Unmap
				glUnmapBufferARB(target);

				#if SE_OPENGL_STATE_CLEANUP
					// Be polite and restore the previous bound OpenGL buffer
					glBindBufferARB(target, static_cast<GLuint>(openGLBufferBackup));
				#endif
			}
		}

		namespace ImplementationDispatch
		{
			//[-------------------------------------------------------]
			//[ Command buffer                                        ]
			//[-------------------------------------------------------]
			void ExecuteCommandBuffer(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ExecuteCommandBuffer* realData = static_cast<const Rhi::Command::ExecuteCommandBuffer*>(data);
				RHI_ASSERT(nullptr != realData->commandBufferToExecute, "The OpenGL command buffer to execute must be valid")
				rhi.submitCommandBuffer(*realData->commandBufferToExecute);
			}

			//[-------------------------------------------------------]
			//[ Graphics                                              ]
			//[-------------------------------------------------------]
			void SetGraphicsRootSignature(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsRootSignature* realData = static_cast<const Rhi::Command::SetGraphicsRootSignature*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setGraphicsRootSignature(realData->rootSignature);
			}

			void SetGraphicsPipelineState(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsPipelineState* realData = static_cast<const Rhi::Command::SetGraphicsPipelineState*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setGraphicsPipelineState(realData->graphicsPipelineState);
			}

			void SetGraphicsResourceGroup(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsResourceGroup* realData = static_cast<const Rhi::Command::SetGraphicsResourceGroup*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setGraphicsResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
			}

			void SetGraphicsVertexArray(const void* data, Rhi::IRhi& rhi)
			{
				// Input-assembler (IA) stage
				const Rhi::Command::SetGraphicsVertexArray* realData = static_cast<const Rhi::Command::SetGraphicsVertexArray*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setGraphicsVertexArray(realData->vertexArray);
			}

			void SetGraphicsViewports(const void* data, Rhi::IRhi& rhi)
			{
				// Rasterizer (RS) stage
				const Rhi::Command::SetGraphicsViewports* realData = static_cast<const Rhi::Command::SetGraphicsViewports*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setGraphicsViewports(realData->numberOfViewports, (nullptr != realData->viewports) ? realData->viewports : reinterpret_cast<const Rhi::Viewport*>(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData)));
			}

			void SetGraphicsScissorRectangles(const void* data, Rhi::IRhi& rhi)
			{
				// Rasterizer (RS) stage
				const Rhi::Command::SetGraphicsScissorRectangles* realData = static_cast<const Rhi::Command::SetGraphicsScissorRectangles*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setGraphicsScissorRectangles(realData->numberOfScissorRectangles, (nullptr != realData->scissorRectangles) ? realData->scissorRectangles : reinterpret_cast<const Rhi::ScissorRectangle*>(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData)));
			}

			void SetGraphicsRenderTarget(const void* data, Rhi::IRhi& rhi)
			{
				// Output-merger (OM) stage
				const Rhi::Command::SetGraphicsRenderTarget* realData = static_cast<const Rhi::Command::SetGraphicsRenderTarget*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setGraphicsRenderTarget(realData->renderTarget);
			}

			void ClearGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ClearGraphics* realData = static_cast<const Rhi::Command::ClearGraphics*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).clearGraphics(realData->clearFlags, realData->color, realData->z, realData->stencil);
			}

			void DrawGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawGraphics* realData = static_cast<const Rhi::Command::DrawGraphics*>(data);
				if (nullptr != realData->indirectBuffer)
				{
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).drawGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).drawGraphicsEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			void DrawIndexedGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawIndexedGraphics* realData = static_cast<const Rhi::Command::DrawIndexedGraphics*>(data);
				if (nullptr != realData->indirectBuffer)
				{
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).drawIndexedGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).drawIndexedGraphicsEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			void DrawMeshTasks(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawMeshTasks* realData = static_cast<const Rhi::Command::DrawMeshTasks*>(data);
				if (nullptr != realData->indirectBuffer)
				{
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).drawMeshTasks(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).drawMeshTasksEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			//[-------------------------------------------------------]
			//[ Compute                                               ]
			//[-------------------------------------------------------]
			void SetComputeRootSignature(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputeRootSignature* realData = static_cast<const Rhi::Command::SetComputeRootSignature*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setComputeRootSignature(realData->rootSignature);
			}

			void SetComputePipelineState(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputePipelineState* realData = static_cast<const Rhi::Command::SetComputePipelineState*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setComputePipelineState(realData->computePipelineState);
			}

			void SetComputeResourceGroup(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputeResourceGroup* realData = static_cast<const Rhi::Command::SetComputeResourceGroup*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setComputeResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
			}

			void DispatchCompute(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DispatchCompute* realData = static_cast<const Rhi::Command::DispatchCompute*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).dispatchCompute(realData->groupCountX, realData->groupCountY, realData->groupCountZ);
			}

			//[-------------------------------------------------------]
			//[ Resource                                              ]
			//[-------------------------------------------------------]
			void SetTextureMinimumMaximumMipmapIndex(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetTextureMinimumMaximumMipmapIndex* realData = static_cast<const Rhi::Command::SetTextureMinimumMaximumMipmapIndex*>(data);
				if (realData->texture->getResourceType() == Rhi::ResourceType::TEXTURE_2D)
				{
					static_cast<OpenGLRhi::Texture2D*>(realData->texture)->setMinimumMaximumMipmapIndex(realData->minimumMipmapIndex, realData->maximumMipmapIndex);
				}
				else
				{
					RHI_LOG(CRITICAL, "Unsupported OpenGL texture resource type")
				}
			}

			void ResolveMultisampleFramebuffer(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ResolveMultisampleFramebuffer* realData = static_cast<const Rhi::Command::ResolveMultisampleFramebuffer*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).resolveMultisampleFramebuffer(*realData->destinationRenderTarget, *realData->sourceMultisampleFramebuffer);
			}

			void CopyResource(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::CopyResource* realData = static_cast<const Rhi::Command::CopyResource*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).copyResource(*realData->destinationResource, *realData->sourceResource);
			}

			void GenerateMipmaps(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::GenerateMipmaps* realData = static_cast<const Rhi::Command::GenerateMipmaps*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).generateMipmaps(*realData->resource);
			}

			//[-------------------------------------------------------]
			//[ Query                                                 ]
			//[-------------------------------------------------------]
			void ResetQueryPool(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ResetQueryPool* realData = static_cast<const Rhi::Command::ResetQueryPool*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).resetQueryPool(*realData->queryPool, realData->firstQueryIndex, realData->numberOfQueries);
			}

			void BeginQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::BeginQuery* realData = static_cast<const Rhi::Command::BeginQuery*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).beginQuery(*realData->queryPool, realData->queryIndex, realData->queryControlFlags);
			}

			void EndQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::EndQuery* realData = static_cast<const Rhi::Command::EndQuery*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).endQuery(*realData->queryPool, realData->queryIndex);
			}

			void WriteTimestampQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::WriteTimestampQuery* realData = static_cast<const Rhi::Command::WriteTimestampQuery*>(data);
				static_cast<OpenGLRhi::OpenGLRhi&>(rhi).writeTimestampQuery(*realData->queryPool, realData->queryIndex);
			}

			//[-------------------------------------------------------]
			//[ Debug                                                 ]
			//[-------------------------------------------------------]
			#if SE_DEBUG
				void SetDebugMarker(const void* data, Rhi::IRhi& rhi)
				{
					const Rhi::Command::SetDebugMarker* realData = static_cast<const Rhi::Command::SetDebugMarker*>(data);
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).setDebugMarker(realData->name);
				}

				void BeginDebugEvent(const void* data, Rhi::IRhi& rhi)
				{
					const Rhi::Command::BeginDebugEvent* realData = static_cast<const Rhi::Command::BeginDebugEvent*>(data);
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).beginDebugEvent(realData->name);
				}

				void EndDebugEvent(const void*, Rhi::IRhi& rhi)
				{
					static_cast<OpenGLRhi::OpenGLRhi&>(rhi).endDebugEvent();
				}
			#else
				void SetDebugMarker(const void*, Rhi::IRhi&)
				{
					// Nothing here
				}

				void BeginDebugEvent(const void*, Rhi::IRhi&)
				{
					// Nothing here
				}

				void EndDebugEvent(const void*, Rhi::IRhi&)
				{
					// Nothing here
				}
			#endif


		}


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr Rhi::ImplementationDispatchFunction DISPATCH_FUNCTIONS[static_cast<uint8_t>(Rhi::CommandDispatchFunctionIndex::NUMBER_OF_FUNCTIONS)] =
		{
			// Command buffer
			&ImplementationDispatch::ExecuteCommandBuffer,
			// Graphics
			&ImplementationDispatch::SetGraphicsRootSignature,
			&ImplementationDispatch::SetGraphicsPipelineState,
			&ImplementationDispatch::SetGraphicsResourceGroup,
			&ImplementationDispatch::SetGraphicsVertexArray,		// Input-assembler (IA) stage
			&ImplementationDispatch::SetGraphicsViewports,			// Rasterizer (RS) stage
			&ImplementationDispatch::SetGraphicsScissorRectangles,	// Rasterizer (RS) stage
			&ImplementationDispatch::SetGraphicsRenderTarget,		// Output-merger (OM) stage
			&ImplementationDispatch::ClearGraphics,
			&ImplementationDispatch::DrawGraphics,
			&ImplementationDispatch::DrawIndexedGraphics,
			&ImplementationDispatch::DrawMeshTasks,
			// Compute
			&ImplementationDispatch::SetComputeRootSignature,
			&ImplementationDispatch::SetComputePipelineState,
			&ImplementationDispatch::SetComputeResourceGroup,
			&ImplementationDispatch::DispatchCompute,
			// Resource
			&ImplementationDispatch::SetTextureMinimumMaximumMipmapIndex,
			&ImplementationDispatch::ResolveMultisampleFramebuffer,
			&ImplementationDispatch::CopyResource,
			&ImplementationDispatch::GenerateMipmaps,
			// Query
			&ImplementationDispatch::ResetQueryPool,
			&ImplementationDispatch::BeginQuery,
			&ImplementationDispatch::EndQuery,
			&ImplementationDispatch::WriteTimestampQuery,
			// Debug
			&ImplementationDispatch::SetDebugMarker,
			&ImplementationDispatch::BeginDebugEvent,
			&ImplementationDispatch::EndDebugEvent
		};


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace OpenGLRhi
{

	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	OpenGLRhi::OpenGLRhi(const Rhi::Context& context) :
		IRhi(Rhi::NameId::OPENGL, context),
		VertexArrayMakeId(),
		GraphicsPipelineStateMakeId(),
		ComputePipelineStateMakeId(),
		mOpenGLRuntimeLinking(nullptr),
		mOpenGLContext(nullptr),
		mExtensions(nullptr),
		mShaderLanguage(nullptr),
		mGraphicsRootSignature(nullptr),
		mComputeRootSignature(nullptr),
		mDefaultSamplerState(nullptr),
		mOpenGLCopyResourceFramebuffer(0),
		mDefaultOpenGLVertexArray(0),
		// States
		mGraphicsPipelineState(nullptr),
		mComputePipelineState(nullptr),
		// Input-assembler (IA) stage
		mVertexArray(nullptr),
		mOpenGLPrimitiveTopology(0xFFFF),	// Unknown default setting
		mNumberOfVerticesPerPatch(0),
		// Output-merger (OM) stage
		mRenderTarget(nullptr),
		// State cache to avoid making redundant OpenGL calls
		mOpenGLClipControlOrigin(GL_INVALID_ENUM),
		mOpenGLProgramPipeline(0),
		mOpenGLProgram(0),
		mOpenGLIndirectBuffer(0),
		// Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
		mOpenGLVertexProgram(0),
		mDrawIdUniformLocation(-1),
		mCurrentStartInstanceLocation(~0u)
		#if SE_DEBUG
			, mDebugBetweenBeginEndScene(false)
		#endif
	{
		// Is OpenGL available?
		mOpenGLRuntimeLinking = RHI_NEW(OpenGLRuntimeLinking)(*this);
		if (mOpenGLRuntimeLinking->isOpenGLAvaiable())
		{
			const handle nativeWindowHandle = mContext.getNativeWindowHandle();
			const Rhi::TextureFormat::Enum textureFormat = Rhi::TextureFormat::Enum::R8G8B8A8;
			const RenderPass renderPass(*this, 1, &textureFormat, Rhi::TextureFormat::Enum::UNKNOWN, 1 RHI_RESOURCE_DEBUG_NAME("OpenGL Unknown"));
#if SE_PLATFORM_WINDOWS
			{
				// TODO(co) Add external OpenGL context support
				mOpenGLContext = RHI_NEW(OpenGLContextWindows)(mOpenGLRuntimeLinking, renderPass.getDepthStencilAttachmentTextureFormat(), nativeWindowHandle);
			}
			#elif defined LINUX
				mOpenGLContext = RHI_NEW(OpenGLContextLinux)(*this, mOpenGLRuntimeLinking, renderPass.getDepthStencilAttachmentTextureFormat(), nativeWindowHandle, mContext.isUsingExternalContext());
			#else
				#error "Unsupported platform"
			#endif

			// We're using "this" in here, so we are not allowed to write the following within the initializer list
			mExtensions = RHI_NEW(Extensions)(*this, *mOpenGLContext);

			// Is the OpenGL context and extensions initialized?
			if (mOpenGLContext->isInitialized() && mExtensions->initialize())
			{
				#if SE_DEBUG
					// "GL_ARB_debug_output"-extension available?
					if (mExtensions->isGL_ARB_debug_output())
					{
						// Synchronous debug output, please
						// -> Makes it easier to find the place causing the issue
						glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

						// Disable severity notifications, most drivers print many things with this severity
						glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, false);

						// Set the debug message callback function
						glDebugMessageCallbackARB(&OpenGLRhi::debugMessageCallback, this);
					}
				#endif

				// Globally enable seamless cube map texture, e.g. Direct3D 11 has this enabled by default so do the same for OpenGL
				// -> The following is just for the sake of completeness: It's the year 2020 and OpenGL on Mac is officially dead. But if someone would still
				//    want to support it in a productive way, one has to take care of the situation that enabling seamless cube map texture can result on
				//    slow software rendering on Mac. For checking whether or not this is the case, see "GL_TEXTURE_CUBE_MAP_SEAMLESS on OS X" published at April 26, 2012 on http://distrustsimplicity.net/articles/gl_texture_cube_map_seamless-on-os-x/
				//    "
				//    GLint gpuVertex, gpuFragment;
				//    CGLGetParameter(CGLGetCurrentContext(), kCGLCPGPUVertexProcessing, &gpuVertex);
				//    CGLGetParameter(CGLGetCurrentContext(), kCGLCPGPUFragmentProcessing, &gpuFragment);
				//    "
				glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

				// Initialize the capabilities
				initializeCapabilities();

				// Create the default sampler state
				mDefaultSamplerState = createSamplerState(Rhi::ISamplerState::getDefaultSamplerState());

				// Create default OpenGL vertex array
				if (mExtensions->isGL_ARB_vertex_array_object())
				{
					glGenVertexArrays(1, &mDefaultOpenGLVertexArray);
					glBindVertexArray(mDefaultOpenGLVertexArray);
				}

				// Add references to the default sampler state and set it
				if (nullptr != mDefaultSamplerState)
				{
					mDefaultSamplerState->addReference();
					// TODO(co) Set default sampler states
				}
			}
		}
	}

	OpenGLRhi::~OpenGLRhi()
	{
		// Set no graphics and compute pipeline state reference, in case we have one
		if (nullptr != mGraphicsPipelineState)
		{
			setGraphicsPipelineState(nullptr);
		}
		if (nullptr != mComputePipelineState)
		{
			setComputePipelineState(nullptr);
		}

		// Set no vertex array reference, in case we have one
		if (nullptr != mVertexArray)
		{
			setGraphicsVertexArray(nullptr);
		}

		// Release instances
		if (nullptr != mRenderTarget)
		{
			mRenderTarget->releaseReference();
			mRenderTarget = nullptr;
		}
		if (nullptr != mDefaultSamplerState)
		{
			mDefaultSamplerState->releaseReference();
			mDefaultSamplerState = nullptr;
		}

		// Destroy the OpenGL framebuffer used by "OpenGLRhi::OpenGLRhi::copyResource()" if the "GL_ARB_copy_image"-extension isn't available
		// -> Silently ignores 0's and names that do not correspond to existing buffer objects
		glDeleteFramebuffers(1, &mOpenGLCopyResourceFramebuffer);

		// Destroy the OpenGL default vertex array
		// -> Silently ignores 0's and names that do not correspond to existing vertex array objects
		glDeleteVertexArrays(1, &mDefaultOpenGLVertexArray);

		// Release the graphics and compute root signature instance, in case we have one
		if (nullptr != mGraphicsRootSignature)
		{
			mGraphicsRootSignature->releaseReference();
		}
		if (nullptr != mComputeRootSignature)
		{
			mComputeRootSignature->releaseReference();
		}

		#if SE_RHI_STATISTICS
		{ // For debugging: At this point there should be no resource instances left, validate this!
			// -> Are the currently any resource instances?
			const uint32_t numberOfCurrentResources = getStatistics().GetNumberOfCurrentResources();
			if (numberOfCurrentResources > 0)
			{
				// Error!
				if (numberOfCurrentResources > 1)
				{
					RHI_LOG(CRITICAL, "The OpenGL RHI implementation is going to be destroyed, but there are still %lu resource instances left (memory leak)", numberOfCurrentResources)
				}
				else
				{
					RHI_LOG(CRITICAL, "The OpenGL RHI implementation is going to be destroyed, but there is still one resource instance left (memory leak)")
				}

				// Use debug output to show the current number of resource instances
				getStatistics().DebugOutputCurrentResouces();
			}
		}
		#endif

		// Release the shader language instance, in case we have one
		if (nullptr != mShaderLanguage)
		{
			mShaderLanguage->releaseReference();
		}

		// Destroy the extensions instance
		RHI_DELETE(Extensions, mExtensions);

		// Destroy the OpenGL context instance
		RHI_DELETE(IOpenGLContext, mOpenGLContext);

		// Destroy the OpenGL runtime linking instance
		RHI_DELETE(OpenGLRuntimeLinking, mOpenGLRuntimeLinking);
	}


	//[-------------------------------------------------------]
	//[ Graphics                                              ]
	//[-------------------------------------------------------]
	void OpenGLRhi::setGraphicsRootSignature(Rhi::IRootSignature* rootSignature)
	{
		if (nullptr != mGraphicsRootSignature)
		{
			mGraphicsRootSignature->releaseReference();
		}
		mGraphicsRootSignature = static_cast<RootSignature*>(rootSignature);
		if (nullptr != mGraphicsRootSignature)
		{
			mGraphicsRootSignature->addReference();

			// Sanity check
			RHI_MATCH_CHECK(*this, *rootSignature)
		}
	}

	void OpenGLRhi::setGraphicsPipelineState(Rhi::IGraphicsPipelineState* graphicsPipelineState)
	{
		if (mGraphicsPipelineState != graphicsPipelineState)
		{
			if (nullptr != graphicsPipelineState)
			{
				// Sanity check
				RHI_MATCH_CHECK(*this, *graphicsPipelineState)

				// Set new graphics pipeline state and add a reference to it
				if (nullptr != mGraphicsPipelineState)
				{
					mGraphicsPipelineState->releaseReference();
				}
				mGraphicsPipelineState = static_cast<GraphicsPipelineState*>(graphicsPipelineState);
				mGraphicsPipelineState->addReference();

				// Set OpenGL primitive topology
				mOpenGLPrimitiveTopology = mGraphicsPipelineState->getOpenGLPrimitiveTopology();
				const int newNumberOfVerticesPerPatch = mGraphicsPipelineState->getNumberOfVerticesPerPatch();
				if (0 != newNumberOfVerticesPerPatch && mNumberOfVerticesPerPatch != newNumberOfVerticesPerPatch)
				{
					mNumberOfVerticesPerPatch = newNumberOfVerticesPerPatch;
					glPatchParameteri(GL_PATCH_VERTICES, mNumberOfVerticesPerPatch);
				}

				// Set graphics pipeline state
				mGraphicsPipelineState->bindGraphicsPipelineState();
			}
			else if (nullptr != mGraphicsPipelineState)
			{
				// TODO(co) Handle this situation by resetting OpenGL states?
				mGraphicsPipelineState->releaseReference();
				mGraphicsPipelineState = nullptr;
			}
		}

		// Set graphics pipeline state
		else if (nullptr != mGraphicsPipelineState)
		{
			// Set OpenGL graphics pipeline state
			// -> This is necessary since OpenGL is using just a single current program, for graphics as well as compute
			setOpenGLGraphicsProgram(mGraphicsPipelineState->getGraphicsProgram());
		}
	}

	void OpenGLRhi::setGraphicsResourceGroup(uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup)
	{
		// Security checks
		#if SE_DEBUG
		{
			if (nullptr == mGraphicsRootSignature)
			{
				RHI_LOG(CRITICAL, "No OpenGL RHI implementation graphics root signature set")
				return;
			}
			const Rhi::RootSignature& rootSignature = mGraphicsRootSignature->getRootSignature();
			if (rootParameterIndex >= rootSignature.numberOfParameters)
			{
				RHI_LOG(CRITICAL, "The OpenGL RHI implementation root parameter index is out of bounds")
				return;
			}
			const Rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			if (Rhi::RootParameterType::DESCRIPTOR_TABLE != rootParameter.parameterType)
			{
				RHI_LOG(CRITICAL, "The OpenGL RHI implementation root parameter index doesn't reference a descriptor table")
				return;
			}
			if (nullptr == reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges))
			{
				RHI_LOG(CRITICAL, "The OpenGL RHI implementation descriptor ranges is a null pointer")
				return;
			}
		}
		#endif

		// Set graphics resource group
		setResourceGroup(*mGraphicsRootSignature, rootParameterIndex, resourceGroup);
	}

	void OpenGLRhi::setGraphicsVertexArray(Rhi::IVertexArray* vertexArray)
	{
		// Input-assembler (IA) stage

		// New vertex array?
		if (mVertexArray != vertexArray)
		{
			// Set a vertex array?
			if (nullptr != vertexArray)
			{
				// Sanity check
				RHI_MATCH_CHECK(*this, *vertexArray)

				// Unset the currently used vertex array
				unsetGraphicsVertexArray();

				// Set new vertex array and add a reference to it
				mVertexArray = static_cast<VertexArray*>(vertexArray);
				mVertexArray->addReference();

				// Evaluate the internal array type of the new vertex array to set
				switch (static_cast<VertexArray*>(mVertexArray)->getInternalResourceType())
				{
					case VertexArray::InternalResourceType::NO_VAO:
						// Enable OpenGL vertex attribute arrays
						static_cast<VertexArrayNoVao*>(mVertexArray)->enableOpenGLVertexAttribArrays();
						break;

					case VertexArray::InternalResourceType::VAO:
						// Bind OpenGL vertex array
						glBindVertexArray(static_cast<VertexArrayVao*>(mVertexArray)->getOpenGLVertexArray());
						break;
				}
			}
			else
			{
				// Unset the currently used vertex array
				unsetGraphicsVertexArray();
			}
		}
	}

	void OpenGLRhi::setGraphicsViewports([[maybe_unused]] uint32_t numberOfViewports, const Rhi::Viewport* viewports)
	{
		// Rasterizer (RS) stage

		// Sanity check
		RHI_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid OpenGL rasterizer state viewports")

		// In OpenGL, the origin of the viewport is left bottom while Direct3D is using a left top origin. To make the
		// Direct3D 11 implementation as efficient as possible the Direct3D convention is used and we have to convert in here.
		// -> This isn't influenced by the "GL_ARB_clip_control"-extension

		// Get the width and height of the current render target
		uint32_t renderTargetHeight = 1;
		if (nullptr != mRenderTarget)
		{
			uint32_t renderTargetWidth = 1;
			mRenderTarget->getWidthAndHeight(renderTargetWidth, renderTargetHeight);
		}

		// Set the OpenGL viewport
		// TODO(co) "GL_ARB_viewport_array" support ("OpenGLRhi::setGraphicsViewports()")
		// TODO(co) Check for "numberOfViewports" out of range or are the debug events good enough?
		RHI_ASSERT(numberOfViewports <= 1, "OpenGL supports only one viewport")
		glViewport(static_cast<GLint>(viewports->topLeftX), static_cast<GLint>(renderTargetHeight - viewports->topLeftY - viewports->height), static_cast<GLsizei>(viewports->width), static_cast<GLsizei>(viewports->height));
		glDepthRange(static_cast<GLclampd>(viewports->minDepth), static_cast<GLclampd>(viewports->maxDepth));
	}

	void OpenGLRhi::setGraphicsScissorRectangles([[maybe_unused]] uint32_t numberOfScissorRectangles, const Rhi::ScissorRectangle* scissorRectangles)
	{
		// Rasterizer (RS) stage

		// Sanity check
		RHI_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid OpenGL rasterizer state scissor rectangles")

		// In OpenGL, the origin of the scissor rectangle is left bottom while Direct3D is using a left top origin. To make the
		// Direct3D 9 & 10 & 11 implementation as efficient as possible the Direct3D convention is used and we have to convert in here.
		// -> This isn't influenced by the "GL_ARB_clip_control"-extension

		// Get the width and height of the current render target
		uint32_t renderTargetHeight = 1;
		if (nullptr != mRenderTarget)
		{
			uint32_t renderTargetWidth = 1;
			mRenderTarget->getWidthAndHeight(renderTargetWidth, renderTargetHeight);
		}

		// Set the OpenGL scissor rectangle
		// TODO(co) "GL_ARB_viewport_array" support ("OpenGLRhi::setGraphicsViewports()")
		// TODO(co) Check for "numberOfViewports" out of range or are the debug events good enough?
		RHI_ASSERT(numberOfScissorRectangles <= 1, "OpenGL supports only one scissor rectangle")
		const GLsizei width  = scissorRectangles->bottomRightX - scissorRectangles->topLeftX;
		const GLsizei height = scissorRectangles->bottomRightY - scissorRectangles->topLeftY;
		glScissor(static_cast<GLint>(scissorRectangles->topLeftX), static_cast<GLint>(renderTargetHeight - static_cast<uint32_t>(scissorRectangles->topLeftY) - height), width, height);
	}

	void OpenGLRhi::setGraphicsRenderTarget(Rhi::IRenderTarget* renderTarget)
	{
		// Output-merger (OM) stage

		// New render target?
		if (mRenderTarget != renderTarget)
		{
			// Set a render target?
			if (nullptr != renderTarget)
			{
				// Sanity check
				RHI_MATCH_CHECK(*this, *renderTarget)

				// Release the render target reference, in case we have one
				if (nullptr != mRenderTarget)
				{
					// Unbind OpenGL framebuffer?
					if (Rhi::ResourceType::FRAMEBUFFER == mRenderTarget->getResourceType() && Rhi::ResourceType::FRAMEBUFFER != renderTarget->getResourceType())
					{
						// Do we need to disable multisample?
						if (static_cast<Framebuffer*>(mRenderTarget)->isMultisampleRenderTarget())
						{
							glDisable(GL_MULTISAMPLE);
						}

						// We do not render into a OpenGL framebuffer
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
					}

					// Release
					mRenderTarget->releaseReference();
				}

				// Set new render target and add a reference to it
				mRenderTarget = renderTarget;
				mRenderTarget->addReference();

				// Evaluate the render target type
				GLenum clipControlOrigin = GL_UPPER_LEFT;
				switch (mRenderTarget->getResourceType())
				{
					case Rhi::ResourceType::SWAP_CHAIN:
					{
						static_cast<SwapChain*>(mRenderTarget)->getOpenGLContext().makeCurrent();
						clipControlOrigin = GL_LOWER_LEFT;	// Compensate OS window coordinate system y-flip
						break;
					}

					case Rhi::ResourceType::FRAMEBUFFER:
					{
						// Get the OpenGL framebuffer instance
						Framebuffer* framebuffer = static_cast<Framebuffer*>(mRenderTarget);

						// Bind the OpenGL framebuffer
						glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getOpenGLFramebuffer());

						// Define the OpenGL buffers to draw into, "GL_ARB_draw_buffers"-extension required
						if (mExtensions->isGL_ARB_draw_buffers())
						{
							// https://www.opengl.org/registry/specs/ARB/draw_buffers.txt - "The draw buffer for output colors beyond <n> is set to NONE."
							// -> Meaning depth only rendering which has no color textures at all will work as well, no need for "glDrawBuffer(GL_NONE)"
							static constexpr GLenum OPENGL_DRAW_BUFFER[16] =
							{
								GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3,
								GL_COLOR_ATTACHMENT4,  GL_COLOR_ATTACHMENT5,  GL_COLOR_ATTACHMENT6,  GL_COLOR_ATTACHMENT7,
								GL_COLOR_ATTACHMENT8,  GL_COLOR_ATTACHMENT9,  GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
								GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15
							};
							glDrawBuffersARB(static_cast<GLsizei>(framebuffer->getNumberOfColorTextures()), OPENGL_DRAW_BUFFER);
						}

						// Do we need to enable multisample?
						if (framebuffer->isMultisampleRenderTarget())
						{
							glEnable(GL_MULTISAMPLE);
						}
						else
						{
							glDisable(GL_MULTISAMPLE);
						}
						break;
					}

					case Rhi::ResourceType::ROOT_SIGNATURE:
					case Rhi::ResourceType::RESOURCE_GROUP:
					case Rhi::ResourceType::GRAPHICS_PROGRAM:
					case Rhi::ResourceType::VERTEX_ARRAY:
					case Rhi::ResourceType::RENDER_PASS:
					case Rhi::ResourceType::QUERY_POOL:
					case Rhi::ResourceType::VERTEX_BUFFER:
					case Rhi::ResourceType::INDEX_BUFFER:
					case Rhi::ResourceType::TEXTURE_BUFFER:
					case Rhi::ResourceType::STRUCTURED_BUFFER:
					case Rhi::ResourceType::INDIRECT_BUFFER:
					case Rhi::ResourceType::UNIFORM_BUFFER:
					case Rhi::ResourceType::TEXTURE_1D:
					case Rhi::ResourceType::TEXTURE_1D_ARRAY:
					case Rhi::ResourceType::TEXTURE_2D:
					case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					case Rhi::ResourceType::TEXTURE_3D:
					case Rhi::ResourceType::TEXTURE_CUBE:
					case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
					case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
					case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
					case Rhi::ResourceType::SAMPLER_STATE:
					case Rhi::ResourceType::VERTEX_SHADER:
					case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
					case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
					case Rhi::ResourceType::GEOMETRY_SHADER:
					case Rhi::ResourceType::FRAGMENT_SHADER:
					case Rhi::ResourceType::TASK_SHADER:
					case Rhi::ResourceType::MESH_SHADER:
					case Rhi::ResourceType::COMPUTE_SHADER:
					default:
						// Not handled in here
						break;
				}

				// Setup clip control
				if (mOpenGLClipControlOrigin != clipControlOrigin && mExtensions->isGL_ARB_clip_control())
				{
					// OpenGL default is "GL_LOWER_LEFT" and "GL_NEGATIVE_ONE_TO_ONE", change it to match Vulkan and Direct3D
					mOpenGLClipControlOrigin = clipControlOrigin;
					glClipControl(mOpenGLClipControlOrigin, GL_ZERO_TO_ONE);
				}
			}
			else if (nullptr != mRenderTarget)
			{
				// Evaluate the render target type
				if (Rhi::ResourceType::FRAMEBUFFER == mRenderTarget->getResourceType())
				{
					// We do not render into a OpenGL framebuffer
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}

				// TODO(co) Set no active render target

				// Release the render target reference, in case we have one
				mRenderTarget->releaseReference();
				mRenderTarget = nullptr;
			}
		}
	}

	void OpenGLRhi::clearGraphics(uint32_t clearFlags, const float color[4], float z, uint32_t stencil)
	{
		// Sanity check
		RHI_ASSERT(z >= 0.0f && z <= 1.0f, "The OpenGL clear graphics z value must be between [0, 1] (inclusive)")

		// Get API flags
		uint32_t flagsApi = 0;
		if (clearFlags & Rhi::ClearFlag::COLOR)
		{
			flagsApi |= GL_COLOR_BUFFER_BIT;
		}
		if (clearFlags & Rhi::ClearFlag::DEPTH)
		{
			flagsApi |= GL_DEPTH_BUFFER_BIT;
		}
		if (clearFlags & Rhi::ClearFlag::STENCIL)
		{
			flagsApi |= GL_STENCIL_BUFFER_BIT;
		}

		// Are API flags set?
		if (0 != flagsApi)
		{
			// Set clear settings
			if (clearFlags & Rhi::ClearFlag::COLOR)
			{
				glClearColor(color[0], color[1], color[2], color[3]);
			}
			if (clearFlags & Rhi::ClearFlag::DEPTH)
			{
				glClearDepth(static_cast<GLclampd>(z));
				if (nullptr != mGraphicsPipelineState && Rhi::DepthWriteMask::ALL != mGraphicsPipelineState->getDepthStencilState().depthWriteMask)
				{
					glDepthMask(GL_TRUE);
				}
			}
			if (clearFlags & Rhi::ClearFlag::STENCIL)
			{
				glClearStencil(static_cast<GLint>(stencil));
			}

			// Unlike OpenGL, when using Direct3D 10 & 11 the scissor rectangle(s) do not affect the clear operation
			// -> We have to compensate the OpenGL behaviour in here

			// Disable OpenGL scissor test, in case it's not disabled, yet
			if (nullptr != mGraphicsPipelineState && mGraphicsPipelineState->getRasterizerState().scissorEnable)
			{
				glDisable(GL_SCISSOR_TEST);
			}

			// Clear
			glClear(flagsApi);

			// Restore the previously set OpenGL states
			if (nullptr != mGraphicsPipelineState && mGraphicsPipelineState->getRasterizerState().scissorEnable)
			{
				glEnable(GL_SCISSOR_TEST);
			}
			if ((clearFlags & Rhi::ClearFlag::DEPTH) && nullptr != mGraphicsPipelineState && Rhi::DepthWriteMask::ALL != mGraphicsPipelineState->getDepthStencilState().depthWriteMask)
			{
				glDepthMask(GL_FALSE);
			}
		}
	}

	void OpenGLRhi::drawGraphics(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, indirectBuffer)
		RHI_ASSERT(numberOfDraws > 0, "Number of OpenGL draws must not be zero")
		RHI_ASSERT(mExtensions->isGL_ARB_draw_indirect(), "The GL_ARB_draw_indirect OpenGL extension isn't supported")
		// It's possible to draw without "mVertexArray"

		// Tessellation support: "glPatchParameteri()" is called within "OpenGLRhi::iaSetPrimitiveTopology()"

		{ // Bind indirect buffer
			const GLuint openGLIndirectBuffer = static_cast<const IndirectBuffer&>(indirectBuffer).getOpenGLIndirectBuffer();
			if (openGLIndirectBuffer != mOpenGLIndirectBuffer)
			{
				mOpenGLIndirectBuffer = openGLIndirectBuffer;
				glBindBufferARB(GL_DRAW_INDIRECT_BUFFER, mOpenGLIndirectBuffer);
			}
		}

		// Draw indirect
		if (1 == numberOfDraws)
		{
			glDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
		}
		else if (numberOfDraws > 1)
		{
			if (mExtensions->isGL_ARB_multi_draw_indirect())
			{
				glMultiDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)), static_cast<GLsizei>(numberOfDraws), 0);	// 0 = tightly packed
			}
			else
			{
				// Emulate multi-draw-indirect
				#if SE_DEBUG
					beginDebugEvent("Multi-draw-indirect emulation");
				#endif
				for (uint32_t i = 0; i < numberOfDraws; ++i)
				{
					glDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
					indirectBufferOffset += sizeof(Rhi::DrawArguments);
				}
				#if SE_DEBUG
					endDebugEvent();
				#endif
			}
		}
	}

	void OpenGLRhi::drawGraphicsEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
		RHI_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")
		// It's possible to draw without "mVertexArray"

		// TODO(co) Currently no buffer overflow check due to lack of interface provided data
		emulationData += indirectBufferOffset;

		// Emit the draw calls
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-draw-indirect emulation");
			}
		#endif
		for (uint32_t i = 0; i < numberOfDraws; ++i)
		{
			const Rhi::DrawArguments& drawArguments = *reinterpret_cast<const Rhi::DrawArguments*>(emulationData);
			updateGL_ARB_base_instanceEmulation(drawArguments.startInstanceLocation);

			// Draw and advance
			if ((drawArguments.instanceCount > 1 && mExtensions->isGL_ARB_draw_instanced()) || (drawArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance()))
			{
				// With instancing
				if (drawArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance())
				{
					glDrawArraysInstancedBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance), static_cast<GLsizei>(drawArguments.instanceCount), drawArguments.startInstanceLocation);
				}
				else
				{
					glDrawArraysInstancedARB(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance), static_cast<GLsizei>(drawArguments.instanceCount));
				}
			}
			else
			{
				// Without instancing
				RHI_ASSERT(drawArguments.instanceCount <= 1, "Invalid OpenGL instance count")
				glDrawArrays(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance));
			}
			emulationData += sizeof(Rhi::DrawArguments);
		}
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
		#endif
	}

	void OpenGLRhi::drawIndexedGraphics(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, indirectBuffer)
		RHI_ASSERT(numberOfDraws > 0, "Number of OpenGL draws must not be zero")
		RHI_ASSERT(nullptr != mVertexArray, "OpenGL draw indexed needs a set vertex array")
		RHI_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "OpenGL draw indexed needs a set vertex array which contains an index buffer")
		RHI_ASSERT(mExtensions->isGL_ARB_draw_indirect(), "The GL_ARB_draw_indirect OpenGL extension isn't supported")

		// Tessellation support: "glPatchParameteri()" is called within "OpenGLRhi::iaSetPrimitiveTopology()"

		{ // Bind indirect buffer
			const GLuint openGLIndirectBuffer = static_cast<const IndirectBuffer&>(indirectBuffer).getOpenGLIndirectBuffer();
			if (openGLIndirectBuffer != mOpenGLIndirectBuffer)
			{
				mOpenGLIndirectBuffer = openGLIndirectBuffer;
				glBindBufferARB(GL_DRAW_INDIRECT_BUFFER, mOpenGLIndirectBuffer);
			}
		}

		// Draw indirect
		if (1 == numberOfDraws)
		{
			glDrawElementsIndirect(mOpenGLPrimitiveTopology, mVertexArray->getIndexBuffer()->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
		}
		else if (numberOfDraws > 1)
		{
			if (mExtensions->isGL_ARB_multi_draw_indirect())
			{
				glMultiDrawElementsIndirect(mOpenGLPrimitiveTopology, mVertexArray->getIndexBuffer()->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)), static_cast<GLsizei>(numberOfDraws), 0);	// 0 = tightly packed
			}
			else
			{
				// Emulate multi-indexed-draw-indirect
				#if SE_DEBUG
					beginDebugEvent("Multi-indexed-draw-indirect emulation");
				#endif
				const uint32_t openGLType = mVertexArray->getIndexBuffer()->getOpenGLType();
				for (uint32_t i = 0; i < numberOfDraws; ++i)
				{
					glDrawElementsIndirect(mOpenGLPrimitiveTopology, openGLType, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
					indirectBufferOffset += sizeof(Rhi::DrawIndexedArguments);
				}
				#if SE_DEBUG
					endDebugEvent();
				#endif
			}
		}
	}

	void OpenGLRhi::drawIndexedGraphicsEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
		RHI_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")
		RHI_ASSERT(nullptr != mVertexArray, "OpenGL draw indexed needs a set vertex array")
		RHI_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "OpenGL draw indexed needs a set vertex array which contains an index buffer")

		// TODO(co) Currently no buffer overflow check due to lack of interface provided data
		emulationData += indirectBufferOffset;

		// Emit the draw calls
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-indexed-draw-indirect emulation");
			}
		#endif
		IndexBuffer* indexBuffer = mVertexArray->getIndexBuffer();
		for (uint32_t i = 0; i < numberOfDraws; ++i)
		{
			const Rhi::DrawIndexedArguments& drawIndexedArguments = *reinterpret_cast<const Rhi::DrawIndexedArguments*>(emulationData);
			updateGL_ARB_base_instanceEmulation(drawIndexedArguments.startInstanceLocation);

			// Draw and advance
			if ((drawIndexedArguments.instanceCount > 1 && mExtensions->isGL_ARB_draw_instanced()) || (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance()))
			{
				// With instancing
				if (drawIndexedArguments.baseVertexLocation > 0)
				{
					// Use start instance location?
					if (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance())
					{
						// Draw with base vertex location and start instance location
						glDrawElementsInstancedBaseVertexBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), static_cast<GLint>(drawIndexedArguments.baseVertexLocation), drawIndexedArguments.startInstanceLocation);
					}

					// Is the "GL_ARB_draw_elements_base_vertex" extension there?
					else if (mExtensions->isGL_ARB_draw_elements_base_vertex())
					{
						// Draw with base vertex location
						glDrawElementsInstancedBaseVertex(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), static_cast<GLint>(drawIndexedArguments.baseVertexLocation));
					}
					else
					{
						// Error!
						RHI_ASSERT(false, "Failed to OpenGL draw indexed emulated")
					}
				}
				else if (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance())
				{
					// Draw without base vertex location and with start instance location
					glDrawElementsInstancedBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), drawIndexedArguments.startInstanceLocation);
				}
				else
				{
					// Draw without base vertex location
					glDrawElementsInstancedARB(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount));
				}
			}
			else
			{
				// Without instancing
				RHI_ASSERT(drawIndexedArguments.instanceCount <= 1, "Invalid OpenGL instance count")

				// Use base vertex location?
				if (drawIndexedArguments.baseVertexLocation > 0)
				{
					// Is the "GL_ARB_draw_elements_base_vertex" extension there?
					if (mExtensions->isGL_ARB_draw_elements_base_vertex())
					{
						// Draw with base vertex location
						glDrawElementsBaseVertex(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLint>(drawIndexedArguments.baseVertexLocation));
					}
					else
					{
						// Error!
						RHI_ASSERT(false, "Failed to OpenGL draw indexed emulated")
					}
				}
				else
				{
					// Draw without base vertex location
					glDrawElements(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())));
				}
			}
			emulationData += sizeof(Rhi::DrawIndexedArguments);
		}
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
		#endif
	}

	void OpenGLRhi::drawMeshTasks([[maybe_unused]] const Rhi::IIndirectBuffer& indirectBuffer, [[maybe_unused]] uint32_t indirectBufferOffset, [[maybe_unused]] uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")

		// TODO(co) Implement me
		/*
		void MultiDrawMeshTasksIndirectNV(intptr indirect,
										sizei drawcount,
										sizei stride);

		void MultiDrawMeshTasksIndirectCountNV( intptr indirect,
												intptr drawcount,
												sizei maxdrawcount,
												sizei stride);
	  */
	}

	void OpenGLRhi::drawMeshTasksEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
		RHI_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")

		// TODO(co) Currently no buffer overflow check due to lack of interface provided data
		emulationData += indirectBufferOffset;

		// Emit the draw calls
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-indexed-draw-indirect emulation");
			}
		#endif
		for (uint32_t i = 0; i < numberOfDraws; ++i)
		{
			const Rhi::DrawMeshTasksArguments& drawMeshTasksArguments = *reinterpret_cast<const Rhi::DrawMeshTasksArguments*>(emulationData);

			// Draw and advance
			glDrawMeshTasksNV(drawMeshTasksArguments.firstTask, drawMeshTasksArguments.numberOfTasks);
			emulationData += sizeof(Rhi::DrawMeshTasksArguments);
		}
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
		#endif
	}


	//[-------------------------------------------------------]
	//[ Compute                                               ]
	//[-------------------------------------------------------]
	void OpenGLRhi::setComputeRootSignature(Rhi::IRootSignature* rootSignature)
	{
		if (nullptr != mComputeRootSignature)
		{
			mComputeRootSignature->releaseReference();
		}
		mComputeRootSignature = static_cast<RootSignature*>(rootSignature);
		if (nullptr != mComputeRootSignature)
		{
			mComputeRootSignature->addReference();

			// Sanity check
			RHI_MATCH_CHECK(*this, *rootSignature)
		}
	}

	void OpenGLRhi::setComputePipelineState(Rhi::IComputePipelineState* computePipelineState)
	{
		if (mComputePipelineState != computePipelineState)
		{
			if (nullptr != computePipelineState)
			{
				// Sanity check
				RHI_MATCH_CHECK(*this, *computePipelineState)

				// Set new compute pipeline state and add a reference to it
				if (nullptr != mComputePipelineState)
				{
					mComputePipelineState->releaseReference();
				}
				mComputePipelineState = static_cast<ComputePipelineState*>(computePipelineState);
				mComputePipelineState->addReference();

				// Set OpenGL compute pipeline state
				setOpenGLComputePipelineState(mComputePipelineState);
			}
			else if (nullptr != mComputePipelineState)
			{
				// TODO(co) Handle this situation by resetting OpenGL states?
				mComputePipelineState->releaseReference();
				mComputePipelineState = nullptr;
			}
		}

		// Set compute pipeline state
		else if (nullptr != mComputePipelineState)
		{
			// Set OpenGL compute pipeline state
			// -> This is necessary since OpenGL is using just a single current program, for graphics as well as compute
			setOpenGLComputePipelineState(mComputePipelineState);
		}
	}

	void OpenGLRhi::setComputeResourceGroup(uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup)
	{
		// Security checks
		#if SE_DEBUG
		{
			if (nullptr == mComputeRootSignature)
			{
				RHI_LOG(CRITICAL, "No OpenGL RHI implementation compute root signature set")
				return;
			}
			const Rhi::RootSignature& rootSignature = mComputeRootSignature->getRootSignature();
			if (rootParameterIndex >= rootSignature.numberOfParameters)
			{
				RHI_LOG(CRITICAL, "The OpenGL RHI implementation root parameter index is out of bounds")
				return;
			}
			const Rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			if (Rhi::RootParameterType::DESCRIPTOR_TABLE != rootParameter.parameterType)
			{
				RHI_LOG(CRITICAL, "The OpenGL RHI implementation root parameter index doesn't reference a descriptor table")
				return;
			}
			if (nullptr == reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges))
			{
				RHI_LOG(CRITICAL, "The OpenGL RHI implementation descriptor ranges is a null pointer")
				return;
			}
		}
		#endif

		// Set compute resource group
		setResourceGroup(*mComputeRootSignature, rootParameterIndex, resourceGroup);
	}

	void OpenGLRhi::dispatchCompute(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		// "GL_ARB_compute_shader"-extension required
		if (mExtensions->isGL_ARB_compute_shader())
		{
			glDispatchCompute(groupCountX, groupCountY, groupCountZ);

			// TODO(co) Compute shader: Memory barrier currently fixed build in: Make sure writing to image has finished before read
			glMemoryBarrierEXT(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			glMemoryBarrierEXT(GL_SHADER_STORAGE_BARRIER_BIT);
		}
	}


	//[-------------------------------------------------------]
	//[ Resource                                              ]
	//[-------------------------------------------------------]
	void OpenGLRhi::resolveMultisampleFramebuffer(Rhi::IRenderTarget& destinationRenderTarget, Rhi::IFramebuffer& sourceMultisampleFramebuffer)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, destinationRenderTarget)
		RHI_MATCH_CHECK(*this, sourceMultisampleFramebuffer)

		// Evaluate the render target type
		switch (destinationRenderTarget.getResourceType())
		{
			case Rhi::ResourceType::SWAP_CHAIN:
			{
				// Get the OpenGL swap chain instance
				// TODO(co) Implement me, not that important in practice so not directly implemented
				// SwapChain& swapChain = static_cast<SwapChain&>(destinationRenderTarget);
				break;
			}

			case Rhi::ResourceType::FRAMEBUFFER:
			{
				// Get the OpenGL framebuffer instances
				const Framebuffer& openGLDestinationFramebuffer = static_cast<const Framebuffer&>(destinationRenderTarget);
				const Framebuffer& openGLSourceMultisampleFramebuffer = static_cast<const Framebuffer&>(sourceMultisampleFramebuffer);

				// Get the width and height of the destination and source framebuffer
				uint32_t destinationWidth = 1;
				uint32_t destinationHeight = 1;
				openGLDestinationFramebuffer.getWidthAndHeight(destinationWidth, destinationHeight);
				uint32_t sourceWidth = 1;
				uint32_t sourceHeight = 1;
				openGLSourceMultisampleFramebuffer.getWidthAndHeight(sourceWidth, sourceHeight);

				// Resolve multisample
				glBindFramebuffer(GL_READ_FRAMEBUFFER, openGLSourceMultisampleFramebuffer.getOpenGLFramebuffer());
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, openGLDestinationFramebuffer.getOpenGLFramebuffer());
				glBlitFramebuffer(
					0, 0, static_cast<GLint>(sourceWidth), static_cast<GLint>(sourceHeight),			// Source
					0, 0, static_cast<GLint>(destinationWidth), static_cast<GLint>(destinationHeight),	// Destination
					GL_COLOR_BUFFER_BIT, GL_NEAREST);
				break;
			}

			case Rhi::ResourceType::ROOT_SIGNATURE:
			case Rhi::ResourceType::RESOURCE_GROUP:
			case Rhi::ResourceType::GRAPHICS_PROGRAM:
			case Rhi::ResourceType::VERTEX_ARRAY:
			case Rhi::ResourceType::RENDER_PASS:
			case Rhi::ResourceType::QUERY_POOL:
			case Rhi::ResourceType::VERTEX_BUFFER:
			case Rhi::ResourceType::INDEX_BUFFER:
			case Rhi::ResourceType::TEXTURE_BUFFER:
			case Rhi::ResourceType::STRUCTURED_BUFFER:
			case Rhi::ResourceType::INDIRECT_BUFFER:
			case Rhi::ResourceType::UNIFORM_BUFFER:
			case Rhi::ResourceType::TEXTURE_1D:
			case Rhi::ResourceType::TEXTURE_1D_ARRAY:
			case Rhi::ResourceType::TEXTURE_2D:
			case Rhi::ResourceType::TEXTURE_2D_ARRAY:
			case Rhi::ResourceType::TEXTURE_3D:
			case Rhi::ResourceType::TEXTURE_CUBE:
			case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
			case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
			case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
			case Rhi::ResourceType::SAMPLER_STATE:
			case Rhi::ResourceType::VERTEX_SHADER:
			case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
			case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
			case Rhi::ResourceType::GEOMETRY_SHADER:
			case Rhi::ResourceType::FRAGMENT_SHADER:
			case Rhi::ResourceType::TASK_SHADER:
			case Rhi::ResourceType::MESH_SHADER:
			case Rhi::ResourceType::COMPUTE_SHADER:
			default:
				// Not handled in here
				break;
		}
	}

	void OpenGLRhi::copyResource(Rhi::IResource& destinationResource, Rhi::IResource& sourceResource)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, destinationResource)
		RHI_MATCH_CHECK(*this, sourceResource)

		// Evaluate the render target type
		switch (destinationResource.getResourceType())
		{
			case Rhi::ResourceType::TEXTURE_2D:
				if (sourceResource.getResourceType() == Rhi::ResourceType::TEXTURE_2D)
				{
					// Get the OpenGL texture 2D instances
					const Texture2D& openGlDestinationTexture2D = static_cast<const Texture2D&>(destinationResource);
					const Texture2D& openGlSourceTexture2D = static_cast<const Texture2D&>(sourceResource);
					RHI_ASSERT(openGlDestinationTexture2D.getWidth() == openGlSourceTexture2D.getWidth(), "OpenGL source and destination width must be identical for resource copy")
					RHI_ASSERT(openGlDestinationTexture2D.getHeight() == openGlSourceTexture2D.getHeight(), "OpenGL source and destination height must be identical for resource copy")

					// Copy resource, but only the top-level mipmap
					const GLsizei width = static_cast<GLsizei>(openGlDestinationTexture2D.getWidth());
					const GLsizei height = static_cast<GLsizei>(openGlDestinationTexture2D.getHeight());
					if (mExtensions->isGL_ARB_copy_image())
					{
						glCopyImageSubData(openGlSourceTexture2D.getOpenGLTexture(), GL_TEXTURE_2D, 0, 0, 0, 0,
										   openGlDestinationTexture2D.getOpenGLTexture(), GL_TEXTURE_2D, 0, 0, 0, 0,
										   width, height, 1);
					}
					else
					{
						#if SE_OPENGL_STATE_CLEANUP
							// Backup the currently bound OpenGL framebuffer
							GLint openGLFramebufferBackup = 0;
							glGetIntegerv(GL_FRAMEBUFFER_BINDING, &openGLFramebufferBackup);
						#endif

						// Copy resource by using a framebuffer, but only the top-level mipmap
						if (0 == mOpenGLCopyResourceFramebuffer)
						{
							glGenFramebuffers(1, &mOpenGLCopyResourceFramebuffer);
						}
						glBindFramebuffer(GL_FRAMEBUFFER, mOpenGLCopyResourceFramebuffer);
						glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, openGlSourceTexture2D.getOpenGLTexture(), 0);
						glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, openGlDestinationTexture2D.getOpenGLTexture(), 0);
						static constexpr GLenum OPENGL_DRAW_BUFFER[1] =
						{
							GL_COLOR_ATTACHMENT1
						};
						glDrawBuffersARB(1, OPENGL_DRAW_BUFFER);	// We could use "glDrawBuffer(GL_COLOR_ATTACHMENT1);", but then we would also have to get the "glDrawBuffer()" function pointer, avoid OpenGL function overkill
						glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

						#if SE_OPENGL_STATE_CLEANUP
							// Be polite and restore the previous bound OpenGL framebuffer
							glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(openGLFramebufferBackup));
						#endif
					}
				}
				else
				{
					// Error!
					RHI_ASSERT(false, "Failed to copy OpenGL resource")
				}
				break;

			case Rhi::ResourceType::ROOT_SIGNATURE:
			case Rhi::ResourceType::RESOURCE_GROUP:
			case Rhi::ResourceType::GRAPHICS_PROGRAM:
			case Rhi::ResourceType::VERTEX_ARRAY:
			case Rhi::ResourceType::RENDER_PASS:
			case Rhi::ResourceType::QUERY_POOL:
			case Rhi::ResourceType::SWAP_CHAIN:
			case Rhi::ResourceType::FRAMEBUFFER:
			case Rhi::ResourceType::VERTEX_BUFFER:
			case Rhi::ResourceType::INDEX_BUFFER:
			case Rhi::ResourceType::TEXTURE_BUFFER:
			case Rhi::ResourceType::STRUCTURED_BUFFER:
			case Rhi::ResourceType::INDIRECT_BUFFER:
			case Rhi::ResourceType::UNIFORM_BUFFER:
			case Rhi::ResourceType::TEXTURE_1D:
			case Rhi::ResourceType::TEXTURE_1D_ARRAY:
			case Rhi::ResourceType::TEXTURE_2D_ARRAY:
			case Rhi::ResourceType::TEXTURE_3D:
			case Rhi::ResourceType::TEXTURE_CUBE:
			case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
			case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
			case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
			case Rhi::ResourceType::SAMPLER_STATE:
			case Rhi::ResourceType::VERTEX_SHADER:
			case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
			case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
			case Rhi::ResourceType::GEOMETRY_SHADER:
			case Rhi::ResourceType::FRAGMENT_SHADER:
			case Rhi::ResourceType::TASK_SHADER:
			case Rhi::ResourceType::MESH_SHADER:
			case Rhi::ResourceType::COMPUTE_SHADER:
			default:
				// Not handled in here
				break;
		}
	}

	void OpenGLRhi::generateMipmaps(Rhi::IResource& resource)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, resource)
		RHI_ASSERT(resource.getResourceType() == Rhi::ResourceType::TEXTURE_2D, "TODO(co) Mipmaps can only be generated for OpenGL 2D texture resources")

		Texture2D& texture2D = static_cast<Texture2D&>(resource);

		// Is "GL_EXT_direct_state_access" there?
		if (mExtensions->isGL_ARB_direct_state_access())
		{
			// Effective direct state access (DSA)
			glGenerateTextureMipmap(texture2D.getOpenGLTexture());
		}
		else if (mExtensions->isGL_EXT_direct_state_access())
		{
			// Effective direct state access (DSA)
			glGenerateTextureMipmapEXT(texture2D.getOpenGLTexture(), GL_TEXTURE_2D);
		}
		else
		{
			// Traditional bind version

			#if SE_OPENGL_STATE_CLEANUP
				// Backup the currently bound OpenGL texture
				// TODO(co) It's possible to avoid calling this multiple times
				GLint openGLTextureBackup = 0;
				glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLTextureBackup);
			#endif

			// Generate mipmaps
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, texture2D.getOpenGLTexture());
			glGenerateMipmap(GL_TEXTURE_2D);

			#if SE_OPENGL_STATE_CLEANUP
				// Be polite and restore the previous bound OpenGL texture
				glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLTextureBackup));
			#endif
		}
	}


	//[-------------------------------------------------------]
	//[ Query                                                 ]
	//[-------------------------------------------------------]
	void OpenGLRhi::resetQueryPool([[maybe_unused]] Rhi::IQueryPool& queryPool, [[maybe_unused]] uint32_t firstQueryIndex, [[maybe_unused]] uint32_t numberOfQueries)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, queryPool)
		RHI_ASSERT(firstQueryIndex < static_cast<const QueryPool&>(queryPool).getNumberOfQueries(), "OpenGL out-of-bounds query index")
		RHI_ASSERT((firstQueryIndex + numberOfQueries) <= static_cast<const QueryPool&>(queryPool).getNumberOfQueries(), "OpenGL out-of-bounds query index")

		// Nothing to do in here for OpenGL
	}

	void OpenGLRhi::beginQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex, uint32_t)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Query pool type dependent processing
		const QueryPool& openGLQueryPool = static_cast<const QueryPool&>(queryPool);
		RHI_ASSERT(queryIndex < openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
		switch (openGLQueryPool.getQueryType())
		{
			case Rhi::QueryType::OCCLUSION:
				// At this point in time we know that the "GL_ARB_occlusion_query"-extension is supported
				glBeginQueryARB(GL_SAMPLES_PASSED_ARB, static_cast<const OcclusionTimestampQueryPool&>(openGLQueryPool).getOpenGLQueries()[queryIndex]);
				break;

			case Rhi::QueryType::PIPELINE_STATISTICS:
				// At this point in time we know that the "GL_ARB_pipeline_statistics_query"-extension is supported
				static_cast<const PipelineStatisticsQueryPool&>(openGLQueryPool).beginQuery(queryIndex);
				break;

			case Rhi::QueryType::TIMESTAMP:
				RHI_ASSERT(false, "OpenGL begin query isn't allowed for timestamp queries, use \"Rhi::Command::WriteTimestampQuery\" instead")
				break;
		}
	}

	void OpenGLRhi::endQuery(Rhi::IQueryPool& queryPool, [[maybe_unused]] uint32_t queryIndex)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Query pool type dependent processing
		const QueryPool& openGLQueryPool = static_cast<const QueryPool&>(queryPool);
		RHI_ASSERT(queryIndex < openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
		switch (openGLQueryPool.getQueryType())
		{
			case Rhi::QueryType::OCCLUSION:
				// At this point in time we know that the "GL_ARB_occlusion_query"-extension is supported
				glEndQueryARB(GL_SAMPLES_PASSED_ARB);
				break;

			case Rhi::QueryType::PIPELINE_STATISTICS:
				// At this point in time we know that the "GL_ARB_pipeline_statistics_query"-extension is supported
				static_cast<const PipelineStatisticsQueryPool&>(openGLQueryPool).endQuery();
				break;

			case Rhi::QueryType::TIMESTAMP:
				RHI_ASSERT(false, "OpenGL end query isn't allowed for timestamp queries, use \"Rhi::Command::WriteTimestampQuery\" instead")
				break;
		}
	}

	void OpenGLRhi::writeTimestampQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Query pool type dependent processing
		const QueryPool& openGLQueryPool = static_cast<const QueryPool&>(queryPool);
		RHI_ASSERT(queryIndex < openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
		switch (openGLQueryPool.getQueryType())
		{
			case Rhi::QueryType::OCCLUSION:
				RHI_ASSERT(false, "OpenGL write timestamp query isn't allowed for occlusion queries, use \"Rhi::Command::BeginQuery\" and \"Rhi::Command::EndQuery\" instead")
				break;

			case Rhi::QueryType::PIPELINE_STATISTICS:
				RHI_ASSERT(false, "OpenGL write timestamp query isn't allowed for pipeline statistics queries, use \"Rhi::Command::BeginQuery\" and \"Rhi::Command::EndQuery\" instead")
				break;

			case Rhi::QueryType::TIMESTAMP:
				// At this point in time we know that the "GL_ARB_timer_query"-extension is supported
				glQueryCounter(static_cast<const OcclusionTimestampQueryPool&>(openGLQueryPool).getOpenGLQueries()[queryIndex], GL_TIMESTAMP);
				break;
		}
	}


	//[-------------------------------------------------------]
	//[ Debug                                                 ]
	//[-------------------------------------------------------]
	#if SE_DEBUG
		void OpenGLRhi::setDebugMarker(const char* name)
		{
			// "GL_KHR_debug"-extension required
			if (mExtensions->isGL_KHR_debug())
			{
				RHI_ASSERT(nullptr != name, "OpenGL debug marker names must not be a null pointer")
				glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1, GL_DEBUG_SEVERITY_NOTIFICATION, -1, name);
			}
		}

		void OpenGLRhi::beginDebugEvent(const char* name)
		{
			// "GL_KHR_debug"-extension required
			if (mExtensions->isGL_KHR_debug())
			{
				RHI_ASSERT(nullptr != name, "OpenGL debug event names must not be a null pointer")
				glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, name);
			}
		}

		void OpenGLRhi::endDebugEvent()
		{
			// "GL_KHR_debug"-extension required
			if (mExtensions->isGL_KHR_debug())
			{
				glPopDebugGroup();
			}
		}
	#endif


	//[-------------------------------------------------------]
	//[ Public virtual Rhi::IRhi methods                      ]
	//[-------------------------------------------------------]
	const char* OpenGLRhi::getName() const
	{
		return "OpenGL";
	}

	bool OpenGLRhi::isInitialized() const
	{
		// Is the OpenGL context initialized?
		return (nullptr != mOpenGLContext && mOpenGLContext->isInitialized());
	}

	bool OpenGLRhi::isDebugEnabled()
	{
		// OpenGL has nothing that is similar to the Direct3D 9 PIX functions (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)

		// Debug disabled
		return false;
	}


	//[-------------------------------------------------------]
	//[ Shader language                                       ]
	//[-------------------------------------------------------]
	uint32_t OpenGLRhi::getNumberOfShaderLanguages() const
	{
		uint32_t numberOfShaderLanguages = 0;

		// "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
		if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects())
		{
			// GLSL supported
			++numberOfShaderLanguages;
		}

		// Done, return the number of supported shader languages
		return numberOfShaderLanguages;
	}

	const char* OpenGLRhi::getShaderLanguageName(uint32_t index) const
	{
		RHI_ASSERT(index < getNumberOfShaderLanguages(), "OpenGL: Shader language index is out-of-bounds")

		// "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
		if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects())
		{
			// GLSL supported
			if (0 == index)
			{
				return ::detail::GLSL_NAME;
			}
		}

		// Error!
		return nullptr;
	}

	Rhi::IShaderLanguage* OpenGLRhi::getShaderLanguage(const char* shaderLanguageName)
	{
		// "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
		if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects())
		{
			// In case "shaderLanguage" is a null pointer, use the default shader language
			if (nullptr != shaderLanguageName)
			{
				// Optimization: Check for shader language name pointer match, first
				if (::detail::GLSL_NAME == shaderLanguageName || !stricmp(shaderLanguageName, ::detail::GLSL_NAME))
				{
					// Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
					if (mExtensions->isGL_ARB_separate_shader_objects())
					{
						// If required, create the separate shader language instance right now
						if (nullptr == mShaderLanguage)
						{
							mShaderLanguage = RHI_NEW(ShaderLanguageSeparate)(*this);
							mShaderLanguage->addReference();	// Internal RHI reference
						}

						// Return the shader language instance
						return mShaderLanguage;
					}
					else if (mExtensions->isGL_ARB_shader_objects())
					{
						// If required, create the monolithic shader language instance right now
						if (nullptr == mShaderLanguage)
						{
							mShaderLanguage = RHI_NEW(ShaderLanguageMonolithic)(*this);
							mShaderLanguage->addReference();	// Internal RHI reference
						}

						// Return the shader language instance
						return mShaderLanguage;
					}
				}
			}
			else
			{
				// Return the shader language instance as default
				return getShaderLanguage(::detail::GLSL_NAME);
			}
		}

		// Error!
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Resource creation                                     ]
	//[-------------------------------------------------------]
	Rhi::IRenderPass* OpenGLRhi::createRenderPass(uint32_t numberOfColorAttachments, const Rhi::TextureFormat::Enum* colorAttachmentTextureFormats, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		return RHI_NEW(RenderPass)(*this, numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IQueryPool* OpenGLRhi::createQueryPool(Rhi::QueryType queryType, uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		RHI_ASSERT(numberOfQueries > 0, "OpenGL: Number of queries mustn't be zero")
		switch (queryType)
		{
			case Rhi::QueryType::OCCLUSION:
				if (!mExtensions->isGL_ARB_occlusion_query())
				{
					RHI_LOG(CRITICAL, "OpenGL extension \"GL_ARB_occlusion_query\" isn't supported")
					return nullptr;
				}
				return RHI_NEW(OcclusionTimestampQueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);

			case Rhi::QueryType::PIPELINE_STATISTICS:
				if (!mExtensions->isGL_ARB_pipeline_statistics_query())
				{
					RHI_LOG(CRITICAL, "OpenGL extension \"GL_ARB_pipeline_statistics_query\" isn't supported")
					return nullptr;
				}
				return RHI_NEW(PipelineStatisticsQueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);

			case Rhi::QueryType::TIMESTAMP:
				if (!mExtensions->isGL_ARB_timer_query())
				{
					RHI_LOG(CRITICAL, "OpenGL extension \"GL_ARB_timer_query\" isn't supported")
					return nullptr;
				}
				return RHI_NEW(OcclusionTimestampQueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}
		return nullptr;
	}

	Rhi::ISwapChain* OpenGLRhi::createSwapChain(Rhi::IRenderPass& renderPass, Rhi::WindowHandle windowHandle, bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, renderPass)
		RHI_ASSERT(SE_NULL_HANDLE != windowHandle.nativeWindowHandle || nullptr != windowHandle.renderWindow, "OpenGL: The provided native window handle or render window must not be a null handle / null pointer")

		// Create the swap chain
		return RHI_NEW(SwapChain)(renderPass, windowHandle, useExternalContext RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IFramebuffer* OpenGLRhi::createFramebuffer(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, renderPass)

		// "GL_ARB_framebuffer_object" required
		if (mExtensions->isGL_ARB_framebuffer_object())
		{
			// Is "GL_EXT_direct_state_access" there?
			if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
			{
				// Effective direct state access (DSA)
				// -> Validation is done inside the framebuffer implementation
				return RHI_NEW(FramebufferDsa)(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Traditional bind version
				// -> Validation is done inside the framebuffer implementation
				return RHI_NEW(FramebufferBind)(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
		}
		else
		{
			// Error!
			return nullptr;
		}
	}

	Rhi::IBufferManager* OpenGLRhi::createBufferManager()
	{
		return RHI_NEW(BufferManager)(*this);
	}

	Rhi::ITextureManager* OpenGLRhi::createTextureManager()
	{
		return RHI_NEW(TextureManager)(*this);
	}

	Rhi::IRootSignature* OpenGLRhi::createRootSignature(const Rhi::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		return RHI_NEW(RootSignature)(*this, rootSignature RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IGraphicsPipelineState* OpenGLRhi::createGraphicsPipelineState(const Rhi::GraphicsPipelineState& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != graphicsPipelineState.rootSignature, "OpenGL: Invalid graphics pipeline state root signature")
		RHI_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "OpenGL: Invalid graphics pipeline state graphics program")
		RHI_ASSERT(nullptr != graphicsPipelineState.renderPass, "OpenGL: Invalid graphics pipeline state render pass")

		// Create graphics pipeline state
		uint16_t id = 0;
		if (GraphicsPipelineStateMakeId.CreateID(id))
		{
			return RHI_NEW(GraphicsPipelineState)(*this, graphicsPipelineState, id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		// Error: Ensure a correct reference counter behaviour
		graphicsPipelineState.rootSignature->addReference();
		graphicsPipelineState.rootSignature->releaseReference();
		graphicsPipelineState.graphicsProgram->addReference();
		graphicsPipelineState.graphicsProgram->releaseReference();
		graphicsPipelineState.renderPass->addReference();
		graphicsPipelineState.renderPass->releaseReference();
		return nullptr;
	}

	Rhi::IComputePipelineState* OpenGLRhi::createComputePipelineState(Rhi::IRootSignature& rootSignature, Rhi::IComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, rootSignature)
		RHI_MATCH_CHECK(*this, computeShader)

		// Create the compute pipeline state
		uint16_t id = 0;
		if ((mExtensions->isGL_ARB_separate_shader_objects() || mExtensions->isGL_ARB_shader_objects()) && ComputePipelineStateMakeId.CreateID(id))
		{
			// Create the compute pipeline state
			// -> Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
			if (mExtensions->isGL_ARB_separate_shader_objects())
			{
				return RHI_NEW(ComputePipelineStateSeparate)(*this, rootSignature, static_cast<ComputeShaderSeparate&>(computeShader), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else if (mExtensions->isGL_ARB_shader_objects())
			{
				return RHI_NEW(ComputePipelineStateMonolithic)(*this, rootSignature, static_cast<ComputeShaderMonolithic&>(computeShader), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
		}

		// Error: Ensure a correct reference counter behaviour
		rootSignature.addReference();
		rootSignature.releaseReference();
		computeShader.addReference();
		computeShader.releaseReference();
		return nullptr;
	}

	Rhi::ISamplerState* OpenGLRhi::createSamplerState(const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Is "GL_ARB_sampler_objects" there?
		if (mExtensions->isGL_ARB_sampler_objects())
		{
			// Effective sampler object (SO)
			return RHI_NEW(SamplerStateSo)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		// Is "GL_EXT_direct_state_access" there?
		else if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
		{
			// Direct state access (DSA) version to emulate a sampler object
			return RHI_NEW(SamplerStateDsa)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}
		else
		{
			// Traditional bind version to emulate a sampler object
			return RHI_NEW(SamplerStateBind)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}
	}


	//[-------------------------------------------------------]
	//[ Resource handling                                     ]
	//[-------------------------------------------------------]
	bool OpenGLRhi::map(Rhi::IResource& resource, uint32_t, Rhi::MapType mapType, uint32_t, Rhi::MappedSubresource& mappedSubresource)
	{
		// Evaluate the resource type
		switch (resource.getResourceType())
		{
			case Rhi::ResourceType::VERTEX_BUFFER:
				return ::detail::mapBuffer(*mExtensions, GL_ARRAY_BUFFER_ARB, GL_ARRAY_BUFFER_BINDING_ARB, static_cast<VertexBuffer&>(resource).getOpenGLArrayBuffer(), mapType, mappedSubresource);

			case Rhi::ResourceType::INDEX_BUFFER:
				return ::detail::mapBuffer(*mExtensions, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, static_cast<IndexBuffer&>(resource).getOpenGLElementArrayBuffer(), mapType, mappedSubresource);

			case Rhi::ResourceType::TEXTURE_BUFFER:
				return ::detail::mapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB, static_cast<TextureBuffer&>(resource).getOpenGLTextureBuffer(), mapType, mappedSubresource);

			case Rhi::ResourceType::STRUCTURED_BUFFER:
				return ::detail::mapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB, static_cast<StructuredBuffer&>(resource).getOpenGLStructuredBuffer(), mapType, mappedSubresource);

			case Rhi::ResourceType::INDIRECT_BUFFER:
				return ::detail::mapBuffer(*mExtensions, GL_DRAW_INDIRECT_BUFFER, GL_DRAW_INDIRECT_BUFFER_BINDING, static_cast<IndirectBuffer&>(resource).getOpenGLIndirectBuffer(), mapType, mappedSubresource);

			case Rhi::ResourceType::UNIFORM_BUFFER:
				return ::detail::mapBuffer(*mExtensions, GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING, static_cast<UniformBuffer&>(resource).getOpenGLUniformBuffer(), mapType, mappedSubresource);

			case Rhi::ResourceType::TEXTURE_1D:
			{
				// TODO(co) Implement me
				return false;
			}

			case Rhi::ResourceType::TEXTURE_1D_ARRAY:
			{
				// TODO(co) Implement me
				return false;
			}

			case Rhi::ResourceType::TEXTURE_2D:
			{
				bool result = false;

				// TODO(co) Implement me
				/*
				// Begin debug event
				RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

				// Get the Direct3D 11 resource instance
				ID3D11Resource* d3d11Resource = nullptr;
				static_cast<Texture2D&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
				if (nullptr != d3d11Resource)
				{
					// Map the Direct3D 11 resource
					result = (S_OK == mD3D11DeviceContext->Map(d3d11Resource, subresource, static_cast<D3D11_MAP>(mapType), mapFlags, reinterpret_cast<D3D11_MAPPED_SUBRESOURCE*>(&mappedSubresource)));

					// Release the Direct3D 11 resource instance
					d3d11Resource->Release();
				}

				// End debug event
				RHI_END_DEBUG_EVENT(this)
				*/

				// Done
				return result;
			}

			case Rhi::ResourceType::TEXTURE_2D_ARRAY:
			{
				bool result = false;

				// TODO(co) Implement me
				/*
				// Begin debug event
				RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

				// Get the Direct3D 11 resource instance
				ID3D11Resource* d3d11Resource = nullptr;
				static_cast<Texture2DArray&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
				if (nullptr != d3d11Resource)
				{
					// Map the Direct3D 11 resource
					result = (S_OK == mD3D11DeviceContext->Map(d3d11Resource, subresource, static_cast<D3D11_MAP>(mapType), mapFlags, reinterpret_cast<D3D11_MAPPED_SUBRESOURCE*>(&mappedSubresource)));

					// Release the Direct3D 11 resource instance
					d3d11Resource->Release();
				}

				// End debug event
				RHI_END_DEBUG_EVENT(this)
				*/

				// Done
				return result;
			}

			case Rhi::ResourceType::TEXTURE_3D:
				return ::detail::mapBuffer(*mExtensions, GL_PIXEL_UNPACK_BUFFER_ARB, GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, static_cast<Texture3D&>(resource).getOpenGLPixelUnpackBuffer(), mapType, mappedSubresource);

			case Rhi::ResourceType::TEXTURE_CUBE:
			{
				// TODO(co) Implement me
				return false;
			}

			case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
			{
				// TODO(co) Implement me
				return false;
			}

			case Rhi::ResourceType::ROOT_SIGNATURE:
			case Rhi::ResourceType::RESOURCE_GROUP:
			case Rhi::ResourceType::GRAPHICS_PROGRAM:
			case Rhi::ResourceType::VERTEX_ARRAY:
			case Rhi::ResourceType::RENDER_PASS:
			case Rhi::ResourceType::QUERY_POOL:
			case Rhi::ResourceType::SWAP_CHAIN:
			case Rhi::ResourceType::FRAMEBUFFER:
			case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
			case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
			case Rhi::ResourceType::SAMPLER_STATE:
			case Rhi::ResourceType::VERTEX_SHADER:
			case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
			case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
			case Rhi::ResourceType::GEOMETRY_SHADER:
			case Rhi::ResourceType::FRAGMENT_SHADER:
			case Rhi::ResourceType::TASK_SHADER:
			case Rhi::ResourceType::MESH_SHADER:
			case Rhi::ResourceType::COMPUTE_SHADER:
			default:
				// Nothing we can map, set known return values
				mappedSubresource.data		 = nullptr;
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;

				// Error!
				return false;
		}
	}

	void OpenGLRhi::unmap(Rhi::IResource& resource, uint32_t)
	{
		// Evaluate the resource type
		switch (resource.getResourceType())
		{
			case Rhi::ResourceType::VERTEX_BUFFER:
				::detail::unmapBuffer(*mExtensions, GL_ARRAY_BUFFER_ARB, GL_ARRAY_BUFFER_BINDING_ARB, static_cast<VertexBuffer&>(resource).getOpenGLArrayBuffer());
				break;

			case Rhi::ResourceType::INDEX_BUFFER:
				::detail::unmapBuffer(*mExtensions, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, static_cast<IndexBuffer&>(resource).getOpenGLElementArrayBuffer());
				break;

			case Rhi::ResourceType::TEXTURE_BUFFER:
				::detail::unmapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB, static_cast<TextureBuffer&>(resource).getOpenGLTextureBuffer());
				break;

			case Rhi::ResourceType::STRUCTURED_BUFFER:
				::detail::unmapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB, static_cast<StructuredBuffer&>(resource).getOpenGLStructuredBuffer());
				break;

			case Rhi::ResourceType::INDIRECT_BUFFER:
				::detail::unmapBuffer(*mExtensions, GL_DRAW_INDIRECT_BUFFER, GL_DRAW_INDIRECT_BUFFER_BINDING, static_cast<IndirectBuffer&>(resource).getOpenGLIndirectBuffer());
				break;

			case Rhi::ResourceType::UNIFORM_BUFFER:
				::detail::unmapBuffer(*mExtensions, GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING, static_cast<UniformBuffer&>(resource).getOpenGLUniformBuffer());
				break;

			case Rhi::ResourceType::TEXTURE_1D:
			{
				// TODO(co) Implement me
				break;
			}

			case Rhi::ResourceType::TEXTURE_1D_ARRAY:
			{
				// TODO(co) Implement me
				break;
			}

			case Rhi::ResourceType::TEXTURE_2D:
			{
				// TODO(co) Implement me
				/*
				// Get the Direct3D 11 resource instance
				ID3D11Resource* d3d11Resource = nullptr;
				static_cast<Texture2D&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
				if (nullptr != d3d11Resource)
				{
					// Unmap the Direct3D 11 resource
					mD3D11DeviceContext->Unmap(d3d11Resource, subresource);

					// Release the Direct3D 11 resource instance
					d3d11Resource->Release();
				}
				*/
				break;
			}

			case Rhi::ResourceType::TEXTURE_2D_ARRAY:
			{
				// TODO(co) Implement me
				/*
				// Get the Direct3D 11 resource instance
				ID3D11Resource* d3d11Resource = nullptr;
				static_cast<Texture2DArray&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
				if (nullptr != d3d11Resource)
				{
					// Unmap the Direct3D 11 resource
					mD3D11DeviceContext->Unmap(d3d11Resource, subresource);

					// Release the Direct3D 11 resource instance
					d3d11Resource->Release();
				}
				*/
				break;
			}

			case Rhi::ResourceType::TEXTURE_3D:
			{
				// Unmap pixel unpack buffer
				const Texture3D& texture3D = static_cast<Texture3D&>(resource);
				const Rhi::TextureFormat::Enum textureFormat = texture3D.getTextureFormat();
				const uint32_t openGLPixelUnpackBuffer = texture3D.getOpenGLPixelUnpackBuffer();
				::detail::unmapBuffer(*mExtensions, GL_PIXEL_UNPACK_BUFFER_ARB, GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, openGLPixelUnpackBuffer);

				// Backup the currently set alignment and currently bound OpenGL pixel unpack buffer
				#if SE_OPENGL_STATE_CLEANUP
					GLint openGLAlignmentBackup = 0;
					glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
					GLint openGLUnpackBufferBackup = 0;
					glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, &openGLUnpackBufferBackup);
				#endif
				glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::getNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

				// Copy pixel unpack buffer to texture
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, openGLPixelUnpackBuffer);
				if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
				{
					// Effective direct state access (DSA)
					if (mExtensions->isGL_ARB_direct_state_access())
					{
						glTextureSubImage3D(texture3D.getOpenGLTexture(), 0, 0, 0, 0, static_cast<GLsizei>(texture3D.getWidth()), static_cast<GLsizei>(texture3D.getHeight()), static_cast<GLsizei>(texture3D.getDepth()), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), 0);
					}
					else
					{
						glTextureSubImage3DEXT(texture3D.getOpenGLTexture(), GL_TEXTURE_3D, 0, 0, 0, 0, static_cast<GLsizei>(texture3D.getWidth()), static_cast<GLsizei>(texture3D.getHeight()), static_cast<GLsizei>(texture3D.getDepth()), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), 0);
					}
				}
				else
				{
					// Traditional bind version

					// Backup the currently bound OpenGL texture
					#if SE_OPENGL_STATE_CLEANUP
						GLint openGLTextureBackup = 0;
						glGetIntegerv(GL_TEXTURE_BINDING_3D, &openGLTextureBackup);
					#endif

					// Copy pixel unpack buffer to texture
					glBindTexture(GL_TEXTURE_3D, texture3D.getOpenGLTexture());
					glTexSubImage3DEXT(GL_TEXTURE_3D, 0, 0, 0, 0, static_cast<GLsizei>(texture3D.getWidth()), static_cast<GLsizei>(texture3D.getHeight()), static_cast<GLsizei>(texture3D.getDepth()), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), 0);

					// Be polite and restore the previous bound OpenGL texture
					#if SE_OPENGL_STATE_CLEANUP
						glBindTexture(GL_TEXTURE_3D, static_cast<GLuint>(openGLTextureBackup));
					#endif
				}

				// Restore previous alignment and pixel unpack buffer
				#if SE_OPENGL_STATE_CLEANUP
					glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
					glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, static_cast<GLuint>(openGLUnpackBufferBackup));
				#else
					glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
				#endif
				break;
			}

			case Rhi::ResourceType::TEXTURE_CUBE:
			{
				// TODO(co) Implement me
				break;
			}

			case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
			{
				// TODO(co) Implement me
				break;
			}

			case Rhi::ResourceType::ROOT_SIGNATURE:
			case Rhi::ResourceType::RESOURCE_GROUP:
			case Rhi::ResourceType::GRAPHICS_PROGRAM:
			case Rhi::ResourceType::VERTEX_ARRAY:
			case Rhi::ResourceType::RENDER_PASS:
			case Rhi::ResourceType::QUERY_POOL:
			case Rhi::ResourceType::SWAP_CHAIN:
			case Rhi::ResourceType::FRAMEBUFFER:
			case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
			case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
			case Rhi::ResourceType::SAMPLER_STATE:
			case Rhi::ResourceType::VERTEX_SHADER:
			case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
			case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
			case Rhi::ResourceType::GEOMETRY_SHADER:
			case Rhi::ResourceType::FRAGMENT_SHADER:
			case Rhi::ResourceType::TASK_SHADER:
			case Rhi::ResourceType::MESH_SHADER:
			case Rhi::ResourceType::COMPUTE_SHADER:
			default:
				// Nothing we can unmap
				break;
		}
	}

	bool OpenGLRhi::getQueryPoolResults(Rhi::IQueryPool& queryPool, [[maybe_unused]] uint32_t numberOfDataBytes, uint8_t* data, uint32_t firstQueryIndex, uint32_t numberOfQueries, uint32_t strideInBytes, uint32_t queryResultFlags)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, queryPool)
		RHI_ASSERT(numberOfDataBytes >= sizeof(UINT64), "OpenGL out-of-memory query access")
		RHI_ASSERT(1 == numberOfQueries || strideInBytes > 0, "OpenGL invalid stride in bytes")
		RHI_ASSERT(numberOfDataBytes >= strideInBytes * numberOfQueries, "OpenGL out-of-memory query access")
		RHI_ASSERT(nullptr != data, "OpenGL out-of-memory query access")
		RHI_ASSERT(numberOfQueries > 0, "OpenGL number of queries mustn't be zero")

		// Query pool type dependent processing
		bool resultAvailable = true;
		const QueryPool& openGLQueryPool = static_cast<const QueryPool&>(queryPool);
		RHI_ASSERT(firstQueryIndex < openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
		RHI_ASSERT((firstQueryIndex + numberOfQueries) <= openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
		const bool waitForResult = ((queryResultFlags & Rhi::QueryResultFlags::WAIT) != 0);
		switch (openGLQueryPool.getQueryType())
		{
			case Rhi::QueryType::OCCLUSION:
			case Rhi::QueryType::TIMESTAMP:	// OpenGL return the time in nanoseconds
			{
				uint8_t* currentData = data;
				const GLuint* openGLQueries = static_cast<const OcclusionTimestampQueryPool&>(openGLQueryPool).getOpenGLQueries();
				for (uint32_t i = 0; i  < numberOfQueries; ++i)
				{
					const GLuint openGLQuery = openGLQueries[firstQueryIndex + i];
					GLuint openGLQueryResult = GL_FALSE;
					do
					{
						glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_AVAILABLE_ARB, &openGLQueryResult);
					}
					while (waitForResult && GL_TRUE != openGLQueryResult);
					if (GL_TRUE == openGLQueryResult)
					{
						glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_ARB, &openGLQueryResult);
						*reinterpret_cast<uint64_t*>(currentData) = openGLQueryResult;
					}
					else
					{
						// Result not ready
						resultAvailable = false;
						break;
					}
					currentData += strideInBytes;
				}
				break;
			}

			case Rhi::QueryType::PIPELINE_STATISTICS:
				RHI_ASSERT(numberOfDataBytes >= sizeof(Rhi::PipelineStatisticsQueryResult), "OpenGL out-of-memory query access")
				RHI_ASSERT(1 == numberOfQueries || strideInBytes >= sizeof(Rhi::PipelineStatisticsQueryResult), "OpenGL out-of-memory query access")
				resultAvailable = static_cast<const PipelineStatisticsQueryPool&>(openGLQueryPool).getQueryPoolResults(data, firstQueryIndex, numberOfQueries, strideInBytes, waitForResult);
				break;
		}

		// Done
		return resultAvailable;
	}


	//[-------------------------------------------------------]
	//[ Operations                                            ]
	//[-------------------------------------------------------]
	bool OpenGLRhi::beginScene()
	{
		// Not required when using OpenGL

		// Sanity check
		#if SE_DEBUG
			RHI_ASSERT(false == mDebugBetweenBeginEndScene, "OpenGL: Begin scene was called while scene rendering is already in progress, missing end scene call?")
			mDebugBetweenBeginEndScene = true;
		#endif

		// Done
		return true;
	}

	void OpenGLRhi::submitCommandBuffer(const Rhi::CommandBuffer& commandBuffer)
	{
		// Sanity check
		RHI_ASSERT(!commandBuffer.isEmpty(), "The OpenGL command buffer to execute mustn't be empty")

		// Loop through all commands
		const uint8_t* commandPacketBuffer = commandBuffer.getCommandPacketBuffer();
		Rhi::ConstCommandPacket constCommandPacket = commandPacketBuffer;
		while (nullptr != constCommandPacket)
		{
			{ // Submit command packet
				const Rhi::CommandDispatchFunctionIndex commandDispatchFunctionIndex = Rhi::CommandPacketHelper::loadCommandDispatchFunctionIndex(constCommandPacket);
				const void* command = Rhi::CommandPacketHelper::loadCommand(constCommandPacket);
				detail::DISPATCH_FUNCTIONS[static_cast<uint32_t>(commandDispatchFunctionIndex)](command, *this);
			}

			{ // Next command
				const uint32_t nextCommandPacketByteIndex = Rhi::CommandPacketHelper::getNextCommandPacketByteIndex(constCommandPacket);
				constCommandPacket = (~0u != nextCommandPacketByteIndex) ? &commandPacketBuffer[nextCommandPacketByteIndex] : nullptr;
			}
		}
	}

	void OpenGLRhi::endScene()
	{
		// Sanity check
		#if SE_DEBUG
			RHI_ASSERT(true == mDebugBetweenBeginEndScene, "OpenGL: End scene was called while scene rendering isn't in progress, missing start scene call?")
			mDebugBetweenBeginEndScene = false;
		#endif

		// We need to forget about the currently set render target
		setGraphicsRenderTarget(nullptr);

		// We need to forget about the currently set vertex array
		unsetGraphicsVertexArray();
	}


	//[-------------------------------------------------------]
	//[ Synchronization                                       ]
	//[-------------------------------------------------------]
	void OpenGLRhi::flush()
	{
		glFlush();
	}

	void OpenGLRhi::finish()
	{
		glFinish();
	}


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	#if SE_DEBUG
		void OpenGLRhi::debugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int, const char* message, const void* userParam)
		{
			// Source to string
			char debugSource[20 + 1]{0};	// +1 for terminating zero
			switch (source)
			{
				case GL_DEBUG_SOURCE_API_ARB:
					strncpy(debugSource, "OpenGL", 20);
					break;

				case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
					strncpy(debugSource, "Windows", 20);
					break;

				case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
					strncpy(debugSource, "Shader compiler", 20);
					break;

				case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
					strncpy(debugSource, "Third party", 20);
					break;

				case GL_DEBUG_SOURCE_APPLICATION_ARB:
					strncpy(debugSource, "Application", 20);
					break;

				case GL_DEBUG_SOURCE_OTHER_ARB:
					strncpy(debugSource, "Other", 20);
					break;

				default:
					strncpy(debugSource, "?", 20);
					break;
			}

			// Debug type to string
			DefaultLog::Type logType = DefaultLog::Type::CRITICAL;
			char debugType[25 + 1]{0};	// +1 for terminating zero
			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR_ARB:
					strncpy(debugType, "Error", 25);
					break;

				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
					logType = DefaultLog::Type::COMPATIBILITY_WARNING;
					strncpy(debugType, "Deprecated behavior", 25);
					break;

				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
					strncpy(debugType, "Undefined behavior", 25);
					break;

				case GL_DEBUG_TYPE_PORTABILITY_ARB:
					logType = DefaultLog::Type::COMPATIBILITY_WARNING;
					strncpy(debugType, "Portability", 25);
					break;

				case GL_DEBUG_TYPE_PERFORMANCE_ARB:
					logType = DefaultLog::Type::PERFORMANCE_WARNING;
					strncpy(debugType, "Performance", 25);
					break;

				case GL_DEBUG_TYPE_OTHER_ARB:
					strncpy(debugType, "Other", 25);
					break;

				case GL_DEBUG_TYPE_MARKER:
					strncpy(debugType, "Marker", 25);
					break;

				case GL_DEBUG_TYPE_PUSH_GROUP:
					strncpy(debugType, "Push group", 25);
					break;

				case GL_DEBUG_TYPE_POP_GROUP:
					strncpy(debugType, "Pop group", 25);
					break;

				default:
					strncpy(debugType, "?", 25);
					break;
			}

			// Debug severity to string
			char debugSeverity[20 + 1]{0};	// +1 for terminating zero
			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH_ARB:
					strncpy(debugSeverity, "High", 20);
					break;

				case GL_DEBUG_SEVERITY_MEDIUM_ARB:
					strncpy(debugSeverity, "Medium", 20);
					break;

				case GL_DEBUG_SEVERITY_LOW_ARB:
					strncpy(debugSeverity, "Low", 20);
					break;

				case GL_DEBUG_SEVERITY_NOTIFICATION:
					strncpy(debugSeverity, "Notification", 20);
					break;

				default:
					strncpy(debugSeverity, "?", 20);
					break;
			}

			// Print into log
			if (GetLog().print(logType, nullptr, __FILE__, static_cast<uint32_t>(__LINE__), "OpenGL debug message\tSource:\"%s\"\tType:\"%s\"\tID:\"%u\"\tSeverity:\"%s\"\tMessage:\"%s\"", debugSource, debugType, id, debugSeverity, message))
			{
				SE_DEBUG_BREAK;
			}
		}
	#else
		void OpenGLRhi::debugMessageCallback(uint32_t, uint32_t, uint32_t, uint32_t, int, const char*, const void*)
		{
			// Nothing here
		}
	#endif


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void OpenGLRhi::initializeCapabilities()
	{
		GLint openGLValue = 0;

		{ // Get device name
		  // -> OpenGL 4.3 Compatibility Profile Specification, section 22.2, page 627: "String queries return pointers to UTF-8 encoded, null-terminated static strings describing properties of the current GL context."
		  // -> For example "AMD Radeon R9 200 Series"
			const size_t numberOfCharacters = Countof(mCapabilities.deviceName) - 1;
			strncpy(mCapabilities.deviceName, reinterpret_cast<const char*>(glGetString(GL_RENDERER)), numberOfCharacters);
			mCapabilities.deviceName[numberOfCharacters] = '\0';
		}

		// Preferred swap chain texture format
		mCapabilities.preferredSwapChainColorTextureFormat		  = Rhi::TextureFormat::Enum::R8G8B8A8;
		mCapabilities.preferredSwapChainDepthStencilTextureFormat = Rhi::TextureFormat::Enum::D32_FLOAT;

		// Maximum number of viewports (always at least 1)
		// TODO(co) "GL_ARB_viewport_array" support ("OpenGLRhi::setGraphicsViewports()")
		mCapabilities.maximumNumberOfViewports = 1;	// TODO(co) GL_ARB_viewport_array

		// Maximum number of simultaneous render targets (if <1 render to texture is not supported, "GL_ARB_draw_buffers" required)
		if (mExtensions->isGL_ARB_draw_buffers())
		{
			glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &openGLValue);
			mCapabilities.maximumNumberOfSimultaneousRenderTargets = static_cast<uint32_t>(openGLValue);
		}
		else
		{
			// "GL_ARB_framebuffer_object"-extension for render to texture required
			mCapabilities.maximumNumberOfSimultaneousRenderTargets = static_cast<uint32_t>(mExtensions->isGL_ARB_framebuffer_object());
		}

		// Maximum texture dimension
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &openGLValue);
		mCapabilities.maximumTextureDimension = static_cast<uint32_t>(openGLValue);

		// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
		// Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
		// Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
		if (mExtensions->isGL_EXT_texture_array())
		{
			glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, &openGLValue);
			mCapabilities.maximumNumberOf1DTextureArraySlices = static_cast<uint32_t>(openGLValue);
			mCapabilities.maximumNumberOf2DTextureArraySlices = static_cast<uint32_t>(openGLValue);
			mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;	// TODO(co) Implement me		 static_cast<uint32_t>(openGLValue);
		}
		else
		{
			mCapabilities.maximumNumberOf1DTextureArraySlices = 0;
			mCapabilities.maximumNumberOf2DTextureArraySlices = 0;
			mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;
		}

		// Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
		if (mExtensions->isGL_ARB_texture_buffer_object())
		{
			glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE_EXT, &openGLValue);
			mCapabilities.maximumTextureBufferSize = static_cast<uint32_t>(openGLValue);
		}
		else
		{
			mCapabilities.maximumTextureBufferSize = 0;
		}

		// Maximum structured buffer size in bytes
		if (mExtensions->isGL_ARB_shader_storage_buffer_object())
		{
			glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &openGLValue);
			mCapabilities.maximumStructuredBufferSize = static_cast<uint32_t>(openGLValue);
		}
		else
		{
			mCapabilities.maximumStructuredBufferSize = 0;
		}

		// Maximum indirect buffer size in bytes
		if (mExtensions->isGL_ARB_draw_indirect())
		{
			// TODO(co) How to get the indirect buffer maximum size? Didn't find any information about this.
			mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB
		}
		else
		{
			mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB
		}

		// Maximum uniform buffer (UBO) size in bytes (usually at least 4096 * 16 bytes, in case there's no support for uniform buffer it's 0)
		if (mExtensions->isGL_ARB_uniform_buffer_object())
		{
			glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &openGLValue);
			mCapabilities.maximumUniformBufferSize = static_cast<uint32_t>(openGLValue);
		}
		else
		{
			mCapabilities.maximumUniformBufferSize = 0;
		}

		// Maximum number of multisamples (always at least 1, usually 8)
		if (mExtensions->isGL_ARB_texture_multisample())
		{
			glGetIntegerv(GL_MAX_SAMPLES, &openGLValue);
			if (openGLValue > 8)
			{
				// Limit to known maximum we can test, even if e.g. GeForce 980m reports 32 here
				openGLValue = 8;
			}
			mCapabilities.maximumNumberOfMultisamples = static_cast<uint8_t>(openGLValue);
		}
		else
		{
			mCapabilities.maximumNumberOfMultisamples = 1;
		}

		// Maximum anisotropy (always at least 1, usually 16)
		// -> "GL_EXT_texture_filter_anisotropic"-extension
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &openGLValue);
		mCapabilities.maximumAnisotropy = static_cast<uint8_t>(openGLValue);

		// Coordinate system
		// -> If the "GL_ARB_clip_control"-extension is available: Left-handed coordinate system with clip space depth value range 0..1
		// -> If the "GL_ARB_clip_control"-extension isn't available: Right-handed coordinate system with clip space depth value range -1..1
		// -> For background theory see "Depth Precision Visualized" by Nathan Reed - https://developer.nvidia.com/content/depth-precision-visualized
		// -> For practical information see "Reversed-Z in OpenGL" by Nicolas Guillemot - https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
		// -> Shaders might want to take the following into account: "Mac computers that use OpenCL and OpenGL graphics" - https://support.apple.com/en-us/HT202823 - "iMac (Retina 5K, 27-inch, 2017)" - OpenGL 4.1
		mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = mExtensions->isGL_ARB_clip_control();

		// Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
		mCapabilities.individualUniforms = true;

		// Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex, "GL_ARB_instanced_arrays" required)
		mCapabilities.instancedArrays = mExtensions->isGL_ARB_instanced_arrays();

		// Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID, "GL_ARB_draw_instanced" required)
		mCapabilities.drawInstanced = mExtensions->isGL_ARB_draw_instanced();

		// Base vertex supported for draw calls?
		mCapabilities.baseVertex = mExtensions->isGL_ARB_draw_elements_base_vertex();

		// OpenGL has no native multithreading
		mCapabilities.nativeMultithreading = false;

		// We don't support the OpenGL program binaries since those are operation system and graphics driver version dependent, which renders them useless for pre-compiled shaders shipping
		mCapabilities.shaderBytecode = mExtensions->isGL_ARB_gl_spirv();

		// Is there support for vertex shaders (VS)?
		mCapabilities.vertexShader = mExtensions->isGL_ARB_vertex_shader();

		// Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
		if (mExtensions->isGL_ARB_tessellation_shader())
		{
			glGetIntegerv(GL_MAX_PATCH_VERTICES, &openGLValue);
			mCapabilities.maximumNumberOfPatchVertices = static_cast<uint32_t>(openGLValue);
		}
		else
		{
			mCapabilities.maximumNumberOfPatchVertices = 0;
		}

		// Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
		if (mExtensions->isGL_ARB_geometry_shader4())
		{
			glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB, &openGLValue);
			mCapabilities.maximumNumberOfGsOutputVertices = static_cast<uint32_t>(openGLValue);
		}
		else
		{
			mCapabilities.maximumNumberOfGsOutputVertices = 0;
		}

		// Is there support for fragment shaders (FS)?
		mCapabilities.fragmentShader = mExtensions->isGL_ARB_fragment_shader();

		// Is there support for task shaders (TS) and mesh shaders (MS)?
		mCapabilities.meshShader = mExtensions->isGL_NV_mesh_shader();

		// Is there support for compute shaders (CS)?
		mCapabilities.computeShader = mExtensions->isGL_ARB_compute_shader();
	}

	void OpenGLRhi::unsetGraphicsVertexArray()
	{
		// Release the currently used vertex array reference, in case we have one
		if (nullptr != mVertexArray)
		{
			// Evaluate the internal array type of the currently set vertex array
			switch (static_cast<VertexArray*>(mVertexArray)->getInternalResourceType())
			{
				case VertexArray::InternalResourceType::NO_VAO:
					// Disable OpenGL vertex attribute arrays
					static_cast<VertexArrayNoVao*>(mVertexArray)->disableOpenGLVertexAttribArrays();
					break;

				case VertexArray::InternalResourceType::VAO:
					// Unbind OpenGL vertex array
					// -> No need to check for "GL_ARB_vertex_array_object", in case were in here we know it must exist
					glBindVertexArray(mDefaultOpenGLVertexArray);
					break;
			}

			// Release reference
			mVertexArray->releaseReference();
			mVertexArray = nullptr;
		}
	}

	void OpenGLRhi::setResourceGroup(const RootSignature& rootSignature, uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup)
	{
		if (nullptr != resourceGroup)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *resourceGroup)

			// Set resource group
			const ResourceGroup* openGLResourceGroup = static_cast<ResourceGroup*>(resourceGroup);
			const uint32_t numberOfResources = openGLResourceGroup->getNumberOfResources();
			Rhi::IResource** resources = openGLResourceGroup->getResources();
			const Rhi::RootParameter& rootParameter = rootSignature.getRootSignature().parameters[rootParameterIndex];
			for (uint32_t resourceIndex = 0; resourceIndex < numberOfResources; ++resourceIndex, ++resources)
			{
				Rhi::IResource* resource = *resources;
				RHI_ASSERT(nullptr != reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
				const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[resourceIndex];

				// Check the type of resource to set
				// TODO(co) Some additional resource type root signature security checks in debug build?
				const Rhi::ResourceType resourceType = resource->getResourceType();
				switch (resourceType)
				{
					case Rhi::ResourceType::TEXTURE_BUFFER:
					case Rhi::ResourceType::TEXTURE_1D:
					case Rhi::ResourceType::TEXTURE_1D_ARRAY:
					case Rhi::ResourceType::TEXTURE_2D:
					case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					case Rhi::ResourceType::TEXTURE_3D:
					case Rhi::ResourceType::TEXTURE_CUBE:
					case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
					{
						// In OpenGL, all shaders share the same texture units (= "Rhi::RootParameter::shaderVisibility" stays unused)
						switch (descriptorRange.rangeType)
						{
							case Rhi::DescriptorRangeType::SRV:
							{
								// Is "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" there?
								if (mExtensions->isGL_ARB_direct_state_access() || mExtensions->isGL_EXT_direct_state_access())
								{
									// Effective direct state access (DSA)
									const bool isArbDsa = mExtensions->isGL_ARB_direct_state_access();

									// "glBindTextureUnit()" unit parameter is zero based so we can simply use the value we received
									const GLuint unit = descriptorRange.baseShaderRegister;

									// TODO(co) Some security checks might be wise *maximum number of texture units*
									// Evaluate the texture type
									switch (resourceType)
									{
										case Rhi::ResourceType::TEXTURE_BUFFER:
											if (isArbDsa)
											{
												glBindTextureUnit(unit, static_cast<TextureBuffer*>(resource)->getOpenGLTexture());
											}
											else
											{
												// "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_BUFFER_ARB, static_cast<TextureBuffer*>(resource)->getOpenGLTexture());
											}
											break;

										case Rhi::ResourceType::TEXTURE_1D:
											if (isArbDsa)
											{
												glBindTextureUnit(unit, static_cast<Texture1D*>(resource)->getOpenGLTexture());
											}
											else
											{
												// "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_1D, static_cast<Texture1D*>(resource)->getOpenGLTexture());
											}
											break;

										case Rhi::ResourceType::TEXTURE_1D_ARRAY:
											// No texture 1D array extension check required, if we in here we already know it must exist
											if (isArbDsa)
											{
												glBindTextureUnit(unit, static_cast<Texture1DArray*>(resource)->getOpenGLTexture());
											}
											else
											{
												// "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_1D_ARRAY_EXT, static_cast<Texture1DArray*>(resource)->getOpenGLTexture());
											}
											break;

										case Rhi::ResourceType::TEXTURE_2D:
											if (isArbDsa)
											{
												glBindTextureUnit(unit, static_cast<Texture2D*>(resource)->getOpenGLTexture());
											}
											else
											{
												// "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												const Texture2D* texture2D = static_cast<Texture2D*>(resource);
												glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->getOpenGLTexture());
											}
											break;

										case Rhi::ResourceType::TEXTURE_2D_ARRAY:
											// No texture 2D array extension check required, if we in here we already know it must exist
											if (isArbDsa)
											{
												glBindTextureUnit(unit, static_cast<Texture2DArray*>(resource)->getOpenGLTexture());
											}
											else
											{
												// "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_2D_ARRAY_EXT, static_cast<Texture2DArray*>(resource)->getOpenGLTexture());
											}
											break;

										case Rhi::ResourceType::TEXTURE_3D:
											if (isArbDsa)
											{
												glBindTextureUnit(unit, static_cast<Texture3D*>(resource)->getOpenGLTexture());
											}
											else
											{
												// "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_3D, static_cast<Texture3D*>(resource)->getOpenGLTexture());
											}
											break;

										case Rhi::ResourceType::TEXTURE_CUBE:
											if (isArbDsa)
											{
												glBindTextureUnit(unit, static_cast<TextureCube*>(resource)->getOpenGLTexture());
											}
											else
											{
												// "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_CUBE_MAP, static_cast<TextureCube*>(resource)->getOpenGLTexture());
											}
											break;

										case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
											// TODO(co) Implement me
											/*
											if (isArbDsa)
											{
												glBindTextureUnit(unit, static_cast<TextureCubeArray*>(resource)->getOpenGLTexture());
											}
											else
											{
												// "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_CUBE_MAP, static_cast<TextureCubeArray*>(resource)->getOpenGLTexture());
											}
											*/
											break;

										case Rhi::ResourceType::ROOT_SIGNATURE:
										case Rhi::ResourceType::RESOURCE_GROUP:
										case Rhi::ResourceType::GRAPHICS_PROGRAM:
										case Rhi::ResourceType::VERTEX_ARRAY:
										case Rhi::ResourceType::RENDER_PASS:
										case Rhi::ResourceType::QUERY_POOL:
										case Rhi::ResourceType::SWAP_CHAIN:
										case Rhi::ResourceType::FRAMEBUFFER:
										case Rhi::ResourceType::VERTEX_BUFFER:
										case Rhi::ResourceType::INDEX_BUFFER:
										case Rhi::ResourceType::STRUCTURED_BUFFER:
										case Rhi::ResourceType::INDIRECT_BUFFER:
										case Rhi::ResourceType::UNIFORM_BUFFER:
										case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
										case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
										case Rhi::ResourceType::SAMPLER_STATE:
										case Rhi::ResourceType::VERTEX_SHADER:
										case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
										case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
										case Rhi::ResourceType::GEOMETRY_SHADER:
										case Rhi::ResourceType::FRAGMENT_SHADER:
										case Rhi::ResourceType::TASK_SHADER:
										case Rhi::ResourceType::MESH_SHADER:
										case Rhi::ResourceType::COMPUTE_SHADER:
											RHI_LOG(CRITICAL, "Invalid OpenGL RHI implementation resource type")
											break;
									}

									// Set the OpenGL sampler states, if required (texture buffer has no sampler state), it's valid that there's no sampler state (e.g. texel fetch instead of sampling might be used)
									if (Rhi::ResourceType::TEXTURE_BUFFER != resourceType && nullptr != openGLResourceGroup->getSamplerState())
									{
										const SamplerState* samplerState = static_cast<const SamplerState*>(openGLResourceGroup->getSamplerState()[resourceIndex]);
										if (nullptr != samplerState)
										{
											// Is "GL_ARB_sampler_objects" there?
											if (mExtensions->isGL_ARB_sampler_objects())
											{
												// Effective sampler object (SO)
												glBindSampler(descriptorRange.baseShaderRegister, static_cast<const SamplerStateSo*>(samplerState)->getOpenGLSampler());
											}
											else
											{
												#if SE_OPENGL_STATE_CLEANUP
													// Backup the currently active OpenGL texture
													GLint openGLActiveTextureBackup = 0;
													glGetIntegerv(GL_ACTIVE_TEXTURE, &openGLActiveTextureBackup);
												#endif

												// TODO(co) Some security checks might be wise *maximum number of texture units*
												// Activate the texture unit we want to manipulate
												// -> "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
												glActiveTextureARB(GL_TEXTURE0_ARB + unit);

												// Is "GL_EXT_direct_state_access" there?
												if (mExtensions->isGL_EXT_direct_state_access())
												{
													// Direct state access (DSA) version to emulate a sampler object
													static_cast<const SamplerStateDsa*>(samplerState)->setOpenGLSamplerStates();
												}
												else
												{
													// Traditional bind version to emulate a sampler object
													static_cast<const SamplerStateBind*>(samplerState)->setOpenGLSamplerStates();
												}

												#if SE_OPENGL_STATE_CLEANUP
													// Be polite and restore the previous active OpenGL texture
													glActiveTextureARB(static_cast<GLenum>(openGLActiveTextureBackup));
												#endif
											}
										}
									}
								}
								else
								{
									// Traditional bind version

									// "GL_ARB_multitexture" required
									if (mExtensions->isGL_ARB_multitexture())
									{
										#if SE_OPENGL_STATE_CLEANUP
											// Backup the currently active OpenGL texture
											GLint openGLActiveTextureBackup = 0;
											glGetIntegerv(GL_ACTIVE_TEXTURE, &openGLActiveTextureBackup);
										#endif

										// TODO(co) Some security checks might be wise *maximum number of texture units*
										// Activate the texture unit we want to manipulate
										// -> "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
										glActiveTextureARB(GL_TEXTURE0_ARB + descriptorRange.baseShaderRegister);

										// Evaluate the resource type
										switch (resourceType)
										{
											case Rhi::ResourceType::TEXTURE_BUFFER:
												glBindTexture(GL_TEXTURE_BUFFER_ARB, static_cast<TextureBuffer*>(resource)->getOpenGLTexture());
												break;

											case Rhi::ResourceType::TEXTURE_1D:
												glBindTexture(GL_TEXTURE_1D, static_cast<Texture1D*>(resource)->getOpenGLTexture());
												break;

											case Rhi::ResourceType::TEXTURE_1D_ARRAY:
												// No extension check required, if we in here we already know it must exist
												glBindTexture(GL_TEXTURE_1D_ARRAY_EXT, static_cast<Texture1DArray*>(resource)->getOpenGLTexture());
												break;

											case Rhi::ResourceType::TEXTURE_2D:
											{
												const Texture2D* texture2D = static_cast<Texture2D*>(resource);
												glBindTexture(static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->getOpenGLTexture());
												break;
											}

											case Rhi::ResourceType::TEXTURE_2D_ARRAY:
												// No extension check required, if we in here we already know it must exist
												glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, static_cast<Texture2DArray*>(resource)->getOpenGLTexture());
												break;

											case Rhi::ResourceType::TEXTURE_3D:
												glBindTexture(GL_TEXTURE_3D, static_cast<Texture3D*>(resource)->getOpenGLTexture());
												break;

											case Rhi::ResourceType::TEXTURE_CUBE:
												glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<TextureCube*>(resource)->getOpenGLTexture());
												break;

											case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
												// TODO(co) Implement me
												// glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<TextureCubeArray*>(resource)->getOpenGLTexture());
												break;

											case Rhi::ResourceType::ROOT_SIGNATURE:
											case Rhi::ResourceType::RESOURCE_GROUP:
											case Rhi::ResourceType::GRAPHICS_PROGRAM:
											case Rhi::ResourceType::VERTEX_ARRAY:
											case Rhi::ResourceType::RENDER_PASS:
											case Rhi::ResourceType::QUERY_POOL:
											case Rhi::ResourceType::SWAP_CHAIN:
											case Rhi::ResourceType::FRAMEBUFFER:
											case Rhi::ResourceType::VERTEX_BUFFER:
											case Rhi::ResourceType::INDEX_BUFFER:
											case Rhi::ResourceType::STRUCTURED_BUFFER:
											case Rhi::ResourceType::INDIRECT_BUFFER:
											case Rhi::ResourceType::UNIFORM_BUFFER:
											case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
											case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
											case Rhi::ResourceType::SAMPLER_STATE:
											case Rhi::ResourceType::VERTEX_SHADER:
											case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
											case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
											case Rhi::ResourceType::GEOMETRY_SHADER:
											case Rhi::ResourceType::FRAGMENT_SHADER:
											case Rhi::ResourceType::TASK_SHADER:
											case Rhi::ResourceType::MESH_SHADER:
											case Rhi::ResourceType::COMPUTE_SHADER:
												RHI_LOG(CRITICAL, "Invalid OpenGL RHI implementation resource type")
												break;
										}

										// Set the OpenGL sampler states, if required (texture buffer has no sampler state), it's valid that there's no sampler state (e.g. texel fetch instead of sampling might be used)
										if (Rhi::ResourceType::TEXTURE_BUFFER != resourceType)
										{
											RHI_ASSERT(nullptr != openGLResourceGroup->getSamplerState(), "Invalid OpenGL sampler state")
											const SamplerState* samplerState = static_cast<const SamplerState*>(openGLResourceGroup->getSamplerState()[resourceIndex]);
											if (nullptr != samplerState)
											{
												// Is "GL_ARB_sampler_objects" there?
												if (mExtensions->isGL_ARB_sampler_objects())
												{
													// Effective sampler object (SO)
													glBindSampler(descriptorRange.baseShaderRegister, static_cast<const SamplerStateSo*>(samplerState)->getOpenGLSampler());
												}
												// Is "GL_EXT_direct_state_access" there?
												else if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
												{
													// Direct state access (DSA) version to emulate a sampler object
													static_cast<const SamplerStateDsa*>(samplerState)->setOpenGLSamplerStates();
												}
												else
												{
													// Traditional bind version to emulate a sampler object
													static_cast<const SamplerStateBind*>(samplerState)->setOpenGLSamplerStates();
												}
											}
										}

										#if SE_OPENGL_STATE_CLEANUP
											// Be polite and restore the previous active OpenGL texture
											glActiveTextureARB(static_cast<GLenum>(openGLActiveTextureBackup));
										#endif
									}
								}
								break;
							}

							case Rhi::DescriptorRangeType::UAV:
							{
								// Is "GL_EXT_shader_image_load_store" there?
								if (mExtensions->isGL_EXT_shader_image_load_store())
								{
									// "glBindImageTextureEXT()" unit parameter is zero based so we can simply use the value we received
									const GLuint unit = descriptorRange.baseShaderRegister;

									// TODO(co) Some security checks might be wise *maximum number of texture units*
									// Evaluate the texture type
									switch (resourceType)
									{
										case Rhi::ResourceType::TEXTURE_BUFFER:
										{
											const TextureBuffer* textureBuffer = static_cast<TextureBuffer*>(resource);
											glBindImageTextureEXT(unit, textureBuffer->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureBuffer->getOpenGLInternalFormat()));
											break;
										}

										case Rhi::ResourceType::TEXTURE_1D:
										{
											const Texture1D* texture1D = static_cast<Texture1D*>(resource);
											glBindImageTextureEXT(unit, texture1D->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture1D->getOpenGLInternalFormat()));
											break;
										}

										case Rhi::ResourceType::TEXTURE_1D_ARRAY:
										{
											const Texture1DArray* texture1DArray = static_cast<Texture1DArray*>(resource);
											glBindImageTextureEXT(unit, texture1DArray->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture1DArray->getOpenGLInternalFormat()));
											break;
										}

										case Rhi::ResourceType::TEXTURE_2D:
										{
											const Texture2D* texture2D = static_cast<Texture2D*>(resource);
											glBindImageTextureEXT(unit, texture2D->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture2D->getOpenGLInternalFormat()));
											break;
										}

										case Rhi::ResourceType::TEXTURE_2D_ARRAY:
										{
											const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(resource);
											glBindImageTextureEXT(unit, texture2DArray->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture2DArray->getOpenGLInternalFormat()));
											break;
										}

										case Rhi::ResourceType::TEXTURE_3D:
										{
											const Texture3D* texture3D = static_cast<Texture3D*>(resource);
											glBindImageTextureEXT(unit, texture3D->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture3D->getOpenGLInternalFormat()));
											break;
										}

										case Rhi::ResourceType::TEXTURE_CUBE:
										{
											const TextureCube* textureCube = static_cast<TextureCube*>(resource);
											glBindImageTextureEXT(unit, textureCube->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureCube->getOpenGLInternalFormat()));
											break;
										}

										case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
										{
											// TODO(co) Implement me
											// const TextureCubeArray* textureCubeArray = static_cast<TextureCubeArray*>(resource);
											// glBindImageTextureEXT(unit, textureCubeArray->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureCubeArray->getOpenGLInternalFormat()));
											break;
										}

										case Rhi::ResourceType::ROOT_SIGNATURE:
										case Rhi::ResourceType::RESOURCE_GROUP:
										case Rhi::ResourceType::GRAPHICS_PROGRAM:
										case Rhi::ResourceType::VERTEX_ARRAY:
										case Rhi::ResourceType::RENDER_PASS:
										case Rhi::ResourceType::QUERY_POOL:
										case Rhi::ResourceType::SWAP_CHAIN:
										case Rhi::ResourceType::FRAMEBUFFER:
										case Rhi::ResourceType::VERTEX_BUFFER:
										case Rhi::ResourceType::INDEX_BUFFER:
										case Rhi::ResourceType::STRUCTURED_BUFFER:
										case Rhi::ResourceType::INDIRECT_BUFFER:
										case Rhi::ResourceType::UNIFORM_BUFFER:
										case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
										case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
										case Rhi::ResourceType::SAMPLER_STATE:
										case Rhi::ResourceType::VERTEX_SHADER:
										case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
										case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
										case Rhi::ResourceType::GEOMETRY_SHADER:
										case Rhi::ResourceType::FRAGMENT_SHADER:
										case Rhi::ResourceType::TASK_SHADER:
										case Rhi::ResourceType::MESH_SHADER:
										case Rhi::ResourceType::COMPUTE_SHADER:
											RHI_LOG(CRITICAL, "Invalid OpenGL RHI implementation resource type")
											break;
									}
								}
								break;
							}

							case Rhi::DescriptorRangeType::UBV:
							case Rhi::DescriptorRangeType::SAMPLER:
							case Rhi::DescriptorRangeType::NUMBER_OF_RANGE_TYPES:
								RHI_LOG(CRITICAL, "Invalid OpenGL descriptor range type")
								break;
						}
						break;
					}

					case Rhi::ResourceType::VERTEX_BUFFER:
					{
						RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL vertex buffer must bound at SRV or UAV descriptor range type")
						RHI_ASSERT(Rhi::ShaderVisibility::ALL == descriptorRange.shaderVisibility || Rhi::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

						// "GL_ARB_uniform_buffer_object" required
						if (mExtensions->isGL_ARB_uniform_buffer_object())
						{
							// "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
							const GLuint index = descriptorRange.baseShaderRegister;

							// Attach the buffer to the given SSBO binding point
							glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<VertexBuffer*>(resource)->getOpenGLArrayBuffer());
						}
						break;
					}

					case Rhi::ResourceType::INDEX_BUFFER:
					{
						RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL index buffer must bound at SRV or UAV descriptor range type")
						RHI_ASSERT(Rhi::ShaderVisibility::ALL == descriptorRange.shaderVisibility || Rhi::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

						// "GL_ARB_uniform_buffer_object" required
						if (mExtensions->isGL_ARB_uniform_buffer_object())
						{
							// "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
							const GLuint index = descriptorRange.baseShaderRegister;

							// Attach the buffer to the given SSBO binding point
							glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<IndexBuffer*>(resource)->getOpenGLElementArrayBuffer());
						}
						break;
					}

					case Rhi::ResourceType::STRUCTURED_BUFFER:
					{
						RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL structured buffer must bound at SRV or UAV descriptor range type")

						// "GL_ARB_uniform_buffer_object" required
						if (mExtensions->isGL_ARB_uniform_buffer_object())
						{
							// "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
							const GLuint index = descriptorRange.baseShaderRegister;

							// Attach the buffer to the given SSBO binding point
							glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<StructuredBuffer*>(resource)->getOpenGLStructuredBuffer());
						}
						break;
					}

					case Rhi::ResourceType::INDIRECT_BUFFER:
					{
						RHI_ASSERT(Rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || Rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL indirect buffer must bound at SRV or UAV descriptor range type")
						RHI_ASSERT(Rhi::ShaderVisibility::ALL == descriptorRange.shaderVisibility || Rhi::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

						// "GL_ARB_uniform_buffer_object" required
						if (mExtensions->isGL_ARB_uniform_buffer_object())
						{
							// "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
							const GLuint index = descriptorRange.baseShaderRegister;

							// Attach the buffer to the given SSBO binding point
							glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<IndirectBuffer*>(resource)->getOpenGLIndirectBuffer());
						}
						break;
					}

					case Rhi::ResourceType::UNIFORM_BUFFER:
						// "GL_ARB_uniform_buffer_object" required
						if (mExtensions->isGL_ARB_uniform_buffer_object())
						{
							// Attach the buffer to the given UBO binding point
							// -> Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
							// -> Direct3D 10 and Direct3D 11 have explicit binding points
							RHI_ASSERT(Rhi::DescriptorRangeType::UBV == descriptorRange.rangeType, "OpenGL uniform buffer must bound at UBV descriptor range type")
							RHI_ASSERT(nullptr != openGLResourceGroup->getResourceIndexToUniformBlockBindingIndex(), "Invalid OpenGL resource index to uniform block binding index")
							glBindBufferBase(GL_UNIFORM_BUFFER, openGLResourceGroup->getResourceIndexToUniformBlockBindingIndex()[resourceIndex], static_cast<UniformBuffer*>(resource)->getOpenGLUniformBuffer());
						}
						break;

					case Rhi::ResourceType::SAMPLER_STATE:
						// Unlike Direct3D >=10, OpenGL directly attaches the sampler settings to the texture (unless the sampler object extension is used)
						break;

					case Rhi::ResourceType::ROOT_SIGNATURE:
					case Rhi::ResourceType::RESOURCE_GROUP:
					case Rhi::ResourceType::GRAPHICS_PROGRAM:
					case Rhi::ResourceType::VERTEX_ARRAY:
					case Rhi::ResourceType::RENDER_PASS:
					case Rhi::ResourceType::QUERY_POOL:
					case Rhi::ResourceType::SWAP_CHAIN:
					case Rhi::ResourceType::FRAMEBUFFER:
					case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
					case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
					case Rhi::ResourceType::VERTEX_SHADER:
					case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
					case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
					case Rhi::ResourceType::GEOMETRY_SHADER:
					case Rhi::ResourceType::FRAGMENT_SHADER:
					case Rhi::ResourceType::TASK_SHADER:
					case Rhi::ResourceType::MESH_SHADER:
					case Rhi::ResourceType::COMPUTE_SHADER:
						RHI_LOG(CRITICAL, "Invalid OpenGL RHI implementation resource type")
						break;
				}
			}
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}

	void OpenGLRhi::setOpenGLGraphicsProgram(Rhi::IGraphicsProgram* graphicsProgram)
	{
		if (nullptr != graphicsProgram)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *graphicsProgram)

			// Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
			if (mExtensions->isGL_ARB_separate_shader_objects())
			{
				// Bind the graphics program pipeline, if required
				GraphicsProgramSeparate* graphicsProgramSeparate = static_cast<GraphicsProgramSeparate*>(graphicsProgram);
				const uint32_t openGLProgramPipeline = graphicsProgramSeparate->getOpenGLProgramPipeline();
				if (openGLProgramPipeline != mOpenGLProgramPipeline)
				{
					mOpenGLProgramPipeline = openGLProgramPipeline;
					{ // Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
						const VertexShaderSeparate* vertexShaderSeparate = graphicsProgramSeparate->getVertexShaderSeparate();
						if (nullptr != vertexShaderSeparate)
						{
							mOpenGLVertexProgram = vertexShaderSeparate->getOpenGLShaderProgram();
							mDrawIdUniformLocation = vertexShaderSeparate->getDrawIdUniformLocation();
						}
						else
						{
							mOpenGLVertexProgram = 0;
							mDrawIdUniformLocation = -1;
						}
						mCurrentStartInstanceLocation = ~0u;
					}
					glBindProgramPipeline(mOpenGLProgramPipeline);
				}
			}
			else if (mExtensions->isGL_ARB_shader_objects())
			{
				// Bind the graphics program, if required
				const GraphicsProgramMonolithic* graphicsProgramMonolithic = static_cast<GraphicsProgramMonolithic*>(graphicsProgram);
				const uint32_t openGLProgram = graphicsProgramMonolithic->getOpenGLProgram();
				if (openGLProgram != mOpenGLProgram)
				{
					mOpenGLProgram = mOpenGLVertexProgram = openGLProgram;
					mDrawIdUniformLocation = graphicsProgramMonolithic->getDrawIdUniformLocation();
					mCurrentStartInstanceLocation = ~0u;
					glUseProgram(mOpenGLProgram);
				}
			}
		}
		else
		{
			// Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
			if (mExtensions->isGL_ARB_separate_shader_objects())
			{
				// Unbind the program pipeline, if required
				if (0 != mOpenGLProgramPipeline)
				{
					glBindProgramPipeline(0);
					mOpenGLProgramPipeline = 0;
				}
			}
			else if (mExtensions->isGL_ARB_shader_objects())
			{
				// Unbind the program, if required
				if (0 != mOpenGLProgram)
				{
					glUseProgram(0);
					mOpenGLProgram = 0;
				}
			}
			mOpenGLVertexProgram = 0;
			mDrawIdUniformLocation = -1;
			mCurrentStartInstanceLocation = ~0u;
		}
	}

	void OpenGLRhi::setOpenGLComputePipelineState(ComputePipelineState* computePipelineState)
	{
		if (nullptr != computePipelineState)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *computePipelineState)

			// Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
			if (mExtensions->isGL_ARB_separate_shader_objects())
			{
				// Bind the program pipeline, if required
				const uint32_t openGLProgramPipeline = static_cast<ComputePipelineStateSeparate*>(computePipelineState)->getOpenGLProgramPipeline();
				if (openGLProgramPipeline != mOpenGLProgramPipeline)
				{
					mOpenGLProgramPipeline = openGLProgramPipeline;
					glBindProgramPipeline(mOpenGLProgramPipeline);
					mOpenGLVertexProgram = 0;
					mDrawIdUniformLocation = -1;
					mCurrentStartInstanceLocation = ~0u;
				}
			}
			else if (mExtensions->isGL_ARB_shader_objects())
			{
				// Bind the program, if required
				const uint32_t openGLProgram = static_cast<ComputePipelineStateMonolithic*>(computePipelineState)->getOpenGLProgram();
				if (openGLProgram != mOpenGLProgram)
				{
					mOpenGLProgram = openGLProgram;
					glUseProgram(mOpenGLProgram);
					mOpenGLVertexProgram = 0;
					mDrawIdUniformLocation = -1;
					mCurrentStartInstanceLocation = ~0u;
				}
			}
		}
		else
		{
			// Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
			if (mExtensions->isGL_ARB_separate_shader_objects())
			{
				// Unbind the program pipeline, if required
				if (0 != mOpenGLProgramPipeline)
				{
					glBindProgramPipeline(0);
					mOpenGLProgramPipeline = 0;
				}
			}
			else if (mExtensions->isGL_ARB_shader_objects())
			{
				// Unbind the program, if required
				if (0 != mOpenGLProgram)
				{
					glUseProgram(0);
					mOpenGLProgram = 0;
				}
			}
			mOpenGLVertexProgram = 0;
			mDrawIdUniformLocation = -1;
			mCurrentStartInstanceLocation = ~0u;
		}
	}

	void OpenGLRhi::updateGL_ARB_base_instanceEmulation(uint32_t startInstanceLocation)
	{
		if (mDrawIdUniformLocation != -1 && 0 != mOpenGLVertexProgram && mCurrentStartInstanceLocation != startInstanceLocation)
		{
			glProgramUniform1ui(mOpenGLVertexProgram, mDrawIdUniformLocation, startInstanceLocation);
			mCurrentStartInstanceLocation = startInstanceLocation;
		}
	}
} // OpenGLRhi

Rhi::IRhi* createOpenGLRhiInstance(const Rhi::Context& context)
{
	return RHI_NEW(OpenGLRhi::OpenGLRhi)(context);
}
#endif // SE_OPENGL
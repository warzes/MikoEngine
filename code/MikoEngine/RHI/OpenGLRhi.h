#pragma once

namespace OpenGLRhi
{
	class OpenGLRhi final : public Rhi::IRhi
	{
		friend class GraphicsPipelineState;
	public:
		MakeID VertexArrayMakeId;
		MakeID GraphicsPipelineStateMakeId;
		MakeID ComputePipelineStateMakeId;


		explicit OpenGLRhi(const handle& nativeWindowHandle);
		virtual ~OpenGLRhi() override;

		[[nodiscard]] inline const IOpenGLContext& getOpenGLContext() const
		{
			return *mOpenGLContext;
		}

		[[nodiscard]] inline const Extensions& getExtensions() const
		{
			return *mExtensions;
		}

		[[nodiscard]] inline Extensions& getExtensions()
		{
			return *mExtensions;
		}

		//[-------------------------------------------------------]
		//[ Graphics                                              ]
		//[-------------------------------------------------------]
		void setGraphicsRootSignature(Rhi::IRootSignature* rootSignature);
		void setGraphicsPipelineState(Rhi::IGraphicsPipelineState* graphicsPipelineState);
		void setGraphicsResourceGroup(uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup);
		void setGraphicsVertexArray(Rhi::IVertexArray* vertexArray);															// Input-assembler (IA) stage
		void setGraphicsViewports(uint32_t numberOfViewports, const Rhi::Viewport* viewports);									// Rasterizer (RS) stage
		void setGraphicsScissorRectangles(uint32_t numberOfScissorRectangles, const Rhi::ScissorRectangle* scissorRectangles);	// Rasterizer (RS) stage
		void setGraphicsRenderTarget(Rhi::IRenderTarget* renderTarget);															// Output-merger (OM) stage
		void clearGraphics(uint32_t clearFlags, const float color[4], float z, uint32_t stencil);
		void drawGraphics(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1);
		void drawGraphicsEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1);
		void drawIndexedGraphics(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1);
		void drawIndexedGraphicsEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1);
		void drawMeshTasks(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1);
		void drawMeshTasksEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1);
		//[-------------------------------------------------------]
		//[ Compute                                               ]
		//[-------------------------------------------------------]
		void setComputeRootSignature(Rhi::IRootSignature* rootSignature);
		void setComputePipelineState(Rhi::IComputePipelineState* computePipelineState);
		void setComputeResourceGroup(uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup);
		void dispatchCompute(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
		//[-------------------------------------------------------]
		//[ Resource                                              ]
		//[-------------------------------------------------------]
		void resolveMultisampleFramebuffer(Rhi::IRenderTarget& destinationRenderTarget, Rhi::IFramebuffer& sourceMultisampleFramebuffer);
		void copyResource(Rhi::IResource& destinationResource, Rhi::IResource& sourceResource);
		void generateMipmaps(Rhi::IResource& resource);
		//[-------------------------------------------------------]
		//[ Query                                                 ]
		//[-------------------------------------------------------]
		void resetQueryPool(Rhi::IQueryPool& queryPool, uint32_t firstQueryIndex, uint32_t numberOfQueries);
		void beginQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex, uint32_t queryControlFlags);
		void endQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex);
		void writeTimestampQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex);
		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
#if SE_DEBUG
		void setDebugMarker(const char* name);
		void beginDebugEvent(const char* name);
		void endDebugEvent();
#endif

		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRhi methods                      ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual const char* getName() const override;
		[[nodiscard]] virtual bool isInitialized() const override;
		[[nodiscard]] virtual bool isDebugEnabled() override;
		//[-------------------------------------------------------]
		//[ Shader language                                       ]
		//[-------------------------------------------------------]
		[[nodiscard]] virtual uint32_t getNumberOfShaderLanguages() const override;
		[[nodiscard]] virtual const char* getShaderLanguageName(uint32_t index) const override;
		[[nodiscard]] virtual Rhi::IShaderLanguage* getShaderLanguage(const char* shaderLanguageName = nullptr) override;
		//[-------------------------------------------------------]
		//[ Resource creation                                     ]
		//[-------------------------------------------------------]
		[[nodiscard]] virtual Rhi::IRenderPass* createRenderPass(uint32_t numberOfColorAttachments, const Rhi::TextureFormat::Enum* colorAttachmentTextureFormats, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat = Rhi::TextureFormat::UNKNOWN, uint8_t numberOfMultisamples = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
		[[nodiscard]] virtual Rhi::IQueryPool* createQueryPool(Rhi::QueryType queryType, uint32_t numberOfQueries = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
		[[nodiscard]] virtual Rhi::ISwapChain* createSwapChain(Rhi::IRenderPass& renderPass, Rhi::WindowHandle windowHandle, bool useExternalContext = false RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
		[[nodiscard]] virtual Rhi::IFramebuffer* createFramebuffer(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
		[[nodiscard]] virtual Rhi::IBufferManager* createBufferManager() override;
		[[nodiscard]] virtual Rhi::ITextureManager* createTextureManager() override;
		[[nodiscard]] virtual Rhi::IRootSignature* createRootSignature(const Rhi::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
		[[nodiscard]] virtual Rhi::IGraphicsPipelineState* createGraphicsPipelineState(const Rhi::GraphicsPipelineState& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
		[[nodiscard]] virtual Rhi::IComputePipelineState* createComputePipelineState(Rhi::IRootSignature& rootSignature, Rhi::IComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
		[[nodiscard]] virtual Rhi::ISamplerState* createSamplerState(const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
		//[-------------------------------------------------------]
		//[ Resource handling                                     ]
		//[-------------------------------------------------------]
		[[nodiscard]] virtual bool map(Rhi::IResource& resource, uint32_t subresource, Rhi::MapType mapType, uint32_t mapFlags, Rhi::MappedSubresource& mappedSubresource) override;
		virtual void unmap(Rhi::IResource& resource, uint32_t subresource) override;
		[[nodiscard]] virtual bool getQueryPoolResults(Rhi::IQueryPool& queryPool, uint32_t numberOfDataBytes, uint8_t* data, uint32_t firstQueryIndex = 0, uint32_t numberOfQueries = 1, uint32_t strideInBytes = 0, uint32_t queryResultFlags = Rhi::QueryResultFlags::WAIT) override;
		//[-------------------------------------------------------]
		//[ Operations                                            ]
		//[-------------------------------------------------------]
		[[nodiscard]] virtual bool beginScene() override;
		virtual void submitCommandBuffer(const Rhi::CommandBuffer& commandBuffer) override;
		virtual void endScene() override;
		//[-------------------------------------------------------]
		//[ Synchronization                                       ]
		//[-------------------------------------------------------]
		virtual void flush() override;
		virtual void finish() override;

		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(OpenGLRhi, this);
		}

		//[-------------------------------------------------------]
		//[ Private static methods                                ]
		//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Debug message callback function called by the "GL_ARB_debug_output"-extension
		*
		*  @param[in] source
		*    Source of the debug message
		*  @param[in] type
		*    Type of the debug message
		*  @param[in] id
		*    ID of the debug message
		*  @param[in] severity
		*    Severity of the debug message
		*  @param[in] length
		*    Length of the debug message
		*  @param[in] message
		*    The debug message
		*  @param[in] userParam
		*    Additional user parameter of the debug message
		*/
		static void CALLBACK debugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char* message, const void* userParam);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit OpenGLRhi(const OpenGLRhi&) = delete;
		OpenGLRhi& operator=(const OpenGLRhi&) = delete;

		void initializeCapabilities();

		void unsetGraphicsVertexArray();

		/**
		*  @brief
		*    Set resource group, method used by graphics and compute
		*
		*  @param[in] rootSignature
		*    Used root signature
		*  @param[in] rootParameterIndex
		*    Root parameter index
		*  @param[in] resourceGroup
		*    Resource group to set, can be a null pointer
		*/
		void setResourceGroup(const RootSignature& rootSignature, uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup);

		void setOpenGLGraphicsProgram(Rhi::IGraphicsProgram* graphicsProgram);

		void setOpenGLComputePipelineState(ComputePipelineState* computePipelineState);

		/**
		*  @brief
		*    Update "GL_ARB_base_instance" emulation
		*
		*  @param[in] startInstanceLocation
		*    Start instance location
		*/
		void updateGL_ARB_base_instanceEmulation(uint32_t startInstanceLocation);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		OpenGLRuntimeLinking* mOpenGLRuntimeLinking;			// OpenGL runtime linking instance, always valid
		IOpenGLContext*		  mOpenGLContext;					// OpenGL context instance, always valid
		Extensions*			  mExtensions;						// Extensions instance, always valid
		Rhi::IShaderLanguage* mShaderLanguage;					// Shader language instance (we keep a reference to it), can be a null pointer
		RootSignature*		  mGraphicsRootSignature;			// Currently set graphics root signature (we keep a reference to it), can be a null pointer
		RootSignature*		  mComputeRootSignature;			// Currently set compute root signature (we keep a reference to it), can be a null pointer
		Rhi::ISamplerState*   mDefaultSamplerState;				// Default rasterizer state (we keep a reference to it), can be a null pointer
		GLuint				  mOpenGLCopyResourceFramebuffer;	// OpenGL framebuffer ("container" object, not shared between OpenGL contexts) used by "OpenGLRhi::OpenGLRhi::copyResource()" if the "GL_ARB_copy_image"-extension isn't available, can be zero if no resource is allocated
		GLuint				  mDefaultOpenGLVertexArray;		// Default OpenGL vertex array ("container" object, not shared between OpenGL contexts) to enable attribute-less rendering, can be zero if no resource is allocated
		// States
		GraphicsPipelineState* mGraphicsPipelineState;	// Currently set graphics pipeline state (we keep a reference to it), can be a null pointer
		ComputePipelineState*  mComputePipelineState;	// Currently set compute pipeline state (we keep a reference to it), can be a null pointer
		// Input-assembler (IA) stage
		VertexArray* mVertexArray;				// Currently set vertex array (we keep a reference to it), can be a null pointer
		GLenum		 mOpenGLPrimitiveTopology;	// OpenGL primitive topology describing the type of primitive to render
		GLint		 mNumberOfVerticesPerPatch;	// Number of vertices per patch
		// Output-merger (OM) stage
		Rhi::IRenderTarget* mRenderTarget;	// Currently set render target (we keep a reference to it), can be a null pointer
		// State cache to avoid making redundant OpenGL calls
		GLenum mOpenGLClipControlOrigin;	// Currently set OpenGL clip control origin
		GLuint mOpenGLProgramPipeline;		// Currently set OpenGL program pipeline, can be zero if no resource is set
		GLuint mOpenGLProgram;				// Currently set OpenGL program, can be zero if no resource is set
		GLuint mOpenGLIndirectBuffer;		// Currently set OpenGL indirect buffer, can be zero if no resource is set
		// Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
		GLuint	 mOpenGLVertexProgram;			// Currently set OpenGL vertex program, can be zero if no resource is set
		GLint	 mDrawIdUniformLocation;		// Draw ID uniform location
		uint32_t mCurrentStartInstanceLocation;	// Currently set start instance location
#if SE_DEBUG
		bool mDebugBetweenBeginEndScene;	// Just here for state tracking in debug builds
#endif
	};
} // namespace OpenGLRhi
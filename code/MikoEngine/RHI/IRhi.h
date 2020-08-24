#pragma once

namespace Rhi
{
	enum class NameId : uint32_t
	{
		VULKAN = 1646768219,	// Vulkan RHI implementation, same value as renderer STRING_ID("Vulkan")
		DIRECT3D12 = 2152506057,// Direct3D 12 RHI implementation, same value as renderer STRING_ID("Direct3D12")
		DIRECT3D11 = 2102173200,// Direct3D 11 RHI implementation, same value as renderer STRING_ID("Direct3D11")
		OPENGL = 1149085807,	// OpenGL RHI implementation, same value as renderer STRING_ID("OpenGL")
		OPENGLES3 = 4137012044,	// OpenGL ES 3 RHI implementation, same value as renderer STRING_ID("OpenGLES3")
		NULL_DUMMY = 3816175889	// Null RHI implementation, same value as renderer STRING_ID("Null")
	};

	class IRhi : public RefCount<IRhi>
	{
		// Friends for none constant statistics access
		friend class IRootSignature;
		friend class IResourceGroup;
		friend class IGraphicsProgram;
		friend class IVertexArray;
		friend class IRenderPass;
		friend class IQueryPool;
		friend class ISwapChain;
		friend class IFramebuffer;
		friend class IVertexBuffer;
		friend class IIndexBuffer;
		friend class ITextureBuffer;
		friend class IStructuredBuffer;
		friend class IIndirectBuffer;
		friend class IUniformBuffer;
		friend class ITexture1D;
		friend class ITexture1DArray;
		friend class ITexture2D;
		friend class ITexture2DArray;
		friend class ITexture3D;
		friend class ITextureCube;
		friend class ITextureCubeArray;
		friend class IGraphicsPipelineState;
		friend class IComputePipelineState;
		friend class ISamplerState;
		friend class IVertexShader;
		friend class ITessellationControlShader;
		friend class ITessellationEvaluationShader;
		friend class IGeometryShader;
		friend class IFragmentShader;
		friend class ITaskShader;
		friend class IMeshShader;
		friend class IComputeShader;

	// Public methods
	public:
		inline virtual ~IRhi() override = default;

		[[nodiscard]] inline NameId getNameId() const
		{
			return mNameId;
		}

		[[nodiscard]] inline handle getNativeWindowHandle() const
		{
			return m_nativeWindowHandle;
		}

		[[nodiscard]] inline const Capabilities& getCapabilities() const
		{
			return mCapabilities;
		}

		[[nodiscard]] inline IShaderLanguage& getDefaultShaderLanguage()
		{
			IShaderLanguage* shaderLanguage = getShaderLanguage();
			RHI_ASSERT(nullptr != shaderLanguage, "There's no default shader language");
			return *shaderLanguage;
		}

#if SE_RHI_STATISTICS
		[[nodiscard]] inline const Statistics& getStatistics() const
		{
			return mStatistics;
		}
#endif

	// Public virtual Rhi::IRhi methods
	public:
		[[nodiscard]] virtual const char* getName() const = 0;

		[[nodiscard]] virtual bool isInitialized() const = 0;

		/**
		*  @brief
		*    Return whether or not debug is enabled
		*
		*  @remarks
		*    By using
		*      "Rhi::IRhi::isDebugEnabled();"
		*    it is possible to check whether or not your application is currently running
		*    within a known debug/profile tool like e.g. Direct3D PIX (also works directly within VisualStudio
		*    2017 out-of-the-box). In case you want at least try to protect your asset, you might want to stop
		*    the execution of your application when a debug/profile tool is used which can e.g. record your data.
		*    Please be aware that this will only make it a little bit harder to debug and e.g. while doing so
		*    reading out your asset data. Public articles like
		*    "PIX: How to circumvent D3DPERF_SetOptions" at
		*      http://www.gamedev.net/blog/1323/entry-2250952-pix-how-to-circumvent-d3dperf-setoptions/
		*    describe how to "hack around" this security measurement, so, don't rely on it. Those debug
		*    methods work fine when using a Direct3D RHI implementation. OpenGL on the other hand
		*    has no Direct3D PIX like functions or extensions, use for instance "gDEBugger" (http://www.gremedy.com/)
		*    instead.
		*    -> When using Direct3D <11.1, those methods map to the Direct3D 9 PIX functions (D3DPERF_* functions)
		*    -> The Direct3D 9 PIX functions are also used for Direct3D 10 and Direct3D 11. Lookout! As soon as using
		*       the debug methods within this interface, the Direct3D 9 dll will be loaded.
		*    -> Starting with Direct3D 11.1, the Direct3D 9 PIX functions no longer work. Instead, the new
		*       "D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY"-flag (does not work with <Direct3D 11.1)
		*       is used when creating the device instance, then the "ID3DUserDefinedAnnotation"-API is used.
		*    -> Optimization: You might want to use those methods only via macros to make it easier to avoid using them
		*       within e.g. a final release build
		*
		*  @return
		*    "true" if debug is enabled, else "false"
		*/
		[[nodiscard]] virtual bool isDebugEnabled() = 0;

		//[-------------------------------------------------------]
		//[ Shader language                                       ]
		//[-------------------------------------------------------]
		[[nodiscard]] virtual uint32_t getNumberOfShaderLanguages() const = 0;

		/**
		*  @brief
		*    Return the name of a supported shader language at the provided index
		*
		*  @param[in] index
		*    Index of the supported shader language to return the name from ([0, getNumberOfShaderLanguages()-1])
		*
		*  @return
		*    The ASCII name (for example "GLSL" or "HLSL") of the supported shader language at the provided index, can be a null pointer
		*
		*  @note
		*    - Do not free the memory the returned pointer is pointing to
		*    - The default shader language is always at index 0
		*/
		[[nodiscard]] virtual const char* getShaderLanguageName(uint32_t index) const = 0;

		/**
		*  @brief
		*    Return a shader language instance
		*
		*  @param[in] shaderLanguageName
		*    The ASCII name of the shader language (for example "GLSL" or "HLSL"), if null pointer or empty string,
		*    the default RHI shader language is used (see "getShaderLanguageName(0)")
		*
		*  @return
		*    The shader language instance, a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] virtual IShaderLanguage* getShaderLanguage(const char* shaderLanguageName = nullptr) = 0;

		//[-------------------------------------------------------]
		//[ Resource creation                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a render pass instance
		*
		*  @param[in] numberOfColorAttachments
		*    Number of color render target textures, must be <="Rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets"
		*  @param[in] colorAttachmentTextureFormats
		*    The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "numberOfColorAttachments" textures in the provided C-array of pointers
		*  @param[in] depthStencilAttachmentTextureFormat
		*    The optional depth stencil render target texture format, can be a "Rhi::TextureFormat::UNKNOWN" if there should be no depth buffer
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*
		*  @return
		*    The created render pass instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IRenderPass* createRenderPass(uint32_t numberOfColorAttachments, const TextureFormat::Enum* colorAttachmentTextureFormats, TextureFormat::Enum depthStencilAttachmentTextureFormat = TextureFormat::UNKNOWN, uint8_t numberOfMultisamples = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a asynchronous query pool instance
		*
		*  @param[in] queryType
		*    Query type (e.g. "Rhi::QueryType::OCCLUSION")
		*  @param[in] numberOfQueries
		*    The number of queries the query pool contains (e.g. 1)
		*
		*  @return
		*    The created query pool instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IQueryPool* createQueryPool(QueryType queryType, uint32_t numberOfQueries = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a swap chain instance
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] windowHandle
		*    Information about the window to render into
		*  @param[in] useExternalContext
		*    Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
		*
		*  @return
		*    The created swap chain instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual ISwapChain* createSwapChain(IRenderPass& renderPass, WindowHandle windowHandle, bool useExternalContext = false RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a framebuffer object (FBO) instance
		*
		*  @param[in] renderPass
		*    Render pass to use, the framebuffer keeps a reference to the render pass
		*  @param[in] colorFramebufferAttachments
		*    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "Rhi::IRenderPass::getNumberOfColorAttachments()" textures in the provided C-array of pointers
		*  @param[in] depthStencilFramebufferAttachment
		*    The optional depth stencil render target texture, can be a null pointer
		*
		*  @return
		*    The created FBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets" is not 0
		*    - The framebuffer keeps a reference to the provided texture instances
		*    - It's invalid to set the same color texture to multiple render targets at one and the same time
		*    - Depending on the used RHI implementation and feature set, there might be the requirement that all provided textures have the same size
		*      (in order to be on the save side, ensure that all provided textures have the same size and same MSAA sample count)
		*/
		[[nodiscard]] virtual IFramebuffer* createFramebuffer(IRenderPass& renderPass, const FramebufferAttachment* colorFramebufferAttachments, const FramebufferAttachment* depthStencilFramebufferAttachment = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a buffer manager instance
		*
		*  @return
		*    The created buffer manager instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IBufferManager* createBufferManager() = 0;

		/**
		*  @brief
		*    Create a texture manager instance
		*
		*  @return
		*    The created texture manager instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual ITextureManager* createTextureManager() = 0;

		/**
		*  @brief
		*    Create a root signature instance
		*
		*  @param[in] rootSignature
		*    Root signature to use
		*
		*  @return
		*    The root signature instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IRootSignature* createRootSignature(const RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a graphics pipeline state instance
		*
		*  @param[in] graphicsPipelineState
		*    Graphics pipeline state to use
		*
		*  @return
		*    The graphics pipeline state instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IGraphicsPipelineState* createGraphicsPipelineState(const GraphicsPipelineState& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a compute pipeline state instance
		*
		*  @param[in] rootSignature
		*    Root signature (compute pipeline state instances keep a reference to the root signature)
		*  @param[in] computeShader
		*    Compute shader used by the compute pipeline state (compute pipeline state instances keep a reference to the shader)
		*
		*  @return
		*    The compute pipeline state instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IComputePipelineState* createComputePipelineState(IRootSignature& rootSignature, IComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a sampler state instance
		*
		*  @param[in] samplerState
		*    Sampler state to use
		*
		*  @return
		*    The sampler state instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual ISamplerState* createSamplerState(const SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		//[-------------------------------------------------------]
		//[ Resource handling                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Map a resource
		*
		*  @param[in]  resource
		*    Resource to map, there's no internal resource validation, so, do only use valid resources in here!
		*  @param[in]  subresource
		*    Subresource
		*  @param[in]  mapType
		*    Map type
		*  @param[in]  mapFlags
		*    Map flags, see "Rhi::MapFlag"-flags
		*  @param[out] mappedSubresource
		*    Receives the mapped subresource information, do only use this data in case this method returns successfully
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] virtual bool map(IResource& resource, uint32_t subresource, MapType mapType, uint32_t mapFlags, MappedSubresource& mappedSubresource) = 0;

		/**
		*  @brief
		*    Unmap a resource
		*
		*  @param[in] resource
		*    Resource to unmap, there's no internal resource validation, so, do only use valid resources in here!
		*  @param[in] subresource
		*    Subresource
		*/
		virtual void unmap(IResource& resource, uint32_t subresource) = 0;

		/**
		*  @brief
		*    Get asynchronous query pool results
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] numberOfDataBytes
		*    Number of data bytes
		*  @param[out] data
		*    Receives the query data
		*  @param[in] firstQueryIndex
		*    First query index (e.g. 0)
		*  @param[in] numberOfQueries
		*    Number of queries (e.g. 1)
		*  @param[in] strideInBytes
		*    Stride in bytes, 0 is only valid in case there's just a single query
		*  @param[in] queryResultFlags
		*    Query control flags (e.g. "Rhi::QueryResultFlags::WAIT")
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] virtual bool getQueryPoolResults(IQueryPool& queryPool, uint32_t numberOfDataBytes, uint8_t* data, uint32_t firstQueryIndex = 0, uint32_t numberOfQueries = 1, uint32_t strideInBytes = 0, uint32_t queryResultFlags = QueryResultFlags::WAIT) = 0;

		//[-------------------------------------------------------]
		//[ Operations                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Begin scene rendering
		*
		*  @return
		*    "true" if all went fine, else "false" (In this case: Don't dare to render something)
		*
		*  @note
		*    - In order to be RHI implementation independent, call this method when starting to render something
		*/
		[[nodiscard]] virtual bool beginScene() = 0;

		/**
		*  @brief
		*    Submit command buffer to RHI
		*
		*  @param[in] commandBuffer
		*    Command buffer to submit
		*/
		virtual void submitCommandBuffer(const CommandBuffer& commandBuffer) = 0;

		/**
		*  @brief
		*    End scene rendering
		*
		*  @note
		*    - In order to be RHI implementation independent, call this method when you're done with rendering
		*/
		virtual void endScene() = 0;

		//[-------------------------------------------------------]
		//[ Synchronization                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Force the execution of render commands in finite time (synchronization)
		*/
		virtual void flush() = 0;

		/**
		*  @brief
		*    Force the execution of render commands in finite time and wait until it's done (synchronization)
		*/
		virtual void finish() = 0;

		// Protected methods
	protected:
		inline IRhi(NameId nameId, const handle& nativeWindowHandle) :
			mNameId(nameId),
			m_nativeWindowHandle(nativeWindowHandle)
		{
		}

		explicit IRhi(const IRhi&) = delete;
		IRhi& operator=(const IRhi&) = delete;

#if SE_RHI_STATISTICS
		[[nodiscard]] inline Statistics& getStatistics()
		{
			return mStatistics;
		}
#endif
		// Protected data
	protected:
		NameId		   mNameId;
		const handle m_nativeWindowHandle;
		Capabilities   mCapabilities;

#if SE_RHI_STATISTICS
	private:
		Statistics mStatistics;
#endif
	};

	typedef SmartRefCount<IRhi> IRhiPtr;
} // namespace Rhi
#pragma once

namespace Direct3D12Rhi
{
	//[-------------------------------------------------------]
//[ Direct3D12Rhi/Direct3D12Rhi.h                         ]
//[-------------------------------------------------------]
	class Direct3D12Rhi final : public Rhi::IRhi
	{
	public:
		static constexpr uint32_t NUMBER_OF_FRAMES = 2;

		MakeID VertexArrayMakeId;
		MakeID GraphicsPipelineStateMakeId;
		MakeID ComputePipelineStateMakeId;

		explicit Direct3D12Rhi(const handle& nativeWindowHandle);
		virtual ~Direct3D12Rhi() override;

		/**
		*  @brief
		*    Return the DXGI factory instance
		*
		*  @return
		*    The DXGI factory instance, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IDXGIFactory4& getDxgiFactory4() const
		{
			RHI_ASSERT(nullptr != mDxgiFactory4, "Invalid Direct3D 12 DXGI factory 4")
				return *mDxgiFactory4;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 device
		*
		*  @return
		*    The Direct3D 12 device, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Device& getD3D12Device() const
		{
			RHI_ASSERT(nullptr != mD3D12Device, "Invalid Direct3D 12 device")
				return *mD3D12Device;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 command queue
		*
		*  @return
		*    The Direct3D 12 command queue, null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12CommandQueue* getD3D12CommandQueue() const
		{
			return mD3D12CommandQueue;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 graphics command list
		*
		*  @return
		*    The Direct3D 12 graphics command list, null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12GraphicsCommandList* getD3D12GraphicsCommandList() const
		{
			return mD3D12GraphicsCommandList;
		}

		/**
		*  @brief
		*    Get the render target to render into
		*
		*  @return
		*    Render target currently bound to the output-merger state, a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline Rhi::IRenderTarget* omGetRenderTarget() const
		{
			return mRenderTarget;
		}

		/**
		*  @brief
		*    Get the upload context
		*
		*  @return
		*    Upload context
		*/
		[[nodiscard]] inline ::detail::UploadContext& getUploadContext()
		{
			return mUploadContext;
		}

		//[-------------------------------------------------------]
		//[ Descriptor heaps                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline ::detail::DescriptorHeap& getShaderResourceViewDescriptorHeap() const
		{
			RHI_ASSERT(nullptr != mShaderResourceViewDescriptorHeap, "Invalid Direct3D 12 shader resource view descriptor heap")
				return *mShaderResourceViewDescriptorHeap;
		}

		[[nodiscard]] inline ::detail::DescriptorHeap& getRenderTargetViewDescriptorHeap() const
		{
			RHI_ASSERT(nullptr != mShaderResourceViewDescriptorHeap, "Invalid Direct3D 12 render target view descriptor heap")
				return *mRenderTargetViewDescriptorHeap;
		}

		[[nodiscard]] inline ::detail::DescriptorHeap& getDepthStencilViewDescriptorHeap() const
		{
			RHI_ASSERT(nullptr != mShaderResourceViewDescriptorHeap, "Invalid Direct3D 12 depth stencil target view descriptor heap")
				return *mDepthStencilViewDescriptorHeap;
		}

		[[nodiscard]] inline ::detail::DescriptorHeap& getSamplerDescriptorHeap() const
		{
			RHI_ASSERT(nullptr != mSamplerDescriptorHeap, "Invalid Direct3D 12 sampler descriptor heap")
				return *mSamplerDescriptorHeap;
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
		[[nodiscard]] inline virtual const char* getName() const override
		{
			return "Direct3D12";
		}

		[[nodiscard]] inline virtual bool isInitialized() const override
		{
			// Is there a Direct3D 12 command queue?
			return (nullptr != mD3D12CommandQueue);
		}

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
			RHI_DELETE(Direct3D12Rhi, this);
		}

	private:
		explicit Direct3D12Rhi(const Direct3D12Rhi&) = delete;
		Direct3D12Rhi& operator =(const Direct3D12Rhi&) = delete;

		void initializeCapabilities();

		/**
		*  @brief
		*    Unset the currently used vertex array
		*/
		void unsetGraphicsVertexArray();

#if SE_DEBUG
		/**
		*  @brief
		*    Reports information about a device object's lifetime for debugging
		*/
		void debugReportLiveDeviceObjects();
#endif

	private:
		IDXGIFactory4*			   mDxgiFactory4 = nullptr;				// DXGI factors instance, always valid for a correctly initialized RHI
		ID3D12Device*			   mD3D12Device = nullptr;				// The Direct3D 12 device, null pointer on error (we don't check because this would be a total overhead, the user has to use "Rhi::IRhi::isInitialized()" and is asked to never ever use a not properly initialized RHI)
		ID3D12CommandQueue*		   mD3D12CommandQueue = nullptr;			// The Direct3D 12 command queue, null pointer on error (we don't check because this would be a total overhead, the user has to use "Rhi::IRhi::isInitialized()" and is asked to never ever use a not properly initialized RHI)
		ID3D12CommandAllocator*	   mD3D12CommandAllocator = nullptr;
		ID3D12GraphicsCommandList* mD3D12GraphicsCommandList = nullptr;
		Rhi::IShaderLanguage*	   mShaderLanguageHlsl = nullptr;			// HLSL shader language instance (we keep a reference to it), can be a null pointer
		::detail::UploadContext	   mUploadContext;
		::detail::DescriptorHeap*  mShaderResourceViewDescriptorHeap = nullptr;
		::detail::DescriptorHeap*  mRenderTargetViewDescriptorHeap = nullptr;
		::detail::DescriptorHeap*  mDepthStencilViewDescriptorHeap = nullptr;
		::detail::DescriptorHeap*  mSamplerDescriptorHeap = nullptr;
		//[-------------------------------------------------------]
		//[ State related                                         ]
		//[-------------------------------------------------------]
		Rhi::IRenderTarget*		 mRenderTarget = nullptr;				// Output-merger (OM) stage: Currently set render target (we keep a reference to it), can be a null pointer
		D3D12_PRIMITIVE_TOPOLOGY mD3D12PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;	// State cache to avoid making redundant Direct3D 12 calls
		RootSignature*			 mGraphicsRootSignature = nullptr;	// Currently set graphics root signature (we keep a reference to it), can be a null pointer
		RootSignature*			 mComputeRootSignature = nullptr;		// Currently set compute root signature (we keep a reference to it), can be a null pointer
		VertexArray*			 mVertexArray = nullptr;				// Currently set vertex array (we keep a reference to it), can be a null pointer
#if SE_DEBUG
		bool mDebugBetweenBeginEndScene = false;	// Just here for state tracking in debug builds
#endif
	};
} // namespace Direct3D12Rhi
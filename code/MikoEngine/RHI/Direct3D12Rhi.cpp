#include "stdafx.h"
#if SE_DIRECT3D12
#include "Rhi.h"
#include "MakeID.h"
#include "Direct3D12Core.h"
#include "Direct3D12Rhi.h"
#include "Direct3D12Unsort.h"

namespace Direct3D12Rhi
{
	Direct3D12Rhi::Direct3D12Rhi(const handle& nativeWindowHandle) :
		IRhi(Rhi::NameId::DIRECT3D12, nativeWindowHandle),
		VertexArrayMakeId(),
		GraphicsPipelineStateMakeId(),
		ComputePipelineStateMakeId()		
	{
		// Enable the Direct3D 12 debug layer
#if SE_DEBUG
		{
			ID3D12Debug* d3d12Debug = nullptr;
			if ( SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&d3d12Debug))) )
			{
				d3d12Debug->EnableDebugLayer();
				d3d12Debug->Release();
			}
		}
#endif
		UINT createFactoryFlags = 0;
#if SE_DEBUG
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		// Create the DXGI factory instance
		if (SUCCEEDED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&mDxgiFactory4))))
		{				

			// Create the Direct3D 12 device
			// -> In case of failure, create an emulated device instance so we can at least test the DirectX 12 API
			if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&mD3D12Device))))
			{
				RHI_LOG(CRITICAL, "Failed to create Direct3D 12 device instance. Creating an emulated Direct3D 11 device instance instead.")

				// Create the DXGI adapter instance
				IDXGIAdapter* dxgiAdapter = nullptr;
				if (SUCCEEDED(mDxgiFactory4->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter))))
				{
					// Create the emulated Direct3D 12 device
					if (FAILED(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mD3D12Device))))
					{
						RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 device instance")
					}

					// Release the DXGI adapter instance
					dxgiAdapter->Release();
				}
				else
				{
					RHI_LOG(CRITICAL, "Failed to create Direct3D 12 DXGI adapter instance")
				}
			}
		}
		else
		{
			RHI_LOG(CRITICAL, "Failed to create Direct3D 12 DXGI factory instance")
		}

		// Is there a valid Direct3D 12 device instance?
		if (nullptr != mD3D12Device)
		{
			// Describe and create the command queue
			D3D12_COMMAND_QUEUE_DESC d3d12CommandQueueDesc;
			d3d12CommandQueueDesc.Type		= D3D12_COMMAND_LIST_TYPE_DIRECT;
			d3d12CommandQueueDesc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			d3d12CommandQueueDesc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
			d3d12CommandQueueDesc.NodeMask	= 0;
			if (SUCCEEDED(mD3D12Device->CreateCommandQueue(&d3d12CommandQueueDesc, IID_PPV_ARGS(&mD3D12CommandQueue))))
			{
				// Create the command allocator
				if (SUCCEEDED(mD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mD3D12CommandAllocator))))
				{
					// Create the command list
					if (SUCCEEDED(mD3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mD3D12CommandAllocator, nullptr, IID_PPV_ARGS(&mD3D12GraphicsCommandList))))
					{
						// Command lists are created in the recording state, but there is nothing to record yet. The main loop expects it to be closed, so close it now.
						if (SUCCEEDED(mD3D12GraphicsCommandList->Close()))
						{
							// Initialize the capabilities
							initializeCapabilities();

							// Create and begin upload context
							mUploadContext.create(*mD3D12Device);

							// Create descriptor heaps
							// TODO(co) The initial descriptor heap sizes are probably too small, additionally the descriptor heap should be able to dynamically grow during runtime (in case it can't be avoided)
							mShaderResourceViewDescriptorHeap = RHI_NEW(::detail::DescriptorHeap)(*mD3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,	64, true);
							mRenderTargetViewDescriptorHeap   = RHI_NEW(::detail::DescriptorHeap)(*mD3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV,			16, false);
							mDepthStencilViewDescriptorHeap	  = RHI_NEW(::detail::DescriptorHeap)(*mD3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV,			16, false);
							mSamplerDescriptorHeap			  = RHI_NEW(::detail::DescriptorHeap)(*mD3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,		16, true);
						}
						else
						{
							RHI_LOG(CRITICAL, "Failed to close the Direct3D 12 command list instance")
						}
					}
					else
					{
						RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 command list instance")
					}
				}
				else
				{
					RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 command allocator instance")
				}
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 command queue instance")
			}
		}
	}

	Direct3D12Rhi::~Direct3D12Rhi()
	{
		// Set no vertex array reference, in case we have one
		setGraphicsVertexArray(nullptr);

		// Release instances
		if (nullptr != mRenderTarget)
		{
			mRenderTarget->ReleaseReference();
			mRenderTarget = nullptr;
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
					RHI_LOG(CRITICAL, "The Direct3D 12 RHI implementation is going to be destroyed, but there are still %u resource instances left (memory leak)", numberOfCurrentResources)
				}
				else
				{
					RHI_LOG(CRITICAL, "The Direct3D 12 RHI implementation is going to be destroyed, but there is still one resource instance left (memory leak)")
				}

				// Use debug output to show the current number of resource instances
				getStatistics().DebugOutputCurrentResouces();
			}
		}
		#endif

		// Release the graphics and compute root signature instance, in case we have one
		if (nullptr != mGraphicsRootSignature)
		{
			mGraphicsRootSignature->ReleaseReference();
		}
		if (nullptr != mComputeRootSignature)
		{
			mComputeRootSignature->ReleaseReference();
		}

		// Destroy upload context
		mUploadContext.destroy();

		// Release descriptor heaps
		RHI_DELETE(::detail::DescriptorHeap, mShaderResourceViewDescriptorHeap);
		RHI_DELETE(::detail::DescriptorHeap, mRenderTargetViewDescriptorHeap);
		RHI_DELETE(::detail::DescriptorHeap, mDepthStencilViewDescriptorHeap);
		RHI_DELETE(::detail::DescriptorHeap, mSamplerDescriptorHeap);

		// Release the HLSL shader language instance, in case we have one
		if (nullptr != mShaderLanguageHlsl)
		{
			mShaderLanguageHlsl->ReleaseReference();
		}

		// Release the Direct3D 12 command queue we've created
		if (nullptr != mD3D12GraphicsCommandList)
		{
			mD3D12GraphicsCommandList->Release();
			mD3D12GraphicsCommandList = nullptr;
		}
		if (nullptr != mD3D12CommandAllocator)
		{
			mD3D12CommandAllocator->Release();
			mD3D12CommandAllocator = nullptr;
		}
		if (nullptr != mD3D12CommandQueue)
		{
			mD3D12CommandQueue->Release();
			mD3D12CommandQueue = nullptr;
		}

		// Release the Direct3D 12 device we've created
		if (nullptr != mD3D12Device)
		{
			mD3D12Device->Release();
			mD3D12Device = nullptr;
		}

		// Release the DXGI factory instance
		if (nullptr != mDxgiFactory4)
		{
			mDxgiFactory4->Release();
			mDxgiFactory4 = nullptr;
		}
	}


	//[-------------------------------------------------------]
	//[ Graphics                                              ]
	//[-------------------------------------------------------]
	void Direct3D12Rhi::setGraphicsRootSignature(Rhi::IRootSignature* rootSignature)
	{
		if (nullptr != mGraphicsRootSignature)
		{
			mGraphicsRootSignature->ReleaseReference();
		}
		mGraphicsRootSignature = static_cast<RootSignature*>(rootSignature);
		if (nullptr != mGraphicsRootSignature)
		{
			mGraphicsRootSignature->AddReference();

			// Sanity check
			RHI_MATCH_CHECK(*this, *rootSignature)

			// Set graphics root signature
			mD3D12GraphicsCommandList->SetGraphicsRootSignature(mGraphicsRootSignature->getD3D12RootSignature());
		}
	}

	void Direct3D12Rhi::setGraphicsPipelineState(Rhi::IGraphicsPipelineState* graphicsPipelineState)
	{
		if (nullptr != graphicsPipelineState)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *graphicsPipelineState)

			// Set primitive topology
			// -> The "Rhi::PrimitiveTopology" values directly map to Direct3D 9 & 10 & 11 && 12 constants, do not change them
			const GraphicsPipelineState* direct3D12GraphicsPipelineState = static_cast<const GraphicsPipelineState*>(graphicsPipelineState);
			if (mD3D12PrimitiveTopology != direct3D12GraphicsPipelineState->getD3D12PrimitiveTopology())
			{
				mD3D12PrimitiveTopology = direct3D12GraphicsPipelineState->getD3D12PrimitiveTopology();
				mD3D12GraphicsCommandList->IASetPrimitiveTopology(mD3D12PrimitiveTopology);
			}

			// Set graphics pipeline state
			mD3D12GraphicsCommandList->SetPipelineState(direct3D12GraphicsPipelineState->getD3D12GraphicsPipelineState());
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}

	void Direct3D12Rhi::setGraphicsResourceGroup(uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup)
	{
		// Security checks
		#if SE_DEBUG
		{
			if (nullptr == mGraphicsRootSignature)
			{
				RHI_LOG(CRITICAL, "No Direct3D 12 RHI implementation graphics root signature set")
				return;
			}
			const Rhi::RootSignature& rootSignature = mGraphicsRootSignature->getRootSignature();
			if (rootParameterIndex >= rootSignature.numberOfParameters)
			{
				RHI_LOG(CRITICAL, "The Direct3D 12 RHI implementation root parameter index is out of bounds")
				return;
			}
			const Rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			if (Rhi::RootParameterType::DESCRIPTOR_TABLE != rootParameter.parameterType)
			{
				RHI_LOG(CRITICAL, "The Direct3D 12 RHI implementation root parameter index doesn't reference a descriptor table")
				return;
			}
			if (nullptr == reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges))
			{
				RHI_LOG(CRITICAL, "The Direct3D 12 RHI implementation descriptor ranges is a null pointer")
				return;
			}
		}
		#endif

		if (nullptr != resourceGroup)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *resourceGroup)

			// Set Direct3D 12 graphics root descriptor table
			const ResourceGroup* direct3D12ResourceGroup = static_cast<ResourceGroup*>(resourceGroup);
			const ::detail::DescriptorHeap& descriptorHeap = (direct3D12ResourceGroup->getD3D12DescriptorHeapType() == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) ? *mShaderResourceViewDescriptorHeap : *mSamplerDescriptorHeap;
			D3D12_GPU_DESCRIPTOR_HANDLE d3d12GpuDescriptorHandle = descriptorHeap.getD3D12GpuDescriptorHandleForHeapStart();
			d3d12GpuDescriptorHandle.ptr += static_cast<UINT64>(direct3D12ResourceGroup->getDescriptorHeapOffset()) * descriptorHeap.getDescriptorSize();
			mD3D12GraphicsCommandList->SetGraphicsRootDescriptorTable(rootParameterIndex, d3d12GpuDescriptorHandle);
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}

	void Direct3D12Rhi::setGraphicsVertexArray(Rhi::IVertexArray* vertexArray)
	{
		// Input-assembler (IA) stage

		// New vertex array?
		if (mVertexArray != vertexArray)
		{
			if (nullptr != vertexArray)
			{
				// Sanity check
				RHI_MATCH_CHECK(*this, *vertexArray)

				// Begin debug event
				RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

				// Unset the currently used vertex array
				unsetGraphicsVertexArray();

				// Set new vertex array and add a reference to it
				mVertexArray = static_cast<VertexArray*>(vertexArray);
				mVertexArray->AddReference();

				mVertexArray->setDirect3DIASetInputLayoutAndStreamSource(*mD3D12GraphicsCommandList);

				// End debug event
				RHI_END_DEBUG_EVENT(this)
			}
			else
			{
				// Unset the currently used vertex array
				unsetGraphicsVertexArray();
			}
		}
	}

	void Direct3D12Rhi::setGraphicsViewports(uint32_t numberOfViewports, const Rhi::Viewport* viewports)
	{
		// Rasterizer (RS) stage

		// Sanity check
		RHI_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid Direct3D 12 rasterizer state viewports")

		// Set the Direct3D 12 viewports
		// -> "Rhi::Viewport" directly maps to Direct3D 12, do not change it
		// -> Let Direct3D 12 perform the index validation for us (the Direct3D 12 debug features are pretty good)
		mD3D12GraphicsCommandList->RSSetViewports(numberOfViewports, reinterpret_cast<const D3D12_VIEWPORT*>(viewports));
	}

	void Direct3D12Rhi::setGraphicsScissorRectangles(uint32_t numberOfScissorRectangles, const Rhi::ScissorRectangle* scissorRectangles)
	{
		// Rasterizer (RS) stage

		// Sanity check
		RHI_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid Direct3D 12 rasterizer state scissor rectangles")

		// Set the Direct3D 12 scissor rectangles
		// -> "Rhi::ScissorRectangle" directly maps to Direct3D 9 & 10 & 11 & 12, do not change it
		// -> Let Direct3D 12 perform the index validation for us (the Direct3D 12 debug features are pretty good)
		mD3D12GraphicsCommandList->RSSetScissorRects(numberOfScissorRectangles, reinterpret_cast<const D3D12_RECT*>(scissorRectangles));
	}

	void Direct3D12Rhi::setGraphicsRenderTarget(Rhi::IRenderTarget* renderTarget)
	{
		// Output-merger (OM) stage

		// New render target?
		if (mRenderTarget != renderTarget)
		{
			// Unset the previous render target
			if (nullptr != mRenderTarget)
			{
				// Evaluate the render target type
				switch (mRenderTarget->getResourceType())
				{
					case Rhi::ResourceType::SWAP_CHAIN:
					{
						// Get the Direct3D 12 swap chain instance
						SwapChain* swapChain = static_cast<SwapChain*>(mRenderTarget);

						// Inform Direct3D 12 about the resource transition
						CD3DX12_RESOURCE_BARRIER d3d12XResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(swapChain->getBackD3D12ResourceRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
						mD3D12GraphicsCommandList->ResourceBarrier(1, &d3d12XResourceBarrier);
						break;
					}

					case Rhi::ResourceType::FRAMEBUFFER:
					{
						// TODO(co) Implement resource transition handling (first "Direct3D12Rhi::Texture2D" needs to be cleaned up)
						/*
						// Get the Direct3D 12 framebuffer instance
						Framebuffer* framebuffer = static_cast<Framebuffer*>(mRenderTarget);

						// Inform Direct3D 12 about the resource transitions
						const uint32_t numberOfColorTextures = framebuffer->getNumberOfColorTextures();
						for (uint32_t i = 0; i < numberOfColorTextures; ++i)
						{
							// TODO(co) Resource type handling, currently only 2D texture is supported
							CD3DX12_RESOURCE_BARRIER d3d12XResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(static_cast<Texture2D*>(framebuffer->getColorTextures()[i])->getD3D12Resource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
							mD3D12GraphicsCommandList->ResourceBarrier(1, &d3d12XResourceBarrier);
						}
						if (nullptr != framebuffer->getDepthStencilTexture())
						{
							// TODO(co) Resource type handling, currently only 2D texture is supported
							CD3DX12_RESOURCE_BARRIER d3d12XResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(static_cast<Texture2D*>(framebuffer->getDepthStencilTexture())->getD3D12Resource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
							mD3D12GraphicsCommandList->ResourceBarrier(1, &d3d12XResourceBarrier);
						}
						*/
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

				// Release the render target reference, in case we have one
				mRenderTarget->ReleaseReference();
				mRenderTarget = nullptr;
			}

			// Set a render target?
			if (nullptr != renderTarget)
			{
				// Sanity check
				RHI_MATCH_CHECK(*this, *renderTarget)

				// Set new render target and add a reference to it
				mRenderTarget = renderTarget;
				mRenderTarget->AddReference();

				// Evaluate the render target type
				switch (mRenderTarget->getResourceType())
				{
					case Rhi::ResourceType::SWAP_CHAIN:
					{
						// Get the Direct3D 12 swap chain instance
						SwapChain* swapChain = static_cast<SwapChain*>(mRenderTarget);

						{ // Inform Direct3D 12 about the resource transition
							CD3DX12_RESOURCE_BARRIER d3d12XResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(swapChain->getBackD3D12ResourceRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
							mD3D12GraphicsCommandList->ResourceBarrier(1, &d3d12XResourceBarrier);
						}

						// Set Direct3D 12 render target
						CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(swapChain->getD3D12DescriptorHeapRenderTargetView()->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(swapChain->getBackD3D12ResourceRenderTargetFrameIndex()), swapChain->getRenderTargetViewDescriptorSize());
						CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(swapChain->getD3D12DescriptorHeapDepthStencilView()->GetCPUDescriptorHandleForHeapStart());
						mD3D12GraphicsCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
						break;
					}

					case Rhi::ResourceType::FRAMEBUFFER:
					{
						// Get the Direct3D 12 framebuffer instance
						Framebuffer* framebuffer = static_cast<Framebuffer*>(mRenderTarget);

						// Set the Direct3D 12 render targets
						const uint32_t numberOfColorTextures = framebuffer->getNumberOfColorTextures();
						D3D12_CPU_DESCRIPTOR_HANDLE d3d12CpuDescriptorHandlesRenderTarget[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
						for (uint32_t i = 0; i < numberOfColorTextures; ++i)
						{
							d3d12CpuDescriptorHandlesRenderTarget[i] = framebuffer->getD3D12DescriptorHeapRenderTargetViews()[i]->GetCPUDescriptorHandleForHeapStart();

							// TODO(co) Implement resource transition handling (first "Direct3D12Rhi::Texture2D" needs to be cleaned up)
							/*
							{ // Inform Direct3D 12 about the resource transition
								// TODO(co) Resource type handling, currently only 2D texture is supported
								CD3DX12_RESOURCE_BARRIER d3d12XResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(static_cast<Texture2D*>(framebuffer->getColorTextures()[i])->getD3D12Resource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
								mD3D12GraphicsCommandList->ResourceBarrier(1, &d3d12XResourceBarrier);
							}
							*/
						}
						ID3D12DescriptorHeap* d3d12DescriptorHeapDepthStencilView = framebuffer->getD3D12DescriptorHeapDepthStencilView();
						if (nullptr != d3d12DescriptorHeapDepthStencilView)
						{
							// TODO(co) Implement resource transition handling (first "Direct3D12Rhi::Texture2D" needs to be cleaned up)
							/*
							{ // Inform Direct3D 12 about the resource transition
								// TODO(co) Resource type handling, currently only 2D texture is supported
								CD3DX12_RESOURCE_BARRIER d3d12XResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(static_cast<Texture2D*>(framebuffer->getDepthStencilTexture())->getD3D12Resource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
								mD3D12GraphicsCommandList->ResourceBarrier(1, &d3d12XResourceBarrier);
							}
							*/

							// Set the Direct3D 12 render targets
							D3D12_CPU_DESCRIPTOR_HANDLE d3d12CpuDescriptorHandlesDepthStencil = d3d12DescriptorHeapDepthStencilView->GetCPUDescriptorHandleForHeapStart();
							mD3D12GraphicsCommandList->OMSetRenderTargets(numberOfColorTextures, d3d12CpuDescriptorHandlesRenderTarget, FALSE, &d3d12CpuDescriptorHandlesDepthStencil);
						}
						else
						{
							mD3D12GraphicsCommandList->OMSetRenderTargets(numberOfColorTextures, d3d12CpuDescriptorHandlesRenderTarget, FALSE, nullptr);
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
			}
			else
			{
				mD3D12GraphicsCommandList->OMSetRenderTargets(0, nullptr, FALSE, nullptr);
			}
		}
	}

	void Direct3D12Rhi::clearGraphics(uint32_t clearFlags, const float color[4], float z, uint32_t stencil)
	{
		// Unlike Direct3D 9, OpenGL or OpenGL ES 3, Direct3D 12 clears a given render target view and not the currently bound
		// -> No resource transition required in here, it's handled inside "Direct3D12Rhi::omSetRenderTarget()"

		// Sanity check
		RHI_ASSERT(z >= 0.0f && z <= 1.0f, "The Direct3D 12 clear graphics z value must be between [0, 1] (inclusive)")

		// Begin debug event
		RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

		// Render target set?
		if (nullptr != mRenderTarget)
		{
			// Evaluate the render target type
			switch (mRenderTarget->getResourceType())
			{
				case Rhi::ResourceType::SWAP_CHAIN:
				{
					// Get the Direct3D 12 swap chain instance
					SwapChain* swapChain = static_cast<SwapChain*>(mRenderTarget);

					// Clear the Direct3D 12 render target view?
					if (clearFlags & Rhi::ClearFlag::COLOR)
					{
						CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(swapChain->getD3D12DescriptorHeapRenderTargetView()->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(swapChain->getBackD3D12ResourceRenderTargetFrameIndex()), swapChain->getRenderTargetViewDescriptorSize());
						mD3D12GraphicsCommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
					}

					{ // Clear the Direct3D 12 depth stencil view?
						ID3D12DescriptorHeap* d3d12DescriptorHeapDepthStencilView = swapChain->getD3D12DescriptorHeapDepthStencilView();
						if (nullptr != d3d12DescriptorHeapDepthStencilView)
						{
							// Get the Direct3D 12 clear flags
							UINT direct3D12ClearFlags = (clearFlags & Rhi::ClearFlag::DEPTH) ? D3D12_CLEAR_FLAG_DEPTH : 0u;
							if (clearFlags & Rhi::ClearFlag::STENCIL)
							{
								direct3D12ClearFlags |= D3D12_CLEAR_FLAG_STENCIL;
							}
							if (0 != direct3D12ClearFlags)
							{
								// Clear the Direct3D 12 depth stencil view
								mD3D12GraphicsCommandList->ClearDepthStencilView(d3d12DescriptorHeapDepthStencilView->GetCPUDescriptorHandleForHeapStart(), static_cast<D3D12_CLEAR_FLAGS>(direct3D12ClearFlags), z, static_cast<UINT8>(stencil), 0, nullptr);
							}
						}
					}
					break;
				}

				case Rhi::ResourceType::FRAMEBUFFER:
				{
					// Get the Direct3D 12 framebuffer instance
					Framebuffer* framebuffer = static_cast<Framebuffer*>(mRenderTarget);

					// Clear all Direct3D 12 render target views?
					if (clearFlags & Rhi::ClearFlag::COLOR)
					{
						// Loop through all Direct3D 12 render target views
						ID3D12DescriptorHeap **d3d12DescriptorHeapRenderTargetViews = framebuffer->getD3D12DescriptorHeapRenderTargetViews() + framebuffer->getNumberOfColorTextures();
						for (ID3D12DescriptorHeap **d3d12DescriptorHeapRenderTargetView = framebuffer->getD3D12DescriptorHeapRenderTargetViews(); d3d12DescriptorHeapRenderTargetView < d3d12DescriptorHeapRenderTargetViews; ++d3d12DescriptorHeapRenderTargetView)
						{
							// Valid Direct3D 12 render target view?
							if (nullptr != *d3d12DescriptorHeapRenderTargetView)
							{
								mD3D12GraphicsCommandList->ClearRenderTargetView((*d3d12DescriptorHeapRenderTargetView)->GetCPUDescriptorHandleForHeapStart(), color, 0, nullptr);
							}
						}
					}

					// Clear the Direct3D 12 depth stencil view?
					ID3D12DescriptorHeap* d3d12DescriptorHeapDepthStencilView = framebuffer->getD3D12DescriptorHeapDepthStencilView();
					if (nullptr != d3d12DescriptorHeapDepthStencilView)
					{
						// Get the Direct3D 12 clear flags
						UINT direct3D12ClearFlags = (clearFlags & Rhi::ClearFlag::DEPTH) ? D3D12_CLEAR_FLAG_DEPTH : 0u;
						if (clearFlags & Rhi::ClearFlag::STENCIL)
						{
							direct3D12ClearFlags |= D3D12_CLEAR_FLAG_STENCIL;
						}
						if (0 != direct3D12ClearFlags)
						{
							// Clear the Direct3D 12 depth stencil view
							mD3D12GraphicsCommandList->ClearDepthStencilView(d3d12DescriptorHeapDepthStencilView->GetCPUDescriptorHandleForHeapStart(), static_cast<D3D12_CLEAR_FLAGS>(direct3D12ClearFlags), z, static_cast<UINT8>(stencil), 0, nullptr);
						}
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
		}
		else
		{
			// In case no render target is currently set we don't have to do anything in here
		}

		// End debug event
		RHI_END_DEBUG_EVENT(this)
	}

	void Direct3D12Rhi::drawGraphics(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, indirectBuffer)
		RHI_ASSERT(numberOfDraws > 0, "Number of Direct3D 12 draws must not be zero")
		// It's possible to draw without "mVertexArray"

		// Execute Direct3D 12 indirect
		const IndirectBuffer& d3d12IndirectBuffer = static_cast<const IndirectBuffer&>(indirectBuffer);
		mD3D12GraphicsCommandList->ExecuteIndirect(d3d12IndirectBuffer.getD3D12CommandSignature(), numberOfDraws, d3d12IndirectBuffer.getD3D12Resource(), indirectBufferOffset, nullptr, 0);
	}

	void Direct3D12Rhi::drawGraphicsEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != emulationData, "The Direct3D 12 emulation data must be valid")
		RHI_ASSERT(numberOfDraws > 0, "The number of Direct3D 12 draws must not be zero")

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

			// Draw
			mD3D12GraphicsCommandList->DrawInstanced(
				drawArguments.vertexCountPerInstance,	// Vertex count per instance (UINT)
				drawArguments.instanceCount,			// Instance count (UINT)
				drawArguments.startVertexLocation,		// Start vertex location (UINT)
				drawArguments.startInstanceLocation		// Start instance location (UINT)
			);

			// Advance
			emulationData += sizeof(Rhi::DrawArguments);
		}
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
		#endif
	}

	void Direct3D12Rhi::drawIndexedGraphics(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, indirectBuffer)
		RHI_ASSERT(numberOfDraws > 0, "Number of Direct3D 12 draws must not be zero")
		RHI_ASSERT(nullptr != mVertexArray, "Direct3D 12 draw indexed needs a set vertex array")
		RHI_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "Direct3D 12 draw indexed needs a set vertex array which contains an index buffer")

		// Execute Direct3D 12 indirect
		const IndirectBuffer& d3d12IndirectBuffer = static_cast<const IndirectBuffer&>(indirectBuffer);
		mD3D12GraphicsCommandList->ExecuteIndirect(d3d12IndirectBuffer.getD3D12CommandSignature(), numberOfDraws, d3d12IndirectBuffer.getD3D12Resource(), indirectBufferOffset, nullptr, 0);
	}

	void Direct3D12Rhi::drawIndexedGraphicsEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != emulationData, "The Direct3D 12 emulation data must be valid")
		RHI_ASSERT(numberOfDraws > 0, "The number of Direct3D 12 draws must not be zero")

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
			const Rhi::DrawIndexedArguments& drawIndexedArguments = *reinterpret_cast<const Rhi::DrawIndexedArguments*>(emulationData);

			// Draw
			mD3D12GraphicsCommandList->DrawIndexedInstanced(
				drawIndexedArguments.indexCountPerInstance,	// Index count per instance (UINT)
				drawIndexedArguments.instanceCount,			// Instance count (UINT)
				drawIndexedArguments.startIndexLocation,	// Start index location (UINT)
				drawIndexedArguments.baseVertexLocation,	// Base vertex location (INT)
				drawIndexedArguments.startInstanceLocation	// Start instance location (UINT)
			);

			// Advance
			emulationData += sizeof(Rhi::DrawIndexedArguments);
		}
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
		#endif
	}

	void Direct3D12Rhi::drawMeshTasks([[maybe_unused]] const Rhi::IIndirectBuffer& indirectBuffer, [[maybe_unused]] uint32_t indirectBufferOffset, [[maybe_unused]] uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")

		// TODO(co) Implement me
	}

	void Direct3D12Rhi::drawMeshTasksEmulated([[maybe_unused]] const uint8_t* emulationData, uint32_t, [[maybe_unused]] uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != emulationData, "The null emulation data must be valid")
		RHI_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")

		// TODO(co) Implement me
	}


	//[-------------------------------------------------------]
	//[ Compute                                               ]
	//[-------------------------------------------------------]
	void Direct3D12Rhi::setComputeRootSignature(Rhi::IRootSignature* rootSignature)
	{
		if (nullptr != mComputeRootSignature)
		{
			mComputeRootSignature->ReleaseReference();
		}
		mComputeRootSignature = static_cast<RootSignature*>(rootSignature);
		if (nullptr != mComputeRootSignature)
		{
			mComputeRootSignature->AddReference();

			// Sanity check
			RHI_MATCH_CHECK(*this, *rootSignature)

			// Set compute root signature
			mD3D12GraphicsCommandList->SetComputeRootSignature(mComputeRootSignature->getD3D12RootSignature());
		}
	}

	void Direct3D12Rhi::setComputePipelineState(Rhi::IComputePipelineState* computePipelineState)
	{
		if (nullptr != computePipelineState)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *computePipelineState)

			// Set compute pipeline state
			mD3D12GraphicsCommandList->SetPipelineState(static_cast<ComputePipelineState*>(computePipelineState)->getD3D12ComputePipelineState());
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}

	void Direct3D12Rhi::setComputeResourceGroup(uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup)
	{
		// Security checks
		#if SE_DEBUG
		{
			if (nullptr == mComputeRootSignature)
			{
				RHI_LOG(CRITICAL, "No Direct3D 12 RHI implementation compute root signature set")
				return;
			}
			const Rhi::RootSignature& rootSignature = mComputeRootSignature->getRootSignature();
			if (rootParameterIndex >= rootSignature.numberOfParameters)
			{
				RHI_LOG(CRITICAL, "The Direct3D 12 RHI implementation root parameter index is out of bounds")
				return;
			}
			const Rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			if (Rhi::RootParameterType::DESCRIPTOR_TABLE != rootParameter.parameterType)
			{
				RHI_LOG(CRITICAL, "The Direct3D 12 RHI implementation root parameter index doesn't reference a descriptor table")
				return;
			}
			if (nullptr == reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges))
			{
				RHI_LOG(CRITICAL, "The Direct3D 12 RHI implementation descriptor ranges is a null pointer")
				return;
			}
		}
		#endif

		if (nullptr != resourceGroup)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *resourceGroup)

			// Set Direct3D 12 compute root descriptor table
			const ResourceGroup* direct3D12ResourceGroup = static_cast<ResourceGroup*>(resourceGroup);
			const ::detail::DescriptorHeap& descriptorHeap = (direct3D12ResourceGroup->getD3D12DescriptorHeapType() == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) ? *mShaderResourceViewDescriptorHeap : *mSamplerDescriptorHeap;
			D3D12_GPU_DESCRIPTOR_HANDLE d3d12GpuDescriptorHandle = descriptorHeap.getD3D12GpuDescriptorHandleForHeapStart();
			d3d12GpuDescriptorHandle.ptr += static_cast<UINT64>(direct3D12ResourceGroup->getDescriptorHeapOffset()) * descriptorHeap.getDescriptorSize();
			mD3D12GraphicsCommandList->SetComputeRootDescriptorTable(rootParameterIndex, d3d12GpuDescriptorHandle);
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}

	void Direct3D12Rhi::dispatchCompute(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		mD3D12GraphicsCommandList->Dispatch(groupCountX, groupCountY, groupCountZ);
	}


	//[-------------------------------------------------------]
	//[ Resource                                              ]
	//[-------------------------------------------------------]
	void Direct3D12Rhi::resolveMultisampleFramebuffer(Rhi::IRenderTarget&, Rhi::IFramebuffer&)
	{
		// TODO(co) Implement me
	}

	void Direct3D12Rhi::copyResource(Rhi::IResource&, Rhi::IResource&)
	{
		// TODO(co) Implement me
	}

	void Direct3D12Rhi::generateMipmaps(Rhi::IResource&)
	{
		// TODO(co) Implement me
	}


	//[-------------------------------------------------------]
	//[ Query                                                 ]
	//[-------------------------------------------------------]
	void Direct3D12Rhi::resetQueryPool([[maybe_unused]] Rhi::IQueryPool& queryPool, [[maybe_unused]] uint32_t firstQueryIndex, [[maybe_unused]] uint32_t numberOfQueries)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, queryPool)
		RHI_ASSERT(firstQueryIndex < static_cast<const QueryPool&>(queryPool).getNumberOfQueries(), "Direct3D 12 out-of-bounds query index")
		RHI_ASSERT((firstQueryIndex + numberOfQueries) <= static_cast<const QueryPool&>(queryPool).getNumberOfQueries(), "Direct3D 12 out-of-bounds query index")

		// Nothing to do in here for Direct3D 12
	}

	void Direct3D12Rhi::beginQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex, uint32_t)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Query pool type dependent processing
		const QueryPool& d3d12QueryPool = static_cast<const QueryPool&>(queryPool);
		RHI_ASSERT(queryIndex < d3d12QueryPool.getNumberOfQueries(), "Direct3D 12 out-of-bounds query index")
		switch (d3d12QueryPool.getQueryType())
		{
			case Rhi::QueryType::OCCLUSION:
				mD3D12GraphicsCommandList->BeginQuery(d3d12QueryPool.getD3D12QueryHeap(), D3D12_QUERY_TYPE_OCCLUSION, queryIndex);
				break;

			case Rhi::QueryType::PIPELINE_STATISTICS:
				mD3D12GraphicsCommandList->BeginQuery(d3d12QueryPool.getD3D12QueryHeap(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, queryIndex);
				break;

			case Rhi::QueryType::TIMESTAMP:
				RHI_ASSERT(false, "Direct3D 12 begin query isn't allowed for timestamp queries, use \"Rhi::Command::WriteTimestampQuery\" instead")
				break;
		}
	}

	void Direct3D12Rhi::endQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Query pool type dependent processing
		const QueryPool& d3d12QueryPool = static_cast<const QueryPool&>(queryPool);
		RHI_ASSERT(queryIndex < d3d12QueryPool.getNumberOfQueries(), "Direct3D 12 out-of-bounds query index")
		switch (d3d12QueryPool.getQueryType())
		{
			case Rhi::QueryType::OCCLUSION:
				mD3D12GraphicsCommandList->EndQuery(d3d12QueryPool.getD3D12QueryHeap(), D3D12_QUERY_TYPE_OCCLUSION, queryIndex);
				break;

			case Rhi::QueryType::PIPELINE_STATISTICS:
				mD3D12GraphicsCommandList->EndQuery(d3d12QueryPool.getD3D12QueryHeap(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, queryIndex);
				break;

			case Rhi::QueryType::TIMESTAMP:
				RHI_ASSERT(false, "Direct3D 12 end query isn't allowed for timestamp queries, use \"Rhi::Command::WriteTimestampQuery\" instead")
				break;
		}
	}

	void Direct3D12Rhi::writeTimestampQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Query pool type dependent processing
		const QueryPool& d3d12QueryPool = static_cast<const QueryPool&>(queryPool);
		RHI_ASSERT(queryIndex < d3d12QueryPool.getNumberOfQueries(), "Direct3D 12 out-of-bounds query index")
		switch (d3d12QueryPool.getQueryType())
		{
			case Rhi::QueryType::OCCLUSION:
				RHI_ASSERT(false, "Direct3D 12 write timestamp query isn't allowed for occlusion queries, use \"Rhi::Command::BeginQuery\" and \"Rhi::Command::EndQuery\" instead")
				break;

			case Rhi::QueryType::PIPELINE_STATISTICS:
				RHI_ASSERT(false, "Direct3D 12 write timestamp query isn't allowed for pipeline statistics queries, use \"Rhi::Command::BeginQuery\" and \"Rhi::Command::EndQuery\" instead")
				break;

			case Rhi::QueryType::TIMESTAMP:
				mD3D12GraphicsCommandList->EndQuery(d3d12QueryPool.getD3D12QueryHeap(), D3D12_QUERY_TYPE_TIMESTAMP, queryIndex);
				break;
		}
	}


	//[-------------------------------------------------------]
	//[ Debug                                                 ]
	//[-------------------------------------------------------]
	#if SE_DEBUG
		void Direct3D12Rhi::setDebugMarker(const char* name)
		{
			if (nullptr != mD3D12GraphicsCommandList)
			{
				RHI_ASSERT(nullptr != name, "Direct3D 12 debug marker names must not be a null pointer")
				const UINT size = static_cast<UINT>((strlen(name) + 1) * sizeof(name[0]));
				mD3D12GraphicsCommandList->SetMarker(DirectX::Detail::PIX_EVENT_ANSI_VERSION, name, size);
			}
		}

		void Direct3D12Rhi::beginDebugEvent(const char* name)
		{
			if (nullptr != mD3D12GraphicsCommandList)
			{
				RHI_ASSERT(nullptr != name, "Direct3D 12 debug event names must not be a null pointer")
				const UINT size = static_cast<UINT>((strlen(name) + 1) * sizeof(name[0]));
				mD3D12GraphicsCommandList->BeginEvent(DirectX::Detail::PIX_EVENT_ANSI_VERSION, name, size);
			}
		}

		void Direct3D12Rhi::endDebugEvent()
		{
			if (nullptr != mD3D12GraphicsCommandList)
			{
				mD3D12GraphicsCommandList->EndEvent();
			}
		}
	#endif


	//[-------------------------------------------------------]
	//[ Public virtual Rhi::IRhi methods                      ]
	//[-------------------------------------------------------]
	bool Direct3D12Rhi::isDebugEnabled()
	{
		#if SE_DEBUG
			return true;
		#else
			return false;
		#endif
	}


	//[-------------------------------------------------------]
	//[ Shader language                                       ]
	//[-------------------------------------------------------]
	uint32_t Direct3D12Rhi::getNumberOfShaderLanguages() const
	{
		uint32_t numberOfShaderLanguages = 1;	// HLSL support is always there

		// Done, return the number of supported shader languages
		return numberOfShaderLanguages;
	}

	const char* Direct3D12Rhi::getShaderLanguageName([[maybe_unused]] uint32_t index) const
	{
		RHI_ASSERT(index < getNumberOfShaderLanguages(), "Direct3D 12: Shader language index is out-of-bounds")
		return ::detail::HLSL_NAME;
	}

	Rhi::IShaderLanguage* Direct3D12Rhi::getShaderLanguage(const char* shaderLanguageName)
	{
		// In case "shaderLanguage" is a null pointer, use the default shader language
		if (nullptr != shaderLanguageName)
		{
			// Optimization: Check for shader language name pointer match, first
			if (::detail::HLSL_NAME == shaderLanguageName || !stricmp(shaderLanguageName, ::detail::HLSL_NAME))
			{
				// If required, create the HLSL shader language instance right now
				if (nullptr == mShaderLanguageHlsl)
				{
					mShaderLanguageHlsl = RHI_NEW(ShaderLanguageHlsl)(*this);
					mShaderLanguageHlsl->AddReference();	// Internal RHI reference
				}

				// Return the shader language instance
				return mShaderLanguageHlsl;
			}

			// Error!
			return nullptr;
		}

		// Return the HLSL shader language instance as default
		return getShaderLanguage(::detail::HLSL_NAME);
	}


	//[-------------------------------------------------------]
	//[ Resource creation                                     ]
	//[-------------------------------------------------------]
	Rhi::IRenderPass* Direct3D12Rhi::createRenderPass(uint32_t numberOfColorAttachments, const Rhi::TextureFormat::Enum* colorAttachmentTextureFormats, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, [[maybe_unused]] uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		return RHI_NEW(RenderPass)(*this, numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IQueryPool* Direct3D12Rhi::createQueryPool([[maybe_unused]] Rhi::QueryType queryType, [[maybe_unused]] uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		RHI_ASSERT(numberOfQueries > 0, "Direct3D 12: Number of queries mustn't be zero")
		return RHI_NEW(QueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::ISwapChain* Direct3D12Rhi::createSwapChain(Rhi::IRenderPass& renderPass, Rhi::WindowHandle windowHandle, bool RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, renderPass)
		RHI_ASSERT(SE_NULL_HANDLE != windowHandle.nativeWindowHandle, "Direct3D 12: The provided native window handle must not be a null handle")

		// Create the swap chain
		return RHI_NEW(SwapChain)(renderPass, windowHandle RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IFramebuffer* Direct3D12Rhi::createFramebuffer(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, renderPass)

		// Create the framebuffer
		return RHI_NEW(Framebuffer)(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IBufferManager* Direct3D12Rhi::createBufferManager()
	{
		return RHI_NEW(BufferManager)(*this);
	}

	Rhi::ITextureManager* Direct3D12Rhi::createTextureManager()
	{
		return RHI_NEW(TextureManager)(*this);
	}

	Rhi::IRootSignature* Direct3D12Rhi::createRootSignature(const Rhi::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		return RHI_NEW(RootSignature)(*this, rootSignature RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IGraphicsPipelineState* Direct3D12Rhi::createGraphicsPipelineState(const Rhi::GraphicsPipelineState& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		RHI_ASSERT(nullptr != graphicsPipelineState.rootSignature, "Direct3D 12: Invalid graphics pipeline state root signature")
		RHI_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "Direct3D 12: Invalid graphics pipeline state graphics program")
		RHI_ASSERT(nullptr != graphicsPipelineState.renderPass, "Direct3D 12: Invalid graphics pipeline state render pass")

		// Create graphics pipeline state
		uint16_t id = 0;
		if (GraphicsPipelineStateMakeId.CreateID(id))
		{
			return RHI_NEW(GraphicsPipelineState)(*this, graphicsPipelineState, id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		// Error: Ensure a correct reference counter behaviour
		graphicsPipelineState.rootSignature->AddReference();
		graphicsPipelineState.rootSignature->ReleaseReference();
		graphicsPipelineState.graphicsProgram->AddReference();
		graphicsPipelineState.graphicsProgram->ReleaseReference();
		graphicsPipelineState.renderPass->AddReference();
		graphicsPipelineState.renderPass->ReleaseReference();
		return nullptr;
	}

	Rhi::IComputePipelineState* Direct3D12Rhi::createComputePipelineState(Rhi::IRootSignature& rootSignature, Rhi::IComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, rootSignature)
		RHI_MATCH_CHECK(*this, computeShader)

		// Create the compute pipeline state
		uint16_t id = 0;
		if (ComputePipelineStateMakeId.CreateID(id))
		{
			return RHI_NEW(ComputePipelineState)(*this, rootSignature, computeShader, id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		// Error: Ensure a correct reference counter behaviour
		rootSignature.AddReference();
		rootSignature.ReleaseReference();
		computeShader.AddReference();
		computeShader.ReleaseReference();
		return nullptr;
	}

	Rhi::ISamplerState* Direct3D12Rhi::createSamplerState(const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// No debug name possible since all sampler states are inside a descriptor heap
		return RHI_NEW(SamplerState)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}


	//[-------------------------------------------------------]
	//[ Resource handling                                     ]
	//[-------------------------------------------------------]
	bool Direct3D12Rhi::map(Rhi::IResource& resource, uint32_t, Rhi::MapType, uint32_t, Rhi::MappedSubresource& mappedSubresource)
	{
		// The "Rhi::MapType" values directly map to Direct3D 10 & 11 constants, do not change them
		// The "Rhi::MappedSubresource" structure directly maps to Direct3D 11, do not change it

		// Define helper macro
		// TODO(co) Port to Direct3D 12
		#define TEXTURE_RESOURCE(type, typeClass) \
			case type: \
			{ \
				return false;	\
			}

		// Evaluate the resource type
		switch (resource.getResourceType())
		{
			case Rhi::ResourceType::VERTEX_BUFFER:
			{
				mappedSubresource.rowPitch = 0;
				mappedSubresource.depthPitch = 0;
				CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
				return SUCCEEDED(static_cast<VertexBuffer&>(resource).getD3D12Resource()->Map(0, &readRange, reinterpret_cast<void**>(&mappedSubresource.data)));
			}

			case Rhi::ResourceType::INDEX_BUFFER:
			{
				mappedSubresource.rowPitch = 0;
				mappedSubresource.depthPitch = 0;
				CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
				return SUCCEEDED(static_cast<IndexBuffer&>(resource).getD3D12Resource()->Map(0, &readRange, reinterpret_cast<void**>(&mappedSubresource.data)));
			}

			case Rhi::ResourceType::TEXTURE_BUFFER:
			{
				mappedSubresource.rowPitch = 0;
				mappedSubresource.depthPitch = 0;
				CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
				// TODO(co) Port to Direct3D 12
				return false;
				// return SUCCEEDED(static_cast<TextureBuffer&>(resource).getD3D12Resource()->Map(0, &readRange, reinterpret_cast<void**>(&mappedSubresource.data)));
			}

			case Rhi::ResourceType::STRUCTURED_BUFFER:
			{
				mappedSubresource.rowPitch = 0;
				mappedSubresource.depthPitch = 0;
				CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
				// TODO(co) Port to Direct3D 12
				return false;
				// return SUCCEEDED(static_cast<StructuredBuffer&>(resource).getD3D12Resource()->Map(0, &readRange, reinterpret_cast<void**>(&mappedSubresource.data)));
			}

			case Rhi::ResourceType::INDIRECT_BUFFER:
			{
				mappedSubresource.rowPitch = 0;
				mappedSubresource.depthPitch = 0;
				CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
				return SUCCEEDED(static_cast<IndirectBuffer&>(resource).getD3D12Resource()->Map(0, &readRange, reinterpret_cast<void**>(&mappedSubresource.data)));
			}

			case Rhi::ResourceType::UNIFORM_BUFFER:
			{
				mappedSubresource.rowPitch = 0;
				mappedSubresource.depthPitch = 0;
				CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
				return SUCCEEDED(static_cast<UniformBuffer&>(resource).getD3D12Resource()->Map(0, &readRange, reinterpret_cast<void**>(&mappedSubresource.data)));
			}

			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_1D, Texture1D)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_1D_ARRAY, Texture1DArray)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_2D, Texture2D)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_2D_ARRAY, Texture2DArray)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_3D, Texture3D)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_CUBE, TextureCube)
			//TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_CUBE_ARRAY, TextureCubeArray)	// TODO(co) Implement me
			case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:

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

		// Undefine helper macro
		#undef TEXTURE_RESOURCE
	}

	void Direct3D12Rhi::unmap(Rhi::IResource& resource, uint32_t)
	{
		// Define helper macro
		// TODO(co) Port to Direct3D 12
		#define TEXTURE_RESOURCE(type, typeClass) \
			case type: \
			{ \
				break; \
			}

		// Evaluate the resource type
		switch (resource.getResourceType())
		{
			case Rhi::ResourceType::VERTEX_BUFFER:
				static_cast<VertexBuffer&>(resource).getD3D12Resource()->Unmap(0, nullptr);
				break;

			case Rhi::ResourceType::INDEX_BUFFER:
				static_cast<IndexBuffer&>(resource).getD3D12Resource()->Unmap(0, nullptr);
				break;

			case Rhi::ResourceType::TEXTURE_BUFFER:
				// TODO(co) Port to Direct3D 12
				// static_cast<TextureBuffer&>(resource).getD3D12Resource()->Unmap(0, nullptr);
				break;

			case Rhi::ResourceType::STRUCTURED_BUFFER:
				// TODO(co) Port to Direct3D 12
				// static_cast<StructuredBuffer&>(resource).getD3D12Resource()->Unmap(0, nullptr);
				break;

			case Rhi::ResourceType::INDIRECT_BUFFER:
				static_cast<IndirectBuffer&>(resource).getD3D12Resource()->Unmap(0, nullptr);
				break;

			case Rhi::ResourceType::UNIFORM_BUFFER:
				static_cast<UniformBuffer&>(resource).getD3D12Resource()->Unmap(0, nullptr);
				break;

			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_1D, Texture1D)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_1D_ARRAY, Texture1DArray)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_2D, Texture2D)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_2D_ARRAY, Texture2DArray)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_3D, Texture3D)
			TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_CUBE, TextureCube)
			// TEXTURE_RESOURCE(Rhi::ResourceType::TEXTURE_CUBE_ARRAY, TextureCubeArray)	// TODO(co) Implement me
			case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:

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

		// Undefine helper macro
		#undef TEXTURE_RESOURCE
	}

	bool Direct3D12Rhi::getQueryPoolResults(Rhi::IQueryPool& queryPool, uint32_t numberOfDataBytes, uint8_t* data, uint32_t firstQueryIndex, uint32_t numberOfQueries, uint32_t strideInBytes, uint32_t)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, queryPool)
		RHI_ASSERT(numberOfDataBytes >= sizeof(UINT64), "Direct3D 12 out-of-memory query access")
		RHI_ASSERT(1 == numberOfQueries || strideInBytes > 0, "Direct3D 12 invalid stride in bytes")
		RHI_ASSERT(numberOfDataBytes >= strideInBytes * numberOfQueries, "Direct3D 12 out-of-memory query access")
		RHI_ASSERT(nullptr != data, "Direct3D 12 out-of-memory query access")
		RHI_ASSERT(numberOfQueries > 0, "Direct3D 12 number of queries mustn't be zero")

		// Get query pool results
		static_cast<QueryPool&>(queryPool).getQueryPoolResults(numberOfDataBytes, data, firstQueryIndex, numberOfQueries, strideInBytes, *mD3D12GraphicsCommandList);

		// Done
		return true;
	}


	//[-------------------------------------------------------]
	//[ Operations                                            ]
	//[-------------------------------------------------------]
	bool Direct3D12Rhi::beginScene()
	{
		bool result = false;	// Error by default

		// Sanity check
		#if SE_DEBUG
			RHI_ASSERT(false == mDebugBetweenBeginEndScene, "Direct3D 12: Begin scene was called while scene rendering is already in progress, missing end scene call?")
			mDebugBetweenBeginEndScene = true;
		#endif

		// Not required when using Direct3D 12
		// TODO(co) Until we have a command list interface, we must perform the command list handling in here

		// Command list allocators can only be reset when the associated
		// command lists have finished execution on the GPU; apps should use
		// fences to determine GPU execution progress.
		if (SUCCEEDED(mD3D12CommandAllocator->Reset()))
		{
			// However, when ExecuteCommandList() is called on a particular command
			// list, that command list can then be reset at any time and must be before
			// re-recording.
			result = SUCCEEDED(mD3D12GraphicsCommandList->Reset(mD3D12CommandAllocator, nullptr));
			if (result)
			{
				// Set descriptor heaps
				ID3D12DescriptorHeap* d3d12DescriptorHeaps[] = { mShaderResourceViewDescriptorHeap->getD3D12DescriptorHeap(), mSamplerDescriptorHeap->getD3D12DescriptorHeap() };
				mD3D12GraphicsCommandList->SetDescriptorHeaps(2, d3d12DescriptorHeaps);
			}
		}

		// Reset our cached states where needed
		mD3D12PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

		// Done
		return result;
	}

	void Direct3D12Rhi::submitCommandBuffer(const Rhi::CommandBuffer& commandBuffer)
	{
		// Sanity check
		RHI_ASSERT(!commandBuffer.isEmpty(), "The Direct3D 12 command buffer to execute mustn't be empty")

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

	void Direct3D12Rhi::endScene()
	{
		// Sanity check
		#if SE_DEBUG
			RHI_ASSERT(true == mDebugBetweenBeginEndScene, "Direct3D 12: End scene was called while scene rendering isn't in progress, missing start scene call?")
			mDebugBetweenBeginEndScene = false;
		#endif

		// Begin debug event
		RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

		// Finish previous uploads and start new ones
		ID3D12CommandList* uploadD3d12CommandList = mUploadContext.getD3d12GraphicsCommandList();
		mUploadContext.begin();

		// We need to forget about the currently set render target
		setGraphicsRenderTarget(nullptr);

		// We need to forget about the currently set vertex array
		unsetGraphicsVertexArray();

		// End debug event
		RHI_END_DEBUG_EVENT(this)

		// Close and execute the command list
		if (SUCCEEDED(mD3D12GraphicsCommandList->Close()))
		{
			if (nullptr != uploadD3d12CommandList)
			{
				ID3D12CommandList* commandLists[] = { uploadD3d12CommandList, mD3D12GraphicsCommandList };
				mD3D12CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
			}
			else
			{
				ID3D12CommandList* commandLists[] = { mD3D12GraphicsCommandList };
				mD3D12CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
			}
		}

		// Release the graphics and compute root signature instance, in case we have one
		if (nullptr != mGraphicsRootSignature)
		{
			mGraphicsRootSignature->ReleaseReference();
			mGraphicsRootSignature = nullptr;
		}
		if (nullptr != mComputeRootSignature)
		{
			mComputeRootSignature->ReleaseReference();
			mComputeRootSignature = nullptr;
		}
	}


	//[-------------------------------------------------------]
	//[ Synchronization                                       ]
	//[-------------------------------------------------------]
	void Direct3D12Rhi::flush()
	{
		// TODO(co) Implement me
	}

	void Direct3D12Rhi::finish()
	{
		// TODO(co) Implement me
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void Direct3D12Rhi::initializeCapabilities()
	{
		// TODO(co) Direct3D 12 update

		// There are no Direct3D 12 device capabilities we could query on runtime, they depend on the chosen feature level
		// -> Have a look at "Devices -> Direct3D 12 on Downlevel Hardware -> Introduction" at MSDN http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx
		//    for a table with a list of the minimum resources supported by Direct3D 12 at the different feature levels

		{ // Get device name
			// Get DXGI adapter
			IDXGIAdapter* dxgiAdapter = nullptr;
			mDxgiFactory4->EnumAdapterByLuid(mD3D12Device->GetAdapterLuid(), IID_PPV_ARGS(&dxgiAdapter));

			// The adapter contains a description like "AMD Radeon R9 200 Series"
			DXGI_ADAPTER_DESC dxgiAdapterDesc = {};
			dxgiAdapter->GetDesc(&dxgiAdapterDesc);

			// Convert UTF-16 string to UTF-8
			const size_t numberOfCharacters = _countof(mCapabilities.deviceName) - 1;
			::WideCharToMultiByte(CP_UTF8, 0, dxgiAdapterDesc.Description, static_cast<int>(wcslen(dxgiAdapterDesc.Description)), mCapabilities.deviceName, static_cast<int>(numberOfCharacters), nullptr, nullptr);
			mCapabilities.deviceName[numberOfCharacters] = '\0';

			// Release references
			dxgiAdapter->Release();
		}

		// Preferred swap chain texture format
		mCapabilities.preferredSwapChainColorTextureFormat		  = Rhi::TextureFormat::Enum::R8G8B8A8;
		mCapabilities.preferredSwapChainDepthStencilTextureFormat = Rhi::TextureFormat::Enum::D32_FLOAT;

		// Evaluate the chosen feature level
		switch (D3D_FEATURE_LEVEL_12_0)
		// switch (mD3D12Device->GetFeatureLevel())	// TODO(co) Direct3D 12 update
		{
			case D3D_FEATURE_LEVEL_9_1:
				// Maximum number of viewports (always at least 1)
				mCapabilities.maximumNumberOfViewports = 1;	// Direct3D 9 only supports a single viewport

				// Maximum number of simultaneous render targets (if <1 render to texture is not supported)
				mCapabilities.maximumNumberOfSimultaneousRenderTargets = 1;

				// Maximum texture dimension
				mCapabilities.maximumTextureDimension = 2048;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 0;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 0;

				// Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
				mCapabilities.maximumNumberOf2DTextureArraySlices = 0;

				// Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
				mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;

				// Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
				mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 0;

				// Maximum indirect buffer size in bytes
				mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

				// Maximum number of multisamples (always at least 1, usually 8)
				mCapabilities.maximumNumberOfMultisamples = 1;	// Don't want to support the legacy DirectX 9 multisample support

				// Maximum anisotropy (always at least 1, usually 16)
				mCapabilities.maximumAnisotropy = 16;

				// Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
				mCapabilities.instancedArrays = false;

				// Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
				mCapabilities.drawInstanced = false;

				// Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
				mCapabilities.maximumNumberOfPatchVertices = 0;	// Direct3D 9.1 has no tessellation support

				// Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
				mCapabilities.maximumNumberOfGsOutputVertices = 0;	// Direct3D 9.1 has no geometry shader support
				break;

			case D3D_FEATURE_LEVEL_9_2:
				// Maximum number of viewports (always at least 1)
				mCapabilities.maximumNumberOfViewports = 1;	// Direct3D 9 only supports a single viewport

				// Maximum number of simultaneous render targets (if <1 render to texture is not supported)
				mCapabilities.maximumNumberOfSimultaneousRenderTargets = 1;

				// Maximum texture dimension
				mCapabilities.maximumTextureDimension = 2048;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 0;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 0;

				// Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
				mCapabilities.maximumNumberOf2DTextureArraySlices = 0;

				// Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
				mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;

				// Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
				mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 0;

				// Maximum indirect buffer size in bytes
				mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

				// Maximum number of multisamples (always at least 1, usually 8)
				mCapabilities.maximumNumberOfMultisamples = 1;	// Don't want to support the legacy DirectX 9 multisample support

				// Maximum anisotropy (always at least 1, usually 16)
				mCapabilities.maximumAnisotropy = 16;

				// Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
				mCapabilities.instancedArrays = false;

				// Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
				mCapabilities.drawInstanced = false;

				// Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
				mCapabilities.maximumNumberOfPatchVertices = 0;	// Direct3D 9.2 has no tessellation support

				// Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
				mCapabilities.maximumNumberOfGsOutputVertices = 0;	// Direct3D 9.2 has no geometry shader support
				break;

			case D3D_FEATURE_LEVEL_9_3:
				// Maximum number of viewports (always at least 1)
				mCapabilities.maximumNumberOfViewports = 1;	// Direct3D 9 only supports a single viewport

				// Maximum number of simultaneous render targets (if <1 render to texture is not supported)
				mCapabilities.maximumNumberOfSimultaneousRenderTargets = 4;

				// Maximum texture dimension
				mCapabilities.maximumTextureDimension = 4096;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 0;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 0;

				// Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
				mCapabilities.maximumNumberOf2DTextureArraySlices = 0;

				// Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
				mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;

				// Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
				mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 0;

				// Maximum indirect buffer size in bytes
				mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

				// Maximum number of multisamples (always at least 1, usually 8)
				mCapabilities.maximumNumberOfMultisamples = 1;	// Don't want to support the legacy DirectX 9 multisample support

				// Maximum anisotropy (always at least 1, usually 16)
				mCapabilities.maximumAnisotropy = 16;

				// Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
				mCapabilities.instancedArrays = true;

				// Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
				mCapabilities.drawInstanced = false;

				// Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
				mCapabilities.maximumNumberOfPatchVertices = 0;	// Direct3D 9.3 has no tessellation support

				// Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
				mCapabilities.maximumNumberOfGsOutputVertices = 0;	// Direct3D 9.3 has no geometry shader support
				break;

			case D3D_FEATURE_LEVEL_10_0:
				// Maximum number of viewports (always at least 1)
				// TODO(co) Direct3D 12 update
				// mCapabilities.maximumNumberOfViewports = D3D10_VIEWPORT_AND_SCISSORRECT_MAX_INDEX + 1;
				mCapabilities.maximumNumberOfViewports = 8;

				// Maximum number of simultaneous render targets (if <1 render to texture is not supported)
				// TODO(co) Direct3D 12 update
				//mCapabilities.maximumNumberOfSimultaneousRenderTargets = D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT;
				mCapabilities.maximumNumberOfSimultaneousRenderTargets = 8;

				// Maximum texture dimension
				mCapabilities.maximumTextureDimension = 8192;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 512;

				// Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
				mCapabilities.maximumNumberOf2DTextureArraySlices = 512;

				// Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
				mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;

				// Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
				mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 128 * 1024 * 1024;	// TODO(co) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention the texture buffer? Currently the OpenGL 3 minimum is used: 128 MiB.

				// Maximum indirect buffer size in bytes
				mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

				// Maximum number of multisamples (always at least 1, usually 8)
				mCapabilities.maximumNumberOfMultisamples = 8;

				// Maximum anisotropy (always at least 1, usually 16)
				mCapabilities.maximumAnisotropy = 16;

				// Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
				mCapabilities.instancedArrays = true;

				// Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
				mCapabilities.drawInstanced = true;

				// Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
				mCapabilities.maximumNumberOfPatchVertices = 0;	// Direct3D 10 has no tessellation support

				// Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
				mCapabilities.maximumNumberOfGsOutputVertices = 1024;
				break;

			case D3D_FEATURE_LEVEL_10_1:
				// Maximum number of viewports (always at least 1)
				// TODO(co) Direct3D 12 update
				//mCapabilities.maximumNumberOfViewports = D3D10_VIEWPORT_AND_SCISSORRECT_MAX_INDEX + 1;
				mCapabilities.maximumNumberOfViewports = 8;

				// Maximum number of simultaneous render targets (if <1 render to texture is not supported)
				// TODO(co) Direct3D 12 update
				//mCapabilities.maximumNumberOfSimultaneousRenderTargets = D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT;
				mCapabilities.maximumNumberOfSimultaneousRenderTargets = 8;

				// Maximum texture dimension
				mCapabilities.maximumTextureDimension = 8192;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 512;

				// Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
				mCapabilities.maximumNumberOf2DTextureArraySlices = 512;

				// Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
				mCapabilities.maximumNumberOfCubeTextureArraySlices = 512;

				// Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
				mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 128 * 1024 * 1024;	// TODO(co) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention the texture buffer? Currently the OpenGL 3 minimum is used: 128 MiB.

				// Maximum indirect buffer size in bytes
				mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

				// Maximum number of multisamples (always at least 1, usually 8)
				mCapabilities.maximumNumberOfMultisamples = 8;

				// Maximum anisotropy (always at least 1, usually 16)
				mCapabilities.maximumAnisotropy = 16;

				// Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
				mCapabilities.instancedArrays = true;

				// Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
				mCapabilities.drawInstanced = true;

				// Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
				mCapabilities.maximumNumberOfPatchVertices = 0;	// Direct3D 10.1 has no tessellation support

				// Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
				mCapabilities.maximumNumberOfGsOutputVertices = 1024;
				break;

			case D3D_FEATURE_LEVEL_11_0:
			case D3D_FEATURE_LEVEL_11_1:
			case D3D_FEATURE_LEVEL_12_0:
			case D3D_FEATURE_LEVEL_12_1:
				// Maximum number of viewports (always at least 1)
				// TODO(co) Direct3D 12 update
				//mCapabilities.maximumNumberOfViewports = D3D12_VIEWPORT_AND_SCISSORRECT_MAX_INDEX + 1;
				mCapabilities.maximumNumberOfViewports = 8;

				// Maximum number of simultaneous render targets (if <1 render to texture is not supported)
				mCapabilities.maximumNumberOfSimultaneousRenderTargets = D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT;

				// Maximum texture dimension
				mCapabilities.maximumTextureDimension = 16384;

				// Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
				mCapabilities.maximumNumberOf1DTextureArraySlices = 512;

				// Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
				mCapabilities.maximumNumberOf2DTextureArraySlices = 512;

				// Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
				mCapabilities.maximumNumberOfCubeTextureArraySlices = 512;

				// Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
				mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 128 * 1024 * 1024;	// TODO(co) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention the texture buffer? Currently the OpenGL 3 minimum is used: 128 MiB.

				// Maximum number of multisamples (always at least 1, usually 8)
				mCapabilities.maximumNumberOfMultisamples = 8;

				// Maximum anisotropy (always at least 1, usually 16)
				mCapabilities.maximumAnisotropy = 16;

				// Maximum indirect buffer size in bytes
				mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

				// Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
				mCapabilities.instancedArrays = true;

				// Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
				mCapabilities.drawInstanced = true;

				// Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
				mCapabilities.maximumNumberOfPatchVertices = 32;

				// Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
				mCapabilities.maximumNumberOfGsOutputVertices = 1024;	// TODO(co) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention it, so I assume it's 1024
				break;
		}

		// TODO(co) Implement me, remove this when done
		mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;

		// The rest is the same for all feature levels

		// Maximum uniform buffer (UBO) size in bytes (usually at least 4096 * 16 bytes, in case there's no support for uniform buffer it's 0)
		// -> Same as DirectX 11: See https://msdn.microsoft.com/en-us/library/windows/desktop/ff819065(v=vs.85).aspx - "Resource Limits (Direct3D 11)" - "Number of elements in a constant buffer D3D12_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096)"
		// -> One element = float4 = 16 bytes
		mCapabilities.maximumUniformBufferSize = 4096 * 16;

		// Left-handed coordinate system with clip space depth value range 0..1
		mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = true;

		// Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
		mCapabilities.individualUniforms = false;

		// Base vertex supported for draw calls?
		mCapabilities.baseVertex = true;

		// Direct3D 12 has native multithreading // TODO(co) But do only set this to true if it has been tested
		mCapabilities.nativeMultithreading = false;

		// Direct3D 12 has shader bytecode support
		// TODO(co) Implement shader bytecode support
		mCapabilities.shaderBytecode = false;

		// Is there support for vertex shaders (VS)?
		mCapabilities.vertexShader = true;

		// Is there support for fragment shaders (FS)?
		mCapabilities.fragmentShader = true;

		// Is there support for task shaders (TS) and mesh shaders (MS)?
		mCapabilities.meshShader = false;	// TODO(co) "DirectX 12 Ultimate" needed

		// Is there support for compute shaders (CS)?
		mCapabilities.computeShader = true;
	}

	void Direct3D12Rhi::unsetGraphicsVertexArray()
	{
		// Release the currently used vertex array reference, in case we have one
		if (nullptr != mVertexArray)
		{
			// Set no Direct3D 12 input layout
			mD3D12GraphicsCommandList->IASetVertexBuffers(0, 0, nullptr);

			// Release reference
			mVertexArray->ReleaseReference();
			mVertexArray = nullptr;
		}
	}

	#if SE_DEBUG
		void Direct3D12Rhi::debugReportLiveDeviceObjects()
		{
			ID3D12DebugDevice* d3d12DebugDevice = nullptr;
			if (SUCCEEDED(mD3D12Device->QueryInterface(IID_PPV_ARGS(&d3d12DebugDevice))))
			{
				d3d12DebugDevice->ReportLiveDeviceObjects(static_cast<D3D12_RLDO_FLAGS>(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL));
				d3d12DebugDevice->Release();
			}
		}
	#endif
} // Direct3D12Rhi

Rhi::IRhi* createDirect3D12RhiInstance(const handle& nativeWindowHandle)
{
	return RHI_NEW(Direct3D12Rhi::Direct3D12Rhi)(nativeWindowHandle);
}

#endif // SE_DIRECT3D12
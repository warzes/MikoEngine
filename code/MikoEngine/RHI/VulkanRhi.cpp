#include "stdafx.h"
#if SE_VULKAN
#include "Rhi.h"
#include "MakeID.h"
#include "VulkanCore.h"
#include "VulkanRhi.h"
#include "VulkanRuntimeLinking.h"
#include "VulkanContext.h"
#include "VulkanUnsort.h"

#pragma comment(lib, "vulkan-1.lib")

namespace VulkanRhi
{
	VulkanRhi::VulkanRhi(const handle& nativeWindowHandle) :
		IRhi(Rhi::NameId::VULKAN, nativeWindowHandle),
		VertexArrayMakeId(),
		GraphicsPipelineStateMakeId(),
		ComputePipelineStateMakeId(),
		mVkAllocationCallbacks{&GetAllocator(), &::detail::vkAllocationFunction, &::detail::vkReallocationFunction, &::detail::vkFreeFunction, nullptr, nullptr},
		mVulkanRuntimeLinking(nullptr),
		mVulkanContext(nullptr),
		mShaderLanguageGlsl(nullptr),
		mGraphicsRootSignature(nullptr),
		mComputeRootSignature(nullptr),
		mDefaultSamplerState(nullptr),
		mInsideVulkanRenderPass(false),
		mVkClearValues{},
		mVertexArray(nullptr),
		mRenderTarget(nullptr)
		#if SE_DEBUG
			, mDebugBetweenBeginEndScene(false)
		#endif
	{
		// TODO(co) Make it possible to enable/disable validation from the outside?
		#if SE_DEBUG
			const bool enableValidation = true;
		#else
			const bool enableValidation = false;
		#endif

		// Is Vulkan available?
		mVulkanRuntimeLinking = RHI_NEW(VulkanRuntimeLinking)(*this, enableValidation);
		if (mVulkanRuntimeLinking->isVulkanAvaiable())
		{
			// TODO(co) Add external Vulkan context support
			mVulkanContext = RHI_NEW(VulkanContext)(*this);

			// Is the Vulkan context initialized?
			if (mVulkanContext->isInitialized())
			{
				// Initialize the capabilities
				initializeCapabilities();

				// Create the default sampler state
				mDefaultSamplerState = createSamplerState(Rhi::ISamplerState::getDefaultSamplerState());

				// Add references to the default sampler state and set it
				if (nullptr != mDefaultSamplerState)
				{
					mDefaultSamplerState->AddReference();
					// TODO(co) Set default sampler states
				}
			}
		}
	}

	VulkanRhi::~VulkanRhi()
	{
		// Set no vertex array reference, in case we have one
		setGraphicsVertexArray(nullptr);

		// Release instances
		if (nullptr != mRenderTarget)
		{
			mRenderTarget->ReleaseReference();
			mRenderTarget = nullptr;
		}
		if (nullptr != mDefaultSamplerState)
		{
			mDefaultSamplerState->ReleaseReference();
			mDefaultSamplerState = nullptr;
		}

		// Release the graphics and compute root signature instance, in case we have one
		if (nullptr != mGraphicsRootSignature)
		{
			mGraphicsRootSignature->ReleaseReference();
		}
		if (nullptr != mComputeRootSignature)
		{
			mComputeRootSignature->ReleaseReference();
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
					RHI_LOG(CRITICAL, "The Vulkan RHI implementation is going to be destroyed, but there are still %u resource instances left (memory leak)", numberOfCurrentResources)
				}
				else
				{
					RHI_LOG(CRITICAL, "The Vulkan RHI implementation is going to be destroyed, but there is still one resource instance left (memory leak)")
				}

				// Use debug output to show the current number of resource instances
				getStatistics().DebugOutputCurrentResouces();
			}
		}
		#endif

		// Release the GLSL shader language instance, in case we have one
		if (nullptr != mShaderLanguageGlsl)
		{
			mShaderLanguageGlsl->ReleaseReference();
		}

		// Destroy the Vulkan context instance
		RHI_DELETE(VulkanContext, mVulkanContext);

		// Destroy the Vulkan runtime linking instance
		RHI_DELETE(VulkanRuntimeLinking, mVulkanRuntimeLinking);
	}


	//[-------------------------------------------------------]
	//[ Graphics                                              ]
	//[-------------------------------------------------------]
	void VulkanRhi::setGraphicsRootSignature(Rhi::IRootSignature* rootSignature)
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
		}
	}

	void VulkanRhi::setGraphicsPipelineState(Rhi::IGraphicsPipelineState* graphicsPipelineState)
	{
		if (nullptr != graphicsPipelineState)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *graphicsPipelineState)

			// Bind Vulkan graphics pipeline
			vkCmdBindPipeline(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<GraphicsPipelineState*>(graphicsPipelineState)->getVkPipeline());
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}

	void VulkanRhi::setGraphicsResourceGroup(uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup)
	{
		// Security checks
		#if SE_DEBUG
		{
			if (nullptr == mGraphicsRootSignature)
			{
				RHI_LOG(CRITICAL, "No Vulkan RHI implementation graphics root signature set")
				return;
			}
			const Rhi::RootSignature& rootSignature = mGraphicsRootSignature->getRootSignature();
			if (rootParameterIndex >= rootSignature.numberOfParameters)
			{
				RHI_LOG(CRITICAL, "The Vulkan RHI implementation root parameter index is out of bounds")
				return;
			}
			const Rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			if (Rhi::RootParameterType::DESCRIPTOR_TABLE != rootParameter.parameterType)
			{
				RHI_LOG(CRITICAL, "The Vulkan RHI implementation root parameter index doesn't reference a descriptor table")
				return;
			}
			if (nullptr == reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges))
			{
				RHI_LOG(CRITICAL, "The Vulkan RHI implementation descriptor ranges is a null pointer")
				return;
			}
		}
		#endif

		if (nullptr != resourceGroup)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *resourceGroup)

			// Bind Vulkan descriptor set
			const VkDescriptorSet vkDescriptorSet = static_cast<ResourceGroup*>(resourceGroup)->getVkDescriptorSet();
			if (VK_NULL_HANDLE != vkDescriptorSet)
			{
				vkCmdBindDescriptorSets(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsRootSignature->getVkPipelineLayout(), rootParameterIndex, 1, &vkDescriptorSet, 0, nullptr);
			}
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}

	void VulkanRhi::setGraphicsVertexArray(Rhi::IVertexArray* vertexArray)
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
				mVertexArray->AddReference();

				// Bind Vulkan buffers
				mVertexArray->bindVulkanBuffers(getVulkanContext().getVkCommandBuffer());
			}
			else
			{
				// Unset the currently used vertex array
				unsetGraphicsVertexArray();
			}
		}
	}

	void VulkanRhi::setGraphicsViewports([[maybe_unused]] uint32_t numberOfViewports, const Rhi::Viewport* viewports)
	{
		// Rasterizer (RS) stage

		// Sanity check
		SE_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid Vulkan rasterizer state viewports")

		// Set Vulkan viewport
		// -> We're using the "VK_KHR_maintenance1"-extension ("VK_KHR_MAINTENANCE1_EXTENSION_NAME"-definition) to be able to specify a negative viewport height,
		//    this way we don't have to apply "<output position>.y = -<output position>.y" inside vertex shaders to compensate for the Vulkan coordinate system
		// TODO(co) Add support for multiple viewports
		VkViewport vkViewport = reinterpret_cast<const VkViewport*>(viewports)[0];
		vkViewport.y += vkViewport.height;
		vkViewport.height = -vkViewport.height;
		vkCmdSetViewport(getVulkanContext().getVkCommandBuffer(), 0, 1, &vkViewport);
	}

	void VulkanRhi::setGraphicsScissorRectangles([[maybe_unused]] uint32_t numberOfScissorRectangles, const Rhi::ScissorRectangle* scissorRectangles)
	{
		// Rasterizer (RS) stage

		// Sanity check
		SE_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid Vulkan rasterizer state scissor rectangles")

		// Set Vulkan scissor
		// TODO(co) Add support for multiple scissor rectangles. Change "Rhi::ScissorRectangle" to Vulkan style to make it the primary API on the long run?
		const VkRect2D vkRect2D =
		{
			{ static_cast<int32_t>(scissorRectangles[0].topLeftX), static_cast<int32_t>(scissorRectangles[0].topLeftY) },
			{ static_cast<uint32_t>(scissorRectangles[0].bottomRightX - scissorRectangles[0].topLeftX), static_cast<uint32_t>(scissorRectangles[0].bottomRightY - scissorRectangles[0].topLeftY) }
		};
		vkCmdSetScissor(getVulkanContext().getVkCommandBuffer(), 0, 1, &vkRect2D);
	}

	void VulkanRhi::setGraphicsRenderTarget(Rhi::IRenderTarget* renderTarget)
	{
		// Output-merger (OM) stage

		// New render target?
		if (mRenderTarget != renderTarget)
		{
			// Release the render target reference, in case we have one
			if (nullptr != mRenderTarget)
			{
				// Start Vulkan render pass, if necessary (for e.g. clearing)
				if (!mInsideVulkanRenderPass && ((mRenderTarget->getResourceType() == Rhi::ResourceType::SWAP_CHAIN && nullptr == renderTarget) || mRenderTarget->getResourceType() == Rhi::ResourceType::FRAMEBUFFER))
				{
					beginVulkanRenderPass();
				}

				// End Vulkan render pass, if necessary
				if (mInsideVulkanRenderPass)
				{
					vkCmdEndRenderPass(getVulkanContext().getVkCommandBuffer());
					mInsideVulkanRenderPass = false;
				}

				// Release
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

				// Set clear color and clear depth stencil values
				const uint32_t numberOfColorAttachments = static_cast<const RenderPass&>(mRenderTarget->getRenderPass()).getNumberOfColorAttachments();
				SE_ASSERT(numberOfColorAttachments < 8, "Vulkan only supports 7 render pass color attachments")
				for (uint32_t i = 0; i < numberOfColorAttachments; ++i)
				{
					mVkClearValues[i] = VkClearValue{0.0f, 0.0f, 0.0f, 1.0f};
				}
				mVkClearValues[numberOfColorAttachments] = VkClearValue{ 1.0f, 0 };
			}
		}
	}

	void VulkanRhi::clearGraphics(uint32_t clearFlags, const float color[4], float z, uint32_t stencil)
	{
		// Sanity checks
		SE_ASSERT(nullptr != mRenderTarget, "Can't execute Vulkan clear command without a render target set")
		SE_ASSERT(!mInsideVulkanRenderPass, "Can't execute clear command inside a Vulkan render pass")
		SE_ASSERT(z >= 0.0f && z <= 1.0f, "The Vulkan clear graphics z value must be between [0, 1] (inclusive)")

		// Clear color
		const uint32_t numberOfColorAttachments = static_cast<const RenderPass&>(mRenderTarget->getRenderPass()).getNumberOfColorAttachments();
		SE_ASSERT(numberOfColorAttachments < 8, "Vulkan only supports 7 render pass color attachments")
		if (clearFlags & Rhi::ClearFlag::COLOR)
		{
			for (uint32_t i = 0; i < numberOfColorAttachments; ++i)
			{
				memcpy(mVkClearValues[i].color.float32, &color[0], sizeof(float) * 4);
			}
		}

		// Clear depth stencil
		if ((clearFlags & Rhi::ClearFlag::DEPTH) || (clearFlags & Rhi::ClearFlag::STENCIL))
		{
			mVkClearValues[numberOfColorAttachments].depthStencil.depth = z;
			mVkClearValues[numberOfColorAttachments].depthStencil.stencil = stencil;
		}
	}

	void VulkanRhi::drawGraphics(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, indirectBuffer)
		SE_ASSERT(numberOfDraws > 0, "Number of Vulkan draws must not be zero")
		// It's possible to draw without "mVertexArray"

		// Start Vulkan render pass, if necessary
		if (!mInsideVulkanRenderPass)
		{
			beginVulkanRenderPass();
		}

		// Vulkan draw indirect command
		vkCmdDrawIndirect(getVulkanContext().getVkCommandBuffer(), static_cast<const IndirectBuffer&>(indirectBuffer).getVkBuffer(), indirectBufferOffset, numberOfDraws, sizeof(VkDrawIndirectCommand));
	}

	void VulkanRhi::drawGraphicsEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		SE_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
		SE_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")
		// It's possible to draw without "mVertexArray"

		// TODO(co) Currently no buffer overflow check due to lack of interface provided data
		emulationData += indirectBufferOffset;

		// Start Vulkan render pass, if necessary
		if (!mInsideVulkanRenderPass)
		{
			beginVulkanRenderPass();
		}

		// Emit the draw calls
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-draw-indirect emulation");
			}
		#endif
		const VkCommandBuffer vkCommandBuffer = getVulkanContext().getVkCommandBuffer();
		for (uint32_t i = 0; i < numberOfDraws; ++i)
		{
			// Draw and advance
			const Rhi::DrawArguments& drawArguments = *reinterpret_cast<const Rhi::DrawArguments*>(emulationData);
			vkCmdDraw(vkCommandBuffer, drawArguments.vertexCountPerInstance, drawArguments.instanceCount, drawArguments.startVertexLocation, drawArguments.startInstanceLocation);
			emulationData += sizeof(Rhi::DrawArguments);
		}
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
		#endif
	}

	void VulkanRhi::drawIndexedGraphics(const Rhi::IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, indirectBuffer)
		SE_ASSERT(numberOfDraws > 0, "Number of Vulkan draws must not be zero")
		SE_ASSERT(nullptr != mVertexArray, "Vulkan draw indexed needs a set vertex array")
		SE_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "Vulkan draw indexed needs a set vertex array which contains an index buffer")

		// Start Vulkan render pass, if necessary
		if (!mInsideVulkanRenderPass)
		{
			beginVulkanRenderPass();
		}

		// Vulkan draw indexed indirect command
		vkCmdDrawIndexedIndirect(getVulkanContext().getVkCommandBuffer(), static_cast<const IndirectBuffer&>(indirectBuffer).getVkBuffer(), indirectBufferOffset, numberOfDraws, sizeof(VkDrawIndexedIndirectCommand));
	}

	void VulkanRhi::drawIndexedGraphicsEmulated(const uint8_t* emulationData, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		SE_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
		SE_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")
		SE_ASSERT(nullptr != mVertexArray, "Vulkan draw indexed needs a set vertex array")
		SE_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "Vulkan draw indexed needs a set vertex array which contains an index buffer")

		// TODO(co) Currently no buffer overflow check due to lack of interface provided data
		emulationData += indirectBufferOffset;

		// Start Vulkan render pass, if necessary
		if (!mInsideVulkanRenderPass)
		{
			beginVulkanRenderPass();
		}

		// Emit the draw calls
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-indexed-draw-indirect emulation");
			}
		#endif
		const VkCommandBuffer vkCommandBuffer = getVulkanContext().getVkCommandBuffer();
		for (uint32_t i = 0; i < numberOfDraws; ++i)
		{
			// Draw and advance
			const Rhi::DrawIndexedArguments& drawIndexedArguments = *reinterpret_cast<const Rhi::DrawIndexedArguments*>(emulationData);
			vkCmdDrawIndexed(vkCommandBuffer, drawIndexedArguments.indexCountPerInstance, drawIndexedArguments.instanceCount, drawIndexedArguments.startIndexLocation, drawIndexedArguments.baseVertexLocation, drawIndexedArguments.startInstanceLocation);
			emulationData += sizeof(Rhi::DrawIndexedArguments);
		}
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
		#endif
	}

	void VulkanRhi::drawMeshTasks([[maybe_unused]] const Rhi::IIndirectBuffer& indirectBuffer, [[maybe_unused]] uint32_t indirectBufferOffset, [[maybe_unused]] uint32_t numberOfDraws)
	{
		// Sanity checks
		SE_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")

		// TODO(co) Implement me
		// vkCmdDrawMeshTasksIndirectNV
		// vkCmdDrawMeshTasksIndirectCountNV
	}

	void VulkanRhi::drawMeshTasksEmulated([[maybe_unused]] const uint8_t* emulationData, uint32_t indirectBufferOffset, uint32_t numberOfDraws)
	{
		// Sanity checks
		SE_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
		SE_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")

		// TODO(co) Currently no buffer overflow check due to lack of interface provided data
		emulationData += indirectBufferOffset;

		// Emit the draw calls
		#if SE_DEBUG
			if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-indexed-draw-indirect emulation");
			}
		#endif
		const VkCommandBuffer vkCommandBuffer = getVulkanContext().getVkCommandBuffer();
		for (uint32_t i = 0; i < numberOfDraws; ++i)
		{
			const Rhi::DrawMeshTasksArguments& drawMeshTasksArguments = *reinterpret_cast<const Rhi::DrawMeshTasksArguments*>(emulationData);

			// Draw and advance
			// TODO:
			//vkCmdDrawMeshTasksNV(vkCommandBuffer, drawMeshTasksArguments.numberOfTasks, drawMeshTasksArguments.firstTask);
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
	void VulkanRhi::setComputeRootSignature(Rhi::IRootSignature* rootSignature)
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
		}
	}

	void VulkanRhi::setComputePipelineState(Rhi::IComputePipelineState* computePipelineState)
	{
		if (nullptr != computePipelineState)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *computePipelineState)

			// Bind Vulkan compute pipeline
			vkCmdBindPipeline(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, static_cast<ComputePipelineState*>(computePipelineState)->getVkPipeline());
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}

	void VulkanRhi::setComputeResourceGroup(uint32_t rootParameterIndex, Rhi::IResourceGroup* resourceGroup)
	{
		// Security checks
		#if SE_DEBUG
		{
			if (nullptr == mComputeRootSignature)
			{
				RHI_LOG(CRITICAL, "No Vulkan RHI implementation compute root signature set")
				return;
			}
			const Rhi::RootSignature& rootSignature = mComputeRootSignature->getRootSignature();
			if (rootParameterIndex >= rootSignature.numberOfParameters)
			{
				RHI_LOG(CRITICAL, "The Vulkan RHI implementation root parameter index is out of bounds")
				return;
			}
			const Rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			if (Rhi::RootParameterType::DESCRIPTOR_TABLE != rootParameter.parameterType)
			{
				RHI_LOG(CRITICAL, "The Vulkan RHI implementation root parameter index doesn't reference a descriptor table")
				return;
			}
			if (nullptr == reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges))
			{
				RHI_LOG(CRITICAL, "The Vulkan RHI implementation descriptor ranges is a null pointer")
				return;
			}
		}
		#endif

		if (nullptr != resourceGroup)
		{
			// Sanity check
			RHI_MATCH_CHECK(*this, *resourceGroup)

			// Bind Vulkan descriptor set
			const VkDescriptorSet vkDescriptorSet = static_cast<ResourceGroup*>(resourceGroup)->getVkDescriptorSet();
			if (VK_NULL_HANDLE != vkDescriptorSet)
			{
				vkCmdBindDescriptorSets(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, mComputeRootSignature->getVkPipelineLayout(), rootParameterIndex, 1, &vkDescriptorSet, 0, nullptr);
			}
		}
		else
		{
			// TODO(co) Handle this situation?
		}
	}


	//[-------------------------------------------------------]
	//[ Resource                                              ]
	//[-------------------------------------------------------]
	void VulkanRhi::resolveMultisampleFramebuffer(Rhi::IRenderTarget&, Rhi::IFramebuffer&)
	{
		// TODO(co) Implement me
	}

	void VulkanRhi::copyResource(Rhi::IResource&, Rhi::IResource&)
	{
		// TODO(co) Implement me
	}

	void VulkanRhi::generateMipmaps(Rhi::IResource&)
	{
		// TODO(co) Implement me: Search for https://github.com/SaschaWillems/Vulkan/tree/master/texturemipmapgen inside this cpp file and unify the code to be able to reuse it in here
	}


	//[-------------------------------------------------------]
	//[ Query                                                 ]
	//[-------------------------------------------------------]
	void VulkanRhi::resetQueryPool(Rhi::IQueryPool& queryPool, uint32_t firstQueryIndex, uint32_t numberOfQueries)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Reset Vulkan query pool
		vkCmdResetQueryPool(getVulkanContext().getVkCommandBuffer(), static_cast<const QueryPool&>(queryPool).getVkQueryPool(), firstQueryIndex, numberOfQueries);
	}

	void VulkanRhi::beginQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex, uint32_t queryControlFlags)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Begin Vulkan query
		vkCmdBeginQuery(getVulkanContext().getVkCommandBuffer(), static_cast<const QueryPool&>(queryPool).getVkQueryPool(), queryIndex, ((queryControlFlags & Rhi::QueryControlFlags::PRECISE) != 0) ? VK_QUERY_CONTROL_PRECISE_BIT : 0u);
	}

	void VulkanRhi::endQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// End Vulkan query
		vkCmdEndQuery(getVulkanContext().getVkCommandBuffer(), static_cast<const QueryPool&>(queryPool).getVkQueryPool(), queryIndex);
	}

	void VulkanRhi::writeTimestampQuery(Rhi::IQueryPool& queryPool, uint32_t queryIndex)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Write Vulkan timestamp query
		vkCmdWriteTimestamp(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, static_cast<const QueryPool&>(queryPool).getVkQueryPool(), queryIndex);
	}


	//[-------------------------------------------------------]
	//[ Debug                                                 ]
	//[-------------------------------------------------------]
	#if SE_DEBUG
		void VulkanRhi::setDebugMarker(const char* name)
		{
			if (nullptr != vkCmdDebugMarkerInsertEXT)
			{
				SE_ASSERT(nullptr != name, "Vulkan debug marker names must not be a null pointer")
				const VkDebugMarkerMarkerInfoEXT vkDebugMarkerMarkerInfoEXT =
				{
					VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,	// sType (VkStructureType)
					nullptr,										// pNext (const void*)
					name,											// pMarkerName (const char*)
					{ // color[4] (float)
						0.0f,
						0.0f,
						1.0f,	// Blue
						1.0f
					}
				};
				vkCmdDebugMarkerInsertEXT(getVulkanContext().getVkCommandBuffer(), &vkDebugMarkerMarkerInfoEXT);
			}
		}

		void VulkanRhi::beginDebugEvent(const char* name)
		{
			if (nullptr != vkCmdDebugMarkerBeginEXT)
			{
				SE_ASSERT(nullptr != name, "Vulkan debug event names must not be a null pointer")
				const VkDebugMarkerMarkerInfoEXT vkDebugMarkerMarkerInfoEXT =
				{
					VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,	// sType (VkStructureType)
					nullptr,										// pNext (const void*)
					name,											// pMarkerName (const char*)
					{ // color[4] (float)
						0.0f,
						1.0f,	// Green
						0.0f,
						1.0f
					}
				};
				vkCmdDebugMarkerBeginEXT(getVulkanContext().getVkCommandBuffer(), &vkDebugMarkerMarkerInfoEXT);
			}
		}

		void VulkanRhi::endDebugEvent()
		{
			if (nullptr != vkCmdDebugMarkerEndEXT)
			{
				vkCmdDebugMarkerEndEXT(getVulkanContext().getVkCommandBuffer());
			}
		}
	#endif


	//[-------------------------------------------------------]
	//[ Public virtual Rhi::IRhi methods                      ]
	//[-------------------------------------------------------]
	const char* VulkanRhi::getName() const
	{
		return "Vulkan";
	}

	bool VulkanRhi::isInitialized() const
	{
		// Is the Vulkan context initialized?
		return (nullptr != mVulkanContext && mVulkanContext->isInitialized());
	}

	bool VulkanRhi::isDebugEnabled()
	{
		// Check for any "VK_EXT_debug_marker"-extension function pointer
		return (nullptr != vkDebugMarkerSetObjectTagEXT || nullptr != vkDebugMarkerSetObjectNameEXT || nullptr != vkCmdDebugMarkerBeginEXT || nullptr != vkCmdDebugMarkerEndEXT || nullptr != vkCmdDebugMarkerInsertEXT);
	}


	//[-------------------------------------------------------]
	//[ Shader language                                       ]
	//[-------------------------------------------------------]
	uint32_t VulkanRhi::getNumberOfShaderLanguages() const
	{
		// Done, return the number of supported shader languages
		return 1;
	}

	const char* VulkanRhi::getShaderLanguageName([[maybe_unused]] uint32_t index) const
	{
		SE_ASSERT(index < getNumberOfShaderLanguages(), "Vulkan: Shader language index is out-of-bounds")
		return ::detail::GLSL_NAME;
	}

	Rhi::IShaderLanguage* VulkanRhi::getShaderLanguage(const char* shaderLanguageName)
	{
		// In case "shaderLanguage" is a null pointer, use the default shader language
		if (nullptr != shaderLanguageName)
		{
			// Optimization: Check for shader language name pointer match, first
			// -> "ShaderLanguageSeparate::NAME" has the same value
			if (::detail::GLSL_NAME == shaderLanguageName || !stricmp(shaderLanguageName, ::detail::GLSL_NAME))
			{
				// If required, create the GLSL shader language instance right now
				if (nullptr == mShaderLanguageGlsl)
				{
					mShaderLanguageGlsl = RHI_NEW(ShaderLanguageGlsl(*this));
					mShaderLanguageGlsl->AddReference();	// Internal RHI reference
				}
				return mShaderLanguageGlsl;
			}
		}
		else
		{
			// Return the shader language instance as default
			return getShaderLanguage(::detail::GLSL_NAME);
		}

		// Error!
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Resource creation                                     ]
	//[-------------------------------------------------------]
	Rhi::IRenderPass* VulkanRhi::createRenderPass(uint32_t numberOfColorAttachments, const Rhi::TextureFormat::Enum* colorAttachmentTextureFormats, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		return RHI_NEW(RenderPass)(*this, numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IQueryPool* VulkanRhi::createQueryPool(Rhi::QueryType queryType, uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		SE_ASSERT(numberOfQueries > 0, "Vulkan: Number of queries mustn't be zero")
		return RHI_NEW(QueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::ISwapChain* VulkanRhi::createSwapChain(Rhi::IRenderPass& renderPass, Rhi::WindowHandle windowHandle, bool RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		RHI_MATCH_CHECK(*this, renderPass)
		SE_ASSERT(SE_NULL_HANDLE != windowHandle.nativeWindowHandle || nullptr != windowHandle.renderWindow, "Vulkan: The provided native window handle or render window must not be a null handle / null pointer")

		// Create the swap chain
		return RHI_NEW(SwapChain)(renderPass, windowHandle RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IFramebuffer* VulkanRhi::createFramebuffer(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, renderPass)

		// Create the framebuffer
		return RHI_NEW(Framebuffer)(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IBufferManager* VulkanRhi::createBufferManager()
	{
		return RHI_NEW(BufferManager)(*this);
	}

	Rhi::ITextureManager* VulkanRhi::createTextureManager()
	{
		return RHI_NEW(TextureManager)(*this);
	}

	Rhi::IRootSignature* VulkanRhi::createRootSignature(const Rhi::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		return RHI_NEW(RootSignature)(*this, rootSignature RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}

	Rhi::IGraphicsPipelineState* VulkanRhi::createGraphicsPipelineState(const Rhi::GraphicsPipelineState& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		SE_ASSERT(nullptr != graphicsPipelineState.rootSignature, "Vulkan: Invalid graphics pipeline state root signature")
		SE_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "Vulkan: Invalid graphics pipeline state graphics program")
		SE_ASSERT(nullptr != graphicsPipelineState.renderPass, "Vulkan: Invalid graphics pipeline state render pass")

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

	Rhi::IComputePipelineState* VulkanRhi::createComputePipelineState(Rhi::IRootSignature& rootSignature, Rhi::IComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
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

	Rhi::ISamplerState* VulkanRhi::createSamplerState(const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		return RHI_NEW(SamplerState)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}


	//[-------------------------------------------------------]
	//[ Resource handling                                     ]
	//[-------------------------------------------------------]
	bool VulkanRhi::map(Rhi::IResource& resource, uint32_t, Rhi::MapType, uint32_t, Rhi::MappedSubresource& mappedSubresource)
	{
		// Evaluate the resource type
		switch (resource.getResourceType())
		{
			case Rhi::ResourceType::VERTEX_BUFFER:
			{
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;
				return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<VertexBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
			}

			case Rhi::ResourceType::INDEX_BUFFER:
			{
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;
				return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<IndexBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
			}

			case Rhi::ResourceType::TEXTURE_BUFFER:
			{
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;
				return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<TextureBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
			}

			case Rhi::ResourceType::STRUCTURED_BUFFER:
			{
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;
				return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<StructuredBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
			}

			case Rhi::ResourceType::INDIRECT_BUFFER:
			{
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;
				return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<IndirectBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
			}

			case Rhi::ResourceType::UNIFORM_BUFFER:
			{
				mappedSubresource.rowPitch   = 0;
				mappedSubresource.depthPitch = 0;
				return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<UniformBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
			}

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
				// TODO(co) Implement me
				return false;
			}

			case Rhi::ResourceType::TEXTURE_2D_ARRAY:
			{
				// TODO(co) Implement me
				return false;
			}

			case Rhi::ResourceType::TEXTURE_3D:
			{
				// TODO(co) Implement me
				return false;
			}

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

	void VulkanRhi::unmap(Rhi::IResource& resource, uint32_t)
	{
		// Evaluate the resource type
		switch (resource.getResourceType())
		{
			case Rhi::ResourceType::VERTEX_BUFFER:
			{
				vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<VertexBuffer&>(resource).getVkDeviceMemory());
				break;
			}

			case Rhi::ResourceType::INDEX_BUFFER:
			{
				vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<IndexBuffer&>(resource).getVkDeviceMemory());
				break;
			}

			case Rhi::ResourceType::TEXTURE_BUFFER:
			{
				vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<TextureBuffer&>(resource).getVkDeviceMemory());
				break;
			}

			case Rhi::ResourceType::STRUCTURED_BUFFER:
			{
				vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<StructuredBuffer&>(resource).getVkDeviceMemory());
				break;
			}

			case Rhi::ResourceType::INDIRECT_BUFFER:
			{
				vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<IndirectBuffer&>(resource).getVkDeviceMemory());
				break;
			}

			case Rhi::ResourceType::UNIFORM_BUFFER:
			{
				vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<UniformBuffer&>(resource).getVkDeviceMemory());
				break;
			}

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
				// TODO(co) Implement me
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

	bool VulkanRhi::getQueryPoolResults(Rhi::IQueryPool& queryPool, uint32_t numberOfDataBytes, uint8_t* data, uint32_t firstQueryIndex, uint32_t numberOfQueries, uint32_t strideInBytes, [[maybe_unused]] uint32_t queryResultFlags)
	{
		// Sanity check
		RHI_MATCH_CHECK(*this, queryPool)

		// Query pool type dependent processing
		const QueryPool& vulkanQueryPool = static_cast<const QueryPool&>(queryPool);
		switch (vulkanQueryPool.getQueryType())
		{
			case Rhi::QueryType::OCCLUSION:
			case Rhi::QueryType::TIMESTAMP:	// TODO(co) Convert time to nanoseconds, see e.g. http://vulkan-spec-chunked.ahcox.com/ch16s05.html - VkPhysicalDeviceLimits::timestampPeriod - The number of nanoseconds it takes for a timestamp value to be incremented by 1
			{
				// Get Vulkan query pool results
				const VkQueryResultFlags vkQueryResultFlags = 0u;
				// const VkQueryResultFlags vkQueryResultFlags = ((queryResultFlags & Rhi::QueryResultFlags::WAIT) != 0) ? VK_QUERY_RESULT_WAIT_BIT : 0u;	// TODO(co)
				return (vkGetQueryPoolResults(getVulkanContext().getVkDevice(), vulkanQueryPool.getVkQueryPool(), firstQueryIndex, numberOfQueries, numberOfDataBytes, data, strideInBytes, VK_QUERY_RESULT_64_BIT | vkQueryResultFlags) == VK_SUCCESS);
			}

			case Rhi::QueryType::PIPELINE_STATISTICS:
			{
				// Our setup results in the same structure layout as used by "D3D11_QUERY_DATA_PIPELINE_STATISTICS" which we use for "Rhi::PipelineStatisticsQueryResult"
				const VkQueryResultFlags vkQueryResultFlags = 0u;
				// const VkQueryResultFlags vkQueryResultFlags = ((queryResultFlags & Rhi::QueryResultFlags::WAIT) != 0) ? VK_QUERY_RESULT_WAIT_BIT : 0u;	// TODO(co)
				return (vkGetQueryPoolResults(getVulkanContext().getVkDevice(), vulkanQueryPool.getVkQueryPool(), firstQueryIndex, numberOfQueries, numberOfDataBytes, data, strideInBytes, VK_QUERY_RESULT_64_BIT | vkQueryResultFlags) == VK_SUCCESS);
			}
		}

		// Result not ready
		return false;
	}


	//[-------------------------------------------------------]
	//[ Operations                                            ]
	//[-------------------------------------------------------]
	bool VulkanRhi::beginScene()
	{
		// Sanity check
		#if SE_DEBUG
			SE_ASSERT(false == mDebugBetweenBeginEndScene, "Vulkan: Begin scene was called while scene rendering is already in progress, missing end scene call?")
			mDebugBetweenBeginEndScene = true;
		#endif

		// Begin Vulkan command buffer
		// -> This automatically resets the Vulkan command buffer in case it was previously already recorded
		static constexpr VkCommandBufferBeginInfo vkCommandBufferBeginInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	// sType (VkStructureType)
			nullptr,										// pNext (const void*)
			0,												// flags (VkCommandBufferUsageFlags)
			nullptr											// pInheritanceInfo (const VkCommandBufferInheritanceInfo*)
		};
		if (vkBeginCommandBuffer(getVulkanContext().getVkCommandBuffer(), &vkCommandBufferBeginInfo) == VK_SUCCESS)
		{
			// Done
			return true;
		}
		else
		{
			// Error!
			RHI_LOG(CRITICAL, "Failed to begin Vulkan command buffer instance")
			return false;
		}
	}

	void VulkanRhi::submitCommandBuffer(const Rhi::CommandBuffer& commandBuffer)
	{
		// Sanity check
		SE_ASSERT(!commandBuffer.isEmpty(), "The Vulkan command buffer to execute mustn't be empty")

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

	void VulkanRhi::endScene()
	{
		// Sanity check
		#if SE_DEBUG
			SE_ASSERT(true == mDebugBetweenBeginEndScene, "Vulkan: End scene was called while scene rendering isn't in progress, missing start scene call?")
			mDebugBetweenBeginEndScene = false;
		#endif

		// We need to forget about the currently set render target
		setGraphicsRenderTarget(nullptr);

		// We need to forget about the currently set vertex array
		unsetGraphicsVertexArray();

		// End Vulkan command buffer
		if (vkEndCommandBuffer(getVulkanContext().getVkCommandBuffer()) != VK_SUCCESS)
		{
			// Error!
			RHI_LOG(CRITICAL, "Failed to end Vulkan command buffer instance")
		}
	}


	//[-------------------------------------------------------]
	//[ Synchronization                                       ]
	//[-------------------------------------------------------]
	void VulkanRhi::flush()
	{
		// TODO(co) Implement me
	}

	void VulkanRhi::finish()
	{
		// TODO(co) Implement me
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void VulkanRhi::initializeCapabilities()
	{
		{ // Get device name
			VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
			vkGetPhysicalDeviceProperties(mVulkanContext->getVkPhysicalDevice(), &vkPhysicalDeviceProperties);
			const size_t numberOfCharacters = ::detail::countof(mCapabilities.deviceName) - 1;
			strncpy(mCapabilities.deviceName, vkPhysicalDeviceProperties.deviceName, numberOfCharacters);
			mCapabilities.deviceName[numberOfCharacters] = '\0';
		}

		// Preferred swap chain texture format
		mCapabilities.preferredSwapChainColorTextureFormat = (SwapChain::findColorVkFormat(m_nativeWindowHandle, mVulkanRuntimeLinking->getVkInstance(), *mVulkanContext) == VK_FORMAT_R8G8B8A8_UNORM) ? Rhi::TextureFormat::Enum::R8G8B8A8 : Rhi::TextureFormat::Enum::B8G8R8A8;

		{ // Preferred swap chain depth stencil texture format
			const VkFormat depthVkFormat = SwapChain::findDepthVkFormat(mVulkanContext->getVkPhysicalDevice());
			if (VK_FORMAT_D32_SFLOAT == depthVkFormat)
			{
				mCapabilities.preferredSwapChainDepthStencilTextureFormat = Rhi::TextureFormat::Enum::D32_FLOAT;
			}
			else
			{
				// TODO(co) Add support for "VK_FORMAT_D32_SFLOAT_S8_UINT" and "VK_FORMAT_D24_UNORM_S8_UINT"
				mCapabilities.preferredSwapChainDepthStencilTextureFormat = Rhi::TextureFormat::Enum::D32_FLOAT;
			}
		}

		// TODO(co) Implement me, this in here is just a placeholder implementation

		{ // "D3D_FEATURE_LEVEL_11_0"
			// Maximum number of viewports (always at least 1)
			mCapabilities.maximumNumberOfViewports = 16;

			// Maximum number of simultaneous render targets (if <1 render to texture is not supported)
			mCapabilities.maximumNumberOfSimultaneousRenderTargets = 8;

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

			// Maximum indirect buffer size in bytes
			mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

			// Maximum number of multisamples (always at least 1, usually 8)
			mCapabilities.maximumNumberOfMultisamples = 1;	// TODO(co) Add multisample support, when setting "VkPhysicalDeviceFeatures" -> "sampleRateShading" -> VK_TRUE don't forget to check whether or not the device sup pots the feature

			// Maximum anisotropy (always at least 1, usually 16)
			mCapabilities.maximumAnisotropy = 16;

			// Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			mCapabilities.instancedArrays = true;

			// Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
			mCapabilities.drawInstanced = true;

			// Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
			mCapabilities.maximumNumberOfPatchVertices = 32;

			// Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
			mCapabilities.maximumNumberOfGsOutputVertices = 1024;	// TODO(co) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention it, so I assume it's 1024
		}

		// The rest is the same for all feature levels

		// Maximum uniform buffer (UBO) size in bytes (usually at least 4096 * 16 bytes, in case there's no support for uniform buffer it's 0)
		// -> See https://msdn.microsoft.com/en-us/library/windows/desktop/ff819065(v=vs.85).aspx - "Resource Limits (Direct3D 11)" - "Number of elements in a constant buffer D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096)"
		// -> One element = float4 = 16 bytes
		mCapabilities.maximumUniformBufferSize = 4096 * 16;

		// Left-handed coordinate system with clip space depth value range 0..1
		mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = true;

		// Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
		mCapabilities.individualUniforms = false;

		// Base vertex supported for draw calls?
		mCapabilities.baseVertex = true;

		// Vulkan has native multithreading
		mCapabilities.nativeMultithreading = false;	// TODO(co) Enable native multithreading when done

		// Vulkan has shader bytecode support
		mCapabilities.shaderBytecode = false;	// TODO(co) Vulkan has shader bytecode support, set "mCapabilities.shaderBytecode" to true later on

		// Is there support for vertex shaders (VS)?
		mCapabilities.vertexShader = true;

		// Is there support for fragment shaders (FS)?
		mCapabilities.fragmentShader = true;

		// Is there support for task shaders (TS) and mesh shaders (MS)?
		mCapabilities.meshShader = false;// TODO: (nullptr != vkCmdDrawMeshTasksNV);

		// Is there support for compute shaders (CS)?
		mCapabilities.computeShader = true;
	}

	void VulkanRhi::unsetGraphicsVertexArray()
	{
		// Release the currently used vertex array reference, in case we have one
		if (nullptr != mVertexArray)
		{
			// Do nothing since the Vulkan specification says "bindingCount must be greater than 0"
			// vkCmdBindVertexBuffers(getVulkanContext().getVkCommandBuffer(), 0, 0, nullptr, nullptr);

			// Release reference
			mVertexArray->ReleaseReference();
			mVertexArray = nullptr;
		}
	}

	void VulkanRhi::beginVulkanRenderPass()
	{
		// Sanity checks
		SE_ASSERT(!mInsideVulkanRenderPass, "We're already inside a Vulkan render pass")
		SE_ASSERT(nullptr != mRenderTarget, "Can't begin a Vulkan render pass without a render target set")

		// Start Vulkan render pass
		const uint32_t numberOfAttachments = static_cast<const RenderPass&>(mRenderTarget->getRenderPass()).getNumberOfAttachments();
		SE_ASSERT(numberOfAttachments < 9, "Vulkan only supports 8 render pass attachments")
		switch (mRenderTarget->getResourceType())
		{
			case Rhi::ResourceType::SWAP_CHAIN:
			{
				const SwapChain* swapChain = static_cast<SwapChain*>(mRenderTarget);
				::detail::beginVulkanRenderPass(*mRenderTarget, swapChain->getVkRenderPass(), swapChain->getCurrentVkFramebuffer(), numberOfAttachments, mVkClearValues, getVulkanContext().getVkCommandBuffer());
				break;
			}

			case Rhi::ResourceType::FRAMEBUFFER:
			{
				const Framebuffer* framebuffer = static_cast<Framebuffer*>(mRenderTarget);
				::detail::beginVulkanRenderPass(*mRenderTarget, framebuffer->getVkRenderPass(), framebuffer->getVkFramebuffer(), numberOfAttachments, mVkClearValues, getVulkanContext().getVkCommandBuffer());
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
		mInsideVulkanRenderPass = true;
	}
} // VulkanRhi

Rhi::IRhi* createVulkanRhiInstance(const handle& nativeWindowHandle)
{
	return RHI_NEW(VulkanRhi::VulkanRhi)(nativeWindowHandle);
}
#endif // SE_VULKAN
#pragma once

#if SE_RHI_STATISTICS
namespace Rhi
{
	class Statistics final
	{
	public:
		Statistics() = default;

		[[nodiscard]] inline uint32_t GetNumberOfCurrentResources() const
		{
			// Calculate the current number of resource instances
			return
				currentNumberOfRootSignatures +
				currentNumberOfResourceGroups +
				currentNumberOfGraphicsPrograms +
				currentNumberOfVertexArrays +
				currentNumberOfRenderPasses +
				currentNumberOfQueryPools +
				// IRenderTarget
				currentNumberOfSwapChains +
				currentNumberOfFramebuffers +
				// IBuffer
				currentNumberOfVertexBuffers +
				currentNumberOfIndexBuffers +
				currentNumberOfTextureBuffers +
				currentNumberOfStructuredBuffers +
				currentNumberOfIndirectBuffers +
				currentNumberOfUniformBuffers +
				// ITexture
				currentNumberOfTexture1Ds +
				currentNumberOfTexture1DArrays +
				currentNumberOfTexture2Ds +
				currentNumberOfTexture2DArrays +
				currentNumberOfTexture3Ds +
				currentNumberOfTextureCubes +
				currentNumberOfTextureCubeArrays +
				// IState
				currentNumberOfGraphicsPipelineStates +
				currentNumberOfComputePipelineStates +
				currentNumberOfSamplerStates +
				// IShader
				currentNumberOfVertexShaders +
				currentNumberOfTessellationControlShaders +
				currentNumberOfTessellationEvaluationShaders +
				currentNumberOfGeometryShaders +
				currentNumberOfFragmentShaders +
				currentNumberOfTaskShaders +
				currentNumberOfMeshShaders +
				currentNumberOfComputeShaders;
		}

		inline void DebugOutputCurrentResouces() const
		{
			// Start
			RHI_LOG(INFORMATION, "** Number of current RHI resource instances **");

			// Misc
			RHI_LOG(INFORMATION, "Root signatures: %u", currentNumberOfRootSignatures.load());
			RHI_LOG(INFORMATION, "Resource groups: %u", currentNumberOfResourceGroups.load());
			RHI_LOG(INFORMATION, "Graphics programs: %u", currentNumberOfGraphicsPrograms.load());
			RHI_LOG(INFORMATION, "Vertex arrays: %u", currentNumberOfVertexArrays.load());
			RHI_LOG(INFORMATION, "Render passes: %u", currentNumberOfRenderPasses.load());
			RHI_LOG(INFORMATION, "Query pools: %u", currentNumberOfQueryPools.load());

			// IRenderTarget
			RHI_LOG(INFORMATION, "Swap chains: %u", currentNumberOfSwapChains.load());
			RHI_LOG(INFORMATION, "Framebuffers: %u", currentNumberOfFramebuffers.load());

			// IBuffer
			RHI_LOG(INFORMATION, "Vertex buffers: %u", currentNumberOfVertexBuffers.load());
			RHI_LOG(INFORMATION, "Index buffers: %u", currentNumberOfIndexBuffers.load());
			RHI_LOG(INFORMATION, "Texture buffers: %u", currentNumberOfTextureBuffers.load());
			RHI_LOG(INFORMATION, "Structured buffers: %u", currentNumberOfStructuredBuffers.load());
			RHI_LOG(INFORMATION, "Indirect buffers: %u", currentNumberOfIndirectBuffers.load());
			RHI_LOG(INFORMATION, "Uniform buffers: %u", currentNumberOfUniformBuffers.load());

			// ITexture
			RHI_LOG(INFORMATION, "1D textures: %u", currentNumberOfTexture1Ds.load());
			RHI_LOG(INFORMATION, "1D texture arrays: %u", currentNumberOfTexture1DArrays.load());
			RHI_LOG(INFORMATION, "2D textures: %u", currentNumberOfTexture2Ds.load());
			RHI_LOG(INFORMATION, "2D texture arrays: %u", currentNumberOfTexture2DArrays.load());
			RHI_LOG(INFORMATION, "3D textures: %u", currentNumberOfTexture3Ds.load());
			RHI_LOG(INFORMATION, "Cube textures: %u", currentNumberOfTextureCubes.load());
			RHI_LOG(INFORMATION, "Cube texture arrays: %u", currentNumberOfTextureCubeArrays.load());

			// IState
			RHI_LOG(INFORMATION, "Graphics pipeline states: %u", currentNumberOfGraphicsPipelineStates.load());
			RHI_LOG(INFORMATION, "Compute pipeline states: %u", currentNumberOfComputePipelineStates.load());
			RHI_LOG(INFORMATION, "Sampler states: %u", currentNumberOfSamplerStates.load());

			// IShader
			RHI_LOG(INFORMATION, "Vertex shaders: %u", currentNumberOfVertexShaders.load());
			RHI_LOG(INFORMATION, "Tessellation control shaders: %u", currentNumberOfTessellationControlShaders.load());
			RHI_LOG(INFORMATION, "Tessellation evaluation shaders: %u", currentNumberOfTessellationEvaluationShaders.load());
			RHI_LOG(INFORMATION, "Geometry shaders: %u", currentNumberOfGeometryShaders.load());
			RHI_LOG(INFORMATION, "Fragment shaders: %u", currentNumberOfFragmentShaders.load());
			RHI_LOG(INFORMATION, "Task shaders: %u", currentNumberOfTaskShaders.load());
			RHI_LOG(INFORMATION, "Mesh shaders: %u", currentNumberOfMeshShaders.load());
			RHI_LOG(INFORMATION, "Compute shaders: %u", currentNumberOfComputeShaders.load());

			// End
			RHI_LOG(INFORMATION, "***************************************************");
		}

		// Resources
		std::atomic<uint32_t> currentNumberOfRootSignatures = 0;	// Current number of root signature instances
		std::atomic<uint32_t> numberOfCreatedRootSignatures = 0;	// Number of created root signature instances
		std::atomic<uint32_t> currentNumberOfResourceGroups = 0;	// Current number of resource group instances
		std::atomic<uint32_t> numberOfCreatedResourceGroups = 0;	// Number of created resource group instances
		std::atomic<uint32_t> currentNumberOfGraphicsPrograms = 0;	// Current number of graphics program instances
		std::atomic<uint32_t> numberOfCreatedGraphicsPrograms = 0;	// Number of created graphics program instances
		std::atomic<uint32_t> currentNumberOfVertexArrays = 0;		// Current number of vertex array object (VAO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> numberOfCreatedVertexArrays = 0;		// Number of created vertex array object (VAO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> currentNumberOfRenderPasses = 0;		// Current number of render pass instances
		std::atomic<uint32_t> numberOfCreatedRenderPasses = 0;		// Number of created render pass instances
		std::atomic<uint32_t> currentNumberOfQueryPools = 0;		// Current number of asynchronous query pool instances
		std::atomic<uint32_t> numberOfCreatedQueryPools = 0;		// Number of created asynchronous query pool instances
		// IRenderTarget
		std::atomic<uint32_t> currentNumberOfSwapChains = 0;		// Current number of swap chain instances
		std::atomic<uint32_t> numberOfCreatedSwapChains = 0;		// Number of created swap chain instances
		std::atomic<uint32_t> currentNumberOfFramebuffers = 0;		// Current number of framebuffer object (FBO) instances
		std::atomic<uint32_t> numberOfCreatedFramebuffers = 0;		// Number of created framebuffer object (FBO) instances
		// IBuffer
		std::atomic<uint32_t> currentNumberOfVertexBuffers = 0;		// Current number of vertex buffer object (VBO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> numberOfCreatedVertexBuffers = 0;		// Number of created vertex buffer object (VBO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> currentNumberOfIndexBuffers = 0;		// Current number of index buffer object (IBO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> numberOfCreatedIndexBuffers = 0;		// Number of created index buffer object (IBO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> currentNumberOfTextureBuffers = 0;	// Current number of texture buffer object (TBO) instances
		std::atomic<uint32_t> numberOfCreatedTextureBuffers = 0;	// Number of created texture buffer object (TBO) instances
		std::atomic<uint32_t> currentNumberOfStructuredBuffers = 0;	// Current number of structured buffer object (SBO) instances
		std::atomic<uint32_t> numberOfCreatedStructuredBuffers = 0;	// Number of created structured buffer object (SBO) instances
		std::atomic<uint32_t> currentNumberOfIndirectBuffers = 0;	// Current number of indirect buffer object instances
		std::atomic<uint32_t> numberOfCreatedIndirectBuffers = 0;	// Number of created indirect buffer object instances
		std::atomic<uint32_t> currentNumberOfUniformBuffers = 0;	// Current number of uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instances
		std::atomic<uint32_t> numberOfCreatedUniformBuffers = 0;	// Number of created uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instances
		// ITexture
		std::atomic<uint32_t> currentNumberOfTexture1Ds = 0;		// Current number of texture 1D instances
		std::atomic<uint32_t> numberOfCreatedTexture1Ds = 0;		// Number of created texture 1D instances
		std::atomic<uint32_t> currentNumberOfTexture1DArrays = 0;	// Current number of texture 1D array instances
		std::atomic<uint32_t> numberOfCreatedTexture1DArrays = 0;	// Number of created texture 1D array instances
		std::atomic<uint32_t> currentNumberOfTexture2Ds = 0;		// Current number of texture 2D instances
		std::atomic<uint32_t> numberOfCreatedTexture2Ds = 0;		// Number of created texture 2D instances
		std::atomic<uint32_t> currentNumberOfTexture2DArrays = 0;	// Current number of texture 2D array instances
		std::atomic<uint32_t> numberOfCreatedTexture2DArrays = 0;	// Number of created texture 2D array instances
		std::atomic<uint32_t> currentNumberOfTexture3Ds = 0;		// Current number of texture 3D instances
		std::atomic<uint32_t> numberOfCreatedTexture3Ds = 0;		// Number of created texture 3D instances
		std::atomic<uint32_t> currentNumberOfTextureCubes = 0;		// Current number of texture cube instances
		std::atomic<uint32_t> numberOfCreatedTextureCubes = 0;		// Number of created texture cube instances
		std::atomic<uint32_t> currentNumberOfTextureCubeArrays = 0;	// Current number of texture cube array instances
		std::atomic<uint32_t> numberOfCreatedTextureCubeArrays = 0;	// Number of created texture cube array instances
		// IState
		std::atomic<uint32_t> currentNumberOfGraphicsPipelineStates = 0;// Current number of graphics pipeline state (PSO) instances
		std::atomic<uint32_t> numberOfCreatedGraphicsPipelineStates = 0;// Number of created graphics pipeline state (PSO) instances
		std::atomic<uint32_t> currentNumberOfComputePipelineStates = 0;// Current number of compute pipeline state (PSO) instances
		std::atomic<uint32_t> numberOfCreatedComputePipelineStates = 0;// Number of created compute pipeline state (PSO) instances
		std::atomic<uint32_t> currentNumberOfSamplerStates = 0;		// Current number of sampler state instances
		std::atomic<uint32_t> numberOfCreatedSamplerStates = 0;		// Number of created sampler state instances
		// IShader
		std::atomic<uint32_t> currentNumberOfVertexShaders = 0;		// Current number of vertex shader (VS) instances
		std::atomic<uint32_t> numberOfCreatedVertexShaders = 0;		// Number of created vertex shader (VS) instances
		std::atomic<uint32_t> currentNumberOfTessellationControlShaders = 0;// Current number of tessellation control shader (TCS, "hull shader" in Direct3D terminology) instances
		std::atomic<uint32_t> numberOfCreatedTessellationControlShaders = 0;// Number of created tessellation control shader (TCS, "hull shader" in Direct3D terminology) instances
		std::atomic<uint32_t> currentNumberOfTessellationEvaluationShaders = 0;// Current number of tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) instances
		std::atomic<uint32_t> numberOfCreatedTessellationEvaluationShaders = 0;// Number of created tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) instances
		std::atomic<uint32_t> currentNumberOfGeometryShaders = 0;	// Current number of geometry shader (GS) instances
		std::atomic<uint32_t> numberOfCreatedGeometryShaders = 0;	// Number of created geometry shader (GS) instances
		std::atomic<uint32_t> currentNumberOfFragmentShaders = 0;	// Current number of fragment shader (FS, "pixel shader" in Direct3D terminology) instances
		std::atomic<uint32_t> numberOfCreatedFragmentShaders = 0;	// Number of created fragment shader (FS, "pixel shader" in Direct3D terminology) instances
		std::atomic<uint32_t> currentNumberOfTaskShaders = 0;		// Current number of task shader (TS) instances
		std::atomic<uint32_t> numberOfCreatedTaskShaders = 0;		// Number of created task shader (TS) instances
		std::atomic<uint32_t> currentNumberOfMeshShaders = 0;		// Current number of mesh shader (MS) instances
		std::atomic<uint32_t> numberOfCreatedMeshShaders = 0;		// Number of created mesh shader (MS) instances
		std::atomic<uint32_t> currentNumberOfComputeShaders = 0;	// Current number of compute shader (CS) instances
		std::atomic<uint32_t> numberOfCreatedComputeShaders = 0;	// Number of created compute shader (CS) instances

	private:
		explicit Statistics(const Statistics&) = delete;
		Statistics& operator=(const Statistics&) = delete;
	};
} // namespace Rhi
#endif // SE_RHI_STATISTICS
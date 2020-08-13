#pragma once

#if SE_STATISTICS
namespace Rhi
{
	class Statistics final
	{
	public:
		// Resources
		std::atomic<uint32_t> currentNumberOfRootSignatures;				// Current number of root signature instances
		std::atomic<uint32_t> numberOfCreatedRootSignatures;				// Number of created root signature instances
		std::atomic<uint32_t> currentNumberOfResourceGroups;				// Current number of resource group instances
		std::atomic<uint32_t> numberOfCreatedResourceGroups;				// Number of created resource group instances
		std::atomic<uint32_t> currentNumberOfGraphicsPrograms;				// Current number of graphics program instances
		std::atomic<uint32_t> numberOfCreatedGraphicsPrograms;				// Number of created graphics program instances
		std::atomic<uint32_t> currentNumberOfVertexArrays;					// Current number of vertex array object (VAO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> numberOfCreatedVertexArrays;					// Number of created vertex array object (VAO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> currentNumberOfRenderPasses;					// Current number of render pass instances
		std::atomic<uint32_t> numberOfCreatedRenderPasses;					// Number of created render pass instances
		std::atomic<uint32_t> currentNumberOfQueryPools;					// Current number of asynchronous query pool instances
		std::atomic<uint32_t> numberOfCreatedQueryPools;					// Number of created asynchronous query pool instances
		// IRenderTarget
		std::atomic<uint32_t> currentNumberOfSwapChains;					// Current number of swap chain instances
		std::atomic<uint32_t> numberOfCreatedSwapChains;					// Number of created swap chain instances
		std::atomic<uint32_t> currentNumberOfFramebuffers;					// Current number of framebuffer object (FBO) instances
		std::atomic<uint32_t> numberOfCreatedFramebuffers;					// Number of created framebuffer object (FBO) instances
		// IBuffer
		std::atomic<uint32_t> currentNumberOfVertexBuffers;					// Current number of vertex buffer object (VBO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> numberOfCreatedVertexBuffers;					// Number of created vertex buffer object (VBO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> currentNumberOfIndexBuffers;					// Current number of index buffer object (IBO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> numberOfCreatedIndexBuffers;					// Number of created index buffer object (IBO, input-assembler (IA) stage) instances
		std::atomic<uint32_t> currentNumberOfTextureBuffers;				// Current number of texture buffer object (TBO) instances
		std::atomic<uint32_t> numberOfCreatedTextureBuffers;				// Number of created texture buffer object (TBO) instances
		std::atomic<uint32_t> currentNumberOfStructuredBuffers;				// Current number of structured buffer object (SBO) instances
		std::atomic<uint32_t> numberOfCreatedStructuredBuffers;				// Number of created structured buffer object (SBO) instances
		std::atomic<uint32_t> currentNumberOfIndirectBuffers;				// Current number of indirect buffer object instances
		std::atomic<uint32_t> numberOfCreatedIndirectBuffers;				// Number of created indirect buffer object instances
		std::atomic<uint32_t> currentNumberOfUniformBuffers;				// Current number of uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instances
		std::atomic<uint32_t> numberOfCreatedUniformBuffers;				// Number of created uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instances
		// ITexture
		std::atomic<uint32_t> currentNumberOfTexture1Ds;					// Current number of texture 1D instances
		std::atomic<uint32_t> numberOfCreatedTexture1Ds;					// Number of created texture 1D instances
		std::atomic<uint32_t> currentNumberOfTexture1DArrays;				// Current number of texture 1D array instances
		std::atomic<uint32_t> numberOfCreatedTexture1DArrays;				// Number of created texture 1D array instances
		std::atomic<uint32_t> currentNumberOfTexture2Ds;					// Current number of texture 2D instances
		std::atomic<uint32_t> numberOfCreatedTexture2Ds;					// Number of created texture 2D instances
		std::atomic<uint32_t> currentNumberOfTexture2DArrays;				// Current number of texture 2D array instances
		std::atomic<uint32_t> numberOfCreatedTexture2DArrays;				// Number of created texture 2D array instances
		std::atomic<uint32_t> currentNumberOfTexture3Ds;					// Current number of texture 3D instances
		std::atomic<uint32_t> numberOfCreatedTexture3Ds;					// Number of created texture 3D instances
		std::atomic<uint32_t> currentNumberOfTextureCubes;					// Current number of texture cube instances
		std::atomic<uint32_t> numberOfCreatedTextureCubes;					// Number of created texture cube instances
		std::atomic<uint32_t> currentNumberOfTextureCubeArrays;				// Current number of texture cube array instances
		std::atomic<uint32_t> numberOfCreatedTextureCubeArrays;				// Number of created texture cube array instances
		// IState
		std::atomic<uint32_t> currentNumberOfGraphicsPipelineStates;		// Current number of graphics pipeline state (PSO) instances
		std::atomic<uint32_t> numberOfCreatedGraphicsPipelineStates;		// Number of created graphics pipeline state (PSO) instances
		std::atomic<uint32_t> currentNumberOfComputePipelineStates;			// Current number of compute pipeline state (PSO) instances
		std::atomic<uint32_t> numberOfCreatedComputePipelineStates;			// Number of created compute pipeline state (PSO) instances
		std::atomic<uint32_t> currentNumberOfSamplerStates;					// Current number of sampler state instances
		std::atomic<uint32_t> numberOfCreatedSamplerStates;					// Number of created sampler state instances
		// IShader
		std::atomic<uint32_t> currentNumberOfVertexShaders;					// Current number of vertex shader (VS) instances
		std::atomic<uint32_t> numberOfCreatedVertexShaders;					// Number of created vertex shader (VS) instances
		std::atomic<uint32_t> currentNumberOfTessellationControlShaders;	// Current number of tessellation control shader (TCS, "hull shader" in Direct3D terminology) instances
		std::atomic<uint32_t> numberOfCreatedTessellationControlShaders;	// Number of created tessellation control shader (TCS, "hull shader" in Direct3D terminology) instances
		std::atomic<uint32_t> currentNumberOfTessellationEvaluationShaders;	// Current number of tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) instances
		std::atomic<uint32_t> numberOfCreatedTessellationEvaluationShaders;	// Number of created tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) instances
		std::atomic<uint32_t> currentNumberOfGeometryShaders;				// Current number of geometry shader (GS) instances
		std::atomic<uint32_t> numberOfCreatedGeometryShaders;				// Number of created geometry shader (GS) instances
		std::atomic<uint32_t> currentNumberOfFragmentShaders;				// Current number of fragment shader (FS, "pixel shader" in Direct3D terminology) instances
		std::atomic<uint32_t> numberOfCreatedFragmentShaders;				// Number of created fragment shader (FS, "pixel shader" in Direct3D terminology) instances
		std::atomic<uint32_t> currentNumberOfTaskShaders;					// Current number of task shader (TS) instances
		std::atomic<uint32_t> numberOfCreatedTaskShaders;					// Number of created task shader (TS) instances
		std::atomic<uint32_t> currentNumberOfMeshShaders;					// Current number of mesh shader (MS) instances
		std::atomic<uint32_t> numberOfCreatedMeshShaders;					// Number of created mesh shader (MS) instances
		std::atomic<uint32_t> currentNumberOfComputeShaders;				// Current number of compute shader (CS) instances
		std::atomic<uint32_t> numberOfCreatedComputeShaders;				// Number of created compute shader (CS) instances

		inline Statistics() :
			currentNumberOfRootSignatures(0),
			numberOfCreatedRootSignatures(0),
			currentNumberOfResourceGroups(0),
			numberOfCreatedResourceGroups(0),
			currentNumberOfGraphicsPrograms(0),
			numberOfCreatedGraphicsPrograms(0),
			currentNumberOfVertexArrays(0),
			numberOfCreatedVertexArrays(0),
			currentNumberOfRenderPasses(0),
			numberOfCreatedRenderPasses(0),
			currentNumberOfQueryPools(0),
			numberOfCreatedQueryPools(0),
			// IRenderTarget
			currentNumberOfSwapChains(0),
			numberOfCreatedSwapChains(0),
			currentNumberOfFramebuffers(0),
			numberOfCreatedFramebuffers(0),
			// IBuffer
			currentNumberOfVertexBuffers(0),
			numberOfCreatedVertexBuffers(0),
			currentNumberOfIndexBuffers(0),
			numberOfCreatedIndexBuffers(0),
			currentNumberOfTextureBuffers(0),
			numberOfCreatedTextureBuffers(0),
			currentNumberOfStructuredBuffers(0),
			numberOfCreatedStructuredBuffers(0),
			currentNumberOfIndirectBuffers(0),
			numberOfCreatedIndirectBuffers(0),
			currentNumberOfUniformBuffers(0),
			numberOfCreatedUniformBuffers(0),
			// ITexture
			currentNumberOfTexture1Ds(0),
			numberOfCreatedTexture1Ds(0),
			currentNumberOfTexture1DArrays(0),
			numberOfCreatedTexture1DArrays(0),
			currentNumberOfTexture2Ds(0),
			numberOfCreatedTexture2Ds(0),
			currentNumberOfTexture2DArrays(0),
			numberOfCreatedTexture2DArrays(0),
			currentNumberOfTexture3Ds(0),
			numberOfCreatedTexture3Ds(0),
			currentNumberOfTextureCubes(0),
			numberOfCreatedTextureCubes(0),
			currentNumberOfTextureCubeArrays(0),
			numberOfCreatedTextureCubeArrays(0),
			// IState
			currentNumberOfGraphicsPipelineStates(0),
			numberOfCreatedGraphicsPipelineStates(0),
			currentNumberOfComputePipelineStates(0),
			numberOfCreatedComputePipelineStates(0),
			currentNumberOfSamplerStates(0),
			numberOfCreatedSamplerStates(0),
			// IShader
			currentNumberOfVertexShaders(0),
			numberOfCreatedVertexShaders(0),
			currentNumberOfTessellationControlShaders(0),
			numberOfCreatedTessellationControlShaders(0),
			currentNumberOfTessellationEvaluationShaders(0),
			numberOfCreatedTessellationEvaluationShaders(0),
			currentNumberOfGeometryShaders(0),
			numberOfCreatedGeometryShaders(0),
			currentNumberOfFragmentShaders(0),
			numberOfCreatedFragmentShaders(0),
			currentNumberOfTaskShaders(0),
			numberOfCreatedTaskShaders(0),
			currentNumberOfMeshShaders(0),
			numberOfCreatedMeshShaders(0),
			currentNumberOfComputeShaders(0),
			numberOfCreatedComputeShaders(0)
		{
		}

		[[nodiscard]] inline uint32_t getNumberOfCurrentResources() const
		{
			// Calculate the current number of resource instances
			return	currentNumberOfRootSignatures +
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

		inline void debugOutputCurrentResouces() const
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

	private:
		explicit Statistics(const Statistics&) = delete;
		Statistics& operator=(const Statistics&) = delete;
	};
} // namespace Rhi
#endif // SE_STATISTICS
#pragma once

namespace Rhi
{
	/**
	*  @brief
	*    Draw instanced arguments
	*
	*  @note
	*    - This structure directly maps to Direct3D 12, DirectX 11, Vulkan, Metal and OpenGL, do not change it
	*    - Direct3D 12: "D3D12_DRAW_ARGUMENTS"
	*    - Direct3D 11: No structure documentation found, only indications that same arguments and order as "ID3D11DeviceContext::DrawInstanced()"
	*    - Vulkan: "VkDrawIndirectCommand"
	*    - Metal: "MTLDrawPrimitivesIndirectArguments"
	*    - OpenGL:"DrawArraysIndirectCommand"
	*
	*  @see
	*    - "D3D12_DRAW_ARGUMENTS"-documentation for details
	*/
	struct DrawArguments final
	{
		uint32_t vertexCountPerInstance;
		uint32_t instanceCount;
		uint32_t startVertexLocation;
		uint32_t startInstanceLocation;
		inline DrawArguments(uint32_t _vertexCountPerInstance, uint32_t _instanceCount = 1, uint32_t _startVertexLocation = 0, uint32_t _startInstanceLocation = 0) :
			vertexCountPerInstance(_vertexCountPerInstance),
			instanceCount(_instanceCount),
			startVertexLocation(_startVertexLocation),
			startInstanceLocation(_startInstanceLocation)
		{
		}
	};

	/**
	*  @brief
	*    Draw indexed instanced arguments
	*
	*  @note
	*    - This structure directly maps to Direct3D 12, DirectX 11, Vulkan, Metal and OpenGL, do not change it
	*    - Direct3D 12: "D3D12_DRAW_INDEXED_ARGUMENTS"
	*    - Direct3D 11: No structure documentation found, only indications that same arguments and order as "ID3D11DeviceContext::DrawIndexedInstanced()"
	*    - Vulkan: "VkDrawIndexedIndirectCommand"
	*    - Metal: "MTLDrawIndexedPrimitivesIndirectArguments"
	*    - OpenGL:"DrawElementsIndirectCommand"
	*
	*  @see
	*    - "D3D12_DRAW_INDEXED_ARGUMENTS"-documentation for details
	*/
	struct DrawIndexedArguments final
	{
		uint32_t indexCountPerInstance;
		uint32_t instanceCount;
		uint32_t startIndexLocation;
		int32_t  baseVertexLocation;
		uint32_t startInstanceLocation;
		inline DrawIndexedArguments(uint32_t _indexCountPerInstance, uint32_t _instanceCount = 1, uint32_t _startIndexLocation = 0, int32_t _baseVertexLocation = 0, uint32_t _startInstanceLocation = 0) :
			indexCountPerInstance(_indexCountPerInstance),
			instanceCount(_instanceCount),
			startIndexLocation(_startIndexLocation),
			baseVertexLocation(_baseVertexLocation),
			startInstanceLocation(_startInstanceLocation)
		{
		}
	};

	/**
	*  @brief
	*    Draw mesh tasks arguments
	*
	*  @note
	*    - This structure directly maps to Vulkan and OpenGL, do not change it
	*    - Vulkan: "VkDrawMeshTasksIndirectCommandNV"
	*    - OpenGL:"DrawMeshTasksIndirectCommandNV"
	*    - Direct3D 12 isn't compatible since it's using "D3D12_DISPATCH_ARGUMENTS" ("ThreadGroupCountX", "ThreadGroupCountY", "ThreadGroupCountZ")
	*
	*  @see
	*    - "VkDrawMeshTasksIndirectCommandNV"-documentation for details
	*/
	struct DrawMeshTasksArguments final
	{
		uint32_t numberOfTasks;
		uint32_t firstTask;
		inline DrawMeshTasksArguments(uint32_t _numberOfTasks, uint32_t _firstTask = 0) :
			numberOfTasks(_numberOfTasks),
			firstTask(_firstTask)
		{
		}
	};
} // namespace Rhi
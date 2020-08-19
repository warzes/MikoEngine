#pragma once

namespace Rhi
{
	/**
	*  @brief
	*    Asynchronous query type
	*/
	enum class QueryType
	{
		OCCLUSION = 0,	// Occlusion query, result is a "uint64_t" containing the number of passed samples
		PIPELINE_STATISTICS = 1,	// Pipeline statistics query, result is a "Rhi::PipelineStatisticsQueryResult"
		TIMESTAMP = 2		// Timestamp query, result is a "uint64_t" containing a timestamp in nanosecond
	};

	/**
	*  @brief
	*    Asynchronous query control flags
	*/
	struct QueryControlFlags final
	{
		enum Enum
		{
			PRECISE = 1 << 0	// Query control precise
		};
	};

	/**
	*  @brief
	*    Asynchronous query result flags
	*/
	struct QueryResultFlags final
	{
		enum Enum
		{
			WAIT = 1 << 0	// Wait for the result
		};
	};

	/**
	*  @brief
	*    Asynchronous pipeline statistics query result
	*
	*  @note
	*    - This pipeline statistics structure maps directly to Direct3D 11 and Direct3D 12, do not change it
	*
	*  @see
	*    - "D3D11_QUERY_DATA_PIPELINE_STATISTICS"-documentation for details
	*/
	struct PipelineStatisticsQueryResult final
	{
		uint64_t numberOfInputAssemblerVertices;					// Number of vertices read by input assembler
		uint64_t numberOfInputAssemblerPrimitives;					// Number of primitives read by the input assembler
		uint64_t numberOfVertexShaderInvocations;					// Number of times a vertex shader was invoked
		uint64_t numberOfGeometryShaderInvocations;					// Number of times a geometry shader was invoked
		uint64_t numberOfGeometryShaderOutputPrimitives;			// Number of primitives output by a geometry shader
		uint64_t numberOfClippingInputPrimitives;					// Number of primitives that were sent to the rasterizer
		uint64_t numberOfClippingOutputPrimitives;					// Number of primitives that were rendered
		uint64_t numberOfFragmentShaderInvocations;					// Number of times a fragment shader (FS, "pixel shader" in Direct3D terminology) was invoked
		uint64_t numberOfTessellationControlShaderInvocations;		// Number of times a hull shader (TCS, "hull shader" in Direct3D terminology) was invoked
		uint64_t numberOfTessellationEvaluationShaderInvocations;	// Number of times a domain shader (TES, "domain shader" in Direct3D terminology) was invoked
		uint64_t numberOfComputeShaderInvocations;					// Number of times a compute shader was invoked
	};
} // namespace Rhi
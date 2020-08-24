#pragma once

namespace Rhi
{
	enum class QueryType
	{
		OCCLUSION           = 0, // Occlusion query, result is a "uint64_t" containing the number of passed samples
		PIPELINE_STATISTICS = 1, // Pipeline statistics query, result is a "Rhi::PipelineStatisticsQueryResult"
		TIMESTAMP           = 2  // Timestamp query, result is a "uint64_t" containing a timestamp in nanosecond
	};

	struct QueryControlFlags final
	{
		enum Enum
		{
			PRECISE = 1 << 0     // Query control precise
		};
	};

	struct QueryResultFlags final
	{
		enum Enum
		{
			WAIT = 1 << 0        // Wait for the result
		};
	};

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
#pragma once

namespace Rhi
{
	enum class MapType
	{
		READ = 1,
		WRITE = 2,
		READ_WRITE = 3,
		WRITE_DISCARD = 4,
		WRITE_NO_OVERWRITE = 5
	};

	struct ClearFlag final
	{
		enum Enum
		{
			COLOR = 1 << 0,
			DEPTH = 1 << 1,
			STENCIL = 1 << 2,
			COLOR_DEPTH = COLOR | DEPTH 
		};
	};

	enum class ComparisonFunc
	{
		NEVER = 1,         // Never pass the comparison
		LESS = 2,          // If the source data is less than the destination data, the comparison passes
		EQUAL = 3,         // If the source data is equal to the destination data, the comparison passes
		LESS_EQUAL = 4,    // If the source data is less than or equal to the destination data, the comparison passes
		GREATER = 5,       // If the source data is greater than the destination data, the comparison passes
		NOT_EQUAL = 6,     // If the source data is not equal to the destination data, the comparison passes
		GREATER_EQUAL = 7, // If the source data is greater than or equal to the destination data, the comparison passes
		ALWAYS = 8         // Always pass the comparison
	};

	struct ColorWriteEnableFlag final
	{
		enum Enum
		{
			RED = 1,
			GREEN = 2,
			BLUE = 4,
			ALPHA = 8,
			ALL = (((RED | GREEN) | BLUE) | ALPHA)
		};
	};

	struct MappedSubresource final
	{
		void*	 data;
		uint32_t rowPitch;
		uint32_t depthPitch;
	};

	struct Viewport final
	{
		float topLeftX;	// Top left x start position
		float topLeftY;	// Top left y start position
		float width;	// Viewport width
		float height;	// Viewport height
		float minDepth;	// Minimum depth value, usually 0.0f, between [0, 1]
		float maxDepth;	// Maximum depth value, usually 1.0f, between [0, 1]
	};

	struct ScissorRectangle final
	{
		long topLeftX;		// Top left x-coordinate of the scissor rectangle
		long topLeftY;		// Top left y-coordinate of the scissor rectangle
		long bottomRightX;	// Bottom right x-coordinate of the scissor rectangle
		long bottomRightY;	// Bottom right y-coordinate of the scissor rectangle
	};

	enum class ResourceType
	{
		ROOT_SIGNATURE = 0,		// Root signature
		RESOURCE_GROUP = 1,		// Resource group
		GRAPHICS_PROGRAM = 2,	// Graphics program, "Rhi::IShader"-related
		VERTEX_ARRAY = 3,		// Vertex array object (VAO, input-assembler (IA) stage), "Rhi::IBuffer"-related
		RENDER_PASS = 4,		// Render pass
		QUERY_POOL = 5,			// Asynchronous query pool
		// IRenderTarget
		SWAP_CHAIN = 6,			// Swap chain
		FRAMEBUFFER = 7,		// Framebuffer object (FBO)
		// IBuffer
		VERTEX_BUFFER = 8,		// Vertex buffer object (VBO, input-assembler (IA) stage)
		INDEX_BUFFER = 9,		// Index buffer object (IBO, input-assembler (IA) stage)
		TEXTURE_BUFFER = 10,	// Texture buffer object (TBO)
		STRUCTURED_BUFFER = 11,	// Structured buffer object (SBO)
		INDIRECT_BUFFER = 12,	// Indirect buffer object
		UNIFORM_BUFFER = 13,	// Uniform buffer object (UBO, "constant buffer" in Direct3D terminology)
		// ITexture
		TEXTURE_1D = 14,		// Texture 1D
		TEXTURE_1D_ARRAY = 15,	// Texture 1D array
		TEXTURE_2D = 16,		// Texture 2D
		TEXTURE_2D_ARRAY = 17,	// Texture 2D array
		TEXTURE_3D = 18,		// Texture 3D
		TEXTURE_CUBE = 19,		// Texture cube
		TEXTURE_CUBE_ARRAY = 20,// Texture cube array
		// IState
			// IPipelineState
			GRAPHICS_PIPELINE_STATE = 21,// Graphics pipeline state (PSO)
			COMPUTE_PIPELINE_STATE = 22,// Compute pipeline state (PSO)
			SAMPLER_STATE = 23,	// Sampler state
			// IShader
			VERTEX_SHADER = 24,	// Vertex shader (VS)
			TESSELLATION_CONTROL_SHADER = 25,// Tessellation control shader (TCS, "hull shader" in Direct3D terminology)
			TESSELLATION_EVALUATION_SHADER = 26,// Tessellation evaluation shader (TES, "domain shader" in Direct3D terminology)
			GEOMETRY_SHADER = 27,// Geometry shader (GS)
			FRAGMENT_SHADER = 28,// Fragment shader (FS, "pixel shader" in Direct3D terminology)
			TASK_SHADER = 29,	// Task shader (TS, "amplification shader" in Direct3D terminology)
			MESH_SHADER = 30,	// Mesh shader (MS)
			COMPUTE_SHADER = 31	// Compute shader (CS)
	};

	// Holds information about an window into which the rendering should be done
	struct WindowHandle final
	{
		handle nativeWindowHandle; // The native window handle
		IRenderWindow *renderWindow; // A pointer to an "Rhi::IRenderWindow"-instance, can be a null pointer
	};

} // namespace Rhi
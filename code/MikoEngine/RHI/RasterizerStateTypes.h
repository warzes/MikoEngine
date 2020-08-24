#pragma once

namespace Rhi
{
	enum class FillMode
	{
		WIREFRAME = 2, // Wireframe
		SOLID     = 3  // Solid
	};

	enum class CullMode
	{
		NONE  = 1, // No culling
		FRONT = 2, // Do not draw triangles that are front-facing
		BACK  = 3  // Do not draw triangles that are back-facing
	};

	enum class ConservativeRasterizationMode
	{
		OFF = 0,	// Conservative rasterization is off
		ON = 1		// Conservative rasterization is on
	};

	struct RasterizerState final
	{
		FillMode						fillMode;						// Default: "Rhi::FillMode::SOLID"
		CullMode						cullMode;						// Default: "Rhi::CullMode::BACK"
		int								frontCounterClockwise;			// Select counter-clockwise polygons as front-facing? Boolean value. Default: "false"
		int								depthBias;						// Default: "0"
		float							depthBiasClamp;					// Default: "0.0f"
		float							slopeScaledDepthBias;			// Default: "0.0f"
		int								depthClipEnable;				// Boolean value. Default: "true"
		int								multisampleEnable;				// Boolean value. Default: "false"
		int								antialiasedLineEnable;			// Boolean value. Default: "false"
		unsigned int					forcedSampleCount;				// Default: "0"
		ConservativeRasterizationMode	conservativeRasterizationMode;	// Boolean value. >= Direct3D 12 only. Default: "false"
		int								scissorEnable;					// Boolean value. Not available in Vulkan or Direct3D 12 (scissor testing is always enabled). Default: "false"
	};
	// TODO: тоже что и BlendState
	struct RasterizerStateBuilder final
	{
		[[nodiscard]] static inline const RasterizerState& getDefaultRasterizerState()
		{
			static constexpr const RasterizerState RASTERIZER_STATE =
			{
				FillMode::SOLID,					// fillMode (Rhi::FillMode)	
				CullMode::BACK,						// cullMode (Rhi::CullMode)	
				false,								// frontCounterClockwise (int)	
				0,									// depthBias (int)
				0.0f,								// depthBiasClamp (float)	
				0.0f,								// slopeScaledDepthBias (float)	
				true,								// depthClipEnable (int)
				false,								// multisampleEnable (int)
				false,								// antialiasedLineEnable (int)
				0,									// forcedSampleCount (unsigned int)
				ConservativeRasterizationMode::OFF,	// conservativeRasterizationMode (Rhi::ConservativeRasterizationMode)
				false								// scissorEnable (int)
			};
			return RASTERIZER_STATE;
		}
	};
} // namespace Rhi
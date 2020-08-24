#pragma once

namespace Rhi
{
	enum class DepthWriteMask
	{
		ZERO = 0,
		ALL  = 1
	};

	enum class StencilOp
	{
		KEEP     = 1,
		ZERO     = 2,
		REPLACE  = 3,
		INCR_SAT = 4,
		DECR_SAT = 5,
		INVERT   = 6,
		INCREASE = 7,
		DECREASE = 8
	};

	struct DepthStencilOpDesc final
	{
		StencilOp		stencilFailOp;		// Default: "Rhi::StencilOp::KEEP"
		StencilOp		stencilDepthFailOp;	// Default: "Rhi::StencilOp::KEEP"
		StencilOp		stencilPassOp;		// Default: "Rhi::StencilOp::KEEP"
		ComparisonFunc	stencilFunc;		// Default: "Rhi::ComparisonFunc::ALWAYS"
	};

	struct DepthStencilState final
	{
		int					depthEnable;		// Boolean value. Default: "true"
		DepthWriteMask		depthWriteMask;		// Default: "Rhi::DepthWriteMask::ALL"
		ComparisonFunc		depthFunc;			// Default: "Rhi::ComparisonFunc::GREATER" instead of "Rhi::ComparisonFunc::LESS" due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
		int					stencilEnable;		// Boolean value. Default: "false"
		uint8_t				stencilReadMask;	// Default: "0xff"
		uint8_t				stencilWriteMask;	// Default: "0xff"
		DepthStencilOpDesc	frontFace;			// Default: See "Rhi::DepthStencilOpDesc"
		DepthStencilOpDesc	backFace;			// Default: See "Rhi::DepthStencilOpDesc"
	};
	// TODO: тоже что и с BlendState
	struct DepthStencilStateBuilder final
	{
		[[nodiscard]] static inline constexpr const DepthStencilState& getDefaultDepthStencilState()
		{
			constexpr const  DepthStencilState DEPTH_STENCIL_STATE =
			{
				true,						// depthEnable (int)
				DepthWriteMask::ALL,		// depthWriteMask (Rhi::DepthWriteMask)
				ComparisonFunc::GREATER,	// depthFunc (Rhi::ComparisonFunc)
				false,						// stencilEnable (int)
				0xff,						// stencilReadMask (uint8_t)
				0xff,						// stencilWriteMask (uint8_t)
				{ // sFrontFace (Rhi::DepthStencilOpDesc)
					StencilOp::KEEP,		// stencilFailOp (Rhi::StencilOp)
					StencilOp::KEEP,		// stencilDepthFailOp (Rhi::StencilOp)
					StencilOp::KEEP,		// stencilPassOp (Rhi::StencilOp)
					ComparisonFunc::ALWAYS	// stencilFunc (Rhi::ComparisonFunc)
				},
				{ // sBackFace (Rhi::DepthStencilOpDesc)
					StencilOp::KEEP,		// stencilFailOp (Rhi::StencilOp)
					StencilOp::KEEP,		// stencilDepthFailOp (Rhi::StencilOp)
					StencilOp::KEEP,		// stencilPassOp (Rhi::StencilOp)
					ComparisonFunc::ALWAYS	// stencilFunc (Rhi::ComparisonFunc)
				}
			};
			return DEPTH_STENCIL_STATE;
		}
	};
} // namespace Rhi
#pragma once

namespace Rhi
{
	// "D3D12_BLEND"-documentation for details
	enum class Blend
	{
		ZERO = 1,
		ONE = 2,
		SRC_COLOR = 3,
		INV_SRC_COLOR = 4,
		SRC_ALPHA = 5,
		INV_SRC_ALPHA = 6,
		DEST_ALPHA = 7,
		INV_DEST_ALPHA = 8,
		DEST_COLOR = 9,
		INV_DEST_COLOR = 10,
		SRC_ALPHA_SAT = 11,
		BLEND_FACTOR = 14,
		INV_BLEND_FACTOR = 15,
		SRC_1_COLOR = 16,
		INV_SRC_1_COLOR = 17,
		SRC_1_ALPHA = 18,
		INV_SRC_1_ALPHA = 19
	};

	// "D3D12_BLEND_OP"-documentation for details
	enum class BlendOp
	{
		ADD = 1,
		SUBTRACT = 2,
		REV_SUBTRACT = 3,
		MIN = 4,
		MAX = 5
	};

	// If you want to know how the default values were chosen, have a look into the "Rhi::BlendStateBuilder::getDefaultBlendState()"-implementation
	// "D3D12_RENDER_TARGET_BLEND_DESC"-documentation for details
	struct RenderTargetBlendDesc final
	{
		int		blendEnable;			// Boolean value. Default: "false"
		Blend	srcBlend;				// Default: "Rhi::Blend::ONE"
		Blend	destBlend;				// Default: "Rhi::Blend::ZERO"
		BlendOp	blendOp;				// Default: "Rhi::BlendOp::ADD"
		Blend	srcBlendAlpha;			// Default: "Rhi::Blend::ONE"
		Blend	destBlendAlpha;			// Default: "Rhi::Blend::ZERO"
		BlendOp	blendOpAlpha;			// Default: "Rhi::BlendOp::ADD"
		uint8_t	renderTargetWriteMask;	// Combination of "Rhi::ColorWriteEnableFlag"-flags. Default: "Rhi::ColorWriteEnableFlag::ALL"
	};

	// If you want to know how the default values were chosen, have a look into the "Rhi::BlendStateBuilder::getDefaultBlendState()"-implementation
	// "D3D12_BLEND_DESC"-documentation for details
	struct BlendState final
	{
		int					  alphaToCoverageEnable;	// Boolean value. Default: "false"
		int					  independentBlendEnable;	// Boolean value. Default: "false"
		RenderTargetBlendDesc renderTarget[8];			// Default: See "Rhi::RenderTargetBlendDesc"
	};

	struct BlendStateBuilder final
	{
		[[nodiscard]] static inline const BlendState& getDefaultBlendState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
			// - Direct3D 11 "D3D11_BLEND_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476087%28v=vs.85%29.aspx
			// - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			// TODO(co) Finish default state comments
			static constexpr BlendState BLEND_STATE =
			{
				false,								// alphaToCoverageEnable (int)															"false"			"false"
				false,								// independentBlendEnable (int)															"false"			"false"

				{ // renderTarget[8]
					// renderTarget[0]
					{
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					},
				// renderTarget[1]
				{
					false,						// blendEnable (int)																	"false"			"false"
					Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
					Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
					Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
					Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
					ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
				},
				// renderTarget[2]
				{
					false,						// blendEnable (int)																	"false"			"false"
					Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
					Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
					Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
					Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
					ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
				},
				// renderTarget[3]
				{
					false,						// blendEnable (int)																	"false"			"false"
					Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
					Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
					Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
					Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
					ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
				},
				// renderTarget[4]
				{
					false,						// blendEnable (int)																	"false"			"false"
					Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
					Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
					Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
					Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
					ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
				},
				// renderTarget[5]
				{
					false,						// blendEnable (int)																	"false"			"false"
					Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
					Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
					Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
					Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
					ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
				},
				// renderTarget[6]
				{
					false,						// blendEnable (int)																	"false"			"false"
					Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
					Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
					Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
					Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
					ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
				},
				// renderTarget[7]
				{
					false,						// blendEnable (int)																	"false"			"false"
					Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
					Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
					Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
					Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
					BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
					ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
				}
			}
			};
			return BLEND_STATE;
		}
	};
} // namespace Rhi
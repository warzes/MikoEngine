#pragma once

namespace Rhi
{
	enum class Blend
	{
		ZERO             = 1,
		ONE              = 2,
		SRC_COLOR        = 3,
		INV_SRC_COLOR    = 4,
		SRC_ALPHA        = 5,
		INV_SRC_ALPHA    = 6,
		DEST_ALPHA       = 7,
		INV_DEST_ALPHA   = 8,
		DEST_COLOR       = 9,
		INV_DEST_COLOR   = 10,
		SRC_ALPHA_SAT    = 11,
		BLEND_FACTOR     = 14,
		INV_BLEND_FACTOR = 15,
		SRC_1_COLOR      = 16,
		INV_SRC_1_COLOR  = 17,
		SRC_1_ALPHA      = 18,
		INV_SRC_1_ALPHA  = 19
	};

	enum class BlendOp
	{
		ADD          = 1,
		SUBTRACT     = 2,
		REV_SUBTRACT = 3,
		MIN          = 4,
		MAX          = 5
	};

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

	struct BlendState final
	{
		int					  alphaToCoverageEnable;	// Boolean value. Default: "false"
		int					  independentBlendEnable;	// Boolean value. Default: "false"
		RenderTargetBlendDesc renderTarget[8];			// Default: See "Rhi::RenderTargetBlendDesc"
	};

	// TODO: BlendStateBuilder::GetDefault() можно перенести в конструктор BlendState. При этом для случаев когда не нужен дефолт, сделать свой конструктор типа BlendState(nodefault); такое решение более правильно - не позволит использовать неинициализованные данные
	struct BlendStateBuilder final
	{
		[[nodiscard]] static inline constexpr const BlendState& GetDefault()
		{
			constexpr const BlendState BLEND_STATE =
			{
				false,								// alphaToCoverageEnable (int)	
				false,								// independentBlendEnable (int)	

				// renderTarget[8]
				{ 
					// renderTarget[0]
					{
						false,						// blendEnable (int)
						Blend::ONE,					// srcBlend (Rhi::Blend)
						Blend::ZERO,				// destBlend (Rhi::Blend)
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags
					},
					// renderTarget[1]
					{
						false,						// blendEnable (int)
						Blend::ONE,					// srcBlend (Rhi::Blend)
						Blend::ZERO,				// destBlend (Rhi::Blend)
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags
					},
					// renderTarget[2]
					{
						false,						// blendEnable (int)
						Blend::ONE,					// srcBlend (Rhi::Blend)
						Blend::ZERO,				// destBlend (Rhi::Blend)
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags
					},
					// renderTarget[3]
					{
						false,						// blendEnable (int)
						Blend::ONE,					// srcBlend (Rhi::Blend)
						Blend::ZERO,				// destBlend (Rhi::Blend)
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags
					},
					// renderTarget[4]
					{
						false,						// blendEnable (int)
						Blend::ONE,					// srcBlend (Rhi::Blend)
						Blend::ZERO,				// destBlend (Rhi::Blend)
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags
					},
					// renderTarget[5]
					{
						false,						// blendEnable (int)
						Blend::ONE,					// srcBlend (Rhi::Blend)
						Blend::ZERO,				// destBlend (Rhi::Blend)
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags
					},
					// renderTarget[6]
					{
						false,						// blendEnable (int)
						Blend::ONE,					// srcBlend (Rhi::Blend)
						Blend::ZERO,				// destBlend (Rhi::Blend)
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags
					},
					// renderTarget[7]
					{
						false,						// blendEnable (int)
						Blend::ONE,					// srcBlend (Rhi::Blend)
						Blend::ZERO,				// destBlend (Rhi::Blend)
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags
					}
				}
			};
			return BLEND_STATE;
		}
	};
} // namespace Rhi
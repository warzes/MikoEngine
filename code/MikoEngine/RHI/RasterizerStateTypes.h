#pragma once

namespace Rhi
{
	/**
	*  @brief
	*    Fill mode
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_FILL_MODE"-documentation for details
	*/
	enum class FillMode
	{
		WIREFRAME = 2,	// Wireframe
		SOLID = 3	// Solid
	};

	/**
	*  @brief
	*    Cull mode
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_CULL_MODE"-documentation for details
	*/
	enum class CullMode
	{
		NONE = 1,	// No culling
		FRONT = 2,	// Do not draw triangles that are front-facing
		BACK = 3	// Do not draw triangles that are back-facing
	};

	/**
	*  @brief
	*    Conservative rasterization mode
	*
	*  @note
	*    - These constants directly map to Direct3D 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_CONSERVATIVE_RASTERIZATION_MODE"-documentation for details
	*/
	enum class ConservativeRasterizationMode
	{
		OFF = 0,	// Conservative rasterization is off
		ON = 1		// Conservative rasterization is on
	};

	/**
	*  @brief
	*    Rasterizer state
	*
	*  @note
	*    - This rasterizer state maps directly to Direct3D 12, do not change it
	*    - This also means that "int" is used over "bool" because in Direct3D it's defined this way
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::RasterizerStateBuilder::getDefaultRasterizerState()"-implementation
	*    - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
	*      Use the convenience "Rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
	*
	*  @see
	*    - "D3D12_RASTERIZER_DESC"-documentation for details
	*/
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
	struct RasterizerStateBuilder final
	{
		/**
		*  @brief
		*    Return the default rasterizer state
		*
		*  @return
		*    The default rasterizer state, see "Rhi::RasterizerState" for the default values
		*/
		[[nodiscard]] static inline const RasterizerState& getDefaultRasterizerState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
			// - Direct3D 12 "D3D12_RASTERIZER_DESC structure"-documentation at MSDN: https://msdn.microsoft.com/de-de/library/windows/desktop/dn770387%28v=vs.85%29.aspx
			// - Direct3D 11 "D3D11_RASTERIZER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476198%28v=vs.85%29.aspx
			// - Direct3D 10 "D3D10_RASTERIZER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172408(v=vs.85).aspx
			// - Direct3D 9 "D3DRENDERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599%28v=vs.85%29.aspx
			// - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			static constexpr RasterizerState RASTERIZER_STATE =
			{																												//	Direct3D 11	Direct3D 10	Direct3D 9		OpenGL
				FillMode::SOLID,					// fillMode (Rhi::FillMode)												"SOLID"			"SOLID"		"SOLID"			"SOLID"
				CullMode::BACK,						// cullMode (Rhi::CullMode)												"BACK"			"Back"		"BACK" (CCW)	"BACK"
				false,								// frontCounterClockwise (int)											"false"			"false"		"false" (CCW)	"true"
				0,									// depthBias (int)														"0"				"0"			"0"				TODO(co)
				0.0f,								// depthBiasClamp (float)												"0.0f"			"0.0f"		<unsupported>	TODO(co)
				0.0f,								// slopeScaledDepthBias (float)											"0.0f"			"0.0f"		"0.0f"			TODO(co)
				true,								// depthClipEnable (int)												"true"			"true"		<unsupported>	TODO(co)
				false,								// multisampleEnable (int)												"false"			"false"		"true"			"true"
				false,								// antialiasedLineEnable (int)											"false"			"false"		"false"			"false"
				0,									// forcedSampleCount (unsigned int)
				ConservativeRasterizationMode::OFF,	// conservativeRasterizationMode (Rhi::ConservativeRasterizationMode)
				false								// scissorEnable (int)													"false"			"false"		"false"			"false"
			};
			return RASTERIZER_STATE;
		}
	};
} // namespace Rhi
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	enum class GraphicsShaderType : uint8_t
	{
		Vertex,					// VS
		TessellationControl,	// TCS, "hull shader" in Direct3D terminology
		TessellationEvaluation,	// TES, "domain shader" in Direct3D terminology
		Geometry,				// GS
		Fragment				// FS, "pixel shader" in Direct3D terminology
	};
	static constexpr uint8_t NUMBER_OF_GRAPHICS_SHADER_TYPES = static_cast<uint8_t>(GraphicsShaderType::Fragment) + 1;	// Total number of graphics shader types


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	// Shader blueprint file format content:
	// - File format header
	// - Shader blueprint header
	// - Asset IDs of the shader pieces to include
	// - Shader blueprint ASCII source code
	namespace v1ShaderBlueprint
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = STRING_ID("ShaderBlueprint");
		static constexpr uint32_t FORMAT_VERSION = 2;

		#pragma pack(push)
		#pragma pack(1)
			struct ShaderBlueprintHeader final
			{
				uint16_t numberOfIncludeShaderPieceAssetIds;
				uint16_t numberOfReferencedShaderProperties;
				uint32_t numberOfShaderSourceCodeBytes;
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1Shader
} // Renderer

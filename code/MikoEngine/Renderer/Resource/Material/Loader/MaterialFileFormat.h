#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Utility/StringId.h"
#include "Core/GetInvalid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialTechniqueId;	// Material technique identifier, result of hashing the material technique name via "Renderer::StringId"
	typedef StringId AssetId;				// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"


	// Material file format content:
	// - File format header
	// - Material header
	// - Material techniques
	// - Material properties
	namespace v1Material
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = STRING_ID("Material");
		static constexpr uint32_t FORMAT_VERSION = 3;

		#pragma pack(push)
		#pragma pack(1)
			struct MaterialHeader final
			{
				uint32_t numberOfTechniques = 0;
				uint32_t numberOfProperties = 0;
			};

			struct Technique final
			{
				MaterialTechniqueId materialTechniqueId = GetInvalid<MaterialTechniqueId>();
				AssetId				materialBlueprintAssetId;
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1Material
} // Renderer

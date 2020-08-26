#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Utility/StringId.h"
#include "Math/Transform.h"
#include "Renderer/Resource/Scene/Item/Light/LightSceneItem.h"
#include "Core/GetInvalid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;				// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef StringId SceneItemTypeId;		// Scene item type identifier, internally just a POD "uint32_t"
	typedef uint32_t MaterialTechniqueId;	// Material technique identifier, result of hashing the material technique name via "Renderer::StringId"


	// Scene file format content:
	// - File format header
	// - Scene header
	namespace v1Scene
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = SE_STRING_ID("Scene");
		static constexpr uint32_t FORMAT_VERSION = 5;

		#pragma pack(push)
		#pragma pack(1)
			struct SceneHeader final
			{
				uint32_t unused;	// TODO(co) Currently the scene header is unused
			};

			struct Nodes final
			{
				uint32_t numberOfNodes;
			};

			struct Node final
			{
				Transform transform;
				uint32_t  numberOfItems;
			};

			struct ItemHeader final
			{
				SceneItemTypeId typeId;
				uint32_t		numberOfBytes;
			};

			struct CameraItem final
			{
			};

			struct LightItem final
			{
				LightSceneItem::LightType lightType			   = LightSceneItem::LightType::POINT;
				float					  color[3]			   = { 1.0f, 1.0f, 1.0f };
				float					  radius			   = 1.0f;
				float					  innerAngle		   = glm::radians(40.0f);
				float					  outerAngle		   = glm::radians(50.0f);
				float					  nearClipDistance	   = 0.0f;
				int32_t					  iesLightProfileIndex = -1;	// Illuminating Engineering Society (IES) light profile index (<0 = no IES)
			};

			struct SunlightItem final
			{
				// Usually fixed
				float sunriseTime	   = 07.50f;	// O'clock
				float sunsetTime	   = 20.50f;	// O'clock
				float eastDirection	   = 0.0f;
				float angleOfIncidence = glm::radians(20.0f);
				float timeOfDay		   = 10.00f;	// O'clock
			};

			struct MeshItem final
			{
				AssetId  meshAssetId;
				uint32_t numberOfSubMeshMaterialAssetIds = 0;
			};

			struct SkeletonMeshItem final	// : public MeshItem -> Not derived by intent to be able to reuse the mesh item serialization 1:1
			{
				AssetId skeletonAnimationAssetId;
			};

			struct MaterialItem
			{
				AssetId				materialAssetId;												// If material blueprint asset ID is set, material asset ID must be invalid
				MaterialTechniqueId	materialTechniqueId		   = GetInvalid<MaterialTechniqueId>();	// Must always be valid
				AssetId				materialBlueprintAssetId;										// If material asset ID is set, material blueprint asset ID must be invalid
				uint32_t			numberOfMaterialProperties = 0;
			};

			struct SkyItem final : public MaterialItem
			{
				// Nothing here
			};

			struct VolumeItem final : public MaterialItem
			{
				// Nothing here
			};

			struct GrassItem final : public MaterialItem
			{
				// Nothing here
			};

			struct TerrainItem final : public MaterialItem
			{
				// Nothing here
			};

			struct ParticlesItem final : public MaterialItem
			{
				// Nothing here
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1Scene
} // Renderer

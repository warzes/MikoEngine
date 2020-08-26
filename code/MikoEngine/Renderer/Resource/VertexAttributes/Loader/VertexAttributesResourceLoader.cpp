#include "stdafx.h"
#include "Renderer/Resource/VertexAttributes/Loader/VertexAttributesResourceLoader.h"
#include "Renderer/Resource/VertexAttributes/Loader/VertexAttributesFileFormat.h"
#include "Renderer/Resource/VertexAttributes/VertexAttributesResource.h"
#include "Renderer/Resource/Scene/Item/Terrain/TerrainSceneItem.h"
#include "Renderer/Resource/Mesh/MeshResource.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/detail/setup.hpp>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void VertexAttributesResourceLoader::initialize(const Asset& asset, bool reload, IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mVertexAttributesResource = static_cast<VertexAttributesResource*>(&resource);
	}

	bool VertexAttributesResourceLoader::onDeserialization(IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1VertexAttributes::FORMAT_TYPE, v1VertexAttributes::FORMAT_VERSION, file);
	}

	void VertexAttributesResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the vertex attributes header
		v1VertexAttributes::VertexAttributesHeader vertexAttributesHeader;
		mMemoryFile.read(&vertexAttributesHeader, sizeof(v1VertexAttributes::VertexAttributesHeader));

		// TODO(co) Implement vertex attributes file format. This here is just a fixed build in dummy.
		if (SE_ASSET_ID("Example/Blueprint/Compositor/VA_Compositor") == getAsset().assetId)
		{
			mVertexAttributesResource->mVertexAttributes = Rhi::VertexAttributes(0, nullptr);
		}
		else if (SE_ASSET_ID("Example/Blueprint/Sky/VA_Sky") == getAsset().assetId)
		{
			Rhi::VertexAttributes& vertexAttributes = const_cast<Rhi::VertexAttributes&>(mVertexAttributesResource->mVertexAttributes);
			static constexpr Rhi::VertexAttribute vertexAttributesLayout[] =
			{
				{ // Attribute 0
					// Data destination
					Rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
					"Position",								// name[32] (char)
					"POSITION",								// semanticName[32] (char)
					0,										// semanticIndex (uint32_t)
					// Data source
					0,										// inputSlot (uint32_t)
					0,										// alignedByteOffset (uint32_t)
					sizeof(float) * 3,						// strideInBytes (uint32_t)
					0										// instancesPerElement (uint32_t)
				}
			};
			vertexAttributes.numberOfAttributes = static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayout));
			vertexAttributes.attributes = vertexAttributesLayout;
		}
		else if (SE_ASSET_ID("Example/Blueprint/Volume/VA_Volume") == getAsset().assetId)
		{
			Rhi::VertexAttributes& vertexAttributes = const_cast<Rhi::VertexAttributes&>(mVertexAttributesResource->mVertexAttributes);
			static constexpr Rhi::VertexAttribute vertexAttributesLayout[] =
			{
				{ // Attribute 0
					// Data destination
					Rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
					"Position",								// name[32] (char)
					"POSITION",								// semanticName[32] (char)
					0,										// semanticIndex (uint32_t)
					// Data source
					0,										// inputSlot (uint32_t)
					0,										// alignedByteOffset (uint32_t)
					sizeof(float) * 3,						// strideInBytes (uint32_t)
					0										// instancesPerElement (uint32_t)
				},
				{ // Attribute 1, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html
					// Data destination
					Rhi::VertexAttributeFormat::UINT_1,		// vertexAttributeFormat (Rhi::VertexAttributeFormat)
					"drawId",								// name[32] (char)
					"DRAWID",								// semanticName[32] (char)
					0,										// semanticIndex (uint32_t)
					// Data source
					1,										// inputSlot (uint32_t)
					0,										// alignedByteOffset (uint32_t)
					sizeof(uint32_t),						// strideInBytes (uint32_t)
					1										// instancesPerElement (uint32_t)
				}
			};
			vertexAttributes.numberOfAttributes = static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayout));
			vertexAttributes.attributes = vertexAttributesLayout;
		}
		else if (SE_ASSET_ID("Example/Blueprint/DebugGui/VA_DebugGui") == getAsset().assetId)
		{
			Rhi::VertexAttributes& vertexAttributes = const_cast<Rhi::VertexAttributes&>(mVertexAttributesResource->mVertexAttributes);
			static constexpr Rhi::VertexAttribute vertexAttributesLayout[] =
			{
				{ // Attribute 0
					// Data destination
					Rhi::VertexAttributeFormat::FLOAT_2,		// vertexAttributeFormat (Rhi::VertexAttributeFormat)
					"Position",									// name[32] (char)
					"POSITION",									// semanticName[32] (char)
					0,											// semanticIndex (uint32_t)
					// Data source
					0,											// inputSlot (uint32_t)
					0,											// alignedByteOffset (uint32_t)
					sizeof(float) * 4 + sizeof(uint8_t) * 4,	// strideInBytes (uint32_t)
					0											// instancesPerElement (uint32_t)
				},
				{ // Attribute 1
					// Data destination
					Rhi::VertexAttributeFormat::FLOAT_2,		// vertexAttributeFormat (Rhi::VertexAttributeFormat)
					"TexCoord",									// name[32] (char)
					"TEXCOORD",									// semanticName[32] (char)
					0,											// semanticIndex (uint32_t)
					// Data source
					0,											// inputSlot (uint32_t)
					sizeof(float) * 2,							// alignedByteOffset (uint32_t)
					sizeof(float) * 4 + sizeof(uint8_t) * 4,	// strideInBytes (uint32_t)
					0											// instancesPerElement (uint32_t)
				},
				{ // Attribute 2
					// Data destination
					Rhi::VertexAttributeFormat::R8G8B8A8_UNORM,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
					"Color",									// name[32] (char)
					"COLOR",									// semanticName[32] (char)
					0,											// semanticIndex (uint32_t)
					// Data source
					0,											// inputSlot (uint32_t)
					sizeof(float) * 4,							// alignedByteOffset (uint32_t)
					sizeof(float) * 4 + sizeof(uint8_t) * 4,	// strideInBytes (uint32_t)
					0											// instancesPerElement (uint32_t)
				}
			};
			vertexAttributes.numberOfAttributes = static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayout));
			vertexAttributes.attributes = vertexAttributesLayout;
		}
		else if (SE_ASSET_ID("Example/Blueprint/Mesh/VA_Mesh") == getAsset().assetId)
		{
			mVertexAttributesResource->mVertexAttributes = Rhi::VertexAttributes(MeshResource::VERTEX_ATTRIBUTES.numberOfAttributes, MeshResource::VERTEX_ATTRIBUTES.attributes);
		}
		else if (SE_ASSET_ID("Example/Blueprint/Mesh/VA_SkinnedMesh") == getAsset().assetId)
		{
			mVertexAttributesResource->mVertexAttributes = Rhi::VertexAttributes(MeshResource::SKINNED_VERTEX_ATTRIBUTES.numberOfAttributes, MeshResource::SKINNED_VERTEX_ATTRIBUTES.attributes);
		}
		else if (SE_ASSET_ID("Example/Blueprint/Particles/VA_Particles") == getAsset().assetId ||
				 SE_ASSET_ID("Example/Blueprint/Grass/VA_Grass") == getAsset().assetId)
		{
			mVertexAttributesResource->mVertexAttributes = Rhi::VertexAttributes(MeshResourceManager::DRAW_ID_VERTEX_ATTRIBUTES.numberOfAttributes, MeshResourceManager::DRAW_ID_VERTEX_ATTRIBUTES.attributes);
		}
		else if (SE_ASSET_ID("Example/Blueprint/Terrain/VA_Terrain") == getAsset().assetId)
		{
			mVertexAttributesResource->mVertexAttributes = Rhi::VertexAttributes(TerrainSceneItem::VERTEX_ATTRIBUTES.numberOfAttributes, TerrainSceneItem::VERTEX_ATTRIBUTES.attributes);
		}
		else
		{
			SE_ASSERT(false, "Unknown vertex attributes asset")
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

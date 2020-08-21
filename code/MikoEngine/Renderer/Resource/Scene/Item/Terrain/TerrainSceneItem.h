#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Scene/Item/MaterialSceneItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Terrain scene item
	*
	*  @remarks
	*    This software contains source code provided by NVIDIA Corporation. The height map terrain tessellation implementation is basing on "DirectX 11 Terrain Tessellation" by Iain Cantlay
	*    ( https://developer.nvidia.com/sites/default/files/akamai/gamedev/files/sdk/11/TerrainTessellation_WhitePaper.pdf ) and the concrete implementation "TerrainTessellation"-sample inside
	*    "NVIDIA Direct3D SDK 11" ( https://developer.nvidia.com/dx11-samples ).
	*
	*    A terrain tile ring is symmetrical in each direction. Don't read much into the exact numbers of #s in this following diagram:
	*    <-   outerWidth  ->
	*    ###################
	*    ###################
	*    ###             ###
	*    ###<-holeWidth->###
	*    ###             ###
	*    ###    (0,0)    ###
	*    ###             ###
	*    ###             ###
	*    ###             ###
	*    ###################
	*    ###################
	*/
	class TerrainSceneItem final : public MaterialSceneItem
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneFactory;	// Needs to be able to create scene item instances


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("TerrainSceneItem");
		static const Rhi::VertexAttributes VERTEX_ATTRIBUTES;


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual SceneItemTypeId getSceneItemTypeId() const override
		{
			return TYPE_ID;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void initialize() override;
		virtual void onMaterialResourceCreated() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static constexpr int MAXIMUM_NUMBER_OF_TERRAIN_TILE_RINGS = 6;
		struct TerrainTileRing final
		{
			int					 numberOfTiles;
			Rhi::IVertexArrayPtr vertexArrayPtr;	// Vertex array object (VAO), considered to be always valid
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit TerrainSceneItem(SceneResource& sceneResource);

		inline virtual ~TerrainSceneItem() override
		{
			// Nothing here
		}

		explicit TerrainSceneItem(const TerrainSceneItem&) = delete;
		TerrainSceneItem& operator=(const TerrainSceneItem&) = delete;
		void createIndexBuffer(Rhi::IBufferManager& bufferManager);
		void createTerrainTileRing(TerrainTileRing& terrainTileRing, Rhi::IBufferManager& bufferManager, int holeWidth, int outerWidth, float tileSize) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Rhi::IIndexBufferPtr mIndexBufferPtr;			// Index buffer which is shared between all terrain tile ring vertex array buffers
		int					 mNumberOfTerrainTileRings;	// Number of terrain tile rings
		TerrainTileRing		 mTerrainTileRings[MAXIMUM_NUMBER_OF_TERRAIN_TILE_RINGS];


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

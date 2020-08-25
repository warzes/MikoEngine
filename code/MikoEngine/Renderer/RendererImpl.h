#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]

#include "Renderer/IRenderer.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::vector<AssetId> AssetIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Renderer class implementation
	*/
	class RendererImpl final : public IRenderer
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the asset IDs of automatically generated dynamic default texture assets
		*
		*  @param[out] assetIds
		*    Receives the asset IDs of automatically generated dynamic default texture assets, the list is not cleared before new entries are added
		*/
		static void getDefaultTextureAssetIds(AssetIds& assetIds);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] context
		*    Renderer context, the renderer context instance must stay valid as long as the renderer instance exists
		*
		*  @note
		*    - The renderer keeps a reference to the provided renderer instance
		*/
		explicit RendererImpl(Context& context);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RendererImpl() override;


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IRenderer methods            ]
	//[-------------------------------------------------------]
	public:
		virtual void reloadResourceByAssetId(AssetId assetId) override;
		virtual void flushAllQueues() override;
		virtual void update() override;

		//[-------------------------------------------------------]
		//[ Pipeline state object cache                           ]
		//[-------------------------------------------------------]
		virtual void clearPipelineStateObjectCache() override;
		virtual void loadPipelineStateObjectCache() override;
		virtual void savePipelineStateObjectCache() override;


	//[-------------------------------------------------------]
	//[ Protected virtual RefCount methods               ]
	//[-------------------------------------------------------]
	protected:
		virtual void selfDestruct() noexcept override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit RendererImpl(const RendererImpl& source) = delete;
		RendererImpl& operator =(const RendererImpl& source) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<AssetId> AssetIdsOfResourcesToReload;	// We're using a vector in here to maintain the provided asset ID order


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Resource hot-reloading
		std::mutex					mAssetIdsOfResourcesToReloadMutex;
		AssetIdsOfResourcesToReload	mAssetIdsOfResourcesToReload;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Scene/Item/ISceneItem.h"
#include "Renderer/Resource/IResourceListener.h"
#include "Renderer/RenderQueue/RenderableManager.h"
#include "Core/GetInvalid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;			// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef uint32_t MeshResourceId;	// POD mesh resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class MeshSceneItem : public ISceneItem, public IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneFactory;	// Needs to be able to create scene item instances


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = SE_STRING_ID("MeshSceneItem");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline MeshResourceId getMeshResourceId() const
		{
			return mMeshResourceId;
		}

		void setMeshResourceId(MeshResourceId meshResourceId);
		void setMeshResourceIdByAssetId(AssetId meshAssetId);

		[[nodiscard]] inline uint32_t getNumberOfSubMeshes() const
		{
			// The renderables contain all LODs, each LOD has the same number of renderables
			return static_cast<uint32_t>(mRenderableManager.getRenderables().size() / mRenderableManager.getNumberOfLods());
		}

		[[nodiscard]] inline uint8_t getNumberOfLods() const
		{
			return mRenderableManager.getNumberOfLods();
		}

		[[nodiscard]] inline MaterialResourceId getMaterialResourceIdOfSubMeshLod(uint32_t subMeshIndex, uint8_t lodIndex) const
		{
			// The renderables contain all LODs, each LOD has the same number of renderables
			SE_ASSERT(subMeshIndex < getNumberOfSubMeshes(), "Invalid sub mesh index")
			SE_ASSERT(lodIndex < mRenderableManager.getNumberOfLods(), "Invalid LOD index")
			return mRenderableManager.getRenderables()[subMeshIndex + lodIndex * getNumberOfSubMeshes()].getMaterialResourceId();
		}

		void setMaterialResourceIdOfSubMeshLod(uint32_t subMeshIndex, uint8_t lodIndex, MaterialResourceId materialResourceId);
		void setMaterialResourceIdOfAllSubMeshesAndLods(MaterialResourceId materialResourceId);


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual SceneItemTypeId getSceneItemTypeId() const override
		{
			return TYPE_ID;
		}

		virtual void deserialize(uint32_t numberOfBytes, const uint8_t* data) override;
		virtual void onAttachedToSceneNode(SceneNode& sceneNode) override;

		inline virtual void onDetachedFromSceneNode(SceneNode& sceneNode) override
		{
			mRenderableManager.setTransform(nullptr);

			// Call the base implementation
			ISceneItem::onDetachedFromSceneNode(sceneNode);
		}

		inline virtual void setVisible(bool visible) override
		{
			mRenderableManager.setVisible(visible);
		}

		[[nodiscard]] inline virtual const RenderableManager* getRenderableManager() const override
		{
			return &mRenderableManager;
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline explicit MeshSceneItem(SceneResource& sceneResource) :
			ISceneItem(sceneResource),
			mMeshResourceId(GetInvalid<MeshResourceId>())
		{
			// Nothing here
		}

		inline virtual ~MeshSceneItem() override
		{
			// Nothing here
		}

		explicit MeshSceneItem(const MeshSceneItem&) = delete;
		MeshSceneItem& operator=(const MeshSceneItem&) = delete;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<AssetId> SubMeshMaterialAssetIds;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MeshResourceId			mMeshResourceId;			// Mesh resource ID, can be set to invalid value
		SubMeshMaterialAssetIds	mSubMeshMaterialAssetIds;	// Sub-mesh material asset IDs received during deserialization
		RenderableManager		mRenderableManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

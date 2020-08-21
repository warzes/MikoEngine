#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/RenderQueue/RenderableManager.h"
#include "Renderer/Resource/IResourceListener.h"
#include "Renderer/Resource/Scene/Item/ISceneItem.h"
#include "Renderer/Resource/Material/MaterialProperties.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;				// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef uint32_t MaterialTechniqueId;	// Material technique identifier, result of hashing the material technique name via "Renderer::StringId"
	typedef uint32_t MaterialResourceId;	// POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract material item
	*/
	class MaterialSceneItem : public ISceneItem, public IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline AssetId getMaterialAssetId() const
		{
			return mMaterialAssetId;
		}

		[[nodiscard]] inline MaterialTechniqueId getMaterialTechniqueId() const
		{
			return mMaterialTechniqueId;
		}

		[[nodiscard]] inline AssetId getMaterialBlueprintAssetId() const
		{
			return mMaterialBlueprintAssetId;
		}

		[[nodiscard]] inline const MaterialProperties& getMaterialProperties() const
		{
			return mMaterialProperties;
		}

		[[nodiscard]] inline MaterialResourceId getMaterialResourceId() const
		{
			return mMaterialResourceId;
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	public:
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

		[[nodiscard]] virtual const RenderableManager* getRenderableManager() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void initialize();
		virtual void onMaterialResourceCreated() = 0;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline MaterialSceneItem(SceneResource& sceneResource, bool cullable = true) :
			ISceneItem(sceneResource, cullable),
			mMaterialTechniqueId(getInvalid<MaterialTechniqueId>()),
			mMaterialResourceId(getInvalid<MaterialResourceId>())
		{
			// Nothing here
		}

		virtual ~MaterialSceneItem() override;
		explicit MaterialSceneItem(const MaterialSceneItem&) = delete;
		MaterialSceneItem& operator=(const MaterialSceneItem&) = delete;
		void createMaterialResource(MaterialResourceId parentMaterialResourceId);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		RenderableManager mRenderableManager;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetId				mMaterialAssetId;			// If material blueprint asset ID is set, material asset ID must be invalid
		MaterialTechniqueId	mMaterialTechniqueId;		// Must always be valid
		AssetId				mMaterialBlueprintAssetId;	// If material asset ID is set, material blueprint asset ID must be invalid
		MaterialProperties	mMaterialProperties;
		MaterialResourceId	mMaterialResourceId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

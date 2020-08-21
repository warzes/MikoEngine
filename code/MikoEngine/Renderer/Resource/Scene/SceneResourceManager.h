
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/ResourceManager.h"
#include "Renderer/Context.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class ISceneFactory;
	class SceneResource;
	class IRenderer;
	class SceneResourceLoader;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t SceneResourceId;	// POD scene resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class SceneResourceManager final : public ResourceManager<SceneResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		[[nodiscard]] inline const ISceneFactory& getSceneFactory() const
		{
			// We know that this pointer is always valid
			RHI_ASSERT(nullptr != mSceneFactory, "Invalid scene factory")
			return *mSceneFactory;
		}

		void setSceneFactory(const ISceneFactory* sceneFactory);
		[[nodiscard]] SceneResource* getSceneResourceByAssetId(AssetId assetId) const;		// Considered to be inefficient, avoid method whenever possible
		[[nodiscard]] SceneResourceId getSceneResourceIdByAssetId(AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void loadSceneResourceByAssetId(AssetId assetId, SceneResourceId& sceneResourceId, IResourceListener* resourceListener = nullptr, bool reload = false, ResourceLoaderTypeId resourceLoaderTypeId = getInvalid<ResourceLoaderTypeId>());	// Asynchronous
		void destroySceneResource(SceneResourceId sceneResourceId);
		void setInvalidResourceId(SceneResourceId& sceneResourceId, IResourceListener& resourceListener) const;


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual uint32_t getNumberOfResources() const override;
		[[nodiscard]] virtual IResource& getResourceByIndex(uint32_t index) const override;
		[[nodiscard]] virtual IResource& getResourceByResourceId(ResourceId resourceId) const override;
		[[nodiscard]] virtual IResource* tryGetResourceByResourceId(ResourceId resourceId) const override;
		virtual void reloadResourceByAssetId(AssetId assetId) override;

		inline virtual void update() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] virtual IResourceLoader* createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit SceneResourceManager(IRenderer& renderer);
		virtual ~SceneResourceManager() override;
		explicit SceneResourceManager(const SceneResourceManager&) = delete;
		SceneResourceManager& operator=(const SceneResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&			 mRenderer;		// Renderer instance, do not destroy the instance
		const ISceneFactory* mSceneFactory;	// Scene factory, always valid, do not destroy the instance

		// Internal resource manager implementation
		ResourceManagerTemplate<SceneResource, SceneResourceLoader, SceneResourceId, 16>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

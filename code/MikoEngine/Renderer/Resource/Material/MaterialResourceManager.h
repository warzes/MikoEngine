#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/ResourceManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IRenderer;
	class MaterialResource;
	class MaterialResourceLoader;
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
	typedef uint32_t MaterialResourceId;	// POD material resource identifier
	typedef uint32_t MaterialTechniqueId;	// Material technique identifier, result of hashing the material technique name via "Renderer::StringId"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class MaterialResourceManager final : public ResourceManager<MaterialResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t DEFAULT_MATERIAL_TECHNIQUE_ID = STRING_ID("Default");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		[[nodiscard]] MaterialResource* getMaterialResourceByAssetId(AssetId assetId) const;		// Considered to be inefficient, avoid method whenever possible
		[[nodiscard]] MaterialResourceId getMaterialResourceIdByAssetId(AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void loadMaterialResourceByAssetId(AssetId assetId, MaterialResourceId& materialResourceId, IResourceListener* resourceListener = nullptr, bool reload = false, ResourceLoaderTypeId resourceLoaderTypeId = GetInvalid<ResourceLoaderTypeId>());	// Asynchronous
		[[nodiscard]] MaterialResourceId createMaterialResourceByAssetId(AssetId assetId, AssetId materialBlueprintAssetId, MaterialTechniqueId materialTechniqueId);	// Material resource is not allowed to exist, yet
		[[nodiscard]] MaterialResourceId createMaterialResourceByCloning(MaterialResourceId parentMaterialResourceId, AssetId assetId = GetInvalid<AssetId>());	// Parent material resource must be fully loaded
		void destroyMaterialResource(MaterialResourceId materialResourceId);
		void setInvalidResourceId(MaterialResourceId& materialResourceId, IResourceListener& resourceListener) const;


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
		explicit MaterialResourceManager(IRenderer& renderer);
		virtual ~MaterialResourceManager() override;
		explicit MaterialResourceManager(const MaterialResourceManager&) = delete;
		MaterialResourceManager& operator=(const MaterialResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer& mRenderer;	// Renderer instance, do not destroy the instance

		// Internal resource manager implementation
		ResourceManagerTemplate<MaterialResource, MaterialResourceLoader, MaterialResourceId, 4096>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

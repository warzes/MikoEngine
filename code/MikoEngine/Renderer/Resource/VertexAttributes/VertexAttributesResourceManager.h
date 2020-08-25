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
	class VertexAttributesResource;
	class VertexAttributesResourceLoader;
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
	typedef uint32_t VertexAttributesResourceId;	// POD vertex attributes resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vertex attributes resource manager
	*/
	class VertexAttributesResourceManager final : public ResourceManager<VertexAttributesResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] VertexAttributesResource* getVertexAttributesResourceByAssetId(AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void loadVertexAttributesResourceByAssetId(AssetId assetId, VertexAttributesResourceId& vertexAttributesResourceId, IResourceListener* resourceListener = nullptr, bool reload = false, ResourceLoaderTypeId resourceLoaderTypeId = GetInvalid<ResourceLoaderTypeId>());	// Asynchronous
		[[nodiscard]] VertexAttributesResourceId createVertexAttributesResourceByAssetId(AssetId assetId);	// Skeleton animation resource is not allowed to exist, yet
		void setInvalidResourceId(VertexAttributesResourceId& vertexAttributesResourceId, IResourceListener& resourceListener) const;


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual uint32_t getNumberOfResources() const override;
		[[nodiscard]] virtual IResource& getResourceByIndex(uint32_t index) const override;
		[[nodiscard]] virtual IResource& getResourceByResourceId(ResourceId resourceId) const override;
		[[nodiscard]] virtual IResource* tryGetResourceByResourceId(ResourceId resourceId) const override;
		virtual void reloadResourceByAssetId(AssetId assetId) override;
		virtual void update() override;


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] virtual IResourceLoader* createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit VertexAttributesResourceManager(IRenderer& renderer);
		virtual ~VertexAttributesResourceManager() override;
		explicit VertexAttributesResourceManager(const VertexAttributesResourceManager&) = delete;
		VertexAttributesResourceManager& operator=(const VertexAttributesResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ResourceManagerTemplate<VertexAttributesResource, VertexAttributesResourceLoader, VertexAttributesResourceId, 32>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

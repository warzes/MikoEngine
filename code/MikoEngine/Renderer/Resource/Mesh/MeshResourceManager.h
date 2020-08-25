#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/ResourceManager.h"
#include "RHI/Rhi.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class MeshResource;
	class IRenderer;
	class IMeshResourceLoader;
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
	typedef uint32_t MeshResourceId;	// POD mesh resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class MeshResourceManager final : public ResourceManager<MeshResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const Rhi::VertexAttributes DRAW_ID_VERTEX_ATTRIBUTES;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline uint8_t getNumberOfTopMeshLodsToRemove() const
		{
			return mNumberOfTopMeshLodsToRemove;
		}

		inline void setNumberOfTopMeshLodsToRemove(uint8_t numberOfTopMeshLodsToRemove)
		{
			mNumberOfTopMeshLodsToRemove = numberOfTopMeshLodsToRemove;
		}

		[[nodiscard]] MeshResource* getMeshResourceByAssetId(AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void loadMeshResourceByAssetId(AssetId assetId, MeshResourceId& meshResourceId, IResourceListener* resourceListener = nullptr, bool reload = false, ResourceLoaderTypeId resourceLoaderTypeId = GetInvalid<ResourceLoaderTypeId>());	// Asynchronous
		[[nodiscard]] MeshResourceId createEmptyMeshResourceByAssetId(AssetId assetId);	// Mesh resource is not allowed to exist, yet, prefer asynchronous mesh resource loading over this method
		void setInvalidResourceId(MeshResourceId& textureResourceId, IResourceListener& resourceListener) const;

		[[nodiscard]] inline const Rhi::IVertexBufferPtr& getDrawIdVertexBufferPtr() const
		{
			return mDrawIdVertexBufferPtr;
		}

		[[nodiscard]] inline const Rhi::IVertexArrayPtr& getDrawIdVertexArrayPtr() const
		{
			return mDrawIdVertexArrayPtr;
		}


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
		explicit MeshResourceManager(IRenderer& renderer);
		virtual ~MeshResourceManager() override;
		explicit MeshResourceManager(const MeshResourceManager&) = delete;
		MeshResourceManager& operator=(const MeshResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint8_t				  mNumberOfTopMeshLodsToRemove;	// The number of top mesh LODs to remove, only has an impact while rendering and not on loading (amount of needed memory is not influenced)
		ResourceManagerTemplate<MeshResource, IMeshResourceLoader, MeshResourceId, 4096>* mInternalResourceManager;
		Rhi::IVertexBufferPtr mDrawIdVertexBufferPtr;		// Draw ID vertex buffer, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html
		Rhi::IVertexArrayPtr  mDrawIdVertexArrayPtr;		// Draw ID vertex array, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

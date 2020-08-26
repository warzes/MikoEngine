#include "stdafx.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"
#include "Renderer/Resource/Mesh/MeshResource.h"
#include "Renderer/Resource/Mesh/Loader/MeshResourceLoader.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"

//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		// Vertex input layout
		static constexpr Rhi::VertexAttribute DrawIdVertexAttributesLayout[] =
		{
			{ // Attribute 0, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html
				// Data destination
				Rhi::VertexAttributeFormat::UINT_1,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
				"drawId",							// name[32] (char)
				"DRAWID",							// semanticName[32] (char)
				0,									// semanticIndex (uint32_t)
				// Data source
				0,									// inputSlot (uint32_t)
				0,									// alignedByteOffset (uint32_t)
				sizeof(uint32_t),					// strideInBytes (uint32_t)
				1									// instancesPerElement (uint32_t)
			}
		};


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const Rhi::VertexAttributes MeshResourceManager::DRAW_ID_VERTEX_ATTRIBUTES(static_cast<uint32_t>(GLM_COUNTOF(::detail::DrawIdVertexAttributesLayout)), ::detail::DrawIdVertexAttributesLayout);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MeshResource* MeshResourceManager::getMeshResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	void MeshResourceManager::loadMeshResourceByAssetId(AssetId assetId, MeshResourceId& meshResourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId)
	{
		// Choose default resource loader type ID, if necessary
		if (IsInvalid(resourceLoaderTypeId))
		{
			resourceLoaderTypeId = MeshResourceLoader::TYPE_ID;
		}

		// Load
		mInternalResourceManager->loadResourceByAssetId(assetId, meshResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	MeshResourceId MeshResourceManager::createEmptyMeshResourceByAssetId(AssetId assetId)
	{
		MeshResource& meshResource = mInternalResourceManager->createEmptyResourceByAssetId(assetId);
		setResourceLoadingState(meshResource, IResource::LoadingState::LOADED);
		return meshResource.getId();
	}

	void MeshResourceManager::setInvalidResourceId(MeshResourceId& meshResourceId, IResourceListener& resourceListener) const
	{
		MeshResource* meshResource = tryGetById(meshResourceId);
		if (nullptr != meshResource)
		{
			meshResource->disconnectResourceListener(resourceListener);
		}
		SetInvalid(meshResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t MeshResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& MeshResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& MeshResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* MeshResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void MeshResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}

	void MeshResourceManager::update()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* MeshResourceManager::createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId)
	{
		switch (resourceLoaderTypeId)
		{
			case MeshResourceLoader::TYPE_ID:
				return new MeshResourceLoader(*this, mInternalResourceManager->getRenderer());

			default:
				// TODO(co) Error handling
				SE_ASSERT(false, "Invalid resource loader type ID")
				return nullptr;
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MeshResourceManager::MeshResourceManager(IRenderer& renderer) :
		mNumberOfTopMeshLodsToRemove(0)
	{
		mInternalResourceManager = new ResourceManagerTemplate<MeshResource, IMeshResourceLoader, MeshResourceId, 4096>(renderer, *this);

		// Create the draw ID vertex buffer, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html
		uint32_t drawIds[4096];
		for (uint32_t i = 0; i < 4096; ++i)
		{
			drawIds[i] = i;
		}
		Rhi::IBufferManager& bufferManager = renderer.getBufferManager();
		mDrawIdVertexBufferPtr = bufferManager.createVertexBuffer(sizeof(drawIds), drawIds, 0, Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME("Draw ID"));

		// Create vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
		const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { mDrawIdVertexBufferPtr };
		mDrawIdVertexArrayPtr = bufferManager.createVertexArray(DRAW_ID_VERTEX_ATTRIBUTES, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, nullptr RHI_RESOURCE_DEBUG_NAME("Draw ID"));
	}

	MeshResourceManager::~MeshResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

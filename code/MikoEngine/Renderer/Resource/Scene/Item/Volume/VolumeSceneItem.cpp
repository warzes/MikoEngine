#include "stdafx.h"
#include "Renderer/Resource/Scene/Item/Volume/VolumeSceneItem.h"
#include "Renderer/Resource/Scene/SceneResource.h"
#include "Renderer/Resource/Scene/SceneNode.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"


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
		static Rhi::IVertexArrayPtr VolumeVertexArrayPtr;	// Vertex array object (VAO), can be a null pointer, shared between all volume instances


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] Rhi::IVertexArray* createVolumeVertexArray(const Renderer::IRenderer& renderer)
		{
			// Vertex input layout
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
			const Rhi::VertexAttributes vertexAttributes(static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayout)), vertexAttributesLayout);

			// Our cube is constructed like this
			/*
					3+------+2  y
					/|     /|   |
				   / |    / |   |
				  / 0+---/--+1  *---x
				7+------+6 /   /
				 | /    | /   z
				 |/     |/
				4+------+5
			*/

			// Create the vertex buffer object (VBO)
			// -> Object space vertex positions
			static constexpr float VERTEX_POSITION[] =
			{
				-0.5f, -0.5f, -0.5f,	// 0
				 0.5f, -0.5f, -0.5f,	// 1
				 0.5f,  0.5f, -0.5f,	// 2
				-0.5f,  0.5f, -0.5f,	// 3
				-0.5f, -0.5f,  0.5f,	// 4
				 0.5f, -0.5f,  0.5f,	// 5
				 0.5f,  0.5f,  0.5f,	// 6
				-0.5f,  0.5f,  0.5f,	// 7
			};
			Rhi::IBufferManager& bufferManager = renderer.getBufferManager();
			Rhi::IVertexBufferPtr vertexBuffer(bufferManager.createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, 0, Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME("Volume")));

			// Create the index buffer object (IBO)
			static constexpr uint16_t INDICES[] =
			{
				// Back		Triangle
				2, 3, 0,	// 0
				0, 1, 2,	// 1
				// Front
				7, 6, 5,	// 0
				5, 4, 7,	// 1
				// Left
				3, 7, 4,	// 0
				4, 0, 3,	// 1
				// Right
				6, 2, 1,	// 0
				1, 5, 6,	// 1
				// Top
				3, 2, 6,	// 0
				6, 7, 3,	// 1
				// Bottom
				0, 4, 5,	// 0
				5, 1, 0		// 1
			};
			Rhi::IIndexBufferPtr indexBuffer(bufferManager.createIndexBuffer(sizeof(INDICES), INDICES, 0, Rhi::BufferUsage::STATIC_DRAW, Rhi::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME("Volume")));

			// Create vertex array object (VAO)
			const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer, renderer.getMeshResourceManager().getDrawIdVertexBufferPtr() };
			return bufferManager.createVertexArray(vertexAttributes, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer RHI_RESOURCE_DEBUG_NAME("Volume"));
		}


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
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	const RenderableManager* VolumeSceneItem::getRenderableManager() const
	{
		// Sanity check
		RHI_ASSERT(mRenderableManager.getTransform().scale.x == mRenderableManager.getTransform().scale.y && mRenderableManager.getTransform().scale.y == mRenderableManager.getTransform().scale.z, "Only uniform scale is supported to keep things simple")

		// Call the base implementation
		return MaterialSceneItem::getRenderableManager();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	void VolumeSceneItem::onMaterialResourceCreated()
	{
		// Setup renderable manager
		#if SE_DEBUG
			const char* debugName = "Volume";
			mRenderableManager.setDebugName(debugName);
		#endif
		mRenderableManager.getRenderables().emplace_back(mRenderableManager, ::detail::VolumeVertexArrayPtr, getSceneResource().getRenderer().getMaterialResourceManager(), getMaterialResourceId(), getInvalid<SkeletonResourceId>(), true, 0, 36, 1 RHI_RESOURCE_DEBUG_NAME(debugName));
		mRenderableManager.updateCachedRenderablesData();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	VolumeSceneItem::VolumeSceneItem(SceneResource& sceneResource) :
		MaterialSceneItem(sceneResource)
	{
		// Add reference to vertex array object (VAO) shared between all volume instances
		if (nullptr == ::detail::VolumeVertexArrayPtr)
		{
			::detail::VolumeVertexArrayPtr = ::detail::createVolumeVertexArray(getSceneResource().getRenderer());
			RHI_ASSERT(nullptr != ::detail::VolumeVertexArrayPtr, "Invalid volume vertex array")
		}
		::detail::VolumeVertexArrayPtr->AddReference();
	}

	VolumeSceneItem::~VolumeSceneItem()
	{
		if (isValid(getMaterialResourceId()))
		{
			// Clear the renderable manager right now so we have no more references to the shared vertex array
			mRenderableManager.getRenderables().clear();
		}

		// Release reference to vertex array object (VAO) shared between all volume instances
		if (nullptr != ::detail::VolumeVertexArrayPtr && 1 == ::detail::VolumeVertexArrayPtr->ReleaseReference())	// +1 for reference to global shared pointer
		{
			::detail::VolumeVertexArrayPtr = nullptr;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

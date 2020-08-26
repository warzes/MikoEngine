#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Mesh/Loader/IMeshResourceLoader.h"
#include "Core/File/MemoryFile.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rhi
{
	class IVertexArray;
	class IBufferManager;
	struct VertexAttribute;
}
namespace Renderer
{
	namespace v1Mesh
	{
		struct SubMesh;
	}
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
	class MeshResourceLoader final : public IMeshResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MeshResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = SE_STRING_ID("mesh");


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] inline virtual bool hasDeserialization() const override
		{
			return true;
		}

		[[nodiscard]] virtual bool onDeserialization(IFile& file) override;

		[[nodiscard]] inline virtual bool hasProcessing() const override
		{
			return true;
		}

		virtual void onProcessing() override;
		[[nodiscard]] virtual bool onDispatch() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		MeshResourceLoader(IResourceManager& resourceManager, IRenderer& renderer);
		virtual ~MeshResourceLoader() override;
		explicit MeshResourceLoader(const MeshResourceLoader&) = delete;
		MeshResourceLoader& operator=(const MeshResourceLoader&) = delete;
		void createVertexArrays();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Rhi::IBufferManager& mBufferManager;	// Buffer manager instance, do not destroy the instance

		// Temporary data
		Rhi::IVertexArray* mVertexArray;			// In case the used RHI implementation supports native multithreading we also create the RHI resource asynchronous, but the final resource pointer reassignment must still happen synchronous
		Rhi::IVertexArray* mPositionOnlyVertexArray;
		MemoryFile		   mMemoryFile;

		// Temporary vertex buffer
		uint32_t mNumberOfVertexBufferDataBytes;
		uint32_t mNumberOfUsedVertexBufferDataBytes;
		uint8_t* mVertexBufferData;

		// Temporary index buffer
		uint8_t  mIndexBufferFormat;	// "Rhi::IndexBufferFormat", don't want to include the header in here
		uint32_t mNumberOfIndexBufferDataBytes;
		uint32_t mNumberOfUsedIndexBufferDataBytes;
		uint8_t* mIndexBufferData;

		// Temporary position-only index buffer
		uint32_t mNumberOfPositionOnlyIndexBufferDataBytes;
		uint32_t mNumberOfUsedPositionOnlyIndexBufferDataBytes;
		uint8_t* mPositionOnlyIndexBufferData;

		// Temporary vertex attributes
		uint32_t			  mNumberOfVertexAttributes;
		uint32_t			  mNumberOfUsedVertexAttributes;
		Rhi::VertexAttribute* mVertexAttributes;

		// Temporary sub-meshes
		uint32_t		 mNumberOfSubMeshes;
		uint32_t		 mNumberOfUsedSubMeshes;
		v1Mesh::SubMesh* mSubMeshes;

		// Optional temporary skeleton
		uint8_t  mNumberOfBones;
		uint8_t* mSkeletonData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

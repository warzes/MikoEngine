#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResource.h"
#include "Renderer/Resource/Mesh/SubMesh.h"
#include "RHI/Rhi.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	template <class ELEMENT_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class PackedElementManager;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
	class IMeshResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::vector<SubMesh> SubMeshes;
	typedef uint32_t			 MeshResourceId;		// POD mesh resource identifier
	typedef uint32_t			 SkeletonResourceId;	// POD skeleton resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Mesh resource class
	*/
	class MeshResource final : public IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend PackedElementManager<MeshResource, MeshResourceId, 4096>;							// Type definition of template class
		friend ResourceManagerTemplate<MeshResource, IMeshResourceLoader, MeshResourceId, 4096>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const Rhi::VertexAttributes VERTEX_ATTRIBUTES;			// Default vertex attributes layout, whenever possible stick to this to be as compatible as possible to the rest
		static const Rhi::VertexAttributes SKINNED_VERTEX_ATTRIBUTES;	// Default skinned vertex attributes layout, whenever possible stick to this to be as compatible as possible to the rest


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Bounding                                              ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline const glm::vec3& getMinimumBoundingBoxPosition() const
		{
			return mMinimumBoundingBoxPosition;
		}

		[[nodiscard]] inline const glm::vec3& getMaximumBoundingBoxPosition() const
		{
			return mMaximumBoundingBoxPosition;
		}

		inline void setBoundingBoxPosition(const glm::vec3& minimumBoundingBoxPosition, const glm::vec3& maximumBoundingBoxPosition)
		{
			mMinimumBoundingBoxPosition = minimumBoundingBoxPosition;
			mMaximumBoundingBoxPosition = maximumBoundingBoxPosition;
		}

		[[nodiscard]] inline const glm::vec3& getBoundingSpherePosition() const
		{
			return mBoundingSpherePosition;
		}

		[[nodiscard]] inline float getBoundingSphereRadius() const
		{
			return mBoundingSphereRadius;
		}

		inline void setBoundingSpherePositionRadius(const glm::vec3& boundingSpherePosition, float boundingSphereRadius)
		{
			mBoundingSpherePosition = boundingSpherePosition;
			mBoundingSphereRadius = boundingSphereRadius;
		}

		//[-------------------------------------------------------]
		//[ Vertex and index data                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline uint32_t getNumberOfVertices() const
		{
			return mNumberOfVertices;
		}

		inline void setNumberOfVertices(uint32_t numberOfVertices)
		{
			mNumberOfVertices = numberOfVertices;
		}

		[[nodiscard]] inline uint32_t getNumberOfIndices() const
		{
			return mNumberOfIndices;
		}

		inline void setNumberOfIndices(uint32_t numberOfIndices)
		{
			mNumberOfIndices = numberOfIndices;
		}

		[[nodiscard]] inline const Rhi::IVertexArrayPtr& getVertexArrayPtr() const
		{
			return mVertexArray;
		}

		[[nodiscard]] inline const Rhi::IVertexArrayPtr& getPositionOnlyVertexArrayPtr() const
		{
			return mPositionOnlyVertexArray;
		}

		inline void setVertexArray(Rhi::IVertexArray* vertexArray, Rhi::IVertexArray* positionOnlyVertexArray = nullptr)
		{
			mVertexArray = vertexArray;
			mPositionOnlyVertexArray = positionOnlyVertexArray;
		}

		//[-------------------------------------------------------]
		//[ Sub-meshes and LODs                                   ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline const SubMeshes& getSubMeshes() const
		{
			return mSubMeshes;
		}

		[[nodiscard]] inline SubMeshes& getSubMeshes()
		{
			return mSubMeshes;
		}

		[[nodiscard]] inline uint8_t getNumberOfLods() const
		{
			return mNumberOfLods;
		}

		inline void setNumberOfLods(uint8_t numberOfLods)
		{
			mNumberOfLods = numberOfLods;
		}

		//[-------------------------------------------------------]
		//[ Optional skeleton                                     ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline SkeletonResourceId getSkeletonResourceId() const
		{
			return mSkeletonResourceId;
		}

		inline void setSkeletonResourceId(SkeletonResourceId skeletonResourceId)
		{
			mSkeletonResourceId = skeletonResourceId;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline MeshResource() :
			// Bounding
			mMinimumBoundingBoxPosition(getInvalid<float>()),
			mMaximumBoundingBoxPosition(getInvalid<float>()),
			mBoundingSpherePosition(getInvalid<float>()),
			mBoundingSphereRadius(getInvalid<float>()),
			// Vertex and index data
			mNumberOfVertices(0),
			mNumberOfIndices(0),
			// Sub-meshes and LODs
			mNumberOfLods(0),
			// Optional skeleton
			mSkeletonResourceId(getInvalid<SkeletonResourceId>())
		{
			// Nothing here
		}

		inline virtual ~MeshResource() override
		{
			// Sanity checks
			RHI_ASSERT(isInvalid(mMinimumBoundingBoxPosition.x) && isInvalid(mMinimumBoundingBoxPosition.y) && isInvalid(mMinimumBoundingBoxPosition.z), "Invalid bounding box")
			RHI_ASSERT(isInvalid(mMaximumBoundingBoxPosition.x) && isInvalid(mMaximumBoundingBoxPosition.y) && isInvalid(mMaximumBoundingBoxPosition.z), "Invalid bounding box")
			RHI_ASSERT(isInvalid(mBoundingSpherePosition.x) && isInvalid(mBoundingSpherePosition.y) && isInvalid(mBoundingSpherePosition.z), "Invalid bounding sphere")
			RHI_ASSERT(isInvalid(mBoundingSphereRadius), "Invalid bounding sphere")
			RHI_ASSERT(0 == mNumberOfVertices, "Invalid number of vertices")
			RHI_ASSERT(0 == mNumberOfIndices, "Invalid number of indices")
			RHI_ASSERT(nullptr == mVertexArray.GetPointer(), "Invalid vertex array")
			RHI_ASSERT(nullptr == mPositionOnlyVertexArray.GetPointer(), "Invalid position only vertex array")
			RHI_ASSERT(mSubMeshes.empty(), "Invalid sub-meshes")
			RHI_ASSERT(isInvalid(mSkeletonResourceId), "Invalid skeleton resource ID")
		}

		explicit MeshResource(const MeshResource&) = delete;
		MeshResource& operator=(const MeshResource&) = delete;

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(MeshResourceId meshResourceId)
		{
			// Sanity checks
			RHI_ASSERT(isInvalid(mMinimumBoundingBoxPosition.x) && isInvalid(mMinimumBoundingBoxPosition.y) && isInvalid(mMinimumBoundingBoxPosition.z), "Invalid bounding box")
			RHI_ASSERT(isInvalid(mMaximumBoundingBoxPosition.x) && isInvalid(mMaximumBoundingBoxPosition.y) && isInvalid(mMaximumBoundingBoxPosition.z), "Invalid bounding box")
			RHI_ASSERT(isInvalid(mBoundingSpherePosition.x) && isInvalid(mBoundingSpherePosition.y) && isInvalid(mBoundingSpherePosition.z), "Invalid bounding sphere")
			RHI_ASSERT(isInvalid(mBoundingSphereRadius), "Invalid bounding sphere")
			RHI_ASSERT(0 == mNumberOfVertices, "Invalid number of vertices")
			RHI_ASSERT(0 == mNumberOfIndices, "Invalid number of indices")
			RHI_ASSERT(nullptr == mVertexArray.GetPointer(), "Invalid vertex array")
			RHI_ASSERT(nullptr == mPositionOnlyVertexArray.GetPointer(), "Invalid position only vertex array")
			RHI_ASSERT(mSubMeshes.empty(), "Invalid sub-meshes")
			RHI_ASSERT(isInvalid(mSkeletonResourceId), "Invalid skeleton resource ID")

			// Call base implementation
			IResource::initializeElement(meshResourceId);
		}

		inline void deinitializeElement()
		{
			// Reset everything
			setInvalid(mMinimumBoundingBoxPosition.x);
			setInvalid(mMinimumBoundingBoxPosition.y);
			setInvalid(mMinimumBoundingBoxPosition.z);
			setInvalid(mMaximumBoundingBoxPosition.x);
			setInvalid(mMaximumBoundingBoxPosition.y);
			setInvalid(mMaximumBoundingBoxPosition.z);
			setInvalid(mBoundingSpherePosition.x);
			setInvalid(mBoundingSpherePosition.y);
			setInvalid(mBoundingSpherePosition.z);
			setInvalid(mBoundingSphereRadius);
			mNumberOfVertices = 0;
			mNumberOfIndices = 0;
			mVertexArray = nullptr;
			mPositionOnlyVertexArray = nullptr;
			mSubMeshes.clear();
			mNumberOfIndices = 0;
			setInvalid(mSkeletonResourceId);

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Bounding, the bounding sphere radius is enclosing the bounding box (don't use the inner bounding box radius)
		glm::vec3 mMinimumBoundingBoxPosition;
		glm::vec3 mMaximumBoundingBoxPosition;
		glm::vec3 mBoundingSpherePosition;
		float	  mBoundingSphereRadius;
		// Vertex and index data
		uint32_t			 mNumberOfVertices;			// Number of vertices
		uint32_t			 mNumberOfIndices;			// Number of indices
		Rhi::IVertexArrayPtr mVertexArray;				// Vertex array object (VAO), can be a null pointer, directly containing also the index data of all LODs
		Rhi::IVertexArrayPtr mPositionOnlyVertexArray;	// Optional position-only vertex array object (VAO) which can reduce the number of processed vertices up to half, can be a null pointer, can be used for position-only rendering (e.g. shadow map rendering) using the same vertex data that the original vertex array object (VAO) uses, directly containing also the index data of all LODs
		// Sub-meshes and LODs
		SubMeshes			 mSubMeshes;			// Sub-meshes, directly containing also the sub-meshes of all LODs, each LOD has the same number of sub-meshes
		uint8_t				 mNumberOfLods;			// Number of LODs, there's always at least one LOD, namely the original none reduced version
		// Optional skeleton
		SkeletonResourceId	 mSkeletonResourceId;	// Resource ID of the used skeleton, can be invalid


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

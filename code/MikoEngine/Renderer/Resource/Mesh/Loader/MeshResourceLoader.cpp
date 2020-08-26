#include "stdafx.h"
#include "Renderer/Resource/Mesh/Loader/MeshResourceLoader.h"
#include "Renderer/Resource/Mesh/Loader/MeshFileFormat.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"
#include "Renderer/Resource/Mesh/MeshResource.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/Skeleton/SkeletonResourceManager.h"
#include "Renderer/Resource/Skeleton/SkeletonResource.h"
#include "Renderer/IRenderer.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	SE_PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/glm.hpp>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	bool MeshResourceLoader::onDeserialization(IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1Mesh::FORMAT_TYPE, v1Mesh::FORMAT_VERSION, file);
	}

	void MeshResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the mesh header
		v1Mesh::MeshHeader meshHeader;
		mMemoryFile.read(&meshHeader, sizeof(v1Mesh::MeshHeader));

		// Sanity checks
		SE_ASSERT(0 != meshHeader.numberOfBytesPerVertex, "Invalid mesh with zero bytes per vertex")
		SE_ASSERT(0 != meshHeader.numberOfVertices, "Invalid mesh which has no vertices")
		SE_ASSERT(0 != meshHeader.numberOfVertexAttributes, "Invalid mesh which has no vertex attributes")
		SE_ASSERT(0 != meshHeader.numberOfSubMeshes, "Invalid mesh which has no sub-meshes")
		SE_ASSERT(0 != meshHeader.numberOfLods, "Invalid mesh which has no LODs: There must always be at least one LOD, namely the original none reduced version")

		// Set basic mesh resource data
		mMeshResource->setBoundingBoxPosition(meshHeader.minimumBoundingBoxPosition, meshHeader.maximumBoundingBoxPosition);
		mMeshResource->setBoundingSpherePositionRadius(meshHeader.boundingSpherePosition, meshHeader.boundingSphereRadius);
		mMeshResource->setNumberOfVertices(meshHeader.numberOfVertices);
		mMeshResource->setNumberOfIndices(meshHeader.numberOfIndices);
		mMeshResource->setNumberOfLods(meshHeader.numberOfLods);

		{ // Read in the vertex buffer
			// Allocate memory for the local vertex buffer data
			mNumberOfUsedVertexBufferDataBytes = meshHeader.numberOfBytesPerVertex * meshHeader.numberOfVertices;
			if (mNumberOfVertexBufferDataBytes < mNumberOfUsedVertexBufferDataBytes)
			{
				mNumberOfVertexBufferDataBytes = mNumberOfUsedVertexBufferDataBytes;
				delete [] mVertexBufferData;
				mVertexBufferData = new uint8_t[mNumberOfVertexBufferDataBytes];
			}

			// Read in the vertex buffer
			mMemoryFile.read(mVertexBufferData, mNumberOfUsedVertexBufferDataBytes);
		}

		// Read in the index buffer
		mIndexBufferFormat = meshHeader.indexBufferFormat;
		mNumberOfUsedIndexBufferDataBytes = Rhi::IndexBufferFormat::GetNumberOfBytesPerElement(static_cast<Rhi::IndexBufferFormat::Enum>(mIndexBufferFormat)) * mMeshResource->getNumberOfIndices();
		if (mNumberOfUsedIndexBufferDataBytes > 0)
		{
			// Allocate memory for the local index buffer data
			if (mNumberOfIndexBufferDataBytes < mNumberOfUsedIndexBufferDataBytes)
			{
				mNumberOfIndexBufferDataBytes = mNumberOfUsedIndexBufferDataBytes;
				delete [] mIndexBufferData;
				mIndexBufferData = new uint8_t[mNumberOfIndexBufferDataBytes];
			}

			// Read in the index buffer
			mMemoryFile.read(mIndexBufferData, mNumberOfUsedIndexBufferDataBytes);
		}

		// Read in the position-only index buffer
		if (meshHeader.hasPositionOnlyIndices)
		{
			mNumberOfUsedPositionOnlyIndexBufferDataBytes = mNumberOfUsedIndexBufferDataBytes;
			if (mNumberOfUsedPositionOnlyIndexBufferDataBytes > 0)
			{
				// Allocate memory for the local position-only index buffer data
				if (mNumberOfPositionOnlyIndexBufferDataBytes < mNumberOfUsedPositionOnlyIndexBufferDataBytes)
				{
					mNumberOfPositionOnlyIndexBufferDataBytes = mNumberOfUsedPositionOnlyIndexBufferDataBytes;
					delete [] mPositionOnlyIndexBufferData;
					mPositionOnlyIndexBufferData = new uint8_t[mNumberOfPositionOnlyIndexBufferDataBytes];
				}

				// Read in the position-only index buffer
				mMemoryFile.read(mPositionOnlyIndexBufferData, mNumberOfUsedPositionOnlyIndexBufferDataBytes);
			}
		}
		else
		{
			mNumberOfUsedPositionOnlyIndexBufferDataBytes = 0;
		}

		// Read in the vertex attributes
		mNumberOfUsedVertexAttributes = meshHeader.numberOfVertexAttributes;
		if (mNumberOfVertexAttributes < mNumberOfUsedVertexAttributes)
		{
			mNumberOfVertexAttributes = mNumberOfUsedVertexAttributes;
			delete [] mVertexAttributes;
			mVertexAttributes = new Rhi::VertexAttribute[mNumberOfVertexAttributes];
		}
		mMemoryFile.read(mVertexAttributes, sizeof(Rhi::VertexAttribute) * mNumberOfUsedVertexAttributes);

		// Read in the sub-meshes
		mNumberOfUsedSubMeshes = meshHeader.numberOfSubMeshes;
		if (mNumberOfSubMeshes < mNumberOfUsedSubMeshes)
		{
			mNumberOfSubMeshes = mNumberOfUsedSubMeshes;
			delete [] mSubMeshes;
			mSubMeshes = new v1Mesh::SubMesh[mNumberOfSubMeshes];
		}
		mMemoryFile.read(mSubMeshes, sizeof(v1Mesh::SubMesh) * mNumberOfUsedSubMeshes);

		// Read in optional skeleton
		mNumberOfBones = meshHeader.numberOfBones;
		if (mNumberOfBones > 0)
		{
			// Read in the skeleton data in a single burst
			const uint32_t numberOfSkeletonDataBytes = (sizeof(uint8_t) + sizeof(uint32_t) + sizeof(glm::mat4) * 2) * mNumberOfBones;
			mSkeletonData = new uint8_t[numberOfSkeletonDataBytes + (sizeof(glm::mat4) + SkeletonResource::NUMBER_OF_BONE_SPACE_DATA_BYTES) * mNumberOfBones];	// "Renderer::SkeletonResource::mGlobalBoneMatrices" & "Renderer::SkeletonResource::mBoneSpaceData" aren't serialized
			mMemoryFile.read(mSkeletonData, numberOfSkeletonDataBytes);
		}

		// Can we create the RHI resource asynchronous as well?
		if (mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			createVertexArrays();
		}
	}

	bool MeshResourceLoader::onDispatch()
	{
		// Create vertex array object (VAO)
		if (!mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			createVertexArrays();
		}
		mMeshResource->setVertexArray(mVertexArray, mPositionOnlyVertexArray);

		{ // Create sub-meshes
			MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
			SubMeshes& subMeshes = mMeshResource->getSubMeshes();
			subMeshes.resize(mNumberOfUsedSubMeshes);
			for (uint32_t i = 0; i < mNumberOfUsedSubMeshes; ++i)
			{
				// Get source and destination sub-mesh references
				SubMesh& subMesh = subMeshes[i];
				const v1Mesh::SubMesh& v1SubMesh = mSubMeshes[i];

				// Setup sub-mesh
				MaterialResourceId materialResourceId = GetInvalid<MaterialResourceId>();
				materialResourceManager.loadMaterialResourceByAssetId(v1SubMesh.materialAssetId, materialResourceId);
				subMesh.setMaterialResourceId(materialResourceId);
				subMesh.setStartIndexLocation(v1SubMesh.startIndexLocation);
				subMesh.setNumberOfIndices(v1SubMesh.numberOfIndices);

				// Sanity check
				SE_ASSERT(IsValid(subMesh.getMaterialResourceId()), "Invalid sub mesh material resource ID")
			}
		}

		// Optional skeleton
		if (mNumberOfBones > 0)
		{
			SkeletonResourceManager& skeletonResourceManager = mRenderer.getSkeletonResourceManager();

			// Get/create skeleton resource
			SkeletonResource* skeletonResource = nullptr;
			if (IsValid(mMeshResource->getSkeletonResourceId()))
			{
				// Reuse existing skeleton resource
				skeletonResource = &skeletonResourceManager.getById(mMeshResource->getSkeletonResourceId());
				skeletonResource->clearSkeletonData();
			}
			else
			{
				// Create new skeleton resource
				const SkeletonResourceId skeletonResourceId = skeletonResourceManager.createSkeletonResourceByAssetId(getAsset().assetId);
				mMeshResource->setSkeletonResourceId(skeletonResourceId);
				skeletonResource = &skeletonResourceManager.getById(skeletonResourceId);
			}

			// Pass on the skeleton data to the skeleton resource
			skeletonResource->mNumberOfBones = mNumberOfBones;
			skeletonResource->mBoneParentIndices = mSkeletonData;
			mSkeletonData += sizeof(uint8_t) * mNumberOfBones;
			skeletonResource->mBoneIds = reinterpret_cast<uint32_t*>(mSkeletonData);
			mSkeletonData += sizeof(uint32_t) * mNumberOfBones;
			skeletonResource->mLocalBoneMatrices = reinterpret_cast<glm::mat4*>(mSkeletonData);
			mSkeletonData += sizeof(glm::mat4) * mNumberOfBones;
			skeletonResource->mBoneOffsetMatrices = reinterpret_cast<glm::mat4*>(mSkeletonData);
			mSkeletonData += sizeof(glm::mat4) * mNumberOfBones;
			skeletonResource->mGlobalBoneMatrices = reinterpret_cast<glm::mat4*>(mSkeletonData);
			mSkeletonData += sizeof(glm::mat4) * mNumberOfBones;
			skeletonResource->mBoneSpaceData = mSkeletonData;
			skeletonResource->localToGlobalPose();

			// Skeleton data has been passed on
			mSkeletonData = nullptr;
		}

		// Fully loaded?
		return isFullyLoaded();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MeshResourceLoader::MeshResourceLoader(IResourceManager& resourceManager, IRenderer& renderer) :
		IMeshResourceLoader(resourceManager, renderer),
		mBufferManager(renderer.getBufferManager()),
		mVertexArray(nullptr),
		mPositionOnlyVertexArray(nullptr),
		// Temporary vertex buffer
		mNumberOfVertexBufferDataBytes(0),
		mNumberOfUsedVertexBufferDataBytes(0),
		mVertexBufferData(nullptr),
		// Temporary index buffer
		mIndexBufferFormat(0),
		mNumberOfIndexBufferDataBytes(0),
		mNumberOfUsedIndexBufferDataBytes(0),
		mIndexBufferData(nullptr),
		// Temporary position-only index buffer
		mNumberOfPositionOnlyIndexBufferDataBytes(0),
		mNumberOfUsedPositionOnlyIndexBufferDataBytes(0),
		mPositionOnlyIndexBufferData(nullptr),
		// Temporary vertex attributes
		mNumberOfVertexAttributes(0),
		mNumberOfUsedVertexAttributes(0),
		mVertexAttributes(nullptr),
		// Temporary sub-meshes
		mNumberOfSubMeshes(0),
		mNumberOfUsedSubMeshes(0),
		mSubMeshes(nullptr),
		// Optional temporary skeleton
		mNumberOfBones(0),
		mSkeletonData(nullptr)
	{
		// Nothing here
	}

	MeshResourceLoader::~MeshResourceLoader()
	{
		delete [] mVertexBufferData;
		delete [] mIndexBufferData;
		delete [] mPositionOnlyIndexBufferData;
		delete [] mVertexAttributes;
		delete [] mSubMeshes;
		delete [] mSkeletonData;	// In case the mesh resource loaded was never dispatched
	}

	void MeshResourceLoader::createVertexArrays()
	{
		// Create the vertex buffer object (VBO)
		Rhi::IVertexBufferPtr vertexBuffer(mBufferManager.createVertexBuffer(mNumberOfUsedVertexBufferDataBytes, mVertexBufferData, 0, Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename)));

		// Create the index buffer object (IBO)
		Rhi::IIndexBufferPtr indexBuffer(mBufferManager.createIndexBuffer(mNumberOfUsedIndexBufferDataBytes, mIndexBufferData, 0, Rhi::BufferUsage::STATIC_DRAW, static_cast<Rhi::IndexBufferFormat::Enum>(mIndexBufferFormat) RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename)));

		// Create vertex array object (VAO)
		const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer, mRenderer.getMeshResourceManager().getDrawIdVertexBufferPtr() };
		const Rhi::VertexAttributes vertexAttributes(mNumberOfUsedVertexAttributes, mVertexAttributes);
		mVertexArray = mBufferManager.createVertexArray(vertexAttributes, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename));

		// Create the position-only vertex array object (VAO)
		if (mNumberOfUsedPositionOnlyIndexBufferDataBytes > 0)
		{
			// Create the index buffer object (IBO)
			indexBuffer = mBufferManager.createIndexBuffer(mNumberOfUsedPositionOnlyIndexBufferDataBytes, mPositionOnlyIndexBufferData, 0, Rhi::BufferUsage::STATIC_DRAW, static_cast<Rhi::IndexBufferFormat::Enum>(mIndexBufferFormat) RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename));

			// Create vertex array object (VAO)
			mPositionOnlyVertexArray = mBufferManager.createVertexArray(vertexAttributes, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename));
		}
		else
		{
			mPositionOnlyVertexArray = nullptr;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

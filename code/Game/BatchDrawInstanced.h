#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    Batch of instanced cubes using draw instanced
*/
class BatchDrawInstanced final
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	inline BatchDrawInstanced() :
		mNumberOfCubeInstances(0)
	{
		// Nothing here
	}

	/**
	*  @brief
	*    Destructor
	*/
	inline ~BatchDrawInstanced()
	{
		// The RHI resource pointers are released automatically
	}

	/**
	*  @brief
	*    Initialize the batch
	*
	*  @param[in] bufferManager
	*    Buffer manager to use
	*  @param[in] rootSignature
	*    Root signature
	*  @param[in] vertexAttributes
	*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
	*  @param[in] graphicsProgram
	*    Graphics program used for rendering
	*  @param[in] renderPass
	*    Render pass used for rendering
	*  @param[in] numberOfCubeInstances
	*    Number of cube instances
	*  @param[in] alphaBlending
	*    Does this batch use alpha blending?
	*  @param[in] numberOfTextures
	*    Number of textures
	*  @param[in] sceneRadius
	*    Scene radius
	*/
	void initialize(Rhi::IBufferManager& bufferManager, Rhi::IRootSignature& rootSignature, const Rhi::VertexAttributes& vertexAttributes, Rhi::IGraphicsProgram& graphicsProgram, Rhi::IRenderPass& renderPass, uint32_t numberOfCubeInstances, bool alphaBlending, uint32_t numberOfTextures, uint32_t sceneRadius);

	/**
	*  @brief
	*    Fill the batch into the given commando buffer
	*
	*  @param[out] commandBuffer
	*    RHI command buffer to fill
	*/
	void fillCommandBuffer(Rhi::CommandBuffer& commandBuffer) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
private:
	explicit BatchDrawInstanced(const BatchDrawInstanced& source) = delete;
	BatchDrawInstanced& operator =(const BatchDrawInstanced& source) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
private:
	Rhi::IRhiPtr					mRhi;					///< Owner RHI instance, can be a null pointer
	uint32_t						mNumberOfCubeInstances;	///< Number of cube instances
	Rhi::IResourceGroupPtr			mTextureBufferGroup;	///< Texture buffer group with per instance data (used via vertex texture fetch), each entry can contain a null pointer
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;	///< Graphics pipeline state object (PSO), can be a null pointer

};

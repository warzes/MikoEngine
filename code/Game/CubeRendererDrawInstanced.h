#pragma once

#include <MikoEngine/MikoHeader.h>
#include "ICubeRenderer.h"

class BatchDrawInstanced;

/**
*  @brief
*    Cube renderer class using instancing
*
*  @remarks
*    Required RHI features:
*    - Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
*    - 2D array texture
*    - Texture buffer
*/
class CubeRendererDrawInstanced final : public ICubeRenderer
{
public:
	CubeRendererDrawInstanced(Rhi::IRhi& rhi, Rhi::IRenderPass& renderPass, uint32_t numberOfTextures, uint32_t sceneRadius);
	virtual ~CubeRendererDrawInstanced() override;

public:
	virtual void setNumberOfCubes(uint32_t numberOfCubes) override;
	virtual void fillCommandBuffer(float globalTimer, float globalScale, float lightPositionX, float lightPositionY, float lightPositionZ, Rhi::CommandBuffer& commandBuffer) override;

private:
	explicit CubeRendererDrawInstanced(const CubeRendererDrawInstanced& source) = delete;
	CubeRendererDrawInstanced& operator =(const CubeRendererDrawInstanced& source) = delete;
	void fillReusableCommandBuffer();

private:
	Rhi::IRhiPtr			 mRhi;								///< RHI instance to use, always valid
	Rhi::IRenderPass&		 mRenderPass;						///< Render pass to use, always valid
	Rhi::IBufferManagerPtr	 mBufferManager;					///< Buffer manager, can be a null pointer
	Rhi::ITextureManagerPtr  mTextureManager;					///< Texture manager, can be a null pointer
	Rhi::CommandBuffer		 mCommandBuffer;					///< Command buffer which is recorded once and then used multiple times
	uint32_t				 mNumberOfTextures;					///< Number of textures
	uint32_t				 mSceneRadius;						///< Scene radius
	uint32_t				 mMaximumNumberOfInstancesPerBatch;	///< Maximum number of instances per batch
	Rhi::IRootSignaturePtr	 mRootSignature;					///< Root signature, can be a null pointer
	Rhi::ITexture2DArrayPtr  mTexture2DArray;					///< 2D texture array, can be a null pointer
	Rhi::IUniformBufferPtr	 mUniformBufferStaticVs;			///< Static vertex shader uniform buffer object (UBO), can be a null pointer
	Rhi::IUniformBufferPtr	 mUniformBufferDynamicVs;			///< Dynamic vertex shader uniform buffer object (UBO), can be a null pointer
	Rhi::IUniformBufferPtr	 mUniformBufferDynamicFs;			///< Dynamic fragment shader uniform buffer object (UBO), can be a null pointer
	Rhi::IResourceGroupPtr	 mResourceGroupVs;					///< Resource group with vertex shader visibility, can be a null pointer
	Rhi::IResourceGroupPtr	 mResourceGroupFs;					///< Resource group with fragment shader visibility, can be a null pointer
	Rhi::IResourceGroupPtr	 mSamplerStateGroup;				///< Sampler state resource group, can be a null pointer
	Rhi::IGraphicsProgramPtr mGraphicsProgram;					///< Graphics program, can be a null pointer
	Rhi::IVertexArrayPtr	 mVertexArray;						///< Vertex array object (VAO), can be a null pointer
	uint32_t				 mNumberOfBatches;					///< Current number of batches
	BatchDrawInstanced*		 mBatches;							///< Batches, can be a null pointer
};
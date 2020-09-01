#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    A compute shader (CS, suited e.g. for General Purpose Computation on Graphics Processing Unit (GPGPU)) example
*
*  @remarks
*    Demonstrates compute shader use-cases:
*    - Texture image processing
*    - Multi-draw indirect buffer written by a compute shader
*
*    Demonstrates infrastructure usage:
*    - Vertex buffer object (VBO)
*    - Index buffer object (IBO)
*    - Vertex array object (VAO)
*    - Texture buffer object (TBO)
*    - Structured buffer object (SBO)
*    - Indirect buffer
*    - Uniform buffer object (UBO)
*    - 2D texture
*    - Sampler state object (SO)
*    - Vertex shader (VS), fragment shader (FS) and compute shader (CS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Framebuffer object (FBO) used for render to texture
*
*  @note
*    - The compute shader had to be split due to OpenGL number of binding point limitations
*/
class ComputeShader : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer();

	Rhi::IBufferManagerPtr			mBufferManager;					///< Buffer manager, can be a null pointer
	Rhi::ITextureManagerPtr			mTextureManager;				///< Texture manager, can be a null pointer
	Rhi::CommandBuffer				mCommandBuffer;					///< Command buffer
	Rhi::IRootSignaturePtr			mGraphicsRootSignature;			///< Graphics root signature, can be a null pointer
	Rhi::IRootSignaturePtr			mComputeRootSignature1;			///< First compute root signature, can be a null pointer
	Rhi::IRootSignaturePtr			mComputeRootSignature2;			///< Second compute root signature, can be a null pointer
	Rhi::IFramebufferPtr			mFramebuffer;					///< Graphics framebuffer object (FBO), can be a null pointer
	Rhi::IResourceGroupPtr			mComputeResourceGroup1;			///< First compute resource group, can be a null pointer
	Rhi::IResourceGroupPtr			mComputeResourceGroup2;			///< Second compute resource group, can be a null pointer
	Rhi::IResourceGroupPtr			mGraphicsResourceGroup;			///< Graphics resource group, can be a null pointer
	Rhi::IResourceGroupPtr			mGraphicsSamplerStateGroup;		///< Graphics sampler state resource group, can be a null pointer
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;			///< Graphics pipeline state object (PSO), can be a null pointer
	Rhi::IComputePipelineStatePtr	mComputePipelineState1;			///< First compute pipeline state object (PSO), can be a null pointer
	Rhi::IComputePipelineStatePtr	mComputePipelineState2;			///< Second compute pipeline state object (PSO), can be a null pointer
	Rhi::IVertexBufferPtr			mComputeInputVertexBuffer;		///< Graphics vertex buffer object (VBO) read by compute shader, can be a null pointer
	Rhi::IVertexBufferPtr			mComputeOutputVertexBuffer;		///< Graphics vertex buffer object (VBO) written by compute shader, can be a null pointer
	Rhi::IIndexBufferPtr			mComputeInputIndexBuffer;		///< Graphics index buffer object (IBO) read by compute shader, can be a null pointer
	Rhi::IIndexBufferPtr			mComputeOutputIndexBuffer;		///< Graphics index buffer object (IBO) written by compute shader, can be a null pointer
	Rhi::IVertexArrayPtr			mVertexArray;					///< Graphics vertex array object (VAO), can be a null pointer
	Rhi::ITextureBufferPtr			mComputeInputTextureBuffer;		///< Graphics texture buffer (TBO) read by compute shader, can be a null pointer
	Rhi::ITextureBufferPtr			mComputeOutputTextureBuffer;	///< Graphics texture buffer (TBO) written by compute shader, can be a null pointer
	Rhi::IStructuredBufferPtr		mComputeInputStructuredBuffer;	///< Graphics structured buffer read by compute shader, can be a null pointer
	Rhi::IStructuredBufferPtr		mComputeOutputStructuredBuffer;	///< Graphics structured buffer written by compute shader, can be a null pointer
	Rhi::IIndirectBufferPtr			mComputeInputIndirectBuffer;	///< Graphics indirect buffer read by compute shader, can be a null pointer
	Rhi::IIndirectBufferPtr			mComputeOutputIndirectBuffer;	///< Graphics indirect buffer written by compute shader, can be a null pointer
	Rhi::IUniformBufferPtr			mComputeInputUniformBuffer;		///< Graphics uniform buffer (UBO) read by compute shader, can be a null pointer

};
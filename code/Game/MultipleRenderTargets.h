#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    An example showing how to render into multiple render targets (MRT)
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Vertex array object (VAO)
*    - 2D texture
*    - Sampler state object (SO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Framebuffer object (FBO) used for render to texture
*    - Multiple render targets (MRT)
*/
class MultipleRenderTargets : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer();

    static constexpr uint32_t TEXTURE_SIZE = 16;	///< Texture size
    static constexpr uint32_t NUMBER_OF_TEXTURES = 2;	///< Number of textures

	Rhi::IBufferManagerPtr			mBufferManager;									///< Buffer manager, can be a null pointer
	Rhi::ITextureManagerPtr			mTextureManager;								///< Texture manager, can be a null pointer
	Rhi::CommandBuffer				mCommandBuffer;									///< Command buffer
	Rhi::IRootSignaturePtr			mRootSignature;									///< Root signature, can be a null pointer
	Rhi::IFramebufferPtr			mFramebuffer;									///< Framebuffer object (FBO), can be a null pointer
	Rhi::IResourceGroupPtr			mTextureGroup;									///< Texture group, can be a null pointer
	Rhi::IResourceGroupPtr			mSamplerStateGroup;								///< Sampler state resource group, can be a null pointer
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineStateMultipleRenderTargets;	///< Graphics pipeline state object (PSO) multiple render targets, can be a null pointer
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;							///< Graphics pipeline state object (PSO), can be a null pointer
	Rhi::IVertexArrayPtr			mVertexArray;									///< Vertex array object (VAO), can be a null pointer
};
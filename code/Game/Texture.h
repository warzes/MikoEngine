#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    An example showing how to work with textures
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Vertex array object (VAO)
*    - 1D and 2D texture
*    - Sampler state object (SO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*/
class Texture : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer();

    Rhi::IBufferManagerPtr			mBufferManager;			///< Buffer manager, can be a null pointer
    Rhi::ITextureManagerPtr			mTextureManager;		///< Texture manager, can be a null pointer
    Rhi::CommandBuffer				mCommandBuffer;			///< Command buffer
    Rhi::IRootSignaturePtr			mRootSignature;			///< Root signature, can be a null pointer
    Rhi::IResourceGroupPtr			mTextureGroup;			///< Texture group, can be a null pointer
    Rhi::IResourceGroupPtr			mSamplerStateGroup;		///< Sampler state resource group, can be a null pointer
    Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;	///< Graphics pipeline state object (PSO), can be a null pointer
    Rhi::IVertexArrayPtr			mVertexArray;			///< Vertex array object (VAO), can be a null pointer
};
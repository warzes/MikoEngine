#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    Tessellation shader example
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Vertex array object (VAO)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Vertex shader (VS), tessellation control shader (TCS), tessellation evaluation shader (TES) and fragment shader (FS)
*/
class TessellationShader : public Application
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
    Rhi::CommandBuffer				mCommandBuffer;			///< Command buffer
    Rhi::IRootSignaturePtr			mRootSignature;			///< Root signature, can be a null pointer
    Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;	///< Graphics pipeline state object (PSO), can be a null pointer
    Rhi::IVertexArrayPtr			mVertexArray;			///< Vertex array object (VAO), can be a null pointer
};
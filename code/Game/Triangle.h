#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    Shows how to bring a triangle onto the screen
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Vertex array object (VAO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Debug methods
*/
class Triangle : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    Rhi::IBufferManagerPtr m_bufferManager;
    Rhi::IRootSignaturePtr m_rootSignature;
    Rhi::IGraphicsPipelineStatePtr m_graphicsPipelineState;
    Rhi::IVertexArrayPtr m_vertexArray;
    Rhi::CommandBuffer m_commandBuffer;
};
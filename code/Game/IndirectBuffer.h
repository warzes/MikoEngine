#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    Shows how to use indirect buffer
*
*  @remarks
*    Demonstrates:
*    - Everything from "Triangle"
*    - Indirect buffer
*/
class IndirectBuffer : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer();
    Rhi::IBufferManagerPtr m_bufferManager;
    Rhi::IRootSignaturePtr m_rootSignature;
    Rhi::IGraphicsPipelineStatePtr m_graphicsPipelineState;
    Rhi::IVertexArrayPtr m_vertexArray;
    Rhi::CommandBuffer m_commandBuffer;

    Rhi::IIndirectBufferPtr m_indirectBuffer;	// Indirect buffer, can be a null pointer
};
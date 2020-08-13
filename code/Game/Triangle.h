#pragma once

#include <MikoEngine/MikoHeader.h>

class Triangle : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    Rhi::IBufferManagerPtr bufferManager;
    Rhi::IRootSignaturePtr rootSignature;
    Rhi::IGraphicsPipelineStatePtr graphicsPipelineState;
    Rhi::IVertexArrayPtr vertexArray;
    Rhi::CommandBuffer commandBuffer;
};
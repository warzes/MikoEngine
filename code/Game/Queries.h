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
class Queries : public Application
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

    Rhi::IQueryPoolPtr mOcclusionQueryPool;				///< Asynchronous occlusion query pool, can be a null pointer
    Rhi::IQueryPoolPtr mPipelineStatisticsQueryPool;	///< Asynchronous pipeline statistics query pool, can be a null pointer
    Rhi::IQueryPoolPtr mTimestampQueryPool;				///< Asynchronous timestamp query pool, can be a null pointer
};
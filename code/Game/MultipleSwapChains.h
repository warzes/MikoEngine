#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    Shows how to use multiple swap chains
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Vertex array object (VAO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Multiple swap chains
*
*  @note
*    - This example is intentionally using OS dependent native window creation in order to keep the example "close to metal"
*/
class MultipleSwapChains : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer(const float color[4], Rhi::CommandBuffer& commandBuffer) const;

    inline virtual bool doesCompleteOwnDrawing() const
    {
        // Default implementation does not complete own drawing
        return true;
    }

    Rhi::IBufferManagerPtr			mBufferManager;			///< Buffer manager, can be a null pointer
    Rhi::CommandBuffer				mCommandBuffer;			///< Command buffer
    Rhi::IRootSignaturePtr			mRootSignature;			///< Root signature, can be a null pointer
    Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;	///< Graphics pipeline state object (PSO), can be a null pointer
    Rhi::IVertexArrayPtr			mVertexArray;			///< Vertex array object (VAO), can be a null pointer
    Rhi::ISwapChainPtr				mSwapChain;				///< Swap chain, can be a null pointer
};
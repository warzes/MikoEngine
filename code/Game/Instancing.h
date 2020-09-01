#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    Instancing example
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Index buffer object (IBO)
*    - Vertex array object (VAO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
*    - Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
*/
class Instancing : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer();

	Rhi::IBufferManagerPtr	mBufferManager;	///< Buffer manager, can be a null pointer
	Rhi::CommandBuffer		mCommandBuffer;	///< Command buffer
	Rhi::IRootSignaturePtr	mRootSignature;	///< Root signature, can be a null pointer
	// Left side (green): Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineStateInstancedArrays;	///< Graphics pipeline state object (PSO), can be a null pointer
	Rhi::IVertexArrayPtr			mVertexArrayInstancedArrays;			///< Vertex array object (VAO), can be a null pointer
	// Right side (blue): Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineStateDrawInstanced;	///< Graphics pipeline state object (PSO), can be a null pointer
	Rhi::IVertexArrayPtr			mVertexArrayDrawInstanced;				///< Vertex array object (VAO), can be a null pointer
};
#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    Shows some vertex buffer object (VBO) use cases
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Vertex array object (VAO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Multiple vertex attributes within a single vertex buffer object (VBO), vertex array object (VAO) is only using one vertex buffer object (VBO)
*    - One vertex buffer object (VBO) per vertex attribute, vertex array object (VAO) is using multiple vertex buffer objects (VBO)
*/
class VertexBuffer : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer();
	Rhi::IBufferManagerPtr	mBufferManager;		///< Buffer manager, can be a null pointer
	Rhi::CommandBuffer		mCommandBuffer;		///< Command buffer
	Rhi::IRootSignaturePtr	mRootSignature;		///< Root signature, can be a null pointer
	// Using one vertex buffer object (VBO)
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineStateVbo;	///< Graphics pipeline state object (PSO), can be a null pointer
	Rhi::IVertexArrayPtr			mVertexArrayVbo;			///< Vertex array object (VAO), can be a null pointer
	// Using multiple vertex buffer objects (VBO)
	Rhi::IVertexArrayPtr			mVertexArrayVbos;			///< Vertex array object (VAO), can be a null pointer
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineStateVbos;	///< Graphics pipeline state object (PSO), can be a null pointer
};
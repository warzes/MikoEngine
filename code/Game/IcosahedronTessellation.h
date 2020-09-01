#pragma once

#include <MikoEngine/MikoHeader.h>

/**
*  @brief
*    More advanced tessellation example
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Index buffer object (IBO)
*    - Vertex array object (VAO)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Uniform buffer object (UBO)
*    - Vertex shader (VS), tessellation control shader (TCS), tessellation evaluation shader (TES), geometry shader (GS) and fragment shader (FS)
*/
class IcosahedronTessellation : public Application
{
public:
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
    void fillCommandBuffer();

	Rhi::IBufferManagerPtr			mBufferManager;				///< Buffer manager, can be a null pointer
	Rhi::CommandBuffer				mCommandBuffer;				///< Command buffer
	Rhi::IRootSignaturePtr			mRootSignature;				///< Root signature, can be a null pointer
	Rhi::IUniformBufferPtr			mUniformBufferDynamicTcs;	///< Dynamic tessellation control shader uniform buffer object (UBO), can be a null pointer
	Rhi::IResourceGroupPtr			mUniformBufferGroupTcs;		///< Uniform buffer group with tessellation control shader visibility, can be a null pointer
	Rhi::IResourceGroupPtr			mUniformBufferGroupTes;		///< Uniform buffer group with tessellation evaluation shader visibility, can be a null pointer
	Rhi::IResourceGroupPtr			mUniformBufferGroupGs;		///< Uniform buffer group with geometry visibility, can be a null pointer
	Rhi::IResourceGroupPtr			mUniformBufferGroupFs;		///< Uniform buffer group with fragment shader visibility, can be a null pointer
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;		///< Graphics pipeline state object (PSO), can be a null pointer
	Rhi::IVertexArrayPtr			mVertexArray;				///< Vertex array object (VAO), can be a null pointer
	float							mTessellationLevelOuter;	///< Outer tessellation level
	float							mTessellationLevelInner;	///< Inner tessellation level
};
#pragma once

#include <MikoEngine/MikoHeader.h>


class CubeTexture : public Application
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
	Rhi::IUniformBufferPtr			mUniformBuffer;			///< Uniform buffer object (UBO), can be a null pointer
	Rhi::IResourceGroupPtr			mTextureGroup;			///< Texture group, can be a null pointer
	Rhi::IResourceGroupPtr			mSamplerStateGroup;		///< Sampler state resource group, can be a null pointer
	Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;	///< Graphics pipeline state object (PSO), can be a null pointer
	Rhi::IGraphicsProgramPtr	    mGraphicsProgram;		///< Graphics program, can be a null pointer
	Rhi::IVertexArrayPtr			mVertexArray;			///< Vertex array object (VAO), can be a null pointer
	// Optimization: Cache data to not bother the RHI implementation to much
	handle mObjectSpaceToClipSpaceMatrixUniformHandle;	///< Object space to clip space matrix uniform handle, can be "NULL_HANDLE"

	float					mGlobalTimer;	///< Global timer
};
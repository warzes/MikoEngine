#pragma once

#include <MikoEngine/MikoHeader.h>
#include "MikoEngine/Renderer/Core/Time/Stopwatch.h"
#include "MikoEngine/Renderer/Resource/IResourceListener.h"
#include "MikoEngine/Renderer/Core/File/IFileManager.h"
#include "MikoEngine/Renderer/RendererInstance.h"
#include "MikoEngine/Renderer/Core/IProfiler.h"

namespace Renderer
{
    typedef uint32_t MeshResourceId;	///< POD mesh resource identifier
    typedef uint32_t TextureResourceId;	///< POD texture resource identifier
}

/**
*  @brief
*    Mesh example
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Index buffer object (IBO)
*    - Vertex array object (VAO)
*    - Texture buffer object (TBO)
*    - Uniform buffer object (UBO)
*    - Sampler state object (SO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Blinn-Phong shading
*    - Albedo, normal, roughness and emissive mapping
*    - Optimization: Cache data to not bother the RHI to much
*    - Compact vertex format (32 bit texture coordinate, QTangent, 56 bytes vs. 28 bytes per vertex)
*/
class Mesh : public Application, public Renderer::IResourceListener
{
public:
	Mesh();
    bool init(int argc, const char* argv[]) override;
    void update(double delta) override;
    void shutdown() override;

    ApplicationSetting intial_app_settings() override;

    void window_resized(int width, int height) override;

private:
	inline virtual void onLoadingStateChange(const Renderer::IResource&) override
	{
		// Forget about the resource group so it's rebuild
		mResourceGroup = nullptr;
		mCommandBuffer.clear();
		fillCommandBuffer();
	}

	void fillCommandBuffer();

	Renderer::IRenderer* getRenderer() const;

	Renderer::IFileManager*		mFileManager;
#ifdef RENDERER_PROFILER
	Renderer::IProfiler*	mProfiler;			///< Profiler instance, can be a null pointer
#endif
	Renderer::Context*			mRendererContext;	///< Renderer context instance, can be a null pointer
	Renderer::RendererInstance* mRendererInstance;	///< Renderer instance, can be a null pointer
#ifdef RENDERER_GRAPHICS_DEBUGGER
	Renderer::IGraphicsDebugger* mGraphicsDebugger;	///< Graphics debugger instance, can be a null pointer
#endif

	Rhi::CommandBuffer			   mCommandBuffer;				///< Command buffer
	Rhi::IRootSignaturePtr		   mRootSignature;				///< Root signature, can be a null pointer
	Rhi::IUniformBufferPtr		   mUniformBuffer;				///< Uniform buffer object (UBO), can be a null pointer
	Rhi::IGraphicsPipelineStatePtr mGraphicsPipelineState;		///< Graphics pipeline state object (PSO), can be a null pointer
	Rhi::IGraphicsProgramPtr	   mGraphicsProgram;			///< Graphics program, can be a null pointer
	Renderer::MeshResourceId	   mMeshResourceId;				///< Mesh resource ID, can be set to invalid value
	Renderer::TextureResourceId	   m_argb_nxaTextureResourceId;
	Renderer::TextureResourceId    m_hr_rg_mb_nyaTextureResourceId;
	Renderer::TextureResourceId    mEmissiveTextureResourceId;
	Rhi::IResourceGroupPtr		   mResourceGroup;				///< Resource group, can be a null pointer
	Rhi::ISamplerStatePtr		   mSamplerStatePtr;			///< Sampler state, can be a null pointer
	Rhi::IResourceGroupPtr		   mSamplerStateGroup;			///< Sampler state resource group, can be a null pointer
	// Optimization: Cache data to not bother the RHI implementation to much
	handle mObjectSpaceToClipSpaceMatrixUniformHandle;	///< Object space to clip space matrix uniform handle, can be "NULL_HANDLE"
	handle mObjectSpaceToViewSpaceMatrixUniformHandle;	///< Object space to view space matrix uniform handle, can be "NULL_HANDLE"
	// For timing
	Renderer::Stopwatch mStopwatch;		///< Stopwatch instance
	float				mGlobalTimer;	///< Global timer
};
#pragma once

#include "DefaultAssert.h"
#include "DefaultLog.h"
#include "DefaultAllocator.h"

#if SE_DEBUG
// Resource name for debugging purposes, ignored when not using "SE_DEBUG"
// debugName - ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
#	define RHI_RESOURCE_DEBUG_NAME_PARAMETER , const char debugName[] = ""
#	define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT , const char debugName[]
// Pass resource name for debugging purposes, ignored when not using "SE_DEBUG"
#	define RHI_RESOURCE_DEBUG_PASS_PARAMETER , debugName
#else
// Resource name for debugging purposes, ignored when not using "SE_DEBUG"
// debugName - ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
#	define RHI_RESOURCE_DEBUG_NAME_PARAMETER
#	define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT
// Pass resource name for debugging purposes, ignored when not using "SE_DEBUG"
#	define RHI_RESOURCE_DEBUG_PASS_PARAMETER
#endif

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
namespace Rhi
{
	class ILog;
	class IAssert;
	class Context;
	class IRhi;
	class IShaderLanguage;
	class IResource;
	class IRootSignature;
	class IResourceGroup;
	class IGraphicsProgram;
	class IVertexArray;
	class IRenderPass;
	class IQueryPool;
	class IRenderTarget;
	class IRenderWindow;
	class ISwapChain;
	class IFramebuffer;
	struct FramebufferAttachment;
	class CommandBuffer;
	class IBufferManager;
	class IBuffer;
	class IVertexBuffer;
	class IIndexBuffer;
	class ITextureBuffer;
	class IStructuredBuffer;
	class IIndirectBuffer;
	class IUniformBuffer;
	class ITextureManager;
	class ITexture;
	class ITexture1D;
	class ITexture1DArray;
	class ITexture2D;
	class ITexture2DArray;
	class ITexture3D;
	class ITextureCube;
	class ITextureCubeArray;
	class IState;
	class IPipelineState;
	class IGraphicsPipelineState;
	class IComputePipelineState;
	class ISamplerState;
	class IShader;
	class IVertexShader;
	class ITessellationControlShader;
	class ITessellationEvaluationShader;
	class IGeometryShader;
	class IFragmentShader;
	class ITaskShader;
	class IMeshShader;
	class IComputeShader;
} // namespace Rhi
// rendering hardware interface
#pragma once

#include "RHIDebug.h"
#include "RHICore.h"
#include "RHIContext.h"
#include "RHITypes.h"
#include "SamplerStateTypes.h"
#include "RootSignatureTypes.h"
#include "TextureTypes.h"
#include "BlendStateTypes.h"
#include "BufferTypes.h"
#include "VertexArrayTypes.h"
#include "IndexBufferTypes.h"
#include "IndirectBufferTypes.h"
#include "RasterizerStateTypes.h"
#include "DepthStencilStateTypes.h"
#include "PipelineStateTypes.h"
#include "ShaderTypes.h"
#include "QueryType.h"
#include "Core/MemoryPtr.h"
#include "Capabilities.h"
#include "Statistics.h"
#include "IRhi.h"
#include "IShaderLanguage.h"
#include "IResource.h"
#include "IRootSignature.h"
#include "IResourceGroup.h"
#include "IGraphicsProgram.h"
#include "IRenderPass.h"
#include "IQueryPool.h"
#include "IRenderTarget.h"
#include "ISwapChain.h"
#include "IFramebuffer.h"
#include "IBufferManager.h"
#include "IVertexArray.h"
#include "IBuffer.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "ITextureBuffer.h"
#include "IStructuredBuffer.h"
#include "IIndirectBuffer.h"
#include "IUniformBuffer.h"
#include "ITextureManager.h"
#include "ITexture.h"
#include "ITexture1D.h"
#include "ITexture1DArray.h"
#include "ITexture2D.h"
#include "ITexture2DArray.h"
#include "ITexture3D.h"
#include "ITextureCube.h"
#include "ITextureCubeArray.h"
#include "IState.h"
#include "IPipelineState.h"
#include "IGraphicsPipelineState.h"
#include "IComputePipelineState.h"
#include "ISamplerState.h"
#include "IShader.h"
#include "IVertexShader.h"
#include "ITessellationControlShader.h"
#include "ITessellationEvaluationShader.h"
#include "IGeometryShader.h"
#include "IFragmentShader.h"
#include "ITaskShader.h"
#include "IMeshShader.h"
#include "IComputeShader.h"
#include "CommandBuffer.h"

//[-------------------------------------------------------]
//[ Debug macros                                          ]
//[-------------------------------------------------------]
#if SE_DEBUG
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name) if (nullptr != rhi) { (rhi)->setDebugMarker(name); }

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->setDebugMarker(__FUNCTION__); }

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name) if (nullptr != rhi) { (rhi)->beginDebugEvent(name); }

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->beginDebugEvent(__FUNCTION__); }

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_END_DEBUG_EVENT(rhi) if (nullptr != rhi) { (rhi)->endDebugEvent(); }

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name) , name

	/**
	*  @brief
	*    Decorate the debug name to make it easier to see the semantic of the resource
	*
	*  @param[in] name
	*    Debug name provided from the outside
	*  @param[in] decoration
	*    Decoration to append in front (e.g. "IBO", will result in appended "IBO: " in front if the provided name isn't empty)
	*  @param[in] numberOfDecorationCharacters
	*    Number of decoration characters
	*
	*  @note
	*    - The result is in local string variable "detailedName"
	*    - Traditional C-string on the runtime stack used for efficiency reasons (just for debugging, but must still be some kind of usable)
	*/
	#define RHI_DECORATED_DEBUG_NAME(name, detailedName, decoration, numberOfDecorationCharacters) \
		RHI_ASSERT(strlen(name) < 256, "Name is not allowed to exceed 255 characters") \
		char detailedName[256 + numberOfDecorationCharacters] = decoration; \
		if (name[0] != '\0') \
		{ \
			strcat(detailedName, ": "); \
			strncat(detailedName, name, 256); \
		}
#else
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name)

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi)

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name)

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi)

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_END_DEBUG_EVENT(rhi)

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name)
#endif
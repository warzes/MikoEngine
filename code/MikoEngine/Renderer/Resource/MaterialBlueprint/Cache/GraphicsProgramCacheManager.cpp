#include "stdafx.h"
#include "Renderer/Resource/MaterialBlueprint/Cache/GraphicsProgramCacheManager.h"
#include "Renderer/Resource/MaterialBlueprint/Cache/GraphicsProgramCache.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "Renderer/Resource/ShaderBlueprint/Cache/ShaderCache.h"
#include "Renderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "Renderer/Resource/VertexAttributes/VertexAttributesResourceManager.h"
#include "Renderer/Resource/VertexAttributes/VertexAttributesResource.h"
#include "Renderer/Core/Math/Math.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	GraphicsProgramCacheId GraphicsProgramCacheManager::generateGraphicsProgramCacheId(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature)
	{
		GraphicsProgramCacheId graphicsProgramCacheId = Math::FNV1a_INITIAL_HASH_32;
		for (uint8_t i = 0; i < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++i)
		{
			const ShaderCombinationId shaderCombinationId = graphicsPipelineStateSignature.getShaderCombinationId(static_cast<GraphicsShaderType>(i));
			if (IsValid(shaderCombinationId))
			{
				graphicsProgramCacheId = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&shaderCombinationId), sizeof(ShaderCombinationId), graphicsProgramCacheId);
			}
		}

		// Done
		return graphicsProgramCacheId;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GraphicsProgramCache* GraphicsProgramCacheManager::getGraphicsProgramCacheByGraphicsPipelineStateSignature(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature)
	{
		// Does the graphics program cache already exist?
		GraphicsProgramCache* graphicsProgramCache = nullptr;
		const GraphicsProgramCacheId graphicsProgramCacheId = generateGraphicsProgramCacheId(graphicsPipelineStateSignature);
		std::unique_lock<std::mutex> mutexLock(mMutex);
		GraphicsProgramCacheById::const_iterator iterator = mGraphicsProgramCacheById.find(graphicsProgramCacheId);
		if (iterator != mGraphicsProgramCacheById.cend())
		{
			graphicsProgramCache = iterator->second;
		}
		else
		{
			// Create the RHI program: Decide which shader language should be used (for example "GLSL" or "HLSL")
			const MaterialBlueprintResource& materialBlueprintResource = mGraphicsPipelineStateCacheManager.getMaterialBlueprintResource();
			const Rhi::IRootSignaturePtr& rootSignaturePtr = materialBlueprintResource.getRootSignaturePtr();
			Rhi::IShaderLanguage& shaderLanguage = rootSignaturePtr->getRhi().getDefaultShaderLanguage();
			const IRenderer& renderer = materialBlueprintResource.getResourceManager<MaterialBlueprintResourceManager>().getRenderer();

			// Create the shaders
			ShaderCacheManager& shaderCacheManager = renderer.getShaderBlueprintResourceManager().getShaderCacheManager();
			Rhi::IShader* shaders[NUMBER_OF_GRAPHICS_SHADER_TYPES] = {};
			for (uint8_t i = 0; i < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++i)
			{
				ShaderCache* shaderCache = shaderCacheManager.getGraphicsShaderCache(graphicsPipelineStateSignature, materialBlueprintResource, shaderLanguage, static_cast<GraphicsShaderType>(i));
				if (nullptr != shaderCache)
				{
					shaders[i] = shaderCache->getShaderPtr();
				}
				else
				{
					// No error, just means there's no shader cache because e.g. there's no shader of the requested type
				}
			}

			// Create the graphics program
			Rhi::IGraphicsProgram* graphicsProgram = shaderLanguage.createGraphicsProgram(*rootSignaturePtr,
				renderer.getVertexAttributesResourceManager().getById(materialBlueprintResource.getVertexAttributesResourceId()).getVertexAttributes(),
				static_cast<Rhi::IVertexShader*>(shaders[static_cast<int>(GraphicsShaderType::Vertex)]),
				static_cast<Rhi::ITessellationControlShader*>(shaders[static_cast<int>(GraphicsShaderType::TessellationControl)]),
				static_cast<Rhi::ITessellationEvaluationShader*>(shaders[static_cast<int>(GraphicsShaderType::TessellationEvaluation)]),
				static_cast<Rhi::IGeometryShader*>(shaders[static_cast<int>(GraphicsShaderType::Geometry)]),
				static_cast<Rhi::IFragmentShader*>(shaders[static_cast<int>(GraphicsShaderType::Fragment)])
				RHI_RESOURCE_DEBUG_NAME("Graphics program cache manager")
			);

			// Create the new graphics program cache instance
			if (nullptr != graphicsProgram)
			{
				graphicsProgramCache = new GraphicsProgramCache(graphicsProgramCacheId, *graphicsProgram);
				mGraphicsProgramCacheById.emplace(graphicsProgramCacheId, graphicsProgramCache);
			}
			else
			{
				// TODO(co) Error handling
				SE_ASSERT(false, "Invalid graphics program")
			}
		}

		// Done
		return graphicsProgramCache;
	}

	void GraphicsProgramCacheManager::clearCache()
	{
		std::unique_lock<std::mutex> mutexLock(mMutex);
		for (auto& graphicsProgramCacheElement : mGraphicsProgramCacheById)
		{
			delete graphicsProgramCacheElement.second;
		}
		mGraphicsProgramCacheById.clear();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

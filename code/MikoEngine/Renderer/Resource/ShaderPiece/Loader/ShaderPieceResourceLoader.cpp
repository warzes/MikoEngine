#include "stdafx.h"
#include "Renderer/Resource/ShaderPiece/Loader/ShaderPieceResourceLoader.h"
#include "Renderer/Resource/ShaderPiece/Loader/ShaderPieceFileFormat.h"
#include "Renderer/Resource/ShaderPiece/ShaderPieceResource.h"
#include "Renderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "Renderer/Resource/ShaderBlueprint/ShaderBlueprintResource.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "Renderer/IRenderer.h"

#include <unordered_set>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void ShaderPieceResourceLoader::initialize(const Asset& asset, bool reload, IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mShaderPieceResource = static_cast<ShaderPieceResource*>(&resource);
	}

	bool ShaderPieceResourceLoader::onDeserialization(IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1ShaderPiece::FORMAT_TYPE, v1ShaderPiece::FORMAT_VERSION, file);
	}

	void ShaderPieceResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the shader piece header
		v1ShaderPiece::ShaderPieceHeader shaderPieceHeader;
		mMemoryFile.read(&shaderPieceHeader, sizeof(v1ShaderPiece::ShaderPieceHeader));

		// Sanity check
		RHI_ASSERT(shaderPieceHeader.numberOfShaderSourceCodeBytes > 0, "Invalid shader piece asset without any shader source code detected")

		// Allocate more temporary memory, if required
		if (mMaximumNumberOfShaderSourceCodeBytes < shaderPieceHeader.numberOfShaderSourceCodeBytes)
		{
			mMaximumNumberOfShaderSourceCodeBytes = shaderPieceHeader.numberOfShaderSourceCodeBytes;
			delete [] mShaderSourceCode;
			mShaderSourceCode = new char[mMaximumNumberOfShaderSourceCodeBytes];
		}

		// Read the shader piece ASCII source code
		mMemoryFile.read(mShaderSourceCode, shaderPieceHeader.numberOfShaderSourceCodeBytes);
		mShaderPieceResource->mShaderSourceCode.assign(mShaderSourceCode, mShaderSourceCode + shaderPieceHeader.numberOfShaderSourceCodeBytes);
	}

	bool ShaderPieceResourceLoader::onDispatch()
	{
		// TODO(co) Cleanup: Get all influenced material blueprint resources
		if (getReload())
		{
			const ShaderPieceResourceId shaderPieceResourceId = mShaderPieceResource->getId();
			const ShaderBlueprintResourceManager& shaderBlueprintResourceManager = mRenderer.getShaderBlueprintResourceManager();
			typedef std::unordered_set<MaterialBlueprintResource*> MaterialBlueprintResourcePointers;
			MaterialBlueprintResourcePointers materialBlueprintResourcePointers;
			const MaterialBlueprintResourceManager& materialBlueprintResourceManager = mRenderer.getMaterialBlueprintResourceManager();
			const uint32_t numberOfElements = materialBlueprintResourceManager.getNumberOfResources();
			for (uint32_t i = 0; i < numberOfElements; ++i)
			{
				MaterialBlueprintResource& materialBlueprintResource = materialBlueprintResourceManager.getByIndex(i);
				ShaderBlueprintResourceId shaderBlueprintResourceId = materialBlueprintResource.getComputeShaderBlueprintResourceId();
				if (isValid(shaderBlueprintResourceId))
				{
					// Compute pipeline state object (PSO)
					const ShaderBlueprintResource::IncludeShaderPieceResourceIds& includeShaderPieceResourceIds = shaderBlueprintResourceManager.getById(shaderBlueprintResourceId).getIncludeShaderPieceResourceIds();
					if (std::find(includeShaderPieceResourceIds.cbegin(), includeShaderPieceResourceIds.cend(), shaderPieceResourceId) != includeShaderPieceResourceIds.cend())
					{
						materialBlueprintResourcePointers.insert(&materialBlueprintResource);
					}
				}
				else
				{
					// Graphics pipeline state object (PSO)
					for (uint8_t graphicsShaderType = 0; graphicsShaderType < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++graphicsShaderType)
					{
						shaderBlueprintResourceId = materialBlueprintResource.getGraphicsShaderBlueprintResourceId(static_cast<GraphicsShaderType>(graphicsShaderType));
						if (isValid(shaderBlueprintResourceId))
						{
							const ShaderBlueprintResource::IncludeShaderPieceResourceIds& includeShaderPieceResourceIds = shaderBlueprintResourceManager.getById(shaderBlueprintResourceId).getIncludeShaderPieceResourceIds();
							if (std::find(includeShaderPieceResourceIds.cbegin(), includeShaderPieceResourceIds.cend(), shaderPieceResourceId) != includeShaderPieceResourceIds.cend())
							{
								materialBlueprintResourcePointers.insert(&materialBlueprintResource);
								break;
							}
						}
					}
				}
			}
			for (MaterialBlueprintResource* materialBlueprintResource : materialBlueprintResourcePointers)
			{
				materialBlueprintResource->getGraphicsPipelineStateCacheManager().clearCache();
				materialBlueprintResource->getGraphicsPipelineStateCacheManager().getGraphicsProgramCacheManager().clearCache();
				materialBlueprintResource->getComputePipelineStateCacheManager().clearCache();
				++const_cast<MaterialProperties&>(materialBlueprintResource->getMaterialProperties()).mShaderCombinationGenerationCounter;
			}

			// TODO(co) Do only clear the influenced shader cache entries
			mRenderer.getShaderBlueprintResourceManager().getShaderCacheManager().clearCache();
		}

		// Fully loaded
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

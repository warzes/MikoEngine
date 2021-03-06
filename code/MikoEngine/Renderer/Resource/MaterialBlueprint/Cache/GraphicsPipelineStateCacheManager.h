#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateSignature.h"
#include "Renderer/Resource/MaterialBlueprint/Cache/GraphicsProgramCacheManager.h"
#include "Core/File/IFile.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class GraphicsPipelineStateCache;
}


// Disable warnings
// TODO(co) See "Renderer::GraphicsPipelineStateCacheManager::GraphicsPipelineStateCacheManager()": How the heck should we avoid such a situation without using complicated solutions like a pointer to an instance? (= more individual allocations/deallocations)
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4355)	// warning C4355: 'this': used in base member initializer list


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t GraphicsPipelineStateSignatureId;	// Graphics pipeline state signature identifier, result of hashing the referenced shaders as well as other pipeline state properties


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Graphics pipeline state cache manager
	*
	*  @remarks
	*    The graphics pipeline state cache is the top of the shader related cache hierarchy and maps to Vulkan, Direct3D 12,
	*    Apple Metal and other rendering APIs using pipeline state objects (PSO). The next cache hierarchy
	*    level is the graphics program cache which maps to linked monolithic OpenGL programs and is also nice as a collection
	*    of shader compiler results which are fed into pipeline states. The next lowest cache hierarchy level is the
	*    shader cache (vertex shader, fragment shader etc.) which handles the binary results of the shader compiler.
	*    As of January 2016, although claimed to fulfill the OpenGL 4.1 specification, Apples OpenGL implementation used
	*    on Mac OS X lacks the feature of receiving the program binary in order to reuse it for the next time instead of
	*    fully compiling a program. Hence, at the lowest cache hierarchy, there's a shader source code cache for the build
	*    shader source codes so at least this doesn't need to be performed during each program execution.
	*
	*    Sum up of the cache hierarchy:
	*    - 0: "Renderer::GraphicsPipelineStateCacheManager": Maps to Vulkan, Direct3D 12, Apple Metal etc.; managed by material blueprint
	*    - 1: "Renderer::GraphicsProgramCacheManager": Maps to linked monolithic OpenGL programs; managed by shader blueprint manager
	*    - 2: "Renderer::ShaderCacheManager": Maps to Direct3D 9 - 11, separate OpenGL shader objects and is still required for Direct3D 12
	*      and other similar designed APIs because the binary shaders are required when creating pipeline state objects;
	*      managed by shader blueprint manager
	*    - 3: "Renderer::ShaderSourceCodeCacheManager": Shader source code cache for the build shader source codes, used for e.g. Apples
	*      OpenGL implementation lacking of binary program support; managed by shader blueprint manager   TODO(co) "Renderer::ShaderSourceCodeCacheManager" doesn't exist, yet
	*
	*    The graphics pipeline state cache has two types of IDs:
	*    - "Renderer::GraphicsPipelineStateSignatureId" -> Result of hashing the material blueprint ID and the shader combination generating shader properties and dynamic shader pieces
	*    - "Renderer::GraphicsPipelineStateCacheId" -> Includes the hashing the build shader source code
	*    Those two types of IDs are required because it's possible that different "Renderer::GraphicsPipelineStateSignatureId" result in one and the
	*    same build shader source code of references shaders.
	*
	*  @note
	*    - One pipeline state cache manager per material blueprint instance
	*
	*  @todo
	*    - TODO(co) For Vulkan, DirectX 12 and Apple Metal the pipeline state object instance will be managed in here
	*    - TODO(co) Direct3D 12: Pipeline state object: Add support for "GetCachedBlob" (super efficient material cache), see https://github.com/Microsoft/DirectX-Graphics-Samples/blob/master/Samples/D3D12PipelineStateCache/src/PSOLibrary.cpp
	*/
	class GraphicsPipelineStateCacheManager final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialBlueprintResource;	// Is creating and using a graphics program cache manager instance


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the owner material blueprint resource
		*
		*  @return
		*    The owner material blueprint resource
		*/
		[[nodiscard]] inline MaterialBlueprintResource& getMaterialBlueprintResource() const
		{
			return mMaterialBlueprintResource;
		}

		/**
		*  @brief
		*    Return the graphics program cache manager
		*
		*  @return
		*    The graphics program cache manager
		*/
		[[nodiscard]] inline GraphicsProgramCacheManager& getGraphicsProgramCacheManager()
		{
			return mGraphicsProgramCacheManager;
		}

		/**
		*  @brief
		*    Request a graphics pipeline state cache instance by combination
		*
		*  @param[in] serializeGraphicsdPipelineStateHash
		*    FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
		*  @param[in] shaderProperties
		*    Shader properties to use
		*  @param[in] allowEmergencySynchronousCompilation
		*    Allow emergency synchronous compilation if no fallback could be found? This will result in a runtime hiccup instead of graphics artifacts.
		*
		*  @return
		*    The requested graphics pipeline state cache instance, null pointer on error, do not destroy the instance
		*/
		[[nodiscard]] const GraphicsPipelineStateCache* getGraphicsPipelineStateCacheByCombination(uint32_t serializedGraphicsPipelineStateHash, const ShaderProperties& shaderProperties, bool allowEmergencySynchronousCompilation);

		/**
		*  @brief
		*    Clear the pipeline state cache manager
		*/
		void clearCache();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit GraphicsPipelineStateCacheManager(MaterialBlueprintResource& materialBlueprintResource) :
			mMaterialBlueprintResource(materialBlueprintResource),
			mGraphicsProgramCacheManager(*this),
			mPipelineStateObjectCacheNeedSaving(false)
		{
			// Nothing here
		}

		explicit GraphicsPipelineStateCacheManager(const GraphicsPipelineStateCacheManager&) = delete;

		inline ~GraphicsPipelineStateCacheManager()
		{
			clearCache();
		}

		GraphicsPipelineStateCacheManager& operator=(const GraphicsPipelineStateCacheManager&) = delete;

		GraphicsPipelineStateCache* getFallbackGraphicsPipelineStateCache(uint32_t serializedGraphicsPipelineStateHash, const ShaderProperties& shaderProperties);

		//[-------------------------------------------------------]
		//[ Pipeline state object cache                           ]
		//[-------------------------------------------------------]
		void loadPipelineStateObjectCache(IFile& file);

		[[nodiscard]] inline bool doesPipelineStateObjectCacheNeedSaving() const
		{
			return mPipelineStateObjectCacheNeedSaving;
		}

		void savePipelineStateObjectCache(IFile& file);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<GraphicsPipelineStateSignatureId, GraphicsPipelineStateCache*> GraphicsPipelineStateCacheByGraphicsPipelineStateSignatureId;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialBlueprintResource&									 mMaterialBlueprintResource;			// Owner material blueprint resource
		GraphicsProgramCacheManager									 mGraphicsProgramCacheManager;
		GraphicsPipelineStateCacheByGraphicsPipelineStateSignatureId mGraphicsPipelineStateCacheByGraphicsPipelineStateSignatureId;
		bool														 mPipelineStateObjectCacheNeedSaving;	// "true" if a cache needs saving due to changes during runtime, else "false"

		// Temporary instances to reduce the number of memory allocations/deallocations
		GraphicsPipelineStateSignature mTemporaryGraphicsPipelineStateSignature;
		ShaderProperties			   mFallbackShaderProperties;
		GraphicsPipelineStateSignature mFallbackGraphicsPipelineStateSignature;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer


// Reset warning manipulations we did above
SE_PRAGMA_WARNING_POP

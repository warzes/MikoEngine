#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/ResourceManager.h"
#include "Renderer/Resource/ShaderBlueprint/Cache/ShaderProperties.h"
#include "Renderer/Resource/ShaderBlueprint/Cache/ShaderCacheManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IFile;
	class IRenderer;
	class ShaderBlueprintResource;
	class ShaderBlueprintResourceLoader;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t ShaderBlueprintResourceId;	// POD shader blueprint resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader blueprint resource manager
	*/
	class ShaderBlueprintResourceManager final : public ResourceManager<ShaderBlueprintResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		/**
		*  @brief
		*    Return the RHI shader properties
		*
		*  @return
		*    The RHI shader properties
		*
		*  @note
		*    - The RHI shader properties depend on the current RHI implementation, contains e.g. "OpenGL", "GLSL", "ZeroToOneClipZ", "UpperLeftOrigin" etc.
		*    - The RHI shader properties are added during shader source code building and hence are not part of the pipeline state signature
		*/
		[[nodiscard]] inline const ShaderProperties& getRhiShaderProperties() const
		{
			return mRhiShaderProperties;
		}

		void loadShaderBlueprintResourceByAssetId(AssetId assetId, ShaderBlueprintResourceId& shaderBlueprintResourceId, IResourceListener* resourceListener = nullptr, bool reload = false, ResourceLoaderTypeId resourceLoaderTypeId = GetInvalid<ResourceLoaderTypeId>());	// Asynchronous
		void setInvalidResourceId(ShaderBlueprintResourceId& shaderBlueprintResourceId, IResourceListener& resourceListener) const;

		/**
		*  @brief
		*    Return the shader cache manager
		*
		*  @return
		*    The shader cache manager
		*/
		[[nodiscard]] inline ShaderCacheManager& getShaderCacheManager()
		{
			return mShaderCacheManager;
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual uint32_t getNumberOfResources() const override;
		[[nodiscard]] virtual IResource& getResourceByIndex(uint32_t index) const override;
		[[nodiscard]] virtual IResource& getResourceByResourceId(ResourceId resourceId) const override;
		[[nodiscard]] virtual IResource* tryGetResourceByResourceId(ResourceId resourceId) const override;
		virtual void reloadResourceByAssetId(AssetId assetId) override;

		inline virtual void update() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] virtual IResourceLoader* createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit ShaderBlueprintResourceManager(IRenderer& renderer);
		virtual ~ShaderBlueprintResourceManager() override;
		explicit ShaderBlueprintResourceManager(const ShaderBlueprintResourceManager&) = delete;
		ShaderBlueprintResourceManager& operator=(const ShaderBlueprintResourceManager&) = delete;

		//[-------------------------------------------------------]
		//[ Pipeline state object cache                           ]
		//[-------------------------------------------------------]
		inline void clearPipelineStateObjectCache()
		{
			mShaderCacheManager.clearCache();
		}

		inline void loadPipelineStateObjectCache(IFile& file)
		{
			mShaderCacheManager.loadCache(file);
		}

		[[nodiscard]] inline bool doesPipelineStateObjectCacheNeedSaving() const
		{
			return mShaderCacheManager.doesCacheNeedSaving();
		}

		inline void savePipelineStateObjectCache(IFile& file)
		{
			mShaderCacheManager.saveCache(file);
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&		   mRenderer;
		ShaderProperties   mRhiShaderProperties;
		ShaderCacheManager mShaderCacheManager;

		// Internal resource manager implementation
		ResourceManagerTemplate<ShaderBlueprintResource, ShaderBlueprintResourceLoader, ShaderBlueprintResourceId, 64>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

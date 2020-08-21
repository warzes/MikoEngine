#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResourceLoader.h"
#include "Renderer/Core/File/MemoryFile.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IRenderer;
	class ShaderBlueprintResource;
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
	class ShaderBlueprintResourceLoader final : public IResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend ResourceManagerTemplate<ShaderBlueprintResource, ShaderBlueprintResourceLoader, ShaderBlueprintResourceId, 64>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("shader_blueprint");


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		virtual void initialize(const Asset& asset, bool reload, IResource& resource) override;

		[[nodiscard]] inline virtual bool hasDeserialization() const override
		{
			return true;
		}

		[[nodiscard]] virtual bool onDeserialization(IFile& file) override;

		[[nodiscard]] inline virtual bool hasProcessing() const override
		{
			return true;
		}

		virtual void onProcessing() override;
		[[nodiscard]] virtual bool onDispatch() override;
		[[nodiscard]] virtual bool isFullyLoaded() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline ShaderBlueprintResourceLoader(IResourceManager& resourceManager, IRenderer& renderer) :
			IResourceLoader(resourceManager),
			mRenderer(renderer),
			mShaderBlueprintResource(nullptr),
			mMaximumNumberOfIncludeShaderPieceAssetIds(0),
			mIncludeShaderPieceAssetIds(nullptr),
			mMaximumNumberOfShaderSourceCodeBytes(0),
			mShaderSourceCode(nullptr)
		{
			// Nothing here
		}

		inline virtual ~ShaderBlueprintResourceLoader() override
		{
			// Free temporary data
			delete [] mIncludeShaderPieceAssetIds;
			delete [] mShaderSourceCode;
		}

		explicit ShaderBlueprintResourceLoader(const ShaderBlueprintResourceLoader&) = delete;
		ShaderBlueprintResourceLoader& operator=(const ShaderBlueprintResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&				 mRenderer;					// Renderer instance, do not destroy the instance
		ShaderBlueprintResource* mShaderBlueprintResource;	// Destination resource

		// Temporary data
		MemoryFile mMemoryFile;
		uint32_t   mMaximumNumberOfIncludeShaderPieceAssetIds;
		AssetId*   mIncludeShaderPieceAssetIds;
		uint32_t   mMaximumNumberOfShaderSourceCodeBytes;
		char*	   mShaderSourceCode;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

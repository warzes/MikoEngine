#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResourceLoader.h"
#include "Core/File/MemoryFile.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IRenderer;
	class MaterialResource;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
	namespace v1Material
	{
		struct Technique;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialBlueprintResourceId;	// POD material blueprint resource identifier


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialResourceId;	// POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class MaterialResourceLoader final : public IResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend ResourceManagerTemplate<MaterialResource, MaterialResourceLoader, MaterialResourceId, 4096>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = SE_STRING_ID("material");


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
		inline MaterialResourceLoader(IResourceManager& resourceManager, IRenderer& renderer) :
			IResourceLoader(resourceManager),
			mRenderer(renderer),
			mMaterialResource(nullptr),
			mMaximumNumberOfMaterialTechniques(0),
			mNumberOfTechniques(0),
			mMaterialTechniques(nullptr)
		{
			// Nothing here
		}

		virtual ~MaterialResourceLoader() override;
		explicit MaterialResourceLoader(const MaterialResourceLoader&) = delete;
		MaterialResourceLoader& operator=(const MaterialResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<MaterialBlueprintResourceId> MaterialBlueprintResourceIds;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&		  mRenderer;			// Renderer instance, do not destroy the instance
		MaterialResource* mMaterialResource;	// Destination resource

		// Temporary data
		MemoryFile mMemoryFile;

		// Temporary data: Techniques
		uint32_t					 mMaximumNumberOfMaterialTechniques;
		uint32_t					 mNumberOfTechniques;
		v1Material::Technique*		 mMaterialTechniques;
		MaterialBlueprintResourceIds mMaterialBlueprintResourceIds;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

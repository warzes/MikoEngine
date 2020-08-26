#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResource.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'argument': conversion from 'long' to 'unsigned int', signed/unsigned mismatch
	SE_PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
	SE_PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <vector>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	template <class ELEMENT_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class PackedElementManager;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
	class CompositorWorkspaceResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t CompositorWorkspaceResourceId;	// POD compositor workspace resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorWorkspaceResource final : public IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorWorkspaceResourceLoader;
		friend PackedElementManager<CompositorWorkspaceResource, CompositorWorkspaceResourceId, 32>;										// Type definition of template class
		friend ResourceManagerTemplate<CompositorWorkspaceResource, CompositorWorkspaceResourceLoader, CompositorWorkspaceResourceId, 32>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<AssetId> CompositorNodeAssetIds;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline void reserveCompositorNodes(uint32_t numberOfCompositorNodes)
		{
			mCompositorNodeAssetIds.reserve(numberOfCompositorNodes);
		}

		inline void addCompositorNode(AssetId compositorNodeAssetId)
		{
			mCompositorNodeAssetIds.push_back(compositorNodeAssetId);
		}

		[[nodiscard]] inline const CompositorNodeAssetIds& getCompositorNodeAssetIds() const
		{
			return mCompositorNodeAssetIds;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline CompositorWorkspaceResource()
		{
			// Nothing here
		}

		inline virtual ~CompositorWorkspaceResource() override
		{
			// Sanity check
			SE_ASSERT(mCompositorNodeAssetIds.empty(), "Invalid compositor node asset IDs")
		}

		explicit CompositorWorkspaceResource(const CompositorWorkspaceResource&) = delete;
		CompositorWorkspaceResource& operator=(const CompositorWorkspaceResource&) = delete;

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(CompositorWorkspaceResourceId compositorWorkspaceResourceId)
		{
			// Sanity check
			SE_ASSERT(mCompositorNodeAssetIds.empty(), "Invalid compositor node asset IDs")

			// Call base implementation
			IResource::initializeElement(compositorWorkspaceResourceId);
		}

		inline void deinitializeElement()
		{
			mCompositorNodeAssetIds.clear();

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CompositorNodeAssetIds mCompositorNodeAssetIds;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

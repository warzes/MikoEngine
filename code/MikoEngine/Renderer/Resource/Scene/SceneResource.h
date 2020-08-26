#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResource.h"
#include "Renderer/Core/Manager.h"

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
	class Transform;
	class SceneNode;
	class ISceneItem;
	class ISceneFactory;
	class IRenderer;
	class SceneCullingManager;
	class SceneResourceLoader;
	template <class ELEMENT_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class PackedElementManager;
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
	typedef uint32_t SceneResourceId;	// POD scene resource identifier
	typedef StringId SceneItemTypeId;	// Scene item type identifier, internally just a POD "uint32_t"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class SceneResource final : public IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneResourceManager;															// Needs to be able to update the scene factory instance
		friend PackedElementManager<SceneResource, SceneResourceId, 16>;							// Type definition of template class
		friend ResourceManagerTemplate<SceneResource, SceneResourceLoader, SceneResourceId, 16>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<SceneNode*> SceneNodes;
		typedef std::vector<ISceneItem*> SceneItems;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] IRenderer& getRenderer() const;

		[[nodiscard]] inline SceneCullingManager& getSceneCullingManager() const
		{
			// We know that this pointer is always valid
			SE_ASSERT(nullptr != mSceneCullingManager, "Invalid scene culling manager")
			return *mSceneCullingManager;
		}

		void destroyAllSceneNodesAndItems();

		//[-------------------------------------------------------]
		//[ Node                                                  ]
		//[-------------------------------------------------------]
		[[nodiscard]] SceneNode* createSceneNode(const Transform& transform);
		void destroySceneNode(SceneNode& sceneNode);
		void destroyAllSceneNodes();

		[[nodiscard]] inline const SceneNodes& getSceneNodes() const
		{
			return mSceneNodes;
		}

		//[-------------------------------------------------------]
		//[ Item                                                  ]
		//[-------------------------------------------------------]
		[[nodiscard]] ISceneItem* createSceneItem(SceneItemTypeId sceneItemTypeId, SceneNode& sceneNode);

		template <typename T>
		[[nodiscard]] inline T* createSceneItem(SceneNode& sceneNode)
		{
			return static_cast<T*>(createSceneItem(T::TYPE_ID, sceneNode));
		}

		void destroySceneItem(ISceneItem& sceneItem);
		void destroyAllSceneItems();

		[[nodiscard]] inline const SceneItems& getSceneItems() const
		{
			return mSceneItems;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline SceneResource() :
			mSceneFactory(nullptr),
			mSceneCullingManager(nullptr)
		{
			// Nothing here
		}

		inline virtual ~SceneResource() override
		{
			// Sanity checks
			SE_ASSERT(nullptr == mSceneFactory, "Invalid scene factory")
			SE_ASSERT(nullptr == mSceneCullingManager, "Invalid scene culling manager")
			SE_ASSERT(mSceneNodes.empty(), "Invalid scene nodes")
			SE_ASSERT(mSceneItems.empty(), "Invalid scene items")
		}

		explicit SceneResource(const SceneResource&) = delete;
		SceneResource& operator=(const SceneResource&) = delete;

		inline SceneResource& operator=(SceneResource&& sceneResource) noexcept
		{
			// Call base implementation
			IResource::operator=(std::move(sceneResource));

			// Swap data
			std::swap(mSceneFactory, sceneResource.mSceneFactory);
			std::swap(mSceneCullingManager, sceneResource.mSceneCullingManager);
			std::swap(mSceneNodes, sceneResource.mSceneNodes);
			std::swap(mSceneItems, sceneResource.mSceneItems);

			// Done
			return *this;
		}

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		void initializeElement(SceneResourceId sceneResourceId);
		void deinitializeElement();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const ISceneFactory* mSceneFactory;			// Scene factory instance, always valid, do not destroy the instance
		SceneCullingManager* mSceneCullingManager;	// Scene culling manager, always valid, destroy the instance if you no longer need it
		SceneNodes			 mSceneNodes;
		SceneItems			 mSceneItems;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#include "stdafx.h"
#include "Renderer/Resource/Scene/SceneResource.h"
#include "Renderer/Resource/Scene/SceneNode.h"
#include "Renderer/Resource/Scene/SceneResourceManager.h"
#include "Renderer/Resource/Scene/Item/ISceneItem.h"
#include "Renderer/Resource/Scene/Factory/ISceneFactory.h"
#include "Renderer/Resource/Scene/Culling/SceneCullingManager.h"
#include "Renderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	IRenderer& SceneResource::getRenderer() const
	{
		return getResourceManager<SceneResourceManager>().getRenderer();
	}

	void SceneResource::destroyAllSceneNodesAndItems()
	{
		destroyAllSceneNodes();
		destroyAllSceneItems();

		// TODO(co) The following is just for culling kickoff and won't stay this way
		// Re-create scene culling manager
		delete mSceneCullingManager;
		mSceneCullingManager = new SceneCullingManager();

		// Update the resource loading state
		setLoadingState(IResource::LoadingState::UNLOADED);
	}

	SceneNode* SceneResource::createSceneNode(const Transform& transform)
	{
		SceneNode* sceneNode = new SceneNode(transform);
		mSceneNodes.push_back(sceneNode);
		return sceneNode;
	}

	void SceneResource::destroySceneNode(SceneNode& sceneNode)
	{
		SceneNodes::iterator iterator = std::find(mSceneNodes.begin(), mSceneNodes.end(), &sceneNode);
		if (iterator != mSceneNodes.end())
		{
			mSceneNodes.erase(iterator);
			delete &sceneNode;
		}
		else
		{
			// TODO(co) Error handling
		}
	}

	void SceneResource::destroyAllSceneNodes()
	{
		const size_t numberOfSceneNodes = mSceneNodes.size();
		for (size_t i = 0; i < numberOfSceneNodes; ++i)
		{
			delete mSceneNodes[i];
		}
		mSceneNodes.clear();
	}

	ISceneItem* SceneResource::createSceneItem(SceneItemTypeId sceneItemTypeId, SceneNode& sceneNode)
	{
		if (nullptr == mSceneFactory)
		{
			mSceneFactory = &getResourceManager<SceneResourceManager>().getSceneFactory();
		}
		RHI_ASSERT(nullptr != mSceneFactory, "Invalid scene factory")
		ISceneItem* sceneItem = mSceneFactory->createSceneItem(sceneItemTypeId, *this);
		if (nullptr != sceneItem)
		{
			sceneNode.attachSceneItem(*sceneItem);
			mSceneItems.push_back(sceneItem);
		}
		else
		{
			// TODO(co) Error handling
		}
		return sceneItem;
	}

	void SceneResource::destroySceneItem(ISceneItem& sceneItem)
	{
		SceneItems::iterator iterator = std::find(mSceneItems.begin(), mSceneItems.end(), &sceneItem);
		if (iterator != mSceneItems.end())
		{
			mSceneItems.erase(iterator);
			delete &sceneItem;
		}
		else
		{
			// TODO(co) Error handling
		}
	}

	void SceneResource::destroyAllSceneItems()
	{
		const size_t numberOfSceneItems = mSceneItems.size();
		for (size_t i = 0; i < numberOfSceneItems; ++i)
		{
			delete mSceneItems[i];
		}
		mSceneItems.clear();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SceneResource::initializeElement(SceneResourceId sceneResourceId)
	{
		// Sanity checks
		RHI_ASSERT(nullptr == mSceneFactory, "Invalid scene factory")
		RHI_ASSERT(nullptr == mSceneCullingManager, "Invalid scene culling manager")
		RHI_ASSERT(mSceneNodes.empty(), "Invalid scene nodes")
		RHI_ASSERT(mSceneItems.empty(), "Invalid scene items")

		// Create scene culling manager
		mSceneCullingManager = new SceneCullingManager();

		// Call base implementation
		IResource::initializeElement(sceneResourceId);
	}

	void SceneResource::deinitializeElement()
	{
		// Reset everything
		destroyAllSceneNodesAndItems();
		mSceneFactory = nullptr;
		delete mSceneCullingManager;
		mSceneCullingManager = nullptr;

		// Call base implementation
		IResource::deinitializeElement();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/StringId.h"
#include "RHI/Rhi.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class Context;
	class SceneNode;
	struct SceneItemSet;
	class SceneResource;
	class RenderableManager;
	class CompositorContextData;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId SceneItemTypeId;	// Scene item type identifier, internally just a POD "uint32_t"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class ISceneItem
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneNode;						// TODO(co) Remove this
		friend class MeshSceneItem;					// TODO(co) Remove this
		friend class SceneResource;					// Needs to be able to destroy scene items
		friend class CompositorWorkspaceInstance;	// Needs to be able to call "Renderer::ISceneItem::onExecuteOnRendering()"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] Context& getContext() const;

		[[nodiscard]] inline SceneResource& getSceneResource() const
		{
			return mSceneResource;
		}

		[[nodiscard]] inline bool hasParentSceneNode() const
		{
			return (nullptr != mParentSceneNode);
		}

		[[nodiscard]] inline SceneNode* getParentSceneNode()
		{
			return mParentSceneNode;
		}

		[[nodiscard]] inline const SceneNode* getParentSceneNode() const
		{
			return mParentSceneNode;
		}

		[[nodiscard]] inline SceneNode& getParentSceneNodeSafe()	// Just safe in context known as safe
		{
			RHI_ASSERT(nullptr != mParentSceneNode, "Invalid parent scene node")
			return *mParentSceneNode;
		}

		[[nodiscard]] inline const SceneNode& getParentSceneNodeSafe() const	// Just safe in context known as safe
		{
			RHI_ASSERT(nullptr != mParentSceneNode, "Invalid parent scene node")
			return *mParentSceneNode;
		}

		[[nodiscard]] inline bool getCallExecuteOnRendering() const
		{
			return mCallExecuteOnRendering;
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual SceneItemTypeId getSceneItemTypeId() const = 0;
		virtual void deserialize(uint32_t numberOfBytes, const uint8_t* data) = 0;

		inline virtual void onAttachedToSceneNode(SceneNode& sceneNode)
		{
			RHI_ASSERT(nullptr == mParentSceneNode, "Invalid parent scene node")
			mParentSceneNode = &sceneNode;
		}

		inline virtual void onDetachedFromSceneNode([[maybe_unused]] SceneNode& sceneNode)
		{
			RHI_ASSERT(nullptr != mParentSceneNode, "Invalid parent scene node")
			mParentSceneNode = nullptr;
		}

		inline virtual void setVisible([[maybe_unused]] bool visible)
		{
			// Nothing here
		}

		[[nodiscard]] inline virtual const RenderableManager* getRenderableManager() const
		{
			return nullptr;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ISceneItem methods        ]
	//[-------------------------------------------------------]
	protected:
		// Only called if "Renderer::ISceneItem::getCallExecuteOnRendering()" returns "true", the default implementation is empty and shouldn't be called
		[[nodiscard]] inline virtual void onExecuteOnRendering([[maybe_unused]] const Rhi::IRenderTarget& renderTarget, [[maybe_unused]] const CompositorContextData& compositorContextData, [[maybe_unused]] Rhi::CommandBuffer& commandBuffer) const
		{
			RHI_ASSERT(true, "Don't call the base implementation of \"Renderer::ISceneItem::getCallExecuteOnRendering()\"")
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		ISceneItem(SceneResource& sceneResource, bool cullable = true);
		virtual ~ISceneItem();
		explicit ISceneItem(const ISceneItem&) = delete;
		ISceneItem& operator=(const ISceneItem&) = delete;

		inline void setCallExecuteOnRendering(bool callExecuteOnRendering)
		{
			mCallExecuteOnRendering = callExecuteOnRendering;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SceneResource& mSceneResource;
		SceneNode*	   mParentSceneNode;		// Parent scene node, can be a null pointer, don't destroy the instance
		SceneItemSet*  mSceneItemSet;			// Scene item set, always valid, don't destroy the instance
		uint32_t	   mSceneItemSetIndex;		// Index inside the scene item set
		bool		   mCallExecuteOnRendering;	// Call execute on rendering? ("Renderer::ISceneItem::onExecuteOnRendering()") Keep this disabled if not needed not waste performance.


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

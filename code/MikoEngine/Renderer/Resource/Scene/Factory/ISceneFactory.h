#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/StringId.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class ISceneItem;
	class SceneResource;
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
	class ISceneFactory
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneResource;	// Needs to create scene node and scene item instances


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ISceneFactory methods     ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual ISceneItem* createSceneItem(const SceneItemTypeId& sceneItemTypeId, SceneResource& sceneResource) const = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline ISceneFactory()
		{
			// Nothing here
		}

		inline virtual ~ISceneFactory()
		{
			// Nothing here
		}

		explicit ISceneFactory(const ISceneFactory&) = delete;
		ISceneFactory& operator=(const ISceneFactory&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

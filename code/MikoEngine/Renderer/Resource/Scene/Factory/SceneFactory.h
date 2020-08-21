#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]

#include "Renderer/Resource/Scene/Factory/ISceneFactory.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class SceneFactory final : public ISceneFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline SceneFactory()
		{
			// Nothing here
		}

		inline virtual ~SceneFactory() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ISceneFactory methods     ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual ISceneItem* createSceneItem(const SceneItemTypeId& sceneItemTypeId, SceneResource& sceneResource) const override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		explicit SceneFactory(const SceneFactory&) = delete;
		SceneFactory& operator=(const SceneFactory&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

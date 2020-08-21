#include "stdafx.h"
#include "Renderer/Resource/Scene/Factory/SceneFactory.h"
#include "Renderer/Resource/Scene/Item/Sky/SkySceneItem.h"
#include "Renderer/Resource/Scene/Item/Volume/VolumeSceneItem.h"
#include "Renderer/Resource/Scene/Item/Grass/GrassSceneItem.h"
#include "Renderer/Resource/Scene/Item/Terrain/TerrainSceneItem.h"
#include "Renderer/Resource/Scene/Item/Camera/CameraSceneItem.h"
#include "Renderer/Resource/Scene/Item/Light/SunlightSceneItem.h"
#include "Renderer/Resource/Scene/Item/Particles/ParticlesSceneItem.h"
#include "Renderer/Resource/Scene/Item/Mesh/SkeletonMeshSceneItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ISceneFactory methods     ]
	//[-------------------------------------------------------]
	ISceneItem* SceneFactory::createSceneItem(const SceneItemTypeId& sceneItemTypeId, SceneResource& sceneResource) const
	{
		ISceneItem* sceneItem = nullptr;

		// Define helper macro
		#define CASE_VALUE(name) case name::TYPE_ID: sceneItem = new name(sceneResource); break;

		// Evaluate the scene item type, sorted by usual frequency
		switch (sceneItemTypeId)
		{
			CASE_VALUE(MeshSceneItem)
			CASE_VALUE(LightSceneItem)
			CASE_VALUE(SkeletonMeshSceneItem)
			CASE_VALUE(ParticlesSceneItem)
			CASE_VALUE(CameraSceneItem)
			CASE_VALUE(SunlightSceneItem)
			CASE_VALUE(SkySceneItem)
			CASE_VALUE(VolumeSceneItem)
			CASE_VALUE(GrassSceneItem)
			CASE_VALUE(TerrainSceneItem)
		}

		// Undefine helper macro
		#undef CASE_VALUE

		// Done
		return sceneItem;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

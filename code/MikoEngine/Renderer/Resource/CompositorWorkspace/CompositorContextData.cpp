#include "stdafx.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorContextData.h"
#include "Renderer/Resource/Scene/Item/Camera/CameraSceneItem.h"
#include "Renderer/Resource/Scene/SceneNode.h"
#include "Renderer/Core/Math/Math.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CompositorContextData::CompositorContextData(const CompositorWorkspaceInstance* compositorWorkspaceInstance, const CameraSceneItem* cameraSceneItem, bool singlePassStereoInstancing, const LightSceneItem* lightSceneItem, const CompositorInstancePassShadowMap* compositorInstancePassShadowMap) :
		mCompositorWorkspaceInstance(compositorWorkspaceInstance),
		mCameraSceneItem(cameraSceneItem),
		mSinglePassStereoInstancing(singlePassStereoInstancing),
		mLightSceneItem(lightSceneItem),
		mCompositorInstancePassShadowMap(compositorInstancePassShadowMap),
		mWorldSpaceCameraPosition((nullptr != cameraSceneItem) ? cameraSceneItem->getWorldSpaceCameraPosition() : Math::DVEC3_ZERO),
		mCurrentlyBoundMaterialBlueprintResource(nullptr),
		mGlobalComputeSize{0, 0, 0}
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

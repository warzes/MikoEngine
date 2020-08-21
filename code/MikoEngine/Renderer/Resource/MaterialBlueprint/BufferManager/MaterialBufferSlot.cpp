#include "stdafx.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/MaterialBufferSlot.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/Material/MaterialResource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MaterialBufferSlot::MaterialBufferSlot(MaterialResource& materialResource) :
		mMaterialResourceManager(&materialResource.getResourceManager<MaterialResourceManager>()),
		mMaterialResourceId(materialResource.getId()),
		mAssignedMaterialPool(nullptr),
		mAssignedMaterialSlot(getInvalid<uint32_t>()),
		mGlobalIndex(getInvalid<int>()),
		mDirty(false)
	{
		// Nothing here
	}

	const MaterialResource& MaterialBufferSlot::getMaterialResource() const
	{
		return mMaterialResourceManager->getById(mMaterialResourceId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

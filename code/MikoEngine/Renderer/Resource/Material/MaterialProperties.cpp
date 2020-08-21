#include "stdafx.h"
#include "Renderer/Resource/Material/MaterialProperties.h"

#include <algorithm>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	const MaterialProperty* MaterialProperties::getPropertyById(MaterialPropertyId materialPropertyId) const
	{
		SortedPropertyVector::const_iterator iterator = std::lower_bound(mSortedPropertyVector.cbegin(), mSortedPropertyVector.cend(), materialPropertyId, detail::OrderByMaterialPropertyId());
		return (iterator != mSortedPropertyVector.end() && iterator->getMaterialPropertyId() == materialPropertyId) ? &(*iterator) : nullptr;
	}

	MaterialProperty* MaterialProperties::setPropertyById(MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, MaterialProperty::Usage materialPropertyUsage, bool changeOverwrittenState)
	{
		// Check whether or not this is a new property or a property value change
		SortedPropertyVector::iterator iterator = std::lower_bound(mSortedPropertyVector.begin(), mSortedPropertyVector.end(), materialPropertyId, detail::OrderByMaterialPropertyId());
		if (iterator == mSortedPropertyVector.end() || iterator->getMaterialPropertyId() != materialPropertyId)
		{
			// Add new material property
			iterator = mSortedPropertyVector.insert(iterator, MaterialProperty(materialPropertyId, materialPropertyUsage, materialPropertyValue));
			if (changeOverwrittenState)
			{
				MaterialProperty* materialProperty = &*iterator;
				materialProperty->mOverwritten = true;
				return materialProperty;
			}
			if (MaterialProperty::Usage::SHADER_COMBINATION == materialPropertyUsage)
			{
				++mShaderCombinationGenerationCounter;
			}
			return &*iterator;
		}

		// Update the material property value, in case there's a material property value change
		else if (*iterator != materialPropertyValue)
		{
			// Sanity checks
			RHI_ASSERT(iterator->getValueType() == materialPropertyValue.getValueType(), "Invalid value type")
			RHI_ASSERT(MaterialProperty::Usage::UNKNOWN == materialPropertyUsage || materialPropertyUsage == iterator->getUsage(), "Invalid usage")

			// Update the material property value
			materialPropertyUsage = iterator->getUsage();
			*iterator = MaterialProperty(materialPropertyId, materialPropertyUsage, materialPropertyValue);
			if (MaterialProperty::Usage::SHADER_COMBINATION == materialPropertyUsage)
			{
				++mShaderCombinationGenerationCounter;
			}

			// Material property change detected
			if (changeOverwrittenState)
			{
				MaterialProperty* materialProperty = &*iterator;
				materialProperty->mOverwritten = true;
				return materialProperty;
			}
			else
			{
				return &*iterator;
			}
		}

		// No material property change detected
		return nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

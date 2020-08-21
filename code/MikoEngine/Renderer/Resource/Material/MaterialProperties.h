#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Material/MaterialProperty.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material properties
	*/
	class MaterialProperties final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderPieceResourceLoader;		// TODO(co) Remove this as soon as the incremental hot reloading cache update has been implemented
		friend class ShaderBlueprintResourceLoader;	// TODO(co) Remove this as soon as the incremental hot reloading cache update has been implemented


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<MaterialProperty> SortedPropertyVector;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline MaterialProperties() :
			mShaderCombinationGenerationCounter(0u)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~MaterialProperties()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Return the properties
		*
		*  @return
		*    The properties
		*/
		[[nodiscard]] inline const SortedPropertyVector& getSortedPropertyVector() const
		{
			return mSortedPropertyVector;
		}

		/**
		*  @brief
		*    Remove all material properties
		*/
		inline void removeAllProperties()
		{
			mSortedPropertyVector.clear();
			mShaderCombinationGenerationCounter = 0;
		}

		/**
		*  @brief
		*    Return a material property by its ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to return
		*
		*  @return
		*    The requested material property, null pointer on error, don't destroy the returned instance
		*/
		[[nodiscard]] const MaterialProperty* getPropertyById(MaterialPropertyId materialPropertyId) const;

		/**
		*  @brief
		*    Set a material property value by its ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value from
		*  @param[in] materialPropertyValue
		*    The material property value to set
		*  @param[in] materialPropertyUsage
		*    The material property usage
		*  @param[in] changeOverwrittenState
		*    Change overwritten state?
		*
		*  @return
		*    Pointer to the added or changed property, null pointer if no material property change has been detected, don't destroy the returned instance
		*/
		MaterialProperty* setPropertyById(MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, MaterialProperty::Usage materialPropertyUsage = MaterialProperty::Usage::UNKNOWN, bool changeOverwrittenState = false);

		/**
		*  @brief
		*    Return the shader combination generation counter
		*
		*  @return
		*    The shader combination generation counter
		*/
		[[nodiscard]] inline uint32_t getShaderCombinationGenerationCounter() const
		{
			return mShaderCombinationGenerationCounter;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SortedPropertyVector mSortedPropertyVector;
		uint32_t			 mShaderCombinationGenerationCounter;	// Incremented each time a shader combination material property gets changed, used for e.g. performance critical renderable cache updates were hashes would be too expensive to generate


	};

	namespace detail
	{
		struct OrderByMaterialPropertyId final
		{
			[[nodiscard]] inline bool operator()(const MaterialProperty& left, MaterialPropertyId right) const
			{
				return (left.getMaterialPropertyId() < right);
			}

			[[nodiscard]] inline bool operator()(MaterialPropertyId left, const MaterialProperty& right) const
			{
				return (left < right.getMaterialPropertyId());
			}
		};
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]

#include "Renderer/Core/StringId.h"
#include "Renderer/Core/GetInvalid.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'argument': conversion from 'long' to 'unsigned int', signed/unsigned mismatch
	SE_PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
	SE_PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <vector>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId ShaderPropertyId;	// Shader property identifier, internally just a POD "uint32_t", result of hashing the property name


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader properties
	*/
	class ShaderProperties final
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct Property final
		{
			ShaderPropertyId shaderPropertyId;
			int32_t			 value;

			inline Property() :
				shaderPropertyId(getInvalid<ShaderPropertyId>()),
				value(getInvalid<int32_t>())
			{
				// Nothing here
			}

			inline Property(ShaderPropertyId _shaderPropertyId, int32_t _value) :
				shaderPropertyId(_shaderPropertyId),
				value(_value)
			{
				// Nothing here
			}

			[[nodiscard]] inline bool operator ==(const Property& property) const
			{
				return (shaderPropertyId == property.shaderPropertyId && value == property.value);
			}
		};

		typedef std::vector<Property> SortedPropertyVector;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline ShaderProperties()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor with memory reserve (doesn't add properties, property set is still initially empty)
		*
		*  @param[in] numberOfPropertiesToReserve
		*    Number of properties to reserve
		*/
		inline explicit ShaderProperties(size_t numberOfPropertiesToReserve)
		{
			mSortedPropertyVector.reserve(numberOfPropertiesToReserve);
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ShaderProperties()
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

		[[nodiscard]] inline SortedPropertyVector& getSortedPropertyVector()
		{
			return mSortedPropertyVector;
		}

		/**
		*  @brief
		*    Remove all shader properties
		*/
		inline void clear()
		{
			mSortedPropertyVector.clear();
		}

		/**
		*  @brief
		*    Return whether or not a shader property exists
		*
		*  @param[in] shaderPropertyId
		*    ID of the shader property to check for
		*
		*  @return
		*    "true" if the requested shader property exists, else "false" if the requested shader property doesn't exist
		*/
		[[nodiscard]] bool hasPropertyValue(ShaderPropertyId shaderPropertyId) const;

		/**
		*  @brief
		*    Return the value of a shader property
		*
		*  @param[in] shaderPropertyId
		*    ID of the shader property to return the value from
		*  @param[out] value
		*    Receives the property value
		*  @param[in] defaultValue
		*    Default value in case the shader property doesn't exist
		*
		*  @return
		*    "true" if the requested shader property exists, else "false" if the requested shader property doesn't exist and the default value was returned instead
		*/
		bool getPropertyValue(ShaderPropertyId shaderPropertyId, int32_t& value, int32_t defaultValue = 0) const;

		/**
		*  @brief
		*    Return the value of a shader property
		*
		*  @param[in] shaderPropertyId
		*    ID of the shader property to return the value from
		*  @param[in] defaultValue
		*    Default value in case the shader property doesn't exist
		*
		*  @return
		*    The property value or the default value
		*
		*  @note
		*    - Unsafe because one can't figure out by the return value whether or not the property exists or if it's just the default value
		*/
		[[nodiscard]] int32_t getPropertyValueUnsafe(ShaderPropertyId shaderPropertyId, int32_t defaultValue = 0) const;

		/**
		*  @brief
		*    Set the value of a shader property
		*
		*  @param[in] shaderPropertyId
		*    ID of the shader property to set the value of
		*  @param[in] value
		*    The shader property value to set
		*/
		void setPropertyValue(ShaderPropertyId shaderPropertyId, int32_t value);

		/**
		*  @brief
		*    Set property values by using a given shader properties instance
		*
		*  @param[in] shaderProperties
		*    Shader properties instance to set the shader properties from
		*/
		void setPropertyValues(const ShaderProperties& shaderProperties);

		[[nodiscard]] bool operator ==(const ShaderProperties& shaderProperties) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SortedPropertyVector mSortedPropertyVector;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

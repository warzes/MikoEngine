#pragma once

#include "Utility/StringId.h"
#include "Core/GetInvalid.h"

namespace Renderer
{
	typedef StringId ShaderPropertyId;	// Shader property identifier, internally just a POD "uint32_t", result of hashing the property name

	class ShaderProperties final
	{
	public:
		struct Property final
		{
			ShaderPropertyId shaderPropertyId;
			int32_t			 value;

			inline Property() :
				shaderPropertyId(GetInvalid<ShaderPropertyId>()),
				value(GetInvalid<int32_t>())
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

		inline ShaderProperties()
		{
		}

		// Constructor with memory reserve (doesn't add properties, property set is still initially empty)
		inline explicit ShaderProperties(size_t numberOfPropertiesToReserve)
		{
			mSortedPropertyVector.reserve(numberOfPropertiesToReserve);
		}

		inline ~ShaderProperties()
		{
			// Nothing here
		}

		[[nodiscard]] inline const SortedPropertyVector& getSortedPropertyVector() const
		{
			return mSortedPropertyVector;
		}

		[[nodiscard]] inline SortedPropertyVector& getSortedPropertyVector()
		{
			return mSortedPropertyVector;
		}

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

	private:
		SortedPropertyVector mSortedPropertyVector;
	};
} // Renderer
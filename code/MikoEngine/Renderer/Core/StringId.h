#pragma once

#include "Core/AssertMacros.h"

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
	*    Compile time string ID class generating an identifier by hashing a given string
	*
	*  @remarks
	*    The following example shows how to use the string ID class:
	*    @code
	*    uint32_t id = STRING_ID("Example/Mesh/Default/Squirrel");	// Result will be 2906231359
	*    @endcode
	*
	*  @note
	*    - Is using compile-time string hashing as described within the Gamasutra article "In-Depth: Quasi Compile-Time String Hashing"
	*      by Stefan Reinalter ( http://www.gamasutra.com/view/news/38198/InDepth_Quasi_CompileTime_String_Hashing.php#.UG1MpVFQbq4 )
	*    - As hash function FNV-1a is used ( http://isthe.com/chongo/tech/comp/fnv/ )
	*/
	class StringId final
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t FNV1a_INITIAL_HASH_32  = 0xcbf29ce4u;
		static constexpr uint32_t FNV1a_MAGIC_PRIME_32	 = 0x1000193u;
		static constexpr uint32_t MAXIMUM_UINT32_T_VALUE = 4294967295u;	// We don't want to include "<limits>" in this lightweight core header just to be able to use "std::numeric_limits<uint32_t>::max()"

		/**
		*  @brief
		*    "const char*"-wrapper enabling the compiler to distinguish between static and dynamic strings
		*/
		struct ConstCharWrapper final
		{
			/**
			*  @brief
			*    "const char*"-wrapper constructor
			*
			*  @param[in] string
			*    Null terminated ASCII string to calculate the hash value for, must be valid
			*
			*  @note
			*    - Not explicit by intent
			*/
			inline ConstCharWrapper(const char* string) :
				mString(string)
			{
				// Nothing here
			}

			const char* mString;	// Null terminated ASCII string to calculate the hash value for, must be valid
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Calculate the hash value of the given string at compile time
		*
		*  @param[in] string
		*    Null terminated ASCII string to calculate the hash value for, must be valid
		*  @param[in] value
		*    Initial hash
		*
		*  @return
		*    The hash value of the given string
		*/
		[[nodiscard]] static constexpr inline uint32_t compileTimeFNV(const char* string, const uint32_t value = FNV1a_INITIAL_HASH_32) noexcept
		{
			// 32-bit FNV-1a implementation basing on http://www.isthe.com/chongo/tech/comp/fnv/
			return ('\0' == string[0]) ? value : compileTimeFNV(&string[1], (value ^ static_cast<uint32_t>(string[0])) * FNV1a_MAGIC_PRIME_32);
		}

		/**
		*  @brief
		*    Calculate the hash value of the given string
		*
		*  @param[in] string
		*    Null terminated ASCII string to calculate the hash value for, must be valid
		*
		*  @return
		*    The hash value of the given string
		*/
		[[nodiscard]] static inline uint32_t calculateFNV(const char* string)
		{
			// Sanity check
			SE_ASSERT(nullptr != string, "The string must be valid to be able to calculate a FNV1a32 hash")

			// 32-bit FNV-1a implementation basing on http://www.isthe.com/chongo/tech/comp/fnv/
			uint32_t hash = FNV1a_INITIAL_HASH_32;
			for ( ; '\0' != *string; ++string)
			{
				hash = (hash ^ *string) * FNV1a_MAGIC_PRIME_32;
			}
			return hash;
		}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline StringId() :
			mId(MAXIMUM_UINT32_T_VALUE)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor for calculating the hash value of a given static string
		*
		*  @param[in] string
		*    Static string to calculate the hash value for, must be valid
		*/
		template <uint32_t N>
		inline StringId(const char (&string)[N]) noexcept :
			mId(compileTimeFNV(string))
		{
			// It's a trap!
			#ifdef _WIN32
				static_assert(false, "Use the \"STRING_ID()\" macro to mark compile string IDs");
			#endif
		}

		/**
		*  @brief
		*    Constructor for calculating the hash value of a given dynamic string
		*
		*  @param[in] string
		*    Dynamic string to calculate the hash value for, must be valid
		*/
		inline explicit StringId(const ConstCharWrapper& string) :
			mId(calculateFNV(string.mString))
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor for directly setting an uint32_t value as string ID
		*
		*  @param[in] id
		*    ID value to set
		*
		*  @note
		*    - By intent not explicit for best possible usability
		*/
		inline StringId(uint32_t id) :
			mId(id)
		{
			// Nothing do to in here
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] stringId
		*    String ID instance to copy from
		*
		*  @remarks
		*    This constructor is by intent not explicit because otherwise GCC 4.9 throws an error when "Renderer::StringId" is used
		*    e.g. as a function parameter and the function is called with a string literal. Example:
		*    "
		*      typedef StringId AssetId;
		*      void function(StringId assetId) {}
		*      void functionAssetId(AssetId assetId) {}
		*    "
		*    Function call results:
		*    - function("BlaBlub"); <-- GCC 4.9 error: no matching function for call to "Renderer::StringId::StringId(Renderer::StringId)"
		*    - functionAssetId("BlaBlub"); <-- GCC 4.9 error: no matching function for call to "Renderer::StringId::StringId(AssetId)"
		*/
		inline StringId(const StringId& stringId) :
			mId(stringId.mId)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Return the generated ID
		*
		*  @return
		*    The generated FNV-1a hash value used as identifier
		*/
		[[nodiscard]] inline uint32_t getId() const
		{
			return mId;
		}

		/**
		*  @brief
		*    Return the generated ID
		*
		*  @return
		*    The generated FNV-1a hash value used as identifier
		*/
		[[nodiscard]] inline operator uint32_t() const
		{
			return mId;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32_t mId;	// The generated FNV-1a hash value which is used as identifier


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer


//[-------------------------------------------------------]
//[ Macros & definitions                                  ]
//[-------------------------------------------------------]
#if _MSC_VER
	// Disable warnings in external headers, we can't fix them: warning C4307: '*': integral constant overflow
	/**
	*  @brief
	*    Compile time string ID macro
	*/
	#define STRING_ID(string) \
			std::integral_constant<uint32_t, Renderer::StringId::compileTimeFNV(string)>::value
#else
	/**
	*  @brief
	*    Compile time string ID macro
	*/
	#define STRING_ID(string) std::integral_constant<uint32_t, Renderer::StringId::compileTimeFNV(string)>::value
#endif

/**
*  @brief
*    Compile time asset ID macro; use this alias instead of "STRING_ID()" to be able to easily search for asset references
*/
#define ASSET_ID(string) STRING_ID(string)

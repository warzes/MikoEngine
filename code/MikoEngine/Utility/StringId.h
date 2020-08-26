#pragma once

#include "Core/AssertMacros.h"

/**
*  @brief
*    Compile time string ID class generating an identifier by hashing a given string
*
*  @remarks
*    The following example shows how to use the string ID class:
*    @code
*    uint32_t id = SE_STRING_ID("Example/Mesh/Default/Squirrel");	// Result will be 2906231359
*    @endcode
*
*  @note
*    - Is using compile-time string hashing as described within the Gamasutra article "In-Depth: Quasi Compile-Time String Hashing"
*      by Stefan Reinalter ( http://www.gamasutra.com/view/news/38198/InDepth_Quasi_CompileTime_String_Hashing.php#.UG1MpVFQbq4 )
*    - As hash function FNV-1a is used ( http://isthe.com/chongo/tech/comp/fnv/ )
*/
class StringId final
{
public:
	static constexpr uint32_t FNV1a_INITIAL_HASH_32 = 0xcbf29ce4u;
	static constexpr uint32_t FNV1a_MAGIC_PRIME_32 = 0x1000193u;
	static constexpr uint32_t MAXIMUM_UINT32_T_VALUE = std::numeric_limits<uint32_t>::max();

	// "const char*"-wrapper enabling the compiler to distinguish between static and dynamic strings
	struct ConstCharWrapper final
	{
		inline ConstCharWrapper(const char* string) noexcept : mString(string) {}
		const char* mString; // Null terminated ASCII string to calculate the hash value for, must be valid
	};

	// Calculate the hash value of the given string at compile time
	[[nodiscard]] static constexpr inline uint32_t CompileTimeFNV(const char* string, const uint32_t value = FNV1a_INITIAL_HASH_32) noexcept
	{
		// 32-bit FNV-1a implementation basing on http://www.isthe.com/chongo/tech/comp/fnv/
		return ('\0' == string[0]) ? value : CompileTimeFNV(&string[1], (value ^ static_cast<uint32_t>(string[0])) * FNV1a_MAGIC_PRIME_32);
	}

	// Calculate the hash value of the given string
	[[nodiscard]] static inline uint32_t CalculateFNV(const char* string) noexcept
	{
		// Sanity check
		SE_ASSERT(nullptr != string, "The string must be valid to be able to calculate a FNV1a32 hash");

		// 32-bit FNV-1a implementation basing on http://www.isthe.com/chongo/tech/comp/fnv/
		uint32_t hash = FNV1a_INITIAL_HASH_32;
		for ( ; '\0' != *string; ++string )
		{
			hash = (hash ^ *string) * FNV1a_MAGIC_PRIME_32;
		}
		return hash;
	}

	inline StringId() noexcept : mId(MAXIMUM_UINT32_T_VALUE)
	{
	}

	// Constructor for calculating the hash value of a given static string
	template <uint32_t N>
	inline StringId(const char(&string)[N]) noexcept : mId(CompileTimeFNV(string))
	{
		// It's a trap!
		static_assert(false, "Use the \"SE_STRING_ID()\" macro to mark compile string IDs");
	}

	// Constructor for calculating the hash value of a given dynamic string
	inline explicit StringId(const ConstCharWrapper& string) noexcept : mId(CalculateFNV(string.mString))
	{
	}

	// Constructor for directly setting an uint32_t value as string ID
	inline StringId(uint32_t id) noexcept : mId(id)
	{
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
	inline StringId(const StringId& stringId) noexcept : mId(stringId.mId)
	{
	}

	// The generated FNV-1a hash value used as identifier
	[[nodiscard]] inline uint32_t GetId() const noexcept
	{
		return mId;
	}

	// The generated FNV-1a hash value used as identifier
	[[nodiscard]] inline operator uint32_t() const noexcept
	{
		return mId;
	}

private:
	uint32_t mId;	// The generated FNV-1a hash value which is used as identifier
};

// Compile time string ID macro
#define SE_STRING_ID(string) std::integral_constant<uint32_t, StringId::CompileTimeFNV(string)>::value
// Compile time asset ID macro; use this alias instead of "SE_STRING_ID()" to be able to easily search for asset references
#define SE_ASSET_ID(string) SE_STRING_ID(string)
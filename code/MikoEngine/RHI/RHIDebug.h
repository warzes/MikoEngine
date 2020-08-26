#pragma once

// Debug Name Parameter

#if SE_DEBUG
// Resource name for debugging purposes, ignored when not using "SE_DEBUG"
// debugName - ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
#	define RHI_RESOURCE_DEBUG_NAME_PARAMETER , const char debugName[] = ""
#	define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT , const char debugName[]
// Resource name for debugging purposes, ignored when not using "SE_DEBUG"
#	define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER , [[maybe_unused]] const char debugName[] = ""
#	define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER_NO_DEFAULT , [[maybe_unused]] const char debugName[]
// Pass resource name for debugging purposes, ignored when not using "SE_DEBUG"
#	define RHI_RESOURCE_DEBUG_PASS_PARAMETER , debugName
#else
#	define RHI_RESOURCE_DEBUG_NAME_PARAMETER
#	define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT
#	define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER
#	define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER_NO_DEFAULT
#	define RHI_RESOURCE_DEBUG_PASS_PARAMETER
#endif // SE_DEBUG

// Debug marker

#if SE_DEBUG
#	define RHI_SET_DEBUG_MARKER(rhi, name) if (nullptr != rhi) { (rhi)->setDebugMarker(name); }

	// Set a debug marker by using the current function name ("__FUNCTION__") as marker name
#	define RHI_SET_DEBUG_MARKER_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->setDebugMarker(__FUNCTION__); }

#	define RHI_BEGIN_DEBUG_EVENT(rhi, name) if (nullptr != rhi) { (rhi)->beginDebugEvent(name); }
#	define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->beginDebugEvent(__FUNCTION__); }

#	define RHI_END_DEBUG_EVENT(rhi) if (nullptr != rhi) { (rhi)->endDebugEvent(); }
#	define RHI_RESOURCE_DEBUG_NAME(name) , name

	/**
	*  @brief
	*    Decorate the debug name to make it easier to see the semantic of the resource
	*
	*  @param[in] name
	*    Debug name provided from the outside
	*  @param[in] decoration
	*    Decoration to append in front (e.g. "IBO", will result in appended "IBO: " in front if the provided name isn't empty)
	*  @param[in] numberOfDecorationCharacters
	*    Number of decoration characters
	*
	*  @note
	*    - The result is in local string variable "detailedName"
	*    - Traditional C-string on the runtime stack used for efficiency reasons (just for debugging, but must still be some kind of usable)
	*/
#	define RHI_DECORATED_DEBUG_NAME(name, detailedName, decoration, numberOfDecorationCharacters) \
		SE_ASSERT(strlen(name) < 256, "Name is not allowed to exceed 255 characters"); \
		char detailedName[256 + numberOfDecorationCharacters] = decoration; \
		if (name[0] != '\0') \
		{ \
			strcat(detailedName, ": "); \
			strncat(detailedName, name, 256); \
		}
#else
#	define RHI_SET_DEBUG_MARKER(rhi, name)
#	define RHI_SET_DEBUG_MARKER_FUNCTION(rhi)
#	define RHI_BEGIN_DEBUG_EVENT(rhi, name)
#	define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi)
#	define RHI_END_DEBUG_EVENT(rhi)
#	define RHI_RESOURCE_DEBUG_NAME(name)
#endif

#if SE_DEBUG
// Check whether or not the given resource is owned by the given RHI
#	define RHI_MATCH_CHECK(rhiReference, resourceReference) \
		SE_ASSERT(&rhiReference == &(resourceReference).getRhi(), "RHI error: The given resource is owned by another RHI instance");
// Debug break on execution failure
#	define FAILED_DEBUG_BREAK(toExecute) if (FAILED(toExecute)) { SE_DEBUG_BREAK; }
#else
#	define RHI_MATCH_CHECK(rhiReference, resourceReference)
#	define FAILED_DEBUG_BREAK(toExecute) toExecute;
#endif // SE_DEBUG

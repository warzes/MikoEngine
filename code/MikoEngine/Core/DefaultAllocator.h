#pragma once

#include "Core/AssertMacros.h"

class DefaultAllocator
{
public:
	[[nodiscard]] static void* Reallocate(void* oldPointer, size_t newNumberOfBytes, size_t alignment)
	{
		// Sanity check
		SE_ASSERT(0 != alignment && !(alignment & (alignment - 1)), "The alignment must be a power of two");

		// Do the work
		if (nullptr != oldPointer && 0 != newNumberOfBytes) // Reallocate
		{			
#if SE_COMPILER_MSVC
			return _aligned_realloc(oldPointer, newNumberOfBytes, alignment);
#else
	// TODO(co) Need aligned version, see e.g. https://github.com/philiptaylor/vulkan-sxs/blob/9cb21b3/common/AllocationCallbacks.cpp#L87 or XSIMD "xsimd_aligned_allocator"
			return realloc(oldPointer, newNumberOfBytes);
#endif
		}
		else // Malloc / free
		{			
#if SE_COMPILER_MSVC
			// Null pointer is valid in here, does nothing in this case
			::_aligned_free(oldPointer);

			// Allocate
			return (0 != newNumberOfBytes) ? ::_aligned_malloc(newNumberOfBytes, alignment) : nullptr;
#elif SE_PLATFORM_ANDROID
			// Null pointer is valid in here, does nothing in this case
			::free(oldPointer);

			// Allocate
			if (0 != newNumberOfBytes)
			{
				void* memptr = nullptr;
				return (posix_memalign(&memptr, alignment, newNumberOfBytes) == 0) ? memptr : nullptr;
			}
			else
			{
				return nullptr;
			}
#else
			// Null pointer is valid in here, does nothing in this case
			::free(oldPointer);

			// Allocate
			return (0 != newNumberOfBytes) ? ::aligned_alloc(alignment, newNumberOfBytes) : nullptr;
#endif
		}
	}
};

#define RHI_MALLOC_TYPED(type, newNumberOfElements) reinterpret_cast<type*>(DefaultAllocator::Reallocate(nullptr, sizeof(type) * (newNumberOfElements), 1))
#define RHI_FREE(oldPointer) DefaultAllocator::Reallocate(oldPointer, 0, 1)

#define RHI_NEW(type) new (DefaultAllocator::Reallocate(nullptr, sizeof(type), 1)) type
#define RHI_DELETE(type, oldPointer)                            \
		do                                                      \
		{                                                       \
			if (nullptr != oldPointer)                          \
			{                                                   \
				typedef type destructor;                        \
				static_cast<type*>(oldPointer)->~destructor();  \
				DefaultAllocator::Reallocate(oldPointer, 0, 1); \
			}                                                   \
		} while (0)
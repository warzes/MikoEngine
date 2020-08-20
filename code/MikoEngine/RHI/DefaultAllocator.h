#pragma once

#include "DefaultAssert.h"

namespace
{
	namespace detail
	{
		[[nodiscard]] void* reallocate(void* oldPointer, size_t, size_t newNumberOfBytes, size_t alignment)
		{
			// Sanity check
			RHI_ASSERT(0 != alignment && !(alignment & (alignment - 1)), "The alignment must be a power of two")

				// Do the work
				if ( nullptr != oldPointer && 0 != newNumberOfBytes )
				{
					// Reallocate
#ifdef _MSC_VER
					return _aligned_realloc(oldPointer, newNumberOfBytes, alignment);
#else
	// TODO(co) Need aligned version, see e.g. https://github.com/philiptaylor/vulkan-sxs/blob/9cb21b3/common/AllocationCallbacks.cpp#L87 or XSIMD "xsimd_aligned_allocator"
					return realloc(oldPointer, newNumberOfBytes);
#endif
				}
				else
				{
					// Malloc / free
#ifdef _MSC_VER
	// Null pointer is valid in here, does nothing in this case
					::_aligned_free(oldPointer);

					// Allocate
					return (0 != newNumberOfBytes) ? ::_aligned_malloc(newNumberOfBytes, alignment) : nullptr;
#elif defined(__ANDROID__)
	// Null pointer is valid in here, does nothing in this case
					::free(oldPointer);

					// Allocate
					if ( 0 != newNumberOfBytes )
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
	} // detail
}

/**
*  @brief
*    Abstract memory allocator interface
*
*  @note
*    - The implementation must be multithreading safe since the RHI is allowed to internally use multiple threads
*    - The interface design is basing on "Nicholas Frechette's Blog Raw bits" - "A memory allocator interface" - http://nfrechette.github.io/2015/05/11/memory_allocator_interface/
*/
/**
*  @brief
*    Default memory allocator implementation class one can use
*
*  @note
*    - Example: uint8_t* spirvOutputBuffer = RHI_MALLOC_TYPED(context, uint8_t, spirvOutputBufferSize);
*    - Designed to be instanced and used inside a single C++ file
*/
class DefaultAllocator
{
	typedef void* (*ReallocateFuntion)(void*, size_t, size_t, size_t);
public:
	template<typename Type>
	[[nodiscard]] static inline Type* constructN(Type* basePointer, size_t count)
	{
		for ( size_t i = 0; i < count; ++i )
		{
			new ((void*)(basePointer + i)) Type();
		}
		return basePointer;
	}

	DefaultAllocator() = default;
	~DefaultAllocator() = default;

	/**
	*  @brief
	*    Reallocate
	*
	*  @param[in] oldPointer
	*    Old pointer, can be a null pointer
	*  @param[in] oldNumberOfBytes
	*    Old number of bytes, must be zero of the old pointer is a null pointer, else can be zero if this information isn't available
	*  @param[in] newNumberOfBytes
	*    New number of bytes
	*  @param[in] alignment
	*    Alignment
	*
	*  @return
	*    New pointer, can be a null pointer
	*/
	inline void* reallocate(void* oldPointer, size_t oldNumberOfBytes, size_t newNumberOfBytes, size_t alignment)
	{
		RHI_ASSERT(nullptr != oldPointer || 0 == oldNumberOfBytes, "Invalid old pointer");
		return ::detail::reallocate(oldPointer, oldNumberOfBytes, newNumberOfBytes, alignment);
	}

private:
	explicit DefaultAllocator(const DefaultAllocator&) = delete;
	DefaultAllocator& operator=(const DefaultAllocator&) = delete;
};

DefaultAllocator& GetAllocator();

// Macros & definitions

// Malloc and free
#define RHI_MALLOC_TYPED(type, newNumberOfElements) reinterpret_cast<type*>(GetAllocator().reallocate(nullptr, 0, sizeof(type) * (newNumberOfElements), 1))
#define RHI_FREE(oldPointer) GetAllocator().reallocate(oldPointer, 0, 0, 1)

// New and delete
// - Using placement new and explicit destructor call
// - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { … } while(0)." for background information about the do-while wrap
#define RHI_NEW(type) new (GetAllocator().reallocate(nullptr, 0, sizeof(type), 1)) type
#define RHI_DELETE(type, oldPointer) \
		do \
		{ \
			if (nullptr != oldPointer) \
			{ \
				typedef type destructor; \
				static_cast<type*>(oldPointer)->~destructor(); \
				GetAllocator().reallocate(oldPointer, 0, 0, 1); \
			} \
		} while (0)
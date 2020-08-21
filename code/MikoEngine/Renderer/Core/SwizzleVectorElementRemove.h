#pragma once

namespace
{
	namespace detail
	{
		/**
		*  @brief
		*    Efficient vector remove
		*
		*  @remarks
		*    Used for efficient removal in "std::vector" and "std::deque" (like an std::list). However it assumes the order of
		*    elements in container is not important or something external to the container holds the index of an element in it
		*    (but still should be kept deterministically across machines). Basically it swaps the iterator with the last iterator,
		*    and pops back. Returns the next iterator.
		*/
		template<typename T>
		typename T::iterator swizzleVectorElementRemove(T& container, typename T::iterator& iterator)
		{
			const int index = static_cast<int>(iterator - container.begin());
			*iterator = container.back();
			container.pop_back();
			return container.begin() + index;
		}
	} // detail
}
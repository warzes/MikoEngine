#pragma once

template <typename T>
inline void SafeDelete(T *&resource) noexcept
{
	delete resource;
	resource = nullptr;
}

template <typename T>
inline void SafeDeleteArray(T *&resource) noexcept
{
	delete[] resource;
	resource = nullptr;
}

// Implementation from "08/02/2015 Better array 'countof' implementation with C++ 11 (updated)" - https://www.g-truc.net/post-0708.html
template<typename T, size_t SizeOfArray>
[[nodiscard]] constexpr size_t Countof(T const (&)[SizeOfArray]) noexcept
{
	return SizeOfArray;
}
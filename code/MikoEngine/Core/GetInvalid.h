#pragma once

// TODO: возможно обернуть пространством имен

// Return invalid default value for a given type
template <typename T>
[[nodiscard]] inline T GetInvalid() noexcept
{
	return std::numeric_limits<T>::max();
}

// Return invalid default value for a type provided by a variable
template <typename T>
[[nodiscard]] inline T GetInvalid(const T&) noexcept
{
	return std::numeric_limits<T>::max();
}

// Set the given variable to invalid default value
template <typename T>
inline void SetInvalid(T& value) noexcept
{
	value = GetInvalid<T>();
}

// Check whether or not the given variable has not the invalid default value
template <typename T>
[[nodiscard]] inline bool IsValid(const T &value) noexcept
{
	return (GetInvalid<T>() != value);
}

// Check whether or not the given variable has the invalid default value
template <typename T>
[[nodiscard]] inline bool IsInvalid(const T &value) noexcept
{
	return (GetInvalid<T>() == value);
}
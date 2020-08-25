#pragma once

// TODO: от виртуальности RefCount можно избавиться, если передавать функцию деструктора
// TODO: RefCount возможно не потокобезопасен, можно сделать спец версию (пример из годота - safe_refcount
// TODO: нужны слабые ссылки (weak ptr)


template <class T>
class RefCount
{
public:
	RefCount() noexcept = default;
	virtual ~RefCount() noexcept = default;

	[[nodiscard]] SE_FORCEINLINE const T* GetPointer() const noexcept
	{
		return static_cast<const T*>(this);
	}

	[[nodiscard]] SE_FORCEINLINE T* GetPointer() noexcept
	{
		return static_cast<T*>(this);
	}
	SE_FORCEINLINE uint32_t AddReference() noexcept
	{
		// Increment reference count
		++m_refCount;
		// Return current reference count
		return m_refCount;
	}

	SE_FORCEINLINE uint32_t ReleaseReference() noexcept
	{
		// Decrement reference count
		if (m_refCount > 1)
		{
			--m_refCount;
			// Return current reference count
			return m_refCount;
		}
		// Destroy object when no references are left
		else
		{
			selfDestruct();
			// This object is no longer
			return 0;
		}
	}

	[[nodiscard]] SE_FORCEINLINE uint32_t GetRefCount() const noexcept
	{
		return m_refCount;
	}

protected:
	virtual void selfDestruct() noexcept = 0;

private:
	uint32_t m_refCount = 0;
};

template <class T>
class SmartRefCount
{
public:
	SmartRefCount() noexcept = default;
	SE_FORCEINLINE SmartRefCount(T *ptr) noexcept
	{
		setPtr(ptr);
	}
	SE_FORCEINLINE SmartRefCount(const SmartRefCount<T> &ptr) noexcept
	{
		setPtr(ptr.getPtr());
	}

	inline ~SmartRefCount() noexcept
	{
		setPtr(nullptr);
	}

	SE_FORCEINLINE SmartRefCount<T>& operator=(T *ptr) noexcept
	{
		if (GetPointer() != ptr)
			setPtr(ptr);
		return *this;
	}

	SE_FORCEINLINE SmartRefCount<T>& operator=(const SmartRefCount<T> &ptr) noexcept
	{
		if (GetPointer() != ptr.GetPointer())
			setPtr(ptr.getPtr());
		return *this;
	}

	[[nodiscard]] SE_FORCEINLINE T* GetPointer() const noexcept
	{
		return (nullptr != m_ptr) ? static_cast<T*>(m_ptr->GetPointer()) : nullptr;
	}

	[[nodiscard]] SE_FORCEINLINE T* operator->() const noexcept
	{
		return GetPointer();
	}

	[[nodiscard]] SE_FORCEINLINE operator T*() const noexcept
	{
		return GetPointer();
	}

	[[nodiscard]] SE_FORCEINLINE bool operator!() const noexcept
	{
		return (nullptr == GetPointer());
	}

	[[nodiscard]] SE_FORCEINLINE bool operator==(T *ptr) const noexcept
	{
		return (GetPointer() == ptr);
	}

	[[nodiscard]] SE_FORCEINLINE bool operator==(const SmartRefCount<T> &ptr) const noexcept
	{
		return (GetPointer() == ptr.GetPointer());
	}

	[[nodiscard]] SE_FORCEINLINE bool operator!=(T *ptr) const noexcept
	{
		return (GetPointer() != ptr);
	}

	[[nodiscard]] SE_FORCEINLINE bool operator!=(const SmartRefCount<T> &ptr) const noexcept
	{
		return (GetPointer() != ptr.GetPointer());
	}

private:
	SE_FORCEINLINE void setPtr(T *ptr) noexcept
	{
		// Release old pointer
		if (nullptr != m_ptr)
			m_ptr->ReleaseReference();
		// Assign new pointer
		if (nullptr != ptr)
			ptr->AddReference();
		m_ptr = ptr;
	}

	[[nodiscard]] SE_FORCEINLINE T* getPtr() const noexcept
	{
		return m_ptr;
	}

	T *m_ptr = nullptr; // Pointer to reference counter, can be a null pointer
};
#pragma once

template <class T>
class RefCount
{
public:
	SE_FORCEINLINE RefCount() = default;
	inline virtual ~RefCount() = default;

	[[nodiscard]] SE_FORCEINLINE const T* GetPointer() const
	{
		return static_cast<const T*>(this);
	}

	[[nodiscard]] SE_FORCEINLINE T* GetPointer()
	{
		return static_cast<T*>(this);
	}

	SE_FORCEINLINE uint32_t AddReference()
	{
		++m_refCount;
		return m_refCount;
	}

	SE_FORCEINLINE uint32_t ReleaseReference()
	{
		if ( m_refCount > 1 )
		{
			--m_refCount;
			return m_refCount;
		}
		else
		{
			selfDestruct();
			return 0;
		}
	}

	[[nodiscard]] SE_FORCEINLINE uint32_t GetRefCount() const
	{
		return m_refCount;
	}

protected:
	virtual void selfDestruct() = 0;
private:
	uint32_t m_refCount = 0;
};

template <class T>
class SmartRefCount
{
public:
	SE_FORCEINLINE SmartRefCount() = default;
	SE_FORCEINLINE SmartRefCount(T *ptr)
	{
		setPtr(ptr);
	}
	SE_FORCEINLINE SmartRefCount(const SmartRefCount<T> &ptr) 
	{
		setPtr(ptr.getPtr());
	}

	inline ~SmartRefCount()
	{
		setPtr(nullptr);
	}

	SE_FORCEINLINE SmartRefCount<T>& operator =(T *ptr)
	{
		if ( GetPointer() != ptr )
			setPtr(ptr);

		return *this;
	}

	SE_FORCEINLINE SmartRefCount<T>& operator =(const SmartRefCount<T> &ptr)
	{
		if ( GetPointer() != ptr.GetPointer() )
			setPtr(ptr.getPtr());
		return *this;
	}
	[[nodiscard]] SE_FORCEINLINE T* GetPointer() const
	{
		return (nullptr != m_ptr) ? static_cast<T*>(m_ptr->GetPointer()) : nullptr;
	}

	[[nodiscard]] SE_FORCEINLINE T* operator->() const
	{
		return GetPointer();
	}

	[[nodiscard]] SE_FORCEINLINE operator T*() const
	{
		return GetPointer();
	}

	[[nodiscard]] SE_FORCEINLINE bool operator!() const
	{
		return (nullptr == GetPointer());
	}

	[[nodiscard]] SE_FORCEINLINE bool operator==(T *ptr) const
	{
		return (GetPointer() == ptr);
	}

	[[nodiscard]] SE_FORCEINLINE bool operator==(const SmartRefCount<T> &ptr) const
	{
		return (GetPointer() == ptr.GetPointer());
	}

	[[nodiscard]] SE_FORCEINLINE bool operator!=(T *ptr) const
	{
		return (GetPointer() != ptr);
	}
	[[nodiscard]] SE_FORCEINLINE bool operator!=(const SmartRefCount<T> &ptr) const
	{
		return (GetPointer() != ptr.GetPointer());
	}

private:
	SE_FORCEINLINE void setPtr(T *ptr)
	{
		if ( nullptr != m_ptr )
			m_ptr->ReleaseReference();

		if ( nullptr != ptr )
			ptr->AddReference();
		m_ptr = ptr;
	}

	[[nodiscard]] SE_FORCEINLINE T* getPtr() const
	{
		return m_ptr;
	}

private:
	T *m_ptr = nullptr;
};
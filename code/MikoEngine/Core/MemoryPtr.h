#pragma once

//[-------------------------------------------------------]
	//[ Rhi/RefCount.h                                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Reference counter template
	*
	*  @note
	*    - Initially the reference counter is 0
	*/
template <class AType>
class RefCount
{

	// Public methods
public:
	/**
	*  @brief
	*    Default constructor
	*/
	SE_FORCEINLINE RefCount() :
		mRefCount(0)
	{
	}

	/**
	*  @brief
	*    Destructor
	*/
	inline virtual ~RefCount()
	{
	}

	/**
	*  @brief
	*    Get a pointer to the object
	*
	*  @return
	*    Pointer to the reference counter's object, never a null pointer!
	*/
	[[nodiscard]] SE_FORCEINLINE const AType* getPointer() const
	{
		return static_cast<const AType*>(this);
	}

	/**
	*  @brief
	*    Get a pointer to the object
	*
	*  @return
	*    Pointer to the reference counter's object, never a null pointer!
	*/
	[[nodiscard]] SE_FORCEINLINE AType* getPointer()
	{
		return static_cast<AType*>(this);
	}

	/**
	*  @brief
	*    Increases the reference count
	*
	*  @return
	*    Current reference count
	*/
	SE_FORCEINLINE uint32_t addReference()
	{
		// Increment reference count
		++mRefCount;

		// Return current reference count
		return mRefCount;
	}

	/**
	*  @brief
	*    Decreases the reference count
	*
	*  @return
	*    Current reference count
	*
	*  @note
	*    - When the last reference was released, the instance is destroyed automatically
	*/
	SE_FORCEINLINE uint32_t releaseReference()
	{
		// Decrement reference count
		if ( mRefCount > 1 )
		{
			--mRefCount;

			// Return current reference count
			return mRefCount;
		}

		// Destroy object when no references are left
		else
		{
			selfDestruct();

			// This object is no longer
			return 0;
		}
	}

	/**
	*  @brief
	*    Gets the current reference count
	*
	*  @return
	*    Current reference count
	*/
	[[nodiscard]] SE_FORCEINLINE uint32_t getRefCount() const
	{
		// Return current reference count
		return mRefCount;
	}

	// Protected virtual Rhi::RefCount methods
protected:
	/**
	*  @brief
	*    Destroy the instance
	*/
	virtual void selfDestruct() = 0;

	// Private data
private:
	uint32_t mRefCount; ///< Reference count

};




//[-------------------------------------------------------]
//[ Rhi/SmartRefCount.h                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Smart reference counter template
*/
template <class AType>
class SmartRefCount
{

	// Public methods
public:
	/**
	*  @brief
	*    Default constructor
	*/
	SE_FORCEINLINE SmartRefCount() :
		mPtr(nullptr)
	{
	}

	/**
	*  @brief
	*    Constructor
	*
	*  @param[in] ptr
	*    Direct pointer to initialize with, can be a null pointer
	*/
	SE_FORCEINLINE SmartRefCount(AType* ptr) :
		mPtr(nullptr)
	{
		setPtr(ptr);
	}

	/**
	*  @brief
	*    Copy constructor
	*
	*  @param[in] ptr
	*    Smart pointer to initialize with
	*/
	SE_FORCEINLINE SmartRefCount(const SmartRefCount<AType>& ptr) :
		mPtr(nullptr)
	{
		setPtr(ptr.getPtr());
	}

	/**
	*  @brief
	*    Destructor
	*/
	inline ~SmartRefCount()
	{
		setPtr(nullptr);
	}

	/**
	*  @brief
	*    Assign a pointer
	*
	*  @param[in] ptr
	*    Direct pointer to assign, can be a null pointer
	*
	*  @return
	*    Reference to the smart pointer
	*/
	SE_FORCEINLINE SmartRefCount<AType>& operator =(AType* ptr)
	{
		if ( getPointer() != ptr )
		{
			setPtr(ptr);
		}
		return *this;
	}

	/**
	*  @brief
	*    Assign a smart pointer
	*
	*  @param[in] ptr
	*    Smart pointer to assign
	*
	*  @return
	*    Reference to the smart pointer
	*/
	SE_FORCEINLINE SmartRefCount<AType>& operator =(const SmartRefCount<AType>& ptr)
	{
		if ( getPointer() != ptr.getPointer() )
		{
			setPtr(ptr.getPtr());
		}
		return *this;
	}

	/**
	*  @brief
	*    Get a direct pointer to the object
	*
	*  @return
	*    Pointer to the object, can be a null pointer
	*/
	[[nodiscard]] SE_FORCEINLINE AType* getPointer() const
	{
		return (nullptr != mPtr) ? static_cast<AType*>(mPtr->getPointer()) : nullptr;
	}

	/**
	*  @brief
	*    Get a pointer to access the object
	*
	*  @return
	*    Pointer to the object, can be a null pointer
	*/
	[[nodiscard]] SE_FORCEINLINE AType* operator ->() const
	{
		return getPointer();
	}

	/**
	*  @brief
	*    Cast to a pointer to the object
	*
	*  @return
	*    Pointer to the object, can be a null pointer
	*/
	[[nodiscard]] SE_FORCEINLINE operator AType*() const
	{
		return getPointer();
	}

	/**
	*  @brief
	*    Check if the pointer is not a null pointer
	*
	*  @return
	*    "true" if the pointer is not a null pointer
	*/
	[[nodiscard]] SE_FORCEINLINE bool operator !() const
	{
		return (nullptr == getPointer());
	}

	/**
	*  @brief
	*    Check for equality
	*
	*  @param[in] ptr
	*    Direct pointer to compare with, can be a null pointer
	*
	*  @return
	*    "true" if the two pointers are equal
	*/
	[[nodiscard]] SE_FORCEINLINE bool operator ==(AType* ptr) const
	{
		return (getPointer() == ptr);
	}

	/**
	*  @brief
	*    Check for equality
	*
	*  @param[in] ptr
	*    Smart pointer to compare with
	*
	*  @return
	*    "true" if the two pointers are equal
	*/
	[[nodiscard]] SE_FORCEINLINE bool operator ==(const SmartRefCount<AType>& ptr) const
	{
		return (getPointer() == ptr.getPointer());
	}

	/**
	*  @brief
	*    Check for equality
	*
	*  @param[in] ptr
	*    Direct pointer to compare with, can be a null pointer
	*
	*  @return
	*    "true" if the two pointers are not equal
	*/
	[[nodiscard]] SE_FORCEINLINE bool operator !=(AType* ptr) const
	{
		return (getPointer() != ptr);
	}

	/**
	*  @brief
	*    Check for equality
	*
	*  @param[in] ptr
	*    Smart pointer to compare with
	*
	*  @return
	*    "true" if the two pointers are not equal
	*/
	[[nodiscard]] SE_FORCEINLINE bool operator !=(const SmartRefCount<AType>& ptr) const
	{
		return (getPointer() != ptr.getPointer());
	}

	// Private methods
private:
	/**
	*  @brief
	*    Assign a pointer to an object that implements RefCount
	*
	*  @param[in] ptr
	*    Pointer to assign, can be a null pointer
	*/
	SE_FORCEINLINE void setPtr(AType* ptr)
	{
		// Release old pointer
		if ( nullptr != mPtr )
		{
			mPtr->releaseReference();
		}

		// Assign new pointer
		if ( nullptr != ptr )
		{
			ptr->addReference();
		}
		mPtr = ptr;
	}

	/**
	*  @brief
	*    Get pointer to the reference counted object
	*
	*  @return
	*    Pointer to the RefCount object, can be a null pointer
	*/
	[[nodiscard]] SE_FORCEINLINE AType* getPtr() const
	{
		// Return pointer
		return mPtr;
	}

	// Private data
private:
	AType* mPtr; ///< Pointer to reference counter, can be a null pointer

};
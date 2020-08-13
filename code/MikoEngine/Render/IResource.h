#pragma once

namespace Rhi
{
	class IResource : public RefCount<IResource>
	{
		// Public methods
	public:
		inline virtual ~IResource() override = default;

#if SE_DEBUG
		[[nodiscard]] inline const char* getDebugName() const
		{
			return mDebugName;
		}
#endif

		[[nodiscard]] inline ResourceType getResourceType() const
		{
			return mResourceType;
		}

		[[nodiscard]] inline IRhi& getRhi() const
		{
			return *mRhi;
		}

		// Public virtual Rhi::IResource methods
	public:
		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the RHI implementation specific resource handle (e.g. native Direct3D texture pointer or OpenGL texture ID)
		*
		*  @return
		*    The RHI implementation specific resource handle, can be a null pointer
		*
		*  @note
		*    - Don't use this RHI implementation specific method if you don't have to
		*/
		[[nodiscard]] inline virtual void* getInternalResourceHandle() const
		{
			return nullptr;
		}

		// Protected methods
	protected:
#if SE_DEBUG
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*
		*  @note
		*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
		*/
		inline IResource(ResourceType resourceType, const char debugName[]) :
			mResourceType(resourceType),
			mRhi(nullptr)	// Only used for rare border cases, use the constructor with the RHI reference whenever possible. Normally the RHI pointer should never ever be a null pointer. So if you're in here, you're considered to be evil.
		{
			RHI_ASSERT(strlen(debugName) < 256, "Resource debug name is not allowed to exceed 255 characters")
				strncpy(mDebugName, debugName, 256);
			mDebugName[255] = '\0';
		}

		inline IResource(ResourceType resourceType, IRhi& rhi, const char debugName[]) :
			mResourceType(resourceType),
			mRhi(&rhi)
		{
			RHI_ASSERT(strlen(debugName) < 256, "Resource debug name is not allowed to exceed 255 characters")
				strncpy(mDebugName, debugName, 256);
			mDebugName[255] = '\0';
		}
#else
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*
		*  @note
		*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
		*/
		inline explicit IResource(ResourceType resourceType) :
			mResourceType(resourceType),
			mRhi(nullptr)	// Only used for rare border cases, use the constructor with the RHI reference whenever possible. Normally the RHI pointer should never ever be a null pointer. So if you're in here, you're considered to be evil.
		{
		}

		inline IResource(ResourceType resourceType, IRhi& rhi) :
			mResourceType(resourceType),
			mRhi(&rhi)
		{
		}
#endif

		explicit IResource(const IResource&) = delete;
		IResource& operator=(const IResource&) = delete;

		// Private data
	private:
#if SE_DEBUG
		char mDebugName[256];	// Debug name for easier resource identification when debugging, contains terminating zero, first member variable by intent to see it at once during introspection (debug memory layout change is no problem here)
#endif
		ResourceType mResourceType;
		IRhi*		 mRhi;
	};

	typedef SmartRefCount<IResource> IResourcePtr;

} // namespace Rhi
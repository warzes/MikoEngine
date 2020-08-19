#pragma once

namespace Rhi
{
	class IBuffer : public IResource
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IBuffer() override
		{
		}

		// Protected methods
	protected:
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
		inline explicit IBuffer(ResourceType resourceType RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IBuffer(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		explicit IBuffer(const IBuffer& source) = delete;
		IBuffer& operator =(const IBuffer& source) = delete;

	};

	typedef SmartRefCount<IBuffer> IBufferPtr;
} // namespace Rhi
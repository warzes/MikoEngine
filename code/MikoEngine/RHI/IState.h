#pragma once

namespace Rhi
{
	class IState : public IResource
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IState() override
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
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IState(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		explicit IState(const IState& source) = delete;
		IState& operator =(const IState& source) = delete;

	};

	typedef SmartRefCount<IState> IStatePtr;
} // namespace Rhi
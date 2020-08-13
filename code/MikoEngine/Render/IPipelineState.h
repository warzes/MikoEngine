#pragma once

namespace Rhi
{
	class IPipelineState : public IState
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IPipelineState() override
		{
		}

		/**
		*  @brief
		*    Return the unique compact graphics or compute pipeline state ID; one internal ID pool per concrete pipeline state type
		*
		*  @return
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline uint16_t getId() const
		{
			return mId;
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
		*  @param[in] id
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline IPipelineState(ResourceType resourceType, IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IState(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mId(id)
		{
		}

		explicit IPipelineState(const IPipelineState& source) = delete;
		IPipelineState& operator =(const IPipelineState& source) = delete;

		// Private data
	private:
		uint16_t mId;
	};

	typedef SmartRefCount<IPipelineState> IPipelineStatePtr;
} // namespace Rhi
#pragma once

namespace Rhi
{
	class IComputePipelineState : public IPipelineState
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IComputePipelineState() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfComputePipelineStates;
#endif
		}

		// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] id
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline IComputePipelineState(IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IPipelineState(ResourceType::COMPUTE_PIPELINE_STATE, rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedComputePipelineStates;
			++rhi.getStatistics().currentNumberOfComputePipelineStates;
#endif
		}

		explicit IComputePipelineState(const IComputePipelineState& source) = delete;
		IGraphicsPipelineState& operator =(const IComputePipelineState& source) = delete;

	};

	typedef SmartRefCount<IComputePipelineState> IComputePipelineStatePtr;
} // namespace Rhi
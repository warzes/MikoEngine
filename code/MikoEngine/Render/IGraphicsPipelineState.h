#pragma once

namespace Rhi
{
	class IGraphicsPipelineState : public IPipelineState
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IGraphicsPipelineState() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfGraphicsPipelineStates;
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
		inline IGraphicsPipelineState(IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IPipelineState(ResourceType::GRAPHICS_PIPELINE_STATE, rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedGraphicsPipelineStates;
			++rhi.getStatistics().currentNumberOfGraphicsPipelineStates;
#endif
		}

		explicit IGraphicsPipelineState(const IGraphicsPipelineState& source) = delete;
		IGraphicsPipelineState& operator =(const IGraphicsPipelineState& source) = delete;

	};

	typedef SmartRefCount<IGraphicsPipelineState> IGraphicsPipelineStatePtr;
} // namespace Rhi
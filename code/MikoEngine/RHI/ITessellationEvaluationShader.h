#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) interface
*/
	class ITessellationEvaluationShader : public IShader
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITessellationEvaluationShader() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTessellationEvaluationShaders;
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
		*/
		inline explicit ITessellationEvaluationShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TESSELLATION_EVALUATION_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTessellationEvaluationShaders;
			++rhi.getStatistics().currentNumberOfTessellationEvaluationShaders;
#endif
		}

		explicit ITessellationEvaluationShader(const ITessellationEvaluationShader& source) = delete;
		ITessellationEvaluationShader& operator =(const ITessellationEvaluationShader& source) = delete;

	};

	typedef SmartRefCount<ITessellationEvaluationShader> ITessellationEvaluationShaderPtr;
} // namespace Rhi
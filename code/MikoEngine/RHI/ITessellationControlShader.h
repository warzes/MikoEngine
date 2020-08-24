#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract tessellation control shader (TCS, "hull shader" in Direct3D terminology) interface
*/
	class ITessellationControlShader : public IShader
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITessellationControlShader() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTessellationControlShaders;
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
		inline explicit ITessellationControlShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TESSELLATION_CONTROL_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTessellationControlShaders;
			++rhi.getStatistics().currentNumberOfTessellationControlShaders;
#endif
		}

		explicit ITessellationControlShader(const ITessellationControlShader& source) = delete;
		ITessellationControlShader& operator =(const ITessellationControlShader& source) = delete;

	};

	typedef SmartRefCount<ITessellationControlShader> ITessellationControlShaderPtr;

} // namespace Rhi
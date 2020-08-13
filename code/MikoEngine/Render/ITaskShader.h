#pragma once

namespace Rhi
{
	class ITaskShader : public IShader
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITaskShader() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTaskShaders;
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
		inline explicit ITaskShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TASK_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTaskShaders;
			++rhi.getStatistics().currentNumberOfTaskShaders;
#endif
		}

		explicit ITaskShader(const ITaskShader& source) = delete;
		ITaskShader& operator =(const ITaskShader& source) = delete;

	};

	typedef SmartRefCount<ITaskShader> ITaskShaderPtr;
} // namespace Rhi
#pragma once

namespace Rhi
{
	class IComputeShader : public IShader
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IComputeShader() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfComputeShaders;
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
		inline explicit IComputeShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::COMPUTE_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedComputeShaders;
			++rhi.getStatistics().currentNumberOfComputeShaders;
#endif
		}

		explicit IComputeShader(const IComputeShader& source) = delete;
		IComputeShader& operator =(const IComputeShader& source) = delete;

	};

	typedef SmartRefCount<IComputeShader> IComputeShaderPtr;
} // namespace Rhi
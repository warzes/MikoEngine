#pragma once

namespace Rhi
{
	class IFragmentShader : public IShader
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IFragmentShader() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfFragmentShaders;
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
		inline explicit IFragmentShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::FRAGMENT_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedFragmentShaders;
			++rhi.getStatistics().currentNumberOfFragmentShaders;
#endif
		}

		explicit IFragmentShader(const IFragmentShader& source) = delete;
		IFragmentShader& operator =(const IFragmentShader& source) = delete;

	};

	typedef SmartRefCount<IFragmentShader> IFragmentShaderPtr;
} // namespace Rhi
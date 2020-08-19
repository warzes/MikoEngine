#pragma once

namespace Rhi
{
	class IVertexShader : public IShader
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IVertexShader() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfVertexShaders;
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
		inline explicit IVertexShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::VERTEX_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedVertexShaders;
			++rhi.getStatistics().currentNumberOfVertexShaders;
#endif
		}

		explicit IVertexShader(const IVertexShader& source) = delete;
		IVertexShader& operator =(const IVertexShader& source) = delete;

	};

	typedef SmartRefCount<IVertexShader> IVertexShaderPtr;
} // namespace Rhi
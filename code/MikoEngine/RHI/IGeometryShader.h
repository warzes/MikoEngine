#pragma once

namespace Rhi
{
	class IGeometryShader : public IShader
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IGeometryShader() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfGeometryShaders;
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
		inline explicit IGeometryShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::GEOMETRY_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedGeometryShaders;
			++rhi.getStatistics().currentNumberOfGeometryShaders;
#endif
		}

		explicit IGeometryShader(const IGeometryShader& source) = delete;
		IGeometryShader& operator =(const IGeometryShader& source) = delete;

	};

	typedef SmartRefCount<IGeometryShader> IGeometryShaderPtr;
} // namespace Rhi
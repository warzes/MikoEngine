#pragma once

namespace Rhi
{
	class IMeshShader : public IShader
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IMeshShader() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfMeshShaders;
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
		inline explicit IMeshShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::MESH_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedMeshShaders;
			++rhi.getStatistics().currentNumberOfMeshShaders;
#endif
		}

		explicit IMeshShader(const IMeshShader& source) = delete;
		IMeshShader& operator =(const IMeshShader& source) = delete;

	};

	typedef SmartRefCount<IMeshShader> IMeshShaderPtr;
} // namespace Rhi
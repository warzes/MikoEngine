#pragma once

namespace Rhi
{
	class IGraphicsProgram : public IResource
	{
	public:
		inline virtual ~IGraphicsProgram() override
		{
#if SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfGraphicsPrograms;
#endif
		}

		// Public virtual Rhi::IGraphicsProgram methods
	public:
		// TODO(co) Cleanup
		[[nodiscard]] inline virtual handle getUniformHandle([[maybe_unused]] const char* uniformName)
		{
			return SE_NULL_HANDLE;
		}

		inline virtual void setUniform1i([[maybe_unused]] handle uniformHandle, [[maybe_unused]] int value)
		{
		}

		inline virtual void setUniform1f([[maybe_unused]] handle uniformHandle, [[maybe_unused]] float value)
		{
		}

		inline virtual void setUniform2fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{
		}

		inline virtual void setUniform3fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{
		}

		inline virtual void setUniform4fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{
		}

		inline virtual void setUniformMatrix3fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{
		}

		inline virtual void setUniformMatrix4fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{
		}

		// Protected methods
	protected:
		inline explicit IGraphicsProgram(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::GRAPHICS_PROGRAM, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedGraphicsPrograms;
			++rhi.getStatistics().currentNumberOfGraphicsPrograms;
#endif
		}

		explicit IGraphicsProgram(const IGraphicsProgram&) = delete;
		IGraphicsProgram& operator=(const IGraphicsProgram&) = delete;
	};

	typedef SmartRefCount<IGraphicsProgram> IGraphicsProgramPtr;

} // namespace Rhi
#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract uniform buffer object (UBO, "constant buffer" in Direct3D terminology) interface; read only structured buffer
*
*  @remarks
*    General usage hint
*      - Maximum size:                 64KByte (or more)
*      - Memory access pattern:        Coherent access
*      - Memory storage:               Usually local memory which is considered to be faster than global memory
*    OpenGL - http://www.opengl.org/wiki/Uniform_Buffer_Object
*      - Core in version:              4.2
*      - Adopted into core in version: 3.1
*      - ARB extension:                GL_ARB_Uniform_Buffer_Object
*    Direct3D - "Shader Constants"-documentation - http://msdn.microsoft.com/en-us/library/windows/desktop/bb509581%28v=vs.85%29.aspx
*      - Direct3D version:             10 and 11
*      - Shader model:                 4
*
*  @note
*    - Structured performance tip: "Constant Buffers without Constant Pain" by Holger Gruen, posted Jan 14 2015 at 10:38AM - https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0
*/
	class IUniformBuffer : public IBuffer
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IUniformBuffer() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfUniformBuffers;
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
		inline explicit IUniformBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::UNIFORM_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedUniformBuffers;
			++rhi.getStatistics().currentNumberOfUniformBuffers;
#endif
		}

		explicit IUniformBuffer(const IUniformBuffer& source) = delete;
		IUniformBuffer& operator =(const IUniformBuffer& source) = delete;

	};

	typedef SmartRefCount<IUniformBuffer> IUniformBufferPtr;
} // namespace Rhi
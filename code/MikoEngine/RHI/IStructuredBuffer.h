#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract structured buffer object (SBO) interface; array of structured data
*
*  @remarks
*    General usage hint
*      - Maximum size:                 128MByte (or more)
*      - Memory access pattern:        Random access
*      - Memory storage:               Global texture memory which is considered to be slower than local memory
*    OpenGL - Shader Storage Buffer Object (SSBO) - https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
*      - Core in version:              4.6
*      - Adopted into core in version: 4.3
*      - ARB extension:                GL_ARB_shader_storage_buffer_object
*    Direct3D - "New Resource Types"-documentation - https://docs.microsoft.com/en-gb/windows/desktop/direct3d11/direct3d-11-advanced-stages-cs-resources
*      - Direct3D version:             11
*      - Shader model:                 5
*
*  @note
*    - There's no support for Direct3D 11 style "append" or "counter" structured buffer by intent since neither OpenGL nor Vulkan support something similar, use atomic shader intrinsics instead
*    - Structured performance tip: "Structured buffer, 128-bit stride, coherent access patterns, and under 64 KB of data = Use uniform buffer instead" - "How About Constant Buffers?" by Evan Hart, posted May 04 2015 at 11:34AM - https://developer.nvidia.com/content/how-about-constant-buffers
*    - Structured performance tip: "Use 128-bit stride" -- "Understanding Structured Buffer Performance" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance
*    - Structured performance tip: "Redundancy and Latency in Structured Buffer Use" by Evan Hart, posted Apr 30 2015 at 06:15PM - https://developer.nvidia.com/content/redundancy-and-latency-structured-buffer-use
*/
	class IStructuredBuffer : public IBuffer
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IStructuredBuffer() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfStructuredBuffers;
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
		inline explicit IStructuredBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::STRUCTURED_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedStructuredBuffers;
			++rhi.getStatistics().currentNumberOfStructuredBuffers;
#endif
		}

		explicit IStructuredBuffer(const IStructuredBuffer& source) = delete;
		IStructuredBuffer& operator =(const IStructuredBuffer& source) = delete;

	};

	typedef SmartRefCount<IStructuredBuffer> IStructuredBufferPtr;
} // namespace Rhi
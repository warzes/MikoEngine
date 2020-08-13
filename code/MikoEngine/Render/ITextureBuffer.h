#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract texture buffer object (TBO) interface; array of homogeneous data
*
*  @remarks
*    General usage hint
*      - Maximum size:                 128MByte (or more)
*      - Memory access pattern:        Random access
*      - Memory storage:               Global texture memory which is considered to be slower than local memory
*    OpenGL - http://www.opengl.org/wiki/Buffer_Texture
*      - Core in version:              4.2
*      - Adopted into core in version: 3.0
*      - ARB extension:                GL_ARB_texture_buffer_object
*      - EXT extension:                GL_EXT_texture_buffer_object
*    Direct3D - "Shader Constants"-documentation - http://msdn.microsoft.com/en-us/library/windows/desktop/bb509581%28v=vs.85%29.aspx
*      - Direct3D version:             10 and 11
*      - Shader model:                 4
*/
	class ITextureBuffer : public IBuffer
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureBuffer() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfTextureBuffers;
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
		inline explicit ITextureBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::TEXTURE_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedTextureBuffers;
			++rhi.getStatistics().currentNumberOfTextureBuffers;
#endif
		}

		explicit ITextureBuffer(const ITextureBuffer& source) = delete;
		ITextureBuffer& operator =(const ITextureBuffer& source) = delete;

	};

	typedef SmartRefCount<ITextureBuffer> ITextureBufferPtr;
} // namespace Rhi
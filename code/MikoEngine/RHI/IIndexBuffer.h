#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract index buffer object (IBO, "element array buffer" in OpenGL terminology) interface
*/
	class IIndexBuffer : public IBuffer
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IIndexBuffer() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfIndexBuffers;
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
		inline explicit IIndexBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::INDEX_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedIndexBuffers;
			++rhi.getStatistics().currentNumberOfIndexBuffers;
#endif
		}

		explicit IIndexBuffer(const IIndexBuffer& source) = delete;
		IIndexBuffer& operator =(const IIndexBuffer& source) = delete;

	};

	typedef SmartRefCount<IIndexBuffer> IIndexBufferPtr;
} // namespace Rhi
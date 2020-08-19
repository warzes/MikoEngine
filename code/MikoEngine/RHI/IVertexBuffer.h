#pragma once

namespace Rhi
{
	class IVertexBuffer : public IBuffer
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IVertexBuffer() override
		{
#ifdef SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfVertexBuffers;
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
		inline explicit IVertexBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::VERTEX_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedVertexBuffers;
			++rhi.getStatistics().currentNumberOfVertexBuffers;
#endif
		}

		explicit IVertexBuffer(const IVertexBuffer& source) = delete;
		IVertexBuffer& operator =(const IVertexBuffer& source) = delete;

	};

	typedef SmartRefCount<IVertexBuffer> IVertexBufferPtr;
} // namespace Rhi
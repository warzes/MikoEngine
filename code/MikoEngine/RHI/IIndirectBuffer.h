#pragma once

namespace Rhi
{
	/**
*  @brief
*    Abstract indirect buffer object interface; array of draw call data
*
*  @note
*    - Contains instances of "Rhi::DrawArguments" and "Rhi::DrawIndexedArguments"
*    - Indirect buffers where originally introduced to be able to let the GPU have some more control over draw commands,
*      but with the introduction of multi indirect draw it became also interesting for reducing RHI implementation overhead (AZDO ("Almost Zero Driver Overhead"))
*/
	class IIndirectBuffer : public IBuffer
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IIndirectBuffer() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfIndirectBuffers;
#endif
		}

		// Public virtual Rhi::IIndirectBuffer methods
	public:
		/**
		*  @brief
		*    Return indirect buffer emulation data pointer
		*
		*  @return
		*    Indirect buffer emulation data pointer, can be a null pointer, don't destroy the returned instance
		*/
		[[nodiscard]] virtual const uint8_t* getEmulationData() const = 0;

		// Protected methods
	protected:
#if SE_DEBUG
		/**
		*  @brief
		*    Default constructor
		*
		*  @note
		*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
		*/
		inline explicit IIndirectBuffer(const char debugName[]) :
			IBuffer(ResourceType::INDIRECT_BUFFER, debugName)
		{
		}
#else
		/**
		*  @brief
		*    Default constructor
		*
		*  @note
		*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
		*/
		inline IIndirectBuffer() :
			IBuffer(ResourceType::INDIRECT_BUFFER)
		{
		}
#endif

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IIndirectBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::INDIRECT_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedIndirectBuffers;
			++rhi.getStatistics().currentNumberOfIndirectBuffers;
#endif
		}

		explicit IIndirectBuffer(const IIndirectBuffer& source) = delete;
		IIndirectBuffer& operator =(const IIndirectBuffer& source) = delete;

	};

	typedef SmartRefCount<IIndirectBuffer> IIndirectBufferPtr;
} // namespace Rhi
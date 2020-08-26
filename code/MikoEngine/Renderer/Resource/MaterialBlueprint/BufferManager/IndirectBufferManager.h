#pragma once

namespace Rhi
{
	class IIndirectBuffer;
}
namespace Renderer
{
	class IRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Indirect buffer manager
	*/
	class IndirectBufferManager final
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct IndirectBuffer final
		{
			Rhi::IIndirectBuffer* indirectBuffer;		// RHI indirect buffer instance, always valid
			uint32_t			  indirectBufferOffset;	// Current indirect buffer offset
			uint8_t*			  mappedData;			// Currently mapped data, don't destroy the data
			explicit IndirectBuffer(Rhi::IIndirectBuffer* _indirectBuffer) :
				indirectBuffer(_indirectBuffer),
				indirectBufferOffset(0),
				mappedData(nullptr)
			{}
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderer
		*    Renderer instance to use, must stay valid as long as the indirect buffer manager instance exists
		*/
		explicit IndirectBufferManager(const IRenderer& renderer);

		/**
		*  @brief
		*    Return the renderer instance to use
		*
		*  @return
		*    The renderer instance to use
		*/
		[[nodiscard]] inline const IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		/**
		*  @brief
		*    Destructor
		*/
		~IndirectBufferManager();

		/**
		*  @brief
		*    Return a indirect buffer
		*
		*  @param[in] numberOfBytes
		*    Number of bytes the indirect buffer must be able to hold
		*
		*  @return
		*    The requested indirect buffer, don't destroy the instance, null pointer in case of a horrible nightmare apocalypse scenario
		*/
		[[nodiscard]] IndirectBuffer* getIndirectBuffer(uint32_t numberOfBytes);

		/**
		*  @brief
		*    Called pre command buffer execution
		*/
		void onPreCommandBufferExecution();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit IndirectBufferManager(const IndirectBufferManager&) = delete;
		IndirectBufferManager& operator=(const IndirectBufferManager&) = delete;
		void unmapCurrentIndirectBuffer();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<IndirectBuffer> IndirectBuffers;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const IRenderer& mRenderer;
		uint32_t		 mMaximumIndirectBufferSize;	// Maximum indirect buffer size in bytes
		IndirectBuffers	 mFreeIndirectBuffers;
		IndirectBuffers	 mUsedIndirectBuffers;
		IndirectBuffer*	 mCurrentIndirectBuffer;		// Currently filled indirect buffer, can be a null pointer, don't destroy the instance since it's just a reference
		uint32_t		 mPreviouslyRequestedNumberOfBytes;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

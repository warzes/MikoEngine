#pragma once

namespace Rhi
{
/**
*  @brief
*    Abstract buffer manager interface
*
*  @remarks
*    The buffer manager is responsible for managing fine granular instances of
*    - Vertex buffer object ("Rhi::IVertexBuffer")
*    - Index buffer object ("Rhi::IIndexBuffer")
*    - Vertex array object ("Rhi::IVertexArray")
*    - Texture buffer object ("Rhi::ITextureBuffer")
*    - Structured buffer object ("Rhi::IStructuredBuffer")
*    - Indirect buffer object ("Rhi::IIndirectBuffer")
*    - Uniform buffer object ("Rhi::IUniformBuffer")
*
*    Implementations are free to implement a naive 1:1 mapping of a resource to an RHI implementation resource.
*    For AZDO ("Almost Zero Driver Overhead") implementations might allocate a few big RHI implementation resources
*    and manage the granular instances internally. Modern RHI implementations like Vulkan or DirectX 12 are build
*    around the concept that the user just allocates a few big memory heaps and then managing them by their own.
*/
	class IBufferManager : public RefCount<IBufferManager>
	{
	public:
		inline virtual ~IBufferManager() override
		{
		}

		[[nodiscard]] inline IRhi& getRhi() const
		{
			return mRhi;
		}

		// Public virtual Rhi::IBufferManager methods
	public:
		//[-------------------------------------------------------]
		//[ Resource creation                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a vertex buffer object (VBO, "array buffer" in OpenGL terminology) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the vertex buffer, must be valid
		*  @param[in] data
		*    Vertex buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*
		*  @return
		*    The created VBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IVertexBuffer* createVertexBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = 0, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an index buffer object (IBO, "element array buffer" in OpenGL terminology) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the index buffer, must be valid
		*  @param[in] data
		*    Index buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] indexBufferFormat
		*    Index buffer data format
		*
		*  @return
		*    The created IBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IIndexBuffer* createIndexBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = 0, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW, IndexBufferFormat::Enum indexBufferFormat = IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a vertex array instance
		*
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] numberOfVertexBuffers
		*    Number of vertex buffers, having zero vertex buffers is valid
		*  @param[in] vertexBuffers
		*    At least "numberOfVertexBuffers" instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*
		*  @return
		*    The created vertex array instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The created vertex array instance keeps a reference to the vertex buffers used by the vertex array attributes
		*    - It's valid that a vertex array implementation is adding a reference and releasing it again at once
		*      (this means that in the case of not having any more references, a vertex buffer might get destroyed when calling this method)
		*/
		[[nodiscard]] virtual IVertexArray* createVertexArray(const VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const VertexArrayVertexBuffer* vertexBuffers, IIndexBuffer* indexBuffer = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an texture buffer object (TBO) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the texture buffer, must be valid
		*  @param[in] data
		*    Texture buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] textureFormat
		*    Texture buffer data format
		*
		*  @return
		*    The created TBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumTextureBufferSize" is not 0
		*/
		[[nodiscard]] virtual ITextureBuffer* createTextureBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = BufferFlag::SHADER_RESOURCE, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW, TextureFormat::Enum textureFormat = TextureFormat::R32G32B32A32F RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an structured buffer object (SBO) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the structured buffer, must be valid
		*  @param[in] data
		*    Structured buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] numberOfStructureBytes
		*    Number of structure bytes
		*
		*  @return
		*    The created structured buffer instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumStructuredBufferSize" is not 0
		*/
		[[nodiscard]] virtual IStructuredBuffer* createStructuredBuffer(uint32_t numberOfBytes, const void* data, uint32_t bufferFlags, BufferUsage bufferUsage, uint32_t numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an indirect buffer object instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the indirect buffer, must be valid
		*  @param[in] data
		*    Indirect buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indirectBufferFlags
		*    Indirect buffer flags, see "Rhi::IndirectBufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*
		*  @return
		*    The created indirect buffer instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumIndirectBufferSize" is >0
		*/
		[[nodiscard]] virtual IIndirectBuffer* createIndirectBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t indirectBufferFlags = 0, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the uniform buffer, must be valid
		*  @param[in] data
		*    Uniform buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*
		*  @return
		*    The created UBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumUniformBufferSize" is >0
		*    - There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
		*/
		[[nodiscard]] virtual IUniformBuffer* createUniformBuffer(uint32_t numberOfBytes, const void* data = nullptr, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		// Protected methods
	protected:
		inline explicit IBufferManager(IRhi& rhi) :
			mRhi(rhi)
		{
		}

		explicit IBufferManager(const IBufferManager& source) = delete;
		IBufferManager& operator =(const IBufferManager& source) = delete;

		// Private data
	private:
		IRhi& mRhi;	// The owner RHI instance

	};

	typedef SmartRefCount<IBufferManager> IBufferManagerPtr;
} // namespace Rhi
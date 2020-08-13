// rendering hardware interface
#pragma once

#include "RHICore.h"
#include "RHIContext.h"
#include "RHITypes.h"
#include "SamplerStateTypes.h"
#include "RootSignatureTypes.h"
#include "TextureTypes.h"
#include "BlendStateTypes.h"
#include "BufferTypes.h"
#include "VertexArrayTypes.h"
#include "IndexBufferTypes.h"
#include "IndirectBufferTypes.h"
#include "RasterizerStateTypes.h"
#include "DepthStencilStateTypes.h"
#include "PipelineStateTypes.h"
#include "ShaderTypes.h"
#include "QueryType.h"
#include "Core/MemoryPtr.h"
#include "Capabilities.h"
#include "Statistics.h"
#include "IRhi.h"
#include "IShaderLanguage.h"
#include "IResource.h"
#include "IRootSignature.h"
#include "IResourceGroup.h"
#include "IGraphicsProgram.h"
#include "IRenderPass.h"
#include "IQueryPool.h"

namespace Rhi
{
	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/IQueryPool.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract asynchronous query pool interface
	*/
	class IQueryPool : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IQueryPool() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfQueryPools;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner rhi instance
		*/
		inline explicit IQueryPool(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::QUERY_POOL, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedQueryPools;
				++rhi.getStatistics().currentNumberOfQueryPools;
			#endif
		}

		explicit IQueryPool(const IQueryPool&) = delete;
		IQueryPool& operator =(const IQueryPool&) = delete;

	};

	typedef SmartRefCount<IQueryPool> IQueryPoolPtr;




	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/IRenderTarget.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract render target interface
	*/
	class IRenderTarget : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IRenderTarget() override
		{
			mRenderPass.releaseReference();
		}

		/**
		*  @brief
		*    Return the render pass
		*
		*  @return
		*    Render pass, don't release the reference unless you add an own reference
		*/
		[[nodiscard]] inline IRenderPass& getRenderPass() const
		{
			return mRenderPass;
		}

	// Public virtual Rhi::IRenderTarget methods
	public:
		/**
		*  @brief
		*    Return the width and height of the render target
		*
		*  @param[out] width
		*    Receives the width of the render target, guaranteed to be never ever zero
		*  @param[out] height
		*    Receives the height of the render target, guaranteed to be never ever zero
		*/
		virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] renderPass
		*    Render pass to use, the render target keeps a reference to the render pass
		*/
		inline IRenderTarget(ResourceType resourceType, IRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, renderPass.getRhi() RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRenderPass(renderPass)
		{
			mRenderPass.addReference();
		}

		explicit IRenderTarget(const IRenderTarget& source) = delete;
		IRenderTarget& operator =(const IRenderTarget& source) = delete;

	// Private data
	private:
		IRenderPass& mRenderPass;	// Render pass to use, the render target keeps a reference to the render pass

	};

	typedef SmartRefCount<IRenderTarget> IRenderTargetPtr;




	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/ISwapChain.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract render window interface which is used to implement some platform specific functionality regarding render window needed by the swap chain implementation
	*
	*  @remarks
	*    This interface can be used to implement the needed platform specific functionality for a platform which isn't known by the RHI implementation
	*    e.g. the user uses a windowing library (e.g. SDL2) which abstracts the window handling on different windowing platforms (e.g. Win32 or Linux/Wayland)
	*    and the application should run on a windowing platform which isn't supported by the swap chain implementation itself.
	*/
	class IRenderWindow
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IRenderWindow()
		{}

	// Public virtual Rhi::IRendererWindow methods
	public:
		/**
		*  @brief
		*    Return the width and height of the render window
		*
		*  @param[out] width
		*    Width of the render window
		*  @param[out] height
		*    Height of the render window
		*/
		virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const = 0;

		/**
		*  @brief
		*    Present the content of the current back buffer
		*
		*  @note
		*    - Swap of front and back buffer
		*/
		virtual void present() = 0;

	// Protected methods
	protected:
		inline IRenderWindow()
		{}

		explicit IRenderWindow(const IRenderWindow& source) = delete;
		IRenderWindow& operator =(const IRenderWindow& source) = delete;

	};

	/**
	*  @brief
	*    Abstract swap chain interface
	*/
	class ISwapChain : public IRenderTarget
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ISwapChain() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfSwapChains;
			#endif
		}

	// Public virtual Rhi::ISwapChain methods
	public:
		/**
		*  @brief
		*    Return the native window handle
		*
		*  @return
		*    Native window handle the swap chain is using as output window, can be a null handle
		*/
		[[nodiscard]] virtual handle getNativeWindowHandle() const = 0;

		/**
		*  @brief
		*    Set vertical synchronization interval
		*
		*  @param[in] synchronizationInterval
		*    Synchronization interval, >0 if vertical synchronization should be used, else zero
		*/
		virtual void setVerticalSynchronizationInterval(uint32_t synchronizationInterval) = 0;

		/**
		*  @brief
		*    Present the content of the current back buffer
		*
		*  @note
		*    - Swap of front and back buffer
		*/
		virtual void present() = 0;

		/**
		*  @brief
		*    Call this method whenever the size of the native window was changed
		*/
		virtual void resizeBuffers() = 0;

		/**
		*  @brief
		*    Return the current fullscreen state
		*
		*  @return
		*    "true" if fullscreen, else "false"
		*/
		[[nodiscard]] virtual bool getFullscreenState() const = 0;

		/**
		*  @brief
		*    Set the current fullscreen state
		*
		*  @param[in] fullscreen
		*    "true" if fullscreen, else "false"
		*/
		virtual void setFullscreenState(bool fullscreen) = 0;

		/**
		*  @brief
		*    Set an render window instance
		*
		*  @param[in] renderWindow
		*    The render window interface instance, can be a null pointer, if valid the instance must stay valid as long as it's connected to the swap chain instance
		*
		*  @remarks
		*    This method can be used to override the platform specific handling for retrieving window size and doing an buffer swap on the render window (aka present).
		*    An instance can be set when the user don't want that the swap chain itself tempers with the given window handle (the handle might be invalid but non zero)
		*    e.g. the user uses a windowing library (e.g. SDL2) which abstracts the window handling on different windowing platforms (e.g. Win32 or Linux/Wayland) and
		*    the application should run on a windowing platform which isn't supported by the swap chain implementation itself.
		*/
		virtual void setRenderWindow(IRenderWindow* renderWindow) = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*/
		inline explicit ISwapChain(IRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IRenderTarget(ResourceType::SWAP_CHAIN, renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++getRhi().getStatistics().numberOfCreatedSwapChains;
				++getRhi().getStatistics().currentNumberOfSwapChains;
			#endif
		}

		explicit ISwapChain(const ISwapChain& source) = delete;
		ISwapChain& operator =(const ISwapChain& source) = delete;

	};

	typedef SmartRefCount<ISwapChain> ISwapChainPtr;




	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/IFramebuffer.h                       ]
	//[-------------------------------------------------------]
	struct FramebufferAttachment final
	{
		ITexture* texture;
		uint32_t  mipmapIndex;
		uint32_t  layerIndex;	// "slice" in Direct3D terminology, depending on the texture type it's a 2D texture array layer, 3D texture slice or cube map face
		inline FramebufferAttachment() :
			texture(nullptr),
			mipmapIndex(0),
			layerIndex(0)
		{}
		inline FramebufferAttachment(ITexture* _texture, uint32_t _mipmapIndex = 0, uint32_t _layerIndex = 0) :
			texture(_texture),
			mipmapIndex(_mipmapIndex),
			layerIndex(_layerIndex)
		{}
	};

	/**
	*  @brief
	*    Abstract framebuffer (FBO) interface
	*/
	class IFramebuffer : public IRenderTarget
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IFramebuffer() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfFramebuffers;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the framebuffer a reference to the render pass
		*/
		inline explicit IFramebuffer(IRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IRenderTarget(ResourceType::FRAMEBUFFER, renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++getRhi().getStatistics().numberOfCreatedFramebuffers;
				++getRhi().getStatistics().currentNumberOfFramebuffers;
			#endif
		}

		explicit IFramebuffer(const IFramebuffer& source) = delete;
		IFramebuffer& operator =(const IFramebuffer& source) = delete;

	};

	typedef SmartRefCount<IFramebuffer> IFramebufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IBufferManager.h                           ]
	//[-------------------------------------------------------]
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

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IBufferManager() override
		{}

		/**
		*  @brief
		*    Return the owner RHI instance
		*
		*  @return
		*    The owner RHI instance, do not release the returned instance unless you added an own reference to it
		*/
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
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner rhi instance
		*/
		inline explicit IBufferManager(IRhi& rhi) :
			mRhi(rhi)
		{}

		explicit IBufferManager(const IBufferManager& source) = delete;
		IBufferManager& operator =(const IBufferManager& source) = delete;

	// Private data
	private:
		IRhi& mRhi;	// The owner RHI instance

	};

	typedef SmartRefCount<IBufferManager> IBufferManagerPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IVertexArray.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract vertex array object (VAO) interface
	*
	*  @remarks
	*    Encapsulates all the data that is associated with the vertex processor.
	*
	*    Direct3D specifies input stream slot indices within "vertex declaration"/"input layout" without binding them to a particular
	*    vertex buffer object (VBO). Vertex buffers are bound to particular input stream slots. OpenGL "vertex array object" (VAO) has no
	*    concept of input stream slot indices, vertex attributes are directly bound to a particular vertex buffer to feed the data into
	*    the input assembly (IA). To be able to map this interface efficiently to Direct3D as well as OpenGL, this interface has to stick
	*    to the OpenGL "vertex array object"-concept. As a result, we have to directly define vertex buffer objects as data source.
	*/
	class IVertexArray : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IVertexArray() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfVertexArrays;
			#endif
		}

		/**
		*  @brief
		*    Return the unique compact vertex array ID
		*
		*  @return
		*    The unique compact vertex array ID
		*/
		inline uint16_t getId() const
		{
			return mId;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		inline IVertexArray(IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::VERTEX_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mId(id)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedVertexArrays;
				++rhi.getStatistics().currentNumberOfVertexArrays;
			#endif
		}

		explicit IVertexArray(const IVertexArray& source) = delete;
		IVertexArray& operator =(const IVertexArray& source) = delete;

	// Private data
	private:
		uint16_t mId;
	};

	typedef SmartRefCount<IVertexArray> IVertexArrayPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IBuffer.h                                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract buffer interface
	*/
	class IBuffer : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IBuffer() override
		{}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*
		*  @note
		*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
		*/
		inline explicit IBuffer(ResourceType resourceType RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IBuffer(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		explicit IBuffer(const IBuffer& source) = delete;
		IBuffer& operator =(const IBuffer& source) = delete;

	};

	typedef SmartRefCount<IBuffer> IBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IVertexBuffer.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract vertex buffer object (VBO, "array buffer" in OpenGL terminology) interface
	*/
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




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IIndexBuffer.h                             ]
	//[-------------------------------------------------------]
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
			#ifdef SE_STATISTICS
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
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedIndexBuffers;
				++rhi.getStatistics().currentNumberOfIndexBuffers;
			#endif
		}

		explicit IIndexBuffer(const IIndexBuffer& source) = delete;
		IIndexBuffer& operator =(const IIndexBuffer& source) = delete;

	};

	typedef SmartRefCount<IIndexBuffer> IIndexBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/ITextureBuffer.h                           ]
	//[-------------------------------------------------------]
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




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IStructuredBuffer.h                        ]
	//[-------------------------------------------------------]
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
			#ifdef SE_STATISTICS
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
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedStructuredBuffers;
				++rhi.getStatistics().currentNumberOfStructuredBuffers;
			#endif
		}

		explicit IStructuredBuffer(const IStructuredBuffer& source) = delete;
		IStructuredBuffer& operator =(const IStructuredBuffer& source) = delete;

	};

	typedef SmartRefCount<IStructuredBuffer> IStructuredBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IIndirectBuffer.h                          ]
	//[-------------------------------------------------------]
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
			#ifdef SE_STATISTICS
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
			{}
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
			{}
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
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedIndirectBuffers;
				++rhi.getStatistics().currentNumberOfIndirectBuffers;
			#endif
		}

		explicit IIndirectBuffer(const IIndirectBuffer& source) = delete;
		IIndirectBuffer& operator =(const IIndirectBuffer& source) = delete;

	};

	typedef SmartRefCount<IIndirectBuffer> IIndirectBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IUniformBuffer.h                           ]
	//[-------------------------------------------------------]
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
			#ifdef SE_STATISTICS
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
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedUniformBuffers;
				++rhi.getStatistics().currentNumberOfUniformBuffers;
			#endif
		}

		explicit IUniformBuffer(const IUniformBuffer& source) = delete;
		IUniformBuffer& operator =(const IUniformBuffer& source) = delete;

	};

	typedef SmartRefCount<IUniformBuffer> IUniformBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITextureManager.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract texture manager interface
	*
	*  @remarks
	*    The texture manager is responsible for managing fine granular instances of
	*    - 1D texture ("Rhi::ITexture1D")
	*    - 1D texture array ("Rhi::ITexture1DArray")
	*    - 2D texture ("Rhi::ITexture2D")
	*    - 2D texture array ("Rhi::ITexture2DArray")
	*    - 3D texture ("Rhi::ITexture3D")
	*    - Cube texture ("Rhi::ITextureCube")
	*    - Cube texture array ("Rhi::ITextureCubeArray")
	*/
	class ITextureManager : public RefCount<ITextureManager>
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureManager() override
		{}

		/**
		*  @brief
		*    Return the owner RHI instance
		*
		*  @return
		*    The owner RHI instance, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IRhi& getRhi() const
		{
			return mRhi;
		}

	// Public virtual Rhi::ITextureManager methods
	public:
		//[-------------------------------------------------------]
		//[ Resource creation                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a 1D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 1D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The following texture data layout is expected: Mip0, Mip1, Mip2, Mip3 ...
		*/
		[[nodiscard]] virtual ITexture1D* createTexture1D(uint32_t width, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 1D array texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 1D array texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture array data consists of a sequence of texture slices. Each the texture slice data of a single texture slice has to
		*    be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOf1DTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITexture1DArray* createTexture1DArray(uint32_t width, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 2D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture data format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*  @param[in] optimizedTextureClearValue
		*    Optional optimized texture clear value
		*
		*  @return
		*    The created 2D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The following texture data layout is expected: Mip0, Mip1, Mip2, Mip3 ...
		*/
		[[nodiscard]] virtual ITexture2D* createTexture2D(uint32_t width, uint32_t height, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT, uint8_t numberOfMultisamples = 1, const OptimizedTextureClearValue* optimizedTextureClearValue = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 2D array texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 2D array texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture array data consists of a sequence of texture slices. Each the texture slice data of a single texture slice has to
		*    be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5 (= positive X, negative X, positive Y, negative Y, positive Z, negative Z)
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOf2DTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITexture2DArray* createTexture2DArray(uint32_t width, uint32_t height, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 3D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] depth
		*    Texture depth, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 3D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5 (= positive X, negative X, positive Y, negative Y, positive Z, negative Z)
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*/
		[[nodiscard]] virtual ITexture3D* createTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a cube texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created cube texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Face0, Face1, Face2, Face3, Face4, Face5
		*    - Mip1: Face0, Face1, Face2, Face3, Face4, Face5
		*    (DDS-texture layout is using face-major order)
		*/
		[[nodiscard]] virtual ITextureCube* createTextureCube(uint32_t width, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a cube texture array instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created cube texture array instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Face0, Face1, Face2, Face3, Face4, Face5
		*    - Mip1: Face0, Face1, Face2, Face3, Face4, Face5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfCubeTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITextureCubeArray* createTextureCubeArray(uint32_t width, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ITextureManager(IRhi& rhi) :
			mRhi(rhi)
		{}

		explicit ITextureManager(const ITextureManager& source) = delete;
		ITextureManager& operator =(const ITextureManager& source) = delete;

	// Private data
	private:
		IRhi& mRhi;	// The owner RHI instance

	};

	typedef SmartRefCount<ITextureManager> ITextureManagerPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture.h                                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract texture interface
	*/
	class ITexture : public IResource
	{

	// Public static methods
	public:
		/**
		*  @brief
		*    Calculate the number of mipmaps for a 1D texture
		*
		*  @param[in] width
		*    Texture width
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width)
		{
			// If "std::log2()" wouldn't be supported, we could write the following: "return static_cast<uint32_t>(1 + std::floor(std::log(width) / 0.69314718055994529));"
			// -> log2(x) = log(x) / log(2)
			// -> log(2) = 0.69314718055994529
			return static_cast<uint32_t>(1 + std::floor(std::log2(width)));
		}

		/**
		*  @brief
		*    Calculate the number of mipmaps for a 2D texture
		*
		*  @param[in] width
		*    Texture width
		*  @param[in] height
		*    Texture height
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width, uint32_t height)
		{
			return getNumberOfMipmaps((width > height) ? width : height);
		}

		/**
		*  @brief
		*    Calculate the number of mipmaps for a 3D texture
		*
		*  @param[in] width
		*    Texture width
		*  @param[in] height
		*    Texture height
		*  @param[in] depth
		*    Texture depth
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width, uint32_t height, uint32_t depth)
		{
			return getNumberOfMipmaps(width, (height > depth) ? height : depth);
		}

		/**
		*  @brief
		*    Calculate the half size
		*
		*  @param[in] size
		*    Size
		*
		*  @return
		*    Half size, 1 as minimum
		*/
		[[nodiscard]] static inline uint32_t getHalfSize(uint32_t size)
		{
			size = (size >> 1);	// /= 2
			return (0u == size) ? 1u : size;
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width)
		{
			if (0u != mipmapIndex)
			{
				width = static_cast<uint32_t>(static_cast<float>(width) / std::exp2f(static_cast<float>(mipmapIndex)));
				if (0u == width)
				{
					width = 1u;
				}
			}
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*  @param[in,out] height
		*    Texture height
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width, uint32_t& height)
		{
			getMipmapSize(mipmapIndex, width);
			getMipmapSize(mipmapIndex, height);
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*  @param[in,out] height
		*    Texture height
		*  @param[in,out] depth
		*    Texture depth
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width, uint32_t& height, uint32_t& depth)
		{
			getMipmapSize(mipmapIndex, width);
			getMipmapSize(mipmapIndex, height);
			getMipmapSize(mipmapIndex, depth);
		}

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture() override
		{}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline ITexture(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		explicit ITexture(const ITexture& source) = delete;
		ITexture& operator =(const ITexture& source) = delete;

	};

	typedef SmartRefCount<ITexture> ITexturePtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture1D.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 1D texture interface
	*
	*  @remarks
	*    Common use cases for 1D textures:
	*    - Illuminating Engineering Society (IES) light profile as 1D texture
	*    - 1D lens color map for screen space lens flares
	*    - 1D screen space ambient occlusion sample kernel map
	*    - 1D light gradient map for cel—shading (a style of cartoon rendering)
	*/
	class ITexture1D : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture1D() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture1Ds;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*/
		inline ITexture1D(IRhi& rhi, uint32_t width RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_1D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture1Ds;
				++rhi.getStatistics().currentNumberOfTexture1Ds;
			#endif
		}

		explicit ITexture1D(const ITexture1D& source) = delete;
		ITexture1D& operator =(const ITexture1D& source) = delete;

	// Private data
	private:
		uint32_t mWidth;	// The width of the texture

	};

	typedef SmartRefCount<ITexture1D> ITexture1DPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture1DArray.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 1D array texture interface
	*
	*  @note
	*    - Common use case for 1D texture array: An 1D array texture is for example useful for storing multiple 1D Illuminating Engineering Society (IES) light profiles packed together in a single texture in order to avoid RHI state changes
	*/
	class ITexture1DArray : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture1DArray() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture1DArrays;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the number of slices
		*
		*  @return
		*    The number of slices
		*/
		[[nodiscard]] inline uint32_t getNumberOfSlices() const
		{
			return mNumberOfSlices;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*/
		inline ITexture1DArray(IRhi& rhi, uint32_t width, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_1D_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mNumberOfSlices(numberOfSlices)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture1DArrays;
				++rhi.getStatistics().currentNumberOfTexture1DArrays;
			#endif
		}

		explicit ITexture1DArray(const ITexture1DArray& source) = delete;
		ITexture1DArray& operator =(const ITexture1DArray& source) = delete;

	// Private data
	private:
		uint32_t mWidth;			// The width of the texture
		uint32_t mNumberOfSlices;	// The number of slices

	};

	typedef SmartRefCount<ITexture1DArray> ITexture1DArrayPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture2D.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 2D texture interface
	*
	*  @note
	*    - Common use case for 2D texture: Pretty much every standard 2D texture
	*/
	class ITexture2D : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture2D() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture2Ds;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the height of the texture
		*
		*  @return
		*    The height of the texture
		*/
		[[nodiscard]] inline uint32_t getHeight() const
		{
			return mHeight;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*/
		inline ITexture2D(IRhi& rhi, uint32_t width, uint32_t height RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_2D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture2Ds;
				++rhi.getStatistics().currentNumberOfTexture2Ds;
			#endif
		}

		explicit ITexture2D(const ITexture2D& source) = delete;
		ITexture2D& operator =(const ITexture2D& source) = delete;

	// Private data
	private:
		uint32_t mWidth;	// The width of the texture
		uint32_t mHeight;	// The height of the texture

	};

	typedef SmartRefCount<ITexture2D> ITexture2DPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture2DArray.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 2D array texture interface
	*
	*  @remarks
	*    Common use cases for 2D texture arrays:
	*    - The cascade 2D textures of shadow mapping
	*    - The detail layer 2D textures (grass, stone etc.) of a terrain
	*    - Decal 2D textures
	*/
	class ITexture2DArray : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture2DArray() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture2DArrays;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the height of the texture
		*
		*  @return
		*    The height of the texture
		*/
		[[nodiscard]] inline uint32_t getHeight() const
		{
			return mHeight;
		}

		/**
		*  @brief
		*    Return the number of slices
		*
		*  @return
		*    The number of slices
		*/
		[[nodiscard]] inline uint32_t getNumberOfSlices() const
		{
			return mNumberOfSlices;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*/
		inline ITexture2DArray(IRhi& rhi, uint32_t width, uint32_t height, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_2D_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height),
			mNumberOfSlices(numberOfSlices)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture2DArrays;
				++rhi.getStatistics().currentNumberOfTexture2DArrays;
			#endif
		}

		explicit ITexture2DArray(const ITexture2DArray& source) = delete;
		ITexture2DArray& operator =(const ITexture2DArray& source) = delete;

	// Private data
	private:
		uint32_t mWidth;			// The width of the texture
		uint32_t mHeight;			// The height of the texture
		uint32_t mNumberOfSlices;	// The number of slices

	};

	typedef SmartRefCount<ITexture2DArray> ITexture2DArrayPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture3D.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 3D texture interface
	*
	*  @remarks
	*    Common use cases for 3D textures:
	*    - 3D color correction lookup table (LUT)
	*    - Volume rendering (medical area and nowadays in games as well)
	*    - Light clusters 3D map
	*/
	class ITexture3D : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture3D() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture3Ds;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the height of the texture
		*
		*  @return
		*    The height of the texture
		*/
		[[nodiscard]] inline uint32_t getHeight() const
		{
			return mHeight;
		}

		/**
		*  @brief
		*    Return the depth of the texture
		*
		*  @return
		*    The depth of the texture
		*/
		[[nodiscard]] inline uint32_t getDepth() const
		{
			return mDepth;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*  @param[in] depth
		*    The depth of the texture
		*/
		inline ITexture3D(IRhi& rhi, uint32_t width, uint32_t height, uint32_t depth RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_3D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height),
			mDepth(depth)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture3Ds;
				++rhi.getStatistics().currentNumberOfTexture3Ds;
			#endif
		}

		explicit ITexture3D(const ITexture3D& source) = delete;
		ITexture3D& operator =(const ITexture3D& source) = delete;

	// Private data
	private:
		uint32_t mWidth;	// The width of the texture
		uint32_t mHeight;	// The height of the texture
		uint32_t mDepth;	// The depth of the texture

	};

	typedef SmartRefCount<ITexture3D> ITexture3DPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITextureCube.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract cube texture interface
	*
	*  @note
	*    - Common use case for cube texture: Environment cube maps
	*/
	class ITextureCube : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureCube() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTextureCubes;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*/
		inline ITextureCube(IRhi& rhi, uint32_t width RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_CUBE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTextureCubes;
				++rhi.getStatistics().currentNumberOfTextureCubes;
			#endif
		}

		explicit ITextureCube(const ITextureCube& source) = delete;
		ITextureCube& operator =(const ITextureCube& source) = delete;

	// Private data
	private:
		uint32_t mWidth;	// The width of the texture

	};

	typedef SmartRefCount<ITextureCube> ITextureCubePtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITextureCubeArray.h                       ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract cube texture array interface
	*
	*  @note
	*    - Common use case for cube texture array: Multiple environment cube maps (created e.g. via environment probes)
	*/
	class ITextureCubeArray : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureCubeArray() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTextureCubeArrays;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the number of slices
		*
		*  @return
		*    The number of slices
		*/
		[[nodiscard]] inline uint32_t getNumberOfSlices() const
		{
			return mNumberOfSlices;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*/
		inline ITextureCubeArray(IRhi& rhi, uint32_t width, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_CUBE_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mNumberOfSlices(numberOfSlices)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTextureCubeArrays;
				++rhi.getStatistics().currentNumberOfTextureCubeArrays;
			#endif
		}

		explicit ITextureCubeArray(const ITextureCubeArray& source) = delete;
		ITextureCubeArray& operator =(const ITextureCubeArray& source) = delete;

	// Private data
	private:
		uint32_t mWidth;			// The width of the texture
		uint32_t mNumberOfSlices;	// The number of slices

	};

	typedef SmartRefCount<ITextureCubeArray> ITextureCubeArrayPtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/IState.h                                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract state interface
	*/
	class IState : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IState() override
		{}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IState(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		explicit IState(const IState& source) = delete;
		IState& operator =(const IState& source) = delete;

	};

	typedef SmartRefCount<IState> IStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/IPipelineState.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract pipeline state interface
	*/
	class IPipelineState : public IState
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IPipelineState() override
		{}

		/**
		*  @brief
		*    Return the unique compact graphics or compute pipeline state ID; one internal ID pool per concrete pipeline state type
		*
		*  @return
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline uint16_t getId() const
		{
			return mId;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] id
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline IPipelineState(ResourceType resourceType, IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IState(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mId(id)
		{}

		explicit IPipelineState(const IPipelineState& source) = delete;
		IPipelineState& operator =(const IPipelineState& source) = delete;

	// Private data
	private:
		uint16_t mId;
	};

	typedef SmartRefCount<IPipelineState> IPipelineStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/IGraphicsPipelineState.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract graphics pipeline state interface
	*/
	class IGraphicsPipelineState : public IPipelineState
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IGraphicsPipelineState() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfGraphicsPipelineStates;
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
		*  @param[in] id
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline IGraphicsPipelineState(IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IPipelineState(ResourceType::GRAPHICS_PIPELINE_STATE, rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedGraphicsPipelineStates;
				++rhi.getStatistics().currentNumberOfGraphicsPipelineStates;
			#endif
		}

		explicit IGraphicsPipelineState(const IGraphicsPipelineState& source) = delete;
		IGraphicsPipelineState& operator =(const IGraphicsPipelineState& source) = delete;

	};

	typedef SmartRefCount<IGraphicsPipelineState> IGraphicsPipelineStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/IComputePipelineState.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract compute pipeline state interface
	*/
	class IComputePipelineState : public IPipelineState
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IComputePipelineState() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfComputePipelineStates;
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
		*  @param[in] id
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline IComputePipelineState(IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IPipelineState(ResourceType::COMPUTE_PIPELINE_STATE, rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedComputePipelineStates;
				++rhi.getStatistics().currentNumberOfComputePipelineStates;
			#endif
		}

		explicit IComputePipelineState(const IComputePipelineState& source) = delete;
		IGraphicsPipelineState& operator =(const IComputePipelineState& source) = delete;

	};

	typedef SmartRefCount<IComputePipelineState> IComputePipelineStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/ISamplerState.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract sampler state interface
	*/
	class ISamplerState : public IState
	{

	// Public static methods
	public:
		/**
		*  @brief
		*    Return the default sampler state
		*
		*  @return
		*    The default sampler state, see "Rhi::SamplerState" for the default values
		*/
		[[nodiscard]] static inline const SamplerState& getDefaultSamplerState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)

			// Direct3D 11 "D3D11_SAMPLER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476207%28v=vs.85%29.aspx

			// The Direct3D 10 documentation is buggy: (online and within the "Microsoft DirectX SDK (June 2010)"-SDK, checked it)
			//   - "D3D10_SAMPLER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172415%28v=vs.85%29.aspx
			//     -> Says "Default filter is Min_Mag_Mip_Point"
			//   - "ID3D10Device::VSSetSamplers method"-documentation at MSDN: msdn.microsoft.com/en-us/library/windows/desktop/bb173627(v=vs.85).aspx
			//     -> Says "Default filter is Min_Mag_Mip_Linear"
			//   -> When testing the behaviour, it "looks like" Min_Mag_Mip_Linear is used

			// Direct3D 9 "D3DSAMPLERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172602%28v=vs.85%29.aspx

			// OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			static constexpr SamplerState SAMPLER_STATE =
			{																				//	Direct3D 11					Direct3D 10						Direct3D 9				OpenGL
				FilterMode::MIN_MAG_MIP_LINEAR,	// filter (Rhi::FilterMode)					"MIN_MAG_MIP_LINEAR"			"MIN_MAG_MIP_LINEAR"			"MIN_MAG_MIP_POINT"		"MIN_POINT_MAG_MIP_LINEAR"
				TextureAddressMode::CLAMP,		// addressU (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				TextureAddressMode::CLAMP,		// addressV (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				TextureAddressMode::CLAMP,		// addressW (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				0.0f,							// mipLodBias (float)						"0.0f"							"0.0f"							"0.0f"					"0.0f"
				16,								// maxAnisotropy (uint32_t)					"16"							"16"							"1"						"1"
				ComparisonFunc::NEVER,			// comparisonFunc (Rhi::ComparisonFunc)		"NEVER"							"NEVER"							<unsupported>			"LESS_EQUAL"
				{
					0.0f,						// borderColor[0] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f,						// borderColor[1] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f,						// borderColor[2] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f						// borderColor[3] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
				},
				-3.402823466e+38f,				// minLod (float) - Default: -FLT_MAX		"-3.402823466e+38F (-FLT_MAX)"	"-3.402823466e+38F (-FLT_MAX)"	<unsupported>			"-1000.0f"
				3.402823466e+38f				// maxLod (float) - Default: FLT_MAX		"3.402823466e+38F (FLT_MAX)"	"3.402823466e+38F (FLT_MAX)"	"0.0f"					"1000.0f"
			};
			return SAMPLER_STATE;
		}

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ISamplerState() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfSamplerStates;
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
		inline explicit ISamplerState(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IState(ResourceType::SAMPLER_STATE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedSamplerStates;
				++rhi.getStatistics().currentNumberOfSamplerStates;
			#endif
		}

		explicit ISamplerState(const ISamplerState& source) = delete;
		ISamplerState& operator =(const ISamplerState& source) = delete;

	};

	typedef SmartRefCount<ISamplerState> ISamplerStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IShader.h                                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract shader interface
	*/
	class IShader : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IShader() override
		{}

	// Public virtual Rhi::IShader methods
	public:
		/**
		*  @brief
		*    Return the name of the shader language the shader is using
		*
		*  @return
		*    The ASCII name of the shader language the shader is using (for example "GLSL" or "HLSL"), always valid
		*
		*  @note
		*    - Do not free the memory the returned pointer is pointing to
		*/
		[[nodiscard]] virtual const char* getShaderLanguageName() const = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    Resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IShader(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		explicit IShader(const IShader& source) = delete;
		IShader& operator =(const IShader& source) = delete;

	};

	typedef SmartRefCount<IShader> IShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IVertexShader.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract vertex shader (VS) interface
	*/
	class IVertexShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IVertexShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfVertexShaders;
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
		inline explicit IVertexShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::VERTEX_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedVertexShaders;
				++rhi.getStatistics().currentNumberOfVertexShaders;
			#endif
		}

		explicit IVertexShader(const IVertexShader& source) = delete;
		IVertexShader& operator =(const IVertexShader& source) = delete;

	};

	typedef SmartRefCount<IVertexShader> IVertexShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/ITessellationControlShader.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract tessellation control shader (TCS, "hull shader" in Direct3D terminology) interface
	*/
	class ITessellationControlShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITessellationControlShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTessellationControlShaders;
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
		inline explicit ITessellationControlShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TESSELLATION_CONTROL_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTessellationControlShaders;
				++rhi.getStatistics().currentNumberOfTessellationControlShaders;
			#endif
		}

		explicit ITessellationControlShader(const ITessellationControlShader& source) = delete;
		ITessellationControlShader& operator =(const ITessellationControlShader& source) = delete;

	};

	typedef SmartRefCount<ITessellationControlShader> ITessellationControlShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/ITessellationEvaluationShader.h            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) interface
	*/
	class ITessellationEvaluationShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITessellationEvaluationShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTessellationEvaluationShaders;
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
		inline explicit ITessellationEvaluationShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TESSELLATION_EVALUATION_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTessellationEvaluationShaders;
				++rhi.getStatistics().currentNumberOfTessellationEvaluationShaders;
			#endif
		}

		explicit ITessellationEvaluationShader(const ITessellationEvaluationShader& source) = delete;
		ITessellationEvaluationShader& operator =(const ITessellationEvaluationShader& source) = delete;

	};

	typedef SmartRefCount<ITessellationEvaluationShader> ITessellationEvaluationShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IGeometryShader.h                          ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract geometry shader (GS) interface
	*/
	class IGeometryShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IGeometryShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfGeometryShaders;
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
		inline explicit IGeometryShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::GEOMETRY_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedGeometryShaders;
				++rhi.getStatistics().currentNumberOfGeometryShaders;
			#endif
		}

		explicit IGeometryShader(const IGeometryShader& source) = delete;
		IGeometryShader& operator =(const IGeometryShader& source) = delete;

	};

	typedef SmartRefCount<IGeometryShader> IGeometryShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IFragmentShader.h                          ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract fragment shader (FS, "pixel shader" in Direct3D terminology) interface
	*/
	class IFragmentShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IFragmentShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfFragmentShaders;
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
		inline explicit IFragmentShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::FRAGMENT_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedFragmentShaders;
				++rhi.getStatistics().currentNumberOfFragmentShaders;
			#endif
		}

		explicit IFragmentShader(const IFragmentShader& source) = delete;
		IFragmentShader& operator =(const IFragmentShader& source) = delete;

	};

	typedef SmartRefCount<IFragmentShader> IFragmentShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/ITaskShader.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract task shader (TS) interface
	*/
	class ITaskShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITaskShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTaskShaders;
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
		inline explicit ITaskShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TASK_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTaskShaders;
				++rhi.getStatistics().currentNumberOfTaskShaders;
			#endif
		}

		explicit ITaskShader(const ITaskShader& source) = delete;
		ITaskShader& operator =(const ITaskShader& source) = delete;

	};

	typedef SmartRefCount<ITaskShader> ITaskShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IMeshShader.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract mesh shader (MS) interface
	*/
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
			#ifdef SE_STATISTICS
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
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedMeshShaders;
				++rhi.getStatistics().currentNumberOfMeshShaders;
			#endif
		}

		explicit IMeshShader(const IMeshShader& source) = delete;
		IMeshShader& operator =(const IMeshShader& source) = delete;

	};

	typedef SmartRefCount<IMeshShader> IMeshShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IComputeShader.h                           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract compute shader (CS) interface
	*/
	class IComputeShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IComputeShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfComputeShaders;
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
		inline explicit IComputeShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::COMPUTE_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedComputeShaders;
				++rhi.getStatistics().currentNumberOfComputeShaders;
			#endif
		}

		explicit IComputeShader(const IComputeShader& source) = delete;
		IComputeShader& operator =(const IComputeShader& source) = delete;

	};

	typedef SmartRefCount<IComputeShader> IComputeShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/CommandBuffer.h                            ]
	//[-------------------------------------------------------]
	enum class CommandDispatchFunctionIndex : uint8_t
	{
		// Command buffer
		EXECUTE_COMMAND_BUFFER = 0,
		// Graphics
		SET_GRAPHICS_ROOT_SIGNATURE,
		SET_GRAPHICS_PIPELINE_STATE,
		SET_GRAPHICS_RESOURCE_GROUP,
		SET_GRAPHICS_VERTEX_ARRAY,			// Input-assembler (IA) stage
		SET_GRAPHICS_VIEWPORTS,				// Rasterizer (RS) stage
		SET_GRAPHICS_SCISSOR_RECTANGLES,	// Rasterizer (RS) stage
		SET_GRAPHICS_RENDER_TARGET,			// Output-merger (OM) stage
		CLEAR_GRAPHICS,
		DRAW_GRAPHICS,
		DRAW_INDEXED_GRAPHICS,
		DRAW_MESH_TASKS,
		// Compute
		SET_COMPUTE_ROOT_SIGNATURE,
		SET_COMPUTE_PIPELINE_STATE,
		SET_COMPUTE_RESOURCE_GROUP,
		DISPATCH_COMPUTE,
		// Resource
		SET_TEXTURE_MINIMUM_MAXIMUM_MIPMAP_INDEX,
		RESOLVE_MULTISAMPLE_FRAMEBUFFER,
		COPY_RESOURCE,
		GENERATE_MIPMAPS,
		// Query
		RESET_QUERY_POOL,
		BEGIN_QUERY,
		END_QUERY,
		WRITE_TIMESTAMP_QUERY,
		// Debug
		SET_DEBUG_MARKER,
		BEGIN_DEBUG_EVENT,
		END_DEBUG_EVENT,
		// Done
		NUMBER_OF_FUNCTIONS
	};

	typedef void (*ImplementationDispatchFunction)(const void*, IRhi& rhi);
	typedef void* CommandPacket;
	typedef const void* ConstCommandPacket;

	// Global functions
	namespace CommandPacketHelper
	{
		static constexpr uint32_t OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX		= 0u;
		static constexpr uint32_t OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION	= OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX + sizeof(uint32_t);
		static constexpr uint32_t OFFSET_COMMAND							= OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION + sizeof(uint32_t);	// Don't use "sizeof(CommandDispatchFunctionIndex)" instead of "sizeof(uint32_t)" so we have a known alignment

		template <typename T>
		[[nodiscard]] inline uint32_t getNumberOfBytes(uint32_t numberOfAuxiliaryBytes)
		{
			return OFFSET_COMMAND + sizeof(T) + numberOfAuxiliaryBytes;
		}

		[[nodiscard]] inline uint32_t getNextCommandPacketByteIndex(const CommandPacket commandPacket)
		{
			return *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(commandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX);
		}

		[[nodiscard]] inline uint32_t getNextCommandPacketByteIndex(const ConstCommandPacket constCommandPacket)
		{
			return *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX);
		}

		inline void storeNextCommandPacketByteIndex(const CommandPacket commandPacket, uint32_t nextPacketByteIndex)
		{
			*reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX) = nextPacketByteIndex;
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex* getCommandDispatchFunctionIndex(const CommandPacket commandPacket)
		{
			return reinterpret_cast<CommandDispatchFunctionIndex*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION);
		}

		[[nodiscard]] inline const CommandDispatchFunctionIndex* getCommandDispatchFunctionIndex(const ConstCommandPacket constCommandPacket)
		{
			return reinterpret_cast<const CommandDispatchFunctionIndex*>(reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION);
		}

		inline void storeImplementationDispatchFunctionIndex(const CommandPacket commandPacket, CommandDispatchFunctionIndex commandDispatchFunctionIndex)
		{
			*getCommandDispatchFunctionIndex(commandPacket) = commandDispatchFunctionIndex;
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex loadCommandDispatchFunctionIndex(const CommandPacket commandPacket)
		{
			return *getCommandDispatchFunctionIndex(commandPacket);
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex loadCommandDispatchFunctionIndex(const ConstCommandPacket constCommandPacket)
		{
			return *getCommandDispatchFunctionIndex(constCommandPacket);
		}

		template <typename T>
		[[nodiscard]] inline T* getCommand(const CommandPacket commandPacket)
		{
			return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_COMMAND);
		}

		[[nodiscard]] inline const void* loadCommand(const CommandPacket commandPacket)
		{
			return reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_COMMAND;
		}

		[[nodiscard]] inline const void* loadCommand(const ConstCommandPacket constCommandPacket)
		{
			return reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_COMMAND;
		}

		/**
		*  @brief
		*    Return auxiliary memory address of the given command; returned memory address is considered unstable and might change as soon as another command is added
		*/
		template <typename T>
		[[nodiscard]] inline uint8_t* getAuxiliaryMemory(T* command)
		{
			return reinterpret_cast<uint8_t*>(command) + sizeof(T);
		}

		/**
		*  @brief
		*    Return auxiliary memory address of the given command; returned memory address is considered unstable and might change as soon as another command is added
		*/
		template <typename T>
		[[nodiscard]] inline const uint8_t* getAuxiliaryMemory(const T* command)
		{
			return reinterpret_cast<const uint8_t*>(command) + sizeof(T);
		}

	}

	/**
	*  @brief
	*    Command buffer
	*
	*  @remarks
	*    Basing on
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering – Part 1" by Stefan Reinalter from November 6, 2014 - https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering – Part 2" by Stefan Reinalter from November 13, 2014 - http://molecularmusings.wordpress.com/2014/11/13/stateless-layered-multi-threaded-rendering-part-2-stateless-api-design/
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering – Part 3" by Stefan Reinalter from December 16, 2014 - http://molecularmusings.wordpress.com/2014/12/16/stateless-layered-multi-threaded-rendering-part-3-api-design-details/
	*    - "realtimecollisiondetection.net – the blog" - "Order your graphics draw calls around!" by Christer Ericson from October 3, 2008 - http://realtimecollisiondetection.net/blog/?p=86
	*    but without a key inside the more general command buffer. Sorting is a job of a more high level construct like a render queue which also automatically will perform
	*    batching and instancing. Also the memory management is much simplified to be cache friendly.
	*
	*  @note
	*    - The commands are stored as a flat contiguous array to be cache friendly
	*    - Each command can have an additional auxiliary buffer, e.g. to store uniform buffer data to submit to the RHI
	*    - It's valid to record a command buffer only once, and submit it multiple times to the RHI
	*/
	class CommandBuffer final
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline CommandBuffer() :
			mCommandPacketBufferNumberOfBytes(0),
			mCommandPacketBuffer(nullptr),
			mPreviousCommandPacketByteIndex(~0u),
			mCurrentCommandPacketByteIndex(0)
			#ifdef SE_STATISTICS
				, mNumberOfCommands(0)
			#endif
		{}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~CommandBuffer()
		{
			delete [] mCommandPacketBuffer;
		}

		/**
		*  @brief
		*    Return whether or not the command buffer is empty
		*
		*  @return
		*    "true" if the command buffer is empty, else "false"
		*/
		[[nodiscard]] inline bool isEmpty() const
		{
			return (~0u == mPreviousCommandPacketByteIndex);
		}

		#ifdef SE_STATISTICS
			/**
			*  @brief
			*    Return the number of commands inside the command buffer
			*
			*  @return
			*    The number of commands inside the command buffer
			*
			*  @note
			*    - Counting the number of commands inside the command buffer is only a debugging feature and not used in optimized builds
			*/
			[[nodiscard]] inline uint32_t getNumberOfCommands() const
			{
				return mNumberOfCommands;
			}
		#endif

		/**
		*  @brief
		*    Return the command packet buffer
		*
		*  @return
		*    The command packet buffer, can be a null pointer, don't destroy the instance
		*
		*  @note
		*    - Internal, don't access the method if you don't have to
		*/
		[[nodiscard]] inline const uint8_t* getCommandPacketBuffer() const
		{
			return (~0u != mPreviousCommandPacketByteIndex) ? mCommandPacketBuffer : nullptr;
		}

		/**
		*  @brief
		*    Clear the command buffer
		*/
		inline void clear()
		{
			mPreviousCommandPacketByteIndex = ~0u;
			mCurrentCommandPacketByteIndex = 0;
			#ifdef SE_STATISTICS
				mNumberOfCommands = 0;
			#endif
		}

		/**
		*  @brief
		*    Add command
		*
		*  @param[in] numberOfAuxiliaryBytes
		*    Optional number of auxiliary bytes, e.g. to store uniform buffer data to submit to the RHI
		*
		*  @return
		*    Pointer to the added command, only null pointer in case of apocalypse, don't destroy the memory
		*/
		template <typename U>
		[[nodiscard]] U* addCommand(uint32_t numberOfAuxiliaryBytes = 0)
		{
			// How many command package buffer bytes are consumed by the command to add?
			const uint32_t numberOfCommandBytes = CommandPacketHelper::getNumberOfBytes<U>(numberOfAuxiliaryBytes);

			// 4294967295 is the maximum value of an "uint32_t"-type: Check for overflow
			// -> We use the magic number here to avoid "std::numeric_limits::max()" usage
			RHI_ASSERT((static_cast<uint64_t>(mCurrentCommandPacketByteIndex) + numberOfCommandBytes) < 4294967295u, "Invalid current command packet byte index")

			// Grow command packet buffer, if required
			if (mCommandPacketBufferNumberOfBytes < mCurrentCommandPacketByteIndex + numberOfCommandBytes)
			{
				// Allocate new memory, grow using a known value but do also add the number of bytes consumed by the current command to add (many auxiliary bytes might be requested)
				const uint32_t newCommandPacketBufferNumberOfBytes = mCommandPacketBufferNumberOfBytes + NUMBER_OF_BYTES_TO_GROW + numberOfCommandBytes;
				uint8_t* newCommandPacketBuffer = new uint8_t[newCommandPacketBufferNumberOfBytes];

				// Copy over current command package buffer content and free it, if required
				if (nullptr != mCommandPacketBuffer)
				{
					memcpy(newCommandPacketBuffer, mCommandPacketBuffer, mCommandPacketBufferNumberOfBytes);
					delete [] mCommandPacketBuffer;
				}

				// Finalize
				mCommandPacketBuffer = newCommandPacketBuffer;
				mCommandPacketBufferNumberOfBytes = newCommandPacketBufferNumberOfBytes;
			}

			// Get command package for the new command
			CommandPacket commandPacket = &mCommandPacketBuffer[mCurrentCommandPacketByteIndex];

			// Setup previous and current command package
			if (~0u != mPreviousCommandPacketByteIndex)
			{
				CommandPacketHelper::storeNextCommandPacketByteIndex(&mCommandPacketBuffer[mPreviousCommandPacketByteIndex], mCurrentCommandPacketByteIndex);
			}
			CommandPacketHelper::storeNextCommandPacketByteIndex(commandPacket, ~0u);
			CommandPacketHelper::storeImplementationDispatchFunctionIndex(commandPacket, U::COMMAND_DISPATCH_FUNCTION_INDEX);
			mPreviousCommandPacketByteIndex = mCurrentCommandPacketByteIndex;
			mCurrentCommandPacketByteIndex += numberOfCommandBytes;

			// Done
			#ifdef SE_STATISTICS
				++mNumberOfCommands;
			#endif
			return CommandPacketHelper::getCommand<U>(commandPacket);
		}

		/**
		*  @brief
		*    Submit the command buffer to the RHI without flushing; use this for recording command buffers once and submit them multiple times
		*
		*  @param[in] rhi
		*    RHI to submit the command buffer to
		*/
		inline void submitToRhi(IRhi& rhi) const
		{
			rhi.submitCommandBuffer(*this);
		}

		/**
		*  @brief
		*    Submit the command buffer to the RHI and clear so the command buffer is empty again
		*
		*  @param[in] rhi
		*    RHI to submit the command buffer to
		*/
		inline void submitToRhiAndClear(IRhi& rhi)
		{
			rhi.submitCommandBuffer(*this);
			clear();
		}

		/**
		*  @brief
		*    Submit the command buffer to another command buffer without flushing; use this for recording command buffers once and submit them multiple times
		*
		*  @param[in] commandBuffer
		*    Command buffer to submit the command buffer to
		*/
		inline void submitToCommandBuffer(CommandBuffer& commandBuffer) const
		{
			// Sanity checks
			RHI_ASSERT(this != &commandBuffer, "Can't submit a command buffer to itself")
			RHI_ASSERT(!isEmpty(), "Can't submit empty command buffers")

			// How many command package buffer bytes are consumed by the command to add?
			const uint32_t numberOfCommandBytes = mCurrentCommandPacketByteIndex;

			// 4294967295 is the maximum value of an "uint32_t"-type: Check for overflow
			// -> We use the magic number here to avoid "std::numeric_limits::max()" usage
			RHI_ASSERT((static_cast<uint64_t>(commandBuffer.mCurrentCommandPacketByteIndex) + numberOfCommandBytes) < 4294967295u, "Invalid current command packet byte index")

			// Grow command packet buffer, if required
			if (commandBuffer.mCommandPacketBufferNumberOfBytes < commandBuffer.mCurrentCommandPacketByteIndex + numberOfCommandBytes)
			{
				// Allocate new memory, grow using a known value but do also add the number of bytes consumed by the current command to add (many auxiliary bytes might be requested)
				const uint32_t newCommandPacketBufferNumberOfBytes = commandBuffer.mCommandPacketBufferNumberOfBytes + NUMBER_OF_BYTES_TO_GROW + numberOfCommandBytes;
				uint8_t* newCommandPacketBuffer = new uint8_t[newCommandPacketBufferNumberOfBytes];

				// Copy over current command package buffer content and free it, if required
				if (nullptr != commandBuffer.mCommandPacketBuffer)
				{
					memcpy(newCommandPacketBuffer, commandBuffer.mCommandPacketBuffer, commandBuffer.mCommandPacketBufferNumberOfBytes);
					delete [] commandBuffer.mCommandPacketBuffer;
				}

				// Finalize
				commandBuffer.mCommandPacketBuffer = newCommandPacketBuffer;
				commandBuffer.mCommandPacketBufferNumberOfBytes = newCommandPacketBufferNumberOfBytes;
			}

			// Copy over the command buffer in one burst
			memcpy(&commandBuffer.mCommandPacketBuffer[commandBuffer.mCurrentCommandPacketByteIndex], mCommandPacketBuffer, mCurrentCommandPacketByteIndex);

			// Setup previous command package
			if (~0u != commandBuffer.mPreviousCommandPacketByteIndex)
			{
				CommandPacketHelper::storeNextCommandPacketByteIndex(&commandBuffer.mCommandPacketBuffer[commandBuffer.mPreviousCommandPacketByteIndex], commandBuffer.mCurrentCommandPacketByteIndex);
			}

			// Update command package indices
			CommandPacket commandPacket = &commandBuffer.mCommandPacketBuffer[commandBuffer.mCurrentCommandPacketByteIndex];
			uint32_t nextCommandPacketByteIndex = CommandPacketHelper::getNextCommandPacketByteIndex(commandPacket);
			while (~0u != nextCommandPacketByteIndex)
			{
				nextCommandPacketByteIndex = commandBuffer.mCurrentCommandPacketByteIndex + nextCommandPacketByteIndex;
				CommandPacketHelper::storeNextCommandPacketByteIndex(commandPacket, nextCommandPacketByteIndex);
				commandPacket = &commandBuffer.mCommandPacketBuffer[nextCommandPacketByteIndex];
				nextCommandPacketByteIndex = CommandPacketHelper::getNextCommandPacketByteIndex(commandPacket);
			}

			// Finalize
			commandBuffer.mPreviousCommandPacketByteIndex = commandBuffer.mCurrentCommandPacketByteIndex + mPreviousCommandPacketByteIndex;
			commandBuffer.mCurrentCommandPacketByteIndex += mCurrentCommandPacketByteIndex;
			#ifdef SE_STATISTICS
				commandBuffer.mNumberOfCommands += mNumberOfCommands;
			#endif
		}

		/**
		*  @brief
		*    Submit the command buffer to another command buffer and clear so the command buffer is empty again
		*
		*  @param[in] commandBuffer
		*    Command buffer to submit the command buffer to
		*/
		inline void submitToCommandBufferAndClear(CommandBuffer& commandBuffer)
		{
			submitToCommandBuffer(commandBuffer);
			clear();
		}

	// Private definitions
	private:
		static constexpr uint32_t NUMBER_OF_BYTES_TO_GROW = 8192;

	// Private data
	private:
		// Memory
		uint32_t mCommandPacketBufferNumberOfBytes;
		uint8_t* mCommandPacketBuffer;
		// Current state
		uint32_t mPreviousCommandPacketByteIndex;
		uint32_t mCurrentCommandPacketByteIndex;
		#ifdef SE_STATISTICS
			uint32_t mNumberOfCommands;
		#endif

	};

	// Concrete commands
	namespace Command
	{

		//[-------------------------------------------------------]
		//[ Command buffer                                        ]
		//[-------------------------------------------------------]
		struct ExecuteCommandBuffer final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, CommandBuffer* commandBufferToExecute)
			{
				RHI_ASSERT(nullptr != commandBufferToExecute, "Invalid command buffer to execute")
				*commandBuffer.addCommand<ExecuteCommandBuffer>() = ExecuteCommandBuffer(commandBufferToExecute);
			}
			// Constructor
			inline explicit ExecuteCommandBuffer(CommandBuffer* _commandBufferToExecute) :
				commandBufferToExecute(_commandBufferToExecute)
			{}
			// Data
			CommandBuffer* commandBufferToExecute;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::EXECUTE_COMMAND_BUFFER;
		};

		//[-------------------------------------------------------]
		//[ Graphics                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the used graphics root signature
		*
		*  @param[in] rootSignature
		*    Graphics root signature to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsRootSignature final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRootSignature* rootSignature)
			{
				*commandBuffer.addCommand<SetGraphicsRootSignature>() = SetGraphicsRootSignature(rootSignature);
			}
			// Constructor
			inline explicit SetGraphicsRootSignature(IRootSignature* _rootSignature) :
				rootSignature(_rootSignature)
			{}
			// Data
			IRootSignature* rootSignature;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_ROOT_SIGNATURE;
		};

		/**
		*  @brief
		*    Set the used graphics pipeline state
		*
		*  @param[in] graphicsPipelineState
		*    Graphics pipeline state to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsPipelineState final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IGraphicsPipelineState* graphicsPipelineState)
			{
				*commandBuffer.addCommand<SetGraphicsPipelineState>() = SetGraphicsPipelineState(graphicsPipelineState);
			}
			// Constructor
			inline explicit SetGraphicsPipelineState(IGraphicsPipelineState* _graphicsPipelineState) :
				graphicsPipelineState(_graphicsPipelineState)
			{}
			// Data
			IGraphicsPipelineState* graphicsPipelineState;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_PIPELINE_STATE;
		};

		/**
		*  @brief
		*    Set a graphics resource group
		*
		*  @param[in] rootParameterIndex
		*    The root parameter index number for binding
		*  @param[in] resourceGroup
		*    Resource group to set
		*/
		struct SetGraphicsResourceGroup final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t rootParameterIndex, IResourceGroup* resourceGroup)
			{
				*commandBuffer.addCommand<SetGraphicsResourceGroup>() = SetGraphicsResourceGroup(rootParameterIndex, resourceGroup);
			}
			// Constructor
			inline SetGraphicsResourceGroup(uint32_t _rootParameterIndex, IResourceGroup* _resourceGroup) :
				rootParameterIndex(_rootParameterIndex),
				resourceGroup(_resourceGroup)
			{}
			// Data
			uint32_t		rootParameterIndex;
			IResourceGroup*	resourceGroup;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_RESOURCE_GROUP;
		};

		/**
		*  @brief
		*    Set the used vertex array, input-assembler (IA) stage
		*
		*  @param[in] vertexArray
		*    Vertex array to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsVertexArray final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IVertexArray* vertexArray)
			{
				*commandBuffer.addCommand<SetGraphicsVertexArray>() = SetGraphicsVertexArray(vertexArray);
			}
			// Constructor
			inline explicit SetGraphicsVertexArray(IVertexArray* _vertexArray) :
				vertexArray(_vertexArray)
			{}
			// Data
			IVertexArray* vertexArray;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_VERTEX_ARRAY;
		};

		/**
		*  @brief
		*    Set the graphics viewports, rasterizer (RS) stage
		*
		*  @param[in] numberOfViewports
		*    Number of viewports, if <1 nothing happens, must be <="Rhi::Capabilities::maximumNumberOfViewports"
		*  @param[in] viewports
		*    C-array of viewports, there must be at least "numberOfViewports"-viewports, in case of a null pointer nothing happens
		*
		*  @note
		*    - The current viewport(s) does not affect the clear operation
		*    - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
		*      Use the convenience "Rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
		*/
		struct SetGraphicsViewports final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfViewports, const Viewport* viewports)
			{
				*commandBuffer.addCommand<SetGraphicsViewports>() = SetGraphicsViewports(numberOfViewports, viewports);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, float minimumDepth = 0.0f, float maximumDepth = 1.0f)
			{
				SetGraphicsViewports* setGraphicsViewportsCommand = commandBuffer.addCommand<SetGraphicsViewports>(sizeof(Viewport));

				// Set command data
				Viewport* viewport = reinterpret_cast<Viewport*>(CommandPacketHelper::getAuxiliaryMemory(setGraphicsViewportsCommand));
				viewport->topLeftX = static_cast<float>(topLeftX);
				viewport->topLeftY = static_cast<float>(topLeftY);
				viewport->width	   = static_cast<float>(width);
				viewport->height   = static_cast<float>(height);
				viewport->minDepth = minimumDepth;
				viewport->maxDepth = maximumDepth;

				// Finalize command
				setGraphicsViewportsCommand->numberOfViewports = 1;
				setGraphicsViewportsCommand->viewports		   = nullptr;
			}
			// Constructor
			inline SetGraphicsViewports(uint32_t _numberOfViewports, const Viewport* _viewports) :
				numberOfViewports(_numberOfViewports),
				viewports(_viewports)
			{}
			// Data
			uint32_t		numberOfViewports;
			const Viewport* viewports;	// If null pointer, command auxiliary memory is used instead
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_VIEWPORTS;
		};

		/**
		*  @brief
		*    Set the graphics scissor rectangles, rasterizer (RS) stage
		*
		*  @param[in] numberOfScissorRectangles
		*    Number of scissor rectangles, if <1 nothing happens, must be <="Rhi::Capabilities::maximumNumberOfViewports"
		*  @param[in] dcissorRectangles
		*    C-array of scissor rectangles, there must be at least "numberOfScissorRectangles" scissor rectangles, in case of a null pointer nothing happens
		*
		*  @note
		*    - Scissor rectangles are only used when "Rhi::RasterizerState::scissorEnable" is true
		*    - The current scissor rectangle(s) does not affect the clear operation
		*/
		struct SetGraphicsScissorRectangles final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfScissorRectangles, const ScissorRectangle* scissorRectangles)
			{
				*commandBuffer.addCommand<SetGraphicsScissorRectangles>() = SetGraphicsScissorRectangles(numberOfScissorRectangles, scissorRectangles);
			}
			static inline void create(CommandBuffer& commandBuffer, long topLeftX, long topLeftY, long bottomRightX, long bottomRightY)
			{
				SetGraphicsScissorRectangles* setGraphicsScissorRectanglesCommand = commandBuffer.addCommand<SetGraphicsScissorRectangles>(sizeof(ScissorRectangle));

				// Set command data
				ScissorRectangle* scissorRectangle = reinterpret_cast<ScissorRectangle*>(CommandPacketHelper::getAuxiliaryMemory(setGraphicsScissorRectanglesCommand));
				scissorRectangle->topLeftX	   = topLeftX;
				scissorRectangle->topLeftY	   = topLeftY;
				scissorRectangle->bottomRightX = bottomRightX;
				scissorRectangle->bottomRightY = bottomRightY;

				// Finalize command
				setGraphicsScissorRectanglesCommand->numberOfScissorRectangles = 1;
				setGraphicsScissorRectanglesCommand->scissorRectangles		   = nullptr;
			}
			// Constructor
			inline SetGraphicsScissorRectangles(uint32_t _numberOfScissorRectangles, const ScissorRectangle* _scissorRectangles) :
				numberOfScissorRectangles(_numberOfScissorRectangles),
				scissorRectangles(_scissorRectangles)
			{}
			// Data
			uint32_t				numberOfScissorRectangles;
			const ScissorRectangle* scissorRectangles;	// If null pointer, command auxiliary memory is used instead
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_SCISSOR_RECTANGLES;
		};

		/**
		*  @brief
		*    Set graphics viewport and scissor rectangle (convenience method)
		*
		*  @param[in] topLeftX
		*    Top left x
		*  @param[in] topLeftY
		*    Top left y
		*  @param[in] width
		*    Width
		*  @param[in] height
		*    Height
		*  @param[in] minimumDepth
		*    Minimum depth
		*  @param[in] maximumDepth
		*    Maximum depth
		*
		*  @note
		*    - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
		*      Use the convenience "Rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
		*/
		struct SetGraphicsViewportAndScissorRectangle final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, float minimumDepth = 0.0f, float maximumDepth = 1.0f)
			{
				// Set the graphics viewport
				SetGraphicsViewports::create(commandBuffer, topLeftX, topLeftY, width, height, minimumDepth, maximumDepth);

				// Set the graphics scissor rectangle
				SetGraphicsScissorRectangles::create(commandBuffer, static_cast<long>(topLeftX), static_cast<long>(topLeftY), static_cast<long>(topLeftX + width), static_cast<long>(topLeftY + height));
			}
		};

		/**
		*  @brief
		*    Set the graphics render target to render into, output-merger (OM) stage
		*
		*  @param[in] renderTarget
		*    Render target to render into by binding it to the output-merger state, can be an null pointer to render into the primary window
		*/
		struct SetGraphicsRenderTarget final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRenderTarget* renderTarget)
			{
				*commandBuffer.addCommand<SetGraphicsRenderTarget>() = SetGraphicsRenderTarget(renderTarget);
			}
			// Constructor
			inline explicit SetGraphicsRenderTarget(IRenderTarget* _renderTarget) :
				renderTarget(_renderTarget)
			{}
			// Data
			IRenderTarget* renderTarget;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_RENDER_TARGET;
		};

		/**
		*  @brief
		*    Clears the graphics viewport to a specified RGBA color, clears the depth buffer,
		*    and erases the stencil buffer
		*
		*  @param[in] clearFlags
		*    Flags that indicate what should be cleared. This parameter can be any
		*    combination of the following flags, but at least one flag must be used:
		*    "Rhi::ClearFlag::COLOR", "Rhi::ClearFlag::DEPTH" and "Rhi::ClearFlag::STENCIL, see "Rhi::ClearFlag"-flags
		*  @param[in] color
		*    RGBA clear color (used if "Rhi::ClearFlag::COLOR" is set)
		*  @param[in] z
		*    Z clear value. (if "Rhi::ClearFlag::DEPTH" is set)
		*    This parameter can be in the range from 0.0 through 1.0. A value of 0.0
		*    represents the nearest distance to the viewer, and 1.0 the farthest distance.
		*  @param[in] stencil
		*    Value to clear the stencil-buffer with. This parameter can be in the range from
		*    0 through 2^n–1, where n is the bit depth of the stencil buffer.
		*
		*  @note
		*    - The current viewport(s) (see "Rhi::Command::SetGraphicsViewports()") does not affect the clear operation
		*    - The current scissor rectangle(s) (see "Rhi::Command::SetGraphicsScissorRectangles()") does not affect the clear operation
		*    - In case there are multiple active render targets, all render targets are cleared
		*/
		struct ClearGraphics final
		{
			// Static methods
			// -> z = 0 instead of 1 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
			static inline void create(CommandBuffer& commandBuffer, uint32_t clearFlags, const float color[4], float z = 0.0f, uint32_t stencil = 0)
			{
				*commandBuffer.addCommand<ClearGraphics>() = ClearGraphics(clearFlags, color, z, stencil);
			}
			// Constructor
			inline ClearGraphics(uint32_t _clearFlags, const float _color[4], float _z, uint32_t _stencil) :
				clearFlags(_clearFlags),
				color{_color[0], _color[1], _color[2], _color[3]},
				z(_z),
				stencil(_stencil)
			{}
			// Data
			uint32_t clearFlags;
			float	 color[4];
			float	 z;
			uint32_t stencil;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::CLEAR_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on an array of vertices instancing and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawArguments" starting at "indirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Draw instanced is a shader model 4 feature, only supported if "Rhi::Capabilities::drawInstanced" is true
		*    - In Direct3D 9, instanced arrays with hardware support is only possible when drawing indexed primitives, see
		*      "Efficiently Drawing Multiple Instances of Geometry (Direct3D 9)"-article at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173349%28v=vs.85%29.aspx#Drawing_Non_Indexed_Geometry
		*    - Fails if no vertex array is set
		*    - If the multi-draw indirect feature is not supported this parameter, multiple draw calls are emitted
		*    - If the draw indirect feature is not supported, a software indirect buffer is used and multiple draw calls are emitted
		*/
		struct DrawGraphics final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawGraphics>() = DrawGraphics(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t vertexCountPerInstance, uint32_t instanceCount = 1, uint32_t startVertexLocation = 0, uint32_t startInstanceLocation = 0)
			{
				DrawGraphics* drawCommand = commandBuffer.addCommand<DrawGraphics>(sizeof(DrawArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawArguments"-instance
				const DrawArguments drawArguments(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawArguments, sizeof(DrawArguments));

				// Finalize command
				drawCommand->indirectBuffer		  = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws		  = 1;
			}
			// Constructor
			inline DrawGraphics(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{}
			// Data
			const IIndirectBuffer* indirectBuffer;	// If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on indexing into an array of vertices, instancing and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawIndexedArguments" starting at bindirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Instanced arrays is a shader model 3 feature, only supported if "Rhi::Capabilities::instancedArrays" is true
		*    - Draw instanced is a shader model 4 feature, only supported if "Rhi::Capabilities::drawInstanced" is true
		*    - This method draws indexed primitives from the current set of data input streams
		*    - Fails if no index and/or vertex array is set
		*    - If the multi-draw indirect feature is not supported this parameter, multiple draw calls are emitted
		*    - If the draw indirect feature is not supported, a software indirect buffer is used and multiple draw calls are emitted
		*/
		struct DrawIndexedGraphics final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawIndexedGraphics>() = DrawIndexedGraphics(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t indexCountPerInstance, uint32_t instanceCount = 1, uint32_t startIndexLocation = 0, int32_t baseVertexLocation = 0, uint32_t startInstanceLocation = 0)
			{
				DrawIndexedGraphics* drawCommand = commandBuffer.addCommand<DrawIndexedGraphics>(sizeof(DrawIndexedArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawIndexedArguments"-instance
				const DrawIndexedArguments drawIndexedArguments(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawIndexedArguments, sizeof(DrawIndexedArguments));

				// Finalize command
				drawCommand->indirectBuffer		  = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws		  = 1;
			}
			// Constructor
			inline DrawIndexedGraphics(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{}
			// Data
			const IIndirectBuffer* indirectBuffer;	// If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_INDEXED_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on a task and mesh shader and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawMeshTasksArguments" starting at "indirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::meshShader" is "true"
		*/
		struct DrawMeshTasks final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawMeshTasks>() = DrawMeshTasks(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfTasks, uint32_t firstTask = 0)
			{
				DrawMeshTasks* drawCommand = commandBuffer.addCommand<DrawMeshTasks>(sizeof(DrawMeshTasksArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawMeshTasksArguments"-instance
				const DrawMeshTasksArguments drawArguments(numberOfTasks, firstTask);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawArguments, sizeof(DrawMeshTasksArguments));

				// Finalize command
				drawCommand->indirectBuffer		  = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws		  = 1;
			}
			// Constructor
			inline DrawMeshTasks(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{}
			// Data
			const IIndirectBuffer* indirectBuffer;	// If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_MESH_TASKS;
		};

		//[-------------------------------------------------------]
		//[ Compute                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the used compute root signature
		*
		*  @param[in] rootSignature
		*    Compute root signature to use, can be an null pointer (default: "nullptr")
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputeRootSignature final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRootSignature* rootSignature)
			{
				*commandBuffer.addCommand<SetComputeRootSignature>() = SetComputeRootSignature(rootSignature);
			}
			// Constructor
			inline explicit SetComputeRootSignature(IRootSignature* _rootSignature) :
				rootSignature(_rootSignature)
			{}
			// Data
			IRootSignature* rootSignature;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_ROOT_SIGNATURE;
		};

		/**
		*  @brief
		*    Set the used compute pipeline state
		*
		*  @param[in] computePipelineState
		*    Compute pipeline state to use, can be an null pointer (default: "nullptr")
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputePipelineState final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IComputePipelineState* computePipelineState)
			{
				*commandBuffer.addCommand<SetComputePipelineState>() = SetComputePipelineState(computePipelineState);
			}
			// Constructor
			inline explicit SetComputePipelineState(IComputePipelineState* _computePipelineState) :
				computePipelineState(_computePipelineState)
			{}
			// Data
			IComputePipelineState* computePipelineState;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_PIPELINE_STATE;
		};

		/**
		*  @brief
		*    Set a compute resource group
		*
		*  @param[in] rootParameterIndex
		*    The root parameter index number for binding
		*  @param[in] resourceGroup
		*    Resource group to set
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputeResourceGroup final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t rootParameterIndex, IResourceGroup* resourceGroup)
			{
				*commandBuffer.addCommand<SetComputeResourceGroup>() = SetComputeResourceGroup(rootParameterIndex, resourceGroup);
			}
			// Constructor
			inline SetComputeResourceGroup(uint32_t _rootParameterIndex, IResourceGroup* _resourceGroup) :
				rootParameterIndex(_rootParameterIndex),
				resourceGroup(_resourceGroup)
			{}
			// Data
			uint32_t		rootParameterIndex;
			IResourceGroup*	resourceGroup;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_RESOURCE_GROUP;
		};

		/**
		*  @brief
		*    Compute dispatch call
		*
		*  @param[in] groupCountX
		*    Number of local workgroups to dispatch in the X dimension
		*  @param[in] groupCountY
		*    Number of local workgroups to dispatch in the Y dimension
		*  @param[in] groupCountZ
		*    Number of local workgroups to dispatch in the Z dimension
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct DispatchCompute final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
			{
				*commandBuffer.addCommand<DispatchCompute>() = DispatchCompute(groupCountX, groupCountY, groupCountZ);
			}
			// Constructor
			inline DispatchCompute(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) :
				groupCountX(_groupCountX),
				groupCountY(_groupCountY),
				groupCountZ(_groupCountZ)
			{}
			// Data
			uint32_t groupCountX;
			uint32_t groupCountY;
			uint32_t groupCountZ;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DISPATCH_COMPUTE;
		};

		//[-------------------------------------------------------]
		//[ Resource                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set texture minimum maximum mipmap index
		*
		*  @param[in] texture
		*    Texture to set the minimum maximum mipmap index of
		*  @param[in] minimumMipmapIndex
		*    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
		*  @param[in] maximumMipmapIndex
		*    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
		*/
		struct SetTextureMinimumMaximumMipmapIndex final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, ITexture& texture, uint32_t minimumMipmapIndex, uint32_t maximumMipmapIndex)
			{
				*commandBuffer.addCommand<SetTextureMinimumMaximumMipmapIndex>() = SetTextureMinimumMaximumMipmapIndex(texture, minimumMipmapIndex, maximumMipmapIndex);
			}
			// Constructor
			inline SetTextureMinimumMaximumMipmapIndex(ITexture& _texture, uint32_t _minimumMipmapIndex, uint32_t _maximumMipmapIndex) :
				texture(&_texture),
				minimumMipmapIndex(_minimumMipmapIndex),
				maximumMipmapIndex(_maximumMipmapIndex)
			{}
			// Data
			ITexture* texture;
			uint32_t  minimumMipmapIndex;
			uint32_t  maximumMipmapIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_TEXTURE_MINIMUM_MAXIMUM_MIPMAP_INDEX;
		};

		/**
		*  @brief
		*    Resolve multisample framebuffer
		*
		*  @param[in] destinationRenderTarget
		*    None multisample destination render target
		*  @param[in] sourceMultisampleFramebuffer
		*    Source multisample framebuffer
		*/
		struct ResolveMultisampleFramebuffer final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRenderTarget& destinationRenderTarget, IFramebuffer& sourceMultisampleFramebuffer)
			{
				*commandBuffer.addCommand<ResolveMultisampleFramebuffer>() = ResolveMultisampleFramebuffer(destinationRenderTarget, sourceMultisampleFramebuffer);
			}
			// Constructor
			inline ResolveMultisampleFramebuffer(IRenderTarget& _destinationRenderTarget, IFramebuffer& _sourceMultisampleFramebuffer) :
				destinationRenderTarget(&_destinationRenderTarget),
				sourceMultisampleFramebuffer(&_sourceMultisampleFramebuffer)
			{}
			// Data
			IRenderTarget* destinationRenderTarget;
			IFramebuffer* sourceMultisampleFramebuffer;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::RESOLVE_MULTISAMPLE_FRAMEBUFFER;
		};

		/**
		*  @brief
		*    Copy resource
		*
		*  @param[in] destinationResource
		*    Destination resource
		*  @param[in] sourceResource
		*    Source Resource
		*/
		struct CopyResource final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IResource& destinationResource, IResource& sourceResource)
			{
				*commandBuffer.addCommand<CopyResource>() = CopyResource(destinationResource, sourceResource);
			}
			// Constructor
			inline CopyResource(IResource& _destinationResource, IResource& _sourceResource) :
				destinationResource(&_destinationResource),
				sourceResource(&_sourceResource)
			{}
			// Data
			IResource* destinationResource;
			IResource* sourceResource;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::COPY_RESOURCE;
		};

		/**
		*  @brief
		*    Generate resource mipmaps
		*
		*  @param[in] resource
		*    Resource
		*/
		struct GenerateMipmaps final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IResource& resource)
			{
				*commandBuffer.addCommand<GenerateMipmaps>() = GenerateMipmaps(resource);
			}
			// Constructor
			inline explicit GenerateMipmaps(IResource& _resource) :
				resource(&_resource)
			{}
			// Data
			IResource* resource;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::GENERATE_MIPMAPS;
		};

		//[-------------------------------------------------------]
		//[ Query                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Reset asynchronous query pool
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] firstQueryIndex
		*    First query index (e.g. 0)
		*  @param[in] numberOfQueries
		*    Number of queries (e.g. 1)
		*/
		struct ResetQueryPool final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t firstQueryIndex = 0, uint32_t numberOfQueries = 1)
			{
				*commandBuffer.addCommand<ResetQueryPool>() = ResetQueryPool(queryPool, firstQueryIndex, numberOfQueries);
			}
			// Constructor
			inline ResetQueryPool(IQueryPool& _queryPool, uint32_t _firstQueryIndex, uint32_t _numberOfQueries) :
				queryPool(&_queryPool),
				firstQueryIndex(_firstQueryIndex),
				numberOfQueries(_numberOfQueries)
			{}
			// Data
			IQueryPool* queryPool;
			uint32_t	firstQueryIndex;
			uint32_t	numberOfQueries;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::RESET_QUERY_POOL;
		};

		/**
		*  @brief
		*    Begin asynchronous query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*  @param[in] queryControlFlags
		*    Query control flags (e.g. "Rhi::QueryControlFlags::PRECISE")
		*/
		struct BeginQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0, uint32_t queryControlFlags = 0)
			{
				*commandBuffer.addCommand<BeginQuery>() = BeginQuery(queryPool, queryIndex, queryControlFlags);
			}
			// Constructor
			inline BeginQuery(IQueryPool& _queryPool, uint32_t _queryIndex, uint32_t _queryControlFlags) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex),
				queryControlFlags(_queryControlFlags)
			{}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			uint32_t	queryControlFlags;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::BEGIN_QUERY;
		};

		/**
		*  @brief
		*    Reset and begin asynchronous query; ease-of-use function in case there's just a single query inside the query pool
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*  @param[in] queryControlFlags
		*    Query control flags (e.g. "Rhi::QueryControlFlags::PRECISE")
		*/
		struct ResetAndBeginQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0, uint32_t queryControlFlags = 0)
			{
				*commandBuffer.addCommand<ResetQueryPool>() = ResetQueryPool(queryPool, queryIndex, 1);
				*commandBuffer.addCommand<BeginQuery>() = BeginQuery(queryPool, queryIndex, queryControlFlags);
			}
		};

		/**
		*  @brief
		*    End asynchronous query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*/
		struct EndQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0)
			{
				*commandBuffer.addCommand<EndQuery>() = EndQuery(queryPool, queryIndex);
			}
			// Constructor
			inline EndQuery(IQueryPool& _queryPool, uint32_t _queryIndex) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex)
			{}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::END_QUERY;
		};

		/**
		*  @brief
		*    Write asynchronous timestamp query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*/
		struct WriteTimestampQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0)
			{
				*commandBuffer.addCommand<WriteTimestampQuery>() = WriteTimestampQuery(queryPool, queryIndex);
			}
			// Constructor
			inline WriteTimestampQuery(IQueryPool& _queryPool, uint32_t _queryIndex) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex)
			{}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::WRITE_TIMESTAMP_QUERY;
		};

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] name
		*    ASCII name of the debug marker, must be valid (there's no internal null pointer test)
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct SetDebugMarker final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const char* name)
			{
				*commandBuffer.addCommand<SetDebugMarker>() = SetDebugMarker(name);
			}
			// Constructor
			inline explicit SetDebugMarker(const char* _name)
			{
				RHI_ASSERT(strlen(_name) < 128, "Invalid name")
				strncpy(name, _name, 128);
				name[127] = '\0';
			}
			// Data
			char name[128];
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_DEBUG_MARKER;
		};

		/**
		*  @brief
		*    Begin debug event
		*
		*  @param[in] name
		*    ASCII name of the debug event, must be valid (there's no internal null pointer test)
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct BeginDebugEvent final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const char* name)
			{
				*commandBuffer.addCommand<BeginDebugEvent>() = BeginDebugEvent(name);
			}
			// Constructor
			inline explicit BeginDebugEvent(const char* _name)
			{
				RHI_ASSERT(strlen(_name) < 128, "Invalid name")
				strncpy(name, _name, 128);
				name[127] = '\0';
			}
			// Data
			char name[128];
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::BEGIN_DEBUG_EVENT;
		};

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct EndDebugEvent final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer)
			{
				*commandBuffer.addCommand<EndDebugEvent>() = EndDebugEvent();
			}
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::END_DEBUG_EVENT;
		};

	}

	// Debug macros
	#if SE_DEBUG
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug marker
		*/
		#define COMMAND_SET_DEBUG_MARKER(commandBuffer, name) Rhi::Command::SetDebugMarker::create(commandBuffer, name);

		/**
		*  @brief
		*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SET_DEBUG_MARKER_FUNCTION(commandBuffer) Rhi::Command::SetDebugMarker::create(commandBuffer, __FUNCTION__);

		/**
		*  @brief
		*    Begin debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT(commandBuffer, name) Rhi::Command::BeginDebugEvent::create(commandBuffer, name);

		/**
		*  @brief
		*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(commandBuffer) Rhi::Command::BeginDebugEvent::create(commandBuffer, __FUNCTION__);

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_END_DEBUG_EVENT(commandBuffer) Rhi::Command::EndDebugEvent::create(commandBuffer);

		/**
		*  @brief
		*    Scoped debug event class
		*/
		class CommandScopedDebugEventOnExit
		{
		// Public methods
		public:
			inline explicit CommandScopedDebugEventOnExit(CommandBuffer& commandBuffer) :
				mCommandBuffer(commandBuffer)
			{}

			inline ~CommandScopedDebugEventOnExit()
			{
				Rhi::Command::EndDebugEvent::create(mCommandBuffer);
			}

		// Private methods
		private:
			explicit CommandScopedDebugEventOnExit(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;
			CommandScopedDebugEventOnExit& operator =(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;

		// Private data
		private:
			CommandBuffer& mCommandBuffer;
		};

		/**
		*  @brief
		*    Scoped debug event, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name) \
			Rhi::Command::BeginDebugEvent::create(commandBuffer, name); \
			SE_PRAGMA_WARNING_PUSH \
				SE_PRAGMA_WARNING_DISABLE_MSVC(4456) \
				Rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(commandBuffer); \
			SE_PRAGMA_WARNING_POP

		/**
		*  @brief
		*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer) \
			Rhi::Command::BeginDebugEvent::create(commandBuffer, __FUNCTION__); \
			SE_PRAGMA_WARNING_PUSH \
				SE_PRAGMA_WARNING_DISABLE_MSVC(4456) \
				Rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(commandBuffer); \
			SE_PRAGMA_WARNING_POP
	#else
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug marker
		*/
		#define COMMAND_SET_DEBUG_MARKER(commandBuffer, name)

		/**
		*  @brief
		*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SET_DEBUG_MARKER_FUNCTION(commandBuffer)

		/**
		*  @brief
		*    Begin debug event, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT(commandBuffer, name)

		/**
		*  @brief
		*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(commandBuffer)

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_END_DEBUG_EVENT(commandBuffer)

		/**
		*  @brief
		*    Scoped debug event, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name)

		/**
		*  @brief
		*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer)
	#endif

} // Rhi

//[-------------------------------------------------------]
//[ Debug macros                                          ]
//[-------------------------------------------------------]
#if SE_DEBUG
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name) if (nullptr != rhi) { (rhi)->setDebugMarker(name); }

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->setDebugMarker(__FUNCTION__); }

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name) if (nullptr != rhi) { (rhi)->beginDebugEvent(name); }

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->beginDebugEvent(__FUNCTION__); }

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_END_DEBUG_EVENT(rhi) if (nullptr != rhi) { (rhi)->endDebugEvent(); }

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name) , name

	/**
	*  @brief
	*    Decorate the debug name to make it easier to see the semantic of the resource
	*
	*  @param[in] name
	*    Debug name provided from the outside
	*  @param[in] decoration
	*    Decoration to append in front (e.g. "IBO", will result in appended "IBO: " in front if the provided name isn't empty)
	*  @param[in] numberOfDecorationCharacters
	*    Number of decoration characters
	*
	*  @note
	*    - The result is in local string variable "detailedName"
	*    - Traditional C-string on the runtime stack used for efficiency reasons (just for debugging, but must still be some kind of usable)
	*/
	#define RHI_DECORATED_DEBUG_NAME(name, detailedName, decoration, numberOfDecorationCharacters) \
		RHI_ASSERT(strlen(name) < 256, "Name is not allowed to exceed 255 characters") \
		char detailedName[256 + numberOfDecorationCharacters] = decoration; \
		if (name[0] != '\0') \
		{ \
			strcat(detailedName, ": "); \
			strncat(detailedName, name, 256); \
		}
#else
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name)

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi)

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name)

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi)

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_END_DEBUG_EVENT(rhi)

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name)
#endif
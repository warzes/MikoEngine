#pragma once

namespace OpenGLRhi
{
	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/VertexBuffer.h                       ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL vertex buffer object (VBO, "array buffer" in OpenGL terminology) interface
	*/
	class VertexBuffer : public Rhi::IVertexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexBuffer() override
		{
			// Destroy the OpenGL array buffer
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteBuffersARB(1, &mOpenGLArrayBuffer);
		}

		/**
		*  @brief
		*    Return the OpenGL array buffer
		*
		*  @return
		*    The OpenGL array buffer, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLArrayBuffer() const
		{
			return mOpenGLArrayBuffer;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(VertexBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit VertexBuffer(OpenGLRhi& openGLRhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IVertexBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLArrayBuffer(0)
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLArrayBuffer;	// OpenGL array buffer, can be zero if no resource is allocated


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator =(const VertexBuffer&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/VertexBufferBind.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL vertex buffer object (VBO, "array buffer" in OpenGL terminology) class, traditional bind version
	*/
	class VertexBufferBind final : public VertexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the vertex buffer, must be valid
		*  @param[in] data
		*    Vertex buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		VertexBufferBind(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			VertexBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently bound OpenGL array buffer
			GLint openGLArrayBufferBackup = 0;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &openGLArrayBufferBackup);
#endif

			// Create the OpenGL array buffer
			glGenBuffersARB(1, &mOpenGLArrayBuffer);

			// Bind this OpenGL array buffer and upload the data
			// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, mOpenGLArrayBuffer);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, static_cast<GLsizeiptrARB>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL array buffer
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLArrayBufferBackup));
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VBO", 6)	// 6 = "VBO: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLArrayBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexBufferBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexBufferBind(const VertexBufferBind&) = delete;
		VertexBufferBind& operator =(const VertexBufferBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/VertexBufferDsa.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL vertex buffer object (VBO, "array buffer" in OpenGL terminology) class, effective direct state access (DSA)
	*/
	class VertexBufferDsa final : public VertexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the vertex buffer, must be valid
		*  @param[in] data
		*    Vertex buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		VertexBufferDsa(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			VertexBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			if ( openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
			{
				// Create the OpenGL array buffer
				glCreateBuffers(1, &mOpenGLArrayBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferData(mOpenGLArrayBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}
			else
			{
				// Create the OpenGL array buffer
				glGenBuffersARB(1, &mOpenGLArrayBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferDataEXT(mOpenGLArrayBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VBO", 6)	// 6 = "VBO: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLArrayBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexBufferDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexBufferDsa(const VertexBufferDsa&) = delete;
		VertexBufferDsa& operator =(const VertexBufferDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/IndexBuffer.h                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL index buffer object (IBO, "element array buffer" in OpenGL terminology) interface
	*/
	class IndexBuffer : public Rhi::IIndexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IndexBuffer() override
		{
			// Destroy the OpenGL element array buffer
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteBuffersARB(1, &mOpenGLElementArrayBuffer);
		}

		/**
		*  @brief
		*    Return the OpenGL element array buffer
		*
		*  @return
		*    The OpenGL element array buffer, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLElementArrayBuffer() const
		{
			return mOpenGLElementArrayBuffer;
		}

		/**
		*  @brief
		*    Return the OpenGL element array buffer data type
		*
		*  @return
		*    The OpenGL element array buffer data type
		*/
		[[nodiscard]] inline GLenum getOpenGLType() const
		{
			return mOpenGLType;
		}

		/**
		*  @brief
		*    Return the number of bytes of an index
		*
		*  @return
		*    The number of bytes of an index
		*/
		[[nodiscard]] inline uint32_t getIndexSizeInBytes() const
		{
			return mIndexSizeInBytes;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(IndexBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] indexBufferFormat
		*    Index buffer data format
		*/
		inline IndexBuffer(OpenGLRhi& openGLRhi, Rhi::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IIndexBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLElementArrayBuffer(0),
			mOpenGLType(Mapping::getOpenGLType(indexBufferFormat)),
			mIndexSizeInBytes(Rhi::IndexBufferFormat::GetNumberOfBytesPerElement(indexBufferFormat))
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint   mOpenGLElementArrayBuffer;	// OpenGL element array buffer, can be zero if no resource is allocated
		GLenum   mOpenGLType;				// OpenGL element array buffer data type
		uint32_t mIndexSizeInBytes;			// Number of bytes of an index


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator =(const IndexBuffer&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/IndexBufferBind.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL index buffer object (IBO, "element array buffer" in OpenGL terminology) class, traditional bind version
	*/
	class IndexBufferBind final : public IndexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the index buffer, must be valid
		*  @param[in] data
		*    Index buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] indexBufferFormat
		*    Index buffer data format
		*/
		IndexBufferBind(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage, Rhi::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IndexBuffer(openGLRhi, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently bound OpenGL element array buffer
			GLint openGLElementArrayBufferBackup = 0;
			glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, &openGLElementArrayBufferBackup);
#endif

			// Create the OpenGL element array buffer
			glGenBuffersARB(1, &mOpenGLElementArrayBuffer);

			// Bind this OpenGL element array buffer and upload the data
			// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mOpenGLElementArrayBuffer);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<GLsizeiptrARB>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL element array buffer
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLElementArrayBufferBackup));
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IBO", 6)	// 6 = "IBO: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLElementArrayBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IndexBufferBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit IndexBufferBind(const IndexBufferBind&) = delete;
		IndexBufferBind& operator =(const IndexBufferBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/IndexBufferDsa.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL index buffer object (IBO, "element array buffer" in OpenGL terminology) class, effective direct state access (DSA)
	*/
	class IndexBufferDsa final : public IndexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the index buffer, must be valid
		*  @param[in] data
		*    Index buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] indexBufferFormat
		*    Index buffer data format
		*/
		IndexBufferDsa(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage, Rhi::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IndexBuffer(openGLRhi, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			if ( openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
			{
				// Create the OpenGL element array buffer
				glCreateBuffers(1, &mOpenGLElementArrayBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferData(mOpenGLElementArrayBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}
			else
			{
				// Create the OpenGL element array buffer
				glGenBuffersARB(1, &mOpenGLElementArrayBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferDataEXT(mOpenGLElementArrayBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IBO", 6)	// 6 = "IBO: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLElementArrayBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IndexBufferDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit IndexBufferDsa(const IndexBufferDsa&) = delete;
		IndexBufferDsa& operator =(const IndexBufferDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/VertexArray.h                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL vertex array interface
	*/
	class VertexArray : public Rhi::IVertexArray
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Internal resource type
		*/
		class InternalResourceType
		{
		public:
			enum Enum
			{
				NO_VAO = 0,	// No vertex array object
				VAO = 1	// Vertex array object
			};
		};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexArray() override
		{
			// Release the index buffer reference
			if ( nullptr != mIndexBuffer )
			{
				mIndexBuffer->ReleaseReference();
			}

			// Free the unique compact vertex array ID
			static_cast<OpenGLRhi&>(getRhi()).VertexArrayMakeId.DestroyID(getId());
		}

		/**
		*  @brief
		*    Return the used index buffer
		*
		*  @return
		*    The used index buffer, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IndexBuffer* getIndexBuffer() const
		{
			return mIndexBuffer;
		}

		/**
		*  @brief
		*    Return the internal resource type
		*
		*  @return
		*    The internal resource type
		*/
		[[nodiscard]] inline InternalResourceType::Enum getInternalResourceType() const
		{
			return mInternalResourceType;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(VertexArray, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*  @param[in] internalResourceType
		*    Internal resource type
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		inline VertexArray(OpenGLRhi& openGLRhi, IndexBuffer* indexBuffer, InternalResourceType::Enum internalResourceType, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IVertexArray(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mIndexBuffer(indexBuffer),
			mInternalResourceType(internalResourceType)
		{
			// Add a reference to the given index buffer
			if ( nullptr != mIndexBuffer )
			{
				mIndexBuffer->AddReference();
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexArray(const VertexArray&) = delete;
		VertexArray& operator =(const VertexArray&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		IndexBuffer*			   mIndexBuffer;			// Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		InternalResourceType::Enum mInternalResourceType;	// Internal resource type


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/VertexArrayNoVao.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL vertex array class, traditional version
	*/
	class VertexArrayNoVao final : public VertexArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] numberOfVertexBuffers
		*    Number of vertex buffers, having zero vertex buffers is valid
		*  @param[in] vertexBuffers
		*    At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		VertexArrayNoVao(OpenGLRhi& openGLRhi, const Rhi::VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			VertexArray(openGLRhi, indexBuffer, InternalResourceType::NO_VAO, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfAttributes(vertexAttributes.numberOfAttributes),
			mAttributes(mNumberOfAttributes ? RHI_MALLOC_TYPED(Rhi::VertexAttribute, mNumberOfAttributes) : nullptr),
			mNumberOfVertexBuffers(numberOfVertexBuffers),
			mVertexBuffers(numberOfVertexBuffers ? RHI_MALLOC_TYPED(Rhi::VertexArrayVertexBuffer, numberOfVertexBuffers) : nullptr),
			mIsGL_ARB_instanced_arrays(openGLRhi.getExtensions().isGL_ARB_instanced_arrays())
		{
			// Copy over the data
			if ( nullptr != mAttributes )
			{
				memcpy(mAttributes, vertexAttributes.attributes, sizeof(Rhi::VertexAttribute) * mNumberOfAttributes);
			}
			if ( nullptr != mVertexBuffers )
			{
				memcpy(mVertexBuffers, vertexBuffers, sizeof(Rhi::VertexArrayVertexBuffer) * mNumberOfVertexBuffers);
			}

			// Add a reference to the used vertex buffers
			const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = mVertexBuffers + mNumberOfVertexBuffers;
			for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = mVertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer )
			{
				vertexBuffer->vertexBuffer->AddReference();
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VertexArrayNoVao() override
		{
			// Destroy the vertex array attributes
			RHI_FREE(mAttributes);

			// Destroy the vertex array vertex buffers
			if ( nullptr != mVertexBuffers )
			{
				// Release the reference to the used vertex buffers
				const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = mVertexBuffers + mNumberOfVertexBuffers;
				for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = mVertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer )
				{
					vertexBuffer->vertexBuffer->ReleaseReference();
				}

				// Cleanup
				RHI_FREE(mVertexBuffers);
			}
		}

		/**
		*  @brief
		*    Enable OpenGL vertex attribute arrays
		*/
		void enableOpenGLVertexAttribArrays()
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently bound OpenGL array buffer
			// -> Using "GL_EXT_direct_state_access" this would not help in here because "glVertexAttribPointerARB" is not specified there :/
			GLint openGLArrayBufferBackup = 0;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &openGLArrayBufferBackup);
#endif

			// Loop through all attributes
			// -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
			GLuint attributeLocation = 0;
			const Rhi::VertexAttribute* attributeEnd = mAttributes + mNumberOfAttributes;
			for ( const Rhi::VertexAttribute* attribute = mAttributes; attribute < attributeEnd; ++attribute, ++attributeLocation )
			{
				// Set the OpenGL vertex attribute pointer
				// TODO(co) Add security check: Is the given resource one of the currently used RHI?
				const Rhi::VertexArrayVertexBuffer& vertexArrayVertexBuffer = mVertexBuffers[attribute->inputSlot];
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<VertexBuffer*>(vertexArrayVertexBuffer.vertexBuffer)->getOpenGLArrayBuffer());
				if ( Mapping::isOpenGLVertexAttributeFormatInteger(attribute->vertexAttributeFormat) )
				{
					glVertexAttribIPointer(attributeLocation,
						Mapping::getOpenGLSize(attribute->vertexAttributeFormat),
						Mapping::getOpenGLType(attribute->vertexAttributeFormat),
						static_cast<GLsizei>(attribute->strideInBytes),
						reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
				}
				else
				{
					glVertexAttribPointerARB(attributeLocation,
						Mapping::getOpenGLSize(attribute->vertexAttributeFormat),
						Mapping::getOpenGLType(attribute->vertexAttributeFormat),
						static_cast<GLboolean>(Mapping::isOpenGLVertexAttributeFormatNormalized(attribute->vertexAttributeFormat)),
						static_cast<GLsizei>(attribute->strideInBytes),
						reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
				}

				// Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
				if ( attribute->instancesPerElement > 0 && mIsGL_ARB_instanced_arrays )
				{
					glVertexAttribDivisorARB(attributeLocation, attribute->instancesPerElement);
				}

				// Enable OpenGL vertex attribute array
				glEnableVertexAttribArrayARB(attributeLocation);
			}

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL array buffer
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLArrayBufferBackup));
#endif

			// Set the used index buffer
			// -> In case of no index buffer we don't bind buffer 0, there's not really a point in it
			const IndexBuffer* indexBuffer = getIndexBuffer();
			if ( nullptr != indexBuffer )
			{
				// Bind OpenGL element array buffer
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer->getOpenGLElementArrayBuffer());
			}
		}

		/**
		*  @brief
		*    Disable OpenGL vertex attribute arrays
		*/
		void disableOpenGLVertexAttribArrays()
		{
			// No previous bound OpenGL element array buffer restore, there's not really a point in it

			// Loop through all attributes
			// -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
			GLuint attributeLocation = 0;
			const Rhi::VertexAttribute* attributeEnd = mAttributes + mNumberOfAttributes;
			for ( const Rhi::VertexAttribute* attribute = mAttributes; attribute < attributeEnd; ++attribute, ++attributeLocation )
			{
				// Disable OpenGL vertex attribute array
				glDisableVertexAttribArrayARB(attributeLocation);

				// Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
				if ( attribute->instancesPerElement > 0 && mIsGL_ARB_instanced_arrays )
				{
					glVertexAttribDivisorARB(attributeLocation, 0);
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexArrayNoVao(const VertexArrayNoVao&) = delete;
		VertexArrayNoVao& operator =(const VertexArrayNoVao&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		uint32_t					  mNumberOfAttributes;			// Number of attributes (position, color, texture coordinate, normal...), having zero attributes is valid
		Rhi::VertexAttribute*		  mAttributes;					// At least "numberOfAttributes" instances of vertex attributes, can be a null pointer in case there are zero attributes
		uint32_t					  mNumberOfVertexBuffers;		// Number of vertex buffers, having zero vertex buffers is valid
		Rhi::VertexArrayVertexBuffer* mVertexBuffers;				// At least mNumberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		bool						  mIsGL_ARB_instanced_arrays;	// Is the "GL_ARB_instanced_arrays"-extension supported?


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/VertexArrayVao.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL vertex array interface, effective vertex array object (VAO)
	*/
	class VertexArrayVao : public VertexArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VertexArrayVao() override
		{
			// Destroy the OpenGL vertex array
			// -> Silently ignores 0's and names that do not correspond to existing vertex array objects
			glDeleteVertexArrays(1, &mOpenGLVertexArray);

			// Release the reference to the used vertex buffers
			if ( nullptr != mVertexBuffers )
			{
				// Release references
				VertexBuffer** vertexBuffersEnd = mVertexBuffers + mNumberOfVertexBuffers;
				for ( VertexBuffer** vertexBuffer = mVertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer )
				{
					(*vertexBuffer)->ReleaseReference();
				}

				// Cleanup
				RHI_FREE(mVertexBuffers);
			}
		}

		/**
		*  @brief
		*    Return the OpenGL vertex array
		*
		*  @return
		*    The OpenGL vertex array, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLVertexArray() const
		{
			return mOpenGLVertexArray;
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfVertexBuffers
		*    Number of vertex buffers, having zero vertex buffers is valid
		*  @param[in] vertexBuffers
		*    At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		VertexArrayVao(OpenGLRhi& openGLRhi, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			VertexArray(openGLRhi, indexBuffer, InternalResourceType::VAO, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLVertexArray(0),
			mNumberOfVertexBuffers(numberOfVertexBuffers),
			mVertexBuffers(nullptr)
		{
			// Add a reference to the used vertex buffers
			if ( numberOfVertexBuffers > 0 )
			{
				mVertexBuffers = RHI_MALLOC_TYPED(VertexBuffer*, numberOfVertexBuffers);

				// Loop through all vertex buffers
				VertexBuffer** currentVertexBuffers = mVertexBuffers;
				const Rhi::VertexArrayVertexBuffer* vertexBuffersEnd = vertexBuffers + numberOfVertexBuffers;
				for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer, ++currentVertexBuffers )
				{
					// TODO(co) Add security check: Is the given resource one of the currently used RHI?
					*currentVertexBuffers = static_cast<VertexBuffer*>(vertexBuffer->vertexBuffer);
					(*currentVertexBuffers)->AddReference();
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint		   mOpenGLVertexArray;		// OpenGL vertex array ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
		uint32_t	   mNumberOfVertexBuffers;	// Number of vertex buffers
		VertexBuffer** mVertexBuffers;			// Vertex buffers (we keep a reference to it) used by this vertex array, can be a null pointer


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit VertexArrayVao(const VertexArrayVao&) = delete;
		VertexArrayVao& operator =(const VertexArrayVao&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/VertexArrayVaoBind.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL vertex array class, effective vertex array object (VAO), traditional bind version
	*/
	class VertexArrayVaoBind final : public VertexArrayVao
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] numberOfVertexBuffers
		*    Number of vertex buffers, having zero vertex buffers is valid
		*  @param[in] vertexBuffers
		*    At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		VertexArrayVaoBind(OpenGLRhi& openGLRhi, const Rhi::VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			VertexArrayVao(openGLRhi, numberOfVertexBuffers, vertexBuffers, indexBuffer, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Vertex buffer reference handling is done within the base class "VertexArrayVao"

#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently bound OpenGL array buffer
			GLint openGLArrayBufferBackup = 0;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &openGLArrayBufferBackup);

			// Backup the currently bound OpenGL element array buffer
			GLint openGLElementArrayBufferBackup = 0;
			glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, &openGLElementArrayBufferBackup);

			// Backup the currently bound OpenGL vertex array
			GLint openGLVertexArrayBackup = 0;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &openGLVertexArrayBackup);
#endif

			// Create the OpenGL vertex array
			glGenVertexArrays(1, &mOpenGLVertexArray);

			// Bind this OpenGL vertex array
			glBindVertexArray(mOpenGLVertexArray);

			// Loop through all attributes
			// -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
			GLuint attributeLocation = 0;
			const Rhi::VertexAttribute* attributeEnd = vertexAttributes.attributes + vertexAttributes.numberOfAttributes;
			for ( const Rhi::VertexAttribute* attribute = vertexAttributes.attributes; attribute < attributeEnd; ++attribute, ++attributeLocation )
			{
				// Set the OpenGL vertex attribute pointer
				// TODO(co) Add security check: Is the given resource one of the currently used RHI?
				const Rhi::VertexArrayVertexBuffer& vertexArrayVertexBuffer = vertexBuffers[attribute->inputSlot];
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<VertexBuffer*>(vertexArrayVertexBuffer.vertexBuffer)->getOpenGLArrayBuffer());
				if ( Mapping::isOpenGLVertexAttributeFormatInteger(attribute->vertexAttributeFormat) )
				{
					glVertexAttribIPointer(attributeLocation,
						Mapping::getOpenGLSize(attribute->vertexAttributeFormat),
						Mapping::getOpenGLType(attribute->vertexAttributeFormat),
						static_cast<GLsizei>(attribute->strideInBytes),
						reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
				}
				else
				{
					glVertexAttribPointerARB(attributeLocation,
						Mapping::getOpenGLSize(attribute->vertexAttributeFormat),
						Mapping::getOpenGLType(attribute->vertexAttributeFormat),
						static_cast<GLboolean>(Mapping::isOpenGLVertexAttributeFormatNormalized(attribute->vertexAttributeFormat)),
						static_cast<GLsizei>(attribute->strideInBytes),
						reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
				}

				// Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
				if ( attribute->instancesPerElement > 0 && openGLRhi.getExtensions().isGL_ARB_instanced_arrays() )
				{
					glVertexAttribDivisorARB(attributeLocation, attribute->instancesPerElement);
				}

				// Enable OpenGL vertex attribute array
				glEnableVertexAttribArrayARB(attributeLocation);
			}

			// Check the used index buffer
			// -> In case of no index buffer we don't bind buffer 0, there's not really a point in it
			if ( nullptr != indexBuffer )
			{
				// Bind OpenGL element array buffer
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer->getOpenGLElementArrayBuffer());
			}

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL vertex array
			glBindVertexArray(static_cast<GLuint>(openGLVertexArrayBackup));

			// Be polite and restore the previous bound OpenGL element array buffer
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLElementArrayBufferBackup));

			// Be polite and restore the previous bound OpenGL array buffer
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLArrayBufferBackup));
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VAO", 6)	// 6 = "VAO: " including terminating zero
					glObjectLabel(GL_VERTEX_ARRAY, mOpenGLVertexArray, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexArrayVaoBind() override
		{
			// Vertex buffer reference handling is done within the base class "VertexArrayVao"
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexArrayVaoBind(const VertexArrayVaoBind&) = delete;
		VertexArrayVaoBind& operator =(const VertexArrayVaoBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/VertexArrayVaoDsa.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL vertex array class, effective vertex array object (VAO), effective direct state access (DSA)
	*/
	class VertexArrayVaoDsa final : public VertexArrayVao
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] numberOfVertexBuffers
		*    Number of vertex buffers, having zero vertex buffers is valid
		*  @param[in] vertexBuffers
		*    At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		VertexArrayVaoDsa(OpenGLRhi& openGLRhi, const Rhi::VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			VertexArrayVao(openGLRhi, numberOfVertexBuffers, vertexBuffers, indexBuffer, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Vertex buffer reference handling is done within the base class "VertexArrayVao"
			const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
			if ( isArbDsa )
			{
				// Create the OpenGL vertex array
				glCreateVertexArrays(1, &mOpenGLVertexArray);
			}
			else
			{
				// Create the OpenGL vertex array
				glGenVertexArrays(1, &mOpenGLVertexArray);
			}

			// Loop through all attributes
			// -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
			GLuint attributeLocation = 0;
			const Rhi::VertexAttribute* attributeEnd = vertexAttributes.attributes + vertexAttributes.numberOfAttributes;
			for ( const Rhi::VertexAttribute* attribute = vertexAttributes.attributes; attribute < attributeEnd; ++attribute, ++attributeLocation )
			{
				// Set the OpenGL vertex attribute pointer
				// TODO(co) Add security check: Is the given resource one of the currently used RHI?
				const Rhi::VertexArrayVertexBuffer& vertexArrayVertexBuffer = vertexBuffers[attribute->inputSlot];

				if ( isArbDsa )
				{
					// Enable attribute
					glEnableVertexArrayAttrib(mOpenGLVertexArray, attributeLocation);

					// Set up the format for my attribute
					if ( Mapping::isOpenGLVertexAttributeFormatInteger(attribute->vertexAttributeFormat) )
					{
						glVertexArrayAttribIFormat(mOpenGLVertexArray, attributeLocation, Mapping::getOpenGLSize(attribute->vertexAttributeFormat), Mapping::getOpenGLType(attribute->vertexAttributeFormat), static_cast<GLuint>(attribute->alignedByteOffset));
					}
					else
					{
						glVertexArrayAttribFormat(mOpenGLVertexArray, attributeLocation, Mapping::getOpenGLSize(attribute->vertexAttributeFormat), Mapping::getOpenGLType(attribute->vertexAttributeFormat), static_cast<GLboolean>(Mapping::isOpenGLVertexAttributeFormatNormalized(attribute->vertexAttributeFormat)), static_cast<GLuint>(attribute->alignedByteOffset));
					}
					glVertexArrayAttribBinding(mOpenGLVertexArray, attributeLocation, attributeLocation);

					// Bind vertex buffer to buffer point
					glVertexArrayVertexBuffer(mOpenGLVertexArray,
						attributeLocation,
						static_cast<VertexBuffer*>(vertexArrayVertexBuffer.vertexBuffer)->getOpenGLArrayBuffer(),
						0,	// No offset to the first element of the buffer
						static_cast<GLsizei>(attribute->strideInBytes));

					// Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
					if ( attribute->instancesPerElement > 0 && openGLRhi.getExtensions().isGL_ARB_instanced_arrays() )
					{
						glVertexArrayBindingDivisor(mOpenGLVertexArray, attributeLocation, attribute->instancesPerElement);
					}
				}
				else
				{
					glVertexArrayVertexAttribOffsetEXT(mOpenGLVertexArray,
						static_cast<VertexBuffer*>(vertexArrayVertexBuffer.vertexBuffer)->getOpenGLArrayBuffer(),
						attributeLocation, Mapping::getOpenGLSize(attribute->vertexAttributeFormat),
						Mapping::getOpenGLType(attribute->vertexAttributeFormat),
						static_cast<GLboolean>(Mapping::isOpenGLVertexAttributeFormatNormalized(attribute->vertexAttributeFormat)),
						static_cast<GLsizei>(attribute->strideInBytes),
						static_cast<GLintptr>(attribute->alignedByteOffset));

					// Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
					if ( attribute->instancesPerElement > 0 && openGLRhi.getExtensions().isGL_ARB_instanced_arrays() )
					{
						// Sadly, DSA has no support for "GL_ARB_instanced_arrays", so, we have to use the bind way
						// -> Keep the bind-horror as local as possible

#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently bound OpenGL vertex array
						GLint openGLVertexArrayBackup = 0;
						glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &openGLVertexArrayBackup);
#endif

						// Bind this OpenGL vertex array
						glBindVertexArray(mOpenGLVertexArray);

						// Set divisor
						if ( attribute->instancesPerElement > 0 )
						{
							glVertexAttribDivisorARB(attributeLocation, attribute->instancesPerElement);
						}

#if SE_OPENGL_STATE_CLEANUP
						// Be polite and restore the previous bound OpenGL vertex array
						glBindVertexArray(static_cast<GLuint>(openGLVertexArrayBackup));
#endif
					}

					// Enable OpenGL vertex attribute array
					glEnableVertexArrayAttribEXT(mOpenGLVertexArray, attributeLocation);
				}
			}

			// Check the used index buffer
			// -> In case of no index buffer we don't bind buffer 0, there's not really a point in it
			if ( nullptr != indexBuffer )
			{
				if ( isArbDsa )
				{
					// Bind the index buffer
					glVertexArrayElementBuffer(mOpenGLVertexArray, indexBuffer->getOpenGLElementArrayBuffer());
				}
				else
				{
					// Sadly, EXT DSA has no support for element array buffer, so, we have to use the bind way
					// -> Keep the bind-horror as local as possible

#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently bound OpenGL vertex array
					GLint openGLVertexArrayBackup = 0;
					glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &openGLVertexArrayBackup);

					// Backup the currently bound OpenGL element array buffer
					GLint openGLElementArrayBufferBackup = 0;
					glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, &openGLElementArrayBufferBackup);
#endif

					// Bind this OpenGL vertex array
					glBindVertexArray(mOpenGLVertexArray);

					// Bind OpenGL element array buffer
					glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer->getOpenGLElementArrayBuffer());

#if SE_OPENGL_STATE_CLEANUP
					// Be polite and restore the previous bound OpenGL vertex array
					glBindVertexArray(static_cast<GLuint>(openGLVertexArrayBackup));

					// Be polite and restore the previous bound OpenGL element array buffer
					glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLElementArrayBufferBackup));
#endif
				}
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VAO", 6)	// 6 = "VAO: " including terminating zero
					glObjectLabel(GL_VERTEX_ARRAY, mOpenGLVertexArray, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexArrayVaoDsa() override
		{
			// Vertex buffer reference handling is done within the base class "VertexArrayVao"
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexArrayVaoDsa(const VertexArrayVaoDsa&) = delete;
		VertexArrayVaoDsa& operator =(const VertexArrayVaoDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/TextureBuffer.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL texture buffer object (TBO) interface
	*/
	class TextureBuffer : public Rhi::ITextureBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureBuffer() override
		{
			// Destroy the OpenGL texture instance
			// -> Silently ignores 0's and names that do not correspond to existing textures
			glDeleteTextures(1, &mOpenGLTexture);

			// Destroy the OpenGL texture buffer
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteBuffersARB(1, &mOpenGLTextureBuffer);
		}

		/**
		*  @brief
		*    Return the OpenGL texture buffer instance
		*
		*  @return
		*    The OpenGL texture buffer instance, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLTextureBuffer() const
		{
			return mOpenGLTextureBuffer;
		}

		/**
		*  @brief
		*    Return the OpenGL texture instance
		*
		*  @return
		*    The OpenGL texture instance, can be zero if no resource is allocated
		*/
		[[nodiscard]] inline GLuint getOpenGLTexture() const
		{
			return mOpenGLTexture;
		}

		/**
		*  @brief
		*    Return the OpenGL internal format
		*
		*  @return
		*    The OpenGL internal format
		*/
		[[nodiscard]] inline GLuint getOpenGLInternalFormat() const
		{
			return mOpenGLInternalFormat;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TextureBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] textureFormat
		*    Texture buffer data format
		*/
		inline TextureBuffer(OpenGLRhi& openGLRhi, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITextureBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLTextureBuffer(0),
			mOpenGLTexture(0),
			mOpenGLInternalFormat(Mapping::getOpenGLInternalFormat(textureFormat))
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLTextureBuffer;	// OpenGL texture buffer, can be zero if no resource is allocated
		GLuint mOpenGLTexture;			// OpenGL texture, can be zero if no resource is allocated
		GLuint mOpenGLInternalFormat;	// OpenGL internal format


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit TextureBuffer(const TextureBuffer&) = delete;
		TextureBuffer& operator =(const TextureBuffer&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/TextureBufferBind.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL texture buffer object (TBO) class, traditional bind version
	*/
	class TextureBufferBind final : public TextureBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the texture buffer, must be valid
		*  @param[in] data
		*    Texture buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] textureFormat
		*    Texture buffer data format
		*/
		TextureBufferBind(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			TextureBuffer(openGLRhi, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			{ // Buffer part
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently bound OpenGL texture buffer
				GLint openGLTextureBufferBackup = 0;
				glGetIntegerv(GL_TEXTURE_BINDING_BUFFER_ARB, &openGLTextureBufferBackup);
#endif

				// Create the OpenGL texture buffer
				glGenBuffersARB(1, &mOpenGLTextureBuffer);

				// Bind this OpenGL texture buffer and upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glBindBufferARB(GL_TEXTURE_BUFFER_ARB, mOpenGLTextureBuffer);
				glBufferDataARB(GL_TEXTURE_BUFFER_ARB, static_cast<GLsizeiptrARB>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

#if SE_OPENGL_STATE_CLEANUP
				// Be polite and restore the previous bound OpenGL texture buffer
				glBindBufferARB(GL_TEXTURE_BUFFER_ARB, static_cast<GLuint>(openGLTextureBufferBackup));
#endif
			}

			{ // Texture part
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently bound OpenGL texture
				GLint openGLTextureBackup = 0;
				glGetIntegerv(GL_TEXTURE_BINDING_BUFFER_ARB, &openGLTextureBackup);
#endif

				// Create the OpenGL texture instance
				glGenTextures(1, &mOpenGLTexture);

				// Make this OpenGL texture instance to the currently used one
				glBindTexture(GL_TEXTURE_BUFFER_ARB, mOpenGLTexture);

				// Attaches the storage for the buffer object to the active buffer texture
				glTexBufferARB(GL_TEXTURE_BUFFER_ARB, mOpenGLInternalFormat, mOpenGLTextureBuffer);

#if SE_OPENGL_STATE_CLEANUP
				// Be polite and restore the previous bound OpenGL texture
				glBindTexture(GL_TEXTURE_BUFFER_ARB, static_cast<GLuint>(openGLTextureBackup));
#endif
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TBO", 6)	// 6 = "TBO: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
				glObjectLabel(GL_BUFFER, mOpenGLTextureBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureBufferBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureBufferBind(const TextureBufferBind&) = delete;
		TextureBufferBind& operator =(const TextureBufferBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/TextureBufferDsa.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL texture buffer object (TBO) class, effective direct state access (DSA)
	*/
	class TextureBufferDsa final : public TextureBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the texture buffer, must be valid
		*  @param[in] data
		*    Texture buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] textureFormat
		*    Texture buffer data format
		*/
		TextureBufferDsa(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			TextureBuffer(openGLRhi, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			if ( openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
			{
				{ // Buffer part
					// Create the OpenGL texture buffer
					glCreateBuffers(1, &mOpenGLTextureBuffer);

					// Upload the data
					// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
					glNamedBufferData(mOpenGLTextureBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
				}

				{ // Texture part
					// Create the OpenGL texture instance
					glCreateTextures(GL_TEXTURE_BUFFER_ARB, 1, &mOpenGLTexture);

					// Attach the storage for the buffer object to the buffer texture
					glTextureBuffer(mOpenGLTexture, mOpenGLInternalFormat, mOpenGLTextureBuffer);
				}
			}
			else
			{
				// Create the OpenGL texture buffer
				glGenBuffersARB(1, &mOpenGLTextureBuffer);

				// Create the OpenGL texture instance
				glGenTextures(1, &mOpenGLTexture);

				// Buffer part
				// -> Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferDataEXT(mOpenGLTextureBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

				{ // Texture part
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently bound OpenGL texture
					GLint openGLTextureBackup = 0;
					glGetIntegerv(GL_TEXTURE_BINDING_BUFFER_ARB, &openGLTextureBackup);
#endif

					// Make this OpenGL texture instance to the currently used one
					glBindTexture(GL_TEXTURE_BUFFER_ARB, mOpenGLTexture);

					// Attaches the storage for the buffer object to the active buffer texture
					// -> Sadly, there's no direct state access (DSA) function defined for this in "GL_EXT_direct_state_access"
					glTexBufferARB(GL_TEXTURE_BUFFER_ARB, mOpenGLInternalFormat, mOpenGLTextureBuffer);

#if SE_OPENGL_STATE_CLEANUP
					// Be polite and restore the previous bound OpenGL texture
					glBindTexture(GL_TEXTURE_BUFFER_ARB, static_cast<GLuint>(openGLTextureBackup));
#endif
				}
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TBO", 6)	// 6 = "TBO: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
				glObjectLabel(GL_BUFFER, mOpenGLTextureBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureBufferDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureBufferDsa(const TextureBufferDsa&) = delete;
		TextureBufferDsa& operator =(const TextureBufferDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/StructuredBuffer.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL structured buffer object (SBO) interface
	*/
	class StructuredBuffer : public Rhi::IStructuredBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StructuredBuffer() override
		{
			// Destroy the OpenGL structured buffer
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteBuffersARB(1, &mOpenGLStructuredBuffer);
		}

		/**
		*  @brief
		*    Return the OpenGL structured buffer instance
		*
		*  @return
		*    The OpenGL structured buffer instance, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLStructuredBuffer() const
		{
			return mOpenGLStructuredBuffer;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(StructuredBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit StructuredBuffer(OpenGLRhi& openGLRhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IStructuredBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLStructuredBuffer(0)
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLStructuredBuffer;	// OpenGL structured buffer, can be zero if no resource is allocated


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit StructuredBuffer(const StructuredBuffer&) = delete;
		StructuredBuffer& operator =(const StructuredBuffer&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/StructuredBufferBind.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL structured buffer object (SBO) class, traditional bind version
	*/
	class StructuredBufferBind final : public StructuredBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the structured buffer, must be valid
		*  @param[in] data
		*    Structured buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		StructuredBufferBind(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			StructuredBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently bound OpenGL structured buffer
			GLint openGLStructuredBufferBackup = 0;
			glGetIntegerv(GL_SHADER_STORAGE_BUFFER_BINDING, &openGLStructuredBufferBackup);
#endif

			// Create the OpenGL structured buffer
			glGenBuffersARB(1, &mOpenGLStructuredBuffer);

			// Bind this OpenGL structured buffer and upload the data
			// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
			glBindBufferARB(GL_SHADER_STORAGE_BUFFER, mOpenGLStructuredBuffer);
			glBufferDataARB(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptrARB>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL structured buffer
			glBindBufferARB(GL_SHADER_STORAGE_BUFFER, static_cast<GLuint>(openGLStructuredBufferBackup));
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "SBO", 6)	// 6 = "SBO: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLStructuredBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~StructuredBufferBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit StructuredBufferBind(const StructuredBufferBind&) = delete;
		StructuredBufferBind& operator =(const StructuredBufferBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/StructuredBufferDsa.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL structured buffer object (SBO) class, effective direct state access (DSA)
	*/
	class StructuredBufferDsa final : public StructuredBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the structured buffer, must be valid
		*  @param[in] data
		*    Structured buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		StructuredBufferDsa(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			StructuredBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			if ( openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
			{
				// Create the OpenGL structured buffer
				glCreateBuffers(1, &mOpenGLStructuredBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferData(mOpenGLStructuredBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}
			else
			{
				// Create the OpenGL structured buffer
				glGenBuffersARB(1, &mOpenGLStructuredBuffer);

				// Buffer part
				// -> Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferDataEXT(mOpenGLStructuredBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "SBO", 6)	// 6 = "SBO: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLStructuredBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~StructuredBufferDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit StructuredBufferDsa(const StructuredBufferDsa&) = delete;
		StructuredBufferDsa& operator =(const StructuredBufferDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/IndirectBuffer.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL indirect buffer object interface
	*/
	class IndirectBuffer : public Rhi::IIndirectBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IndirectBuffer() override
		{
			// Destroy the OpenGL indirect buffer
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteBuffersARB(1, &mOpenGLIndirectBuffer);
		}

		/**
		*  @brief
		*    Return the OpenGL indirect buffer
		*
		*  @return
		*    The OpenGL indirect buffer, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLIndirectBuffer() const
		{
			return mOpenGLIndirectBuffer;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IIndirectBuffer methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const uint8_t* getEmulationData() const override
		{
			return nullptr;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(IndirectBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit IndirectBuffer(OpenGLRhi& openGLRhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IIndirectBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLIndirectBuffer(0)
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLIndirectBuffer;	// OpenGL indirect buffer, can be zero if no resource is allocated


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit IndirectBuffer(const IndirectBuffer&) = delete;
		IndirectBuffer& operator =(const IndirectBuffer&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/IndirectBufferBind.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL indirect buffer object class, traditional bind version
	*/
	class IndirectBufferBind final : public IndirectBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the indirect buffer, must be valid
		*  @param[in] data
		*    Indirect buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		IndirectBufferBind(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IndirectBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently bound OpenGL indirect buffer
			GLint openGLIndirectBufferBackup = 0;
			glGetIntegerv(GL_DRAW_INDIRECT_BUFFER_BINDING, &openGLIndirectBufferBackup);
#endif

			// Create the OpenGL indirect buffer
			glGenBuffersARB(1, &mOpenGLIndirectBuffer);

			// Bind this OpenGL indirect buffer and upload the data
			// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
			glBindBufferARB(GL_DRAW_INDIRECT_BUFFER, mOpenGLIndirectBuffer);
			glBufferDataARB(GL_DRAW_INDIRECT_BUFFER, static_cast<GLsizeiptrARB>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL indirect buffer
			glBindBufferARB(GL_DRAW_INDIRECT_BUFFER, static_cast<GLuint>(openGLIndirectBufferBackup));
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IndirectBufferObject", 23)	// 23 = "IndirectBufferObject: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLIndirectBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IndirectBufferBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit IndirectBufferBind(const IndirectBufferBind&) = delete;
		IndirectBufferBind& operator =(const IndirectBufferBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/IndirectBufferDsa.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL indirect buffer object class, effective direct state access (DSA)
	*/
	class IndirectBufferDsa final : public IndirectBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the indirect buffer, must be valid
		*  @param[in] data
		*    Indirect buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		IndirectBufferDsa(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IndirectBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			if ( openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
			{
				// Create the OpenGL indirect buffer
				glCreateBuffers(1, &mOpenGLIndirectBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferData(mOpenGLIndirectBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}
			else
			{
				// Create the OpenGL indirect buffer
				glGenBuffersARB(1, &mOpenGLIndirectBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferDataEXT(mOpenGLIndirectBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IndirectBufferObject", 23)	// 23 = "IndirectBufferObject: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLIndirectBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IndirectBufferDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit IndirectBufferDsa(const IndirectBufferDsa&) = delete;
		IndirectBufferDsa& operator =(const IndirectBufferDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/UniformBuffer.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL uniform buffer object (UBO, "constant buffer" in Direct3D terminology) interface
	*/
	class UniformBuffer : public Rhi::IUniformBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~UniformBuffer() override
		{
			// Destroy the OpenGL uniform buffer
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteBuffersARB(1, &mOpenGLUniformBuffer);
		}

		/**
		*  @brief
		*    Return the OpenGL uniform buffer instance
		*
		*  @return
		*    The OpenGL uniform buffer instance, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLUniformBuffer() const
		{
			return mOpenGLUniformBuffer;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(UniformBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit UniformBuffer(OpenGLRhi& openGLRhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IUniformBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLUniformBuffer(0)
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLUniformBuffer;	// OpenGL uniform buffer, can be zero if no resource is allocated


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator =(const UniformBuffer&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/UniformBufferBind.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL uniform buffer object (UBO, "constant buffer" in Direct3D terminology) class, traditional bind version
	*/
	class UniformBufferBind final : public UniformBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the uniform buffer, must be valid
		*  @param[in] data
		*    Uniform buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		UniformBufferBind(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			UniformBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// TODO(co) Review OpenGL uniform buffer alignment topic

#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently bound OpenGL uniform buffer
			GLint openGLUniformBufferBackup = 0;
			glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &openGLUniformBufferBackup);
#endif

			// Create the OpenGL uniform buffer
			glGenBuffersARB(1, &mOpenGLUniformBuffer);

			// Bind this OpenGL uniform buffer and upload the data
			// -> Usage: These constants directly map to GL_ARB_vertex_buffer_object and OpenGL ES 3 constants, do not change them
			glBindBufferARB(GL_UNIFORM_BUFFER, mOpenGLUniformBuffer);
			glBufferDataARB(GL_UNIFORM_BUFFER, static_cast<GLsizeiptrARB>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL uniform buffer
			glBindBufferARB(GL_UNIFORM_BUFFER, static_cast<GLuint>(openGLUniformBufferBackup));
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "UBO", 6)	// 6 = "UBO: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLUniformBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~UniformBufferBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit UniformBufferBind(const UniformBufferBind&) = delete;
		UniformBufferBind& operator =(const UniformBufferBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/UniformBufferDsa.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL uniform buffer object (UBO, "constant buffer" in Direct3D terminology) class, effective direct state access (DSA)
	*/
	class UniformBufferDsa final : public UniformBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the uniform buffer, must be valid
		*  @param[in] data
		*    Uniform buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		UniformBufferDsa(OpenGLRhi& openGLRhi, uint32_t numberOfBytes, const void* data, Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			UniformBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// TODO(co) Review OpenGL uniform buffer alignment topic

			if ( openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
			{
				// Create the OpenGL uniform buffer
				glCreateBuffers(1, &mOpenGLUniformBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferData(mOpenGLUniformBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}
			else
			{
				// Create the OpenGL uniform buffer
				glGenBuffersARB(1, &mOpenGLUniformBuffer);

				// Upload the data
				// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
				glNamedBufferDataEXT(mOpenGLUniformBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "UBO", 6)	// 6 = "UBO: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLUniformBuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~UniformBufferDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit UniformBufferDsa(const UniformBufferDsa&) = delete;
		UniformBufferDsa& operator =(const UniformBufferDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Buffer/BufferManager.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL buffer manager interface
	*/
	class BufferManager final : public Rhi::IBufferManager
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit BufferManager(OpenGLRhi& openGLRhi) :
			IBufferManager(openGLRhi),
			mExtensions(&openGLRhi.getExtensions())
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BufferManager() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IBufferManager methods            ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual Rhi::IVertexBuffer* createVertexBuffer(uint32_t numberOfBytes, const void* data = nullptr, [[maybe_unused]] uint32_t bufferFlags = 0, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// "GL_ARB_vertex_buffer_object" required
			if ( mExtensions->isGL_ARB_vertex_buffer_object() )
			{
				// Is "GL_EXT_direct_state_access" there?
				OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
				if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(VertexBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(VertexBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
			}
			else
			{
				// Error!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IIndexBuffer* createIndexBuffer(uint32_t numberOfBytes, const void* data = nullptr, [[maybe_unused]] uint32_t bufferFlags = 0, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW, Rhi::IndexBufferFormat::Enum indexBufferFormat = Rhi::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// "GL_ARB_vertex_buffer_object" required
			if ( mExtensions->isGL_ARB_vertex_buffer_object() )
			{
				// Is "GL_EXT_direct_state_access" there?
				OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
				if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(IndexBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(IndexBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
			}
			else
			{
				// Error!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IVertexArray* createVertexArray(const Rhi::VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, Rhi::IIndexBuffer* indexBuffer = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity checks
#if SE_DEBUG
			{
				const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
				for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer )
				{
					SE_ASSERT(&openGLRhi == &vertexBuffer->vertexBuffer->getRhi(), "OpenGL error: The given vertex buffer resource is owned by another RHI instance")
				}
			}
#endif
			SE_ASSERT(nullptr == indexBuffer || &openGLRhi == &indexBuffer->getRhi(), "OpenGL error: The given index buffer resource is owned by another RHI instance")

				// Create vertex array
				uint16_t id = 0;
			if ( openGLRhi.VertexArrayMakeId.CreateID(id) )
			{
				// Is "GL_ARB_vertex_array_object" there?
				if ( mExtensions->isGL_ARB_vertex_array_object() )
				{
					// Effective vertex array object (VAO)

					// Is "GL_EXT_direct_state_access" there?
					if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
					{
						// Effective direct state access (DSA)
						return RHI_NEW(VertexArrayVaoDsa)(openGLRhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
					else
					{
						// Traditional bind version
						return RHI_NEW(VertexArrayVaoBind)(openGLRhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
				}
				else
				{
					// Traditional version
					return RHI_NEW(VertexArrayNoVao)(openGLRhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
			}

			// Error: Ensure a correct reference counter behaviour
			const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
			for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer )
			{
				vertexBuffer->vertexBuffer->AddReference();
				vertexBuffer->vertexBuffer->ReleaseReference();
			}
			if ( nullptr != indexBuffer )
			{
				indexBuffer->AddReference();
				indexBuffer->ReleaseReference();
			}
			return nullptr;
		}

		[[nodiscard]] virtual Rhi::ITextureBuffer* createTextureBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t = Rhi::BufferFlag::SHADER_RESOURCE, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW, Rhi::TextureFormat::Enum textureFormat = Rhi::TextureFormat::R32G32B32A32F RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT((numberOfBytes % Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat)) == 0, "The OpenGL texture buffer size must be a multiple of the selected texture format bytes per texel")

				// "GL_ARB_texture_buffer_object" required
				if ( mExtensions->isGL_ARB_texture_buffer_object() )
				{
					// Is "GL_EXT_direct_state_access" there?
					if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
					{
						// Effective direct state access (DSA)
						return RHI_NEW(TextureBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
					else
					{
						// Traditional bind version
						return RHI_NEW(TextureBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
				}
				else
				{
					// Error!
					return nullptr;
				}
		}

		[[nodiscard]] virtual Rhi::IStructuredBuffer* createStructuredBuffer(uint32_t numberOfBytes, const void* data, [[maybe_unused]] uint32_t bufferFlags, Rhi::BufferUsage bufferUsage, [[maybe_unused]] uint32_t numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity checks
			SE_ASSERT((numberOfBytes % numberOfStructureBytes) == 0, "The OpenGL structured buffer size must be a multiple of the given number of structure bytes")
				SE_ASSERT((numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The OpenGL structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

				// "GL_ARB_shader_storage_buffer_object" required
				if ( mExtensions->isGL_ARB_shader_storage_buffer_object() )
				{
					// Is "GL_EXT_direct_state_access" there?
					if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
					{
						// Effective direct state access (DSA)
						return RHI_NEW(StructuredBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
					else
					{
						// Traditional bind version
						return RHI_NEW(StructuredBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
				}
				else
				{
					// Error!
					return nullptr;
				}
		}

		[[nodiscard]] virtual Rhi::IIndirectBuffer* createIndirectBuffer(uint32_t numberOfBytes, const void* data = nullptr, [[maybe_unused]] uint32_t indirectBufferFlags = 0, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity checks
			SE_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 || (indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0, "Invalid OpenGL flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" is missing")
				SE_ASSERT(!((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 && (indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0), "Invalid OpenGL flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" must be set, but not both at one and the same time")
				SE_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) == 0 || (numberOfBytes % sizeof(Rhi::DrawArguments)) == 0, "OpenGL indirect buffer element type flags specification is \"DRAW_ARGUMENTS\" but the given number of bytes don't align to this")
				SE_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) == 0 || (numberOfBytes % sizeof(Rhi::DrawIndexedArguments)) == 0, "OpenGL indirect buffer element type flags specification is \"DRAW_INDEXED_ARGUMENTS\" but the given number of bytes don't align to this")

				// "GL_ARB_draw_indirect" required
				if ( mExtensions->isGL_ARB_draw_indirect() )
				{
					// Is "GL_EXT_direct_state_access" there?
					if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
					{
						// Effective direct state access (DSA)
						return RHI_NEW(IndirectBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
					else
					{
						// Traditional bind version
						return RHI_NEW(IndirectBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
				}
				else
				{
					// Error!
					return nullptr;
				}
		}

		[[nodiscard]] virtual Rhi::IUniformBuffer* createUniformBuffer(uint32_t numberOfBytes, const void* data = nullptr, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// "GL_ARB_uniform_buffer_object" required
			if ( mExtensions->isGL_ARB_uniform_buffer_object() )
			{
				OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

				// Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
				// -> Inside GLSL "layout(binding = 0, std140) writeonly uniform OutputUniformBuffer" will result in the GLSL compiler error "Failed to parse the GLSL shader source code: ERROR: 0:85: 'assign' :  l-value required "anon@6" (can't modify a uniform)"
				// -> Inside GLSL "layout(binding = 0, std430) writeonly buffer  OutputUniformBuffer" will work in OpenGL but will fail in Vulkan with "Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT" Object: "0" Location: "0" Message code: "13" Layer prefix: "Validation" Message: "Object: VK_NULL_HANDLE (Type = 0) | Type mismatch on descriptor slot 0.0 (used as type `ptr to uniform struct of (vec4 of float32)`) but descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER""
				// SE_ASSERT((bufferFlags & Rhi::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid OpenGL buffer flags, uniform buffer can't be used for unordered access")
				// SE_ASSERT((bufferFlags & Rhi::BufferFlag::SHADER_RESOURCE) != 0, "Invalid OpenGL buffer flags, uniform buffer must be used as shader resource")

				// Is "GL_EXT_direct_state_access" there?
				if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(UniformBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(UniformBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
			}
			else
			{
				// Error!
				return nullptr;
			}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(BufferManager, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit BufferManager(const BufferManager&) = delete;
		BufferManager& operator =(const BufferManager&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		const Extensions* mExtensions;	// Extensions instance, always valid


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture1D.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL 1D texture interface
	*/
	class Texture1D : public Rhi::ITexture1D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture1D() override
		{
			// Destroy the OpenGL texture instance
			// -> Silently ignores 0's and names that do not correspond to existing textures
			glDeleteTextures(1, &mOpenGLTexture);
		}

		/**
		*  @brief
		*    Return the OpenGL texture
		*
		*  @return
		*    The OpenGL texture, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLTexture() const
		{
			return mOpenGLTexture;
		}

		/**
		*  @brief
		*    Return the OpenGL internal format
		*
		*  @return
		*    The OpenGL internal format
		*/
		[[nodiscard]] inline GLuint getOpenGLInternalFormat() const
		{
			return mOpenGLInternalFormat;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IResource methods                 ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual void* getInternalResourceHandle() const override
		{
			return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLTexture));
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture1D, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] textureFormat
		*    Texture format
		*/
		inline Texture1D(OpenGLRhi& openGLRhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture1D(openGLRhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLTexture(0),
			mOpenGLInternalFormat(Mapping::getOpenGLInternalFormat(textureFormat))
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLTexture;			// OpenGL texture, can be zero if no resource is allocated
		GLuint mOpenGLInternalFormat;	// OpenGL internal format


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit Texture1D(const Texture1D&) = delete;
		Texture1D& operator =(const Texture1D&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture1DBind.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 1D texture class, traditional bind version
	*/
	class Texture1DBind final : public Texture1D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture1DBind(OpenGLRhi& openGLRhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture1D(openGLRhi, width, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Sanity checks
			SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

				// Create the OpenGL texture instance
				glGenTextures(1, &mOpenGLTexture);

#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently set alignment
			GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

			// Backup the currently bound OpenGL texture
			GLint openGLTextureBackup = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_1D, &openGLTextureBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width) : 1;

			// Make this OpenGL texture instance to the currently used one
			glBindTexture(GL_TEXTURE_1D, mOpenGLTexture);

			// Upload the texture data
			if ( Rhi::TextureFormat::isCompressed(textureFormat) )
			{
				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Upload all mipmaps
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1));
						glCompressedTexImage1DARB(GL_TEXTURE_1D, static_cast<GLint>(mipmap), mOpenGLInternalFormat, static_cast<GLsizei>(width), 0, numberOfBytesPerSlice, data);

						// Move on to the next mipmap and ensure the size is always at least 1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					glCompressedTexImage1DARB(GL_TEXTURE_1D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), 0, static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1)), data);
				}
			}
			else
			{
				// Texture format is not compressed

				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t type = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1));
						glTexImage1D(GL_TEXTURE_1D, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), 0, format, type, data);

						// Move on to the next mipmap and ensure the size is always at least 1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					glTexImage1D(GL_TEXTURE_1D, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
				}
			}

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS) && openGLRhi.getExtensions().isGL_ARB_framebuffer_object() )
			{
				glGenerateMipmap(GL_TEXTURE_1D);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL texture
			glBindTexture(GL_TEXTURE_1D, static_cast<GLuint>(openGLTextureBackup));

			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture", 13)	// 13 = "1D texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture1DBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture1DBind(const Texture1DBind&) = delete;
		Texture1DBind& operator =(const Texture1DBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture1DDsa.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 1D texture class, effective direct state access (DSA)
	*/
	class Texture1DDsa final : public Texture1D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture1DDsa(OpenGLRhi& openGLRhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture1D(openGLRhi, width, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Sanity checks
			SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

				// Multisample texture?
				const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();

#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently set alignment
			GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width) : 1;

			// Create the OpenGL texture instance
			if ( isArbDsa )
			{
				glCreateTextures(GL_TEXTURE_1D, 1, &mOpenGLTexture);
				glTextureStorage1D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(width));
			}
			else
			{
				glGenTextures(1, &mOpenGLTexture);
			}

			// Upload the texture data
			if ( Rhi::TextureFormat::isCompressed(textureFormat) )
			{
				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1));
						if ( isArbDsa )
						{
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glCompressedTextureSubImage1D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, static_cast<GLsizei>(width), format, numberOfBytesPerSlice, data);
						}
						else
						{
							glCompressedTextureImage1DEXT(mOpenGLTexture, GL_TEXTURE_1D, static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width), 0, numberOfBytesPerSlice, data);
						}

						// Move on to the next mipmap and ensure the size is always at least 1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glCompressedTextureSubImage1D(mOpenGLTexture, 0, 0, static_cast<GLsizei>(width), Mapping::getOpenGLFormat(textureFormat), static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1)), data);
						}
					}
					else
					{
						glCompressedTextureImage1DEXT(mOpenGLTexture, GL_TEXTURE_1D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), 0, static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1)), data);
					}
				}
			}
			else
			{
				// Texture format is not compressed

				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t type = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1));
						if ( isArbDsa )
						{
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glTextureSubImage1D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, static_cast<GLsizei>(width), format, type, data);
						}
						else
						{
							glTextureImage1DEXT(mOpenGLTexture, GL_TEXTURE_1D, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), 0, format, type, data);
						}

						// Move on to the next mipmap and ensure the size is always at least 1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glTextureSubImage1D(mOpenGLTexture, 0, 0, static_cast<GLsizei>(width), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
						}
					}
					else
					{
						glTextureImage1DEXT(mOpenGLTexture, GL_TEXTURE_1D, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
					}
				}
			}

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS )
			{
				if ( isArbDsa )
				{
					glGenerateTextureMipmap(mOpenGLTexture);
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
				else
				{
					glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_1D);
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
			}
			else
			{
				if ( isArbDsa )
				{
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				else
				{
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
			}

			if ( isArbDsa )
			{
				glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

#if SE_OPENGL_STATE_CLEANUP
			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture", 13)	// 13 = "1D texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture1DDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture1DDsa(const Texture1DDsa&) = delete;
		Texture1DDsa& operator =(const Texture1DDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture1DArray.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL 1D array texture interface
	*/
	class Texture1DArray : public Rhi::ITexture1DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture1DArray() override
		{
			// Destroy the OpenGL texture instance
			// -> Silently ignores 0's and names that do not correspond to existing textures
			glDeleteTextures(1, &mOpenGLTexture);
		}

		/**
		*  @brief
		*    Return the OpenGL texture
		*
		*  @return
		*    The OpenGL texture, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLTexture() const
		{
			return mOpenGLTexture;
		}

		/**
		*  @brief
		*    Return the OpenGL internal format
		*
		*  @return
		*    The OpenGL internal format
		*/
		[[nodiscard]] inline GLuint getOpenGLInternalFormat() const
		{
			return mOpenGLInternalFormat;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture1DArray, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*  @param[in] textureFormat
		*    Texture format
		*/
		inline Texture1DArray(OpenGLRhi& openGLRhi, uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture1DArray(openGLRhi, width, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLTexture(0),
			mOpenGLInternalFormat(Mapping::getOpenGLInternalFormat(textureFormat))
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLTexture;			// OpenGL texture, can be zero if no resource is allocated
		GLuint mOpenGLInternalFormat;	// OpenGL internal format


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit Texture1DArray(const Texture1DArray&) = delete;
		Texture1DArray& operator =(const Texture1DArray&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture1DArrayBind.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 1D array texture class, traditional bind version
	*/
	class Texture1DArrayBind final : public Texture1DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture1DArrayBind(OpenGLRhi& openGLRhi, uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture1DArray(openGLRhi, width, numberOfSlices, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently set alignment
			GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

			// Backup the currently bound OpenGL texture
			GLint openGLTextureBackup = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_1D_ARRAY_EXT, &openGLTextureBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Create the OpenGL texture instance
			glGenTextures(1, &mOpenGLTexture);

			// Make this OpenGL texture instance to the currently used one
			glBindTexture(GL_TEXTURE_1D_ARRAY_EXT, mOpenGLTexture);

			// TODO(co) Add support for user provided mipmaps
			// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
			//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
			//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
			//   etc.

			// Upload the base map of the texture (mipmaps are automatically created as soon as the base map is changed)
			glTexImage2D(GL_TEXTURE_1D_ARRAY_EXT, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS) && openGLRhi.getExtensions().isGL_ARB_framebuffer_object() )
			{
				glGenerateMipmap(GL_TEXTURE_1D_ARRAY_EXT);
				glTexParameteri(GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			glTexParameteri(GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL texture
			glBindTexture(GL_TEXTURE_1D_ARRAY_EXT, static_cast<GLuint>(openGLTextureBackup));

			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture array", 19)	// 19 = "1D texture array: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture1DArrayBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture1DArrayBind(const Texture1DArrayBind&) = delete;
		Texture1DArrayBind& operator =(const Texture1DArrayBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture1DArrayDsa.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 1D array texture class, effective direct state access (DSA)
	*/
	class Texture1DArrayDsa final : public Texture1DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture1DArrayDsa(OpenGLRhi& openGLRhi, uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture1DArray(openGLRhi, width, numberOfSlices, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently set alignment
			GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width) : 1;

			// Create the OpenGL texture instance
#if SE_PLATFORM_WINDOWS
				// TODO(co) It appears that DSA "glGenerateTextureMipmap()" is not working (one notices the noise) or we're using it wrong, tested with
				//			- "InstancedCubes"-example -> "CubeRendereDrawInstanced"
				//		    - AMD 290X Radeon software version 17.7.2 as well as with GeForce 980m 384.94
				//		    - Windows 10 x64
			const bool isArbDsa = (openGLRhi.getExtensions().isGL_ARB_direct_state_access() && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS) == 0);
#else
			const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
#endif
			if ( isArbDsa )
			{
				glCreateTextures(GL_TEXTURE_1D_ARRAY_EXT, 1, &mOpenGLTexture);
				glTextureStorage2D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices));
			}
			else
			{
				glGenTextures(1, &mOpenGLTexture);
			}

			// Upload the texture data
			if ( Rhi::TextureFormat::isCompressed(textureFormat) )
			{
				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.

					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1) * numberOfSlices);
						if ( isArbDsa )
						{
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glCompressedTextureSubImage2D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), format, numberOfBytesPerSlice, data);
						}
						else
						{
							glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), 0, numberOfBytesPerSlice, data);
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glCompressedTextureSubImage2D(mOpenGLTexture, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), Mapping::getOpenGLFormat(textureFormat), static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1) * numberOfSlices), data);
						}
					}
					else
					{
						glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), 0, static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1) * numberOfSlices), data);
					}
				}
			}
			else
			{
				// Texture format is not compressed

				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.

					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t type = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1) * numberOfSlices);
						if ( isArbDsa )
						{
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glTextureSubImage2D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), format, type, data);
						}
						else
						{
							glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), 0, format, type, data);
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glTextureSubImage2D(mOpenGLTexture, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
						}
					}
					else
					{
						glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(numberOfSlices), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
					}
				}
			}

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS )
			{
				if ( isArbDsa )
				{
					glGenerateTextureMipmap(mOpenGLTexture);
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
				else
				{
					glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT);
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
			}
			else
			{
				if ( isArbDsa )
				{
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				else
				{
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
			}

			if ( isArbDsa )
			{
				glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

#if SE_OPENGL_STATE_CLEANUP
			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture array", 19)	// 19 = "1D texture array: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture1DArrayDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture1DArrayDsa(const Texture1DArrayDsa&) = delete;
		Texture1DArrayDsa& operator =(const Texture1DArrayDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture2D.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL 2D texture interface
	*/
	class Texture2D : public Rhi::ITexture2D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture2D() override
		{
			// Destroy the OpenGL texture instance
			// -> Silently ignores 0's and names that do not correspond to existing textures
			glDeleteTextures(1, &mOpenGLTexture);
		}

		/**
		*  @brief
		*    Return the number of multisamples
		*
		*  @return
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*/
		[[nodiscard]] inline uint8_t getNumberOfMultisamples() const
		{
			return mNumberOfMultisamples;
		}

		/**
		*  @brief
		*    Return the OpenGL texture
		*
		*  @return
		*    The OpenGL texture, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLTexture() const
		{
			return mOpenGLTexture;
		}

		/**
		*  @brief
		*    Return the OpenGL internal format
		*
		*  @return
		*    The OpenGL internal format
		*/
		[[nodiscard]] inline GLuint getOpenGLInternalFormat() const
		{
			return mOpenGLInternalFormat;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Texture2D methods                      ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Set minimum maximum mipmap index
		*
		*  @param[in] minimumMipmapIndex
		*    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
		*  @param[in] maximumMipmapIndex
		*    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
		*/
		virtual void setMinimumMaximumMipmapIndex(uint32_t minimumMipmapIndex, uint32_t maximumMipmapIndex) = 0;


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IResource methods                 ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual void* getInternalResourceHandle() const override
		{
			return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLTexture));
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture2D, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*/
		inline Texture2D(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture2D(openGLRhi, width, height RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfMultisamples(numberOfMultisamples),
			mOpenGLTexture(0),
			mOpenGLInternalFormat(Mapping::getOpenGLInternalFormat(textureFormat))
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		uint8_t mNumberOfMultisamples;	// The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		GLuint  mOpenGLTexture;			// OpenGL texture, can be zero if no resource is allocated
		GLuint  mOpenGLInternalFormat;	// OpenGL internal format


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit Texture2D(const Texture2D&) = delete;
		Texture2D& operator =(const Texture2D&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture2DBind.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 2D texture class, traditional bind version
	*/
	class Texture2DBind final : public Texture2D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*/
		Texture2DBind(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture2D(openGLRhi, width, height, textureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Sanity checks
			SE_ASSERT(numberOfMultisamples == 1 || numberOfMultisamples == 2 || numberOfMultisamples == 4 || numberOfMultisamples == 8, "Invalid OpenGL texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || nullptr == data, "Invalid OpenGL texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS), "Invalid OpenGL texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 == (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS), "Invalid OpenGL texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 != (textureFlags & Rhi::TextureFlag::RENDER_TARGET), "Invalid OpenGL texture parameters")
				SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

				// Create the OpenGL texture instance
				glGenTextures(1, &mOpenGLTexture);

			// Multisample texture?
			if ( numberOfMultisamples > 1 )
			{
#if SE_OPENGL_STATE_CLEANUP
				// Backup the currently bound OpenGL texture
				GLint openGLTextureBackup = 0;
				glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, &openGLTextureBackup);
#endif

				// Make this OpenGL texture instance to the currently used one
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mOpenGLTexture);

				// Define the texture
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numberOfMultisamples, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_TRUE);

#if SE_OPENGL_STATE_CLEANUP
				// Be polite and restore the previous bound OpenGL texture
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLuint>(openGLTextureBackup));
#endif
			}
			else
			{
#if SE_OPENGL_STATE_CLEANUP
				// Backup the currently set alignment
				GLint openGLAlignmentBackup = 0;
				glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

				// Backup the currently bound OpenGL texture
				GLint openGLTextureBackup = 0;
				glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLTextureBackup);
#endif

				// Set correct unpack alignment
				glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

				// Calculate the number of mipmaps
				const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
				const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
				const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width, height) : 1;

				// Make this OpenGL texture instance to the currently used one
				glBindTexture(GL_TEXTURE_2D, mOpenGLTexture);

				// Upload the texture data
				if ( Rhi::TextureFormat::isCompressed(textureFormat) )
				{
					// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
					if ( dataContainsMipmaps )
					{
						// Upload all mipmaps
						for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
						{
							// Upload the current mipmap
							const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height));
							glCompressedTexImage2DARB(GL_TEXTURE_2D, static_cast<GLint>(mipmap), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, numberOfBytesPerSlice, data);

							// Move on to the next mipmap and ensure the size is always at least 1x1
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
							width = getHalfSize(width);
							height = getHalfSize(height);
						}
					}
					else
					{
						// The user only provided us with the base texture, no mipmaps
						glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height)), data);
					}
				}
				else
				{
					// Texture format is not compressed

					// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
					if ( dataContainsMipmaps )
					{
						// Upload all mipmaps
						const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
						const uint32_t type = Mapping::getOpenGLType(textureFormat);
						for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
						{
							// Upload the current mipmap
							const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height));
							glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, format, type, data);

							// Move on to the next mipmap and ensure the size is always at least 1x1
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
							width = getHalfSize(width);
							height = getHalfSize(height);
						}
					}
					else
					{
						// The user only provided us with the base texture, no mipmaps
						glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
					}
				}

				// Build mipmaps automatically on the GPU? (or GPU driver)
				if ( (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS) && openGLRhi.getExtensions().isGL_ARB_framebuffer_object() )
				{
					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if SE_OPENGL_STATE_CLEANUP
				// Be polite and restore the previous bound OpenGL texture
				glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLTextureBackup));

				// Restore previous alignment
				glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)	// 13 = "2D texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture2DBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Texture2D methods                      ]
		//[-------------------------------------------------------]
	public:
		virtual void setMinimumMaximumMipmapIndex(uint32_t minimumMipmapIndex, uint32_t maximumMipmapIndex) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently bound OpenGL texture
			GLint openGLTextureBackup = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLTextureBackup);
#endif

			// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_sampler_objects.txt
			// "
			//  2) What is the set of state associated with a sampler object?
			//     Specifically, should TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL be
			//     part of the sampler or the texture?
			//  DISCUSSION: TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL are presently
			//  part of the image state (texture) and are thus not included in the
			//  sampler object.
			// "
			glBindTexture(GL_TEXTURE_2D, mOpenGLTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, static_cast<GLint>(minimumMipmapIndex));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(maximumMipmapIndex));

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL texture
			glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLTextureBackup));
#endif
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture2DBind(const Texture2DBind&) = delete;
		Texture2DBind& operator =(const Texture2DBind&) = delete;


	};






	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture2DDsa.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 2D texture class, effective direct state access (DSA)
	*/
	class Texture2DDsa final : public Texture2D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*/
		Texture2DDsa(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture2D(openGLRhi, width, height, textureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Sanity checks
			SE_ASSERT(numberOfMultisamples == 1 || numberOfMultisamples == 2 || numberOfMultisamples == 4 || numberOfMultisamples == 8, "Invalid OpenGL texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || nullptr == data, "Invalid OpenGL texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS), "Invalid OpenGL texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 == (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS), "Invalid OpenGL texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 != (textureFlags & Rhi::TextureFlag::RENDER_TARGET), "Invalid OpenGL texture parameters")
				SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

				// Multisample texture?
				const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
			if ( numberOfMultisamples > 1 )
			{
				if ( isArbDsa )
				{
					// Create the OpenGL texture instance
					glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &mOpenGLTexture);
					glTextureStorage2DMultisample(mOpenGLTexture, numberOfMultisamples, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_TRUE);
				}
				else
				{
					// Create the OpenGL texture instance
					glGenTextures(1, &mOpenGLTexture);

#if SE_OPENGL_STATE_CLEANUP
					// Backup the currently bound OpenGL texture
					GLint openGLTextureBackup = 0;
					glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, &openGLTextureBackup);
#endif

					// Make this OpenGL texture instance to the currently used one
					glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mOpenGLTexture);

					// Define the texture
					// -> Sadly, there's no direct state access (DSA) function defined for this in "GL_EXT_direct_state_access"
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numberOfMultisamples, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_TRUE);

#if SE_OPENGL_STATE_CLEANUP
					// Be polite and restore the previous bound OpenGL texture
					glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLuint>(openGLTextureBackup));
#endif
				}
			}
			else
			{
#if SE_OPENGL_STATE_CLEANUP
				// Backup the currently set alignment
				GLint openGLAlignmentBackup = 0;
				glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

				// Set correct unpack alignment
				glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

				// Calculate the number of mipmaps
				const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
				const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
				const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width, height) : 1;

				// Create the OpenGL texture instance
				if ( isArbDsa )
				{
					glCreateTextures(GL_TEXTURE_2D, 1, &mOpenGLTexture);
					glTextureStorage2D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
				}
				else
				{
					glGenTextures(1, &mOpenGLTexture);
				}

				// Upload the texture data
				if ( Rhi::TextureFormat::isCompressed(textureFormat) )
				{
					// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
					if ( dataContainsMipmaps )
					{
						// Upload all mipmaps
						const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
						for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
						{
							// Upload the current mipmap
							const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height));
							if ( isArbDsa )
							{
								// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
								glCompressedTextureSubImage2D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), format, numberOfBytesPerSlice, data);
							}
							else
							{
								glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_2D, static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, numberOfBytesPerSlice, data);
							}

							// Move on to the next mipmap and ensure the size is always at least 1x1
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
							width = getHalfSize(width);
							height = getHalfSize(height);
						}
					}
					else
					{
						// The user only provided us with the base texture, no mipmaps
						if ( isArbDsa )
						{
							if ( nullptr != data )
							{
								glCompressedTextureSubImage2D(mOpenGLTexture, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), Mapping::getOpenGLFormat(textureFormat), static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height)), data);
							}
						}
						else
						{
							glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_2D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height)), data);
						}
					}
				}
				else
				{
					// Texture format is not compressed

					// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
					if ( dataContainsMipmaps )
					{
						// Upload all mipmaps
						const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
						const uint32_t type = Mapping::getOpenGLType(textureFormat);
						for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
						{
							// Upload the current mipmap
							const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height));
							if ( isArbDsa )
							{
								// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
								glTextureSubImage2D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), format, type, data);
							}
							else
							{
								glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_2D, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, format, type, data);
							}

							// Move on to the next mipmap and ensure the size is always at least 1x1
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
							width = getHalfSize(width);
							height = getHalfSize(height);
						}
					}
					else
					{
						// The user only provided us with the base texture, no mipmaps
						if ( isArbDsa )
						{
							if ( nullptr != data )
							{
								glTextureSubImage2D(mOpenGLTexture, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
							}
						}
						else
						{
							glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_2D, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
						}
					}
				}

				// Build mipmaps automatically on the GPU? (or GPU driver)
				if ( textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS )
				{
					if ( isArbDsa )
					{
						glGenerateTextureMipmap(mOpenGLTexture);
						glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					}
					else
					{
						glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_2D);
						glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					}
				}
				else
				{
					if ( isArbDsa )
					{
						glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					}
					else
					{
						glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					}
				}

				if ( isArbDsa )
				{
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				else
				{
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}

#if SE_OPENGL_STATE_CLEANUP
				// Restore previous alignment
				glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)	// 13 = "2D texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture2DDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Texture2D methods                      ]
		//[-------------------------------------------------------]
	public:
		virtual void setMinimumMaximumMipmapIndex(uint32_t minimumMipmapIndex, uint32_t maximumMipmapIndex) override
		{
			// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_sampler_objects.txt
			// "
			//  2) What is the set of state associated with a sampler object?
			//     Specifically, should TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL be
			//     part of the sampler or the texture?
			//  DISCUSSION: TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL are presently
			//  part of the image state (texture) and are thus not included in the
			//  sampler object.
			// "
			glTextureParameteri(mOpenGLTexture, GL_TEXTURE_BASE_LEVEL, static_cast<GLint>(minimumMipmapIndex));
			glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(maximumMipmapIndex));
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture2DDsa(const Texture2DDsa&) = delete;
		Texture2DDsa& operator =(const Texture2DDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture2DArray.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL 2D array texture interface
	*/
	class Texture2DArray : public Rhi::ITexture2DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture2DArray() override
		{
			// Destroy the OpenGL texture instance
			// -> Silently ignores 0's and names that do not correspond to existing textures
			glDeleteTextures(1, &mOpenGLTexture);
		}

		/**
		*  @brief
		*    Return the number of multisamples
		*
		*  @return
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*/
		[[nodiscard]] inline uint8_t getNumberOfMultisamples() const
		{
			return mNumberOfMultisamples;
		}

		/**
		*  @brief
		*    Return the OpenGL texture
		*
		*  @return
		*    The OpenGL texture, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLTexture() const
		{
			return mOpenGLTexture;
		}

		/**
		*  @brief
		*    Return the OpenGL internal format
		*
		*  @return
		*    The OpenGL internal format
		*/
		[[nodiscard]] inline GLuint getOpenGLInternalFormat() const
		{
			return mOpenGLInternalFormat;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture2DArray, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*  @param[in] textureFormat
		*    Texture format
		*/
		inline Texture2DArray(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture2DArray(openGLRhi, width, height, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfMultisamples(1),	// TODO(co) Currently no MSAA support for 2D array textures
			mOpenGLTexture(0),
			mOpenGLInternalFormat(Mapping::getOpenGLInternalFormat(textureFormat))
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		uint8_t mNumberOfMultisamples;	// The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		GLuint  mOpenGLTexture;			// OpenGL texture, can be zero if no resource is allocated
		GLuint  mOpenGLInternalFormat;	// OpenGL internal format


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit Texture2DArray(const Texture2DArray&) = delete;
		Texture2DArray& operator =(const Texture2DArray&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture2DArrayBind.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 2D array texture class, traditional bind version
	*/
	class Texture2DArrayBind final : public Texture2DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture2DArrayBind(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture2DArray(openGLRhi, width, height, numberOfSlices, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently set alignment
			GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

			// Backup the currently bound OpenGL texture
			GLint openGLTextureBackup = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY_EXT, &openGLTextureBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Create the OpenGL texture instance
			glGenTextures(1, &mOpenGLTexture);

			// Make this OpenGL texture instance to the currently used one
			glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, mOpenGLTexture);

			// TODO(co) Add support for user provided mipmaps
			// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
			//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
			//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
			//   etc.

			// Upload the base map of the texture (mipmaps are automatically created as soon as the base map is changed)
			glTexImage3DEXT(GL_TEXTURE_2D_ARRAY_EXT, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS) && openGLRhi.getExtensions().isGL_ARB_framebuffer_object() )
			{
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY_EXT);
				glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL texture
			glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, static_cast<GLuint>(openGLTextureBackup));

			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture array", 19)	// 19 = "2D texture array: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture2DArrayBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture2DArrayBind(const Texture2DArrayBind&) = delete;
		Texture2DArrayBind& operator =(const Texture2DArrayBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture2DArrayDsa.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 2D array texture class, effective direct state access (DSA)
	*/
	class Texture2DArrayDsa final : public Texture2DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture2DArrayDsa(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture2DArray(openGLRhi, width, height, numberOfSlices, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently set alignment
			GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width, height) : 1;

			// Create the OpenGL texture instance
#if SE_PLATFORM_WINDOWS
				// TODO(co) It appears that DSA "glGenerateTextureMipmap()" is not working (one notices the noise) or we're using it wrong, tested with
				//			- "InstancedCubes"-example -> "CubeRendereDrawInstanced"
				//		    - AMD 290X Radeon software version 17.7.2 as well as with GeForce 980m 384.94
				//		    - Windows 10 x64
			const bool isArbDsa = (openGLRhi.getExtensions().isGL_ARB_direct_state_access() && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS) == 0);
#else
			const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
#endif
			if ( isArbDsa )
			{
				glCreateTextures(GL_TEXTURE_2D_ARRAY_EXT, 1, &mOpenGLTexture);
				glTextureStorage3D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices));
			}
			else
			{
				glGenTextures(1, &mOpenGLTexture);
			}

			// Upload the texture data
			if ( Rhi::TextureFormat::isCompressed(textureFormat) )
			{
				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.

					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * numberOfSlices);
						if ( isArbDsa )
						{
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glCompressedTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), format, numberOfBytesPerSlice, data);
						}
						else
						{
							glCompressedTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_2D_ARRAY_EXT, static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), 0, numberOfBytesPerSlice, data);
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
						height = getHalfSize(height);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glCompressedTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), Mapping::getOpenGLFormat(textureFormat), static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * numberOfSlices), data);
						}
					}
					else
					{
						glCompressedTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_2D_ARRAY_EXT, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), 0, static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * numberOfSlices), data);
					}
				}
			}
			else
			{
				// Texture format is not compressed

				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.

					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t type = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * numberOfSlices);
						if ( isArbDsa )
						{
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), format, type, data);
						}
						else
						{
							glTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_2D_ARRAY_EXT, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), 0, format, type, data);
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
						height = getHalfSize(height);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
						}
					}
					else
					{
						glTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_2D_ARRAY_EXT, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(numberOfSlices), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
					}
				}
			}

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS )
			{
				if ( isArbDsa )
				{
					glGenerateTextureMipmap(mOpenGLTexture);
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
				else
				{
					glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_2D_ARRAY_EXT);
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
			}
			else
			{
				if ( isArbDsa )
				{
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				else
				{
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
			}

			if ( isArbDsa )
			{
				glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

#if SE_OPENGL_STATE_CLEANUP
			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture array", 19)	// 19 = "2D texture array: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture2DArrayDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture2DArrayDsa(const Texture2DArrayDsa&) = delete;
		Texture2DArrayDsa& operator =(const Texture2DArrayDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture3D.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL 3D texture interface
	*/
	class Texture3D : public Rhi::ITexture3D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture3D() override
		{
			// Destroy the OpenGL texture instance
			// -> Silently ignores 0's and names that do not correspond to existing textures
			glDeleteTextures(1, &mOpenGLTexture);
			glDeleteBuffersARB(1, &mOpenGLPixelUnpackBuffer);
		}

		/**
		*  @brief
		*    Return the OpenGL texture
		*
		*  @return
		*    The OpenGL texture, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLTexture() const
		{
			return mOpenGLTexture;
		}

		/**
		*  @brief
		*    Return the texture format
		*
		*  @return
		*    The texture format
		*/
		[[nodiscard]] inline Rhi::TextureFormat::Enum getTextureFormat() const
		{
			return mTextureFormat;
		}

		/**
		*  @brief
		*    Return the OpenGL internal format
		*
		*  @return
		*    The OpenGL internal format
		*/
		[[nodiscard]] inline GLuint getOpenGLInternalFormat() const
		{
			return mOpenGLInternalFormat;
		}

		/**
		*  @brief
		*    Return the OpenGL pixel unpack buffer for dynamic textures
		*
		*  @return
		*    The OpenGL pixel unpack buffer for dynamic textures, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLPixelUnpackBuffer() const
		{
			return mOpenGLPixelUnpackBuffer;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IResource methods                 ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual void* getInternalResourceHandle() const override
		{
			return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLTexture));
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture3D, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*  @param[in] depth
		*    The depth of the texture
		*  @param[in] textureFormat
		*    Texture format
		*/
		inline Texture3D(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture3D(openGLRhi, width, height, depth RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLTexture(0),
			mTextureFormat(textureFormat),
			mOpenGLInternalFormat(Mapping::getOpenGLInternalFormat(textureFormat)),
			mOpenGLPixelUnpackBuffer(0)
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint					 mOpenGLTexture;			// OpenGL texture, can be zero if no resource is allocated
		Rhi::TextureFormat::Enum mTextureFormat;
		GLuint					 mOpenGLInternalFormat;		// OpenGL internal format
		GLuint					 mOpenGLPixelUnpackBuffer;	// OpenGL pixel unpack buffer for dynamic textures, can be zero if no resource is allocated


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit Texture3D(const Texture3D&) = delete;
		Texture3D& operator =(const Texture3D&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture3DBind.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 3D texture class, traditional bind version
	*/
	class Texture3DBind final : public Texture3D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] depth
		*    Texture depth, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*/
		Texture3DBind(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags, Rhi::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture3D(openGLRhi, width, height, depth, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Sanity checks
			SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

				// Create the OpenGL texture instance
				glGenTextures(1, &mOpenGLTexture);

#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently set alignment
			GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

			// Backup the currently bound OpenGL texture
			GLint openGLTextureBackup = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_3D, &openGLTextureBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Create OpenGL pixel unpack buffer for dynamic textures, if necessary
			if ( Rhi::TextureUsage::IMMUTABLE != textureUsage )
			{
				// Backup the currently bound OpenGL pixel unpack buffer
#if SE_OPENGL_STATE_CLEANUP
				GLint openGLUnpackBufferBackup = 0;
				glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, &openGLUnpackBufferBackup);
#endif

				// Create the OpenGL pixel unpack buffer
				glGenBuffersARB(1, &mOpenGLPixelUnpackBuffer);

				// Bind this OpenGL pixel unpack buffer, the OpenGL pixel unpack buffer must be able to hold the top-level mipmap
				// TODO(co) Or must the OpenGL pixel unpack buffer able to hold the entire texture including all mipmaps? Depends on the later usage I assume.
				const uint32_t numberOfBytes = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth;
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mOpenGLPixelUnpackBuffer);
				glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, static_cast<GLsizeiptrARB>(numberOfBytes), nullptr, static_cast<GLenum>(GL_STREAM_DRAW));

				// Be polite and restore the previous bound OpenGL pixel unpack buffer
#if SE_OPENGL_STATE_CLEANUP
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, static_cast<GLuint>(openGLUnpackBufferBackup));
#else
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
#endif
			}

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			SE_ASSERT(Rhi::TextureUsage::IMMUTABLE != textureUsage || !generateMipmaps, "OpenGL immutable texture usage can't be combined with automatic mipmap generation")
				const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width, height, depth) : 1;

			// Make this OpenGL texture instance to the currently used one
			glBindTexture(GL_TEXTURE_3D, mOpenGLTexture);

			// Upload the texture data
			if ( Rhi::TextureFormat::isCompressed(textureFormat) )
			{
				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.

					// Upload all mipmaps
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerMipmap = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth);
						glCompressedTexImage3DARB(GL_TEXTURE_3D, static_cast<GLint>(mipmap), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, numberOfBytesPerMipmap, data);

						// Move on to the next mipmap and ensure the size is always at least 1x1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerMipmap;
						width = getHalfSize(width);
						height = getHalfSize(height);
						depth = getHalfSize(depth);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					glCompressedTexImage3DARB(GL_TEXTURE_3D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height)), data);
				}
			}
			else
			{
				// Texture format is not compressed

				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.

					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t type = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerMipmap = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth);
						glTexImage3DEXT(GL_TEXTURE_3D, static_cast<GLint>(mipmap), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, format, type, data);

						// Move on to the next mipmap and ensure the size is always at least 1x1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerMipmap;
						width = getHalfSize(width);
						height = getHalfSize(height);
						depth = getHalfSize(depth);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					glTexImage3DEXT(GL_TEXTURE_3D, 0, static_cast<GLenum>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
				}
			}

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS) && openGLRhi.getExtensions().isGL_ARB_framebuffer_object() )
			{
				glGenerateMipmap(GL_TEXTURE_3D);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL texture
			glBindTexture(GL_TEXTURE_3D, static_cast<GLuint>(openGLTextureBackup));

			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "3D texture", 13)	// 13 = "3D texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture3DBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture3DBind(const Texture3DBind&) = delete;
		Texture3DBind& operator =(const Texture3DBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/Texture3DDsa.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL 3D texture class, effective direct state access (DSA)
	*/
	class Texture3DDsa final : public Texture3D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] depth
		*    Texture depth, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*/
		Texture3DDsa(OpenGLRhi& openGLRhi, uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags, Rhi::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Texture3D(openGLRhi, width, height, depth, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Sanity checks
			SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

#if SE_OPENGL_STATE_CLEANUP
				// Backup the currently set alignment
				GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Create OpenGL pixel unpack buffer for dynamic textures, if necessary
			if ( Rhi::TextureUsage::IMMUTABLE != textureUsage )
			{
				// Create the OpenGL pixel unpack buffer
				glCreateBuffers(1, &mOpenGLPixelUnpackBuffer);

				// The OpenGL pixel unpack buffer must be able to hold the top-level mipmap
				// TODO(co) Or must the OpenGL pixel unpack buffer able to hold the entire texture including all mipmaps? Depends on the later usage I assume.
				const uint32_t numberOfBytes = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth;
				glNamedBufferData(mOpenGLPixelUnpackBuffer, static_cast<GLsizeiptr>(numberOfBytes), nullptr, GL_STREAM_DRAW);
			}

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			SE_ASSERT(Rhi::TextureUsage::IMMUTABLE != textureUsage || !generateMipmaps, "OpenGL immutable texture usage can't be combined with automatic mipmap generation")
				const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width, height, depth) : 1;

			// Create the OpenGL texture instance
			const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
			if ( isArbDsa )
			{
				glCreateTextures(GL_TEXTURE_3D, 1, &mOpenGLTexture);
				glTextureStorage3D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth));
			}
			else
			{
				glGenTextures(1, &mOpenGLTexture);
			}

			// Upload the texture data
			if ( Rhi::TextureFormat::isCompressed(textureFormat) )
			{
				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.

					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerMipmap = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth);
						if ( isArbDsa )
						{
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glCompressedTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), format, numberOfBytesPerMipmap, data);
						}
						else
						{
							glCompressedTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, numberOfBytesPerMipmap, data);
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerMipmap;
						width = getHalfSize(width);
						height = getHalfSize(height);
						depth = getHalfSize(depth);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glCompressedTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), Mapping::getOpenGLFormat(textureFormat), static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height)), data);
						}
					}
					else
					{
						glCompressedTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height)), data);
					}
				}
			}
			else
			{
				// Texture format is not compressed

				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.

					// Upload all mipmaps
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t type = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const GLsizei numberOfBytesPerMipmap = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth);
						if ( isArbDsa )
						{
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), format, type, data);
						}
						else
						{
							glTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, format, type, data);
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerMipmap;
						width = getHalfSize(width);
						height = getHalfSize(height);
						depth = getHalfSize(depth);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
						}
					}
					else
					{
						glTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
					}
				}
			}

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS )
			{
				if ( isArbDsa )
				{
					glGenerateTextureMipmap(mOpenGLTexture);
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
				else
				{
					glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_3D);
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
			}
			else
			{
				if ( isArbDsa )
				{
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				else
				{
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
			}

			if ( isArbDsa )
			{
				glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

#if SE_OPENGL_STATE_CLEANUP
			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "3D texture", 13)	// 13 = "3D texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Texture3DDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture3DDsa(const Texture3DDsa&) = delete;
		Texture3DDsa& operator =(const Texture3DDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/TextureCube.h                       ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL cube texture interface
	*/
	class TextureCube : public Rhi::ITextureCube
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureCube() override
		{
			// Destroy the OpenGL texture instance
			// -> Silently ignores 0's and names that do not correspond to existing textures
			glDeleteTextures(1, &mOpenGLTexture);
		}

		/**
		*  @brief
		*    Return the OpenGL texture
		*
		*  @return
		*    The OpenGL texture, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLTexture() const
		{
			return mOpenGLTexture;
		}

		/**
		*  @brief
		*    Return the OpenGL internal format
		*
		*  @return
		*    The OpenGL internal format
		*/
		[[nodiscard]] inline GLuint getOpenGLInternalFormat() const
		{
			return mOpenGLInternalFormat;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IResource methods                 ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual void* getInternalResourceHandle() const override
		{
			return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLTexture));
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TextureCube, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] textureFormat
		*    Texture format
		*/
		inline TextureCube(OpenGLRhi& openGLRhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITextureCube(openGLRhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLTexture(0),
			mOpenGLInternalFormat(Mapping::getOpenGLInternalFormat(textureFormat))
		{
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLTexture;			// OpenGL texture, can be zero if no resource is allocated
		GLuint mOpenGLInternalFormat;	// OpenGL internal format


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit TextureCube(const TextureCube&) = delete;
		TextureCube& operator =(const TextureCube&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/TextureCubeBind.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL cube texture class, traditional bind version
	*/
	class TextureCubeBind final : public TextureCube
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		TextureCubeBind(OpenGLRhi& openGLRhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			TextureCube(openGLRhi, width, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Sanity checks
			SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

				// Create the OpenGL texture instance
				glGenTextures(1, &mOpenGLTexture);

#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently set alignment
			GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

			// Backup the currently bound OpenGL texture
			GLint openGLTextureBackup = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &openGLTextureBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width) : 1;

			// Make this OpenGL texture instance to the currently used one
			glBindTexture(GL_TEXTURE_CUBE_MAP, mOpenGLTexture);

			// Upload the texture data
			if ( Rhi::TextureFormat::isCompressed(textureFormat) )
			{
				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Face0, Face1, Face2, Face3, Face4, Face5
					//   Mip1: Face0, Face1, Face2, Face3, Face4, Face5
					//   etc.

					// Upload all mipmaps of all faces
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width));
						for ( uint32_t face = 0; face < 6; ++face )
						{
							// Upload the current face
							glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, static_cast<GLint>(mipmap), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, numberOfBytesPerSlice, data);

							// Move on to the next face
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width);
					for ( uint32_t face = 0; face < 6; ++face )
					{
						glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, static_cast<GLsizei>(numberOfBytesPerSlice), data);
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
					}
				}
			}
			else
			{
				// Texture format is not compressed

				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Face0, Face1, Face2, Face3, Face4, Face5
					//   Mip1: Face0, Face1, Face2, Face3, Face4, Face5
					//   etc.

					// Upload all mipmaps of all faces
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t type = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width));
						for ( uint32_t face = 0; face < 6; ++face )
						{
							// Upload the current face
							glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, format, type, data);

							// Move on to the next face
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width);
					const uint32_t openGLFormat = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t openGLType = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t face = 0; face < 6; ++face )
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, openGLFormat, openGLType, data);
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
					}
				}
			}

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS) && openGLRhi.getExtensions().isGL_ARB_framebuffer_object() )
			{
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL texture
			glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<GLuint>(openGLTextureBackup));

			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Cube texture", 15)	// 15 = "Cube texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureCubeBind() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureCubeBind(const TextureCubeBind&) = delete;
		TextureCubeBind& operator =(const TextureCubeBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/TextureCubeDsa.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL cube texture class, effective direct state access (DSA)
	*/
	class TextureCubeDsa final : public TextureCube
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		TextureCubeDsa(OpenGLRhi& openGLRhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			TextureCube(openGLRhi, width, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Sanity checks
			SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

#if SE_OPENGL_STATE_CLEANUP
				// Backup the currently set alignment
				GLint openGLAlignmentBackup = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

			// Set correct unpack alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, (Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width) : 1;

			// Create the OpenGL texture instance
			// TODO(co) "GL_ARB_direct_state_access" AMD graphics card driver bug ahead
			// -> AMD graphics card: 13.02.2017 using Radeon software 17.1.1 on Microsoft Windows: Looks like "GL_ARB_direct_state_access" is broken when trying to use "glCompressedTextureSubImage3D()" for upload
			// -> Describes the same problem: https://community.amd.com/thread/194748 - "Upload data to GL_TEXTURE_CUBE_MAP with glTextureSubImage3D (DSA) broken ?"
#if SE_PLATFORM_WINDOWS
			const bool isArbDsa = false;
#else
			const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
#endif
			if ( isArbDsa )
			{
				glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mOpenGLTexture);
				glTextureStorage2D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(width));
			}
			else
			{
				glGenTextures(1, &mOpenGLTexture);
			}

			// Upload the texture data
			if ( Rhi::TextureFormat::isCompressed(textureFormat) )
			{
				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Face0, Face1, Face2, Face3, Face4, Face5
					//   Mip1: Face0, Face1, Face2, Face3, Face4, Face5
					//   etc.

					// Upload all mipmaps of all faces
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width));
						if ( isArbDsa )
						{
							// With ARB DSA cube maps are a special form of a cube map array so we can upload all 6 faces at once per mipmap
							// See https://www.khronos.org/opengl/wiki/Direct_State_Access (Last paragraph in "Changes from EXT")
							// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
							glCompressedTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 6, format, numberOfBytesPerSlice * 6, data);

							// Move on to the next mipmap
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice * 6;
						}
						else
						{
							for ( uint32_t face = 0; face < 6; ++face )
							{
								// Upload the current face
								glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, numberOfBytesPerSlice, data);

								// Move on to the next face
								data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
							}
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glCompressedTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 6, Mapping::getOpenGLFormat(textureFormat), static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width)) * 6, data);
						}
					}
					else
					{
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width));
						for ( uint32_t face = 0; face < 6; ++face )
						{
							// Upload the current face
							glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, numberOfBytesPerSlice, data);

							// Move on to the next face
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						}
					}
				}
			}
			else
			{
				// Texture format is not compressed

				// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
				if ( dataContainsMipmaps )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Face0, Face1, Face2, Face3, Face4, Face5
					//   Mip1: Face0, Face1, Face2, Face3, Face4, Face5
					//   etc.

					// Upload all mipmaps of all faces
					const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
					const uint32_t type = Mapping::getOpenGLType(textureFormat);
					for ( uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap )
					{
						const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width));
						for ( uint32_t face = 0; face < 6; ++face )
						{
							// Upload the current face
							if ( isArbDsa )
							{
								// We know that "data" must be valid when we're in here due to the "Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
								glTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, static_cast<GLint>(face), static_cast<GLsizei>(width), static_cast<GLsizei>(width), 1, format, type, data);
							}
							else
							{
								glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, format, type, data);
							}

							// Move on to the next face
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						width = getHalfSize(width);
					}
				}
				else
				{
					// The user only provided us with the base texture, no mipmaps
					if ( isArbDsa )
					{
						if ( nullptr != data )
						{
							glTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 6, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
						}
					}
					else
					{
						const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width);
						const uint32_t openGLFormat = Mapping::getOpenGLFormat(textureFormat);
						const uint32_t openGLType = Mapping::getOpenGLType(textureFormat);
						for ( uint32_t face = 0; face < 6; ++face )
						{
							glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, openGLFormat, openGLType, data);
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						}
					}
				}
			}

			// Build mipmaps automatically on the GPU? (or GPU driver)
			if ( textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS )
			{
				if ( isArbDsa )
				{
					glGenerateTextureMipmap(mOpenGLTexture);
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
				else
				{
					glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP);
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}
			}
			else
			{
				if ( isArbDsa )
				{
					glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				else
				{
					glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
			}

			if ( isArbDsa )
			{
				glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

#if SE_OPENGL_STATE_CLEANUP
			// Restore previous alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Cube texture", 15)	// 15 = "Cube texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureCubeDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureCubeDsa(const TextureCubeDsa&) = delete;
		TextureCubeDsa& operator =(const TextureCubeDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Texture/TextureManager.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL texture manager interface
	*/
	class TextureManager final : public Rhi::ITextureManager
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit TextureManager(OpenGLRhi& openGLRhi) :
			ITextureManager(openGLRhi),
			mExtensions(&openGLRhi.getExtensions())
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureManager() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::ITextureManager methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual Rhi::ITexture1D* createTexture1D(uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0, "OpenGL create texture 1D was called with invalid parameters")

				// Create 1D texture resource: Is "GL_EXT_direct_state_access" there?
				// -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
				if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(Texture1DDsa)(openGLRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(Texture1DBind)(openGLRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
		}

		[[nodiscard]] virtual Rhi::ITexture1DArray* createTexture1DArray(uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0 && numberOfSlices > 0, "OpenGL create texture 1D array was called with invalid parameters")

				// Create 1D texture array resource, "GL_EXT_texture_array"-extension required
				// -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
				if ( mExtensions->isGL_EXT_texture_array() )
				{
					// Is "GL_EXT_direct_state_access" there?
					if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
					{
						// Effective direct state access (DSA)
						return RHI_NEW(Texture1DArrayDsa)(openGLRhi, width, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
					else
					{
						// Traditional bind version
						return RHI_NEW(Texture1DArrayBind)(openGLRhi, width, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
				}
				else
				{
					return nullptr;
				}
		}

		[[nodiscard]] virtual Rhi::ITexture2D* createTexture2D(uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT, uint8_t numberOfMultisamples = 1, [[maybe_unused]] const Rhi::OptimizedTextureClearValue* optimizedTextureClearValue = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0 && height > 0, "OpenGL create texture 2D was called with invalid parameters")

				// Create 2D texture resource: Is "GL_EXT_direct_state_access" there?
				// -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
				if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(Texture2DDsa)(openGLRhi, width, height, textureFormat, data, textureFlags, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(Texture2DBind)(openGLRhi, width, height, textureFormat, data, textureFlags, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
		}

		[[nodiscard]] virtual Rhi::ITexture2DArray* createTexture2DArray(uint32_t width, uint32_t height, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0 && height > 0 && numberOfSlices > 0, "OpenGL create texture 2D array was called with invalid parameters")

				// Create 2D texture array resource, "GL_EXT_texture_array"-extension required
				// -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
				if ( mExtensions->isGL_EXT_texture_array() )
				{
					// Is "GL_EXT_direct_state_access" there?
					if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
					{
						// Effective direct state access (DSA)
						return RHI_NEW(Texture2DArrayDsa)(openGLRhi, width, height, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
					else
					{
						// Traditional bind version
						return RHI_NEW(Texture2DArrayBind)(openGLRhi, width, height, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
					}
				}
				else
				{
					return nullptr;
				}
		}

		[[nodiscard]] virtual Rhi::ITexture3D* createTexture3D(uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0 && height > 0 && depth > 0, "OpenGL create texture 3D was called with invalid parameters")

				// Create 3D texture resource: Is "GL_EXT_direct_state_access" there?
				// -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
				if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(Texture3DDsa)(openGLRhi, width, height, depth, textureFormat, data, textureFlags, textureUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(Texture3DBind)(openGLRhi, width, height, depth, textureFormat, data, textureFlags, textureUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
		}

		[[nodiscard]] virtual Rhi::ITextureCube* createTextureCube(uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0, "OpenGL create texture cube was called with invalid parameters")

				// Create cube texture resource
				// -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
				// Is "GL_EXT_direct_state_access" or "GL_ARB_direct_state_access" there?
				if ( mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(TextureCubeDsa)(openGLRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(TextureCubeBind)(openGLRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
		}

		[[nodiscard]] virtual Rhi::ITextureCubeArray* createTextureCubeArray([[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t numberOfSlices, [[maybe_unused]] Rhi::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data = nullptr, [[maybe_unused]] uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// TODO(co) Implement me
#if SE_DEBUG
			debugName = debugName;
#endif
			return nullptr;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TextureManager, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureManager(const TextureManager&) = delete;
		TextureManager& operator =(const TextureManager&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		const Extensions* mExtensions;	// Extensions instance, always valid


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/State/SamplerState.h                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL sampler state interface
	*/
	class SamplerState : public Rhi::ISamplerState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SamplerState() override
		{
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(SamplerState, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit SamplerState(OpenGLRhi& openGLRhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ISamplerState(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SamplerState(const SamplerState&) = delete;
		SamplerState& operator =(const SamplerState&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/State/SamplerStateBind.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL sampler state class, traditional bind version to emulate a sampler object
	*/
	class SamplerStateBind final : public SamplerState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] samplerState
		*    Sampler state to use
		*/
		SamplerStateBind(OpenGLRhi& openGLRhi, const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			SamplerState(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLMagFilterMode(Mapping::getOpenGLMagFilterMode(samplerState.filter)),
			mOpenGLMinFilterMode(Mapping::getOpenGLMinFilterMode(samplerState.filter, samplerState.maxLod > 0.0f)),
			mOpenGLTextureAddressModeS(Mapping::getOpenGLTextureAddressMode(samplerState.addressU)),
			mOpenGLTextureAddressModeT(Mapping::getOpenGLTextureAddressMode(samplerState.addressV)),
			mOpenGLTextureAddressModeR(Mapping::getOpenGLTextureAddressMode(samplerState.addressW)),
			mMipLodBias(samplerState.mipLodBias),
			mMaxAnisotropy(static_cast<float>(samplerState.maxAnisotropy)),	// Maximum anisotropy is "uint32_t" in Direct3D 10 & 11
			mOpenGLCompareMode(Mapping::getOpenGLCompareMode(samplerState.filter)),
			mOpenGLComparisonFunc(Mapping::getOpenGLComparisonFunc(samplerState.comparisonFunc)),
			mMinLod(samplerState.minLod),
			mMaxLod(samplerState.maxLod)
		{
			// Sanity check
			SE_ASSERT(samplerState.maxAnisotropy <= openGLRhi.getCapabilities().maximumAnisotropy, "Maximum OpenGL anisotropy value violated")

				// Rhi::SamplerState::borderColor[4]
				mBorderColor[0] = samplerState.borderColor[0];
			mBorderColor[1] = samplerState.borderColor[1];
			mBorderColor[2] = samplerState.borderColor[2];
			mBorderColor[3] = samplerState.borderColor[3];
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SamplerStateBind() override
		{
		}

		/**
		*  @brief
		*    Set the OpenGL sampler states
		*/
		void setOpenGLSamplerStates() const
		{
			// TODO(co) Support other targets, not just "GL_TEXTURE_2D"

			// Rhi::SamplerState::filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mOpenGLMagFilterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mOpenGLMinFilterMode);

			// Rhi::SamplerState::addressU
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mOpenGLTextureAddressModeS);

			// Rhi::SamplerState::addressV
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mOpenGLTextureAddressModeT);
			/*
			// TODO(co) Support for 3D textures
			// Rhi::SamplerState::addressW
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, mOpenGLTextureAddressModeR);

			// TODO(co) Complete me
			// Rhi::SamplerState::mipLodBias
			// -> "GL_EXT_texture_lod_bias"-extension
			glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_LOD_BIAS, samplerState.mipLodBias);

			// Rhi::SamplerState::maxAnisotropy
			glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, samplerState.maxAnisotropy);

			// Rhi::SamplerState::comparisonFunc
			// -> "GL_EXT_shadow_funcs"/"GL_EXT_shadow_samplers"-extension
			glTexParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_MODE, Mapping::getOpenGLCompareMode(samplerState.filter));
			glTexParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_FUNC, Mapping::getOpenGLComparisonFunc(samplerState.comparisonFunc));

			// Rhi::SamplerState::borderColor[4]
			glSamplerParameterfv(mOpenGLSampler, GL_TEXTURE_BORDER_COLOR, samplerState.borderColor);

			// Rhi::SamplerState::minLod
			glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MIN_LOD, samplerState.minLod);

			// Rhi::SamplerState::maxLod
			glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_LOD, samplerState.maxLod);
			*/
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SamplerStateBind(const SamplerStateBind&) = delete;
		SamplerStateBind& operator =(const SamplerStateBind&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		// "Rhi::SamplerState" translated into OpenGL
		GLint  mOpenGLMagFilterMode;		// Rhi::SamplerState::filter
		GLint  mOpenGLMinFilterMode;		// Rhi::SamplerState::filter
		GLint  mOpenGLTextureAddressModeS;	// Rhi::SamplerState::addressU
		GLint  mOpenGLTextureAddressModeT;	// Rhi::SamplerState::addressV
		GLint  mOpenGLTextureAddressModeR;	// Rhi::SamplerState::addressW
		float  mMipLodBias;					// Rhi::SamplerState::mipLodBias
		float  mMaxAnisotropy;				// Rhi::SamplerState::maxAnisotropy
		GLint  mOpenGLCompareMode;			// Rhi::SamplerState::comparisonFunc
		GLenum mOpenGLComparisonFunc;		// Rhi::SamplerState::comparisonFunc
		float  mBorderColor[4];				// Rhi::SamplerState::borderColor[4]
		float  mMinLod;						// Rhi::SamplerState::minLod
		float  mMaxLod;						// Rhi::SamplerState::maxLod


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/State/SamplerStateDsa.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL sampler state class, direct state access (DSA) version to emulate a sampler object
	*/
	class SamplerStateDsa final : public SamplerState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] samplerState
		*    Sampler state to use
		*/
		inline SamplerStateDsa(OpenGLRhi& openGLRhi, const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			SamplerState(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mSamplerState(samplerState)
		{
			// Sanity check
			SE_ASSERT(samplerState.maxAnisotropy <= openGLRhi.getCapabilities().maximumAnisotropy, "Maximum OpenGL anisotropy value violated")
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SamplerStateDsa() override
		{
		}

		/**
		*  @brief
		*    Set the OpenGL sampler states
		*/
		inline void setOpenGLSamplerStates() const
		{
			// TODO(co) Implement me
			// http://www.opengl.org/registry/specs/ARB/sampler_objects.txt - GL_ARB_sampler_objects
			// http://www.ozone3d.net/blogs/lab/20110908/tutorial-opengl-3-3-sampler-states-configurer-unites-de-texture/#more-701 - sample
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SamplerStateDsa(const SamplerStateDsa&) = delete;
		SamplerStateDsa& operator =(const SamplerStateDsa&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::SamplerState mSamplerState;	// Sampler state


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/State/SamplerStateSo.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL sampler state class, effective sampler object (SO)
	*/
	class SamplerStateSo final : public SamplerState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] samplerState
		*    Sampler state to use
		*/
		SamplerStateSo(OpenGLRhi& openGLRhi, const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			SamplerState(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLSampler(0)
		{
			// Sanity check
			SE_ASSERT(samplerState.maxAnisotropy <= openGLRhi.getCapabilities().maximumAnisotropy, "Maximum OpenGL anisotropy value violated")

				// Create the OpenGL sampler
				glGenSamplers(1, &mOpenGLSampler);

			// Rhi::SamplerState::filter
			glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_MAG_FILTER, Mapping::getOpenGLMagFilterMode(samplerState.filter));
			glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_MIN_FILTER, Mapping::getOpenGLMinFilterMode(samplerState.filter, samplerState.maxLod > 0.0f));

			// Rhi::SamplerState::addressU
			glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_S, Mapping::getOpenGLTextureAddressMode(samplerState.addressU));

			// Rhi::SamplerState::addressV
			glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_T, Mapping::getOpenGLTextureAddressMode(samplerState.addressV));

			// Rhi::SamplerState::addressW
			glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_R, Mapping::getOpenGLTextureAddressMode(samplerState.addressW));

			// Rhi::SamplerState::mipLodBias
			// -> "GL_EXT_texture_lod_bias"-extension
			glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_LOD_BIAS, samplerState.mipLodBias);

			// Rhi::SamplerState::maxAnisotropy
			// -> Maximum anisotropy is "uint32_t" in Direct3D 10 & 11
			glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(samplerState.maxAnisotropy));

			// Rhi::SamplerState::comparisonFunc
			// -> "GL_EXT_shadow_funcs"/"GL_EXT_shadow_samplers"-extension
			glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_MODE, Mapping::getOpenGLCompareMode(samplerState.filter));
			glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(Mapping::getOpenGLComparisonFunc(samplerState.comparisonFunc)));

			// Rhi::SamplerState::borderColor[4]
			glSamplerParameterfv(mOpenGLSampler, GL_TEXTURE_BORDER_COLOR, samplerState.borderColor);

			// Rhi::SamplerState::minLod
			glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MIN_LOD, samplerState.minLod);

			// Rhi::SamplerState::maxLod
			glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_LOD, samplerState.maxLod);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Sampler state", 16)	// 16 = "Sampler state: " including terminating zero
					glObjectLabel(GL_SAMPLER, mOpenGLSampler, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SamplerStateSo() override
		{
			// Destroy the OpenGL sampler
			// -> Silently ignores 0's and names that do not correspond to existing samplers
			glDeleteSamplers(1, &mOpenGLSampler);
		}

		/**
		*  @brief
		*    Return the OpenGL sampler
		*
		*  @return
		*    The OpenGL sampler, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLSampler() const
		{
			return mOpenGLSampler;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SamplerStateSo(const SamplerStateSo&) = delete;
		SamplerStateSo& operator =(const SamplerStateSo&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLSampler;	// OpenGL sampler, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/State/IState.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract state base class
	*/
	class IState
	{


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		inline IState()
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~IState()
		{
		}


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/State/RasterizerState.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL rasterizer state class
	*/
	class RasterizerState final : public IState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rasterizerState
		*    Rasterizer state to use
		*/
		inline explicit RasterizerState(const Rhi::RasterizerState& rasterizerState) :
			mRasterizerState(rasterizerState),
			mOpenGLFrontFaceMode(static_cast<GLenum>(mRasterizerState.frontCounterClockwise ? GL_CCW : GL_CW))
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~RasterizerState()
		{
		}

		/**
		*  @brief
		*    Return the rasterizer state
		*
		*  @return
		*    The rasterizer state
		*/
		[[nodiscard]] inline const Rhi::RasterizerState& getRasterizerState() const
		{
			return mRasterizerState;
		}

		/**
		*  @brief
		*    Set the OpenGL rasterizer states
		*/
		void setOpenGLRasterizerStates() const
		{
			// Rhi::RasterizerState::fillMode
			switch ( mRasterizerState.fillMode )
			{
				// Wireframe
			case Rhi::FillMode::WIREFRAME:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;

				// Solid
			default:
			case Rhi::FillMode::SOLID:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			}

			// Rhi::RasterizerState::cullMode
			switch ( mRasterizerState.cullMode )
			{
				// No culling
			default:
			case Rhi::CullMode::NONE:
				glDisable(GL_CULL_FACE);
				break;

				// Selects clockwise polygons as front-facing
			case Rhi::CullMode::FRONT:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				break;

				// Selects counterclockwise polygons as front-facing
			case Rhi::CullMode::BACK:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;
			}

			// Rhi::RasterizerState::frontCounterClockwise
			glFrontFace(mOpenGLFrontFaceMode);

			// TODO(co) Map the rest of the rasterizer states

			// RasterizerState::depthBias

			// RasterizerState::depthBiasClamp

			// RasterizerState::slopeScaledDepthBias

			// RasterizerState::depthClipEnable
			if ( mRasterizerState.depthClipEnable )
			{
				glDisable(GL_DEPTH_CLAMP);
			}
			else
			{
				glEnable(GL_DEPTH_CLAMP);
			}

			// RasterizerState::scissorEnable
			if ( mRasterizerState.scissorEnable )
			{
				glEnable(GL_SCISSOR_TEST);
			}
			else
			{
				glDisable(GL_SCISSOR_TEST);
			}

			// RasterizerState::multisampleEnable

			// RasterizerState::antialiasedLineEnable
			if ( mRasterizerState.antialiasedLineEnable )
			{
				glEnable(GL_LINE_SMOOTH);
			}
			else
			{
				glDisable(GL_LINE_SMOOTH);
			}
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::RasterizerState mRasterizerState;		// Rasterizer state
		GLenum				 mOpenGLFrontFaceMode;	// OpenGL front face mode


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/State/DepthStencilState.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL depth stencil state class
	*/
	class DepthStencilState final : public IState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] depthStencilState
		*    Depth stencil state to use
		*/
		inline explicit DepthStencilState(const Rhi::DepthStencilState& depthStencilState) :
			mDepthStencilState(depthStencilState),
			mOpenGLDepthMaskEnabled(static_cast<GLboolean>((Rhi::DepthWriteMask::ALL == mDepthStencilState.depthWriteMask) ? GL_TRUE : GL_FALSE)),
			mOpenGLDepthFunc(Mapping::getOpenGLComparisonFunc(depthStencilState.depthFunc))
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~DepthStencilState()
		{
		}

		/**
		*  @brief
		*    Return the depth stencil state
		*
		*  @return
		*    The depth stencil state
		*/
		[[nodiscard]] inline const Rhi::DepthStencilState& getDepthStencilState() const
		{
			return mDepthStencilState;
		}

		/**
		*  @brief
		*    Set the OpenGL depth stencil states
		*/
		void setOpenGLDepthStencilStates() const
		{
			// Rhi::DepthStencilState::depthEnable
			if ( mDepthStencilState.depthEnable )
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}

			// Rhi::DepthStencilState::depthWriteMask
			glDepthMask(mOpenGLDepthMaskEnabled);

			// Rhi::DepthStencilState::depthFunc
			glDepthFunc(static_cast<GLenum>(mOpenGLDepthFunc));

			// TODO(co) Map the rest of the depth stencil states
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::DepthStencilState mDepthStencilState;		// Depth stencil state
		GLboolean			   mOpenGLDepthMaskEnabled;	// OpenGL depth mask enabled state
		GLenum				   mOpenGLDepthFunc;		// OpenGL depth function


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/State/BlendState.h                          ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL blend state class
	*/
	class BlendState final : public IState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] blendState
		*    Blend state to use
		*/
		inline explicit BlendState(const Rhi::BlendState& blendState) :
			mBlendState(blendState),
			mOpenGLSrcBlend(Mapping::getOpenGLBlendType(mBlendState.renderTarget[0].srcBlend)),
			mOpenGLDstBlend(Mapping::getOpenGLBlendType(mBlendState.renderTarget[0].destBlend))
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~BlendState()
		{
		}

		/**
		*  @brief
		*    Return the blend state
		*
		*  @return
		*    The blend state
		*/
		[[nodiscard]] inline const Rhi::BlendState& getBlendState() const
		{
			return mBlendState;
		}

		/**
		*  @brief
		*    Set the OpenGL blend states
		*/
		void setOpenGLBlendStates() const
		{
			// "GL_ARB_multisample"-extension
			if ( mBlendState.alphaToCoverageEnable )
			{
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
			}
			else
			{
				glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
			}

			// TODO(co) Add support for blend state per render target
			if ( mBlendState.renderTarget[0].blendEnable )
			{
				glEnable(GL_BLEND);
				glBlendFunc(mOpenGLSrcBlend, mOpenGLDstBlend);
			}
			else
			{
				glDisable(GL_BLEND);
			}

			// TODO(co) Map the rest of the blend states
			// GL_EXT_blend_func_separate
			// (GL_EXT_blend_equation_separate)
			// GL_EXT_blend_color
			// GL_EXT_blend_minmax
			// GL_EXT_blend_subtract
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::BlendState mBlendState;		// Blend state
		GLenum			mOpenGLSrcBlend;	// OpenGL source blend function
		GLenum			mOpenGLDstBlend;	// OpenGL destination blend function


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/RenderTarget/RenderPass.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL render pass interface
	*/
	class RenderPass final : public Rhi::IRenderPass
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] numberOfColorAttachments
		*    Number of color render target textures, must be <="Rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets"
		*  @param[in] colorAttachmentTextureFormats
		*    The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "numberOfColorAttachments" textures in the provided C-array of pointers
		*  @param[in] depthStencilAttachmentTextureFormat
		*    The optional depth stencil render target texture format, can be a "Rhi::TextureFormat::UNKNOWN" if there should be no depth buffer
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*/
		RenderPass(Rhi::IRhi& rhi, uint32_t numberOfColorAttachments, const Rhi::TextureFormat::Enum* colorAttachmentTextureFormats, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IRenderPass(rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfColorAttachments(numberOfColorAttachments),
			mDepthStencilAttachmentTextureFormat(depthStencilAttachmentTextureFormat),
			mNumberOfMultisamples(numberOfMultisamples)
		{
			SE_ASSERT(mNumberOfColorAttachments < 8, "Invalid number of OpenGL color attachments")
				memcpy(mColorAttachmentTextureFormats, colorAttachmentTextureFormats, sizeof(Rhi::TextureFormat::Enum) * mNumberOfColorAttachments);
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~RenderPass() override
		{
		}

		/**
		*  @brief
		*    Return the number of color render target textures
		*
		*  @return
		*    The number of color render target textures
		*/
		[[nodiscard]] inline uint32_t getNumberOfColorAttachments() const
		{
			return mNumberOfColorAttachments;
		}

		/**
		*  @brief
		*    Return the number of render target textures (color and depth stencil)
		*
		*  @return
		*    The number of render target textures (color and depth stencil)
		*/
		[[nodiscard]] inline uint32_t getNumberOfAttachments() const
		{
			return (mDepthStencilAttachmentTextureFormat != Rhi::TextureFormat::Enum::UNKNOWN) ? (mNumberOfColorAttachments + 1) : mNumberOfColorAttachments;
		}

		/**
		*  @brief
		*    Return the color attachment texture format
		*
		*  @param[in] colorAttachmentIndex
		*    Color attachment index
		*
		*  @return
		*    The color attachment texture format
		*/
		[[nodiscard]] inline Rhi::TextureFormat::Enum getColorAttachmentTextureFormat(uint32_t colorAttachmentIndex) const
		{
			SE_ASSERT(colorAttachmentIndex < mNumberOfColorAttachments, "Invalid OpenGL color attachment index")
				return mColorAttachmentTextureFormats[colorAttachmentIndex];
		}

		/**
		*  @brief
		*    Return the depth stencil attachment texture format
		*
		*  @return
		*    The depth stencil attachment texture format
		*/
		[[nodiscard]] inline Rhi::TextureFormat::Enum getDepthStencilAttachmentTextureFormat() const
		{
			return mDepthStencilAttachmentTextureFormat;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(RenderPass, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit RenderPass(const RenderPass&) = delete;
		RenderPass& operator =(const RenderPass&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		uint32_t				 mNumberOfColorAttachments;
		Rhi::TextureFormat::Enum mColorAttachmentTextureFormats[8];
		Rhi::TextureFormat::Enum mDepthStencilAttachmentTextureFormat;
		uint8_t					 mNumberOfMultisamples;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/QueryPool.h                                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL asynchronous query pool base class
	*/
	class QueryPool : public Rhi::IQueryPool
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] queryType
		*    Query type
		*  @param[in] numberOfQueries
		*    Number of queries
		*/
		inline QueryPool(OpenGLRhi& openGLRhi, Rhi::QueryType queryType, uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IQueryPool(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mQueryType(queryType),
			mNumberOfQueries(numberOfQueries)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QueryPool() override
		{
		}

		/**
		*  @brief
		*    Return the query type
		*
		*  @return
		*    The query type
		*/
		[[nodiscard]] inline Rhi::QueryType getQueryType() const
		{
			return mQueryType;
		}

		/**
		*  @brief
		*    Return the number of queries
		*
		*  @return
		*    The number of queries
		*/
		[[nodiscard]] inline uint32_t getNumberOfQueries() const
		{
			return mNumberOfQueries;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit QueryPool(const QueryPool&) = delete;
		QueryPool& operator =(const QueryPool&) = delete;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		Rhi::QueryType mQueryType;
		uint32_t	   mNumberOfQueries;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/OcclusionTimestampQueryPool.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL asynchronous occlusion and timestamp query pool interface
	*/
	class OcclusionTimestampQueryPool final : public QueryPool
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] queryType
		*    Query type
		*  @param[in] numberOfQueries
		*    Number of queries
		*/
		OcclusionTimestampQueryPool(OpenGLRhi& openGLRhi, Rhi::QueryType queryType, uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			QueryPool(openGLRhi, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLQueries(RHI_MALLOC_TYPED(GLuint, numberOfQueries))
		{
			// If possible, use "glCreateQueries()" (OpenGL 4.5) in order to create the query instance at once
			if ( nullptr != glCreateQueries )
			{
				switch ( queryType )
				{
				case Rhi::QueryType::OCCLUSION:
					glCreateQueries(GL_SAMPLES_PASSED_ARB, static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
					break;

				case Rhi::QueryType::PIPELINE_STATISTICS:
					SE_ASSERT(false, "Use \"OpenGLRhi::PipelineStatisticsQueryPool\"")
						break;

				case Rhi::QueryType::TIMESTAMP:
					glCreateQueries(GL_TIMESTAMP, static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
					break;
				}
			}
			else
			{
				glGenQueriesARB(static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				switch ( queryType )
				{
				case Rhi::QueryType::OCCLUSION:
					{
						RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Occlusion query", 18)	// 18 = "Occlusion query: " including terminating zero
							for ( uint32_t i = 0; i < mNumberOfQueries; ++i )
							{
								glObjectLabel(GL_QUERY, mOpenGLQueries[i], -1, detailedDebugName);
							}
						break;
					}

				case Rhi::QueryType::PIPELINE_STATISTICS:
					SE_ASSERT(false, "Use \"OpenGLRhi::PipelineStatisticsQueryPool\"")
						break;

				case Rhi::QueryType::TIMESTAMP:
					{
						RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Timestamp query", 18)	// 18 = "Timestamp query: " including terminating zero
							for ( uint32_t i = 0; i < mNumberOfQueries; ++i )
							{
								glObjectLabel(GL_QUERY, mOpenGLQueries[i], -1, detailedDebugName);
							}
						break;
					}
				}
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OcclusionTimestampQueryPool() override
		{
			// Destroy the OpenGL queries
			// -> Silently ignores 0's and names that do not correspond to existing queries
			glDeleteQueriesARB(static_cast<GLsizei>(mNumberOfQueries), mOpenGLQueries);
			RHI_FREE(mOpenGLQueries);
		}

		/**
		*  @brief
		*    Return the OpenGL queries
		*
		*  @return
		*    The OpenGL queries
		*/
		[[nodiscard]] inline const GLuint* getOpenGLQueries() const
		{
			return mOpenGLQueries;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(OcclusionTimestampQueryPool, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit OcclusionTimestampQueryPool(const OcclusionTimestampQueryPool&) = delete;
		OcclusionTimestampQueryPool& operator =(const OcclusionTimestampQueryPool&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint* mOpenGLQueries;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/PipelineStatisticsQueryPool.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL asynchronous pipeline statistics query pool interface
	*/
	class PipelineStatisticsQueryPool final : public QueryPool
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] queryType
		*    Query type
		*  @param[in] numberOfQueries
		*    Number of queries
		*/
		PipelineStatisticsQueryPool(OpenGLRhi& openGLRhi, Rhi::QueryType queryType, uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			QueryPool(openGLRhi, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVerticesSubmittedOpenGLQueries(RHI_MALLOC_TYPED(GLuint, numberOfQueries * 11)),
			mPrimitivesSubmittedOpenGLQueries(mVerticesSubmittedOpenGLQueries + numberOfQueries),
			mVertexShaderInvocationsOpenGLQueries(mPrimitivesSubmittedOpenGLQueries + numberOfQueries),
			mGeometryShaderInvocationsOpenGLQueries(mVertexShaderInvocationsOpenGLQueries + numberOfQueries),
			mGeometryShaderPrimitivesEmittedOpenGLQueries(mGeometryShaderInvocationsOpenGLQueries + numberOfQueries),
			mClippingInputPrimitivesOpenGLQueries(mGeometryShaderPrimitivesEmittedOpenGLQueries + numberOfQueries),
			mClippingOutputPrimitivesOpenGLQueries(mClippingInputPrimitivesOpenGLQueries + numberOfQueries),
			mFragmentShaderInvocationsOpenGLQueries(mClippingOutputPrimitivesOpenGLQueries + numberOfQueries),
			mTessControlShaderPatchesOpenGLQueries(mFragmentShaderInvocationsOpenGLQueries + numberOfQueries),
			mTesEvaluationShaderInvocationsOpenGLQueries(mTessControlShaderPatchesOpenGLQueries + numberOfQueries),
			mComputeShaderInvocationsOpenGLQueries(mTesEvaluationShaderInvocationsOpenGLQueries + numberOfQueries)
		{
			// "glCreateQueries()" (OpenGL 4.5) doesn't support "GL_ARB_pipeline_statistics_query"
			glGenQueriesARB(static_cast<GLsizei>(numberOfQueries) * 11, mVerticesSubmittedOpenGLQueries);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				switch ( queryType )
				{
				case Rhi::QueryType::OCCLUSION:
				case Rhi::QueryType::TIMESTAMP:
					SE_ASSERT(false, "Use \"OpenGLRhi::OcclusionTimestampQueryPool\"")
						break;

				case Rhi::QueryType::PIPELINE_STATISTICS:
					{
						// Enforce instant query creation so we can set a debug name
						for ( uint32_t i = 0; i < numberOfQueries; ++i )
						{
							beginQuery(i);
							endQuery();
						}

						// Set debug name
						RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Pipeline statistics query", 28)	// 28 = "Pipeline statistics query: " including terminating zero
							for ( uint32_t i = 0; i < mNumberOfQueries * 11; ++i )
							{
								glObjectLabel(GL_QUERY, mVerticesSubmittedOpenGLQueries[i], -1, detailedDebugName);
							}
						break;
					}
				}
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PipelineStatisticsQueryPool() override
		{
			// Destroy the OpenGL queries
			// -> Silently ignores 0's and names that do not correspond to existing queries
			glDeleteQueriesARB(static_cast<GLsizei>(mNumberOfQueries) * 11, mVerticesSubmittedOpenGLQueries);
			RHI_FREE(mVerticesSubmittedOpenGLQueries);
		}

		void beginQuery(uint32_t queryIndex) const
		{
			glBeginQueryARB(GL_VERTICES_SUBMITTED_ARB, mVerticesSubmittedOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_PRIMITIVES_SUBMITTED_ARB, mPrimitivesSubmittedOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_VERTEX_SHADER_INVOCATIONS_ARB, mVertexShaderInvocationsOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_GEOMETRY_SHADER_INVOCATIONS, mGeometryShaderInvocationsOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB, mGeometryShaderPrimitivesEmittedOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_CLIPPING_INPUT_PRIMITIVES_ARB, mClippingInputPrimitivesOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_CLIPPING_OUTPUT_PRIMITIVES_ARB, mClippingOutputPrimitivesOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_FRAGMENT_SHADER_INVOCATIONS_ARB, mFragmentShaderInvocationsOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_TESS_CONTROL_SHADER_PATCHES_ARB, mTessControlShaderPatchesOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB, mTesEvaluationShaderInvocationsOpenGLQueries[queryIndex]);
			glBeginQueryARB(GL_COMPUTE_SHADER_INVOCATIONS_ARB, mComputeShaderInvocationsOpenGLQueries[queryIndex]);
		}

		void endQuery() const
		{
			glEndQueryARB(GL_VERTICES_SUBMITTED_ARB);
			glEndQueryARB(GL_PRIMITIVES_SUBMITTED_ARB);
			glEndQueryARB(GL_VERTEX_SHADER_INVOCATIONS_ARB);
			glEndQueryARB(GL_GEOMETRY_SHADER_INVOCATIONS);
			glEndQueryARB(GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB);
			glEndQueryARB(GL_CLIPPING_INPUT_PRIMITIVES_ARB);
			glEndQueryARB(GL_CLIPPING_OUTPUT_PRIMITIVES_ARB);
			glEndQueryARB(GL_FRAGMENT_SHADER_INVOCATIONS_ARB);
			glEndQueryARB(GL_TESS_CONTROL_SHADER_PATCHES_ARB);
			glEndQueryARB(GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB);
			glEndQueryARB(GL_COMPUTE_SHADER_INVOCATIONS_ARB);
		}

		bool getQueryPoolResults(uint8_t* data, uint32_t firstQueryIndex, uint32_t numberOfQueries, uint32_t strideInBytes, bool waitForResult) const
		{
			bool resultAvailable = true;

			// Define a helper macro
#define GET_QUERY_RESULT(openGLQueries, queryResult) \
				resultAvailable = getQueryPoolResult(openGLQueries[firstQueryIndex + i], waitForResult, currentPipelineStatisticsQueryResult->queryResult); \
				if (!resultAvailable) \
				{ \
					break; \
				}

			// Get query pool results
			Rhi::PipelineStatisticsQueryResult* currentPipelineStatisticsQueryResult = reinterpret_cast<Rhi::PipelineStatisticsQueryResult*>(data);
			for ( uint32_t i = 0; i < numberOfQueries; ++i )
			{
				GET_QUERY_RESULT(mVerticesSubmittedOpenGLQueries, numberOfInputAssemblerVertices)
					GET_QUERY_RESULT(mPrimitivesSubmittedOpenGLQueries, numberOfInputAssemblerPrimitives)
					GET_QUERY_RESULT(mVertexShaderInvocationsOpenGLQueries, numberOfVertexShaderInvocations)
					GET_QUERY_RESULT(mGeometryShaderInvocationsOpenGLQueries, numberOfGeometryShaderInvocations)
					GET_QUERY_RESULT(mGeometryShaderPrimitivesEmittedOpenGLQueries, numberOfGeometryShaderOutputPrimitives)
					GET_QUERY_RESULT(mClippingInputPrimitivesOpenGLQueries, numberOfClippingInputPrimitives)
					GET_QUERY_RESULT(mClippingOutputPrimitivesOpenGLQueries, numberOfClippingOutputPrimitives)
					GET_QUERY_RESULT(mFragmentShaderInvocationsOpenGLQueries, numberOfFragmentShaderInvocations)
					GET_QUERY_RESULT(mTessControlShaderPatchesOpenGLQueries, numberOfTessellationControlShaderInvocations)
					GET_QUERY_RESULT(mTesEvaluationShaderInvocationsOpenGLQueries, numberOfTessellationEvaluationShaderInvocations)
					GET_QUERY_RESULT(mComputeShaderInvocationsOpenGLQueries, numberOfComputeShaderInvocations)
					currentPipelineStatisticsQueryResult += strideInBytes;
			}

			// Undefine helper macro
#undef GET_QUERY_RESULT

// Done
			return resultAvailable;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(PipelineStatisticsQueryPool, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit PipelineStatisticsQueryPool(const PipelineStatisticsQueryPool&) = delete;
		PipelineStatisticsQueryPool& operator =(const PipelineStatisticsQueryPool&) = delete;

		bool getQueryPoolResult(GLuint openGLQuery, bool waitForResult, uint64_t& queryResult) const
		{
			bool resultAvailable = true;
			GLuint openGLQueryResult = GL_FALSE;
			do
			{
				glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_AVAILABLE_ARB, &openGLQueryResult);
			} while ( waitForResult && GL_TRUE != openGLQueryResult );
			if ( GL_TRUE == openGLQueryResult )
			{
				glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_ARB, &openGLQueryResult);
				queryResult = static_cast<uint64_t>(openGLQueryResult);
			}
			else
			{
				// Result not ready
				resultAvailable = false;
			}

			// Done
			return resultAvailable;
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint* mVerticesSubmittedOpenGLQueries;				// "GL_VERTICES_SUBMITTED_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfInputAssemblerVertices"
		GLuint* mPrimitivesSubmittedOpenGLQueries;				// "GL_PRIMITIVES_SUBMITTED_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfInputAssemblerPrimitives"
		GLuint* mVertexShaderInvocationsOpenGLQueries;			// "GL_VERTEX_SHADER_INVOCATIONS_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfVertexShaderInvocations"
		GLuint* mGeometryShaderInvocationsOpenGLQueries;		// "GL_GEOMETRY_SHADER_INVOCATIONS", "Rhi::PipelineStatisticsQueryResult::numberOfGeometryShaderInvocations"
		GLuint* mGeometryShaderPrimitivesEmittedOpenGLQueries;	// "GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfGeometryShaderOutputPrimitives"
		GLuint* mClippingInputPrimitivesOpenGLQueries;			// "GL_CLIPPING_INPUT_PRIMITIVES_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfClippingInputPrimitives"
		GLuint* mClippingOutputPrimitivesOpenGLQueries;			// "GL_CLIPPING_OUTPUT_PRIMITIVES_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfClippingOutputPrimitives"
		GLuint* mFragmentShaderInvocationsOpenGLQueries;		// "GL_FRAGMENT_SHADER_INVOCATIONS_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfFragmentShaderInvocations"
		GLuint* mTessControlShaderPatchesOpenGLQueries;			// "GL_TESS_CONTROL_SHADER_PATCHES_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfTessellationControlShaderInvocations"
		GLuint* mTesEvaluationShaderInvocationsOpenGLQueries;	// "GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfTessellationEvaluationShaderInvocations"
		GLuint* mComputeShaderInvocationsOpenGLQueries;			// "GL_COMPUTE_SHADER_INVOCATIONS_ARB", "Rhi::PipelineStatisticsQueryResult::numberOfComputeShaderInvocations"


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/RenderTarget/SwapChain.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL swap chain class
	*/
	class SwapChain final : public Rhi::ISwapChain
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] windowHandle
		*    Information about the window to render into
		*  @param[in] useExternalContext
		*    Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
		*/
		SwapChain(Rhi::IRenderPass& renderPass, Rhi::WindowHandle windowHandle, [[maybe_unused]] bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ISwapChain(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNativeWindowHandle(windowHandle.nativeWindowHandle),
#if SE_PLATFORM_WINDOWS
			mOpenGLContext(RHI_NEW(OpenGLContextWindows)(static_cast<const RenderPass&>(renderPass).getDepthStencilAttachmentTextureFormat(), windowHandle.nativeWindowHandle, static_cast<const OpenGLContextWindows*>(&static_cast<OpenGLRhi&>(renderPass.getRhi()).getOpenGLContext()))),
#elif defined LINUX
			mOpenGLContext(RHI_NEW(OpenGLContextLinux)(static_cast<OpenGLRhi&>(renderPass.getRhi()), static_cast<const RenderPass&>(renderPass).getDepthStencilAttachmentTextureFormat(), windowHandle.nativeWindowHandle, useExternalContext, static_cast<const OpenGLContextLinux*>(&static_cast<OpenGLRhi&>(renderPass.getRhi()).getOpenGLContext()))),
#else
#error "Unsupported platform"
#endif
			mOwnsOpenGLContext(true),
			mRenderWindow(windowHandle.renderWindow),
			mVerticalSynchronizationInterval(0),
			mNewVerticalSynchronizationInterval(0)	// 0 instead of ~0u to ensure that we always set the swap interval at least once to have a known initial setting
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SwapChain() override
		{
			if ( mOwnsOpenGLContext )
			{
				RHI_DELETE(IOpenGLContext, mOpenGLContext);
			}
		}

		/**
		*  @brief
		*    Return the OpenGL context
		*
		*  @return
		*    The OpenGL context
		*/
		[[nodiscard]] inline IOpenGLContext& getOpenGLContext() const
		{
			return *mOpenGLContext;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRenderTarget methods             ]
		//[-------------------------------------------------------]
	public:
		virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const override
		{
			// Return stored width and height when both valid
			if ( nullptr != mRenderWindow )
			{
				mRenderWindow->getWidthAndHeight(width, height);
				return;
			}
#if SE_PLATFORM_WINDOWS
			// Is there a valid native OS window?
			if ( SE_NULL_HANDLE != mNativeWindowHandle )
			{
				// Get the width and height
				long swapChainWidth = 1;
				long swapChainHeight = 1;
				{
					// Get the client rectangle of the native output window
					// -> Don't use the width and height stored in "DXGI_SWAP_CHAIN_DESC" -> "DXGI_MODE_DESC"
					//    because it might have been modified in order to avoid zero values
					RECT rect;
					::GetClientRect(reinterpret_cast<HWND>(mNativeWindowHandle), &rect);

					// Get the width and height...
					swapChainWidth = rect.right - rect.left;
					swapChainHeight = rect.bottom - rect.top;

					// ... and ensure that none of them is ever zero
					if ( swapChainWidth < 1 )
					{
						swapChainWidth = 1;
					}
					if ( swapChainHeight < 1 )
					{
						swapChainHeight = 1;
					}
				}

				// Write out the width and height
				width = static_cast<UINT>(swapChainWidth);
				height = static_cast<UINT>(swapChainHeight);
			}
			else
#elif defined LINUX
			if ( SE_NULL_HANDLE != mNativeWindowHandle )
			{
				OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
				Display* display = static_cast<const OpenGLContextLinux&>(openGLRhi.getOpenGLContext()).getDisplay();

				// Get the width and height...
				::Window rootWindow = 0;
				int positionX = 0, positionY = 0;
				unsigned int unsignedWidth = 0, unsignedHeight = 0, border = 0, depth = 0;
				if ( nullptr != display )
				{
					XGetGeometry(display, mNativeWindowHandle, &rootWindow, &positionX, &positionY, &unsignedWidth, &unsignedHeight, &border, &depth);
				}

				// ... and ensure that none of them is ever zero
				if ( unsignedWidth < 1 )
				{
					unsignedWidth = 1;
				}
				if ( unsignedHeight < 1 )
				{
					unsignedHeight = 1;
				}

				// Done
				width = unsignedWidth;
				height = unsignedHeight;
			}
			else
#else
#error "Unsupported platform"
#endif
			{
				// Set known default return values
				width = 1;
				height = 1;
			}
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::ISwapChain methods                ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual handle getNativeWindowHandle() const override
		{
			return mNativeWindowHandle;
		}

		inline virtual void setVerticalSynchronizationInterval(uint32_t synchronizationInterval) override
		{
			mNewVerticalSynchronizationInterval = synchronizationInterval;
		}

		virtual void present() override
		{
			if ( nullptr != mRenderWindow )
			{
				mRenderWindow->present();
				return;
			}
#if SE_PLATFORM_WINDOWS
			{
				// Set new vertical synchronization interval?
				// -> We do this in here to avoid having to use "wglMakeCurrent()"/"glXMakeCurrent()" to often at multiple places
				if ( ~0u != mNewVerticalSynchronizationInterval )
				{
					const Extensions& extensions = static_cast<OpenGLRhi&>(getRhi()).getExtensions();
					if ( extensions.isWGL_EXT_swap_control() )
					{
						// Use adaptive vertical synchronization if possible
						wglSwapIntervalEXT((extensions.isWGL_EXT_swap_control_tear() && mNewVerticalSynchronizationInterval > 0) ? -static_cast<int>(mNewVerticalSynchronizationInterval) : static_cast<int>(mNewVerticalSynchronizationInterval));
					}
					mVerticalSynchronizationInterval = mNewVerticalSynchronizationInterval;
					mNewVerticalSynchronizationInterval = ~0u;
				}

				// Swap buffers
				::SwapBuffers(static_cast<OpenGLContextWindows*>(mOpenGLContext)->getDeviceContext());
				if ( mVerticalSynchronizationInterval > 0 )
				{
					glFinish();
				}
			}
#elif defined LINUX
			// TODO(co) Add support for vertical synchronization and adaptive vertical synchronization: "GLX_EXT_swap_control" and "GLX_EXT_swap_control_tear"
			if ( SE_NULL_HANDLE != mNativeWindowHandle )
			{
				glXSwapBuffers(static_cast<const OpenGLContextLinux&>(static_cast<OpenGLRhi&>(getRhi()).getOpenGLContext()).getDisplay(), mNativeWindowHandle);
			}
#else
#error "Unsupported platform"
#endif
		}

		inline virtual void resizeBuffers() override
		{
			// Nothing here
		}

		[[nodiscard]] inline virtual bool getFullscreenState() const override
		{
			// TODO(co) Implement me
			return false;
		}

		inline virtual void setFullscreenState(bool) override
		{
			// TODO(co) Implement me
		}

		inline virtual void setRenderWindow(Rhi::IRenderWindow* renderWindow) override
		{
			mRenderWindow = renderWindow;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(SwapChain, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SwapChain(const SwapChain&) = delete;
		SwapChain& operator =(const SwapChain&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		handle			mNativeWindowHandle;	// Native window handle window, can be a null handle
		IOpenGLContext*		mOpenGLContext;			// OpenGL context, must be valid
		bool				mOwnsOpenGLContext;		// Does this swap chain own the OpenGL context?
		Rhi::IRenderWindow* mRenderWindow;			// Render window instance, can be a null pointer, don't destroy the instance since we don't own it
		uint32_t			mVerticalSynchronizationInterval;
		uint32_t			mNewVerticalSynchronizationInterval;


	};

	//[-------------------------------------------------------]
	//[ OpenGLRhi/RenderTarget/Framebuffer.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract OpenGL framebuffer interface
	*/
	class Framebuffer : public Rhi::IFramebuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Framebuffer() override
		{
			// Destroy the OpenGL framebuffer
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteFramebuffers(1, &mOpenGLFramebuffer);

			// Release the reference to the used color textures
			if ( nullptr != mColorTextures )
			{
				// Release references
				Rhi::ITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
				for ( Rhi::ITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture )
				{
					(*colorTexture)->ReleaseReference();
				}

				// Cleanup
				RHI_FREE(mColorTextures);
			}

			// Release the reference to the used depth stencil texture
			if ( nullptr != mDepthStencilTexture )
			{
				// Release reference
				mDepthStencilTexture->ReleaseReference();
			}
		}

		/**
		*  @brief
		*    Return the OpenGL framebuffer
		*
		*  @return
		*    The OpenGL framebuffer, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLFramebuffer() const
		{
			return mOpenGLFramebuffer;
		}

		/**
		*  @brief
		*    Return the number of color render target textures
		*
		*  @return
		*    The number of color render target textures
		*/
		[[nodiscard]] inline uint32_t getNumberOfColorTextures() const
		{
			return mNumberOfColorTextures;
		}

		/**
		*  @brief
		*    Return whether or not the framebuffer is a multisample render target
		*
		*  @return
		*    "true" if the framebuffer is a multisample render target, else "false"
		*/
		[[nodiscard]] inline bool isMultisampleRenderTarget() const
		{
			return mMultisampleRenderTarget;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IResource methods                 ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual void* getInternalResourceHandle() const override
		{
			return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLFramebuffer));
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRenderTarget methods             ]
		//[-------------------------------------------------------]
	public:
		inline virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const override
		{
			// No fancy implementation in here, just copy over the internal information
			width = mWidth;
			height = mHeight;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Framebuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] colorFramebufferAttachments
		*    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "Rhi::IRenderPass::getNumberOfColorAttachments()" textures in the provided C-array of pointers
		*  @param[in] depthStencilFramebufferAttachment
		*    The depth stencil render target texture, can be a null pointer
		*
		*  @note
		*    - The framebuffer keeps a reference to the provided texture instances
		*/
		Framebuffer(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLFramebuffer(0),
			mNumberOfColorTextures(static_cast<RenderPass&>(renderPass).getNumberOfColorAttachments()),
			mColorTextures(nullptr),	// Set below
			mDepthStencilTexture(nullptr),
			mWidth(UINT_MAX),
			mHeight(UINT_MAX),
			mMultisampleRenderTarget(false)
		{
			// The "GL_ARB_framebuffer_object"-extension documentation says the following about the framebuffer width and height
			//   "If the attachment sizes are not all identical, rendering will be limited to the largest area that can fit in
			//    all of the attachments (i.e. an intersection of rectangles having a lower left of (0,0) and an upper right of
			//    (width,height) for each attachment)"

			// Add a reference to the used color textures
			if ( mNumberOfColorTextures > 0 )
			{
				mColorTextures = RHI_MALLOC_TYPED(Rhi::ITexture*, mNumberOfColorTextures);

				// Loop through all color textures
				Rhi::ITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
				for ( Rhi::ITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture, ++colorFramebufferAttachments )
				{
					// Sanity check
					SE_ASSERT(nullptr != colorFramebufferAttachments->texture, "Invalid OpenGL color framebuffer attachment texture")

						// TODO(co) Add security check: Is the given resource one of the currently used RHI?
						*colorTexture = colorFramebufferAttachments->texture;
					(*colorTexture)->AddReference();

					// Evaluate the color texture type
					switch ( (*colorTexture)->getResourceType() )
					{
					case Rhi::ResourceType::TEXTURE_2D:
						{
							const Texture2D* texture2D = static_cast<Texture2D*>(*colorTexture);

							// Sanity checks
							SE_ASSERT(colorFramebufferAttachments->mipmapIndex < Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid OpenGL color framebuffer attachment mipmap index")
								SE_ASSERT(0 == colorFramebufferAttachments->layerIndex, "Invalid OpenGL color framebuffer attachment layer index")

								// Update the framebuffer width and height if required
								::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2D->getWidth(), texture2D->getHeight(), mWidth, mHeight);
							break;
						}

					case Rhi::ResourceType::TEXTURE_2D_ARRAY:
						{
							// Update the framebuffer width and height if required
							const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(*colorTexture);
							::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2DArray->getWidth(), texture2DArray->getHeight(), mWidth, mHeight);
							break;
						}

					case Rhi::ResourceType::ROOT_SIGNATURE:
					case Rhi::ResourceType::RESOURCE_GROUP:
					case Rhi::ResourceType::GRAPHICS_PROGRAM:
					case Rhi::ResourceType::VERTEX_ARRAY:
					case Rhi::ResourceType::RENDER_PASS:
					case Rhi::ResourceType::QUERY_POOL:
					case Rhi::ResourceType::SWAP_CHAIN:
					case Rhi::ResourceType::FRAMEBUFFER:
					case Rhi::ResourceType::VERTEX_BUFFER:
					case Rhi::ResourceType::INDEX_BUFFER:
					case Rhi::ResourceType::TEXTURE_BUFFER:
					case Rhi::ResourceType::STRUCTURED_BUFFER:
					case Rhi::ResourceType::INDIRECT_BUFFER:
					case Rhi::ResourceType::UNIFORM_BUFFER:
					case Rhi::ResourceType::TEXTURE_1D:
					case Rhi::ResourceType::TEXTURE_1D_ARRAY:
					case Rhi::ResourceType::TEXTURE_3D:
					case Rhi::ResourceType::TEXTURE_CUBE:
					case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
					case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
					case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
					case Rhi::ResourceType::SAMPLER_STATE:
					case Rhi::ResourceType::VERTEX_SHADER:
					case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
					case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
					case Rhi::ResourceType::GEOMETRY_SHADER:
					case Rhi::ResourceType::FRAGMENT_SHADER:
					case Rhi::ResourceType::TASK_SHADER:
					case Rhi::ResourceType::MESH_SHADER:
					case Rhi::ResourceType::COMPUTE_SHADER:
					default:
						// Nothing here
						break;
					}
				}
			}

			// Add a reference to the used depth stencil texture
			if ( nullptr != depthStencilFramebufferAttachment )
			{
				mDepthStencilTexture = depthStencilFramebufferAttachment->texture;
				SE_ASSERT(nullptr != mDepthStencilTexture, "Invalid OpenGL depth stencil framebuffer attachment texture")
					mDepthStencilTexture->AddReference();

				// Evaluate the depth stencil texture type
				switch ( mDepthStencilTexture->getResourceType() )
				{
				case Rhi::ResourceType::TEXTURE_2D:
					{
						const Texture2D* texture2D = static_cast<Texture2D*>(mDepthStencilTexture);

						// Sanity checks
						SE_ASSERT(depthStencilFramebufferAttachment->mipmapIndex < Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid OpenGL depth stencil framebuffer attachment mipmap index")
							SE_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex, "Invalid OpenGL depth stencil framebuffer attachment layer index")

							// Update the framebuffer width and height if required
							::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2D->getWidth(), texture2D->getHeight(), mWidth, mHeight);
						break;
					}

				case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					{
						// Update the framebuffer width and height if required
						const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(mDepthStencilTexture);
						::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2DArray->getWidth(), texture2DArray->getHeight(), mWidth, mHeight);
						break;
					}

				case Rhi::ResourceType::ROOT_SIGNATURE:
				case Rhi::ResourceType::RESOURCE_GROUP:
				case Rhi::ResourceType::GRAPHICS_PROGRAM:
				case Rhi::ResourceType::VERTEX_ARRAY:
				case Rhi::ResourceType::RENDER_PASS:
				case Rhi::ResourceType::QUERY_POOL:
				case Rhi::ResourceType::SWAP_CHAIN:
				case Rhi::ResourceType::FRAMEBUFFER:
				case Rhi::ResourceType::VERTEX_BUFFER:
				case Rhi::ResourceType::INDEX_BUFFER:
				case Rhi::ResourceType::TEXTURE_BUFFER:
				case Rhi::ResourceType::STRUCTURED_BUFFER:
				case Rhi::ResourceType::INDIRECT_BUFFER:
				case Rhi::ResourceType::UNIFORM_BUFFER:
				case Rhi::ResourceType::TEXTURE_1D:
				case Rhi::ResourceType::TEXTURE_1D_ARRAY:
				case Rhi::ResourceType::TEXTURE_3D:
				case Rhi::ResourceType::TEXTURE_CUBE:
				case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
				case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
				case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
				case Rhi::ResourceType::SAMPLER_STATE:
				case Rhi::ResourceType::VERTEX_SHADER:
				case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
				case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
				case Rhi::ResourceType::GEOMETRY_SHADER:
				case Rhi::ResourceType::FRAGMENT_SHADER:
				case Rhi::ResourceType::TASK_SHADER:
				case Rhi::ResourceType::MESH_SHADER:
				case Rhi::ResourceType::COMPUTE_SHADER:
				default:
					// Nothing here
					break;
				}
			}

			// Validate the framebuffer width and height
			if ( 0 == mWidth || UINT_MAX == mWidth )
			{
				SE_ASSERT(false, "Invalid OpenGL framebuffer width")
					mWidth = 1;
			}
			if ( 0 == mHeight || UINT_MAX == mHeight )
			{
				SE_ASSERT(false, "Invalid OpenGL framebuffer height")
					mHeight = 1;
			}
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint			mOpenGLFramebuffer;			// OpenGL framebuffer ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
		uint32_t		mNumberOfColorTextures;		// Number of color render target textures
		Rhi::ITexture** mColorTextures;				// The color render target textures (we keep a reference to it), can be a null pointer or can contain null pointers, if not a null pointer there must be at least "mNumberOfColorTextures" textures in the provided C-array of pointers
		Rhi::ITexture*  mDepthStencilTexture;		// The depth stencil render target texture (we keep a reference to it), can be a null pointer
		uint32_t		mWidth;						// The framebuffer width
		uint32_t		mHeight;					// The framebuffer height
		bool			mMultisampleRenderTarget;	// Multisample render target?


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator =(const Framebuffer&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/RenderTarget/FramebufferBind.h              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL framebuffer class, traditional bind version
	*/
	class FramebufferBind final : public Framebuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] colorFramebufferAttachments
		*    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "Rhi::IRenderPass::getNumberOfColorAttachments()" textures in the provided C-array of pointers
		*  @param[in] depthStencilFramebufferAttachment
		*    The depth stencil render target texture, can be a null pointer
		*
		*  @note
		*    - The framebuffer keeps a reference to the provided texture instances
		*/
		FramebufferBind(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Framebuffer(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Texture reference handling is done within the base class "Framebuffer"
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(renderPass.getRhi());

#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently bound OpenGL framebuffer
			GLint openGLFramebufferBackup = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &openGLFramebufferBackup);
#endif

			// Create the OpenGL framebuffer
			glGenFramebuffers(1, &mOpenGLFramebuffer);

			// Bind this OpenGL framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, mOpenGLFramebuffer);

			// Loop through all framebuffer color attachments
			const Rhi::FramebufferAttachment* colorFramebufferAttachment = colorFramebufferAttachments;
			const Rhi::FramebufferAttachment* colorFramebufferAttachmentEnd = colorFramebufferAttachments + mNumberOfColorTextures;
			for ( GLenum openGLAttachment = GL_COLOR_ATTACHMENT0; colorFramebufferAttachment < colorFramebufferAttachmentEnd; ++colorFramebufferAttachment, ++openGLAttachment )
			{
				Rhi::ITexture* texture = colorFramebufferAttachment->texture;

				// Security check: Is the given resource owned by this RHI?
#if SE_DEBUG
				if ( &openGLRhi != &texture->getRhi() )
				{
					// Output an error message and keep on going in order to keep a reasonable behaviour even in case on an error
					RHI_LOG(CRITICAL, "OpenGL error: The given color texture at index %u is owned by another RHI instance", colorFramebufferAttachment - colorFramebufferAttachments)

						// Continue, there's no point in trying to do any error handling in here
						continue;
				}
#endif

				// Evaluate the color texture type
				switch ( texture->getResourceType() )
				{
				case Rhi::ResourceType::TEXTURE_2D:
					{
						// Set the OpenGL framebuffer color attachment
						const Texture2D* texture2D = static_cast<const Texture2D*>(texture);
						glFramebufferTexture2D(GL_FRAMEBUFFER, openGLAttachment, static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->getOpenGLTexture(), static_cast<GLint>(colorFramebufferAttachment->mipmapIndex));
						if ( !mMultisampleRenderTarget && texture2D->getNumberOfMultisamples() > 1 )
						{
							mMultisampleRenderTarget = true;
						}
						break;
					}

				case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					{
						// Set the OpenGL framebuffer color attachment
						const Texture2DArray* texture2DArray = static_cast<const Texture2DArray*>(texture);
						glFramebufferTextureLayer(GL_FRAMEBUFFER, openGLAttachment, texture2DArray->getOpenGLTexture(), static_cast<GLint>(colorFramebufferAttachment->mipmapIndex), static_cast<GLint>(colorFramebufferAttachment->layerIndex));
						if ( !mMultisampleRenderTarget && texture2DArray->getNumberOfMultisamples() > 1 )
						{
							mMultisampleRenderTarget = true;
						}
						break;
					}

				case Rhi::ResourceType::ROOT_SIGNATURE:
				case Rhi::ResourceType::RESOURCE_GROUP:
				case Rhi::ResourceType::GRAPHICS_PROGRAM:
				case Rhi::ResourceType::VERTEX_ARRAY:
				case Rhi::ResourceType::RENDER_PASS:
				case Rhi::ResourceType::QUERY_POOL:
				case Rhi::ResourceType::SWAP_CHAIN:
				case Rhi::ResourceType::FRAMEBUFFER:
				case Rhi::ResourceType::VERTEX_BUFFER:
				case Rhi::ResourceType::INDEX_BUFFER:
				case Rhi::ResourceType::TEXTURE_BUFFER:
				case Rhi::ResourceType::STRUCTURED_BUFFER:
				case Rhi::ResourceType::INDIRECT_BUFFER:
				case Rhi::ResourceType::UNIFORM_BUFFER:
				case Rhi::ResourceType::TEXTURE_1D:
				case Rhi::ResourceType::TEXTURE_1D_ARRAY:
				case Rhi::ResourceType::TEXTURE_3D:
				case Rhi::ResourceType::TEXTURE_CUBE:
				case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
				case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
				case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
				case Rhi::ResourceType::SAMPLER_STATE:
				case Rhi::ResourceType::VERTEX_SHADER:
				case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
				case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
				case Rhi::ResourceType::GEOMETRY_SHADER:
				case Rhi::ResourceType::FRAGMENT_SHADER:
				case Rhi::ResourceType::TASK_SHADER:
				case Rhi::ResourceType::MESH_SHADER:
				case Rhi::ResourceType::COMPUTE_SHADER:
				default:
					RHI_LOG(CRITICAL, "The type of the given color texture at index %ld is not supported by the OpenGL RHI implementation", colorFramebufferAttachment - colorFramebufferAttachments)
						break;
				}
			}

			// Depth stencil texture
			if ( nullptr != mDepthStencilTexture )
			{
				// Security check: Is the given resource owned by this RHI?
#if SE_DEBUG
				if ( &openGLRhi != &mDepthStencilTexture->getRhi() )
				{
					// Output an error message and keep on going in order to keep a reasonable behaviour even in case on an error
					RHI_LOG(CRITICAL, "OpenGL error: The given depth stencil texture is owned by another RHI instance")
				}
#endif

				// Evaluate the depth stencil texture type
				switch ( mDepthStencilTexture->getResourceType() )
				{
				case Rhi::ResourceType::TEXTURE_2D:
					{
						const Texture2D* texture2D = static_cast<const Texture2D*>(mDepthStencilTexture);

						// Sanity check
						SE_ASSERT(depthStencilFramebufferAttachment->mipmapIndex < Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid OpenGL depth stencil framebuffer attachment mipmap index")
							SE_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex, "Invalid OpenGL depth stencil framebuffer attachment layer index")

							// Bind the depth stencil texture to framebuffer
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->getOpenGLTexture(), static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex));
						if ( !mMultisampleRenderTarget && texture2D->getNumberOfMultisamples() > 1 )
						{
							mMultisampleRenderTarget = true;
						}
						break;
					}

				case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					{
						// Bind the depth stencil texture to framebuffer
						const Texture2DArray* texture2DArray = static_cast<const Texture2DArray*>(mDepthStencilTexture);
						glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture2DArray->getOpenGLTexture(), static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex), static_cast<GLint>(depthStencilFramebufferAttachment->layerIndex));
						if ( !mMultisampleRenderTarget && texture2DArray->getNumberOfMultisamples() > 1 )
						{
							mMultisampleRenderTarget = true;
						}
						break;
					}

				case Rhi::ResourceType::ROOT_SIGNATURE:
				case Rhi::ResourceType::RESOURCE_GROUP:
				case Rhi::ResourceType::GRAPHICS_PROGRAM:
				case Rhi::ResourceType::VERTEX_ARRAY:
				case Rhi::ResourceType::RENDER_PASS:
				case Rhi::ResourceType::QUERY_POOL:
				case Rhi::ResourceType::SWAP_CHAIN:
				case Rhi::ResourceType::FRAMEBUFFER:
				case Rhi::ResourceType::VERTEX_BUFFER:
				case Rhi::ResourceType::INDEX_BUFFER:
				case Rhi::ResourceType::TEXTURE_BUFFER:
				case Rhi::ResourceType::STRUCTURED_BUFFER:
				case Rhi::ResourceType::INDIRECT_BUFFER:
				case Rhi::ResourceType::UNIFORM_BUFFER:
				case Rhi::ResourceType::TEXTURE_1D:
				case Rhi::ResourceType::TEXTURE_1D_ARRAY:
				case Rhi::ResourceType::TEXTURE_3D:
				case Rhi::ResourceType::TEXTURE_CUBE:
				case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
				case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
				case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
				case Rhi::ResourceType::SAMPLER_STATE:
				case Rhi::ResourceType::VERTEX_SHADER:
				case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
				case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
				case Rhi::ResourceType::GEOMETRY_SHADER:
				case Rhi::ResourceType::FRAGMENT_SHADER:
				case Rhi::ResourceType::TASK_SHADER:
				case Rhi::ResourceType::MESH_SHADER:
				case Rhi::ResourceType::COMPUTE_SHADER:
				default:
					RHI_LOG(CRITICAL, "OpenGL error: The type of the given depth stencil texture is not supported by the OpenGL RHI implementation")
						break;
				}
			}

			// Check the status of the OpenGL framebuffer
			const GLenum openGLStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			switch ( openGLStatus )
			{
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				RHI_LOG(CRITICAL, "OpenGL error: Not all framebuffer attachment points are framebuffer attachment complete (\"GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\")")
					break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				RHI_LOG(CRITICAL, "OpenGL error: No images are attached to the framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\")")
					break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				RHI_LOG(CRITICAL, "OpenGL error: Incomplete draw buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\")")
					break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				RHI_LOG(CRITICAL, "OpenGL error: Incomplete read buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\")")
					break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				RHI_LOG(CRITICAL, "OpenGL error: Incomplete multisample framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\")")
					break;

			case GL_FRAMEBUFFER_UNDEFINED:
				RHI_LOG(CRITICAL, "OpenGL error: Undefined framebuffer (\"GL_FRAMEBUFFER_UNDEFINED\")")
					break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				RHI_LOG(CRITICAL, "OpenGL error: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions (\"GL_FRAMEBUFFER_UNSUPPORTED\")")
					break;

				// From "GL_EXT_framebuffer_object" (should no longer matter, should)
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				RHI_LOG(CRITICAL, "OpenGL error: Not all attached images have the same width and height (\"GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\")")
					break;

				// From "GL_EXT_framebuffer_object" (should no longer matter, should)
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				RHI_LOG(CRITICAL, "OpenGL error: Incomplete formats framebuffer object (\"GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\")")
					break;

			default:
			case GL_FRAMEBUFFER_COMPLETE:
				// Nothing here
				break;
			}

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous bound OpenGL framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(openGLFramebufferBackup));
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
					glObjectLabel(GL_FRAMEBUFFER, mOpenGLFramebuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~FramebufferBind() override
		{
			// Nothing here, texture reference handling is done within the base class "Framebuffer"
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit FramebufferBind(const FramebufferBind&) = delete;
		FramebufferBind& operator =(const FramebufferBind&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/RenderTarget/FramebufferDsa.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenGL framebuffer class, effective direct state access (DSA)
	*/
	class FramebufferDsa final : public Framebuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] colorFramebufferAttachments
		*    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "Rhi::IRenderPass::getNumberOfColorAttachments()" textures in the provided C-array of pointers
		*  @param[in] depthStencilFramebufferAttachment
		*    The depth stencil render target texture, can be a null pointer
		*
		*  @note
		*    - The framebuffer keeps a reference to the provided texture instances
		*/
		FramebufferDsa(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Framebuffer(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			// Texture reference handling is done within the base class "Framebuffer"
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(renderPass.getRhi());
			const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();

			// Create the OpenGL framebuffer
			if ( isArbDsa )
			{
				glCreateFramebuffers(1, &mOpenGLFramebuffer);
			}
			else
			{
				glGenFramebuffers(1, &mOpenGLFramebuffer);
			}

			// Loop through all framebuffer color attachments
			const Rhi::FramebufferAttachment* colorFramebufferAttachment = colorFramebufferAttachments;
			const Rhi::FramebufferAttachment* colorFramebufferAttachmentEnd = colorFramebufferAttachments + mNumberOfColorTextures;
			for ( GLenum openGLAttachment = GL_COLOR_ATTACHMENT0; colorFramebufferAttachment < colorFramebufferAttachmentEnd; ++colorFramebufferAttachment, ++openGLAttachment )
			{
				Rhi::ITexture* texture = colorFramebufferAttachment->texture;

				// Security check: Is the given resource owned by this RHI?
#if SE_DEBUG
				if ( &openGLRhi != &texture->getRhi() )
				{
					// Output an error message and keep on going in order to keep a reasonable behaviour even in case on an error
					RHI_LOG(CRITICAL, "OpenGL error: The given color texture at index %u is owned by another RHI instance", colorFramebufferAttachment - colorFramebufferAttachments)

						// Continue, there's no point in trying to do any error handling in here
						continue;
				}
#endif

				// Evaluate the color texture type
				switch ( texture->getResourceType() )
				{
				case Rhi::ResourceType::TEXTURE_2D:
					{
						// Set the OpenGL framebuffer color attachment
						const Texture2D* texture2D = static_cast<const Texture2D*>(texture);
						if ( isArbDsa )
						{
							glNamedFramebufferTexture(mOpenGLFramebuffer, openGLAttachment, texture2D->getOpenGLTexture(), static_cast<GLint>(colorFramebufferAttachment->mipmapIndex));
						}
						else
						{
							glNamedFramebufferTexture2DEXT(mOpenGLFramebuffer, openGLAttachment, static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->getOpenGLTexture(), static_cast<GLint>(colorFramebufferAttachment->mipmapIndex));
						}
						if ( !mMultisampleRenderTarget && texture2D->getNumberOfMultisamples() > 1 )
						{
							mMultisampleRenderTarget = true;
						}
						break;
					}

				case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					{
						// Set the OpenGL framebuffer color attachment
						const Texture2DArray* texture2DArray = static_cast<const Texture2DArray*>(texture);
						if ( isArbDsa )
						{
							glNamedFramebufferTextureLayer(mOpenGLFramebuffer, openGLAttachment, texture2DArray->getOpenGLTexture(), static_cast<GLint>(colorFramebufferAttachment->mipmapIndex), static_cast<GLint>(colorFramebufferAttachment->layerIndex));
						}
						else
						{
							glNamedFramebufferTextureLayerEXT(mOpenGLFramebuffer, openGLAttachment, texture2DArray->getOpenGLTexture(), static_cast<GLint>(colorFramebufferAttachment->mipmapIndex), static_cast<GLint>(colorFramebufferAttachment->layerIndex));
						}
						if ( !mMultisampleRenderTarget && texture2DArray->getNumberOfMultisamples() > 1 )
						{
							mMultisampleRenderTarget = true;
						}
						break;
					}

				case Rhi::ResourceType::ROOT_SIGNATURE:
				case Rhi::ResourceType::RESOURCE_GROUP:
				case Rhi::ResourceType::GRAPHICS_PROGRAM:
				case Rhi::ResourceType::VERTEX_ARRAY:
				case Rhi::ResourceType::RENDER_PASS:
				case Rhi::ResourceType::QUERY_POOL:
				case Rhi::ResourceType::SWAP_CHAIN:
				case Rhi::ResourceType::FRAMEBUFFER:
				case Rhi::ResourceType::VERTEX_BUFFER:
				case Rhi::ResourceType::INDEX_BUFFER:
				case Rhi::ResourceType::TEXTURE_BUFFER:
				case Rhi::ResourceType::STRUCTURED_BUFFER:
				case Rhi::ResourceType::INDIRECT_BUFFER:
				case Rhi::ResourceType::UNIFORM_BUFFER:
				case Rhi::ResourceType::TEXTURE_1D:
				case Rhi::ResourceType::TEXTURE_1D_ARRAY:
				case Rhi::ResourceType::TEXTURE_3D:
				case Rhi::ResourceType::TEXTURE_CUBE:
				case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
				case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
				case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
				case Rhi::ResourceType::SAMPLER_STATE:
				case Rhi::ResourceType::VERTEX_SHADER:
				case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
				case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
				case Rhi::ResourceType::GEOMETRY_SHADER:
				case Rhi::ResourceType::FRAGMENT_SHADER:
				case Rhi::ResourceType::TASK_SHADER:
				case Rhi::ResourceType::MESH_SHADER:
				case Rhi::ResourceType::COMPUTE_SHADER:
				default:
					RHI_LOG(CRITICAL, "The type of the given color texture at index %ld is not supported by the OpenGL RHI implementation", colorFramebufferAttachment - colorFramebufferAttachments)
						break;
				}
			}

			// Depth stencil texture
			if ( nullptr != mDepthStencilTexture )
			{
				// Security check: Is the given resource owned by this RHI?
#if SE_DEBUG
				if ( &openGLRhi != &mDepthStencilTexture->getRhi() )
				{
					// Output an error message and keep on going in order to keep a reasonable behaviour even in case on an error
					RHI_LOG(CRITICAL, "OpenGL error: The given depth stencil texture is owned by another RHI instance")
				}
#endif

				// Evaluate the depth stencil texture type
				switch ( mDepthStencilTexture->getResourceType() )
				{
				case Rhi::ResourceType::TEXTURE_2D:
					{
						const Texture2D* texture2D = static_cast<const Texture2D*>(mDepthStencilTexture);

						// Sanity checks
						SE_ASSERT(depthStencilFramebufferAttachment->mipmapIndex < Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid OpenGL depth stencil framebuffer attachment mipmap index")
							SE_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex, "Invalid OpenGL depth stencil framebuffer attachment layer index")

							// Bind the depth stencil texture to framebuffer
							if ( isArbDsa )
							{
								glNamedFramebufferTexture(mOpenGLFramebuffer, GL_DEPTH_ATTACHMENT, texture2D->getOpenGLTexture(), static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex));
							}
							else
							{
								glNamedFramebufferTexture2DEXT(mOpenGLFramebuffer, GL_DEPTH_ATTACHMENT, static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->getOpenGLTexture(), static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex));
							}
						if ( !mMultisampleRenderTarget && texture2D->getNumberOfMultisamples() > 1 )
						{
							mMultisampleRenderTarget = true;
						}
						break;
					}

				case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					{
						// Bind the depth stencil texture to framebuffer
						const Texture2DArray* texture2DArray = static_cast<const Texture2DArray*>(mDepthStencilTexture);
						if ( isArbDsa )
						{
							glNamedFramebufferTextureLayer(mOpenGLFramebuffer, GL_DEPTH_ATTACHMENT, texture2DArray->getOpenGLTexture(), static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex), static_cast<GLint>(depthStencilFramebufferAttachment->layerIndex));
						}
						else
						{
							glNamedFramebufferTextureLayerEXT(mOpenGLFramebuffer, GL_DEPTH_ATTACHMENT, texture2DArray->getOpenGLTexture(), static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex), static_cast<GLint>(depthStencilFramebufferAttachment->layerIndex));
						}
						if ( !mMultisampleRenderTarget && texture2DArray->getNumberOfMultisamples() > 1 )
						{
							mMultisampleRenderTarget = true;
						}
						break;
					}

				case Rhi::ResourceType::ROOT_SIGNATURE:
				case Rhi::ResourceType::RESOURCE_GROUP:
				case Rhi::ResourceType::GRAPHICS_PROGRAM:
				case Rhi::ResourceType::VERTEX_ARRAY:
				case Rhi::ResourceType::RENDER_PASS:
				case Rhi::ResourceType::QUERY_POOL:
				case Rhi::ResourceType::SWAP_CHAIN:
				case Rhi::ResourceType::FRAMEBUFFER:
				case Rhi::ResourceType::VERTEX_BUFFER:
				case Rhi::ResourceType::INDEX_BUFFER:
				case Rhi::ResourceType::TEXTURE_BUFFER:
				case Rhi::ResourceType::STRUCTURED_BUFFER:
				case Rhi::ResourceType::INDIRECT_BUFFER:
				case Rhi::ResourceType::UNIFORM_BUFFER:
				case Rhi::ResourceType::TEXTURE_1D:
				case Rhi::ResourceType::TEXTURE_1D_ARRAY:
				case Rhi::ResourceType::TEXTURE_3D:
				case Rhi::ResourceType::TEXTURE_CUBE:
				case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
				case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
				case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
				case Rhi::ResourceType::SAMPLER_STATE:
				case Rhi::ResourceType::VERTEX_SHADER:
				case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
				case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
				case Rhi::ResourceType::GEOMETRY_SHADER:
				case Rhi::ResourceType::FRAGMENT_SHADER:
				case Rhi::ResourceType::TASK_SHADER:
				case Rhi::ResourceType::MESH_SHADER:
				case Rhi::ResourceType::COMPUTE_SHADER:
				default:
					RHI_LOG(CRITICAL, "The type of the given depth stencil texture is not supported by the OpenGL RHI implementation")
						break;
				}
			}

			// Check the status of the OpenGL framebuffer
			const GLenum openGLStatus = isArbDsa ? glCheckNamedFramebufferStatus(mOpenGLFramebuffer, GL_FRAMEBUFFER) : glCheckNamedFramebufferStatusEXT(mOpenGLFramebuffer, GL_FRAMEBUFFER);
			switch ( openGLStatus )
			{
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				RHI_LOG(CRITICAL, "OpenGL error: Not all framebuffer attachment points are framebuffer attachment complete (\"GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\")")
					break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				RHI_LOG(CRITICAL, "OpenGL error: No images are attached to the framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\")")
					break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				RHI_LOG(CRITICAL, "OpenGL error: Incomplete draw buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\")")
					break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				RHI_LOG(CRITICAL, "OpenGL error: Incomplete read buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\")")
					break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				RHI_LOG(CRITICAL, "OpenGL error: Incomplete multisample framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\")")
					break;

			case GL_FRAMEBUFFER_UNDEFINED:
				RHI_LOG(CRITICAL, "OpenGL error: Undefined framebuffer (\"GL_FRAMEBUFFER_UNDEFINED\")")
					break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				RHI_LOG(CRITICAL, "OpenGL error: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions (\"GL_FRAMEBUFFER_UNSUPPORTED\")")
					break;

				// From "GL_EXT_framebuffer_object" (should no longer matter, should)
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				RHI_LOG(CRITICAL, "OpenGL error: Not all attached images have the same width and height (\"GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\")")
					break;

				// From "GL_EXT_framebuffer_object" (should no longer matter, should)
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				RHI_LOG(CRITICAL, "OpenGL error: Incomplete formats framebuffer object (\"GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\")")
					break;

			default:
			case GL_FRAMEBUFFER_COMPLETE:
				// Nothing here
				break;
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
					glObjectLabel(GL_FRAMEBUFFER, mOpenGLFramebuffer, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~FramebufferDsa() override
		{
			// Nothing here, texture reference handling is done within the base class "Framebuffer"
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit FramebufferDsa(const FramebufferDsa&) = delete;
		FramebufferDsa& operator =(const FramebufferDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/VertexShaderMonolithic.h  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic vertex shader class
	*/
	class VertexShaderMonolithic final : public Rhi::IVertexShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a vertex shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline VertexShaderMonolithic(OpenGLRhi& openGLRhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IVertexShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShader(::detail::loadShaderFromSourcecode(GL_VERTEX_SHADER_ARB, sourceCode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShader && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VS", 5)	// 5 = "VS: " including terminating zero
					glObjectLabel(GL_SHADER, mOpenGLShader, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexShaderMonolithic() override
		{
			// Destroy the OpenGL shader
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteShader(mOpenGLShader);
		}

		/**
		*  @brief
		*    Return the OpenGL shader
		*
		*  @return
		*    The OpenGL shader, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShader() const
		{
			return mOpenGLShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(VertexShaderMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexShaderMonolithic(const VertexShaderMonolithic&) = delete;
		VertexShaderMonolithic& operator =(const VertexShaderMonolithic&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShader;	// OpenGL shader, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/TessellationControlShaderMonolithic.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic tessellation control shader ("hull shader" in Direct3D terminology) class
	*/
	class TessellationControlShaderMonolithic final : public Rhi::ITessellationControlShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a tessellation control shader ("hull shader" in Direct3D terminology) shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline TessellationControlShaderMonolithic(OpenGLRhi& openGLRhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationControlShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShader(::detail::loadShaderFromSourcecode(GL_TESS_CONTROL_SHADER, sourceCode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShader && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TCS", 6)	// 6 = "TCS: " including terminating zero
					glObjectLabel(GL_SHADER, mOpenGLShader, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TessellationControlShaderMonolithic() override
		{
			// Destroy the OpenGL shader
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteShader(mOpenGLShader);
		}

		/**
		*  @brief
		*    Return the OpenGL shader
		*
		*  @return
		*    The OpenGL shader, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShader() const
		{
			return mOpenGLShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TessellationControlShaderMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TessellationControlShaderMonolithic(const TessellationControlShaderMonolithic&) = delete;
		TessellationControlShaderMonolithic& operator =(const TessellationControlShaderMonolithic&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShader;	// OpenGL shader, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/TessellationEvaluationShaderMonolithic.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic tessellation evaluation shader ("domain shader" in Direct3D terminology) class
	*/
	class TessellationEvaluationShaderMonolithic final : public Rhi::ITessellationEvaluationShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a tessellation evaluation shader ("domain shader" in Direct3D terminology) shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline TessellationEvaluationShaderMonolithic(OpenGLRhi& openGLRhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationEvaluationShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShader(::detail::loadShaderFromSourcecode(GL_TESS_EVALUATION_SHADER, sourceCode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShader && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TES", 6)	// 6 = "TES: " including terminating zero
					glObjectLabel(GL_SHADER, mOpenGLShader, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TessellationEvaluationShaderMonolithic() override
		{
			// Destroy the OpenGL shader
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteShader(mOpenGLShader);
		}

		/**
		*  @brief
		*    Return the OpenGL shader
		*
		*  @return
		*    The OpenGL shader, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShader() const
		{
			return mOpenGLShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TessellationEvaluationShaderMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TessellationEvaluationShaderMonolithic(const TessellationEvaluationShaderMonolithic&) = delete;
		TessellationEvaluationShaderMonolithic& operator =(const TessellationEvaluationShaderMonolithic&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShader;	// OpenGL shader, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/GeometryShaderMonolithic.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic geometry shader class
	*/
	class GeometryShaderMonolithic final : public Rhi::IGeometryShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a geometry shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*  @param[in] gsInputPrimitiveTopology
		*    Geometry shader input primitive topology
		*  @param[in] gsOutputPrimitiveTopology
		*    Geometry shader output primitive topology
		*  @param[in] numberOfOutputVertices
		*    Number of output vertices
		*/
		inline GeometryShaderMonolithic(OpenGLRhi& openGLRhi, const char* sourceCode, Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGeometryShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShader(::detail::loadShaderFromSourcecode(GL_GEOMETRY_SHADER_ARB, sourceCode)),
			mOpenGLGsInputPrimitiveTopology(static_cast<int>(gsInputPrimitiveTopology)),	// The "Rhi::GsInputPrimitiveTopology" values directly map to OpenGL constants, do not change them
			mOpenGLGsOutputPrimitiveTopology(static_cast<int>(gsOutputPrimitiveTopology)),	// The "Rhi::GsOutputPrimitiveTopology" values directly map to OpenGL constants, do not change them
			mNumberOfOutputVertices(numberOfOutputVertices)
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShader && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
					glObjectLabel(GL_SHADER, mOpenGLShader, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~GeometryShaderMonolithic() override
		{
			// Destroy the OpenGL shader
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteShader(mOpenGLShader);
		}

		/**
		*  @brief
		*    Return the OpenGL shader
		*
		*  @return
		*    The OpenGL shader, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShader() const
		{
			return mOpenGLShader;
		}

		/**
		*  @brief
		*    Return the OpenGL geometry shader input primitive topology
		*
		*  @return
		*    The OpenGL geometry shader input primitive topology
		*/
		[[nodiscard]] inline GLint getOpenGLGsInputPrimitiveTopology() const
		{
			return mOpenGLGsInputPrimitiveTopology;
		}

		/**
		*  @brief
		*    Return the OpenGL geometry shader output primitive topology
		*
		*  @return
		*    The OpenGL geometry shader output primitive topology
		*/
		[[nodiscard]] inline GLint getOpenGLGsOutputPrimitiveTopology() const
		{
			return mOpenGLGsOutputPrimitiveTopology;
		}

		/**
		*  @brief
		*    Return the number of output vertices
		*
		*  @return
		*    The number of output vertices
		*/
		[[nodiscard]] inline uint32_t getNumberOfOutputVertices() const
		{
			return mNumberOfOutputVertices;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(GeometryShaderMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GeometryShaderMonolithic(const GeometryShaderMonolithic&) = delete;
		GeometryShaderMonolithic& operator =(const GeometryShaderMonolithic&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint	 mOpenGLShader;						// OpenGL shader, can be zero if no resource is allocated
		GLint	 mOpenGLGsInputPrimitiveTopology;	// OpenGL geometry shader input primitive topology
		GLint	 mOpenGLGsOutputPrimitiveTopology;	// OpenGL geometry shader output primitive topology
		uint32_t mNumberOfOutputVertices;			// Number of output vertices


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/FragmentShaderMonolithic.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic fragment shader (FS, "pixel shader" in Direct3D terminology) class
	*/
	class FragmentShaderMonolithic final : public Rhi::IFragmentShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a fragment shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline FragmentShaderMonolithic(OpenGLRhi& openGLRhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IFragmentShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShader(::detail::loadShaderFromSourcecode(GL_FRAGMENT_SHADER_ARB, sourceCode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShader && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FS", 5)	// 5 = "FS: " including terminating zero
					glObjectLabel(GL_SHADER, mOpenGLShader, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~FragmentShaderMonolithic() override
		{
			// Destroy the OpenGL shader
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteShader(mOpenGLShader);
		}

		/**
		*  @brief
		*    Return the OpenGL shader
		*
		*  @return
		*    The OpenGL shader, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShader() const
		{
			return mOpenGLShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(FragmentShaderMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit FragmentShaderMonolithic(const FragmentShaderMonolithic&) = delete;
		FragmentShaderMonolithic& operator =(const FragmentShaderMonolithic&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShader;	// OpenGL shader, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/TaskShaderMonolithic.h    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic task shader (TS, "amplification shader" in Direct3D terminology) class
	*/
	class TaskShaderMonolithic final : public Rhi::ITaskShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a task shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline TaskShaderMonolithic(OpenGLRhi& openGLRhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITaskShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShader(::detail::loadShaderFromSourcecode(GL_TASK_SHADER_NV, sourceCode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShader && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TS", 5)	// 5 = "TS: " including terminating zero
					glObjectLabel(GL_SHADER, mOpenGLShader, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TaskShaderMonolithic() override
		{
			// Destroy the OpenGL shader
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteShader(mOpenGLShader);
		}

		/**
		*  @brief
		*    Return the OpenGL shader
		*
		*  @return
		*    The OpenGL shader, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShader() const
		{
			return mOpenGLShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TaskShaderMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TaskShaderMonolithic(const TaskShaderMonolithic&) = delete;
		TaskShaderMonolithic& operator =(const TaskShaderMonolithic&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShader;	// OpenGL shader, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/MeshShaderMonolithic.h    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic mesh shader (MS) class
	*/
	class MeshShaderMonolithic final : public Rhi::IMeshShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a mesh shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline MeshShaderMonolithic(OpenGLRhi& openGLRhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IMeshShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShader(::detail::loadShaderFromSourcecode(GL_MESH_SHADER_NV, sourceCode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShader && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "MS", 5)	// 5 = "MS: " including terminating zero
					glObjectLabel(GL_SHADER, mOpenGLShader, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MeshShaderMonolithic() override
		{
			// Destroy the OpenGL shader
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteShader(mOpenGLShader);
		}

		/**
		*  @brief
		*    Return the OpenGL shader
		*
		*  @return
		*    The OpenGL shader, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShader() const
		{
			return mOpenGLShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(MeshShaderMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit MeshShaderMonolithic(const MeshShaderMonolithic&) = delete;
		MeshShaderMonolithic& operator =(const MeshShaderMonolithic&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShader;	// OpenGL shader, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/ComputeShaderMonolithic.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic compute shader (CS) class
	*/
	class ComputeShaderMonolithic final : public Rhi::IComputeShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a compute shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline ComputeShaderMonolithic(OpenGLRhi& openGLRhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IComputeShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShader(::detail::loadShaderFromSourcecode(GL_COMPUTE_SHADER, sourceCode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShader && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "CS", 5)	// 5 = "CS: " including terminating zero
					glObjectLabel(GL_SHADER, mOpenGLShader, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ComputeShaderMonolithic() override
		{
			// Destroy the OpenGL shader
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteShader(mOpenGLShader);
		}

		/**
		*  @brief
		*    Return the OpenGL shader
		*
		*  @return
		*    The OpenGL shader, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShader() const
		{
			return mOpenGLShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ComputeShaderMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ComputeShaderMonolithic(const ComputeShaderMonolithic&) = delete;
		ComputeShaderMonolithic& operator =(const ComputeShaderMonolithic&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShader;	// OpenGL shader, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/GraphicsProgramMonolithic.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic graphics program class
	*/
	class GraphicsProgramMonolithic : public Rhi::IGraphicsProgram
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for traditional graphics program
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] vertexShaderMonolithic
		*    Vertex shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationControlShaderMonolithic
		*    Tessellation control shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationEvaluationShaderMonolithic
		*    Tessellation evaluation shader the graphics program is using, can be a null pointer
		*  @param[in] geometryShaderMonolithic
		*    Geometry shader the graphics program is using, can be a null pointer
		*  @param[in] fragmentShaderMonolithic
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		GraphicsProgramMonolithic(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, const Rhi::VertexAttributes& vertexAttributes, VertexShaderMonolithic* vertexShaderMonolithic, TessellationControlShaderMonolithic* tessellationControlShaderMonolithic, TessellationEvaluationShaderMonolithic* tessellationEvaluationShaderMonolithic, GeometryShaderMonolithic* geometryShaderMonolithic, FragmentShaderMonolithic* fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLProgram(glCreateProgram()),
			mDrawIdUniformLocation(-1)
		{
			// Attach the shaders to the program
			// -> We don't need to keep a reference to the shader, to add and release at once to ensure a nice behaviour
			if ( nullptr != vertexShaderMonolithic )
			{
				vertexShaderMonolithic->AddReference();
				glAttachShader(mOpenGLProgram, vertexShaderMonolithic->getOpenGLShader());
				vertexShaderMonolithic->ReleaseReference();
			}
			if ( nullptr != tessellationControlShaderMonolithic )
			{
				tessellationControlShaderMonolithic->AddReference();
				glAttachShader(mOpenGLProgram, tessellationControlShaderMonolithic->getOpenGLShader());
				tessellationControlShaderMonolithic->ReleaseReference();
			}
			if ( nullptr != tessellationEvaluationShaderMonolithic )
			{
				tessellationEvaluationShaderMonolithic->AddReference();
				glAttachShader(mOpenGLProgram, tessellationEvaluationShaderMonolithic->getOpenGLShader());
				tessellationEvaluationShaderMonolithic->ReleaseReference();
			}
			if ( nullptr != geometryShaderMonolithic )
			{
				// Add a reference to the shader
				geometryShaderMonolithic->AddReference();

				// Attach the monolithic shader to the monolithic program
				glAttachShader(mOpenGLProgram, geometryShaderMonolithic->getOpenGLShader());

				// In modern GLSL, "geometry shader input primitive topology" & "geometry shader output primitive topology" & "number of output vertices" can be directly set within GLSL by writing e.g.
				//   "layout(triangles) in;"
				//   "layout(triangle_strip, max_vertices = 3) out;"
				// -> To be able to support older GLSL versions, we have to provide this information also via OpenGL API functions

				// Set the OpenGL geometry shader input primitive topology
				glProgramParameteriARB(mOpenGLProgram, GL_GEOMETRY_INPUT_TYPE_ARB, geometryShaderMonolithic->getOpenGLGsInputPrimitiveTopology());

				// Set the OpenGL geometry shader output primitive topology
				glProgramParameteriARB(mOpenGLProgram, GL_GEOMETRY_OUTPUT_TYPE_ARB, geometryShaderMonolithic->getOpenGLGsOutputPrimitiveTopology());

				// Set the number of output vertices
				glProgramParameteriARB(mOpenGLProgram, GL_GEOMETRY_VERTICES_OUT_ARB, static_cast<GLint>(geometryShaderMonolithic->getNumberOfOutputVertices()));

				// Release the shader
				geometryShaderMonolithic->ReleaseReference();
			}
			if ( nullptr != fragmentShaderMonolithic )
			{
				fragmentShaderMonolithic->AddReference();
				glAttachShader(mOpenGLProgram, fragmentShaderMonolithic->getOpenGLShader());
				fragmentShaderMonolithic->ReleaseReference();
			}

			{ // Define the vertex array attribute binding locations ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 & 12 terminology)
				const uint32_t numberOfVertexAttributes = vertexAttributes.numberOfAttributes;
				for ( uint32_t vertexAttribute = 0; vertexAttribute < numberOfVertexAttributes; ++vertexAttribute )
				{
					glBindAttribLocation(mOpenGLProgram, vertexAttribute, vertexAttributes.attributes[vertexAttribute].name);
				}
			}

			// Link the program
			linkProgram(openGLRhi, rootSignature);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for task and mesh shader based graphics program
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] taskShaderMonolithic
		*    Task shader the graphics program is using, can be a null pointer
		*  @param[in] meshShaderMonolithic
		*    Mesh shader the graphics program is using
		*  @param[in] fragmentShaderMonolithic
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		GraphicsProgramMonolithic(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, TaskShaderMonolithic* taskShaderMonolithic, MeshShaderMonolithic& meshShaderMonolithic, FragmentShaderMonolithic* fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLProgram(glCreateProgram()),
			mDrawIdUniformLocation(-1)
		{
			// Attach the shaders to the program
			// -> We don't need to keep a reference to the shader, to add and release at once to ensure a nice behaviour
			if ( nullptr != taskShaderMonolithic )
			{
				taskShaderMonolithic->AddReference();
				glAttachShader(mOpenGLProgram, taskShaderMonolithic->getOpenGLShader());
				taskShaderMonolithic->ReleaseReference();
			}
			meshShaderMonolithic.AddReference();
			glAttachShader(mOpenGLProgram, meshShaderMonolithic.getOpenGLShader());
			meshShaderMonolithic.ReleaseReference();
			if ( nullptr != fragmentShaderMonolithic )
			{
				fragmentShaderMonolithic->AddReference();
				glAttachShader(mOpenGLProgram, fragmentShaderMonolithic->getOpenGLShader());
				fragmentShaderMonolithic->ReleaseReference();
			}

			// Link the program
			linkProgram(openGLRhi, rootSignature);

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~GraphicsProgramMonolithic() override
		{
			// Destroy the OpenGL program
			// -> A value of 0 for program will be silently ignored
			glDeleteShader(mOpenGLProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL program
		*
		*  @return
		*    The OpenGL program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLProgram() const
		{
			return mOpenGLProgram;
		}

		/**
		*  @brief
		*    Return the draw ID uniform location
		*
		*  @return
		*    Draw ID uniform location, -1 if there's no such uniform
		*/
		[[nodiscard]] inline GLint getDrawIdUniformLocation() const
		{
			return mDrawIdUniformLocation;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IGraphicsProgram methods          ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual handle getUniformHandle(const char* uniformName) override
		{
			return static_cast<handle>(glGetUniformLocation(mOpenGLProgram, uniformName));
		}

		virtual void setUniform1i(handle uniformHandle, int value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program
			const GLhandleARB openGLProgramBackup = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
			if ( openGLProgramBackup == mOpenGLProgram )
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUniform1i(static_cast<GLint>(uniformHandle), value);
			}
			else
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUseProgram(mOpenGLProgram);
				glUniform1i(static_cast<GLint>(uniformHandle), value);

				// Be polite and restore the previous used OpenGL program
				glUseProgram(openGLProgramBackup);
			}
#else
			// Set uniform, please note that for this our program must be the currently used one
			glUseProgram(mOpenGLProgram);
			glUniform1i(static_cast<GLint>(uniformHandle), value);
#endif
		}

		virtual void setUniform1f(handle uniformHandle, float value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program
			const GLhandleARB openGLProgramBackup = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
			if ( openGLProgramBackup == mOpenGLProgram )
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUniform1f(static_cast<GLint>(uniformHandle), value);
			}
			else
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUseProgram(mOpenGLProgram);
				glUniform1f(static_cast<GLint>(uniformHandle), value);

				// Be polite and restore the previous used OpenGL program
				glUseProgram(openGLProgramBackup);
			}
#else
			// Set uniform, please note that for this our program must be the currently used one
			glUseProgram(mOpenGLProgram);
			glUniform1f(static_cast<GLint>(uniformHandle), value);
#endif
		}

		virtual void setUniform2fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program
			const GLhandleARB openGLProgramBackup = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
			if ( openGLProgramBackup == mOpenGLProgram )
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUniform2fv(static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUseProgram(mOpenGLProgram);
				glUniform2fv(static_cast<GLint>(uniformHandle), 1, value);

				// Be polite and restore the previous used OpenGL program
				glUseProgram(openGLProgramBackup);
			}
#else
			// Set uniform, please note that for this our program must be the currently used one
			glUseProgram(mOpenGLProgram);
			glUniform2fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
		}

		virtual void setUniform3fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program
			const GLhandleARB openGLProgramBackup = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
			if ( openGLProgramBackup == mOpenGLProgram )
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUniform3fv(static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUseProgram(mOpenGLProgram);
				glUniform3fv(static_cast<GLint>(uniformHandle), 1, value);

				// Be polite and restore the previous used OpenGL program
				glUseProgram(openGLProgramBackup);
			}
#else
			// Set uniform, please note that for this our program must be the currently used one
			glUseProgram(mOpenGLProgram);
			glUniform3fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
		}

		virtual void setUniform4fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program
			const GLhandleARB openGLProgramBackup = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
			if ( openGLProgramBackup == mOpenGLProgram )
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUniform4fv(static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUseProgram(mOpenGLProgram);
				glUniform4fv(static_cast<GLint>(uniformHandle), 1, value);

				// Be polite and restore the previous used OpenGL program
				glUseProgram(openGLProgramBackup);
			}
#else
			// Set uniform, please note that for this our program must be the currently used one
			glUseProgram(mOpenGLProgram);
			glUniform4fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
		}

		virtual void setUniformMatrix3fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program
			const GLhandleARB openGLProgramBackup = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
			if ( openGLProgramBackup == mOpenGLProgram )
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUniformMatrix3fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
			else
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUseProgram(mOpenGLProgram);
				glUniformMatrix3fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);

				// Be polite and restore the previous used OpenGL program
				glUseProgram(openGLProgramBackup);
			}
#else
			// Set uniform, please note that for this our program must be the currently used one
			glUseProgram(mOpenGLProgram);
			glUniformMatrix3fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
#endif
		}

		virtual void setUniformMatrix4fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program
			const GLhandleARB openGLProgramBackup = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
			if ( openGLProgramBackup == mOpenGLProgram )
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUniformMatrix4fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
			else
			{
				// Set uniform, please note that for this our program must be the currently used one
				glUseProgram(mOpenGLProgram);
				glUniformMatrix4fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);

				// Be polite and restore the previous used OpenGL program
				glUseProgram(openGLProgramBackup);
			}
#else
			// Set uniform, please note that for this our program must be the currently used one
			glUseProgram(mOpenGLProgram);
			glUniformMatrix4fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
#endif
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(GraphicsProgramMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLProgram;	// OpenGL program, can be zero if no resource is allocated


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit GraphicsProgramMonolithic(const GraphicsProgramMonolithic&) = delete;
		GraphicsProgramMonolithic& operator =(const GraphicsProgramMonolithic&) = delete;

		void linkProgram(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature)
		{
			// Link the program
			glLinkProgram(mOpenGLProgram);

			// Check the link status
			GLint linked = GL_FALSE;
			glGetProgramiv(mOpenGLProgram, GL_LINK_STATUS, &linked);
			if ( GL_TRUE == linked )
			{
				// We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
				// -> Use modern GLSL:
				//    "layout(location = 0) out vec4 ColorOutput0;"
				//    "layout(location = 1) out vec4 ColorOutput1;"
				// -> Use legacy GLSL if necessary:
				//    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
				//    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

				// Get draw ID uniform location
				if ( !openGLRhi.getExtensions().isGL_ARB_base_instance() )
				{
					mDrawIdUniformLocation = glGetUniformLocation(mOpenGLProgram, "drawIdUniform");
				}

				// The actual locations assigned to uniform variables are not known until the program object is linked successfully
				// -> So we have to build a root signature parameter index -> uniform location mapping here
				const Rhi::RootSignature& rootSignatureData = static_cast<const RootSignature&>(rootSignature).getRootSignature();
				const uint32_t numberOfRootParameters = rootSignatureData.numberOfParameters;
				uint32_t uniformBlockBindingIndex = 0;
				for ( uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex )
				{
					const Rhi::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
					{
						SE_ASSERT(nullptr != reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
							const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
						for ( uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex )
						{
							const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

							// Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
							if ( Rhi::DescriptorRangeType::UBV == descriptorRange.rangeType )
							{
								// Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension,
								// for backward compatibility, ask for the uniform block index
								const GLuint uniformBlockIndex = glGetUniformBlockIndex(mOpenGLProgram, descriptorRange.baseShaderRegisterName);
								if ( GL_INVALID_INDEX != uniformBlockIndex )
								{
									// Associate the uniform block with the given binding point
									glUniformBlockBinding(mOpenGLProgram, uniformBlockIndex, uniformBlockBindingIndex);
									++uniformBlockBindingIndex;
								}
							}
							else if ( Rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType )
							{
								const GLint uniformLocation = glGetUniformLocation(mOpenGLProgram, descriptorRange.baseShaderRegisterName);
								if ( uniformLocation >= 0 )
								{
									// OpenGL/GLSL is not automatically assigning texture units to samplers, so, we have to take over this job
									// -> When using OpenGL or OpenGL ES 3 this is required
									// -> OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension supports explicit binding points ("layout(binding = 0)"
									//    in GLSL shader) , for backward compatibility we don't use it in here
									// -> When using Direct3D 9, 10, 11 or 12, the texture unit
									//    to use is usually defined directly within the shader by using the "register"-keyword
									// -> Use the "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" extension if possible to not change OpenGL states
									if ( nullptr != glProgramUniform1i )
									{
										glProgramUniform1i(mOpenGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
									}
									else if ( nullptr != glProgramUniform1iEXT )
									{
										glProgramUniform1iEXT(mOpenGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
									}
									else
									{
										// TODO(co) There's room for binding API call related optimization in here (will certainly be no huge overall efficiency gain)
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently used OpenGL program
										GLint openGLProgramBackup = 0;
										glGetIntegerv(GL_CURRENT_PROGRAM, &openGLProgramBackup);
										if ( static_cast<GLuint>(openGLProgramBackup) == mOpenGLProgram )
										{
											// Set uniform, please note that for this our program must be the currently used one
											glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
										}
										else
										{
											// Set uniform, please note that for this our program must be the currently used one
											glUseProgram(mOpenGLProgram);
											glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));

											// Be polite and restore the previous used OpenGL program
											glUseProgram(static_cast<GLhandleARB>(openGLProgramBackup));
										}
#else
										glUseProgram(mOpenGLProgram);
										glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
#endif
									}
								}
							}
						}
					}
				}
			}
			else
			{
				// Error, program link failed!

				// Get the length of the information (including a null termination)
				GLint informationLength = 0;
				glGetProgramiv(mOpenGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
				if ( informationLength > 1 )
				{
					// Allocate memory for the information
					char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

					// Get the information
					glGetProgramInfoLog(mOpenGLProgram, informationLength, nullptr, informationLog);

					// Output the debug string
					RHI_LOG(CRITICAL, informationLog)

						// Cleanup information memory
						RHI_FREE(informationLog);
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLint mDrawIdUniformLocation;	// Draw ID uniform location, used for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/GraphicsProgramMonolithicDsa.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic graphics program class, effective direct state access (DSA)
	*/
	class GraphicsProgramMonolithicDsa final : public GraphicsProgramMonolithic
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for traditional graphics program
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] vertexShaderMonolithic
		*    Vertex shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationControlShaderMonolithic
		*    Tessellation control shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationEvaluationShaderMonolithic
		*    Tessellation evaluation shader the graphics program is using, can be a null pointer
		*  @param[in] geometryShaderMonolithic
		*    Geometry shader the graphics program is using, can be a null pointer
		*  @param[in] fragmentShaderMonolithic
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		inline GraphicsProgramMonolithicDsa(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, const Rhi::VertexAttributes& vertexAttributes, VertexShaderMonolithic* vertexShaderMonolithic, TessellationControlShaderMonolithic* tessellationControlShaderMonolithic, TessellationEvaluationShaderMonolithic* tessellationEvaluationShaderMonolithic, GeometryShaderMonolithic* geometryShaderMonolithic, FragmentShaderMonolithic* fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			GraphicsProgramMonolithic(openGLRhi, rootSignature, vertexAttributes, vertexShaderMonolithic, tessellationControlShaderMonolithic, tessellationEvaluationShaderMonolithic, geometryShaderMonolithic, fragmentShaderMonolithic RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		/**
		*  @brief
		*    Constructor for task and mesh shader based graphics program
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] taskShaderMonolithic
		*    Task shader the graphics program is using, can be a null pointer
		*  @param[in] meshShaderMonolithic
		*    Mesh shader the graphics program is using
		*  @param[in] fragmentShaderMonolithic
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		inline GraphicsProgramMonolithicDsa(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, TaskShaderMonolithic* taskShaderMonolithic, MeshShaderMonolithic& meshShaderMonolithic, FragmentShaderMonolithic* fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			GraphicsProgramMonolithic(openGLRhi, rootSignature, taskShaderMonolithic, meshShaderMonolithic, fragmentShaderMonolithic RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~GraphicsProgramMonolithicDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IGraphicsProgram methods          ]
		//[-------------------------------------------------------]
	public:
		virtual void setUniform1f(handle uniformHandle, float value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniform1f(mOpenGLProgram, static_cast<GLint>(uniformHandle), value);
			}
			else
			{
				glProgramUniform1fEXT(mOpenGLProgram, static_cast<GLint>(uniformHandle), value);
			}
		}

		virtual void setUniform2fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniform2fv(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				glProgramUniform2fvEXT(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, value);
			}
		}

		virtual void setUniform3fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniform3fv(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				glProgramUniform3fvEXT(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, value);
			}
		}

		virtual void setUniform4fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniform4fv(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				glProgramUniform4fvEXT(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, value);
			}
		}

		virtual void setUniformMatrix3fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniformMatrix3fv(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
			else
			{
				glProgramUniformMatrix3fvEXT(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
		}

		virtual void setUniformMatrix4fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniformMatrix4fv(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
			else
			{
				glProgramUniformMatrix4fvEXT(mOpenGLProgram, static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GraphicsProgramMonolithicDsa(const GraphicsProgramMonolithicDsa&) = delete;
		GraphicsProgramMonolithicDsa& operator =(const GraphicsProgramMonolithicDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/ComputePipelineState.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract compute pipeline state base class
	*/
	class ComputePipelineState : public Rhi::IComputePipelineState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] id
		*    The unique compact compute pipeline state ID
		*/
		inline explicit ComputePipelineState(OpenGLRhi& openGLRhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IComputePipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ComputePipelineState() override
		{
			// Free the unique compact compute pipeline state ID
			static_cast<OpenGLRhi&>(getRhi()).ComputePipelineStateMakeId.DestroyID(getId());
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ComputePipelineState(const ComputePipelineState&) = delete;
		ComputePipelineState& operator =(const ComputePipelineState&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/ComputePipelineStateMonolithic.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic compute pipeline state class
	*/
	class ComputePipelineStateMonolithic : public ComputePipelineState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] computeShaderMonolithic
		*    Compute shader the compute pipeline state is using
		*  @param[in] id
		*    The unique compact compute pipeline state ID
		*
		*  @note
		*    - The compute pipeline state keeps a reference to the provided compute shader and releases it when no longer required
		*/
		ComputePipelineStateMonolithic(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, ComputeShaderMonolithic& computeShaderMonolithic, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ComputePipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLProgram(glCreateProgram())
		{
			// Attach the compute shader to the program
			// -> We don't need to keep a reference to the shader, to add and release at once to ensure a nice behaviour
			computeShaderMonolithic.AddReference();
			glAttachShader(mOpenGLProgram, computeShaderMonolithic.getOpenGLShader());
			computeShaderMonolithic.ReleaseReference();

			// Link the program
			glLinkProgram(mOpenGLProgram);

			// Check the link status
			GLint linked = GL_FALSE;
			glGetShaderiv(mOpenGLProgram, GL_LINK_STATUS, &linked);
			if ( GL_TRUE == linked )
			{
				// We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
				// -> Use modern GLSL:
				//    "layout(location = 0) out vec4 ColorOutput0;"
				//    "layout(location = 1) out vec4 ColorOutput1;"
				// -> Use legacy GLSL if necessary:
				//    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
				//    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

				// The actual locations assigned to uniform variables are not known until the program object is linked successfully
				// -> So we have to build a root signature parameter index -> uniform location mapping here
				const Rhi::RootSignature& rootSignatureData = static_cast<const RootSignature&>(rootSignature).getRootSignature();
				const uint32_t numberOfRootParameters = rootSignatureData.numberOfParameters;
				uint32_t uniformBlockBindingIndex = 0;
				for ( uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex )
				{
					const Rhi::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
					{
						SE_ASSERT(nullptr != reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
							const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
						for ( uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex )
						{
							const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

							// Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
							if ( Rhi::DescriptorRangeType::UBV == descriptorRange.rangeType )
							{
								// Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension,
								// for backward compatibility, ask for the uniform block index
								const GLuint uniformBlockIndex = glGetUniformBlockIndex(mOpenGLProgram, descriptorRange.baseShaderRegisterName);
								if ( GL_INVALID_INDEX != uniformBlockIndex )
								{
									// Associate the uniform block with the given binding point
									glUniformBlockBinding(mOpenGLProgram, uniformBlockIndex, uniformBlockBindingIndex);
									++uniformBlockBindingIndex;
								}
							}
							else if ( Rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType )
							{
								const GLint uniformLocation = glGetUniformLocation(mOpenGLProgram, descriptorRange.baseShaderRegisterName);
								if ( uniformLocation >= 0 )
								{
									// OpenGL/GLSL is not automatically assigning texture units to samplers, so, we have to take over this job
									// -> When using OpenGL or OpenGL ES 3 this is required
									// -> OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension supports explicit binding points ("layout(binding = 0)"
									//    in GLSL shader) , for backward compatibility we don't use it in here
									// -> When using Direct3D 9, 10, 11 or 12, the texture unit
									//    to use is usually defined directly within the shader by using the "register"-keyword
									// -> Use the "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" extension if possible to not change OpenGL states
									if ( nullptr != glProgramUniform1i )
									{
										glProgramUniform1i(mOpenGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
									}
									else if ( nullptr != glProgramUniform1iEXT )
									{
										glProgramUniform1iEXT(mOpenGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
									}
									else
									{
										// TODO(co) There's room for binding API call related optimization in here (will certainly be no huge overall efficiency gain)
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently used OpenGL program
										GLint openGLProgramBackup = 0;
										glGetIntegerv(GL_CURRENT_PROGRAM, &openGLProgramBackup);
										if ( static_cast<GLuint>(openGLProgramBackup) == mOpenGLProgram )
										{
											// Set uniform, please note that for this our program must be the currently used one
											glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
										}
										else
										{
											// Set uniform, please note that for this our program must be the currently used one
											glUseProgram(mOpenGLProgram);
											glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));

											// Be polite and restore the previous used OpenGL program
											glUseProgram(static_cast<GLhandleARB>(openGLProgramBackup));
										}
#else
										glUseProgram(mOpenGLProgram);
										glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
#endif
									}
								}
							}
						}
					}
				}
			}
			else
			{
				// Error, program link failed!

				// Get the length of the information (including a null termination)
				GLint informationLength = 0;
				glGetProgramiv(mOpenGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
				if ( informationLength > 1 )
				{
					// Allocate memory for the information
					char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

					// Get the information
					glGetProgramInfoLog(mOpenGLProgram, informationLength, nullptr, informationLog);

					// Output the debug string
					RHI_LOG(CRITICAL, informationLog)

						// Cleanup information memory
						RHI_FREE(informationLog);
				}
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Compute PSO", 14)	// 14 = "Compute PSO: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ComputePipelineStateMonolithic() override
		{
			// Destroy the OpenGL program
			// -> A value of 0 for program will be silently ignored
			glDeleteShader(mOpenGLProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL program
		*
		*  @return
		*    The OpenGL program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLProgram() const
		{
			return mOpenGLProgram;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ComputePipelineStateMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLProgram;	// OpenGL program, can be zero if no resource is allocated


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit ComputePipelineStateMonolithic(const ComputePipelineStateMonolithic&) = delete;
		ComputePipelineStateMonolithic& operator =(const ComputePipelineStateMonolithic&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Monolithic/ShaderLanguageMonolithic.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Monolithic shader language class
	*/
	class ShaderLanguageMonolithic final : public Rhi::IShaderLanguage
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit ShaderLanguageMonolithic(OpenGLRhi& openGLRhi) :
			IShaderLanguage(openGLRhi)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ShaderLanguageMonolithic() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShaderLanguage methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}

		[[nodiscard]] inline virtual Rhi::IVertexShader* createVertexShaderFromBytecode(const Rhi::VertexAttributes&, const Rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override
		{
			// Error!
			SE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
				return nullptr;
		}

		[[nodiscard]] virtual Rhi::IVertexShader* createVertexShaderFromSourceCode([[maybe_unused]] const Rhi::VertexAttributes& vertexAttributes, const Rhi::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's vertex shader support
			// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			if ( openGLRhi.getExtensions().isGL_ARB_vertex_shader() )
			{
				return RHI_NEW(VertexShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no vertex shader support!
				return nullptr;
			}
		}

		[[nodiscard]] inline virtual Rhi::ITessellationControlShader* createTessellationControlShaderFromBytecode(const Rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override
		{
			// Error!
			SE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
				return nullptr;
		}

		[[nodiscard]] virtual Rhi::ITessellationControlShader* createTessellationControlShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's tessellation support
			// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			if ( openGLRhi.getExtensions().isGL_ARB_tessellation_shader() )
			{
				return RHI_NEW(TessellationControlShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no tessellation support!
				return nullptr;
			}
		}

		[[nodiscard]] inline virtual Rhi::ITessellationEvaluationShader* createTessellationEvaluationShaderFromBytecode(const Rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override
		{
			// Error!
			SE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
				return nullptr;
		}

		[[nodiscard]] virtual Rhi::ITessellationEvaluationShader* createTessellationEvaluationShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's tessellation support
			// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			if ( openGLRhi.getExtensions().isGL_ARB_tessellation_shader() )
			{
				return RHI_NEW(TessellationEvaluationShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no tessellation support!
				return nullptr;
			}
		}

		[[nodiscard]] inline virtual Rhi::IGeometryShader* createGeometryShaderFromBytecode(const Rhi::ShaderBytecode&, Rhi::GsInputPrimitiveTopology, Rhi::GsOutputPrimitiveTopology, uint32_t RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override
		{
			// Error!
			SE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
				return nullptr;
		}

		[[nodiscard]] virtual Rhi::IGeometryShader* createGeometryShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices, [[maybe_unused]] Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's geometry shader support
			// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			if ( openGLRhi.getExtensions().isGL_ARB_geometry_shader4() )
			{
				// In modern GLSL, "geometry shader input primitive topology" & "geometry shader output primitive topology" & "number of output vertices" can be directly set within GLSL by writing e.g.
				//   "layout(triangles) in;"
				//   "layout(triangle_strip, max_vertices = 3) out;"
				// -> To be able to support older GLSL versions, we have to provide this information also via OpenGL API functions
				return RHI_NEW(GeometryShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode, gsInputPrimitiveTopology, gsOutputPrimitiveTopology, numberOfOutputVertices RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no geometry shader support!
				return nullptr;
			}
		}

		[[nodiscard]] inline virtual Rhi::IFragmentShader* createFragmentShaderFromBytecode(const Rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override
		{
			// Error!
			SE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
				return nullptr;
		}

		[[nodiscard]] virtual Rhi::IFragmentShader* createFragmentShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's fragment shader support
			// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			if ( openGLRhi.getExtensions().isGL_ARB_fragment_shader() )
			{
				return RHI_NEW(FragmentShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no fragment shader support!
				return nullptr;
			}
		}

		[[nodiscard]] inline virtual Rhi::ITaskShader* createTaskShaderFromBytecode(const Rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override
		{
			// Error!
			SE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
				return nullptr;
		}

		[[nodiscard]] virtual Rhi::ITaskShader* createTaskShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's mesh shader support
			// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			if ( openGLRhi.getExtensions().isGL_NV_mesh_shader() )
			{
				return RHI_NEW(TaskShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no task shader support!
				return nullptr;
			}
		}

		[[nodiscard]] inline virtual Rhi::IMeshShader* createMeshShaderFromBytecode(const Rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override
		{
			// Error!
			SE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
				return nullptr;
		}

		[[nodiscard]] virtual Rhi::IMeshShader* createMeshShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's mesh shader support
			// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			if ( openGLRhi.getExtensions().isGL_NV_mesh_shader() )
			{
				return RHI_NEW(MeshShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no mesh shader support!
				return nullptr;
			}
		}

		[[nodiscard]] inline virtual Rhi::IComputeShader* createComputeShaderFromBytecode(const Rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override
		{
			// Error!
			SE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
				return nullptr;
		}

		[[nodiscard]] virtual Rhi::IComputeShader* createComputeShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's compute shader support
			// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			if ( openGLRhi.getExtensions().isGL_ARB_compute_shader() )
			{
				return RHI_NEW(ComputeShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no compute shader support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IGraphicsProgram* createGraphicsProgram(const Rhi::IRootSignature& rootSignature, const Rhi::VertexAttributes& vertexAttributes, Rhi::IVertexShader* vertexShader, Rhi::ITessellationControlShader* tessellationControlShader, Rhi::ITessellationEvaluationShader* tessellationEvaluationShader, Rhi::IGeometryShader* geometryShader, Rhi::IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity checks
			// -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
			// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
			//    the name is safe because we know that we always reference to one and the same name address
			// TODO(co) Add security check: Is the given resource one of the currently used RHI?
			SE_ASSERT(nullptr == vertexShader || vertexShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL vertex shader language mismatch")
				SE_ASSERT(nullptr == tessellationControlShader || tessellationControlShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL tessellation control shader language mismatch")
				SE_ASSERT(nullptr == tessellationEvaluationShader || tessellationEvaluationShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL tessellation evaluation shader language mismatch")
				SE_ASSERT(nullptr == geometryShader || geometryShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL geometry shader language mismatch")
				SE_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL fragment shader language mismatch")

				// Create the graphics program: Is "GL_EXT_direct_state_access" there?
				if ( openGLRhi.getExtensions().isGL_EXT_direct_state_access() || openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(GraphicsProgramMonolithicDsa)(openGLRhi, rootSignature, vertexAttributes, static_cast<VertexShaderMonolithic*>(vertexShader), static_cast<TessellationControlShaderMonolithic*>(tessellationControlShader), static_cast<TessellationEvaluationShaderMonolithic*>(tessellationEvaluationShader), static_cast<GeometryShaderMonolithic*>(geometryShader), static_cast<FragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(GraphicsProgramMonolithic)(openGLRhi, rootSignature, vertexAttributes, static_cast<VertexShaderMonolithic*>(vertexShader), static_cast<TessellationControlShaderMonolithic*>(tessellationControlShader), static_cast<TessellationEvaluationShaderMonolithic*>(tessellationEvaluationShader), static_cast<GeometryShaderMonolithic*>(geometryShader), static_cast<FragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
		}

		[[nodiscard]] virtual Rhi::IGraphicsProgram* createGraphicsProgram(const Rhi::IRootSignature& rootSignature, Rhi::ITaskShader* taskShader, Rhi::IMeshShader& meshShader, Rhi::IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity checks
			// -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
			// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
			//    the name is safe because we know that we always reference to one and the same name address
			// TODO(co) Add security check: Is the given resource one of the currently used RHI?
			SE_ASSERT(nullptr == taskShader || taskShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL task shader language mismatch")
				SE_ASSERT(meshShader.getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL mesh shader language mismatch")
				SE_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL fragment shader language mismatch")

				// Create the graphics program: Is "GL_EXT_direct_state_access" there?
				if ( openGLRhi.getExtensions().isGL_EXT_direct_state_access() || openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
				{
					// Effective direct state access (DSA)
					return RHI_NEW(GraphicsProgramMonolithicDsa)(openGLRhi, rootSignature, static_cast<TaskShaderMonolithic*>(taskShader), static_cast<MeshShaderMonolithic&>(meshShader), static_cast<FragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
				else
				{
					// Traditional bind version
					return RHI_NEW(GraphicsProgramMonolithic)(openGLRhi, rootSignature, static_cast<TaskShaderMonolithic*>(taskShader), static_cast<MeshShaderMonolithic&>(meshShader), static_cast<FragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
				}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ShaderLanguageMonolithic, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ShaderLanguageMonolithic(const ShaderLanguageMonolithic&) = delete;
		ShaderLanguageMonolithic& operator =(const ShaderLanguageMonolithic&) = delete;


	};


	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/VertexShaderSeparate.h      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate vertex shader class
	*/
	class VertexShaderSeparate final : public Rhi::IVertexShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a vertex shader from shader bytecode
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		inline VertexShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::VertexAttributes& vertexAttributes, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IVertexShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromBytecode(vertexAttributes, GL_VERTEX_SHADER_ARB, shaderBytecode)),
			mDrawIdUniformLocation(openGLRhi.getExtensions().isGL_ARB_base_instance() ? -1 : glGetUniformLocation(mOpenGLShaderProgram, "drawIdUniform"))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VS", 5)	// 5 = "VS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a vertex shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline VertexShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::VertexAttributes& vertexAttributes, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IVertexShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromSourcecode(vertexAttributes, GL_VERTEX_SHADER_ARB, sourceCode)),
			mDrawIdUniformLocation(openGLRhi.getExtensions().isGL_ARB_base_instance() ? -1 : glGetUniformLocation(mOpenGLShaderProgram, "drawIdUniform"))
		{
			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				::detail::shaderSourceCodeToShaderBytecode(GL_VERTEX_SHADER_ARB, sourceCode, *shaderBytecode);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VS", 5)	// 5 = "VS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VertexShaderSeparate() override
		{
			// Destroy the OpenGL shader program
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteProgram(mOpenGLShaderProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL shader program
		*
		*  @return
		*    The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShaderProgram() const
		{
			return mOpenGLShaderProgram;
		}

		/**
		*  @brief
		*    Return the draw ID uniform location
		*
		*  @return
		*    Draw ID uniform location, -1 if there's no such uniform
		*/
		[[nodiscard]] inline GLint getDrawIdUniformLocation() const
		{
			return mDrawIdUniformLocation;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(VertexShaderSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexShaderSeparate(const VertexShaderSeparate&) = delete;
		VertexShaderSeparate& operator =(const VertexShaderSeparate&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShaderProgram;	// OpenGL shader program, can be zero if no resource is allocated
		GLint  mDrawIdUniformLocation;	// Draw ID uniform location, used for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/TessellationControlShaderSeparate.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate tessellation control shader ("hull shader" in Direct3D terminology) class
	*/
	class TessellationControlShaderSeparate final : public Rhi::ITessellationControlShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a tessellation control shader ("hull shader" in Direct3D terminology) shader from shader bytecode
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		inline TessellationControlShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationControlShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromBytecode(GL_TESS_CONTROL_SHADER, shaderBytecode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TCS", 6)	// 6 = "TCS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a tessellation control shader ("hull shader" in Direct3D terminology) shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline TessellationControlShaderSeparate(OpenGLRhi& openGLRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationControlShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromSourceCode(GL_TESS_CONTROL_SHADER, sourceCode))
		{
			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				::detail::shaderSourceCodeToShaderBytecode(GL_TESS_CONTROL_SHADER, sourceCode, *shaderBytecode);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TCS", 6)	// 6 = "TCS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TessellationControlShaderSeparate() override
		{
			// Destroy the OpenGL shader program
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteProgram(mOpenGLShaderProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL shader program
		*
		*  @return
		*    The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShaderProgram() const
		{
			return mOpenGLShaderProgram;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TessellationControlShaderSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TessellationControlShaderSeparate(const TessellationControlShaderSeparate&) = delete;
		TessellationControlShaderSeparate& operator =(const TessellationControlShaderSeparate&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShaderProgram;	// OpenGL shader program, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/TessellationEvaluationShaderSeparate.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate tessellation evaluation shader ("domain shader" in Direct3D terminology) class
	*/
	class TessellationEvaluationShaderSeparate final : public Rhi::ITessellationEvaluationShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a tessellation evaluation shader ("domain shader" in Direct3D terminology) shader from shader bytecode
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		inline TessellationEvaluationShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationEvaluationShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromBytecode(GL_TESS_EVALUATION_SHADER, shaderBytecode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TES", 6)	// 6 = "TES: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a tessellation evaluation shader ("domain shader" in Direct3D terminology) shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline TessellationEvaluationShaderSeparate(OpenGLRhi& openGLRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITessellationEvaluationShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromSourceCode(GL_TESS_EVALUATION_SHADER, sourceCode))
		{
			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				::detail::shaderSourceCodeToShaderBytecode(GL_TESS_EVALUATION_SHADER, sourceCode, *shaderBytecode);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TES", 6)	// 6 = "TES: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TessellationEvaluationShaderSeparate() override
		{
			// Destroy the OpenGL shader program
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteProgram(mOpenGLShaderProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL shader program
		*
		*  @return
		*    The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShaderProgram() const
		{
			return mOpenGLShaderProgram;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TessellationEvaluationShaderSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TessellationEvaluationShaderSeparate(const TessellationEvaluationShaderSeparate&) = delete;
		TessellationEvaluationShaderSeparate& operator =(const TessellationEvaluationShaderSeparate&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShaderProgram;	// OpenGL shader program, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/GeometryShaderSeparate.h    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate geometry shader class
	*/
	class GeometryShaderSeparate final : public Rhi::IGeometryShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a geometry shader from shader bytecode
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*  @param[in] gsInputPrimitiveTopology
		*    Geometry shader input primitive topology
		*  @param[in] gsOutputPrimitiveTopology
		*    Geometry shader output primitive topology
		*  @param[in] numberOfOutputVertices
		*    Number of output vertices
		*/
		inline GeometryShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::ShaderBytecode& shaderBytecode, [[maybe_unused]] Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, [[maybe_unused]] Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, [[maybe_unused]] uint32_t numberOfOutputVertices RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGeometryShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromBytecode(GL_GEOMETRY_SHADER_ARB, shaderBytecode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a geometry shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*  @param[in] gsInputPrimitiveTopology
		*    Geometry shader input primitive topology
		*  @param[in] gsOutputPrimitiveTopology
		*    Geometry shader output primitive topology
		*  @param[in] numberOfOutputVertices
		*    Number of output vertices
		*/
		inline GeometryShaderSeparate(OpenGLRhi& openGLRhi, const char* sourceCode, Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGeometryShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromSourceCode(GL_GEOMETRY_SHADER_ARB, sourceCode))
		{
			// In modern GLSL, "geometry shader input primitive topology" & "geometry shader output primitive topology" & "number of output vertices" can be directly set within GLSL by writing e.g.
			//   "layout(triangles) in;"
			//   "layout(triangle_strip, max_vertices = 3) out;"
			// -> To be able to support older GLSL versions, we have to provide this information also via OpenGL API functions
			glProgramParameteriARB(mOpenGLShaderProgram, GL_GEOMETRY_INPUT_TYPE_ARB, static_cast<int>(gsInputPrimitiveTopology));	// The "Rhi::GsInputPrimitiveTopology" values directly map to OpenGL constants, do not change them
			glProgramParameteriARB(mOpenGLShaderProgram, GL_GEOMETRY_OUTPUT_TYPE_ARB, static_cast<int>(gsOutputPrimitiveTopology));	// The "Rhi::GsOutputPrimitiveTopology" values directly map to OpenGL constants, do not change them
			glProgramParameteriARB(mOpenGLShaderProgram, GL_GEOMETRY_VERTICES_OUT_ARB, static_cast<GLint>(numberOfOutputVertices));

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				::detail::shaderSourceCodeToShaderBytecode(GL_GEOMETRY_SHADER_ARB, sourceCode, *shaderBytecode);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~GeometryShaderSeparate() override
		{
			// Destroy the OpenGL shader program
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteProgram(mOpenGLShaderProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL shader program
		*
		*  @return
		*    The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShaderProgram() const
		{
			return mOpenGLShaderProgram;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(GeometryShaderSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GeometryShaderSeparate(const GeometryShaderSeparate&) = delete;
		GeometryShaderSeparate& operator =(const GeometryShaderSeparate&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShaderProgram;	// OpenGL shader program, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/FragmentShaderSeparate.h    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate fragment shader (FS, "pixel shader" in Direct3D terminology) class
	*/
	class FragmentShaderSeparate final : public Rhi::IFragmentShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a fragment shader from shader bytecode
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		inline FragmentShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IFragmentShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromBytecode(GL_FRAGMENT_SHADER_ARB, shaderBytecode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FS", 5)	// 5 = "FS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a fragment shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline FragmentShaderSeparate(OpenGLRhi& openGLRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IFragmentShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromSourceCode(GL_FRAGMENT_SHADER_ARB, sourceCode))
		{
			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				::detail::shaderSourceCodeToShaderBytecode(GL_FRAGMENT_SHADER_ARB, sourceCode, *shaderBytecode);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FS", 5)	// 5 = "FS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~FragmentShaderSeparate() override
		{
			// Destroy the OpenGL shader program
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteProgram(mOpenGLShaderProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL shader program
		*
		*  @return
		*    The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShaderProgram() const
		{
			return mOpenGLShaderProgram;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(FragmentShaderSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit FragmentShaderSeparate(const FragmentShaderSeparate&) = delete;
		FragmentShaderSeparate& operator =(const FragmentShaderSeparate&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShaderProgram;	// OpenGL shader program, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/TaskShaderSeparate.h        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate task shader (TS, "amplification shader" in Direct3D terminology) class
	*/
	class TaskShaderSeparate final : public Rhi::ITaskShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a task shader from shader bytecode
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		inline TaskShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITaskShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromBytecode(GL_TASK_SHADER_NV, shaderBytecode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TS", 5)	// 5 = "TS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a task shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline TaskShaderSeparate(OpenGLRhi& openGLRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITaskShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromSourceCode(GL_TASK_SHADER_NV, sourceCode))
		{
			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				::detail::shaderSourceCodeToShaderBytecode(GL_TASK_SHADER_NV, sourceCode, *shaderBytecode);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TS", 5)	// 5 = "TS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TaskShaderSeparate() override
		{
			// Destroy the OpenGL shader program
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteProgram(mOpenGLShaderProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL shader program
		*
		*  @return
		*    The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShaderProgram() const
		{
			return mOpenGLShaderProgram;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TaskShaderSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TaskShaderSeparate(const TaskShaderSeparate&) = delete;
		TaskShaderSeparate& operator =(const TaskShaderSeparate&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShaderProgram;	// OpenGL shader program, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/MeshShaderSeparate.h        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate mesh shader (MS) class
	*/
	class MeshShaderSeparate final : public Rhi::IMeshShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a mesh shader from shader bytecode
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		inline MeshShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IMeshShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromBytecode(GL_MESH_SHADER_NV, shaderBytecode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "MS", 5)	// 5 = "MS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a mesh shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline MeshShaderSeparate(OpenGLRhi& openGLRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IMeshShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromSourceCode(GL_MESH_SHADER_NV, sourceCode))
		{
			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				::detail::shaderSourceCodeToShaderBytecode(GL_MESH_SHADER_NV, sourceCode, *shaderBytecode);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "MS", 5)	// 5 = "MS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MeshShaderSeparate() override
		{
			// Destroy the OpenGL shader program
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteProgram(mOpenGLShaderProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL shader program
		*
		*  @return
		*    The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShaderProgram() const
		{
			return mOpenGLShaderProgram;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(MeshShaderSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit MeshShaderSeparate(const MeshShaderSeparate&) = delete;
		MeshShaderSeparate& operator =(const MeshShaderSeparate&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShaderProgram;	// OpenGL shader program, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/ComputeShaderSeparate.h     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate compute shader (CS) class
	*/
	class ComputeShaderSeparate final : public Rhi::IComputeShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a compute shader from shader bytecode
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		inline ComputeShaderSeparate(OpenGLRhi& openGLRhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IComputeShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromBytecode(GL_COMPUTE_SHADER, shaderBytecode))
		{
			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "CS", 5)	// 5 = "CS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for creating a compute shader from shader source code
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		inline ComputeShaderSeparate(OpenGLRhi& openGLRhi, const char* sourceCode, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IComputeShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLShaderProgram(::detail::loadShaderProgramFromSourceCode(GL_COMPUTE_SHADER, sourceCode))
		{
			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				::detail::shaderSourceCodeToShaderBytecode(GL_COMPUTE_SHADER, sourceCode, *shaderBytecode);
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "CS", 5)	// 5 = "CS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ComputeShaderSeparate() override
		{
			// Destroy the OpenGL shader program
			// -> Silently ignores 0's and names that do not correspond to existing buffer objects
			glDeleteProgram(mOpenGLShaderProgram);
		}

		/**
		*  @brief
		*    Return the OpenGL shader program
		*
		*  @return
		*    The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLShaderProgram() const
		{
			return mOpenGLShaderProgram;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ComputeShaderSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ComputeShaderSeparate(const ComputeShaderSeparate&) = delete;
		ComputeShaderSeparate& operator =(const ComputeShaderSeparate&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLuint mOpenGLShaderProgram;	// OpenGL shader program, can be zero if no resource is allocated


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/GraphicsProgramSeparate.h   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate graphics program class
	*/
	class GraphicsProgramSeparate : public Rhi::IGraphicsProgram
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for traditional graphics program
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] vertexShaderSeparate
		*    Vertex shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationControlShaderSeparate
		*    Tessellation control shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationEvaluationShaderSeparate
		*    Tessellation evaluation shader the graphics program is using, can be a null pointer
		*  @param[in] geometryShaderSeparate
		*    Geometry shader the graphics program is using, can be a null pointer
		*  @param[in] fragmentShaderSeparate
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		GraphicsProgramSeparate(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, VertexShaderSeparate* vertexShaderSeparate, TessellationControlShaderSeparate* tessellationControlShaderSeparate, TessellationEvaluationShaderSeparate* tessellationEvaluationShaderSeparate, GeometryShaderSeparate* geometryShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLProgramPipeline(0),
			// Traditional graphics program
			mVertexShaderSeparate(vertexShaderSeparate),
			mTessellationControlShaderSeparate(tessellationControlShaderSeparate),
			mTessellationEvaluationShaderSeparate(tessellationEvaluationShaderSeparate),
			mGeometryShaderSeparate(geometryShaderSeparate),
			// Both graphics programs
			mFragmentShaderSeparate(fragmentShaderSeparate),
			// Task and mesh shader based graphics program
			mTaskShaderSeparate(nullptr),
			mMeshShaderSeparate(nullptr)
		{
			// Create the OpenGL program pipeline
			glGenProgramPipelines(1, &mOpenGLProgramPipeline);

			// If the "GL_ARB_direct_state_access" nor "GL_EXT_direct_state_access" extension is available, we need to change OpenGL states during resource creation (nasty thing)
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
#endif
			if ( nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT )
			{
#if SE_OPENGL_STATE_CLEANUP
				glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);
#endif
				glBindProgramPipeline(mOpenGLProgramPipeline);
			}

			// Add references to the provided shaders
#define USE_PROGRAM_STAGES(ShaderBit, ShaderSeparate) if (nullptr != ShaderSeparate) { ShaderSeparate->AddReference(); glUseProgramStages(mOpenGLProgramPipeline, ShaderBit, ShaderSeparate->getOpenGLShaderProgram()); }
			USE_PROGRAM_STAGES(GL_VERTEX_SHADER_BIT, mVertexShaderSeparate)
				USE_PROGRAM_STAGES(GL_TESS_CONTROL_SHADER_BIT, mTessellationControlShaderSeparate)
				USE_PROGRAM_STAGES(GL_TESS_EVALUATION_SHADER_BIT, mTessellationEvaluationShaderSeparate)
				USE_PROGRAM_STAGES(GL_GEOMETRY_SHADER_BIT, mGeometryShaderSeparate)
				USE_PROGRAM_STAGES(GL_FRAGMENT_SHADER_BIT, mFragmentShaderSeparate)
#undef USE_PROGRAM_STAGES

				// Validate program pipeline
				glValidateProgramPipeline(mOpenGLProgramPipeline);
			GLint validateStatus = 0;
			glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
			if ( GL_TRUE == validateStatus )
			{
				// We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
				// -> Use modern GLSL:
				//    "layout(location = 0) out vec4 ColorOutput0;"
				//    "layout(location = 1) out vec4 ColorOutput1;"
				// -> Use legacy GLSL if necessary:
				//    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
				//    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

				// The actual locations assigned to uniform variables are not known until the program object is linked successfully
				// -> So we have to build a root signature parameter index -> uniform location mapping here
				const Rhi::RootSignature& rootSignatureData = static_cast<const RootSignature&>(rootSignature).getRootSignature();
				const uint32_t numberOfRootParameters = rootSignatureData.numberOfParameters;
				uint32_t uniformBlockBindingIndex = 0;
				for ( uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex )
				{
					const Rhi::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
					{
						SE_ASSERT(nullptr != reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
							const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
						for ( uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex )
						{
							const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

							// Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
							if ( Rhi::DescriptorRangeType::UBV == descriptorRange.rangeType )
							{
#define BIND_UNIFORM_BLOCK(ShaderSeparate) if (nullptr != ShaderSeparate) ::detail::bindUniformBlock(descriptorRange, ShaderSeparate->getOpenGLShaderProgram(), uniformBlockBindingIndex);
								switch ( descriptorRange.shaderVisibility )
								{
								case Rhi::ShaderVisibility::ALL:
								case Rhi::ShaderVisibility::ALL_GRAPHICS:
									BIND_UNIFORM_BLOCK(mVertexShaderSeparate)
										BIND_UNIFORM_BLOCK(mTessellationControlShaderSeparate)
										BIND_UNIFORM_BLOCK(mTessellationEvaluationShaderSeparate)
										BIND_UNIFORM_BLOCK(mGeometryShaderSeparate)
										BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
										break;

								case Rhi::ShaderVisibility::VERTEX:
									BIND_UNIFORM_BLOCK(mVertexShaderSeparate)
										break;

								case Rhi::ShaderVisibility::TESSELLATION_CONTROL:
									BIND_UNIFORM_BLOCK(mTessellationControlShaderSeparate)
										break;

								case Rhi::ShaderVisibility::TESSELLATION_EVALUATION:
									BIND_UNIFORM_BLOCK(mTessellationEvaluationShaderSeparate)
										break;

								case Rhi::ShaderVisibility::GEOMETRY:
									BIND_UNIFORM_BLOCK(mGeometryShaderSeparate)
										break;

								case Rhi::ShaderVisibility::FRAGMENT:
									BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
										break;

								case Rhi::ShaderVisibility::TASK:
								case Rhi::ShaderVisibility::MESH:
								case Rhi::ShaderVisibility::COMPUTE:
									RHI_LOG(CRITICAL, "Invalid OpenGL shader visibility")
										break;
								}
#undef BIND_UNIFORM_BLOCK
								++uniformBlockBindingIndex;
							}
							else if ( Rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType )
							{
#define BIND_UNIFORM_LOCATION(ShaderSeparate) if (nullptr != ShaderSeparate) ::detail::bindUniformLocation(descriptorRange, mOpenGLProgramPipeline, ShaderSeparate->getOpenGLShaderProgram());
								switch ( descriptorRange.shaderVisibility )
								{
								case Rhi::ShaderVisibility::ALL:
								case Rhi::ShaderVisibility::ALL_GRAPHICS:
									BIND_UNIFORM_LOCATION(mVertexShaderSeparate)
										BIND_UNIFORM_LOCATION(mTessellationControlShaderSeparate)
										BIND_UNIFORM_LOCATION(mTessellationEvaluationShaderSeparate)
										BIND_UNIFORM_LOCATION(mGeometryShaderSeparate)
										BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
										break;

								case Rhi::ShaderVisibility::VERTEX:
									BIND_UNIFORM_LOCATION(mVertexShaderSeparate)
										break;

								case Rhi::ShaderVisibility::TESSELLATION_CONTROL:
									BIND_UNIFORM_LOCATION(mTessellationControlShaderSeparate)
										break;

								case Rhi::ShaderVisibility::TESSELLATION_EVALUATION:
									BIND_UNIFORM_LOCATION(mTessellationEvaluationShaderSeparate)
										break;

								case Rhi::ShaderVisibility::GEOMETRY:
									BIND_UNIFORM_LOCATION(mGeometryShaderSeparate)
										break;

								case Rhi::ShaderVisibility::FRAGMENT:
									BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
										break;

								case Rhi::ShaderVisibility::TASK:
								case Rhi::ShaderVisibility::MESH:
								case Rhi::ShaderVisibility::COMPUTE:
									RHI_LOG(CRITICAL, "Invalid OpenGL shader visibility")
										break;
								}
#undef BIND_UNIFORM_LOCATION
							}
						}
					}
				}
			}
			else
			{
				// Error, program pipeline validation failed!

				// Get the length of the information (including a null termination)
				GLint informationLength = 0;
				glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_INFO_LOG_LENGTH, &informationLength);
				if ( informationLength > 1 )
				{
					// Allocate memory for the information
					char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

					// Get the information
					glGetProgramPipelineInfoLog(mOpenGLProgramPipeline, informationLength, nullptr, informationLog);

					// Output the debug string
					RHI_LOG(CRITICAL, informationLog)

						// Cleanup information memory
						RHI_FREE(informationLog);
				}
			}

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous used OpenGL program pipeline
			if ( nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT )
			{
				glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
			}
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLProgramPipeline && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
					glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Constructor for task and mesh shader based graphics program
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] taskShaderSeparate
		*    Task shader the graphics program is using, can be a null pointer
		*  @param[in] meshShaderSeparate
		*    Mesh shader the graphics program is using
		*  @param[in] fragmentShaderSeparate
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		GraphicsProgramSeparate(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, TaskShaderSeparate* taskShaderSeparate, MeshShaderSeparate& meshShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLProgramPipeline(0),
			// Traditional graphics program
			mVertexShaderSeparate(nullptr),
			mTessellationControlShaderSeparate(nullptr),
			mTessellationEvaluationShaderSeparate(nullptr),
			mGeometryShaderSeparate(nullptr),
			// Both graphics programs
			mFragmentShaderSeparate(fragmentShaderSeparate),
			// Task and mesh shader based graphics program
			mTaskShaderSeparate(taskShaderSeparate),
			mMeshShaderSeparate(&meshShaderSeparate)
		{
			// Create the OpenGL program pipeline
			glGenProgramPipelines(1, &mOpenGLProgramPipeline);

			// If the "GL_ARB_direct_state_access" nor "GL_EXT_direct_state_access" extension is available, we need to change OpenGL states during resource creation (nasty thing)
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
#endif
			if ( nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT )
			{
#if SE_OPENGL_STATE_CLEANUP
				glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);
#endif
				glBindProgramPipeline(mOpenGLProgramPipeline);
			}

			// Add references to the provided shaders
#define USE_PROGRAM_STAGES(ShaderBit, ShaderSeparate) if (nullptr != ShaderSeparate) { ShaderSeparate->AddReference(); glUseProgramStages(mOpenGLProgramPipeline, ShaderBit, ShaderSeparate->getOpenGLShaderProgram()); }
			USE_PROGRAM_STAGES(GL_TASK_SHADER_BIT_NV, mTaskShaderSeparate)
				USE_PROGRAM_STAGES(GL_MESH_SHADER_BIT_NV, mMeshShaderSeparate)
				USE_PROGRAM_STAGES(GL_FRAGMENT_SHADER_BIT, mFragmentShaderSeparate)
#undef USE_PROGRAM_STAGES

				// Validate program pipeline
				glValidateProgramPipeline(mOpenGLProgramPipeline);
			GLint validateStatus = 0;
			glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
			if ( GL_TRUE == validateStatus )
			{
				// We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
				// -> Use modern GLSL:
				//    "layout(location = 0) out vec4 ColorOutput0;"
				//    "layout(location = 1) out vec4 ColorOutput1;"
				// -> Use legacy GLSL if necessary:
				//    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
				//    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

				// The actual locations assigned to uniform variables are not known until the program object is linked successfully
				// -> So we have to build a root signature parameter index -> uniform location mapping here
				const Rhi::RootSignature& rootSignatureData = static_cast<const RootSignature&>(rootSignature).getRootSignature();
				const uint32_t numberOfRootParameters = rootSignatureData.numberOfParameters;
				uint32_t uniformBlockBindingIndex = 0;
				for ( uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex )
				{
					const Rhi::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
					{
						SE_ASSERT(nullptr != reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
							const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
						for ( uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex )
						{
							const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

							// Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
							if ( Rhi::DescriptorRangeType::UBV == descriptorRange.rangeType )
							{
#define BIND_UNIFORM_BLOCK(ShaderSeparate) if (nullptr != ShaderSeparate) ::detail::bindUniformBlock(descriptorRange, ShaderSeparate->getOpenGLShaderProgram(), uniformBlockBindingIndex);
								switch ( descriptorRange.shaderVisibility )
								{
								case Rhi::ShaderVisibility::ALL:
								case Rhi::ShaderVisibility::ALL_GRAPHICS:
									BIND_UNIFORM_BLOCK(mTaskShaderSeparate)
										BIND_UNIFORM_BLOCK(mMeshShaderSeparate)
										break;

								case Rhi::ShaderVisibility::FRAGMENT:
									BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
										break;

								case Rhi::ShaderVisibility::TASK:
									BIND_UNIFORM_BLOCK(mTaskShaderSeparate)
										break;

								case Rhi::ShaderVisibility::MESH:
									BIND_UNIFORM_BLOCK(mMeshShaderSeparate)
										break;

								case Rhi::ShaderVisibility::VERTEX:
								case Rhi::ShaderVisibility::TESSELLATION_CONTROL:
								case Rhi::ShaderVisibility::TESSELLATION_EVALUATION:
								case Rhi::ShaderVisibility::GEOMETRY:
								case Rhi::ShaderVisibility::COMPUTE:
									RHI_LOG(CRITICAL, "Invalid OpenGL shader visibility")
										break;
								}
#undef BIND_UNIFORM_BLOCK
								++uniformBlockBindingIndex;
							}
							else if ( Rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType )
							{
#define BIND_UNIFORM_LOCATION(ShaderSeparate) if (nullptr != ShaderSeparate) ::detail::bindUniformLocation(descriptorRange, mOpenGLProgramPipeline, ShaderSeparate->getOpenGLShaderProgram());
								switch ( descriptorRange.shaderVisibility )
								{
								case Rhi::ShaderVisibility::ALL:
								case Rhi::ShaderVisibility::ALL_GRAPHICS:
									BIND_UNIFORM_LOCATION(mTaskShaderSeparate)
										BIND_UNIFORM_LOCATION(mMeshShaderSeparate)
										break;

								case Rhi::ShaderVisibility::FRAGMENT:
									BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
										break;

								case Rhi::ShaderVisibility::TASK:
									BIND_UNIFORM_LOCATION(mTaskShaderSeparate)
										break;

								case Rhi::ShaderVisibility::MESH:
									BIND_UNIFORM_LOCATION(mMeshShaderSeparate)
										break;

								case Rhi::ShaderVisibility::VERTEX:
								case Rhi::ShaderVisibility::TESSELLATION_CONTROL:
								case Rhi::ShaderVisibility::TESSELLATION_EVALUATION:
								case Rhi::ShaderVisibility::GEOMETRY:
								case Rhi::ShaderVisibility::COMPUTE:
									RHI_LOG(CRITICAL, "Invalid OpenGL shader visibility")
										break;
								}
#undef BIND_UNIFORM_LOCATION
							}
						}
					}
				}
			}
			else
			{
				// Error, program pipeline validation failed!

				// Get the length of the information (including a null termination)
				GLint informationLength = 0;
				glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_INFO_LOG_LENGTH, &informationLength);
				if ( informationLength > 1 )
				{
					// Allocate memory for the information
					char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

					// Get the information
					glGetProgramPipelineInfoLog(mOpenGLProgramPipeline, informationLength, nullptr, informationLog);

					// Output the debug string
					RHI_LOG(CRITICAL, informationLog)

						// Cleanup information memory
						RHI_FREE(informationLog);
				}
			}

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous used OpenGL program pipeline
			if ( nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT )
			{
				glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
			}
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( 0 != mOpenGLProgramPipeline && openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
					glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GraphicsProgramSeparate() override
		{
			// Destroy the OpenGL program pipeline
			glDeleteProgramPipelines(1, &mOpenGLProgramPipeline);

			// Release the shader references
			if ( nullptr != mVertexShaderSeparate )
			{
				mVertexShaderSeparate->ReleaseReference();
			}
			if ( nullptr != mTessellationControlShaderSeparate )
			{
				mTessellationControlShaderSeparate->ReleaseReference();
			}
			if ( nullptr != mTessellationEvaluationShaderSeparate )
			{
				mTessellationEvaluationShaderSeparate->ReleaseReference();
			}
			if ( nullptr != mGeometryShaderSeparate )
			{
				mGeometryShaderSeparate->ReleaseReference();
			}
			if ( nullptr != mFragmentShaderSeparate )
			{
				mFragmentShaderSeparate->ReleaseReference();
			}
			if ( nullptr != mTaskShaderSeparate )
			{
				mTaskShaderSeparate->ReleaseReference();
			}
			if ( nullptr != mMeshShaderSeparate )
			{
				mMeshShaderSeparate->ReleaseReference();
			}
		}

		/**
		*  @brief
		*    Return the OpenGL program pipeline
		*
		*  @return
		*    The OpenGL program pipeline, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLProgramPipeline() const
		{
			return mOpenGLProgramPipeline;
		}

		/**
		*  @brief
		*    Return the vertex shader the program is using
		*
		*  @return
		*    Vertex shader the program is using, can be a null pointer, don't destroy the instance
		*/
		[[nodiscard]] inline VertexShaderSeparate* getVertexShaderSeparate() const
		{
			return mVertexShaderSeparate;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IGraphicsProgram methods          ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual handle getUniformHandle(const char* uniformName) override
		{
			GLint uniformLocation = -1;
#define GET_UNIFORM_LOCATION(ShaderSeparate) if (uniformLocation < 0 && nullptr != ShaderSeparate) uniformLocation = glGetUniformLocation(ShaderSeparate->getOpenGLShaderProgram(), uniformName);
			GET_UNIFORM_LOCATION(mVertexShaderSeparate)
				GET_UNIFORM_LOCATION(mTessellationControlShaderSeparate)
				GET_UNIFORM_LOCATION(mTessellationEvaluationShaderSeparate)
				GET_UNIFORM_LOCATION(mGeometryShaderSeparate)
				GET_UNIFORM_LOCATION(mFragmentShaderSeparate)
				GET_UNIFORM_LOCATION(mTaskShaderSeparate)
				GET_UNIFORM_LOCATION(mMeshShaderSeparate)
#undef GET_UNIFORM_LOCATION
				return static_cast<handle>(uniformLocation);
		}

		virtual void setUniform1i(handle uniformHandle, int value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
			glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);

			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform1i(static_cast<GLint>(uniformHandle), value);

			// Be polite and restore the previous used OpenGL program pipeline
			glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
#else
			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform1i(static_cast<GLint>(uniformHandle), value);
#endif
		}

		virtual void setUniform1f(handle uniformHandle, float value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
			glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);

			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform1f(static_cast<GLint>(uniformHandle), value);

			// Be polite and restore the previous used OpenGL program pipeline
			glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
#else
			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform1f(static_cast<GLint>(uniformHandle), value);
#endif
		}

		virtual void setUniform2fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
			glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);

			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform2fv(static_cast<GLint>(uniformHandle), 1, value);

			// Be polite and restore the previous used OpenGL program pipeline
			glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
#else
			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform2fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
		}

		virtual void setUniform3fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
			glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);

			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform3fv(static_cast<GLint>(uniformHandle), 1, value);

			// Be polite and restore the previous used OpenGL program pipeline
			glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
#else
			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform3fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
		}

		virtual void setUniform4fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
			glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);

			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform4fv(static_cast<GLint>(uniformHandle), 1, value);

			// Be polite and restore the previous used OpenGL program pipeline
			glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
#else
			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniform4fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
		}

		virtual void setUniformMatrix3fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
			glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);

			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniformMatrix3fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);

			// Be polite and restore the previous used OpenGL program pipeline
			glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
#else
			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniformMatrix3fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
#endif
		}

		virtual void setUniformMatrix4fv(handle uniformHandle, const float* value) override
		{
#if SE_OPENGL_STATE_CLEANUP
			// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
			glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);

			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniformMatrix4fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);

			// Be polite and restore the previous used OpenGL program pipeline
			glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
#else
			// Set uniform, please note that for this our program must be the currently used one
			glBindProgramPipeline(mOpenGLProgramPipeline);
			glActiveShaderProgram(mOpenGLProgramPipeline, mVertexShaderSeparate->getOpenGLShaderProgram());
			glUniformMatrix4fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
#endif
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(GraphicsProgramSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint mOpenGLProgramPipeline;	// OpenGL program pipeline ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
		// Traditional graphics program
		VertexShaderSeparate*				  mVertexShaderSeparate;					// Vertex shader the program is using (we keep a reference to it), can be a null pointer
		TessellationControlShaderSeparate*	  mTessellationControlShaderSeparate;		// Tessellation control shader the program is using (we keep a reference to it), can be a null pointer
		TessellationEvaluationShaderSeparate* mTessellationEvaluationShaderSeparate;	// Tessellation evaluation shader the program is using (we keep a reference to it), can be a null pointer
		GeometryShaderSeparate*				  mGeometryShaderSeparate;					// Geometry shader the program is using (we keep a reference to it), can be a null pointer
		// Both graphics programs
		FragmentShaderSeparate* mFragmentShaderSeparate;	// Fragment shader the program is using (we keep a reference to it), can be a null pointer
		// Task and mesh shader based graphics program
		TaskShaderSeparate* mTaskShaderSeparate;	// Task shader the program is using (we keep a reference to it), can be a null pointer
		MeshShaderSeparate* mMeshShaderSeparate;	// Mesh shader the program is using (we keep a reference to it), can be a null pointer


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit GraphicsProgramSeparate(const GraphicsProgramSeparate&) = delete;
		GraphicsProgramSeparate& operator =(const GraphicsProgramSeparate&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/GraphicsProgramSeparateDsa.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate graphics program class, effective direct state access (DSA)
	*/
	class GraphicsProgramSeparateDsa final : public GraphicsProgramSeparate
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for traditional graphics program
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] vertexShaderSeparate
		*    Vertex shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationControlShaderSeparate
		*    Tessellation control shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationEvaluationShaderSeparate
		*    Tessellation evaluation shader the graphics program is using, can be a null pointer
		*  @param[in] geometryShaderSeparate
		*    Geometry shader the graphics program is using, can be a null pointer
		*  @param[in] fragmentShaderSeparate
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		inline GraphicsProgramSeparateDsa(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, VertexShaderSeparate* vertexShaderSeparate, TessellationControlShaderSeparate* tessellationControlShaderSeparate, TessellationEvaluationShaderSeparate* tessellationEvaluationShaderSeparate, GeometryShaderSeparate* geometryShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			GraphicsProgramSeparate(openGLRhi, rootSignature, vertexShaderSeparate, tessellationControlShaderSeparate, tessellationEvaluationShaderSeparate, geometryShaderSeparate, fragmentShaderSeparate RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		/**
		*  @brief
		*    Constructor for task and mesh shader based graphics program
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] taskShaderSeparate
		*    Task shader the graphics program is using, can be a null pointer
		*  @param[in] meshShaderSeparate
		*    Mesh shader the graphics program is using
		*  @param[in] fragmentShaderSeparate
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		inline GraphicsProgramSeparateDsa(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, TaskShaderSeparate* taskShaderSeparate, MeshShaderSeparate& meshShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			GraphicsProgramSeparate(openGLRhi, rootSignature, taskShaderSeparate, meshShaderSeparate, fragmentShaderSeparate RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~GraphicsProgramSeparateDsa() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IGraphicsProgram methods          ]
		//[-------------------------------------------------------]
	public:
		virtual void setUniform1f(handle uniformHandle, float value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniform1f(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), value);
			}
			else
			{
				glProgramUniform1fEXT(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), value);
			}
		}

		virtual void setUniform2fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniform2fv(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				glProgramUniform2fvEXT(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, value);
			}
		}

		virtual void setUniform3fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniform3fv(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				glProgramUniform3fvEXT(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, value);
			}
		}

		virtual void setUniform4fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniform4fv(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, value);
			}
			else
			{
				glProgramUniform4fvEXT(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, value);
			}
		}

		virtual void setUniformMatrix3fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniformMatrix3fv(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
			else
			{
				glProgramUniformMatrix3fvEXT(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
		}

		virtual void setUniformMatrix4fv(handle uniformHandle, const float* value) override
		{
			if ( static_cast<OpenGLRhi&>(getRhi()).getExtensions().isGL_ARB_direct_state_access() )
			{
				glProgramUniformMatrix4fv(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
			else
			{
				glProgramUniformMatrix4fvEXT(mVertexShaderSeparate->getOpenGLShaderProgram(), static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GraphicsProgramSeparateDsa(const GraphicsProgramSeparateDsa&) = delete;
		GraphicsProgramSeparateDsa& operator =(const GraphicsProgramSeparateDsa&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/ComputePipelineStateSeparate.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate compute pipeline state class
	*/
	class ComputePipelineStateSeparate : public ComputePipelineState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] computeShaderSeparate
		*    Compute shader the compute pipeline state is using
		*  @param[in] id
		*    The unique compact compute pipeline state ID
		*
		*  @note
		*    - The compute pipeline state keeps a reference to the provided compute shader and releases it when no longer required
		*/
		ComputePipelineStateSeparate(OpenGLRhi& openGLRhi, const Rhi::IRootSignature& rootSignature, ComputeShaderSeparate& computeShaderSeparate, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ComputePipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLProgramPipeline(0),
			mComputeShaderSeparate(computeShaderSeparate)
		{
			// Create the OpenGL program pipeline
			glGenProgramPipelines(1, &mOpenGLProgramPipeline);

			// If the "GL_ARB_direct_state_access" nor "GL_EXT_direct_state_access" extension is available, we need to change OpenGL states during resource creation (nasty thing)
#if SE_OPENGL_STATE_CLEANUP
	// Backup the currently used OpenGL program pipeline
			GLint openGLProgramPipelineBackup = 0;
#endif
			if ( nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT )
			{
#if SE_OPENGL_STATE_CLEANUP
				glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);
#endif
				glBindProgramPipeline(mOpenGLProgramPipeline);
			}

			// Add reference to the provided compute shader
			computeShaderSeparate.AddReference();
			glUseProgramStages(mOpenGLProgramPipeline, GL_COMPUTE_SHADER_BIT, computeShaderSeparate.getOpenGLShaderProgram());

			// Validate program pipeline
			glValidateProgramPipeline(mOpenGLProgramPipeline);
			GLint validateStatus = 0;
			glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
			if ( true )	// TODO(co) Compute shader: Validate status always returns failure without log when using a compute shader? AMD 290X Radeon software version 18.7.1.
			//if (GL_TRUE == validateStatus)
			{
				// We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
				// -> Use modern GLSL:
				//    "layout(location = 0) out vec4 ColorOutput0;"
				//    "layout(location = 1) out vec4 ColorOutput1;"
				// -> Use legacy GLSL if necessary:
				//    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
				//    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

				// The actual locations assigned to uniform variables are not known until the program object is linked successfully
				// -> So we have to build a root signature parameter index -> uniform location mapping here
				const Rhi::RootSignature& rootSignatureData = static_cast<const RootSignature&>(rootSignature).getRootSignature();
				const uint32_t numberOfRootParameters = rootSignatureData.numberOfParameters;
				uint32_t uniformBlockBindingIndex = 0;
				for ( uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex )
				{
					const Rhi::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
					{
						SE_ASSERT(nullptr != reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
							const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
						for ( uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex )
						{
							const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

							// Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
							if ( Rhi::DescriptorRangeType::UBV == descriptorRange.rangeType )
							{
								switch ( descriptorRange.shaderVisibility )
								{
								case Rhi::ShaderVisibility::ALL_GRAPHICS:
								case Rhi::ShaderVisibility::VERTEX:
								case Rhi::ShaderVisibility::TESSELLATION_CONTROL:
								case Rhi::ShaderVisibility::TESSELLATION_EVALUATION:
								case Rhi::ShaderVisibility::GEOMETRY:
								case Rhi::ShaderVisibility::FRAGMENT:
								case Rhi::ShaderVisibility::TASK:
								case Rhi::ShaderVisibility::MESH:
									RHI_LOG(CRITICAL, "Invalid OpenGL shader visibility")
										break;

								case Rhi::ShaderVisibility::ALL:
								case Rhi::ShaderVisibility::COMPUTE:
									::detail::bindUniformBlock(descriptorRange, mComputeShaderSeparate.getOpenGLShaderProgram(), uniformBlockBindingIndex);
									break;
								}
								++uniformBlockBindingIndex;
							}
							else if ( Rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType )
							{
								switch ( descriptorRange.shaderVisibility )
								{
								case Rhi::ShaderVisibility::ALL_GRAPHICS:
								case Rhi::ShaderVisibility::VERTEX:
								case Rhi::ShaderVisibility::TESSELLATION_CONTROL:
								case Rhi::ShaderVisibility::TESSELLATION_EVALUATION:
								case Rhi::ShaderVisibility::GEOMETRY:
								case Rhi::ShaderVisibility::FRAGMENT:
								case Rhi::ShaderVisibility::TASK:
								case Rhi::ShaderVisibility::MESH:
									RHI_LOG(CRITICAL, "Invalid OpenGL shader visibility")
										break;

								case Rhi::ShaderVisibility::ALL:
								case Rhi::ShaderVisibility::COMPUTE:
									::detail::bindUniformLocation(descriptorRange, mOpenGLProgramPipeline, mComputeShaderSeparate.getOpenGLShaderProgram());
									break;
								}
							}
						}
					}
				}
			}
			else
			{
				// Error, program pipeline validation failed!

				// Get the length of the information (including a null termination)
				GLint informationLength = 0;
				glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_INFO_LOG_LENGTH, &informationLength);
				if ( informationLength > 1 )
				{
					// Allocate memory for the information
					char* informationLog = RHI_MALLOC_TYPED(char, informationLength);

					// Get the information
					glGetProgramPipelineInfoLog(mOpenGLProgramPipeline, informationLength, nullptr, informationLog);

					// Output the debug string
					RHI_LOG(CRITICAL, informationLog)

						// Cleanup information memory
						RHI_FREE(informationLog);
				}
			}

#if SE_OPENGL_STATE_CLEANUP
			// Be polite and restore the previous used OpenGL program pipeline
			if ( nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT )
			{
				glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
			}
#endif

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			if ( openGLRhi.getExtensions().isGL_KHR_debug() )
			{
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Compute PSO", 14)	// 14 = "Compute PSO: " including terminating zero
					glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComputePipelineStateSeparate() override
		{
			// Destroy the OpenGL program pipeline
			glDeleteProgramPipelines(1, &mOpenGLProgramPipeline);

			// Release the compute shader reference
			mComputeShaderSeparate.ReleaseReference();
		}

		/**
		*  @brief
		*    Return the OpenGL program pipeline
		*
		*  @return
		*    The OpenGL program pipeline, can be zero if no resource is allocated, do not destroy the returned resource
		*/
		[[nodiscard]] inline GLuint getOpenGLProgramPipeline() const
		{
			return mOpenGLProgramPipeline;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ComputePipelineStateSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
	protected:
		GLuint				   mOpenGLProgramPipeline;	// OpenGL program pipeline ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
		ComputeShaderSeparate& mComputeShaderSeparate;	// Compute shader the compute pipeline state is using (we keep a reference to it)


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit ComputePipelineStateSeparate(const ComputePipelineStateSeparate&) = delete;
		ComputePipelineStateSeparate& operator =(const ComputePipelineStateSeparate&) = delete;


	};




	//[-------------------------------------------------------]
	//[ OpenGLRhi/Shader/Separate/ShaderLanguageSeparate.h    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Separate shader language class
	*/
	class ShaderLanguageSeparate final : public Rhi::IShaderLanguage
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*/
		inline explicit ShaderLanguageSeparate(OpenGLRhi& openGLRhi) :
			IShaderLanguage(openGLRhi)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ShaderLanguageSeparate() override
		{
			// De-initialize glslang, if necessary
#ifdef SE_GLSLTOSPIRV
			if ( ::detail::GlslangInitialized )
			{
				// TODO(co) Fix glslang related memory leaks. See also
				//		    - "Fix a few memory leaks #916" - https://github.com/KhronosGroup/glslang/pull/916
				//		    - "FreeGlobalPools is never called in glslang::FinalizeProcess()'s path. #928" - https://github.com/KhronosGroup/glslang/issues/928
				glslang::FinalizeProcess();
				::detail::GlslangInitialized = false;
			}
#endif
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShaderLanguage methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::GLSL_NAME;
		}

		[[nodiscard]] virtual Rhi::IVertexShader* createVertexShaderFromBytecode(const Rhi::VertexAttributes& vertexAttributes, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL vertex shader bytecode is invalid")

				// Check whether or not there's vertex shader support
				const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_vertex_shader() && extensions.isGL_ARB_gl_spirv() )
			{
				return RHI_NEW(VertexShaderSeparate)(openGLRhi, vertexAttributes, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no vertex shader support or no decent shader bytecode support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IVertexShader* createVertexShaderFromSourceCode(const Rhi::VertexAttributes& vertexAttributes, const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's vertex shader support
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_vertex_shader() )
			{
				return RHI_NEW(VertexShaderSeparate)(openGLRhi, vertexAttributes, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no vertex shader support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::ITessellationControlShader* createTessellationControlShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL tessellation control shader bytecode is invalid")

				// Check whether or not there's tessellation support
				const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_tessellation_shader() && extensions.isGL_ARB_gl_spirv() )
			{
				return RHI_NEW(TessellationControlShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no tessellation support or no decent shader bytecode support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::ITessellationControlShader* createTessellationControlShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's tessellation support
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_tessellation_shader() )
			{
				return RHI_NEW(TessellationControlShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no tessellation support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::ITessellationEvaluationShader* createTessellationEvaluationShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL tessellation evaluation shader bytecode is invalid")

				// Check whether or not there's tessellation support
				const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_tessellation_shader() && extensions.isGL_ARB_gl_spirv() )
			{
				return RHI_NEW(TessellationEvaluationShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no tessellation support or no decent shader bytecode support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::ITessellationEvaluationShader* createTessellationEvaluationShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's tessellation support
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_tessellation_shader() )
			{
				return RHI_NEW(TessellationEvaluationShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no tessellation support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IGeometryShader* createGeometryShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode, Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL geometry shader bytecode is invalid")

				// Check whether or not there's geometry shader support
				const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_geometry_shader4() && extensions.isGL_ARB_gl_spirv() )
			{
				// In modern GLSL, "geometry shader input primitive topology" & "geometry shader output primitive topology" & "number of output vertices" can be directly set within GLSL by writing e.g.
				//   "layout(triangles) in;"
				//   "layout(triangle_strip, max_vertices = 3) out;"
				// -> To be able to support older GLSL versions, we have to provide this information also via OpenGL API functions
				return RHI_NEW(GeometryShaderSeparate)(openGLRhi, shaderBytecode, gsInputPrimitiveTopology, gsOutputPrimitiveTopology, numberOfOutputVertices RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no geometry shader support or no decent shader bytecode support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IGeometryShader* createGeometryShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's geometry shader support
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_geometry_shader4() )
			{
				// In modern GLSL, "geometry shader input primitive topology" & "geometry shader output primitive topology" & "number of output vertices" can be directly set within GLSL by writing e.g.
				//   "layout(triangles) in;"
				//   "layout(triangle_strip, max_vertices = 3) out;"
				// -> To be able to support older GLSL versions, we have to provide this information also via OpenGL API functions
				return RHI_NEW(GeometryShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, gsInputPrimitiveTopology, gsOutputPrimitiveTopology, numberOfOutputVertices, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no geometry shader support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IFragmentShader* createFragmentShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL fragment shader bytecode is invalid")

				// Check whether or not there's fragment shader support
				const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_fragment_shader() && extensions.isGL_ARB_gl_spirv() )
			{
				return RHI_NEW(FragmentShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no fragment shader support or no decent shader bytecode support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IFragmentShader* createFragmentShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's fragment shader support
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_fragment_shader() )
			{
				return RHI_NEW(FragmentShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no fragment shader support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::ITaskShader* createTaskShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL task shader bytecode is invalid")

				// Check whether or not there's task shader support
				const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_NV_mesh_shader() && extensions.isGL_ARB_gl_spirv() )
			{
				return RHI_NEW(TaskShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no task shader support or no decent shader bytecode support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::ITaskShader* createTaskShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's task shader support
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_NV_mesh_shader() )
			{
				return RHI_NEW(TaskShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no task shader support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IMeshShader* createMeshShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL mesh shader bytecode is invalid")

				// Check whether or not there's mesh shader support
				const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_NV_mesh_shader() && extensions.isGL_ARB_gl_spirv() )
			{
				return RHI_NEW(MeshShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no mesh shader support or no decent shader bytecode support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IMeshShader* createMeshShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's mesh shader support
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_NV_mesh_shader() )
			{
				return RHI_NEW(MeshShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no mesh shader support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IComputeShader* createComputeShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL compute shader bytecode is invalid")

				// Check whether or not there's compute shader support
				const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_compute_shader() && extensions.isGL_ARB_gl_spirv() )
			{
				return RHI_NEW(ComputeShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no compute shader support or no decent shader bytecode support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IComputeShader* createComputeShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// Check whether or not there's compute shader support
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());
			const Extensions& extensions = openGLRhi.getExtensions();
			if ( extensions.isGL_ARB_compute_shader() )
			{
				return RHI_NEW(ComputeShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Error! There's no compute shader support!
				return nullptr;
			}
		}

		[[nodiscard]] virtual Rhi::IGraphicsProgram* createGraphicsProgram(const Rhi::IRootSignature& rootSignature, [[maybe_unused]] const Rhi::VertexAttributes& vertexAttributes, Rhi::IVertexShader* vertexShader, Rhi::ITessellationControlShader* tessellationControlShader, Rhi::ITessellationEvaluationShader* tessellationEvaluationShader, Rhi::IGeometryShader* geometryShader, Rhi::IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// A shader can be a null pointer, but if it's not the shader and graphics program language must match
			// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
			//    the name is safe because we know that we always reference to one and the same name address
			// TODO(co) Add security check: Is the given resource one of the currently used RHI?
			if ( nullptr != vertexShader && vertexShader->getShaderLanguageName() != ::detail::GLSL_NAME )
			{
				// Error! Vertex shader language mismatch!
			}
			else if ( nullptr != tessellationControlShader && tessellationControlShader->getShaderLanguageName() != ::detail::GLSL_NAME )
			{
				// Error! Tessellation control shader language mismatch!
			}
			else if ( nullptr != tessellationEvaluationShader && tessellationEvaluationShader->getShaderLanguageName() != ::detail::GLSL_NAME )
			{
				// Error! Tessellation evaluation shader language mismatch!
			}
			else if ( nullptr != geometryShader && geometryShader->getShaderLanguageName() != ::detail::GLSL_NAME )
			{
				// Error! Geometry shader language mismatch!
			}
			else if ( nullptr != fragmentShader && fragmentShader->getShaderLanguageName() != ::detail::GLSL_NAME )
			{
				// Error! Fragment shader language mismatch!
			}

			// Is "GL_EXT_direct_state_access" there?
			else if ( openGLRhi.getExtensions().isGL_EXT_direct_state_access() || openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
			{
				// Effective direct state access (DSA)
				return RHI_NEW(GraphicsProgramSeparateDsa)(openGLRhi, rootSignature, static_cast<VertexShaderSeparate*>(vertexShader), static_cast<TessellationControlShaderSeparate*>(tessellationControlShader), static_cast<TessellationEvaluationShaderSeparate*>(tessellationEvaluationShader), static_cast<GeometryShaderSeparate*>(geometryShader), static_cast<FragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Traditional bind version
				return RHI_NEW(GraphicsProgramSeparate)(openGLRhi, rootSignature, static_cast<VertexShaderSeparate*>(vertexShader), static_cast<TessellationControlShaderSeparate*>(tessellationControlShader), static_cast<TessellationEvaluationShaderSeparate*>(tessellationEvaluationShader), static_cast<GeometryShaderSeparate*>(geometryShader), static_cast<FragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}

			// Error! Shader language mismatch!
			// -> Ensure a correct reference counter behaviour, even in the situation of an error
			if ( nullptr != vertexShader )
			{
				vertexShader->AddReference();
				vertexShader->ReleaseReference();
			}
			if ( nullptr != tessellationControlShader )
			{
				tessellationControlShader->AddReference();
				tessellationControlShader->ReleaseReference();
			}
			if ( nullptr != tessellationEvaluationShader )
			{
				tessellationEvaluationShader->AddReference();
				tessellationEvaluationShader->ReleaseReference();
			}
			if ( nullptr != geometryShader )
			{
				geometryShader->AddReference();
				geometryShader->ReleaseReference();
			}
			if ( nullptr != fragmentShader )
			{
				fragmentShader->AddReference();
				fragmentShader->ReleaseReference();
			}

			// Error!
			return nullptr;
		}

		[[nodiscard]] virtual Rhi::IGraphicsProgram* createGraphicsProgram(const Rhi::IRootSignature& rootSignature, Rhi::ITaskShader* taskShader, Rhi::IMeshShader& meshShader, Rhi::IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(getRhi());

			// A shader can be a null pointer, but if it's not the shader and graphics program language must match
			// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
			//    the name is safe because we know that we always reference to one and the same name address
			// TODO(co) Add security check: Is the given resource one of the currently used RHI?
			if ( nullptr != taskShader && taskShader->getShaderLanguageName() != ::detail::GLSL_NAME )
			{
				// Error! Vertex shader language mismatch!
			}
			else if ( meshShader.getShaderLanguageName() != ::detail::GLSL_NAME )
			{
				// Error! Fragment shader language mismatch!
			}
			else if ( nullptr != fragmentShader && fragmentShader->getShaderLanguageName() != ::detail::GLSL_NAME )
			{
				// Error! Vertex shader language mismatch!
			}

			// Is "GL_EXT_direct_state_access" there?
			else if ( openGLRhi.getExtensions().isGL_EXT_direct_state_access() || openGLRhi.getExtensions().isGL_ARB_direct_state_access() )
			{
				// Effective direct state access (DSA)
				return RHI_NEW(GraphicsProgramSeparateDsa)(openGLRhi, rootSignature, static_cast<TaskShaderSeparate*>(taskShader), static_cast<MeshShaderSeparate&>(meshShader), static_cast<FragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}
			else
			{
				// Traditional bind version
				return RHI_NEW(GraphicsProgramSeparate)(openGLRhi, rootSignature, static_cast<TaskShaderSeparate*>(taskShader), static_cast<MeshShaderSeparate&>(meshShader), static_cast<FragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}

			// Error! Shader language mismatch!
			// -> Ensure a correct reference counter behaviour, even in the situation of an error
			if ( nullptr != taskShader )
			{
				taskShader->AddReference();
				taskShader->ReleaseReference();
			}
			meshShader.AddReference();
			meshShader.ReleaseReference();
			if ( nullptr != fragmentShader )
			{
				fragmentShader->AddReference();
				fragmentShader->ReleaseReference();
			}

			// Error!
			return nullptr;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ShaderLanguageSeparate, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ShaderLanguageSeparate(const ShaderLanguageSeparate&) = delete;
		ShaderLanguageSeparate& operator =(const ShaderLanguageSeparate&) = delete;
	};

	//[-------------------------------------------------------]
//[ OpenGLRhi/State/GraphicsPipelineState.h               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL graphics pipeline state class
*/
	class GraphicsPipelineState final : public Rhi::IGraphicsPipelineState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] graphicsPipelineState
		*    Graphics pipeline state to use
		*  @param[in] id
		*    The unique compact graphics pipeline state ID
		*/
		GraphicsPipelineState(OpenGLRhi& openGLRhi, const Rhi::GraphicsPipelineState& graphicsPipelineState, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IGraphicsPipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mOpenGLPrimitiveTopology(0xFFFF),	// Unknown default setting
			mNumberOfVerticesPerPatch(0),
			mGraphicsProgram(graphicsPipelineState.graphicsProgram),
			mRenderPass(graphicsPipelineState.renderPass),
			mRasterizerState(graphicsPipelineState.rasterizerState),
			mDepthStencilState(graphicsPipelineState.depthStencilState),
			mBlendState(graphicsPipelineState.blendState)
		{
			// Tessellation support: Up to 32 vertices per patch are supported "Rhi::PrimitiveTopology::PATCH_LIST_1" ... "Rhi::PrimitiveTopology::PATCH_LIST_32"
			if ( graphicsPipelineState.primitiveTopology >= Rhi::PrimitiveTopology::PATCH_LIST_1 )
			{
				// Use tessellation

				// Get number of vertices that will be used to make up a single patch primitive
				// -> There's no need to check for the "GL_ARB_tessellation_shader" extension, it's there if "Rhi::Capabilities::maximumNumberOfPatchVertices" is not 0
				const int numberOfVerticesPerPatch = static_cast<int>(graphicsPipelineState.primitiveTopology) - static_cast<int>(Rhi::PrimitiveTopology::PATCH_LIST_1) + 1;
				if ( numberOfVerticesPerPatch <= static_cast<int>(openGLRhi.getCapabilities().maximumNumberOfPatchVertices) )
				{
					// Set number of vertices that will be used to make up a single patch primitive
					mNumberOfVerticesPerPatch = numberOfVerticesPerPatch;

					// Set OpenGL primitive topology
					mOpenGLPrimitiveTopology = GL_PATCHES;
				}
				else
				{
					// Error!
					SE_ASSERT(false, "Invalid number of OpenGL vertices per patch")
				}
			}
			else
			{
				// Do not use tessellation

				// Map and backup the set OpenGL primitive topology
				mOpenGLPrimitiveTopology = Mapping::getOpenGLType(graphicsPipelineState.primitiveTopology);
			}

			// Ensure a correct reference counter behaviour
			graphicsPipelineState.rootSignature->AddReference();
			graphicsPipelineState.rootSignature->ReleaseReference();

			// Add a reference to the referenced RHI resources
			mGraphicsProgram->AddReference();
			mRenderPass->AddReference();
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GraphicsPipelineState() override
		{
			// Release referenced RHI resources
			mGraphicsProgram->ReleaseReference();
			mRenderPass->ReleaseReference();

			// Free the unique compact graphics pipeline state ID
			static_cast<OpenGLRhi&>(getRhi()).GraphicsPipelineStateMakeId.DestroyID(getId());
		}

		/**
		*  @brief
		*    Return the graphics program
		*
		*  @return
		*    Graphics program, always valid
		*/
		[[nodiscard]] inline Rhi::IGraphicsProgram* getGraphicsProgram() const
		{
			return mGraphicsProgram;
		}

		/**
		*  @brief
		*    Return the OpenGL primitive topology describing the type of primitive to render
		*
		*  @return
		*    OpenGL primitive topology describing the type of primitive to render
		*/
		[[nodiscard]] inline GLenum getOpenGLPrimitiveTopology() const
		{
			return mOpenGLPrimitiveTopology;
		}

		/**
		*  @brief
		*    Return the number of vertices per patch
		*
		*  @return
		*    Return the number of vertices per patch
		*/
		[[nodiscard]] inline GLint getNumberOfVerticesPerPatch() const
		{
			return mNumberOfVerticesPerPatch;
		}

		/**
		*  @brief
		*    Bind the graphics pipeline state
		*/
		void bindGraphicsPipelineState() const
		{
			static_cast<OpenGLRhi&>(getRhi()).setOpenGLGraphicsProgram(mGraphicsProgram);

			// Set the OpenGL rasterizer state
			mRasterizerState.setOpenGLRasterizerStates();

			// Set OpenGL depth stencil state
			mDepthStencilState.setOpenGLDepthStencilStates();

			// Set OpenGL blend state
			mBlendState.setOpenGLBlendStates();
		}

		//[-------------------------------------------------------]
		//[ Detail state access                                   ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline const Rhi::RasterizerState& getRasterizerState() const
		{
			return mRasterizerState.getRasterizerState();
		}

		[[nodiscard]] inline const Rhi::DepthStencilState& getDepthStencilState() const
		{
			return mDepthStencilState.getDepthStencilState();
		}

		[[nodiscard]] inline const Rhi::BlendState& getBlendState() const
		{
			return mBlendState.getBlendState();
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(GraphicsPipelineState, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GraphicsPipelineState(const GraphicsPipelineState&) = delete;
		GraphicsPipelineState& operator =(const GraphicsPipelineState&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		GLenum				   mOpenGLPrimitiveTopology;	// OpenGL primitive topology describing the type of primitive to render
		GLint				   mNumberOfVerticesPerPatch;	// Number of vertices per patch
		Rhi::IGraphicsProgram* mGraphicsProgram;
		Rhi::IRenderPass*	   mRenderPass;
		RasterizerState		   mRasterizerState;
		DepthStencilState	   mDepthStencilState;
		BlendState			   mBlendState;
	};
} // namespace OpenGLRhi
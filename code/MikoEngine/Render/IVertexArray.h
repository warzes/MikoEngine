#pragma once

namespace Rhi
{
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
} // namespace Rhi
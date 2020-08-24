#pragma once

namespace Rhi
{
	enum class VertexAttributeFormat : uint8_t
	{
		FLOAT_1 = 0,	// Float 1 (one component per element, 32 bit floating point per component), supported by DirectX 11, OpenGL and OpenGL ES 3
		FLOAT_2 = 1,	// Float 2 (two components per element, 32 bit floating point per component), supported by DirectX 11, OpenGL and OpenGL ES 3
		FLOAT_3 = 2,	// Float 3 (three components per element, 32 bit floating point per component), supported by DirectX 11, OpenGL and OpenGL ES 3
		FLOAT_4 = 3,	// Float 4 (four components per element, 32 bit floating point per component), supported by DirectX 11, OpenGL and OpenGL ES 3
		R8G8B8A8_UNORM = 4,	// Unsigned byte 4 (four components per element, 8 bit integer per component), will be passed in a normalized form into shaders, supported by DirectX 11, OpenGL and OpenGL ES 3
		R8G8B8A8_UINT = 5,	// Unsigned byte 4 (four components per element, 8 bit integer per component), supported by DirectX 11, OpenGL and OpenGL ES 3
		SHORT_2 = 6,	// Short 2 (two components per element, 16 bit integer per component), supported by DirectX 11, OpenGL and OpenGL ES 3
		SHORT_4 = 7,	// Short 4 (four components per element, 16 bit integer per component), supported by DirectX 11, OpenGL and OpenGL ES 3
		UINT_1 = 8		// Unsigned integer 1 (one components per element, 32 bit unsigned integer per component), supported by DirectX 11, OpenGL and OpenGL ES 3
	};

#pragma pack(push)
#pragma pack(1)
	struct VertexAttribute final
	{
		// Data destination
		VertexAttributeFormat vertexAttributeFormat;	// Vertex attribute format
		char				  name[32];					// Vertex attribute name
		char				  semanticName[32];			// Vertex attribute semantic name
		uint32_t			  semanticIndex;			// Vertex attribute semantic index
		// Data source
		uint32_t			  inputSlot;				// Index of the vertex input slot to use (see "Rhi::VertexArrayVertexBuffer")
		uint32_t			  alignedByteOffset;		// Offset (in bytes) from the start of the vertex to this certain attribute
		uint32_t			  strideInBytes;			// Specifies the size in bytes of each vertex entry
		uint32_t			  instancesPerElement;		/**< Number of instances to draw with the same data before advancing in the buffer by one element.
															 0 for no instancing meaning the data is per-vertex instead of per-instance, 1 for drawing one
															 instance with the same data, 2 for drawing two instances with the same data and so on.
															 Instanced arrays is a shader model 3 feature, only supported if "Rhi::Capabilities::instancedArrays" is true.*/
	};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
	struct VertexAttributes final
	{
		uint32_t numberOfAttributes = 0; // Number of attributes (position, color, texture coordinate, normal...), having zero attributes is valid
		const VertexAttribute *attributes = nullptr; // At least "numberOfAttributes" instances of vertex array attributes, can be a null pointer in case there are zero attributes, the data is internally copied and you have to free your memory if you no longer need it

		// Public methods
		inline VertexAttributes() = default;
		inline VertexAttributes(uint32_t _numberOfAttributes, const VertexAttribute* _attributes) :
			numberOfAttributes(_numberOfAttributes),
			attributes(_attributes)
		{
		}
	};
#pragma pack(pop)

	struct VertexArrayVertexBuffer final
	{
		IVertexBuffer* vertexBuffer = nullptr; // Vertex buffer used at this vertex input slot (vertex array instances keep a reference to the vertex buffers used by the vertex array attributes, see "Rhi::IRhi::createVertexArray()" for details)
	};
} // namespace Rhi
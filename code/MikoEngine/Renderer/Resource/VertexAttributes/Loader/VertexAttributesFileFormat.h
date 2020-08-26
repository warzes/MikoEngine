#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	// Vertex attributes file format content:
	// - File format header
	// - Vertex attributes animation header
	namespace v1VertexAttributes
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = SE_STRING_ID("VertexAttributes");
		static constexpr uint32_t FORMAT_VERSION = 1;

		#pragma pack(push)
		#pragma pack(1)
			// TODO(co) Implement vertex attributes file format
			struct VertexAttributesHeader final
			{
				uint8_t  numberOfVertexAttributes;
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1VertexAttributes
} // Renderer

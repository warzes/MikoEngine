#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	// CRN array file format content:
	// - File format header
	// - Number of slices
	namespace v1CrnArray
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = SE_STRING_ID("crn_array");
		static constexpr uint32_t FORMAT_VERSION = 1;

		struct CrnArrayHeader final
		{
			uint32_t numberOfSlices;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1CrnArray
} // Renderer

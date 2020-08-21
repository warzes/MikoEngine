#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]

#include "Renderer/Core/Math/Plane.h"


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
	*    Frustum
	*/
	class Frustum final
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*   Enumeration for clip planes
		*/
		enum PlaneIndex
		{
			PLANE_LEFT = 0,
			PLANE_RIGHT,
			PLANE_TOP,
			PLANE_BOTTOM,
			PLANE_NEAR,
			PLANE_FAR,
			NUMBER_OF_PLANES
		};


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		Plane planes[NUMBER_OF_PLANES];	// Plane normals point into frustum


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		Frustum() = delete;

		/**
		*  @brief
		*    Constructor creating frustum from matrix
		*
		*  @remarks
		*    - If extracted from projection matrix, planes will be in view space
		*    - If extracted from world space to clip space matrix (aka view projection matrix), planes will be in world space
		*    - If extracted from object space to clip space matrix (aka model view projection), planes will be in object space
		*/
		explicit Frustum(const glm::mat4& matrix);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Frustum()
		{
			// Nothing here
		}


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

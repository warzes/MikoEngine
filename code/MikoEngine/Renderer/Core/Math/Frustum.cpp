#include "stdafx.h"
#include "Renderer/Core/Math/Frustum.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	Frustum::Frustum(const glm::mat4& matrix) :
		planes
		{
			// Create non-normalized clipping planes
			// -> Basing on "Fast Extraction of Viewing Frustum Planes from the World-View-Projection Matrix" by Gil Gribb and Klaus Hartmann ( http://gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf ) - "B.2 Plane Extraction for Direct3D"
			glm::vec4(matrix[0][3] + matrix[0][0], matrix[1][3] + matrix[1][0], matrix[2][3] + matrix[2][0], matrix[3][3] + matrix[3][0]),	// Left clipping plane
			glm::vec4(matrix[0][3] - matrix[0][0], matrix[1][3] - matrix[1][0], matrix[2][3] - matrix[2][0], matrix[3][3] - matrix[3][0]),	// Right clipping plane
			glm::vec4(matrix[0][3] - matrix[0][1], matrix[1][3] - matrix[1][1], matrix[2][3] - matrix[2][1], matrix[3][3] - matrix[3][1]),	// Top clipping plane
			glm::vec4(matrix[0][3] + matrix[0][1], matrix[1][3] + matrix[1][1], matrix[2][3] + matrix[2][1], matrix[3][3] + matrix[3][1]),	// Bottom clipping plane
			glm::vec4(				 matrix[0][2],				  matrix[1][2],				   matrix[2][2],				matrix[3][2]),	// Near clipping plane
			glm::vec4(matrix[0][3] - matrix[0][2], matrix[1][3] - matrix[1][2], matrix[2][3] - matrix[2][2], matrix[3][3] - matrix[3][2])	// Far clipping plane
		}
	{
		// Normalize the plane equations
		for (int i = 0; i < NUMBER_OF_PLANES; ++i)
		{
			planes[i].normalize();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
